/**
 * @file Misere_Classes.cpp
 * @brief Implementation of Misère (Inverse) Tic-Tac-Toe.
 */

#include "Misere_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

const char Misere_Board::blank_symbol = ' ';

// -----------------------------------------------------------------------------
// Misere_Board Implementation
// -----------------------------------------------------------------------------

Misere_Board::Misere_Board() : Board<char>(3, 3) {
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            board[i][j] = blank_symbol;
    n_moves = 0;
}

bool Misere_Board::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char sym = move->get_symbol();

    if (r < 0 || r >= rows || c < 0 || c >= columns) {
        cout << "Invalid move: (" << r << "," << c << ") out of bounds.\n";
        return false;
    }
    if (board[r][c] != blank_symbol) {
        cout << "Invalid move: Cell (" << r << "," << c << ") is occupied.\n";
        return false;
    }
    board[r][c] = sym;
    ++n_moves;
    return true;
}

bool Misere_Board::player_has_three(char symbol) const {
    // Check rows
    for (int i = 0; i < 3; ++i)
        if (board[i][0] == symbol && board[i][1] == symbol && board[i][2] == symbol) return true;
    // Check cols
    for (int j = 0; j < 3; ++j)
        if (board[0][j] == symbol && board[1][j] == symbol && board[2][j] == symbol) return true;
    // Check diagonals
    if (board[0][0] == symbol && board[1][1] == symbol && board[2][2] == symbol) return true;
    if (board[0][2] == symbol && board[1][1] == symbol && board[2][0] == symbol) return true;
    return false;
}

bool Misere_Board::is_win(Player<char>* /*player*/) {
    // In Misere, making a line is losing, so you never "win" by placing a piece.
    // The other player loses on their turn.
    return false;
}

bool Misere_Board::is_lose(Player<char>* player) {
    if (!player) return false;
    return player_has_three(player->get_symbol());
}

bool Misere_Board::is_draw(Player<char>* /*player*/) {
    // If board is full and NO ONE has 3-in-a-row, it's a draw.
    if (n_moves < rows * columns) return false;
    if (!player_has_three('X') && !player_has_three('O')) return true;
    return false;
}

bool Misere_Board::game_is_over(Player<char>* player) {
    return is_lose(player) || is_draw(player);
}

// ... Helper functions for AI (cell_empty, get_empty_cells, make_temp_move, undo_temp_move) ...

bool Misere_Board::cell_empty(int r, int c) const {
    return board[r][c] == blank_symbol;
}

vector<pair<int,int>> Misere_Board::get_empty_cells() const {
    vector<pair<int,int>> empties;
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            if (board[r][c] == blank_symbol)
                empties.emplace_back(r,c);
    return empties;
}

bool Misere_Board::make_temp_move(int r, int c, char symbol) {
    if (r < 0 || r >= rows || c < 0 || c >= columns) return false;
    if (board[r][c] != blank_symbol) return false;
    board[r][c] = symbol;
    ++n_moves;
    return true;
}

void Misere_Board::undo_temp_move(int r, int c) {
    if (r < 0 || r >= rows || c < 0 || c >= columns) return;
    if (board[r][c] != blank_symbol) {
        board[r][c] = blank_symbol;
        --n_moves;
    }
}

bool Misere_Board::placement_creates_three(int r, int c, char symbol) const {
    // Temporarily checks if a move at (r,c) completes a line for 'symbol'
    // Check row r
    bool row_three = true;
    for (int j = 0; j < 3; ++j) {
        char val = (j == c) ? symbol : board[r][j];
        if (val != symbol) { row_three = false; break; }
    }
    if (row_three) return true;

    // Check column c
    bool col_three = true;
    for (int i = 0; i < 3; ++i) {
        char val = (i == r) ? symbol : board[i][c];
        if (val != symbol) { col_three = false; break; }
    }
    if (col_three) return true;

    // Diagonals
    if (r == c) {
        bool diag = true;
        for (int i = 0; i < 3; ++i) {
            char val = (i == r) ? symbol : board[i][i];
            if (val != symbol) { diag = false; break; }
        }
        if (diag) return true;
    }
    if (r + c == 2) {
        bool diag = true;
        for (int i = 0; i < 3; ++i) {
            int j = 2 - i;
            char val = (i == r && j == c) ? symbol : board[i][j];
            if (val != symbol) { diag = false; break; }
        }
        if (diag) return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Misere_AIPlayer Implementation
// -----------------------------------------------------------------------------

Misere_AIPlayer::Misere_AIPlayer(const string& name, char symbol)
    : Player<char>(name, symbol, PlayerType::AI) {}

Move<char>* Misere_AIPlayer::get_smart_move(Misere_Board* board, char opponent_symbol) {
    cout << "AI " << get_name() << " is thinking (full backtracking)...\n";

    vector<pair<int,int>> empties = board->get_empty_cells();
    if (empties.empty()) return new Move<char>(0,0,get_symbol());

    int best_score = -2;
    vector<pair<int,int>> best_moves;

    for (auto mv : empties) {
        int r = mv.first, c = mv.second;

        // 1. Immediate Suicidal Check
        // If playing here makes ME create 3-in-a-row, I lose instantly. Score = -1 (Bad).
        board->make_temp_move(r, c, get_symbol());
        if (board->placement_creates_three(r, c, get_symbol())) {
            board->undo_temp_move(r, c);
            if (-1 > best_score) {
                best_score = -1;
                best_moves.clear();
                best_moves.push_back(mv);
            } else if (-1 == best_score) {
                best_moves.push_back(mv);
            }
            continue;
        }

        // 2. Otherwise, see if this leads to a win down the line
        int score = backtrack_minimax(board, opponent_symbol);
        board->undo_temp_move(r, c);

        if (score > best_score) {
            best_score = score;
            best_moves.clear();
            best_moves.push_back(mv);
        } else if (score == best_score) {
            best_moves.push_back(mv);
        }
    }

    pair<int,int> chosen = tiebreak_choose(best_moves);
    cout << "AI " << get_name() << " selected (" << chosen.first << "," << chosen.second << ") with score " << best_score << "\n";
    return new Move<char>(chosen.first, chosen.second, get_symbol());
}

int Misere_AIPlayer::backtrack_minimax(Misere_Board* board, char current_symbol) {
    auto empties = board->get_empty_cells();
    if (empties.empty()) return 0; // Draw

    bool is_ai_turn = (current_symbol == this->get_symbol());
    int best = is_ai_turn ? -2 : 2;

    for (auto mv : empties) {
        int r = mv.first, c = mv.second;
        board->make_temp_move(r, c, current_symbol);

        // Does this move force the current player to lose?
        if (board->placement_creates_three(r, c, current_symbol)) {
            // Mover loses.
            // If AI moved and lost -> Outcome -1
            // If Opponent moved and lost -> Outcome +1
            int outcome = (current_symbol == this->get_symbol()) ? -1 : 1;
            
            board->undo_temp_move(r, c);

            if (is_ai_turn) best = max(best, outcome);
            else best = min(best, outcome);
            
            // Pruning
            if (is_ai_turn && best == 1) return 1;
            if (!is_ai_turn && best == -1) return -1;
            continue;
        }

        // Recurse
        char next = (current_symbol == 'X') ? 'O' : 'X';
        int score = backtrack_minimax(board, next);
        board->undo_temp_move(r, c);

        if (is_ai_turn) best = max(best, score);
        else best = min(best, score);

        if (is_ai_turn && best == 1) return 1;
        if (!is_ai_turn && best == -1) return -1;
    }

    if (best == -2 || best == 2) return 0;
    return best;
}

pair<int,int> Misere_AIPlayer::tiebreak_choose(const vector<pair<int,int>>& choices) {
    pair<int,int> center = {1,1};
    for (auto p : choices) if (p == center) return p; // Prefer Center

    vector<pair<int,int>> corners = { {0,0}, {0,2}, {2,0}, {2,2} };
    for (auto c : corners) {
        for (auto p : choices) if (p == c) return p; // Prefer Corners
    }
    return choices.front(); // Then Sides
}

// -----------------------------------------------------------------------------
// Misere_UI Implementation
// -----------------------------------------------------------------------------

Misere_UI::Misere_UI(Misere_Board* board) : UI<char>("Welcome to Misere Tic-Tac-Toe", 1), board_ptr(board) {
    srand((unsigned)time(nullptr));
}

Player<char>** Misere_UI::setup_players() {
    // ... Standard setup logic ...
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };

    cout << "\n--- Misère Tic-Tac-Toe Player Setup ---\n";
    string name1 = get_player_name("Player 1");
    PlayerType typeX = get_player_type_choice("Player 1", type_options);
    if (typeX == PlayerType::COMPUTER) players[0] = new Misere_AIPlayer(name1, 'X');
    else players[0] = create_player(name1, 'X', typeX);

    string nameO = get_player_name("Player 2");
    PlayerType typeO = get_player_type_choice("Player 2", type_options);
    if (typeO == PlayerType::COMPUTER) players[1] = new Misere_AIPlayer(nameO, 'O');
    else players[1] = create_player(nameO, 'O', typeO);

    return players;
}

Player<char>* Misere_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* Misere_UI::get_move(Player<char>* player) {
    char sym = player->get_symbol();
    if (player->get_type() == PlayerType::HUMAN) {
        // ... Input loop ...
        int r, c;
        while(true) {
            cout << player->get_name() << " (" << sym << ") Enter row col: ";
            if(cin >> r >> c && board_ptr->cell_empty(r, c)) break;
            cout << "Invalid.\n";
            cin.clear(); cin.ignore(10000, '\n');
        }
        return new Move<char>(r, c, sym);
    } else {
        Misere_AIPlayer* ai = dynamic_cast<Misere_AIPlayer*>(player);
        char opp_sym = (sym == 'X') ? 'O' : 'X';
        return ai->get_smart_move(board_ptr, opp_sym);
    }
}
#include "FourInRow_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>
#include <vector>

using namespace std;

FourInRow_Board::FourInRow_Board() : Board(6, 7) {
    for (auto& row : board) {
        for (auto& cell : row) {
            cell = blank_symbol;
        }
    }
}

bool FourInRow_Board::update_board(Move<char>* move) {
    int col = move->get_y();
    char symbol = move->get_symbol();

    if (col < 0 || col >= columns) return false;

    int row = get_next_empty_row(col);
    if (row == -1) return false;

    board[row][col] = symbol;
    n_moves++;
    return true;
}

int FourInRow_Board::get_next_empty_row(int col) {
    for (int row = rows - 1; row >= 0; row--) {
        if (board[row][col] == blank_symbol) {
            return row;
        }
    }
    return -1;
}

bool FourInRow_Board::check_direction(int row, int col, int delta_row, int delta_col, char symbol) {
    int count = 0;
    for (int i = 0; i < 4; i++) {
        int new_row = row + i * delta_row;
        int new_col = col + i * delta_col;
        if (new_row < 0 || new_row >= rows || new_col < 0 || new_col >= columns) return false;
        if (board[new_row][new_col] == symbol) count++;
        else break;
    }
    return count == 4;
}

bool FourInRow_Board::check_win_condition(int row, int col, char symbol) {
    // Horizontal, Vertical, Diagonal 1, Diagonal 2
    for (int c = max(0, col - 3); c <= min(columns - 4, col); c++)
        if (check_direction(row, c, 0, 1, symbol)) return true;
    for (int r = max(0, row - 3); r <= min(rows - 4, row); r++)
        if (check_direction(r, col, 1, 0, symbol)) return true;
    for (int r = max(0, row - 3); r <= min(rows - 4, row); r++)
        for (int c = max(0, col - 3); c <= min(columns - 4, col); c++)
            if (check_direction(r, c, 1, 1, symbol)) return true;
    for (int r = max(0, row - 3); r <= min(rows - 4, row); r++)
        for (int c = min(columns - 1, col + 3); c >= max(3, col); c--)
            if (check_direction(r, c, 1, -1, symbol)) return true;

    return false;
}

bool FourInRow_Board::is_win(Player<char>* player) {
    char symbol = player->get_symbol();
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            if (board[row][col] == symbol && check_win_condition(row, col, symbol)) {
                return true;
            }
        }
    }
    return false;
}

bool FourInRow_Board::is_lose(Player<char>* player) { return false; }
bool FourInRow_Board::is_draw(Player<char>* player) { return n_moves == rows * columns; }
bool FourInRow_Board::game_is_over(Player<char>* player) { return is_win(player) || is_draw(player); }

bool FourInRow_Board::is_valid_move(int col) {
    return col >= 0 && col < columns && board[0][col] == blank_symbol;
}

vector<int> FourInRow_Board::get_valid_moves() {
    vector<int> moves;
    int order[] = {3, 2, 4, 1, 5, 0, 6};
    for(int col : order) {
        if(is_valid_move(col)) moves.push_back(col);
    }
    return moves;
}


int FourInRow_Board::evaluate_window(const vector<char>& window, char piece) {
    int score = 0;
    char opp_piece = (piece == 'X') ? 'O' : 'X';
    int piece_count = 0;
    int empty_count = 0;
    int opp_count = 0;

    for (char cell : window) {
        if (cell == piece) piece_count++;
        else if (cell == blank_symbol) empty_count++;
        else if (cell == opp_piece) opp_count++;
    }

    if (piece_count == 4) score += 100;
    else if (piece_count == 3 && empty_count == 1) score += 5;
    else if (piece_count == 2 && empty_count == 2) score += 2;

    if (opp_count == 3 && empty_count == 1) score -= 4; // Block opponent!

    return score;
}

int FourInRow_Board::score_position(char piece) {
    int score = 0;

    // 1. Center Column Preference
    vector<char> center_array;
    for (int i = 0; i < rows; i++) {
        center_array.push_back(board[i][columns / 2]);
    }
    int center_count = 0;
    for (char c : center_array) {
        if (c == piece) center_count++;
    }
    score += center_count * 3;

    // 2. Horizontal Windows
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns - 3; c++) {
            vector<char> window = {board[r][c], board[r][c+1], board[r][c+2], board[r][c+3]};
            score += evaluate_window(window, piece);
        }
    }

    // 3. Vertical Windows
    for (int c = 0; c < columns; c++) {
        for (int r = 0; r < rows - 3; r++) {
            vector<char> window = {board[r][c], board[r+1][c], board[r+2][c], board[r+3][c]};
            score += evaluate_window(window, piece);
        }
    }

    // 4. Diagonal Windows (Positive slope)
    for (int r = 0; r < rows - 3; r++) {
        for (int c = 0; c < columns - 3; c++) {
            vector<char> window = {board[r][c], board[r+1][c+1], board[r+2][c+2], board[r+3][c+3]};
            score += evaluate_window(window, piece);
        }
    }

    // 5. Diagonal Windows (Negative slope)
    for (int r = 0; r < rows - 3; r++) {
        for (int c = 0; c < columns - 3; c++) {
            vector<char> window = {board[r+3][c], board[r+2][c+1], board[r+1][c+2], board[r][c+3]};
            score += evaluate_window(window, piece);
        }
    }

    return score;
}

int FourInRow_Board::minimax(int depth, int alpha, int beta, bool maximizingPlayer, char ai_symbol) {
    char opp_symbol = (ai_symbol == 'X') ? 'O' : 'X';


    Player<char> ai_p("AI", ai_symbol, PlayerType::AI);
    Player<char> opp_p("Opp", opp_symbol, PlayerType::AI); // Temp player for check

    if (is_win(&ai_p)) return 1000000;
    if (is_win(&opp_p)) return -1000000;
    if (n_moves == rows * columns) return 0; // Draw
    if (depth == 0) return score_position(ai_symbol);

    vector<int> valid_moves = get_valid_moves();

    if (maximizingPlayer) {
        int maxEval = -2000000000; // negative infinity
        for (int col : valid_moves) {
            int row = get_next_empty_row(col);

            // Make Move
            board[row][col] = ai_symbol;
            n_moves++;

            int eval = minimax(depth - 1, alpha, beta, false, ai_symbol);

            // Undo Move
            board[row][col] = blank_symbol;
            n_moves--;

            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) break; // Pruning
        }
        return maxEval;
    }
    else {
        int minEval = 2000000000; // positive infinity
        for (int col : valid_moves) {
            int row = get_next_empty_row(col);

            // Make Move
            board[row][col] = opp_symbol;
            n_moves++;

            int eval = minimax(depth - 1, alpha, beta, true, ai_symbol);

            // Undo Move
            board[row][col] = blank_symbol;
            n_moves--;

            minEval = min(minEval, eval);
            beta = min(beta, eval);
            if (beta <= alpha) break; // Pruning
        }
        return minEval;
    }
}

int FourInRow_Board::get_best_move(char ai_symbol) {
    int depth = 5;

    vector<int> valid_moves = get_valid_moves();
    int bestVal = -2000000000;
    int bestMove = valid_moves[0];

    for (int col : valid_moves) {
        int row = get_next_empty_row(col);

        board[row][col] = ai_symbol;
        n_moves++;

        int moveVal = minimax(depth - 1, -2000000000, 2000000000, false, ai_symbol);

        // Undo Move
        board[row][col] = blank_symbol;
        n_moves--;

        if (moveVal > bestVal) {
            bestMove = col;
            bestVal = moveVal;
        }
    }
    return bestMove;
}


FourInRow_UI::FourInRow_UI(FourInRow_Board* board) :
    UI<char>("Welcome to Four-in-a-Row Game! Connect four to win.", 3), board_ptr(board) {
    srand(time(0));
}

Player<char>** FourInRow_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };

    cout << "\n--- Four-in-a-Row Game Player Setup ---\n";

    string name1 = get_player_name("Player 1 (X)");
    PlayerType type1 = get_player_type_choice("Player 1 (X)", type_options);
    players[0] = create_player(name1, 'X', type1);

    string name2 = get_player_name("Player 2 (O)");
    PlayerType type2 = get_player_type_choice("Player 2 (O)", type_options);
    players[1] = create_player(name2, 'O', type2);

    return players;
}

Player<char>* FourInRow_UI::create_player(string& name, char symbol, PlayerType type) {
    string type_str = (type == PlayerType::HUMAN) ? "Human" : "Computer";
    cout << "Created " << type_str << " player: " << name << " (Symbol: " << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}

Move<char>* FourInRow_UI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::HUMAN) {
        cout << "\n" << player->get_name() << "'s turn (Symbol: " << player->get_symbol() << ")\n";
        int col;
        while (true) {
            cout << "Enter column (0-6): ";
            if (cin >> col) {
                if (board_ptr->is_valid_move(col)) break;
                else cout << "Invalid or full column.\n";
            } else {
                cin.clear(); cin.ignore(10000, '\n');
            }
        }
        return new Move<char>(0, col, player->get_symbol());
    }
    else {
        cout << "AI " << player->get_name() << " is thinking...\n";
        int best_col = board_ptr->get_best_move(player->get_symbol());
        cout << "AI chose column " << best_col << "\n";
        return new Move<char>(0, best_col, player->get_symbol());
    }
}
#include "FourInRow_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;


FourInRow_Board::FourInRow_Board() : Board(6, 7) {
    // Initialize all cells with blank_symbol
    for (auto& row : board) {
        for (auto& cell : row) {
            cell = blank_symbol;
        }
    }
}

bool FourInRow_Board::update_board(Move<char>* move) {
    int col = move->get_y();
    char symbol = move->get_symbol();

    if (col < 0 || col >= columns) {
        cout << "Invalid move: Column " << col << " is out of bounds.\n";
        return false;
    }

    int row = get_next_empty_row(col);
    if (row == -1) {
        cout << "Invalid move: Column " << col << " is full.\n";
        return false;
    }

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
    return -1; // Column is full
}

bool FourInRow_Board::check_direction(int row, int col, int delta_row, int delta_col, char symbol) {
    int count = 0;
    for (int i = 0; i < 4; i++) {
        int new_row = row + i * delta_row;
        int new_col = col + i * delta_col;

        if (new_row < 0 || new_row >= rows || new_col < 0 || new_col >= columns) {
            return false;
        }

        if (board[new_row][new_col] == symbol) {
            count++;
        } else {
            break;
        }
    }
    return count == 4;
}

bool FourInRow_Board::check_win_condition(int row, int col, char symbol) {
    // Check horizontal (left to right)
    for (int c = max(0, col - 3); c <= min(columns - 4, col); c++) {
        if (check_direction(row, c, 0, 1, symbol)) return true;
    }

    // Check vertical (bottom to top)
    for (int r = max(0, row - 3); r <= min(rows - 4, row); r++) {
        if (check_direction(r, col, 1, 0, symbol)) return true;
    }

    // Check diagonal (bottom-left to top-right)
    for (int r = max(0, row - 3); r <= min(rows - 4, row); r++) {
        for (int c = max(0, col - 3); c <= min(columns - 4, col); c++) {
            if (check_direction(r, c, 1, 1, symbol)) return true;
        }
    }

    // Check diagonal (bottom-right to top-left)
    for (int r = max(0, row - 3); r <= min(rows - 4, row); r++) {
        for (int c = min(columns - 1, col + 3); c >= max(3, col); c--) {
            if (check_direction(r, c, 1, -1, symbol)) return true;
        }
    }

    return false;
}

bool FourInRow_Board::is_win(Player<char>* player) {
    char symbol = player->get_symbol();

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            if (check_win_condition(row, col, symbol)) {
                return true;
            }
        }
    }
    return false;
}

bool FourInRow_Board::is_lose(Player<char>* player) {
    return false;
}

bool FourInRow_Board::is_draw(Player<char>* player) {
    return n_moves == rows * columns;
}

bool FourInRow_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

bool FourInRow_Board::is_valid_move(int col) {
    return col >= 0 && col < columns && board[0][col] == blank_symbol;
}

vector<int> FourInRow_Board::get_valid_moves() {
    vector<int> valid_moves;
    for (int col = 0; col < columns; col++) {
        if (is_valid_move(col)) {
            valid_moves.push_back(col);
        }
    }
    return valid_moves;
}

void FourInRow_Board::make_temp_move(int col, char symbol) {
    int row = get_next_empty_row(col);
    if (row != -1) {
        board[row][col] = symbol;
        n_moves++;
    }
}

void FourInRow_Board::undo_temp_move(int col) {
    for (int row = 0; row < rows; row++) {
        if (board[row][col] != blank_symbol) {
            board[row][col] = blank_symbol;
            n_moves--;
            break;
        }
    }
}

bool FourInRow_Board::would_win(int col, char symbol) {
    make_temp_move(col, symbol);
    Player<char> temp_player("temp", symbol, PlayerType::HUMAN);
    bool wins = is_win(&temp_player);
    undo_temp_move(col);
    return wins;
}

int FourInRow_Board::evaluate_board(char ai_symbol, char human_symbol) {
    Player<char> ai_player("AI", ai_symbol, PlayerType::AI);
    Player<char> human_player("Human", human_symbol, PlayerType::HUMAN);

    // 1. Check game-ending conditions
    if (is_win(&ai_player)) return 1000;
    if (is_win(&human_player)) return -1000;
    if (is_draw(&ai_player)) return 0;

    for (int col = 0; col < columns; col++) {
        if (is_valid_move(col)) {
            if (would_win(col, ai_symbol)) return  1000;
            if (would_win(col, human_symbol)) return  -1000;
        }
    }


    int score = 0;

    // 2. Simple position evaluation
    for (int col = 2; col <= 4; col++) {
        if (is_valid_move(col)) {
            if (col == 3) score += 3;
            else score += 1;
        }
    }



    return score;
}
//================================
// Smart AI Player Implementation with Proper Backtracking
//================================

int FourInRow_AIPlayer::backtracking_decision(FourInRow_Board* board, char ai_symbol, char human_symbol) {
    int best_score = -9999;
    int best_move = -1;

    vector<int> valid_moves = board->get_valid_moves();

    // PHASE 1: Check for immediate winning moves
    for (int col : valid_moves) {
        if (board->would_win(col, ai_symbol)) {
            return col;
        }
    }

    // PHASE 2: Check if opponent can win next turn
    for (int col : valid_moves) {
        if (board->would_win(col, human_symbol)) {
            return col;
        }
    }

    // PHASE 3: Evaluate all moves
    for (int ai_col : valid_moves) {
        board->make_temp_move(ai_col, ai_symbol);

        // Find the WORST human response
        int worst_score = 9999;

        vector<int> human_moves = board->get_valid_moves();
        for (int human_col : human_moves) {
            // Try human's response temporarily
            board->make_temp_move(human_col, human_symbol);

            // Evaluate the resulting position
            int score = board->evaluate_board(ai_symbol, human_symbol);

            // Restore human's move
            board->undo_temp_move(human_col);

            // Remember the WORST outcome for AI
            if (score < worst_score) {
                worst_score = score;
            }
        }

        // Restore AI's move
        board->undo_temp_move(ai_col);

        // AI chooses the move with the BEST worst-case scenario
        if (worst_score > best_score || best_move == -1) {
            best_score = worst_score;
            best_move = ai_col;
        }
    }

    return best_move;
}


Move<char>* FourInRow_AIPlayer::get_smart_move(FourInRow_Board* board, char human_symbol) {
    cout << "AI " << get_name() << " is thinking...\n";

    int best_col = backtracking_decision(board, get_symbol(), human_symbol);

    cout << "AI " << get_name() << " placed '" << get_symbol()
         << "' in column " << best_col << "\n";

    return new Move<char>(0, best_col, get_symbol());
}


FourInRow_UI::FourInRow_UI(FourInRow_Board* board) : UI<char>("Welcome to Four-in-a-Row Game! Connect four in a row to win.", 3),
                                    board_ptr(board) {
    srand(time(0));
}

Player<char>** FourInRow_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };

    cout << "\n--- Four-in-a-Row Game Player Setup ---\n";

    // Player 1
    string name1 = get_player_name("Player 1 (X)");
    PlayerType type1 = get_player_type_choice("Player 1 (X)", type_options);
    if (type1 == PlayerType::COMPUTER) {
        players[0] = new FourInRow_AIPlayer(name1, 'X');
    } else {
        players[0] = create_player(name1, 'X', type1);
    }

    // Player 2
    string name2 = get_player_name("Player 2 (O)");
    PlayerType type2 = get_player_type_choice("Player 2 (O)", type_options);
    if (type2 == PlayerType::COMPUTER) {
        players[1] = new FourInRow_AIPlayer(name2, 'O');
    } else {
        players[1] = create_player(name2, 'O', type2);
    }

    cout << "----------------------------------------\n";
    return players;
}

Player<char>* FourInRow_UI::create_player(string& name, char symbol, PlayerType type) {
    string type_str = (type == PlayerType::HUMAN) ? "Human" : "Computer";
    cout << "Created " << type_str << " player: " << name
         << " (Symbol: " << symbol << ")\n";
    return new Player<char>(name, symbol, type);
}


Move<char>* FourInRow_UI::get_move(Player<char>* player) {
    int col;

    if (player->get_type() == PlayerType::HUMAN) {
        // Human player logic
        cout << "\n---------------------------------\n";
        cout << player->get_name() << "'s turn (Symbol: " << player->get_symbol() << ")\n";

        // Get column
        while (true) {
            cout << "Enter column (0-6): ";
            if (cin >> col) {
                if (col >= 0 && col < 7) {
                    if (board_ptr->is_valid_move(col)) {
                        break;
                    } else {
                        cout << "Column " << col << " is full! Choose another column.\n";
                    }
                } else {
                    cout << "Invalid column! Please enter a number between 0-6.\n";
                }
            } else {
                cout << "Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
        }
        cout << "---------------------------------\n";

        return new Move<char>(0, col, player->get_symbol());

    } else if (player->get_type() == PlayerType::AI) {
        // Smart AI player using backtracking
        FourInRow_AIPlayer* ai_player = dynamic_cast<FourInRow_AIPlayer*>(player);
        char human_symbol = (player->get_symbol() == 'X') ? 'O' : 'X';
        return ai_player->get_smart_move(board_ptr, human_symbol);

    }
}
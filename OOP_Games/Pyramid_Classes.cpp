#include "Pyramid_Classes.h"
#include <iostream>
#include <iomanip>

using namespace std;


// Initialize the board
bool Pyramid_Board::is_valid_position(int x, int y) {
    // Geometry of the pyramid
    if (x == 0 && y == 2) return true;
    if (x == 1 && y >= 1 && y <= 3) return true;
    if (x == 2 && y >= 0 && y <= 4) return true;
    return false;
}
Pyramid_Board::Pyramid_Board() : Board(3, 5) {
    // Initialize the board.
    // We mark invalid cells with spaces ' ' and valid cells with standard dots '.'
    // Valid cells: (0,2), (1,1..3), (2,0..4)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (is_valid_position(i, j)) {
                board[i][j] = '.'; // Valid empty cell
            } else {
                board[i][j] = ' '; // Invalid cell (outside pyramid)
            }
        }
    }
}


bool Pyramid_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char symbol = move->get_symbol();

    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        return false;
    }

    if (!is_valid_position(x, y)) {
        return false;
    }

    if (board[x][y] != '.') {
        return false;
    }

    board[x][y] = symbol;
    n_moves++;
    return true;
}

bool Pyramid_Board::is_win(Player<char>* player) {
    char s = player->get_symbol();
    
    // Check all winning lines
    
    // Horizontal
    if (board[1][1] == s && board[1][2] == s && board[1][3] == s) return true;
    if (board[2][0] == s && board[2][1] == s && board[2][2] == s) return true;
    if (board[2][1] == s && board[2][2] == s && board[2][3] == s) return true;
    if (board[2][2] == s && board[2][3] == s && board[2][4] == s) return true;

    // Vertical
    if (board[0][2] == s && board[1][2] == s && board[2][2] == s) return true;

    // Diagonal
    if (board[0][2] == s && board[1][1] == s && board[2][0] == s) return true;
    if (board[0][2] == s && board[1][3] == s && board[2][4] == s) return true;

    return false;
}

bool Pyramid_Board::is_lose(Player<char>* player) {
    return false;
}

bool Pyramid_Board::is_draw(Player<char>* player) {
    // Total valid cells = 1 + 3 + 5 = 9.
    return (n_moves == 9 && !is_win(player));
}

bool Pyramid_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

vector<pair<int, int>> Pyramid_Board::get_valid_moves() {
    vector<pair<int, int>> moves;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (is_valid_position(i, j) && board[i][j] == '.') {
                moves.push_back({i, j});
            }
        }
    }
    return moves;
}

int Pyramid_Board::evaluate_move(int x, int y, char symbol) {
    // Simple heuristic for AI:
    // 1. Win immediately
    // 2. Block opponent win
    
    char original = board[x][y];
    char opponent = (symbol == 'X') ? 'O' : 'X';
    
    // Check for win
    board[x][y] = symbol;

    bool win = false;
    if ((board[1][1]==symbol && board[1][2]==symbol && board[1][3]==symbol) ||
        (board[2][0]==symbol && board[2][1]==symbol && board[2][2]==symbol) ||
        (board[2][1]==symbol && board[2][2]==symbol && board[2][3]==symbol) ||
        (board[2][2]==symbol && board[2][3]==symbol && board[2][4]==symbol) ||
        (board[0][2]==symbol && board[1][2]==symbol && board[2][2]==symbol) ||
        (board[0][2]==symbol && board[1][1]==symbol && board[2][0]==symbol) ||
        (board[0][2]==symbol && board[1][3]==symbol && board[2][4]==symbol)) {
        win = true;
    }
    
    if (win) {
        board[x][y] = original;
        return 100; // Winning move
    }

    // Check for blocking
    board[x][y] = opponent;
    bool block = false;
    if ((board[1][1]==opponent && board[1][2]==opponent && board[1][3]==opponent) ||
        (board[2][0]==opponent && board[2][1]==opponent && board[2][2]==opponent) ||
        (board[2][1]==opponent && board[2][2]==opponent && board[2][3]==opponent) ||
        (board[2][2]==opponent && board[2][3]==opponent && board[2][4]==opponent) ||
        (board[0][2]==opponent && board[1][2]==opponent && board[2][2]==opponent) ||
        (board[0][2]==opponent && board[1][1]==opponent && board[2][0]==opponent) ||
        (board[0][2]==opponent && board[1][3]==opponent && board[2][4]==opponent)) {
        block = true;
    }
    
    board[x][y] = original; // Reset
    
    if (block) return 50; // Blocking move
    if (x == 1 && y == 2) return 10; // Center is good
    if (x == 0 && y == 2) return 10; // Top is good
    
    return 1; // Standard move
}

// --- Pyramid_UI Implementation ---

Pyramid_UI::Pyramid_UI(Pyramid_Board* board) : UI<char>("Welcome to Pyramid Tic-Tac-Toe!", 3), board_ptr(board) {}

Player<char>* Pyramid_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char>* Pyramid_UI::get_move(Player<char>* player) {
    if (player->get_type() == PlayerType::HUMAN) {
        int x, y;
        while (true) {
            cout << player->get_name() << " (" << player->get_symbol() << "), enter your move (row col): ";
            if (cin >> x >> y) {
                if (board_ptr->is_valid_position(x, y) && board_ptr->get_board_matrix()[x][y] == '.') {
                    break;
                } else {
                    cout << "Invalid move. Please try again.\n";
                }
            } else {
                cout << "Invalid input. Enter numbers.\n";
                cin.clear();
                cin.ignore(10000, '\n');
            }
        }
        return new Move<char>(x, y, player->get_symbol());
    } else {
        // AI Logic
        vector<pair<int, int>> valid_moves = board_ptr->get_valid_moves();
        if (valid_moves.empty()) return new Move<char>(-1, -1, ' ');

        int best_score = -1;
        pair<int, int> best_move = valid_moves[0];

        for (auto& move : valid_moves) {
            int score = board_ptr->evaluate_move(move.first, move.second, player->get_symbol());
            if (score > best_score) {
                best_score = score;
                best_move = move;
            }
        }
        
        cout << "Player " << player->get_name() << " chooses " << best_move.first << " " << best_move.second << endl;
        return new Move<char>(best_move.first, best_move.second, player->get_symbol());
    }
}
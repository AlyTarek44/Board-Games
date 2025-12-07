#include "Pyramid_Classes.h"
#include <iostream>
#include <iomanip>
#include <algorithm>   // for std::max, std::min
#include <vector>

using namespace std;

// -----------------------------------------------------------------------------
// Pyramid_Board Implementation
// -----------------------------------------------------------------------------

// Helper: check if (row, col) is a valid cell in the pyramid shape
bool Pyramid_Board::is_valid_position(int row, int col) {
    // Row 0: only col 2
    if (row == 0 && col == 2) return true;

    // Row 1: cols 1,2,3
    if (row == 1 && col >= 1 && col <= 3) return true;

    // Row 2: cols 0,1,2,3,4
    if (row == 2 && col >= 0 && col <= 4) return true;

    // Anything else is invalid
    return false;
}

Pyramid_Board::Pyramid_Board() : Board(3, 5) {
    // Initialize board with a pyramid layout:
    // - Valid cells: '.'
    // - Invalid cells: ' ' (space)
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            if (is_valid_position(row, col)) {
                board[row][col] = '.'; // empty valid cell
            } else {
                board[row][col] = ' '; // invalid cell (outside pyramid)
            }
        }
    }
}

bool Pyramid_Board::update_board(Move<char> *move) {
    int row = move->get_x();
    int col = move->get_y();
    char token = move->get_symbol();

    // Basic bounds check
    if (row < 0 || row >= rows || col < 0 || col >= columns)
        return false;

    // Must be a valid pyramid position
    if (!is_valid_position(row, col))
        return false;

    // Cell must be empty
    if (board[row][col] != '.')
        return false;

    // Place the token
    board[row][col] = token;
    n_moves++;
    return true;
}

bool Pyramid_Board::is_win(Player<char> *player) {
    return check_win_symbol(player->get_symbol());
}

// Check if the given symbol has a winning line
bool Pyramid_Board::check_win_symbol(char s) {
    // Horizontal lines
    if (board[1][1] == s && board[1][2] == s && board[1][3] == s) return true;

    if (board[2][0] == s && board[2][1] == s && board[2][2] == s) return true;
    if (board[2][1] == s && board[2][2] == s && board[2][3] == s) return true;
    if (board[2][2] == s && board[2][3] == s && board[2][4] == s) return true;

    // Vertical line
    if (board[0][2] == s && board[1][2] == s && board[2][2] == s) return true;

    // Diagonals from the top
    if (board[0][2] == s && board[1][1] == s && board[2][0] == s) return true;
    if (board[0][2] == s && board[1][3] == s && board[2][4] == s) return true;

    return false;
}

bool Pyramid_Board::is_lose(Player<char> *player) {
    return false;
}

bool Pyramid_Board::is_draw(Player<char> *player) {
    return (n_moves == 9 && !is_win(player));
}

bool Pyramid_Board::game_is_over(Player<char> *player) {
    return is_win(player) || is_draw(player);
}

// Collect all currently available moves (empty valid cells)
vector<pair<int, int> > Pyramid_Board::get_valid_moves() {
    vector<pair<int, int> > moves;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            if (is_valid_position(row, col) && board[row][col] == '.') {
                moves.push_back({row, col});
            }
        }
    }
    return moves;
}

// Minimax Algorithm


int Pyramid_Board::minimax(int depth, bool isMaximizing,
                           char aiSymbol, char humanSymbol) {
    // 1) Check terminal positions
    if (check_win_symbol(aiSymbol)) {
        // AI wins: higher score is better, reward faster wins
        return 10 - depth;
    }
    if (check_win_symbol(humanSymbol)) {
        // Human wins: lower score for AI, we prefer to delay this
        return depth - 10;
    }

    // Check if there are no more moves (draw)
    vector<pair<int, int> > validMoves = get_valid_moves();
    if (validMoves.empty()) {
        return 0; // Draw
    }

    // 2) Recursive search
    if (isMaximizing) {
        int bestScore = -1000;

        for (auto &move: validMoves) {
            int row = move.first;
            int col = move.second;

            // Try this move for AI
            board[row][col] = aiSymbol;

            int score = minimax(depth + 1, false, aiSymbol, humanSymbol);

            // Undo the move
            board[row][col] = '.';

            bestScore = max(bestScore, score);
        }
        return bestScore;
    } else {
        int bestScore = 1000;

        for (auto &move: validMoves) {
            int row = move.first;
            int col = move.second;

            // Try this move for the human
            board[row][col] = humanSymbol;

            int score = minimax(depth + 1, true, aiSymbol, humanSymbol);

            // Undo the move
            board[row][col] = '.';

            bestScore = min(bestScore, score);
        }
        return bestScore;
    }
}


// Choose the best move for the AI by running minimax on each option
pair<int, int> Pyramid_Board::get_best_move(char aiSymbol) {
    char humanSymbol = (aiSymbol == 'X') ? 'O' : 'X';
    vector<pair<int, int> > validMoves = get_valid_moves();

    int bestScore = -1000;
    pair<int, int> bestMove = {-1, -1};

    for (auto &move: validMoves) {
        int row = move.first;
        int col = move.second;

        // Try this move for the AI
        board[row][col] = aiSymbol;

        // Now it's the human's turn -> isMaximizing = false
        int score = minimax(0, false, aiSymbol, humanSymbol);

        // Undo the move
        board[row][col] = '.';

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }

    return bestMove;
}

// --- Pyramid_UI Implementation ---

Pyramid_UI::Pyramid_UI(Pyramid_Board *board) : UI<char>("Welcome to Pyramid Tic-Tac-Toe!", 3), board_ptr(board) {
}

Player<char> *Pyramid_UI::create_player(string &name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

Move<char> *Pyramid_UI::get_move(Player<char> *player) {
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
        // AI Logic using Minimax
        pair<int, int> best_move = board_ptr->get_best_move(player->get_symbol());
        cout << "Player " << player->get_name() << " chooses " << best_move.first << " " << best_move.second << endl;
        return new Move<char>(best_move.first, best_move.second, player->get_symbol());
    }
}

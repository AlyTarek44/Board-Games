#include "FourByFour_Classes.h"
#include <iostream>
#include <climits>

using namespace std;

// ---------------------------------------
// FourByFour_Board Implementation
// ---------------------------------------

FourByFour_Board::FourByFour_Board() : Board(4, 4) {
    // Start with an empty board
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            board[r][c] = '.';

    // Initial layout:
    // Row 0: O X O X
    board[0][0] = 'O';
    board[0][1] = 'X';
    board[0][2] = 'O';
    board[0][3] = 'X';

    // Row 3: X O X O
    board[3][0] = 'X';
    board[3][1] = 'O';
    board[3][2] = 'X';
    board[3][3] = 'O';
}

bool FourByFour_Board::update_board(Move<char> *move) {
    FourByFour_Move *m = static_cast<FourByFour_Move *>(move);

    int fr = m->get_from_r();
    int fc = m->get_from_c();
    int tr = m->get_to_r();
    int tc = m->get_to_c();
    char symbol = m->get_symbol();

    // 1) Bounds check
    if (fr < 0 || fr >= 4 || fc < 0 || fc >= 4) {
        cout << "Invalid move: source cell is outside the board.\n";
        return false;
    }
    if (tr < 0 || tr >= 4 || tc < 0 || tc >= 4) {
        cout << "Invalid move: destination cell is outside the board.\n";
        return false;
    }

    // 2) Source must contain player's symbol
    if (board[fr][fc] != symbol) {
        if (board[fr][fc] == '.') {
            cout << "Invalid move: source cell is empty.\n";
        } else {
            cout << "Invalid move: source cell does not contain your token.\n";
        }
        return false;
    }

    // 3) Destination must be empty
    if (board[tr][tc] != '.') {
        cout << "Invalid move: destination cell is not empty.\n";
        return false;
    }

    // 4) Movement rule: one step horizontally or vertically
    int dr = abs(fr - tr);
    int dc = abs(fc - tc);

    if (dr == 0 && dc == 0) {
        cout << "Invalid move: you must move to a different cell.\n";
        return false;
    }

    bool one_step_orthogonal =
            (dr == 1 && dc == 0) ||
            (dr == 0 && dc == 1);

    if (!one_step_orthogonal) {
        cout << "Invalid move: you can only move one step up, down, left, or right.\n";
        return false;
    }

    // 5) Apply the move
    board[tr][tc] = board[fr][fc];
    board[fr][fc] = '.';

    n_moves++;
    return true;
}

bool FourByFour_Board::check_win_for_symbol(char s) {
    // Horizontal 3 in a row
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c <= 4 - 3; ++c) {
            if (board[r][c] == s &&
                board[r][c + 1] == s &&
                board[r][c + 2] == s)
                return true;
        }
    }

    // Vertical 3 in a column
    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r <= 4 - 3; ++r) {
            if (board[r][c] == s &&
                board[r + 1][c] == s &&
                board[r + 2][c] == s)
                return true;
        }
    }

    // Main diagonal direction (3-length)
    if (board[0][0] == s && board[1][1] == s && board[2][2] == s) return true;
    if (board[1][1] == s && board[2][2] == s && board[3][3] == s) return true;
    if (board[0][1] == s && board[1][2] == s && board[2][3] == s) return true;
    if (board[1][0] == s && board[2][1] == s && board[3][2] == s) return true;

    // Anti-diagonal direction
    if (board[0][3] == s && board[1][2] == s && board[2][1] == s) return true;
    if (board[1][2] == s && board[2][1] == s && board[3][0] == s) return true;
    if (board[0][2] == s && board[1][1] == s && board[2][0] == s) return true;
    if (board[1][3] == s && board[2][2] == s && board[3][1] == s) return true;

    return false;
}

bool FourByFour_Board::is_win(Player<char> *player) {
    return check_win_for_symbol(player->get_symbol());
}

bool FourByFour_Board::is_lose(Player<char> *player) {
    return false;
}

bool FourByFour_Board::is_draw(Player<char> *player) {
    // Just a safety to avoid infinite loops
    return n_moves > 250;
}

bool FourByFour_Board::game_is_over(Player<char> *player) {
    return is_win(player) || is_draw(player);
}

vector<FourByFour_Move> FourByFour_Board::get_valid_moves(char symbol) {
    vector<FourByFour_Move> moves;

    // Directions: up, down, left, right
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            if (board[r][c] != symbol) continue;

            for (int k = 0; k < 4; ++k) {
                int nr = r + dr[k];
                int nc = c + dc[k];

                // Check bounds
                if (nr < 0 || nr >= 4 || nc < 0 || nc >= 4) continue;

                // Must be empty destination
                if (board[nr][nc] == '.') {
                    moves.emplace_back(r, c, nr, nc, symbol);
                }
            }
        }
    }

    return moves;
}

int FourByFour_Board::calc_heuristic_value(char ai_symbol, char opp_symbol) {
    int score = 0;

    if (check_win_for_symbol(ai_symbol)) score += 1000;
    if (check_win_for_symbol(opp_symbol)) score -= 1000;

    return score;
}

// ---------------------------------------
// FourByFour_UI Implementation
// ---------------------------------------

FourByFour_UI::FourByFour_UI(FourByFour_Board *board)
    : UI<char>("Welcome to 4x4 Tic-Tac-Toe! Move your pieces to align 3.", 3),
      board_ptr(board) {
}

Player<char> *FourByFour_UI::create_player(string &name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}


int FourByFour_UI::minimax(int depth, bool isMaximizing, char ai_symbol, char opp_symbol) {
    if (board_ptr->check_win_for_symbol(ai_symbol))
        return 1000 + depth; // earlier win is better

    if (board_ptr->check_win_for_symbol(opp_symbol))
        return -1000 - depth; // later loss is better

    if (depth == 0)
        return 0;

    char current_symbol = isMaximizing ? ai_symbol : opp_symbol;
    vector<FourByFour_Move> moves = board_ptr->get_valid_moves(current_symbol);

    if (moves.empty())
        return 0;

    if (isMaximizing) {
        int bestVal = -1000000;

        for (auto &move: moves) {
            int fr = move.get_from_r();
            int fc = move.get_from_c();
            int tr = move.get_to_r();
            int tc = move.get_to_c();

            // Apply move for AI
            board_ptr->set_cell(tr, tc, current_symbol);
            board_ptr->set_cell(fr, fc, '.');

            int val = minimax(depth - 1, false, ai_symbol, opp_symbol);

            // Undo move
            board_ptr->set_cell(fr, fc, current_symbol);
            board_ptr->set_cell(tr, tc, '.');

            if (val > bestVal)
                bestVal = val;
        }
        return bestVal;
    } else {
        int bestVal = 1000000;

        for (auto &move: moves) {
            int fr = move.get_from_r();
            int fc = move.get_from_c();
            int tr = move.get_to_r();
            int tc = move.get_to_c();

            // Apply move for opponent
            board_ptr->set_cell(tr, tc, current_symbol);
            board_ptr->set_cell(fr, fc, '.');

            int val = minimax(depth - 1, true, ai_symbol, opp_symbol);

            // Undo move
            board_ptr->set_cell(fr, fc, current_symbol);
            board_ptr->set_cell(tr, tc, '.');

            if (val < bestVal)
                bestVal = val;
        }
        return bestVal;
    }
}

FourByFour_Move FourByFour_UI::find_best_move(char ai, char opp, int depth) {
    auto moves = board_ptr->get_valid_moves(ai);

    int bestVal = -10000000;
    FourByFour_Move bestMove(0, 0, 0, 0, ai);

    for (auto &move: moves) {
        int fr = move.get_from_r();
        int fc = move.get_from_c();
        int tr = move.get_to_r();
        int tc = move.get_to_c();

        // Apply
        board_ptr->set_cell(tr, tc, ai);
        board_ptr->set_cell(fr, fc, '.');

        int val = minimax(depth - 1, false, ai, opp);

        // Undo
        board_ptr->set_cell(fr, fc, ai);
        board_ptr->set_cell(tr, tc, '.');

        if (val > bestVal) {
            bestVal = val;
            bestMove = move;
        }
    }

    return bestMove;
}

Move<char> *FourByFour_UI::get_move(Player<char> *player) {
    char symbol = player->get_symbol();

    if (player->get_type() == PlayerType::HUMAN) {
        int fr, fc, tr, tc;

        while (true) {
            cout << player->get_name() << " (" << symbol << ")\n";
            cout << "Choose a token to move (row col): ";
            cin >> fr >> fc;

            if (fr < 0 || fr > 3 || fc < 0 || fc > 3 ||
                board_ptr->get_cell(fr, fc) != symbol) {
                cout << "Invalid selection. This is not your token.\n";
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            cout << "Choose destination (row col): ";
            cin >> tr >> tc;

            if (tr < 0 || tr > 3 || tc < 0 || tc > 3 ||
                board_ptr->get_cell(tr, tc) != '.') {
                cout << "Invalid destination.\n";
                continue;
            }

            return new FourByFour_Move(fr, fc, tr, tc, symbol);
        }
    } else {
        char opp = (symbol == 'X') ? 'O' : 'X';

        FourByFour_Move best = find_best_move(symbol, opp, 6);


        cout << "AI (" << player->get_name() << ") moves" << symbol << "from ("
                << best.get_from_r() << "," << best.get_from_c()
                << ") to (" << best.get_to_r()
                << "," << best.get_to_c() << ")\n";

        return new FourByFour_Move(
            best.get_from_r(),
            best.get_from_c(),
            best.get_to_r(),
            best.get_to_c(),
            symbol
        );
    }
}

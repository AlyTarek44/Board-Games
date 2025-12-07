/**
* @file Infinity_Tic-Tac-Toe.cpp
 * @brief Implementation of Infinity Tic-Tac-Toe mechanics.
 */

#include "Infinity_Tic-Tac-Toe.h"
#include <iostream>
#include <iomanip>
#include <cctype>
#include <deque>
using namespace std;

inf_xo_Board::inf_xo_Board() { }

/**
 * @brief Places a mark and handles the removal of old marks.
 *
 * Pushes the new move to the `moves` deque.
 * If the number of tracked moves exceeds the limit (3), the front of the deque (oldest move)
 * is popped and cleared from the board.
 *
 * @param move The move to be applied.
 * @return true if successful, false if invalid.
 */
bool inf_xo_Board::update_board(Move<char> *move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Validation
    if (x < 0 || x > rows || y < 0 || y > columns || board[x][y] != blank_symbol) {
        return false;
    }

    // Apply new move
    board[x][y] = mark;
    moves.push_back(make_pair(x,y));
    counter++;

    // Logic to remove the oldest move if limit is reached.
    // Note: The logic here seems to check n_moves (global) and counter.
    if (n_moves == 3) {
        auto oldest_move = moves.front();
        moves.pop_front();
        board[oldest_move.first][oldest_move.second] = blank_symbol;
        counter = 0;
    }

    // Additional check based on counter
    if (counter == 3) {
        auto old = moves.front();
        moves.pop_front();
        board[old.first][old.second] = blank_symbol;
        counter = 0;
    }

    return true;
}

bool inf_xo_Board::is_draw(Player<char>* player) {
    if ( !is_win(player) && !check_available_moves()) {
        return true;
    }
    return false;
}

bool inf_xo_Board::check_available_moves() {
    for (auto& row : board) {
        for (auto& cell : row) {
            if (cell == blank_symbol) {
                return true;
            }
        }
    }
    return false;
}
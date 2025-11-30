//
// Created by Ali Tarek on 25/11/2025.
//

#include "Infinity_Tic-Tac-Toe.h"
#include <iostream>
#include <iomanip>
#include <cctype>
#include <deque>
using namespace std;

inf_xo_Board::inf_xo_Board() { }

bool inf_xo_Board::update_board(Move<char> *move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark=move->get_symbol();

    if (x < 0 || x > rows || y < 0 || y > columns || board[x][y] != blank_symbol) {
        return false;
    }

board[x][y] = mark;
moves.push_back(make_pair(x,y));
    counter++;
    if (n_moves == 3) {
        auto oldest_move = moves.front();
        moves.pop_front();
        board[oldest_move.first][oldest_move.second] = blank_symbol;
        counter = 0;

    }
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
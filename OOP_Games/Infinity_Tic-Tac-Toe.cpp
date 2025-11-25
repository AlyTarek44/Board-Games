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

bool inf_xo_Board::update_board(Move<char>* move) {
    int x = move->get_x();
    int y = move->get_y();
    char mark = move->get_symbol();

    // Validate move and apply if valid
    if (!(x < 0 || x >= rows || y < 0 || y >= columns) &&
        (board[x][y] == blank_symbol)) {

        n_moves++;
        board[x][y] = toupper(mark);
        moves.push_back({x, y});
        counter++;

        if (counter == 3) {
            auto old = moves.front();
            moves.pop_front();
            board[old.first][old.second] = blank_symbol;
            counter = 0;
        }

        return true;
        }

    return false;
}

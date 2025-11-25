//
// Created by Ali Tarek on 25/11/2025.
//

#ifndef INFINITY_TIC_TAC_TOE_H
#define INFINITY_TIC_TAC_TOE_H
#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "deque"


using namespace std;

class inf_xo_Board : public X_O_Board {
private:
    char blank_symbol = '.';
    deque<pair<int,int>> moves;
    int counter = 0;
public:

        inf_xo_Board();

    bool update_board(Move<char>* move)override;

};

#endif //INFINITY_TIC_TAC_TOE_H

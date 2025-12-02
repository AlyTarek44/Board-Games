//
// Created by Ali Tarek on 01/12/2025.
//

#ifndef OBSTACLES_TIC_TAC_TOE_H
#define OBSTACLES_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include<iostream>
using namespace std;

class obstacles_TTT_board : public Board<char> {
  private :
    char blank_symbol = '.';
    char obstacle_symbol = '#';
    public :
     obstacles_TTT_board();
      bool update_board(Move<char> *move) override;
      bool is_draw(Player<char> *) override;
      bool is_win(Player<char> *) override;
      bool is_lose(Player<char> *) override;
      bool game_is_over(Player<char> *) override;
      bool check_win();

      vector<pair<int, int>> get_moves();



};


class obstacles_TTT_UI : public UI<char> {

obstacles_TTT_board *board;

public :

    obstacles_TTT_UI(obstacles_TTT_board *b);

    Player<char> *createplayer(string& name, char symbol, PlayerType type) ;

    Move<char> *get_move(Player<char> *);

};

















#endif //OBSTACLES_TIC_TAC_TOE_H

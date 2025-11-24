//
// Created by Ali Tarek on 23/11/2025.
//

#ifndef FIVEXFIVETTT_CLASSES_H
#define FIVEXFIVETTT_CLASSES_H



#include "BoardGame_Classes.h"
using namespace std;




class FxFTTT_board : public Board<char> {
private :
  char blank_symbol = '.';

  public :
    FxFTTT_board();

    bool update_board(Move<char>* move)override;
    bool is_win(Player<char>* player)override;
    bool is_draw(Player<char>* player)override;
    bool is_lose(Player<char> *player)override;
    bool game_is_over(Player<char> *) override;
    int  check_all_lines();
};
class FxFTTT_ui : public UI<char> {
  public :
    FxFTTT_ui();

    Player<char>* create_player(string& name, char symbol, PlayerType type);
    virtual Move<char>* get_move(Player<char>* player);
};



#endif //FIVEXFIVETTT_CLASSES_H

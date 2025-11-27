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
    char ai_symbol = 'X';
    char opp_symbol = 'O';
  public :
    FxFTTT_board();




    bool update_board(Move<char>* move)override;
    bool is_win(Player<char>* player)override;
    bool is_draw(Player<char>* player)override;
    bool is_lose(Player<char> *player)override;
    bool game_is_over(Player<char> *) override;
    int  check_all_lines();

    // AI functions
     void simulate_move( int row, int col, char symbol);
     void undo_move( int row, int col);
     std::vector<std::pair<int,int>> get_available_moves();
     int minimax(int depth, bool is_max, int alpha, int beta);
     int evaluate_board();
     std::pair<int,int> find_best_move();
    void set_ai_symbols(char ai, char opp);
};

class FxFTTT_ui : public UI<char> {
  public :
    FxFTTT_ui();

    Player<char>* create_player(string& name, char symbol, PlayerType type)override;
     Move<char>* get_move(Player<char>* player)override;
};






#endif //FIVEXFIVETTT_CLASSES_H

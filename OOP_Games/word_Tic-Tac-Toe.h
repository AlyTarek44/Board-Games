//
// Created by Ali Tarek on 26/11/2025.
//
#ifndef WORD_TIC_TAC_TOE_H
#define WORD_TIC_TAC_TOE_H
#include "BoardGame_Classes.h"
#include <unordered_set>


class word_xo_board : public Board<char>{
private:
  unordered_set<string> dictionary;

  char blank_symbol = '.';

public:
  word_xo_board();

  bool update_board(Move<char> *move) override;
  bool is_win(Player<char>* player)override;
  bool is_draw(Player<char>* player)override;
  bool is_lose(Player<char>* player)override;
  bool game_is_over(Player<char>* player)override;
  bool is_valid_word();
  void load_dictionary();

void simulate_move(int row , int col , char symbol);
void undo_move(int row , int col , char symbol);

  Move<char>* find_best_move();




};


class word_xo_ui : public UI<char>{
word_xo_board *board;
  public:
  word_xo_ui(word_xo_board*b);

  Player<char>* create_player(string& name, char symbol, PlayerType type)override;

  Move<char> *get_move(Player<char> *) override;

};


























#endif //WORD_TIC_TAC_TOE_H

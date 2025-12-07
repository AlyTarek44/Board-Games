/**
* @file word_Tic-Tac-Toe.h
 * @brief Header for Word Tic-Tac-Toe.
 *
 * Players place any letter on a 3x3 board.
 * Winner is the first to form a valid English word of length 3 (horizontally, vertically, or diagonally).
 */

#ifndef WORD_TIC_TAC_TOE_H
#define WORD_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include <unordered_set>

using namespace std;

/**
 * @class word_xo_board
 * @brief Board that validates English words for win condition.
 */
class word_xo_board : public Board<char> {
private:
  unordered_set<string> dictionary; ///< Stores valid words loaded from `dic.txt`.
  char blank_symbol = '.';

public:
  word_xo_board();

  /**
   * @brief Updates board with a letter placed by player.
   * @param move Pointer to Move (contains char and position).
   * @return true if cell empty and move valid.
   */
  bool update_board(Move<char> *move) override;

  /**
   * @brief Checks if a valid word is formed.
   * @param player Pointer to player.
   * @return true if valid word exists on board.
   */
  bool is_win(Player<char>* player) override;

  /**
   * @brief Checks for draw (full board, no words).
   * @param player Pointer to player.
   * @return true if draw.
   */
  bool is_draw(Player<char>* player) override;

  /**
   * @brief Checks lose condition.
   * @return Always false (standard win/draw rules).
   */
  bool is_lose(Player<char>* player) override;

  /**
   * @brief Checks if game is over.
   * @return true if win or draw.
   */
  bool game_is_over(Player<char>* player) override;

  /**
   * @brief Checks if any line on the board forms a valid word.
   * @return true if word found.
   */
  bool is_valid_word();

  /**
   * @brief Loads `dic.txt` into the set.
   */
  void load_dictionary();

  // Helpers for AI simulation
  void simulate_move(int row , int col , char symbol);
  void undo_move(int row , int col , char symbol);

  /**
   * @brief Finds the best move for AI.
   * Tries to find a winning move, or block an opponent's winning move.
   * @return Pointer to best Move.
   */
  Move<char>* find_best_move();
};

/**
 * @class word_xo_ui
 * @brief UI for Word Tic-Tac-Toe (allows any char input).
 */
class word_xo_ui : public UI<char> {
  word_xo_board *board;
public:
  word_xo_ui(word_xo_board*b);

  /**
   * @brief Creates a player.
   */
  Player<char>* create_player(string& name, char symbol, PlayerType type) override;

  /**
   * @brief Gets move from player (asks for char AND position).
   * @param player Pointer to player.
   * @return Pointer to Move.
   */
  Move<char> *get_move(Player<char> *) override;
};

#endif //WORD_TIC_TAC_TOE_H
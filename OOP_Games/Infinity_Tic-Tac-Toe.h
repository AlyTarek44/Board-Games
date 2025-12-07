/**
* @file Infinity_Tic-Tac-Toe.h
 * @brief Header file for Infinity Tic-Tac-Toe.
 *
 * Implements a variant of Tic-Tac-Toe where players can only have 3 marks on the board.
 * If a 4th mark is placed, the oldest one is removed.
 */

#ifndef INFINITY_TIC_TAC_TOE_H
#define INFINITY_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include "XO_Classes.h"
#include "deque"

using namespace std;

/**
 * @class inf_xo_Board
 * @brief Represents the Infinity Tic-Tac-Toe board.
 *
 * Inherits from `X_O_Board` but overrides the update logic to manage
 * a queue of moves, ensuring only the last 3 moves per player (or total, depending on logic) exist.
 */
class inf_xo_Board : public X_O_Board {
private:
    char blank_symbol = '.'; ///< Empty cell symbol.
    deque<pair<int,int>> moves; ///< Stores the history of moves to identify the oldest one.
    int counter = 0; ///< Internal counter (used for logic tracking).

public:
    /**
     * @brief Constructor.
     */
    inf_xo_Board();

    /**
     * @brief Updates the board with specific infinity mechanics.
     * Removes the oldest move if the limit is reached.
     * @param move Pointer to move object.
     * @return true if move valid.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks for a draw condition (rare in this variant).
     * @param player Pointer to player.
     * @return true if draw.
     */
    bool is_draw(Player<char> *) override;

    /**
     * @brief Checks if there are any empty cells left.
     * @return true if moves are available.
     */
    bool check_available_moves();
};

#endif //INFINITY_TIC_TAC_TOE_H
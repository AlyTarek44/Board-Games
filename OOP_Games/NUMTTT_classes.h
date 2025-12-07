/**
 * @file NUMTTT_classes.h
 * @brief Header file for Numerical Tic-Tac-Toe.
 *
 * Players play with numbers (1-9) instead of X and O.
 * Player 1: Odd numbers (1, 3, 5, 7, 9).
 * Player 2: Even numbers (2, 4, 6, 8).
 * Winning Condition: 3 cells in a line sum to 15.
 */

#ifndef NUMTTT_CLASSES_H
#define NUMTTT_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>

using namespace std;

class NumTTT_Board;

/**
 * @class NumTTT_Board
 * @brief 3x3 Board for integers. Checks for sum=15.
 */
class NumTTT_Board : public Board<int> {
private:
    int blank_symbol = 0; ///< 0 represents an empty cell.

    // Lists to track available numbers for each player
    vector<int> odd_nums;
    vector<int> even_nums;

    /**
     * @brief Checks all 8 lines (rows, cols, diags) for a sum of 15.
     * @return true if any line sums to 15.
     */
    bool check_all_lines();

public:
    /**
     * @brief Constructor for Numerical TTT Board.
     */
    NumTTT_Board();

    /**
     * @brief Validates move (correct player using correct number) and updates board.
     * @param move Pointer to Move object (int type).
     * @return true if move valid.
     */
    bool update_board(Move<int>* move) override;

    /**
     * @brief Checks if player has won (sum 15).
     * @param player Pointer to player.
     * @return true if winning condition met.
     */
    bool is_win(Player<int>* player) override;

    /**
     * @brief Checks lose condition.
     * @return Always false.
     */
    bool is_lose(Player<int>* player) override { return false; }

    /**
     * @brief Checks draw condition.
     * @param player Pointer to player.
     * @return true if board full and no sum 15.
     */
    bool is_draw(Player<int>* player) override;

    /**
     * @brief Checks if game is over.
     * @param player Pointer to player.
     * @return true if win or draw.
     */
    bool game_is_over(Player<int>* player) override;

    const vector<int>& get_odd_nums() const { return odd_nums; }
    const vector<int>& get_even_nums() const { return even_nums; }
};

/**
 * @class NumTTT_UI
 * @brief UI handling for integer inputs and number selection.
 */
class NumTTT_UI : public UI<int> {
private:
    NumTTT_Board* board_ptr;

    /**
     * @brief Prints available numbers for the current player.
     * @param nums Vector of available integers.
     */
    void print_available(const vector<int>& nums);

public:
    NumTTT_UI(NumTTT_Board* b);

    /**
     * @brief Creates a player for Numerical TTT.
     */
    Player<int>* create_player(string& name, int symbol, PlayerType type) override;

    /**
     * @brief Gets move. Asks for both position AND number to place.
     * @param player Pointer to player.
     * @return Pointer to Move object.
     */
    Move<int>* get_move(Player<int>* player) override;

    /**
     * @brief Setup players.
     */
    Player<int>** setup_players() override;
};

#endif
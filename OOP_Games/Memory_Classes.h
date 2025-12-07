/**
 * @file Memory_Classes.h
 * @brief Header file for Memory Tic-Tac-Toe.
 *
 * In this game, the board is hidden from the players. After a move is made,
 * the board is briefly shown, then hidden again. Players must rely on their memory
 * to know which cells are occupied.
 */

#ifndef MEMORY_CLASSES_H
#define MEMORY_CLASSES_H

#include "BoardGame_Classes.h"
using namespace std;

/**
 * @class Memory_Board
 * @brief A 3x3 Board that manages hidden vs. real states.
 */
class Memory_Board : public Board<char> {
private:
    char blank_symbol = '.';
    char hidden_symbol = '?';       ///< Symbol used to mask occupied cells.
    vector<vector<char>> real_board; ///< Stores the actual values ('X', 'O') which are hidden from view.

public:
    /**
     * @brief Constructor initializing a 3x3 board.
     */
    Memory_Board();

    /**
     * @brief Updates the internal real board with the move.
     * @param move Pointer to move object.
     * @return true if valid move, false if cell occupied or out of bounds.
     */
    bool update_board(Move<char>* move);

    /**
     * @brief Standard Tic-Tac-Toe win check on the real board.
     * @param player Pointer to player.
     * @return true if player has winning line.
     */
    bool is_win(Player<char>* player);

    /**
     * @brief Checks if player has lost.
     * @return Always false.
     */
    bool is_lose(Player<char>*) { return false; };

    /**
     * @brief Checks for draw on real board.
     * @param player Pointer to player.
     * @return true if full and no winner.
     */
    bool is_draw(Player<char>* player);

    /**
     * @brief Checks if game is over.
     * @param player Pointer to player.
     * @return true if win or draw.
     */
    bool game_is_over(Player<char>* player);

    /**
     * @brief Returns a masked version of the board.
     * Occupied cells are shown as '?', empty cells as '.'.
     * @return Matrix of characters.
     */
    vector<vector<char>> get_board_matrix() const;

    /**
     * @brief Returns a special view of the board for the "reveal" phase.
     * Shows only the specific symbol just played, hiding everything else with '?'.
     * @param move_x The row of the current move.
     * @param move_y The column of the current move.
     * @param symbol The symbol to reveal at (move_x, move_y).
     * @return Matrix of characters.
     */
    vector<vector<char>> get_move_display_board(int move_x, int move_y, char symbol) const;
};

/**
 * @class Memory_UI
 * @brief UI that handles the timed reveal and screen clearing mechanics.
 */
class Memory_UI : public UI<char> {
private:
    Memory_Board* memory_board;

    /**
     * @brief Clears the console screen (cross-platform implementation).
     */
    void clear_screen();

public:
    Memory_UI(Memory_Board* board);
    ~Memory_UI() {};

    Player<char>* create_player(string& name, char symbol, PlayerType type);

    /**
     * @brief Gets move input (displays hidden board state).
     * @param player Pointer to player.
     * @return Pointer to Move.
     */
    virtual Move<char>* get_move(Player<char>* player);

    virtual void display_board_matrix(const vector<vector<char>>& matrix) const;

    /**
     * @brief Custom game loop manager.
     * Handles the specific flow: Show Hidden -> Get Move -> Update -> Show Move -> Sleep -> Clear -> Repeat.
     * @param players Array of 2 player pointers.
     */
    void run_game(Player<char>* players[2]);
};

#endif // MEMORY_CLASSES_H
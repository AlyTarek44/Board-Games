/**
 * @file Obstacles_Tic-Tac-Toe.h
 * @brief Header for Obstacles Tic-Tac-Toe.
 *
 * A 6x6 board where the goal is to get n-in-a-row (usually 4),
 * but random obstacles are placed on the board periodically to block paths.
 */

#ifndef OBSTACLES_TIC_TAC_TOE_H
#define OBSTACLES_TIC_TAC_TOE_H

#include "BoardGame_Classes.h"
#include<iostream>
using namespace std;

/**
 * @class obstacles_TTT_board
 * @brief Represents the Obstacles Tic-Tac-Toe game board.
 *
 * Inherits from Board<char>. Includes logic for placing obstacles
 * and checking for a 4-in-a-row win condition on a 6x6 grid.
 */
class obstacles_TTT_board : public Board<char> {
private:
    char blank_symbol = '.';      ///< Represents an empty cell.
    char obstacle_symbol = '#';   ///< Represents a blocked cell (obstacle).

public:
    /**
     * @brief Default constructor for Obstacles Tic-Tac-Toe board.
     */
    obstacles_TTT_board();

    /**
     * @brief Updates the board with the player's move and potentially adds obstacles.
     * @param move Pointer to the Move object containing coordinates and symbol.
     * @return true if the move is valid, false otherwise.
     * @note Random obstacles may be added every few turns.
     */
    bool update_board(Move<char> *move) override;

    /**
     * @brief Checks if the game is a draw.
     * @param player Pointer to the player (unused in logic).
     * @return true if board is full and no winner, false otherwise.
     */
    bool is_draw(Player<char> *) override;

    /**
     * @brief Checks if the player has won (4-in-a-row).
     * @param player Pointer to the player to check.
     * @return true if the player has winning sequence, false otherwise.
     */
    bool is_win(Player<char> *) override;

    /**
     * @brief Checks if the player has lost.
     * @param player Pointer to the player.
     * @return Always false for this game variant.
     */
    bool is_lose(Player<char> *) override;

    /**
     * @brief Checks if the game is over (win or draw).
     * @param player Pointer to the player.
     * @return true if the game has ended, false otherwise.
     */
    bool game_is_over(Player<char> *) override;

    /**
     * @brief Helper function to check for 4 consecutive symbols.
     * @return true if any line of 4 exists.
     */
    bool check_win();

    /**
     * @brief Returns list of all empty cells.
     * @return Vector of coordinate pairs (row, col).
     */
    vector<pair<int, int>> get_moves();
};

/**
 * @class obstacles_TTT_UI
 * @brief User Interface for Obstacles Tic-Tac-Toe.
 */
class obstacles_TTT_UI : public UI<char> {
    obstacles_TTT_board *board;
public:
    /**
     * @brief Constructor for Obstacles UI.
     * @param b Pointer to the game board.
     */
    obstacles_TTT_UI(obstacles_TTT_board *b);

    /**
     * @brief Creates a new player.
     * @param name Name of the player.
     * @param symbol Symbol for the player.
     * @param type Type of player (Human/Random/Smart).
     * @return Pointer to the created Player.
     */
    Player<char> *createplayer(string& name, char symbol, PlayerType type);

    /**
     * @brief Gets the move from the player.
     * @param player Pointer to the player whose turn it is.
     * @return Pointer to the Move object.
     */
    Move<char> *get_move(Player<char> *);
};

#endif //OBSTACLES_TIC_TAC_TOE_H
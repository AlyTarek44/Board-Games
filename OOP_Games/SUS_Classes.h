/**
 * @file SUS_Classes.h
 * @brief Header for the SUS game.
 *
 * Players compete to form the sequence 'S-U-S' on a 3x3 grid.
 * Points are awarded for each sequence formed. The player with the most points wins.
 */

#ifndef SUS_CLASSES_H
#define SUS_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>

using namespace std;

/**
 * @class SUS_Board
 * @brief 3x3 Board with scoring logic for S-U-S sequences.
 */
class SUS_Board : public Board<char> {
private:
    char blank_symbol = '.';
    int player1_score = 0;
    int player2_score = 0;
    char player1_letter, player2_letter; ///< Stores 'S' or 'U' for each player.

    /**
     * @brief Checks if the last move completed any 'S-U-S' sequences.
     * @param last_x Row of last move.
     * @param last_y Col of last move.
     * @param symbol Symbol placed.
     * @return Number of new sequences formed.
     */
    int check_and_score_sequences(int last_x, int last_y, char symbol);

    /**
     * @brief Returns coordinates of all possible winning lines (rows, cols, diagonals).
     * @return Vector of lines (each line is a vector of coordinates).
     */
    vector<vector<pair<int, int>>> get_all_lines();

public:
    /**
     * @brief Constructor for SUS Board.
     */
    SUS_Board();

    /**
     * @brief Updates board with move and calculates score.
     * @param move Pointer to Move object.
     * @return true if valid move, false otherwise.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks win by comparing final scores (only when board is full).
     * @param player Pointer to player.
     * @return true if player score > opponent score when full.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks lose condition.
     * @param player Pointer to player.
     * @return true if player score < opponent score when full.
     */
    bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks draw condition.
     * @param player Pointer to player.
     * @return true if scores are equal when board is full.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks if game is over (board is full).
     * @param player Pointer to player.
     * @return true if no empty cells remain.
     */
    bool game_is_over(Player<char>* player) override;

    void set_player_letters(char p1_letter, char p2_letter);
    int get_player1_score() const { return player1_score; }
    int get_player2_score() const { return player2_score; }
    char get_player1_letter() const { return player1_letter; }
    char get_player2_letter() const { return player2_letter; }

    /**
     * @brief Returns list of valid moves.
     * @return Vector of empty cell coordinates.
     */
    vector<pair<int, int>> get_valid_moves();

    /**
     * @brief Heuristic evaluation for AI to determine move quality.
     * @param x Row coordinate.
     * @param y Column coordinate.
     * @param symbol Symbol to place.
     * @param depth Lookahead depth.
     * @return Score evaluation.
     */
    int evaluate_move(int x, int y, char symbol, int depth = 2);
};

/**
 * @class SUS_UI
 * @brief UI for SUS game, handles letter selection (S or U).
 */
class SUS_UI : public UI<char> {
private:
    SUS_Board* board_ptr;
    char player1_letter, player2_letter;

    /**
     * @brief Prompts players to choose 'S' or 'U'.
     */
    void choose_letters();

public:
    SUS_UI(SUS_Board* board);
    ~SUS_UI() {}

    /**
     * @brief Creates a player for SUS.
     * @param name Name of player.
     * @param symbol Symbol (not used directly, uses selected letter).
     * @param type Player type.
     * @return Pointer to Player.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Gets move from player.
     * @param player Pointer to player.
     * @return Pointer to Move.
     */
    Move<char>* get_move(Player<char>* player) override;

    /**
     * @brief Sets up both players.
     * @return Array of two Player pointers.
     */
    Player<char>** setup_players() override;

    /**
     * @brief Displays current scores of both players.
     */
    void display_scores() const;
};

#endif
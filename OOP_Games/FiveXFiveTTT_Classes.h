/**
* @file FiveXFiveTTT_Classes.h
 * @brief Header for 5x5 Tic-Tac-Toe.
 *
 * In this variant, the winner is determined by who has the most 3-in-a-row patterns
 * on a 5x5 board when the game ends (24 moves limit).
 */

#ifndef FIVEXFIVETTT_CLASSES_H
#define FIVEXFIVETTT_CLASSES_H

#include "BoardGame_Classes.h"
using namespace std;

/**
 * @class FxFTTT_board
 * @brief Represents the 5x5 Tic-Tac-Toe board logic.
 */
class FxFTTT_board : public Board<char> {
private :
    char blank_symbol = '.';
    char ai_symbol = 'X';
    char opp_symbol = 'O';

public :
    /**
     * @brief Constructor for 5x5 Board.
     */
    FxFTTT_board();

    /**
     * @brief Updates the board with a move.
     * @param move Pointer to the Move object.
     * @return true if valid move, false otherwise.
     */
    bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if the player has won (has more 3-in-a-rows).
     * @param player Pointer to the player.
     * @return true if player's count > opponent's count at end of game.
     */
    bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if game is a draw (equal 3-in-a-rows).
     * @param player Pointer to the player.
     * @return true if scores are equal at end of game.
     */
    bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks if player has lost.
     * @param player Pointer to the player.
     * @return true if player's count < opponent's count at end of game.
     */
    bool is_lose(Player<char> *player) override;

    /**
     * @brief Checks if game is over (24 moves reached).
     * @param player Pointer to the player.
     * @return true if 24 moves have been played.
     */
    bool game_is_over(Player<char> *) override;

    /**
     * @brief Counts the total 3-in-a-row patterns for both X and O.
     * @return 1 if X > O, -1 if O > X, 0 if equal.
     */
    int check_all_lines();

    // --- AI functions ---

    /**
     * @brief Simulates a move for AI calculation.
     */
    void simulate_move( int row, int col, char symbol);

    /**
     * @brief Undoes a simulated move.
     */
    void undo_move( int row, int col);

    /**
     * @brief Gets all available empty spots.
     * @return Vector of coordinates.
     */
    std::vector<std::pair<int,int>> get_available_moves();

    /**
     * @brief Minimax algorithm for 5x5 board optimization.
     * @param depth Search depth.
     * @param is_max Maximizing or minimizing player.
     * @param alpha Alpha value for pruning.
     * @param beta Beta value for pruning.
     * @return Heuristic score.
     */
    int minimax(int depth, bool is_max, int alpha, int beta);

    /**
     * @brief Evaluates the current board state.
     * @return Score based on 3-in-a-row counts.
     */
    int evaluate_board();

    /**
     * @brief Finds the best move for the AI.
     * @return Pair of integers (row, col).
     */
    std::pair<int,int> find_best_move();

    void set_ai_symbols(char ai, char opp);
};

/**
 * @class FxFTTT_ui
 * @brief UI for 5x5 Tic-Tac-Toe.
 */
class FxFTTT_ui : public UI<char> {
public :
    FxFTTT_ui();

    /**
     * @brief Creates a player.
     */
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;

    /**
     * @brief Gets move from player.
     */
    Move<char>* get_move(Player<char>* player) override;
};

#endif //FIVEXFIVETTT_CLASSES_H
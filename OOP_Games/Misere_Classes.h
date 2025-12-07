/**
 * @file Misere_Classes.h
 * @brief Header for Misere Tic-Tac-Toe.
 *
 * In Misère games, the winning condition is inverted: The player who
 * completes a line of 3 **loses**.
 */

#ifndef MISERE_CLASSES_H
#define MISERE_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>
#include <vector>

using namespace std;

/**
 * @class Misere_Board
 * @brief Tic-Tac-Toe board where 3-in-a-row loses the game.
 */
class Misere_Board : public Board<char> {
public:
    Misere_Board();
    virtual ~Misere_Board() {}

    /**
     * @brief Updates board with move.
     * @param move Pointer to move.
     * @return true if valid.
     */
    virtual bool update_board(Move<char>* move) override;

    /**
     * @brief Checks if player has won.
     * @param player Pointer to player.
     * @return Always false, as the game ends when a player *loses*.
     */
    virtual bool is_win(Player<char>* player) override;

    /**
     * @brief Checks if player has lost (i.e., successfully made 3-in-a-row).
     * @param player Pointer to player.
     * @return true if player has 3 consecutive symbols.
     */
    virtual bool is_lose(Player<char>* player) override;

    /**
     * @brief Checks for draw.
     * @param player Pointer to player.
     * @return true if full and no 3-in-a-row (rare/impossible in 3x3 if filled? check logic).
     */
    virtual bool is_draw(Player<char>* player) override;

    /**
     * @brief Checks if game is over.
     * @param player Pointer to player.
     * @return true if lose condition met or draw.
     */
    virtual bool game_is_over(Player<char>* player) override;

    bool player_has_three(char symbol) const;
    bool cell_empty(int r, int c) const;
    vector<pair<int,int>> get_empty_cells() const;

    bool make_temp_move(int r, int c, char symbol);
    void undo_temp_move(int r, int c);

    /**
     * @brief Checks if placing 'symbol' at (r,c) would create a 3-in-a-row line.
     * Used by AI to avoid suicidal moves.
     */
    bool placement_creates_three(int r, int c, char symbol) const;

    static const char blank_symbol;
};

/**
 * @class Misere_AIPlayer
 * @brief Smart AI for Misere Tic-Tac-Toe.
 */
class Misere_AIPlayer : public Player<char> {
public:
    Misere_AIPlayer(const string& name, char symbol);
    virtual ~Misere_AIPlayer() {}

    /**
     * @brief Calculates smart move to avoid winning (losing).
     * @param board Pointer to board.
     * @param opponent_symbol Opponent's char.
     * @return Pointer to best Move.
     */
    Move<char>* get_smart_move(Misere_Board* board, char opponent_symbol);

private:
    /**
     * @brief Recursive Minimax algorithm for Misere logic.
     * @return +1 if AI wins (forces opponent to lose), -1 if AI loses.
     */
    int backtrack_minimax(Misere_Board* board, char current_symbol);

    pair<int,int> tiebreak_choose(const vector<pair<int,int>>& choices);
};

/**
 * @class Misere_UI
 * @brief UI for Misere game.
 */
class Misere_UI : public UI<char> {
    Misere_Board* board_ptr;
public:
    Misere_UI(Misere_Board* board);
    virtual ~Misere_UI() {}

    virtual Player<char>** setup_players() override;
    virtual Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    virtual Move<char>* get_move(Player<char>* player) override;
};

#endif
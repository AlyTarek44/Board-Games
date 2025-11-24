#ifndef MISERE_CLASSES_H
#define MISERE_CLASSES_H

#include "BoardGame_Classes.h"
#include <string>
#include <vector>

using namespace std;

// Misère Tic-Tac-Toe (3x3). Losing condition: the player who completes 3-in-a-row loses.

class Misere_Board : public Board<char> {
public:
    Misere_Board();
    virtual ~Misere_Board() {}

    virtual bool update_board(Move<char>* move) override;

    virtual bool is_win(Player<char>* player) override; // returns false for the player who made 3-in-a-row
    virtual bool is_lose(Player<char>* player) override;// returns true if player has a 3-in-a-row
    virtual bool is_draw(Player<char>* player) override;
    virtual bool game_is_over(Player<char>* player) override;

    // Helpers used by AI/backtracking
    bool player_has_three(char symbol) const;
    bool cell_empty(int r, int c) const;
    vector<pair<int,int>> get_empty_cells() const;

    bool make_temp_move(int r, int c, char symbol);
    void undo_temp_move(int r, int c);

    bool placement_creates_three(int r, int c, char symbol) const;

    static const char blank_symbol;
};

class Misere_AIPlayer : public Player<char> {
public:
    Misere_AIPlayer(const string& name, char symbol);
    virtual ~Misere_AIPlayer() {}

    Move<char>* get_smart_move(Misere_Board* board, char opponent_symbol);

private:
    // returns score from AI perspective: +1 = AI forces opponent to lose, 0 = draw, -1 = AI forced to lose
    int backtrack_minimax(Misere_Board* board, char current_symbol);

    // prefer center, then corners, then sides when scores tie
    pair<int,int> tiebreak_choose(const vector<pair<int,int>>& choices);
};

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

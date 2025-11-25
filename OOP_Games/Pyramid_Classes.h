#ifndef PYRAMID_CLASSES_H
#define PYRAMID_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>
#include <string>
#include <algorithm> 

using namespace std;

class Pyramid_Board : public Board<char> {
public:
    Pyramid_Board();
    
    // Core game logic overrides
    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* player) override;
    bool is_lose(Player<char>* player) override;
    bool is_draw(Player<char>* player) override;
    bool game_is_over(Player<char>* player) override;

    // Helper functions for AI and validation
    vector<pair<int, int>> get_valid_moves();
    int evaluate_move(int x, int y, char symbol);
    bool is_valid_position(int x, int y);
};

class Pyramid_UI : public UI<char> {
private:
    Pyramid_Board* board_ptr;

public:
    Pyramid_UI(Pyramid_Board* board);
    
    // Required overrides
    Player<char>* create_player(string& name, char symbol, PlayerType type) override;
    Move<char>* get_move(Player<char>* player) override;
};

#endif
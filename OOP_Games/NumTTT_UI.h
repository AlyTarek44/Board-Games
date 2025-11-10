#ifndef NUMTTT_UI_H
#define NUMTTT_UI_H

#include "BoardGame_Classes.h"
#include "NumTTT_Board.h" // Include new board header
#include <vector>

using namespace std;

//================================
// UI Class Declaration
//================================

// Inherit from UI, specifying 'int' as the template type.
class NumTTT_UI : public UI<int> {
private:
    // Pointer to the board to access available number lists
    NumTTT_Board* board_ptr;

    // Helper to print a vector of numbers
    void print_available(const vector<int>& nums);

public:
    // Constructor now takes a pointer to the board
    NumTTT_UI(NumTTT_Board* b);

    // Override virtual functions from the base UI class
    Player<int>* create_player(string& name, int symbol, PlayerType type) override;
    Move<int>* get_move(Player<int>* player) override;

    // We MUST override setup_players to create our specific int-based players
    Player<int>** setup_players() override;
};

#endif // NUMTTT_UI_H
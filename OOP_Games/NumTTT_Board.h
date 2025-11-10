#ifndef NUMTTT_BOARD_H
#define NUMTTT_BOARD_H

#include "BoardGame_Classes.h"
#include <vector>

using namespace std;

//================================
// Board Class Declaration
//================================

// Inherit from Board, specifying 'int' as the template type.
class NumTTT_Board : public Board<int> {
private:
    // Use 0 to represent a blank cell
    int blank_symbol = 0;

    // Lists to track available numbers for each player
    vector<int> odd_nums;
    vector<int> even_nums;

    // Helper function to check all 8 lines for a win
    bool check_all_lines();

public:
    // Constructor
    NumTTT_Board();

    // Override virtual functions from the base Board class
    bool update_board(Move<int>* move) override;
    bool is_win(Player<int>* player) override;
    bool is_lose(Player<int>* player) override { return false; } // Not used
    bool is_draw(Player<int>* player) override;
    bool game_is_over(Player<int>* player) override;

    // Public getter methods for the UI to read
    const vector<int>& get_odd_nums() const { return odd_nums; }
    const vector<int>& get_even_nums() const { return even_nums; }
};

#endif // NUMTTT_BOARD_H
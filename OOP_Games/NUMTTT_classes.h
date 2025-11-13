#ifndef NUMTTT_CLASSES_H
#define NUMTTT_CLASSES_H

#include "BoardGame_Classes.h"
#include <vector>

using namespace std;

// Forward declaration
class NumTTT_Board;

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
    bool is_lose(Player<int>* player) override { return false; }
    bool is_draw(Player<int>* player) override;
    bool game_is_over(Player<int>* player) override;

    // Public getter methods for the UI to read
    const vector<int>& get_odd_nums() const { return odd_nums; }
    const vector<int>& get_even_nums() const { return even_nums; }
};


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

#endif // NUMTTT_CLASSES_H
#include "NumTTT_UI.h"
#include <iostream>
#include <vector>
#include <string>


using namespace std;

//================================
// UI Class Implementation
//================================

/**
 * @brief Constructor.
 * @param b Pointer to the game board, needed to display available numbers.
 */
NumTTT_UI::NumTTT_UI(NumTTT_Board *b) : UI<int>("Welcome to Numerical Tic-Tac-Toe!", 3), // 3 is cell width
                                        board_ptr(b) {
}

Player<int> **NumTTT_UI::setup_players() {
    Player<int> **players = new Player<int> *[2];

    cout << "\n--- Player Setup ---\n";

    // Player 1 (Odd)
    string name1 = get_player_name("Player 1 (Odd)");
    PlayerType type1 = PlayerType::HUMAN; // Hardcoded to HUMAN
    // We assign symbol '1' as an ID for the Odd player
    players[0] = create_player(name1, 1, type1);

    // Player 2 (Even)
    string name2 = get_player_name("Player 2 (Even)");
    PlayerType type2 = PlayerType::HUMAN; // Hardcoded to HUMAN
    // We assign symbol '2' as an ID for the Even player
    players[1] = create_player(name2, 2, type2);

    cout << "--------------------\n";

    return players;
}

/**
 * @brief Creates a new Player<int>.
 */
Player<int> *NumTTT_UI::create_player(string &name, int symbol, PlayerType type) {
    // Note: 'symbol' is our internal ID (1 for Odd, 2 for Even)
    cout << "Created player " << symbol << ": " << name << " (" << (symbol == 1 ? "Odd" : "Even") << ")\n\n";

    return new Player<int>(name, symbol, type);
}

/**
 * @brief Overridden setup to create two players for this game.
 */


/**
 * @brief Gets a move (number and position) from the current player.
 * Contains logic for HUMAN player type only.
 */
Move<int> *NumTTT_UI::get_move(Player<int> *player) {
    // --- Human Player Logic ---
    int x, y, number;

    cout << "---------------------------------\n";
    cout << player->get_name() << "'s turn.\n";

    // Check player's symbol (ID) to see which numbers to display
    bool is_odd_player = (player->get_symbol() == 1);

    if (is_odd_player) {
        cout << "Available Odd Numbers: ";
        print_available(board_ptr->get_odd_nums());
    } else {
        cout << "Available Even Numbers: ";
        print_available(board_ptr->get_even_nums());
    }

    // Get the number they want to play
    while (true) {
        cout << "Enter number to play: ";
        if (cin >> number) {
            break; // Valid integer
        } else {
            cout << "Invalid input. Please enter an integer.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    // Get the position
    while (true) {
        cout << "Enter position (row col): ";
        if (cin >> x >> y) {
            break; // Valid integers
        } else {
            cout << "Invalid input. Please enter two integers.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
    cout << "---------------------------------\n";

    // Create the Move object with the chosen number as the "symbol"
    return new Move<int>(x, y, number);
}

/**
 * @brief Helper to print a list of available numbers.
 */
void NumTTT_UI::print_available(const vector<int> &nums) {
    cout << "{ ";
    for (size_t i = 0; i < nums.size(); ++i) {
        cout << nums[i] << (i == nums.size() - 1 ? " " : ", ");
    }
    cout << "}\n";
}

#include "NumTTT_Classes.h" // Use the new consolidated header
#include <iostream>
#include <vector>
#include <numeric>   // For std::accumulate
#include <algorithm> // For std::find
#include <string>
#include <cstdlib>   // For rand()
#include <ctime>     // For time()

using namespace std;

//================================
// Board Class Implementation
//================================

// Constructor
NumTTT_Board::NumTTT_Board() : Board(3, 3) {
    // Initialize the board with the blank symbol (0)
    for (auto &row : board) {
        for (auto &cell : row) {
            cell = blank_symbol;
        }
    }
    // Initialize the number lists
    odd_nums = {1, 3, 5, 7, 9};
    even_nums = {2, 4, 6, 8};
}

/**
 * @brief Updates the board with the given move.
 * This is the core validation logic.
 */
bool NumTTT_Board::update_board(Move<int>* move) {
    int x = move->get_x();
    int y = move->get_y();
    int number = move->get_symbol(); // This is the number (1-9) to play

    //  Check bounds
    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        if (x != -1) { // Don't print for dummy moves
            cout << "Invalid move: Position (" << x << "," << y << ") is out of bounds.\n";
        }
        return false;
    }

    // Check if cell is empty
    if (board[x][y] != blank_symbol) {
       cout << "Invalid move: Cell (" << x << "," << y << ") is already taken.\n";
        return false;
    }

    // 3. Determine whose turn it is and validate the number
    bool is_odd_turn = (n_moves % 2 == 0);

    if (is_odd_turn) {
        // Player 1 (Odd) turn
        auto it = find(odd_nums.begin(), odd_nums.end(), number);
        if (it != odd_nums.end()) {
            odd_nums.erase(it); // Valid number, remove it from list
        } else {
            cout << "Invalid move: " << number << " is not an available odd number.\n";
            return false;
        }
    } else {
        // Player 2 (Even) turn
        auto it = find(even_nums.begin(), even_nums.end(), number);
        if (it != even_nums.end()) {
            even_nums.erase(it); // Valid number, remove it from list
        } else {
            cout << "Invalid move: " << number << " is not an available even number.\n";
            return false;
        }
    }

    // 4. All checks passed.
    board[x][y] = number;
    n_moves++;
    return true;
}

//Checks if the current player has won by summing lines to 15.


/**
 * @brief Helper function to check all 8 lines.
 * @return true if any line sums to 15, false otherwise.
 */
bool NumTTT_Board::check_all_lines() {
    int sum;

    // Check rows
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] != 0 && board[i][1] != 0 && board[i][2] != 0) {
            sum = board[i][0] + board[i][1] + board[i][2];
            if (sum == 15) return true;
        }
    }

    // Check columns
    for (int j = 0; j < 3; ++j) {
        if (board[0][j] != 0 && board[1][j] != 0 && board[2][j] != 0) {
            sum = board[0][j] + board[1][j] + board[2][j];
            if (sum == 15) return true;
        }
    }

    // Check main diagonal
    if (board[0][0] != 0 && board[1][1] != 0 && board[2][2] != 0) {
        sum = board[0][0] + board[1][1] + board[2][2];
        if (sum == 15) return true;
    }

    // Check anti-diagonal
    if (board[0][2] != 0 && board[1][1] != 0 && board[2][0] != 0) {
        sum = board[0][2] + board[1][1] + board[2][0];
        if (sum == 15) return true;
    }

    return false; // No winning line found
}

bool NumTTT_Board::is_win(Player<int>* player) {
    // Win condition is independent of player, just check board
    return check_all_lines();
}

// Checks if the game is a draw.
bool NumTTT_Board::is_draw(Player<int>* player) {
    // Called after is_win() fails, so just check if board is full
    return (n_moves == 9);
}

//Checks if the game is over (win or draw).

bool NumTTT_Board::game_is_over(Player<int>* player) {
    return is_win(player) || is_draw(player);
}


//================================
// UI Class Implementation
//================================

/**
 * @brief Constructor.
 * @param b Pointer to the game board, needed to display available numbers.
 */
NumTTT_UI::NumTTT_UI(NumTTT_Board *b) : UI<int>("Welcome to Numerical Tic-Tac-Toe!", 3),
                                        board_ptr(b) {
}

/**
 * @brief Overridden setup to create two players, allowing choice of Human or Computer.
 */
Player<int> **NumTTT_UI::setup_players() {
    Player<int> **players = new Player<int> *[2];
    vector<string> type_options = { "Human", "Computer" };

    cout << "\n--- Player Setup ---\n";

    // Player 1 (Odd)
    string name1 = get_player_name("Player 1 (Odd)");
    // Ask for player type
    PlayerType type1 = get_player_type_choice("Player 1 (Odd)", type_options); 
    players[0] = create_player(name1, 1, type1); // 1 is ID for Odd

    // Player 2 (Even)
    string name2 = get_player_name("Player 2 (Even)");
    // Ask for player type
    PlayerType type2 = get_player_type_choice("Player 2 (Even)", type_options);
    players[1] = create_player(name2, 2, type2); // 2 is ID for Even

    cout << "--------------------\n";

    return players;
}

/**
 * @brief Creates a new Player<int>.
 */
Player<int> *NumTTT_UI::create_player(string &name, int symbol, PlayerType type) {
    // Note: 'symbol' is our internal ID (1 for Odd, 2 for Even)
    string type_str = (type == PlayerType::HUMAN) ? "Human" : "Computer";
    cout << "Created player " << symbol << ": " << name
         << " (" << (symbol == 1 ? "Odd" : "Even") << ") as " << type_str << "\n\n";

    return new Player<int>(name, symbol, type);
}


/**
 * @brief Gets a move (number and position) from the current player.
 * Contains logic for both HUMAN and COMPUTER player types.
 */
Move<int> *NumTTT_UI::get_move(Player<int> *player) {
    int x, y, number; // Declare variables at the top
    
    // Check player's symbol (ID) to see which numbers to use
    bool is_odd_player = (player->get_symbol() == 1);

    cout << "---------------------------------\n";
    cout << player->get_name() << "'s turn.\n";

    if (player->get_type() == PlayerType::HUMAN) {
        // --- Human Player Logic ---
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

    } else if (player->get_type() == PlayerType::COMPUTER) {
        //  Computer Player Logic
        
        // Get the correct list of numbers
        const vector<int>& available_list = is_odd_player ? board_ptr->get_odd_nums() : board_ptr->get_even_nums();

        //  Get a random number
        int rand_index = rand() % available_list.size();
        number = available_list[rand_index];
        
        // NOTE: We do NOT remove the number from the list here.
        // The update_board() function will handle validation and removal.

        // 2. Get random, empty coordinates
        int r = board_ptr->get_rows();
        int c = board_ptr->get_columns();
        do {
            x = rand() % r;
            y = rand() % c;
        } while (board_ptr->get_board_matrix()[x][y] != 0); // 0 is blank_symbol

        cout << "Computer " << player->get_name() << " chose number " << number
             << " at (" << x << ", " << y << ")\n";
        cout << "---------------------------------\n";
    }

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
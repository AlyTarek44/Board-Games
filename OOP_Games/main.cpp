#include <iostream>
#include <cstdlib> // For srand
#include <ctime>   // For time
#include "BoardGame_Classes.h"
#include "NumTTT_Classes.h"
#include "XO_Classes.h"

using namespace std;

void run_numerical_ttt() {
    srand(time(0)); // Initialize random seed ONCE per game run

    // 1) Create the board
    // We can use Board<int>* as the pointer type
    Board<int> *board = new NumTTT_Board();

    // 2) Create UI
    // We can use UI<int>* as the pointer type
    UI<int> *ui = new NumTTT_UI(static_cast<NumTTT_Board *>(board));

    // 3) Set players
    Player<int> **players = ui->setup_players();

    // 4) Create GameManager
    GameManager<int> gm(board, players, ui);

    // 5) Run
    gm.run();

    // 6) Cleanup
    delete board;
    delete ui;
    delete players[0];
    delete players[1];
    delete[] players;
}

void run_XO() {
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

    // Create an instance of the specific UI for X-O using a pointer
    UI<char> *game_ui = new XO_UI();

    // Create the game board. For X-O, this is an X_O_Board.
    Board<char> *xo_board = new X_O_Board();

    // Use the UI to set up the players for the game.
    // The UI returns a dynamically allocated array of Player pointers.
    Player<char> **players = game_ui->setup_players();

    // Create the game manager with the board and the array of players.
    GameManager<char> x_o_game(xo_board, players, game_ui);

    // Run the game loop.
    x_o_game.run();

    // --- Cleanup ---
    // Delete the dynamically allocated board object.
    delete xo_board;
    delete game_ui;

    // Delete the individual player objects.
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    // Delete the dynamically allocated array of player pointers itself.
    delete[] players;
}

int main() {
    int choice;

    while (true) {
        cout << "\n==============================\n";
        cout << "   Welcome to Game Center!    \n";
        cout << "==============================\n";
        cout << "Select a game to play:\n";
        cout << "1. Numerical Tic-Tac-Toe\n";
        cout << "2. 3*3 Tic-Tac-Toe\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1:
                run_numerical_ttt();
                break;
            case 2:
                run_XO();
                break;
            case 0:
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please select [1,2,0].\n";
        }
    }
}

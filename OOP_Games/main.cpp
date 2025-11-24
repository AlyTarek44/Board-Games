#include <iostream>
#include <cstdlib> // For srand
#include <ctime>   // For time
#include "BoardGame_Classes.h"
#include "NumTTT_Classes.h"
#include "XO_Classes.h"
#include "FourInRow_Classes.h"  // Add Four-in-a-row game header

using namespace std;

void run_numerical_ttt() {
    srand(time(0));

    // 1) Create the board
    Board<int> *board = new NumTTT_Board();

    // 2) Create UI
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
    srand(static_cast<unsigned int>(time(0)));

    UI<char> *game_ui = new XO_UI();

    Board<char> *xo_board = new X_O_Board();

    Player<char> **players = game_ui->setup_players();

    GameManager<char> x_o_game(xo_board, players, game_ui);

    // Run the game loop.
    x_o_game.run();

    // --- Cleanup ---
    delete xo_board;
    delete game_ui;

    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;
}

void run_FourInRow() {
    srand(static_cast<unsigned int>(time(0)));

    FourInRow_Board *four_board = new FourInRow_Board();

    FourInRow_UI *four_ui = new FourInRow_UI(four_board);

    Player<char> **players = four_ui->setup_players();

    GameManager<char> four_game(four_board, players, four_ui);

    four_game.run();

    delete four_board;
    delete four_ui;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
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
        cout << "3. Four-in-a-Row\n";
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
            case 3:
                run_FourInRow();
                break;
            case 0:
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please select [1,2,3,0].\n";
        }
    }
}
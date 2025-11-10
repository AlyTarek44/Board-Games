#include <iostream>
#include "BoardGame_Classes.h"
#include "NumTTT_Board.h"
#include "NumTTT_UI.h"

using namespace std;

void run_numerical_ttt() {

    // 1) Create the board
    Board<int>* board = new NumTTT_Board();

    // 2) Create UI
    UI<int>* ui = new NumTTT_UI( static_cast<NumTTT_Board*>(board) );

    // 3) Set players
    Player<int>** players = ui->setup_players();

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

int main() {

    int choice;

    while (true) {
        cout << "\n==============================\n";
        cout << "   Welcome to Game Center!    \n";
        cout << "==============================\n";
        cout << "Select a game to play:\n";
        cout << "1. Numerical Tic-Tac-Toe\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                run_numerical_ttt();
            break;
            case 0:
                cout << "Goodbye!\n";
            return 0;
            default:
                cout << "Invalid choice.\n";
        }
    }
}

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "BoardGame_Classes.h"
#include "NumTTT_Classes.h"
#include "XO_Classes.h"
#include "FourInRow_Classes.h"
#include "Misere_Classes.h"
#include "FiveXFiveTTT_Classes.h"
#include "Infinity_Tic-Tac-Toe.h"
#include "SUS_Classes.h"
#include "Pyramid_Classes.h"
#include "word_Tic-Tac-Toe.h"


using namespace std;

void run_numerical_ttt() {
    srand(time(0));

    Board<int> *board = new NumTTT_Board();

    UI<int> *ui = new NumTTT_UI(static_cast<NumTTT_Board *>(board));

    Player<int> **players = ui->setup_players();

    GameManager<int> gm(board, players, ui);

    gm.run();

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

    x_o_game.run();

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

void run_FiveXFiveTTT() {
    srand(static_cast<unsigned int>(time(0)));
    Board<char>*FxFBoard = new FxFTTT_board;
    UI<char>*game_ui = new FxFTTT_ui;
    Player<char> **players = game_ui->setup_players();
    GameManager<char> game_f(FxFBoard, players, game_ui);
    game_f.run();
    delete FxFBoard;
    delete game_ui;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;
}
void run_word_ttt() {
    srand(static_cast<unsigned int>(time(0)));
    word_xo_board *word_board = new word_xo_board();
    UI<char>* game_ui = new word_xo_ui(word_board);
    Player<char> **players = game_ui->setup_players();
    GameManager<char> game_f(word_board, players, game_ui);
    game_f.run();
    delete word_board;
    delete game_ui;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;

}
void run_Misere() {
    srand(static_cast<unsigned int>(time(0)));

    Misere_Board *board = new Misere_Board();
    Misere_UI *ui = new Misere_UI(board);

    Player<char> **players = ui->setup_players();

    GameManager<char> gm(board, players, ui);

    gm.run();

    delete board;
    delete ui;
    delete players[0];
    delete players[1];
    delete[] players;
}
void run_SUS() {
    srand(static_cast<unsigned int>(time(0)));

    SUS_Board *sus_board = new SUS_Board();

    SUS_UI *sus_ui = new SUS_UI(static_cast<SUS_Board *>(sus_board));

    Player<char> **players = sus_ui->setup_players();

    GameManager<char> sus_game(sus_board, players, sus_ui);

    sus_game.run();

    delete sus_board;
    delete sus_ui;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;
}
void run_Pyramid() {
    srand(static_cast<unsigned int>(time(0)));

    Pyramid_Board *board = new Pyramid_Board();
    Pyramid_UI *ui = new Pyramid_UI(board);

    Player<char> **players = ui->setup_players();

    GameManager<char> gm(board, players, ui);

    gm.run();

    delete board;
    delete ui;
    for (int i = 0; i < 2; ++i) {
        delete players[i];
    }
    delete[] players;
}
void run_infinite() {
    srand(static_cast<unsigned int>(time(0)));

    UI<char> *game_ui = new XO_UI();

    Board<char> *xo_board = new inf_xo_Board;

    Player<char> **players = game_ui->setup_players();

    GameManager<char> x_o_game(xo_board, players, game_ui);

    x_o_game.run();

    delete xo_board;
    delete game_ui;

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
        cout << "4. 5*5 Tic-Tac-Toe\n";
        cout << "5. word Tic-Tac-Toe\n";
        cout << "6. Misere Tic-Tac-Toe(3x3)\n";
        cout << "6. SUS\n";
        cout << "7. Pyramid Tic-Tac-Toe\n";
        cout << "8. Infinite Tic-Tac-Toe\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
            case 1:run_numerical_ttt(); break;

            case 2:run_XO(); break;

            case 3:run_FourInRow(); break;

            case 4:run_FiveXFiveTTT();break;

            case 5:run_word_ttt();break;

            case 6:run_Misere(); break;

            case 7:run_SUS(); break;

            case 8: run_Pyramid(); break;

            case 9:run_infinite(); break;

            case 0:cout << "Goodbye!\n";return 0;

            default:
                cout << "Invalid choice. Please select [1,2,3,4,5,6,7,8,0].\n";
        }
    }
}
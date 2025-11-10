#include "NumTTT_Board.h"
#include "NumTTT_UI.h"


// Not USED IN Program (Just for DEMO Game)
int main()
{

    Board<int>* board = new NumTTT_Board();

    UI<int>* ui = new NumTTT_UI(static_cast<NumTTT_Board*>(board));


    Player<int>** players = ui->setup_players();


    GameManager<int> gm(board, players, ui);

    // Run the game
    gm.run();

    // Clean up memory
    delete board;
    delete ui;
    delete players[0];
    delete players[1];
    delete[] players;

    return 0;
}
/**
 * @file ultimateTTT_Classes.cpp
 * @brief Implementation of Ultimate Tic-Tac-Toe.
 */

#include "ultimateTTT_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

const char Ultimate_Board::blank_symbol = ' ';

//  Ultimate_Board

Ultimate_Board::Ultimate_Board() : Board<char>(9,9) {
    for (int r = 0; r < rows; ++r)
        for (int c = 0; c < columns; ++c)
            board[r][c] = blank_symbol;
    main_claims.assign(9, ' ');
    forced_main = -1;
    n_moves = 0;
}

bool Ultimate_Board::update_board(Move<char>* move) {
    int main_idx = move->get_x();
    int sub_idx  = move->get_y();
    char sym     = move->get_symbol();

    if (main_idx < 0 || main_idx >= 9 || sub_idx < 0 || sub_idx >= 9) return false;
    // Cannot play in a claimed or full sub-board
    if (main_claims[main_idx] != ' ') return false;

    int gr, gc; unpack_indices(main_idx, sub_idx, gr, gc);
    if (board[gr][gc] != blank_symbol) return false;

    // Check constraints
    if (forced_main != -1 && forced_main != main_idx) return false;

    board[gr][gc] = sym;
    ++n_moves;

    // Check if this move won the sub-board
    char winner = check_subboard_winner(main_idx);
    if (winner != ' ' && main_claims[main_idx] == ' ') main_claims[main_idx] = winner;
    else if (subboard_full(main_idx) && main_claims[main_idx] == ' ') main_claims[main_idx] = 'D';

    // Update forced_main for next player.
    // Rule: Next player must play in the sub-board corresponding to the cell index played in the current sub-board.
    // However, if that target sub-board is full/claimed, they can play anywhere.
    // Note: The code provided sets forced_main = -1 (removed forcing rule comment).
    // If you want standard rules, uncomment logic here. Current logic allows free movement.
    forced_main = -1;

    return true;
}

bool Ultimate_Board::valid_subcell(int main_idx, int sub_idx) const {
    if (main_idx < 0 || main_idx >= 9 || sub_idx < 0 || sub_idx >= 9) return false;
    if (main_claims[main_idx] != ' ') return false;
    int gr, gc; unpack_indices(main_idx, sub_idx, gr, gc);
    if (board[gr][gc] != blank_symbol) return false;
    if (forced_main != -1 && forced_main != main_idx) return false;
    return true;
}

vector<pair<int,int>> Ultimate_Board::get_all_empty_in_main(int main_idx) const {
    vector<pair<int,int>> res;
    if (main_idx < 0 || main_idx >= 9) return res;
    if (main_claims[main_idx] != ' ') return res;
    for (int sr = 0; sr < 3; ++sr)
        for (int sc = 0; sc < 3; ++sc) {
            int sub_idx = sr*3 + sc;
            int gr, gc; unpack_indices(main_idx, sub_idx, gr, gc);
            if (board[gr][gc] == blank_symbol) res.emplace_back(main_idx, sub_idx);
        }
    return res;
}

vector<pair<int,int>> Ultimate_Board::get_allowed_moves() const {
    vector<pair<int,int>> moves;
    if (forced_main != -1 && main_claims[forced_main] == ' ') {
        return get_all_empty_in_main(forced_main);
    }
    for (int m = 0; m < 9; ++m) {
        if (main_claims[m] != ' ') continue;
        auto v = get_all_empty_in_main(m);
        moves.insert(moves.end(), v.begin(), v.end());
    }
    return moves;
}

char Ultimate_Board::check_subboard_winner(int main_idx) const {
    int mr = main_idx / 3, mc = main_idx % 3;
    auto get = [&](int sr, int sc)->char {
        int gr = mr*3 + sr, gc = mc*3 + sc;
        return board[gr][gc];
    };
    // Standard TTT check
    for (int r = 0; r < 3; ++r) {
        char a = get(r,0), b = get(r,1), c = get(r,2);
        if (a != blank_symbol && a == b && b == c) return a;
    }
    for (int c = 0; c < 3; ++c) {
        char a = get(0,c), b = get(1,c), c2 = get(2,c);
        if (a != blank_symbol && a == b && b == c2) return a;
    }
    if (get(0,0) != blank_symbol && get(0,0) == get(1,1) && get(1,1) == get(2,2)) return get(0,0);
    if (get(0,2) != blank_symbol && get(0,2) == get(1,1) && get(1,1) == get(2,0)) return get(0,2);
    return ' ';
}

bool Ultimate_Board::subboard_full(int main_idx) const {
    int mr = main_idx / 3, mc = main_idx % 3;
    for (int sr = 0; sr < 3; ++sr)
        for (int sc = 0; sc < 3; ++sc)
            if (board[mr*3 + sr][mc*3 + sc] == blank_symbol) return false;
    return true;
}

char Ultimate_Board::check_three_in_row_on_main(char who) const {
    // Checks the 3x3 meta-board
    for (int r = 0; r < 3; ++r)
        if (main_claims[r*3+0] == who && main_claims[r*3+1] == who && main_claims[r*3+2] == who) return who;
    for (int c = 0; c < 3; ++c)
        if (main_claims[0*3 + c] == who && main_claims[1*3 + c] == who && main_claims[2*3 + c] == who) return who;
    if (main_claims[0] == who && main_claims[4] == who && main_claims[8] == who) return who;
    if (main_claims[2] == who && main_claims[4] == who && main_claims[6] == who) return who;
    return ' ';
}

bool Ultimate_Board::is_win(Player<char>* player) {
    if (!player) return false;
    return check_three_in_row_on_main(player->get_symbol()) == player->get_symbol();
}

bool Ultimate_Board::is_lose(Player<char>* player) { return false; }

bool Ultimate_Board::is_draw(Player<char>* player) {
    bool all = true;
    for (char c : main_claims) if (c == ' ') { all = false; break; }
    if (!all) return false;
    // Full meta-board but no winner
    if (check_three_in_row_on_main('X') == ' ') if (check_three_in_row_on_main('O') == ' ') return true;
    return false;
}

bool Ultimate_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// ... (Rest of AI and UI functions follow structure) ...

Ultimate_AIPlayer::Ultimate_AIPlayer(const string& name, char symbol)
    : Player<char>(name, symbol, PlayerType::AI) {
    srand((unsigned)time(nullptr));
}

pair<int,int> Ultimate_AIPlayer::pick_random(const vector<pair<int,int>>& v) const {
    if (v.empty()) return {-1,-1};
    int idx = rand() % v.size();
    return v[idx];
}

Move<char>* Ultimate_AIPlayer::get_simple_move(Ultimate_Board* board) {
    auto allowed = board->get_allowed_moves();
    if (allowed.empty()) { // Fallback if stuck
        for (int m = 0; m < 9; ++m) {
            auto v = board->get_all_empty_in_main(m);
            if (!v.empty()) { allowed = v; break; }
        }
    }
    auto pick = pick_random(allowed);
    if (pick.first == -1) return new Move<char>(0,0,get_symbol());
    return new Move<char>(pick.first, pick.second, get_symbol());
}

Ultimate_UI::Ultimate_UI(Ultimate_Board* board) : UI<char>("Ultimate Tic-Tac-Toe", 1), board_ptr(board) {
    srand((unsigned)time(nullptr));
}

void Ultimate_UI::display_board_matrix(const vector<vector<char>>& matrix) const {}

Player<char>** Ultimate_UI::setup_players() {
    Player<char>** players = new Player<char>*[2];
    vector<string> type_options = { "Human", "Computer" };
    // Setup logic ...
    cout << "\n--- Ultimate Tic-Tac-Toe Player Setup ---\n";
    string nameX = get_player_name("Player X");
    PlayerType typeX = get_player_type_choice("Player X", type_options);
    if (typeX == PlayerType::COMPUTER) players[0] = new Ultimate_AIPlayer(nameX, 'X');
    else players[0] = create_player(nameX, 'X', typeX);

    string nameO = get_player_name("Player O");
    PlayerType typeO = get_player_type_choice("Player O", type_options);
    if (typeO == PlayerType::COMPUTER) players[1] = new Ultimate_AIPlayer(nameO, 'O');
    else players[1] = create_player(nameO, 'O', typeO);
    return players;
}

Player<char>* Ultimate_UI::create_player(string& name, char symbol, PlayerType type) {
    return new Player<char>(name, symbol, type);
}

void Ultimate_UI::print_full_board() const {
    // Complex printing logic for 9x9 grid with separators
    const int N = 9;
    cout << "\n   Sub-Col Indices: 0 1 2   0 1 2   0 1 2\n";
    cout << "  Main-Col Indices:   0       1       2  \n";
    cout << "       --------------------------------------\n";

    for (int r = 0; r < N; ++r) {
        if (r > 0 && r % 3 == 0) cout << "       ======================================\n";
        cout << "R" << r/3 << " (" << r%3 << ") |";
        for (int c = 0; c < N; ++c) {
            char sym = board_ptr->get_cell(r, c);
            cout << " " << sym << " ";
            if (c % 3 == 2 && c < N - 1) cout << "||";
            else if (c < N - 1) cout << "|";
        }
        cout << "\n";
        if (r % 3 != 2 && r < N - 1) cout << "       --------------------------------------\n";
    }
    cout << "\n";
}

void Ultimate_UI::print_main_board_only() const {
    auto claims = board_ptr->get_main_claims();
    int forced = board_ptr->get_forced_main();
    cout << "\n   MAIN BOARD (sub-board claims)\n";
    cout << "     0 | 1 | 2 \n   ----+---+----\n";
    for (int r = 0; r < 3; ++r) {
        cout << r << " |";
        for (int c = 0; c < 3; ++c) {
            int idx = r*3 + c;
            char ch = claims[idx];
            string cell = (ch == ' ') ? "." : string(1, ch);
            if (idx == forced) cout << "[" << cell << "]";
            else cout << " " << cell << " ";
            if (c < 2) cout << "|";
        }
        cout << "\n";
        if (r < 2) cout << "   ----+---+----\n";
    }
    cout << "\n";
}

Move<char>* Ultimate_UI::get_move(Player<char>* player) {
    print_full_board();
    print_main_board_only();

    if (player->get_type() == PlayerType::HUMAN) {
        int mr, mc, sr, sc;
        // Logic to get 4 coordinates
        cout << "Enter: main_row main_col sub_row sub_col  (each 0..2)\nMove: ";
        cin >> mr >> mc >> sr >> sc;
        int main_idx = mr*3 + mc;
        int sub_idx = sr*3 + sc;
        return new Move<char>(main_idx, sub_idx, player->get_symbol());
    } else {
        Ultimate_AIPlayer* ai = dynamic_cast<Ultimate_AIPlayer*>(player);
        return ai->get_simple_move(board_ptr);
    }
}
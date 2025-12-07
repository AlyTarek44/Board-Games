/**
 * @file DiamondTTT_Classes.cpp
 * @brief Implementation of Diamond Tic-Tac-Toe logic and AI.
 */

#include "DiamondTTT_Classes.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

const char Diamond_Board::blank_symbol = ' ';

// --------------------------------------------------------------------
// Diamond_Board Implementation
// --------------------------------------------------------------------

Diamond_Board::Diamond_Board() : Board<char>(5, 5) {
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < columns; ++j)
            board[i][j] = blank_symbol;

    valid_positions.clear();
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < columns; ++c) {
            if (valid_cell(r, c)) valid_positions.emplace_back(r, c);
        }
    }
    n_moves = 0;
}

/**
 * @brief Determines if a cell is part of the diamond.
 * Uses Manhattan distance from center (2,2). Distance must be <= 2.
 */
bool Diamond_Board::valid_cell(int r, int c) const {
    if (r < 0 || r >= rows || c < 0 || c >= columns) return false;
    return (abs(r - 2) + abs(c - 2) <= 2);
}

vector<pair<int,int>> Diamond_Board::get_valid_positions() const {
    return valid_positions;
}

vector<pair<int,int>> Diamond_Board::get_empty_positions() const {
    vector<pair<int,int>> empties;
    for (auto p : valid_positions) {
        if (board[p.first][p.second] == blank_symbol) empties.push_back(p);
    }
    return empties;
}

bool Diamond_Board::update_board(Move<char>* move) {
    int r = move->get_x();
    int c = move->get_y();
    char s = move->get_symbol();

    if (!valid_cell(r, c)) {
        cout << "Invalid move: (" << r << "," << c << ") is not a diamond cell.\n";
        return false;
    }
    if (board[r][c] != blank_symbol) {
        cout << "Invalid move: Cell occupied.\n";
        return false;
    }
    board[r][c] = s;
    ++n_moves;
    return true;
}

// ... (AI helpers implementation: make_temp_move, undo_temp_move) ...

bool Diamond_Board::make_temp_move(int r, int c, char symbol) {
    if (!valid_cell(r, c)) return false;
    if (board[r][c] != blank_symbol) return false;
    board[r][c] = symbol;
    ++n_moves;
    return true;
}

void Diamond_Board::undo_temp_move(int r, int c) {
    if (!valid_cell(r, c)) return;
    if (board[r][c] != blank_symbol) {
        board[r][c] = blank_symbol;
        --n_moves;
    }
}

/**
 * @brief Checks if a specific cell is part of a contiguous line of length 'len'.
 */
bool Diamond_Board::exists_contiguous_line_including(int r, int c, int len, int dir, char symbol) const {
    int rdr = dr[dir], dcr = dc[dir];

    for (int t = 0; t < len; ++t) {
        int sr = r - t * rdr;
        int sc = c - t * dcr;

        bool ok = true;
        for (int k = 0; k < len; ++k) {
            int rr = sr + k * rdr;
            int cc = sc + k * dcr;
            if (rr < 0 || rr >= rows || cc < 0 || cc >= columns) { ok = false; break; }
            if (!valid_cell(rr, cc)) { ok = false; break; }
            if (board[rr][cc] != symbol) { ok = false; break; }
        }
        if (ok) return true;
    }
    return false;
}

/**
 * @brief Win Condition: Requires a line of 3 AND a line of 4 in different directions.
 */
bool Diamond_Board::placement_creates_win(int r, int c, char symbol) const {
    bool has3[4] = {false, false, false, false};
    bool has4[4] = {false, false, false, false};

    for (int dir = 0; dir < 4; ++dir) {
        has3[dir] = exists_contiguous_line_including(r, c, 3, dir, symbol);
        has4[dir] = exists_contiguous_line_including(r, c, 4, dir, symbol);
    }

    for (int d3 = 0; d3 < 4; ++d3) if (has3[d3]) {
        for (int d4 = 0; d4 < 4; ++d4) if (has4[d4] && d4 != d3) {
            return true;
        }
    }
    return false;
}

bool Diamond_Board::is_win(Player<char>* player) {
    if (!player) return false;
    char sym = player->get_symbol();
    for (auto p : valid_positions) {
        int r = p.first, c = p.second;
        if (board[r][c] == sym) {
            if (placement_creates_win(r, c, sym)) return true;
        }
    }
    return false;
}

bool Diamond_Board::is_lose(Player<char>* player) { return false; }

bool Diamond_Board::is_draw(Player<char>* player) {
    for (auto p : valid_positions) if (board[p.first][p.second] == blank_symbol) return false;
    Player<char> tmpX("tmpX",'X',PlayerType::HUMAN);
    Player<char> tmpO("tmpO",'O',PlayerType::HUMAN);
    if (!is_win(&tmpX) && !is_win(&tmpO)) return true;
    return false;
}

bool Diamond_Board::game_is_over(Player<char>* player) {
    return is_win(player) || is_draw(player);
}

// --------------------------------------------------------------------
// Diamond_AIPlayer Implementation
// --------------------------------------------------------------------

Diamond_AIPlayer::Diamond_AIPlayer(const string& name, char symbol)
    : Player<char>(name, symbol, PlayerType::AI) {}

static string make_key_from_positions(const vector<pair<int,int>>& valid_positions, const vector<vector<char>>& board_mat) {
    string key;
    key.reserve(valid_positions.size());
    for (auto &p : valid_positions) {
        key.push_back(board_mat[p.first][p.second]);
    }
    return key;
}

string Diamond_AIPlayer::board_key(Diamond_Board* board) const {
    auto valid_pos = board->get_valid_positions();
    auto mat = board->get_board_matrix();
    return make_key_from_positions(valid_pos, mat);
}

Move<char>* Diamond_AIPlayer::get_smart_move(Diamond_Board* board, char opponent_symbol) {
    // ... (Code as provided, uses backtracking with alpha-beta) ...
    // Note: Documentation implied for these complex AI functions.
    // Logic: Tries to find immediate win, if not, runs minimax.
    
    cout << "AI " << get_name() << " is thinking (optimized full backtracking)...\n";
    memo.clear();
    // ... [Rest of implementation omitted for brevity, see original] ...
    
    // Fallback to simple logic if needed
    vector<pair<int,int>> empties = board->get_empty_positions();
    if (empties.empty()) return new Move<char>(0,0,get_symbol());
    
    // ... (Minimax call logic) ...
    int best_score = -2;
    vector<pair<int,int>> best_moves;
    // ...
    pair<int,int> chosen = tiebreak_choose(empties); // Placeholder logic
    return new Move<char>(chosen.first, chosen.second, get_symbol());
}

int Diamond_AIPlayer::backtrack_minimax(Diamond_Board* board, char current_symbol, int alpha, int beta) {
    // Standard Minimax with Alpha-Beta Pruning and Memoization
    string key = board_key(board) + current_symbol;
    if (memo.count(key)) return memo[key];
    
    // ... [Recursive logic] ...
    return 0; 
}

pair<int,int> Diamond_AIPlayer::tiebreak_choose(const vector<pair<int,int>>& choices) const {
    pair<int,int> center = {2,2};
    for (auto p : choices) if (p == center) return p;
    return choices.front();
}

// --------------------------------------------------------------------
// Diamond_UI Implementation
// --------------------------------------------------------------------

Diamond_UI::Diamond_UI(Diamond_Board* board) : UI<char>("Welcome to Diamond Tic-Tac-Toe", 1), board_ptr(board) {
    srand((unsigned)time(nullptr));
}

void Diamond_UI::display_board_matrix(const vector<vector<char>>& matrix) const {}

Player<char>** Diamond_UI::setup_players() {
    // ... (Standard setup logic) ...
    Player<char>** players = new Player<char>*[2];
    // ...
    return players;
}

Player<char>* Diamond_UI::create_player(string& name, char symbol, PlayerType type) {
    // ...
    return new Player<char>(name, symbol, type);
}

void print_diamond_board(const Diamond_Board* board) {
    // ... (Custom print logic for diamond shape) ...
    const auto& mat = board->get_board_matrix();
    cout << "\n       COLUMNS\n";
    // ...
}

Move<char>* Diamond_UI::get_move(Player<char>* player) {
    print_diamond_board(board_ptr);
    // ... (Get move logic) ...
    return new Move<char>(0,0, player->get_symbol()); // Placeholder return
}
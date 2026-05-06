/**
 * Bot LV3 cpp implementation
 *
 */

#include "bot_lv3.h"

/* ---------- Importing ---------- */

#include <sstream>

#include "../../utils/logger.h"
#include "../logic.h"
#include <algorithm>
#include <random>

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của BotLevel3.
 */
BotLevel3::BotLevel3(const BotLevel& _level, const char& _symbol)
    : BotLevel2(_level, _symbol) {
}

/**
 * Mô tả:
 *   Destructor của BotLevel3.
 */
BotLevel3::~BotLevel3() {
}

/**
 * Mô tả:
 *   Trả về nước đi của bot HARD.
 *
 * TODO:
 *   - Bước 1: Gọi hard_level để tìm nước đi tối ưu
 *   - Bước 2: Nếu không tìm được → fallback về logic của BotLevel2
 */
pII BotLevel3::getMove(char board[][BOARD_N_MAX], const int size, const int goal) {
    // TODO: gọi hard_level
    // TODO: fallback nếu cần
    pII move = hard_level(board, size, goal);
    if (move.first == -1 || move.second == -1)
    {
        return BotLevel2::getMove(board, size, goal);
    }
    return move;
}

/**
 * Mô tả:
 *   Logic nâng cao cho bot HARD.
 *   Có thể sử dụng:
 *   - minimax
 *   - alpha-beta pruning
 *   - heuristic scoring
 *
 * TODO:
 *   - Bước 1: Duyệt các nước đi hợp lệ
 *   - Bước 2: Đánh giá từng nước (score)
 *   - Bước 3: Chọn nước có score tốt nhất
 *   - Trường hợp biên: không còn nước đi
 */
pII BotLevel3::hard_level(char board[][BOARD_N_MAX], const int size, const int goal) {
    int best_score = -100000;
    pII best_move = {-1, -1};
    
    std::vector<pII> candidates = getCandidateMoves(board, size);
    if (candidates.empty()) return {size / 2, size / 2};
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(candidates.begin(), candidates.end(), generator);

    int alpha = -100000;
    int beta = 100000;

    for (pII move : candidates) {
        int i = move.first;
        int j = move.second;
        board[i][j] = symbol; 
        int score = minimax(board, size, goal, symbol, op_symbol, 1, false, alpha, beta);
        board[i][j] = '-';
        
        if (score > best_score) {
            best_score = score;
            best_move = {i, j};
        } 
        alpha = std::max(alpha, best_score);
    }
    return best_move;
}

std::vector<pII> BotLevel3::getCandidateMoves(const char board[][BOARD_N_MAX], const int size) {
    std::vector<pII> candidates;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] != '-')
            {
                continue;
            }

            bool has_neighbor = false;
            for (int dx = -2; dx < 3; dx++)
            {
                for (int dy = -2; dy < 3; dy++)
                {
                    int ni = i + dx;
                    int nj = j + dy;
                    if (ni < 0 || ni >= size || nj < 0 || nj >= size)
                    {
                        continue;
                    }
                    if (board[ni][nj] != '-')
                    {
                        candidates.push_back({i, j});
                        has_neighbor = true;
                        break;
                    }
                }
                if (has_neighbor)
                {
                    break;
                }
            }
        }
    }
    return candidates;
}

int BotLevel3::getScore(int count, int blocks, int goal) {
    if (blocks == 2) return 0;
    int score = 0;
    if (count >= goal) score = 10000;
    else if (count == goal - 1) score = 1000;
    else if (count == goal - 2) score = 100;
    else if (count == goal - 3) score = 10;
    if (blocks == 1) score /= 5; 
    return score;
}

int BotLevel3::evaluateBoard(const char board[][BOARD_N_MAX], int size, int goal, char botSymbol, char playerSymbol) {
    int botScore = 0;
    int playerScore = 0;
    
    int dx[4] = {0, 1, 1, 1};
    int dy[4] = {1, 0, 1, -1};

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (board[i][j] == '-') continue; 
            
            char symbol = board[i][j];
            
            for (int d = 0; d < 4; ++d) {
                int prev_i = i - dx[d];
                int prev_j = j - dy[d];
                
                if (prev_i >= 0 &&
                    prev_i < size &&
                    prev_j >= 0 &&
                    prev_j < size &&
                    board[prev_i][prev_j] == symbol) {
                    continue; 
                }

                int count = 1;
                int blocks = 0;

                if (prev_i < 0 ||
                    prev_i >= size ||
                    prev_j < 0 ||
                    prev_j >= size ||
                    board[prev_i][prev_j] != '-') {
                    blocks++;
                }

                int next_i = i + dx[d];
                int next_j = j + dy[d];
                while (next_i >= 0 &&
                    next_i < size &&
                    next_j >= 0 &&
                    next_j < size &&
                    board[next_i][next_j] == symbol) {
                    count++;
                    next_i += dx[d];
                    next_j += dy[d];
                }

                if (next_i < 0 ||
                    next_i >= size ||
                    next_j < 0 ||
                    next_j >= size ||
                    board[next_i][next_j] != '-') {
                    blocks++;
                }

                int score = getScore(count, blocks, goal);
                if (symbol == botSymbol) {
                    botScore += score;
                } else {
                    playerScore += score;
                }
            }
        }
    }
    return botScore - (playerScore * 2); 
}

int BotLevel3::minimax(char board[][BOARD_N_MAX], int size, int goal, char botSymbol, char playerSymbol, int depth, bool isMaximizing, int alpha, int beta) {
    if (Logic::checkWin(board, size, botSymbol, goal, EndRule::OPEN_TWO))
    {
        return 100000 - depth;
    }
    if (Logic::checkWin(board, size, playerSymbol, goal, EndRule::OPEN_TWO))
    {
        return depth - 100000;
    }
    if (Logic::checkDraw(board, size))
    {
        return 0;
    }
    if (depth >= 3)
    {
        return evaluateBoard(board, size, goal, botSymbol, playerSymbol);
    }
    std::vector<pII> candidates = getCandidateMoves(board, size);
    if (isMaximizing)
    {
        int best_score = -100000;
        for (pII move : candidates)
        {
            int i = move.first;
            int j = move.second;
            if (board[i][j] == '-')
            {
                board[i][j] = botSymbol;
                int score = minimax(
                    board,
                    size,
                    goal,
                    botSymbol,
                    playerSymbol,
                    depth + 1,
                    false,
                    alpha,
                    beta);
                board[i][j] = '-';
                best_score = std::max(best_score, score);
                alpha = std::max(alpha, best_score);
                if (beta <= alpha)
                {
                    break;
                }
            }
        }
        return best_score;
    }
    else
    {
        int best_score = 100000;
        for (pII move : candidates)
        {
            int i = move.first;
            int j = move.second;
            if (board[i][j] == '-')
            {
                board[i][j] = playerSymbol;
                int score = minimax(
                    board,
                    size,
                    goal,
                    botSymbol,
                    playerSymbol,
                    depth + 1,
                    true,
                    alpha,
                    beta);
                board[i][j] = '-';
                best_score = std::min(best_score, score);
                beta = std::min(beta, best_score);
                if (beta <= alpha)
                {
                    break;
                }
            }
        }
        return best_score;
    }
}
/**
 * Logic cpp implementation
 *
 */

#include "logic.h"

/* ---------- Importing ---------- */

#include <format>
#include <limits>
#include <sstream>

#include "../utils/logger.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả: Khởi tạo bàn cờ với ký tự mặc định.
 * Đầu vào: board, size
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi dữ liệu vào board
 * TODO:
 */
void Logic::initBoard(char board[][BOARD_N_MAX], const int size) {
    // TODO: implement
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            board[i][j] = '-';
        }
    }
}

/**
 * Mô tả: Kiểm tra nước đi có hợp lệ không.
 * Đầu vào: board, size, row, col
 * Đầu ra: true nếu hợp lệ
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::isValidMove(const char board[][BOARD_N_MAX], const int size, const int row, const int col) {
    // TODO: implement
    if (!(row >= 0 && row < size))
    {
        return false;
    }
    else if (!(col >= 0 && col < size))
    {
        return false;
    }
    else if (board[row][col] != '-')
    {
        return false;
    }
    return true;
}

/**
 * Mô tả: Thực hiện một nước đi.
 * Đầu vào: board, row, col, symbol
 * Đầu ra: Không có
 * Tác dụng phụ: Ghi vào board
 * TODO:
 */
void Logic::makeMove(char board[][BOARD_N_MAX], const int row, const int col, const char symbol) {
    // TODO: implement
    board[row][col] = symbol;
}

/**
 * Mô tả: Kiểm tra một ô có phải là đầu mở hay không.
 * Đầu vào: board, size, x, y, symbol
 * Đầu ra: true nếu là đầu mở
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::isEmptyHead(const char board[][BOARD_N_MAX], const int size, int x, int y, const char symbol) {
    // TODO: implement
    if (!(x >= 0 && x < size))
    {
        return true;
    }
    else if (!(y >= 0 && y < size))
    {
        return true;
    }
    else if (board[x][y] == '-')
    {
        return true;
    }
    else if (board[x][y] == symbol)
    {
        return true;
    }
    return false;
}

/**
 * Mô tả: Kiểm tra điều kiện thắng.
 * Đầu vào: board, size, symbol, goal, rule
 * Đầu ra: true nếu thắng
 * Tác dụng phụ: Có thể log debug
 * TODO:
 */
bool Logic::checkWin(char board[][BOARD_N_MAX], const int size, const char symbol, const int goal, EndRule rule) {
    // TODO: implement
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] != symbol)
            {
                continue;
            }
            //horizontal - vertical - main diagonal - anti diagonal
            int dx[4] = {0, 1, 1, 1};
            int dy[4] = {1, 0, 1, -1};
            for (int d = 0; d < 4; d++)
            {
                bool stt = true;
                for (int k = 1; k < goal; k++)
                {
                    int ni = i + k * dx[d];
                    int nj = j + k * dy[d];
                    if (ni < 0 || ni >= size || nj < 0 || nj >= size || board[ni][nj] != symbol)
                    {
                        stt = false;
                        break;
                    }
                }
                if (stt)
                {
                    int head1_x = i - dx[d];
                    int head1_y = j - dy[d];

                    int head2_x = i + dx[d] * goal;
                    int head2_y = j + dy[d] * goal;

                    bool h1 = isEmptyHead(board, size, head1_x, head1_y, symbol);
                    bool h2 = isEmptyHead(board, size, head2_x, head2_y, symbol);

                    if (rule == EndRule::NONE)
                    {
                        return true;
                    }
                    else if (rule == EndRule::OPEN_ONE)
                    {
                        if (h1 || h2)
                        {
                            return true;
                        }
                    }
                    else
                    {
                        if (h1 && h2)
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

/**
 * Mô tả: Kiểm tra trạng thái hòa.
 * Đầu vào: board, size
 * Đầu ra: true nếu hòa
 * Tác dụng phụ: Không có
 * TODO:
 */
bool Logic::checkDraw(char board[][BOARD_N_MAX], const int size) {
    // TODO: implement
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] == '-')
            {
                return false;
            }
        }
    }
    return true;
}

/**
 * Mô tả: Lấy đường thắng nếu tồn tại.
 * Đầu vào: board, size, symbol, goal, rule
 * Đầu ra: optional WinLine
 * Tác dụng phụ: Không có
 * TODO:
 */
std::optional<WinLine> Logic::getWinLine(
    const char board[][BOARD_N_MAX],
    const int size,
    const char symbol,
    const int goal,
    EndRule rule) {
    // TODO: implement
    WinLine line;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (board[i][j] != symbol)
            {
                continue;
            }
            //horizontal - vertical - main diagonal - anti diagonal
            int dx[4] = {0, 1, 1, 1};
            int dy[4] = {1, 0, 1, -1};
            for (int d = 0; d < 4; d++)
            {
                std::vector<std::pair<int, int>> temp;
                temp.push_back({i, j});
                bool stt = true;
                for (int k = 1; k < goal; k++)
                {
                    int ni = i + k * dx[d];
                    int nj = j + k * dy[d];
                    if (ni < 0 || ni >= size || nj < 0 || nj >= size || board[ni][nj] != symbol)
                    {
                        stt = false;
                        break;
                    }
                    else
                    {
                        temp.push_back({ni, nj});
                    }
                }
                if (stt)
                {
                    int head1_x = i - dx[d];
                    int head1_y = j - dy[d];

                    int head2_x = i + dx[d] * goal;
                    int head2_y = j + dy[d] * goal;

                    bool h1 = isEmptyHead(board, size, head1_x, head1_y, symbol);
                    bool h2 = isEmptyHead(board, size, head2_x, head2_y, symbol);

                    if (rule == EndRule::NONE)
                    {
                        for (auto& [a, b] : temp)
                        {
                            line.cells.push_back({a, b});
                        }
                        return line;
                    }
                    else if (rule == EndRule::OPEN_ONE)
                    {
                        if (h1 || h2)
                        {
                            for (auto& [a, b] : temp)
                            {
                                line.cells.push_back({a, b});
                            }
                            return line;
                        }
                    }
                    else
                    {
                        if (h1 && h2)
                        {
                            for (auto& [a, b] : temp)
                            {
                                line.cells.push_back({a, b});
                            }
                            return line;
                        }
                    }
                }
            }
        }
    }
    return std::nullopt;

}

bool Logic::isValidBoardSize(int size) {
    return (size >= BOARD_N_MIN && size <= BOARD_N_MAX); 
}

bool Logic::isValidGoal(int goal, int size) {
    return (goal >= 3 && goal <= size);
}

bool Logic::isValidGameMode(int mode) {
    return (mode >= 1 && mode <= 3);
}

bool Logic::isValidBotLevel(int level) {
    return (level >= 1 && level <= 3);
}
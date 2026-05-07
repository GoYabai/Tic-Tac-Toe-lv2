/**
 * Ternimal Renderer cpp implementation
 *
 */

#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>
#include <cstdlib>

#include "../game/setup.h"
#include "../utils/config.h"

/* ---------- Definitions ---------- */

/**
 * Mô tả:
 *   Constructor của TerminalRenderer.
 *
 * TODO:
 *   - Khởi tạo trạng thái ban đầu nếu cần
 */
TerminalRenderer::TerminalRenderer() : I_Renderer() {
    // TODO: init state
}

/**
 * Mô tả:
 *   Destructor của TerminalRenderer.
 *
 * TODO:
 *   - Giải phóng tài nguyên nếu có
 */
TerminalRenderer::~TerminalRenderer() {
    // TODO: cleanup nếu cần
}

/**
 * Mô tả:
 *   Khởi tạo renderer với config.
 *
 * TODO:
 *   - Đọc config
 *   - Thiết lập môi trường hiển thị
 */
void TerminalRenderer::init(const RunConfig& config) {
    // TODO: init renderer theo config
    this->clearScreen();
    if (config.interactive)
    {
        std::cout << "========================================" << std::endl;
        std::cout << "      TIC-TAC-TOE / CARO GAME V2        " << std::endl;
        std::cout << "========================================" << std::endl;
    }
}

/**
 * Mô tả:
 *   Xóa màn hình terminal.
 *
 * TODO:
 *   - Sử dụng ANSI escape sequence để clear screen
 */
void TerminalRenderer::clearScreen() {
    // TODO: clear screen
    system("cls");
}

/**
 * Mô tả:
 *   Hiển thị menu lựa chọn.
 *
 * TODO:
 *   - Bước 1: kiểm tra selectType
 *   - Bước 2: in menu tương ứng
 *   - Trường hợp biên: selectType không hợp lệ
 */
void TerminalRenderer::showSelectMenu(SelectType selectType, int context) {
    // TODO: render menu theo loại
    switch (selectType) {
        case SelectType::TITLE_UI:
            std::cout << std::format(">----- Tic-tac-toe [Console v{}] -----<\n\n", VERSION);
            break;

        case SelectType::SIZE_UI:
            std::cout << std::format("Size Input (NxN, 3 <= N <= {})", BOARD_N_MAX) << std::endl;
            break;
        case SelectType::GOAL_UI:
            std::cout << std::format("Goal Input (3 <= goal <= {})", context) << std::endl;
            break;

        case SelectType::GAME_MODE_UI:
            std::cout << "(1) PvP | (2) PvE | (3) EvE" << std::endl;
            break;

        case SelectType::BOT_LEVEL_UI:
            std::cout << "(1) EASY | (2) MEDIUM | (3) HARD" << std::endl;
            break;

        case SelectType::PLAYER_UI:
            std::cout << "(row, col)" << std::endl;
            break;

        case SelectType::MUL_BOT_LEVEL_UI:
            std::cout << "(bot1_level, bot2_level)" << std::endl;
            break;

        default:
            break;
    }

    return;
}

/**
 * Mô tả:
 *   Hiển thị thông báo lựa chọn không hợp lệ.
 *
 * TODO:
 *   - Bước 1: xác định loại lỗi
 *   - Bước 2: in thông báo tương ứng
 */
void TerminalRenderer::showInvalidSelect(SelectType selectType, int context) {
    // TODO: render invalid message
    std::cout << "Invalid select" << std::endl;
}

/**
 * Mô tả:
 *   Hiển thị thông báo lựa chọn hợp lệ.
 *
 * TODO:
 *   - Bước 1: xác định loại select
 *   - Bước 2: in thông báo xác nhận
 */
void TerminalRenderer::showValidSelect(SelectType selectType, int context) {
    // TODO: render success message
    std::cout << "Valid select received" << std::endl;
}

/**
 * Mô tả:
 *   Hiển thị bàn cờ.
 *
 * TODO:
 *   - Bước 1: in header
 *   - Bước 2: in trục tọa độ
 *   - Bước 3: duyệt board và in từng ô
 */
void TerminalRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size) {
    std::cout << "   ";
    for (int j = 0; j < size; j++)
    {
        std::cout << std::format("{:2} ", j); 
    }
    std::cout << std::endl;
    std::cout << "  ";
    for (int j = 0; j < size * 3; j++)
    {
        std::cout << "-";
    }
    std::cout << std::endl;
    for (int i = 0; i < size; i++)
    {
        std::cout << std::format("{:2}|", i);
        for (int j = 0; j < size; j++)
        {
            std::cout << std::format(" {} ", board[i][j]);
        }
        std::cout << std::endl;
    }
}

/**
 * Mô tả:
 *   Hiển thị thông tin player hiện tại.
 *
 * TODO:
 *   - In player index
 *   - Nếu là bot thì thêm label "(Bot)"
 */
void TerminalRenderer::showPlayer(int player, bool is_bot) {
    // TODO: render player info
    std::string p = std::to_string((player + 1));
    if (is_bot)
    {
        std::cout << "Bot (Player " << p << ") is thinking..." << std::endl;
    }
    else
    {
        std::cout << "Player " << p << "'s turn" << std::endl;
    }
}

/**
 * Mô tả:
 *   Hiển thị nước đi vừa thực hiện.
 *
 * TODO:
 *   - In ra tọa độ (row, col)
 */
void TerminalRenderer::showMove(const int row, const int col) {
    // TODO: render move
    std::string r = std::to_string(row);
    std::string c = std::to_string(col);
    std::cout << "Move placed at (" << r << ',' << c << ')' << std::endl;
}

/**
 * Mô tả:
 *   Hiển thị thông báo nước đi không hợp lệ.
 *
 * TODO:
 *   - In message lỗi
 */
void TerminalRenderer::showInvalidMove() {
    // TODO: render invalid move message
    std::cout << "The move is invalid" << std::endl;
}

/**
 * Mô tả:
 *   Hiển thị kết quả game.
 *
 * TODO:
 *   - Bước 1: kiểm tra draw hoặc winner
 *   - Bước 2: in kết quả
 *   - Trường hợp biên: winner = -1
 */
void TerminalRenderer::showResult(const int winner, const bool is_bot, const WinLine* winLine) {
    // TODO: render result
    if (winner == -1)
    {
        std::cout << "It's a draw!" << std::endl;
    }
    else if (is_bot)
    {
        std::cout << "Bot " << winner << " win!"<< std::endl;
    }
    else
    {
        std::cout << "Player " << std::to_string(winner + 1) << " wins!" << std::endl;
    }
}

/**
 * Mô tả:
 *   In kết quả đơn giản (judge mode).
 *
 * TODO:
 *   - In winner và số lượt
 */
void TerminalRenderer::printResult(const GameResult& gameResult) {
    // TODO: print result đơn giản
    std::cout << gameResult.winner << " " << gameResult.turns << std::endl;
}

/**
 * Mô tả:
 *   Đóng renderer.
 *
 * TODO:
 *   - Giải phóng tài nguyên nếu có
 */
void TerminalRenderer::close() {
    // TODO: cleanup renderer
}
/**
 * SDL Renderer cpp implementation
 *
 */

#include "renderer.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>

#include "../game/setup.h"
#include "../utils/config.h"

/* ---------- Definitions ---------- */

SDLRenderer::SDLRenderer() : I_Renderer()
{
}

/**
 * Mô tả: Destructor của SDLRenderer.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Không (việc giải phóng thực hiện trong close()).
 */
SDLRenderer::~SDLRenderer()
{
}

/**
 * Mô tả: Khởi tạo SDL, window và renderer.
 * Đầu vào: config - cấu hình màn hình và layout.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Khởi tạo SDL subsystem.
 *   - Tạo window và renderer.
 *   - Thiết lập chế độ blend.
 * NOTE: Phải gọi trước khi render.
 */
void SDLRenderer::init(const RunConfig &config)
{
    // read config
    this->screenWidth = config.screenWidth;
    this->screenHeight = config.screenHeight;
    this->boardPadding = config.boardPadding;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow(
        "TicTacToe SDL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // load font
    fontTitle = TTF_OpenFont("assets/Montserrat-Bold.ttf", 48);
    fontNormal = TTF_OpenFont("assets/Montserrat-Bold.ttf", 28);
    fontSmall = TTF_OpenFont("assets/Montserrat-Regular.ttf", 16);
}

/**
 * Mô tả: Xóa toàn bộ nội dung màn hình (background tối).
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Reset frame hiện tại.
 */
void SDLRenderer::clearScreen()
{
    SDL_SetRenderDrawColor(renderer, COLOR_BG.r, COLOR_BG.g, COLOR_BG.b, 255);
    SDL_RenderClear(renderer);
}

/**
 * Mô tả: Hiển thị frame đã render lên màn hình.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Swap buffer để hiển thị nội dung.
 */
void SDLRenderer::renderPresent()
{
    SDL_RenderPresent(renderer);
}

/**
 * Mô tả: Vẽ hình chữ nhật lên renderer.
 * Đầu vào:
 *   - x, y: tọa độ.
 *   - w, h: kích thước.
 *   - color: màu sắc.
 *   - filled: true nếu fill, false nếu vẽ viền.
 * Đầu ra: Không.
 * Tác dụng phụ: Vẽ trực tiếp lên renderer.
 */
void SDLRenderer::drawRect(int x, int y, int w, int h, SDL_Color color, bool filled)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    if (filled)
        SDL_RenderFillRect(renderer, &rect);
    else
        SDL_RenderDrawRect(renderer, &rect);
}

void SDLRenderer::drawButton(const std::string &text, int y)
{
    int w = 400;
    int h = 60;
    int x = (screenWidth - w) / 2;

    drawRect(x, y, w, h, BTN_COLOR, true);
    renderTextCentered(fontNormal, text, y + 15, BTN_TEXT_COLOR);
}

void SDLRenderer::renderText(TTF_Font *targetFont, const std::string &text, int x, int y, SDL_Color color)
{
    if (!targetFont)
        return;

    SDL_Surface *surface = TTF_RenderText_Solid(targetFont, text.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void SDLRenderer::renderTextCentered(TTF_Font *targetFont, const std::string &text, int y, SDL_Color color)
{
    if (!targetFont)
        return;

    SDL_Surface *surface = TTF_RenderText_Solid(targetFont, text.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    int x = (screenWidth - surface->w) / 2;
    SDL_Rect rect = {x, y, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, nullptr, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

/**
 * Mô tả: Hiển thị menu lựa chọn.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Vẽ UI lên màn hình.
 * TODO:
 *   - Bước 1: Xác định loại menu.
 *   - Bước 2: Render text tương ứng.
 *   - Trường hợp biên: selectType không hợp lệ.
 */
void SDLRenderer::showSelectMenu(SelectType selectType, int context)
{
    clearScreen();

    std::string titleText = "";
    std::string promptText = "";
    std::vector<std::string> buttons;

    switch (selectType)
    {
    case SelectType::TITLE_UI:
        titleText = "TIC-TAC-TOE / CARO";
        buttons = {"START GAME"};
        break;

    case SelectType::GAME_MODE_UI:
        titleText = "SELECT GAME MODE";
        buttons = {"1. Player vs Player", "2. Player vs Bot", "3. Bot vs Bot"};
        break;

    case SelectType::BOT_LEVEL_UI:
        titleText = std::format("BOT {} DIFFICULTY", context + 1);
        buttons = {"1. EASY", "2. MEDIUM", "3. HARD"};
        break;

    case SelectType::SIZE_UI:
    {
        titleText = "BOARD SIZE CONFIG";
        promptText = "Select (3-12) or Type:";

        renderTextCentered(fontTitle, titleText, 100, COLOR_TEXT);
        renderTextCentered(fontNormal, promptText, 170, COLOR_TEXT);

        int btnW = 70, btnH = 60, gap = 15;
        int startX = (screenWidth - (5 * btnW + 4 * gap)) / 2;

        // 1. Vẽ Lưới nút từ 3 -> 12 (2 hàng, mỗi hàng 5 nút)
        for (int i = 0; i < 10; i++)
        {
            int val = i + 3;
            int row = i / 5;
            int col = i % 5;
            int x = startX + col * (btnW + gap);
            int y = 230 + row * (btnH + gap);

            drawRect(x, y, btnW, btnH, BTN_COLOR, true);
            renderTextCentered(fontNormal, std::to_string(val), y + 15, BTN_TEXT_COLOR);
        }

        int bottomY = 400;
        drawRect(startX, bottomY, 200, 60, COLOR_CELL, true);
        renderText(fontNormal, std::to_string(context), startX + 20, bottomY + 15, COLOR_TEXT);

        drawRect(startX + 215, bottomY, 80, 60, ERROR_COLOR, true);
        renderText(fontNormal, "DEL", startX + 225, bottomY + 15, BTN_TEXT_COLOR);

        drawRect(startX + 310, bottomY, 100, 60, SUCCESS_COLOR, true);
        renderText(fontNormal, "OK", startX + 335, bottomY + 15, BTN_TEXT_COLOR);
        break;
    }

    case SelectType::GOAL_UI: {
            // Giải nén bit: Lấy 16 bit đầu làm maxSize, 16 bit sau làm số đang hiển thị
            int maxSize = (context >> 16) & 0xFFFF;
            int currentVal = context & 0xFFFF;

            titleText = "WINNING GOAL CONFIG";
            promptText = std::format("Select (3-{}) or Type:", maxSize);

            renderTextCentered(fontTitle, titleText, 100, COLOR_TEXT);
            renderTextCentered(fontNormal, promptText, 170, COLOR_TEXT);

            int btnW = 70, btnH = 60, gap = 15;
            int startX = (screenWidth - (5 * btnW + 4 * gap)) / 2;

            // 1. ĐIỂM SÁNG UX: Chỉ vẽ ra các nút bấm hợp lệ từ 3 đến maxSize
            int maxButtons = maxSize - 3 + 1;
            for (int i = 0; i < maxButtons; i++) {
                int val = i + 3;
                int row = i / 5;
                int col = i % 5;
                int x = startX + col * (btnW + gap);
                int y = 230 + row * (btnH + gap);

                drawRect(x, y, btnW, btnH, BTN_COLOR, true);
                renderTextCentered(fontNormal, std::to_string(val), y + 15, BTN_TEXT_COLOR);
            }

            // 2. Vẽ Khung hiển thị & Các nút điều khiển DEL, OK
            int bottomY = 400;
            drawRect(startX, bottomY, 200, 60, COLOR_CELL, true);
            renderText(fontNormal, std::to_string(currentVal), startX + 20, bottomY + 15, COLOR_TEXT);

            drawRect(startX + 215, bottomY, 80, 60, ERROR_COLOR, true);
            renderText(fontNormal, "DEL", startX + 225, bottomY + 15, BTN_TEXT_COLOR);

            drawRect(startX + 310, bottomY, 100, 60, SUCCESS_COLOR, true);
            renderText(fontNormal, "OK", startX + 335, bottomY + 15, BTN_TEXT_COLOR);
            break;
        }

    case SelectType::PLAYER_UI:
        titleText = "YOUR TURN";
        promptText = "Select a cell on the board";
        break;

    case SelectType::MUL_BOT_LEVEL_UI:
        titleText = "BOT vs BOT SETUP";
        promptText = "Enter (bot1_level, bot2_level) in terminal";
        break;

    default:
        break;
    }

    if (!titleText.empty())
    {
        renderTextCentered(fontTitle, titleText, 150, COLOR_TEXT);
    }

    if (!promptText.empty())
    {
        renderTextCentered(fontNormal, promptText, 250, COLOR_TEXT);
    }

    int startY = 280;
    int spacing = 80;
    for (size_t i = 0; i < buttons.size(); i++)
    {
        drawButton(buttons[i], startY + i * spacing);
    }

    std::string footerText = "Hint: Use Keyboard to type or Mouse to click";
    renderText(fontSmall, footerText, 20, screenHeight - 40, COLOR_TEXT);

    renderPresent();
}

/**
 * Mô tả: Hiển thị thông báo lựa chọn không hợp lệ.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị thông báo lỗi.
 * TODO:
 *   - Bước 1: Xác định loại lỗi.
 *   - Bước 2: Render text cảnh báo.
 */
void SDLRenderer::showInvalidSelect(SelectType selectType, int context)
{
    // TODO: Render thông báo lỗi
    clearScreen();

    renderTextCentered(fontTitle, "INVALID INPUT", screenHeight / 2 - 50, ERROR_COLOR);
    renderTextCentered(fontNormal, "Please check your selection and try again", screenHeight / 2 + 20, COLOR_TEXT);
    renderTextCentered(fontSmall, "Click or press ENTER to continue", screenHeight - 50, COLOR_TEXT);

    renderPresent();
}

/**
 * Mô tả: Hiển thị thông báo lựa chọn hợp lệ.
 * Đầu vào: selectType, context.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị xác nhận.
 * TODO:
 *   - Bước 1: Xác định loại selection.
 *   - Bước 2: Render thông báo thành công.
 */
void SDLRenderer::showValidSelect(SelectType selectType, int context)
{
    // TODO: Render thông báo thành công
    clearScreen();
    renderTextCentered(fontTitle, "SUCCESS", screenHeight / 2 - 20, SUCCESS_COLOR);
    renderPresent();
}

/**
 * Mô tả: Vẽ bàn cờ lên màn hình.
 * Đầu vào: board, size.
 * Đầu ra: Không.
 * Tác dụng phụ: Render grid và ký hiệu.
 * TODO:
 *   - Bước 1: Tính toán layout ô.
 *   - Bước 2: Vẽ grid.
 *   - Bước 3: Vẽ X/O.
 */
void SDLRenderer::displayBoard(const char board[][BOARD_N_MAX], const int size)
{
    int gap = 8;
    int boardSizePx = screenWidth - 2 * boardPadding;
    int cellSizePx = (boardSizePx - gap * (size + 1)) / size;

    int startX = boardPadding;
    int startY = (screenHeight - boardSizePx) / 2;
    drawRect(startX, startY, boardSizePx, boardSizePx, COLOR_BOARD, true);

    for (int k = 0; k < size; k++)
    {
        std::string numStr = std::to_string(k);
        int centerOfCellX = startX + gap + k * (cellSizePx + gap) + (cellSizePx / 2);
        int centerOfCellY = startY + gap + k * (cellSizePx + gap) + (cellSizePx / 2);
        renderText(fontNormal, numStr, centerOfCellX - 8, startY - 35, COLOR_TEXT);
        renderText(fontNormal, numStr, startX - 35, centerOfCellY - 14, COLOR_TEXT);
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            int cellX = startX + gap + j * (cellSizePx + gap);
            int cellY = startY + gap + i * (cellSizePx + gap);

            drawRect(cellX, cellY, cellSizePx, cellSizePx, COLOR_CELL, true);

            if (board[i][j] == 'X' || board[i][j] == 'O')
            {
                std::string mark(1, board[i][j]);
                SDL_Color markColor = (board[i][j] == 'X') ? COLOR_X : COLOR_O;

                renderText(fontTitle, mark, cellX + 15, cellY + 10, markColor);
            }
        }
    }
    renderPresent();
}

/**
 * Mô tả: Hiển thị nước đi vừa thực hiện.
 * Đầu vào: row, col.
 * Đầu ra: Không.
 * Tác dụng phụ: Highlight ô.
 * TODO:
 *   - Bước 1: Xác định vị trí ô.
 *   - Bước 2: Vẽ highlight.
 */
void SDLRenderer::showMove(const int row, const int col)
{
    // TODO: Highlight move
    std::string moveText = std::format("Last move placed at: {}, {}", row, col);
    renderTextCentered(fontSmall, moveText, screenHeight - 80, COLOR_TEXT);
    renderPresent();
}

/**
 * Mô tả: Hiển thị thông báo nước đi không hợp lệ.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị lỗi.
 * TODO:
 *   - Bước 1: Render thông báo lỗi.
 */
void SDLRenderer::showInvalidMove()
{
    // TODO: Render invalid move message
    renderTextCentered(fontNormal, "INVALID MOVE! Cell taken or out of bounds.", 30, ERROR_COLOR);
    renderPresent();
}

/**
 * Mô tả: Hiển thị người chơi hiện tại.
 * Đầu vào: player, is_bot.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị thông tin turn.
 * TODO:
 *   - Bước 1: Xác định text.
 *   - Bước 2: Render lên màn hình.
 */
void SDLRenderer::showPlayer(const int player, const bool is_bot)
{
    // TODO: Render player info
    std::string playerName = is_bot ? std::format("BOT {}", player + 1) : std::format("PLAYER {}", player + 1);
    std::string mark = (player == 0) ? "X" : "O";
    std::string turnText = std::format("TURN: {} ({})", playerName, mark);
    SDL_Color playerColor = (player == 0) ? COLOR_X : COLOR_O;
    renderTextCentered(fontNormal, turnText, 30, playerColor);
    renderPresent();
}

/**
 * Mô tả: Hiển thị kết quả game.
 * Đầu vào: winner, is_bot, winLine.
 * Đầu ra: Không.
 * Tác dụng phụ: Hiển thị kết quả và highlight đường thắng.
 * TODO:
 *   - Bước 1: Kiểm tra draw hoặc win.
 *   - Bước 2: Render text.
 *   - Bước 3: Highlight winLine nếu có.
 */
void SDLRenderer::showResult(const int winner, const bool is_bot, const WinLine *winLine)
{
    // TODO: Render result
    SDL_Color overlayColor = {COLOR_BG.r, COLOR_BG.g, COLOR_BG.b, 200};
    drawRect(0, 0, screenWidth, screenHeight, overlayColor, true);
    std::string resultText = "";
    SDL_Color winColor = COLOR_TEXT;

    if (winner == -1)
    {
        resultText = "IT'S A DRAW!";
    }
    else
    {
        std::string type = is_bot ? "BOT" : "PLAYER";
        resultText = std::format("{} {} WINS!", type, winner + 1);
        winColor = (winner == 0) ? COLOR_X : COLOR_O;
    }

    renderTextCentered(fontTitle, resultText, screenHeight / 2 - 50, winColor);
    renderTextCentered(fontSmall, "Game Over - Press ENTER or CLICK to exit", screenHeight / 2 + 50, COLOR_TEXT);

    renderPresent();
}

/**
 * Mô tả: In kết quả ra stdout (judge mode).
 * Đầu vào: gameResult.
 * Đầu ra: Không.
 * Tác dụng phụ: In console.
 * TODO:
 *   - Bước 1: Format output.
 *   - Bước 2: In ra std::cout.
 */
void SDLRenderer::printResult(const GameResult &gameResult)
{
    // TODO: Print result
    std::cout << "=== GAME OVER ===" << std::endl;
    std::cout << "[Judge Mode] Result recorded successfully." << std::endl;
}

/**
 * Mô tả: Giải phóng tài nguyên SDL.
 * Đầu vào: Không.
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Destroy renderer và window.
 *   - Shutdown SDL subsystem.
 */
void SDLRenderer::close()
{
    // if (font) {
    //     TTF_CloseFont(font);
    //     font = nullptr;
    // }
    if (fontTitle)
        TTF_CloseFont(fontTitle);
    if (fontNormal)
        TTF_CloseFont(fontNormal);
    if (fontSmall)
        TTF_CloseFont(fontSmall);

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
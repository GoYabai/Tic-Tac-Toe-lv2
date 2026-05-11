/**
 * SDL Renderer header file
 *
 */

#pragma once

/* ---------- Importing ---------- */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <SDL2/SDL_mixer.h>

#include "../game/interface/i_renderer.h"
#include "../game/logic.h"

/* ---------- Declarations ---------- */

/**
 * Mô tả: Lớp Renderer sử dụng SDL để hiển thị giao diện đồ họa (GUI).
 * Đầu vào: Không (khởi tạo thông qua init với RunConfig).
 * Đầu ra: Không.
 * Tác dụng phụ:
 *   - Tạo cửa sổ SDL và context render.
 *   - Vẽ các thành phần giao diện (board, text, UI).
 * NOTE: Phải gọi init() trước khi sử dụng các hàm render.
 */
class SDLRenderer : public I_Renderer {
   private:
    SDL_Window* window = nullptr;      // Con trỏ tới cửa sổ SDL
    SDL_Renderer* renderer = nullptr;  // Context để vẽ (rendering)
    TTF_Font* fontTitle = nullptr;
    TTF_Font* fontNormal = nullptr;
    TTF_Font* fontSmall = nullptr;

    Mix_Music* bgmMusic = nullptr;
    Mix_Chunk* soundHover = nullptr;
    Mix_Chunk* soundClick = nullptr;
    std::string lastHoveredItem = "";

    int screenWidth;
    int screenHeight;
    int boardPadding;

    int lastRow = -1;
    int lastCol = -1;

    const SDL_Color COLOR_BG            = {250, 248, 239, 255};
    const SDL_Color COLOR_BOARD         = {187, 173, 160, 255};
    const SDL_Color COLOR_CELL          = {205, 193, 180, 255};
    const SDL_Color COLOR_TEXT          = {119, 110, 101, 255};
    const SDL_Color COLOR_X             = {219, 90, 66, 255};
    const SDL_Color COLOR_O             = {33, 150, 243, 255};
    const SDL_Color COLOR_BTN_HOVER     = {173, 152, 132, 255};
    const SDL_Color ERROR_COLOR_HOVER   = {255, 143, 136, 255};
    const SDL_Color SUCCESS_COLOR_HOVER = {144, 223, 244, 255};
    const SDL_Color SUCCESS_COLOR       = {114, 203, 224, 255};
    const SDL_Color ERROR_COLOR         = {235, 113, 106, 255};
    const SDL_Color BTN_COLOR           = {143, 122, 102, 255};
    const SDL_Color BTN_TEXT_COLOR      = {255, 255, 255, 255};

    /**
     * Mô tả: Đẩy nội dung đã vẽ lên màn hình (swap buffer).
     * Đầu vào: Không.
     * Đầu ra: Không.
     * Tác dụng phụ: Hiển thị frame hiện tại lên cửa sổ.
     * NOTE: Phải gọi sau khi hoàn tất các lệnh vẽ.
     */
    void renderPresent();

    /**
     * Mô tả: Vẽ hình chữ nhật lên màn hình.
     * Đầu vào:
     *   - x, y: tọa độ góc trên bên trái.
     *   - w, h: chiều rộng và chiều cao.
     *   - color: màu sắc (SDL_Color).
     *   - filled: true nếu vẽ hình đặc, false nếu chỉ vẽ viền.
     * Đầu ra: Không.
     * Tác dụng phụ: Vẽ trực tiếp lên renderer.
     */
    void drawRect(int x, int y, int w, int h, SDL_Color color, bool filled);

    void drawButton(const std::string& text, int y);
    void renderText(TTF_Font* targetFont, const std::string& text, int x, int y, SDL_Color color);
    void renderTextCentered(TTF_Font* targetFont, const std::string& text, int y, SDL_Color color);

   public:

    void playClickSound() override;

    /**
     * Mô tả: Constructor khởi tạo SDLRenderer.
     * Đầu vào: Không.
     * Đầu ra: Không.
     * Tác dụng phụ: Khởi tạo các con trỏ về nullptr.
     */
    SDLRenderer();

    /**
     * Mô tả: Destructor giải phóng tài nguyên SDL.
     * Đầu vào: Không.
     * Đầu ra: Không.
     * Tác dụng phụ: Giải phóng window và renderer nếu đã tạo.
     */
    ~SDLRenderer();

    /**
     * Mô tả: Khởi tạo hệ thống SDL (window, renderer, font, ...).
     * Đầu vào: config - cấu hình runtime (kích thước màn hình, layout,...).
     * Đầu ra: Không.
     * Tác dụng phụ:
     *   - Tạo window SDL.
     *   - Tạo renderer.
     * NOTE: Bắt buộc gọi trước khi render bất kỳ thứ gì.
     */
    void init(const RunConfig& config) override;

    /**
     * Mô tả: Xóa màn hình (clear toàn bộ frame hiện tại).
     * Đầu vào: Không.
     * Đầu ra: Không.
     * Tác dụng phụ: Reset nội dung render.
     */
    void clearScreen() override;

    /**
     * Mô tả: Hiển thị menu lựa chọn (UI).
     * Đầu vào:
     *   - selectType: loại menu cần hiển thị.
     *   - context: thông tin bổ sung (ví dụ index bot).
     * Đầu ra: Không.
     * Tác dụng phụ: Vẽ nội dung UI lên màn hình.
     */
    void showSelectMenu(SelectType selectType, int context = NO_CONTEXT) override;

    /**
     * Mô tả: Hiển thị thông báo lựa chọn không hợp lệ.
     * Đầu vào:
     *   - selectType: loại input.
     *   - context: giá trị input sai.
     * Đầu ra: Không.
     * Tác dụng phụ: Hiển thị thông báo lỗi trên UI.
     */
    void showInvalidSelect(SelectType selectType, int context = NO_CONTEXT) override;

    /**
     * Mô tả: Hiển thị thông báo lựa chọn hợp lệ.
     * Đầu vào:
     *   - selectType: loại input.
     *   - context: giá trị đã chọn.
     * Đầu ra: Không.
     * Tác dụng phụ: Hiển thị thông báo xác nhận trên UI.
     */
    void showValidSelect(SelectType selectType, int context = NO_CONTEXT) override;

    /**
     * Mô tả: Hiển thị bàn cờ lên màn hình.
     * Đầu vào:
     *   - board: ma trận trạng thái bàn cờ.
     *   - size: kích thước bàn cờ.
     * Đầu ra: Không.
     * Tác dụng phụ: Vẽ grid và ký hiệu X/O.
     */
    void displayBoard(const char board[][BOARD_N_MAX], const int size) override;

    /**
     * Mô tả: Hiển thị nước đi vừa thực hiện.
     * Đầu vào:
     *   - row, col: tọa độ nước đi.
     * Đầu ra: Không.
     * Tác dụng phụ: Có thể highlight ô vừa chọn.
     */
    void showMove(const int row, const int col) override;

    /**
     * Mô tả: Hiển thị thông báo nước đi không hợp lệ.
     * Đầu vào: Không.
     * Đầu ra: Không.
     * Tác dụng phụ: Hiển thị lỗi trên UI.
     */
    void showInvalidMove() override;

    /**
     * Mô tả: Hiển thị thông tin người chơi hiện tại.
     * Đầu vào:
     *   - player: index người chơi (0 hoặc 1).
     *   - is_bot: true nếu là bot.
     * Đầu ra: Không.
     * Tác dụng phụ: Hiển thị trạng thái turn.
     */
    void showPlayer(const int player, const bool is_bot) override;

    /**
     * Mô tả: Hiển thị kết quả trận đấu.
     * Đầu vào:
     *   - winner: người thắng (-1 nếu hòa).
     *   - is_bot: người thắng có phải bot không.
     *   - winLine: danh sách ô thắng (nếu có).
     * Đầu ra: Không.
     * Tác dụng phụ: Hiển thị kết quả và highlight đường thắng.
     */
    void showResult(const int winner, const bool is_bot, const WinLine* winLine = nullptr) override;

    /**
     * Mô tả: In kết quả ra stdout (dùng cho judge mode).
     * Đầu vào: gameResult - kết quả game.
     * Đầu ra: Không.
     * Tác dụng phụ: In ra console.
     */
    void printResult(const GameResult& gameResult) override;

    /**
     * Mô tả: Giải phóng tài nguyên SDL.
     * Đầu vào: Không.
     * Đầu ra: Không.
     * Tác dụng phụ:
     *   - Destroy renderer và window.
     *   - Shutdown SDL subsystem.
     */
    void close() override;

};
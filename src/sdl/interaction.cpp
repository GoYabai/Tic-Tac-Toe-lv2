/**
 * SDL Interaction cpp implementation
 *
 */

#include "interaction.h"

/* ---------- Importing ---------- */

#include <format>
#include <iostream>

#include <sstream>
#include <string>

/* ---------- Definitions ---------- */

/**
 * Mô tả: Kiểm tra sự kiện quit từ SDL.
 * Đầu vào:
 *   - e: SDL_Event hiện tại.
 * Đầu ra:
 *   - false nếu không phải quit event.
 * Tác dụng phụ:
 *   - Ném QuitException nếu nhận được sự kiện thoát.
 */
bool SDLInteraction::waitForQuit(SDL_Event &e)
{
    if (e.type == SDL_QUIT)
    {
        throw QuitException();
    }
    return false;
}

/**
 * Mô tả: Khởi tạo đối tượng SDLInteraction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ: Không có.
 */
SDLInteraction::SDLInteraction()
{
}

/**
 * Mô tả: Destructor của SDLInteraction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ: Không có.
 */
SDLInteraction::~SDLInteraction()
{
}

/**
 * Mô tả: Khởi tạo hệ thống interaction cho SDL.
 * Đầu vào:
 *   - config: cấu hình runtime của chương trình.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Thiết lập trạng thái ban đầu cho input SDL.
 */
void SDLInteraction::init(const RunConfig &config)
{
    // TODO:
    // - Khởi tạo các thành phần cần thiết cho input SDL
    // - Có thể reset event queue hoặc trạng thái input
    this->screenWidth = config.screenWidth;
    this->screenHeight = config.screenHeight;
    this->boardPadding = config.boardPadding;

    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
}

/**
 * Mô tả: Tạm dừng chương trình trong SDL.
 *        - Nếu có timeout > 0: delay trong khoảng thời gian tương ứng.
 *        - Nếu timeout == 0: chờ người dùng tương tác (nhấn phím hoặc click chuột).
 * Đầu vào:
 *   - timeout: thời gian chờ (milliseconds). Nếu = 0 thì chờ event từ người dùng.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Có thể block thread hiện tại.
 *   - Có thể ném QuitException nếu người dùng đóng cửa sổ.
 */
void SDLInteraction::pause(int timeout)
{
    if (timeout > 0)
    {
        SDL_Delay(timeout);
        return;
    }

    bool waiting = true;
    SDL_Event event;

    int btnW = 400;
    int btnH = 60;
    int btnX = (screenWidth - btnW) / 2;
    int btnY = 280;

    // Biến theo dõi: Nhớ trạng thái trước đó để chỉ kêu "tạch" 1 lần khi vừa lướt vào nút
    bool wasHovered = false;

    while (waiting)
    {
        // 🌟 LIÊN TỤC VẼ LẠI TITLE SCREEN KHI CÓ SỰ KIỆN ĐỂ CẬP NHẬT MÀU HOVER
        if (isTitleScreen && this->renderer)
        {
            this->renderer->showSelectMenu(SelectType::TITLE_UI);
        }

        if (SDL_WaitEvent(&event))
        {
            if (waitForQuit(event)) {}

            // =================================================================
            // 🚀 MỞ RỘNG TƯƠNG LAI: BẮT TÍN HIỆU ÂM THANH "TẠCH TẠCH"
            // =================================================================
            if (event.type == SDL_MOUSEMOTION && isTitleScreen) 
            {
                int mx = event.motion.x;
                int my = event.motion.y;
                bool isCurrentlyHovered = (mx >= btnX && mx <= btnX + btnW && my >= btnY && my <= btnY + btnH);
                
                // Nếu chuột VỪA BƯỚC VÀO nút (trước đó ở ngoài, giờ ở trong)
                if (isCurrentlyHovered && !wasHovered) {
                    // TODO (Khi tích hợp SDL_mixer): 
                    // Mix_PlayChannel(-1, soundHover, 0); 
                    std::cout << "[SFX] *Tach tach*" << std::endl; // In tạm ra console cho vui tai
                }
                wasHovered = isCurrentlyHovered;
            }

            // A. XỬ LÝ ENTER
            if (event.type == SDL_KEYDOWN)
            {
                SDL_Keycode key = event.key.keysym.sym;
                if (key == SDLK_RETURN || key == SDLK_KP_ENTER)
                {
                    waiting = false;
                    isTitleScreen = false;
                }
            }
            // B. XỬ LÝ CLICK CHUỘT
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
            {
                int mx = event.button.x;
                int my = event.button.y;

                if (isTitleScreen)
                {
                    if (mx >= btnX && mx <= btnX + btnW && my >= btnY && my <= btnY + btnH)
                    {
                        waiting = false;
                        isTitleScreen = false;
                    }
                }
                else
                {
                    waiting = false;
                }
            }
        }
    }
}

/**
 * Mô tả: Lấy input kích thước bàn cờ từ người dùng qua SDL.
 * Đầu vào:
 *   - size: con trỏ lưu kết quả kích thước.
 * Đầu ra:
 *   - true nếu input hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật giá trị tại size nếu thành công.
 */

bool SDLInteraction::selectSize(int* size) {
    // TODO:
    // - Lắng nghe event từ SDL (keyboard/mouse)
    // - Parse input thành số nguyên
    // - Kiểm tra điều kiện hợp lệ (BOARD_N_MIN <= size <= BOARD_N_MAX)
    // - Trả về true nếu hợp lệ, ngược lại false
    
    SDL_Event event;
    std::string inputBuffer = "";
    bool waiting = true;

    // Tọa độ bounding box khớp tuyệt đối với lưới vẽ Numpad
    int btnW = 70, btnH = 60, gap = 15;
    int startX = (screenWidth - (5 * btnW + 4 * gap)) / 2;
    int gridY = 230;
    int bottomY = 400;

    SDL_StartTextInput();

    while (waiting) {
        // 🌟 ĐIỂM SÁNG UX: Cập nhật màn hình liên tục theo từng cú bấm
        int displayVal = inputBuffer.empty() ? 0 : std::stoi(inputBuffer);
        if (this->renderer) {
            this->renderer->showSelectMenu(SelectType::SIZE_UI, displayVal);
        }

        if (SDL_WaitEvent(&event)) {
            if (waitForQuit(event)) {}

            // A. XỬ LÝ BÀN PHÍM
            if (event.type == SDL_TEXTINPUT) {
                char c = event.text.text[0];
                if (c >= '0' && c <= '9') {
                    inputBuffer += c;
                }
            } 
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && !inputBuffer.empty()) {
                    inputBuffer.pop_back();
                }
                else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                    if (!inputBuffer.empty()) waiting = false;
                }
            }

            // B. XỬ LÝ CLICK CHUỘT
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;

                // 1. Quét click trên lưới số từ 3 đến 12
                for (int i = 0; i < 10; i++) {
                    int row = i / 5; 
                    int col = i % 5;
                    int x = startX + col * (btnW + gap);
                    int y = gridY + row * (btnH + gap);
                    
                    if (mx >= x && mx <= x + btnW && my >= y && my <= y + btnH) {
                        inputBuffer = std::to_string(i + 3); // Ghi nhận số được bấm
                    }
                }

                // 2. Quét click nút DEL (Xóa lùi)
                if (mx >= startX + 215 && mx <= startX + 295 && my >= bottomY && my <= bottomY + 60) {
                    if (!inputBuffer.empty()) {
                        inputBuffer.pop_back();
                    }
                }

                // 3. Quét click nút OK (Xác nhận)
                if (mx >= startX + 310 && mx <= startX + 410 && my >= bottomY && my <= bottomY + 60) {
                    if (!inputBuffer.empty()) {
                        waiting = false;
                    }
                }
            }
        }
    }

    SDL_StopTextInput();

    // Kiểm tra chốt hạ an toàn trước khi trả về dữ liệu
    try {
        int val = std::stoi(inputBuffer);
        if (val >= 3 && val <= 12) { 
            *size = val;
            this->currentBoardSize = val;
            return true; 
        }
    } catch (...) {}

    return false;
}

/**
 * Mô tả: Lấy input goal (số ô liên tiếp để thắng).
 * Đầu vào:
 *   - goal: con trỏ lưu giá trị goal.
 *   - size: kích thước bàn cờ hiện tại.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật goal nếu hợp lệ.
 */
bool SDLInteraction::selectGoal(int* goal, const int size) {
    SDL_Event event;
    std::string inputBuffer = "";
    bool waiting = true;

    // Tọa độ Bounding Box gióng hàng chuẩn xác theo layout Numpad chung
    int btnW = 70, btnH = 60, gap = 15;
    int startX = (screenWidth - (5 * btnW + 4 * gap)) / 2;
    int gridY = 230;
    int bottomY = 400;

    SDL_StartTextInput();

    while (waiting) {
        // ĐÓNG GÓI DỮ LIỆU: Nhét cả 'size tối đa' và 'số đang gõ' vào chung biến context
        int displayVal = inputBuffer.empty() ? 0 : std::stoi(inputBuffer);
        int packedContext = (size << 16) | (displayVal & 0xFFFF);

        // Gọi Họa sĩ cập nhật màn hình liên tục
        if (this->renderer) {
            this->renderer->showSelectMenu(SelectType::GOAL_UI, packedContext);
        }

        if (SDL_WaitEvent(&event)) {
            if (waitForQuit(event)) {}

            // A. XỬ LÝ BÀN PHÍM
            if (event.type == SDL_TEXTINPUT) {
                char c = event.text.text[0];
                if (c >= '0' && c <= '9') {
                    inputBuffer += c;
                }
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && !inputBuffer.empty()) {
                    inputBuffer.pop_back();
                }
                else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                    if (!inputBuffer.empty()) waiting = false;
                }
            }

            // B. XỬ LÝ CLICK CHUỘT
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;

                // 1. Quét click trên các nút số hợp lệ (chỉ quét từ 3 đến size)
                int maxButtons = size - 3 + 1;
                for (int i = 0; i < maxButtons; i++) {
                    int row = i / 5;
                    int col = i % 5;
                    int x = startX + col * (btnW + gap);
                    int y = gridY + row * (btnH + gap);

                    if (mx >= x && mx <= x + btnW && my >= y && my <= y + btnH) {
                        inputBuffer = std::to_string(i + 3); // Ghi nhận số được click
                    }
                }

                // 2. Quét click nút DEL (Xóa lùi)
                if (mx >= startX + 215 && mx <= startX + 295 && my >= bottomY && my <= bottomY + 60) {
                    if (!inputBuffer.empty()) {
                        inputBuffer.pop_back();
                    }
                }

                // 3. Quét click nút OK (Xác nhận)
                if (mx >= startX + 310 && mx <= startX + 410 && my >= bottomY && my <= bottomY + 60) {
                    if (!inputBuffer.empty()) {
                        waiting = false;
                    }
                }
            }
        }
    }

    SDL_StopTextInput();

    // Kiểm tra chốt hạ an toàn: 3 <= goal <= size
    try {
        int val = std::stoi(inputBuffer);
        if (val >= 3 && val <= size) {
            *goal = val;
            return true;
        }
    } catch (...) {}

    return false;
}

/**
 * Mô tả: Lấy input chế độ chơi (PVP, PVE, EVE).
 * Đầu vào:
 *   - mode: con trỏ lưu giá trị GameMode.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật mode nếu hợp lệ.
 */
bool SDLInteraction::selectGameMode(GameMode* mode) {
    SDL_Event event;
    bool waiting = true;
    int choice = 0;

    // Tọa độ Bounding Box khớp tuyệt đối với hàm drawButton trong renderer.cpp
    int btnW = 400;
    int btnH = 60;
    int btnX = (screenWidth - btnW) / 2;
    int startY = 280;
    int spacing = 80;

    // Dọn sạch event rác trước khi vào màn hình chọn
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

    while (waiting) {
        // Gọi Họa sĩ vẽ giao diện chọn Mode (không cần context vì menu này cố định)
        if (this->renderer) {
            this->renderer->showSelectMenu(SelectType::GAME_MODE_UI);
        }

        if (SDL_WaitEvent(&event)) {
            if (waitForQuit(event)) {}

            // A. XỬ LÝ BÀN PHÍM (Bấm phím 1, 2, 3 ngang hoặc phím phụ Numpad)
            if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;
                if (key == SDLK_1 || key == SDLK_KP_1) { 
                    choice = 1; 
                    waiting = false; 
                }
                else if (key == SDLK_2 || key == SDLK_KP_2) { 
                    choice = 2; 
                    waiting = false; 
                }
                else if (key == SDLK_3 || key == SDLK_KP_3) { 
                    choice = 3; 
                    waiting = false; 
                }
            }

            // B. XỬ LÝ CLICK CHUỘT
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;

                // Quét tọa độ của 3 nút bấm xếp dọc
                for (int i = 0; i < 3; i++) {
                    int currentBtnY = startY + i * spacing;
                    
                    if (mx >= btnX && mx <= btnX + btnW && 
                        my >= currentBtnY && my <= currentBtnY + btnH) {
                        choice = i + 1; // Trả về 1, 2 hoặc 3
                        waiting = false;
                        break;
                    }
                }
            }
        }
    }

    // Ánh xạ (Mapping) kết quả sang giá trị enum GameMode chuẩn
    if (choice >= 1 && choice <= 3) {
        if (choice == 1) {
            *mode = GameMode::PVP;
        } else if (choice == 2) {
            *mode = GameMode::PVE;
        } else if (choice == 3) {
            *mode = GameMode::EVE;
        }
        return true;
    }

    return false;
}

/**
 * Mô tả: Lấy input cấp độ bot.
 * Đầu vào:
 *   - levels: mảng lưu cấp độ bot.
 *   - index: vị trí bot cần gán.
 * Đầu ra:
 *   - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 *   - Cập nhật levels[index] nếu hợp lệ.
 */
bool SDLInteraction::selectBotLevel(BotLevel* levels, const int index) {
    SDL_Event event;
    bool waiting = true;
    int choice = 0;

    // Tọa độ Bounding Box khớp tuyệt đối với layout 3 nút dọc của Renderer
    int btnW = 400;
    int btnH = 60;
    int btnX = (screenWidth - btnW) / 2;
    int startY = 280;
    int spacing = 80;

    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

    while (waiting) {
        // Gọi Họa sĩ vẽ Menu chọn độ khó, truyền index vào context để đổi tiêu đề
        if (this->renderer) {
            this->renderer->showSelectMenu(SelectType::BOT_LEVEL_UI, index);
        }

        if (SDL_WaitEvent(&event)) {
            if (waitForQuit(event)) {}

            // A. XỬ LÝ BÀN PHÍM (1, 2, 3)
            if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;
                if (key == SDLK_1 || key == SDLK_KP_1) { choice = 1; waiting = false; }
                else if (key == SDLK_2 || key == SDLK_KP_2) { choice = 2; waiting = false; }
                else if (key == SDLK_3 || key == SDLK_KP_3) { choice = 3; waiting = false; }
            }

            // B. XỬ LÝ CLICK CHUỘT
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;

                for (int i = 0; i < 3; i++) {
                    int currentBtnY = startY + i * spacing;
                    if (mx >= btnX && mx <= btnX + btnW && 
                        my >= currentBtnY && my <= currentBtnY + btnH) {
                        choice = i + 1;
                        waiting = false;
                        break;
                    }
                }
            }
        }
    }

    // Ánh xạ lựa chọn vào mảng levels tại vị trí index
    if (choice >= 1 && choice <= 3) {
        if (choice == 1) levels[index] = BotLevel::EASY;
        else if (choice == 2) levels[index] = BotLevel::MEDIUM;
        else if (choice == 3) levels[index] = BotLevel::HARD;
        return true;
    }

    return false;
}

/**
 * Mô tả: Lấy nước đi từ người chơi (row, col) qua SDL.
 * Đầu vào:
 *   - row: con trỏ lưu hàng.
 *   - col: con trỏ lưu cột.
 * Đầu ra:
 *   - true nếu lấy được input hợp lệ, false nếu không.
 * Tác dụng phụ:
 *   - Cập nhật row, col nếu hợp lệ.
 */
/**
 * Mô tả: Lấy nước đi từ người chơi (row, col) qua SDL.
 * Hỗ trợ song song: 
 * 1. Click chuột trực tiếp vào ô cờ (loại trừ hoàn toàn các rãnh hở gap).
 * 2. Gõ phím nhập tọa độ "hàng cột" (ví dụ: "0 1") rồi nhấn Enter.
 */
bool SDLInteraction::getPlayerMove(int* row, int* col) {
    SDL_Event event;
    bool waiting = true;
    std::string inputBuffer = "";

    SDL_StartTextInput();
    std::cout << "[SDL Input] Your turn. Click a cell directly OR type 'row col' (e.g. 0 1) and press ENTER: " << std::flush;

    int size = this->currentBoardSize;

    int gap = 8;
    int boardSizePx = screenHeight - 2 * boardPadding;
    int cellSizePx = (boardSizePx - gap * (size + 1)) / size;
    int startX = (screenWidth - boardSizePx) / 2;
    int startY = boardPadding;

    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

    while (waiting) {
        // ĐÃ XÓA BLOCK GỌI sdlRenderer->redrawCachedBoard() TẠI ĐÂY

        if (SDL_WaitEvent(&event)) {
            if (waitForQuit(event)) {}

            // A. XỬ LÝ BÀN PHÍM (Gõ tọa độ: "hàng cột")
            if (event.type == SDL_TEXTINPUT) {
                char c = event.text.text[0];
                if ((c >= '0' && c <= '9') || c == ' ') {
                    inputBuffer += c;
                    std::cout << c << std::flush;
                }
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && !inputBuffer.empty()) {
                    inputBuffer.pop_back();
                    std::cout << "\b \b" << std::flush;
                }
                else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                    std::cout << std::endl;
                    
                    if (!inputBuffer.empty()) {
                        std::stringstream ss(inputBuffer);
                        int r = -1, c = -1;
                        
                        if (ss >> r >> c) {
                            if (r >= 0 && r < size && c >= 0 && c < size) {
                                *row = r;
                                *col = c;
                                SDL_StopTextInput();
                                return true;
                            } else {
                                std::cout << "[SDL Input Error] Cell (" << r << ", " << c << ") is out of bounds (0 to " << size - 1 << ")!\n[SDL Input] Try again: " << std::flush;
                                inputBuffer = "";
                            }
                        } else {
                            std::cout << "[SDL Input Error] Invalid format! Please type two numbers separated by space (e.g. 1 2).\n[SDL Input] Try again: " << std::flush;
                            inputBuffer = "";
                        }
                    }
                }
            }

            // B. XỬ LÝ CLICK CHUỘT
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;

                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < size; j++) {
                        int cellX = startX + gap + j * (cellSizePx + gap);
                        int cellY = startY + gap + i * (cellSizePx + gap);

                        if (mx >= cellX && mx <= cellX + cellSizePx &&
                            my >= cellY && my <= cellY + cellSizePx) {
                            
                            *row = i;
                            *col = j;
                            
                            std::cout << "Mouse clicked on cell (" << i << ", " << j << ")" << std::endl;
                            SDL_StopTextInput();
                            return true;
                        }
                    }
                }
            }
        }
    }

    SDL_StopTextInput();
    return false;
}

/**
 * Mô tả: Dọn dẹp tài nguyên liên quan đến SDL interaction.
 * Đầu vào: Không có.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 *   - Giải phóng hoặc reset trạng thái input nếu cần.
 */

void SDLInteraction::close() {
    SDL_StopTextInput();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    this->renderer = nullptr;
    std::cout << "[SDL Interaction] Closed safely." << std::endl;
}
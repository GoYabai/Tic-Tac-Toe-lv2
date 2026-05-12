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

void SDLInteraction::playClickSFX()
{
    if (this->renderer) {
        this->renderer->playClickSound(); // Gọi xuyên suốt qua Abstract Interface
    }
}

/**
 * Mô tả: Kiểm tra sự kiện quit từ SDL.
 * Đầu vào:
 * - e: SDL_Event hiện tại.
 * Đầu ra:
 * - false nếu không phải quit event.
 * Tác dụng phụ:
 * - Ném QuitException nếu nhận được sự kiện thoát.
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
 * - config: cấu hình runtime của chương trình.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 * - Thiết lập trạng thái ban đầu cho input SDL.
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
 * - Nếu có timeout > 0: delay trong khoảng thời gian tương ứng.
 * - Nếu timeout == 0: chờ người dùng tương tác (nhấn phím hoặc click chuột).
 * Đầu vào:
 * - timeout: thời gian chờ (milliseconds). Nếu = 0 thì chờ event từ người dùng.
 * Đầu ra: Không có.
 * Tác dụng phụ:
 * - Có thể block thread hiện tại.
 * - Có thể ném QuitException nếu người dùng đóng cửa sổ.
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

    while (waiting)
    {
        if (isTitleScreen && this->renderer)
        {
            this->renderer->showSelectMenu(SelectType::TITLE_UI);
        }

        if (SDL_WaitEvent(&event))
        {
            if (waitForQuit(event)) {}

            // A. XỬ LÝ ENTER
            if (event.type == SDL_KEYDOWN)
            {
                SDL_Keycode key = event.key.keysym.sym;
                if (key == SDLK_RETURN || key == SDLK_KP_ENTER)
                {
                    playClickSFX();
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
                        playClickSFX();
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
 * - size: con trỏ lưu kết quả kích thước.
 * Đầu ra:
 * - true nếu input hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 * - Cập nhật giá trị tại size nếu thành công.
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

    int btnW = 70, btnH = 60, gap = 15;
    int startX = (screenWidth - (5 * btnW + 4 * gap)) / 2;
    int gridY = 230;
    int bottomY = 400;

    SDL_StartTextInput();

    while (waiting) {
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
                    playClickSFX();
                    inputBuffer += c;
                }
            } 
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && !inputBuffer.empty()) {
                    playClickSFX();
                    inputBuffer.pop_back();
                }
                else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                    if (!inputBuffer.empty()) {
                        playClickSFX();
                        
                        // 🌟 KIỂM TRA LỖI TRỰC TIẾP TRƯỚC KHI THOÁT VÒNG LẶP
                        try {
                            int checkVal = std::stoi(inputBuffer);
                            if (checkVal >= 3 && checkVal <= 12) {
                                // Hợp lệ -> Cho phép thoát để đi tiếp
                                waiting = false;
                            } else {
                                // Sai phạm vi -> Kích hoạt hiển thị lỗi rõ ràng
                                if (this->renderer) {
                                    this->renderer->showInvalidSelect(SelectType::SIZE_UI, checkVal);
                                    SDL_Delay(1200); // Tạm dừng 1.2s để người chơi kịp nhìn thấy cảnh báo
                                }
                                inputBuffer = ""; // Reset sạch bộ đệm để nhập lại từ đầu
                                SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                            }
                        } catch (...) {
                            if (this->renderer) {
                                this->renderer->showInvalidSelect(SelectType::SIZE_UI, 0);
                                SDL_Delay(1200);
                            }
                            inputBuffer = "";
                            SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                        }
                    }
                }
            }

            // B. XỬ LÝ CLICK CHUỘT
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;

                // 1. Quét click trên lưới số
                for (int i = 0; i < 10; i++) {
                    int row = i / 5; 
                    int col = i % 5;
                    int x = startX + col * (btnW + gap);
                    int y = gridY + row * (btnH + gap);
                    
                    if (mx >= x && mx <= x + btnW && my >= y && my <= y + btnH) {
                        playClickSFX();
                        inputBuffer = std::to_string(i + 3); 
                    }
                }

                // 2. Quét click nút DEL
                if (mx >= startX + 215 && mx <= startX + 295 && my >= bottomY && my <= bottomY + 60) {
                    if (!inputBuffer.empty()) {
                        playClickSFX();
                        inputBuffer.pop_back();
                    }
                }

                // 3. Quét click nút OK
                if (mx >= startX + 310 && mx <= startX + 410 && my >= bottomY && my <= bottomY + 60) {
                    if (!inputBuffer.empty()) {
                        playClickSFX();
                        
                        // 🌟 KIỂM TRA LỖI KHI BẤM NÚT OK TRÊN GIAO DIỆN
                        try {
                            int checkVal = std::stoi(inputBuffer);
                            if (checkVal >= 3 && checkVal <= 12) {
                                waiting = false;
                            } else {
                                if (this->renderer) {
                                    this->renderer->showInvalidSelect(SelectType::SIZE_UI, checkVal);
                                    SDL_Delay(1200);
                                }
                                inputBuffer = "";
                                SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                            }
                        } catch (...) {
                            if (this->renderer) {
                                this->renderer->showInvalidSelect(SelectType::SIZE_UI, 0);
                                SDL_Delay(1200);
                            }
                            inputBuffer = "";
                            SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                        }
                    }
                }
            }
        }
    }

    SDL_StopTextInput();

    try {
        int val = std::stoi(inputBuffer);
        if (val >= 3 && val <= 12) { 
            *size = val;
            this->currentBoardSize = val;
            
            // Hiển thị thông báo hợp lệ trọn vẹn trước khi chuyển cảnh
            if (this->renderer) {
                this->renderer->showValidSelect(SelectType::SIZE_UI, val);
                SDL_Delay(500);
            }
            return true; 
        }
    } catch (...) {}

    return false;
}

/**
 * Mô tả: Lấy input goal (số ô liên tiếp để thắng).
 * Đầu vào:
 * - goal: con trỏ lưu giá trị goal.
 * - size: kích thước bàn cờ hiện tại.
 * Đầu ra:
 * - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 * - Cập nhật goal nếu hợp lệ.
 */
bool SDLInteraction::selectGoal(int* goal, const int size) {
    SDL_Event event;
    std::string inputBuffer = "";
    bool waiting = true;

    int btnW = 70, btnH = 60, gap = 15;
    int startX = (screenWidth - (5 * btnW + 4 * gap)) / 2;
    int gridY = 230;
    int bottomY = 400;

    SDL_StartTextInput();

    while (waiting) {
        int displayVal = inputBuffer.empty() ? 0 : std::stoi(inputBuffer);
        int packedContext = (size << 16) | (displayVal & 0xFFFF);

        if (this->renderer) {
            this->renderer->showSelectMenu(SelectType::GOAL_UI, packedContext);
        }

        if (SDL_WaitEvent(&event)) {
            if (waitForQuit(event)) {}

            if (event.type == SDL_TEXTINPUT) {
                char c = event.text.text[0];
                if (c >= '0' && c <= '9') {
                    playClickSFX();
                    inputBuffer += c;
                }
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && !inputBuffer.empty()) {
                    playClickSFX();
                    inputBuffer.pop_back();
                }
                else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                    if (!inputBuffer.empty()) {
                        playClickSFX();
                        
                        // 🌟 KIỂM TRA LỖI KHI BẤM ENTER Ở MÀN HÌNH GOAL
                        try {
                            int checkVal = std::stoi(inputBuffer);
                            if (checkVal >= 3 && checkVal <= size) {
                                waiting = false;
                            } else {
                                if (this->renderer) {
                                    this->renderer->showInvalidSelect(SelectType::GOAL_UI, checkVal);
                                    SDL_Delay(1200);
                                }
                                inputBuffer = "";
                                SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                            }
                        } catch (...) {
                            if (this->renderer) {
                                this->renderer->showInvalidSelect(SelectType::GOAL_UI, 0);
                                SDL_Delay(1200);
                            }
                            inputBuffer = "";
                            SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                        }
                    }
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;

                int maxButtons = size - 3 + 1;
                for (int i = 0; i < maxButtons; i++) {
                    int row = i / 5;
                    int col = i % 5;
                    int x = startX + col * (btnW + gap);
                    int y = gridY + row * (btnH + gap);

                    if (mx >= x && mx <= x + btnW && my >= y && my <= y + btnH) {
                        playClickSFX();
                        inputBuffer = std::to_string(i + 3); 
                    }
                }

                if (mx >= startX + 215 && mx <= startX + 295 && my >= bottomY && my <= bottomY + 60) {
                    if (!inputBuffer.empty()) {
                        playClickSFX();
                        inputBuffer.pop_back();
                    }
                }

                if (mx >= startX + 310 && mx <= startX + 410 && my >= bottomY && my <= bottomY + 60) {
                    if (!inputBuffer.empty()) {
                        playClickSFX();
                        
                        // 🌟 KIỂM TRA LỖI KHI BẤM NÚT OK TRÊN GIAO DIỆN GOAL
                        try {
                            int checkVal = std::stoi(inputBuffer);
                            if (checkVal >= 3 && checkVal <= size) {
                                waiting = false;
                            } else {
                                if (this->renderer) {
                                    this->renderer->showInvalidSelect(SelectType::GOAL_UI, checkVal);
                                    SDL_Delay(1200);
                                }
                                inputBuffer = "";
                                SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                            }
                        } catch (...) {
                            if (this->renderer) {
                                this->renderer->showInvalidSelect(SelectType::GOAL_UI, 0);
                                SDL_Delay(1200);
                            }
                            inputBuffer = "";
                            SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
                        }
                    }
                }
            }
        }
    }

    SDL_StopTextInput();

    try {
        int val = std::stoi(inputBuffer);
        if (val >= 3 && val <= size) {
            *goal = val;
            
            if (this->renderer) {
                this->renderer->showValidSelect(SelectType::GOAL_UI, val);
                SDL_Delay(500);
            }
            return true;
        }
    } catch (...) {}

    return false;
}

/**
 * Mô tả: Lấy input chế độ chơi (PVP, PVE, EVE).
 * Đầu vào:
 * - mode: con trỏ lưu giá trị GameMode.
 * Đầu ra:
 * - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 * - Cập nhật mode nếu hợp lệ.
 */
bool SDLInteraction::selectGameMode(GameMode* mode) {
    SDL_Event event;
    bool waiting = true;
    int choice = 0;

    int btnW = 400;
    int btnH = 60;
    int btnX = (screenWidth - btnW) / 2;
    int startY = 280;
    int spacing = 80;

    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

    while (waiting) {
        if (this->renderer) {
            this->renderer->showSelectMenu(SelectType::GAME_MODE_UI);
        }

        if (SDL_WaitEvent(&event)) {
            if (waitForQuit(event)) {}

            if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;
                if (key == SDLK_1 || key == SDLK_KP_1) { playClickSFX(); choice = 1; waiting = false; }
                else if (key == SDLK_2 || key == SDLK_KP_2) { playClickSFX(); choice = 2; waiting = false; }
                else if (key == SDLK_3 || key == SDLK_KP_3) { playClickSFX(); choice = 3; waiting = false; }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;

                for (int i = 0; i < 3; i++) {
                    int currentBtnY = startY + i * spacing;
                    if (mx >= btnX && mx <= btnX + btnW && my >= currentBtnY && my <= currentBtnY + btnH) {
                        playClickSFX();
                        choice = i + 1; 
                        waiting = false;
                        break;
                    }
                }
            }
        }
    }

    if (choice >= 1 && choice <= 3) {
        if (choice == 1) *mode = GameMode::PVP;
        else if (choice == 2) *mode = GameMode::PVE;
        else if (choice == 3) *mode = GameMode::EVE;
        return true;
    }

    return false;
}

/**
 * Mô tả: Lấy input cấp độ bot.
 * Đầu vào:
 * - levels: mảng lưu cấp độ bot.
 * - index: vị trí bot cần gán.
 * Đầu ra:
 * - true nếu hợp lệ, false nếu không hợp lệ.
 * Tác dụng phụ:
 * - Cập nhật levels[index] nếu hợp lệ.
 */
bool SDLInteraction::selectBotLevel(BotLevel* levels, const int index) {
    SDL_Event event;
    bool waiting = true;
    int choice = 0;

    int btnW = 400;
    int btnH = 60;
    int btnX = (screenWidth - btnW) / 2;
    int startY = 280;
    int spacing = 80;

    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

    while (waiting) {
        if (this->renderer) {
            this->renderer->showSelectMenu(SelectType::BOT_LEVEL_UI, index);
        }

        if (SDL_WaitEvent(&event)) {
            if (waitForQuit(event)) {}

            if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key = event.key.keysym.sym;
                if (key == SDLK_1 || key == SDLK_KP_1) { playClickSFX(); choice = 1; waiting = false; }
                else if (key == SDLK_2 || key == SDLK_KP_2) { playClickSFX(); choice = 2; waiting = false; }
                else if (key == SDLK_3 || key == SDLK_KP_3) { playClickSFX(); choice = 3; waiting = false; }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;

                for (int i = 0; i < 3; i++) {
                    int currentBtnY = startY + i * spacing;
                    if (mx >= btnX && mx <= btnX + btnW && my >= currentBtnY && my <= currentBtnY + btnH) {
                        playClickSFX();
                        choice = i + 1;
                        waiting = false;
                        break;
                    }
                }
            }
        }
    }

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
 * - row: con trỏ lưu hàng.
 * - col: con trỏ lưu cột.
 * Đầu ra:
 * - true nếu lấy được input hợp lệ, false nếu không.
 * Tác dụng phụ:
 * - Cập nhật row, col nếu hợp lệ.
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

    // 🌟 KHỞI TẠO GIAO DIỆN TYPING: Hiển thị trạng thái sẵn sàng với ký tự trỏ "_" ngay đầu lượt
    if (this->renderer) {
        this->renderer->showTypingGameBuffer(inputBuffer);
    }

    while (waiting) {
        if (SDL_WaitEvent(&event)) {
            if (waitForQuit(event)) {}

            // A. XỬ LÝ GÕ PHÍM TRỰC TIẾP
            if (event.type == SDL_TEXTINPUT) {
                char c = event.text.text[0];
                // Hỗ trợ gõ các phím số và phím khoảng cách để tách tọa độ
                if ((c >= '0' && c <= '9') || c == ' ') {
                    playClickSFX();
                    inputBuffer += c;
                    std::cout << c << std::flush;

                    // 🌟 CẬP NHẬT TRỰC QUAN LUỒNG CHỮ VỪA GÕ LÊN MÀN HÌNH
                    if (this->renderer) {
                        this->renderer->showTypingGameBuffer(inputBuffer);
                    }
                }
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && !inputBuffer.empty()) {
                    playClickSFX();
                    inputBuffer.pop_back();
                    std::cout << "\b \b" << std::flush;

                    // 🌟 PHẢN HỒI GIAO DIỆN KHI NGƯỜI CHƠI XÓA LÙI KÝ TỰ
                    if (this->renderer) {
                        this->renderer->showTypingGameBuffer(inputBuffer);
                    }
                }
                else if (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER) {
                    std::cout << std::endl;
                    if (!inputBuffer.empty()) {
                        std::stringstream ss(inputBuffer);
                        int r = -1, c = -1;
                        if (ss >> r >> c) {
                            if (r >= 0 && r < size && c >= 0 && c < size) {
                                playClickSFX();
                                *row = r;
                                *col = c;
                                SDL_StopTextInput();

                                // 🌟 DỌN DẸP SẠCH SẼ Ô TYPING KHI ĐÃ ĐIỀN TỌA ĐỘ THÀNH CÔNG
                                if (this->renderer) {
                                    this->renderer->showTypingGameBuffer("");
                                }
                                return true;
                            } else {
                                std::cout << "[SDL Input Error] Cell out of bounds!\n[SDL Input] Try again: " << std::flush;
                                inputBuffer = "";
                                // Xóa rỗng ô Typing trên giao diện nếu nhập tọa độ vượt quá giới hạn
                                if (this->renderer) this->renderer->showTypingGameBuffer(inputBuffer);
                            }
                        } else {
                            std::cout << "[SDL Input Error] Invalid format!\n[SDL Input] Try again: " << std::flush;
                            inputBuffer = "";
                            // Xóa rỗng ô Typing trên giao diện nếu gõ sai cú pháp phân tách
                            if (this->renderer) this->renderer->showTypingGameBuffer(inputBuffer);
                        }
                    }
                }
            }
            // B. XỬ LÝ ĐIỀN TỌA ĐỘ BẰNG CLICK CHUỘT
            else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;

                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < size; j++) {
                        int cellX = startX + gap + j * (cellSizePx + gap);
                        int cellY = startY + gap + i * (cellSizePx + gap);

                        if (mx >= cellX && mx <= cellX + cellSizePx &&
                            my >= cellY && my <= cellY + cellSizePx) {
                            
                            playClickSFX();
                            *row = i;
                            *col = j;
                            
                            std::cout << "Mouse clicked on cell (" << i << ", " << j << ")" << std::endl;
                            SDL_StopTextInput();

                            // 🌟 DỌN DẸP SẠCH SẼ Ô TYPING NẾU NGƯỜI CHƠI CHỌN DÙNG CHUỘT
                            if (this->renderer) {
                                this->renderer->showTypingGameBuffer("");
                            }
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
 * - Giải phóng hoặc reset trạng thái input nếu cần.
 */
void SDLInteraction::close() {
    SDL_StopTextInput();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
    this->renderer = nullptr;
    std::cout << "[SDL Interaction] Closed safely." << std::endl;
}
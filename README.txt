========================================================================
TIC-TAC-TOE / CARO GAME V2 - README
========================================================================
------------------------------------------------------------------------
1. YÊU CẦU HỆ THỐNG & THƯ VIỆN (PREREQUISITES)
------------------------------------------------------------------------
- Trình biên dịch: Hỗ trợ tiêu chuẩn C++20 (GCC 10+, Clang 10+, hoặc MSVC 2019+).
- Thư viện đồ họa đa phương tiện SDL2 và các gói mở rộng bắt buộc:
  + SDL2 (Thư viện cốt lõi)
  + SDL2_ttf (Kết xuất phông chữ TrueType)
  + SDL2_mixer (Xử lý âm thanh nhạc nền và hiệu ứng)
- Thư mục tài nguyên `assets/` (phải nằm cùng cấp với tệp chạy):
  + assets/Montserrat-Bold.ttf, assets/Montserrat-Regular.ttf
  + assets/bgm.mp3, assets/click.wav, assets/hover.wav

------------------------------------------------------------------------
2. HƯỚNG DẪN BIÊN DỊCH (COMPILATION)
------------------------------------------------------------------------
Mở Terminal/Command Prompt tại thư mục chứa mã nguồn dự án và lựa chọn một trong hai hình thức biên dịch dưới đây:

Cách 1: Biên dịch nhanh bằng dòng lệnh g++ (GCC)
Chạy lệnh sau để biên dịch toàn bộ các tệp nguồn trong thư mục `src/` và liên kết với thư viện SDL2:

g++ -std=c++20 src/*.cpp -o main -lSDL2 -lSDL2_ttf -lSDL2_mixer

(Lưu ý: Đối với Windows dùng MinGW, hãy đảm bảo đã thêm đường dẫn chứa các file .h vào -I và tệp .a vào -L nếu hệ thống không tự nhận diện).

Cách 2: Biên dịch chuẩn hóa bằng CMake
Nếu dự án sử dụng cấu hình tệp `CMakeLists.txt`:
1. Tạo thư mục build và di chuyển vào trong:
   mkdir build && cd build
2. Cấu hình hệ thống build:
   cmake ..
3. Tiến hành biên dịch chương trình:
   cmake --build .

------------------------------------------------------------------------
3. HƯỚNG DẪN CHẠY CHƯƠNG TRÌNH (EXECUTION)
------------------------------------------------------------------------
Sau khi biên dịch thành công, một tệp thực thi (tên là `game` hoặc `game.exe`) sẽ được sinh ra. Bạn có thể chạy trực tiếp bằng cách nhấp đúp chuột hoặc khởi chạy thông qua Terminal kèm theo các cờ tham số cấu hình:

- Chạy chế độ Giao diện đồ họa GUI:
  ./game --gui

- Chạy chế độ Dòng lệnh Console:
  ./game

Các tham số dòng lệnh tùy chọn nâng cao hỗ trợ quản lý luồng:
  --verbose : Kích hoạt chế độ Log chi tiết (DEBUG mode) ra màn hình hoặc file log.txt để theo dõi thời gian thực thi của Engine và AI Bot.
  --help    : Hiển thị danh sách các cờ cấu hình được hệ thống hỗ trợ.

========================================================================

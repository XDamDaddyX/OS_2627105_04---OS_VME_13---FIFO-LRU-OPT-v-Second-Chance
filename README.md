# Page Replacement Simulator (OS_VME_13)

Mô phỏng các thuật toán thay trang FIFO, LRU, OPT và Second-Chance.
Môn Hệ điều hành (OS105) - Nhóm 4. Ngôn ngữ: C++.

## Thành viên và phân công

| Thành viên | Vai trò | Phần phụ trách chi tiết |
|---|---|---|
| Phạm Tiến Thành Nhân | Nhóm trưởng, Thuật toán Second-Chance | Quản lý repo, review Pull Request, theo dõi tiến độ, cài đặt Second-Chance, tổng hợp báo cáo |
| Ngô Văn Chung | Thuật toán & Output | Cài đặt thuật toán thay trang (phần được chia), xuất trace/kết quả ra CSV (`src/io`), test phần output |
| Tô Hoàng Vũ | Thuật toán & Input | Cài đặt thuật toán thay trang (phần được chia), đọc CSV và kiểm tra dữ liệu đầu vào |
| Võ Đăng Anh Tuấn | GUI & Hiệu năng | Dựng giao diện, hiển thị frame theo từng bước, đo hiệu năng và stress test |
| Bùi Ngọc Nhựt | Thuật toán & GUI | Cài đặt thuật toán thay trang (phần được chia), phối hợp dựng giao diện |

## Cấu trúc thư mục

| Thư mục | Nội dung |
|---|---|
| `src/core` | Thuật toán thuần C++, không gọi GUI |
| `src/io` | Đọc/ghi CSV, kiểm tra dữ liệu |
| `src/gui` | Giao diện |
| `input` | CSV đầu vào mẫu |
| `output` | CSV kết quả/trace mẫu |
| `tests` | Test case, kết quả mong đợi, test hiệu năng |
| `report` | Báo cáo |
| `docs` | Pseudocode, test plan, ghi chú thiết kế |

## Yêu cầu và cách cài, chạy

### Yêu cầu
- Windows 10/11 (64-bit)
- Ngôn ngữ: C++17
- GUI: Qt 6.x.x (Qt Widgets), bộ "MinGW 64-bit"
- Công cụ: Qt Creator, CMake, Ninja (cài cùng Qt bằng Qt Online Installer)

### Cài đặt
1. Tải Qt Online Installer từ trang chủ Qt, đăng nhập bằng tài khoản Qt miễn phí.
2. Chọn Qt 6.x.x > "MinGW 64-bit" và Developer and Designer Tools > Qt Creator, CMake, Ninja, MinGW 64-bit.
3. Cài xong, mở Qt Creator.

### Lấy mã nguồn
```
git clone https://github.com/XDamDaddyX/OS_2627105_04---OS_VME_13---FIFO-LRU-OPT-v-Second-Chance.git
```

### Build và chạy bằng Qt Creator
1. File > Open File or Project, chọn `CMakeLists.txt` ở thư mục gốc repo.
2. Chọn kit "Desktop Qt 6.x.x MinGW 64-bit", bấm Configure Project.
3. Bấm nút Run (tam giác xanh) để biên dịch và chạy.

(Lưu ý: `CMakeLists.txt` sẽ được bổ sung khi nhóm dựng khung dự án.)

### Chạy thử với dữ liệu mẫu
1. Mở ứng dụng, chọn file `input/sample_input.csv`.
2. Chọn thuật toán và bấm chạy.
3. Kết quả và trace được ghi vào thư mục `output/`.

### Chạy kiểm thử
(Cập nhật khi nhóm chốt công cụ kiểm thử.)

### Lỗi thường gặp
(Ghi dần theo thực tế khi nhóm gặp lỗi.)
## Định dạng CSV

Input (`input/sample_input.csv`):

| Cột | Ý nghĩa |
|---|---|
| `test_id` | Mã test |
| `frames` | Số frame |
| `reference_string` | Chuỗi tham chiếu trang, cách nhau bằng dấu cách |

Output trace (đề xuất, chốt lại trong nhóm):
`algorithm,step,page,frames_state,result,victim,faults_so_far`

## Nguồn tham khảo

[1] A. Silberschatz, P. B. Galvin, and G. Gagne, *Operating System Concepts*, 10th ed. Hoboken, NJ, USA: Wiley, 2018. (Chương Virtual Memory, mục Page Replacement)

[2] A. S. Tanenbaum and H. Bos, *Modern Operating Systems*, 4th ed. Upper Saddle River, NJ, USA: Pearson, 2015. (Chương Memory Management, mục Page Replacement Algorithms)

[3] Slide môn Hệ điều hành, Chapter 03: Memory Management, v2025, Trường Đại học Giao thông vận tải TP.HCM.

[4] L. A. Belady, R. A. Nelson, and G. S. Shedler, "An anomaly in space-time characteristics of certain programs running in a paging machine," *Communications of the ACM*, vol. 12, no. 6, pp. 349-353, 1969.

## Khai báo sử dụng AI

Nhóm sử dụng trợ lý AI (Claude, Anthropic,Chat GPT,Gemini,Google) cho các việc sau:
- Giải thích khái niệm và thuật toán thay trang để nhóm học tập.
- Đề xuất cấu trúc thư mục repo, mẫu README và bản nháp file khai báo giao diện `src/core/page_replacement.h`.

Cách nhóm kiểm chứng: (điền sau khi làm, ví dụ: đối chiếu kết quả với slide Chapter 03 và giáo trình, chạy tay trên chuỗi tham chiếu mẫu.)

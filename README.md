# Page Replacement Simulator — OS_VME_13

**Môn học:** Hệ điều hành (OS105) — Nhóm 4  
**Ngôn ngữ:** C++17  
**Giao diện:** Qt 6 Widgets  
**Build:** CMake + Ninja + MinGW 64-bit  
**Hệ điều hành:** Windows 10/11 (64-bit)

## 1. Giới thiệu dự án

Dự án mô phỏng bốn thuật toán thay trang trong quản lý bộ nhớ:

- **FIFO** — First-In, First-Out
- **LRU** — Least Recently Used
- **OPT** — Optimal Page Replacement
- **Second-Chance** — Clock Algorithm

**Mục tiêu sản phẩm:**

- Nhập chuỗi tham chiếu trang và số lượng frame hoặc đọc dữ liệu từ CSV.
- Mô phỏng từng bước, hiển thị trang đang truy cập, trạng thái frame, Page Hit, Page Fault và trang bị thay thế.
- Thống kê tổng số Hit/Fault và so sánh bốn thuật toán.
- Xuất kết quả và trace ra CSV.
- Có test case, kiểm thử hiệu năng và báo cáo.

**Trạng thái hiện tại:** Đã hoàn thành bộ khung Qt + CMake và các file thuật toán. Logic FIFO, LRU, OPT, Second-Chance, GUI mô phỏng hoàn chỉnh và đọc/ghi CSV vẫn cần được phát triển.

## 2. Phân công nhiệm vụ

| Thành viên | Nhiệm vụ chính | Branch |
|---|---|---|
| **Phạm Tiến Thành Nhân** | Nhóm trưởng, Second-Chance, review PR, tích hợp, tổng hợp báo cáo | `feature/second-chance` |
| **Bùi Ngọc Nhựt** | FIFO, kiểm thử FIFO, hỗ trợ chức năng điều khiển GUI | `feature/fifo` |
| **Tô Hoàng Vũ** | LRU, đọc CSV, kiểm tra dữ liệu đầu vào | `feature/lru-input` |
| **Ngô Văn Chung** | OPT, xuất kết quả/trace CSV, kiểm thử Output | `feature/opt-output` |
| **Võ Đăng Anh Tuấn** | GUI Qt, hiển thị frame từng bước, hiệu năng, stress test | `feature/gui` |

**Trách nhiệm của mỗi thành viên:**

1. Tìm hiểu và viết pseudocode cho phần được giao.
2. Lập trình chức năng.
3. Tự Build và kiểm thử.
4. Tạo test case và ghi nhận kết quả.
5. Tạo Pull Request để nhóm trưởng review.
6. Viết phần báo cáo tương ứng với công việc mình thực hiện.

Không giao toàn bộ việc test hoặc báo cáo cho một người.

**Phối hợp GUI:** Anh Tuấn là người phụ trách chính `src/gui/`. Ngọc Nhựt có thể hỗ trợ GUI nhưng phải thống nhất file và công việc với Tuấn trước khi chỉnh sửa.

## 3. Cấu trúc thư mục

```text
OS_VME_13/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── src/
│   ├── main.cpp
│   ├── core/
│   │   ├── page_replacement.h
│   │   ├── page_replacement.cpp
│   │   ├── fifo.cpp
│   │   ├── lru.cpp
│   │   ├── opt.cpp
│   │   └── second_chance.cpp
│   ├── gui/
│   └── io/
├── input/
├── output/
├── tests/
├── docs/
└── report/
```

| Thư mục | Chức năng |
|---|---|
| `src/core/` | Thuật toán C++ độc lập với GUI |
| `src/gui/` | Giao diện Qt Widgets |
| `src/io/` | Đọc, ghi và kiểm tra dữ liệu CSV |
| `input/` | Dữ liệu đầu vào mẫu |
| `output/` | Kết quả và trace |
| `tests/` | Test thuật toán, I/O, hiệu năng |
| `docs/` | Pseudocode, thiết kế, test plan |
| `report/` | Báo cáo dự án |

## 4. Hướng dẫn cài đặt môi trường

### 4.1. Cài Git

Tải Git cho Windows:

https://git-scm.com/downloads/win

Sau khi cài, mở PowerShell hoặc Terminal trong VS Code:

```powershell
git --version
```

Nếu cần cấu hình danh tính Git:

```powershell
git config --global user.name "Ten cua ban"
git config --global user.email "email-cua-ban@example.com"
```

Mỗi thành viên dùng tên và email GitHub của chính mình.

### 4.2. Cài Qt 6

1. Truy cập https://www.qt.io/download-qt-installer.
2. Tải **Qt Online Installer**.
3. Đăng nhập hoặc tạo tài khoản Qt.
4. Chọn phiên bản Qt 6.x, ưu tiên cùng phiên bản nhóm trưởng đang dùng là **Qt 6.12.0**.
5. Cài các thành phần:
   - Qt 6.x — **MinGW 64-bit**
   - Qt Creator
   - CMake
   - Ninja
   - MinGW 64-bit compiler
6. Hoàn tất cài đặt.

### 4.3. Mở project bằng Qt Creator

1. Mở **Qt Creator**.
2. Chọn **File → Open File or Project**.
3. Chọn `CMakeLists.txt` trong thư mục gốc repository.
4. Chọn kit **Desktop Qt 6.x MinGW 64-bit**.
5. Nhấn **Configure Project**.
6. Nhấn **Build**.
7. Nhấn **Run** để chạy chương trình.

**Lưu ý:** Không chọn nhầm kit MSVC hoặc compiler không tương thích với Qt MinGW.

Có thể lập trình bằng VS Code nhưng phải mở đúng thư mục gốc project và chọn đúng Qt MinGW kit.

Project dùng **C++17 + Qt Widgets**, không yêu cầu Node.js hay xây dựng website.

## 5. Clone GitHub về máy

Repository:

https://github.com/XDamDaddyX/OS_2627105_04---OS_VME_13---FIFO-LRU-OPT-v-Second-Chance

Mở PowerShell ở thư mục muốn lưu project:

```powershell
git clone https://github.com/XDamDaddyX/OS_2627105_04---OS_VME_13---FIFO-LRU-OPT-v-Second-Chance.git
```

Vào thư mục:

```powershell
cd OS_2627105_04---OS_VME_13---FIFO-LRU-OPT-v-Second-Chance
```

Chuyển sang branch tích hợp:

```powershell
git switch develop
git pull origin develop
```

**Quy định:** Không lập trình trực tiếp trên `main` hoặc `develop`. Mỗi thành viên phải tạo branch riêng từ `develop`.

## 6. Git Workflow bắt buộc

### 6.1. Phân loại branch

| Branch | Mục đích |
|---|---|
| `main` | Phiên bản ổn định để demo hoặc nộp bài |
| `develop` | Tích hợp code và kiểm thử |
| `feature/fifo` | Ngọc Nhựt |
| `feature/lru-input` | Hoàng Vũ |
| `feature/opt-output` | Văn Chung |
| `feature/gui` | Anh Tuấn |
| `feature/second-chance` | Nhân |
| `fix/...` | Sửa lỗi riêng |
| `docs/...` | Chỉnh sửa tài liệu |

**Quy trình:**

```text
feature/* 
    |
    | Pull Request
    v
 develop
    |
    | Build + Test + Review
    | Pull Request
    v
   main
```

Mọi PR tính năng phải có **base là `develop`**.

Chỉ đưa code từ `develop` lên `main` sau khi đã kiểm thử tích hợp.

### 6.2. Tạo branch làm việc

Ví dụ thành viên làm FIFO:

```powershell
git status
git switch develop
git pull origin develop
git switch -c feature/fifo
```

Các thành viên khác thay `feature/fifo` thành branch mình phụ trách.

**Lưu ý:** `git switch -c` dùng để tạo branch mới. Nếu branch đã tồn tại trên máy, chỉ cần `git switch ten-branch`.

### 6.3. Commit và Push

Sau khi sửa code, kiểm tra:

```powershell
git status
```

Chọn file thay đổi. Ví dụ:

```powershell
git add src/core/fifo.cpp
```

Commit:

```powershell
git commit -m "feat: implement FIFO page replacement"
```

Push lần đầu:

```powershell
git push -u origin feature/fifo
```

Những lần sau trên cùng branch:

```powershell
git push
```

**Quy định khi commit:**

- `feat:` — Thêm chức năng.
- `fix:` — Sửa lỗi.
- `docs:` — Sửa tài liệu.
- `test:` — Thêm hoặc sửa kiểm thử.
- `refactor:` — Cải tổ code mà không đổi chức năng.

Không dùng `git add .` khi chưa kiểm tra các file được đưa vào commit.

Không commit thư mục `build/`, file `.exe`, thông tin cá nhân, mật khẩu, token hoặc API key.

### 6.4. Tạo Pull Request

1. Vào GitHub → **Pull requests**.
2. Chọn **New pull request**.
3. Chọn:
   - **base:** `develop`
   - **compare:** branch cá nhân.
4. Đặt tiêu đề rõ ràng.
5. Mô tả chức năng đã hoàn thành.
6. Ghi rõ test case đã chạy và kết quả.
7. Gửi link PR cho nhóm trưởng.
8. Chờ review trước khi merge.

Ví dụ:

```text
Title: feat: implement FIFO algorithm

Base: develop
Compare: feature/fifo
```

Nếu PR bị yêu cầu sửa, thành viên sửa code trên feature branch rồi commit/push tiếp. Pull Request tự cập nhật.

**Không tự ý merge PR hoặc mở PR tính năng vào `main`.**

### 6.5. Lấy code mới nhất

Để cập nhật `develop`:

```powershell
git switch develop
git pull origin develop
```

Để đưa thay đổi mới từ `develop` vào branch đang làm:

```powershell
git switch feature/fifo
git merge develop
```

Nếu xảy ra **merge conflict**, trao đổi với nhóm trưởng và người phụ trách file liên quan trước khi giải quyết.

Không tự ý xóa hay ghi đè code của người khác.

## 7. Quy định chỉnh sửa file

### 7.1. File được chủ động chỉnh sửa

| Thành viên | File/phần việc |
|---|---|
| Ngọc Nhựt | `src/core/fifo.cpp`, test và tài liệu FIFO |
| Hoàng Vũ | `src/core/lru.cpp`, phần đọc CSV trong `src/io/` |
| Văn Chung | `src/core/opt.cpp`, phần ghi CSV trong `src/io/` |
| Anh Tuấn | File thuộc `src/gui/`, test hiệu năng |
| Nhân | `src/core/second_chance.cpp`, tích hợp và review |

Các thành viên có thể tạo thêm test hoặc tài liệu cho phần mình phụ trách.

### 7.2. File KHÔNG được tự ý chỉnh sửa

**Các file chung cần trao đổi trước khi sửa:**

| File | Lý do |
|---|---|
| `CMakeLists.txt` | Cấu hình build chung; sửa sai có thể khiến mọi người build lỗi |
| `src/main.cpp` | Điểm khởi động Qt, liên quan tích hợp GUI |
| `src/core/page_replacement.h` | Khai báo cấu trúc và giao diện của tất cả thuật toán |
| `src/core/page_replacement.cpp` | Điều phối chung bốn thuật toán |
| `README.md` | Quy định và hướng dẫn của nhóm |
| `.gitignore` | Cấu hình Git chung |
| `tests/expected_results.csv` | Dữ liệu kết quả chuẩn |
| `input/sample_input.csv` | Dữ liệu đầu vào mẫu dùng chung |

Ngoài ra, không tự ý chỉnh sửa file thuộc phần việc của thành viên khác.

**Quy trình khi cần sửa file chung:**

1. Thông báo lý do cần sửa.
2. Trao đổi với nhóm trưởng và người liên quan.
3. Thống nhất cách thay đổi.
4. Sửa trên branch riêng.
5. Tạo PR vào `develop`.
6. Review và kiểm thử trước khi merge.

Quy định này không cấm sửa file chung vĩnh viễn, mà ngăn những thay đổi thiếu phối hợp.

### 7.3. Các hành vi không được phép

- Push trực tiếp lên `main` hoặc `develop`.
- Tự xóa các branch chung.
- Force push lên các branch chung.
- Merge code chưa được thống nhất.
- Chỉnh sửa hoặc xóa code của người khác mà không thông báo.
- Đưa file build, mật khẩu hoặc thông tin nhạy cảm lên GitHub.

## 8. Quy chuẩn code thuật toán

Bốn thuật toán sử dụng chung hợp đồng trong:

`src/core/page_replacement.h`

### Cấu trúc Step

```cpp
struct Step {
    int index;
    int page;
    std::vector<int> frames;
    bool hit;
    int victim;
    int faultsSoFar;
};
```

### Cấu trúc Result

```cpp
struct Result {
    Algorithm algo;
    std::vector<Step> steps;
    int faults;
    int hits;
};
```

### Các hàm cần triển khai

```cpp
Result runFIFO(const std::vector<int>& refs, int frameCount);

Result runLRU(const std::vector<int>& refs, int frameCount);

Result runOPT(const std::vector<int>& refs, int frameCount);

Result runSecondChance(const std::vector<int>& refs, int frameCount);
```

Hàm điều phối chung:

```cpp
Result simulate(
    Algorithm algo,
    const std::vector<int>& refs,
    int frameCount
);
```

**Quy định kết quả:**

- `index` bắt đầu từ `1`.
- `frames` là trạng thái frame sau mỗi bước.
- `-1` đại diện cho frame trống.
- `hit = true` nếu trang đã nằm trong bộ nhớ.
- `victim = -1` nếu không có trang nào bị thay thế.
- `faultsSoFar` là số Page Fault cộng dồn.
- `faults + hits` phải bằng số lượt tham chiếu đối với input hợp lệ.
- Số lượng frame phải lớn hơn 0.

Không tự ý thay đổi cấu trúc `Step`, `Result` hay chữ ký hàm khi chưa thống nhất trong nhóm.

## 9. Input và Output

### Input CSV

File mẫu:

`input/sample_input.csv`

| Cột | Ý nghĩa |
|---|---|
| `test_id` | Mã test |
| `frames` | Số frame |
| `reference_string` | Chuỗi tham chiếu trang |

Hoàng Vũ phụ trách đọc file, kiểm tra dữ liệu và thông báo lỗi nếu đầu vào không hợp lệ.

### Output CSV

Định dạng trace đề xuất:

```text
algorithm,step,page,frames_state,result,victim,faults_so_far
```

Văn Chung phụ trách xuất kết quả.

Cách biểu diễn `frames_state` phải được nhóm thống nhất trước khi tích hợp.

## 10. Quy định kiểm thử

Mỗi thành viên phải tự kiểm thử phần mình đã lập trình.

**Một Pull Request được xem xét merge khi:**

1. Code Build thành công.
2. Không phát sinh lỗi biên dịch.
3. Đã có ít nhất các test case phù hợp với chức năng.
4. Kết quả khớp với tính toán hoặc dữ liệu mong đợi.
5. Có mô tả đầy đủ cách kiểm thử trong PR.
6. Đã trao đổi trước nếu sửa file chung.

**Kiểm thử tích hợp trên `develop`:**

- Kiểm tra cả bốn thuật toán trên cùng bộ dữ liệu.
- Kiểm tra Page Hit, Page Fault và Victim.
- Kiểm tra trạng thái frame theo từng bước.
- Kiểm tra đọc/ghi CSV.
- Kiểm tra GUI khi chạy nhiều lần.
- Kiểm tra dữ liệu không hợp lệ.
- Chạy stress test và kiểm tra hiệu năng.

Hiện nhóm chưa thiết lập CI/CD hoàn chỉnh. Có thể bổ sung CTest và GitHub Actions sau khi thống nhất bộ test.

## 11. Tiến độ dự án

- [x] Tạo repository và cấu trúc thư mục.
- [x] Khởi tạo Qt Widgets + CMake.
- [x] Tạo bộ khung các thuật toán.
- [x] Tạo branch `develop`.
- [ ] Hoàn thành FIFO.
- [ ] Hoàn thành LRU.
- [ ] Hoàn thành OPT.
- [ ] Hoàn thành Second-Chance.
- [ ] Hoàn thành GUI mô phỏng.
- [ ] Hoàn thành đọc/ghi CSV.
- [ ] Kiểm thử tích hợp và hiệu năng.
- [ ] Hoàn thành báo cáo.
- [ ] Chuẩn bị demo.

## 12. Báo cáo và tài liệu

Mỗi thành viên đóng góp phần tài liệu tương ứng với nhiệm vụ:

- Giới thiệu chức năng hoặc thuật toán.
- Nguyên lý hoạt động.
- Pseudocode.
- Mô tả cách triển khai.
- Ví dụ minh họa.
- Test case và kết quả.
- Những lỗi phát sinh và cách khắc phục.

Nhóm trưởng tổng hợp và chuẩn hóa báo cáo.

**Nguồn tham khảo:**

1. A. Silberschatz, P. B. Galvin, and G. Gagne, *Operating System Concepts*, 10th ed., Wiley, 2018.
2. A. S. Tanenbaum and H. Bos, *Modern Operating Systems*, 4th ed., Pearson, 2015.
3. Slide Hệ điều hành, Chapter 03: Memory Management, v2025, Trường Đại học Giao thông vận tải TP.HCM.
4. L. A. Belady, R. A. Nelson, and G. S. Shedler, “An anomaly in space-time characteristics of certain programs running in a paging machine,” *Communications of the ACM*, vol. 12, no. 6, pp. 349–353, 1969.

## 13. Khai báo sử dụng AI

Nhóm có thể sử dụng công cụ AI để:

- Giải thích kiến thức về hệ điều hành và thuật toán.
- Tham khảo cấu trúc chương trình và giải pháp thiết kế.
- Gợi ý kiểm thử và phát hiện lỗi.
- Hỗ trợ rà soát code và tài liệu.

Tuy nhiên, mỗi thành viên phải hiểu, tự kiểm chứng và chịu trách nhiệm về code mình đóng góp.

Kết quả được đối chiếu với giáo trình, slide môn học, test case tính tay và quá trình chạy thử. Việc sử dụng AI được khai báo theo yêu cầu của giảng viên.

---

**Nguyên tắc chung của nhóm:**

**Mỗi người làm phần mình → Commit/Push lên feature branch → Tạo PR vào develop → Review và kiểm thử → Đưa bản ổn định lên main.**

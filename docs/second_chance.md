# Thuật toán Thay thế Trang Second-Chance (Clock Algorithm)

## 1. Giới thiệu tổng quan

- **Người phụ trách:** Phạm Tiến Thành Nhân (Nhóm trưởng)
- **Tập tin nguồn:** `src/core/second_chance.cpp`
- **Tập tin kiểm thử:** `tests/test_second_chance.cpp`
- **Mục tiêu:** Mô phỏng giải thuật thay thế trang **Second-Chance** (còn gọi là **Thuật toán Đồng hồ - Clock Algorithm**), một giải thuật xấp xỉ tối ưu giữa tính đơn giản của FIFO và hiệu quả tiệm cận của LRU.

---

## 2. Nguyên lý hoạt động

Thuật toán Second-Chance là phiên bản cải tiến của FIFO bằng cách gắn thêm cho mỗi khung trang (frame) một **bit tham chiếu (reference bit / second-chance bit)** có giá trị `0` hoặc `1`:

1. **Khởi tạo:**
   - Tất cả các frame trong bộ nhớ ban đầu có trạng thái trống (`-1`) và reference bit là `0`.
   - Một con trỏ đồng hồ (`pointer / clock hand`) trỏ vào frame đầu tiên (`index = 0`).

2. **Khi một trang được truy cập (Page Access):**
   - **Trường hợp Page Hit (Trang đã có sẵn trong khung nhớ):**
     - Đặt `reference bit` của frame chứa trang đó thành `1` (cấp cho trang này thêm một "cơ hội thứ hai").
     - Vị trí con trỏ đồng hồ không đổi.
     - Tăng biến đếm `hits`.
   - **Trường hợp Page Fault (Trang chưa có trong khung nhớ):**
     - Tăng biến đếm `faults`.
     - **Nếu còn frame trống:**
       - Nạp trang mới vào frame trống đầu tiên.
       - Gán `reference bit = 1`.
       - Di chuyển con trỏ sang vị trí kế tiếp: `pointer = (emptyIndex + 1) % frameCount`.
     - **Nếu các frame đều đã đầy (Cần thay thế trang - Replacement):**
       - Con trỏ duyệt vòng tròn các frame theo chiều kim đồng hồ:
         - Nếu `reference bit == 1`: Xóa bit về `0` (trang đã dùng hết cơ hội thứ hai) và tăng con trỏ lên vị trí kế tiếp (`pointer = (pointer + 1) % frameCount`).
         - Nếu `reference bit == 0`: Chọn frame này làm **Victim** (trang bị loại bỏ).
       - Nạp trang mới vào vị trí victim vừa chọn.
       - Đặt lại `reference bit = 1`.
       - Di chuyển con trỏ đồng hồ sang vị trí kế tiếp: `pointer = (victimIndex + 1) % frameCount`.

---

## 3. Mã giả (Pseudocode)

```text
Function runSecondChance(refs: List of Int, frameCount: Int) -> Result:
    Khởi tạo frames: Mảng frameCount phần tử, gán -1
    Khởi tạo refBits: Mảng frameCount phần tử, gán 0
    pointer = 0
    faults = 0, hits = 0
    steps = Empty List

    Với mỗi page trong refs:
        hitIndex = Tìm page trong frames
        victim = -1

        Nếu hitIndex != -1 (Page Hit):
            refBits[hitIndex] = 1
            hits += 1
            hit = True
        Ngược lại (Page Fault):
            faults += 1
            hit = False
            emptyIndex = Tìm ô -1 trong frames

            Nếu emptyIndex != -1:
                frames[emptyIndex] = page
                refBits[emptyIndex] = 1
                pointer = (emptyIndex + 1) mod frameCount
            Ngược lại:
                Lặp vô hạn:
                    Nếu refBits[pointer] == 0:
                        victim = frames[pointer]
                        frames[pointer] = page
                        refBits[pointer] = 1
                        pointer = (pointer + 1) mod frameCount
                        Thoát lặp
                    Ngược lại:
                        refBits[pointer] = 0
                        pointer = (pointer + 1) mod frameCount

        Ghi nhận Step(page, frames, hit, victim, faults) vào steps

    Trả về Result(Algorithm::SecondChance, steps, faults, hits)
```

---

## 4. Kiểm thử và Đối chiếu (Testing & Verification)

### 4.1 Chuỗi tham chiếu mẫu (Standard Benchmark từ Sách Silberschatz)
- **Reference String:** `7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1`
- **Số Frames:** `3`

**Kết quả chạy thực tế qua unit test:**
- **Tổng số lần truy xuất:** 20
- **Page Hits:** 6
- **Page Faults:** 14
- **Tỉ lệ lỗi trang (Fault Rate):** $\frac{14}{20} = 70\%$

### 4.2 Các trường hợp biên (Edge Cases)
- **Chuỗi rỗng:** `refs = []` $\to$ Hits = 0, Faults = 0.
- **Khung nhớ kích thước tối thiểu:** `frameCount = 1` $\to$ Thay thế liên tục khi khác trang.
- **Truy cập trang trùng lặp liên tục:** `refs = [2, 2, 2, 2]` $\to$ 1 Fault ban đầu và 3 Hits liên tiếp.

---

## 5. Đóng góp vào Báo cáo Đồ án

| Mục báo cáo | Nội dung đóng góp |
|---|---|
| **Mục 2: Cơ sở lý thuyết** | Đặc tả giải thuật Clock, ưu nhược điểm so với FIFO và LRU |
| **Mục 3: Hiện thực thuật toán** | Giải thích mã nguồn C++ `second_chance.cpp`, cấu trúc vòng lặp con trỏ modulo |
| **Mục 4: Đánh giá & So sánh** | Bảng số liệu đối sánh số lần Faults giữa FIFO (15 faults), Second-Chance (14 faults), LRU (12 faults) và OPT (9 faults) |

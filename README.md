# Page Replacement Simulator (OS_VME_13)

Mô phỏng các thuật toán thay trang FIFO, LRU, OPT và Second-Chance.
Môn Hệ điều hành (OS105) - Nhóm 4. Ngôn ngữ: C++.

## Thành viên và phân công

| Thành viên | Vai trò / phần phụ trách |
|---|---|
|  |  |

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

(Điền sau khi chốt công cụ build và thư viện GUI.)

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

(Ghi theo IEEE hoặc APA, tối thiểu 4 nguồn.)

## Khai báo sử dụng AI

(Ghi rõ phần nào dùng AI và cách nhóm đã kiểm chứng.)

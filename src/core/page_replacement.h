// Hợp đồng giữa lõi thuật toán, I/O và GUI. Cả nhóm cần thống nhất trước khi code.
#pragma once
#include <vector>

enum class Algorithm { FIFO, LRU, OPT, SecondChance };

struct Step {
    int index;                // thứ tự lượt truy cập (bắt đầu từ 1)
    int page;                 // trang được truy cập
    std::vector<int> frames;  // nội dung frame sau bước này, -1 là ô trống
    bool hit;                 // true nếu hit, false nếu page fault
    int victim;               // trang bị thay, -1 nếu không có
    int faultsSoFar;          // số page fault tính đến bước này
};

struct Result {
    Algorithm algo;
    std::vector<Step> steps;
    int faults;
    int hits;
};

// Chạy mô phỏng một thuật toán trên reference string với frameCount frame.
Result simulate(Algorithm algo, const std::vector<int>& refs, int frameCount);

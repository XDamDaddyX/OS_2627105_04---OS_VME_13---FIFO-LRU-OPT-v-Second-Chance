
#pragma once

#include <string>
#include <vector>

struct InputCase {
    std::string testId;
    int frameCount;
    std::vector<int> refs;
};

// Đọc các bộ dữ liệu từ CSV.
// Ném std::runtime_error nếu file hoặc dữ liệu không hợp lệ.
std::vector<InputCase> loadInputCsv(const std::string& filePath);

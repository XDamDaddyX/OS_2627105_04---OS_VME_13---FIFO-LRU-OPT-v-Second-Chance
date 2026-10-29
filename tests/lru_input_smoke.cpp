
#include "page_replacement.h"
#include "input_csv.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

int main()
{
    // Test 1: kiểm tra LRU.
    const auto result = runLRU({1, 2, 3, 1, 4}, 3);

    assert(result.algo == Algorithm::LRU);
    assert(result.faults == 4);
    assert(result.hits == 1);
    assert(result.steps.size() == 5);
    assert(result.steps.back().victim == 2);

    // Kiểm tra frame sau lần truy cập cuối.
    assert((result.steps.back().frames == std::vector<int>{1, 4, 3}));

    // Test 2: một frame, mọi lần truy cập đều fault.
    const auto oneFrame = runLRU({1, 2, 1, 2}, 1);
    assert(oneFrame.faults == 4);
    assert(oneFrame.hits == 0);

    // Test 3: từ chối số frame không hợp lệ.
    bool rejected = false;
    try {
        runLRU({1, 2}, 0);
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    assert(rejected);

    // Test 4: đọc CSV mẫu.
    const auto cases = loadInputCsv("input/sample_input.csv");
    assert(cases.size() == 3);
    assert(cases[0].testId == "T01");
    assert(cases[0].frameCount == 3);
    assert(cases[0].refs.size() == 20);

    std::cout << "All smoke tests passed.\n";
    return 0;
}

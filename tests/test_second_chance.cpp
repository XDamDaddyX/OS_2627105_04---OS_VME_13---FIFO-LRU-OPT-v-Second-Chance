#include <iostream>
#include <cassert>
#include <vector>
#include "../src/core/page_replacement.h"

// Prototype của runSecondChance để test độc lập
Result runSecondChance(const std::vector<int>& refs, int frameCount);

void testBasicSecondChance()
{
    // Ví dụ kinh điển trong sách Giáo trình Silberschatz:
    // Reference string: 7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1
    // Số frames = 3
    std::vector<int> refs = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
    int frameCount = 3;

    Result res = runSecondChance(refs, frameCount);

    std::cout << "[TEST 1] Standard reference string Silberschatz:" << std::endl;
    std::cout << "Total accesses: " << refs.size() << std::endl;
    std::cout << "Hits: " << res.hits << ", Faults: " << res.faults << std::endl;

    assert(res.hits + res.faults == (int)refs.size());
    assert(res.steps.size() == refs.size());

    // In chi tiết từng bước
    std::cout << "Step | Page | Hit/Fault | Victim | Frames" << std::endl;
    std::cout << "-----------------------------------------" << std::endl;
    for (const auto& s : res.steps) {
        std::cout << s.index << "\t" << s.page << "\t" << (s.hit ? "HIT " : "FAULT") << "\t" << s.victim << "\t[ ";
        for (int f : s.frames) {
            std::cout << f << " ";
        }
        std::cout << "]" << std::endl;
    }
}

void testEdgeCases()
{
    std::cout << "\n[TEST 2] Edge Cases..." << std::endl;

    // Test chuỗi rỗng
    std::vector<int> emptyRefs;
    Result resEmpty = runSecondChance(emptyRefs, 3);
    assert(resEmpty.hits == 0 && resEmpty.faults == 0);

    // Test số frame = 1
    std::vector<int> singleFrameRefs = {1, 2, 1, 3, 1};
    Result resSingle = runSecondChance(singleFrameRefs, 1);
    assert(resSingle.faults == 5); // Mỗi lần trang mới vào frame size 1 đều fault

    // Test lặp trang (toàn hit sau lần đầu)
    std::vector<int> repeatRefs = {2, 2, 2, 2};
    Result resRepeat = runSecondChance(repeatRefs, 2);
    assert(resRepeat.faults == 1 && resRepeat.hits == 3);

    std::cout << "Edge cases passed successfully!" << std::endl;
}

int main()
{
    std::cout << "================ RUNNING SECOND-CHANCE TESTS ================" << std::endl;
    testBasicSecondChance();
    testEdgeCases();
    std::cout << "\nALL SECOND-CHANCE TESTS PASSED!" << std::endl;
    return 0;
}

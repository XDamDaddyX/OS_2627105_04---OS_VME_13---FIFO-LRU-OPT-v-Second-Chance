
#include "page_replacement.h"

#include <algorithm>
#include <stdexcept>
#include <vector>

Result runLRU(const std::vector<int>& refs, int frameCount)
{
    if (frameCount <= 0) {
        throw std::invalid_argument(
            "Frame count must be greater than zero"
        );
    }

    Result result;
    result.algo = Algorithm::LRU;
    result.faults = 0;
    result.hits = 0;

    // -1 biểu thị frame đang trống.
    std::vector<int> frames(frameCount, -1);

    // lastUsed[i] là thời điểm gần nhất frame i được truy cập.
    // -1 có nghĩa là frame chưa chứa trang nào.
    std::vector<int> lastUsed(frameCount, -1);

    for (int i = 0; i < static_cast<int>(refs.size()); ++i) {
        const int page = refs[i];

        // Tìm xem trang đã có trong bộ nhớ hay chưa.
        auto it = std::find(frames.begin(), frames.end(), page);

        const bool hit = (it != frames.end());
        int victim = -1;

        if (hit) {
            // Page hit: cập nhật thời điểm truy cập gần nhất.
            const int frameIndex =
                static_cast<int>(it - frames.begin());

            lastUsed[frameIndex] = i;
            ++result.hits;
        } else {
            // Page fault.
            ++result.faults;

            // Ưu tiên sử dụng frame còn trống.
            auto emptyIt = std::find(frames.begin(), frames.end(), -1);
            int frameIndex;

            if (emptyIt != frames.end()) {
                frameIndex =
                    static_cast<int>(emptyIt - frames.begin());
            } else {
                // Không còn frame trống: thay trang lâu nhất
                // chưa được sử dụng.
                frameIndex = static_cast<int>(
                    std::min_element(
                        lastUsed.begin(),
                        lastUsed.end()
                    ) - lastUsed.begin()
                );

                victim = frames[frameIndex];
            }

            frames[frameIndex] = page;
            lastUsed[frameIndex] = i;
        }

        // Lưu trạng thái sau mỗi lần truy cập trang.
        Step step;
        step.index = i + 1;
        step.page = page;
        step.frames = frames;
        step.hit = hit;
        step.victim = victim;
        step.faultsSoFar = result.faults;

        result.steps.push_back(step);
    }

    return result;
}

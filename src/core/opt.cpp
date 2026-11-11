#include "page_replacement.h"
#include <stdexcept>
#include <vector>
#include <algorithm>

/**
 * @brief Cài đặt thuật toán thay thế trang Tối ưu OPT (Optimal Page Replacement)
 * 
 * Nguyên lý:
 * - Thay thế trang sẽ KHÔNG được sử dụng trong khoảng thời gian lâu nhất ở tương lai.
 * - Cho tỉ lệ Page Fault thấp nhất có thể (dùng làm chuẩn đối sánh lý thuyết).
 */
Result runOPT(const std::vector<int>& refs, int frameCount)
{
    if (frameCount <= 0) {
        throw std::invalid_argument("Frame count must be greater than zero");
    }

    for (int page : refs) {
        if (page < 0) {
            throw std::invalid_argument("Page numbers must be non-negative");
        }
    }

    Result result;
    result.algo = Algorithm::OPT;
    result.faults = 0;
    result.hits = 0;

    std::vector<int> frames(frameCount, -1);
    int n = static_cast<int>(refs.size());

    for (int stepIndex = 0; stepIndex < n; ++stepIndex) {
        int page = refs[stepIndex];
        Step step;
        step.index = stepIndex + 1;
        step.page = page;
        step.victim = -1;

        bool hit = false;
        for (int i = 0; i < frameCount; ++i) {
            if (frames[i] == page) {
                hit = true;
                break;
            }
        }

        if (hit) {
            step.hit = true;
            result.hits++;
        } else {
            step.hit = false;
            result.faults++;

            int emptyIndex = -1;
            for (int i = 0; i < frameCount; ++i) {
                if (frames[i] == -1) {
                    emptyIndex = i;
                    break;
                }
            }

            if (emptyIndex != -1) {
                frames[emptyIndex] = page;
            } else {
                // Tìm frame mà trang của nó xuất hiện xa nhất ở tương lai (hoặc không bao giờ xuất hiện lại)
                int optVictimIndex = -1;
                int farthestNextUse = -1;

                for (int i = 0; i < frameCount; ++i) {
                    int currentPage = frames[i];
                    int nextUse = -1;

                    // Duyệt các bước tiếp theo trong tương lai
                    for (int j = stepIndex + 1; j < n; ++j) {
                        if (refs[j] == currentPage) {
                            nextUse = j;
                            break;
                        }
                    }

                    // Nếu trang này không bao giờ được dùng lại ở tương lai
                    if (nextUse == -1) {
                        optVictimIndex = i;
                        break;
                    }

                    if (nextUse > farthestNextUse) {
                        farthestNextUse = nextUse;
                        optVictimIndex = i;
                    }
                }

                step.victim = frames[optVictimIndex];
                frames[optVictimIndex] = page;
            }
        }

        step.frames = frames;
        step.faultsSoFar = result.faults;
        result.steps.push_back(step);
    }

    return result;
}

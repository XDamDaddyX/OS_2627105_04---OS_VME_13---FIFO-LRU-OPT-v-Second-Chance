
#include "page_replacement.h"
#include <stdexcept>
#include <queue>
#include <algorithm>
/**
 * @brief Simulates FIFO page replacement.
 *
 * @param refs Sequence of referenced pages.
 * @param frameCount Number of available memory frames.
 * @return Simulation result with hits, faults and steps.
 * @throws std::invalid_argument If frameCount is not positive.
 */

Result runFIFO(const std::vector<int>& refs, int frameCount)
{
    if (frameCount <= 0) {
        throw std::invalid_argument("Frame count must be positive");
    }

for (int page : refs) {
    if (page < 0) {
        throw std::invalid_argument(
            "Page numbers must be non-negative"
        );
    }
}


    Result result{};
    result.algo = Algorithm::FIFO;
    result.faults = 0;
    result.hits = 0;

    std::vector<int> frames(frameCount, -1);
    std::queue<int> fifoQueue;

    for (int i = 0; i < static_cast<int>(refs.size()); ++i) {
        int page = refs[i];

        Step step{};
        step.index = i + 1;
        step.page = page;
        step.victim = -1;

        auto it = std::find(frames.begin(), frames.end(), page);
        step.hit = (it != frames.end());

        if (step.hit) {
            result.hits++;
        } else {
            result.faults++;

            auto empty = std::find(frames.begin(), frames.end(), -1);

            if (empty != frames.end()) {
                *empty = page;
                fifoQueue.push(page);
            } else {
                int victim = fifoQueue.front();
                fifoQueue.pop();

                step.victim = victim;

                auto pos = std::find(frames.begin(), frames.end(), victim);
                *pos = page;

                fifoQueue.push(page);
            }
        }

        step.frames = frames;
        step.faultsSoFar = result.faults;

        result.steps.push_back(step);
    }

    return result;
}
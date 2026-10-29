
#include "page_replacement.h"
#include <stdexcept>

// Khai bao cac ham thuat toan
// Moi thanh vien se cai dat mot ham rieng

Result runFIFO(const std::vector<int>& refs, int frameCount);

Result runLRU(const std::vector<int>& refs, int frameCount);

Result runOPT(const std::vector<int>& refs, int frameCount);

Result runSecondChance(const std::vector<int>& refs, int frameCount);

// Ham dieu phoi chinh
Result simulate(Algorithm algo,
                const std::vector<int>& refs,
                int frameCount)
{
    // Kiem tra so frame hop le
    if (frameCount <= 0) {
        throw std::invalid_argument(
            "Frame count must be greater than zero"
        );
    }

    // Chon thuat toan can chay
    switch (algo) {
        case Algorithm::FIFO:
            return runFIFO(refs, frameCount);

        case Algorithm::LRU:
            return runLRU(refs, frameCount);

        case Algorithm::OPT:
            return runOPT(refs, frameCount);

        case Algorithm::SecondChance:
            return runSecondChance(refs, frameCount);
    }

    throw std::invalid_argument("Unknown algorithm");
}

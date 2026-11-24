
#include "output_csv.h"

#include <fstream>
#include <sstream>
#include <string>

namespace {

const char* getAlgorithmName(Algorithm algo) {
    switch (algo) {
    case Algorithm::FIFO:
        return "FIFO";
    case Algorithm::LRU:
        return "LRU";
    case Algorithm::OPT:
        return "OPT";
    case Algorithm::SecondChance:
        return "SecondChance";
    default:
        return "Unknown";
    }
}

std::string framesToString(const std::vector<int>& frames) {
    std::ostringstream result;

    for (std::size_t i = 0; i < frames.size(); ++i) {
        if (i > 0) {
            result << ' ';
        }

        result << frames[i];
    }

    return "\"" + result.str() + "\"";
}

}

bool exportTraceCsv(const Result& result,
                    const std::string& filePath) {
    std::ofstream file(filePath);

    if (!file.is_open()) {
        return false;
    }

    file << "algorithm,step,page,frames_state,"
            "result,victim,faults_so_far\n";

    for (const Step& step : result.steps) {
        file << getAlgorithmName(result.algo) << ','
             << step.index << ','
             << step.page << ','
             << framesToString(step.frames) << ','
             << (step.hit ? "HIT" : "FAULT") << ','
             << step.victim << ','
             << step.faultsSoFar << '\n';
    }

    return file.good();
}

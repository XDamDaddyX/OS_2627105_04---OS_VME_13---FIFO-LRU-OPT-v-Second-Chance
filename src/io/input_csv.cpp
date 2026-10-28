
#include "input_csv.h"

#include <charconv>
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

namespace {

std::string trim(const std::string& value)
{
    std::size_t first = 0;
    while (first < value.size() &&
           std::isspace(static_cast<unsigned char>(value[first]))) {
        ++first;
    }

    std::size_t last = value.size();
    while (last > first &&
           std::isspace(static_cast<unsigned char>(value[last - 1]))) {
        --last;
    }

    return value.substr(first, last - first);
}

std::vector<std::string> parseCsvLine(const std::string& line)
{
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;

    for (std::size_t i = 0; i < line.size(); ++i) {
        const char ch = line[i];

        if (ch == '"') {
            if (inQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                field += '"';
                ++i;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (ch == ',' && !inQuotes) {
            fields.push_back(trim(field));
            field.clear();
        } else {
            field += ch;
        }
    }

    if (inQuotes) {
        throw std::runtime_error("Unclosed quote in CSV row");
    }

    fields.push_back(trim(field));
    return fields;
}

int parseInteger(const std::string& value, const std::string& fieldName)
{
    const std::string text = trim(value);

    if (text.empty()) {
        throw std::runtime_error(fieldName + " must not be empty");
    }

    int number = 0;
    const char* begin = text.data();
    const char* end = begin + text.size();

    const auto result = std::from_chars(begin, end, number);

    if (result.ec != std::errc{} || result.ptr != end) {
        throw std::runtime_error(
            fieldName + " must be a valid integer: " + text
        );
    }

    return number;
}

std::vector<int> parseReferenceString(const std::string& value)
{
    std::istringstream stream(value);
    std::vector<int> refs;
    std::string token;

    while (stream >> token) {
        const int page = parseInteger(token, "reference_string");

        if (page < 0) {
            throw std::runtime_error(
                "Page numbers must be non-negative"
            );
        }

        refs.push_back(page);
    }

    if (refs.empty()) {
        throw std::runtime_error("reference_string must not be empty");
    }

    return refs;
}

} // namespace

std::vector<InputCase> loadInputCsv(const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open CSV file: " + filePath);
    }

    std::string line;
    std::size_t lineNumber = 0;

    // Đọc dòng tiêu đề.
    if (!std::getline(file, line)) {
        throw std::runtime_error("CSV file is empty");
    }
    ++lineNumber;

    // Loại bỏ UTF-8 BOM nếu có.
    if (line.size() >= 3 &&
        static_cast<unsigned char>(line[0]) == 0xEF &&
        static_cast<unsigned char>(line[1]) == 0xBB &&
        static_cast<unsigned char>(line[2]) == 0xBF) {
        line.erase(0, 3);
    }

    const auto header = parseCsvLine(line);

    if (header.size() != 3 ||
        header[0] != "test_id" ||
        header[1] != "frames" ||
        header[2] != "reference_string") {
        throw std::runtime_error(
            "CSV header must be: test_id,frames,reference_string"
        );
    }

    std::vector<InputCase> cases;
    std::unordered_set<std::string> testIds;

    while (std::getline(file, line)) {
        ++lineNumber;

        if (trim(line).empty()) {
            continue;
        }

        try {
            const auto fields = parseCsvLine(line);

            if (fields.size() != 3) {
                throw std::runtime_error(
                    "Expected exactly 3 CSV columns"
                );
            }

            const std::string testId = trim(fields[0]);

            if (testId.empty()) {
                throw std::runtime_error("test_id must not be empty");
            }

            if (!testIds.insert(testId).second) {
                throw std::runtime_error("Duplicate test_id: " + testId);
            }

            const int frameCount = parseInteger(fields[1], "frames");

            if (frameCount <= 0) {
                throw std::runtime_error("frames must be greater than zero");
            }

            InputCase inputCase;
            inputCase.testId = testId;
            inputCase.frameCount = frameCount;
            inputCase.refs = parseReferenceString(fields[2]);

            cases.push_back(inputCase);
        } catch (const std::exception& error) {
            throw std::runtime_error(
                "CSV line " + std::to_string(lineNumber) +
                ": " + error.what()
            );
        }
    }

    if (cases.empty()) {
        throw std::runtime_error("CSV contains no data rows");
    }

    return cases;
}


#pragma once

#include <string>
#include "core/page_replacement.h"

// Export toàn bộ quá trình mô phỏng ra file CSV.
bool exportTraceCsv(const Result& result,
                    const std::string& filePath);

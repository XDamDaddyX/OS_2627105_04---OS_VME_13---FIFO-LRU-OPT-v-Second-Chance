#include "page_replacement.h"
#include <stdexcept>
#include <vector>

/**
 * @brief Cài đặt thuật toán thay thế trang Second-Chance (Clock Algorithm)
 * 
 * Nguyên lý hoạt động:
 * - Sử dụng mảng tròn (circular list) các khung trang (frames) cùng với bit cơ hội thứ hai (reference bit).
 * - Một con trỏ (clock hand / pointer) trỏ tới frame tiếp theo cần xem xét thay thế.
 * - Khi có truy cập trang (Page Access):
 *   1. Nếu trang đã có trong bộ nhớ (Page Hit):
 *      - Đặt reference bit của frame chứa trang đó thành 1 (cho trang thêm một cơ hội).
 *      - Không thay đổi vị trí con trỏ clock hand.
 *   2. Nếu trang chưa có trong bộ nhớ (Page Fault):
 *      - Nếu còn frame trống:
 *        + Nạp trang mới vào frame trống đầu tiên.
 *        + Đặt reference bit = 1.
 *        + Con trỏ di chuyển sang vị trí kế tiếp (hoặc giữ vị trí tuần tự).
 *      - Nếu tất cả frames đã đầy (Cần thay thế trang - Page Replacement):
 *        + Duyệt tuần tự từ vị trí con trỏ hiện tại:
 *          * Nếu reference bit == 1: Đặt lại bit = 0 (xóa cơ hội thứ hai) và tăng con trỏ lên (modulo frameCount).
 *          * Nếu reference bit == 0: Chọn frame này làm victim để thay thế!
 *        + Nạp trang mới vào vị trí victim này, đặt reference bit = 1.
 *        + Di chuyển con trỏ sang vị trí kế tiếp: pointer = (victim_index + 1) % frameCount.
 * 
 * @param refs Danh sách chuỗi tham chiếu trang (Reference string)
 * @param frameCount Số lượng khung trang (khung nhớ vật lý)
 * @return Result Cấu trúc chứa chi tiết từng bước, tổng số page faults và page hits
 */
Result runSecondChance(const std::vector<int>& refs, int frameCount)
{
    if (frameCount <= 0) {
        throw std::invalid_argument("Frame count must be greater than zero");
    }

    Result result;
    result.algo = Algorithm::SecondChance;
    result.faults = 0;
    result.hits = 0;

    // Khởi tạo các khung trang rỗng (-1) và mảng bit cơ hội thứ hai (reference bit)
    std::vector<int> frames(frameCount, -1);
    std::vector<int> refBits(frameCount, 0);

    int pointer = 0; // Con trỏ đồng hồ (Clock hand)
    int stepIndex = 1;

    for (int page : refs) {
        Step step;
        step.index = stepIndex++;
        step.page = page;
        step.victim = -1;

        // 1. Kiểm tra Page Hit
        int hitIndex = -1;
        for (int i = 0; i < frameCount; ++i) {
            if (frames[i] == page) {
                hitIndex = i;
                break;
            }
        }

        if (hitIndex != -1) {
            // Page Hit
            step.hit = true;
            refBits[hitIndex] = 1; // Cấp lại cơ hội thứ hai
            result.hits++;
        } else {
            // Page Fault
            step.hit = false;
            result.faults++;

            // Kiểm tra xem có frame trống không
            int emptyIndex = -1;
            for (int i = 0; i < frameCount; ++i) {
                if (frames[i] == -1) {
                    emptyIndex = i;
                    break;
                }
            }

            if (emptyIndex != -1) {
                // Còn frame trống: nạp trực tiếp vào frame trống
                frames[emptyIndex] = page;
                refBits[emptyIndex] = 1;
                pointer = (emptyIndex + 1) % frameCount;
            } else {
                // Tất cả frame đều đầy: Áp dụng giải thuật Clock để tìm nạn nhân (Victim)
                while (true) {
                    if (refBits[pointer] == 0) {
                        // Tìm thấy frame có bit = 0 -> Chọn làm victim
                        step.victim = frames[pointer];
                        frames[pointer] = page;
                        refBits[pointer] = 1;
                        pointer = (pointer + 1) % frameCount;
                        break;
                    } else {
                        // Bit == 1 -> Xóa bit về 0 (cho qua cơ hội) và chuyển con trỏ sang vị trí kế tiếp
                        refBits[pointer] = 0;
                        pointer = (pointer + 1) % frameCount;
                    }
                }
            }
        }

        step.frames = frames;
        step.faultsSoFar = result.faults;
        result.steps.push_back(step);
    }

    return result;
}

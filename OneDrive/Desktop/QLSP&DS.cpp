#include "ProductManager.h"
#include <iostream>
#include <fstream>
#include <iomanip> // Thư viện để căn lề bảng bằng setw
Product::Product() {
    maSP = "";
    tenSP = "";
    giaSP = 0.0;
}
Product::Product(std::string ma, std::string ten, double gia) {
    maSP = ma;
    tenSP = ten;
    giaSP = gia;
}
std::string Product::getMaSP() const { 
    return maSP; 
}
std::string Product::getTenSP() const { 
    return tenSP; 
}
double Product::getGiaSP() const { 
    return giaSP; 
}
// Hàm đọc dữ liệu từ file .txt và phân tách chuỗi bằng dấu '|'
void ProductManager::docFileSanPham(std::string tenFile) {
    danhSachSP.clear(); // Xóa sạch danh sách cũ nếu có
    std::ifstream file(tenFile);
    
    // Kiểm tra nếu không mở được file
    if (!file.is_open()) {
        std::cout << "[Loi] Khong the mo file " << tenFile << " de doc du lieu!\n";
        return;
    }
    std::string dong;
    while (std::getline(file, dong)) {
        if (dong.empty()) continue; // Bỏ qua nếu dòng trống
        
        // Tìm vị trí thủ công của các dấu gạch đứng '|'
        size_t viTriGach1 = dong.find('|');
        size_t viTriGach2 = dong.find('|', viTriGach1 + 1);
        
        // Kiểm tra dòng hợp lệ khi có đủ 2 dấu '|'
        if (viTriGach1 != std::string::npos && viTriGach2 != std::string::npos) {  
            // Cắt chuỗi dựa theo các vị trí tìm được
            std::string ma = dong.substr(0, viTriGach1);
            std::string ten = dong.substr(viTriGach1 + 1, viTriGach2 - viTriGach1 - 1);
            std::string giaChuoi = dong.substr(viTriGach2 + 1);
            
            // Chuyển chuỗi giá tiền thành số double
            double gia = std::stod(giaChuoi);
            
            // Tạo đối tượng và thêm vào danh sách
            Product sp(ma, ten, gia);
            danhSachSP.push_back(sp);
        }
    }
    file.close(); // Đóng file sau khi xử lý xong
}
// Hàm in danh sách sản phẩm dưới dạng bảng console
void ProductManager::hienThiDangBang() const {
    std::cout << "\n==================== DANH SACH SAN PHAM ====================\n";
    if (danhSachSP.empty()) {
        std::cout << " Cua hang hien tai chua co san pham nao hoac file bi trong!\n";
        std::cout << "============================================================\n";
        return;
    }
    // In tiêu đề các cột (Căn lề trái thẳng hàng tự động bằng setw)
    std::cout << std::left << std::setw(8)  << "STT" 
                           << std::setw(12) << "Ma SP" 
                           << std::setw(25) << "Ten San Pham" 
              << std::right << std::setw(15) << "Gia ban (VND)" << "\n";
    std::cout << "------------------------------------------------------------\n";
    // Vòng lặp in từng sản phẩm theo dòng
    for (size_t i = 0; i < danhSachSP.size(); ++i) {
        std::cout << std::left << std::setw(8)  << (i + 1)
                               << std::setw(12) << danhSachSP[i].getMaSP()
                               << std::setw(25) << danhSachSP[i].getTenSP()
                  << std::right << std::setw(15) << std::fixed << std::setprecision(0) 
                                << danhSachSP[i].getGiaSP() << "d\n";
    }
    std::cout << "============================================================\n";
}
// Hàm lấy toàn bộ danh sách (để các thành viên khác trong nhóm lấy data sử dụng)
std::vector<Product> ProductManager::getDanhSachSP() const {
    return danhSachSP;
}
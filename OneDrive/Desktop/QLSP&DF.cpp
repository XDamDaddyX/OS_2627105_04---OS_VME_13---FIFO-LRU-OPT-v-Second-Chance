#pragma once
#ifndef PRODUCT_MANAGER_H
#define PRODUCT_MANAGER_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip> // Thư viện để căn lề bảng bằng setw
class Product 
{
private:
    std::string maSP;  // Mã sản phẩm
    std::string tenSP; // Tên sản phẩm
    double giaSP;      // Giá sản phẩm

public:
    // Các Constructor
    Product() {
        maSP = "";
        tenSP = "";
        giaSP = 0.0;
    }

    Product(std::string ma, std::string ten, double gia) {
        maSP = ma;
        tenSP = ten;
        giaSP = gia;
    }

    // Các hàm lấy giá trị (Getter)
    std::string getMaSP() const { return maSP; }
    std::string getTenSP() const { return tenSP; }
    double getGiaSP() const { return giaSP; }
};
class ProductManager
{
private:
    std::vector<Product> danhSachSP;

public:
    // Hàm đọc dữ liệu từ file .txt và phân tách chuỗi bằng dấu '|'
    void docFileSanPham(std::string tenFile) {
        danhSachSP.clear(); // Xóa sạch danh sách cũ nếu có
        std::ifstream file(tenFile);
        // Kiểm tra nếu không mở được file
        if (!file.is_open()) {
            std::cout << "[Lỗi] Không thể mở file " << tenFile << " để đọc dữ liệu!\n";
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
    void hienThiDangBang() const {
        std::cout << "\n==================== DANH SACH SAN PHAM ====================\n";
        if (danhSachSP.empty()) {
            std::cout << " Cửa hàng hiện tại chưa có sản phẩm nào hoặc file bị trống!\n";
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
    std::vector<Product> getDanhSachSP() const {
        return danhSachSP;
    }
};

#endif
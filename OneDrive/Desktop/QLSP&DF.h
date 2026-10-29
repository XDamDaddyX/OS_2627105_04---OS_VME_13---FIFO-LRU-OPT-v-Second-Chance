#pragma once
#ifndef PRODUCT_MANAGER_H
#define PRODUCT_MANAGER_H

#include <string>
#include <vector>
class Product 
{
private:
    std::string maSP;  // Mã sản phẩm
    std::string tenSP; // Tên sản phẩm
    double giaSP;      // Giá sản phẩm

public:
    // Các Constructor
    Product();
    Product(std::string ma, std::string ten, double gia);

    // Các hàm lấy giá trị (Getter)
    std::string getMaSP() const;
    std::string getTenSP() const;
    double getGiaSP() const;
};
class ProductManager
{
private:
    std::vector<Product> danhSachSP;

public:
    // Hàm đọc dữ liệu từ file .txt và phân tách chuỗi bằng dấu '|'
    void docFileSanPham(std::string tenFile);
    // Hàm in danh sách sản phẩm dưới dạng bảng console
    void hienThiDangBang() const;
    // Hàm lấy toàn bộ danh sách (để các thành viên khác trong nhóm lấy data sử dụng)
    std::vector<Product> getDanhSachSP() const;
};

#endif
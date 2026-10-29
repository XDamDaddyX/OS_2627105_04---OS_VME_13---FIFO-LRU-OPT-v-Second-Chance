DROP TABLE IF EXISTS OrderDetails;
DROP TABLE IF EXISTS Orders;
DROP TABLE IF EXISTS Products;
DROP TABLE IF EXISTS Customers;

-- Tạo bảng Khách hàng
CREATE TABLE Customers (
    CustomerID INT PRIMARY KEY,
    CustomerName NVARCHAR(100),
    Email VARCHAR(100),
    PhoneNumber VARCHAR(15)
);

-- Tạo bảng Sản phẩm
CREATE TABLE Products (
    ProductID INT PRIMARY KEY,
    ProductName NVARCHAR(100),
    Price DECIMAL(18,2),
    StockQuantity INT
);

-- Tạo bảng Đơn hàng
CREATE TABLE Orders (
    OrderID INT PRIMARY KEY,
    CustomerID INT,
    OrderDate DATE,
    TotalAmount DECIMAL(18,2),
    FOREIGN KEY (CustomerID) REFERENCES Customers(CustomerID)
);

-- Tạo bảng Chi tiết đơn hàng
CREATE TABLE OrderDetails (
    OrderDetailID INT IDENTITY(1,1) PRIMARY KEY,
    OrderID INT,
    ProductID INT,
    Quantity INT,
    UnitPrice DECIMAL(18,2),
    FOREIGN KEY (OrderID) REFERENCES Orders(OrderID),
    FOREIGN KEY (ProductID) REFERENCES Products(ProductID)
);

INSERT INTO Customers VALUES (1, N'Nguyễn Văn A', 'anguyen@gmail.com', '0901234567');
INSERT INTO Customers VALUES (2, N'Trần Thị B', 'btran@gmail.com', '0907654321');
INSERT INTO Customers VALUES (3, N'Lê Hoàng C', 'cle@gmail.com', '0912345678');
INSERT INTO Customers VALUES (4, N'Phạm Minh D', 'dpham@gmail.com', '0987654321'); -- Khách này không mua gì

INSERT INTO Products VALUES (1, N'Điện thoại iPhone 15', 25000000, 50);
INSERT INTO Products VALUES (2, N'Laptop Asus ROG', 30000000, 20);
INSERT INTO Products VALUES (3, N'Chuột không dây Logi', 500000, 100);
INSERT INTO Products VALUES (4, N'Bàn phím cơ AKKO', 1500000, 15);
INSERT INTO Products VALUES (5, N'Tai nghe Sony WH', 4000000, 10); -- Sản phẩm này chưa ai mua

INSERT INTO Orders VALUES (101, 1, '2026-07-01', 55500000);
INSERT INTO Orders VALUES (102, 2, '2026-07-02', 500000);
INSERT INTO Orders VALUES (103, 3, '2026-07-03', 30000000);
INSERT INTO Orders VALUES (104, 1, '2026-07-04', 1500000);
GO


SELECT * FROM Customers
WHERE CustomerID IN (
    SELECT CustomerID 
    FROM Orders 
    WHERE TotalAmount > (SELECT AVG(TotalAmount) FROM Orders)
);

-- [CÁCH 2]: Dùng EXISTS để tối ưu hơn (Kiếm điểm cộng từ giảng viên)
SELECT c.CustomerID, c.CustomerName 
FROM Customers c
WHERE EXISTS (
    SELECT 1 
    FROM Orders o 
    WHERE o.CustomerID = c.CustomerID 
      AND o.TotalAmount > (SELECT AVG(TotalAmount) FROM Orders)
);
GO


SELECT ProductID, ProductName, Price
FROM Products
WHERE ProductID NOT IN (
    SELECT ProductID 
    FROM OrderDetails 
);

-- [CÁCH 2]: Dùng LEFT JOIN tìm các dòng bị NULL (Cách giải khác để lấy điểm bonus)
SELECT p.ProductID, p.ProductName, p.Price
FROM Products p
LEFT JOIN OrderDetails od ON p.ProductID = od.ProductID
WHERE od.ProductID IS NULL;
GO


DROP PROC IF EXISTS sp_TinhTongChiTieu;
GO

-- Tiến hành tạo Procedure
CREATE PROC sp_TinhTongChiTieu
    @MaKH int
AS
BEGIN
    -- Sử dụng cú pháp gõ đầy đủ tên bảng thay vì dùng alias viết tắt
    SELECT Customers.CustomerID, Customers.CustomerName, SUM(Orders.TotalAmount) AS TongTienDaChi
    FROM Customers
    INNER JOIN Orders ON Customers.CustomerID = Orders.CustomerID
    WHERE Customers.CustomerID = @MaKH
    GROUP BY Customers.CustomerID, Customers.CustomerName
END;
GO

-- Lệnh CHẠY THỬ và áp dụng Procedure để giải quyết truy vấn cho Khách hàng số 1:
EXEC sp_TinhTongChiTieu @MaKH = 1;
GO


DROP TRIGGER IF EXISTS trg_TruTonKho;
GO

-- Tiến hành tạo Trigger (Gài logic kiểu năm nhất: lấy biến đơn lẻ từ bảng inserted)
CREATE TRIGGER trg_TruTonKho
ON OrderDetails
AFTER INSERT
AS
BEGIN
    DECLARE @MaSP INT
    DECLARE @SoLuongMua INT

    -- Gán giá trị từ dòng vừa được chèn vào biến
    SELECT @MaSP = ProductID, @SoLuongMua = Quantity FROM inserted

    -- Cập nhật lại số kho của sản phẩm đó trong bảng Products
    UPDATE Products
    SET StockQuantity = StockQuantity - @SoLuongMua
    WHERE ProductID = @MaSP
END;
GO

-- Lệnh CHẠY THỬ để chứng minh Trigger hoạt động đúng:
-- Bước A: Xem số lượng tồn kho hiện tại của sản phẩm số 4 (Bàn phím cơ) trước khi test
SELECT ProductID, ProductName, StockQuantity FROM Products WHERE ProductID = 4;

-- Bước B: Chèn thử 1 dòng vào chi tiết đơn hàng (Mua 2 chiếc bàn phím cơ)
INSERT INTO OrderDetails (OrderID, ProductID, Quantity, UnitPrice)
VALUES (104, 4, 2, 1500000);

-- Bước C: Xem lại tồn kho sau khi chèn để chứng minh nó tự động trừ đi 2 (Từ 15 xuống 13)
SELECT ProductID, ProductName, StockQuantity FROM Products WHERE ProductID = 4;
GO


SELECT TOP 3 Customers.CustomerName, SUM(Orders.TotalAmount) AS DoanhThuKH
FROM Customers, Orders
WHERE Customers.CustomerID = Orders.CustomerID
GROUP BY Customers.CustomerName
ORDER BY DoanhThuKH DESC;

SELECT TOP 3 c.CustomerID, c.CustomerName, SUM(o.TotalAmount) AS TongTien
FROM Customers c
INNER JOIN Orders o ON c.CustomerID = o.CustomerID
GROUP BY c.CustomerID, c.CustomerName
ORDER BY TongTien DESC;
GO
#include <iostream>

// Nhúng các tầng theo đúng thứ tự phụ thuộc:
// 1. core.cpp: Nơi định nghĩa struct, dữ liệu RAM và các hàm thuật toán chính
#include "dsa_core.cpp"

// 2. persistence.cpp: Nơi chứa các hàm đọc/ghi file CSV (dùng dữ liệu từ core.cpp)
// Lưu ý: Đảm bảo trong persistence.cpp KHÔNG có dòng #include "core.cpp" nữa để tránh lỗi trùng lặp
#include "persistence.cpp"

// 3. presentation.cpp: Nơi chứa menu CLI và các màn hình console (gọi hàm từ core.cpp)
#include "presentation.cpp"

using namespace std;

int main() {
    cout << "====================================================\n";
    cout << "    HE THONG DANG KY HOC PHAN & WAITLIST   \n";
    cout << "====================================================\n";

    // 1. TANG PERSISTENCE: Nap du lieu tu cac file CSV vao RAM khi bat dau
    cout << "[1/2] Dang nap du lieu ban dau vao bo nho RAM...\n";

    // Duong dan file tuong doi tinh tu vi tri chay file exe
    nap_du_lieu_sinh_vien("../02_Data/Sinh_Vien.csv");
    nap_du_lieu_hoc_phan("../02_Data/courses.csv");

    cout << "[2/2] Khoi tao du lieu hoan tat!\n\n";

    // 2. TANG PRESENTATION: Chay vong lap menu console cho nguoi dung tuong tac
    chay_giao_dien();

    // 3. TANG PERSISTENCE: Tu dong ghi nhat ky thao tac ra file CSV truoc khi thoat
    cout << "\nDang luu nhat ky he thong truoc khi tat...\n";
    luu_nhat_ky_ra_file("02_Data/operation_history.csv");

    cout << "====================================================\n";
    cout << "   CHUONG TRINH DA KET THUC VA LUU DU LIEU AN TOAN! \n";
    cout << "====================================================\n";

    return 0;
}
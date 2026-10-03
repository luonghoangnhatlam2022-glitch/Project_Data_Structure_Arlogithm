#pragma once

// Thư viện C++
#include <string>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

// Tự cài đặt
#include "list.h"
#include "unordered_map.h"

// KHÔNG dùng "using namespace std;" để tránh đụng độ với custom list và unordered_map
using std::string;
using std::vector;
using std::unordered_set;
using std::stringstream;

// Thông tin sinh viên
struct SinhVien
{
    string mssv;
    string ho_ten;
    string ngay_sinh;
    string ma_nganh;
};

// Thông tin học phần
struct HocPhan
{
    string ma_mon;
    string ten_mon;
    int si_so_toi_da;
    int si_so_hien_tai;
};

// Thao tác của sinh viên đối với môn học
struct LichSu
{
    string thoi_gian;
    string hanh_dong;
    string mssv;
    string ma_mon;
};

// Lưu danh sách chờ của từng môn học
// list dùng để lưu thứ tự của sinh viên trong hàng đợi
// hash table lưu địa chỉ của phần tử trong list: đảm bảo FIFO + khả năng tìm kiếm, xóa O(1)
struct DanhSachCho
{
    list hang_doi;
    unordered_map<string, Node*> vi_tri_node; // Giả định struct Node được định nghĩa public trong list.h
};

// Hash table lưu danh sách tất cả sinh viên <mssv, SinhVien>
unordered_map<string, SinhVien> ds_sinh_vien;

// Hash table lưu danh sách tất cả môn học <ma_mon, HocPhan>
unordered_map<string, HocPhan> ds_hoc_phan;

// Hash table lưu danh sách sinh viên chính thức của một môn học
// <ma_mon, hash_set<mssv> >
unordered_map<string, unordered_set<string>> ds_chinh_thuc;

// Danh sách chờ đăng ký của từng môn <ma_mon, DanhSachCho>
unordered_map<string, DanhSachCho> ds_cho;

// DANG_KY, VAO_HANG_CHO, RUT_HANG_CHO, HUY_MON, DON_LEN_CHINH_THUC
vector<LichSu> nhat_ky_he_thong;


// MC1: Tra cứu sinh viên theo MSSV - O(1)
SinhVien* tim_sinh_vien(string mssv)
{
    if (ds_sinh_vien.find(mssv) != ds_sinh_vien.end())
    {
        return &ds_sinh_vien[mssv]; // operator[] đã được viết để trả về reference
    }
    return nullptr;
}

// MC1: Tra cứu thông tin học phần theo mã môn - O(1)
HocPhan* tim_hoc_phan(string ma_mon)
{
    if (ds_hoc_phan.find(ma_mon) != ds_hoc_phan.end())
    {
        return &ds_hoc_phan[ma_mon];
    }
    return nullptr;
}

// Lấy thời gian hiện tại dạng chuỗi để ghi log
string lay_thoi_gian_hien_tai()
{
    auto hien_tai = std::chrono::system_clock::now();
    auto thoi_gian_c = std::chrono::system_clock::to_time_t(hien_tai);
    stringstream ss;
    ss << std::put_time(std::localtime(&thoi_gian_c), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void ghi_nhat_ky(string hanh_dong, string mssv, string ma_mon)
{
    nhat_ky_he_thong.push_back({lay_thoi_gian_hien_tai(), hanh_dong, mssv, ma_mon});
}


// Chức năng chính: Xử lý logic đăng ký hoặc đưa vào hàng chờ
string dang_ky_mon(string ma_mon, string mssv) {
    // Bước 1: Kiểm tra tính hợp lệ
    if (ds_hoc_phan.find(ma_mon) == ds_hoc_phan.end()) return "LOI_MON_KHONG_TON_TAI";
    if (ds_sinh_vien.find(mssv) == ds_sinh_vien.end()) return "LOI_SINH_VIEN_KHONG_TON_TAI";

    // Bước 2: Kiểm tra sinh viên đã có trong danh sách chính thức chưa
    auto &lop_chinh_thuc = ds_chinh_thuc[ma_mon];
    if (lop_chinh_thuc.find(mssv) != lop_chinh_thuc.end()) {
        ghi_nhat_ky("DANG_KY_THANH_CONG", mssv, ma_mon);
        return "DA_DANG_KY_CHINH_THUC";
    }



    // Bước 3: Kiểm tra sinh viên đã xếp hàng ở môn này chưa
    auto &hang_cho = ds_cho[ma_mon];
    if (hang_cho.vi_tri_node.find(mssv) != hang_cho.vi_tri_node.end()) return "DA_NAM_TRONG_DANH_SACH_CHO";

    HocPhan &hp = ds_hoc_phan[ma_mon];

    // Bước 4: Nếu lớp CÒN CHỖ -> Đăng ký thẳng vào lớp chính thức
    if (hp.si_so_hien_tai < hp.si_so_toi_da) {
        hp.si_so_hien_tai++;
        lop_chinh_thuc.insert(mssv);
        ghi_nhat_ky("DANG_KY", mssv, ma_mon);
        return "THANH_CONG_CHINH_THUC";
    }

    // Bước 5: Nếu lớp ĐÃ ĐẦY -> Đẩy vào cuối danh sách chờ (Waitlist)
    hang_cho.vi_tri_node[mssv] = hang_cho.hang_doi.push_back(mssv);
    ghi_nhat_ky("VAO_HANG_CHO", mssv, ma_mon);
    return "THANH_CONG_VAO_HANG_CHO";
}

// Chức năng: Trích xuất lịch sử gần nhất (Đọc ngược từ cuối mảng)
vector<LichSu> lay_lich_su_gan_day(int so_luong) {
    vector<LichSu> ket_qua;
    int dem = 0;
    for (auto it = nhat_ky_he_thong.rbegin(); it != nhat_ky_he_thong.rend() && dem < so_luong; ++it, ++dem) {
        ket_qua.push_back(*it);
    }
    return ket_qua;
}

// Giải quyết xung đột thiết kế: Cập nhật đồng thời Hàng đợi & Hash Map
string huy_mon_chinh_thuc(string ma_mon, string mssv) {
    if (ds_hoc_phan.find(ma_mon) == ds_hoc_phan.end()) return "LOI_MON_KHONG_TON_TAI";

    auto& lop_chinh_thuc = ds_chinh_thuc[ma_mon];
    if (lop_chinh_thuc.find(mssv) == lop_chinh_thuc.end()) {
        return "SINH_VIEN_KHONG_CO_TRONG_LOP";
    }

    lop_chinh_thuc.erase(mssv);
    ghi_nhat_ky("HUY_MON", mssv, ma_mon);

    HocPhan& hp = ds_hoc_phan[ma_mon];
    auto& hang_cho = ds_cho[ma_mon];

    // Kiểm tra danh sách chờ, lấy người đầu tiên đẩy lên lớp
    if (!hang_cho.hang_doi.empty()) {
        string mssv_duoc_chon = hang_cho.hang_doi.front();
        hang_cho.hang_doi.pop_front();
        hang_cho.vi_tri_node.erase(mssv_duoc_chon); // Xóa khỏi custom map bằng Key

        lop_chinh_thuc.insert(mssv_duoc_chon);

        ghi_nhat_ky("DON_LEN_CHINH_THUC", mssv_duoc_chon, ma_mon);
        return "DA_HUY_VA_DON_SINH_VIEN_" + mssv_duoc_chon;
    } else {
        hp.si_so_hien_tai--;
        return "HUY_THANH_CONG";
    }
}

bool rut_khoi_hang_cho(string ma_mon, string mssv) {
    if (ds_hoc_phan.find(ma_mon) == ds_hoc_phan.end()) return false;

    auto& hang_cho = ds_cho[ma_mon];
    auto it = hang_cho.vi_tri_node.find(mssv);

    if (it == hang_cho.vi_tri_node.end()) return false;

    // it->second lấy ra value là Node*
    hang_cho.hang_doi.erase(it->second);

    // Custom unordered_map của chúng ta đã được cập nhật hàm erase(Iterator)
    hang_cho.vi_tri_node.erase(it);

    ghi_nhat_ky("RUT_HANG_CHO", mssv, ma_mon);
    return true;
}
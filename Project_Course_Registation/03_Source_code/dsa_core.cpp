#pragma once

//Thu vien C++
#include <string>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

// Tu cai dat
#include "list.h"
#include "unordered_map.h"

using namespace std;

// Thong tin sinh vien
struct SinhVien
{
    string mssv;
    string ho_ten;
    string ngay_sinh;
    string ma_nganh;
};

// Thong tin hoc phan
struct HocPhan
{
    string ma_mon;
    string ten_mon;
    int si_so_toi_da;
    int si_so_hien_tai;
};

// Thao tac cua sinh vien doi voi mon hoc
struct LichSu
{
    string thoi_gian;
    string hanh_dong;
    string mssv;
    string ma_mon;
};

// Luu danh sach cho cua tung mon hoc
// list dung de luu thu tu cua sinh vien trong hang doi
// hash table luu dia chi cua phan tu trong list: dam bao FIFO + kha nang tim kiem, xoa
struct DanhSachCho
{
    list hang_doi;
    unordered_map<string, Node*> vi_tri_node;
};

// Hash table luu danh sach tat ca sinh vien
// <mssv, SinhVien>
unordered_map<string, SinhVien> ds_sinh_vien;

// Hash table luu danh sach tat ca mon hoc
// <ma_mon, HocPhan>
unordered_map<string, HocPhan> ds_hoc_phan;

// Hash table luu danh sach sinh vien chinh thuc cua mot mon hoc
// Hash set luu danh sach mssv cua sinh vien
// <ma_mon, hash_set<mssv> >
unordered_map<string, unordered_set<string>> ds_chinh_thuc;

// Danh sach cho dang ky cua tung mon
// <ma_mon, mssv>
unordered_map<string, DanhSachCho> ds_cho;

// DANG_KY, VAO_HANG_CHO, RUT_HANG_CHO, HUY_MON, DON_LEN_CHINH_THUC
// Dung nhu la stack luu cac thao tac tren
vector<LichSu> nhat_ky_he_thong;


// Lay thoi gian hien tai dang chuoi de ghi log
string lay_thoi_gian_hien_tai()
{
    auto hien_tai = chrono::system_clock::now();
    auto thoi_gian_c = chrono::system_clock::to_time_t(hien_tai);
    stringstream ss;
    ss << put_time(localtime(&thoi_gian_c), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void ghi_nhat_ky(string hanh_dong, string mssv, string ma_mon)
{
    nhat_ky_he_thong.push_back({lay_thoi_gian_hien_tai(), hanh_dong, mssv, ma_mon});
}

// Tra ra con tro tro den sinh vien co mssv
SinhVien* tim_sinh_vien(string mssv)
{
    if (ds_sinh_vien.find(mssv) != ds_sinh_vien.end())
    {
        return &ds_sinh_vien[mssv];
    }
    return nullptr;
}

string lay_thoi_gian_hien_tai() {
    auto hien_tai = chrono::system_clock::now();
    auto thoi_gian_c = chrono::system_clock::to_time_t(hien_tai);
    stringstream ss;
    ss << put_time(localtime(&thoi_gian_c), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Hàm hỗ trợ: Ghi lại một thao tác vào mảng nhật ký
void ghi_nhat_ky(string hanh_dong, string mssv, string ma_mon) {
    nhat_ky_he_thong.push_back({lay_thoi_gian_hien_tai(), hanh_dong, mssv, ma_mon});
}

// Tìm sinh viên bằng Hash Map -> Độ phức tạp trung bình O(1)
SinhVien* tim_sinh_vien(string mssv) {
    if (ds_sinh_vien.find(mssv) != ds_sinh_vien.end()) {
        return &ds_sinh_vien[mssv]; // Trả về con trỏ trỏ đến vùng nhớ của SV
    }
    return nullptr;
}

// Tìm môn học bằng Hash Map -> Độ phức tạp trung bình O(1)
HocPhan *tim_hoc_phan(string ma_mon) {
    if (ds_hoc_phan.find(ma_mon) != ds_hoc_phan.end()) {
        return &ds_hoc_phan[ma_mon];
    }
    return nullptr;
}

// Chức năng chính: Xử lý logic đăng ký hoặc đưa vào hàng chờ
string dang_ky_mon(string ma_mon, string mssv) {
    // Bước 1: Kiểm tra tính hợp lệ của dữ liệu đầu vào
    if (ds_hoc_phan.find(ma_mon) == ds_hoc_phan.end()) return "LOI_MON_KHONG_TON_TAI";
    if (ds_sinh_vien.find(mssv) == ds_sinh_vien.end()) return "LOI_SINH_VIEN_KHONG_TON_TAI";

    // Bước 2: Kiểm tra sinh viên đã có trong danh sách chính thức chưa
    auto &lop_chinh_thuc = ds_chinh_thuc[ma_mon];
    if (lop_chinh_thuc.find(mssv) != lop_chinh_thuc.end()) return "DA_DANG_KY_CHINH_THUC";

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
    // Lưu lại con trỏ Node vào Hash Map để sau này tìm kiếm/rút môn với thời gian O(1)
    hang_cho.vi_tri_node[mssv] = hang_cho.hang_doi.push_back(mssv);
    ghi_nhat_ky("VAO_HANG_CHO", mssv, ma_mon);
    return "THANH_CONG_VAO_HANG_CHO";
}

// Chức năng: Trích xuất lịch sử gần nhất (Đọc ngược từ cuối mảng)
vector<LichSu> lay_lich_su_gan_day(int so_luong) {
    vector<LichSu> ket_qua;
    int dem = 0;
    // Dùng rbegin() và rend() để duyệt mảng từ cuối lên đầu (mới nhất -> cũ nhất)
    for (auto it = nhat_ky_he_thong.rbegin(); it != nhat_ky_he_thong.rend() && dem < so_luong; ++it, ++dem) {
        ket_qua.push_back(*it);
    }
    return ket_qua;
}

string huy_mon_chinh_thuc(string ma_mon, string mssv) {
    if (ds_hoc_phan.find(ma_mon) == ds_hoc_phan.end()) return "LOI_MON_KHONG_TON_TAI";

    auto& lop_chinh_thuc = ds_chinh_thuc[ma_mon];
    int vi_tri = -1;
    for (int i = 0; i < (int)lop_chinh_thuc.size(); ++i) {
        if (lop_chinh_thuc[i] == mssv) {
            vi_tri = i;
            break;
        }
    }

    if (vi_tri == -1) return "SINH_VIEN_KHONG_CO_TRONG_LOP";

    lop_chinh_thuc.erase(lop_chinh_thuc.begin() + vi_tri);

    ghi_nhat_ky("HUY_MON", mssv, ma_mon);
    HocPhan& hp = ds_hoc_phan[ma_mon];
    auto& hang_cho = ds_cho[ma_mon];
    if (!hang_cho.hang_doi.empty()) {
        string mssv_duoc_chon = hang_cho.hang_doi.front();
        hang_cho.hang_doi.pop_front();
        hang_cho.vi_tri_node.erase(mssv_duoc_chon);

        lop_chinh_thuc.push_back(mssv_duoc_chon);
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

    hang_cho.hang_doi.erase(it->second);
    hang_cho.vi_tri_node.erase(it);

    ghi_nhat_ky("RUT_HANG_CHO", mssv, ma_mon);
    return true;
}


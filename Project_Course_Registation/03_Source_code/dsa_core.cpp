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
#pragma once

#include <iostream>
#include "presentation.cpp"
#include "dsa_core.cpp" // Import phần xử lý lõi để gọi hàm

using namespace std;

void in_menu() {
    cout << "\n============================================\n";
    cout << "    HE THONG DANG KY HOC PHAN & WAITLIST   \n";
    cout << "============================================\n";
    cout << "1. Tra cuu thong tin Sinh vien \n";
    cout << "2. Tra cuu thong tin Hoc phan \n";
    cout << "3. Dang ky Mon hoc \n";
    cout << "6. Xem Lich su Thao tac gan nhat \n";
    cout << "0. Thoat chuong trinh\n";
    cout << "============================================\n";
    cout << "Nhap lua chon: ";
}

void man_hinh_tim_sinh_vien() {
    string ma_sv;
    cout << "-> Nhap MSSV can tra cuu: ";
    cin >> ma_sv;
    SinhVien* sv = tim_sinh_vien(ma_sv);
    if (sv != nullptr) {
        cout << "[Ket qua] " << sv->ho_ten << " - Nganh: " << sv->ma_nganh << "\n";
    } else {
        cout << "[Loi] Khong tim thay sinh vien!\n";
    }
}

void man_hinh_tim_hoc_phan() {
    string ma_mh;
    cout << "-> Nhap Ma mon hoc: ";
    cin >> ma_mh;
    HocPhan* hp = tim_hoc_phan(ma_mh);
    if (hp != nullptr) {
        cout << "[Ket qua] " << hp->ten_mon << " | Si so: " << hp->si_so_hien_tai << "/" << hp->si_so_toi_da << "\n";
    } else {
        cout << "[Loi] Khong tim thay mon hoc!\n";
    }
}

void man_hinh_dang_ky() {
    string ma_mh, ma_sv;
    cout << "-> Nhap Ma mon hoc: "; cin >> ma_mh;
    cout << "-> Nhap MSSV: "; cin >> ma_sv;

    // Gọi hàm core và xử lý các kịch bản trả về
    string ket_qua = dang_ky_mon(ma_mh, ma_sv);
    cout << "[Thong bao] " << ket_qua << "\n";
}

void man_hinh_xem_lich_su() {
    int k;
    cout << "-> Nhap so thao tac muon xem: ";
    if (!(cin >> k) || k <= 0) {
        cout << "[Loi] So luong khong hop le!\n";
        return;
    }

    vector<LichSu> nhat_ky = lay_lich_su_gan_day(k);
    for (const auto& entry : nhat_ky) {
        cout << "[" << entry.thoi_gian << "] " << entry.hanh_dong
             << " | SV: " << entry.mssv << " | Mon: " << entry.ma_mon << "\n";
    }
}

// Vòng lặp điều hướng của toàn bộ chương trình
void chay_giao_dien() {
    int lua_chon = -1;
    while (true) {
        in_menu();

        // Bắt lỗi người dùng nhập chữ thay vì số để tránh crash chương trình
        if (!(cin >> lua_chon)) {
            cin.clear(); // Xóa cờ lỗi
            cin.ignore(10000, '\n'); // Bỏ qua bộ nhớ đệm
            cout << "[Loi] Vui long nhap so hop le!\n";
            continue;
        }

        if (lua_chon == 0) {
            cout << "Thoat chuong trinh...\n";
            break;
        }

        switch (lua_chon) {
            case 1: man_hinh_tim_sinh_vien(); break;
            case 2: man_hinh_tim_hoc_phan(); break;
            case 3: man_hinh_dang_ky(); break;
            case 6: man_hinh_xem_lich_su(); break;
            default: cout << "[Loi] Lua chon khong hop le!\n"; break;
        }
    }
}
// Màn hình 1: Hủy môn chính thức
void man_hinh_huy_mon() {
    string ma_mh, ma_sv;
    cout << "-> Nhap Ma mon hoc muon huy: ";
    cin >> ma_mh;
    cout << "-> Nhap MSSV: ";
    cin >> ma_sv;

    string ket_qua = huy_mon_chinh_thuc(ma_mh, ma_sv);

    if (ket_qua == "HUY_THANH_CONG") {
        cout << "[Thong bao] Da huy mon thanh cong (khong co sinh vien nao trong hang cho).\n";
    }
    else if (ket_qua.rfind("DA_HUY_VA_DON_SINH_VIEN_", 0) == 0) {
        string sv_don_len = ket_qua.substr(24);
        cout << "[Thong bao] Huy mon thanh cong! Sinh vien [" << sv_don_len
             << "] tu danh sach cho da duoc tu dong don len lop chinh thuc.\n";
    }
    else if (ket_qua == "SINH_VIEN_KHONG_CO_TRONG_LOP") {
        cout << "[Loi] Sinh vien khong theo hoc mon nay!\n";
    }
    else if (ket_qua == "LOI_MON_KHONG_TON_TAI") {
        cout << "[Loi] Ma mon hoc khong ton tai!\n";
    }
}

// Màn hình 2: Rút khỏi danh sách chờ
void man_hinh_rut_hang_cho() {
    string ma_mh, ma_sv;
    cout << "-> Nhap Ma mon hoc: ";
    cin >> ma_mh;
    cout << "-> Nhap MSSV can rut khoi hang cho: ";
    cin >> ma_sv;

    bool thanh_cong = rut_khoi_hang_cho(ma_mh, ma_sv);

    if (thanh_cong) {
        cout << "[Thong bao] Da rut ten khoi danh sach cho thanh cong!\n";
    }
    else {
        cout << "[Loi] Sinh vien khong co trong danh sach cho hoac ma mon khong hop le!\n";
    }
}

// Màn hình 3: Xem lịch sử thao tác
void man_hinh_xem_lich_su() {
    int k;
    cout << "-> Nhap so thao tac gan nhat muon xem: ";
    if (!(cin >> k) || k <= 0) {
        cout << "[Loi] So luong khong hop le!\n";
        return;
    }

    vector<LichSu> nhat_ky = lay_lich_su_gan_day(k);

    cout << "\n----- DANH SACH " << nhat_ky.size() << " THAO TAC GAN NHAT -----\n";
    if (nhat_ky.empty()) {
        cout << "(Chua co thao tac nao duoc ghi nhan)\n";
        return;
    }

    for (const auto &entry : nhat_ky) {
        cout << "[" << entry.thoi_gian << "] "
             << "Hanh dong: " << entry.hanh_dong
             << " | MSSV: " << entry.mssv
             << " | Ma mon: " << entry.ma_mon << "\n";
    }
    cout << "-------------------------------------------\n";
}


// Vòng lặp điều khiển chính của giao diện
void chay_giao_dien() {
    int lua_chon = -1;
    while (true) {
        in_menu();
        if (!(cin >> lua_chon)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "[Loi] Vui long nhap so tu 0 den 3!\n";
            continue;
        }

        if (lua_chon == 0) {
            cout << "\nDang chuan bi thoat chuong trinh...\n";
            break;
        }

        switch (lua_chon) {
        case 1:
            man_hinh_huy_mon();
            break;
        case 2:
            man_hinh_rut_hang_cho();
            break;
        case 3:
            man_hinh_xem_lich_su();
            break;
        default:
            cout << "[Loi] Lua chon khong hop le, vui long chon lai!\n";
            break;
        }
    }
}

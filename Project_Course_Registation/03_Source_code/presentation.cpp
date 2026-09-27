#include <iostream>
#include "presentation.h"
#include "dsa_core.h" // Import phần xử lý lõi để gọi hàm

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

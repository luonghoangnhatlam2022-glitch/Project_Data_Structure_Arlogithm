#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "../03_Source_Code/dsa_core.cpp"
#include "../03_Source_Code/persistence.cpp"

using namespace std;

bool test_data(string duong_dan_file)
{
    ifstream tep_tin(duong_dan_file);
    if (!tep_tin.is_open())
    {
        cout << "[Loi] Khong the mo file: " << duong_dan_file << endl;
        return false;
    }

    string dong_du_lieu;
    // Doc bo dong tieu de dau tien (header)
    getline(tep_tin, dong_du_lieu);

    while (getline(tep_tin, dong_du_lieu))
    {
        if (dong_du_lieu.empty())
            continue;

        stringstream tach_chuoi(dong_du_lieu);
        string lenh, ma_mh, ma_sv;

        // Tach cac cot duoc phan cach bang dau phay
        if (getline(tach_chuoi, lenh, ','))
        {
            if (lenh == "ADD")
            {
                getline(tach_chuoi, ma_mh, ',');
                getline(tach_chuoi, ma_sv, ',');
                string ket_qua = dang_ky_mon(ma_mh, ma_sv);
            }
            else if (lenh == "REMOVE")
            {
                getline(tach_chuoi, ma_mh, ',');
                getline(tach_chuoi, ma_sv, ',');
                string ket_qua = huy_mon_chinh_thuc(ma_mh, ma_sv);
                if (ket_qua != "HUY_THANH_CONG" && ket_qua.rfind("DA_HUY_VA_DON_SINH_VIEN_", 0) == 0)
                    bool thanh_cong = rut_khoi_hang_cho(ma_mh, ma_sv);
            }
            else if (lenh == "GETSV")
            {
                getline(tach_chuoi, ma_sv, ',');
                SinhVien *sv = tim_sinh_vien(ma_sv);
            }
            else if (lenh == "GETHP")
            {
                getline(tach_chuoi, ma_mh, ',');
                HocPhan *hp = tim_hoc_phan(ma_mh);
            }
        }
    }
    tep_tin.close();
    return true;
}
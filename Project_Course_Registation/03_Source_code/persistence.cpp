//
// Created by Lenovo on 9/27/2026.
//
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "dsa_core.cpp"

using namespace std;


// Co 3 Main Function 
// 1. Doc file danh sach sinh vien (Sinh_Vien.csv)
// 2. Doc file danh sach hoc phan (courses.csv)
// 3. Luu nhat ky he thong ra file CSV truoc khi tat chuong trinh

bool nap_du_lieu_sinh_vien(string duong_dan_file) {
    ifstream tep_tin(duong_dan_file);

    if (!tep_tin.is_open()) {
        cout << "[Loi] Khong the mo file: " << duong_dan_file << endl;
        return false;
    }

    string dong_header;
    // Doc bo dong tieu de dau tien (header)
    // Thuong file CSV co ten cac cot VD: NAME , ID , CLASS bo qua dong nay  
    getline(tep_tin, dong_header);

    int dem_sinh_vien = 0;
    // tu dong 2 sau header 
    while (getline(tep_tin,dong_header)) {
        if (dong_header.empty()) continue;

        stringstream tach_chuoi(dong_header);
        string ma_sv, ten_sv, nganh, ngay_sinh;

        // Tach 4 cot duoc phan cach bang dau phay
        if (getline(tach_chuoi, ma_sv, ',') &&
            getline(tach_chuoi, ten_sv, ',') &&
            getline(tach_chuoi, nganh, ',') &&
            getline(tach_chuoi, ngay_sinh, ',')) {

            SinhVien sv;
            sv.mssv = ma_sv;
            sv.ho_ten = ten_sv;
            sv.ma_nganh = nganh;
            sv.ngay_sinh = ngay_sinh;

            // Nap truc tiep vao bang bam O(1) tren RAM
            ds_sinh_vien[ma_sv] = sv;
            dem_sinh_vien++;
            }
    }

    tep_tin.close();
    cout << "[Thanh cong] Da nap " << dem_sinh_vien << " sinh vien vao RAM." << endl;
    return true;
}

bool nap_du_lieu_hoc_phan(string duong_dan_file) {
    ifstream tep_tin(duong_dan_file);
    if (!tep_tin.is_open()) {
        cout << "[Loi] Khong the mo file: " << duong_dan_file << endl;
        return false;
    }

    string dong_header;
    getline(tep_tin, dong_header);

    int dem_sinh_vien = 0;
    while (getline(tep_tin, dong_header)) {
        if (dong_header.empty()) continue;

        stringstream tach_chuoi(dong_header);
        string ma_mon_hoc, ten_mon_hoc, chuoi_si_so;

        if (getline(tach_chuoi, ma_mon_hoc, ',') &&
            getline(tach_chuoi, ten_mon_hoc, ',') &&
            getline(tach_chuoi, chuoi_si_so, ',')) {

            HocPhan hp;
            hp.ma_mon = ma_mon_hoc;
            hp.ten_mon = ten_mon_hoc;
            hp.si_so_toi_da = stoi(chuoi_si_so);
            hp.si_so_hien_tai = 0; // ban dau 

            ds_hoc_phan[ma_mon_hoc] = hp;
            dem_sinh_vien++;
        }
    }

    tep_tin.close();
    cout << "[Thanh cong] Da nap " << dem_sinh_vien << " hoc phan vao RAM." << endl;
    return true;
}


bool luu_nhat_ky_ra_file(string duong_dan_file) {
    ofstream tep_tin(duong_dan_file);
    if (!tep_tin.is_open()) {
        cout << "[Loi] Khong the ghi file nhat ky: " << duong_dan_file << endl;
        return false;
    }

    // Ghi tieu de cho lich su 
    tep_tin << "thoi_gian,hanh_dong,mssv,ma_mon\n";

    //
    for (const auto& log : nhat_ky_he_thong) {
        tep_tin << log.thoi_gian << ","
                << log.hanh_dong << "," 
                << log.mssv << ","
                << log.ma_mon << "\n";
    }

    tep_tin.close();
    cout << "[Thanh cong] Da luu nhat ky hoat dong ra file." << endl;
    return true;
}
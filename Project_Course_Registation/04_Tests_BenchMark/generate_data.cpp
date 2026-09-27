#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;
using ll = long long;

ll seed = 49;
ll random(int x)
{
    seed = (1103515245 * seed + 12345) % 2147483648;
    return seed % x;
}

ll so_luong_sinh_vien = 0;
unordered_map<int, unordered_map<string, bool>> danh_sach_sinh_vien;
int so_luong_hoc_phan = 0;
vector<string> danh_sach_hoc_phan;

void nhap_danh_sach_hoc_phan()
{
    ifstream hocphan("../02_Data/courses.csv");
    string dong_du_lieu;
    // Doc bo dong tieu de dau tien (header)
    getline(hocphan, dong_du_lieu);

    while (getline(hocphan, dong_du_lieu))
    {
        if (dong_du_lieu.empty())
            continue;

        stringstream tach_chuoi(dong_du_lieu);
        string ma_mon, ten_mon, si_so_toi_da;

        // Tach 3 cot duoc phan cach bang dau phay
        if (getline(tach_chuoi, ma_mon, ',') &&
            getline(tach_chuoi, ten_mon, ',') &&
            getline(tach_chuoi, si_so_toi_da, ','))
        {

            danh_sach_hoc_phan.push_back(ma_mon);
        }
        so_luong_hoc_phan++;
    }

    hocphan.close();
}
void dem_so_sinh_vien()
{
    ifstream sinhvien("../02_Data/Sinh_Vien.csv");
    string dong_du_lieu;
    // Doc bo dong tieu de dau tien (header)
    getline(sinhvien, dong_du_lieu);

    while (getline(sinhvien, dong_du_lieu))
    {
        if (dong_du_lieu.empty())
            continue;
        so_luong_sinh_vien++;
    }

    sinhvien.close();
}

void sinh_data(ll n)
{
    ofstream tep_tin("../02_Data/students_1k.csv");

    ll dem = 0;
    string hp;
    int sv;

    ll tong_so_dang_ky = 0;

    while (dem < n)
    {
        int lenhso = random(100);

        if (lenhso < 60)
        {
            hp = danh_sach_hoc_phan[random(so_luong_hoc_phan)];
            sv = 25001 + random(so_luong_sinh_vien);

            if (danh_sach_sinh_vien[sv][hp] == false)
            {
                danh_sach_sinh_vien[sv][hp] = true;
                tong_so_dang_ky++;
                tep_tin << "ADD," << hp << "," << sv << "\n";
                dem++;
            }
        }
        else if (lenhso < 80)
        {
            if (tong_so_dang_ky == 0)
                continue;

            int sv_remove;
            vector<string> cac_mon_da_dang_ky;

            while (true)
            {
                sv_remove = 25001 + random(so_luong_sinh_vien);

                if (danh_sach_sinh_vien.find(sv_remove) != danh_sach_sinh_vien.end())
                {
                    for (pair<string, bool> pair : danh_sach_sinh_vien[sv_remove])
                    {
                        if (pair.second == true)
                        {
                            cac_mon_da_dang_ky.push_back(pair.first);
                        }
                    }

                    if (!cac_mon_da_dang_ky.empty())
                    {
                        break;
                    }
                }
            }

            hp = cac_mon_da_dang_ky[random(cac_mon_da_dang_ky.size())];

            danh_sach_sinh_vien[sv_remove][hp] = false;
            tong_so_dang_ky--;
            tep_tin << "REMOVE," << hp << "," << sv_remove << "\n";
            dem++;
        }
        else if (lenhso < 90)
        {
            sv = 25001 + random(so_luong_sinh_vien);
            tep_tin << "GETSV," << sv << "\n";
            dem++;
        }
        else
        {
            hp = danh_sach_hoc_phan[random(so_luong_hoc_phan)];
            tep_tin << "GETHP," << hp << "\n";
            dem++;
        }
    }

    tep_tin.close();
}

int main()
{
    ll solenh;
    cin >> solenh;
    nhap_danh_sach_hoc_phan();
    dem_so_sinh_vien();
    sinh_data(solenh);
}

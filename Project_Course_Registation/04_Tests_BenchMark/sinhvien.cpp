#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

long long seed = 1111;

long long random(int x)
{
    seed = (1103515245 * seed + 12345) % 2147483648;
    return seed % x;
}

void Ngay_Thang(vector<pair<int, int>> &a, int x)
{
    int i = 0;
    int n, t;
    while (i < x)
    {
        t = random(12) + 1;
        while (true)
        {
            n = random(31) + 1;
            if (n <= 28)
                break;
            if (t == 1 || t == 3 || t == 5 || t == 7 || t == 8 || t == 10 || t == 12)
                break;
            if (t != 4 && n <= 30)
                break;
        }
        a.push_back({n, t});
        i++;
    }
}
void Ho_Ten(vector<string> &s, int n)
{
    ifstream file("../02_Data/ten.csv");
    string line;
    vector<string> Ho, Lnam, Lnu;
    vector<pair<string, int>> Ten;
    int i = 0;
    while (getline(file, line))
    {
        stringstream ss(line);
        string item;
        vector<string> row;

        while (getline(ss, item, ','))
        {
            row.push_back(item);
        }
        if (i < 10)
        {
            Ho.push_back(row[0]);
        }
        else if (i < 20)
        {
            Lnam.push_back(row[0]);
        }
        else if (i < 30)
        {
            Lnu.push_back(row[0]);
        }
        else
        {
            if (row.size() >= 2)
            {
                Ten.push_back({row[0], stoi(row[1])});
            }
        }
        i++;
    }
    file.close();

    int soVongLap = n / 100;
    for (int i = 0; i < 100; ++i)
    {
        for (int k = 0; k < soVongLap; ++k)
        {
            int dodai = random(3) + 2;

            string ten = Ten[i].first;
            int gioi_tinh = Ten[i].second; // 0: nam nữ đều được, 1: nam, 2: nữ

            if (gioi_tinh == 0)
            {
                // j ở đây đang là j hiện tại sau khi đã bị dịch chuyển bởi update_seed
                gioi_tinh = (i + k) % 2 + 1; // Đảm bảo luôn ra 1 (nam) hoặc 2 (nữ)
            }

            int ho = random(10);
            string ho1 = Ho[ho];
            string ho2 = "";

            // Nếu độ dài = 4 thì chạy seed lấy thêm họ 2
            while (dodai == 4)
            {
                ho = random(10);
                ho2 = Ho[ho];
                if (ho2 != ho1)
                    break;
            }

            // 4. Lấy Lót (Nếu độ dài >= 3)
            string lot = "";
            if (dodai >= 3)
            {
                do
                {
                    int tlot = random(10);
                    if (gioi_tinh == 1)
                    {
                        lot = Lnam[tlot];
                    }
                    else
                    {
                        lot = Lnu[tlot];
                    }
                } while (lot == ten);
            }

            // 5. Nối chuỗi tạo tên hoàn chỉnh
            string full_name = ho1;
            if (dodai == 4)
            {
                full_name += " " + ho2;
            }
            if (dodai >= 3)
            {
                full_name += " " + lot;
            }
            full_name += " " + ten;

            // Push vào vector tổng
            s.push_back(full_name);
        }
    }
}
string Nganh(vector<string> s, int x)
{
    x = random(4);
    return s[x];
}

int main()
{
    vector<string> name;
    vector<pair<int, int>> sinh;

    vector<string> nganh = {"CNTT", "ATTT", "KTDL", "AI"};
    int n ;
    cout<<"Nhap so luong sinh vien: ";
    cin>>n;
    Ho_Ten(name, n);
    Ngay_Thang(sinh, n);

    string filename = "../02_Data/Sinh_Vien.csv";

    ofstream file(filename);

    file << "mssv,ho_ten,ma_nganh,ngay_sinh\n";

    int mssv = 250000;
    // 4. Ghi từng dòng dữ liệu
    for (int i = 0; i < n; i++)
    {
        file << ++mssv << ","
             << name[i] << ","
             << Nganh(nganh, 4) << ","
             << sinh[i].first << "/" << sinh[i].second << "/2007\n";
    }

    // 5. Đóng file
    file.close();

    cout << "Da ghi file CSV thanh cong: " << filename << endl;

    return 0;
}
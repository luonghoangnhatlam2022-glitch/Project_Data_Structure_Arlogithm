#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

#include "test_data.cpp"
using namespace std;
using namespace std::chrono;

void Benchmark(string duong_dan_file)
{
    // Moc thoi gian truoc khi chay
    auto start = high_resolution_clock::now();

    // Chay ham
    bool run = test_data(duong_dan_file);
    if (run)
    {
        cout << "Chay du lieu thanh cong! \n";
    }
    else
        cout << "Chay du lieu that bai! \n";

    // Thoi gian sau khi chay
    auto end = high_resolution_clock::now();

    // Tinh toan chenh lech
    // Doi ms
    auto duration_ms = duration_cast<milliseconds>(end - start);

    cout << "Thoi gian chay: " << duration_ms.count() << " mili-giay.\n";
}
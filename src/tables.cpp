//
// Created by Bohdan on 5/8/2021.
//

#include <iostream>
#include <chrono>

#include <PolyphaseMergeSort.hpp>

#include "fort.hpp"


using namespace std::chrono;
using namespace std;





int main() {
    const int MinPowOfTwo = 3;
    const int MaxPowOfTwo = 20;

    fort::char_table table;

    table << fort::header;

    table[0][0] = "N";
    table[0][0].set_cell_content_text_style(fort::text_style::bold);
    table[0][0].set_cell_content_fg_color(fort::color::cyan);

    table[1][0] = "TIME";
    table[1][0].set_cell_content_text_style(fort::text_style::bold);
    table[1][0].set_cell_content_fg_color(fort::color::cyan);

    for (int i = 1; i <= MaxPowOfTwo; ++i) {
        table[0][i].set_cell_content_fg_color(fort::color::light_red);
    }


    arr<int> in;
    for (int i = MinPowOfTwo, tableId = 1; i < MaxPowOfTwo; ++i, ++tableId) {
        int n = pow(2, i);

        in.resize(n);

        for (int i = 0; i < n; ++i) {
            in[i] = random();
        }

        auto start = steady_clock::now();
        MergeSort<int>::Sort(in, 0, n - 1, 1);
        auto end = steady_clock::now();

        table[0][tableId] = std::to_string(n);
        table[1][tableId] = std::to_string(duration_cast<microseconds>(end - start).count());
    }



    std::cout << table.to_string();






//    std::cout << "end sort " << (duration_cast<milliseconds>(end - start).count()) * 0.001 << " sec" << std::endl;

    return 0;
}



//
// Created by Bohdan on 5/8/2021.
//

#include <chrono>
#include <cmath>

#include <PolyphaseMergeSort.hpp>

#include "fort.hpp"


using namespace std::chrono;
using namespace std;





int main() {
    const int MinPowOfTwo = 18;
    const int MaxPowOfTwo = 25;
    const int MinAmountOfThreads = 1;
    const int MaxAmountOfThreads = 8;

    fort::char_table table;

    table << fort::header;

    table[0][0] = "Threads\\N";
    table[0][0].set_cell_content_text_style(fort::text_style::bold);
    table[0][0].set_cell_content_fg_color(fort::color::cyan);


    for (int rowId = 1, MPOT = MinPowOfTwo; MPOT <= MaxPowOfTwo; ++rowId, ++MPOT) {
        table[0][rowId].set_cell_content_fg_color(fort::color::light_red);
        table[0][rowId] = std::to_string(int64_t(std::pow(2, MPOT)));
    }


//    int * in;

    for (int AmountOfThreads = MinAmountOfThreads, rows = 1;
             AmountOfThreads <= MaxAmountOfThreads;
             ++AmountOfThreads, ++rows) {

        table[rows][0] = "TH " + to_string(AmountOfThreads);
        table[rows][0].set_cell_content_text_style(fort::text_style::bold);
        table[rows][0].set_cell_content_fg_color(fort::color::light_yellow);

        for (int i = MinPowOfTwo, rowId = 1; i <= MaxPowOfTwo; ++i, ++rowId) {
            const int n = pow(2, i);


            const char * s_file = "source.bin";
            FILE * source = fopen(s_file, "wb");

            int RandomNumber;
            for (int j = n; j > 0; j--) {
                RandomNumber = rand(); //module?
                fwrite(&RandomNumber, sizeof(int), 1, source);
            }
            fclose(source);

            std::cout << "finished filling\n";
            /*in = new int [n];

            for (int j = 0; j < n; ++j) {
                in[j] = random();
            }*/



            auto start = steady_clock::now();
//            MergeSort<int>::Sort2(in, n, AmountOfThreads);
            PolyphaseMergeSort<int>::SortFile(s_file, 6, n, n / 64 + n % 64, AmountOfThreads,
                                              PolyphaseMergeSort<int>::Sort::Sort2);
            auto end = steady_clock::now();

            table[rows][rowId] = std::to_string(duration_cast<microseconds>(end - start).count());

//            delete [] in;
        }
    }



    std::cout << table.to_string();

    return 0;
}



#include <iostream>
#include <chrono>

#include <PolyphaseMergeSort.hpp>

#include <doctest/doctest.h>

using namespace std::chrono;
using namespace std;




/*TEST_CASE("Test for correct work of algorithm") try {
    const char * s_file = "source.bin";
    const char * o_file = "0.bin";
    const int n         = 30000000;


    FILE * source = fopen(s_file, "wb");


   int RandomNumber;
    for (int i = n; i > 0; i--) {
        RandomNumber = rand(); //module?
        fwrite(&RandomNumber, sizeof(int), 1, source);
    }
    fclose(source);

    std::cout << "finished filling\n";







    auto start = steady_clock::now();
    PolyphaseMergeSort::SortFile(s_file, 6, n, 1000000);
    auto end = steady_clock::now();



    bool isCorrect = PolyphaseMergeSort::isSorted(o_file);

    std::cout << "end sort " << (duration_cast<milliseconds>(end - start).count()) * 0.001 << " sec" << std::endl;



    CHECK(isCorrect);
} catch (const exception & ex) {
    std::cout << ex.what();
}*/



TEST_CASE("Test ConcurrencySort") {
    const int n = 1000000 ;

    auto * in = new int [n];

    for (int i = 0; i < n; ++i) {
        in[i] = random();
    }

//    MergeSort<int>::PrintArr(in, n);



    auto start = steady_clock::now();
    MergeSort<int>::Sort(in, 0, n - 1, 8);
    auto end = steady_clock::now();

//    MergeSort<int>::PrintArr(in, n);

    std::cout << "end sort " << (duration_cast<microseconds>(end - start).count()) << std::endl;

    CHECK(MergeSort<int>::isSorted(in, n));
}

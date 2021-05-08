#include <iostream>
#include <chrono>

#include <PolyphaseMergeSort.hpp>

#include <doctest/doctest.h>

using namespace std::chrono;
using namespace std;

template<typename Type>
void PrintArr (arr<Type> & A, const int n) {
    for (int i = 0; i < n; ++i) {
        std::cout << A[i] << ' ';
    }
    std::cout << std::endl;
}


/*TEST_CASE("Test for correct work of algorithm") {
    const char * s_file = "source.bin";
    const char * o_file = "0.bin";
    const int n         = 30000;


    FILE * s = fopen(s_file, "wb");
    FILE * output = fopen(o_file, "rb");


    int RandomNumber;
    for (int i = n; i > 0; i--) {
        RandomNumber = rand() % 1000; //module?
        fwrite(&RandomNumber, sizeof(int), 1, s);
    }
    fclose(s);









    auto start = steady_clock::now();
    PolyphaseMergeSort::SortFile(s_file, 5, n, 3300);
    auto end = steady_clock::now();



    bool isCorrect = PolyphaseMergeSort::isSorted(o_file);

    std::cout << "end sort " << (duration_cast<milliseconds>(end - start).count()) * 0.001 << " sec" << std::endl;



    CHECK(isCorrect);
}*/


TEST_CASE("Test ConcurrencySort") {
    const int n = 3;

    arr<int> in (n);
    arr<int> out (n);

    for (int i = 0; i < n; ++i) {
        in[i] = n - i;
    }

    //    print "arr"
    PrintArr(in, n);

    MergeSort<int>::Sort(in, 0, n - 1, out);

//    print "out"
    PrintArr(out, n);
}
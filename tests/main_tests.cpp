#include <chrono>

#include <PolyphaseMergeSort.hpp>


#include <doctest/doctest.h>

using namespace std::chrono;
using namespace std;




TEST_CASE("Test for correct work of algorithm") try {
    const char * s_file = "source.bin";
    const char * o_file = "0.bin";
    const int n         = 3000000;


    FILE * source = fopen(s_file, "wb");


   int RandomNumber;
    for (int i = n; i > 0; i--) {
        RandomNumber = rand(); //module?
        fwrite(&RandomNumber, sizeof(int), 1, source);
    }
    fclose(source);

    std::cout << "finished filling\n";







    auto start = steady_clock::now();
    PolyphaseMergeSort<int>::SortFile(s_file, 6, n, 100000, 8, PolyphaseMergeSort<int>::Sort::Sort2);
    auto end = steady_clock::now();



    bool isCorrect = PolyphaseMergeSort<int>::isSorted(o_file);

    std::cout << "end sort " << (duration_cast<milliseconds>(end - start).count()) * 0.001 << " sec" << std::endl;



    CHECK(isCorrect);
} catch (const exception & ex) {
    std::cout << ex.what();
}


TEST_CASE("Test ConcurrencySort1") {
    const int n = 1000000;

    auto * in = new int [n];

    for (int i = 0; i < n; ++i) {
        in[i] = n - i;
    }

//    MergeSort<int>::PrintArr(in, n);

    auto start = steady_clock::now();
    MergeSort<int>::Sort1(in, n, 5);
    auto end   = steady_clock::now();

//    MergeSort<int>::PrintArr(in, n);

    std::cout << "end sort " << (duration_cast<microseconds>(end - start).count()) << std::endl;

    CHECK(MergeSort<int>::isSorted(in, n));
}


TEST_CASE("Test ConcurrencySort2") {
    const int n = 1000000;

    auto * in = new int [n];

    for (int i = 0; i < n; ++i) {
        in[i] = n - i;
    }

//    MergeSort<int>::PrintArr(in, n);

    auto start = steady_clock::now();
    MergeSort<int>::Sort2(in, n, 5);
    auto end   = steady_clock::now();

//    MergeSort<int>::PrintArr(in, n);

    std::cout << "end sort " << (duration_cast<microseconds>(end - start).count()) << std::endl;

    CHECK(MergeSort<int>::isSorted(in, n));
}


TEST_CASE("Test BinaryAscendingHeap") {
    const size_t n = 100;

    BinaryHeap<int, char> Heap(n);

    for (int i = 0; i < n; ++i) {
        Heap.Insert({random(), 'a' + i % 26});
    }


    for (int i = 0; i < n; i += n / 3) {
        Heap.IncreaseKey(i, random());
    }


    auto * arr = new int [n];

    for (int i = 0; !Heap.isEmpty(); ++i) {
        arr[i] = Heap.GetMinimum().first;
        Heap.ExtractMinimum();
    }

    CHECK(MergeSort<int>::isSorted(arr, n));
}

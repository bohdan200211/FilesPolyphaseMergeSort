#include <iostream>
#include <chrono>

#include <PolyphaseMergeSort.hpp>

using namespace std::chrono;
using namespace std;





int main() {
    const char * s_file = "source.bin";
    const char * o_file = "0.bin";
    const int n = 10000;


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





//    int ReadNumber;
//    while (fread(&ReadNumber, sizeof(int), 1, output)) {
//        cout << ReadNumber << ", ";
//    }
//    cout << "\n\n\n";

    if (PolyphaseMergeSort::isSorted(o_file)){
        cout << "success";
    } else {
        cout << "fault";
    }
    cout << endl;
    std::cout << "end sort " << (duration_cast<milliseconds>(end - start).count()) * 0.001 << " sec" << std::endl;

    return 0;
}

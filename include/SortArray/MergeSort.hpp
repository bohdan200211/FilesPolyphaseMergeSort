//
// Created by Bohdan on 5/6/2021.
//

#pragma once

#include <thread>
#include <algorithm>
#include <cstring>
#include <iostream>

#include "../SpecialHeap/BinaryHeap.hpp"



template<typename Type>
class MergeSort {
public:
    static void Sort1 (Type * A, size_t n, size_t AmountOfTh);
    static void Sort2 (Type * A, size_t n, size_t AmountOfTh);

    static void PrintArr (Type * A, size_t n);
    static bool isSorted (Type * A, size_t n);

private:
    static void _sort2 (Type * A, int left, int right, size_t AmountOfTh);

    static void _merge1 (Type * out, const Type * in, const size_t * Intervals, size_t AmountOfThreads);
    static void _merge2 (Type * A, int left, int mid, int right);
};

#include "MergeSort.inl"
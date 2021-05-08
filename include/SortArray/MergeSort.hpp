//
// Created by Bohdan on 5/6/2021.
//

#pragma once

#include <thread>
#include <vector>
#include <iostream>
#include <algorithm>



template<typename Type>
class MergeSort {
public:
    static void Sort (Type * A, int left, int right, size_t AmountOfTh);

    static void PrintArr (Type * A, const int n);

    static bool isSorted (Type * A, const int n);

private:
    static void _merge (Type * A, int left, int mid, int right);
};

#include "MergeSort.inl"
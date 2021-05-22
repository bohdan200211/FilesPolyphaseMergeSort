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
/// This algorithm create Split-List also know as Intervals from _merge1() sorts N subarrays.
    static void Sort1 (Type * A, size_t n, size_t AmountOfTh);

/// The start point of _sort2() function
    static void Sort2 (Type * A, size_t n, size_t AmountOfTh);

/// Print Array in one row divided by Space
    static void PrintArr (Type * A, size_t n);

/// The simple method that can check array for Sorted-property
    static bool isSorted (Type * A, size_t n);

private:
/// This is the recursively algorithm that splits the A in half and evoke _sort2(A1.left, A2.right, AmountOfTh / 2)
/// for each part.
/// @see T.Cormen Introduction to Algorithms. Theme: The Concurrency Merge Sort algorithm
    static void _sort2 (Type * A, int left, int right, size_t AmountOfTh);


/// This algorithm sorts N sorted subarrays in 1 array using BinaryHeap Binary Heap with min-heap property.
/// @param in Array that contains N sorted subarrays
/// @param Intervals Array that indicates where each subarray ends.
/// Example [ Intervals[i - 1], Intervals[i] ) indicates where the i-subarray ends(i counts from 0 to N - 1)
/// @see https://www.geeksforgeeks.org/merge-k-sorted-arrays/ or other sources
/// @note N - this is value that indicates amount of sorted subarrays
    static void _merge1 (Type * out, const Type * in, const size_t * Intervals, size_t N);

/// This is the basic Merge() algorithm that used in the Simple Merge Sort
/// @see https://www.geeksforgeeks.org/merge-sort/ or other sources
    static void _merge2 (Type * A, int left, int mid, int right);
};

#include "MergeSort.inl"
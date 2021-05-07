//
// Created by Bohdan on 5/6/2021.
//

#pragma once

#include <thread>
#include <vector>

template<typename Type>
using arr = std::vector<Type>;

template<typename Type>
class MergeSort {
public:
    static void Sort (arr<Type> & A, const int left, const int right, arr<Type> & B, const int st = 0);

private:


    static void _merge (arr<Type> & T, int left1, int right1, int left2, int right2,
                        arr<Type> & A, int left3);

    static int _bin_search (const Type & pivot , arr<Type> & T, int left, int right);
};

template<typename Type>
void MergeSort<Type>::Sort (arr<Type> & A, const int p, const int r, arr<Type> & B, const int st) {
    int n = r - p + 1;
    if (n == 1) {
        B[st] = A[p];
    } else {
        arr<Type> T(n);

        int q1 = (p + r) / 2;
        int q2 = q1 - p + 1;

//        std::thread th(Sort, A, p, q1, T, 0);
        Sort(A, p, q1, T, 0);

        Sort(A, q1 + 1, r, T, q2);

//        th.join();

        _merge(T, 0, q2 - 1, q2, n - 1, B, st);

//        delete [] T;
    }
}


template<typename Type>
int MergeSort<Type>::_bin_search (const Type & pivot, arr<Type> & A, int left, int right) {
    int low  = left;
    int high = std::max(left, right + 1);

    while (low < high) {
        int mid = (low + high) / 2;

        if (pivot <= A[mid]) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }

    return high;
}

template<typename Type>
void MergeSort<Type>::_merge (arr<Type> & T, int left1, int right1, int left2, int right2, arr<Type> & B, int left3) { //A - arr that will returned
    int n1 = right1 - left1 + 1;
    int n2 = right2 - left2 + 1;

    if (n1 < n2) {
        std::swap(left1, left2);
        std::swap(right1, right2);
        std::swap(n1, n2);
    }

    if (n1 == 0) {
        return;
    }

    int q1 = (left1 + right1) / 2;
    int q2 = _bin_search(T[q1], T, left2, right2);
    int q3 = left3 + (q1 - left1) + (q2 - left2);

    B[q3] = T[q1];

//    std::thread th(_merge, T, left1, q1 - 1, left2, q2 - 1, A, left3);
    _merge(T, left1, q1 - 1, left2, q2 - 1, B, left3);

    _merge(T, q1 + 1, right1, q2, right2, B, q3 + 1);

//    th.join();
}

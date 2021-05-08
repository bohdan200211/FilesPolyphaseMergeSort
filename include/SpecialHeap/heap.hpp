//
// Created by Bohdan on 07.09.2020.
//


#pragma once

#include <cstdio>
#include <algorithm>
#include <cmath>

const int n_size = sizeof(int);

using namespace std;


class Heap {
private:
    struct Chunk {
        int    MinValue;
        FILE * File;
    };
    int   capacity;
    int   size;
    Chunk * heap;

    inline int parent(int i) { return (i - 1) / 2; }
    inline int left(int i)   { return i * 2 + 1; }
    inline int right(int i)  { return i * 2 + 2; }

    void heapify(int index);
public:
    Heap(int n_size);

    ~Heap();

    inline void SetSizeToZero () { size = 0; }
    inline bool isEmpty() { return heap[0].MinValue == INT32_MAX; }

    int GetMinimum();
    void Insert(int x, FILE * ptr);
};

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
        int    iteration;
    };
    int   capacity;
    int   size;
    Chunk * heap;

    int parent(int i) { return (i - 1)/2;}
    int left(int i)   { return i*2 + 1;}
    int right(int i)  { return i*2 + 2;}

    void heapify(int index) {
        int r = right(index);
        int l = left(index);

        int smallest = index;

        if (l < size && heap[l].MinValue < heap[smallest].MinValue)
            smallest = l;

        if (r < size && heap[r].MinValue < heap[smallest].MinValue)
            smallest = r;

        if (smallest != index) {
            swap(heap[smallest], heap[index]);
            heapify(smallest);
        }
    }
public:
    Heap(int n_size) {
        capacity = n_size;
        size     = 0;
        heap     = new Chunk[n_size];
    }

    ~Heap() {
        for (int i = 0; i < capacity; i++) {
            fclose(heap[i].File);
        }
    }

    void SetSizeToZero () { size = 0; }
    bool isEmpty() { return heap[0].MinValue == INT32_MAX; }

    int GetMinimum() {
        int root = heap[0].MinValue;

        if(!fread(&heap[0].MinValue, n_size, 1, heap[0].File)
            || root > heap[0].MinValue) {


            if(root > heap[0].MinValue) {
                fseek(heap[0].File, -n_size, SEEK_CUR);
            }

            heap[0].MinValue = INT32_MAX;
        }
        heapify(0);
        return root;
    }
    void Insert(int x, FILE * ptr, int y) {
        size++;
        int index = size - 1;

        if (x == INT32_MAX) {
            heap[index] = {INT32_MAX,
                           ptr,
                           0};
            return;
        } else {
            heap[index] = {x,
                           ptr,
                           y};
        }

        while (index != 0) {
            int p = parent(index);
            if(heap[p].MinValue <= heap[index].MinValue) {
                break;
            }

            swap(heap[p], heap[index]);
            index = p;
        }
    }
};

//
// Created by Bohdan on 5/8/2021.
//

#include "Heap.hpp"

Heap::Heap(int n_size) {
    capacity = n_size;
    size     = 0;
    heap     = new Chunk[n_size];
}

Heap::~Heap() {
    for (int i = 0; i < capacity; i++) {
        fclose(heap[i].File);
    }

    delete [] heap;
}

void Heap::heapify(int index) {
    int r = right(index);
    int l = left(index);

    int smallest = index;

    if (l < size &&
        heap[l].MinValue < heap[smallest].MinValue) {

        smallest = l;
    }

    if (r < size &&
        heap[r].MinValue < heap[smallest].MinValue) {

        smallest = r;
    }

    if (smallest != index) {
        swap(heap[smallest], heap[index]);
        heapify(smallest);
    }
}

int Heap::GetMinimum() {
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

void Heap::Insert(int x, FILE * ptr) {
    size++;
    int index = size - 1;

    if (x == INT32_MAX) {
        heap[index] = {INT32_MAX,
                       ptr};
        return;
    } else {
        heap[index] = {x,
                       ptr};
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
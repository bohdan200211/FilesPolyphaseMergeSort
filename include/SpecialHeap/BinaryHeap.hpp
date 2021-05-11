//
// Created by Bohdan on 07.09.2020.
//


#pragma once

#include <cstdio>
#include <stdexcept>

template<typename Key, typename Data>
class BinaryHeap {
private:
    using Elm = std::pair<Key, Data>;

    size_t m_Capacity;
    size_t m_Size;
    Elm *  m_Heap;

    [[nodiscard]] inline size_t parent(size_t i) const { return (i - 1) / 2; }
    [[nodiscard]] inline size_t left  (size_t i) const { return i * 2 + 1; }
    [[nodiscard]] inline size_t right (size_t i) const { return i * 2 + 2; }

    void heapify(size_t index);

public:
    BinaryHeap (size_t _size);
    ~BinaryHeap ();

    inline bool isEmpty () const { return m_Size == 0; }

    Elm & GetMinimum () const;
    void ExtractMinimum ();

    void Insert (const Elm & x);
    void DecreaseKey (size_t index, const Key & newKey);
    void IncreaseKey (size_t index, const Key & newKey);
};

#include "BinaryHeap.inl"

//
// Created by Bohdan on 5/8/2021.
//


template<typename Key, typename Data>
BinaryHeap<Key, Data>::BinaryHeap(size_t _size) {
    m_Capacity = _size;
    m_Size     = 0;
    m_Heap     = new Elm[_size];
}

template<typename Key, typename Data>
BinaryHeap<Key, Data>::~BinaryHeap() {
    delete [] m_Heap;
}

template<typename Key, typename Data>
void BinaryHeap<Key, Data>::heapify(size_t index) {
    size_t rightSon = right(index);
    size_t leftSon  = left(index);

    size_t smallest = index;

    if (leftSon < m_Size &&
        m_Heap[leftSon].first < m_Heap[smallest].first) {

        smallest = leftSon;
    }

    if (rightSon < m_Size &&
        m_Heap[rightSon].first < m_Heap[smallest].first) {

        smallest = rightSon;
    }

    if (smallest != index) {
        std::swap(m_Heap[smallest], m_Heap[index]);

        heapify(smallest);
    }
}

template<typename Key, typename Data>
typename BinaryHeap<Key, Data>::Elm & BinaryHeap<Key, Data>::GetMinimum() const {
    return m_Heap[0];
}

template<typename Key, typename Data>
void BinaryHeap<Key, Data>::Insert(const Elm & x) {
    if (m_Size == m_Capacity) {
        throw std::runtime_error("Heap overflow\n");
    }


    ++m_Size;
    int index = m_Size - 1;


//    DecreaseKey(index, x);

    m_Heap[index] = x;

    while (index != 0 && m_Heap[parent(index)].first > m_Heap[index].first) {
        std::swap(m_Heap[index], m_Heap[parent(index)]);

        index = parent(index);
    }
}

template<typename Key, typename Data>
void BinaryHeap<Key, Data>::DecreaseKey(size_t index, const Key & newKey) {
    if (m_Heap[index].first <= newKey) {
        return;
    }

    m_Heap[index].first = newKey;
    while (index != 0 && m_Heap[parent(index)].first > m_Heap[index].first) {
        std::swap(m_Heap[index], m_Heap[parent(index)]);
        index = parent(index);
    }
}

template<typename Key, typename Data>
void BinaryHeap<Key, Data>::ExtractMinimum() {
    if (m_Size == 0) {
        throw std::runtime_error("Try to extract min from empty Heap\n");
    }

    if (m_Size == 1) {
        --m_Size;
        return;
    }

    m_Heap[0] = m_Heap[m_Size - 1];

    --m_Size;

    heapify(0);
}

template<typename Key, typename Data>
void BinaryHeap<Key, Data>::IncreaseKey(size_t index, const Key & newKey) {
    if (m_Heap[index].first >= newKey) {
        return;
    }

    m_Heap[index].first = newKey;

    heapify(index);
}

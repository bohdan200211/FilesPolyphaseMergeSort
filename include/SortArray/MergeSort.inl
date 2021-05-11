//
// Created by Bohdan on 5/6/2021.
//


template<typename Type>
void MergeSort<Type>::Sort1 (Type * A, const size_t n, size_t AmountOfThreads) {
    if (AmountOfThreads == 0) {
        throw std::runtime_error("AmountOfThread = 0");
    }

    if (n > 0) {
        //sort intervals part

        size_t OptimalAmountOfThreads = std::min(n, AmountOfThreads);
        size_t ThreadInterval         = n / OptimalAmountOfThreads;
        size_t ThreadRemainder        = n % OptimalAmountOfThreads;

        size_t Intervals[OptimalAmountOfThreads];

        for (auto & x : Intervals) {
            x = ThreadInterval;
        }
        for (size_t i = 0; i < ThreadRemainder; ++i) {
            ++Intervals[i];
        }

        for (int i = 1; i < OptimalAmountOfThreads; ++i) {
            Intervals[i] += Intervals[i - 1];
        }


        size_t i        = 0;
        size_t StartPos = 0;

        auto OneThSort = [](Type * A, size_t st, size_t end) { std::sort(A + st, A + end); };

        std::thread ArrOfThreads[OptimalAmountOfThreads - 1];

        while (i < OptimalAmountOfThreads - 1) {

            ArrOfThreads[i] = std::thread(OneThSort, A, StartPos, Intervals[i]);
//            OneThSort(StartPos, Intervals[i]);


//            std::sort(A + StartPos, A + Intervals[i]);


            StartPos = Intervals[i];

            ++i;
        }


        std::sort(A + StartPos, A + Intervals[i]);

        
        for (auto & th : ArrOfThreads) {
            if (th.joinable()){
                th.join();
            }
        }


        //merging part
        if (AmountOfThreads > 1) {
            auto *copiedArr = new Type[n];
            std::memcpy(copiedArr, A, n * sizeof(Type));


            _merge1(A, copiedArr, Intervals, OptimalAmountOfThreads);


            delete[] copiedArr;
        }
    }
}


template<typename Type>
void MergeSort<Type>::Sort2(Type * A, size_t n, size_t AmountOfTh) {
    _sort2(A, 0, n - 1, AmountOfTh);
}


template<typename Type>
void MergeSort<Type>::PrintArr (Type * A, const size_t n) {
    for (int i = 0; i < n; ++i) {
        std::cout << A[i] << ' ';
    }
    std::cout << std::endl;
}

template<typename Type>
bool MergeSort<Type>::isSorted (Type * A, const size_t n) {
    for (int i = 1; i < n; ++i) {
        if (A[i - 1] > A[i]) {
            return false;
        }
    }

    return true;
}


template<typename Type>
void MergeSort<Type>::_sort2 (Type * A, int left, int right, size_t AmountOfTh) {
    int n = right - left + 1;

    if (n > 0) {
        if (AmountOfTh == 0) {
            std::sort(A + left, A + right + 1);
        } else {
            int mid = (left + right) / 2;


            std::thread th(_sort2, A, left, mid, AmountOfTh / 2);
//            Sort(std::ref(A), left, mid, AmountOfTh / 2);

            _sort2(A, mid + 1, right, AmountOfTh / 2);


            th.join();

            _merge2(A, left, mid, right);
        }
    }
}



template<typename Type>
void MergeSort<Type>::_merge1 (Type * out, const Type * in, const size_t * Intervals, const size_t AmountOfThreads) {
    const size_t n = Intervals[AmountOfThreads - 1];

//    < Key, { currPoint, EndPoint } >
    BinaryHeap<Type, std::pair<size_t, size_t>> Heap(AmountOfThreads);


    //create init "Heap"
    Heap.Insert({ in[0], { 0, Intervals[1] } });
    for (int i = 0; i < AmountOfThreads - 1; ++i) {
        Heap.Insert({ in[Intervals[i]], { Intervals[i], Intervals[i + 1] } });
    }

    for (int i = 0; i < n; ++i) {
        auto & x = Heap.GetMinimum();

//        copy min elm into sorted arr
        out[i] = x.first;

//        move currId
        x.second.first++;

//        check if currId == endOfInterval
        if (x.second.first == x.second.second) {
            Heap.ExtractMinimum();
        } else {
            Heap.IncreaseKey(0, in[x.second.first]);
        }
    }
}

template<typename Type>
void MergeSort<Type>::_merge2 (Type * A, int left, int mid, int right) { //A - arr that will returned
    int nLeft;
    int nRight;

    //size of left and right sub-arrays
    nLeft  = mid - left + 1;
    nRight = right - mid;

    auto * arrLeft  = new Type[nLeft];
    auto * arrRight = new Type[nRight];

    //fill left and right sub-arrays
    for(int i = 0; i < nLeft; ++i) {
        arrLeft[i]  = A[left + i];
    }
    for(int j = 0; j < nRight; ++j) {
        arrRight[j] = A[mid + j + 1];
    }

    int indexLeft  = 0;
    int indexRight = 0;
    int indexArray = left;

    //marge temp arrays to real array
    while (indexLeft < nLeft &&
           indexRight < nRight) {
        if (arrLeft[indexLeft] <= arrRight[indexRight]) {
            A[indexArray] = arrLeft[indexLeft];
            ++indexLeft;
        } else {
            A[indexArray] = arrRight[indexRight];
            ++indexRight;
        }
        ++indexArray;
    }


    while (indexLeft < nLeft) {
        A[indexArray] = arrLeft[indexLeft];
        ++indexLeft;
        ++indexArray;
    }

    while (indexRight < nRight) {
        A[indexArray] = arrRight[indexRight];
        ++indexRight;
        ++indexArray;
    }

    delete [] arrLeft;
    delete [] arrRight;
}

//
// Created by Bohdan on 5/6/2021.
//

template<typename Type>
void MergeSort<Type>::Sort (Type * A, int left, int right, size_t AmountOfTh) {
    int n = right - left + 1;

    if (n > 0) {
        if (AmountOfTh == 0) {
            std::sort(A + left, A + right + 1);
        } else {
            int mid = (left + right) / 2;


            std::thread th(Sort, std::ref(A), left, mid, AmountOfTh / 2);
//            Sort(std::ref(A), left, mid, AmountOfTh / 2);

            Sort(std::ref(A), mid + 1, right, AmountOfTh / 2);


            th.join();

            _merge(A, left, mid, right);
        }
    }
}



template<typename Type>
void MergeSort<Type>::_merge (Type * A, int left, int mid, int right) { //A - arr that will returned
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

template<typename Type>
void MergeSort<Type>::PrintArr (Type * A, const int n) {
    for (int i = 0; i < n; ++i) {
        std::cout << A[i] << ' ';
    }
    std::cout << std::endl;
}

template<typename Type>
bool MergeSort<Type>::isSorted (Type * A, const int n) {
    for (int i = 1; i < n; ++i) {
        if (A[i - 1] > A[i]) {
            return false;
        }
    }

    return true;
}

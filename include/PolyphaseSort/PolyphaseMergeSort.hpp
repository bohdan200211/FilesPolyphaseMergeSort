//
// Created by Bohdan on 5/5/2021.
//

#pragma once

#include <cmath>

#include "../SpecialHeap/BinaryHeap.hpp"
#include "../SortArray/MergeSort.hpp"

/// Polyphase Merge Sort algorithm is very difficult.<br>
/// The best way to describe this algorithm is to write an article. So, there are <b>brief</b> descriptions below
template<typename IntegerType>
class PolyphaseMergeSort {
public:
    enum class Sort {Sort1, Sort2};

/// Main function that sort file
    static void SortFile (const char * SourceFile,
                          size_t AmountOfFiles, size_t AmountOfElms, size_t AmountOfElmsInChunk,
                          size_t AmountOfThread, Sort version = Sort::Sort2);

    static bool isSorted (const char * file);

private:
    static const char * extension;
    static size_t       iterations;
    static const size_t numSize;


/// Sorts each chunk using Concurrency Merge Sort and SetOfFiles
    static void FilesInit (const char * SourceFile, size_t AmountOfChunks,
                           FILE ** SetOfFiles,
                           const size_t * AmountOfChunksInFile, size_t AmountOfElmsInChunk, size_t AmountOfElms,
                           size_t AmountOfThread, Sort version = Sort::Sort2);

/// Initializes variable iterations and returns split list
    static size_t * CreateSplitList (size_t AmountOfFiles, size_t AmountOfChunks);

/// Create files and returns their pointers
    static FILE ** CreatePtrFilesByAmount (size_t n);

/// Convert number to string + ".bin"
    static const char * GetFilenameByNumber (size_t num);
};

#include "PolyphaseMergeSort.inl"
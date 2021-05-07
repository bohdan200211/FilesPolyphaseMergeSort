//
// Created by Bohdan on 5/5/2021.
//

#pragma once

#include "../SpecialHeap/heap.hpp"
#include "../SortArray/MergeSort.hpp"

class PolyphaseMergeSort {
public:
    static void SortFile (const char * SourceFile,
                          int AmountOfFiles, int AmountOfElms, int AmountOfElmsInChunk);

    static bool isSorted (const char * file);

private:
    static const char * extension;
    static int          iterations;



    static void FilesInit (const char * SourceFile, int AmountOfChunks,
                           FILE ** SetOfFiles,
                           const int * AmountOfChunksInFile, int AmountOfElmsInChunk, int AmountOfElms);

    static int * CreateSplitList (int num_of_files, int num_of_chunks);

    static FILE ** CreatePtrFilesByAmount (int n);

    static const char * GetFilenameByNumber (int num);
};

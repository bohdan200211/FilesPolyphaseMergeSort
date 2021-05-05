//
// Created by Bohdan on 5/5/2021.
//

#pragma once

#include "../SpecialHeap/heap.hpp"

class PolyphaseMergeSort {
public:
    static void SortFile (const char * SourceFile,
                          int num_of_files, int num_of_elements, int num_el_in_chunk);

    static bool isSorted (const char * file);

private:
    static const char * extension;
    static int          iterations;



    static void FilesInit (const char * SourceFile, int size_SetOfFiles,
                           FILE ** SetOfFiles,
                           const int * f_split, int num_elm_in_chunk, int num_of_elements);

    static int * CreateSplitList (int num_of_files, int num_of_chunks);

    static FILE ** CreatePtrFilesByAmount (int n);

    static const char * GetFilenameByNumber (int num);
};

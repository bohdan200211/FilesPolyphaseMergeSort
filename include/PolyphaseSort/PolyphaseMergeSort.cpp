//
// Created by Bohdan on 5/5/2021.
//

#include "PolyphaseMergeSort.hpp"

const char * PolyphaseMergeSort::extension  = ".bin";
int          PolyphaseMergeSort::iterations = 1;

void PolyphaseMergeSort::SortFile(const char * SourceFile, const int AmountOfFiles, const int AmountOfElms,
                                  const int AmountOfElmsInChunk) {

    iterations = 1;

    const int num_of_chunks = AmountOfElms / AmountOfElmsInChunk
                              + (AmountOfElms % AmountOfElmsInChunk ? 1 : 0);

    int * AmountOfChunksInFile = CreateSplitList(AmountOfFiles, num_of_chunks);
    FILE ** SetOfFiles         = CreatePtrFilesByAmount(AmountOfFiles);

    FilesInit(SourceFile, AmountOfFiles, SetOfFiles, AmountOfChunksInFile, AmountOfElmsInChunk, AmountOfElms);

    int indexEmptyFile = (iterations - 1) % AmountOfFiles;
    int indexMinFile   = (indexEmptyFile - 1) % AmountOfFiles;
    int minAmountOfChunkInFile;
    int temp;

    if (indexMinFile == -1) {
        indexMinFile = AmountOfFiles - 1;
    }

    Heap heap = Heap(AmountOfFiles);
    for (int i = 0; i < AmountOfFiles; i++) {
        fclose(SetOfFiles[i]);
        SetOfFiles[i] = fopen(GetFilenameByNumber(i), "rb");
    }

    for (int c = 0; c < iterations; c++) {
        //reopen file in order to rewrite file
        fclose(SetOfFiles[indexEmptyFile]);
        SetOfFiles[indexEmptyFile] = fopen(GetFilenameByNumber(indexEmptyFile), "wb");

        minAmountOfChunkInFile = AmountOfChunksInFile[indexMinFile];

        for (int a = 0; a < minAmountOfChunkInFile; a++) {
            for (int i = 0; i < AmountOfFiles; i++) {
                if(fread(&temp, n_size, 1, SetOfFiles[i]))
                    heap.Insert(temp, SetOfFiles[i], minAmountOfChunkInFile);
                else
                    heap.Insert(INT32_MAX, SetOfFiles[i], 0);
            }
            while (!heap.isEmpty()) {
                temp = heap.GetMinimum();
                fwrite(&temp, n_size, 1, SetOfFiles[indexEmptyFile]);
            }
            heap.SetSizeToZero();
        }

        for (int i = 0; i < AmountOfFiles; i++) {
            if (AmountOfChunksInFile[i]) {
                AmountOfChunksInFile[i] -= minAmountOfChunkInFile;
            }
        }
        AmountOfChunksInFile[indexEmptyFile] = minAmountOfChunkInFile;

        fclose(SetOfFiles[indexEmptyFile]);

        SetOfFiles[indexEmptyFile] = fopen(GetFilenameByNumber(indexEmptyFile), "rb");


        indexEmptyFile = indexMinFile;
        indexMinFile   = (indexEmptyFile - 1) % AmountOfFiles;

        if (indexMinFile == -1) {
            indexMinFile = AmountOfFiles - 1;
        }
    }

    delete [] AmountOfChunksInFile;
    delete [] SetOfFiles;
}

bool PolyphaseMergeSort::isSorted(const char * file) {
    FILE * ptrFile = fopen(file, "rb");

    int parentElm = INT32_MIN;
    int currElm;
    while (fread(&currElm, n_size, 1, ptrFile)) {
        if (parentElm > currElm) {
            return false;
        }
        parentElm = currElm;
    }
    return true;
}




//private part
void PolyphaseMergeSort::FilesInit(const char * SourceFile, int AmountOfChunks, FILE ** SetOfFiles,
                                   const int * AmountOfChunksInFile, const int AmountOfElmsInChunk, const int AmountOfElms) {


    int Buffer[AmountOfElmsInChunk];
    int MinElmOfCurrChunk;
    int MaxElmOfPrevChunk  = INT32_MAX;
    int i                  = 0;
    int AmountOfFullChunks = AmountOfElms / AmountOfElmsInChunk;

    FILE * source = fopen(SourceFile, "rb");

    //multithreaded variables



    while (i < AmountOfChunks) {

        int j = 0;
        while (j++ < AmountOfChunksInFile[i]) {
            if (AmountOfFullChunks-- < 1) {
                goto l_out;
            }

            fread(&Buffer, n_size, AmountOfElmsInChunk, source);

            std::sort(Buffer, Buffer + AmountOfElmsInChunk);

            MinElmOfCurrChunk = Buffer[0];

            if (MaxElmOfPrevChunk != INT32_MAX && MaxElmOfPrevChunk < MinElmOfCurrChunk) {
                Buffer[0] = MaxElmOfPrevChunk;

                fseek(SetOfFiles[i], -n_size, SEEK_CUR);
                fwrite(&MinElmOfCurrChunk, n_size, 1, SetOfFiles[i]);
            }
            fwrite(&Buffer, n_size, AmountOfElmsInChunk, SetOfFiles[i]);

            MaxElmOfPrevChunk = Buffer[AmountOfElmsInChunk - 1];
        }
        MaxElmOfPrevChunk = INT32_MAX;
        i++;
    }

    l_out:

    int residualElms = AmountOfElms % AmountOfElmsInChunk;
    if (residualElms > 0) {
        int temper;
        int j = 0;


        while (fread(&temper,n_size,1,source)) {
            Buffer[j++] = temper;
        }


        std::sort(Buffer, Buffer + residualElms);


        MinElmOfCurrChunk = Buffer[0];

        if (MaxElmOfPrevChunk != INT32_MAX && MaxElmOfPrevChunk < MinElmOfCurrChunk) {
            Buffer[0] = MaxElmOfPrevChunk;
            fseek(SetOfFiles[i], -n_size, SEEK_CUR);
            fwrite(&MinElmOfCurrChunk, n_size, 1, SetOfFiles[i]);
        }

        fwrite(&Buffer, n_size, residualElms, SetOfFiles[i]);
    }
}

int * PolyphaseMergeSort::CreateSplitList (const int num_of_files, const int num_of_chunks) {
    int * arr          = new int[num_of_files];
    int currAmountOfChunks            = num_of_files - 1;
    int curr_index_max = 1;
    int max_n          = 1;

    arr[0] = 0;
    for (int i = 1; i < num_of_files; i++) {
        arr[i] = 1;
    }


    //init
    while (currAmountOfChunks < num_of_chunks) {
        currAmountOfChunks += (num_of_files - 2) * max_n;

        for (int i = 0; i < num_of_files; i++) {
            arr[i] += max_n;
        }
        arr[curr_index_max] = 0;
        curr_index_max      = (curr_index_max + 1) % num_of_files;
        max_n               = arr[curr_index_max];
        iterations++;
    }
    return arr;
}

FILE ** PolyphaseMergeSort::CreatePtrFilesByAmount (const int n) {
    FILE ** set_of_filenames = new FILE* [n];


    for (int i = 0; i < n; i++) {
        set_of_filenames[i] = fopen(GetFilenameByNumber(i), "wb");
    }

    return set_of_filenames;
}

const char * PolyphaseMergeSort::GetFilenameByNumber (int number) {
    int length = int(log10(number + 1) + 1);


    //length + ".bin" + \'0'
    const int newLength = length + 5;
    char * filename = new char[newLength];

    for (int i = length - 1; i >= 0; i--) {
        filename[i] = char(number % 10 + '0');
        number /= 10;
    }
    for (int i = length, j = 0; i < newLength; i++, j++) {
        filename[i] = extension[j];
    }

    filename[length + 4] = '\0';

    return filename;
}

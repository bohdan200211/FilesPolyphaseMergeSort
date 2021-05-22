//
// Created by Bohdan on 5/5/2021.
//



template<typename IntegerType> const char * PolyphaseMergeSort<IntegerType>::extension  = ".bin";
template<typename IntegerType> size_t       PolyphaseMergeSort<IntegerType>::iterations = 1;
template<typename IntegerType> const size_t PolyphaseMergeSort<IntegerType>::numSize    = sizeof(IntegerType);


template<typename IntegerType>
void PolyphaseMergeSort<IntegerType>::SortFile(const char * SourceFile, const size_t AmountOfFiles, const size_t AmountOfElms,
                                  const size_t AmountOfElmsInChunk, size_t AmountOfThread, Sort version) {
    iterations = 1;

    const size_t num_of_chunks = AmountOfElms / AmountOfElmsInChunk
                              + (AmountOfElms % AmountOfElmsInChunk ? 1 : 0);

    size_t * AmountOfChunksInFile = CreateSplitList(AmountOfFiles, num_of_chunks);
    FILE ** SetOfFiles            = CreatePtrFilesByAmount(AmountOfFiles);

    FilesInit(SourceFile, AmountOfFiles, SetOfFiles,
              AmountOfChunksInFile, AmountOfElmsInChunk, AmountOfElms,
              AmountOfThread, version);

    size_t indexEmptyFile = (iterations - 1) % AmountOfFiles;
    size_t indexMinFile   = (indexEmptyFile - 1) % AmountOfFiles;
    size_t minAmountOfChunksInFile;
    size_t temp;

    if (indexMinFile == -1) {
        indexMinFile = AmountOfFiles - 1;
    }

    BinaryHeap<size_t, FILE*> Heap(AmountOfFiles);

    for (int i = 0; i < AmountOfFiles; i++) {
        fclose(SetOfFiles[i]);
        SetOfFiles[i] = fopen(GetFilenameByNumber(i), "rb");
    }

    for (int c = 0; c < iterations; c++) {
        //reopen file in order to rewrite file
        fclose(SetOfFiles[indexEmptyFile]);
        SetOfFiles[indexEmptyFile] = fopen(GetFilenameByNumber(indexEmptyFile), "wb");

        minAmountOfChunksInFile = AmountOfChunksInFile[indexMinFile];

        for (int chnkId = 0; chnkId < minAmountOfChunksInFile; ++chnkId) {
            for (int i = 0; i < AmountOfFiles; ++i) {
                if(fread(&temp, numSize, 1, SetOfFiles[i])) {
                    Heap.Insert({ temp, SetOfFiles[i] });
                }
            }

            while (!Heap.isEmpty()) {
                auto & x = Heap.GetMinimum();

                fwrite(&x.first, numSize, 1, SetOfFiles[indexEmptyFile]);

//                check for end of currChunk
                if(!fread(&temp, numSize, 1, x.second)
                   || x.first > temp) {


                    if(x.first > temp) {
                        fseek(x.second, -numSize, SEEK_CUR);
                    }

                    Heap.ExtractMinimum();
                } else {
                    Heap.IncreaseKey(0, temp);
                }
            }
        }

        for (int i = 0; i < AmountOfFiles; i++) {
            if (AmountOfChunksInFile[i]) {
                AmountOfChunksInFile[i] -= minAmountOfChunksInFile;
            }
        }
        AmountOfChunksInFile[indexEmptyFile] = minAmountOfChunksInFile;

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

template<typename IntegerType>
bool PolyphaseMergeSort<IntegerType>::isSorted(const char * file) {
    FILE * ptrFile = fopen(file, "rb");

    IntegerType parentElm;
    fread(&parentElm, numSize, 1, ptrFile);

    IntegerType currElm;
    while (fread(&currElm, numSize, 1, ptrFile)) {
        if (parentElm > currElm) {
            return false;
        }
        parentElm = currElm;
    }
    return true;
}


//private part
template<typename IntegerType>
void PolyphaseMergeSort<IntegerType>::FilesInit(const char * SourceFile, size_t AmountOfChunks, FILE ** SetOfFiles,
                                   const size_t * AmountOfChunksInFile, const size_t AmountOfElmsInChunk,
                                   const size_t AmountOfElms, size_t AmountOfThread, Sort version) {


    auto * Buffer = new IntegerType [AmountOfElmsInChunk];
    size_t      MinElmOfCurrChunk;
    IntegerType MaxElmOfPrevChunk  = std::numeric_limits<IntegerType>::max();
    size_t      i                  = 0;
    size_t      AmountOfFullChunks = AmountOfElms / AmountOfElmsInChunk;

    FILE * source = fopen(SourceFile, "rb");




    while (i < AmountOfChunks) {

        int j = 0;
        while (j++ < AmountOfChunksInFile[i]) {
            if (AmountOfFullChunks-- < 1) {
                goto l_out;
            }

            fread(Buffer, numSize, AmountOfElmsInChunk, source);

            if (version == Sort::Sort1) {
                MergeSort<int>::Sort1(Buffer, AmountOfElmsInChunk, AmountOfThread);
            } else if (version == Sort::Sort2) {
                MergeSort<int>::Sort2(Buffer, AmountOfElmsInChunk, AmountOfThread);
            }

            MinElmOfCurrChunk = Buffer[0];

            if (MaxElmOfPrevChunk != std::numeric_limits<IntegerType>::max() && MaxElmOfPrevChunk < MinElmOfCurrChunk) {
                Buffer[0] = MaxElmOfPrevChunk;

                fseek(SetOfFiles[i], -numSize, SEEK_CUR);
                fwrite(&MinElmOfCurrChunk, numSize, 1, SetOfFiles[i]);
            }
            fwrite(Buffer, numSize, AmountOfElmsInChunk, SetOfFiles[i]);

            MaxElmOfPrevChunk = Buffer[AmountOfElmsInChunk - 1];
        }
        MaxElmOfPrevChunk = std::numeric_limits<IntegerType>::max();
        i++;
    }

    l_out:

    int residualElms = AmountOfElms % AmountOfElmsInChunk;
    if (residualElms > 0) {
        int temper;
        int j = 0;


        while (fread(&temper,numSize,1,source)) {
            Buffer[j++] = temper;
        }

        if (version == Sort::Sort1) {
            MergeSort<int>::Sort1(Buffer, residualElms, AmountOfThread);
        } else if (version == Sort::Sort2) {
            MergeSort<int>::Sort2(Buffer, residualElms, AmountOfThread);
        }


        MinElmOfCurrChunk = Buffer[0];

        if (MaxElmOfPrevChunk != std::numeric_limits<IntegerType>::max() && MaxElmOfPrevChunk < MinElmOfCurrChunk) {
            Buffer[0] = MaxElmOfPrevChunk;
            fseek(SetOfFiles[i], -numSize, SEEK_CUR);
            fwrite(&MinElmOfCurrChunk, numSize, 1, SetOfFiles[i]);
        }

        fwrite(Buffer, numSize, residualElms, SetOfFiles[i]);
    }

    delete [] Buffer;
}

template<typename IntegerType>
size_t * PolyphaseMergeSort<IntegerType>::CreateSplitList (const size_t AmountOfFiles, const size_t AmountOfChunks) {
    auto * arr                = new size_t[AmountOfFiles];
    size_t currAmountOfChunks = AmountOfFiles - 1;
    size_t curr_index_max     = 1;
    size_t max_n              = 1;

    arr[0] = 0;
    for (size_t i = 1; i < AmountOfFiles; i++) {
        arr[i] = 1;
    }


    //init
    while (currAmountOfChunks < AmountOfChunks) {
        currAmountOfChunks += (AmountOfFiles - 2) * max_n;

        for (size_t i = 0; i < AmountOfFiles; i++) {
            arr[i] += max_n;
        }
        arr[curr_index_max] = 0;
        curr_index_max      = (curr_index_max + 1) % AmountOfFiles;
        max_n               = arr[curr_index_max];
        iterations++;
    }
    return arr;
}

template<typename IntegerType>
FILE ** PolyphaseMergeSort<IntegerType>::CreatePtrFilesByAmount (const size_t n) {
    FILE ** set_of_filenames = new FILE* [n];


    for (int i = 0; i < n; ++i) {
        set_of_filenames[i] = fopen(GetFilenameByNumber(i), "wb");
    }

    return set_of_filenames;
}

template<typename IntegerType>
const char * PolyphaseMergeSort<IntegerType>::GetFilenameByNumber (size_t number) {
    if (number == 0) {
        return "0.bin";
    }

    size_t length = size_t(log10(number) + 1);


    //length + ".bin" + \'0'
    const size_t newLength = length + 5;
    auto * filename        = new char[newLength];

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

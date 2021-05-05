//
// Created by Bohdan on 5/5/2021.
//

#include "PolyphaseMergeSort.hpp"

const char * PolyphaseMergeSort::extension  = ".bin";
int          PolyphaseMergeSort::iterations = 1;

void PolyphaseMergeSort::SortFile(const char * SourceFile, const int num_of_files, const int num_of_elements,
                                  const int num_elm_in_chunk) {

    iterations = 1;

    const int num_of_chunks = num_of_elements / num_elm_in_chunk + (num_of_elements % num_elm_in_chunk ? 1 : 0);
    int * split_arr         = CreateSplitList(num_of_files,num_of_chunks);
    FILE ** set_files       = CreatePtrFilesByAmount(num_of_files);

    FilesInit(SourceFile, num_of_chunks, set_files, split_arr, num_elm_in_chunk, num_of_elements);

    int indexEmptyFile = (iterations - 1) % num_of_files;
    int indexMinFile   = (indexEmptyFile - 1) % num_of_files;
    int min_val;
    int temp;

    if (indexMinFile == -1) {
        indexMinFile = num_of_files - 1;
    }

    Heap heap = Heap(num_of_files);
    for (int i = 0; i < num_of_files; i++) {
        fclose(set_files[i]);
        set_files[i] = fopen(GetFilenameByNumber(i), "rb");
    }

    for (int c = 0; c < iterations; c++) {
        fclose(set_files[indexEmptyFile]);
        set_files[indexEmptyFile] = fopen(GetFilenameByNumber(indexEmptyFile), "wb");
        min_val = split_arr[indexMinFile];

        for (int a = 0; a < min_val; a++) {
            for (int i = 0; i < num_of_files; i++) {
                if(fread(&temp, n_size, 1, set_files[i]))
                    heap.Insert(temp, set_files[i], min_val);
                else
                    heap.Insert(INT32_MAX, set_files[i], 0);
            }
            while (!heap.isEmpty()) {
                temp = heap.GetMinimum();
                fwrite(&temp, n_size, 1, set_files[indexEmptyFile]);
            }
            heap.SetSizeToZero();
        }

        for (int i = 0; i < num_of_files; i++) {
            if (split_arr[i]) {
                split_arr[i] -= min_val;
            }
        }
        split_arr[indexEmptyFile] = min_val;

        fclose(set_files[indexEmptyFile]);
        set_files[indexEmptyFile] = fopen(GetFilenameByNumber(indexEmptyFile), "rb");


        indexEmptyFile = indexMinFile;
        indexMinFile   = (indexEmptyFile - 1) % num_of_files;

        if (indexMinFile == -1) {
            indexMinFile = num_of_files - 1;
        }
    }

    delete [] split_arr;
    delete [] set_files;
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
void PolyphaseMergeSort::FilesInit(const char * SourceFile, int size_SetOfFiles, FILE ** SetOfFiles,
                                    const int * f_split, const int num_elm_in_chunk, const int num_of_elements) {

    int temp[num_elm_in_chunk];
    int prev = INT32_MAX;
    int curr;
    int i = 0, amount_of_full_chunks = num_of_elements / num_elm_in_chunk;

    FILE * source = fopen(SourceFile, "rb");


    while (i < size_SetOfFiles) {
        int j = 0;
        while (j++ < f_split[i]) {
            if (amount_of_full_chunks-- < 1) {
                goto l_out;
            }

            fread(&temp, n_size, num_elm_in_chunk, source);
            std::sort(temp, temp + num_elm_in_chunk);

            curr = temp[0];

            if (prev != INT32_MAX && prev < curr) {
                temp[0] = prev;
                fseek(SetOfFiles[i], -n_size, SEEK_CUR);
                fwrite(&curr, n_size, 1, SetOfFiles[i]);
            }
            fwrite(&temp, n_size, num_elm_in_chunk, SetOfFiles[i]);
            prev = temp[num_elm_in_chunk - 1];
        }
        prev = INT32_MAX;
        i++;
    }

    l_out:

    int s = num_of_elements % num_elm_in_chunk;
    if (s > 0) {
        int temper;
        int j = 0;
        int temp1[s];


        while (fread(&temper,n_size,1,source)) {
            temp1[j++] = temper;
        }


        std::sort(temp1, temp1 + s);

        curr = temp1[0];


        if (prev != INT32_MAX && prev < curr) {
            temp1[0] = prev;
            fseek(SetOfFiles[i], -n_size, SEEK_CUR);
            fwrite(&curr, n_size, 1, SetOfFiles[i]);
        }

        fwrite(&temp1, n_size, s, SetOfFiles[i]);
    }
}

int * PolyphaseMergeSort::CreateSplitList (const int num_of_files, const int num_of_chunks) {
    int * arr          = new int[num_of_files];
    int sum            = num_of_files - 1;
    int curr_index_max = 1;
    int max_n          = 1;

    for (int i = 1; i < num_of_files; i++) {
        arr[i] = 1;
    }
    arr[0] = 0;

    //init
    while (sum < num_of_chunks) {
        sum += (num_of_files - 2) * max_n;

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

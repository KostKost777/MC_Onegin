#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <sys/stat.h>
#include <errno.h>

#include "sort_functions.h"
#include "read_poem_from_file_functions.h"
#include "print_poem_in_file_functions.h"
#include "text_functions.h"
#include "clean_memory_functions.h"

int main(){
    const char* input_filename = "poem_orig.txt";
    const char* out_file_name = "new_poem_v2.txt";

    FILE* out_file = fopen(out_file_name, "w");

    struct Struct_Poem Poem_Onegin = {};

    if (ReadPoemStructFromFile(&Poem_Onegin, input_filename)  == -1)
        return 1;

    fprintf(out_file, "\n\n\n****************"
                      "Sorting by beginning of line"
                      "*******************\n\n\n");

    BubbleSort(&Poem_Onegin, &CompareByFirstChars);

    if (PrintPoemInFile(&Poem_Onegin, out_file, out_file_name)  == -1)
        return 1;

    fprintf(out_file, "\n\n\n****************"
                      "Sorting by end of line"
                      "*******************\n\n\n");

    qsort(Poem_Onegin.poem_ptr_array,
          Poem_Onegin.number_of_lines,
          sizeof(struct Struct_Line),
          &QsortLineEndsComparator);

    if (PrintPoemInFile(&Poem_Onegin, out_file, out_file_name)  == -1)
        return 1;

    fprintf(out_file, "\n\n\n****************"
                      "Original Poem"
                      "*******************\n\n\n");

    fprintf(out_file, "%s", Poem_Onegin.buffer);

    FreeDataPoem(&Poem_Onegin);
    printf("\nDone\n");
    fclose(out_file);
}

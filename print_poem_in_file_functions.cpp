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
#include "clean_memory_functions.h"
#include "text_functions.h"

int PrintPoemInFile(struct Struct_Poem* Poem,
                    FILE* out_file,
                    const char* out_file_name)
{
    assert(Poem != NULL);
    assert(Poem->buffer != NULL);
    assert(Poem->poem_ptr_array != NULL);

    if (out_file == NULL) {
        fprintf(stderr, "Error opening file |%s|", out_file_name);
        perror("");
        return -1;
    }

    //printf("%d", Poem->number_of_lines);
    for (size_t i = 0; i < Poem->number_of_lines; ++i) {
        fprintf(out_file, "%.*s",
                Poem->poem_ptr_array[i].str_len + 1,
                Poem->poem_ptr_array[i].line_ptr);
    }
    return 0;
}

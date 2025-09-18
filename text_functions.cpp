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

size_t CountSymbol(struct Struct_Poem* Poem, char sym)
{
    assert(Poem != NULL);
    assert(Poem->buffer != NULL);

    int counter_enter = 0;
    for (int i = 0; i < Poem->size_of_file; ++i) {
       // printf("[%d] - %c\n", (Poem->buffer)[i], (Poem->buffer)[i]);
        if ((Poem->buffer)[i] == sym)
            counter_enter++;
    }
    return counter_enter;
}

size_t GoForward(struct Struct_Line str)
{
    assert(str.line_ptr != NULL);

    size_t index = 0;
    while (!isalpha(str.line_ptr[index]))
            index++;

    return index;
}

void SwapStrings(struct Struct_Line* str1,
                 struct Struct_Line* str2)
{
    assert(str1->line_ptr != NULL);
    assert(str2->line_ptr != NULL);

    char* twin_ptr = str1->line_ptr;
    str1->line_ptr = str2->line_ptr;
    str2->line_ptr = twin_ptr;

    int twin_len = str1->str_len;
    str1->str_len = str2->str_len;
    str2->str_len = twin_len;
}

size_t GoBackwards(struct Struct_Line str)
{
    assert(str.line_ptr != NULL);

    size_t index = str.str_len - 1;
    while (!isalpha(str.line_ptr[index]))
            index--;

    return index;
}

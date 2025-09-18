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

void BubbleSort(struct Struct_Poem* Poem,
                Compare_Func_t Comparator)
{
    assert(Poem != NULL);
    assert(Poem->poem_ptr_array != NULL);

    for (size_t sorted_el = 0; sorted_el < Poem->number_of_lines; ++sorted_el) {
        int count_swaps = 0;
        for (size_t i = 0; i < Poem->number_of_lines - sorted_el - 1; ++i) {
            if (Comparator(Poem->poem_ptr_array[i],
                           Poem->poem_ptr_array[i + 1]) > 0) {
                count_swaps++;
                SwapStrings(&Poem->poem_ptr_array[i],
                            &Poem->poem_ptr_array[i + 1]);
            }
        }
        if (count_swaps == 0)
            break;
    }
}

int CompareByFirstChars(struct Struct_Line str1,  // 1 == 2 -> 0
                        struct Struct_Line str2)  // 1 > 2 -> 1
{                                                 // 1 < 2 -> -1
    assert(str1.line_ptr != NULL);
    assert(str2.line_ptr != NULL);

    size_t index_s1 = GoForward(str1);
    size_t index_s2 = GoForward(str2);

    while (str1.line_ptr[index_s1] != '\n' && str2.line_ptr[index_s2] != '\n') {

        if (str1.line_ptr[index_s1] == '\n' &&
            str2.line_ptr[index_s2] != '\n')
            return -1;

        if (str1.line_ptr[index_s1] != '\n' &&
            str2.line_ptr[index_s2] == '\n')
            return 1;

        if (!isalpha(str1.line_ptr[index_s1])) {
            index_s1++;
            continue;
        }

        if (!isalpha(str2.line_ptr[index_s2])) {
            index_s2++;
            continue;
        }

        if (tolower(str1.line_ptr[index_s1]) - 'a' <
            tolower(str2.line_ptr[index_s2]) - 'a')
            return -1;                                      // english 'a'

        else if (tolower(str1.line_ptr[index_s1]) - 'a' >
                 tolower(str2.line_ptr[index_s2]) - 'a')
            return 1;                                       // english 'a'

        index_s1++;
        index_s2++;
    }

    return 0;
}

int QsortLineEndsComparator(const void* param1, const void* param2) // 1 == 2 -> 0
{                                                                   // 1 > 2 -> 1
    assert(param1 != NULL);                                         // 1 < 2 -> -1
    assert(param2 != NULL);

    const struct Struct_Line* str1 = (const struct Struct_Line*)param1;
    const struct Struct_Line* str2 = (const struct Struct_Line*)param2;

    assert(str1->line_ptr != NULL);
    assert(str2->line_ptr != NULL);

    size_t index_s1 = GoBackwards(*str1);
    size_t index_s2 = GoBackwards(*str2);

    while (index_s1 != 0 && index_s2 != 0) {

        if (index_s1 == 0 && index_s2 != 0)
            return -1;

        if (index_s1 != 0 && index_s2 == 0)
            return 1;

        if (!isalpha(str1->line_ptr[index_s1])) {
            index_s1--;
            continue;
        }

        if (!isalpha(str2->line_ptr[index_s2])) {
            index_s2--;
            continue;
        }

        if (tolower(str1->line_ptr[index_s1]) - 'a' <
            tolower(str2->line_ptr[index_s2]) - 'a')
            return -1;                                      // english 'a'

        else if (tolower(str1->line_ptr[index_s1]) - 'a' >
                 tolower(str2->line_ptr[index_s2]) - 'a')
            return 1;                                       // english 'a'

        index_s1--;
        index_s2--;
    }

    return 0;
}


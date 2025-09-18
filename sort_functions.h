#ifndef sort_functions
#define sort_functions

///@file

/**
    @brief Ñonvenient notation for a function pointer
**/

typedef int (*Compare_Func_t)(struct Struct_Line str1,
                              struct Struct_Line str2);

/**
    @brief Bubble sort function
    @param Poem this is the struct of the poem
    @param Comparator function pointer
**/

void BubbleSort(struct Struct_Poem* Poem,
                Compare_Func_t Comparator);

/**
    @brief Ñomparison function at the beginning of strings
    @param str1 first line structure
    @param str2 second line structure
**/

int CompareByFirstChars(struct Struct_Line str1,
                        struct Struct_Line str2);

/**
    @brief Comparator for qsort sorting
    @param param1 first parameter of the comparator
    @param param2 second parameter of the comparator
**/

int QsortLineEndsComparator(const void* param1,
                            const void* param2);

#endif

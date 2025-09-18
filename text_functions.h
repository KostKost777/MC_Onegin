#ifndef text_functions
#define text_functions
///@file

/**
    @brief Function to count characters in a string
    @param Poem this is the struct of the poem
    @param sym the symbol you want to count
**/

size_t CountSymbol(struct Struct_Poem* Poem, char sym);

/**
    @brief String swap function
    @param str1 the line you want to change to str2
    @param str1 the line you want to change to str1
**/

void SwapStrings(struct Struct_Line* str1,
                 struct Struct_Line* str2);

/**
    @brief Function to skip non-alphanumeric characters from the beginning
    @param str string structure
**/

size_t GoForward(struct Struct_Line str);

/**
    @brief function to skip non-alphanumeric characters from the end
    @param str string structure
**/

size_t GoBackwards(struct Struct_Line str);

#endif

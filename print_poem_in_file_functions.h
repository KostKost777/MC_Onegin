#ifndef print_poem_in_file_functions
#define print_poem_in_file_functions

///@file

/**
    @brief Function for outputting text to a file
    @param Poem structure
    @param out_file pointer to output file
    @param out_file_name out file name

**/

int PrintPoemInFile(struct Struct_Poem* Poem,
                    FILE* out_file,
                    const char* out_file_name);

#endif

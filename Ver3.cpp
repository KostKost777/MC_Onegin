#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <sys/stat.h>
#include <errno.h>

typedef int (*Compare_Func_t)(struct Struct_Line str1,
                              struct Struct_Line str2);

struct Struct_Line{
    char* line_ptr;
    size_t str_len;
};

struct Struct_Poem{
    char* buffer;
    struct Struct_Line* poem_ptr_array;
    ssize_t size_of_file;
    size_t number_of_lines;
};

void CopyFromBufferInRaggedArray(struct Struct_Poem* Poem);

void ReplaceSymbolInBuffer(struct Struct_Poem* Poem,
                           char sym1,
                           char sym2);

int PrintInFilePoem(struct Struct_Poem* Poem,
                    FILE* out_file,
                    const char* out_file_name);

ssize_t GetSizeOfFile(const char* filename);

size_t CountSymbol(struct Struct_Poem* Poem, char sym);

int ReadPoemStructFromFile(struct Struct_Poem* Poem,
                          const char* input_filename);

void FreeDataPoem(struct Struct_Poem* Poem);

void BubbleSort(struct Struct_Poem* Poem,
                Compare_Func_t Comparator);

int CompareByFirstChars(struct Struct_Line str1,
                         struct Struct_Line str2);

void SwapStrings(struct Struct_Line* str1,
                 struct Struct_Line* str2);

int CompareByLastChars(struct Struct_Line str1,
                        struct Struct_Line str2);

size_t GetLineEndIndex(char* str);

size_t GoForward(struct Struct_Line str);

size_t GoBackwards(struct Struct_Line str);

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

    if (PrintInFilePoem(&Poem_Onegin, out_file, out_file_name)  == -1)
        return 1;

    fprintf(out_file, "\n\n\n****************"
                      "Sorting by end of line"
                      "*******************\n\n\n");

    BubbleSort(&Poem_Onegin, &CompareByLastChars);

    if (PrintInFilePoem(&Poem_Onegin, out_file, out_file_name)  == -1)
        return 1;

    fprintf(out_file, "\n\n\n****************"
                      "Original Poem"
                      "*******************\n\n\n");

    fprintf(out_file, "%s", Poem_Onegin.buffer);

    FreeDataPoem(&Poem_Onegin);
    fclose(out_file);
}

int ReadPoemStructFromFile(struct Struct_Poem* Poem,
                          const char* input_filename)
{
    assert(Poem != NULL);
    assert(input_filename != NULL);

    int file_descriptor = open(input_filename, O_RDONLY);

    if (file_descriptor == -1) {
        fprintf(stderr, "Error opening file |%s|", input_filename);
        close(file_descriptor);
        perror("");
        return -1;
    }

    Poem->size_of_file = GetSizeOfFile(input_filename);

    if (Poem->size_of_file == -1)
        return -1;

    Poem->buffer = (char* )calloc(Poem->size_of_file + 1, sizeof(char));
    assert(Poem->buffer != NULL);
    //printf("%d\n", Poem->size_of_file);
    Poem->size_of_file = read(file_descriptor, Poem->buffer, Poem->size_of_file);

    if (Poem->size_of_file == -1) {
        fprintf(stderr, "Error reading file |%s|", input_filename);
        close(file_descriptor);
        perror("");
        return -1;
    }
//    printf("%d\n", Poem->size_of_file);

    Poem->number_of_lines = CountSymbol(Poem, '\n');

    Poem->poem_ptr_array = (struct Struct_Line*)calloc(Poem->number_of_lines,
                                                       sizeof(struct Struct_Line));
    assert(Poem->poem_ptr_array != NULL);

    //ReplaceSymbolInBuffer(Poem, '\n', '\0');
    CopyFromBufferInRaggedArray(Poem);
    /*
    for (int i = 0;i < Poem->number_of_lines; ++i) {
        printf("%s\n", Poem->poem_ptr_array[i].line_ptr);
    }
    */

    close(file_descriptor);
    return 0;
}

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

void CopyFromBufferInRaggedArray(struct Struct_Poem* Poem)
{
    assert(Poem != NULL);
    assert(Poem->buffer != NULL);
    assert(Poem->poem_ptr_array != NULL);

    char* line_begin_ptr = Poem->buffer;
    char* now_ptr = NULL;
    size_t buffer_index = 0;
    size_t poem_index = 0;
    for (; poem_index < Poem->number_of_lines; ++buffer_index) {
        //printf("%d\n", (Poem->buffer)[buffer_index]);
        if ((Poem->buffer)[buffer_index] == '\n') {
            now_ptr = &(Poem->buffer)[buffer_index];
            (Poem->poem_ptr_array[poem_index].line_ptr) = line_begin_ptr;

            (Poem->poem_ptr_array[poem_index].str_len) =
            now_ptr - line_begin_ptr;

            //printf("%s\n", line_begin_ptr);
            poem_index++;
            line_begin_ptr = now_ptr + 1;
        }
    }
    *line_begin_ptr = '\0';
}

void ReplaceSymbolInBuffer(struct Struct_Poem* Poem,
                           char sym1,
                           char sym2)
{
    assert(Poem != NULL);
    assert(Poem->buffer != NULL);

    for (int i = 0; i < Poem->size_of_file; ++i) {
        if ((Poem->buffer)[i] == sym1)
            (Poem->buffer)[i] = sym2;
    }
}

ssize_t GetSizeOfFile(const char* filename)
{
    assert(filename != NULL);
    struct stat file_info = {};

    if (stat(filename, &file_info) == -1) {
        fprintf(stderr, "Ошибка получения информации из файла |%s|", filename);
        perror("");
        return -1;
    }
    return file_info.st_size;
}

int PrintInFilePoem(struct Struct_Poem* Poem,
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

void FreeDataPoem(struct Struct_Poem* Poem)
{
    assert(Poem);
    assert(Poem->poem_ptr_array);
    assert(Poem->buffer);

    free(Poem->poem_ptr_array);
    Poem->poem_ptr_array = NULL;

    free(Poem->buffer);
    Poem->buffer = NULL;
}

void BubbleSort(struct Struct_Poem* Poem,
                Compare_Func_t Comparator)
{
    assert(Poem != NULL);
    assert(Poem->poem_ptr_array != NULL);

    for (size_t sorted_el = 0; sorted_el < Poem->number_of_lines; ++sorted_el) {
        for (size_t i = 0; i < Poem->number_of_lines - sorted_el - 1; ++i) {
            if (Comparator(Poem->poem_ptr_array[i],
                              Poem->poem_ptr_array[i + 1]) > 0) {

                SwapStrings(&Poem->poem_ptr_array[i],
                            &Poem->poem_ptr_array[i + 1]);
            }
        }
    }
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

size_t GoForward(struct Struct_Line str)
{
    assert(str.line_ptr != NULL);

    size_t index = 0;
    while (!isalpha(str.line_ptr[index]))
            index++;
    return index;
}

size_t GoBackwards(struct Struct_Line str)
{
    assert(str.line_ptr != NULL);

    size_t index = str.str_len - 1;
    while (!isalpha(str.line_ptr[index]))
            index--;
}
                                                    // 1 == 2 -> 0
int CompareByFirstChars(struct Struct_Line str1,
                         struct Struct_Line str2)  // 1 > 2 -> 1
{                                                   // 1 < 2 -> -1
    assert(str1.line_ptr != NULL);
    assert(str2.line_ptr != NULL);

    size_t index_s1 = GoForward(str1);
    size_t index_s2 = GoForward(str2);

    while (str1.line_ptr[index_s1] != '\n' && str2.line_ptr[index_s2] != '\n') {

        if (str1.line_ptr[index_s1] == '\n' && str2.line_ptr[index_s2] != '\n')
            return -1;

        if (str1.line_ptr[index_s1] != '\n' && str2.line_ptr[index_s2] == '\n')
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

size_t GetLineEndIndex(char* str){
    assert(str != NULL);

    size_t end_index = 0;
    for (; str[end_index] != '\0'; end_index++)
        continue;
    return --end_index;
}
                                                 // 1 == 2 -> 0
int CompareByLastChars(struct Struct_Line str1,
                       struct Struct_Line str2) // 1 > 2 -> 1
{                                                // 1 < 2 -> -1
    assert(str1.line_ptr != NULL);
    assert(str2.line_ptr != NULL);

    size_t index_s1 = GoBackwards(str1);
    size_t index_s2 = GoBackwards(str2);

    while (index_s1 != 0 && index_s2 != 0) {

        if (index_s1 == 0 && index_s2 != 0)
            return -1;

        if (index_s1 != 0 && index_s2 == 0)
            return 1;

        if (!isalpha(str1.line_ptr[index_s1])) {
            index_s1--;
            continue;
        }

        if (!isalpha(str2.line_ptr[index_s2])) {
            index_s2--;
            continue;
        }

        if (tolower(str1.line_ptr[index_s1]) - 'a' <
            tolower(str2.line_ptr[index_s2]) - 'a')
            return -1;                                      // english 'a'

        else if (tolower(str1.line_ptr[index_s1]) - 'a' >
                 tolower(str2.line_ptr[index_s2]) - 'a')
            return 1;                                       // english 'a'

        index_s1--;
        index_s2--;
    }

    return 0;
}

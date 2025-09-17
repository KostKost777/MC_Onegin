#include <TXLib.h>
#include <sys/stat.h>
#include <errno.h>

const int BEGINOFLINE = 0;
const int ENDOFLINE = 1;

struct Struct_Poem{
    char* buffer;
    char** poem_ptr_array;
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

int SetPoemStructFromFile(struct Struct_Poem* Poem,
                          const char* input_filename);

void FreeDataPoem(struct Struct_Poem* Poem);

void BubbleSort(struct Struct_Poem* Poem,
                int (*comparator)(char* str1, char* str2));

int MyStrcmpByFirstChars(char* str1, char* str2);

void SwapStrings(char** str1, char** str2);

int MyStrcmpByLastChars(char* str1, char* str2);

size_t LineEndIndex(char* str);

size_t GetIndexFirstAlpha(char* str, const int start_point);

int main(){
    const char* input_filename = "poem_orig.txt";
    const char* out_file_name = "new_poem_v2.txt";
    FILE* out_file = fopen(out_file_name, "w");

    struct Struct_Poem Poem_Onegin = {};

    if (SetPoemStructFromFile(&Poem_Onegin, input_filename)  == -1)
        return 1;

    fprintf(out_file, "\n\n\n****************"
                      "Sorting by beginning of line"
                      "*******************\n\n\n");

    BubbleSort(&Poem_Onegin, &MyStrcmpByFirstChars);

    if (PrintInFilePoem(&Poem_Onegin, out_file, out_file_name)  == -1)
        return 1;

    fprintf(out_file, "\n\n\n****************"
                      "Sorting by end of line"
                      "*******************\n\n\n");

    BubbleSort(&Poem_Onegin, &MyStrcmpByLastChars);

    if (PrintInFilePoem(&Poem_Onegin, out_file, out_file_name)  == -1)
        return 1;

    FreeDataPoem(&Poem_Onegin);
    fclose(out_file);
}

int SetPoemStructFromFile(struct Struct_Poem* Poem,
                          const char* input_filename)
{
    int file_descriptor = open(input_filename, O_RDONLY);

    if (file_descriptor == -1) {
        fprintf(stderr, "Ошибка открытия файла |%s|", input_filename);
        close(file_descriptor);
        perror("");
        return -1;
    }

    Poem->size_of_file = GetSizeOfFile(input_filename);

    if (Poem->size_of_file == -1)
        return -1;

    Poem->buffer = (char* )calloc(Poem->size_of_file + 1, sizeof(char));
    assert(Poem->buffer != NULL);

    Poem->size_of_file = read(file_descriptor, Poem->buffer, Poem->size_of_file);

    if(Poem->size_of_file == -1){
        fprintf(stderr, "Ошибка чтения файла |%s|", input_filename);
        close(file_descriptor);
        perror("");
        return -1;
    }

    char* ptr_check = (char* )realloc(Poem->buffer, Poem->size_of_file + 1);

    if (ptr_check == NULL) {
        free(Poem->buffer);
        Poem->buffer = NULL;
        fprintf(stderr, "Ошибка выделения памяти");
        perror("");
        close(file_descriptor);
        return -1;
    }

    Poem->buffer = ptr_check;

    Poem->number_of_lines = CountSymbol(Poem, '\n');

    Poem->poem_ptr_array = (char** )calloc(Poem->number_of_lines, sizeof(char*));
    assert(Poem->poem_ptr_array != NULL);


    ReplaceSymbolInBuffer(Poem, '\n', '\0');
    CopyFromBufferInRaggedArray(Poem);

    close(file_descriptor);
    return 0;
}

size_t CountSymbol(struct Struct_Poem* Poem, char sym)
{
    assert(Poem != NULL);
    assert(Poem->buffer != NULL);

    int counter_enter = 0;
    for (int i = 0; i < Poem->size_of_file; ++i){
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

    char* now_ptr = Poem->buffer;
    size_t buffer_index = 0;
    size_t poem_index = 0;
    for (; poem_index < Poem->number_of_lines; ++buffer_index){

        if ((Poem->buffer)[buffer_index] == '\0'){
            (Poem->poem_ptr_array)[poem_index] = now_ptr;
            poem_index++;
            now_ptr = &(Poem->buffer)[buffer_index] + 1;
        }
    }
}

void ReplaceSymbolInBuffer(struct Struct_Poem* Poem,
                           char sym1,
                           char sym2)
{
    assert(Poem != NULL);
    assert(Poem->buffer != NULL);

    for (int i = 0; i < Poem->size_of_file; ++i){
        if ((Poem->buffer)[i] == sym1)
            (Poem->buffer)[i] = sym2;
    }
}

ssize_t GetSizeOfFile(const char* filename)
{
    struct stat file_info = {};

    if (stat(filename, &file_info) == -1){
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

    if (out_file == NULL){
        fprintf(stderr, "Ошибка откытия файла |%s|", out_file_name);
        perror("");
        return -1;
    }

    //printf("%d", Poem->number_of_lines);
    for (size_t i = 0; i < Poem->number_of_lines; ++i)
        fprintf(out_file, "%s\n", (Poem->poem_ptr_array)[i]);

    return 0;
}

void FreeDataPoem(struct Struct_Poem* Poem)
{
    free(Poem->poem_ptr_array);
    Poem->poem_ptr_array = NULL;

    free(Poem->buffer);
    Poem->buffer = NULL;
}

void BubbleSort(struct Struct_Poem* Poem,
                int (*comparator)(char* str1, char* str2))
{
    assert(Poem != NULL);
    assert(Poem->poem_ptr_array != NULL);

    for (size_t sorted_el = 0; sorted_el < Poem->number_of_lines; ++sorted_el) {
        for (size_t i = 0; i < Poem->number_of_lines - sorted_el - 1; ++i) {
            if ((*comparator)(Poem->poem_ptr_array[i],
                                    Poem->poem_ptr_array[i + 1]) > 0) {
                SwapStrings(&(Poem->poem_ptr_array[i]),
                            &(Poem->poem_ptr_array[i + 1]));
            }
        }
    }
}

void SwapStrings(char** str1, char** str2)
{
    assert(*str1 != NULL);
    assert(*str2 != NULL);

    char* twin_ptr = *str1;
    *str1 = *str2;
    *str2 = twin_ptr;
}

size_t GetIndexFirstAlpha(char* str, const int start_point)
{
    assert(str != NULL);

    size_t index = 0;
    switch(start_point) {
        case BEGINOFLINE:
            while (!isalpha(str[index]))
                index++;
            break;

        case ENDOFLINE:
            index = LineEndIndex(str);

            while (!isalpha(str[index]))
                index--;
            break;

        default:
            break;
    }
    return index;
}
                                                // 1 == 2 -> 0
int MyStrcmpByFirstChars(char* str1, char* str2) // 1 > 2 -> 1
{                                               // 1 < 2 -> -1
    assert(str1 != NULL);
    assert(str2 != NULL);

    size_t index_s1 = GetIndexFirstAlpha(str1, BEGINOFLINE);
    size_t index_s2 = GetIndexFirstAlpha(str2, BEGINOFLINE);

    while (str1[index_s1] != '\0' && str2[index_s2] != '\0') {

        if (str1[index_s1] == '\0' && str2[index_s2] != '\0')
            return -1;

        if (str1[index_s1] != '\0' && str2[index_s2] == '\0')
            return 1;

        if (!isalpha(str1[index_s1])){
            index_s1++;
            continue;
        }

        if (!isalpha(str2[index_s2])){
            index_s2++;
            continue;
        }

        if (tolower(str1[index_s1]) - 'a' < tolower(str2[index_s2]) - 'a')
            return -1;                    // english 'a'

        else if (tolower(str1[index_s1]) - 'a' > tolower(str2[index_s2]) - 'a')
            return 1;                     // english 'a'

        index_s1++;
        index_s2++;
    }

    return 0;
}

size_t LineEndIndex(char* str){
    assert(str != NULL);

    size_t end_index = 0;
    for (; str[end_index] != '\0'; end_index++)
        continue;
    return --end_index;
}
                                                // 1 == 2 -> 0
int MyStrcmpByLastChars(char* str1, char* str2) // 1 > 2 -> 1
{                                               // 1 < 2 -> -1
    assert(str1 != NULL);
    assert(str2 != NULL);

    size_t index_s1 = GetIndexFirstAlpha(str1, ENDOFLINE);
    size_t index_s2 = GetIndexFirstAlpha(str2, ENDOFLINE);

    while (index_s1 != 0 && index_s2 != 0) {

        if (index_s1 == 0 && index_s2 != 0)
            return -1;

        if (index_s1 != 0 && index_s2 == 0)
            return 1;

        if (!isalpha(str1[index_s1])){
            index_s1--;
            continue;
        }

        if (!isalpha(str2[index_s2])){
            index_s2--;
            continue;
        }

        if (tolower(str1[index_s1]) - 'a' < tolower(str2[index_s2]) - 'a')
            return -1;                    // english 'a'

        else if (tolower(str1[index_s1]) - 'a' > tolower(str2[index_s2]) - 'a')
            return 1;                     // english 'a'

        index_s1--;
        index_s2--;
    }

    return 0;
}

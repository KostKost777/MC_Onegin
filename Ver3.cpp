#include <TXLib.h>
#include <sys/stat.h>
#include <errno.h>

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

int PrintRaggedArray(struct Struct_Poem* Poem, const char* name_of_out_file);

ssize_t GetSizeOfFile(const char* filename);

size_t CountSymbol(struct Struct_Poem* Poem, char sym);

int SetPoemStructFromFile(struct Struct_Poem* Poem,
                          const char* input_filename);

void FreeDataPoem(struct Struct_Poem* Poem);

int main(){
    const char* input_filename = "poem_orig.txt";
    const char* name_of_out_file = "new_poem_v2.txt";

    struct Struct_Poem Poem_Onegin = {};

    if (SetPoemStructFromFile(&Poem_Onegin, input_filename)  == -1)
        return 1;

    if (PrintRaggedArray(&Poem_Onegin, name_of_out_file)  == -1)
        return 1;

    FreeDataPoem(&Poem_Onegin);
}

int SetPoemStructFromFile(struct Struct_Poem* Poem,
                          const char* input_filename)
{
    int file_descriptor = open(input_filename, O_RDONLY);

    if (file_descriptor == -1) {
        fprintf(stderr, "Ошибка открытия файла |%s|", input_filename);
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
        perror("");
        return -1;
    }

    Poem->buffer = (char* )realloc(Poem->buffer, Poem->size_of_file + 1);

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
        printf("[%d] - %c\n", (Poem->buffer)[i], (Poem->buffer)[i]);
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
    int buffer_index = 0;
    int poem_index = 0;
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

int PrintRaggedArray(struct Struct_Poem* Poem, const char* name_of_out_file)
{
    assert(Poem != NULL);
    assert(Poem->buffer != NULL);
    assert(Poem->poem_ptr_array != NULL);

    FILE* out_file = fopen(name_of_out_file, "w");

    if (out_file == NULL){
        fprintf(stderr, "Ошибка откытия файла |%s|", name_of_out_file);
        perror("");
        return -1;
    }

    //printf("%d", Poem->number_of_lines);
    for (int i = 0; i < Poem->number_of_lines; ++i)
        fprintf(out_file, "%s\n", (Poem->poem_ptr_array)[i]);

    fclose(out_file);
}

void FreeDataPoem(struct Struct_Poem* Poem)
{
    free(Poem->poem_ptr_array);
    Poem->poem_ptr_array = NULL;

    free(Poem->buffer);
    Poem->buffer = NULL;
}

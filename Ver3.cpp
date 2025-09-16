#include <TXLib.h>
#include <sys/stat.h>
#include <errno.h>

void CopyFromBufferInRaggedArray(char** poem, char* buffer,
                                 size_t SIZE_OF_FILE,
                                 size_t NUMBER_OF_LINES);

void DeleteEnterFromBuffer(char* buffer, size_t SIZE_OF_FILE);

void PrintRaggedArray(char** poem, size_t NUMBER_OF_LINES, FILE* out_file);

int CountLines(const char* buffer, size_t SIZE_OF_FILE);

int main(){
    int file_descriptor = 0;
    const char* filename = "poem_orig.txt";
    struct stat file_info = {};

    int file_descriptor = open(filename, O_RDONLY);

    if (file_descriptor == -1) {
        fprintf(stderr, "Ошибка открытия файла |%s|", filename);
        perror("");
        return 1;
    }

    if (fstat(file_descriptor, &file_info) == -1){
        fprintf(stderr, "Ошибка получения информации из файла %s", filename);
        perror("");
        return 1;
    }

    const size_t SIZE_OF_FILE = file_info.st_size;
    char* buffer = (char* )calloc(SIZE_OF_FILE + 1, sizeof(char));
    assert(buffer != NULL);

    if(read(file_descriptor, buffer, SIZE_OF_FILE) == -1){
        fprintf(stderr, "Ошибка чтения файла %s", filename);
        perror("");
        return 1;
    }

    const size_t NUMBER_OF_LINES = CountLines(buffer, SIZE_OF_FILE);

    char** poem = (char** )calloc(NUMBER_OF_LINES, sizeof(char*));
    assert(poem != NULL);

    DeleteEnterFromBuffer(buffer, SIZE_OF_FILE);
    CopyFromBufferInRaggedArray(poem, buffer, SIZE_OF_FILE, NUMBER_OF_LINES);

    const char* name_of_out_file = "new_poem_v2.txt";
    FILE* out_file = fopen(name_of_out_file, "w");
    if (out_file == NULL){
        fprintf(stderr, "Ошибка откытия файла %s", name_of_out_file);
        perror("");
        return 1;
    }

    PrintRaggedArray(poem, NUMBER_OF_LINES, out_file);
    free(poem);
    poem = NULL;
    free(buffer);
    buffer = NULL;
    fclose(out_file);
}

int CountLines(const char* buffer, size_t SIZE_OF_FILE){
    assert(buffer != NULL);

    int counter_enter = 0;
    for (int i = 0; i < SIZE_OF_FILE; ++i){
        if (buffer[i] == '\n')
            counter_enter++;
    }
    counter_enter--;
    return counter_enter;
}

void CopyFromBufferInRaggedArray(char** poem, char* buffer,
                                 size_t SIZE_OF_FILE,
                                 size_t NUMBER_OF_LINES)
{
    assert(buffer != NULL);
    assert(poem != NULL);

    char* now_ptr = buffer;
    int buffer_index = 0;
    int poem_index = 0;
    for (; poem_index < NUMBER_OF_LINES; ++buffer_index){

        if (buffer[buffer_index] == '\0'){
            poem[poem_index] = now_ptr;
            poem_index++;
            now_ptr = &buffer[buffer_index] + 1;
        }
    }
}

void DeleteEnterFromBuffer(char* buffer,
                           size_t SIZE_OF_FILE)
{
    assert(buffer != NULL);
    for (int i = 0; i < SIZE_OF_FILE; ++i){
        if (buffer[i] == '\n')
            buffer[i] = '\0';
    }
}

void PrintRaggedArray(char** poem, size_t NUMBER_OF_LINES, FILE* out_file)
{
    assert(poem != NULL);
    assert(out_file != NULL);

    for (int i = 0; i < NUMBER_OF_LINES; ++i){
        fputs(poem[i], out_file);
        fputs("\n", out_file);
    }
}

#include <TXLib.h>

const size_t NUMBER_OF_LINES = 14;
const size_t MAX_LINE_LEN = 35;

void WrongInputRaggedArrayPoem(char** poem, FILE* inp_file);

void PrintRaggedArrayPoem(char const * const * poem, FILE* out_file);

void InputRaggedArrayPoem(char** poem, FILE* inp_file);

void FreeData(char** poem);

int main(){
    char** poem = (char** )calloc(NUMBER_OF_LINES, sizeof(char*));
    FILE* inp_file = fopen("poem_orig.txt", "r");

//    WrongInputRaggedArrayPoem(poem, inp_file);

    InputRaggedArrayPoem(poem, inp_file);
    PrintRaggedArrayPoem(poem, stdout);
    FreeData(poem);
    fclose(inp_file);


}
/*
 void WrongInputRaggedArrayPoem(char** poem, FILE* inp_file)
  {
      char buffer[MAX_LINE_LEN] = {};
      char* status = "";
      for (int i = 0;; ++i) {
          status = fgets(buffer, MAX_LINE_LEN, inp_file);
          if (status == NULL)
              break;
          poem[i] = buffer;
      }
      for (int i = 0; i < NUMBER_OF_LINES; ++i)
          printf("[%d] - %s", i, poem[i]);

  }
 */

void InputRaggedArrayPoem(char** poem, FILE* inp_file) {

    assert(poem != NULL);
    assert(inp_file != NULL);

    char buffer[MAX_LINE_LEN] = {};
    for (int i = 0;; ++i) {
        char* status = fgets(buffer, MAX_LINE_LEN, inp_file);
        if (status == NULL)
            break;
        poem[i] = strdup(buffer);
    }
}

void PrintRaggedArrayPoem(char const * const * poem, FILE* out_file)
{
    assert(poem != NULL);
    assert(out_file != NULL);

    for (int i = 0; i < NUMBER_OF_LINES; ++i)
        fputs(poem[i], out_file);
}

void FreeData(char** poem)
{
    for (int i = 0; i < NUMBER_OF_LINES; ++i) {
        free(poem[i]);
        poem[i] = NULL;
    }
    free(poem);
    poem = NULL;
}


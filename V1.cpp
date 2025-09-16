#include <TXLib.h>

const size_t MAX_LINE_LEN = 35;
const size_t COLUMNS = 20;


void InputInSquareArrayPoem(char poem[][MAX_LINE_LEN], FILE* file);

void PrintSquareArrayPoem(char poem[][MAX_LINE_LEN]);

int main()
{
    char poem[COLUMNS][MAX_LINE_LEN] = {};
    FILE* file =fopen("poem_orig.txt", "r");

    InputInSquareArrayPoem(poem);
    PrintSquareArrayPoem(poem);
    fclose(file);
}

void InputInSquareArrayPoem(char poem[][MAX_LINE_LEN], *FILE file)
{
    char* status = "";
    for (int i = 0;i < COLUMNS && status != NULL; ++i)
        status = fgets(poem[i], MAX_LINE_LEN, file);
}

void PrintSquareArrayPoem(char poem[][MAX_LINE_LEN])
{
    for (int i = 0; i < columns; ++i)
        printf("%s", poem[i]);
}




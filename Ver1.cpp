#include <TXLib.h>

const size_t MAX_LINE_LEN = 35;
const size_t NUMBER_OF_LINES = 14;

void InputInSquareArrayPoem(char poem[][MAX_LINE_LEN], FILE* inp_file);

void PrintSquareArrayPoem(const char poem[][MAX_LINE_LEN], FILE* out_file);

int MyStrcmp(char* str1, char* str2);

void BubbleSort(char poem[][MAX_LINE_LEN]);

void SwapStrings(char* str1, char* str2);

int main()
{
    char poem[NUMBER_OF_LINES][MAX_LINE_LEN] = {};
    FILE* inp_file = fopen("poem_orig.txt", "r");
    FILE* out_file = fopen("new_poem.txt", "w");

    InputInSquareArrayPoem(poem, inp_file);
    BubbleSort(poem);
    PrintSquareArrayPoem(poem, out_file);
    printf("Done");
    fclose(inp_file);
    fclose(out_file);
}

void InputInSquareArrayPoem(char poem[][MAX_LINE_LEN], FILE* inp_file)
{
    assert(poem != NULL);
    assert(inp_file != NULL);

    char* status = "";
    for (int i = 0; status != NULL; ++i)
        status = fgets(poem[i], MAX_LINE_LEN, inp_file);
}

void PrintSquareArrayPoem(const char poem[][MAX_LINE_LEN], FILE* out_file)
{
    assert(poem != NULL);
    assert(out_file != NULL);

    for (int i = 0; i < NUMBER_OF_LINES; ++i)
        fputs(poem[i], out_file);
}

void BubbleSort(char poem[][MAX_LINE_LEN])
{
    assert(poem != NULL);

    for (int sorted_el = 0; sorted_el < NUMBER_OF_LINES; ++sorted_el) {
        for (int i = 0; i < NUMBER_OF_LINES - sorted_el - 1; ++i) {
            if (MyStrcmp(poem[i], poem[i + 1]) > 0) {
                SwapStrings(poem[i], poem[i + 1]);
            }
        }
    }
}

void SwapStrings(char str1[MAX_LINE_LEN], char str2[MAX_LINE_LEN])
{
    assert(str1 != NULL);
    assert(str2 != NULL);

    char buffer[MAX_LINE_LEN] = {};
    strcpy(buffer, str1);
    strcpy(str1, str2);
    strcpy(str2, buffer);
}
                                     // 1 == 2 -> 0
int MyStrcmp(char* str1, char* str2) // 1 > 2 -> 1
{                                    // 1 < 2 -> -1
    assert(str1 != NULL);
    assert(str2 != NULL);

    size_t index_s1 = 0;
    size_t index_s2 = 0;

    while (!isalpha(str1[index_s1]))
            index_s1++;

    while (!isalpha(str1[index_s2]))
            index_s2++;

    while (true) {
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

        if (tolower(str1[index_s1]) - 'à' < tolower(str2[index_s2]) - 'à')
            return -1;

        else if (tolower(str1[index_s1]) - 'à' > tolower(str2[index_s2]) - 'à')
            return 1;

        index_s1++;
        index_s2++;
    }

    return 0;
}

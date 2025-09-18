#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <sys/stat.h>
#include <errno.h>

#include "sort_functions.h"
#include "read_poem_from_file_functions.h"
#include "print_poem_in_file_functions.h"
#include "text_functions.h"
#include "clean_memory_functions.h"

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

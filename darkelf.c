#include <stdlib.h>
#include <stdio.h>
#include "darkelf.h"

void allocElfFile(ElfFile **elf)
{
    *elf = (ElfFile *)malloc(sizeof(ElfFile));
    (*elf)->f = NULL;
    (*elf)->size = 0;
    (*elf)->contents = NULL;
}

void freeElfFile(ElfFile **elf)
{
    if((*elf)->f != NULL)
        fclose((*elf)->f);
    
    if((*elf)->contents != NULL)
        free((*elf)->contents);
    
    free(*elf);
}

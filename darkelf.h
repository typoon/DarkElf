#ifndef DARKELF_H
#define DARKELF_H

#include <elf.h>

//Structs
typedef struct _ElfFile
{
    FILE *f;
    int size;
    unsigned char *contents;
} ElfFile;

// Functions
void allocElfFile(ElfFile **elf);
void freeElfFile(ElfFile **elf);

#include "darkelfRead.h"

#endif

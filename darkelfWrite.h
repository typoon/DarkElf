#ifndef DARKELFWRITE_H
#define DARKELFWRITE_H

#include "darkelf.h"

int addCodeIntoFunction(ElfFile *elf, char *functionName, unsigned char *code, int size);

#endif

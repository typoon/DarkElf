#ifndef DARKELFREAD_H
#define DARKELFREAD_H

#include "darkelf.h"

ElfFile* readElf(char *path);
void getElfHeader(ElfFile *elf, Elf32_Ehdr *header);
int getElfSectionHeaders(ElfFile *elf, Elf32_Shdr **sections);
char* getSectionName(ElfFile *elf, Elf32_Shdr *section);
int getSymbolTableEntries(ElfFile *elf, Elf32_Sym **symTableEntries);
Elf32_Shdr* getSectionHeaderByName(ElfFile *elf, char *name);
char* getSymbolName(ElfFile *elf, Elf32_Sym *symbol);
Elf32_Sym* getSymbolByName(ElfFile *elf, char *name);


#endif

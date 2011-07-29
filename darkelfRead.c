#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "darkelf.h"

ElfFile* readElf(char *path)
{

    ElfFile *elf;
    struct stat buffer;
    const char ELFMAGIC[] = {0x7F, 0x45, 0x4C, 0x46};
    
    elf = (ElfFile *)malloc(sizeof(ElfFile));
    
    elf->f = fopen(path, "r+");
    if(elf->f == NULL)
    {
        printf("File %s could not be opened. \n", path);
        freeElfFile(&elf);
        return NULL;
    }
    
    if(stat(path, &buffer) != 0)
    {
        printf("Cannot determine file stat.\n");
        freeElfFile(&elf);
        return NULL;
    }
    
    elf->size = buffer.st_size;
    elf->contents = (unsigned char *)malloc(sizeof(char) * elf->size);
    if(fread(elf->contents, 1, elf->size, elf->f) != elf->size)
    {
        printf("Cannot read file contents.\n");
        freeElfFile(&elf);
        return NULL;
    }
    
    if(memcmp(elf->contents, ELFMAGIC, 4) != 0)
    {
        printf("Not a valid ELF file.\n");
        freeElfFile(&elf);
        return NULL;
    }
    
    return elf;
}

void getElfHeader(ElfFile *elf, Elf32_Ehdr *header)
{
    memcpy(header, elf->contents, sizeof(Elf32_Ehdr));
}

/*
 * Reads the headers of all sections and returns the number
 * of headers read and the headers
 */ 

int getElfSectionHeaders(ElfFile *elf, Elf32_Shdr **sections)
{
    Elf32_Ehdr header;
    
    getElfHeader(elf, &header);
    
    *sections = (Elf32_Shdr *)malloc(sizeof(Elf32_Shdr)*header.e_shnum);
    memcpy(*sections, elf->contents+header.e_shoff, header.e_shnum * header.e_shentsize);
    
    return header.e_shnum;
}

char* getSectionName(ElfFile *elf, Elf32_Shdr *section)
{
    char *str;
    Elf32_Ehdr header;
    Elf32_Shdr *s;
    
    getElfHeader(elf, &header);
    getElfSectionHeaders(elf, &s);

    str = (char *)&elf->contents[(s[header.e_shstrndx].sh_offset + section->sh_name)];

    free(s);

    return str;
}

char* getSymbolName(ElfFile *elf, Elf32_Sym *symbol)
{
    char *ret;
    Elf32_Shdr *sectionHeaders;
    
    getElfSectionHeaders(elf, &sectionHeaders);
    
    // TODO: Create a function to get the index of the .strtab section
    // in order to properly find the name of the symbols
    ret = (char *)&elf->contents[sectionHeaders[30].sh_offset + symbol->st_name];

    free(sectionHeaders);
    
    return ret;
}

int getSymbolTableEntries(ElfFile *elf, Elf32_Sym **symTableEntries)
{
    int i;
    int x = 0;
    int numEntries = 0;
    int numSections;
    int offset = 0;
    int entrySize = 0;
    Elf32_Shdr *sections;
    
    numSections = getElfSectionHeaders(elf, &sections);
        
    printf("SHT_SYMTAB = %d\n", SHT_SYMTAB);
    for(i = 0; i < numSections; i++)
    {
        if(sections[i].sh_type == SHT_SYMTAB)
        {
            numEntries = sections[i].sh_size / sections[i].sh_entsize;
            offset = sections[i].sh_offset;
            entrySize = sections[i].sh_entsize;
            
#ifdef DEBUG
            printf("sections[%d].sh_type = %d\n", i, sections[i].sh_type);
            printf("Symbol table entry size: %d\n", sections[i].sh_entsize);
            printf("Symbol table size: %d\n", sections[i].sh_size);
#endif
            break;
        }
    }
    
    *symTableEntries = (Elf32_Sym *)malloc(sizeof(Elf32_Sym) * numEntries);
    
    for(i = 0; i < numEntries; i++)
    {
        memcpy(&(*symTableEntries)[x++], &elf->contents[offset + i * entrySize], entrySize);
    }
    
    return x;
    
}

Elf32_Shdr* getSectionHeaderByName(ElfFile *elf, char *name)
{
    int i;
    int numSections;
    char *sectionName;
    Elf32_Ehdr header;
    Elf32_Shdr *sections;
    Elf32_Shdr *ret = NULL;
    
    getElfHeader(elf, &header);
    numSections = getElfSectionHeaders(elf, &sections);

    for(i = 0; i < numSections; i++)
    {
        sectionName = getSectionName(elf, &sections[i]);
        if(memcmp(name, sectionName, strlen(name)) == 0)
        {
            ret = (Elf32_Shdr *)malloc(sizeof(Elf32_Shdr));
            memcpy(ret, &sections[i], sizeof(Elf32_Shdr));
            break;
        }
        
    }

    free(sections);
    
    return ret;
}

Elf32_Sym* getSymbolByName(ElfFile *elf, char *name)
{
    int i;
    int totalSymbols;
    Elf32_Sym *result = NULL;
    Elf32_Sym *symbolTableEntries;
    
    totalSymbols = getSymbolTableEntries(elf, &symbolTableEntries);
    
    for(i = 0; i < totalSymbols; i++)
    {
        if(memcmp(getSymbolName(elf, &symbolTableEntries[i]), name, strlen(name)) == 0)
        {
            result = (Elf32_Sym *)malloc(sizeof(Elf32_Sym));
            memcpy(result, &symbolTableEntries[i], sizeof(Elf32_Sym));
        }
    }
    
    free(symbolTableEntries);
    
    return result;
    
}

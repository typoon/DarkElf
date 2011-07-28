#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "darkelf.h"

//typedef struct
//{
  //unsigned char e_ident[EI_NIDENT];     /* Magic number and other info */
  //Elf32_Half    e_type;                 /* Object file type */
  //Elf32_Half    e_machine;              /* Architecture */
  //Elf32_Word    e_version;              /* Object file version */
  //Elf32_Addr    e_entry;                /* Entry point virtual address */
  //Elf32_Off     e_phoff;                /* Program header table file offset */
  //Elf32_Off     e_shoff;                /* Section header table file offset */
  //Elf32_Word    e_flags;                /* Processor-specific flags */
  //Elf32_Half    e_ehsize;               /* ELF header size in bytes */
  //Elf32_Half    e_phentsize;            /* Program header table entry size */
  //Elf32_Half    e_phnum;                /* Program header table entry count */
  //Elf32_Half    e_shentsize;            /* Section header table entry size */
  //Elf32_Half    e_shnum;                /* Section header table entry count */
  //Elf32_Half    e_shstrndx;             /* Section header string table index */
//} Elf32_Ehdr;

int main(int argc, char **argv)
{

    int i;
    ElfFile *elf;
    Elf32_Ehdr header;
    Elf32_Shdr *sectionHeaders;
    Elf32_Shdr *shText;
    Elf32_Sym *symTableEntries;
    int numberOfSections;
    int numberOfSymbolEntries;
    char *name;
    

    if(argc < 2)
    {
        printf("Use: %s <executable>\n", argv[0]);
        return -1;
    }
    
    if((elf = readElf(argv[1])) == NULL)
    {
        printf("An error occured. Aborting...\n");
        return -1;
    }
        
    getElfHeader(elf, &header);
    numberOfSections = getElfSectionHeaders(elf, &sectionHeaders);
    
    printf("Entry point: %08X\n", header.e_entry);
    printf("Program header offset %d\n", header.e_phoff);
    printf("Section header offset %d\n", header.e_shoff);
    printf("Program header table entry size %d\n", header.e_phentsize);
    printf("Number of entries in the program header %d\n", header.e_phnum);
    printf("Section header table entry size %d\n", header.e_shentsize);
    printf("Number of entries in the section header %d\n", header.e_shnum);
    printf("String table section header index %d\n", header.e_shstrndx);
    
    
    printf("INDEX\tNAME\t\t\tOFFSET\n");
    for(i = 0; i < numberOfSections; i++)
    {
        name = getSectionName(elf, &sectionHeaders[i]);
        printf("%d\t%s\t\t\t0x%08X\n", i, name, sectionHeaders[i].sh_offset);
    }
    
    numberOfSymbolEntries = getSymbolTableEntries(elf, &symTableEntries);
    printf("Number of symbol table entries: %d\n", numberOfSymbolEntries);
    
    for(i = 0; i < numberOfSymbolEntries; i++)
    {
        //printf("%d - %s\n", symTableEntries[i].st_name, &elf->contents[sectionHeaders[30].sh_offset + symTableEntries[i].st_name]);
    }
    
    //printf("String Table size: %d - %s\n", (sectionHeaders[header.e_shstrndx-1].sh_size), elf->contents[(sectionHeaders[header.e_shstrndx-1].sh_offset) + symTableEntries[i].st_name]);
    
    shText = getSectionHeaderByName(elf, ".text");
    if(shText != NULL)
    {
        printf("Name: %s\n", getSectionName(elf, shText));
        
        for(i = 0; i < shText->sh_size; i++)
        {
            printf("%02X ", elf->contents[shText->sh_offset + i]);
        }
        printf("\n");
        
    }
    
    free(shText);
    free(sectionHeaders);
    free(symTableEntries);
    
    return 0;
    
    

}

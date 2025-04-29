#ifndef ELF_READER_H
#define ELF_READER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>
#include <stdint.h>

typedef struct {
    uint32_t startAddr;
    uint32_t endAddr;
    char* funcName;
}funcBounds;


typedef struct {
    FILE *elf_file;
    Elf64_Ehdr elf_header;
    Elf64_Phdr *program_headers;
    Elf64_Shdr *sectionHeaders;
    Elf64_Shdr shstrtabHeader;
    char *shstrtab;
    char *text_section;
    int text_section_offset;
    int text_section_size;
    int program_counter;
    int pc_increment;
} Elf_Reader;

extern Elf_Reader rvisor_elf_reader;

    /**
     * Prints all sections in binary 
    */
    void printHeaders();

    /**
     * Prints all sections in binary 
    */
    void printSectionNames();
    /**
     * Prints the content of a specific section from an ELF binary
     * @param sectionName name of section
    */
    void printSection(const char *sectionName);
    
    /**
     * Move programs counter to main function 
    */
    int jumpToMain();

    /**
     * Move programs counter to entry function 
    */
    int jumpToEntry();

    /**
     * Stores data sections from binary in a buffer
    */
//    int rvisor_get_data_sections(FILE *elf_file, Elf64_Shdr *sectionHeaders, int sectionCount, char *buffer, int *bound, char *shstrtab);
   int rvisor_get_data_sections(char *buffer, int *bound);

    /**
     * Loads text sections into vector text_section
    */
   void rvisor_get_text_section();

    /**
     * Returns value of pseudo program counter
     * @return program counter
    */
    int getProgramCounter();

    /**
     * Returns address of text section
     * @return text section address
    */
    int rvisor_get_text_sectionOffset();

    /**
     * Moves pseudo program counter to an arbitrary location
     * @param newPC new value for program Counter
    */
    void setProgramCounter(int newPC);

    /**
     * Returns instruction at the location of the program counter
     * @return next instruction as uint32_t
    */
    // uint32_t rvisor_get_next_instruction(uint8_t *text_section, size_t textSize, size_t program_counter, size_t text_section_offset, size_t *pc_increment); 
    uint32_t rvisor_get_next_instruction(); 

    /**
     * Increments pseudo program counter by 4
    */
    void incProgramCounter();

    /**
     * Decrements pseudo program counter by 4
    */
    void decProgramCounter();
    
    /**
     * Retrieves information on symbols in a symbol table
     * @param symtabName name of symbol table
    */
    int getSymbols(const char *symtabName);

    /**
     * Retrieves the name of a function from the symbol table based on the address
     * @param addr address of function
     * @return name of function as string
    */
    const char* getFunctionName(int addr);

    /**
     * Retrieves the info for a symbol from the symbol table based on the name
     * @param symName name of the symbol
     * @return Symbol 
    */
    Elf64_Sym getSymbol(const char *symName);
    void getFunctions();

    int rvisor_init_rvisor_elf_reader(const char *filename);

    void print_sections(FILE* outfile, Elf64_Shdr* sectionHeaders, size_t section_count, const char* shstrtab);

    void free_rvisor_elf_reader(Elf_Reader *reader);

#endif
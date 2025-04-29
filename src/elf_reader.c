#include "elf_reader.h"

// Function to initialize Elf_Reader

Elf_Reader rvisor_elf_reader;


int rvisor_init_rvisor_elf_reader(const char *filename) {
    rvisor_elf_reader.elf_file = fopen(filename, "rb");
    if (!rvisor_elf_reader.elf_file) {
        perror("Error opening file");
        return -1;
    }

    // Read ELF header
    fread(&rvisor_elf_reader.elf_header, sizeof(Elf64_Ehdr), 1, rvisor_elf_reader.elf_file);

    // Check ELF magic number
    if (strncmp((char *)rvisor_elf_reader.elf_header.e_ident, "\x7F""ELF", 4) != 0) {
        fprintf(stderr, "Not a valid ELF file: %s\n", filename);
        return -1;
    }

    // Check if it's RISC-V
    if (rvisor_elf_reader.elf_header.e_machine != EM_RISCV) {
        fprintf(stderr, "Not a RISC-V Binary: %s\n", filename);
        return -1;
    }

    // Read program headers
    rvisor_elf_reader.program_headers = malloc(rvisor_elf_reader.elf_header.e_phnum * sizeof(Elf64_Phdr));
    fseek(rvisor_elf_reader.elf_file, rvisor_elf_reader.elf_header.e_phoff, SEEK_SET);
    fread(rvisor_elf_reader.program_headers, sizeof(Elf64_Phdr), rvisor_elf_reader.elf_header.e_phnum, rvisor_elf_reader.elf_file);

    // Read section headers
    rvisor_elf_reader.sectionHeaders = malloc(rvisor_elf_reader.elf_header.e_shnum * sizeof(Elf64_Shdr));
    fseek(rvisor_elf_reader.elf_file, rvisor_elf_reader.elf_header.e_shoff, SEEK_SET);
    fread(rvisor_elf_reader.sectionHeaders, sizeof(Elf64_Shdr), rvisor_elf_reader.elf_header.e_shnum, rvisor_elf_reader.elf_file);

    // Read section header string table
    Elf64_Shdr shstrtabHeader = rvisor_elf_reader.sectionHeaders[rvisor_elf_reader.elf_header.e_shstrndx];
    rvisor_elf_reader.shstrtab = malloc(shstrtabHeader.sh_size);
    fseek(rvisor_elf_reader.elf_file, shstrtabHeader.sh_offset, SEEK_SET);
    fread(rvisor_elf_reader.shstrtab, 1, shstrtabHeader.sh_size, rvisor_elf_reader.elf_file);

    rvisor_elf_reader.pc_increment = 4;

    return 0;
}


void printHeaders(Elf_Reader *reader) {
    // ...existing code...
}

void print_sections(FILE* outfile, Elf64_Shdr* sectionHeaders, size_t section_count, const char* shstrtab) {
    fprintf(outfile, "Sections: \n");
    for (size_t i = 0; i < section_count; i++) {
        const char* name = shstrtab + sectionHeaders[i].sh_name;
        fprintf(outfile, "Name: %s\n", name);
        fprintf(outfile, "Type: %u\n", sectionHeaders[i].sh_type);
        fprintf(outfile, "Size: %lu\n", sectionHeaders[i].sh_size);
        fprintf(outfile, "Address: 0x%lx\n", sectionHeaders[i].sh_addr);
        fprintf(outfile, "-----------------------------------------\n");
    }
}


void rvisor_get_text_section() {
    for (int i = 0; i < rvisor_elf_reader.elf_header.e_shnum; ++i) {
        const char *name = rvisor_elf_reader.shstrtab + rvisor_elf_reader.sectionHeaders[i].sh_name;
        if (strcmp(name, ".text") == 0) {
            rvisor_elf_reader.text_section_offset = rvisor_elf_reader.sectionHeaders[i].sh_addr;
            fseek(rvisor_elf_reader.elf_file, rvisor_elf_reader.sectionHeaders[i].sh_offset, SEEK_SET);
            rvisor_elf_reader.text_section = (char *)malloc(rvisor_elf_reader.sectionHeaders[i].sh_size);
            fread(rvisor_elf_reader.text_section, rvisor_elf_reader.sectionHeaders[i].sh_size, 1, rvisor_elf_reader.elf_file);
            rvisor_elf_reader.program_counter = rvisor_elf_reader.sectionHeaders[i].sh_offset;
            rvisor_elf_reader.text_section_size = rvisor_elf_reader.sectionHeaders[i].sh_size;
            break;
        }
    }
}


int rvisor_get_data_sections(char *buffer, int *bound) {
    uint64_t start_addr = 0;
    char *buffer_pointer = buffer;

    size_t index = 0;

    index++;
    
    for (index; index < rvisor_elf_reader.elf_header.e_shnum; index++) {
        Elf64_Shdr *section = &rvisor_elf_reader.sectionHeaders[index];
        const char *name = rvisor_elf_reader.shstrtab + section->sh_name;

        if (strcmp(name, ".text") == 0)
            continue;
        
        if (section->sh_addr == 0)
            continue;

        *bound = section->sh_addr + section->sh_size;

        // Zero out .bss-like sections
        if (strcmp(name, ".sbss") == 0 || strcmp(name, ".bss") == 0 || strcmp(name, ".tbss") == 0) {
            buffer_pointer = (char *)(section->sh_addr);
            memset(buffer_pointer, 0x0, section->sh_size);
            continue;
        }

        // Seek to the section offset in the ELF file
        if (fseek(rvisor_elf_reader.elf_file, section->sh_offset, SEEK_SET) != 0) {
            fprintf(stderr, "Failed to seek to section %s\n", name);
            return start_addr;
        }

        // Read the section into the buffer
        buffer_pointer = section->sh_addr;
        if (fread(buffer_pointer, 1, section->sh_size, rvisor_elf_reader.elf_file) != section->sh_size) {
            fprintf(stderr, "Failed to read section %s\n", name);
            return start_addr;
        }
    }

    // Align the bound address to the next page
    *bound = (*bound + 0x1fff) & 0xfffffffffffff000;

    return start_addr;
}

uint32_t rvisor_get_next_instruction() {
    size_t offset = rvisor_elf_reader.program_counter - rvisor_elf_reader.text_section_offset;
    // printf("Offset: %lu\n", offset);
    // printf("Text Section Size: %lu\n", rvisor_elf_reader.text_section_size);
    uint32_t instruction = 0;

    if (offset >= rvisor_elf_reader.text_section_size) {
        return 0; // Return 0 if out of bounds
    }

    uint32_t opcode = (uint32_t)rvisor_elf_reader.text_section[offset];

    if ((opcode & 0b11) == 0b11) {
        rvisor_elf_reader.pc_increment = 4; // Full 32-bit instruction
        instruction = *(uint32_t *)(rvisor_elf_reader.text_section + offset);
    } else {
        rvisor_elf_reader.pc_increment = 2; // Compressed 16-bit instruction
        instruction = *(uint16_t *)(rvisor_elf_reader.text_section + offset);
    }

    // Combine bytes into a 32-bit instruction
    // for (size_t j = 0; j < *pc_increment; ++j) {
    //     instruction |= (uint32_t)text_section[offset + j] << (j * 8);
    // }

    return instruction;
}


// Function to clean up memory
void free_rvisor_elf_reader(Elf_Reader *reader) {
    fclose(reader->elf_file);
    free(reader->program_headers);
    free(reader->sectionHeaders);
    free(reader->shstrtab);
    free(reader->text_section);
}
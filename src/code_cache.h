#ifndef CODE_CACHE_H
#define CODE_CACHE_H

#include <stdint.h>
#include <sys/mman.h>

#include "./../helpers/uthash.h"
#include "decode.h"
#include "dispatcher.h"
#include "elf_reader.h"
#include "encode.h"
#include "instruction_names.h"
#include "instructions.h"
#include "opcodes.h"
#include "printUtils.h"
#include "railBasicBlock.h"
#include "util.h"
#include "rail.h"

#define CACHE_SIZE (4 * 1024 * 1024)
#define BB_BLOCK_SIZE 2048 // Allocate in chunks of 1024 entries

extern char *memory;
extern int rvisor_memory_index;
extern int rvisor_stub_code;
extern uint64_t rvisor_current_bb;

typedef struct {
  uint64_t addr;               // Key: start address of the BB
  rvisor_basic_block metadata; // Metadata
  UT_hash_handle hh;           // Hash table handle
} bb_entry;

typedef struct {
  uint64_t bin_addr;   // Key: start address of the BB
  uint64_t cache_addr; // Metadata
  UT_hash_handle hh2;   // Hash table handle
} bb_exit_address;

extern bb_exit_address *bb_exit_map;

extern bb_entry *basic_blocks_map;
extern bb_entry *bb_pool; // Pool of preallocated blocks
extern int bb_pool_index;

// Function to insert a basic block
void rvisor_insert_basic_block(uint64_t addr, rvisor_basic_block metadata);
void rvisor_insert_exit(uint64_t bin_addr, uint64_t cache_addr);
// Function to find a basic block by address
rvisor_basic_block *find_basic_block(uint64_t addr);
uint64_t *find_exit(uint64_t addr);

// Function to clear the hash table
void clear_basic_blocks();

int cache_init();

int rvisor_allocate_root();

#ifdef STUBREGIONS
  int insert_stub_region(int override);
#endif

void rvisor_insert_inst16(uint16_t newInst);
void rvisor_insert_inst32(uint32_t newInst);
void allocate_bb_pool();
int rvisor_allocate_bb(uint64_t binary_address);
int rvisor_allocate_trace(uint64_t binary_address, int count);
int rvisor_allocate_trace_block(uint64_t binaryAddress,
                                rvisor_basic_block railBB, int count);

#endif
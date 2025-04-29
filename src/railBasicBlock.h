#ifndef RAILBASICBLOCK_H
#define RAILBASICBLOCK_H

typedef enum {
  BRANCH,
  DIRECT_JUMP,
  INDIRECT_JUMP,
  FUNCTION_CALL,
  FUNCTION_RETURN,
  SYSCALL,
  SEGMENTED
} rvisor_bb_type;

static const char *basic_block_type_str[6] = {"Indirect Branch", "Direct Branch",
                                    "Function Call",   "Function Return",
                                    "Syscall",         "Segmented Block"};

typedef struct {
  uint64_t first_addr;            // First address from binary
  uint64_t last_addr;             // Last address from binary
  uint64_t start_location_in_cache; // rvisor_memory_index where block begins in cache
  uint64_t end_location_in_cache;   // rvisor_memory_index where block ends in cache
  int num_instructions;
  int taken_block; // Control switched here from a taken branch from a previous
                   // block
  rvisor_bb_type type;
  int resume;
  // bool taken = true;
  uint32_t start_inst;         // Bytes for last instruction
  uint32_t terminal_inst;      // Bytes for last instruction
  uint64_t basic_block_address; // If block is segmented then this stores the
                              // address of the first inst
  uint64_t taken_addr;
  uint64_t fall_through_addr;
  uint64_t ecall_next;
  // unordered_map<int, uint64_t> bbExits;
} rvisor_basic_block;

#endif
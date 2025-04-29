#include "code_cache.h"

#define MAX_STUB_DISTANCE 900 

// Define the extern variables
char *memory;
int rvisor_memory_index = 0;
int rvisor_stub_code = 0;
uint64_t rvisor_current_bb = 0;

int stub_distance_tracker = 0;
#ifdef STUBREGIONS
  uint64_t nearest_stub_region = 0;
  int num_stub_regions = 0;
#endif

uint64_t addr;
uint64_t addr_lsb;
uint64_t addr_msb;
uint64_t lui_inst;
uint64_t addi_inst;
int err;

bb_entry *basic_blocks_map = NULL;
bb_exit_address *bb_exit_map = NULL;
bb_entry *bb_pool = NULL;
int bb_pool_index = 0;

#define INSERT_LI(value, reg)                                                  \
  do {                                                                         \
    addr = (uint64_t)(value);                                                  \
    addr_lsb = addr & 0xfff;                                                   \
    addr_lsb = addr_lsb >> 1;                                                  \
    addr_msb = (addr >> 12) & 0xfffff;                                         \
    lui_inst = LUI_OP | (reg << 7) | (addr_msb << 12);                         \
    *(uint64_t *)(memory + rvisor_memory_index) = lui_inst;                            \
    rvisor_memory_index += 4;                                                          \
    addi_inst =                                                                \
        OPIMM_OP | (reg << 7) | (0 << 12) | (reg << 15) | (addr_lsb << 20);    \
    *(int *)(memory + rvisor_memory_index) = addi_inst;                                \
    rvisor_memory_index += 4;                                                          \
    *(int *)(memory + rvisor_memory_index) = addi_inst;                                \
    rvisor_memory_index += 4;                                                          \
    stub_distance_tracker += 3;                                                          \
    rvisor_stub_code += 12;                                                          \
  } while (0)

#define INSERT_ROUTINE(routine, length)                                        \
  do {                                                                         \
    for (int k = 0; k < length; k++) {                                         \
      *(int *)(memory + rvisor_memory_index) = *(int *)(routine + k * 4);              \
      rvisor_memory_index += 4;                                                        \
    }                                                                          \
    rvisor_stub_code += length*4;                                                        \
    stub_distance_tracker += length;                                                        \
  } while (0)

#define INSERT_INST32(inst)                                                    \
  do {                                                                         \
    *(int *)(memory + rvisor_memory_index) = inst;                                     \
    rvisor_memory_index += 4;                                                          \
    stub_distance_tracker++;                                                          \
  } while (0)
  
  #define INSERT_INST16(inst)                                                    \
  do {                                                                         \
    *(int *)(memory + rvisor_memory_index) = inst;                                     \
    rvisor_memory_index += 2;                                                          \
    stub_distance_tracker++;                                                          \
  } while (0)

#define INSERT_BB(railBB)                                                      \
  do {                                                                         \
    rvisor_insert_basic_block(railBB.first_addr, railBB);                      \
  } while (0)

// #define INSERT_INLINE_ROUTINE(vector)                                          \
//   do {                                                                         \
//     if (vector.size) {                                                         \
//       for (size_t i = 0; i < vector.size; i++) {                               \
//         INSERT_INST32(vector.data[i]);                                         \
//       }                                                                        \
//     }                                                                          \
//   } while (0)

#define INSERT_INLINE_ROUTINE(vector)                                          \
  do {                                                                         \
    uint64_t *ptr = (vector).data;                                             \
    uint64_t *end = ptr + (vector).size;                                       \
    while (ptr + 3 < end) {                                                    \
      INSERT_INST32(*ptr++);                                                   \
      INSERT_INST32(*ptr++);                                                   \
      INSERT_INST32(*ptr++);                                                   \
      INSERT_INST32(*ptr++);                                                   \
    }                                                                          \
    while (ptr < end) {                                                        \
      INSERT_INST32(*ptr++);                                                   \
    }                                                                          \
  } while (0)


void rvisor_insert_inst16(uint16_t newInst) {
  *(int *)(memory + rvisor_memory_index) = newInst;
  rvisor_memory_index += 2;
}

void rvisor_insert_inst32(uint32_t newInst) {
  *(int *)(memory + rvisor_memory_index) = newInst;
  rvisor_memory_index += 4;
}

// ALLOCATE BB POOL
void allocate_bb_pool() {
  bb_pool = (bb_entry *)malloc(BB_BLOCK_SIZE * sizeof(bb_entry));
  bb_pool_index = 0;
}

// INITIALIZE CACHE
int cache_init() {
  memory = (char *)(mmap((void *)(0x5000000), // address
                         CACHE_SIZE,          // size = 4MB
                         PROT_READ | PROT_WRITE | PROT_EXEC,
                         MAP_ANONYMOUS | MAP_PRIVATE,
                         -1,  // fd (not used here)
                         0)); // offset (not used here)

  if (memory == MAP_FAILED) {
    perror("mmap for memory failed");
    exit(1);
  }
  rvisor_memory_index = 0;
  rvisor_current_bb = 0;
  return 0;
}

// ALLOCATE ROOT
int rvisor_allocate_root() {
  rvisor_elf_reader.program_counter = rvisor_elf_reader.elf_header.e_entry;
  INSERT_LI((uint64_t)(&init_switch), T6);
  INSERT_INST32(0x000f8067); // jalr x0, 0(t6)

  #ifdef STUBREGIONS
    insert_stub_region(1);
  #endif

  return 0;
}

// ALLOCATE BASIC BLOCK
int rvisor_allocate_bb(uint64_t binary_address) {
  uint32_t jalr_inst = 0x000f8067;

  // printf("%p\n", memory + rvisor_memory_index);

  if (rvisor_memory_index >= (4 * 1024 * 1024) / 4 - 30) {
    printf("Clearing Cache\n");
    // rvisor_basic_blocks.clear();
    rvisor_memory_index = 0;
    rvisor_stub_code += 4;
  }

  rvisor_elf_reader.program_counter = binary_address;
  uint32_t nextInst = rvisor_get_next_instruction();

  rvisor_basic_block railBB;
  railBB.first_addr = rvisor_elf_reader.program_counter;
  railBB.basic_block_address = rvisor_elf_reader.program_counter;
  railBB.start_inst = nextInst;
  railBB.num_instructions = 0;
  // CodeCache::setCurrentBB(railBB.first_addr)
  rvisor_current_bb = railBB.first_addr;
  railBB.start_location_in_cache = (uint64_t *)(memory + rvisor_memory_index);

  // INSERT_ROUTINE((char *)&restore_scratch, 1);
  INSERT_INST32(0xfe013f83);
  rvisor_stub_code += 4;

  int shouldInstrumentOnInst;

  RvInst decodedInst;
  int compressed = 0;
  uint8_t opcode;
  int replace;

  // TODO: Code for Inline Routines

  if(rvisor_bb_routine_pre_alloc){
    rvisor_bb_routine_pre_alloc(railBB, NULL);
  }

  INSERT_INLINE_ROUTINE(rvisor_inline_inst_bb_vec_pre);

  while (nextInst) {
    replace = 0;
    compressed = 0;
    // #ifdef DEBUG
    // printf("Next Instruction: %08x\n", nextInst);
    //     decode_instruction(nextInst, 1);
    // #endif
    // printf("Here!!!\n");
    // printf("%x\n", rvisor_elf_reader.program_counter);
    // printf("%08x :\n", nextInst);
    // fprintf(rvisor_logger, "%05x:\t %08x \t", elqf_reader.program_counter,
    // nextInst); decode_instruction(nextInst, 0); printf("Moving to next
    // instruction\n");

    opcode = nextInst & 0x7f;

    if ((opcode & 0b11) != 0b11) {
      decodedInst = decode_instruction16(nextInst);
      compressed = 1;
    }

    // TODO: Code for Pre Routines
    // TODO: Code for Pre Routines Inline

    if (opcode == BRANCH_OP || decodedInst.name == C_BEQZ ||
        decodedInst.name == C_BNEZ) {
      railBB.type = BRANCH;
      railBB.last_addr = rvisor_elf_reader.program_counter;
      railBB.terminal_inst = rvisor_get_next_instruction();

      INSERT_INLINE_ROUTINE(rvisor_inline_inst_bb_vec_post);

      INSERT_ROUTINE((char *)&inline_save, 2); // 2 insts

      // RvInst decodedInst;
      if (decodedInst.name == C_BEQZ) {
        printf("Found C.BEQZ\n");
        decodedInst = decode_instruction(nextInst, 0);
        decodedInst.rs1 = decodedInst.rs1 + 8;
        decodedInst.rs2 = 0;
        decodedInst.funct3 = 0;

        railBB.taken_addr = railBB.last_addr + decodedInst.imm;
        railBB.fall_through_addr = railBB.last_addr + 2;

      } else if (decodedInst.name == C_BNEZ) {
        printf("Found C.BNEZ\n");
        decodedInst = decode_instruction(nextInst, 0);
        decodedInst.rs1 = decodedInst.rs1 + 8;
        decodedInst.rs2 = 0;
        decodedInst.funct3 = 1;

        railBB.taken_addr = railBB.last_addr + decodedInst.imm;
        railBB.fall_through_addr = railBB.last_addr + 2;

      } else {
        decodedInst = decode_Btype(nextInst);

        railBB.taken_addr = railBB.last_addr + decodedInst.imm;
        railBB.fall_through_addr = railBB.last_addr + 4;
      }

      // #ifdef STUBREGIONS
      //   // BRANCH_OP TAKEN
      //   long int cur_index = memory + rvisor_memory_index;
      //   decodedInst.imm = nearest_stub_region - cur_index + 24;
        
      //   uint32_t encodedBranch = ((BRANCH_OP & 0b1111111) << 0) |
      //                           (((decodedInst.imm >> 11) & 0b1) << 7) |
      //                           (((decodedInst.imm >> 1) & 0b1111) << 8) |
      //                           (((decodedInst.imm >> 5) & 0b111111) << 25) |
      //                           (((decodedInst.imm >> 12) & 0b1) << 31) |
      //                           ((decodedInst.funct3 & 0b111) << 12) |
      //                           ((decodedInst.rs1 & 0b11111) << 15) |
      //                           ((decodedInst.rs2 & 0b11111) << 20) | 0;
        
      //   printf("IMM is %d\n", decodedInst.imm);
      //   printf("Encoded Branch at %x is %x\n", cur_index, encodedBranch);
      //   printf("Stub distance is %d\n", stub_distance_tracker);
      //   INSERT_INST32(encodedBranch);
 
      //   // BRANCH_OP NOT TAKEN
      //   uint32_t not_taken = encode_JAL(ZERO, nearest_stub_region - cur_index);
      //   printf("Encoded not taken at %x is %x\n", cur_index, not_taken);
      //   printf("\n");
      //   INSERT_INST32(not_taken);
      //   rvisor_stub_code += 8;
      // #else
        decodedInst.imm = 20;

        uint32_t encodedBranch = ((BRANCH_OP & 0b1111111) << 0) |
                                (((decodedInst.imm >> 11) & 0b1) << 7) |
                                (((decodedInst.imm >> 1) & 0b1111) << 8) |
                                (((decodedInst.imm >> 5) & 0b111111) << 25) |
                                (((decodedInst.imm >> 12) & 0b1) << 31) |
                                ((decodedInst.funct3 & 0b111) << 12) |
                                ((decodedInst.rs1 & 0b11111) << 15) |
                                ((decodedInst.rs2 & 0b11111) << 20) | 0;
        INSERT_INST32(encodedBranch);

        // BRANCH_OP NOT TAKEN
        INSERT_LI((uint64_t)(&context_switch), T6); // 3 insts
        INSERT_INST32(jalr_inst);

        // BRANCH_OP TAKEN
        INSERT_LI((uint64_t)(&context_switch_taken),
                  T6); // 3 insts
        INSERT_INST32(jalr_inst);
        rvisor_stub_code += 8;
      // #endif

      railBB.end_location_in_cache = (uint64_t *)(memory + rvisor_memory_index);

      break;
    } else if (opcode == JAL_OP || decodedInst.name == C_J) {
      railBB.type = DIRECT_JUMP;
      railBB.last_addr = rvisor_elf_reader.program_counter;
      railBB.terminal_inst = rvisor_get_next_instruction();

      decodedInst = decode_instruction(nextInst, 0);
      railBB.taken_addr = railBB.last_addr + decodedInst.imm;
      railBB.fall_through_addr = 0;

      INSERT_INLINE_ROUTINE(rvisor_inline_inst_bb_vec_post);
      
      #ifdef STUBREGIONS
        long int cur_index = memory + rvisor_memory_index;
        uint32_t not_taken = encode_JAL(ZERO, nearest_stub_region - cur_index + 24);
        INSERT_INST32(not_taken);  
        rvisor_stub_code += 4;
      #else
        INSERT_ROUTINE((char *)&inline_save, 2);          // 2 insts
        INSERT_LI((uint64_t)(&context_switch_taken), T6); // 3 insts
        INSERT_INST32(jalr_inst);
        rvisor_stub_code += 4;
      #endif
      break;
    } else if (opcode == JALR_OP || decodedInst.name == C_JR ||
               decodedInst.name == C_JALR) {
      railBB.type = INDIRECT_JUMP;
      railBB.last_addr = rvisor_elf_reader.program_counter;
      railBB.terminal_inst = rvisor_get_next_instruction();

      INSERT_INLINE_ROUTINE(rvisor_inline_inst_bb_vec_post);

      #ifdef STUBREGIONS
        long int cur_index = memory + rvisor_memory_index;
        uint32_t not_taken = encode_JAL(ZERO, nearest_stub_region - cur_index + 24);
        INSERT_INST32(not_taken);  
        rvisor_stub_code += 4;
      #else
        INSERT_ROUTINE((char *)&inline_save, 2);
        INSERT_LI((uint64_t)(&context_switch_taken), T6);
        INSERT_INST32(jalr_inst);
        rvisor_stub_code += 4;
      #endif

      break;
    } else if (opcode == AUIPC_OP && !compressed) {

      decodedInst = decode_Utype(nextInst); // Replace with getImm
      uint64_t addr =
          rvisor_elf_reader.program_counter + (decodedInst.imm << 12);

      INSERT_LI(addr, (GPregs)(decodedInst.rd));

      // INST TYPE RUNTIME ROUTINE 
      if(inst_type_routine_count){
        RvInst decoded_inst = decode_instruction(nextInst, 0);
        Rvisor_Rt_Routine_Inst typeInst = get_inst_type_routine(decoded_inst.type);
        if(typeInst){
          railBB.type = SEGMENTED;
          railBB.last_addr = rvisor_elf_reader.program_counter;
          railBB.terminal_inst = nextInst;  
          railBB.end_location_in_cache = (uint64_t *)(memory + rvisor_memory_index);
      
          INSERT_ROUTINE((char *)&inline_save, 2);          // 2 insts
          INSERT_LI((uint64_t)(&context_switch_taken), T6); // 3 insts
          INSERT_INST32(jalr_inst);
          rvisor_stub_code += 4;
          INSERT_BB(railBB);
          return 1;
        }
      }

      rvisor_elf_reader.program_counter += rvisor_elf_reader.pc_increment;
      nextInst = rvisor_get_next_instruction();
      continue;

    } else if (opcode == SYSTEM_OP && !compressed) {
      INSERT_ROUTINE((char *)&inline_save, 2);
      INSERT_INST32(0x0200006f); // j 32

      INSERT_LI((uint64_t)(&rvisor_exit_binary), T6);
      INSERT_INST32(jalr_inst);

      // Handling for BRK
      INSERT_INST32(0x02051463); // bne a0, zero, 40
      INSERT_INST32(0x10000537); // lui a0, 0x10000
      INSERT_INST32(0x0200006f); // jal x0, 32

      // Handling for emulated syscalls
      INSERT_INST32(0x0d600f93); // addi t6, zero, 214
      INSERT_INST32(0xfff888e3); // beq a7, t6, -16
      INSERT_INST32(0x05d00f93); // addi t6, zero, 93
      INSERT_INST32(0xfdf88ce3); // beq a7, t6, -40
      INSERT_INST32(0x05e00f93); // addi t6, zero, 94
      INSERT_INST32(0xfdf888e3); // beq a7, t6, -48
      INSERT_INST32(nextInst);   // ecall

      INSERT_ROUTINE((char *)&inline_load, 2);
      
      rvisor_stub_code += 44;

      
      rvisor_elf_reader.program_counter += rvisor_elf_reader.pc_increment;
      nextInst = rvisor_get_next_instruction();

      continue;
    }
    
    // if (rvisor_inst_routine_post_alloc) {
    //   replace = rvisor_inst_routine_post_alloc(decode_instruction(nextInst, 0));
    //   if(replace){
    //     rvisor_elf_reader.program_counter += rvisor_elf_reader.pc_increment;
    //     nextInst = rvisor_get_next_instruction();
    //     continue;
    //   }
    // }  
    
    // INST GROUP ALLOCATOR ROUTINE 
    if(inst_group_routine_count){
      RvInst decoded_inst = decode_instruction(nextInst, 0);
      Rvisor_Alloc_Routine_Inst groupInst = get_inst_group_routine(decoded_inst.group);
      if(groupInst){
        replace = groupInst(decoded_inst);
        if(replace){
          rvisor_elf_reader.program_counter += rvisor_elf_reader.pc_increment;
          nextInst = rvisor_get_next_instruction();
          continue;
        }
      }
    }
    
    
    
    if (compressed) {
      INSERT_INST16(nextInst);
    } else {
      INSERT_INST32(nextInst);
    }

    // INST TYPE RUNTIME ROUTINE 
    if(inst_type_routine_count){
      RvInst decoded_inst = decode_instruction(nextInst, 0);
      Rvisor_Rt_Routine_Inst typeInst = get_inst_type_routine(decoded_inst.type);
      if(typeInst){
        railBB.type = SEGMENTED;
        railBB.last_addr = rvisor_elf_reader.program_counter;
        railBB.terminal_inst = nextInst;  
        railBB.end_location_in_cache = (uint64_t *)(memory + rvisor_memory_index);
    
        INSERT_ROUTINE((char *)&inline_save, 2);          // 2 insts
        INSERT_LI((uint64_t)(&context_switch_taken), T6); // 3 insts
        INSERT_INST32(jalr_inst);
        rvisor_stub_code += 4;
        INSERT_BB(railBB);
        return 1;
      }
    }

    // INST ROUTINE RUNTIME
    if(rvisor_inst_routine_post_rt || rvisor_inst_routine_pre_rt) {
      railBB.type = SEGMENTED;
      railBB.last_addr = rvisor_elf_reader.program_counter;
      railBB.terminal_inst = nextInst;  
      railBB.end_location_in_cache = (uint64_t *)(memory + rvisor_memory_index);
  
      INSERT_ROUTINE((char *)&inline_save, 2);          // 2 insts
      INSERT_LI((uint64_t)(&context_switch_taken), T6); // 3 insts
      INSERT_INST32(jalr_inst);
      rvisor_stub_code += 4;
      INSERT_BB(railBB);
      return 1;
    }


    rvisor_elf_reader.program_counter += rvisor_elf_reader.pc_increment;
    nextInst = rvisor_get_next_instruction();
  }

  if (rvisor_bb_routine_post_alloc) {
    rvisor_bb_routine_post_alloc();
  }

  #ifdef STUBREGIONS
    insert_stub_region(0);
  #endif

  INSERT_BB(railBB);
  return 1;
}

void rvisor_insert_basic_block(uint64_t addr, rvisor_basic_block metadata) {
  if (bb_pool == NULL || bb_pool_index >= BB_BLOCK_SIZE) {
    allocate_bb_pool();
  }

  // bb_entry *entry = malloc(sizeof(bb_entry));
  bb_entry *entry = &bb_pool[bb_pool_index++];
  entry->addr = addr;
  entry->metadata = metadata;
  HASH_ADD(hh, basic_blocks_map, addr, sizeof(uint64_t), entry);
}

// Function to find a basic block by address
rvisor_basic_block *find_basic_block(uint64_t addr) {
  bb_entry *entry;
  HASH_FIND(hh, basic_blocks_map, &addr, sizeof(uint64_t), entry);
  return entry ? &entry->metadata : NULL;
}

// Function to clear the hash table
void clear_basic_blocks() {
  bb_entry *entry, *tmp;
  HASH_ITER(hh, basic_blocks_map, entry, tmp) {
    HASH_DEL(basic_blocks_map, entry);
    free(entry);
  }
}

void rvisor_insert_exit(uint64_t bin_addr, uint64_t cache_addr) {
  bb_exit_address *bb_exit = malloc(sizeof(bb_exit_address));
  bb_exit->bin_addr = bin_addr;
  bb_exit->cache_addr = cache_addr;
  HASH_ADD(hh2, bb_exit_map, bin_addr, sizeof(uint64_t), bb_exit);
}

uint64_t *find_exit(uint64_t addr) {
  bb_exit_address *bb_exit;
  HASH_FIND(hh2, bb_exit_map, &addr, sizeof(uint64_t), bb_exit);
  return bb_exit ? &bb_exit->cache_addr : NULL;
}

int rvisor_allocate_trace(uint64_t binary_address, int count) {
  uint32_t nextInst = rvisor_get_next_instruction();
  rvisor_elf_reader.program_counter = binary_address;

  rvisor_basic_block railBB;
  railBB.first_addr = rvisor_elf_reader.program_counter;
  railBB.basic_block_address = rvisor_elf_reader.program_counter;
  railBB.start_inst = nextInst;
  railBB.num_instructions = 0;
  rvisor_current_bb = railBB.first_addr;
  railBB.start_location_in_cache = (uint64_t *)(memory + rvisor_memory_index);

  INSERT_INST32(0xfe013f83); // ld t6, -32(sp)

  for (int i = count; i > 0; i--) {
    int next_address =
        rvisor_allocate_trace_block(binary_address, railBB, i - 1);
    if (next_address == 0)
      break;
    binary_address = next_address;
  }

  railBB.end_location_in_cache = (uint64_t *)(memory + rvisor_memory_index);
  railBB.last_addr = rvisor_elf_reader.program_counter;
  railBB.terminal_inst = rvisor_get_next_instruction();
  INSERT_BB(railBB);

  return 0;
}

int rvisor_allocate_trace_block(uint64_t binary_address,
                                rvisor_basic_block railBB, int count) {
  uint32_t jalr_inst = 0x000f8067;

  // printf("%p\n", memory + rvisor_memory_index);

  if (rvisor_memory_index >= CACHE_SIZE / 4 - 30) {
    printf("Clearing Cache\n");
    // rvisor_basic_blocks.clear();
    rvisor_memory_index = 0;
  }

  rvisor_elf_reader.program_counter = binary_address;
  uint32_t nextInst = rvisor_get_next_instruction();

  // INSERT_INST32(0xfe013f83);

  RvInst decodedInst;
  int compressed = 0;
  uint8_t opcode;
  int replace;

  // TODO: Code for Inline Routines

  while (nextInst) {
    compressed = 0;
    // #ifdef DEBUG
    // printf("Next Instruction: %08x\n", nextInst);
    //     decode_instruction(nextInst, 1);
    // #endif
    // printf("Here!!!\n");
    // printf("%x\n", rvisor_elf_reader.program_counter);
    // printf("%08x :\n", nextInst);
    // fprintf(rvisor_logger, "%05x:\t %08x \t", elqf_reader.program_counter,
    // nextInst); decode_instruction(nextInst, 0); printf("Moving to next
    // instruction\n");

    opcode = nextInst & 0x7f;

    if ((opcode & 0b11) != 0b11) {
      decodedInst = decode_instruction16(nextInst);
      compressed = 1;
    }

    // TODO: Code for Pre Routines
    // TODO: Code for Pre Routines Inline

    if (opcode == BRANCH_OP || decodedInst.name == C_BEQZ ||
        decodedInst.name == C_BNEZ) {
      railBB.type = BRANCH;
      railBB.last_addr = rvisor_elf_reader.program_counter;
      railBB.terminal_inst = rvisor_get_next_instruction();

      // RvInst decodedInst;
      if (decodedInst.name == C_BEQZ) {
        printf("Found C.BEQZ\n");
        decodedInst = decode_instruction(nextInst, 0);
        decodedInst.rs1 = decodedInst.rs1 + 8;
        decodedInst.rs2 = 0;
        decodedInst.funct3 = 0;

        railBB.taken_addr = railBB.last_addr + decodedInst.imm;
        railBB.fall_through_addr = railBB.last_addr + 2;

      } else if (decodedInst.name == C_BNEZ) {
        printf("Found C.BNEZ\n");
        decodedInst = decode_instruction(nextInst, 0);
        decodedInst.rs1 = decodedInst.rs1 + 8;
        decodedInst.rs2 = 0;
        decodedInst.funct3 = 1;

        railBB.taken_addr = railBB.last_addr + decodedInst.imm;
        railBB.fall_through_addr = railBB.last_addr + 2;

      } else {
        decodedInst = decode_Btype(nextInst);

        railBB.taken_addr = railBB.last_addr + decodedInst.imm;
        railBB.fall_through_addr = railBB.last_addr + 4;
      }

      // exits[elfReader.getProgramCounter()] = (uint64_t)((int *)(memory +
      // rvisor_memory_index));
      INSERT_INLINE_ROUTINE(rvisor_inline_inst_bb_vec_post);

      INSERT_ROUTINE((char *)&inline_save, 2); // 2 insts

      rvisor_insert_exit(rvisor_elf_reader.program_counter,
                         (uint64_t)((int *)(memory + rvisor_memory_index)));
      decodedInst.imm = 36;

      // uint32_t encodedBranch =
      //     encode_Btype(BRANCH_OP, decodedInst.imm, decodedInst.funct3,
      //                  decodedInst.rs1, decodedInst.rs2);
      uint32_t encodedBranch = ((BRANCH_OP & 0b1111111) << 0) |
                               (((decodedInst.imm >> 11) & 0b1) << 7) |
                               (((decodedInst.imm >> 1) & 0b1111) << 8) |
                               (((decodedInst.imm >> 5) & 0b111111) << 25) |
                               (((decodedInst.imm >> 12) & 0b1) << 31) |
                               ((decodedInst.funct3 & 0b111) << 12) |
                               ((decodedInst.rs1 & 0b11111) << 15) |
                               ((decodedInst.rs2 & 0b11111) << 20) | 0;
      INSERT_INST32(encodedBranch);

      // BRANCH_OP NOT TAKEN
      INSERT_LI(rvisor_elf_reader.program_counter, T6); // 3 insts
      INSERT_INST32(0x01f13423);                        // sd T6, 8(sp)

      INSERT_LI((uint64_t)(&context_switch_trace_exit), T6); // 3 insts
      INSERT_INST32(jalr_inst);

      // BRANCH_OP TAKEN
      INSERT_LI(rvisor_elf_reader.program_counter, T6); // 3 insts
      INSERT_INST32(0x01f13423);                        // sd T6, 8(sp)

      INSERT_LI((uint64_t)(&context_switch_trace_exit_taken),
                T6); // 3 insts
      INSERT_INST32(jalr_inst);

      // railBB.end_location_in_cache = (uint64_t *)(memory + rvisor_memory_index);
      // INSERT_BB(railBB);

      return 0;
    } else if (opcode == JAL_OP || decodedInst.name == C_J) {
      decodedInst = decode_instruction(nextInst, 0);

      INSERT_INLINE_ROUTINE(rvisor_inline_inst_bb_vec_post);

      if (count > 0) {
        if (decodedInst.rd != ZERO) {
          INSERT_LI(rvisor_elf_reader.program_counter + 4, decodedInst.rd);
        }
        return rvisor_elf_reader.program_counter + decodedInst.imm;
      } else {
        rvisor_insert_exit(rvisor_elf_reader.program_counter,
                           (uint64_t)((int *)(memory + rvisor_memory_index)));
        INSERT_ROUTINE((char *)&inline_save, 2); // 2 insts

        INSERT_LI(rvisor_elf_reader.program_counter, T6); // 3 insts
        INSERT_INST32(0x01f13423);                        // sd T6, 8(sp)

        INSERT_LI((uint64_t)(&context_switch_trace_exit_taken), T6); // 3 insts
        INSERT_INST32(jalr_inst);
      }
      return 0;
    } else if (opcode == JALR_OP || decodedInst.name == C_JR ||
               decodedInst.name == C_JALR) {

      INSERT_INLINE_ROUTINE(rvisor_inline_inst_bb_vec_post);

      rvisor_insert_exit(rvisor_elf_reader.program_counter,
                         (uint64_t)((int *)(memory + rvisor_memory_index)));
      INSERT_ROUTINE((char *)&inline_save, 2);

      INSERT_LI(rvisor_elf_reader.program_counter, T6); // 3 insts
      INSERT_INST32(0x01f13423);                        // sd T6, 8(sp)

      INSERT_LI((uint64_t)(&context_switch_trace_exit_taken), T6);
      INSERT_INST32(jalr_inst);

      railBB.end_location_in_cache = (uint64_t *)(memory + rvisor_memory_index);
      // INSERT_BB(railBB);
      return 0;
    } else if (opcode == AUIPC_OP && !compressed) {

      decodedInst = decode_Utype(nextInst); // Replace with getImm
      uint64_t addr =
          rvisor_elf_reader.program_counter + (decodedInst.imm << 12);

      INSERT_LI(addr, (GPregs)(decodedInst.rd));

      rvisor_elf_reader.program_counter += rvisor_elf_reader.pc_increment;
      nextInst = rvisor_get_next_instruction();
      continue;

    } else if (opcode == SYSTEM_OP && !compressed) {
      INSERT_ROUTINE((char *)&inline_save, 2);
      INSERT_INST32(0x0200006f); // j 32

      INSERT_LI((uint64_t)(&rvisor_exit_binary), T6);
      INSERT_INST32(jalr_inst);

      // Handling for BRK
      INSERT_INST32(0x02051463); // bne a0, zero, 40
      INSERT_INST32(0x10000537); // lui a0, 0x10000
      INSERT_INST32(0x0200006f); // jal x0, 32

      // Handling for emulated syscalls
      INSERT_INST32(0x0d600f93); // addi t6, zero, 214
      INSERT_INST32(0xfff888e3); // beq a7, t6, -16
      INSERT_INST32(0x05d00f93); // addi t6, zero, 93
      INSERT_INST32(0xfdf88ce3); // beq a7, t6, -40
      INSERT_INST32(0x05e00f93); // addi t6, zero, 94
      INSERT_INST32(0xfdf888e3); // beq a7, t6, -48
      INSERT_INST32(nextInst);   // ecall

      rvisor_stub_code += 44;

      INSERT_ROUTINE((char *)&inline_load, 2);

      rvisor_elf_reader.program_counter += rvisor_elf_reader.pc_increment;
      nextInst = rvisor_get_next_instruction();
      continue;
    }

    if (compressed) {
      INSERT_INST16(nextInst);
    } else {
      INSERT_INST32(nextInst);
    }

    rvisor_elf_reader.program_counter += rvisor_elf_reader.pc_increment;
    nextInst = rvisor_get_next_instruction();
  }
}

#ifdef STUBREGIONS
  int insert_stub_region(int override){
      if((MAX_STUB_DISTANCE - stub_distance_tracker <= 0) || override){
        
        nearest_stub_region = memory + rvisor_memory_index;
        printf("Stub distance is %d\n", stub_distance_tracker);
        printf("INSERTED STUB REGION at %p\n", nearest_stub_region);

        // FOR NOT TAKEN
        INSERT_ROUTINE((char *)&inline_save, 2);
        INSERT_LI((uint64_t)(&context_switch), T6);
        INSERT_INST32(0x000f8067);
        

        // FOR TAKEN
        INSERT_ROUTINE((char *)&inline_save, 2);
        INSERT_LI((uint64_t)(&context_switch_taken), T6);
        INSERT_INST32(0x000f8067);

        stub_distance_tracker = 0;
        num_stub_regions++;
    }
  }
#endif
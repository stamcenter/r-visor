#include "dispatcher.h"

char **rvisor_envp;
char **rvisor_argv;
int rvisor_argc;
int rvisor_trace_linking_enabled = 0;

// void rvisor_register_args(int argc, char **argv, char **envp) {
//   rvisor_argc = argc - 1;
//   rvisor_argv = &argv[1];
//   rvisor_envp = &envp[0];
// }

uint64_t rvisor_initialize_bin(uint64_t *TRAPFRAME) {
  // printf("Allocating first\n");
  // if(rvisor_trace_linking_enabled){
  //   rvisor_allocate_trace(rvisor_elf_reader.elf_header.e_entry, 1);
  // }else{
    rvisor_allocate_bb(rvisor_elf_reader.elf_header.e_entry);
  // }

  rvisor_basic_block *next_bb = find_basic_block(rvisor_elf_reader.elf_header.e_entry);
  int stEnd = 0;

  *(TRAPFRAME + stEnd++) = rvisor_argc;
  // *(TRAPFRAME) = rvisor::rvisorArgc;
  // stEnd++;
  for (int i = 0; i < rvisor_argc; i++) {
    *(TRAPFRAME + stEnd++) = (uint64_t)(rvisor_argv[i]);
  }

  *(TRAPFRAME + stEnd++) = 0;

  // Copying Envp
  int envp_count = 0;
  for (envp_count; rvisor_envp[envp_count] != NULL; envp_count++) {
    *(TRAPFRAME + stEnd++) = (uint64_t)(rvisor_envp[envp_count]);
  }

  *(TRAPFRAME + stEnd++) = 0;
  *(TRAPFRAME + stEnd++) = 0;
  *(TRAPFRAME + stEnd++) = 0;
  *(TRAPFRAME + stEnd++) = 0;
  *(TRAPFRAME + stEnd++) = 0;
  *(TRAPFRAME + stEnd++) = 0;
  *(TRAPFRAME + stEnd++) = 0;

  // printf("Returning to %p\n", next_bb->start_location_in_cache);

  return next_bb->start_location_in_cache;
}

uint64_t rvisor_stub(uint64_t *TRAPFRAME, uint64_t taken, int shouldInstrument,
                     uint64_t last_inst_address) {
  // printf("Stubbing\n");
  rvisor_basic_block *current_bb = find_basic_block(rvisor_current_bb);

  if (current_bb == NULL) {
    printf("Basic block not found\n");
    exit(1);
  }
  RvInst last_inst;
  rvisor_bb_type exit_type;
  
  // printf("Last Inst Address: %lx\n", last_inst_address);

  uint64_t *exit_addr = find_exit(last_inst_address);
  // printf("Exit Address: %lx\n", *exit_addr);

  if(last_inst_address && exit_addr){
    rvisor_elf_reader.program_counter = last_inst_address;
    last_inst = decode_instruction(rvisor_get_next_instruction(), 0);
    last_inst.address = last_inst_address;
    
    // printf("HERE!!!\n");
    
    switch(last_inst.name){
      case BNE:            
      case BEQ:            
      case BLT:            
      case BLTU:            
      case BGE:            
      case BGEU:            
      case C_BEQZ:            
      case C_BNEZ:            
      exit_type = BRANCH;
          break;
      case JAL:
      exit_type = DIRECT_JUMP;
          // taken = 1;
          break;
      case JALR:
      case C_JALR:
      case C_JR:
      exit_type = INDIRECT_JUMP;
      // printf("Indirect Jump\n");
          break;
      case ECALL:
      exit_type = SYSCALL;
          break;
      default:
      exit_type = SEGMENTED;
          break;
    }
  }else{
    last_inst = decode_instruction(current_bb->terminal_inst, 0);
    exit_type = current_bb->type;
    last_inst.address = current_bb->last_addr;
  }

  uint64_t *offset_TRAPFRAME = TRAPFRAME - 1;

  // Executing POST routines
  if(rvisor_bb_routine_post_rt && current_bb->type != SEGMENTED){
    rvisor_bb_routine_post_rt(*current_bb, offset_TRAPFRAME);
  }

  if(rvisor_inst_routine_post_rt){
    rvisor_inst_routine_post_rt(last_inst, offset_TRAPFRAME);
  }

  if(inst_type_routine_count){
    Rvisor_Rt_Routine_Inst typeInst = get_inst_type_routine(last_inst.type);
    if(typeInst){
      typeInst(last_inst, offset_TRAPFRAME);
    }
  }

  


  uint32_t rs1 = last_inst.rs1;
  uint32_t rd = last_inst.rd;
  uint32_t opcode;

  uint64_t next_block_addr;
  int compressed = 0;

  switch (exit_type) {
  case BRANCH:
    opcode = last_inst.opcode;

    if ((opcode & 0b11) != 0b11) {
      compressed = 1;
    }
    if (taken) {
      next_block_addr = last_inst.address + last_inst.imm;
      current_bb->taken_block = 1;
    } else {
      next_block_addr =
          compressed ? last_inst.address + 2 : last_inst.address + 4;
      current_bb->taken_block = 0;
    }
    break;
  case DIRECT_JUMP:
    next_block_addr = last_inst.address + last_inst.imm;
    current_bb->taken_block = 1;

    if (last_inst.type == J_TYPE && rd != ZERO) {
      TRAPFRAME[rd - 1] = last_inst.address + 4;
    }
    break;
  case INDIRECT_JUMP:
    if (last_inst.name == C_JALR || last_inst.name == C_JR) {
      rs1 = last_inst.rd_rs1;
      rd = last_inst.rs2;
      next_block_addr = TRAPFRAME[rs1 - 1];
    } else {
      next_block_addr = TRAPFRAME[rs1 - 1] + last_inst.imm;
    }
    if (rd != ZERO) {
      TRAPFRAME[rd - 1] = last_inst.address + 4;
    }
    break;
  case SEGMENTED:
    next_block_addr = last_inst.address + 4;
    break;
  default:
    break;
  }

  // fprintf(rvisor_logger, "GP: %x\tA0: %x\tA1: %d\tA2: %x\tA3: %lx\n", TRAPFRAME[GP-1], TRAPFRAME[A1-1], TRAPFRAME[A2-1], TRAPFRAME[A3-1]);

  rvisor_basic_block *next_bb = find_basic_block(next_block_addr);
  
  if (next_bb == NULL) {
    // printf("ALLOCATING BB at: %x\n", next_block_addr);
    if(rvisor_trace_linking_enabled){
      // printf("Allocating trace at %x\n", next_block_addr);
      rvisor_allocate_trace(next_block_addr, 10);
      // printf("Allocated trace\n");
    }else{
      rvisor_allocate_bb(next_block_addr);
    }

    next_bb = find_basic_block(next_block_addr);

    if (next_bb == NULL) {
        // printf("rd: %d, rs1: %d\n", rd, rs1);
      printf("Basic block not found %d\n", next_block_addr);
      exit(1);
    }
  }

  if(exit_type == BRANCH && exit_addr){
    if(!taken){
      int target = (long int)(next_bb->start_location_in_cache+4) - (*exit_addr+8);
      uint64_t patchedInst = encode_Jtype(JAL_OP, ZERO, target);
       *(int *)(*exit_addr+4) = 0x02010113; // addi sp, sp, 32
       *(int *)(*exit_addr+8) = patchedInst;
    }else{
      int target = (long int)(next_bb->start_location_in_cache+4) - (*exit_addr+40);
      uint64_t patchedInst = encode_Jtype(JAL_OP, ZERO, target);
       *(int *)(*exit_addr+36) = 0x02010113; // addi sp, sp, 32
       *(int *)(*exit_addr+40) = patchedInst;
    }
  }


  if (current_bb->type == SEGMENTED) {
    next_bb->basic_block_address = current_bb->basic_block_address;
  } else {
    next_bb->basic_block_address = current_bb->first_addr;
  }

  rvisor_current_bb = next_bb->first_addr;

  // printf("Returning to %p at %p\n", next_bb->first_addr, next_bb->start_location_in_cache);

  // Executing PRE Routines
  if(rvisor_bb_routine_pre_rt && next_bb->type != SEGMENTED){
    rvisor_bb_routine_pre_rt(*next_bb, offset_TRAPFRAME);
  }

  if(rvisor_inst_routine_pre_rt){
    RvInst first_inst = decode_instruction(next_bb->start_inst, 0);
    rvisor_inst_routine_pre_rt(first_inst, offset_TRAPFRAME);
  }


  return next_bb->start_location_in_cache;

}

uint64_t rvisor_instrument(uint64_t *TRAPFRAME, uint64_t taken) {}

void rvisor_exit_binary(uint64_t *TRAPFRAME) {
  uint64_t *offset_TRAPFRAME = TRAPFRAME - 1;
  if(rvisor_exit_routine)rvisor_exit_routine(offset_TRAPFRAME);
  exit(0);
}
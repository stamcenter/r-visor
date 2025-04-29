#include "rail.h"

Inst_Type_Routine_Map *rvisor_inst_type_routine_map = NULL;
Inst_Group_Alloc_Routine_Map *rvisor_inst_group_routine_map = NULL;

Rvisor_Alloc_Routine_BB rvisor_bb_routine_post_alloc = NULL;
Rvisor_Alloc_Routine_BB rvisor_bb_routine_pre_alloc = NULL;

Rvisor_Alloc_Routine_Inst rvisor_inst_routine_post_alloc = NULL;
Rvisor_Alloc_Routine_Inst rvisor_inst_routine_pre_alloc = NULL;

Rvisor_Rt_Routine_BB rvisor_bb_routine_post_rt = NULL;
Rvisor_Rt_Routine_BB rvisor_bb_routine_pre_rt = NULL;

Rvisor_Rt_Routine_Inst rvisor_inst_routine_post_rt = NULL;
Rvisor_Rt_Routine_Inst rvisor_inst_routine_pre_rt = NULL;

Rvisor_Exit_Routine rvisor_exit_routine = NULL;

rvisor_vec rvisor_inline_inst_bb_vec_pre;
rvisor_vec rvisor_inline_inst_inst_vec_pre;

rvisor_vec rvisor_inline_inst_bb_vec_post;
rvisor_vec rvisor_inline_inst_inst_vec_post;

void rvisor_register_args(int argc, char **argv, char **envp) {
  rvisor_argc = argc - 1;
  rvisor_argv = &argv[1];
  rvisor_envp = &envp[0];
}

void rvisor_init(char *target) {
  // Setting up memory regions for binary
  char *dataBuffer = (char *)(mmap((void *)(0x10000), // address
                                   CACHE_SIZE,        // size = 4MB
                                   PROT_READ | PROT_WRITE | PROT_EXEC,
                                   MAP_ANONYMOUS | MAP_PRIVATE,
                                   -1, // fd (not used here)
                                   0));
  if (dataBuffer == MAP_FAILED) {
    perror("mmap for dataBuffer failed");
    exit(1);
  }

  cache_init();

  char *binBrk = (char *)(mmap((void *)(0x10000000), // address
                               CACHE_SIZE,           // size = 4MB
                               PROT_READ | PROT_WRITE | PROT_EXEC,
                               MAP_PRIVATE | MAP_ANONYMOUS,
                               -1, // fd (not used here)
                               0));

  if (binBrk == MAP_FAILED) {
    perror("mmap for memory failed");
    exit(1);
  }
  rvisor_init_rvisor_elf_reader(target);

  int bound = 0;
  int inst_group_routine_count = 0;
  int pc_increment = 4;

  rvisor_get_text_section();

  rvisor_get_data_sections(dataBuffer, &bound);

  rvisor_vec_init(&rvisor_inline_inst_bb_vec_pre, 16);   // Start with capacity 10
  rvisor_vec_init(&rvisor_inline_inst_inst_vec_pre, 16); // Start with capacity 10

  rvisor_vec_init(&rvisor_inline_inst_bb_vec_post, 16);   // Start with capacity 10
  rvisor_vec_init(&rvisor_inline_inst_inst_vec_post, 16); // Start with capacity 10
}

void rvisor_run() {
  int addr = memory;
  addr +=rvisor_memory_index;
  // printf("ADDR is %x\n", addr);

  rvisor_allocate_root();

  int (*fn)(int) = (int (*)(int))(addr);
  int retval = fn(CACHE_SIZE);
}

void rvisor_register_exit_routine(void (*func)()) {
  rvisor_exit_routine = func;
}

void rvisor_register_inst_routine(void (*func)(), Rvisor_Ipoint ipoint,
                                Rvisor_Invoke invoke) {
  if (invoke == ALLOCATOR) {
    if (ipoint == PRE) {
      rvisor_inst_routine_pre_alloc = func;
    } else if (ipoint == POST) {
      rvisor_inst_routine_post_alloc = func;
    }
  } else if (invoke == RUNTIME) {
    if (ipoint == PRE) {
      rvisor_inst_routine_pre_rt = func;
    } else if (ipoint == POST) {
      rvisor_inst_routine_post_rt = func;
    }
  }
}

void rvisor_register_bb_routine(void (*func)(), Rvisor_Ipoint ipoint,
                                Rvisor_Invoke invoke) {
  if (invoke == ALLOCATOR) {
    if (ipoint == PRE) {
      rvisor_bb_routine_pre_alloc = func;
    } else if (ipoint == POST) {
      rvisor_bb_routine_post_alloc = func;
    }
  } else if (invoke == RUNTIME) {
    if (ipoint == PRE) {
      rvisor_bb_routine_pre_rt = func;
    } else if (ipoint == POST) {
      rvisor_bb_routine_post_rt = func;
    }
  }
}


void add_inst_type_routine(InstType key, Rvisor_Rt_Routine_Inst routine) {
  Inst_Type_Routine_Map *s;

  HASH_FIND_INT(rvisor_inst_type_routine_map, &key, s);
  if (s == NULL) {
    s = (Inst_Type_Routine_Map *)malloc(sizeof *s);
    s->key = key;
    HASH_ADD_INT(rvisor_inst_type_routine_map, key, s);
  }
  s->routine = routine;
}

Rvisor_Rt_Routine_Inst get_inst_type_routine(InstType key) {
  Inst_Type_Routine_Map *s;

  HASH_FIND_INT(rvisor_inst_type_routine_map, &key, s);
  if (s == NULL) {
    return NULL;
  }
  return s->routine;
}

void add_inst_group_routine(RvInstGroup key,
                            Rvisor_Alloc_Routine_Inst routine) {
  Inst_Type_Routine_Map *s;

  HASH_FIND_INT(rvisor_inst_type_routine_map, &key, s);
  if (s == NULL) {
    s = (Inst_Type_Routine_Map *)malloc(sizeof *s);
    s->key = key;
    HASH_ADD_INT(rvisor_inst_type_routine_map, key, s);
  }
  s->routine = routine;
}

Rvisor_Alloc_Routine_Inst get_inst_group_routine(RvInstGroup key) {
  Inst_Type_Routine_Map *s;

  HASH_FIND_INT(rvisor_inst_type_routine_map, &key, s);
  if (s == NULL) {
    return NULL;
  }
  return s->routine;
}

void rvisor_register_inst_type_routine(InstType type, void (*func)(), Rvisor_Ipoint ipoint, Rvisor_Invoke invoke){
  if(invoke == ALLOCATOR){
    if(ipoint == PRE){
      printf("POST ALLOCATOR TYPE routines not implemented\n");
    }else if(ipoint == POST){
      printf("PRE ALLOCATOR TYPE routines not implemented\n");
    }
  }else if(invoke == RUNTIME){
    if(ipoint == PRE){
      printf("PRE RUNTIME TYPE routines not implemented\n");
    }else if(ipoint == POST){
      add_inst_type_routine(type, func);
      inst_type_routine_count++;
    }
  }
}

void rvisor_register_inst_group_routine(RvInstGroup group, void (*func)(), Rvisor_Ipoint ipoint, Rvisor_Invoke invoke){
  if(invoke == ALLOCATOR){
    if(ipoint == PRE){
      printf("PRE ALLOCATOR GROUP routines not implemented\n");
    }else if(ipoint == POST){
      add_inst_group_routine(group, func);
      inst_group_routine_count++;
    }
  }else if(invoke == RUNTIME){
  if(ipoint == PRE){
    printf("PRE RUNTIME GROUP routines not implemented\n");
  }else if(ipoint == POST){
    printf("POST RUNTIME GROUP routines not implemented\n");
    }
  }
}

void rvisor_add_inline_inst_bb(uint32_t instruction, Rvisor_Ipoint ipoint) {
  if (ipoint == PRE) {
    rvisor_vec_push_back(&rvisor_inline_inst_bb_vec_pre, instruction);
  } else {
    rvisor_vec_push_back(&rvisor_inline_inst_bb_vec_post, instruction);
  }
}

void rvisor_add_inline_inst_inst(uint32_t instruction, Rvisor_Ipoint ipoint) {
  if (ipoint == PRE) {
    rvisor_vec_push_back(&rvisor_inline_inst_inst_vec_pre, instruction);
  } else {
    rvisor_vec_push_back(&rvisor_inline_inst_inst_vec_post, instruction);
  }
}

void rvisor_add_inline_li_inst(uint64_t value, GPregs reg,
                               Rvisor_Ipoint ipoint) {
  uint32_t addr, addr_lsb, addr_msb, lui_inst, addi_inst;
  addr = (uint64_t)(value);
  addr_lsb = addr & 0xfff;
  addr_lsb = addr_lsb >> 1;
  addr_msb = (addr >> 12) & 0xfffff;
  lui_inst = LUI_OP | (reg << 7) | (addr_msb << 12);
  addi_inst =
  OPIMM_OP | (reg << 7) | (0 << 12) | (reg << 15) | (addr_lsb << 20);
  
  if(ipoint == PRE){
    rvisor_vec_push_back(&rvisor_inline_inst_inst_vec_pre, lui_inst);
    rvisor_vec_push_back(&rvisor_inline_inst_inst_vec_pre, addi_inst);
    rvisor_vec_push_back(&rvisor_inline_inst_inst_vec_pre, addi_inst);
    }else{
        rvisor_vec_push_back(&rvisor_inline_inst_inst_vec_post, lui_inst);
        rvisor_vec_push_back(&rvisor_inline_inst_inst_vec_post, addi_inst);
        rvisor_vec_push_back(&rvisor_inline_inst_inst_vec_post, addi_inst);
}
}

void rvisor_add_inline_li_bb(uint64_t value, GPregs reg, Rvisor_Ipoint ipoint) {
  uint32_t addr, addr_lsb, addr_msb, lui_inst, addi_inst;
  addr = (uint64_t)(value);
  addr_lsb = addr & 0xfff;
  addr_lsb = addr_lsb >> 1;
  addr_msb = (addr >> 12) & 0xfffff;
  lui_inst = LUI_OP | (reg << 7) | (addr_msb << 12);
  addi_inst =
  OPIMM_OP | (reg << 7) | (0 << 12) | (reg << 15) | (addr_lsb << 20);
  
  if(ipoint == PRE){
    rvisor_vec_push_back(&rvisor_inline_inst_bb_vec_pre, lui_inst);
    rvisor_vec_push_back(&rvisor_inline_inst_bb_vec_pre, addi_inst);
    rvisor_vec_push_back(&rvisor_inline_inst_bb_vec_pre, addi_inst);
}else{
    rvisor_vec_push_back(&rvisor_inline_inst_bb_vec_post, lui_inst);
    rvisor_vec_push_back(&rvisor_inline_inst_bb_vec_post, addi_inst);
    rvisor_vec_push_back(&rvisor_inline_inst_bb_vec_post, addi_inst);
}
}

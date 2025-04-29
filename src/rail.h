#ifndef RAIL_H
#define RAIL_H

#include <stdint.h>
#include <sys/mman.h>

#include "dispatcher.h"
#include "code_cache.h"
#include "elf_reader.h"
#include "railBasicBlock.h"
#include "../helpers/uthash.h"
#include "../helpers/vector.h"

typedef enum{
    PRE,
    POST
}Rvisor_Ipoint;

typedef enum{
    ALLOCATOR,
    RUNTIME
}Rvisor_Invoke;

int inst_group_routine_count;
int inst_type_routine_count;

typedef void (*Rvisor_Rt_Routine_Inst)(RvInst, uint64_t*);
typedef int (*Rvisor_Alloc_Routine_Inst)(RvInst);

typedef void (*Rvisor_Rt_Routine_BB)(rvisor_basic_block, uint64_t*);
typedef void (*Rvisor_Alloc_Routine_BB)();

typedef void (*Rvisor_Exit_Routine)(uint64_t*);

extern Rvisor_Alloc_Routine_BB rvisor_bb_routine_post_alloc;
extern Rvisor_Alloc_Routine_BB rvisor_bb_routine_pre_alloc;

extern Rvisor_Alloc_Routine_Inst rvisor_inst_routine_post_alloc;
extern Rvisor_Alloc_Routine_Inst rvisor_inst_routine_pre_alloc;

extern Rvisor_Rt_Routine_BB rvisor_bb_routine_post_rt;
extern Rvisor_Rt_Routine_BB rvisor_bb_routine_pre_rt;

extern Rvisor_Rt_Routine_Inst rvisor_inst_routine_post_rt;
extern Rvisor_Rt_Routine_Inst rvisor_inst_routine_pre_rt;

extern Rvisor_Exit_Routine rvisor_exit_routine;

extern rvisor_vec rvisor_inline_inst_bb_vec_pre;
extern rvisor_vec rvisor_inline_inst_inst_vec_pre;

extern rvisor_vec rvisor_inline_inst_bb_vec_post;
extern rvisor_vec rvisor_inline_inst_inst_vec_post;


typedef struct {
    InstType key;
    Rvisor_Rt_Routine_Inst routine;
    UT_hash_handle hh;
} Inst_Type_Routine_Map;

extern Inst_Type_Routine_Map *rvisor_inst_type_routine_map;

void add_inst_type_routine(InstType key, Rvisor_Rt_Routine_Inst routine);
Rvisor_Rt_Routine_Inst get_inst_type_routine(InstType key);

typedef struct {
    InstType key;
    Rvisor_Alloc_Routine_Inst routine;
    UT_hash_handle hh;
} Inst_Group_Alloc_Routine_Map;

Inst_Group_Alloc_Routine_Map *rvisor_inst_group_routine_map;

void add_inst_group_routine(RvInstGroup key, Rvisor_Alloc_Routine_Inst routine);
Rvisor_Alloc_Routine_Inst get_inst_group_routine(RvInstGroup key);


void rvisor_register_args(int argc, char **argv, char **envp);

void rvisor_init(char *target);

void rvisor_run();

void rvisor_register_exit_routine(void (*func)());

void rvisor_register_bb_routine(void (*func)(), Rvisor_Ipoint ipoint, Rvisor_Invoke invoke); // 1 routine max

void rvisor_register_inst_routine(void (*func)(), Rvisor_Ipoint ipoint, Rvisor_Invoke invoke); // 1 routine max

void rvisor_register_inst_type_routine(InstType type, void (*func)(), Rvisor_Ipoint ipoint, Rvisor_Invoke invoke); // 1 routine per type

void rvisor_register_inst_group_routine(RvInstGroup group, void (*func)(), Rvisor_Ipoint ipoint, Rvisor_Invoke invoke); // 1 routine per group

void rvisor_add_inline_inst_bb(uint32_t instruction, Rvisor_Ipoint ipoint); // push to vector
void rvisor_add_inline_inst_inst(uint32_t instruction, Rvisor_Ipoint ipoint); // push to vector

void rvisor_add_inline_li_inst(uint64_t value, GPregs reg, Rvisor_Ipoint ipoint); // push to vector
void rvisor_add_inline_li_bb(uint64_t value, GPregs reg, Rvisor_Ipoint ipoint); // push to vector

#endif
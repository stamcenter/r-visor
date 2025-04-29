#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "../helpers/uthash.h"
#include "../src/code_cache.h"
#include "../src/decode.h"
#include "../src/elf_reader.h"
#include "../src/logger.h"
#include "../src/railBasicBlock.h"
#include "../src/rail.h"

int num_instructions;
int num_basic_blocks;
#define CACHE_SIZE (4 * 1024 * 1024)

void bb_counting(rvisor_basic_block bb, uint64_t *regs){
  num_basic_blocks++;
}

void exitFxn(uint64_t *regfile){
    // fprintf( rvisor_logger, "Number of Basic Blocks: %d\n", num_basic_blocks);
    int *bbaddr = (int *)(0x5000000);
    fprintf( rvisor_logger, "Number of Basic Blocks: %d\n", *bbaddr);
    fclose(rvisor_logger);
}

int main(int argc, char **argv, char **envp) {
  set_logging_file("tlrvisor_bb_logs.txt", "w");

  if (argc < 2) {
    printf("Please provide a target binary\n");
    exit(1);
  }

  rvisor_trace_linking_enabled = 1;
  rvisor_init(argv[1]);

  *(int *)(memory + rvisor_memory_index) = 0;
  rvisor_memory_index +=4;  

  rvisor_register_args(argc, argv, envp);

  rvisor_add_inline_inst_bb(encode_ADDI(SP, SP, -16), POST);
  rvisor_add_inline_inst_bb(encode_SD(0, SP, T6), POST);
  rvisor_add_inline_inst_bb(encode_SD(8, SP, T5), POST);
//   rvisor_add_inline_li_bb((uint64_t)(&num_basic_blocks), T6, POST);
  rvisor_add_inline_inst_bb(0x05000fb7, POST); // lui t6, 0x5000
  rvisor_add_inline_inst_bb(0x000fbf03, POST); // ld t5, 0(t6)
  rvisor_add_inline_inst_bb(encode_ADDI(T5, T5, 1), POST); 
  rvisor_add_inline_inst_bb(encode_SD(0, T6, T5), POST);
  rvisor_add_inline_inst_bb(0x00013f83, POST); // ld t6, 0(sp)
  rvisor_add_inline_inst_bb(0x00813f03, POST); // ld t5 8(sp)
  rvisor_add_inline_inst_bb(encode_ADDI(SP, SP, 16), POST);
  

  rvisor_register_exit_routine(exitFxn);

  rvisor_run();

}

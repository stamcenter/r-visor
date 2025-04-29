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

int main(int argc, char **argv, char **envp) {
  set_logging_file("tlrvisor_logs.txt", "w");

  if (argc < 2) {
    printf("Please provide a target binary\n");
    exit(1);
  }

  rvisor_trace_linking_enabled = 1;
  rvisor_init(argv[1]);
  rvisor_register_args(argc, argv, envp);

  rvisor_run();

}

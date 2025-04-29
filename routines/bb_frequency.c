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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct {
    uint64_t key;
    int value;
    UT_hash_handle hh;
} bb_freq_entry;

bb_freq_entry *bb_freqs = NULL;

void exit_routine(uint64_t *regfile) {
    bb_freq_entry *entry, *tmp;
    HASH_ITER(hh, bb_freqs, entry, tmp) {
        fprintf(rvisor_logger, "%x,%d\n", entry->key, entry->value);
        HASH_DEL(bb_freqs, entry);
        free(entry);
    }
}

void bb_frequency(rvisor_basic_block rail_bb, uint64_t *regfile) {
    bb_freq_entry *entry;
    HASH_FIND(hh, bb_freqs, &rail_bb.first_addr, sizeof(uint64_t), entry);
    if (entry) {
        entry->value++;
    } else {
        entry = (bb_freq_entry *)malloc(sizeof(bb_freq_entry));
        if (!entry) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        entry->key = rail_bb.first_addr;
        entry->value = 1;
        HASH_ADD(hh, bb_freqs, key, sizeof(uint64_t), entry);
    }
}

int main(int argc, char **argv, char **envp) {

  if (argc < 2) {
    printf("Please provide a target binary\n");
    exit(1);
  }

  rvisor_trace_linking_enabled = 0;
  rvisor_init(argv[1]);
  rvisor_register_args(argc, argv, envp);
  set_logging_file("bb_frequency_logs.txt", "w");

  fprintf(rvisor_logger,"basic_block,frequency\n");

  rvisor_register_bb_routine(bb_frequency, POST, RUNTIME);
  
  rvisor_register_exit_routine(exit_routine);

  rvisor_run();

}

#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <signal.h>

// extern "C"{
int testfunc(int num);
extern void context_switch();
extern void context_switch_taken();
extern void context_switch_trace_exit();
extern void context_switch_trace_exit_taken();
extern void context_switch_instrument();
extern void inline_save();
extern void inline_save_T5();
extern void inline_load();
extern void inline_load_T5();
extern void restore_ra();
extern void restore_scratch();
extern void run_instrument();
extern void init_switch();
extern void exit_context_switch();
// }
#endif
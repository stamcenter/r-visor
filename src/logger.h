#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

extern FILE *rvisor_logger;

void set_logging_file(const char *filename, const char *mode);
void closeFile();

#endif
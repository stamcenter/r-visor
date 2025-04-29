#include "logger.h"

FILE *rvisor_logger = NULL;  // Define the global file pointer

void set_logging_file(const char *filename, const char *mode) {
    rvisor_logger = fopen(filename, mode);
    if (!rvisor_logger) {
        perror("Failed to open file");
    }
}

void closeFile() {
    if (rvisor_logger) {
        fclose(rvisor_logger);
        rvisor_logger = NULL;
    }
}

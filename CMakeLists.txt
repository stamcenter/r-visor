cmake_minimum_required(VERSION 3.13.4)

project("riscv-lifter")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the source directory
set(SRCDIR src)
set(HELPERDIR helpers)
set(ROUTINESDIR routines)

enable_language(C ASM)

# Add a custom target to run the shell command
add_custom_target(
    parse
    COMMAND ${CMAKE_COMMAND} -E chdir ${SRCDIR} python3 parse.py decodeFile.rdec
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running parse.py on decodeFile.rdec"
)

add_custom_target(
    parse-dbt
    COMMAND ${CMAKE_COMMAND} -E chdir ${SRCDIR} python3 parse.py decodeFileDBT.rdec
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running parse.py on decodeFileDBT.rdec"
)

# Define the cross-compiler and related flags for RISC-V
set(CMAKE_C_COMPILER riscv64-unknown-linux-gnu-gcc)
set(CMAKE_ASM_COMPILER riscv64-unknown-linux-gnu-as)
set(CMAKE_CXX_COMPILER riscv64-unknown-linux-gnu-g++)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR riscv64)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -march=rv64imafd -mabi=lp64d -mno-relax")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=rv64imafd -mabi=lp64d -mno-relax")

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -g -STUBREGIONS -march=rv64imafd -mabi=lp64d -mno-relax")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -STUBREGIONS -march=rv64imafd -mabi=lp64d -mno-relax")


file(GLOB HEADER_FILES ${SRCDIR}/*.h ${SRCDIR}/*.c ${SRCDIR}/*.cpp ${SRCDIR}/*.S ${HELPERDIR}/*.h ${HELPERDIR}/*.c ${HELPERDIR}/*.cpp ${HELPERDIR}/*.S)

# Set static linking
set(CMAKE_EXE_LINKER_FLAGS " -static -Wl,-Ttext-segment=0x2000000")

# Set the output directory for executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

add_executable(tlrvisor ${ROUTINESDIR}/tlrvisor.c ${HEADER_FILES})
add_executable(tlrvisor_bb ${ROUTINESDIR}/tlrvisor_bb.c ${HEADER_FILES})

# add_executable(bb_frequency ${ROUTINESDIR}/bb_frequency.c ${HEADER_FILES})
# add_executable(dbt ${ROUTINESDIR}/dbt.c ${HEADER_FILES})

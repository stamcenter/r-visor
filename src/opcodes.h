#ifndef OPCODES_H
#define OPCODES_H

// INSTRUCTION TYPES: BEGINS
// R Type
#define OP_OP          0b0110011
#define OP_32_OP       0b0111011
#define AMO_OP         0b0101111

// I Type
#define OPIMM_OP       0b0010011
#define OPIMM_32_OP    0b0011011
#define JALR_OP        0b1100111
#define LOAD_OP        0b0000011
#define MISC_MEM_OP    0b0001111
#define SYSTEM_OP      0b1110011

// S Type
#define STORE_OP       0b0100011

// U Type
#define AUIPC_OP       0b0010111
#define LUI_OP         0b0110111

// B Type
#define BRANCH_OP      0b1100011

// J Type
#define JAL_OP         0b1101111

// RV32F
#define LOAD_FP_OP     0b0000111
#define STORE_FP_OP    0b0100111

#define MADD_OP        0b1000011
#define MSUB_OP        0b1000111

#define NMADD_OP       0b1001011
#define NMSUB_OP       0b1001111

#define OP_FP_OP       0b1010011 

// INSTRUCTION TYPES: ENDS

// OPFUNCT for COMPRESSED
// 00_000       = c.addi4spn    = CIW
// 00_010       = c.lw          = CL
// 00_110       = c.sw          = CS
// 
// 01_000       = c.nop         = CI
// 01_000       = c.addi        = CI
// 01_001       = c.jal         = CJ
// 01_010       = c.li          = CI
// 
// 01_011       = c.lui         = CI
// 01_011       = c.addi16sp    = CI
// 
// 01_100x00    = c.srli        = CB
// 01_100x01    = c.srai        = CB
// 01_100x10    = c.andi        = CB
// 01_10001111  = c.and         = CS
// 01_10001110  = c.or          = CS
// 01_10001101  = c.xor         = CS
// 01_10001100  = c.sub         = CS
// 
// 01_101       = c.j           = CJ
// 01_110       = c.beqz        = CB
// 01_111       = c.bnez        = CB
// 
// 10_0000      = c.mv          = CR
// 10_000       = c.slli        = CI
// 10_010       = c.lwsp        = CI
// 
// 10_1000      = c.jr          = CR
// 10_1001      = c.add         = CR
// 10_1001      = c.jalr        = CR
// 10_1001      = c.ebreak      = CR
// 
// 10_110       = c.swsp        = CSS

#endif
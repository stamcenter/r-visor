#ifndef PRINTUTILS_H
#define PRINTUTILS_H

#include <stdio.h>
#include <stdint.h>

#include "instructions.h"
#include "regfile.h"
#include "opcodes.h"
#include "logger.h"

/**
 * Generic function to print decoded RISC-V instructions
 * @param inst RvInst object containing the instruction
*/
void printInstruction(RvInst inst);


/**
 * Prints a RISC-V instruction whose opcode is under R-Type
 * @param inst RvInst object containing the instruction
*/
void printRType(RvInst r_type);

/**
 * Prints a RISC-V instruction whose opcode is under I-Type
 * @param inst RvInst object containing the instruction
*/
void printIType(RvInst i_type);

/**
 * Prints a RISC-V instruction whose opcode is under S-Type
 * @param inst RvInst object containing the instruction
*/
void printSType(RvInst s_type);

/**
 * Prints a RISC-V instruction whose opcode is under U-Type
 * @param inst RvInst object containing the instruction
*/
void printUType(RvInst u_type);

/**
 * Prints a RISC-V instruction whose opcode is under B-Type
 * @param inst RvInst object containing the instruction
*/
void printBType(RvInst b_type);

/**
 * Prints a RISC-V instruction whose opcode is under J-Type
 * @param inst RvInst object containing the instruction
*/
void printJType(RvInst j_type);

void printR4Type(RvInst r4_type);

void printCRType(RvInst cr_type);
void printCIType(RvInst ci_type);
void printCSSType(RvInst css_type);
void printCIWType(RvInst ciw_type);
void printCLType(RvInst cl_type);
void printCSType(RvInst cs_type);
void printCBType(RvInst cb_type);
void printCJType(RvInst cj_type);
void printCRIType(RvInst cl_type);

/**
 * Prints hexadecimal values for each component of an R-Type Instruction
 * @param inst RvInst object containing the instruction
*/
void printRType(RvInst r_type);

/**
 * Prints hexadecimal values for each component of an I-Type Instruction
 * @param inst RvInst object containing the instruction
*/
void printIType(RvInst i_type);

/**
 * Prints hexadecimal values for each component of an S-Type Instruction
 * @param inst RvInst object containing the instruction
*/
void printSType(RvInst s_type);

/**
 * Prints hexadecimal values for each component of an U-Type Instruction
 * @param inst RvInst object containing the instruction
*/
void printUType(RvInst u_type);

/**
 * Prints hexadecimal values for each component of an B-Type Instruction
 * @param inst RvInst object containing the instruction
*/
void printBType(RvInst b_type);

/**
 * Prints hexadecimal values for each component of an J-Type Instruction
 * @param inst RvInst object containing the instruction
*/
void printJType(RvInst j_type);

#endif
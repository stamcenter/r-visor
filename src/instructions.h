#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <stdint.h>
#include "instruction_names.h"
typedef enum {R_TYPE,R4_TYPE,RAMO_TYPE,I_TYPE,S_TYPE,U_TYPE,B_TYPE,J_TYPE,CR_TYPE,CRI_TYPE,CI_TYPE,CIS_TYPE,CISD_TYPE,CI16_TYPE,CSS_TYPE,CSSD_TYPE,CIW_TYPE,CL_TYPE,CLD_TYPE,CS_TYPE,CSD_TYPE,CB_TYPE,CBI_TYPE,CJ_TYPE,UNKNOWN_TYPE} InstType;

static InstType getOpcodeType(int opcode){
switch(opcode){
case 0b0110111: return U_TYPE; 
case 0b0010111: return U_TYPE; 
case 0b1101111: return J_TYPE; 
case 0b1100111: return I_TYPE; 
case 0b1100011: return B_TYPE; 
case 0b0000011: return I_TYPE; 
case 0b0100011: return S_TYPE; 
case 0b0010011: return I_TYPE; 
case 0b0110011: return R_TYPE; 
case 0b0001111: return I_TYPE; 
case 0b1110011: return I_TYPE; 
case 0b0011011: return I_TYPE; 
case 0b0111011: return R_TYPE; 
case 0b0101111: return RAMO_TYPE; 
case 0b0000111: return I_TYPE; 
case 0b0100111: return S_TYPE; 
case 0b1000011: return R4_TYPE; 
case 0b1000111: return R4_TYPE; 
case 0b1001011: return R4_TYPE; 
case 0b1001111: return R4_TYPE; 
case 0b1010011: return R_TYPE; 
default: return UNKNOWN_TYPE;
}
};

static InstType getOpfunctType (int funct){
switch(funct){
case 0b00000: return CIW_TYPE; 
case 0b00001: return CLD_TYPE; 
case 0b00010: return CL_TYPE; 
case 0b00011: return CL_TYPE; 
case 0b00101: return CSD_TYPE; 
case 0b00110: return CS_TYPE; 
case 0b00111: return CSD_TYPE; 
case 0b01000: return CI_TYPE; 
case 0b01001: return CI_TYPE; 
case 0b01010: return CI_TYPE; 
case 0b01011: return CI_TYPE; 
case 0b01101: return CJ_TYPE; 
case 0b01110: return CB_TYPE; 
case 0b01111: return CB_TYPE; 
case 0b10000: return CI_TYPE; 
case 0b10001: return CISD_TYPE; 
case 0b10010: return CIS_TYPE; 
case 0b10011: return CISD_TYPE; 
case 0b10100: return CR_TYPE; 
case 0b10101: return CSSD_TYPE; 
case 0b10110: return CSS_TYPE; 
case 0b10111: return CSSD_TYPE; 
case 0b01100: return CRI_TYPE; 
default: return UNKNOWN_TYPE;
}
};

typedef struct{
	int32_t funct6;
	int32_t rs1;
	int32_t op;
	int32_t rd_rs1p;
	int32_t aq;
	int32_t funct4;
	int32_t funct7;
	int32_t rdp;
	int32_t rs3;
	int32_t rd;
	int32_t funct2;
	int32_t rl;
	int32_t rd_rs1;
	int32_t rs2;
	int32_t imm;
	int32_t opcode;
	int32_t rs2p;
	int32_t funct3;
	int32_t funct5;
	int32_t rs1p;
	uint64_t group;
	uint64_t address;
	InstName name;
	InstType type;
}RvInst;

typedef enum{
	NO_GROUP,
	DBT_GROUP,
}RvInstGroup;

#endif
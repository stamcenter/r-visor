#ifndef ENCODE_H
#define ENCODE_H
#include <stdio.h>
#include <stdint.h>
#include "decode.h"
#include "instruction_names.h"
uint32_t inline encode_Rtype(int opcode, int rd, int funct3, int rs1, int rs2, int funct7){
uint32_t instruction = ((opcode & 0b1111111) << 0) | ((rd & 0b11111) << 7) | ((funct3 & 0b111) << 12) | ((rs1 & 0b11111) << 15) | ((rs2 & 0b11111) << 20) | ((funct7 & 0b1111111) << 25) | 0;
return instruction;
}

uint32_t inline encode_R4type(int opcode, int rd, int funct3, int rs1, int rs2, int funct2, int rs3){
uint32_t instruction = ((opcode & 0b1111111) << 0) | ((rd & 0b11111) << 7) | ((funct3 & 0b111) << 12) | ((rs1 & 0b11111) << 15) | ((rs2 & 0b11111) << 20) | ((funct2 & 0b11) << 25) | ((rs3 & 0b11111) << 27) | 0;
return instruction;
}

uint32_t inline encode_RAMOtype(int opcode, int rd, int funct3, int rs1, int rs2, int rl, int aq, int funct5){
uint32_t instruction = ((opcode & 0b1111111) << 0) | ((rd & 0b11111) << 7) | ((funct3 & 0b111) << 12) | ((rs1 & 0b11111) << 15) | ((rs2 & 0b11111) << 20) | ((rl & 0b1) << 25) | ((aq & 0b1) << 26) | ((funct5 & 0b11111) << 27) | 0;
return instruction;
}

uint32_t inline encode_Itype(int opcode, int rd, int funct3, int rs1, int imm){
uint32_t instruction = ((opcode & 0b1111111) << 0) | ((rd & 0b11111) << 7) | ((funct3 & 0b111) << 12) | ((rs1 & 0b11111) << 15) | (((imm >> 0) & 0b111111111111) << 20) | 0;
return instruction;
}

uint32_t inline encode_Stype(int opcode, int imm, int funct3, int rs1, int rs2){
uint32_t instruction = ((opcode & 0b1111111) << 0) | (((imm >> 0) & 0b11111) << 7) | (((imm >> 5) & 0b1111111) << 25) | ((funct3 & 0b111) << 12) | ((rs1 & 0b11111) << 15) | ((rs2 & 0b11111) << 20) | 0;
return instruction;
}

uint32_t inline encode_Utype(int opcode, int rd, int imm){
uint32_t instruction = ((opcode & 0b1111111) << 0) | ((rd & 0b11111) << 7) | ((imm & 0b11111111111111111111) << 12) | 0;
return instruction;
}

uint32_t inline encode_Btype(int opcode, int imm, int funct3, int rs1, int rs2){
uint32_t instruction = ((opcode & 0b1111111) << 0) | (((imm >> 11) & 0b1) << 7) | (((imm >> 1) & 0b1111) << 8) | (((imm >> 5) & 0b111111) << 25) | (((imm >> 12) & 0b1) << 31) | ((funct3 & 0b111) << 12) | ((rs1 & 0b11111) << 15) | ((rs2 & 0b11111) << 20) | 0;
return instruction;
}

uint32_t inline encode_Jtype(int opcode, int rd, int imm){
uint32_t instruction = ((opcode & 0b1111111) << 0) | ((rd & 0b11111) << 7) | (((imm >> 12) & 0b11111111) << 12) | (((imm >> 11) & 0b1) << 20) | (((imm >> 1) & 0b1111111111) << 21) | (((imm >> 20) & 0b1) << 31) | 0;
return instruction;
}

uint32_t inline encode_CRtype(int op, int rs2, int rd_rs1, int funct4){
uint32_t instruction = ((op & 0b11) << 0) | ((rs2 & 0b11111) << 2) | ((rd_rs1 & 0b11111) << 7) | ((funct4 & 0b1111) << 12) | 0;
return instruction;
}

uint32_t inline encode_CRItype(int op, int rs2p, int funct2, int rd_rs1p, int funct6){
uint32_t instruction = ((op & 0b11) << 0) | ((rs2p & 0b111) << 2) | ((funct2 & 0b11) << 5) | ((rd_rs1p & 0b111) << 7) | ((funct6 & 0b111111) << 10) | 0;
return instruction;
}

uint32_t inline encode_CItype(int op, int imm, int rd_rs1, int funct3){
uint32_t instruction = ((op & 0b11) << 0) | (((imm >> 0) & 0b11111) << 2) | (((imm >> 5) & 0b1) << 12) | ((rd_rs1 & 0b11111) << 7) | ((funct3 & 0b111) << 13) | 0;
return instruction;
}

uint32_t inline encode_CIStype(int op, int imm, int rd_rs1, int funct3){
uint32_t instruction = ((op & 0b11) << 0) | (((imm >> 6) & 0b11) << 2) | (((imm >> 2) & 0b111) << 4) | (((imm >> 5) & 0b1) << 12) | ((rd_rs1 & 0b11111) << 7) | ((funct3 & 0b111) << 13) | 0;
return instruction;
}

uint32_t inline encode_CISDtype(int op, int imm, int rd_rs1, int funct3){
uint32_t instruction = ((op & 0b11) << 0) | (((imm >> 6) & 0b111) << 2) | (((imm >> 3) & 0b11) << 5) | (((imm >> 5) & 0b1) << 12) | ((rd_rs1 & 0b11111) << 7) | ((funct3 & 0b111) << 13) | 0;
return instruction;
}

uint32_t inline encode_CI16type(int op, int imm, int rd_rs1, int funct3){
uint32_t instruction = ((op & 0b11) << 0) | (((imm >> 5) & 0b1) << 2) | (((imm >> 7) & 0b11) << 3) | (((imm >> 6) & 0b1) << 5) | (((imm >> 4) & 0b1) << 6) | (((imm >> 9) & 0b1) << 12) | ((rd_rs1 & 0b11111) << 7) | ((funct3 & 0b111) << 13) | 0;
return instruction;
}

uint32_t inline encode_CSStype(int op, int rs2, int imm, int funct3){
uint32_t instruction = ((op & 0b11) << 0) | ((rs2 & 0b11111) << 2) | (((imm >> 6) & 0b11) << 7) | (((imm >> 2) & 0b1111) << 9) | ((funct3 & 0b111) << 13) | 0;
return instruction;
}

uint32_t inline encode_CSSDtype(int op, int rs2, int imm, int funct3){
uint32_t instruction = ((op & 0b11) << 0) | ((rs2 & 0b11111) << 2) | (((imm >> 6) & 0b111) << 7) | (((imm >> 3) & 0b111) << 10) | ((funct3 & 0b111) << 13) | 0;
return instruction;
}

uint32_t inline encode_CIWtype(int op, int rdp, int imm, int funct3){
uint32_t instruction = ((op & 0b11) << 0) | ((rdp & 0b111) << 2) | (((imm >> 3) & 0b1) << 5) | (((imm >> 2) & 0b1) << 6) | (((imm >> 6) & 0b1111) << 7) | (((imm >> 4) & 0b11) << 11) | ((funct3 & 0b111) << 13) | 0;
return instruction;
}

uint32_t inline encode_CLtype(int op, int rdp, int imm, int rs1p, int funct3){
uint32_t instruction = ((op & 0b11) << 0) | ((rdp & 0b111) << 2) | (((imm >> 2) & 0b1) << 5) | (((imm >> 6) & 0b1) << 6) | (((imm >> 3) & 0b111) << 10) | ((rs1p & 0b111) << 7) | ((funct3 & 0b111) << 13) | 0;
return instruction;
}

uint32_t inline encode_CLDtype(int op, int rdp, int imm, int rs1p, int funct3){
uint32_t instruction = ((op & 0b11) << 0) | ((rdp & 0b111) << 2) | (((imm >> 6) & 0b11) << 5) | (((imm >> 3) & 0b111) << 10) | ((rs1p & 0b111) << 7) | ((funct3 & 0b111) << 13) | 0;
return instruction;
}

uint32_t inline encode_CStype(int op, int rs2p, int imm, int rs1p, int funct3){
uint32_t instruction = ((op & 0b11) << 0) | ((rs2p & 0b111) << 2) | (((imm >> 2) & 0b1) << 5) | (((imm >> 6) & 0b1) << 6) | (((imm >> 3) & 0b111) << 10) | ((rs1p & 0b111) << 7) | ((funct3 & 0b111) << 13) | 0;
return instruction;
}

uint32_t inline encode_CSDtype(int op, int rs2p, int imm, int rs1p, int funct3){
uint32_t instruction = ((op & 0b11) << 0) | ((rs2p & 0b111) << 2) | (((imm >> 6) & 0b11) << 5) | (((imm >> 3) & 0b111) << 10) | ((rs1p & 0b111) << 7) | ((funct3 & 0b111) << 13) | 0;
return instruction;
}

uint32_t inline encode_CBtype(int op, int imm, int rs1, int funct3){
uint32_t instruction = ((op & 0b11) << 0) | (((imm >> 5) & 0b1) << 2) | (((imm >> 1) & 0b11) << 3) | (((imm >> 6) & 0b11) << 5) | (((imm >> 3) & 0b11) << 10) | (((imm >> 8) & 0b1) << 12) | ((rs1 & 0b111) << 7) | ((funct3 & 0b111) << 13) | 0;
return instruction;
}

uint32_t inline encode_CBItype(int op, int imm, int rd_rs1p, int funct2, int funct3){
uint32_t instruction = ((op & 0b11) << 0) | (((imm >> 0) & 0b11111) << 2) | (((imm >> 5) & 0b1) << 12) | ((rd_rs1p & 0b111) << 7) | ((funct2 & 0b11) << 10) | ((funct3 & 0b111) << 13) | 0;
return instruction;
}

uint32_t inline encode_CJtype(int op, int imm, int funct3){
uint32_t instruction = ((op & 0b11) << 0) | (((imm >> 5) & 0b1) << 2) | (((imm >> 1) & 0b111) << 3) | (((imm >> 7) & 0b1) << 6) | (((imm >> 6) & 0b1) << 7) | (((imm >> 10) & 0b1) << 8) | (((imm >> 8) & 0b11) << 9) | (((imm >> 4) & 0b1) << 11) | (((imm >> 11) & 0b1) << 12) | ((funct3 & 0b111) << 13) | 0;
return instruction;
}


uint32_t inline encode_LUI(int rd, int imm){
	return encode_Utype(0b0110111, rd, imm);
}

uint32_t inline encode_AUIPC(int rd, int imm){
	return encode_Utype(0b0010111, rd, imm);
}

uint32_t inline encode_JAL(int rd, int imm){
	return encode_Jtype(0b1101111, rd, imm);
}

uint32_t inline encode_JALR(int rd, int rs1, int imm){
	return encode_Itype(0b1100111, rd, 0b000, rs1, imm);
}

uint32_t inline encode_BEQ(int imm, int rs1, int rs2){
	return encode_Btype(0b1100011, imm, 0b000, rs1, rs2);
}

uint32_t inline encode_BNE(int imm, int rs1, int rs2){
	return encode_Btype(0b1100011, imm, 0b001, rs1, rs2);
}

uint32_t inline encode_BLT(int imm, int rs1, int rs2){
	return encode_Btype(0b1100011, imm, 0b100, rs1, rs2);
}

uint32_t inline encode_BGE(int imm, int rs1, int rs2){
	return encode_Btype(0b1100011, imm, 0b101, rs1, rs2);
}

uint32_t inline encode_BLTU(int imm, int rs1, int rs2){
	return encode_Btype(0b1100011, imm, 0b110, rs1, rs2);
}

uint32_t inline encode_BGEU(int imm, int rs1, int rs2){
	return encode_Btype(0b1100011, imm, 0b111, rs1, rs2);
}

uint32_t inline encode_LB(int rd, int rs1, int imm){
	return encode_Itype(0b0000011, rd, 0b000, rs1, imm);
}

uint32_t inline encode_LH(int rd, int rs1, int imm){
	return encode_Itype(0b0000011, rd, 0b001, rs1, imm);
}

uint32_t inline encode_LW(int rd, int rs1, int imm){
	return encode_Itype(0b0000011, rd, 0b010, rs1, imm);
}

uint32_t inline encode_LBU(int rd, int rs1, int imm){
	return encode_Itype(0b0000011, rd, 0b100, rs1, imm);
}

uint32_t inline encode_LHU(int rd, int rs1, int imm){
	return encode_Itype(0b0000011, rd, 0b101, rs1, imm);
}

uint32_t inline encode_SB(int imm, int rs1, int rs2){
	return encode_Stype(0b0100011, imm, 0b000, rs1, rs2);
}

uint32_t inline encode_SH(int imm, int rs1, int rs2){
	return encode_Stype(0b0100011, imm, 0b001, rs1, rs2);
}

uint32_t inline encode_SW(int imm, int rs1, int rs2){
	return encode_Stype(0b0100011, imm, 0b010, rs1, rs2);
}

uint32_t inline encode_ADDI(int rd, int rs1, int imm){
	return encode_Itype(0b0010011, rd, 0b000, rs1, imm);
}

uint32_t inline encode_SLTI(int rd, int rs1, int imm){
	return encode_Itype(0b0010011, rd, 0b010, rs1, imm);
}

uint32_t inline encode_SLTIU(int rd, int rs1, int imm){
	return encode_Itype(0b0010011, rd, 0b011, rs1, imm);
}

uint32_t inline encode_XORI(int rd, int rs1, int imm){
	return encode_Itype(0b0010011, rd, 0b100, rs1, imm);
}

uint32_t inline encode_ORI(int rd, int rs1, int imm){
	return encode_Itype(0b0010011, rd, 0b110, rs1, imm);
}

uint32_t inline encode_ANDI(int rd, int rs1, int imm){
	return encode_Itype(0b0010011, rd, 0b111, rs1, imm);
}

uint32_t inline encode_ADD(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b000, rs1, rs2, 0b0000000);
}

uint32_t inline encode_SUB(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b000, rs1, rs2, 0b0100000);
}

uint32_t inline encode_SLL(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b001, rs1, rs2, 0b0000000);
}

uint32_t inline encode_SLT(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b010, rs1, rs2, 0b0000000);
}

uint32_t inline encode_SLTU(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b011, rs1, rs2, 0b0000000);
}

uint32_t inline encode_XOR(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b100, rs1, rs2, 0b0000000);
}

uint32_t inline encode_SRL(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b101, rs1, rs2, 0b0000000);
}

uint32_t inline encode_SRA(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b101, rs1, rs2, 0b0100000);
}

uint32_t inline encode_OR(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b110, rs1, rs2, 0b0000000);
}

uint32_t inline encode_AND(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b111, rs1, rs2, 0b0000000);
}

uint32_t inline encode_FENCE(int rd, int rs1, int imm){
	return encode_Itype(0b0001111, rd, 0b000, rs1, imm);
}

uint32_t inline encode_ECALL(int rd, int rs1){
	return encode_Itype(0b1110011, rd, 0b000, rs1, 0b00000000);
}

uint32_t inline encode_EBREAK(int rd, int rs1){
	return encode_Itype(0b1110011, rd, 0b000, rs1, 0b00000001);
}

uint32_t inline encode_LWU(int rd, int rs1, int imm){
	return encode_Itype(0b0000011, rd, 0b110, rs1, imm);
}

uint32_t inline encode_LD(int rd, int rs1, int imm){
	return encode_Itype(0b0000011, rd, 0b111, rs1, imm);
}

uint32_t inline encode_SD(int imm, int rs1, int rs2){
	return encode_Stype(0b0100011, imm, 0b011, rs1, rs2);
}

uint32_t inline encode_SLLI(int rd, int rs1, int rs2){
	return encode_Rtype(0b0010011, rd, 0b001, rs1, rs2, 0b0000000);
}

uint32_t inline encode_SRLI(int rd, int rs1, int rs2){
	return encode_Rtype(0b0010011, rd, 0b101, rs1, rs2, 0b0000000);
}

uint32_t inline encode_SRAI(int rd, int rs1, int rs2){
	return encode_Rtype(0b0010011, rd, 0b101, rs1, rs2, 0b0100000);
}

uint32_t inline encode_ADDIW(int rd, int rs1, int imm){
	return encode_Itype(0b0011011, rd, 0b000, rs1, imm);
}

uint32_t inline encode_SLLIW(int rd, int rs1, int rs2){
	return encode_Rtype(0b0011011, rd, 0b001, rs1, rs2, 0b0000000);
}

uint32_t inline encode_SRLIW(int rd, int rs1, int rs2){
	return encode_Rtype(0b0011011, rd, 0b101, rs1, rs2, 0b0000000);
}

uint32_t inline encode_SRAIW(int rd, int rs1, int rs2){
	return encode_Rtype(0b0011011, rd, 0b101, rs1, rs2, 0b0100000);
}

uint32_t inline encode_ADDW(int rd, int rs1, int rs2){
	return encode_Rtype(0b0111011, rd, 0b000, rs1, rs2, 0b0000000);
}

uint32_t inline encode_SUBW(int rd, int rs1, int rs2){
	return encode_Rtype(0b0111011, rd, 0b000, rs1, rs2, 0b0100000);
}

uint32_t inline encode_SLLW(int rd, int rs1, int rs2){
	return encode_Rtype(0b0111011, rd, 0b001, rs1, rs2, 0b0000000);
}

uint32_t inline encode_SRLW(int rd, int rs1, int rs2){
	return encode_Rtype(0b0111011, rd, 0b101, rs1, rs2, 0b0000000);
}

uint32_t inline encode_SRAW(int rd, int rs1, int rs2){
	return encode_Rtype(0b0111011, rd, 0b101, rs1, rs2, 0b0100000);
}

uint32_t inline encode_MUL(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b000, rs1, rs2, 0b0000001);
}

uint32_t inline encode_MULH(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b001, rs1, rs2, 0b0000001);
}

uint32_t inline encode_MULHSU(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b010, rs1, rs2, 0b0000001);
}

uint32_t inline encode_MULHU(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b011, rs1, rs2, 0b0000001);
}

uint32_t inline encode_DIV(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b100, rs1, rs2, 0b0000001);
}

uint32_t inline encode_DIVU(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b101, rs1, rs2, 0b0000001);
}

uint32_t inline encode_REM(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b110, rs1, rs2, 0b0000001);
}

uint32_t inline encode_REMU(int rd, int rs1, int rs2){
	return encode_Rtype(0b0110011, rd, 0b111, rs1, rs2, 0b0000001);
}

uint32_t inline encode_MULW(int rd, int rs1, int rs2){
	return encode_Rtype(0b0111011, rd, 0b000, rs1, rs2, 0b0000001);
}

uint32_t inline encode_DIVW(int rd, int rs1, int rs2){
	return encode_Rtype(0b0111011, rd, 0b100, rs1, rs2, 0b0000001);
}

uint32_t inline encode_DIVUW(int rd, int rs1, int rs2){
	return encode_Rtype(0b0111011, rd, 0b101, rs1, rs2, 0b0000001);
}

uint32_t inline encode_REMW(int rd, int rs1, int rs2){
	return encode_Rtype(0b0111011, rd, 0b110, rs1, rs2, 0b0000001);
}

uint32_t inline encode_REMUW(int rd, int rs1, int rs2){
	return encode_Rtype(0b0111011, rd, 0b111, rs1, rs2, 0b0000001);
}

uint32_t inline encode_LR_W(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b010, rs1, rs2, rl, aq, 0b00010);
}

uint32_t inline encode_SC_W(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b010, rs1, rs2, rl, aq, 0b00011);
}

uint32_t inline encode_AMOSWAP_W(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b010, rs1, rs2, rl, aq, 0b00001);
}

uint32_t inline encode_AMOADD_W(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b010, rs1, rs2, rl, aq, 0b00000);
}

uint32_t inline encode_AMOXOR_W(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b010, rs1, rs2, rl, aq, 0b00100);
}

uint32_t inline encode_AMOAND_W(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b010, rs1, rs2, rl, aq, 0b01100);
}

uint32_t inline encode_AMOOR_W(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b010, rs1, rs2, rl, aq, 0b01000);
}

uint32_t inline encode_AMOMIN_W(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b010, rs1, rs2, rl, aq, 0b10000);
}

uint32_t inline encode_AMOMAX_W(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b010, rs1, rs2, rl, aq, 0b10100);
}

uint32_t inline encode_AMOMINU_W(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b010, rs1, rs2, rl, aq, 0b11000);
}

uint32_t inline encode_AMOMAXU_W(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b010, rs1, rs2, rl, aq, 0b11100);
}

uint32_t inline encode_LR_D(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b011, rs1, rs2, rl, aq, 0b00010);
}

uint32_t inline encode_SC_D(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b011, rs1, rs2, rl, aq, 0b00011);
}

uint32_t inline encode_AMOSWAP_D(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b011, rs1, rs2, rl, aq, 0b00001);
}

uint32_t inline encode_AMOADD_D(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b011, rs1, rs2, rl, aq, 0b00000);
}

uint32_t inline encode_AMOXOR_D(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b011, rs1, rs2, rl, aq, 0b00100);
}

uint32_t inline encode_AMOAND_D(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b011, rs1, rs2, rl, aq, 0b01100);
}

uint32_t inline encode_AMOOR_D(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b011, rs1, rs2, rl, aq, 0b01000);
}

uint32_t inline encode_AMOMIN_D(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b011, rs1, rs2, rl, aq, 0b10000);
}

uint32_t inline encode_AMOMAX_D(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b011, rs1, rs2, rl, aq, 0b10100);
}

uint32_t inline encode_AMOMINU_D(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b011, rs1, rs2, rl, aq, 0b11000);
}

uint32_t inline encode_AMOMAXU_D(int rd, int rs1, int rs2, int rl, int aq){
	return encode_RAMOtype(0b0101111, rd, 0b011, rs1, rs2, rl, aq, 0b11100);
}

uint32_t inline encode_FLW(int rd, int rs1, int imm){
	return encode_Itype(0b0000111, rd, 0b010, rs1, imm);
}

uint32_t inline encode_FSW(int imm, int rs1, int rs2){
	return encode_Stype(0b0100111, imm, 0b010, rs1, rs2);
}

uint32_t inline encode_FMADD_S(int rd, int funct3, int rs1, int rs2, int rs3){
	return encode_R4type(0b1000011, rd, funct3, rs1, rs2, 0b00, rs3);
}

uint32_t inline encode_FMSUB_S(int rd, int funct3, int rs1, int rs2, int rs3){
	return encode_R4type(0b1000111, rd, funct3, rs1, rs2, 0b00, rs3);
}

uint32_t inline encode_FNMSUB_S(int rd, int funct3, int rs1, int rs2, int rs3){
	return encode_R4type(0b1001011, rd, funct3, rs1, rs2, 0b00, rs3);
}

uint32_t inline encode_FNMADD_S(int rd, int funct3, int rs1, int rs2, int rs3){
	return encode_R4type(0b1001111, rd, funct3, rs1, rs2, 0b00, rs3);
}

uint32_t inline encode_FADD_S(int rd, int funct3, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, funct3, rs1, rs2, 0b0000000);
}

uint32_t inline encode_FSUB_S(int rd, int funct3, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, funct3, rs1, rs2, 0b0000100);
}

uint32_t inline encode_FMUL_S(int rd, int funct3, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, funct3, rs1, rs2, 0b0001000);
}

uint32_t inline encode_FDIV_S(int rd, int funct3, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, funct3, rs1, rs2, 0b0001100);
}

uint32_t inline encode_FSQRT_S(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00000, 0b0101100);
}

uint32_t inline encode_FSGNJ_S(int rd, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, 0b000, rs1, rs2, 0b0010000);
}

uint32_t inline encode_FSGNJN_S(int rd, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, 0b001, rs1, rs2, 0b0010000);
}

uint32_t inline encode_FSGNJX_S(int rd, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, 0b010, rs1, rs2, 0b0010000);
}

uint32_t inline encode_FMIN_S(int rd, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, 0b000, rs1, rs2, 0b0010100);
}

uint32_t inline encode_FMAX_S(int rd, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, 0b001, rs1, rs2, 0b0010100);
}

uint32_t inline encode_FCVT_W_S(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00000, 0b1100000);
}

uint32_t inline encode_FCVT_WU_S(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00001, 0b1100000);
}

uint32_t inline encode_FMV_X_S(int rd, int rs1){
	return encode_Rtype(0b1010011, rd, 0b000, rs1, 0b00000, 0b1110000);
}

uint32_t inline encode_FEQ_S(int rd, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, 0b010, rs1, rs2, 0b1010000);
}

uint32_t inline encode_FLT_S(int rd, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, 0b001, rs1, rs2, 0b1010000);
}

uint32_t inline encode_FLE_S(int rd, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, 0b000, rs1, rs2, 0b1010000);
}

uint32_t inline encode_FCLASS_S(int rd, int rs1){
	return encode_Rtype(0b1010011, rd, 0b001, rs1, 0b00000, 0b1110000);
}

uint32_t inline encode_FCVT_S_W(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00000, 0b1101000);
}

uint32_t inline encode_FCVT_S_WU(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00001, 0b1101000);
}

uint32_t inline encode_FMV_W_X(int rd, int rs1){
	return encode_Rtype(0b1010011, rd, 0b000, rs1, 0b00000, 0b1111000);
}

uint32_t inline encode_FCVT_L_S(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00010, 0b1100000);
}

uint32_t inline encode_FCVT_LU_S(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00011, 0b1100000);
}

uint32_t inline encode_FCVT_S_L(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00010, 0b1100000);
}

uint32_t inline encode_FCVT_S_LU(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00010, 0b1100000);
}

uint32_t inline encode_FLD(int rd, int rs1, int imm){
	return encode_Itype(0b0000111, rd, 0b011, rs1, imm);
}

uint32_t inline encode_FSD(int imm, int rs1, int rs2){
	return encode_Stype(0b0100111, imm, 0b011, rs1, rs2);
}

uint32_t inline encode_FMADD_D(int rd, int funct3, int rs1, int rs2, int rs3){
	return encode_R4type(0b1000011, rd, funct3, rs1, rs2, 0b01, rs3);
}

uint32_t inline encode_FMSUB_D(int rd, int funct3, int rs1, int rs2, int rs3){
	return encode_R4type(0b1000111, rd, funct3, rs1, rs2, 0b01, rs3);
}

uint32_t inline encode_FNMSUB_D(int rd, int funct3, int rs1, int rs2, int rs3){
	return encode_R4type(0b1001011, rd, funct3, rs1, rs2, 0b01, rs3);
}

uint32_t inline encode_FNMADD_D(int rd, int funct3, int rs1, int rs2, int rs3){
	return encode_R4type(0b1001111, rd, funct3, rs1, rs2, 0b01, rs3);
}

uint32_t inline encode_FADD_D(int rd, int funct3, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, funct3, rs1, rs2, 0b0000001);
}

uint32_t inline encode_FSUB_D(int rd, int funct3, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, funct3, rs1, rs2, 0b0000101);
}

uint32_t inline encode_FMUL_D(int rd, int funct3, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, funct3, rs1, rs2, 0b0001001);
}

uint32_t inline encode_FDIV_D(int rd, int funct3, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, funct3, rs1, rs2, 0b0001101);
}

uint32_t inline encode_FSQRT_D(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00000, 0b0101101);
}

uint32_t inline encode_FSGNJ_D(int rd, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, 0b000, rs1, rs2, 0b0010001);
}

uint32_t inline encode_FSGNJN_D(int rd, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, 0b001, rs1, rs2, 0b0010001);
}

uint32_t inline encode_FSGNJX_D(int rd, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, 0b010, rs1, rs2, 0b0010001);
}

uint32_t inline encode_FMIN_D(int rd, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, 0b000, rs1, rs2, 0b0010101);
}

uint32_t inline encode_FMAX_D(int rd, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, 0b001, rs1, rs2, 0b0010101);
}

uint32_t inline encode_FCVT_S_D(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00001, 0b0100000);
}

uint32_t inline encode_FCVT_D_S(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00000, 0b0100001);
}

uint32_t inline encode_FEQ_D(int rd, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, 0b010, rs1, rs2, 0b1010001);
}

uint32_t inline encode_FLT_D(int rd, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, 0b001, rs1, rs2, 0b1010001);
}

uint32_t inline encode_FLE_D(int rd, int rs1, int rs2){
	return encode_Rtype(0b1010011, rd, 0b000, rs1, rs2, 0b1010001);
}

uint32_t inline encode_FCLASS_D(int rd, int rs1){
	return encode_Rtype(0b1010011, rd, 0b001, rs1, 0b00000, 0b1110001);
}

uint32_t inline encode_FCVT_W_D(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00000, 0b1100001);
}

uint32_t inline encode_FCVT_WU_D(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00001, 0b1100001);
}

uint32_t inline encode_FCVT_D_W(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00000, 0b1101001);
}

uint32_t inline encode_FCVT_D_WU(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00001, 0b1101001);
}

uint32_t inline encode_FCVT_L_D(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00010, 0b1100001);
}

uint32_t inline encode_FCVT_LU_D(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00011, 0b1100001);
}

uint32_t inline encode_FMV_X_D(int rd, int rs1){
	return encode_Rtype(0b1010011, rd, 0b000, rs1, 0b00000, 0b1110001);
}

uint32_t inline encode_FCVT_D_L(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00010, 0b1101001);
}

uint32_t inline encode_FCVT_D_LU(int rd, int funct3, int rs1){
	return encode_Rtype(0b1010011, rd, funct3, rs1, 0b00011, 0b1101001);
}

uint32_t inline encode_FMV_D_X(int rd, int rs1){
	return encode_Rtype(0b1010011, rd, 0b000, rs1, 0b00000, 0b1111001);
}

uint32_t inline encode_C_ADDI4SPN(int rdp, int imm){
	return encode_CIWtype(0b00, rdp, imm, 0b000);
}

uint32_t inline encode_C_FLD(int rdp, int imm, int rs1p){
	return encode_CLDtype(0b00, rdp, imm, rs1p, 0b001);
}

uint32_t inline encode_C_LW(int rdp, int imm, int rs1p){
	return encode_CLtype(0b00, rdp, imm, rs1p, 0b010);
}

uint32_t inline encode_C_LD(int rdp, int imm, int rs1p){
	return encode_CLtype(0b00, rdp, imm, rs1p, 0b011);
}

uint32_t inline encode_C_FSD(int rs2p, int imm, int rs1p){
	return encode_CSDtype(0b00, rs2p, imm, rs1p, 0b101);
}

uint32_t inline encode_C_SW(int rs2p, int imm, int rs1p){
	return encode_CStype(0b00, rs2p, imm, rs1p, 0b110);
}

uint32_t inline encode_C_SD(int rs2p, int imm, int rs1p){
	return encode_CSDtype(0b00, rs2p, imm, rs1p, 0b111);
}

uint32_t inline encode_C_NOP(){
	return encode_CItype(0b01, 0, 0, 0b000);
}

uint32_t inline encode_C_ADDI(int imm, int rd_rs1){
	return encode_CItype(0b01, imm, rd_rs1, 0b000);
}

uint32_t inline encode_C_ADDIW(int imm, int rd_rs1){
	return encode_CItype(0b01, imm, rd_rs1, 0b001);
}

uint32_t inline encode_C_LI(int imm, int rd_rs1){
	return encode_CItype(0b01, imm, rd_rs1, 0b010);
}

uint32_t inline encode_C_ADDI16SP(int imm, int rd_rs1){
	return encode_CItype(0b01, imm, rd_rs1, 0b011);
}

uint32_t inline encode_C_LUI(int imm, int rd_rs1){
	return encode_CItype(0b01, imm, rd_rs1, 0b011);
}

uint32_t inline encode_C_J(int imm){
	return encode_CJtype(0b01, imm, 0b101);
}

uint32_t inline encode_C_BEQZ(int imm, int rs1){
	return encode_CBtype(0b01, imm, rs1, 0b110);
}

uint32_t inline encode_C_BNEZ(int imm, int rs1){
	return encode_CBtype(0b01, imm, rs1, 0b111);
}

uint32_t inline encode_C_SLLI(int imm, int rd_rs1){
	return encode_CItype(0b10, imm, rd_rs1, 0b000);
}

uint32_t inline encode_C_FLDSP(int imm, int rd_rs1){
	return encode_CISDtype(0b10, imm, rd_rs1, 0b001);
}

uint32_t inline encode_C_LWSP(int imm, int rd_rs1){
	return encode_CIStype(0b10, imm, rd_rs1, 0b010);
}

uint32_t inline encode_C_LDSP(int imm, int rd_rs1){
	return encode_CISDtype(0b10, imm, rd_rs1, 0b011);
}

uint32_t inline encode_C_JR(int rd_rs1){
	return encode_CRtype(0b10, 0, rd_rs1, 0b1000);
}

uint32_t inline encode_C_MV(int rs2, int rd_rs1){
	return encode_CRtype(0b10, rs2, rd_rs1, 0b1000);
}

uint32_t inline encode_C_EBREAK(){
	return encode_CRtype(0b10, 0, 0, 0b1001);
}

uint32_t inline encode_C_JALR(int rd_rs1){
	return encode_CRtype(0b10, 0, rd_rs1, 0b1001);
}

uint32_t inline encode_C_ADD(int rs2, int rd_rs1){
	return encode_CRtype(0b10, rs2, rd_rs1, 0b1001);
}

uint32_t inline encode_C_FSDSP(int rs2, int imm){
	return encode_CSSDtype(0b10, rs2, imm, 0b101);
}

uint32_t inline encode_C_SWSP(int rs2, int imm){
	return encode_CSStype(0b10, rs2, imm, 0b110);
}

uint32_t inline encode_C_SDSP(int rs2, int imm){
	return encode_CSSDtype(0b10, rs2, imm, 0b111);
}

uint32_t inline encode_C_SUB(int rs2p, int rd_rs1p){
	return encode_CRItype(0b01, rs2p, 0b00, rd_rs1p, 0b100011);
}

uint32_t inline encode_C_XOR(int rs2p, int rd_rs1p){
	return encode_CRItype(0b01, rs2p, 0b01, rd_rs1p, 0b100011);
}

uint32_t inline encode_C_OR(int rs2p, int rd_rs1p){
	return encode_CRItype(0b01, rs2p, 0b10, rd_rs1p, 0b100011);
}

uint32_t inline encode_C_AND(int rs2p, int rd_rs1p){
	return encode_CRItype(0b01, rs2p, 0b11, rd_rs1p, 0b100011);
}

uint32_t inline encode_C_SUBW(int rs2p, int rd_rs1p){
	return encode_CRItype(0b01, rs2p, 0b00, rd_rs1p, 0b100111);
}

uint32_t inline encode_C_ADDW(int rs2p, int rd_rs1p){
	return encode_CRItype(0b01, rs2p, 0b01, rd_rs1p, 0b100111);
}
#endif


#include "decode.h"

#include "instruction_names.h"
RvInst decode_Rtype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = R_TYPE;
	decodedInst.opcode = (instruction >> 0) & 0b1111111;
	decodedInst.rd = (instruction >> 7) & 0b11111;
	decodedInst.funct3 = (instruction >> 12) & 0b111;
	decodedInst.rs1 = (instruction >> 15) & 0b11111;
	decodedInst.rs2 = (instruction >> 20) & 0b11111;
	decodedInst.funct7 = (instruction >> 25) & 0b1111111;
	return decodedInst;
}

RvInst decode_R4type(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = R4_TYPE;
	decodedInst.opcode = (instruction >> 0) & 0b1111111;
	decodedInst.rd = (instruction >> 7) & 0b11111;
	decodedInst.funct3 = (instruction >> 12) & 0b111;
	decodedInst.rs1 = (instruction >> 15) & 0b11111;
	decodedInst.rs2 = (instruction >> 20) & 0b11111;
	decodedInst.funct2 = (instruction >> 25) & 0b11;
	decodedInst.rs3 = (instruction >> 27) & 0b11111;
	return decodedInst;
}

RvInst decode_RAMOtype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = RAMO_TYPE;
	decodedInst.opcode = (instruction >> 0) & 0b1111111;
	decodedInst.rd = (instruction >> 7) & 0b11111;
	decodedInst.funct3 = (instruction >> 12) & 0b111;
	decodedInst.rs1 = (instruction >> 15) & 0b11111;
	decodedInst.rs2 = (instruction >> 20) & 0b11111;
	decodedInst.rl = (instruction >> 25) & 0b1;
	decodedInst.aq = (instruction >> 26) & 0b1;
	decodedInst.funct5 = (instruction >> 27) & 0b11111;
	return decodedInst;
}

RvInst decode_Itype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = I_TYPE;
	decodedInst.opcode = (instruction >> 0) & 0b1111111;
	decodedInst.rd = (instruction >> 7) & 0b11111;
	decodedInst.funct3 = (instruction >> 12) & 0b111;
	decodedInst.rs1 = (instruction >> 15) & 0b11111;
	int imm_0 = (instruction >> 20) & 0b111111111111;
	int imm = (imm_0 << 0);
	imm = (imm >> 11) ? imm | 0b11111111111111111111100000000000 : imm;
	decodedInst.imm = imm;
	return decodedInst;
}

RvInst decode_Stype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = S_TYPE;
	decodedInst.opcode = (instruction >> 0) & 0b1111111;
	int imm_0 = (instruction >> 7) & 0b11111;
	int imm_5 = (instruction >> 25) & 0b1111111;
	int imm = (imm_0 << 0)|(imm_5 << 5);
	imm = (imm >> 11) ? imm | 0b11111111111111111111100000000000 : imm;
	decodedInst.imm = imm;
	decodedInst.funct3 = (instruction >> 12) & 0b111;
	decodedInst.rs1 = (instruction >> 15) & 0b11111;
	decodedInst.rs2 = (instruction >> 20) & 0b11111;
	return decodedInst;
}

RvInst decode_Utype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = U_TYPE;
	decodedInst.opcode = (instruction >> 0) & 0b1111111;
	decodedInst.rd = (instruction >> 7) & 0b11111;
	decodedInst.imm = (instruction >> 12) & 0b11111111111111111111;
	return decodedInst;
}

RvInst decode_Btype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = B_TYPE;
	decodedInst.opcode = (instruction >> 0) & 0b1111111;
	int imm_11 = (instruction >> 7) & 0b1;
	int imm_1 = (instruction >> 8) & 0b1111;
	int imm_5 = (instruction >> 25) & 0b111111;
	int imm_12 = (instruction >> 31) & 0b1;
	int imm = (imm_11 << 11)|(imm_1 << 1)|(imm_5 << 5)|(imm_12 << 12);
	imm = (imm >> 12) ? imm | 0b11111111111111111111000000000000 : imm;
	decodedInst.imm = imm;
	decodedInst.funct3 = (instruction >> 12) & 0b111;
	decodedInst.rs1 = (instruction >> 15) & 0b11111;
	decodedInst.rs2 = (instruction >> 20) & 0b11111;
	return decodedInst;
}

RvInst decode_Jtype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = J_TYPE;
	decodedInst.opcode = (instruction >> 0) & 0b1111111;
	decodedInst.rd = (instruction >> 7) & 0b11111;
	int imm_12 = (instruction >> 12) & 0b11111111;
	int imm_11 = (instruction >> 20) & 0b1;
	int imm_1 = (instruction >> 21) & 0b1111111111;
	int imm_20 = (instruction >> 31) & 0b1;
	int imm = (imm_12 << 12)|(imm_11 << 11)|(imm_1 << 1)|(imm_20 << 20);
	imm = (imm >> 20) ? imm | 0b11111111111100000000000000000000 : imm;
	decodedInst.imm = imm;
	return decodedInst;
}

RvInst decode_CRtype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = CR_TYPE;
	decodedInst.op = (instruction >> 0) & 0b11;
	decodedInst.rs2 = (instruction >> 2) & 0b11111;
	decodedInst.rd_rs1 = (instruction >> 7) & 0b11111;
	decodedInst.funct4 = (instruction >> 12) & 0b1111;
	return decodedInst;
}

RvInst decode_CRItype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = CRI_TYPE;
	decodedInst.op = (instruction >> 0) & 0b11;
	decodedInst.rs2p = (instruction >> 2) & 0b111;
	decodedInst.funct2 = (instruction >> 5) & 0b11;
	decodedInst.rd_rs1p = (instruction >> 7) & 0b111;
	decodedInst.funct6 = (instruction >> 10) & 0b111111;
	return decodedInst;
}

RvInst decode_CItype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = CI_TYPE;
	decodedInst.op = (instruction >> 0) & 0b11;
	int imm_0 = (instruction >> 2) & 0b11111;
	int imm_5 = (instruction >> 12) & 0b1;
	int imm = (imm_0 << 0)|(imm_5 << 5);
	decodedInst.imm = imm;
	decodedInst.rd_rs1 = (instruction >> 7) & 0b11111;
	decodedInst.funct3 = (instruction >> 13) & 0b111;
	return decodedInst;
}

RvInst decode_CIStype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = CIS_TYPE;
	decodedInst.op = (instruction >> 0) & 0b11;
	int imm_6 = (instruction >> 2) & 0b11;
	int imm_2 = (instruction >> 4) & 0b111;
	int imm_5 = (instruction >> 12) & 0b1;
	int imm = (imm_6 << 6)|(imm_2 << 2)|(imm_5 << 5);
	decodedInst.imm = imm;
	decodedInst.rd_rs1 = (instruction >> 7) & 0b11111;
	decodedInst.funct3 = (instruction >> 13) & 0b111;
	return decodedInst;
}

RvInst decode_CISDtype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = CISD_TYPE;
	decodedInst.op = (instruction >> 0) & 0b11;
	int imm_6 = (instruction >> 2) & 0b111;
	int imm_3 = (instruction >> 5) & 0b11;
	int imm_5 = (instruction >> 12) & 0b1;
	int imm = (imm_6 << 6)|(imm_3 << 3)|(imm_5 << 5);
	decodedInst.imm = imm;
	decodedInst.rd_rs1 = (instruction >> 7) & 0b11111;
	decodedInst.funct3 = (instruction >> 13) & 0b111;
	return decodedInst;
}

RvInst decode_CI16type(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = CI16_TYPE;
	decodedInst.op = (instruction >> 0) & 0b11;
	int imm_5 = (instruction >> 2) & 0b1;
	int imm_7 = (instruction >> 3) & 0b11;
	int imm_6 = (instruction >> 5) & 0b1;
	int imm_4 = (instruction >> 6) & 0b1;
	int imm_9 = (instruction >> 12) & 0b1;
	int imm = (imm_5 << 5)|(imm_7 << 7)|(imm_6 << 6)|(imm_4 << 4)|(imm_9 << 9);
	decodedInst.imm = imm;
	decodedInst.rd_rs1 = (instruction >> 7) & 0b11111;
	decodedInst.funct3 = (instruction >> 13) & 0b111;
	return decodedInst;
}

RvInst decode_CSStype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = CSS_TYPE;
	decodedInst.op = (instruction >> 0) & 0b11;
	decodedInst.rs2 = (instruction >> 2) & 0b11111;
	int imm_6 = (instruction >> 7) & 0b11;
	int imm_2 = (instruction >> 9) & 0b1111;
	int imm = (imm_6 << 6)|(imm_2 << 2);
	decodedInst.imm = imm;
	decodedInst.funct3 = (instruction >> 13) & 0b111;
	return decodedInst;
}

RvInst decode_CSSDtype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = CSSD_TYPE;
	decodedInst.op = (instruction >> 0) & 0b11;
	decodedInst.rs2 = (instruction >> 2) & 0b11111;
	int imm_6 = (instruction >> 7) & 0b111;
	int imm_3 = (instruction >> 10) & 0b111;
	int imm = (imm_6 << 6)|(imm_3 << 3);
	decodedInst.imm = imm;
	decodedInst.funct3 = (instruction >> 13) & 0b111;
	return decodedInst;
}

RvInst decode_CIWtype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = CIW_TYPE;
	decodedInst.op = (instruction >> 0) & 0b11;
	decodedInst.rdp = (instruction >> 2) & 0b111;
	int imm_3 = (instruction >> 5) & 0b1;
	int imm_2 = (instruction >> 6) & 0b1;
	int imm_6 = (instruction >> 7) & 0b1111;
	int imm_4 = (instruction >> 11) & 0b11;
	int imm = (imm_3 << 3)|(imm_2 << 2)|(imm_6 << 6)|(imm_4 << 4);
	decodedInst.imm = imm;
	decodedInst.funct3 = (instruction >> 13) & 0b111;
	return decodedInst;
}

RvInst decode_CLtype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = CL_TYPE;
	decodedInst.op = (instruction >> 0) & 0b11;
	decodedInst.rdp = (instruction >> 2) & 0b111;
	int imm_2 = (instruction >> 5) & 0b1;
	int imm_6 = (instruction >> 6) & 0b1;
	int imm_3 = (instruction >> 10) & 0b111;
	int imm = (imm_2 << 2)|(imm_6 << 6)|(imm_3 << 3);
	decodedInst.imm = imm;
	decodedInst.rs1p = (instruction >> 7) & 0b111;
	decodedInst.funct3 = (instruction >> 13) & 0b111;
	return decodedInst;
}

RvInst decode_CLDtype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = CLD_TYPE;
	decodedInst.op = (instruction >> 0) & 0b11;
	decodedInst.rdp = (instruction >> 2) & 0b111;
	int imm_6 = (instruction >> 5) & 0b11;
	int imm_3 = (instruction >> 10) & 0b111;
	int imm = (imm_6 << 6)|(imm_3 << 3);
	decodedInst.imm = imm;
	decodedInst.rs1p = (instruction >> 7) & 0b111;
	decodedInst.funct3 = (instruction >> 13) & 0b111;
	return decodedInst;
}

RvInst decode_CStype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = CS_TYPE;
	decodedInst.op = (instruction >> 0) & 0b11;
	decodedInst.rs2p = (instruction >> 2) & 0b111;
	int imm_2 = (instruction >> 5) & 0b1;
	int imm_6 = (instruction >> 6) & 0b1;
	int imm_3 = (instruction >> 10) & 0b111;
	int imm = (imm_2 << 2)|(imm_6 << 6)|(imm_3 << 3);
	decodedInst.imm = imm;
	decodedInst.rs1p = (instruction >> 7) & 0b111;
	decodedInst.funct3 = (instruction >> 13) & 0b111;
	return decodedInst;
}

RvInst decode_CSDtype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = CSD_TYPE;
	decodedInst.op = (instruction >> 0) & 0b11;
	decodedInst.rs2p = (instruction >> 2) & 0b111;
	int imm_6 = (instruction >> 5) & 0b11;
	int imm_3 = (instruction >> 10) & 0b111;
	int imm = (imm_6 << 6)|(imm_3 << 3);
	decodedInst.imm = imm;
	decodedInst.rs1p = (instruction >> 7) & 0b111;
	decodedInst.funct3 = (instruction >> 13) & 0b111;
	return decodedInst;
}

RvInst decode_CBtype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = CB_TYPE;
	decodedInst.op = (instruction >> 0) & 0b11;
	int imm_5 = (instruction >> 2) & 0b1;
	int imm_1 = (instruction >> 3) & 0b11;
	int imm_6 = (instruction >> 5) & 0b11;
	int imm_3 = (instruction >> 10) & 0b11;
	int imm_8 = (instruction >> 12) & 0b1;
	int imm = (imm_5 << 5)|(imm_1 << 1)|(imm_6 << 6)|(imm_3 << 3)|(imm_8 << 8);
	imm = (imm >> 8) ? imm | 0b11111111111111111111111100000000 : imm;
	decodedInst.imm = imm;
	decodedInst.rs1 = (instruction >> 7) & 0b111;
	decodedInst.funct3 = (instruction >> 13) & 0b111;
	return decodedInst;
}

RvInst decode_CBItype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = CBI_TYPE;
	decodedInst.op = (instruction >> 0) & 0b11;
	int imm_0 = (instruction >> 2) & 0b11111;
	int imm_5 = (instruction >> 12) & 0b1;
	int imm = (imm_0 << 0)|(imm_5 << 5);
	decodedInst.imm = imm;
	decodedInst.rd_rs1p = (instruction >> 7) & 0b111;
	decodedInst.funct2 = (instruction >> 10) & 0b11;
	decodedInst.funct3 = (instruction >> 13) & 0b111;
	return decodedInst;
}

RvInst decode_CJtype(uint32_t instruction){
	RvInst decodedInst;
	decodedInst.type = CJ_TYPE;
	decodedInst.op = (instruction >> 0) & 0b11;
	int imm_5 = (instruction >> 2) & 0b1;
	int imm_1 = (instruction >> 3) & 0b111;
	int imm_7 = (instruction >> 6) & 0b1;
	int imm_6 = (instruction >> 7) & 0b1;
	int imm_10 = (instruction >> 8) & 0b1;
	int imm_8 = (instruction >> 9) & 0b11;
	int imm_4 = (instruction >> 11) & 0b1;
	int imm_11 = (instruction >> 12) & 0b1;
	int imm = (imm_5 << 5)|(imm_1 << 1)|(imm_7 << 7)|(imm_6 << 6)|(imm_10 << 10)|(imm_8 << 8)|(imm_4 << 4)|(imm_11 << 11);
	imm = (imm >> 11) ? imm | 0b11111111111111111111100000000000 : imm;
	decodedInst.imm = imm;
	decodedInst.funct3 = (instruction >> 13) & 0b111;
	return decodedInst;
}

RvInst decode_instruction32(uint32_t instruction){
RvInst decodedInst;
int opcode = instruction & 0b1111111;
switch(opcode){
	case 0b0110111:
		decodedInst = decode_Utype(instruction);
		if(1){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = LUI;
			return decodedInst;
		}
		break;
	case 0b0010111:
		decodedInst = decode_Utype(instruction);
		if(1){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AUIPC;
			return decodedInst;
		}
		break;
	case 0b1101111:
		decodedInst = decode_Jtype(instruction);
		if(1){
			decodedInst.group = NO_GROUP;
			decodedInst.name = JAL;
			return decodedInst;
		}
		break;
	case 0b1100111:
		decodedInst = decode_Itype(instruction);
		if(decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = JALR;
			return decodedInst;
		}
		break;
	case 0b1100011:
		decodedInst = decode_Btype(instruction);
		if(decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = BEQ;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = BNE;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b100 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = BLT;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b101 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = BGE;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b110 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = BLTU;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b111 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = BGEU;
			return decodedInst;
		}
		break;
	case 0b0000011:
		decodedInst = decode_Itype(instruction);
		if(decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = LB;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = LH;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b010 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = LW;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b100 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = LBU;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b101 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = LHU;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b110 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = LWU;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b111 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = LD;
			return decodedInst;
		}
		break;
	case 0b0100011:
		decodedInst = decode_Stype(instruction);
		if(decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SB;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SH;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b010 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = SW;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = SD;
			return decodedInst;
		}
		break;
	case 0b0010011:
		decodedInst = decode_Itype(instruction);
		if(decodedInst.funct3 == 0b000 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = ADDI;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SLTI;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SLTIU;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b100 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = XORI;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b110 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = ORI;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b111 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = ANDI;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b001 && decodedInst.funct7 == 0b0000000 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = SLLI;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b101 && decodedInst.funct7 == 0b0000000 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = SRLI;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b101 && decodedInst.funct7 == 0b0100000 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = SRAI;
			return decodedInst;
		}
		break;
	case 0b0110011:
		decodedInst = decode_Rtype(instruction);
		if(decodedInst.funct3 == 0b000 && decodedInst.funct7 == 0b0000000 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = ADD;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b000 && decodedInst.funct7 == 0b0100000 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = SUB;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b001 && decodedInst.funct7 == 0b0000000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SLL;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b010 && decodedInst.funct7 == 0b0000000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SLT;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 && decodedInst.funct7 == 0b0000000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SLTU;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b100 && decodedInst.funct7 == 0b0000000 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = XOR;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b101 && decodedInst.funct7 == 0b0000000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SRL;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b101 && decodedInst.funct7 == 0b0100000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SRA;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b110 && decodedInst.funct7 == 0b0000000 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = OR;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b111 && decodedInst.funct7 == 0b0000000 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = AND;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b000 && decodedInst.funct7 == 0b0000001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = MUL;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b001 && decodedInst.funct7 == 0b0000001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = MULH;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b010 && decodedInst.funct7 == 0b0000001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = MULHSU;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 && decodedInst.funct7 == 0b0000001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = MULHU;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b100 && decodedInst.funct7 == 0b0000001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = DIV;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b101 && decodedInst.funct7 == 0b0000001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = DIVU;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b110 && decodedInst.funct7 == 0b0000001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = REM;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b111 && decodedInst.funct7 == 0b0000001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = REMU;
			return decodedInst;
		}
		break;
	case 0b0001111:
		decodedInst = decode_Itype(instruction);
		if(decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FENCE;
			return decodedInst;
		}
		break;
	case 0b1110011:
		decodedInst = decode_Itype(instruction);
		if(decodedInst.funct3 == 0b000 && decodedInst.imm == 0b00000000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = ECALL;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b000 && decodedInst.imm == 0b00000001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = EBREAK;
			return decodedInst;
		}
		break;
	case 0b0011011:
		decodedInst = decode_Itype(instruction);
		if(decodedInst.funct3 == 0b000 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = ADDIW;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b001 && decodedInst.funct7 == 0b0000000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SLLIW;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b101 && decodedInst.funct7 == 0b0000000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SRLIW;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b101 && decodedInst.funct7 == 0b0100000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SRAIW;
			return decodedInst;
		}
		break;
	case 0b0111011:
		decodedInst = decode_Rtype(instruction);
		if(decodedInst.funct3 == 0b000 && decodedInst.funct7 == 0b0000000 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = ADDW;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b000 && decodedInst.funct7 == 0b0100000 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = SUBW;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b001 && decodedInst.funct7 == 0b0000000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SLLW;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b101 && decodedInst.funct7 == 0b0000000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SRLW;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b101 && decodedInst.funct7 == 0b0100000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SRAW;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b000 && decodedInst.funct7 == 0b0000001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = MULW;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b100 && decodedInst.funct7 == 0b0000001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = DIVW;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b101 && decodedInst.funct7 == 0b0000001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = DIVUW;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b110 && decodedInst.funct7 == 0b0000001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = REMW;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b111 && decodedInst.funct7 == 0b0000001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = REMUW;
			return decodedInst;
		}
		break;
	case 0b0101111:
		decodedInst = decode_RAMOtype(instruction);
		if(decodedInst.funct3 == 0b010 && decodedInst.funct5 == 0b00010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = LR_W;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b010 && decodedInst.funct5 == 0b00011 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SC_W;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b010 && decodedInst.funct5 == 0b00001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOSWAP_W;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b010 && decodedInst.funct5 == 0b00000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOADD_W;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b010 && decodedInst.funct5 == 0b00100 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOXOR_W;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b010 && decodedInst.funct5 == 0b01100 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOAND_W;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b010 && decodedInst.funct5 == 0b01000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOOR_W;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b010 && decodedInst.funct5 == 0b10000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOMIN_W;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b010 && decodedInst.funct5 == 0b10100 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOMAX_W;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b010 && decodedInst.funct5 == 0b11000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOMINU_W;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b010 && decodedInst.funct5 == 0b11100 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOMAXU_W;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 && decodedInst.funct5 == 0b00010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = LR_D;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 && decodedInst.funct5 == 0b00011 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = SC_D;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 && decodedInst.funct5 == 0b00001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOSWAP_D;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 && decodedInst.funct5 == 0b00000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOADD_D;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 && decodedInst.funct5 == 0b00100 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOXOR_D;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 && decodedInst.funct5 == 0b01100 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOAND_D;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 && decodedInst.funct5 == 0b01000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOOR_D;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 && decodedInst.funct5 == 0b10000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOMIN_D;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 && decodedInst.funct5 == 0b10100 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOMAX_D;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 && decodedInst.funct5 == 0b11000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOMINU_D;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 && decodedInst.funct5 == 0b11100 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = AMOMAXU_D;
			return decodedInst;
		}
		break;
	case 0b0000111:
		decodedInst = decode_Itype(instruction);
		if(decodedInst.funct3 == 0b010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FLW;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = FLD;
			return decodedInst;
		}
		break;
	case 0b0100111:
		decodedInst = decode_Stype(instruction);
		if(decodedInst.funct3 == 0b010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FSW;
			return decodedInst;
		}
		if(decodedInst.funct3 == 0b011 ){
			decodedInst.group = DBT_GROUP;
			decodedInst.name = FSD;
			return decodedInst;
		}
		break;
	case 0b1000011:
		decodedInst = decode_R4type(instruction);
		if(decodedInst.funct2 == 0b00 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FMADD_S;
			return decodedInst;
		}
		if(decodedInst.funct2 == 0b01 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FMADD_D;
			return decodedInst;
		}
		break;
	case 0b1000111:
		decodedInst = decode_R4type(instruction);
		if(decodedInst.funct2 == 0b00 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FMSUB_S;
			return decodedInst;
		}
		if(decodedInst.funct2 == 0b01 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FMSUB_D;
			return decodedInst;
		}
		break;
	case 0b1001011:
		decodedInst = decode_R4type(instruction);
		if(decodedInst.funct2 == 0b00 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FNMSUB_S;
			return decodedInst;
		}
		if(decodedInst.funct2 == 0b01 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FNMSUB_D;
			return decodedInst;
		}
		break;
	case 0b1001111:
		decodedInst = decode_R4type(instruction);
		if(decodedInst.funct2 == 0b00 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FNMADD_S;
			return decodedInst;
		}
		if(decodedInst.funct2 == 0b01 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FNMADD_D;
			return decodedInst;
		}
		break;
	case 0b1010011:
		decodedInst = decode_Rtype(instruction);
		if(decodedInst.funct7 == 0b0000000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FADD_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0000100 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FSUB_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0001000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FMUL_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0001100 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FDIV_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0101100 && decodedInst.rs2 == 0b00000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FSQRT_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0010000 && decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FSGNJ_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0010000 && decodedInst.funct3 == 0b001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FSGNJN_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0010000 && decodedInst.funct3 == 0b010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FSGNJX_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0010100 && decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FMIN_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0010100 && decodedInst.funct3 == 0b001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FMAX_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1100000 && decodedInst.rs2 == 0b00000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_W_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1100000 && decodedInst.rs2 == 0b00001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_WU_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1110000 && decodedInst.rs2 == 0b00000 && decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FMV_X_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1010000 && decodedInst.funct3 == 0b010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FEQ_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1010000 && decodedInst.funct3 == 0b001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FLT_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1010000 && decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FLE_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1110000 && decodedInst.funct3 == 0b001 && decodedInst.rs2 == 0b00000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCLASS_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1101000 && decodedInst.rs2 == 0b00000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_S_W;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1101000 && decodedInst.rs2 == 0b00001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_S_WU;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1111000 && decodedInst.funct3 == 0b000 && decodedInst.rs2 == 0b00000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FMV_W_X;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1100000 && decodedInst.rs2 == 0b00010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_L_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1100000 && decodedInst.rs2 == 0b00011 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_LU_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1100000 && decodedInst.rs2 == 0b00010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_S_L;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1100000 && decodedInst.rs2 == 0b00010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_S_LU;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0000001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FADD_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0000101 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FSUB_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0001001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FMUL_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0001101 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FDIV_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0101101 && decodedInst.rs2 == 0b00000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FSQRT_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0010001 && decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FSGNJ_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0010001 && decodedInst.funct3 == 0b001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FSGNJN_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0010001 && decodedInst.funct3 == 0b010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FSGNJX_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0010101 && decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FMIN_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0010101 && decodedInst.funct3 == 0b001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FMAX_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0100000 && decodedInst.rs2 == 0b00001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_S_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b0100001 && decodedInst.rs2 == 0b00000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_D_S;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1010001 && decodedInst.funct3 == 0b010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FEQ_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1010001 && decodedInst.funct3 == 0b001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FLT_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1010001 && decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FLE_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1110001 && decodedInst.funct3 == 0b001 && decodedInst.rs2 == 0b00000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCLASS_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1100001 && decodedInst.rs2 == 0b00000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_W_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1100001 && decodedInst.rs2 == 0b00001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_WU_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1101001 && decodedInst.rs2 == 0b00000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_D_W;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1101001 && decodedInst.rs2 == 0b00001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_D_WU;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1100001 && decodedInst.rs2 == 0b00010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_L_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1100001 && decodedInst.rs2 == 0b00011 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_LU_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1110001 && decodedInst.rs2 == 0b00000 && decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FMV_X_D;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1101001 && decodedInst.rs2 == 0b00010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_D_L;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1101001 && decodedInst.rs2 == 0b00011 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FCVT_D_LU;
			return decodedInst;
		}
		if(decodedInst.funct7 == 0b1111001 && decodedInst.funct3 == 0b000 && decodedInst.rs2 == 0b00000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = FMV_D_X;
			return decodedInst;
		}
		break;
	default: 
		return decodedInst;
};
return decodedInst;
}

RvInst decode_instruction16(uint32_t instruction){
RvInst decodedInst;
int opfunct3 = ((instruction & 0b11) << 3) | (instruction >> 13);
switch(opfunct3){
	case 0b00000:
		decodedInst = decode_CIWtype(instruction);
		if(decodedInst.op == 0b00 && decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_ADDI4SPN;
			return decodedInst;
		}
		break;
	case 0b00001:
		decodedInst = decode_CLDtype(instruction);
		if(decodedInst.op == 0b00 && decodedInst.funct3 == 0b001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_FLD;
			return decodedInst;
		}
		break;
	case 0b00010:
		decodedInst = decode_CLtype(instruction);
		if(decodedInst.op == 0b00 && decodedInst.funct3 == 0b010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_LW;
			return decodedInst;
		}
		break;
	case 0b00011:
		decodedInst = decode_CLtype(instruction);
		if(decodedInst.op == 0b00 && decodedInst.funct3 == 0b011 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_LD;
			return decodedInst;
		}
		break;
	case 0b00101:
		decodedInst = decode_CSDtype(instruction);
		if(decodedInst.op == 0b00 && decodedInst.funct3 == 0b101 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_FSD;
			return decodedInst;
		}
		break;
	case 0b00110:
		decodedInst = decode_CStype(instruction);
		if(decodedInst.op == 0b00 && decodedInst.funct3 == 0b110 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_SW;
			return decodedInst;
		}
		break;
	case 0b00111:
		decodedInst = decode_CSDtype(instruction);
		if(decodedInst.op == 0b00 && decodedInst.funct3 == 0b111 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_SD;
			return decodedInst;
		}
		break;
	case 0b01000:
		decodedInst = decode_CItype(instruction);
		if(decodedInst.imm == 0 && decodedInst.rd_rs1 == 0 && decodedInst.op == 0b01 && decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_NOP;
			return decodedInst;
		}
		if(decodedInst.op == 0b01 && decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_ADDI;
			return decodedInst;
		}
		break;
	case 0b01001:
		decodedInst = decode_CItype(instruction);
		if(decodedInst.op == 0b01 && decodedInst.funct3 == 0b001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_ADDIW;
			return decodedInst;
		}
		break;
	case 0b01010:
		decodedInst = decode_CItype(instruction);
		if(decodedInst.op == 0b01 && decodedInst.funct3 == 0b010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_LI;
			return decodedInst;
		}
		break;
	case 0b01011:
		decodedInst = decode_CItype(instruction);
		if(decodedInst.rd == 2 && decodedInst.op == 0b01 && decodedInst.funct3 == 0b011 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_ADDI16SP;
			return decodedInst;
		}
		if(decodedInst.op == 0b01 && decodedInst.funct3 == 0b011 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_LUI;
			return decodedInst;
		}
		break;
	case 0b01101:
		decodedInst = decode_CJtype(instruction);
		if(decodedInst.op == 0b01 && decodedInst.funct3 == 0b101 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_J;
			return decodedInst;
		}
		break;
	case 0b01110:
		decodedInst = decode_CBtype(instruction);
		if(decodedInst.op == 0b01 && decodedInst.funct3 == 0b110 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_BEQZ;
			return decodedInst;
		}
		break;
	case 0b01111:
		decodedInst = decode_CBtype(instruction);
		if(decodedInst.op == 0b01 && decodedInst.funct3 == 0b111 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_BNEZ;
			return decodedInst;
		}
		break;
	case 0b10000:
		decodedInst = decode_CItype(instruction);
		if(decodedInst.op == 0b10 && decodedInst.funct3 == 0b000 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_SLLI;
			return decodedInst;
		}
		break;
	case 0b10001:
		decodedInst = decode_CISDtype(instruction);
		if(decodedInst.op == 0b10 && decodedInst.funct3 == 0b001 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_FLDSP;
			return decodedInst;
		}
		break;
	case 0b10010:
		decodedInst = decode_CIStype(instruction);
		if(decodedInst.op == 0b10 && decodedInst.funct3 == 0b010 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_LWSP;
			return decodedInst;
		}
		break;
	case 0b10011:
		decodedInst = decode_CISDtype(instruction);
		if(decodedInst.op == 0b10 && decodedInst.funct3 == 0b011 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_LDSP;
			return decodedInst;
		}
		break;
	case 0b10100:
		decodedInst = decode_CRtype(instruction);
		if(decodedInst.funct4 == 0b1000 && decodedInst.rs2 == 0 && decodedInst.op == 0b10 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_JR;
			return decodedInst;
		}
		if(decodedInst.funct4 == 0b1000 && decodedInst.op == 0b10 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_MV;
			return decodedInst;
		}
		if(decodedInst.funct4 == 0b1001 && decodedInst.rs2 == 0 && decodedInst.rd_rs1 == 0 && decodedInst.op == 0b10 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_EBREAK;
			return decodedInst;
		}
		if(decodedInst.funct4 == 0b1001 && decodedInst.rs2 == 0 && decodedInst.op == 0b10 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_JALR;
			return decodedInst;
		}
		if(decodedInst.funct4 == 0b1001 && decodedInst.op == 0b10 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_ADD;
			return decodedInst;
		}
		break;
	case 0b10101:
		decodedInst = decode_CSSDtype(instruction);
		if(decodedInst.op == 0b10 && decodedInst.funct3 == 0b101 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_FSDSP;
			return decodedInst;
		}
		break;
	case 0b10110:
		decodedInst = decode_CSStype(instruction);
		if(decodedInst.op == 0b10 && decodedInst.funct3 == 0b110 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_SWSP;
			return decodedInst;
		}
		break;
	case 0b10111:
		decodedInst = decode_CSSDtype(instruction);
		if(decodedInst.op == 0b10 && decodedInst.funct3 == 0b111 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_SDSP;
			return decodedInst;
		}
		break;
	case 0b01100:
		decodedInst = decode_CRItype(instruction);
		if(decodedInst.op == 0b01 && decodedInst.funct2 == 0b00 && decodedInst.funct6 == 0b100011 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_SUB;
			return decodedInst;
		}
		if(decodedInst.op == 0b01 && decodedInst.funct2 == 0b01 && decodedInst.funct6 == 0b100011 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_XOR;
			return decodedInst;
		}
		if(decodedInst.op == 0b01 && decodedInst.funct2 == 0b10 && decodedInst.funct6 == 0b100011 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_OR;
			return decodedInst;
		}
		if(decodedInst.op == 0b01 && decodedInst.funct2 == 0b11 && decodedInst.funct6 == 0b100011 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_AND;
			return decodedInst;
		}
		if(decodedInst.op == 0b01 && decodedInst.funct2 == 0b00 && decodedInst.funct6 == 0b100111 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_SUBW;
			return decodedInst;
		}
		if(decodedInst.op == 0b01 && decodedInst.funct2 == 0b01 && decodedInst.funct6 == 0b100111 ){
			decodedInst.group = NO_GROUP;
			decodedInst.name = C_ADDW;
			return decodedInst;
		}
		break;
	default: 
		return decodedInst;
};
return decodedInst;
}

RvInst decode_instruction(uint32_t instruction, int debug){
RvInst decodedInst;
uint32_t instType = instruction & 0b11;
if(instType == 0b11) 
decodedInst = decode_instruction32(instruction);
else 
decodedInst = decode_instruction16(instruction);
if(debug){
printInstruction(decodedInst);
}
return decodedInst;
}


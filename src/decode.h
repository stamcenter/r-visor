#ifndef DECODE_H
#define DECODE_H

#include <stdint.h>
#include <stdio.h>

#include "printUtils.h"
#include "instructions.h"

RvInst decode_Rtype(uint32_t instruction);
RvInst decode_R4type(uint32_t instruction);
RvInst decode_RAMOtype(uint32_t instruction);
RvInst decode_Itype(uint32_t instruction);
RvInst decode_Stype(uint32_t instruction);
RvInst decode_Utype(uint32_t instruction);
RvInst decode_Btype(uint32_t instruction);
RvInst decode_Jtype(uint32_t instruction);
RvInst decode_CRtype(uint32_t instruction);
RvInst decode_CRItype(uint32_t instruction);
RvInst decode_CItype(uint32_t instruction);
RvInst decode_CIStype(uint32_t instruction);
RvInst decode_CISDtype(uint32_t instruction);
RvInst decode_CI16type(uint32_t instruction);
RvInst decode_CSStype(uint32_t instruction);
RvInst decode_CSSDtype(uint32_t instruction);
RvInst decode_CIWtype(uint32_t instruction);
RvInst decode_CLtype(uint32_t instruction);
RvInst decode_CLDtype(uint32_t instruction);
RvInst decode_CStype(uint32_t instruction);
RvInst decode_CSDtype(uint32_t instruction);
RvInst decode_CBtype(uint32_t instruction);
RvInst decode_CBItype(uint32_t instruction);
RvInst decode_CJtype(uint32_t instruction);
RvInst decode_instruction32(uint32_t instruction);
RvInst decode_instruction16(uint32_t instruction);
RvInst decode_instruction(uint32_t instruction, int debug);
#endif


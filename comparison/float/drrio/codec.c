/* Encode the 3rd source fixed-point register field:
 * |31 27|26  7|6      0|
 * | rs3 | ... | opcode |
 *  ^---^
 * Applies to the R4 uncompressed format.
 */
static bool
encode_rs3fp_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    uint32_t rd = opnd_get_reg(opnd) - DR_REG_F0;
    *out |= SET_FIELD(rd, 31, 27);
    return true;
}

/* Decode the 3rd source fixed-point register field:
 * |31 27|26  7|6      0|
 * | rs3 | ... | opcode |
 *  ^---^
 * Applies to the R4 uncompressed format.
 */
static bool
decode_rs3fp_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                  int idx, instr_t *out)
{
    reg_t reg = DR_REG_F0 + GET_FIELD(inst, 31, 27);
    opnd_t opnd = opnd_create_reg(reg);
    instr_set_src(out, idx, opnd);
    return true;
}

/* Decode the rounding mode in floating-point instructions:
 * |31 15|14  12|11  7|6      0|
 * | ... |  rm  | ... | opcode |
 *        ^----^
 * The valid values can be found in Table 11.1 in the RISC-V
 * Instruction Set Manual Volume I: Unprivileged ISA (ver. 20191213).
 */
static bool
decode_rm_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc, int idx,
               instr_t *out)
{
    int32_t imm = GET_FIELD(inst, 14, 12);
    /* Invalid. Reserved for future use. */
    ASSERT(imm != 0b101 && imm != 0b110);
    opnd_t opnd = opnd_create_immed_int(imm, op_sz);
    instr_set_src(out, idx, opnd);
    return true;
}

/* Decode the 2nd source floating-point register field:
 * |31 25|24   20|19  7|6      0|
 * | ... |  rs2  | ... | opcode |
 *        ^-----^
 * Applies to R, R4, S and B uncompressed formats.
 */
static bool
decode_rs2fp_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                  int idx, instr_t *out)
{
    reg_t reg = DR_REG_F0 + GET_FIELD(inst, 24, 20);
    opnd_t opnd = opnd_create_reg(reg);
    instr_set_src(out, idx, opnd);
    return true;
}


/* Decode the destination floating-point register field:
 * |31 12|11   7|6      0|
 * | ... |  rd  | opcode |
 *        ^----^
 * Applies to R, R4, I, U and J uncompressed formats.
 */
static bool
decode_rdfp_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                 int idx, instr_t *out)
{
    reg_t reg = DR_REG_F0 + GET_FIELD(inst, 11, 7);
    opnd_t opnd = opnd_create_reg(reg);
    instr_set_dst(out, idx, opnd);
    return true;
}

/* Decode the 1st source floating-point register field:
 * |31 20|19   15|14  7|6      0|
 * | ... |  rs1  | ... | opcode |
 *        ^-----^
 * Applies to R, R4, I, S and B uncompressed formats.
 */
static bool
decode_rs1fp_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                  int idx, instr_t *out)
{
    reg_t reg = DR_REG_F0 + GET_FIELD(inst, 19, 15);
    opnd_t opnd = opnd_create_reg(reg);
    instr_set_src(out, idx, opnd);
    return true;
}


/* Encode the destination floating-point register field:
 * |31 12|11   7|6      0|
 * | ... |  rd  | opcode |
 *        ^----^
 * Applies to R, R4, I, U and J uncompressed formats.
 */
static bool
encode_rdfp_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_dst(instr, idx);
    ASSERT(opnd_get_reg(opnd) >= DR_REG_F0);
    uint32_t rd = opnd_get_reg(opnd) - DR_REG_F0;
    *out |= SET_FIELD(rd, 11, 7);
    return true;
}

/* Encode the 1st source floating-point register field:
 * |31 20|19   15|14  7|6      0|
 * | ... |  rs1  | ... | opcode |
 *        ^-----^
 * Applies to R, R4, I, S and B uncompressed formats.
 */
static bool
encode_rs1fp_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    ASSERT(opnd_get_reg(opnd) >= DR_REG_F0);
    uint32_t rd = opnd_get_reg(opnd) - DR_REG_F0;
    *out |= SET_FIELD(rd, 19, 15);
    return true;
}

/* Encode the 2nd source floating-point register field:
 * |31 25|24   20|19  7|6      0|
 * | ... |  rs2  | ... | opcode |
 *        ^-----^
 * Applies to R, R4, S and B uncompressed formats.
 */
static bool
encode_rs2fp_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    ASSERT(opnd_get_reg(opnd) >= DR_REG_F0);
    uint32_t rd = opnd_get_reg(opnd) - DR_REG_F0;
    *out |= SET_FIELD(rd, 24, 20);
    return true;
}

/* Encode the rounding mode in floating-point instructions:
 * |31 15|14  12|11  7|6      0|
 * | ... |  rm  | ... | opcode |
 *        ^----^
 * The valid values can be found in Table 11.1 in the RISC-V
 * Instruction Set Manual Volume I: Unprivileged ISA (ver. 20191213).
 */
static bool
encode_rm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    int32_t imm = opnd_get_immed_int(opnd);
    /* Invalid. Reserved for future use. */
    ASSERT(imm != 0b101 && imm != 0b110);
    *out |= SET_FIELD(imm, 14, 12);
    return true;
}

/* Encode the 2nd source floating-point register field:
 * |31 25|24   20|19  7|6      0|
 * | ... |  rs2  | ... | opcode |
 *        ^-----^
 * Applies to R, R4, S and B uncompressed formats.
 */
static bool
encode_rs2fp_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    ASSERT(opnd_get_reg(opnd) >= DR_REG_F0);
    uint32_t rd = opnd_get_reg(opnd) - DR_REG_F0;
    *out |= SET_FIELD(rd, 24, 20);
    return true;
}


opnd_enc_func_t opnd_encoders[] = {
    [RISCV64_FLD_RDFP] = encode_rdfp_opnd,
    [RISCV64_FLD_RS1FP] = encode_rs1fp_opnd,
    [RISCV64_FLD_RS2FP] = encode_rs2fp_opnd,
    [RISCV64_FLD_RS3FP] = encode_rs3fp_opnd,
    [RISCV64_FLD_RM] = encode_rm_opnd,
};

opnd_dec_func_t opnd_decoders[] = {
    [RISCV64_FLD_RDFP] = decode_rdfp_opnd,
    [RISCV64_FLD_RS1FP] = decode_rs1fp_opnd,
    [RISCV64_FLD_RS2FP] = decode_rs2fp_opnd,
    [RISCV64_FLD_RS3FP] = decode_rs3fp_opnd,
    [RISCV64_FLD_RM] = decode_rm_opnd,
};
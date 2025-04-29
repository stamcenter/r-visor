typedef enum {
  /* Compressed instruction fields */
  RISCV64_FLD_CRD,
  RISCV64_FLD_CRDFP,
  RISCV64_FLD_CRS1,
  RISCV64_FLD_CRS2,
  RISCV64_FLD_CRS2FP,
  RISCV64_FLD_CRD_,
  RISCV64_FLD_CRD_FP,
  RISCV64_FLD_CRS1_,
  RISCV64_FLD_CRS2_,
  RISCV64_FLD_CRS2_FP,
  RISCV64_FLD_CRD__,
  RISCV64_FLD_CSHAMT,
  RISCV64_FLD_CSR_IMM,
  RISCV64_FLD_CADDI16SP_IMM,
  RISCV64_FLD_CLWSP_IMM,
  RISCV64_FLD_CLDSP_IMM,
  RISCV64_FLD_CLUI_IMM,
  RISCV64_FLD_CSWSP_IMM,
  RISCV64_FLD_CSDSP_IMM,
  RISCV64_FLD_CIW_IMM,
  RISCV64_FLD_CLW_IMM,
  RISCV64_FLD_CLD_IMM,
  RISCV64_FLD_CSW_IMM,
  RISCV64_FLD_CSD_IMM,
  RISCV64_FLD_CIMM5,
  RISCV64_FLD_CB_IMM,
  RISCV64_FLD_CJ_IMM,
} riscv64_fld_t;

/* Decode the destination fixed-point register field:
 * |31 12|11   7|6   2|1      0|
 * | ... |  rd  | ... | opcode |
 *        ^----^
 * Applies to CR and CI compressed formats.
 */
static bool decode_crd_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc,
                            byte *orig_pc, int idx, instr_t *out) {
  reg_t reg = DR_REG_X0 + GET_FIELD(inst, 11, 7);
  opnd_t opnd = opnd_create_reg(reg);
  instr_set_dst(out, idx, opnd);
  return true;
}

/* Decode the destination floating-point register field:
 * |31 12|11   7|6   2|1      0|
 * | ... |  rd  | ... | opcode |
 *        ^----^
 * Applies to CR and CI compressed formats.
 */
static bool decode_crdfp_opnd(dcontext_t *dc, uint32_t inst, int op_sz,
                              byte *pc, byte *orig_pc, int idx, instr_t *out) {
  reg_t reg = DR_REG_F0 + GET_FIELD(inst, 11, 7);
  opnd_t opnd = opnd_create_reg(reg);
  instr_set_dst(out, idx, opnd);
  return true;
}

/* Decode the 1st source fixed-point register field:
 * |31 12|11   7|6   2|1      0|
 * | ... |  rd  | ... | opcode |
 *        ^----^
 * Applies to CR and CI compressed formats.
 */
static bool decode_crs1_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc,
                             byte *orig_pc, int idx, instr_t *out) {
  reg_t reg = DR_REG_X0 + GET_FIELD(inst, 11, 7);
  opnd_t opnd = opnd_create_reg(reg);
  instr_set_src(out, idx, opnd);
  return true;
}

/* Decode the 2nd source fixed-point register field:
 * |31  7|6   2|1      0|
 * | ... | rs2 | opcode |
 *        ^---^
 * Applies to CR and CSS compressed formats.
 */
static bool decode_crs2_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc,
                             byte *orig_pc, int idx, instr_t *out) {
  reg_t reg = DR_REG_X0 + GET_FIELD(inst, 6, 2);
  opnd_t opnd = opnd_create_reg(reg);
  instr_set_src(out, idx, opnd);
  return true;
}

/* Decode the 2nd source floating-point register field:
 * |31  7|6   2|1      0|
 * | ... | rs2 | opcode |
 *        ^---^
 * Applies to CR and CSS compressed formats.
 */
static bool decode_crs2fp_opnd(dcontext_t *dc, uint32_t inst, int op_sz,
                               byte *pc, byte *orig_pc, int idx, instr_t *out) {
  reg_t reg = DR_REG_F0 + GET_FIELD(inst, 6, 2);
  opnd_t opnd = opnd_create_reg(reg);
  instr_set_src(out, idx, opnd);
  return true;
}

/* Decode the limited range (x8-x15) destination fixed-point register field:
 * |31  5|4   2|1      0|
 * | ... | rd' | opcode |
 *        ^---^
 * Applies to CIW and CL compressed formats.
 */
static bool decode_crd__opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc,
                             byte *orig_pc, int idx, instr_t *out) {
  reg_t reg = DR_REG_X8 + GET_FIELD(inst, 4, 2);
  opnd_t opnd = opnd_create_reg(reg);
  instr_set_dst(out, idx, opnd);
  return true;
}

/* Decode the limited range (x8-x15) destination floating-point register field:
 * |31  5|4   2|1      0|
 * | ... | rd' | opcode |
 *        ^---^
 * Applies to CIW and CL compressed formats.
 */
static bool decode_crd_fp_opnd(dcontext_t *dc, uint32_t inst, int op_sz,
                               byte *pc, byte *orig_pc, int idx, instr_t *out) {
  reg_t reg = DR_REG_F8 + GET_FIELD(inst, 4, 2);
  opnd_t opnd = opnd_create_reg(reg);
  instr_set_dst(out, idx, opnd);
  return true;
}

/* Decode the limited range (x8-x15) 1st source fixed-point register field:
 * |31 10|9    7|6   2|1      0|
 * | ... | rs1' | ... | opcode |
 *        ^---^
 * Applies to CL, CS, CA and CB compressed formats.
 */
static bool decode_crs1__opnd(dcontext_t *dc, uint32_t inst, int op_sz,
                              byte *pc, byte *orig_pc, int idx, instr_t *out) {
  reg_t reg = DR_REG_X8 + GET_FIELD(inst, 9, 7);
  opnd_t opnd = opnd_create_reg(reg);
  instr_set_src(out, idx, opnd);
  return true;
}

/* Decode the limited range (x8-x15) 2nd source fixed-point register field:
 * |31  5|4    2|1      0|
 * | ... | rs2' | opcode |
 *        ^---^
 * Applies to CS and CA compressed formats.
 */
static bool decode_crs2__opnd(dcontext_t *dc, uint32_t inst, int op_sz,
                              byte *pc, byte *orig_pc, int idx, instr_t *out) {
  reg_t reg = DR_REG_X8 + GET_FIELD(inst, 4, 2);
  opnd_t opnd = opnd_create_reg(reg);
  instr_set_src(out, idx, opnd);
  return true;
}

/* Decode the limited range (x8-x15) 2nd source floating-point register field:
 * |31  5|4    2|1      0|
 * | ... | rs2' | opcode |
 *        ^---^
 * Applies to CS and CA compressed formats.
 */
static bool decode_crs2_fp_opnd(dcontext_t *dc, uint32_t inst, int op_sz,
                                byte *pc, byte *orig_pc, int idx,
                                instr_t *out) {
  reg_t reg = DR_REG_F8 + GET_FIELD(inst, 4, 2);
  opnd_t opnd = opnd_create_reg(reg);
  instr_set_src(out, idx, opnd);
  return true;
}

/* Decode the limited range (x8-x15) destination fixed-point register field:
 * |31  5|4   2|1      0|
 * | ... | rd' | opcode |
 *        ^---^
 * Applies to the CA compressed format.
 */
static bool decode_crd___opnd(dcontext_t *dc, uint32_t inst, int op_sz,
                              byte *pc, byte *orig_pc, int idx, instr_t *out) {
  reg_t reg = DR_REG_X8 + GET_FIELD(inst, 9, 7);
  opnd_t opnd = opnd_create_reg(reg);
  instr_set_dst(out, idx, opnd);
  return true;
}

/* Decode RVC quadrant 0.
 *
 * The values are derived from table 16.5 in the RISC-V Instruction Set Manual
 * Volume I: Unprivileged ISA (ver. 20191213).
 */
static inline rv_instr_info_t *match_op_0(int funct, bool rv32, bool rv64) {
  switch (funct) {
  case 0:
    return &instr_infos[OP_c_addi4spn];
  case 1:
    return &instr_infos[OP_c_fld];
  case 2:
    return &instr_infos[OP_c_lw];
  case 3:
    if (rv32)
      return &instr_infos[OP_c_flw];
    if (rv64)
      return &instr_infos[OP_c_ld];
    return NULL;
  // 4 is reserved.
  case 5:
    return &instr_infos[OP_c_fsd];
  case 6:
    return &instr_infos[OP_c_sw];
  case 7:
    if (rv32)
      return &instr_infos[OP_c_fsw];
    else if (rv64)
      return &instr_infos[OP_c_sd];
    else
      return NULL;
  default:
    return NULL;
  }
}

/* Decode RVC quadrant 1.
 *
 * The values are derived from table 16.6 in the RISC-V Instruction Set Manual
 * Volume I: Unprivileged ISA (ver. 20191213).
 */
static inline rv_instr_info_t *match_op_1(int funct, int funct2, int funct3,
                                          int bit11to7, int bit12, bool rv32,
                                          bool rv64) {
  switch (funct) {
  case 0:
    if (bit11to7 == 0)
      return &instr_infos[OP_c_nop];
    else
      return &instr_infos[OP_c_addi];
  case 1:
    if (rv32)
      return &instr_infos[OP_c_jal];
    else if (rv64)
      return &instr_infos[OP_c_addiw];
    else
      return NULL;
  case 2:
    return &instr_infos[OP_c_li];
  case 3:
    if (bit11to7 == 2)
      return &instr_infos[OP_c_addi16sp];
    else
      return &instr_infos[OP_c_lui];
  case 4:
    switch (funct2) {
    case 0:
      return &instr_infos[OP_c_srli];
    case 1:
      return &instr_infos[OP_c_srai];
    case 2:
      return &instr_infos[OP_c_andi];
    case 3:
      switch (bit12) {
      case 0:
        switch (funct3) {
        case 0:
          return &instr_infos[OP_c_sub];
        case 1:
          return &instr_infos[OP_c_xor];
        case 2:
          return &instr_infos[OP_c_or];
        case 3:
          return &instr_infos[OP_c_and];
        default:
          return NULL;
        }
      case 1:
        switch (funct3) {
        case 0:
          return &instr_infos[OP_c_subw];
        case 1:
          return &instr_infos[OP_c_addw];
        // 2 and 3 are reserved.
        default:
          return NULL;
        }
      default:
        return NULL;
      }
    default:
      return NULL;
    }
  case 5:
    return &instr_infos[OP_c_j];
  case 6:
    return &instr_infos[OP_c_beqz];
  case 7:
    return &instr_infos[OP_c_bnez];
  default:
    return NULL;
  };
}

/* Decode RVC quadrant 2
 *
 * The values are derived from table 16.7 in the RISC-V Instruction Set Manual
 * Volume I: Unprivileged ISA (ver. 20191213).
 */
static inline rv_instr_info_t *match_op_2(int funct, int bit11to7, int bit6to2,
                                          int bit12, bool rv32, bool rv64) {
  switch (funct) {
  case 0:
    return &instr_infos[OP_c_slli];
  case 1:
    return &instr_infos[OP_c_fldsp];
  case 2:
    return &instr_infos[OP_c_lwsp];
  case 3:
    if (rv32)
      return &instr_infos[OP_c_flwsp];
    else if (rv64)
      return &instr_infos[OP_c_ldsp];
    else
      return NULL;
  case 4:
    switch (bit12) {
    case 0:
      if (bit6to2 == 0)
        return &instr_infos[OP_c_jr];
      else
        return &instr_infos[OP_c_mv];
    case 1:
      if ((bit11to7 == 0) && (bit6to2 == 0))
        return &instr_infos[OP_c_ebreak];
      else if (bit6to2 == 0)
        return &instr_infos[OP_c_jalr];
      else
        return &instr_infos[OP_c_add];
    default:
      return NULL;
    }
  case 5:
    return &instr_infos[OP_c_fsdsp];
  case 6:
    return &instr_infos[OP_c_swsp];
  case 7:
    if (rv32)
      return &instr_infos[OP_c_fswsp];
    else if (rv64)
      return &instr_infos[OP_c_sdsp];
    else
      return NULL;
  default:
    return NULL;
  }
}

static rv_instr_info_t *get_rvc_instr_info(uint32_t inst, int xlen) {
  /* 0 is an illegal instruction which is often used as a canary. */
  if (inst == 0)
    return &instr_infos[OP_unimp];

  int op = GET_FIELD(inst, 1, 0);
  int funct = GET_FIELD(inst, 15, 13);
  int bit11to7 = GET_FIELD(inst, 11, 7);
  int funct2 = GET_FIELD(inst, 11, 10);
  int bit12 = BIT(inst, 12);
  int funct3 = GET_FIELD(inst, 6, 5);
  int bit6to2 = GET_FIELD(inst, 6, 2);
  bool rv32 = xlen == 32;
  bool rv64 = xlen == 64;
  rv_instr_info_t *info = NULL;
  uint32_t mask, match;

  switch (op) {
  case 0:
    info = match_op_0(funct, rv32, rv64);
    break;
  case 1:
    info = match_op_1(funct, funct2, funct3, bit11to7, bit12, rv32, rv64);
    break;
  case 2:
    info = match_op_2(funct, bit11to7, bit6to2, bit12, rv32, rv64);
    break;
  }

  if (info == NULL)
    return NULL;
  mask = GET_FIELD(info->info.code, 31, 0);
  match = GET_FIELD(info->info.code, 63, 32);

  ASSERT_MESSAGE(CHKLVL_DEFAULT, "Malformed matching in RVC",
                 (inst & mask) == match);
  return info;
}

/* Encode the immediate of the caddi16sp instruction:
 * |15 13|   12   |11  7|6              2|1      0|
 * | ... | imm[9] | ... | imm[4|6|8:7|5] | opcode |
 *        ^------^       ^--------------^
 * From:
 * |31     9|8        4|3   0|
 * | imm[9] | imm[8:4] |  0  |
 */
static bool encode_caddi16sp_imm_opnd(instr_t *instr, byte *pc, int idx,
                                      uint32_t *out, decode_info_t *di) {
  opnd_t opnd = instr_get_src(instr, idx);
  int32_t imm = opnd_get_immed_int(opnd);
  *out |= SET_FIELD(imm >> 5, 2, 2) | SET_FIELD(imm >> 7, 4, 3) |
          SET_FIELD(imm >> 6, 5, 5) | SET_FIELD(imm >> 4, 6, 6) |
          SET_FIELD(imm >> 9, 12, 12);
  return true;
}

/* Encode the SP-based immediate offset of c.lwsp and c.flwsp instructions:
 * |15 13|   12   |11  7|6            2|1      0|
 * | ... | imm[5] | ... | imm[4:2|7:6] | opcode |
 *        ^------^       ^------------^
 * From:
 *      |31  8|7        2|3   0|
 * sp + |  0  | imm[7:2] |  0  |
 */
static bool encode_clwsp_imm_opnd(instr_t *instr, byte *pc, int idx,
                                  uint32_t *out, decode_info_t *di) {
  opnd_t opnd = instr_get_src(instr, idx);
  int32_t imm = opnd_get_disp(opnd);
  ASSERT(opnd_get_base(opnd) == DR_REG_SP);
  *out |= SET_FIELD(imm >> 6, 3, 2) | SET_FIELD(imm >> 2, 6, 4) |
          SET_FIELD(imm >> 5, 12, 12);
  return true;
}

/* Encode the SP-based immediate offset of c.ldsp and c.fldsp instructions:
 * |15 13|   12   |11  7|6            2|1      0|
 * | ... | imm[5] | ... | imm[4:3|8:6] | opcode |
 *        ^------^       ^------------^
 * From:
 *      |31  9|8        2|3   0|
 * sp + |  0  | imm[8:3] |  0  |
 */
static bool encode_cldsp_imm_opnd(instr_t *instr, byte *pc, int idx,
                                  uint32_t *out, decode_info_t *di) {
  opnd_t opnd = instr_get_src(instr, idx);
  int32_t imm = opnd_get_disp(opnd);
  ASSERT(opnd_get_base(opnd) == DR_REG_SP);
  *out |= SET_FIELD(imm >> 6, 4, 2) | SET_FIELD(imm >> 3, 6, 5) |
          SET_FIELD(imm >> 5, 12, 12);
  return true;
}

/* Encode the immediate of the c.lui instruction:
 * |15 13|   12    |11  7|6          2|1      0|
 * | ... | imm[17] | ... | imm[16:12] | opcode |
 *        ^-------^       ^----------^
 * From:
 * |31     17|16        12|11  0|
 * | imm[17] | imm[16:12] |  0  |
 */
static bool encode_clui_imm_opnd(instr_t *instr, byte *pc, int idx,
                                 uint32_t *out, decode_info_t *di) {
  opnd_t opnd = instr_get_src(instr, idx);
  int32_t imm = opnd_get_immed_int(opnd);
  *out |= SET_FIELD(imm, 6, 2) | SET_FIELD(imm >> 5, 12, 12);
  return true;
}

/* Encode the SP-based offset immediate of c.swsp and c.fswsp instructions:
 * |15 13|12           7|6   2|1      0|
 * | ... | imm[5:2|7:6] | ... | opcode |
 *        ^------------^
 * From:
 *      |31  8|7        2|1 0|
 * sp + |  0  | imm[7:2] | 0 |
 */
static bool encode_cswsp_imm_opnd(instr_t *instr, byte *pc, int idx,
                                  uint32_t *out, decode_info_t *di) {
  opnd_t opnd = instr_get_dst(instr, idx);
  int32_t imm = opnd_get_disp(opnd);
  ASSERT(opnd_get_base(opnd) == DR_REG_SP);
  *out |= SET_FIELD(imm >> 6, 8, 7) | SET_FIELD(imm >> 2, 12, 9);
  return true;
}

/* Encode the SP-based offset immediate of c.sdsp and c.fsdsp instructions:
 * |15 13|12           7|6   2|1      0|
 * | ... | imm[5:3|8:6] | ... | opcode |
 *        ^------------^
 * From:
 *      |31  9|8        3|2 0|
 * sp + |  0  | imm[7:3] | 0 |
 */
static bool encode_csdsp_imm_opnd(instr_t *instr, byte *pc, int idx,
                                  uint32_t *out, decode_info_t *di) {
  opnd_t opnd = instr_get_dst(instr, idx);
  int32_t imm = opnd_get_disp(opnd);
  ASSERT(opnd_get_base(opnd) == DR_REG_SP);
  *out |= SET_FIELD(imm >> 6, 9, 7) | SET_FIELD(imm >> 3, 12, 10);
  return true;
}

/* Encode the immediate of the c.addi4spn instruction:
 * |15 13|12               5|4   2|1      0|
 * | ... | imm[5:4|9:6|2|3] | ... | opcode |
 *        ^----------------^
 * From:
 * |31 10|9        2|1 0|
 * |  0  | imm[9:2] | 0 |
 */
static bool encode_ciw_imm_opnd(instr_t *instr, byte *pc, int idx,
                                uint32_t *out, decode_info_t *di) {
  opnd_t opnd = instr_get_src(instr, idx);
  int32_t imm = opnd_get_immed_int(opnd);
  *out |= SET_FIELD(imm >> 3, 5, 5) | SET_FIELD(imm >> 2, 6, 6) |
          SET_FIELD(imm >> 6, 10, 7) | SET_FIELD(imm >> 4, 12, 11);
  return true;
}

/* Encode the base register and offset immediate of c.lw and c.flw
 * instructions:
 * |15 13|12      10|9   7|6        5|4   2|1      0|
 * | ... | imm[5:3] | rs1' | imm[2|6] | ... | opcode |
 *        ^--------^ ^----^ ^--------^
 * From:
 *        |31  7|6        2|1 0|
 * rs1' + |  0  | imm[6:2] | 0 |
 */
static bool encode_clw_imm_opnd(instr_t *instr, byte *pc, int idx,
                                uint32_t *out, decode_info_t *di) {
  opnd_t opnd = instr_get_src(instr, idx);
  uint32_t reg = opnd_get_base(opnd) - DR_REG_X8;
  *out |= SET_FIELD(reg, 9, 7);
  int32_t imm = opnd_get_disp(opnd);
  *out |= SET_FIELD(imm >> 6, 5, 5) | SET_FIELD(imm >> 2, 6, 6) |
          SET_FIELD(imm >> 3, 12, 10);
  return true;
}

/* Encode the base register and offset immediate of c.ld and c.fld
 * instructions:
 * |15 13|12      10|9    7|6        5|4   2|1      0|
 * | ... | imm[5:3] | rs1' | imm[7:6] | ... | opcode |
 *        ^--------^ ^----^ ^--------^
 * From:
 *        |31  8|7        3|2 0|
 * rs1' + |  0  | imm[7:3] | 0 |
 */
static bool encode_cld_imm_opnd(instr_t *instr, byte *pc, int idx,
                                uint32_t *out, decode_info_t *di) {
  opnd_t opnd = instr_get_src(instr, idx);
  uint32_t reg = opnd_get_base(opnd) - DR_REG_X8;
  *out |= SET_FIELD(reg, 9, 7);
  int32_t imm = opnd_get_disp(opnd);
  *out |= SET_FIELD(imm >> 6, 6, 5) | SET_FIELD(imm >> 3, 12, 10);
  return true;
}

/* Encode the base register and offset immediate of c.sw and c.fsw
 * instructions:
 * |15 13|12      10|9    7|6        5|4   2|1      0|
 * | ... | imm[5:3] | rs1' | imm[2|6] | ... | opcode |
 *        ^--------^ ^----^ ^--------^
 * From:
 *        |31  7|6        2|1 0|
 * rs1' + |  0  | imm[6:2] | 0 |
 */
static bool encode_csw_imm_opnd(instr_t *instr, byte *pc, int idx,
                                uint32_t *out, decode_info_t *di) {
  opnd_t opnd = instr_get_dst(instr, idx);
  uint32_t reg = opnd_get_base(opnd) - DR_REG_X8;
  *out |= SET_FIELD(reg, 9, 7);
  int32_t imm = opnd_get_disp(opnd);
  *out |= SET_FIELD(imm >> 6, 5, 5) | SET_FIELD(imm >> 2, 6, 6) |
          SET_FIELD(imm >> 3, 12, 10);
  return true;
}

/* Encode the base register and offset immediate of c.sd and c.fsd
 * instructions:
 * |15 13|12      10|9    7|6        5|4   2|1      0|
 * | ... | imm[5:3] | rs1' | imm[7:6] | ... | opcode |
 *        ^--------^ ^----^ ^--------^
 * From:
 *        |31  8|7        3|2 0|
 * rs1' + |  0  | imm[7:3] | 0 |
 */
static bool encode_csd_imm_opnd(instr_t *instr, byte *pc, int idx,
                                uint32_t *out, decode_info_t *di) {
  opnd_t opnd = instr_get_dst(instr, idx);
  uint32_t reg = opnd_get_base(opnd) - DR_REG_X8;
  *out |= SET_FIELD(reg, 9, 7);
  int32_t imm = opnd_get_disp(opnd);
  *out |= SET_FIELD(imm >> 6, 6, 5) | SET_FIELD(imm >> 3, 12, 10);
  return true;
}

/* Encode the base immediate of c.addi, c.addiw, c.li, c.andi instructions:
 * |15 13|   12   |11  7|6        2|1      0|
 * | ... | imm[5] | ... | imm[4:0] | opcode |
 *        ^------^       ^--------^
 * From:
 * |31     5|4        0|
 * | imm[5] | imm[4:0] |
 */
static bool encode_cimm5_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out,
                              decode_info_t *di) {
  opnd_t opnd = instr_get_src(instr, idx);
  int32_t imm = opnd_get_immed_int(opnd);
  *out |= SET_FIELD(imm, 6, 2) | SET_FIELD(imm >> 5, 12, 12);
  return true;
}

/* Encode the immediate field of the CB-type format as a pc-relative offset:
 * |15 13|12        10|9   7|6              2|1      0|
 * | ... | imm[8|4:3] | ... | imm[7:6|2:1|5] | opcode |
 *        ^----------^       ^--------------^
 * From:
 * |31     8|7        1| 0 |
 * | imm[8] | imm[7:1] | 0 |
 */
static bool encode_cb_imm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out,
                               decode_info_t *di) {
  opnd_t opnd = instr_get_target(instr);

  int32_t imm;
  if (opnd.kind == PC_kind)
    imm = opnd_get_pc(opnd) - pc;
  else if (opnd.kind == INSTR_kind)
    imm = (byte *)opnd_get_instr(opnd)->offset - (byte *)instr->offset;
  else
    return false;

  *out |= SET_FIELD(imm >> 5, 2, 2) | SET_FIELD(imm >> 1, 4, 3) |
          SET_FIELD(imm >> 6, 6, 5) | SET_FIELD(imm >> 3, 11, 10) |
          SET_FIELD(imm >> 8, 12, 12);
  return true;
}

/* Encode the immediate field of the CJ-type format as a pc-relative offset:
 * |15 13|12                      2|1      0|
 * | ... | [11|4|9:8|10|6|7|3:1|5] | opcode |
 *        ^-----------------------^
 * From:
 * |31     11|10        1| 0 |
 * | imm[11] | imm[10:1] | 0 |
 */
static bool encode_cj_imm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out,
                               decode_info_t *di) {
  opnd_t opnd = instr_get_target(instr);

  int32_t imm;
  if (opnd.kind == PC_kind)
    imm = opnd_get_pc(opnd) - pc;
  else if (opnd.kind == INSTR_kind)
    imm = (byte *)opnd_get_instr(opnd)->offset - (byte *)instr->offset;
  else
    return false;

  *out |= SET_FIELD(imm >> 5, 2, 2) | SET_FIELD(imm >> 1, 5, 3) |
          SET_FIELD(imm >> 7, 6, 6) | SET_FIELD(imm >> 6, 7, 7) |
          SET_FIELD(imm >> 10, 8, 8) | SET_FIELD(imm >> 8, 10, 9) |
          SET_FIELD(imm >> 4, 11, 11) | SET_FIELD(imm >> 11, 12, 12);
  return true;
}

opnd_enc_func_t opnd_encoders[] = {
    [RISCV64_FLD_CRD] = encode_crd_opnd,
    [RISCV64_FLD_CRDFP] = encode_crdfp_opnd,
    [RISCV64_FLD_CRS1] = encode_crs1_opnd,
    [RISCV64_FLD_CRS2] = encode_crs2_opnd,
    [RISCV64_FLD_CRS2FP] = encode_crs2fp_opnd,
    [RISCV64_FLD_CRD_] = encode_crd__opnd,
    [RISCV64_FLD_CRD_FP] = encode_crd_fp_opnd,
    [RISCV64_FLD_CRS1_] = encode_crs1__opnd,
    [RISCV64_FLD_CRS2_] = encode_crs2__opnd,
    [RISCV64_FLD_CRS2_FP] = encode_crs2_fp_opnd,
    [RISCV64_FLD_CRD__] = encode_crd___opnd,
    [RISCV64_FLD_CSHAMT] = encode_cshamt_opnd,
    [RISCV64_FLD_CSR_IMM] = encode_csr_imm_opnd,
    [RISCV64_FLD_CADDI16SP_IMM] = encode_caddi16sp_imm_opnd,
    [RISCV64_FLD_CLWSP_IMM] = encode_clwsp_imm_opnd,
    [RISCV64_FLD_CLDSP_IMM] = encode_cldsp_imm_opnd,
    [RISCV64_FLD_CLUI_IMM] = encode_clui_imm_opnd,
    [RISCV64_FLD_CSWSP_IMM] = encode_cswsp_imm_opnd,
    [RISCV64_FLD_CSDSP_IMM] = encode_csdsp_imm_opnd,
    [RISCV64_FLD_CIW_IMM] = encode_ciw_imm_opnd,
    [RISCV64_FLD_CLW_IMM] = encode_clw_imm_opnd,
    [RISCV64_FLD_CLD_IMM] = encode_cld_imm_opnd,
    [RISCV64_FLD_CSW_IMM] = encode_csw_imm_opnd,
    [RISCV64_FLD_CSD_IMM] = encode_csd_imm_opnd,
    [RISCV64_FLD_CIMM5] = encode_cimm5_opnd,
    [RISCV64_FLD_CB_IMM] = encode_cb_imm_opnd,
    [RISCV64_FLD_CJ_IMM] = encode_cj_imm_opnd,
};

opnd_dec_func_t opnd_decoders[] = {
    [RISCV64_FLD_CRD] = decode_crd_opnd,
    [RISCV64_FLD_CRDFP] = decode_crdfp_opnd,
    [RISCV64_FLD_CRS1] = decode_crs1_opnd,
    [RISCV64_FLD_CRS2] = decode_crs2_opnd,
    [RISCV64_FLD_CRS2FP] = decode_crs2fp_opnd,
    [RISCV64_FLD_CRD_] = decode_crd__opnd,
    [RISCV64_FLD_CRD_FP] = decode_crd_fp_opnd,
    [RISCV64_FLD_CRS1_] = decode_crs1__opnd,
    [RISCV64_FLD_CRS2_] = decode_crs2__opnd,
    [RISCV64_FLD_CRS2_FP] = decode_crs2_fp_opnd,
    [RISCV64_FLD_CRD__] = decode_crd___opnd,
    [RISCV64_FLD_CSHAMT] = decode_cshamt_opnd,
    [RISCV64_FLD_CSR_IMM] = decode_csr_imm_opnd,
    [RISCV64_FLD_CADDI16SP_IMM] = decode_caddi16sp_imm_opnd,
    [RISCV64_FLD_CLWSP_IMM] = decode_clwsp_imm_opnd,
    [RISCV64_FLD_CLDSP_IMM] = decode_cldsp_imm_opnd,
    [RISCV64_FLD_CLUI_IMM] = decode_clui_imm_opnd,
    [RISCV64_FLD_CSWSP_IMM] = decode_cswsp_imm_opnd,
    [RISCV64_FLD_CSDSP_IMM] = decode_csdsp_imm_opnd,
    [RISCV64_FLD_CIW_IMM] = decode_ciw_imm_opnd,
    [RISCV64_FLD_CLW_IMM] = decode_clw_imm_opnd,
    [RISCV64_FLD_CLD_IMM] = decode_cld_imm_opnd,
    [RISCV64_FLD_CSW_IMM] = decode_csw_imm_opnd,
    [RISCV64_FLD_CSD_IMM] = decode_csd_imm_opnd,
    [RISCV64_FLD_CIMM5] = decode_cimm5_opnd,
    [RISCV64_FLD_CB_IMM] = decode_cb_imm_opnd,
    [RISCV64_FLD_CJ_IMM] = decode_cj_imm_opnd};


/* Encode the destination fixed-point register field:
 * |31 12|11   7|6   2|1      0|
 * | ... |  rd  | ... | opcode |
 *        ^----^
 * Applies to CR and CI compressed formats.
 */
static bool
encode_crd_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_dst(instr, idx);
    uint32_t rd = opnd_get_reg(opnd) - DR_REG_X0;
    *out |= SET_FIELD(rd, 11, 7);
    return true;
}

/* Encode the destination floating-point register field:
 * |31 12|11   7|6   2|1      0|
 * | ... |  rd  | ... | opcode |
 *        ^----^
 * Applies to CR and CI compressed formats.
 */
static bool
encode_crdfp_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_dst(instr, idx);
    uint32_t rd = opnd_get_reg(opnd) - DR_REG_F0;
    *out |= SET_FIELD(rd, 11, 7);
    return true;
}

/* Encode the 1st source fixed-point register field:
 * |31 12|11   7|6   2|1      0|
 * | ... |  rd  | ... | opcode |
 *        ^----^
 * Applies to CR and CI compressed formats.
 */
static bool
encode_crs1_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    uint32_t rd = opnd_get_reg(opnd) - DR_REG_X0;
    *out |= SET_FIELD(rd, 11, 7);
    return true;
}

/* Encode the 2nd source fixed-point register field:
 * |31  7|6   2|1      0|
 * | ... | rs2 | opcode |
 *        ^---^
 * Applies to CR and CSS compressed formats.
 */
static bool
encode_crs2_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    uint32_t rd = opnd_get_reg(opnd) - DR_REG_X0;
    *out |= SET_FIELD(rd, 6, 2);
    return true;
}

/* Encode the 2nd source floating-point register field:
 * |31  7|6   2|1      0|
 * | ... | rs2 | opcode |
 *        ^---^
 * Applies to CR and CSS compressed formats.
 */
static bool
encode_crs2fp_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    uint32_t rd = opnd_get_reg(opnd) - DR_REG_F0;
    *out |= SET_FIELD(rd, 6, 2);
    return true;
}

/* Encode the limited range (x8-x15) destination fixed-point register field:
 * |31  5|4   2|1      0|
 * | ... | rd' | opcode |
 *        ^---^
 * Applies to CIW and CL compressed formats.
 */
static bool
encode_crd__opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_dst(instr, idx);
    uint32_t rd = opnd_get_reg(opnd) - DR_REG_X8;
    *out |= SET_FIELD(rd, 4, 2);
    return true;
}

/* Encode the limited range (x8-x15) destination floating-point register field:
 * |31  5|4   2|1      0|
 * | ... | rd' | opcode |
 *        ^---^
 * Applies to CIW and CL compressed formats.
 */
static bool
encode_crd_fp_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_dst(instr, idx);
    uint32_t rd = opnd_get_reg(opnd) - DR_REG_F8;
    *out |= SET_FIELD(rd, 4, 2);
    return true;
}

/* Encode the limited range (x8-x15) 1st source fixed-point register field:
 * |31 10|9    7|6   2|1      0|
 * | ... | rs1' | ... | opcode |
 *        ^---^
 * Applies to CL, CS, CA and CB compressed formats.
 */
static bool
encode_crs1__opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    uint32_t rd = opnd_get_reg(opnd) - DR_REG_X8;
    *out |= SET_FIELD(rd, 9, 7);
    return true;
}

/* Encode the limited range (x8-x15) 2nd source fixed-point register field:
 * |31  5|4    2|1      0|
 * | ... | rs2' | opcode |
 *        ^---^
 * Applies to CS and CA compressed formats.
 */
static bool
encode_crs2__opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    uint32_t rd = opnd_get_reg(opnd) - DR_REG_X8;
    *out |= SET_FIELD(rd, 4, 2);
    return true;
}

/* Encode the limited range (x8-x15) 2nd source floating-point register field:
 * |31  5|4    2|1      0|
 * | ... | rs2' | opcode |
 *        ^---^
 * Applies to CS and CA compressed formats.
 */
static bool
encode_crs2_fp_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    uint32_t rd = opnd_get_reg(opnd) - DR_REG_F8;
    *out |= SET_FIELD(rd, 4, 2);
    return true;
}

/* Encode the limited range (x8-x15) destination fixed-point register field:
 * |31  5|4   2|1      0|
 * | ... | rd' | opcode |
 *        ^---^
 * Applies to the CA compressed format.
 */
static bool
encode_crd___opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_dst(instr, idx);
    uint32_t rd = opnd_get_reg(opnd) - DR_REG_X8;
    *out |= SET_FIELD(rd, 9, 7);
    return true;
}



/* Encode the 6-bit (6th bit always 0 in rv32) shift amount:
 * |15    13|   12   |11    10|9    7|6        2|1      0|
 * | funct3 | imm[5] | funct2 | rs1' | imm[4:0] | opcode |
 *           ^------^                 ^--------^
 */
static bool
encode_cshamt_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    int32_t imm = opnd_get_immed_int(opnd);
    *out |= SET_FIELD(imm, 6, 2) | SET_FIELD(imm >> 5, 12, 12);
    return true;
}

/* Encode the CSR immediate in instructions from the Zicsr extension:
 * |31 20|19      15|14  7|6      0|
 * | csr | imm[4:0] | ... | opcode |
 *        ^--------^
 * From:
 * |31  5|4        0|
 * |  0  | imm[4:0] |
 */
static bool
encode_csr_imm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    int32_t imm = opnd_get_immed_int(opnd);
    *out |= SET_FIELD(imm, 19, 15);
    return true;
}

/* Encode the immediate of the caddi16sp instruction:
 * |15 13|   12   |11  7|6              2|1      0|
 * | ... | imm[9] | ... | imm[4|6|8:7|5] | opcode |
 *        ^------^       ^--------------^
 * From:
 * |31     9|8        4|3   0|
 * | imm[9] | imm[8:4] |  0  |
 */
static bool
encode_caddi16sp_imm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out,
                          decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    int32_t imm = opnd_get_immed_int(opnd);
    *out |= SET_FIELD(imm >> 5, 2, 2) | SET_FIELD(imm >> 7, 4, 3) |
        SET_FIELD(imm >> 6, 5, 5) | SET_FIELD(imm >> 4, 6, 6) |
        SET_FIELD(imm >> 9, 12, 12);
    return true;
}

/* Encode the SP-based immediate offset of c.lwsp and c.flwsp instructions:
 * |15 13|   12   |11  7|6            2|1      0|
 * | ... | imm[5] | ... | imm[4:2|7:6] | opcode |
 *        ^------^       ^------------^
 * From:
 *      |31  8|7        2|3   0|
 * sp + |  0  | imm[7:2] |  0  |
 */
static bool
encode_clwsp_imm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    int32_t imm = opnd_get_disp(opnd);
    ASSERT(opnd_get_base(opnd) == DR_REG_SP);
    *out |= SET_FIELD(imm >> 6, 3, 2) | SET_FIELD(imm >> 2, 6, 4) |
        SET_FIELD(imm >> 5, 12, 12);
    return true;
}

/* Encode the SP-based immediate offset of c.ldsp and c.fldsp instructions:
 * |15 13|   12   |11  7|6            2|1      0|
 * | ... | imm[5] | ... | imm[4:3|8:6] | opcode |
 *        ^------^       ^------------^
 * From:
 *      |31  9|8        2|3   0|
 * sp + |  0  | imm[8:3] |  0  |
 */
static bool
encode_cldsp_imm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    int32_t imm = opnd_get_disp(opnd);
    ASSERT(opnd_get_base(opnd) == DR_REG_SP);
    *out |= SET_FIELD(imm >> 6, 4, 2) | SET_FIELD(imm >> 3, 6, 5) |
        SET_FIELD(imm >> 5, 12, 12);
    return true;
}

/* Encode the immediate of the c.lui instruction:
 * |15 13|   12    |11  7|6          2|1      0|
 * | ... | imm[17] | ... | imm[16:12] | opcode |
 *        ^-------^       ^----------^
 * From:
 * |31     17|16        12|11  0|
 * | imm[17] | imm[16:12] |  0  |
 */
static bool
encode_clui_imm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    int32_t imm = opnd_get_immed_int(opnd);
    *out |= SET_FIELD(imm, 6, 2) | SET_FIELD(imm >> 5, 12, 12);
    return true;
}

/* Encode the SP-based offset immediate of c.swsp and c.fswsp instructions:
 * |15 13|12           7|6   2|1      0|
 * | ... | imm[5:2|7:6] | ... | opcode |
 *        ^------------^
 * From:
 *      |31  8|7        2|1 0|
 * sp + |  0  | imm[7:2] | 0 |
 */
static bool
encode_cswsp_imm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_dst(instr, idx);
    int32_t imm = opnd_get_disp(opnd);
    ASSERT(opnd_get_base(opnd) == DR_REG_SP);
    *out |= SET_FIELD(imm >> 6, 8, 7) | SET_FIELD(imm >> 2, 12, 9);
    return true;
}

/* Encode the SP-based offset immediate of c.sdsp and c.fsdsp instructions:
 * |15 13|12           7|6   2|1      0|
 * | ... | imm[5:3|8:6] | ... | opcode |
 *        ^------------^
 * From:
 *      |31  9|8        3|2 0|
 * sp + |  0  | imm[7:3] | 0 |
 */
static bool
encode_csdsp_imm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_dst(instr, idx);
    int32_t imm = opnd_get_disp(opnd);
    ASSERT(opnd_get_base(opnd) == DR_REG_SP);
    *out |= SET_FIELD(imm >> 6, 9, 7) | SET_FIELD(imm >> 3, 12, 10);
    return true;
}

/* Encode the immediate of the c.addi4spn instruction:
 * |15 13|12               5|4   2|1      0|
 * | ... | imm[5:4|9:6|2|3] | ... | opcode |
 *        ^----------------^
 * From:
 * |31 10|9        2|1 0|
 * |  0  | imm[9:2] | 0 |
 */
static bool
encode_ciw_imm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    int32_t imm = opnd_get_immed_int(opnd);
    *out |= SET_FIELD(imm >> 3, 5, 5) | SET_FIELD(imm >> 2, 6, 6) |
        SET_FIELD(imm >> 6, 10, 7) | SET_FIELD(imm >> 4, 12, 11);
    return true;
}

/* Encode the base register and offset immediate of c.lw and c.flw
 * instructions:
 * |15 13|12      10|9   7|6        5|4   2|1      0|
 * | ... | imm[5:3] | rs1' | imm[2|6] | ... | opcode |
 *        ^--------^ ^----^ ^--------^
 * From:
 *        |31  7|6        2|1 0|
 * rs1' + |  0  | imm[6:2] | 0 |
 */
static bool
encode_clw_imm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    uint32_t reg = opnd_get_base(opnd) - DR_REG_X8;
    *out |= SET_FIELD(reg, 9, 7);
    int32_t imm = opnd_get_disp(opnd);
    *out |= SET_FIELD(imm >> 6, 5, 5) | SET_FIELD(imm >> 2, 6, 6) |
        SET_FIELD(imm >> 3, 12, 10);
    return true;
}

/* Encode the base register and offset immediate of c.ld and c.fld
 * instructions:
 * |15 13|12      10|9    7|6        5|4   2|1      0|
 * | ... | imm[5:3] | rs1' | imm[7:6] | ... | opcode |
 *        ^--------^ ^----^ ^--------^
 * From:
 *        |31  8|7        3|2 0|
 * rs1' + |  0  | imm[7:3] | 0 |
 */
static bool
encode_cld_imm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    uint32_t reg = opnd_get_base(opnd) - DR_REG_X8;
    *out |= SET_FIELD(reg, 9, 7);
    int32_t imm = opnd_get_disp(opnd);
    *out |= SET_FIELD(imm >> 6, 6, 5) | SET_FIELD(imm >> 3, 12, 10);
    return true;
}

/* Encode the base register and offset immediate of c.sw and c.fsw
 * instructions:
 * |15 13|12      10|9    7|6        5|4   2|1      0|
 * | ... | imm[5:3] | rs1' | imm[2|6] | ... | opcode |
 *        ^--------^ ^----^ ^--------^
 * From:
 *        |31  7|6        2|1 0|
 * rs1' + |  0  | imm[6:2] | 0 |
 */
static bool
encode_csw_imm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_dst(instr, idx);
    uint32_t reg = opnd_get_base(opnd) - DR_REG_X8;
    *out |= SET_FIELD(reg, 9, 7);
    int32_t imm = opnd_get_disp(opnd);
    *out |= SET_FIELD(imm >> 6, 5, 5) | SET_FIELD(imm >> 2, 6, 6) |
        SET_FIELD(imm >> 3, 12, 10);
    return true;
}

/* Encode the base register and offset immediate of c.sd and c.fsd
 * instructions:
 * |15 13|12      10|9    7|6        5|4   2|1      0|
 * | ... | imm[5:3] | rs1' | imm[7:6] | ... | opcode |
 *        ^--------^ ^----^ ^--------^
 * From:
 *        |31  8|7        3|2 0|
 * rs1' + |  0  | imm[7:3] | 0 |
 */
static bool
encode_csd_imm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_dst(instr, idx);
    uint32_t reg = opnd_get_base(opnd) - DR_REG_X8;
    *out |= SET_FIELD(reg, 9, 7);
    int32_t imm = opnd_get_disp(opnd);
    *out |= SET_FIELD(imm >> 6, 6, 5) | SET_FIELD(imm >> 3, 12, 10);
    return true;
}

/* Encode the base immediate of c.addi, c.addiw, c.li, c.andi instructions:
 * |15 13|   12   |11  7|6        2|1      0|
 * | ... | imm[5] | ... | imm[4:0] | opcode |
 *        ^------^       ^--------^
 * From:
 * |31     5|4        0|
 * | imm[5] | imm[4:0] |
 */
static bool
encode_cimm5_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_src(instr, idx);
    int32_t imm = opnd_get_immed_int(opnd);
    *out |= SET_FIELD(imm, 6, 2) | SET_FIELD(imm >> 5, 12, 12);
    return true;
}

/* Encode the immediate field of the CB-type format as a pc-relative offset:
 * |15 13|12        10|9   7|6              2|1      0|
 * | ... | imm[8|4:3] | ... | imm[7:6|2:1|5] | opcode |
 *        ^----------^       ^--------------^
 * From:
 * |31     8|7        1| 0 |
 * | imm[8] | imm[7:1] | 0 |
 */
static bool
encode_cb_imm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_target(instr);

    int32_t imm;
    if (opnd.kind == PC_kind)
        imm = opnd_get_pc(opnd) - pc;
    else if (opnd.kind == INSTR_kind)
        imm = (byte *)opnd_get_instr(opnd)->offset - (byte *)instr->offset;
    else
        return false;

    *out |= SET_FIELD(imm >> 5, 2, 2) | SET_FIELD(imm >> 1, 4, 3) |
        SET_FIELD(imm >> 6, 6, 5) | SET_FIELD(imm >> 3, 11, 10) |
        SET_FIELD(imm >> 8, 12, 12);
    return true;
}

/* Encode the immediate field of the CJ-type format as a pc-relative offset:
 * |15 13|12                      2|1      0|
 * | ... | [11|4|9:8|10|6|7|3:1|5] | opcode |
 *        ^-----------------------^
 * From:
 * |31     11|10        1| 0 |
 * | imm[11] | imm[10:1] | 0 |
 */
static bool
encode_cj_imm_opnd(instr_t *instr, byte *pc, int idx, uint32_t *out, decode_info_t *di)
{
    opnd_t opnd = instr_get_target(instr);

    int32_t imm;
    if (opnd.kind == PC_kind)
        imm = opnd_get_pc(opnd) - pc;
    else if (opnd.kind == INSTR_kind)
        imm = (byte *)opnd_get_instr(opnd)->offset - (byte *)instr->offset;
    else
        return false;

    *out |= SET_FIELD(imm >> 5, 2, 2) | SET_FIELD(imm >> 1, 5, 3) |
        SET_FIELD(imm >> 7, 6, 6) | SET_FIELD(imm >> 6, 7, 7) |
        SET_FIELD(imm >> 10, 8, 8) | SET_FIELD(imm >> 8, 10, 9) |
        SET_FIELD(imm >> 4, 11, 11) | SET_FIELD(imm >> 11, 12, 12);
    return true;
}


/* Decode the 6-bit (6th bit always 0 in rv32) shift amount:
 * |15    13|   12   |11    10|9    7|6        2|1      0|
 * | funct3 | imm[5] | funct2 | rs1' | imm[4:0] | opcode |
 *           ^------^                 ^--------^
 */
static bool
decode_cshamt_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                   int idx, instr_t *out)
{
    int32_t imm = (BIT(inst, 12) << 5) | GET_FIELD(inst, 6, 2);
    opnd_t opnd =
        opnd_add_flags(opnd_create_immed_int(imm, op_sz), DR_OPND_IMM_PRINT_DECIMAL);
    instr_set_src(out, idx, opnd);
    return true;
}

/* Decode the CSR immediate in instructions from the Zicsr extension:
 * |31 20|19      15|14  7|6      0|
 * | csr | imm[4:0] | ... | opcode |
 *        ^--------^
 * Into:
 * |31  5|4        0|
 * |  0  | imm[4:0] |
 */
static bool
decode_csr_imm_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                    int idx, instr_t *out)
{
    int32_t imm = GET_FIELD(inst, 19, 15);
    opnd_t opnd =
        opnd_add_flags(opnd_create_immed_int(imm, op_sz), DR_OPND_IMM_PRINT_DECIMAL);
    instr_set_src(out, idx, opnd);
    return true;
}

/* Decode the immediate of the caddi16sp instruction:
 * |15 13|   12   |11  7|6              2|1      0|
 * | ... | imm[9] | ... | imm[4|6|8:7|5] | opcode |
 *        ^------^       ^--------------^
 * Into:
 * |31     9|8        4|3   0|
 * | imm[9] | imm[8:4] |  0  |
 */
static bool
decode_caddi16sp_imm_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc,
                          byte *orig_pc, int idx, instr_t *out)
{
    int32_t imm = (BIT(inst, 12) << 9);
    imm |= (GET_FIELD(inst, 4, 3) << 7);
    imm |= (BIT(inst, 5) << 6);
    imm |= (BIT(inst, 2) << 5);
    imm |= (BIT(inst, 6) << 4);
    imm = SIGN_EXTEND(imm, 10);
    opnd_t opnd =
        opnd_add_flags(opnd_create_immed_int(imm, op_sz), DR_OPND_IMM_PRINT_DECIMAL);
    instr_set_src(out, idx, opnd);
    return true;
}

/* Decode the SP-based immediate offset of c.lwsp and c.flwsp instructions:
 * |15 13|   12   |11  7|6            2|1      0|
 * | ... | imm[5] | ... | imm[4:2|7:6] | opcode |
 *        ^------^       ^------------^
 * Into:
 *      |31  8|7        2|3   0|
 * sp + |  0  | imm[7:2] |  0  |
 */
static bool
decode_clwsp_imm_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                      int idx, instr_t *out)
{
    int32_t imm = GET_FIELD(inst, 3, 2) << 6;
    imm |= BIT(inst, 12) << 5;
    imm |= GET_FIELD(inst, 6, 4) << 2;
    opnd_t opnd =
        opnd_add_flags(opnd_create_base_disp(DR_REG_SP, DR_REG_NULL, 0, imm, OPSZ_4),
                       DR_OPND_IMM_PRINT_DECIMAL);
    instr_set_src(out, idx, opnd);
    return true;
}

/* Decode the SP-based immediate offset of c.ldsp and c.fldsp instructions:
 * |15 13|   12   |11  7|6            2|1      0|
 * | ... | imm[5] | ... | imm[4:3|8:6] | opcode |
 *        ^------^       ^------------^
 * Into:
 *      |31  9|8        2|3   0|
 * sp + |  0  | imm[8:3] |  0  |
 */
static bool
decode_cldsp_imm_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                      int idx, instr_t *out)
{
    int32_t imm = GET_FIELD(inst, 4, 2) << 6;
    imm |= BIT(inst, 12) << 5;
    imm |= GET_FIELD(inst, 6, 5) << 3;
    opnd_t opnd =
        opnd_add_flags(opnd_create_base_disp(DR_REG_SP, DR_REG_NULL, 0, imm, OPSZ_8),
                       DR_OPND_IMM_PRINT_DECIMAL);
    instr_set_src(out, idx, opnd);
    return true;
}

/* Decode the immediate of the c.lui instruction:
 * |15 13|   12    |11  7|6          2|1      0|
 * | ... | imm[17] | ... | imm[16:12] | opcode |
 *        ^-------^       ^----------^
 * Into:
 * |31     17|16        12|11  0|
 * | imm[17] | imm[16:12] |  0  |
 */
static bool
decode_clui_imm_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                     int idx, instr_t *out)
{
    int32_t imm = (BIT(inst, 12) << 5) | GET_FIELD(inst, 6, 2);
    opnd_t opnd = opnd_create_immed_int(imm, op_sz);
    instr_set_src(out, idx, opnd);
    return true;
}

/* Decode the SP-based offset immediate of c.swsp and c.fswsp instructions:
 * |15 13|12           7|6   2|1      0|
 * | ... | imm[5:2|7:6] | ... | opcode |
 *        ^------------^
 * Into:
 *      |31  8|7        2|1 0|
 * sp + |  0  | imm[7:2] | 0 |
 */
static bool
decode_cswsp_imm_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                      int idx, instr_t *out)
{
    int32_t imm = (GET_FIELD(inst, 8, 7) << 6) | (GET_FIELD(inst, 12, 9) << 2);
    opnd_t opnd =
        opnd_add_flags(opnd_create_base_disp(DR_REG_SP, DR_REG_NULL, 0, imm, OPSZ_4),
                       DR_OPND_IMM_PRINT_DECIMAL);
    instr_set_dst(out, idx, opnd);
    return true;
}

/* Decode the SP-based offset immediate of c.sdsp and c.fsdsp instructions:
 * |15 13|12           7|6   2|1      0|
 * | ... | imm[5:3|8:6] | ... | opcode |
 *        ^------------^
 * Into:
 *      |31  9|8        3|2 0|
 * sp + |  0  | imm[7:3] | 0 |
 */
static bool
decode_csdsp_imm_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                      int idx, instr_t *out)
{
    int32_t imm = (GET_FIELD(inst, 9, 7) << 6) | (GET_FIELD(inst, 12, 10) << 3);
    opnd_t opnd =
        opnd_add_flags(opnd_create_base_disp(DR_REG_SP, DR_REG_NULL, 0, imm, OPSZ_8),
                       DR_OPND_IMM_PRINT_DECIMAL);
    instr_set_dst(out, idx, opnd);
    return true;
}

/* Decode the immediate of the c.addi4spn instruction:
 * |15 13|12               5|4   2|1      0|
 * | ... | imm[5:4|9:6|2|3] | ... | opcode |
 *        ^----------------^
 * Into:
 * |31 10|9        2|1 0|
 * |  0  | imm[9:2] | 0 |
 */
static bool
decode_ciw_imm_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                    int idx, instr_t *out)
{
    int32_t imm = GET_FIELD(inst, 10, 7) << 6;
    imm |= GET_FIELD(inst, 12, 11) << 4;
    imm |= BIT(inst, 5) << 3;
    imm |= BIT(inst, 6) << 2;
    opnd_t opnd =
        opnd_add_flags(opnd_create_immed_int(imm, op_sz), DR_OPND_IMM_PRINT_DECIMAL);
    instr_set_src(out, idx, opnd);
    return true;
}

/* Decode the base register and offset immediate of c.lw and c.flw
 * instructions:
 * |15 13|12      10|9   7|6        5|4   2|1      0|
 * | ... | imm[5:3] | rs1' | imm[2|6] | ... | opcode |
 *        ^--------^ ^----^ ^--------^
 * Into:
 *        |31  7|6        2|1 0|
 * rs1' + |  0  | imm[6:2] | 0 |
 */
static bool
decode_clw_imm_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                    int idx, instr_t *out)
{
    reg_t reg = DR_REG_X8 + GET_FIELD(inst, 9, 7);
    int32_t imm = BIT(inst, 5) << 6;
    imm |= GET_FIELD(inst, 12, 10) << 3;
    imm |= BIT(inst, 6) << 2;
    opnd_t opnd = opnd_add_flags(opnd_create_base_disp(reg, DR_REG_NULL, 0, imm, OPSZ_4),
                                 DR_OPND_IMM_PRINT_DECIMAL);
    instr_set_src(out, idx, opnd);
    return true;
}

/* Decode the base register and offset immediate of c.ld and c.fld
 * instructions:
 * |15 13|12      10|9    7|6        5|4   2|1      0|
 * | ... | imm[5:3] | rs1' | imm[7:6] | ... | opcode |
 *        ^--------^ ^----^ ^--------^
 * Into:
 *        |31  8|7        3|2 0|
 * rs1' + |  0  | imm[7:3] | 0 |
 */
static bool
decode_cld_imm_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                    int idx, instr_t *out)
{
    reg_t reg = DR_REG_X8 + GET_FIELD(inst, 9, 7);
    int32_t imm = (GET_FIELD(inst, 6, 5) << 6) | (GET_FIELD(inst, 12, 10) << 3);
    opnd_t opnd = opnd_add_flags(opnd_create_base_disp(reg, DR_REG_NULL, 0, imm, OPSZ_8),
                                 DR_OPND_IMM_PRINT_DECIMAL);
    instr_set_src(out, idx, opnd);
    return true;
}

/* Decode the base register and offset immediate of c.sw and c.fsw
 * instructions:
 * |15 13|12      10|9    7|6        5|4   2|1      0|
 * | ... | imm[5:3] | rs1' | imm[2|6] | ... | opcode |
 *        ^--------^ ^----^ ^--------^
 * Into:
 *        |31  7|6        2|1 0|
 * rs1' + |  0  | imm[6:2] | 0 |
 */
static bool
decode_csw_imm_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                    int idx, instr_t *out)
{
    reg_t reg = DR_REG_X8 + GET_FIELD(inst, 9, 7);
    int32_t imm = BIT(inst, 5) << 6;
    imm |= GET_FIELD(inst, 12, 10) << 3;
    imm |= BIT(inst, 6) << 2;
    opnd_t opnd = opnd_add_flags(opnd_create_base_disp(reg, DR_REG_NULL, 0, imm, OPSZ_4),
                                 DR_OPND_IMM_PRINT_DECIMAL);
    instr_set_dst(out, idx, opnd);
    return true;
}

/* Decode the base register and offset immediate of c.sd and c.fsd
 * instructions:
 * |15 13|12      10|9    7|6        5|4   2|1      0|
 * | ... | imm[5:3] | rs1' | imm[7:6] | ... | opcode |
 *        ^--------^ ^----^ ^--------^
 * Into:
 *        |31  8|7        3|2 0|
 * rs1' + |  0  | imm[7:3] | 0 |
 */
static bool
decode_csd_imm_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                    int idx, instr_t *out)
{
    reg_t reg = DR_REG_X8 + GET_FIELD(inst, 9, 7);
    int32_t imm = (GET_FIELD(inst, 6, 5) << 6) | (GET_FIELD(inst, 12, 10) << 3);
    opnd_t opnd = opnd_add_flags(opnd_create_base_disp(reg, DR_REG_NULL, 0, imm, OPSZ_8),
                                 DR_OPND_IMM_PRINT_DECIMAL);
    instr_set_dst(out, idx, opnd);
    return true;
}

/* Decode the base immediate of c.addi, c.addiw, c.li, c.andi instructions:
 * |15 13|   12   |11  7|6        2|1      0|
 * | ... | imm[5] | ... | imm[4:0] | opcode |
 *        ^------^       ^--------^
 * Into:
 * |31     5|4        0|
 * | imm[5] | imm[4:0] |
 */
static bool
decode_cimm5_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                  int idx, instr_t *out)
{
    int32_t imm = SIGN_EXTEND((BIT(inst, 12) << 5) | GET_FIELD(inst, 6, 2), 6);
    opnd_t opnd =
        opnd_add_flags(opnd_create_immed_int(imm, op_sz), DR_OPND_IMM_PRINT_DECIMAL);
    instr_set_src(out, idx, opnd);
    return true;
}

/* Decode the immediate field of the CB-type format as a pc-relative offset:
 * |15 13|12        10|9   7|6              2|1      0|
 * | ... | imm[8|4:3] | ... | imm[7:6|2:1|5] | opcode |
 *        ^----------^       ^--------------^
 * Into:
 * |31     8|7        1| 0 |
 * | imm[8] | imm[7:1] | 0 |
 */
static bool
decode_cb_imm_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                   int idx, instr_t *out)
{
    int32_t imm = BIT(inst, 12) << 8;
    imm |= GET_FIELD(inst, 6, 5) << 6;
    imm |= BIT(inst, 2) << 5;
    imm |= GET_FIELD(inst, 11, 10) << 3;
    imm |= GET_FIELD(inst, 4, 3) << 1;
    imm = SIGN_EXTEND(imm, 9);

    opnd_t opnd = opnd_create_pc(orig_pc + imm);
    instr_set_target(out, opnd);
    return true;
}

/* Decode the immediate field of the CJ-type format as a pc-relative offset:
 * |15 13|12                      2|1      0|
 * | ... | [11|4|9:8|10|6|7|3:1|5] | opcode |
 *        ^-----------------------^
 * Into:
 * |31     11|10        1| 0 |
 * | imm[11] | imm[10:1] | 0 |
 */
static bool
decode_cj_imm_opnd(dcontext_t *dc, uint32_t inst, int op_sz, byte *pc, byte *orig_pc,
                   int idx, instr_t *out)
{
    int32_t imm = BIT(inst, 12) << 11;
    imm |= BIT(inst, 8) << 10;
    imm |= GET_FIELD(inst, 10, 9) << 8;
    imm |= BIT(inst, 6) << 7;
    imm |= BIT(inst, 7) << 6;
    imm |= BIT(inst, 2) << 5;
    imm |= BIT(inst, 11) << 4;
    imm |= GET_FIELD(inst, 5, 3) << 1;
    imm = SIGN_EXTEND(imm, 12);

    opnd_t opnd = opnd_create_pc(orig_pc + imm);
    instr_set_target(out, opnd);
    return true;
}

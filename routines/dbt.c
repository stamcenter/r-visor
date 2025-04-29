#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "./../src/decode.h"
#include "./../src/instructions.h"
#include "./../src/code_cache.h"
#include "./../src/logger.h"
#include "./../src/printUtils.h"
#include "./../src/rail.h"
#include "./../src/regfile.h"
// #include "./../src/system_calls.h"

// rail::RvDecoder decoder;

int num_instructions;
int num_instructions_nr;
static int num_basic_blocks;


void exitFxn(uint64_t *regfile) {
fprintf(rvisor_logger, "Number of Basic Blocks: %d\n", num_basic_blocks);
fprintf(rvisor_logger, "Number of Instructions Replaced: %d\n", num_instructions);
fprintf(rvisor_logger, "Number of Instructions Not Replaced: %d\n", num_instructions_nr);
// fprintf(outfile, "Stub Code: %x\n", rail::CodeCache::stubCode);
float cached = rvisor_memory_index - rvisor_stub_code;
float actual = (cached + num_instructions * 2);
fprintf(rvisor_logger, "Code Cached: %x\n", (int)cached);
fprintf(rvisor_logger, "Original Size: %x\n", (int)actual);

float reduction = ((actual - cached) / actual) * 100;

fprintf(rvisor_logger, "Percentage reduction: %.2f\n", reduction);
fprintf(rvisor_logger, "EXIT PROGRAM\n");
}

uint16_t newInst;

int dbt(RvInst railInst) {

  uint32_t imm;
  int rep = 0;
  switch (railInst.name) {
//   case LUI:
//     if ((abs(railInst.imm) < 32) && (railInst.imm != 0) && (railInst.rd != 0) &&
//         (railInst.rd != 2)) {
//       newInst = ((0b01) << 0) | (((railInst.imm) & 0b11111) << 2) |
//                 (((railInst.rd) & 0b11111) << 7) |
//                 (((railInst.imm >> 6) & 0b1) << 12) | ((0b011) << 13);
//       // cout << dec << railInst.imm << "\t" << hex << newInst << endl;
//       rep = 1;
//     }
//     break;
  case ADDI:
    if ((railInst.rd == SP) && (railInst.rs1 == SP)) {

      if (!(railInst.imm % 16) && (abs(railInst.imm) < 512)) {
        // outfile << "(ADDI16SP)\t";
        // newInst = encode_C_ADDI16SPN(railInst.rd, railInst.imm);
        newInst = ((0b01) << 0) | (((railInst.imm >> 5) & 0b1) << 2) |
                  (((railInst.imm >> 7) & 0b11) << 3) |
                  (((railInst.imm >> 6) & 0b1) << 5) |
                  (((railInst.imm >> 4) & 0b1) << 6) | ((0b10) << 7) |
                  (((railInst.imm >> 9) & 0b1) << 12) | ((0b011) << 13);

        // cout << hex << newInst << endl;

        rep = 1;
      } else if (!(railInst.imm % 4) && (abs(railInst.imm) < 512)) {
        // outfile << "(ADDI4SP)\t";
        newInst = encode_C_ADDI4SPN(railInst.rd, railInst.imm);
        rep = 1;
      }
    } else if (abs(railInst.imm) < 32 && railInst.imm != 0 &&
               (railInst.rd == railInst.rs1) && railInst.rd != SP) {
      newInst = encode_C_ADDI(railInst.imm, railInst.rd);
      rep = 1;
    } else if (railInst.rd == ZERO && railInst.rs1 == ZERO) {
      newInst = encode_C_NOP();
      rep = 1;
    } else if (railInst.rs1 == ZERO && abs(railInst.imm) < 32) {
      newInst = encode_C_LI(railInst.imm, railInst.rd);
      rep = 1;
    } else if (railInst.imm == 0 && railInst.rs1 != ZERO) {
      newInst = encode_C_MV(railInst.rs1, railInst.rd);
      rep = 1;
    }
    break;
  case ADDIW:
    if ((railInst.rd == railInst.rs1) && (railInst.imm < 32) &&
        (railInst.imm >= 0)) {
      // outfile << "(ADDIW) " << (railInst.imm & 0x0000003f) << "\t";
      //   newInst == encode_C_ADDIW(railInst.imm, railInst.rd);
      newInst = ((0b01) << 0) | (((railInst.imm >> 0) & 0b11111) << 2) |
                (((railInst.imm >> 5) & 0b1) << 12) |
                ((railInst.rd & 0b11111) << 7) | ((0b001) << 13) | 0;
      //   outfile << hex << newInst << "\t";
      rep = 1;
    }
    break;
  case LW:
    if (railInst.rs1 == SP && abs(railInst.imm) < 256) {
      // newInst = encode_CIStype(0b10, railInst.imm, railInst.rd, 0b010);
      newInst = encode_C_LWSP(railInst.imm, railInst.rd);
      rep = 1;
    } else if (railInst.rs1 > 7 && railInst.rd > 7 && railInst.rs1 < 16 &&
               railInst.rd < 16) {
      if (railInst.imm > 0 && railInst.imm < 64) {
        newInst = encode_C_LW(railInst.rd, railInst.imm, railInst.rs1);
        rep = 1;
      }
    }
    break;
  case LD:
    if (railInst.rs1 == SP && abs(railInst.imm) < 512) {
      // newInst = encode_CISDtype(0b10, railInst.imm, railInst.rd, 0b011);
      newInst = encode_C_LDSP(railInst.imm, railInst.rd);
      rep = 1;
    } else if (railInst.rs1 > 7 && railInst.rd > 7 && railInst.rs1 < 16 &&
               railInst.rd < 16) {
      if (railInst.imm > 0 && railInst.imm < 128) {
        newInst = encode_C_LD(railInst.rd, railInst.imm, railInst.rs1);
        rep = 1;
      }
    }
    break;
  case SW:
    if (railInst.rs1 == SP && abs(railInst.imm) < 256) {
      // newInst = encode_CSStype(0b10, railInst.rs2, railInst.imm, 0b110);
      newInst = encode_C_SWSP(railInst.rs2, railInst.imm);
      rep = 1;
    } else if (railInst.rs1 > 7 && railInst.rs2 > 7 && railInst.rs1 < 16 &&
               railInst.rs2 < 16) {
      if (railInst.imm > 0 && railInst.imm < 64) {
        newInst = encode_C_SW(railInst.rs2, railInst.imm, railInst.rs1);
        rep = 1;
      }
    }
    break;
  case SD:
    if (railInst.rs1 == SP && abs(railInst.imm) < 512) {
      // newInst = encode_CSSDtype(0b10, railInst.rs2, railInst.imm, 0b111);
      newInst = encode_C_SDSP(railInst.rs2, railInst.imm);
      rep = 1;
    } else if (railInst.rs1 > 7 && railInst.rs2 > 7 && railInst.rs1 < 16 &&
               railInst.rs2 < 16) {
      if (railInst.imm >= 0 && railInst.imm < 128) {
        newInst = encode_C_SD(railInst.rs2, railInst.imm, railInst.rs1);
        rep = 1;
      }
    }
    break;
  case FLD: // NT
    if (railInst.rs1 == SP) {
      // newInst = encode_CISDtype(0b10, railInst.imm, railInst.rd, 0b001);
      newInst = encode_C_FLDSP(railInst.imm, railInst.rs2);
      rep = 1;
    } else if (railInst.rs1 > 7 && railInst.rd > 7 && railInst.rs1 < 16 &&
               railInst.rd < 16) {
      if (railInst.imm > 0 && railInst.imm < 128) {
        newInst = encode_C_LD(railInst.rd, railInst.imm, railInst.rs1);
        rep = 1;
      }
    }
    break;
  case FSD: // NT
    if (railInst.rs1 == SP) {
      // newInst = encode_CSSDtype(0b10, railInst.rs2, railInst.imm, 0b111);
      newInst = encode_C_FSDSP(railInst.rs2, railInst.imm);
      rep = 1;
    } else if (railInst.rs1 > 7 && railInst.rs2 > 7 && railInst.rs1 < 16 &&
               railInst.rs2 < 16) {
      if (railInst.imm > 0 && railInst.imm < 128) {
        newInst = encode_C_FSD(railInst.rs2, railInst.imm, railInst.rs1);
        rep = 1;
      }
    }
    break;
  case SUB:
    if (railInst.rs1 > 7 && railInst.rs2 > 7 && railInst.rs1 < 16 &&
        railInst.rs2 < 16) {
      if (railInst.rs1 == railInst.rd) {
        newInst = encode_C_SUB(railInst.rs2, railInst.rd);
        rep = 1;
      }
    }
    break;
  case XOR:
    if (railInst.rs1 > 7 && railInst.rs2 > 7 && railInst.rs1 < 16 &&
        railInst.rs2 < 16) {
      if (railInst.rs1 == railInst.rd) {
        newInst = encode_C_XOR(railInst.rs2, railInst.rd);
        rep = 1;
      } else if (railInst.rs2 == railInst.rd) {
        newInst = encode_C_XOR(railInst.rs1, railInst.rd);
        rep = 1;
      }
    }
    break;
  case OR:
    if (railInst.rs1 > 7 && railInst.rs2 > 7 && railInst.rs1 < 16 &&
        railInst.rs2 < 16) {
      if (railInst.rs1 == railInst.rd) {
        newInst = encode_C_OR(railInst.rs2, railInst.rd);
        rep = 1;
      } else if (railInst.rs2 == railInst.rd) {
        newInst = encode_C_OR(railInst.rs1, railInst.rd);
        rep = 1;
      }
    }
    break;
  case AND:
    //   cout << "HERE" << endl;
    if (railInst.rs1 > 7 && railInst.rs2 > 7 && railInst.rs1 < 16 &&
        railInst.rs2 < 16) {
      if (railInst.rs1 == railInst.rd) {
        newInst = encode_C_AND(railInst.rs2, railInst.rd);
        rep = 1;
      } else if (railInst.rs2 == railInst.rd) {
        newInst = encode_C_AND(railInst.rs1, railInst.rd);
        rep = 1;
      }
    }
    break;
  case ANDI:
    // cout << "ANDI\n";
    if ((railInst.rs1 > 7) && (railInst.rd > 7) && (railInst.rs1 < 16) &&
        (railInst.rd < 16)) {
      if ((railInst.rs1 == railInst.rd) && (abs(railInst.imm) < 32)) {
        newInst = ((0b01) << 0) | (((railInst.imm) & 0b11111) << 2) |
                  ((railInst.rs1 & 0b111) << 7) | ((0b10) << 10) |
                  (((railInst.imm >> 5) & 0b1) << 12) | ((0b100) << 13);
        rep = 1;
      }
    }
    break;
  case SUBW:
    if (railInst.rs1 > 7 && railInst.rs2 > 7 && railInst.rs1 < 16 &&
        railInst.rs2 < 16) {
      if (railInst.rs1 == railInst.rd) {
        newInst = encode_C_SUBW(railInst.rs2, railInst.rd);
        rep = 1;
      }
    }
    break;
  case ADDW:
    if (railInst.rs1 > 7 && railInst.rs2 > 7 && railInst.rs1 < 16 &&
        railInst.rs2 < 16) {
      if (railInst.rs1 == railInst.rd) {
        newInst = encode_C_ADDW(railInst.rs2, railInst.rd);
        rep = 1;
      } else if (railInst.rs2 == railInst.rd) {
        newInst = encode_C_ADDW(railInst.rs1, railInst.rd);
        rep = 1;
      }
    }
    break;
  case ADD:
      if (railInst.rs1 == railInst.rd) {
        newInst = encode_C_ADD(railInst.rs2, railInst.rd);
        rep = 1;
      } else if (railInst.rs2 == railInst.rd) {
        newInst = encode_C_ADD(railInst.rs1, railInst.rd);
        rep = 1;
      }
    // }
    break;
  case SRLI:
    if ((railInst.rs1 > 7) && (railInst.rd > 7) && (railInst.rs1 < 16) &&
        (railInst.rd < 16)) {
      if ((railInst.rs1 == railInst.rd) && (abs(railInst.imm) < 32)) {
        newInst = ((0b01) << 0) | (((railInst.imm) & 0b11111) << 2) |
                  ((railInst.rs1 & 0b111) << 7) | ((0b00) << 10) |
                  (((railInst.imm >> 5) & 0b1) << 12) | ((0b100) << 13);
      }
    }
    break;
    case SRAI:
      if ((railInst.rs1 > 7) && (railInst.rd > 7) && (railInst.rs1 < 16) &&
          (railInst.rd < 16)) {
        if ((railInst.rs1 == railInst.rd) && (abs(railInst.imm) < 32)) {
          newInst = ((0b01) << 0) | (((railInst.imm) & 0b11111) << 2) |
                    ((railInst.rs1 & 0b111) << 7) | ((0b10) << 10) |
                    (((railInst.imm >> 5) & 0b1) << 12) | ((0b100) << 13);
        }
      }
    break;
  case SLLI:
    break;
  default:
    break;
  }
//   outfile << hex << railer.elfReaderPtr->getProgramCounter() << "\t";
//   printf("%x\t",rvisor_elf_reader.program_counter);
    // printf("%s\n", InstNameString[railInst.name]);

  if (rep) {
    num_instructions++;
    rvisor_insert_inst16(newInst);
    // printInstruction(railInst);
    // decode_instruction(newInst, 1);
    // outfile << "-------------------------------\n";
    return 1;
  } else {
    num_instructions_nr++;
    // fprintf(rvisor_logger, "NOT REPLACED\t");
    // printInstruction(railInst);
    // outfile << "-------------------------------\n";
    return 0;
  }
}

int main(int argc, char **argv, char **envp) {

    if (argc < 2) {
    printf("Please provide a target binary\n");
    exit(1);
    }

    rvisor_init(argv[1]);
    rvisor_register_args(argc, argv, envp);

    set_logging_file("rvisor_dbt_logs.txt", "w");
    rvisor_register_inst_group_routine(DBT_GROUP, dbt, POST, ALLOCATOR);
    rvisor_register_exit_routine(exitFxn);
    rvisor_run();
}
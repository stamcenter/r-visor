#include "printUtils.h"


void printRType(RvInst r_type) {
    fprintf(rvisor_logger,"%s %s, %s, %s\n", InstNameString[r_type.name],
           GPregnames[r_type.rd], GPregnames[r_type.rs1], GPregnames[r_type.rs2]);
}

void printIType(RvInst i_type) {
    if (i_type.opcode == LOAD_OP) {
        fprintf(rvisor_logger,"%s %s, %d(%s)\n", InstNameString[i_type.name],
               GPregnames[i_type.rd], i_type.imm, GPregnames[i_type.rs1]);
    } else if (i_type.opcode == SYSTEM_OP) {
        fprintf(rvisor_logger,"%s\n", InstNameString[i_type.name]);
    } else {
        fprintf(rvisor_logger,"%s %s, %s, %d\n", InstNameString[i_type.name],
               GPregnames[i_type.rd], GPregnames[i_type.rs1], i_type.imm);
    }
}

void printSType(RvInst s_type) {
    fprintf(rvisor_logger,"%s %s, %d(%s)\n", InstNameString[s_type.name],
           GPregnames[s_type.rs2], s_type.imm, GPregnames[s_type.rs1]);
}

void printUType(RvInst u_type) {
    fprintf(rvisor_logger,"%s %s, %d\n", InstNameString[u_type.name],
           GPregnames[u_type.rd], u_type.imm);
}

void printBType(RvInst b_type) {
    fprintf(rvisor_logger,"%s %s, %s, %d\n", InstNameString[b_type.name],
           GPregnames[b_type.rs1], GPregnames[b_type.rs2], b_type.imm);
}

void printJType(RvInst j_type) {
    fprintf(rvisor_logger,"%s %s, %d\n", InstNameString[j_type.name],
           GPregnames[j_type.rd], j_type.imm);
}

void printInstruction(RvInst inst) {
    switch (inst.type) {
        case R_TYPE: printRType(inst); break;
        case I_TYPE: printIType(inst); break;
        case S_TYPE: printSType(inst); break;
        case U_TYPE: printUType(inst); break;
        case B_TYPE: printBType(inst); break;
        case J_TYPE: printJType(inst); break;
        default:
            fprintf(rvisor_logger,"INVALID INSTRUCTION\n");
    }
}

void printRTypeHex(RvInst r_type) {
    fprintf(rvisor_logger,"R Type\nOpcode: %07x\nFunct3: %03x\nFunct7: %07x\nRd: %05x\nRs1: %05x\nRs2: %05x\n",
           r_type.opcode, r_type.funct3, r_type.funct7, r_type.rd, r_type.rs1, r_type.rs2);
}

void printITypeHex(RvInst i_type) {
    fprintf(rvisor_logger,"I Type\nOpcode: %07x\nFunct3: %03x\nRd: %05x\nImm: %012x\nRs1: %05x\n",
           i_type.opcode, i_type.funct3, i_type.rd, i_type.imm, i_type.rs1);
}

void printSTypeHex(RvInst s_type) {
    fprintf(rvisor_logger,"S Type\nOpcode: %07x\nFunct3: %03x\nImm: %012x\nRs1: %05x\nRs2: %05x\n",
           s_type.opcode, s_type.funct3, s_type.imm, s_type.rs1, s_type.rs2);
}

void printUTypeHex(RvInst u_type) {
    fprintf(rvisor_logger,"U Type\nOpcode: %07x\nRd: %05x\nImm: %020x\n",
           u_type.opcode, u_type.rd, u_type.imm);
}

void printBTypeHex(RvInst b_type) {
    fprintf(rvisor_logger,"B Type\nOpcode: %07x\nFunct3: %03x\nImm: %012x\nRs1: %05x\nRs2: %05x\n",
           b_type.opcode, b_type.funct3, b_type.imm, b_type.rs1, b_type.rs2);
}

void printJTypeHex(RvInst j_type) {
    fprintf(rvisor_logger,"J Type\nOpcode: %07x\nImm: %020x\nRd: %05x\n",
           j_type.opcode, j_type.imm, j_type.rd);
}

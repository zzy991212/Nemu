#include "cpu/exec/template-start.h"

#define instr jbe

static void do_execute () {
	DATA_TYPE_S imm = op_src -> val;
    print_asm("jbe\t%x", cpu.eip + 1 + DATA_BYTE + imm);
    if (cpu.CF == 1 || cpu.ZF == 1) cpu.eip += imm;
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"

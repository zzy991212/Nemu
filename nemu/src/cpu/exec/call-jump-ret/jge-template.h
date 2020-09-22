#include "cpu/exec/template-start.h"

#define instr jge

static void do_execute () {
	DATA_TYPE_S imm = op_src -> val;
    print_asm("jge\t%x", cpu.eip + 1 + DATA_BYTE + imm);
    if (cpu.ZF == 1 || cpu.SF == cpu.OF) cpu.eip += imm;
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"

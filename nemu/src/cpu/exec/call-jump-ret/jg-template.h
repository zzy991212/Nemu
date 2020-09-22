#include "cpu/exec/template-start.h"

#define instr jg

static void do_execute () {
	DATA_TYPE_S imm = op_src -> val;
    print_asm("jg\t%x", cpu.eip + 1 + DATA_BYTE + imm);
    if (cpu.ZF == 0 && cpu.SF == cpu.OF) cpu.eip += imm;
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"

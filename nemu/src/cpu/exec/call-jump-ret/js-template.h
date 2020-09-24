#include "cpu/exec/template-start.h"

#define instr js

static void do_execute () {
	DATA_TYPE_S imm = op_src -> val;
    print_asm("js\t%x", cpu.eip + 1 + DATA_BYTE + imm);
    if (cpu.SF == 1) cpu.eip += imm;
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"

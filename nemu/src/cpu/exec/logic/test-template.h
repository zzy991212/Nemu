#include "cpu/exec/template-start.h"

#define instr test

static void do_execute () {
	DATA_TYPE ret = op_dest -> val & op_src -> val;
    cpu.SF = ret >> ((DATA_BYTE << 3) - 1);
    cpu.ZF = !ret;
    cpu.CF = 0;
    cpu.OF = 0;
    ret ^= ret >> 4;
    ret ^= ret >> 2;
    ret ^= ret >> 1;
    ret &= 1;
    cpu.PF = !ret;
    print_asm_template2();
}

make_instr_helper(r2rm)
make_instr_helper(i2rm)
make_instr_helper(i2a)

#include "cpu/exec/template-end.h"

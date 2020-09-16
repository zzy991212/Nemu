#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute () {
	DATA_TYPE ret = op_dest -> val - op_src -> val;
    cpu.ZF = !ret;
    cpu.CF = op_dest -> val < op_src -> val;
    cpu.SF = ret >> ((DATA_BYTE << 3) - 1);
    int tmp1 = (op_dest -> val) >> ((DATA_BYTE << 3) - 1);
    int tmp2 = (op_src -> val) >> ((DATA_BYTE << 3) - 1);
    cpu.OF = (tmp1 != tmp2 && tmp2 == cpu.SF);
    ret ^= ret >> 4;
    ret ^= ret >> 2;
    ret ^= ret >> 1;
    ret &= 1;
    cpu.PF = !ret;
    print_asm_template2();    
}

make_instr_helper(i2rm)

#include "cpu/exec/template-end.h"

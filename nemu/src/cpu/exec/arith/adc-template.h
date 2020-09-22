#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute () {
	DATA_TYPE ret = op_dest -> val + op_src -> val + cpu.CF;
	OPERAND_W(op_dest, ret);

	/* TODO: Update EFLAGS. */
    cpu.ZF = !ret;
    cpu.SF = ret >> ((DATA_BYTE << 3) - 1);
    cpu.CF = (ret < op_dest -> val);
    int tmp1 = (op_dest -> val) >> ((DATA_BYTE << 3) - 1);
    int tmp2 = (op_src -> val) >> ((DATA_BYTE << 3) - 1);
    cpu.OF = (tmp1 == tmp2 && tmp1 != cpu.SF);
    ret ^= ret >> 4;
    ret ^= ret >> 2;
    ret ^= ret >> 1;
    ret &= 1;
    cpu.PF = !ret;
	print_asm_template2();
}

make_instr_helper(i2a);
make_instr_helper(i2rm);
make_instr_helper(r2rm);
make_instr_helper(rm2r);

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

#include "cpu/exec/template-end.h"

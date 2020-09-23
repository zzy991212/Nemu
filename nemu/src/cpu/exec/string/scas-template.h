#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat(scas_m_, SUFFIX)) {
    op_src -> val = (uint32_t)MEM_R(reg_l(R_EDI));
    op_dest -> val = (uint32_t)REG(R_EAX);

    uint32_t ret = op_dest -> val - op_src -> val;

	/* TODO: Update EFLAGS. */
    cpu.ZF = !ret;
    cpu.SF = ret >> ((DATA_BYTE << 3) - 1);
    cpu.CF = (op_dest -> val < op_src -> val);
    int tmp1 = (op_dest -> val) >> ((DATA_BYTE << 3) - 1);
    int tmp2 = (op_src -> val) >> ((DATA_BYTE << 3) - 1);
    cpu.OF = (tmp1 != tmp2 && tmp2 == cpu.SF);
    ret ^= ret >> 4;
    ret ^= ret >> 2;
    ret ^= ret >> 1;
    ret &= 1;
    cpu.PF = !ret;

    int op = 1;
    if (cpu.DF == 1) op = -1;
    reg_l(R_EDI) += op * DATA_BYTE;
	print_asm("scas");
    return 1;
}



#include "cpu/exec/template-end.h"
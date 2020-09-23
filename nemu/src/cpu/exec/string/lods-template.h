#include "cpu/exec/template-start.h"

#define instr lods

make_helper(concat(lods_m_, SUFFIX)) {
    REG(R_EAX) = MEM_R(reg_l(R_ESI));
    int op = 1;
    if (cpu.DF == 1) op = -1;
    reg_l(R_ESI) += op * DATA_BYTE;
	print_asm_template2();
    return 1;
}



#include "cpu/exec/template-end.h"
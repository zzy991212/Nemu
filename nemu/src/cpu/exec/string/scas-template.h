#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat(scas_m_, SUFFIX)) {

    if (!reg_l(R_ECX) && cpu.ZF == 0)
        reg_l(R_EDI) += DATA_BYTE;
	print_asm("scas");
    return 1;
}



#include "cpu/exec/template-end.h"
#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_n_, SUFFIX)) {

    swaddr_write(REG(R_EDI),DATA_BYTE,MEM_R(REG(R_ESI)));
	if (cpu.DF == 0) {
        REG(R_EDI) += DATA_BYTE;
        REG(R_ESI) += DATA_BYTE;
    }else {
        REG(R_EDI) -= DATA_BYTE;
        REG(R_ESI) -= DATA_BYTE;
    }
	print_asm("movs");
    return 1;
}



#include "cpu/exec/template-end.h"
#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_n_, SUFFIX)) {

    swaddr_write(REG(R_DI),DATA_BYTE,MEM_R(REG(R_SI)));
	
	print_asm("movs");
    return 0;
}



#include "cpu/exec/template-end.h"
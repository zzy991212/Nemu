#include "cpu/exec/template-start.h"

#define instr ret

make_helper(concat(ret_n_, SUFFIX)) {

    cpu.eip = MEM_R(REG(R_EBP));
    REG(R_EBP) += DATA_BYTE;
    print_asm("ret");
    return 1;
} 

#include "cpu/exec/template-end.h"
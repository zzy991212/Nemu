#include "cpu/exec/template-start.h"

#define instr ret

make_helper(concat(ret_n_, SUFFIX)) {

    cpu.eip = MEM_R(REG(R_ESP));
    REG(R_ESP) += DATA_BYTE;
    print_asm("ret");
    return 1;
} 

#include "cpu/exec/template-end.h"
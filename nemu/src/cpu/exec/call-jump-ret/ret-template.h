#include "cpu/exec/template-start.h"

#define instr ret

make_helper(concat(ret_n_, SUFFIX)) {

    cpu.eip = MEM_R(reg_l(R_ESP));
    printf("MEM 0x%x\t\t sw 0x%x\n",MEM_R(reg_l(R_ESP)),swaddr_read(reg_l(R_ESP),4));
    reg_l(R_ESP) += DATA_BYTE;
    print_asm("ret");
    return 1;
} 

#include "cpu/exec/template-end.h"
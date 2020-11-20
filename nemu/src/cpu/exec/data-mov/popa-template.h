#include "cpu/exec/template-start.h"

#define instr popa

#if DATA_BYTE == 2
static uint32_t pops_w(){
    current_sreg = R_SS;
    uint32_t ret = swaddr_read(reg_l(R_ESP),DATA_BYTE);
    swaddr_write(reg_l(R_ESP),DATA_BYTE,0);
    reg_l(R_ESP) += DATA_BYTE;
    return ret;
}
#endif
#if DATA_BYTE == 4
static uint32_t pops_l(){
    current_sreg = R_SS;
    uint32_t ret = swaddr_read(reg_l(R_ESP),DATA_BYTE);
    swaddr_write(reg_l(R_ESP),DATA_BYTE,0);
    reg_l(R_ESP) += DATA_BYTE;
    return ret;
}
#endif

make_helper(concat(popa_,SUFFIX)){
    current_sreg = R_SS;
    REG(R_EDI) = concat(pops_,SUFFIX)();
    REG(R_ESI) = concat(pops_,SUFFIX)();
    REG(R_EBP) = concat(pops_,SUFFIX)();
    uint32_t throwaway = concat(pops_,SUFFIX)();
    throwaway = 0;
    REG(R_EBX) = concat(pops_,SUFFIX)();
    REG(R_EDX) = concat(pops_,SUFFIX)();
    REG(R_ECX) = concat(pops_,SUFFIX)();
    REG(R_EAX) = concat(pops_,SUFFIX)();
    print_asm("popa");
    return 1;
}


#include "cpu/exec/template-end.h"
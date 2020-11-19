#include "cpu/exec/template-start.h"

#define instr popa

#if DATA_BYTE == 2
static uint32_t pops(){
    current_sreg = R_SS;
    uint32_t ret = swaddr_read(reg_l(R_ESP),4);
    swaddr_write(reg_l(R_ESP),4,0);
    reg_l(R_ESP) += 4;
    return ret;
}
#endif

make_helper(concat(popa_,SUFFIX)){
    current_sreg = R_SS;
    REG(R_EDI) = pops();
    REG(R_ESI) = pops();
    REG(R_EBP) = pops();
    uint32_t throwaway = pops();
    throwaway = 0;
    REG(R_EBX) = pops();
    REG(R_EDX) = pops();
    REG(R_ECX) = pops();
    REG(R_EAX) = pops();
    print_asm("popa");
    return 0;
}


#include "cpu/exec/template-end.h"
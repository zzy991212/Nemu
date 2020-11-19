#include "cpu/exec/template-start.h"

#define instr pusha

#if DATA_BYTE == 2
static void push_w(uint32_t val){
    REG(R_ESP) -= DATA_BYTE;
    swaddr_write(REG(R_ESP),DATA_BYTE,val);
}
#endif

#if DATA_BYTE == 4
static void push_l(uint32_t val){
    REG(R_ESP) -= DATA_BYTE;
    swaddr_write(REG(R_ESP),DATA_BYTE,val);
}
#endif

make_helper(concat(pusha_,SUFFIX)){
    current_sreg = R_SS;
    // DATA_TYPE tmp = REG(R_ESP);
    concat(push_,SUFFIX)(REG(R_EAX));
    // push(REG(R_EAX));
    // push(REG(R_ECX));
    // push(REG(R_EDX));
    // push(REG(R_EBX));
    // push(tmp);
    // push(REG(R_EBP));
    // push(REG(R_ESI));
    // push(REG(R_EDI));
    print_asm_template1();
    return 0;
}


#include "cpu/exec/template-end.h"
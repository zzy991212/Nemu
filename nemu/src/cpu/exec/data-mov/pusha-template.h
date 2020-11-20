#include "cpu/exec/template-start.h"

#define instr pusha

#if DATA_BYTE == 2
static void push_w(uint32_t val){
    reg_l(R_ESP) -= DATA_BYTE;
    swaddr_write(reg_l(R_ESP),DATA_BYTE,(DATA_TYPE)val);
}
#endif

#if DATA_BYTE == 4
static void push_l(uint32_t val){
    reg_l(R_ESP) -= DATA_BYTE;
    swaddr_write(reg_l(R_ESP),DATA_BYTE,val);
}
#endif

make_helper(concat(pusha_,SUFFIX)){
    current_sreg = R_SS;
    DATA_TYPE tmp = REG(R_ESP);
    concat(push_,SUFFIX)(REG(R_EAX));
    concat(push_,SUFFIX)(REG(R_ECX));
    concat(push_,SUFFIX)(REG(R_EDX));
    concat(push_,SUFFIX)(REG(R_EBX));
    concat(push_,SUFFIX)(tmp);
    concat(push_,SUFFIX)(REG(R_EBP));
    concat(push_,SUFFIX)(REG(R_ESI));
    concat(push_,SUFFIX)(REG(R_EDI));
    print_asm_template1();
    return 1;
}


#include "cpu/exec/template-end.h"
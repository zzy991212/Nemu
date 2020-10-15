#include "cpu/exec/template-start.h"

#define instr ret

make_helper(concat(ret_n_, SUFFIX)) {
    current_sreg = R_SS;
    cpu.eip = MEM_R(reg_l(R_ESP));
    reg_l(R_ESP) += DATA_BYTE;
    print_asm("ret");
    return 0;
} 

make_helper(concat(ret_i_,SUFFIX)) {
    current_sreg = R_SS;
    int val = instr_fetch(eip + 1, DATA_BYTE);
	cpu.eip = MEM_R(REG(R_ESP));
	if (DATA_BYTE == 2) cpu.eip &= 0xffff;
    REG(R_ESP) += DATA_BYTE;

    int i;
	for (i = 0;i < val; i += DATA_BYTE)
	    MEM_W(REG(R_ESP) + i,0);
	
    REG(R_ESP) += val;
	print_asm("ret\t$0x%x",val);
	return 0;
}
#include "cpu/exec/template-end.h"
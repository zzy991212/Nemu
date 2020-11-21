#include "cpu/exec/template-start.h"
#include <nemu.h>

#define instr iret
#if DATA_BYTE == 2
int popfromstack_w(){
    DATA_TYPE ret = swaddr_read(reg_l(R_ESP),DATA_BYTE);
    swaddr_write(reg_l(R_ESP),DATA_BYTE,0);
    reg_l(R_ESP) += DATA_BYTE;
    return ret;
}
#endif

#if DATA_BYTE == 4
int popfromstack_l(){
    DATA_TYPE ret = swaddr_read(reg_l(R_ESP),DATA_BYTE);
    swaddr_write(reg_l(R_ESP),DATA_BYTE,0);
    reg_l(R_ESP) += DATA_BYTE;
    return ret;
}
#endif

make_helper(concat(iret_,SUFFIX)) {
	if (cpu.cr0.protect_enable == 0){
		cpu.eip = concat(popfromstack_,SUFFIX)();
		cpu.cs.selector = concat(popfromstack_,SUFFIX)();
		cpu.EFLAGS = concat(popfromstack_,SUFFIX)();
	}else{
		current_sreg = R_SS;
		cpu.eip = concat(popfromstack_,SUFFIX)();
		cpu.cs.selector = concat(popfromstack_,SUFFIX)();
		cpu.EFLAGS = concat(popfromstack_,SUFFIX)();
		current_sreg = R_CS;
		sreg_load(R_CS);
	}
	print_asm("iret");
	return 0;
}

#include "cpu/exec/template-end.h"
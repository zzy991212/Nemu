#include "cpu/exec/helper.h"
#include <nemu.h>

uint32_t popfromstack(){
    int ret = swaddr_read(reg_l(R_ESP),4);
    swaddr_write(reg_l(R_ESP),4,0);
    reg_l(R_ESP) += 4;
    return ret;
}

make_helper(iret) {
	if (cpu.cr0.protect_enable == 0){
		cpu.eip = popfromstack();
		cpu.cs.selector = popfromstack();
		cpu.EFLAGS = popfromstack();
	}else{
		current_sreg = R_SS;
		cpu.eip = popfromstack();
		cpu.cs.selector = popfromstack();
		cpu.EFLAGS = popfromstack();
		current_sreg = R_CS;
		sreg_load(R_CS);
	}
	print_asm("iret");
	return 1;
}
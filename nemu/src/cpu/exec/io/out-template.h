#include "cpu/exec/template-start.h"
#include "device/port-io.h"
#define instr out

// static void do_execute () {

// 	pio_write(op_dest -> addr,DATA_BYTE,op_src -> val);

// 	print_asm_template2();
// }
make_helper(concat(out_si2a_,SUFFIX)){
	uint8_t imm8 = instr_fetch(cpu.eip+1,1);
	pio_write(imm8,DATA_BYTE,REG(R_EAX));
	print_asm("out\t%x",imm8);
	return 2;
}
make_helper(concat(out_r2rm_,SUFFIX)){
	pio_write(reg_w(R_EDX),DATA_BYTE,REG(R_EAX));
	print_asm("out");
	return 1;
}
// make_instr_helper(si2a);
// make_instr_helper(r2rm);

#include "cpu/exec/template-end.h"

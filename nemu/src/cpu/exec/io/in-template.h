#include "cpu/exec/template-start.h"
#include "device/port-io.h"
#define instr in


// static void do_execute () {
// 	OPERAND_W(op_dest,pio_read(op_src -> addr,DATA_BYTE));
// 	print_asm_template2();
// }

make_helper(concat(in_si2a_,SUFFIX)){
	uint8_t imm8 = instr_fetch(cpu.eip+1,1);
	REG(R_EAX) = pio_read(imm8,DATA_BYTE);
	print_asm("in\t%x",imm8);
	return 2;
}
make_helper(concat(in_r2rm_,SUFFIX)){
	REG(R_EAX) = pio_read((ioaddr_t)reg_w(R_EDX),DATA_BYTE);
	print_asm("in");
	return 1;
}
// make_instr_helper(si2a);
// make_instr_helper(r2rm);

#include "cpu/exec/template-end.h"

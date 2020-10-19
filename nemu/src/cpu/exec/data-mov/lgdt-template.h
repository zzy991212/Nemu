#include "cpu/exec/template-start.h"

#define instr lgdt


static void do_execute() {
	cpu.gdtr.limit = swaddr_read(op_src -> addr,2);
	if (op_src -> size == 2){
		cpu.gdtr.base = swaddr_read(op_src -> addr + 2,3);//24 bit
	}else if (op_src -> size == 4){
		cpu.gdtr.base = swaddr_read(op_src -> addr + 2,4);//32 bit
	}
	//printf("lgdt:%d\n",current_sreg);
	print_asm_template1();
}


make_instr_helper(rm);


#include "cpu/exec/template-end.h"
#include "cpu/exec/helper.h"

make_helper(leave) {
	current_sreg = R_SS;
	cpu.esp = cpu.ebp;
	cpu.ebp = swaddr_read(cpu.esp, 4);
	cpu.esp += 4;

	print_asm("leave");
	return 1;
}

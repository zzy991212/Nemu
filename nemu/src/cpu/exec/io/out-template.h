#include "cpu/exec/template-start.h"

#define instr out

void pio_write(ioaddr_t, size_t, uint32_t);

static void do_execute () {
	printf("%x\n",op_dest -> addr);
	pio_write(op_dest -> addr,DATA_BYTE,op_src -> val);
	printf("%x\n",op_dest -> addr);
	print_asm_template2();
}

make_instr_helper(i2a);
make_instr_helper(r2rm);

#include "cpu/exec/template-end.h"

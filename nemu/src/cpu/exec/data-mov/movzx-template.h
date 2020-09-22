#include "cpu/exec/template-start.h"

#define instr movzx


static void do_movzx_b() {
	DATA_TYPE ret = (op_src -> val) & 0xff;
    ret = (uint32_t)ret;
	OPERAND_W(op_dest, ret);
	print_asm_template2();
}


static void do_movzx_w() {
	DATA_TYPE ret = (op_src -> val) & 0xffff;
    ret = (uint32_t)ret;
	OPERAND_W(op_dest, ret);
	print_asm_template2();
}

make_helper(movzx_rm2r_b){
	return idex(eip, decode_rm2r_b, do_movzx_b);
}

make_helper(movzx_rm2r_w){
	return idex(eip, decode_rm2r_w, do_movzx_w);
}

#include "cpu/exec/template-end.h"
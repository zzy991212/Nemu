#include "cpu/exec/template-start.h"

#define instr movzb


static void do_execute() {
	DATA_TYPE ret = (uint8_t)(op_src -> val) & 0xff;
	OPERAND_W(op_dest, ret);
	print_asm_template2();
}


make_instr_helper(rm2r);

// #if DATA_BYTE != 1
// make_helper(concat(movzb_rm2r_, SUFFIX)){
//     uint32_t len = decode_rm_b_internal(eip + 1, op_src, op_dest);
//     DATA_TYPE res = (uint8_t)op_src->val &0xff;
//     // res &= 0xff;
//     concat(write_operand_, SUFFIX)(op_dest, res);
//     print_asm(str(instr) " %s,%s", op_src->str, op_dest->str);
//     return len + 1;
// }
// #endif


#include "cpu/exec/template-end.h"
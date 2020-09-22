#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute () {
	DATA_TYPE_S imm = op_src -> val;
    if (op_src -> type == OP_TYPE_IMM){
        cpu.eip += imm;
    }else {
        // int len = concat(decode_i_, SUFFIX)(cpu.eip + 1);

    }
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"

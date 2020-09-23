#include "cpu/exec/template-start.h"

#define instr pop

#if DATA_BYTE == 2 || DATA_BYTE == 4
static void do_execute() {
    OPERAND_W(op_src,MEM_R(reg_l(R_ESP)));
    MEM_W(reg_l(R_ESP),0);
    reg_l(R_ESP) += DATA_BYTE;

    print_asm_template1();
}
#endif 

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
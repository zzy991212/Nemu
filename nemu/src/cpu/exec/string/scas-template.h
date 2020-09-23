#include "cpu/exec/template-start.h"

#define instr scas

make_helper(concat(scas_m_, SUFFIX)) {

    DATA_TYPE dest_index = reg_l(R_EDI);   
    REG(R_EAX) -= dest_index;
    if (cpu.DF == 0) reg_l(R_EDI) += DATA_BYTE;
    else reg_l(R_EDI) -= DATA_BYTE;
	print_asm("scas");
    return 1;
}



#include "cpu/exec/template-end.h"
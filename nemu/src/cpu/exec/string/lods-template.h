#include "cpu/exec/template-start.h"

#define instr lods

make_helper(concat(lods_m_, SUFFIX)) {

    swaddr_write(REG(R_EAX),DATA_BYTE,MEM_R(REG(R_ESI)));
	print_asm("lods");
    return 0;
}



#include "cpu/exec/template-end.h"
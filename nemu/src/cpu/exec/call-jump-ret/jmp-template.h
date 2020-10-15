#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute () {
	DATA_TYPE_S imm = op_src -> val;
    if (op_src -> type == OP_TYPE_IMM){
        cpu.eip += imm;
    }else {
        int len = concat(decode_rm_, SUFFIX)(cpu.eip + 1);
        cpu.eip = imm - len - 1;
    }
    print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(rm)
#if DATA_BYTE == 4
make_helper(ljmp){
    extern Sreg_Descriptor *sreg_desc;
    Sreg_Descriptor new_sreg_desc;
    sreg_desc = &new_sreg_desc;
    uint32_t op1 = instr_fetch(eip + 1,4);
    uint16_t op2 = instr_fetch(eip + 1 + 4,2);

    printf("%08x %08x\n",op1,op2);
    cpu.eip = op1 - 1 - 6;//opcode + 6byte
    cpu.cs.selector = op2;

    uint16_t idx = cpu.cs.selector >> 3;//index of sreg

	Assert((idx << 3) <= cpu.gdtr.limit,"Segement Selector Is Out Of The Limit!");

	swaddr_t chart_addr = cpu.gdtr.base + (idx << 3);//chart addr
    
	sreg_desc -> part1 = lnaddr_read(chart_addr, 4);
	sreg_desc -> part2 = lnaddr_read(chart_addr + 4, 4);
    printf("%08x %08x\n",sreg_desc -> part1,sreg_desc -> part2);
	Assert(sreg_desc -> p == 1, "Segement Not Exist!");//p bit, whether sreg_desc exists

	cpu.cs.base = 0;
	cpu.cs.base |= sreg_desc -> base1;
	cpu.cs.base |= sreg_desc -> base2 << 16;
	cpu.cs.base |= sreg_desc -> base3 << 24;

	cpu.cs.limit = 0;
	cpu.cs.limit |= sreg_desc -> limit1;
	cpu.cs.limit |= sreg_desc -> limit2 << 16;
	cpu.cs.limit |= 0xfff << 24;
	if (sreg_desc -> g == 1) cpu.cs.limit <<= 12;//G = 0, unit = 1B;G = 1, unit = 4KB

    print_asm("ljump %x %x",op2,op1);
    return 1+6;    
}
#endif
#include "cpu/exec/template-end.h"

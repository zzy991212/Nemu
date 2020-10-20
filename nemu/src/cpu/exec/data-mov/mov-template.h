#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"
#include "memory/tlb.h"
#define instr mov

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	current_sreg = R_DS;//A0 A1
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	current_sreg = R_DS;//A2 A3
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}
#if DATA_BYTE == 4
make_helper(mov_cr2r){
	uint8_t modrm= instr_fetch(eip + 1,1);
	uint8_t cr_num = (modrm >> 3) & 7; // reg
	uint8_t reg_num = modrm & 7; // r/m
	switch (cr_num){
		case 0:
			reg_l(reg_num) = cpu.cr0.val;
			print_asm("mov CR0 %s",REG_NAME(reg_num));
			break;
		case 3:
			reg_l(reg_num) = cpu.cr3.val;
			print_asm("mov CR3 %s",REG_NAME(reg_num));
			break;
		default:
			break;
	}
	return 2;
}

make_helper(mov_r2cr){
	uint8_t modrm= instr_fetch(eip + 1,1);
	uint8_t cr_num = (modrm >> 3) & 7; // reg
	uint8_t reg_num = modrm & 7; // r/m
	switch (cr_num){
		case 0:
			cpu.cr0.val = reg_l(reg_num);
			print_asm("mov %s CR0",REG_NAME(reg_num));
			break;
		case 3:
			init_tlb();
			cpu.cr3.val = reg_l(reg_num);
			print_asm("mov %s CR3",REG_NAME(reg_num));
			break;
		default:
			break;
	}
	return 2;
}
#endif

#if DATA_BYTE == 2
make_helper(mov_sreg2rm){
	uint8_t modrm= instr_fetch(eip + 1,1);
	uint8_t sreg_num = ((modrm >> 3) & 7); // reg
	uint8_t reg_num = (modrm & 7); // r/m
	cpu.sreg[sreg_num].selector = reg_w(reg_num);
	
	sreg_load(sreg_num);

	print_asm("mov %s SREG[%u]",REG_NAME(reg_num),sreg_num);
	return 2;
}
#endif
#include "cpu/exec/template-end.h"

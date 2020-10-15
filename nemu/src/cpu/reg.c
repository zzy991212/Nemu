#include "nemu.h"
#include <stdlib.h>
#include <time.h>

CPU_state cpu;

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test() {
	srand(time(0));
	uint32_t sample[8];
	uint32_t eip_sample = rand();
	cpu.eip = eip_sample;

	int i;
	for(i = R_EAX; i <= R_EDI; i ++) {
		sample[i] = rand();
		reg_l(i) = sample[i];
		assert(reg_w(i) == (sample[i] & 0xffff));
	}

	assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
	assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
	assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
	assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
	assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
	assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
	assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
	assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

	assert(sample[R_EAX] == cpu.eax);
	assert(sample[R_ECX] == cpu.ecx);
	assert(sample[R_EDX] == cpu.edx);
	assert(sample[R_EBX] == cpu.ebx);
	assert(sample[R_ESP] == cpu.esp);
	assert(sample[R_EBP] == cpu.ebp);
	assert(sample[R_ESI] == cpu.esi);
	assert(sample[R_EDI] == cpu.edi);

	assert(eip_sample == cpu.eip);
}

void sreg_load(uint8_t sreg_num){
	Assert(cpu.cr0.protect_enable,"Not In Protect Mode!");

	uint16_t idx = cpu.sreg[sreg_num].selector >> 3;//index of sreg

	Assert((idx << 3) <= cpu.gdtr.limit,"Segement Selector Is Out Of The Limit!");

	lnaddr_t chart_addr = cpu.gdtr.base + (idx << 3);//chart addr
	sreg_desc -> part1 = lnaddr_read(chart_addr, 4);
	sreg_desc -> part2 = lnaddr_read(chart_addr + 4, 4);

	Assert(sreg_desc -> p == 1, "Segement Not Exist!");//p bit, whether sreg_desc exists
	
	
	uint32_t bases = 0;
	
	bases += ((uint32_t)sreg_desc -> base1);
	
	bases += ((uint32_t)sreg_desc -> base2)<< 16;
	
	bases += ((uint32_t)sreg_desc -> base3) << 24;
	cpu.sreg[sreg_num].base = bases;
	//printf("%p\n",&(cpu.sreg[sreg_num].base));

	uint32_t limits = 0;
	limits += ((uint32_t)sreg_desc -> limit1);
	limits += ((uint32_t)sreg_desc -> limit2) << 16;
	limits += ((uint32_t)0xfff) << 24;
	cpu.sreg[sreg_num].limit = limits;
	if (sreg_desc -> g == 1) cpu.sreg[sreg_num].limit <<= 12;//G = 0, unit = 1B;G = 1, unit = 4KB
}
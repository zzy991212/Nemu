#ifndef __HELPER_H__
#define __HELPER_H__

#include "nemu.h"
#include "cpu/decode/operand.h"
extern uint8_t current_sreg;
/* All function defined with 'make_helper' return the length of the operation. */
#define make_helper(name) int name(swaddr_t eip)

static inline uint32_t instr_fetch(swaddr_t addr, size_t len) {
	uint8_t past_sreg = current_sreg;
	current_sreg = R_CS;
	uint32_t ret = swaddr_read(addr, len);
	current_sreg = past_sreg;
	return ret;
}

/* Instruction Decode and EXecute */
static inline int idex(swaddr_t eip, int (*decode)(swaddr_t), void (*execute) (void)) {
	/* eip is pointing to the opcode */
	int len = decode(eip + 1);
	execute();
	return len + 1;	// "1" for opcode
}

/* shared by all helper function */
extern Operands ops_decoded;

#define op_src (&ops_decoded.src)
#define op_src2 (&ops_decoded.src2)
#define op_dest (&ops_decoded.dest)


#endif

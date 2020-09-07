#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */
	uint32_t val;
	char exprs[32];
	
} WP;

WP* new_wp();
void free_wp(WP*);
void print_w();
WP* delete_wp(int, bool*);
void check_wp(bool*);
#endif

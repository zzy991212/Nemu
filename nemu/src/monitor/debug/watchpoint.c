#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP];
static WP *head, *free_;

void init_wp_pool() {
	int i;
	for (i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */


WP* new_wp() {

	if (free_ == NULL) return NULL;

	WP *f_top, *h_tail;
	f_top = free_;
	h_tail = head;
	free_ = free_ -> next;

	f_top -> next = NULL;

	if (h_tail == NULL) head = f_top;
	else {
		while (h_tail -> next != NULL)
			h_tail = h_tail -> next;
		h_tail -> next = f_top;
	}
	return f_top;
}

void free_wp(WP *wp) {

	WP *h, *f;
	h = head;
	f = free_;
	if (h == wp) {
		head = wp -> next;
	} else {
		while (h != NULL && h -> next != wp) {
			h = h -> next;
		}
		h -> next = h -> next -> next;
	}
	wp -> next = free_;
	free_ = wp;
	wp -> val = 0;
	wp -> exprs[0] = '\0';
}

void print_w() {
	WP *h = head;
	while (h != NULL) {
		printf("[Watchpoint NO.%d]\tExpression: %s\tValue: %d\n", h -> NO, h -> exprs, h -> val);
		h = h -> next;
	}
}

WP* delete_wp(int id, bool* f) {
	WP* ret = head;
	while (ret != NULL && ret -> NO != id) {
		ret = ret -> next;
	}
	if (ret == NULL) *f = false;
	return ret;
}
void check_wp(bool* f) {
	WP* h = head;
	while (h != NULL) {
		bool tmp = true;
		uint32_t nxtv = expr(h->exprs, &tmp);
		if (nxtv != h -> val) {
			printf("[Watchpoint NO.%d]\tExpression: %s\tOrigin Value: 0x%x\tNew Value: 0x%x\n", h -> NO, h -> exprs, h -> val, nxtv);
			h -> val = nxtv;
			*f = true;
		}
		h = h -> next;
	}
}
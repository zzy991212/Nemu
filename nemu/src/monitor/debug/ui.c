#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <elf.h>
#include "memory.h"
#define TestCorrect(x) if(x){printf("Invalid Command!\n");return 0;}
void cpu_exec(uint32_t);

void GetFunctionAddr(swaddr_t EIP,char* name);
hwaddr_t page_translate(lnaddr_t addr);
hwaddr_t page_translate_additional(lnaddr_t addr,int* flag);
/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
#ifdef Test
	printf("test_time:%d\n",test_time);
#endif
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

/*New*/
static int cmd_si(char *args) {
	int step;
	if (args == NULL) step = 1;
	else sscanf(args, "%d", &step);
	cpu_exec(step);
	return 0;
}

static int cmd_info(char *args) {
	TestCorrect(args == NULL);
	if (args[0] == 'r') {
		int i;
		for (i = R_EAX; i <= R_EDI ; i++) {
			printf("$%s\t0x%08x\n", regsl[i], reg_l(i));
		}
		printf("$eip\t0x%08x\n", cpu.eip);
	} else if (args[0] == 'w') {
		print_w();
	}
	return 0;
}

static int cmd_x(char *args) {
	TestCorrect(args == NULL);
	current_sreg = R_DS;
	char* tokens = strtok(args, " ");
	int N, exprs;
	sscanf(tokens, "%d", &N);
	char* eps = tokens + strlen(tokens) + 1;
	bool flag = true;
	exprs = expr(eps, &flag);
	TestCorrect(!flag);
	int i;
	for (i = 0; i < N; i++) {
		printf("0x%08x\t0x%08x\n", exprs + i * 4, swaddr_read(exprs + i * 4, 4));
	}
	return 0;
}

static int cmd_p(char *args) {
	TestCorrect(args == NULL);
	uint32_t ans;
	bool flag;
	ans = expr(args, &flag);
	TestCorrect(!flag) 
	else {
		printf("%d\n", ans);
	}
	return 0;
}

static int cmd_w(char* args) {
	TestCorrect(args == NULL);
	bool flag = true;
	uint32_t v = expr(args, &flag);
	TestCorrect(!flag);
	WP *wp = new_wp();
	if (wp == NULL) {
		printf("No space to add an extra watchpoint!");
		return 0;
	}
	strcpy(wp -> exprs, args);
	wp -> val = v;
	printf("Succefully add watchpoint NO.%d\n", wp -> NO);
	return 0;
}
static int cmd_d(char* args) {
	TestCorrect(args == NULL);
	int id;
	sscanf(args, "%d", &id);
	bool flag = true;
	WP* wp = delete_wp(id, &flag);
	if (!flag) {
		printf("Cannot Find!\n");
		return 0;
	}
	free_wp(wp);
	printf("Succefully Delete!\n");
	return 0;
}
typedef struct {
	swaddr_t prev_ebp;
	swaddr_t ret_addr;
	uint32_t args[4];
}PartOfStackFrame ;
static int cmd_bt(char* args){
	if (args != NULL){
		printf("Wrong Command!");
		return 0;
	}
	current_sreg = R_SS;
	PartOfStackFrame EBP;
	char name[32];
	int cnt = 0;
	EBP.ret_addr = cpu.eip;
	swaddr_t addr = cpu.ebp;
	// printf("%d\n",addr);
	int i;
	while (addr){
		GetFunctionAddr(EBP.ret_addr,name);
		if (name[0] == '\0') break;
		printf("#%d\t0x%08x\t",cnt++,EBP.ret_addr);
		printf("%s",name);
		EBP.prev_ebp = swaddr_read(addr,4);
		EBP.ret_addr = swaddr_read(addr + 4, 4);
		printf("(");
		for (i = 0;i < 4;i ++){
			EBP.args[i] = swaddr_read(addr + 8 + i * 4, 4);
			printf("0x%x",EBP.args[i]);
			if (i == 3) printf(")\n");else printf(", ");
		}
		addr = EBP.prev_ebp;
	}
	return 0;
}
static int cmd_page(char* args) {
	TestCorrect(args == NULL);
	uint32_t addr;
	sscanf(args, "%x", &addr);
	int flag = 0;
	uint32_t real_addr = page_translate_additional(addr,&flag);
	if (flag == 0) printf("0x%08x\n",real_addr);
	else if (flag == 1) printf("Dir Cannot Be Used!\n");
	else printf("Page Cannot Be Used!\n");
	return 0;
}
static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Execute some steps, initial -> 1 step", cmd_si},
	{ "info", "Print values of all registers", cmd_info},
	{ "x", "Calculate expressions, let it be the starting memery address, print continuous N 4 bytes.", cmd_x},
	{ "p", "Calculate expressions", cmd_p},
	{ "w", "Add watchpoint", cmd_w},
	{ "d", "Delete watchpoint", cmd_d},
	{ "bt", "Print stack frame chain", cmd_bt},
	{ "page", "Translate ADDR in PAGE MODE", cmd_page}
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if (arg == NULL) {
		/* no argument given */
		for (i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for (i = 0; i < NR_CMD; i ++) {
			if (strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while (1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if (cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if (args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for (i = 0; i < NR_CMD; i ++) {
			if (strcmp(cmd, cmd_table[i].name) == 0) {
				if (cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}

#include "nemu.h"
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <elf.h>

enum {
	NOTYPE = 256, EQ, NUMBER, HEXNUMBER, REGISTER, NEQ, AND, OR, NOT, MINUS, POINTER, MARK

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */
	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"==", EQ},						// equal
	{"!=", NEQ},
	{"!", NOT},
	{"&&", AND},
	{"\\|\\|", OR},
	{"\\$[a-zA-z]+", REGISTER},		// register
	{"\\b0[xX][0-9a-fA-F]+\\b", HEXNUMBER}, // hexnumber
	{"\\b[0-9]+\\b", NUMBER},				// number
	{"-", '-'},						// minus
	{"\\*", '*'},					// multiply
	{"/", '/'},						// divide
	{"\\(", '('},					// left bracket
	{"\\)", ')'},					// right bracket
	{"\\b[a-zA-Z0-9_]+\\b",MARK},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

uint32_t GetMarkValue(char *str,bool *success);

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;
bool* cando;
static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0') {
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i ++) {
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

//				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);

				position += substr_len;
				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */

				switch (rules[i].token_type) {
				case NOTYPE: break;
				default: {
					tokens[nr_token].type = rules[i].token_type;
					if (rules[i].token_type == REGISTER) { //Register
						char* reg_start = e + (position - substr_len) + 1;
						strncpy(tokens[nr_token].str, reg_start, substr_len - 1);
						int t;
						for (t = 0; t <= strlen(tokens[nr_token].str); t++) { // tolower
							int x = tokens[nr_token].str[t];
							if (x >= 'A' && x <= 'Z') x += ('a' - 'A');
							tokens[nr_token].str[t] = (char)x;
						}
					} else
						strncpy(tokens[nr_token].str, substr_start, substr_len);

					tokens[nr_token].str[substr_len] = '\0';

					//						printf("%s\n", tokens[nr_token].str);
					nr_token ++;
				}
				}

				break;
			}
		}

		if (i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	return true;
}

bool check_bracket(int l, int r) {
	if (tokens[l].type != '(' || tokens[r].type != ')') return false;
	int b_num = 0, i;
	for (i = l; i <= r; i ++) {
		if (tokens[i].type == '(') b_num ++;
		if (tokens[i].type == ')') b_num --;
		if (b_num == 0 && i != r) return false;
	}
	if (b_num == 0) return true;
	return false;
}

int dominant_op(int l, int r) {
	if (*cando == false) return 0;
	int i;
	int pos = l;
	int pri = 0;
	int b_num = 0;
	for (i = r; i >= l; i --) {
		if (tokens[i].type == ')') b_num++;
		if (tokens[i].type == '(') b_num--;
		if (b_num != 0) continue;
		switch (tokens[i].type) {
		case '+': { // pri = 4
			if (pri < 4) pos = i, pri = 4;
			break;
		}
		case '-': { // pri = 4
			if (pri < 4) pos = i, pri = 4;
			break;
		}
		case '*': { // pri = 3
			if (pri < 3) pos = i, pri = 3;
			break;
		}
		case '/': { // pri = 3
			if (pri < 3) pos = i, pri = 3;
			break;
		}
		case NOT: { // pri = 2
			if (pri < 2) pos = i, pri = 2;
			break;
		}
		case EQ: { // pri = 7
			if (pri < 7) pos = i, pri = 7;
			break;
		}
		case NEQ: { // pri = 7
			if (pri < 7) pos = i, pri = 7;
			break;
		}
		case AND: { // pri = 11
			if (pri < 11) pos = i, pri = 11;
			break;
		}
		case OR: { // pri = 12
			if (pri < 12) pos = i, pri = 12;
			break;
		}
		case MINUS: { // pri = 2
			if (pri < 2) pos = i, pri = 2;
			break;
		}
		case POINTER: { // pri = 2
			if (pri < 2) pos = i, pri = 2;
			break;
		}
		default: break;
		}
	}
//	printf("%d-%d %d %d\n",l,r,pos,pri);
	if (pri == 0) {*cando = false; return 0;}

	if (pri == 2) {
		pri = 0;
		for (i = l; i <= r; i ++) {
			if (tokens[i].type == '(') b_num++;
			if (tokens[i].type == ')') b_num--;
			if (b_num != 0) continue;
			switch (tokens[i].type) {
			case MINUS: { // pri = 2
				if (pri < 2) pos = i, pri = 2;
				break;
			}
			case NOT: { // pri = 2
				if (pri < 2) pos = i, pri = 2;
				break;
			}
			case POINTER: { // pri = 2
				if (pri < 2) pos = i, pri = 2;
				break;
			}
			}
		}
	}
	return pos;
}
uint32_t eval(int l, int r) {
	if (*cando == false) return 0;
	if (l > r) {
		*cando = false;
		return 0;
	}
	if (l == r) {
		uint32_t num = 0;
		if (tokens[l].type == MARK){
			num = GetMarkValue(tokens[l].str, cando);
			if (*cando == false) return 0;
		} else if (tokens[l].type == NUMBER) {
			sscanf(tokens[l].str, "%d", &num);
		} else if (tokens[l].type == HEXNUMBER) {
			sscanf(tokens[l].str, "%x", &num);
		} else if (tokens[l].type == REGISTER) {
			if (strlen(tokens[l].str) == 3) { //length = 3
				int i;
				for (i = R_EAX; i <= R_EDI ; i ++) {
					if (strcmp(tokens[l].str, regsl[i]) == 0) {
						break;
					}
				}
				if (i > R_EDI) {
					if (strcmp(tokens[l].str, "eip") == 0) {
						num = cpu.eip;
					} else {*cando = false; return 0;}
				} else return num = reg_l(i);
			}
			else if (strlen(tokens[l].str) == 2) { //length = 2
				int i;
				for (i = R_AX; i <= R_DI; i ++) {
					if (strcmp(tokens[l].str, regsw[i]) == 0) {
						break;
					}
				}
				if (i <= R_DI) return num = reg_w(i);
				for (i = R_AL; i <= R_BH; i ++) {
					if (strcmp(tokens[l].str, regsb[i]) == 0) {
						break;
					}
				}
				if (i <= R_BH) return num = reg_b(i);
				*cando = false; return 0;
			}
			else {*cando = false; return 0;}
		} else {*cando = false; return 0;}
		return num;
	}
	uint32_t ans = 0;

	if (check_bracket(l, r)) return eval(l + 1, r - 1);
	else {
		int pos = dominant_op(l, r);
		if (l == pos || tokens[pos].type == NOT || tokens[pos].type == MINUS || tokens[pos].type == POINTER) {
			uint32_t r_ans = eval(pos + 1, r);
			switch (tokens[pos].type) {
			case POINTER: current_sreg = R_DS;return swaddr_read(r_ans, 4);
			case NOT: return !r_ans;
			case MINUS: return -r_ans;
			default: {*cando = false; return 0;}
			}
		}
		uint32_t l_ans = eval(l, pos - 1), r_ans =  eval(pos + 1, r);
		switch (tokens[pos].type) {
		case '+': ans = l_ans + r_ans; break;
		case '-': ans = l_ans - r_ans; break;
		case '*': ans = l_ans * r_ans; break;
		case '/': if (r_ans == 0) {*cando = false; return 0;} else ans = l_ans / r_ans; break;
		case EQ : ans = l_ans == r_ans; break;
		case NEQ: ans = l_ans != r_ans; break;
		case AND: ans = l_ans && r_ans; break;
		case OR : ans = l_ans && r_ans; break;
		default: {*cando = false; return 0;}
		}
	}
	return ans;
}

uint32_t expr(char *e, bool *success) {
	if (!make_token(e)) {
		*success = false;
		return 0;
	}
	cando = success;
	/* TODO: Insert codes to evaluate the expression. */
	/*check whether brackets are matched*/
	int i, brack = 0;
	for (i = 0; i < nr_token ; i++) {
		if (tokens[i].type == '(') brack ++;
		if (tokens[i].type == ')') brack --;
		if (brack < 0) {
			*success = false;
			return 0;
		}
		if (tokens[i].type == '-' && (i == 0 || (tokens[i - 1].type != ')' && tokens[i - 1].type != NUMBER && tokens[i - 1].type != HEXNUMBER && tokens[i - 1].type != REGISTER))) {
			tokens[i].type = MINUS;
		}
		if (tokens[i].type == '*' && (i == 0 || (tokens[i - 1].type != ')' && tokens[i - 1].type != NUMBER && tokens[i - 1].type != HEXNUMBER && tokens[i - 1].type != REGISTER))) {
			tokens[i].type = POINTER;
		}
	}
	if (brack != 0) {
		*success = false;
		return 0;
	}

//	panic("please implement me");
	*success = true;
	return eval(0, nr_token - 1);
}


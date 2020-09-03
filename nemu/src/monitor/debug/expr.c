#include "nemu.h"
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NUMBER

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
	{"[0-9]+", NUMBER},				// number
	{"-",'-'},						// minus
	{"\\*",'*'},					// multiply
	{"/",'/'},						// divide
	{"\\(",'('},					// left bracket
	{"\\)",')'},					// right bracket
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
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

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

//				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);

				position += substr_len;
				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
					case NOTYPE: break;
					default: {
						tokens[nr_token].type = rules[i].token_type;
						strncpy(tokens[nr_token].str, substr_start, substr_len);
//						printf("%s\n", tokens[nr_token].str);
						nr_token ++;
					}
				}


				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}
	return true; 
}

bool check_bracket(int l,int r){
	if (tokens[l].type != '(' || tokens[r].type != ')') return false;
	int b_num = 0,i;
	for (i = l; i <= r; i ++){
		if (tokens[i].type == '(') b_num ++;
		if (tokens[i].type == ')') b_num --;
		if (b_num == 0 && i != r) return false;
	}
	if (b_num == 0) return true;
	return false;
}

int dominant_op(int l,int r){
	int i;
	int pos = l;
	int pri = 10;
	int b_num = 0;
	for (i = l; i <= r; i ++){
		if (tokens[i].type == '(') b_num++;
		if (tokens[i].type == ')') b_num--;
		if (b_num != 0) continue;
		switch(tokens[i].type){
			case '+':{// pri = 2
				if (pri >= 2){
					pos = i;
					pri = 2;
				}
				break; 
			}
			case '-':{// pri = 2
				if (pri >= 2){
					pos = i;
					pri = 2;
				}
				break; 
			}
			case '*':{// pri = 8
				if (pri >= 8) {
					pos = i;
					pri = 8;
				}
				break; 
			}
			case '/':{// pri = 8
				if (pri >= 8) {
					pos = i;
					pri = 8;
				}
				break; 
			}
			default:break;
		}
	}
//	printf("%d-%d %d %d\n",l,r,pos,pri);
	if (pos == l) assert(0);
	return pos;
}
uint32_t eval(int l,int r){
	if (l>r){
		Assert(l<r,"Wrong!");
		return 0;
	}
	if (l==r){
		uint32_t num=0;
		if (tokens[l].type == NUMBER){
			sscanf(tokens[l].str,"%d",&num);
		}
		return num;
	}
	uint32_t ans=0;

	if (check_bracket(l,r)) return eval(l+1,r-1);
	else {
		int pos = dominant_op(l,r);
		uint32_t l_ans = eval(l,pos-1),r_ans =  eval(pos+1,r);
		switch(tokens[pos].type){
			case '+':ans = l_ans + r_ans;break;
			case '-':ans = l_ans - r_ans;break;
			case '*':ans = l_ans * r_ans;break;
			case '/':ans = l_ans / r_ans;break;
			default:assert(0);
 		} 
	}
	return ans;
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	/*check whether brackets are matched*/
	int i,brack=0;
	for (i = 0; i < nr_token ; i++){
		if (tokens[i].type=='(') brack++;
		if (tokens[i].type==')') brack--;
		if (brack < 0){
			*success = false;
			return 0;
		}
	}
	if (brack != 0){
		*success = false;
		return 0;
	}
//	panic("please implement me");
	*success = true;
	return eval(0,nr_token-1);
}


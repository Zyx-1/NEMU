#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdint.h>

void cpu_exec(uint32_t);
int breakpoint_counter = 1;
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
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}


static int cmd_help(char *args);
	

static int cmd_si(char *args) {
	char *read = strtok(args, " ");
	if (read == NULL)
		cpu_exec(1);
	else{
		int num = atoi(read);
		cpu_exec(num);
	}
	return 0;
}

static int cmd_info(char *args){
	char *read = strtok(args, " ");
	if(strcmp(read, "r") == 0){
		int i;		
		for(i = R_EAX; i <= R_EDI; i++)
			printf("%s\t0x%08x\n", regsl[i], reg_l(i));
		printf("eip\t0x%08x\n", cpu.eip);
		return 0;
	}
	else if(strcmp(read, "w") == 0){
		info_wp();
		return 0;
	}			
	printf("Wrong Input! \n");
	return 1;
}

static int cmd_x(char *args){
	if(args == NULL){
		printf("Wrong Input! \n");
		return 1;
	}
	char *read = strtok(args, " ");
	if(read == NULL){
		printf("Wrong Input! \n");
		return 1;
	}
	int n = atoi(read);	
	char *EXPR = strtok(NULL, " ");
	if(EXPR == NULL){
		printf("Wrong Input! \n");
		return 1;
	}
	if(strtok(NULL, " ") != NULL){
		printf("Wrong Input! \n");
		return 1;
	}
	bool flag = true;
	if(flag != true){
		printf("ERROR \n");
		return 0;	
	}
	char *str;
	swaddr_t addrass = strtol(EXPR, &str, 16);
	int i, j;	
	for(i = 0; i < n; i++){
		uint32_t data = swaddr_read(addrass + i * 4, 4);
		printf("0x%08x ", addrass + i * 4);
		for(j = 0; j < 4; j++){
			printf("0x%02x ", data & 0xff);
			data = data >> 8;
		}
		printf("\n");
	}
	return 0;
}

static int cmd_p(char *args){
	bool success;
	uint32_t i;
	i = expr(args, &success);
	printf("0x%x:\t%d\n", i, i);
	return 0;
}

static int cmd_b(char *args){
	bool suc;
	swaddr_t addr;
	addr = expr(args+1, &suc);
	if(!suc) assert(1);
	sprintf(args, "$eip == 0x%x", addr);
	printf("Breakpoint %d at 0x%x\n", breakpoint_counter, addr);
	WP *f;
	f = new_wp();
	f->val = expr(args, &suc);
	f->b = breakpoint_counter;
	breakpoint_counter++;
	strcpy(f->expr, args);
	return 0;
}

static int cmd_w(char *args){
	WP *f;
	bool suc;
	f = new_wp();
	printf("Watchpoint %d: %s\n", f->NO, args);
	f->val = expr(args, &suc);
	strcpy(f->expr, args);
	if(!suc) Assert(1, "Wrong input\n");
	printf("Value : %d\n", f->val);
	return 0;
}

static int cmd_d(char *args){
	char *read = strtok(args, " ");
	if(read != NULL){
		int num = atoi(read);
		delete_wp(num);
		return 0;
	}
	printf("Wrong input\n");
	return 1;
}

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Single step", cmd_si },
	{ "info", "Print regiters", cmd_info },
	{ "x", "Scan memory", cmd_x},
	{ "p", "Expression evaluation", cmd_p},
	{ "w", "Add watchpoint", cmd_w},	
	{ "b", "Add breakpoint", cmd_b},	
	{ "d", "Delete a watchpoint", cmd_d}
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))
static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}

#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	int value;
	int newvalue;
	char type;
	char Enb;
	char str[32];
	/* TODO: Add more members if necessary */


} WP;

void print_wp();
void free_wp(int );
int judge_wp();

#endif

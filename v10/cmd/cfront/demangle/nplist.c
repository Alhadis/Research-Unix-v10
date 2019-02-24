/*ident	"@(#)ctrans:demangler/nplist.c	1.2"*/
/*
 * C++ Demangler Source Code
 * @(#)master	1.5
 * 7/27/88 13:54:37
 */
#include "String.h"

struct name_pair {
	char *s1,*s2;
};
int oplen = 0;

struct name_pair nplist[100] = {
	{"lt","<"},   {"ls","<<"},  {"dv","/"}, 
	{"gt",">"},   {"rs",">>"},  {"md","%"}, 
	{"le","<="},  {"ml","*"},   {"pl","+"}, 
	{"ge",">="},  {"ad","&"},   {"mi","-"}, 
	{"ne","!="},  {"or","|"},   {"er","^"}, 
	{"aa","&&"},  {"oo","||"},  {"as","="}, 
	{"apl","+="}, {"ami","-="}, {"amu","*="}, 
	{"adv","/="}, {"amd","%="}, {"aad","&="}, 
	{"aor","|="},{"aer","^="}, {"als","<<="},
	{"ars",">>="},{"pp","++"},  {"mm","--"},
	{"vc","[]"},  {"cl","()"},  {"rf","->"},
	{"eq","=="},  {"co","~"},   {"nt","!"},
	{"nw"," new"},{"dl"," delete"}, {0,0} };

/* This routine is designed to demangle */
char *
findop(c)
char *c;
{
	register int i;
	for(oplen=0;c[oplen] && c[oplen] != '_';oplen++)
		;
	for(i=0;nplist[i].s1;i++) {
		if(strncmp(nplist[i].s1,c,oplen) == 0)
			return nplist[i].s2;
	}
	return 0;
}

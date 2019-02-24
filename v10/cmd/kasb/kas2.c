/* @(#)kas2.c	1.2 */
#include <stdio.h>
#include "kas.h"
#include "kas.yh"
#define OP(a,b,c)  {a,b|XTAG,c}
#define R(a,b)	{a,REG|XTAG,b}
#define RL(a,b)	{a,REGL|XTAG,b}
#define RH(a,b)	{a,REGH|XTAG,b}
#define M(a,b)	{a,DEBUG|XTAG,b}
#define P(a,b)	{a,PAGE|XTAG,b}
struct symtab instab[] = {
OP("mov", MVINS, 0),
OP("inc", SFINS, (3<<4)),
OP("adc", SFINS, (4<<4)),
OP("asl", SFINS, (5<<4)),
OP("aslc", SFINS, (6<<4)),
OP("dec", SFINS, (7<<4)),
OP("add", DFINS, (0<<4)),
OP("addc", DFINS, (1<<4)),
OP("subc", DFINS, (2<<4)),
OP("orn", DFINS, (10<<4)),
OP("and", DFINS, (11<<4)),
OP("or", DFINS, (12<<4)),
OP("xor", DFINS, (13<<4)),
OP("sub", DFINS, (14<<4)),
OP("addn", DFINS, (15<<4)),
OP("jmp", JMPINS, (0<<8)|0100000),
OP("br", BRINS, (1<<8)|0100000),
OP("brc", BRINS, (2<<8)|0100000),
OP("brz", BRINS, (3<<8)|0100000),
OP("br0", BRINS, (4<<8)|0100000),
OP("br1", BRINS, (5<<8)|0100000),
OP("br4", BRINS, (6<<8)|0100000),
OP("br7", BRINS, (7<<8)|0100000),
R("r0", 0),
R("r1", 1),
R("r2", 2),
R("r3", 3),
R("r4", 4),
R("r5", 5),
R("r6", 6),
R("r7", 7),
R("r8", 8),
R("r9", 9),
R("r10", 10),
R("r11", 11),
R("r12", 12),
R("r13", 13),
R("r14", 14),
R("r15", 15),
{"mem", MEM|XTAG, 0},
{"brg", BRG|XTAG, 0},
{"mar", MAR|XTAG, 0},
{"pc", PC|XTAG, 0},
{"pcr", PCR|XTAG, 0},
RL("idl", 0),
RL("idh", 1),
RL("odl", 2),
RL("odh", 3),
RL("ial", 4),
RL("iah", 5),
RL("oal", 6),
RL("oah", 7),
RL("lur0", 8+0),
RL("lur1", 8+1),
RL("lur2", 8+2),
RL("lur3", 8+3),
RL("lur4", 8+4),
RL("lur5", 8+5),
RL("lur6", 8+6),
RL("lur7", 8+7),
RH("csr0", 0),
RH("csr1", 1),
RH("csr2", 2),
RH("csr3", 3),
RH("csr4", 4),
RH("csr5", 5),
RH("csr6", 6),
RH("csr7", 7),
RH("npr", 8+0),
RH("nprx", 8+1),
P("p0",0),
P("p1",1<<11),
P("p2",2<<11),
P("p3",3<<11),
{".", XTEXT, 0},
{".text", SEG|XTAG, XTEXT},
{".data", SEG|XTAG, XDATA},
{".org", ORG|XTAG, 0},
{".byte", DATA|XTAG, 1},
{".word", DATA|XTAG, 2},
M(".load", DLOAD),
M(".dump", DDUMP),
M(".step", DSTEP),
M(".run", DRUN),
M(".reset", DRESET),
M(".wcsr", DWCSR),
M(".clear", DCLEAR),
M(".stop", DSTOP),
M(".regs", DREGS),
0,
};

struct	symtab	symtab[NSYM];
struct	symtab	*hshtab[NHASH];
struct	symtab	loclab[10];
struct	symtab	*dot;
int	tsize;
int	dsize;
int	anyerrs;
int	textsv, datasv;
char	dbmode;
char	*outfile, *dbfile, *infile;
int	dbfd;
int	dbtab[3];
char	dbstr[50], instr[50];
short	ispace[NKMCI], reloc[NKMCI];
char	dspace[NKMCD];
struct ir ir;

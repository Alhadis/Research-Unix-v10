#include "mfile2.h"
#include "setjmp.h"
extern jmp_buf back;
extern char *regnames[], *frameptr, *argptr;
#define REGMASK 0x3f
#define REGVAR	6
/* these would be 3 and 3 respectively on the mac32, 3f and 6 on the vax */
/* registers 0..REGVAR-1 are scratch */
typedef struct {
	unsigned char ans;
	unsigned char regmask;
	unsigned short flag;
} ret;
#define CC	1
#define VALUE	2
#define TOSTACK	4
#define ASADDR	8
#define SCRATCH	16
#define ICON0	32
#define ICON1	64
#define ISREG	128
#define DESTISLEFT	256
#define CANINDIR	512
#define FAILX	1024
#define INDEX	2048
#define FAIL0	4096
#define FAIL (FAILX|FAIL0)
#define USED	8192
#define NOGOOD	(FAIL|USED)
#define AUTO	(16384)	/* ans is auto incr/decr */
#define FSTASG	0x8000	/* to Stasg, for Stasg */
/* as param; as return
 * CC just need the condition codes; condition codes are valid
 * VALUE need the value (for incr/decr.  as opposed to just the side effect)
 * TOSTACK put the result on the stack
 * ASADDR as an address, for various weird environments (calls, extzv, ...)
 * SCRATCH ; the result is in a scratch place
 * ICON0 ; the result is the constant 0
 * ICON1 ; the result is the constant 1
 * ISREG ; the result is in a register
 * DESTISLEFT put the result in the left operand (asg ops)
 * CANINDIR ; the result can be indirected through
 * FAIL ; there is no result, for we ran out of registers
 * INDEX ; some size-dependent addressing mode was used, so watch it
 * FAIL0 ; we ran out of register 0
 * USED plan to use result; don't re-use, as in 3+(*p++=*q++), register p, q
/* ASADDR is never used with a dest, and is always VALUE|ASADDR */
ret doit(), allocreg(), specialreg(), indir(), tostack(), indirit(), simpler(),
	alloctmp(), checksize(), addrsimp();

#define LEFT 1
#define RIGHT 2
char *genjmp(), *genjbc();
#define BUF 256
extern char bufs[][BUF];
extern char *buf, *bufend;
extern char prbuf[];
char *prptr;
#define done(s, n, rm) s.ans = (buf - bufs[0])/BUF; buf += BUF; s.flag = n; s.regmask = rm; if(buf >= bufend) overr(); return(s)
#define str(s)	bufs[s.ans]

enum op { Andeq = 1, And, Cmp, Comop, Decr, Diveq, Div, Xoreq, Xor,
	Incr, Lseq, Ls, Minuseq, Minus, Modeq, Mod, Muleq, Mul, Oreq,
	Or, Pluseq, Plus, Rseq, Rs, Asg, Cm, Call, Stcall, Stasg, Compl,
	Conv, Genbr, Genlab, Genubr, Star, Addr, Ucall, Uminus, Ustcall,
	Init, Funarg, Fld, Starg, Auto, Reg, Name, Param, Icon, Snode,
	Qnode, Rnode};
enum type { Tpoint, Tchar, Tuchar, Tshort, Tushort, Tint, Tuint, Tlong,
	Tulong, Tfloat, Tdouble, Tstruct, Tvoid};
typedef struct node {
	enum op op;
	enum type type;
	struct node *left, *right;
	char *name;
	int lval, rval, label, lop;
	int stsize, argsize;
} mnod;

mnod myt[];
mnod *copytree(), *gimmenode();
extern int ntree, regvar, Pflag, bbcount;
#define debugpr(s) /*printf(s), prtree(svq), putchar('\n')*/

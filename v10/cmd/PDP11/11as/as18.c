/* a8 -- pdp-11 assembler pass 1 */
#include "as1.h"

#define C_1 C_SQUO
#define C_2 C_DQUO
#define C_A C_STR
#define C_G C_GARB
#define C_I C_SKIP
#define C_P C_ESC
#define C_T C_TERM
#define C_V C_IOR
#define C_W C_WHITE
#define C_Z C_EOL

char chartab[] = {
	C_G,C_G,C_G,C_G,C_Z,C_G,C_G,C_G,
	C_G,C_W,C_Z,C_G,C_G,C_W,C_G,C_G,
	C_G,C_G,C_G,C_G,C_G,C_G,C_G,C_G,
	C_G,C_G,C_G,C_G,C_G,C_G,C_G,C_G,
	C_W,C_T,C_2,C_G,C_T,C_T,C_T,C_1,
	C_T,C_T,C_T,C_T,C_T,C_T,'.',C_I,
	'0','1','2','3','4','5','6','7',
	'8','9',C_T,C_Z,C_A,C_T,C_G,C_G,
	C_G,'A','B','C','D','E','F','G',
	'H','I','J','K','L','M','N','O',
	'P','Q','R','S','T','U','V','W',
	'X','Y','Z',C_T,C_P,C_T,C_T,'_',
	C_G,'a','b','c','d','e','f','g',
	'h','i','j','k','l','m','n','o',
	'p','q','r','s','t','u','v','w',
	'x','y','z',C_G,C_V,C_G,'~',C_G,
};

struct symtab *symend;
struct symtab *usymtab;

int curfb[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
char curfbr[10];
struct nxtfb nxtfb;

short savdot[3];
struct symtab *hshtab[HSHSIZ];
FILE *pof;
FILE *fbfil;
int errflg;
char ch;
char symbol[NCPS];
int line;
int ifflg;
int nargs;
char **curarg;
int numval;

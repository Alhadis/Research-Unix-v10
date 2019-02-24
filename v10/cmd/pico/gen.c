#include <stdio.h>
#include "pico.h"

#define	NJTAB	1000
#define	NGTAB	40
#define	NPTAB	200

static	char	*jtab[NJTAB];
static	int	jtabi;
static	int	ninstr;
static	int	spoff;

#define	LJ	1
#define	CJ	2
#define	INF	100

struct	ptab
{
	char	*at;
	char	*to;
	short	del;
	short	flag;
};

static	struct	ptab	ptab[NPTAB];
static	int	ptabi;

#define	TO	1
#define	AT	2
struct	gtab
{
	long	name;
	char	*ato;
	short	flag;
};

static	struct	gtab	gtab[NGTAB];
static	int	gtabi;

static	char	*p;

#define	MAXJ	0x7F
Node	zero =
{
	CONST
};
Node	one =
{
	CONST
};

/*
 * Addressing modes
 */
#define	_SP		0x7E	/* -(sp) */
#define	SPPLUS		0x8E	/* (sp)+ */
#define	OSP		0xAE	/* x(SP) */
#define	STARPC		0x9F	/* *(pc)+ */
#define	R0		0x50
#define	I0		0x40
#define	SHORTIM(c)	c
#define	LONGIM		0x8F	/* 32 bit immediate */
#define	R(x)		(R0|(x))/* register var */
#define	I(x)		(I0|(x))/* indexed var */
/*
 * Instructions
 */
#define	ADDL2	0xC0
#define	ADDL3	0xC1
#define	ASHL	0x78
#define	BEQL	0x13
#define	BICL2	0xCA
#define	BICL3	0xCB
#define	BGEQ	0x18
#define	BGTR	0x14
#define	BISL2	0xC8
#define	BISL3	0xC9
#define	BLEQ	0x15
#define	BLSS	0x19
#define	BNEQ	0x12
#define	BRB	0x11
#define	BRW	0x31
#define	JSB	0x16
#define	CALLS	0xFB
#define	CLRL	0xD4
#define	CMPL	0xD1
#define	BITL	0xD3
#define	MCOML	0xD2
#define	MNEGL	0xCE
#define	MOVL	0xD0
#define MOVW	0xB0
#define	MOVB	0x90
#define	MOVZBL	0x9A
#define	CLRB	0x94
#define	MULL2	0xC4
#define	MULL3	0xC5
#define	PUSHL	0xDD
#define	RSB	0x05
#define	SUBL2	0xC2
#define	SUBL3	0xC3
#define	TSTL	0xD5
#define	XORL2	0xCC
#define	XORL3	0xCD
#define DIVL2	0xC6
#define DIVL3	0xC7
#define JMP	0x17

#define	stufflong(c)	*((int *)p)=(c), p+=sizeof(long)
#define	stuffword(c)	*((short *)p)=(c), p+=sizeof(short)

#define	TOOMANY	6	/* r6 is not available for scratch */

char *program;
extern char seetree, optim;

compile(n, progr)
	Node *n;
	char *progr;
{
	register fd;

	program = progr;
	one.arg = 1;
	if (seetree && n) ptree(n, "ORIGINAL");
	if (optim)
	{	rewrite(n, 1);
		if (seetree) ptree(n, "OPTIMIZED");
	}
	gencode(n);
	if((fd=creat("/tmp/pico.code", 0666))>=0)
	{	write(fd, program, p-program);
		close(fd);
	}
}

gencode(n)
	Node *n;
{
	spoff = 0;
	ninstr = 0;
	jtabi = 0;
	ptabi = 0;
	gtabi = 0;
	p = program;
	codegen(n, 0, 1, 0);
	emitins(RSB);
	addptab();
	if(spoff != 0)
		yyerror("phase error in spoff");
}

testgen(n, used, dest, true)
	Node *n;
{
	int j;

	j = BNEQ;
	if(!true)
		j = BEQL;
	switch(n->type) {
	default:
		codegen(n, used, dest, 0);
		break;

	case OCALL:
	case CCALL:
		codegen(n, used, dest, 0);
		if(dest == 1) {
			emitins(TSTL);
			emitreg(dest);
		}
		break;

	case CONST:
	case VAR:
	case REG:
	case OARG:
		emitins(TSTL);
		codegen(n, used, 0, 0);
		break;

	case CONDI:
		markj(0);
		testgen(n->other, used, dest, 1);
			markj(0);
			testgen(n->right, used, dest, true);
		markj(0);
		markj(BRB);
		patchj(2);
			markj(0);
			testgen(n->left, used, dest, true);
		patchj(1);
			combj(0);
			combj(1);
		return;

	case ONE:
	case OLE:
	case OGE:
	case OEQ:
	case OLT:
	case OGT:
	case OAND:
		j = compareop(n, used, dest, true);
		break;

	case ONOT:
		testgen(n->left, used, dest, !true);
		return;

	case OANDAND:
	case OOROR:
		if((n->type==OANDAND) ^ true) {
			testgen(n->left, used, dest, true);
			testgen(n->right, used, dest, true);
			return;
		}
		markj(0);
		testgen(n->left, used, dest, !true);
		markj(0);
		testgen(n->right, used, dest, true);
		patchj(1);
		combj(0);
		return;

	case OCOMMA:
		codegen(n->left, used, dest, 0);
		testgen(n->right, used, dest, true);
		return;
	}
	markj(j);
}

patchj(bn)
{	int i, l;

	jtab[jtabi] = 0;
	l = jbn(bn);
	for(i=l+1; jtab[i]; i++) {
		ptab[ptabi].at = jtab[i];
		ptab[ptabi].to = p;
		ptab[ptabi].del = 0;
		ptab[ptabi].flag = 0;
		if(jtab[i][-1] != BRB)
			ptab[ptabi].flag |= CJ;
		ptabi++;
		if(ptabi >= NPTAB)
			yyerror("NPTAB too small");
	}
	while(i < jtabi)
		jtab[l++] = jtab[i++];
	jtabi = l;
}

pcmp(p1, p2)
	struct ptab *p1, *p2;
{
	return(p1->at - p2->at);
}

addptab()
{	char *q;
	int i, j, f, d;

	for(i=0; i<gtabi; i++)
	if(gtab[i].flag & AT)
	{	gtab[i].flag = 0;
		for(j=0; j<gtabi; j++)
		if(gtab[j].flag & TO && gtab[i].name == gtab[j].name)
		{	ptab[ptabi].at = gtab[i].ato;
			ptab[ptabi].to = gtab[j].ato;
			ptab[ptabi].del = 0;
			ptab[ptabi].flag = 0;
			ptabi++;
			if(ptabi >= NPTAB)
				yyerror("NPTAB too small");
			break;
		}
		if (j == gtabi)
			yyerror("label not declared");
	}

	ptab[ptabi].at = program;
	ptab[ptabi].to = p;
	ptab[ptabi].del = 0;
	ptab[ptabi].flag = LJ;
	ptabi++;
	qsort(ptab, ptabi, sizeof ptab[0], pcmp);
loop:
	f = 0;
	for(i=0; i<ptabi; i++) {
		if(ptab[i].flag & LJ)
			continue;
		d = ptab[i].to - ptab[i].at - 1;
		if(d < 0)
			d = -d;
		d += ptab[i].del;
		if(d >= MAXJ) {
			f++;
			ptab[i].flag |= LJ;
			for(j=0; j<ptabi; j++)
			if(betwn(ptab[i].at, ptab[j])) {
				ptab[j].del += 1;
				if(ptab[i].flag & CJ)
					ptab[j].del += 2;
			}
		}
	}
	if(f)
		goto loop;
	q = p;
	p += ptab[0].del;
	for(;;) {
		*--p = *--q;
		if(q > ptab[ptabi-1].at)
			continue;
		if(q <= program)
			break;
		ptabi--;
		d = ptab[ptabi].to - ptab[ptabi].at - 1;
		if(ptab[ptabi].to > ptab[ptabi].at)
			d += ptab[ptabi].del;
		else
			d -= ptab[ptabi].del;
		f = ptab[ptabi].flag;
		if((f & LJ) == 0) {
			*p = d;
			continue;
		}
		p--; q--;
		*(short *)p = d;
		*--p = BRW;
		if((f & CJ) == 0)
			continue;
		*--p = 3;
		*--p = invrbr(*q & 0xFF);
		ninstr++;
	}
	if(p != q)
		yyerror("long jump phase error");
	p = ptab[0].to + ptab[0].del;
}

betwn(p, j)
char *p;
struct ptab j;
{

	if(j.to > j.at) {
		if(p > j.at && p < j.to)
			return 1;
		return 0;
	}
	if(p >= j.to && p <= j.at)
		return 1;
	return 0;
}

combj(bn)
{
	int i, l;

	l = jbn(bn);
	i = l+1;
	while(i < jtabi)
		jtab[l++] = jtab[i++];
	jtabi = l;
}

jbn(bn)
{
	int i;

	i = jtabi;
	while(bn >= 0) {
		for(i--; jtab[i]; i--)
			;
		bn--;
	}
	return i;
}

markj(j)
{

	if(jtabi >= NJTAB)
		yyerror("NJTAB too small");
	if(j == 0) {
		jtab[jtabi++] = 0;
		return;
	}
	emitins(j);
	jtab[jtabi++] = p;
	*p++ = 0;
}

codegen(n, used, dest, brgen)
Node *n;
{
	long c;
	int o;

	markj(0);
	if(p-program > NPROG-10)
		yyerror("NPROG too small");

	if(n)
	switch(n->type)
	{
	default:
		printf("unknown type = %d\n", n->type);
		yyerror("unknown opcode in codegen");
		break;

	case LABL:
		if(gtabi >= NGTAB)
			yyerror("NGTAB too small");
		gtab[gtabi].name = n->arg;
		gtab[gtabi].ato = p;
		gtab[gtabi].flag = TO;
		gtabi++;
		codegen(n->left, used, dest, brgen);
		break;

	case GOTO:
		emitins(BRB);
		if(gtabi >= NGTAB)
			yyerror("NGTAB too small");
		gtab[gtabi].name = n->arg;
		gtab[gtabi].ato = p;
		gtab[gtabi].flag = AT;
		gtabi++;
		*p++ = 0;
		break;

	case CONSTB:
		if(n->arg & ~63)
		if(!dest) {
			*p++ = LONGIM;
			*p++ = n->arg;
			break;
		}
	case CONST:
		if(dest) {
			if(n->arg == 0) {
				emitins(CLRL);
				emitreg(dest);
				break;
			}
			emitins(MOVL);
			codegen(n, used, 0, 0);
			emitreg(dest);
			break;
		}
		emitcon(n->arg);
		break;

	case VAR:
		if(dest) {
			emitins(MOVL);
			codegen(n, used, 0, 0);
			emitreg(dest);
			break;
		}
		emitabs(n->arg);
		break;

	case OARG:
		if(dest) {
			emitins(MOVL);
			codegen(n, used, 0, 0);
			emitreg(dest);
			break;
		}
		*p++ = OSP;
		emitcon((n->arg+1)*4 + spoff);
		break;

	case REG:
		if(dest) {
			emitins(MOVL);
			codegen(n, used, 0, 0);
			emitreg(dest);
			break;
		}
		*p++ = R(n->arg);
		break;

	case CCALL:
	case OCALL:
		for(o=0; o<used; o++) {
			emitins(MOVL);
			*p++ = R(o);
			*p++ = _SP;
			spoff += 4;
		}
		c = pushargs(n->left);
		if(n->type == CCALL) {
			emitins(CALLS);
			emitcon(c);
			emitabs(n->arg);
		} else {
			emitins(JSB);
			emitabs(n->arg);
			if(c) {
				emitins(ADDL2);
				emitcon(c*4);
				*p++ = R(0XE);
			}
		}
		spoff -= 4*c;
		if(dest != 1) {
			emitins(MOVL);
			*p++ = R(0);
			*p++ = R(dest-1);
		}
		for(o=used-1; o>=0; o--) {
			emitins(MOVL);
			*p++ = SPPLUS;
			*p++ = R(o);
			spoff -= 4;
		}
		break;

	case CONDI:
		markj(0);
		if(!n->left) {
			testgen(n->other, used, dest, 1);
			codegen(n->right, used, dest, 0);
			combj(0);
			break;
		}
		if(!n->right) {
			testgen(n->other, used, dest, 0);
			codegen(n->left, used, dest, 0);
			combj(0);
			break;
		}
		testgen(n->other, used, dest, 1);
			codegen(n->right, used, dest, 1);
		patchj(1);
			codegen(n->left, used, dest, 0);
		combj(0);
		break;

	case OADD:
		binop(1, ADDL3, ADDL2, n, used, dest);
		break;

	case OMUL:
		if(n->right->type == CONST) {
			c = n->right->arg;
			if(c >= 2 && c <= 4) {
				if(n->left->type == REG) {
					emitins(ADDL3);
					codegen(n->left, used, 0, 0);
					codegen(n->left, used, 0, 0);
				} else {
					codegen(n->left, used, dest, 0);
					emitins(ADDL3);
					emitreg(dest);
					emitreg(dest);
				}
				if(c == 3) {
					*p++ = _SP;
					emitins(ADDL3);
					*p++ = SPPLUS;
					if(n->left->type == REG)
						codegen(n->left, used, 0, 0);
					else
						emitreg(dest);
				}
				if(c == 4) {
					emitreg(dest);
					emitins(ADDL3);
					emitreg(dest);
					emitreg(dest);
				}
				emitreg(dest);
				break;
			}
		}
		binop(1, MULL3, MULL2, n, used, dest);
		break;

	case OPOW:
		if(n->right->type == CONST) {
			c = n->right->arg;
			if(c >= 2 && c <= 4) {
				if(n->left->type == REG) {
					emitins(MULL3);
					codegen(n->left, used, 0, 0);
					codegen(n->left, used, 0, 0);
				} else {
					codegen(n->left, used, dest, 0);
					emitins(MULL3);
					emitreg(dest);
					emitreg(dest);
				}
				if(c == 3) {
					*p++ = _SP;
					emitins(MULL2);
					*p++ = SPPLUS;
				}
				if(c == 4) {
					emitreg(dest);
					emitins(MULL3);
					emitreg(dest);
					emitreg(dest);
				}
				emitreg(dest);
				break;
			}
		}
		yyerror("POW not implemented");
		break;

	case OXOR:
		binop(1, XORL3, XORL2, n, used, dest);
		break;

	case OOR:
		binop(1, BISL3, BISL2, n, used, dest);
		break;

	case OISUB:
		invert(n, OSUB);
	case OSUB:
		binop(0, SUBL3, SUBL2, n, used, dest);
		break;

	case OIDIV:
		invert(n, ODIV);
	case ODIV:
		binop(0, DIVL3, DIVL2, n, used, dest);
		break;

	case OBIC:
		binop(0, BICL3, BICL2, n, used, dest);
		break;

	case OAND:
		binop(2, BICL3, BICL2, n, used, dest);
		break;

	case ONEG:
		unop(MCOML, n->left, used, dest);
		break;

	case OMINUS:
		unop(MNEGL, n->left, used, dest);
		break;

	case OLSH:
		if(n->right->type == CONST) {
			c = n->right->arg;
			if(c >= 1 && c <= 2) {
				if(n->left->type == REG) {
					emitins(ADDL3);
					codegen(n->left, used, 0, 0);
					codegen(n->left, used, 0, 0);
				} else {
					codegen(n->left, used, dest, 0);
					emitins(ADDL3);
					emitreg(dest);
					emitreg(dest);
				}
				if(c == 2) {
					emitreg(dest);
					emitins(ADDL3);
					emitreg(dest);
					emitreg(dest);
				}
				emitreg(dest);
				break;
			}
			n->right->type = CONSTB;
			binop(0, ASHL, 0, n, used, dest);
			n->right->type = CONST;
			break;
		}
		binop(0, ASHL, 0, n, used, dest);
		break;

	case OCOMMA:
		codegen(n->left, used, dest, 0);
		codegen(n->right, used, dest, 0);
		break;

	case OLT:
	case OLE:
	case OEQ:
	case ONE:
	case OGT:
	case OGE:
	case ONOT:
	case OOROR:
	case OANDAND:
		markj(0);
		testgen(n, used, dest, 1);
			codegen(&zero, used, dest, 1);
		patchj(1);
			codegen(&one, used, dest, 0);
		combj(0);
		break;

	case ORETURN:
		codegen(n->left, 0, 1, 0);
		emitins(RSB);
		break;

	case OASS:
		if(complex(n->left) == 0) {
			codegen(n->right, used, dest, 0);
			emitins(MOVL);
			emitreg(dest);
			codegen(n->left, used, 0, 0);
			break;
		}
		o = n->left->type;
		if(o != DEREFB && o != DEREFL && o != DEREFS)
		{	fprintf(stderr, "o == %d -> %d: ", OASS, o);
			yyerror("assignment to non-lvalue");
		}
		if(complex(n->left->left))
			yyerror("index of non-lvalue");
		if(o == DEREFB)
			o = MOVB;
		else if (o == DEREFL)
			o = MOVL;
		else
			o = MOVW;
		codegen(n->right, used, dest, 0);
		if(n->left->right) {
			if(n->left->right->type == REG) {
				emitins(o);
				emitreg(dest);
				*p++ = I(n->left->right->arg);
			} else {
				codegen(n->left->right, used+1, used+2, 0);
				emitins(o);
				emitreg(dest);
				*p++ = I(used+2-1);
			}
			codegen(n->left->left, used, 0, 0);
			break;
		}
		emitins(o);
		emitreg(dest);
		codegen(n->left->left, used, 0, 0);
		break;

	case DEREFL:
	case DEREFS:
	case DEREFB:
		o = n->type;
		if(o == DEREFB)
			o = MOVZBL;
		else if (o == DEREFL)
			o = MOVL;
		else
			o = MOVW;
		if(complex(n->left))
			yyerror("index of non-lvalue");
		if(n->right) {
			if(n->right->type == REG) {
				emitins(o);
				*p++ = I(n->right->arg);
			} else {
				codegen(n->right, used, dest, 0);
				emitins(o);
				*p++ = I(dest-1);
			}
		} else {
			emitins(o);
		}
		codegen(n->left, used, 0, 0);
		emitreg(dest);
		break;

	case COMP:
		switch(listcount(n->left)) {
		case 1:
			movsp((Node *)0, 1, used, dest);
			movsp(n->left, 3, used, dest);
			break;
		case 2:
			movsp(n->left->right, 1, used, dest);
			movsp(n->left->left, 3, used, dest);
			break;
		case 3:
			movsp((Node *)0, 1, used, dest);
			movsp(n->left->right->right, 1, used, dest);
			movsp(n->left->right->left, 1, used, dest);
			movsp(n->left->left, 1, used, dest);
			break;
		case 4:
			movsp(n->left->right->right->right, 1, used, dest);
			movsp(n->left->right->right->left, 1, used, dest);
			movsp(n->left->right->left, 1, used, dest);
			movsp(n->left->left, 1, used, dest);
			break;
		default:
			yyerror("[] must have 1<=n<=4 args");
		}
		emitins(MOVL);
		*p++ = SPPLUS;
		emitreg(dest);
		spoff -= 4;
		break;
	}
	if(brgen)
		markj(BRB);
	else
		patchj(0);
}

emitcon(c)
{

	if((c & ~63) == 0) {
		*p++ = SHORTIM(c);
		return;
	}
	*p++ = LONGIM;
	stufflong(c);
}

emitabs(c)
{

	*p++ = STARPC;
	stufflong(c);
}

pushargs(n)
Node *n;
{
	register c;

	if(!n)
		return 0;
	if(n->type == ACOMMA) {
		c = pushargs(n->right);
		c += pushargs(n->left);
		return c;
	}
	if(complex(n)) {
		codegen(n, 0, 1, 0);
		emitins(MOVL);
		*p++ = R(0);
	} else {
		emitins(MOVL);
		codegen(n, 0, 0, 0);
	}
	*p++ = _SP;
	spoff += 4;
	return 1;
}

invert(n, o)
Node *n;
{
	n->other = n->right;
	n->right = n->left;
	n->left = n->other;
	n->other = 0;
	n->type = o;
}

listcount(n)
Node *n;
{
	register i;
	for(i=1; n->type==ACOMMA; i++, n=n->right)
		;
	return i;
}

movsp(n, count, used, dest)
Node *n;
{
	register i, c;
	if(n==0) {	/* push 255 */
		emitins(MOVB);
		*p++ = LONGIM;
		*p++ = 255;
		*p++ = _SP;
		spoff += 1;
		return;
	}
	c = complex(n);
	for(i=0; i<count; i++) {
		if(c == 0) {
			if(n->type == CONST) {
				if(n->arg == 0) {
					emitins(CLRB);
					*p++ = _SP;
					spoff += 1;
					continue;
				}
				n->type = CONSTB;
			}
			emitins(MOVB);
			codegen(n, used, 0, 0);
			*p++ = _SP;
			spoff += 1;
			continue;
		}
		if(i == 0)
			codegen(n, used, dest, 0);
		emitins(MOVB);
		emitreg(dest);
		*p++ = _SP;
		spoff += 1;
	}
}

binop(comm, threeaddr, twoaddr, n, used, dest)
Node *n;
{
	int cl, cr;

	cl = complex(n->left);
	cr = complex(n->right);
	if(cl == 0 && cr == 0)
	if(comm != 2 && threeaddr) {
		emitins(threeaddr);
		codegen(n->right, used, 0, 0);
		codegen(n->left, used, 0, 0);
		emitreg(dest);
		return;
	}
	if(cr == 0) {
		if(comm == 2) { /* OAND hack */
			unop(MCOML, n->left, used, dest);
			emitins(threeaddr);
			emitreg(dest);
			codegen(n->right, used, 0, 0);
			emitreg(dest);
			return;
		}
		codegen(n->left, used, dest, 0);
		if(twoaddr) {
			emitins(twoaddr);
			codegen(n->right, used, 0, 0);
		} else {
			emitins(threeaddr);
			codegen(n->right, used, 0, 0);
			emitreg(dest);
		}
		emitreg(dest);
		return;
	}
	if(cl == 0) {
		if(comm == 2) { /* OAND hack */
			unop(MCOML, n->right, used, dest);
			emitins(threeaddr);
			emitreg(dest);
			codegen(n->left, used, 0, 0);
			emitreg(dest);
			return;
		}
		codegen(n->right, used, dest, 0);
		if(comm && twoaddr) {
			emitins(twoaddr);
		} else {
			emitins(threeaddr);
			emitreg(dest);
		}
		codegen(n->left, used, 0, 0);
		emitreg(dest);
		return;
	}
	if(cl >= cr) {
		codegen(n->left, used, dest, 0);
		codegen(n->right, used+1, used+2, 0);
		if(comm == 2) { /* OAND hack */
			emitins(MCOML);
			emitreg(used+2);
			emitreg(used+2);
		}
		if(twoaddr) {
			emitins(twoaddr);
			emitreg(used+2);
		} else {
			emitins(threeaddr);
			emitreg(used+2);
			emitreg(dest);
		}
		emitreg(dest);
		return;
	}
	codegen(n->right, used, dest, 0);
	codegen(n->left, used+1, used+2, 0);
	if(comm == 2) { /* OAND hack */
		emitins(MCOML);
		emitreg(used+2);
		emitreg(used+2);
	}
	if(comm && twoaddr) {
		emitins(twoaddr);
	} else {
		emitins(threeaddr);
		emitreg(dest);
	}
	emitreg(used+2);
	emitreg(dest);
}

compareop(n, used, dest, true)
Node *n;
{
	int o;
	int cl, cr;

	cl = complex(n->left);
	if(n->right == Z) {
		if(cl == 0) {
			emitins(TSTL);
			codegen(n->left, used, 0, 0);
			goto out;
		}
		codegen(n->left, used, dest, 0);
		o = n->left->type;
		if(o == OCALL || o == CCALL)
		if(dest == 1) {
			emitins(TSTL);
			emitreg(dest);
		}
		goto out;
	}
	cr = complex(n->right);
	o = CMPL;
	if(n->type == OAND)
		o = BITL;
	if(cl == 0 && cr == 0) {
		emitins(o);
		codegen(n->left, used, 0, 0);
		codegen(n->right, used, 0, 0);
		goto out;
	}
	if(cr == 0) {
		codegen(n->left, used, dest, 0);
		emitins(o);
		emitreg(dest);
		codegen(n->right, used, 0, 0);
		goto out;
	}
	if(cl == 0) {
		codegen(n->right, used, dest, 0);
		emitins(o);
		codegen(n->left, used, 0, 0);
		emitreg(dest);
		goto out;
	}
	if(cl >= cr) {
		codegen(n->left, used, dest, 0);
		codegen(n->right, used+1, used+2, 0);
		emitins(o);
		emitreg(dest);
		emitreg(used+2);
		goto out;
	}
	codegen(n->right, used, dest, 0);
	codegen(n->left, used+1, used+2, 0);
	emitins(o);
	emitreg(used+2);
	emitreg(dest);

out:
	switch(n->type)
	{
	case ONE:
	case OAND:	o = BNEQ; break;
	case OEQ:	o = BEQL; break;
	case OLT:	o = BLSS; break;
	case OLE:	o = BLEQ; break;
	case OGE:	o = BGEQ; break;
	case OGT:	o = BGTR; break;
	default:	yyerror("unknown compare");
	}
	if(!true)
		o = invrbr(o);
	return o;
}

invrbr(o)
{
	switch(o)
	{
	case BNEQ:	o = BEQL; break;
	case BEQL:	o = BNEQ; break;
	case BLSS:	o = BGEQ; break;
	case BLEQ:	o = BGTR; break;
	case BGEQ:	o = BLSS; break;
	case BGTR:	o = BLEQ; break;
	case BRB:	o = BRW;  break;
	default:	yyerror("unknown branch");
	}
	return o;
}

unop(inst, n, used, dest)
Node *n;
{
	int c;
	c = complex(n);
	if(c == 0) {
		emitins(inst);
		codegen(n, used, 0, 0);
		emitreg(dest);
		return;
	}
	codegen(n, used, dest, 0);
	emitins(inst);
	emitreg(dest);
	emitreg(dest);
}

complex(n)
Node *n;
{
	int cl, cr;

	switch(n->type) {
	case CONST:
	case CONSTB:
	case VAR:
	case REG:
	case OARG:
	case GOTO:
		return 0;

	case OCALL:
	case CCALL:
		return INF;

	case OADD:
	case OMUL:
	case OPOW:
	case OXOR:
	case OOR:
	case OSUB:
	case OISUB:
	case OBIC:
	case OAND:
	case OIDIV:
	case ODIV:
	case OLT:
	case OLE:
	case OEQ:
	case ONE:
	case OGT:
	case OGE:
	case OLSH:
	case DEREFB:
	case DEREFS:
	case DEREFL:
		cl = complex(n->left);
		cr = 0;
		if(n->right)
			cr = complex(n->right);
		if(cl == cr)
			return cl+1;
		if(cl > cr)
			return cl;
		return cr;

	case LABL:
	case ORETURN:
		cl = complex(n->left);
		if(cl == 0)
			cl++;
		return cl;

	case OASS:
		cl = complex(n->left);
		cr = complex(n->right);
		if(cl >= cr)
			return cl+1;
		return cr;

	case CONDI:
		cl = complex(n->left);
		cr = complex(n->right);
		if(cl < cr)
			cl = cr;
		cr = complex(n->other);
		if(cl < cr)
			cl = cr;
		if(cl == 0)
			cl++;
		return cl;

	case OANDAND:
	case OOROR:
	case OCOMMA:
	case ACOMMA:
		cl = complex(n->left);
		cr = complex(n->right);
		if(cl < cr)
			cl = cr;
		if(cl == 0)
			cl++;
		return cl;

	case ONEG:
	case ONOT:
	case OMINUS:
	case COMP:
		cl = complex(n->left);
		if(cl == 0)
			cl++;
		return cl;
	}
	yyerror("unknown operator in complex");
	return 0;
}

emitins(o)
{
	*p++ = o;
	ninstr++;
}

emitreg(dest)
{
	if(dest>=TOOMANY)
		yyerror("TOOMANY too small");
	*p++ = R(dest-1);
}

ptree(n, s)
Node *n;
char *s;
{
	printf("== %s ==\n", s);
	prtree(n, 1, 0);
}

prtree(n, f, d)
	Node *n;
{	int i;

	if (f)
	for(i=0; i<d; i++)
		printf("  ");
	f = 0;
	d++;
	switch(n->type) {
	case LABL:	printf("Lb%d\n", n->arg); break;
	case GOTO:	printf("Gt%d\n", n->arg); return;
	case OCALL:
	case CCALL:	printf("()%d\n", n->arg); f = 1; break;
	case REG:	printf("R%d\n", n->arg); return;
	case VAR:	printf("V%d\n", n->arg); return;
	case OARG:	printf("A%d\n", n->arg); return;
	case CONST:	printf("C%d\n", n->arg); return;
	case CONDI:	printf("? "); prtree(n->other, f, d); f = 1; break;
	case COMP:	printf("[]"); break;
	case ORETURN:	printf("rt"); break;
	case DEREFB:	printf("b*"); break;
	case DEREFS:	printf("s*"); break;
	case DEREFL:	printf("l*"); break;
	case OADD:	printf("+ "); break;
	case OAND:	printf("& "); break;
	case OANDAND:	printf("&&"); break;
	case OASS:	printf(":="); break;
	case OBIC:	printf("&~"); break;
	case ACOMMA:	printf(", "); break;
	case OCOMMA:	printf("; "); break;
	case ODIV:	printf("/ "); break;
	case OEQ:	printf("=="); break;
	case OGE:	printf(">="); break;
	case OGT:	printf("> "); break;
	case OLE:	printf("<="); break;
	case OLSH:	printf("<<"); break;
	case OLT:	printf("< "); break;
	case OMINUS:	printf("u-"); break;
	case OMUL:	printf("* "); break;
	case ONE:	printf("!="); break;
	case ONEG:	printf("~ "); break;
	case ONOT:	printf("! "); break;
	case OOR:	printf("| "); break;
	case OOROR:	printf("||"); break;
	case OPOW:	printf("**"); break;
	case OSUB:	printf("- "); break;
	case OISUB:	printf("-i"); break;
	case OXOR:	printf("^ "); break;
	default:	printf("@"); break;
	}
	if (n->left)	{ prtree(n->left,  f, d); f = 1; }
	if (n->right)	{ prtree(n->right, f, d); f = 1; }
	if (!f)		printf("\n");
}

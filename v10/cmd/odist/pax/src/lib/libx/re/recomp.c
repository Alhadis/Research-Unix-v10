/*
 * AT&T Bell Laboratories
 *
 * regular expression compiler
 *
 * derived from the 9th edition regexp(3):
 *
 *	\[0-9] sub-expression references allowed in patterns
 *
 *	8 bit transparent
 *
 *	ed(1) style syntax supported as option
 */

#include "relib.h"

#if DEBUG
#include <stdio.h>
#endif

typedef struct Node			/* parser info			*/
{
	Inst*	first;
	Inst*	last;
} Node;

#define	NSTACK	32			/* misc stack depth		*/

struct state
{
	Node	andstack[NSTACK];
	Node*	andp;
	int	atorstack[NSTACK];
	int*	atorp;
	int	subidstack[NSTACK];	/* parallel to atorstack	*/
	int*	subidp;
	int	cursubid;		/* current sub-expression id	*/
	int	refsubid;		/* reference sub-expression id	*/
	int	lastwasand;		/* last token was operand	*/
	int	nbra;
	unsigned char*	exprp;		/* next source expression char	*/
	int	nclass;
	Class*	classp;
	Inst*	freep;
	int	errors;
	int	flags;			/* RE_MATCH if '\[0-9]'		*/
};

static struct state*	re;		/* compiler state		*/

#if __STDC__ || __cplusplus
extern int	chresc(const char*, char**);
extern char*	memset(char*, int, int);
extern char*	strcopy(char*, const char*);
#else
extern int	chresc();
extern char*	memset();
extern char*	strcopy();
#endif

static void
rcerror(s)
char*	s;
{
	re->errors++;
	reerror(s);
}

static void
reerr2(s, c)
register char*	s;
int		c;
{
	char	buf[100];

	s = strcopy(buf, s);
	*s++ = c;
	*s = 0;
	rcerror(buf);
}

static void
cant(s)
char*	s;
{
	char	buf[100];

	(void)strcopy(strcopy(buf, "internal error: "), s);
	rcerror(buf);
}

static Inst*
newinst(t)
int	t;
{
	re->freep->type = t;
	re->freep->left = 0;
	re->freep->right = 0;
	return(re->freep++);
}

static void
pushand(f, l)
Inst*	f;
Inst*	l;
{
	if (re->andp >= &re->andstack[NSTACK]) cant("operand stack overflow");
	re->andp->first = f;
	re->andp->last = l;
	re->andp++;
}

static Node*
popand(op)
int	op;
{
	register Inst*	inst;

	if (re->andp <= &re->andstack[0])
	{
		reerr2("missing operand for ", op);
		inst = newinst(NOP);
		pushand(inst, inst);
	}
	return(--re->andp);
}

static void
pushator(t)
int	t;
{
	if (re->atorp >= &re->atorstack[NSTACK]) cant("operator stack overflow");
	*re->atorp++ = t;
	*re->subidp++ = re->cursubid;
}

static int
popator()
{
	if (re->atorp <= &re->atorstack[0]) cant("operator stack underflow");
	re->subidp--;
	return(*--re->atorp);
}

static void
evaluntil(pri)
register int	pri;
{
	register Node*	op1;
	register Node*	op2;
	register Inst*	inst1;
	register Inst*	inst2;

	while (pri == RBRA || re->atorp[-1] >= pri)
	{
		switch(popator())
		{
		case LBRA:
			/*
			 * must have been RBRA
			 */

			op1 = popand('(');
			inst2 = newinst(RBRA);
			inst2->subid = *re->subidp;
			op1->last->next = inst2;
			inst1 = newinst(LBRA);
			inst1->subid = *re->subidp;
			inst1->next = op1->first;
			pushand(inst1, inst2);
			return;
		case OR:
			op2 = popand('|');
			op1 = popand('|');
			inst2 = newinst(NOP);
			op2->last->next = inst2;
			op1->last->next = inst2;
			inst1 = newinst(OR);
			inst1->right = op1->first;
			inst1->left = op2->first;
			pushand(inst1, inst2);
			break;
		case CAT:
			op2 = popand(0);
			op1 = popand(0);
			op1->last->next = op2->first;
			pushand(op1->first, op2->last);
			break;
		case STAR:
			op2 = popand('*');
			inst1 = newinst(OR);
			op2->last->next = inst1;
			inst1->right = op2->first;
			pushand(inst1, inst1);
			break;
		case PLUS:
			op2 = popand('+');
			inst1 = newinst(OR);
			op2->last->next = inst1;
			inst1->right = op2->first;
			pushand(op2->first, inst1);
			break;
		case QUEST:
			op2 = popand('?');
			inst1 = newinst(OR);
			inst2 = newinst(NOP);
			inst1->left = inst2;
			inst1->right = op2->first;
			op2->last->next = inst2;
			pushand(inst1, inst2);
			break;
		default:
			cant("unknown operator in evaluntil()");
			break;
		}
	}
}

static void
operator(t)
register int	t;
{
	register int	thisisand = 0;

	switch (t)
	{
	case LBRA:
		if (re->cursubid < RE_NMATCH) re->cursubid++;
		re->nbra++;
		if (re->lastwasand) operator(CAT);
		pushator(t);
		re->lastwasand = 0;
		break;
	case RBRA:
		if (--re->nbra < 0) rcerror("unmatched )");
		evaluntil(t);
		re->lastwasand = 1;
		break;
	case STAR:
	case QUEST:
	case PLUS:
		thisisand = 1;
		/* fall through ... */
	default:
		evaluntil(t);
		pushator(t);
		re->lastwasand = thisisand;
		break;
	}
}

static void
operand(t)
int	t;
{
	register Inst*	i;

	/*
	 * catenate is implicit
	 */

	if (re->lastwasand) operator(CAT);
	i = newinst(t);
	switch (t)
	{
	case CCLASS:
		i->cclass = re->classp[re->nclass - 1].map;
		break;
	case SUBEXPR:
		i->subid = re->refsubid;
		break;
	}
	pushand(i, i);
	re->lastwasand = 1;
}

static void
optimize(pp)
Prog*	pp;
{
	register Inst*	inst;
	register Inst*	target;

	for (inst = pp->firstinst; inst->type != END; inst++)
	{
		target = inst->next;
		while (target->type == NOP) target = target->next;
		inst->next = target;
	}
}

#if DEBUG
static void
dumpstack()
{
	Node*	stk;
	int*	ip;

	printf("operators\n");
	for (ip = re->atorstack; ip < re->atorp; ip++)
		printf("0%o\n", *ip);
	printf("operands\n");
	for (stk = re->andstack; stk < re->andp; stk++)
		printf("0%o\t0%o\n", stk->first->type, stk->last->type);
}

static void
dump(pp)
Prog*	pp;
{
	Inst*	l;

	l = pp->firstinst;
	do
	{
		printf("%d:\t0%o\t%d\t%d\n",
			l-pp->firstinst, l->type,
			l->left-pp->firstinst, l->right-pp->firstinst);
	} while (l++->type);
}
#endif

static int
nextc()
{
	register int	c;

	switch (c = *re->exprp++)
	{
	case 0:
		rcerror("missing ] in character class");
		break;
	case '\\':
		if (!(c = chresc((char*)re->exprp - 1, (char**)&re->exprp)))
			rcerror("trailing \\ is invalid");
		break;
	case ']':
		c = 0;
		break;
	}
	return(c);
}

static void
bldcclass()
{
	register int	c1;
	register int	c2;
	register char*	map;
	register int	negate;

	if (re->nclass >= NCLASS) reerr2("too many character classes -- limit ", NCLASS + '0');
	map = re->classp[re->nclass++].map;
	(void)memset(map, 0, elements(re->classp[0].map));

	/*
	 * we have already seen the '['
	 */

	if (*re->exprp == '^')
	{
		re->exprp++;
		negate = 1;
	}
	else negate = 0;
	if (*re->exprp == ']')
	{
		re->exprp++;
		setbit(map, ']');
	}
	if (*re->exprp == '-')
	{
		re->exprp++;
		setbit(map, '-');
	}
	while (c1 = c2 = nextc())
	{
		if (*re->exprp == '-')
		{
			re->exprp++;
			c2 = nextc();
		}
		for (; c1 <= c2; c1++) setbit(map, c1);
	}
	if (negate)
		for (c1 = 0; c1 < elements(re->classp[0].map); c1++)
			map[c1] = ~map[c1];

	/*
	 * always exclude '\0'
	 */

	clrbit(map, 0);
}

static int
lex()
{
	register int	c;

	switch(c = *re->exprp++)
	{
	case 0:
		c = END;
		re->exprp--;
		break;
	case '\\':
		switch (c = *re->exprp++)
		{
		case 0:
			re->exprp--;
			rcerror("trailing \\ is invalid");
			break;
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			if ((c - '0') > re->cursubid) reerr2("invalid sub-expression reference \\", c);
			else
			{
				re->refsubid = c - '0';
				re->flags |= RE_MATCH;
				c = SUBEXPR;
			}
			break;
		case '?':
			if (re->flags & RE_EDSTYLE) c = QUEST;
			break;
		case '+':
			if (re->flags & RE_EDSTYLE) c = PLUS;
			break;
		case '|':
			if (re->flags & RE_EDSTYLE) c = OR;
			break;
		case '(':
			if (re->flags & RE_EDSTYLE) c = LBRA;
			break;
		case ')':
			if (re->flags & RE_EDSTYLE) c = RBRA;
			break;
		case '<':
			if (re->flags & RE_EDSTYLE) c = BID;
			break;
		case '>':
			if (re->flags & RE_EDSTYLE) c = EID;
			break;
		default:
			c = chresc((char*)re->exprp - 2, (char**)&re->exprp);
			break;
		}
		break;
	case '*':
		c = STAR;
		break;
	case '.':
		c = ANY;
		break;
	case '^':
		c = BOL;
		break;
	case '$':
		c = EOL;
		break;
	case '[':
		c = CCLASS;
		bldcclass();
		break;
	case '?':
		if (!(re->flags & RE_EDSTYLE)) c = QUEST;
		break;
	case '+':
		if (!(re->flags & RE_EDSTYLE)) c = PLUS;
		break;
	case '|':
		if (!(re->flags & RE_EDSTYLE)) c = OR;
		break;
	case '(':
		if (!(re->flags & RE_EDSTYLE)) c = LBRA;
		break;
	case ')':
		if (!(re->flags & RE_EDSTYLE)) c = RBRA;
		break;
	case '<':
		if (!(re->flags & RE_EDSTYLE)) c = BID;
		break;
	case '>':
		if (!(re->flags & RE_EDSTYLE)) c = EID;
		break;
	}
	return(c);
}

#if __STDC__ || __cplusplus
reprogram*
recomp(const char* s, int reflags)
#else
reprogram*
recomp(s, reflags)
char*	s;
int	reflags;
#endif
{
	register int	token;
	Prog*		pp;
	struct state	restate;

	/*
	 * get memory for the program
	 */

	if (!(pp = (Prog*)malloc(sizeof(Prog) + 3 * sizeof(Inst) * strlen(s))))
	{
		rcerror("out of memory");
		return(0);
	}
	re = &restate;
	re->freep = pp->firstinst;
	re->classp = pp->class;
	re->errors = 0;
	re->flags = reflags & ((1<<RE_EXTERNAL) - 1);

	/*
	 * go compile the sucker
	 */

	re->exprp = (unsigned char*)s;
	re->nclass = 0;
	re->nbra = 0;
	re->atorp = re->atorstack;
	re->andp = re->andstack;
	re->subidp = re->subidstack;
	re->lastwasand = 0;
	re->cursubid = 0;

	/*
	 * start with a low priority operator to prime parser
	 */

	pushator(START - 1);
	while ((token = lex()) != END)
	{
		if (token >= OPERATOR) operator(token);
		else operand(token);
	}

	/*
	 * close with a low priority operator
	 */

	evaluntil(START);

	/*
	 * force END
	 */

	operand(END);
	evaluntil(START);
#if DEBUG
	dumpstack();
#endif
	if (re->nbra) rcerror("unmatched (");
	re->andp--;

	/*
	 * re->andp points to first and only operand
	 */

	pp->startinst = re->andp->first;
	pp->flags = re->flags;
#if DEBUG
	dump(pp);
#endif
	optimize(pp);
#ifdef DEBUG
	printf("start: %d\n", re->andp->first-pp->firstinst);
	dump(pp);
#endif
	if (re->errors)
	{
		free(pp);
		pp = 0;
	}
	return((reprogram*)pp);
}

/*
 * free program compiled by recomp()
 */

void
refree(pp)
reprogram*	pp;
{
	free(pp);
}

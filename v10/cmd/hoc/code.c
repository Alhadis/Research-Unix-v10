#include "hoc.h"
#include "y.tab.h"
#include <stdio.h>

#define	NSTACK	256

static Datum stack[NSTACK];	/* the stack */
static Datum *stackp;		/* next free spot on stack */

#define	NPROG	2000
Inst	prog[NPROG];	/* the machine */
Inst	*progp;		/* next free spot for code generation */
Inst	*pc;		/* program counter during execution */
Inst	*progbase = prog; /* start of current subprogram */
int	returning;	/* 1 if return stmt seen */

typedef struct Frame {	/* proc/func call stack frame */
	Symbol	*sp;	/* symbol table entry */
	Inst	*retpc;	/* where to resume after return */
	Datum	*argn;	/* n-th argument on stack */
	int	nargs;	/* number of arguments */
} Frame;
#define	NFRAME	100
Frame	frame[NFRAME];
Frame	*fp;		/* frame pointer */

initcode() {
	progp = progbase;
	stackp = stack;
	fp = frame;
	returning = 0;
}

push(d)
	Datum d;
{
	if (stackp >= &stack[NSTACK])
		execerror("stack too deep", (char *)0);
	*stackp++ = d;
}

Datum pop()
{
	if (stackp == stack)
		execerror("stack underflow", (char *)0);
	return *--stackp;
}

xpop()	/* for when no value is wanted */
{
	if (stackp == stack)
		execerror("stack underflow", (char *)0);
	--stackp;
}

constpush()
{
	Datum d;
	d.val = ((Symbol *)*pc++)->u.val;
	push(d);
}

varpush()
{
	Datum d;
	d.sym = (Symbol *)(*pc++);
	push(d);
}

whilecode()
{
	Datum d;
	Inst *savepc = pc;

	execute(savepc+2);	/* condition */
	d = pop();
	while (d.val) {
		execute(*((Inst **)(savepc)));	/* body */
		if (returning)
			break;
		execute(savepc+2);	/* condition */
		d = pop();
	}
	if (!returning)
		pc = *((Inst **)(savepc+1)); /* next stmt */
}

forcode()
{
	Datum d;
	Inst *savepc = pc;

	execute(savepc+4);		/* precharge */
	(void) pop();
	execute(*((Inst **)(savepc)));	/* condition */
	d = pop();
	while (d.val) {
		execute(*((Inst **)(savepc+2)));	/* body */
		if (returning)
			break;
		execute(*((Inst **)(savepc+1)));	/* post loop */
		(void) pop();
		execute(*((Inst **)(savepc)));	/* condition */
		d = pop();
	}
	if (!returning)
		pc = *((Inst **)(savepc+3)); /* next stmt */
}
ifcode() 
{
	Datum d;
	Inst *savepc = pc;	/* then part */

	execute(savepc+3);	/* condition */
	d = pop();
	if (d.val)
		execute(*((Inst **)(savepc)));	
	else if (*((Inst **)(savepc+1))) /* else part? */
		execute(*((Inst **)(savepc+1)));
	if (!returning)
		pc = *((Inst **)(savepc+2)); /* next stmt */
}

define(sp)	/* put func/proc in symbol table */
	Symbol *sp;
{
	sp->u.defn = progbase;	/* start of code */
	progbase = progp;	/* next code starts here */
}

call() 		/* call a function */
{
	Symbol *sp = (Symbol *)pc[0]; /* symbol table entry */
				      /* for function */
	if (fp++ >= &frame[NFRAME-1])
		execerror(sp->name, "call nested too deeply");
	fp->sp = sp;
	fp->nargs = (int)pc[1];
	fp->retpc = pc + 2;
	fp->argn = stackp - 1;	/* last argument */
	execute(sp->u.defn);
	returning = 0;
}

ret() 		/* common return from func or proc */
{
	int i;
	for (i = 0; i < fp->nargs; i++)
		pop();	/* pop arguments */
	pc = (Inst *)fp->retpc;
	--fp;
	returning = 1;
}

funcret() 	/* return from a function */
{
	Datum d;
	if (fp->sp->type == PROCEDURE)
		execerror(fp->sp->name, "(proc) returns value");
	d = pop();	/* preserve function return value */
	ret();
	push(d);
}

procret() 	/* return from a procedure */
{
	if (fp->sp->type == FUNCTION)
		execerror(fp->sp->name,
			"(func) returns no value");
	ret();
}

double *getarg() 	/* return pointer to argument */
{
	int nargs = (int) *pc++;
	if (nargs > fp->nargs)
	    execerror(fp->sp->name, "not enough arguments");
	return &fp->argn[nargs - fp->nargs].val;
}

arg() 	/* push argument onto stack */
{
	Datum d;
	d.val = *getarg();
	push(d);
}

argassign() 	/* store top of stack in argument */
{
	Datum d;
	d = pop();
	push(d);	/* leave value on stack */
	*getarg() = d.val;
}

argaddeq() 	/* store top of stack in argument */
{
	Datum d;
	d = pop();
	d.val = *getarg() += d.val;
	push(d);	/* leave value on stack */
}

argsubeq() 	/* store top of stack in argument */
{
	Datum d;
	d = pop();
	d.val = *getarg() -= d.val;
	push(d);	/* leave value on stack */
}

argmuleq() 	/* store top of stack in argument */
{
	Datum d;
	d = pop();
	d.val = *getarg() *= d.val;
	push(d);	/* leave value on stack */
}

argdiveq() 	/* store top of stack in argument */
{
	Datum d;
	d = pop();
	d.val = *getarg() /= d.val;
	push(d);	/* leave value on stack */
}

argmodeq() 	/* store top of stack in argument */
{
	Datum d;
	double *x;
	long y;
	d = pop();
	/* d.val = *getarg() %= d.val; */
	x = getarg();
	y = *x;
	d.val = *x = y %= (long) d.val;
	push(d);	/* leave value on stack */
}

bltin() 
{

	Datum d;
	d = pop();
	d.val = (*(double (*)())*pc++)(d.val);
	push(d);
}

add()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val += d2.val;
	push(d1);
}

sub()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val -= d2.val;
	push(d1);
}

mul()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val *= d2.val;
	push(d1);
}

div()
{
	Datum d1, d2;
	d2 = pop();
	if (d2.val == 0.0)
		execerror("division by zero", (char *)0);
	d1 = pop();
	d1.val /= d2.val;
	push(d1);
}

mod()
{
	Datum d1, d2;
	long x;
	d2 = pop();
	if (d2.val == 0.0)
		execerror("division by zero", (char *)0);
	d1 = pop();
	/* d1.val %= d2.val; */
	x = d1.val;
	x %= (long) d2.val;
	d1.val = d2.val = x;
	push(d1);
}

negate()
{
	Datum d;
	d = pop();
	d.val = -d.val;
	push(d);
}

verify(s)
	Symbol *s;
{
	if (s->type != VAR && s->type != UNDEF)
		execerror("attempt to evaluate non-variable", s->name);
	if (s->type == UNDEF)
		execerror("undefined variable", s->name);
}

eval()		/* evaluate variable on stack */
{
	Datum d;
	d = pop();
	verify(d.sym);
	d.val = d.sym->u.val;
	push(d);
}

preinc()
{
	Datum d;
	d.sym = (Symbol *)(*pc++);
	verify(d.sym);
	d.val = d.sym->u.val += 1.0;
	push(d);
}

predec()
{
	Datum d;
	d.sym = (Symbol *)(*pc++);
	verify(d.sym);
	d.val = d.sym->u.val -= 1.0;
	push(d);
}

postinc()
{
	Datum d;
	double v;
	d.sym = (Symbol *)(*pc++);
	verify(d.sym);
	v = d.sym->u.val;
	d.sym->u.val += 1.0;
	d.val = v;
	push(d);
}

postdec()
{
	Datum d;
	double v;
	d.sym = (Symbol *)(*pc++);
	verify(d.sym);
	v = d.sym->u.val;
	d.sym->u.val -= 1.0;
	d.val = v;
	push(d);
}

gt()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val > d2.val);
	push(d1);
}

lt()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val < d2.val);
	push(d1);
}

ge()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val >= d2.val);
	push(d1);
}

le()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val <= d2.val);
	push(d1);
}

eq()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val == d2.val);
	push(d1);
}

ne()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val != d2.val);
	push(d1);
}

and()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val != 0.0 && d2.val != 0.0);
	push(d1);
}

or()
{
	Datum d1, d2;
	d2 = pop();
	d1 = pop();
	d1.val = (double)(d1.val != 0.0 || d2.val != 0.0);
	push(d1);
}

not()
{
	Datum d;
	d = pop();
	d.val = (double)(d.val == 0.0);
	push(d);
}

power()
{
	Datum d1, d2;
	extern double Pow();
	d2 = pop();
	d1 = pop();
	d1.val = Pow(d1.val, d2.val);
	push(d1);
}

assign()
{
	Datum d1, d2;
	d1 = pop();
	d2 = pop();
	if (d1.sym->type != VAR && d1.sym->type != UNDEF)
		execerror("assignment to non-variable",
			d1.sym->name);
	d1.sym->u.val = d2.val;
	d1.sym->type = VAR;
	push(d2);
}

addeq()
{
	Datum d1, d2;
	d1 = pop();
	d2 = pop();
	if (d1.sym->type != VAR && d1.sym->type != UNDEF)
		execerror("assignment to non-variable",
			d1.sym->name);
	d2.val = d1.sym->u.val += d2.val;
	d1.sym->type = VAR;
	push(d2);
}

subeq()
{
	Datum d1, d2;
	d1 = pop();
	d2 = pop();
	if (d1.sym->type != VAR && d1.sym->type != UNDEF)
		execerror("assignment to non-variable",
			d1.sym->name);
	d2.val = d1.sym->u.val -= d2.val;
	d1.sym->type = VAR;
	push(d2);
}

muleq()
{
	Datum d1, d2;
	d1 = pop();
	d2 = pop();
	if (d1.sym->type != VAR && d1.sym->type != UNDEF)
		execerror("assignment to non-variable",
			d1.sym->name);
	d2.val = d1.sym->u.val *= d2.val;
	d1.sym->type = VAR;
	push(d2);
}

diveq()
{
	Datum d1, d2;
	d1 = pop();
	d2 = pop();
	if (d1.sym->type != VAR && d1.sym->type != UNDEF)
		execerror("assignment to non-variable",
			d1.sym->name);
	d2.val = d1.sym->u.val /= d2.val;
	d1.sym->type = VAR;
	push(d2);
}

modeq()
{
	Datum d1, d2;
	long x;
	d1 = pop();
	d2 = pop();
	if (d1.sym->type != VAR && d1.sym->type != UNDEF)
		execerror("assignment to non-variable",
			d1.sym->name);
	/* d2.val = d1.sym->u.val %= d2.val; */
	x = d1.sym->u.val;
	x %= (long) d2.val;
	d2.val = d1.sym->u.val = x;
	d1.sym->type = VAR;
	push(d2);
}

print()	/* pop top value from stack, print it */
{
	Datum d;
	static Symbol *s;	/* last value computed */
	if (s == NULL)
		s = install("_", VAR, 0.0);
	d = pop();
	printf("%.*g\n", (int)lookup("PREC")->u.val, d.val);
	s->u.val = d.val;
}

prexpr()	/* print numeric value */
{
	Datum d;
	d = pop();
	printf("%.*g ", (int)lookup("PREC")->u.val, d.val);
}

prstr()		/* print string value */ 
{
	printf("%s", (char *) *pc++);
}

varread()	/* read into variable */
{
	Datum d;
	extern FILE *fin;
	Symbol *var = (Symbol *) *pc++;
  Again:
	switch (fscanf(fin, "%lf", &var->u.val)) {
	case EOF:
		if (moreinput())
			goto Again;
		d.val = var->u.val = 0.0;
		break;
	case 0:
		execerror("non-number read into", var->name);
		break;
	default:
		d.val = 1.0;
		break;
	}
	var->type = VAR;
	push(d);
}

Inst *code(f)	/* install one instruction or operand */
	Inst f;
{
	Inst *oprogp = progp;
	if (progp >= &prog[NPROG])
		execerror("program too big", (char *)0);
	*progp++ = f;
	return oprogp;
}

/*
This one blows up if the increment of pc is done after the function is called.
Sigh.
 *execute(p)
 *	Inst *p;
 *{
 *	for (pc = p; *pc != STOP && !returning; )
 *		(*(*pc++))();
 *}
 */

execute(p)
	Inst *p;
{
	Inst *fp;

	for (pc = p; *pc != STOP && !returning; ) {
		fp = pc++;
		(*(*fp))();
	}
}

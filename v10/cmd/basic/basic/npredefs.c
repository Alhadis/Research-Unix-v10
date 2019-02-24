/*	"@(#) npredefs.c: V 1.16 8/19/83"	*/

/*
********* Numeric Predefined Function Handlers ************
*/

#include	<errno.h>
#include	<math.h>
#include	<stdio.h>
#include	<fcntl.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<values.h>
#include	"bas.h"
#include	"tokens.h"

struct value	usrval;
short		usrtype;

extern int	errno;

double		floor(),
		ceil(),
		pow(),
		sin(),
		cos(),
		tan(),
		atan();

extern char	strstack[],
		*strtop,	/* next available space for strings
				   on the strstack		*/
		*__loc1,
		*regcmp(),
		*regex();
extern STAKREC	*semtop;

#define ONENUMARG 	if (numargs != 1)\
			{\
				error(inst.thing.linno,inst.thing.stmtno, 52);\
				return(-1);\
			}\
			if (semtop->type == STRING)\
			{\
				error(inst.thing.linno,inst.thing.stmtno, 50);\
				return(-1);\
			}

#define ONESTRARG 	if (numargs != 1)\
			{\
				error(inst.thing.linno,inst.thing.stmtno, 52);\
				return(-1);\
			}\
			if (semtop->type != STRING)\
			{\
				error(inst.thing.linno,inst.thing.stmtno, 50);\
				return(-1);\
			}

#define STRCHEK(STRPTR)	if ((strtop + strlen(STRPTR)) > (strstack + STRMAX * 4))\
			{\
				error(inst.thing.linno,inst.thing.stmtno, 48);\
				return(-1);\
			}



absolute(numargs)
int	numargs;
{
	ONENUMARG
	switch (semtop->type)
	{
	case INTEGER:
		if (semtop->val.ival < 0)
			semtop->val.ival = -semtop->val.ival;
		break;
	case SINGLE:
		if (semtop->val.fval < 0)
			semtop->val.fval = -semtop->val.fval;
		break;
	case DOUBLE:
		if (semtop->val.dval < 0)
			semtop->val.dval = -semtop->val.dval;
		break;
	}
	return(0);
}



atn(numargs)
int	numargs;
{
	double	atan();

	ONENUMARG
	switch (semtop->type)
	{
	case INTEGER:
		semtop->val.dval = atan((double)(semtop->val.ival));
		break;
	case SINGLE:
		semtop->val.dval = atan(semtop->val.fval);
		break;
	case DOUBLE:
		semtop->val.dval = atan(semtop->val.dval);
		break;
	}
	semtop->type = DOUBLE;
	return(0);
}



cosine(numargs)
int	numargs;
{
	double	cos();

	ONENUMARG
	switch (semtop->type)
	{
	case INTEGER:
		semtop->val.dval = cos((double)(semtop->val.ival));
		break;
	case SINGLE:
		semtop->val.dval = cos(semtop->val.fval);
		break;
	case DOUBLE:
		semtop->val.dval = cos(semtop->val.dval);
		break;
	}
	semtop->type = DOUBLE;
	return(0);
}


cdbl(numargs)
int	numargs;
{
	ONENUMARG
	if (semtop->type == INTEGER)
	{
		semtop->val.dval = semtop->val.ival;
		semtop->type = DOUBLE;
	}
	else if (semtop->type == SINGLE)
	{
		semtop->val.dval = semtop->val.fval;
		semtop->type = DOUBLE;
	}
	return(0);
}


csng(numargs)
int	numargs;
{
	ONENUMARG
	if (semtop->type == INTEGER)
	{
		semtop->val.fval = semtop->val.ival;
		semtop->type = SINGLE;
	}
	else if (semtop->type == DOUBLE)
	{
		semtop->val.fval = mkfloat(semtop->val.dval);
		semtop->type = SINGLE;
	}
	return(0);
}


cint(numargs)
int	numargs;
{
	ONENUMARG
	if (semtop->type == DOUBLE)
	{
		semtop->val.ival = mkint(semtop->val.dval);
		semtop->type = INTEGER;
	}
	else if (semtop->type == SINGLE)
	{
		semtop->val.ival = mkint(semtop->val.fval);
		semtop->type = INTEGER;
	}
	return(0);
}


extern	int 	err;

extern	LINNO	erl;

erlfunc(numargs)
int numargs;
{
	if (numargs != 0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 52);
		return(-1);
	}
	semtop++;
	semtop->type = INTEGER;
	semtop->val.ival = erl;
	return(0);
}


errfunc(numargs)
int numargs;
{
	if (numargs != 0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 52);
		return(-1);
	}
	semtop++;
	semtop->type = INTEGER;
	semtop->val.ival = err;
	return(0);
}


struct FILTBL 	{
		FILE		*fildes;
		char		mode;
		unsigned char	contin;
		short		currec;
		short		curchr;
		short		recsize;
		short		numrecs;
		char		*buffer;
		};

extern struct FILTBL	filtbl[];

eof(numargs)
int	numargs;
{
	int 	filno,
	    	c;
	FILE	*fd;

	ONENUMARG
	switch (semtop->type)
	{
	case INTEGER:
		filno = semtop->val.ival;
		break;
	case SINGLE:
		filno = intify(semtop->val.fval);
		break;
	case DOUBLE:
		filno = intify(semtop->val.dval);
		break;
	}
	if (!legfildes(filno) || filtbl[filno - 1].fildes == 0
		      	      || filtbl[filno - 1].mode != 'r')
	{
		error(inst.thing.linno,inst.thing.stmtno, 37);
		return(-1);
	}
	fd = filtbl[filno - 1].fildes;
	semtop->type = INTEGER;
	if ((c = getc(fd)) == EOF)
		semtop->val.ival = -1;
	else
		semtop->val.ival = 0;
	ungetc(c, fd);
	return(0);
}



_exp(numargs)
int	numargs;
{
	double	exp();

	ONENUMARG
	switch (semtop->type)
	{
	case INTEGER:
		semtop->val.dval = exp((double)(semtop->val.ival));
		break;
	case SINGLE:
		semtop->val.dval = exp(semtop->val.fval);
		break;
	case DOUBLE:
		semtop->val.dval = exp(semtop->val.dval);
		break;
	}
	semtop->type = DOUBLE;
	return(0);
}



fix(numargs)
int	numargs;
{
	ONENUMARG
	if (semtop->type == DOUBLE)
	{
		if (semtop->val.dval > 0)
			semtop->val.dval = floor(semtop->val.dval);
		else
			semtop->val.dval = ceil(semtop->val.dval);
		semtop->type = DOUBLE;
	}
	else if (semtop->type == SINGLE)
	{
		if (semtop->val.fval > 0)
			semtop->val.fval = floor(semtop->val.fval);
		else
			semtop->val.fval = ceil(semtop->val.fval);
		semtop->type = SINGLE;
	}
	return(0);
}



instr(numargs)
int	numargs;
{
	char	subbuf[STRMAX+1],
		patbuf[2*STRMAX+1],
		*patptr,
		*argptr,
		*machine;
	int	i;

	if (numargs != 2 && numargs != 3)
	{
		error(inst.thing.linno,inst.thing.stmtno, 52);
		return(-1);
	}
	if (semtop->type != STRING || (semtop-1)->type != STRING)
	{
		error(inst.thing.linno,inst.thing.stmtno, 50);
		return(-1);
	}
	patptr = patbuf;
	argptr = semtop->val.sval;
	while (*argptr)
	{
		*patptr++ = '\\';
		*patptr++ = *argptr++;
	}
	*patptr = '\0';
	machine = regcmp(patbuf, 0);
	semtop--;
	strcpy(subbuf, semtop->val.sval);
	strtop = semtop->val.sval;
	i = 1;
	if (numargs == 3)
	{
		semtop--;
		switch (semtop->type)
		{
		case DOUBLE:	i = intify(semtop->val.dval);
				break;
		case SINGLE:	i = intify(semtop->val.fval);
				break;
		case INTEGER:	i = semtop->val.ival;
				break;
		case STRING: 	error(inst.thing.linno,inst.thing.stmtno, 50);
				if (machine != NULL)
					free(machine);
				return(-1);
		}
	}
	if (i < 1 || i > 255)
	{
		error(inst.thing.linno,inst.thing.stmtno, 55);
		if (machine != NULL)
			free(machine);
		return(-1);
	}
	semtop->type = INTEGER;
	if (*patbuf == '\0')
	{
		semtop->val.ival = i;
		if (machine != NULL)
			free(machine);
		return(0);
	}
	else if (i > strlen(subbuf) || regex(machine, subbuf+i-1) == NULL)
	{
		semtop->val.ival = 0;
		if (machine != NULL)
			free(machine);
		return(0);
	}
	else
	{
		semtop->val.ival = (__loc1 - subbuf) + 1;
		if (machine != NULL)
			free(machine);
		return(0);
	}
}


_int(numargs)
int	numargs;
{
	ONENUMARG
	switch (semtop->type)
	{
	case INTEGER:
		break;
	case SINGLE:
		semtop->val.fval = floor(semtop->val.fval);
		break;
	case DOUBLE:
		semtop->val.dval = floor(semtop->val.dval);
		break;
	}
	return(0);
}



long
flen(fp)
FILE	*fp;
{
	struct stat	buf;

	fflush(fp);
	fstat(fileno(fp), &buf);
	return((long) buf.st_size);
}


loc(numargs)
int	numargs;
{
	int 	filno;

	ONENUMARG
	switch (semtop->type)
	{
	case INTEGER:
		filno = semtop->val.ival;
		break;
	case SINGLE:
		filno = intify(semtop->val.fval);
		break;
	case DOUBLE:
		filno = intify(semtop->val.dval);
		break;
	}
	if (!legfildes(filno))
	{
		error(inst.thing.linno,inst.thing.stmtno, 47);
		return(-1);
	}
	if (filtbl[filno - 1].fildes == 0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 83);
		return(-1);
	}
	semtop->type = INTEGER;
	if (filtbl[filno -1].mode == 'u')
		semtop->val.ival = filtbl[filno - 1].currec;
	else
		semtop->val.ival = ftell(filtbl[filno - 1].fildes);
	return(0);
}



lof(numargs)
int	numargs;
{
	int 	filno;

	ONENUMARG
	switch (semtop->type)
	{
	case INTEGER:
		filno = semtop->val.ival;
		break;
	case SINGLE:
		filno = intify(semtop->val.fval);
		break;
	case DOUBLE:
		filno = intify(semtop->val.dval);
		break;
	}
	if (!legfildes(filno))
	{
		error(inst.thing.linno,inst.thing.stmtno, 47);
		return(-1);
	}
	if (filtbl[filno - 1].fildes == 0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 83);
		return(-1);
	}
	semtop->type = INTEGER;
	semtop->val.ival = flen(filtbl[filno - 1].fildes);
	return(0);
}



extern short	startpos;


pos(numargs)
int	numargs;
{
	ONENUMARG;
	semtop->type = INTEGER;
	semtop->val.ival = startpos;
	return(0);
}



_log(numargs)
int	numargs;
{
	double	log();

	ONENUMARG
	errno = 0;
	switch (semtop->type)
	{
	case INTEGER:
		semtop->val.dval = log((double)(semtop->val.ival));
		break;
	case SINGLE:
		semtop->val.dval = log(semtop->val.fval);
		break;
	case DOUBLE:
		semtop->val.dval = log(semtop->val.dval);
		break;
	}
	if (errno == DOMAIN || errno == SING)
		return(-1);
	semtop->type = DOUBLE;
	return(0);
}



sh(numargs)
int	numargs;
{
	int	retval;

	ONESTRARG
	retval = system(semtop->val.sval);
	strtop = semtop->val.sval;
	semtop->type = INTEGER;
	semtop->val.ival = retval;
	return(0);
}


sgn(numargs)
int	numargs;
{
	ONENUMARG
	switch (semtop->type)
	{
	case INTEGER:
		if (semtop->val.ival == 0)
			semtop->val.ival = 0;
		else if (semtop->val.ival > 0)
			semtop->val.ival = 1;
		else if (semtop->val.ival < 0)
			semtop->val.ival = -1;
		break;
	case SINGLE:
		if (semtop->val.fval == 0)
			semtop->val.ival = 0;
		else if (semtop->val.fval > 0)
			semtop->val.ival = 1;
		else if (semtop->val.fval < 0)
			semtop->val.ival = -1;
		semtop->type = INTEGER;
		break;
	case DOUBLE:
		if (semtop->val.dval == 0)
			semtop->val.ival = 0;
		else if (semtop->val.dval > 0)
			semtop->val.ival = 1;
		else if (semtop->val.dval < 0)
			semtop->val.ival = -1;
		semtop->type = INTEGER;
		break;
	}
	return(0);
}


double	lastrand;

rndgen(numargs)
int	numargs;
{
	double		drand48();
	double		argval;

	if (numargs == 0)
	{
		semtop++;
		semtop->type = DOUBLE;
		semtop->val.dval = lastrand = drand48();
		return(0);
	}
	if (numargs != 1)
	{
		error(inst.thing.linno,inst.thing.stmtno, 52);
		return(-1);
	}
	switch (semtop->type)
	{
	case STRING:
		error(inst.thing.linno,inst.thing.stmtno, 50);
		return(-1);
	case INTEGER:
		argval = semtop->val.ival;
		break;
	case SINGLE:
		argval = semtop->val.fval;
		break;
	case DOUBLE:
		argval = semtop->val.dval;
		break;
	}
	semtop->type = DOUBLE;
	if (argval > 0)
		semtop->val.dval = lastrand = drand48();
	else if (argval == 0)
	{
		if (lastrand != 0)
			semtop->val.dval = lastrand;
		else
			semtop->val.dval = lastrand = drand48();
	}
	else
	{
		srand48((long) -argval);
		semtop->val.dval = lastrand = drand48();
	}
	return(0);
}




sine(numargs)
int	numargs;
{
	double	sin();

	ONENUMARG
	switch (semtop->type)
	{
	case INTEGER:
		semtop->val.dval = sin((double)(semtop->val.ival));
		break;
	case SINGLE:
		semtop->val.dval = sin(semtop->val.fval);
		break;
	case DOUBLE:
		semtop->val.dval = sin(semtop->val.dval);
		break;
	}
	semtop->type = DOUBLE;
	return(0);
}




sqroot(numargs)
int	numargs;
{
	double	pow(),
		num;

	ONENUMARG
	switch (semtop->type)
	{
	case INTEGER:
		num = semtop->val.ival;
		break;
	case SINGLE:
		num = semtop->val.fval;
		break;
	case DOUBLE:
		num = semtop->val.dval;
		break;
	}
	if (num < 0.0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 19);
		return(-1);
	}
	semtop->type = DOUBLE;
	semtop->val.dval = pow(num, 0.5);
	return(0);
}



tangent(numargs)
int	numargs;
{
	double	tan();

	ONENUMARG
	switch (semtop->type)
	{
	case INTEGER:
		semtop->val.dval = tan((double)(semtop->val.ival));
		break;
	case SINGLE:
		semtop->val.dval = tan(semtop->val.fval);
		break;
	case DOUBLE:
		semtop->val.dval = tan(semtop->val.dval);
		break;
	}
	semtop->type = DOUBLE;
	return(0);
}




asc(numargs)
int	numargs;
{
	ONESTRARG
	strtop = semtop->val.sval;
	semtop->type = INTEGER;
	semtop->val.ival = *(semtop->val.sval);
	return(0);
}



len(numargs)
int	numargs;
{
	ONESTRARG
	strtop = semtop->val.sval;
	semtop->type = INTEGER;
	semtop->val.ival = strlen(semtop->val.sval);
	return(0);
}


usr0(numargs)
int	numargs;
{
	return(usr(0, numargs));
}


usr1(numargs)
int	numargs;
{
	return(usr(1, numargs));
}


usr2(numargs)
int	numargs;
{
	return(usr(2, numargs));
}


usr3(numargs)
int	numargs;
{
	return(usr(3, numargs));
}


usr4(numargs)
int	numargs;
{
	return(usr(4, numargs));
}


usr5(numargs)
int	numargs;
{
	return(usr(5, numargs));
}


usr6(numargs)
int	numargs;
{
	return(usr(6, numargs));
}


usr7(numargs)
int	numargs;
{
	return(usr(7, numargs));
}


usr8(numargs)
int	numargs;
{
	return(usr(8, numargs));
}


usr9(numargs)
int	numargs;
{
	return(usr(9, numargs));
}


usr(usrno, numargs)
int	usrno,
	numargs;
{
	if (numargs != 1)
	{
		error(inst.thing.linno,inst.thing.stmtno, 52);
		return(-1);
	}
	switch(usrtype = semtop->type)
	{
	case DOUBLE:	usrval.dval = semtop->val.dval;
			break;
	case SINGLE:	usrval.fval = semtop->val.fval;
			break;
	case INTEGER:	usrval.ival = semtop->val.ival;
			break;
	case STRING:	strtop = semtop->val.sval;
			strcpy(usrval.sval, semtop->val.sval);
			break;
	}
	if (do_usr(usrno) < 0)
		return(-1);
	switch(semtop->type = usrtype)
	{
	case DOUBLE:	semtop->val.dval = usrval.dval;
			break;
	case SINGLE:	semtop->val.fval = usrval.fval;
			break;
	case INTEGER:	semtop->val.ival = usrval.ival;
			break;
	case STRING:	STRCHEK(usrval.sval)
			strcpy(semtop->val.sval, usrval.sval);
			strtop += strlen(usrval.sval) + 1;
			break;
	}
	return(0);
}


val(numargs)
int	numargs;
{
	double 	atof1();

	ONESTRARG
	strtop = semtop->val.sval;
	semtop->type = DOUBLE;
	semtop->val.dval = atof1(semtop->val.sval);
	return(0);
}



/* handles exceptions generated by C math library routines used in the */
/* implementation of BASIC predefined functions and arithmetic ops (^) */
matherr(x)
register struct exception *x;
{
	switch (x->type)
	{
		case DOMAIN:
			if (strcmp("log", x->name) == 0)
			{
				error(inst.thing.linno,inst.thing.stmtno, 66);	/* LOG of neg.*/
				errno = DOMAIN;
				x->retval = 0;
				return(1);
			}
			if (strcmp("pow", x->name) == 0)
			{
				error(inst.thing.linno,inst.thing.stmtno, 85);	/* neg.^frac.*/
				errno = DOMAIN;
				x->retval = 0;
				return(1);
			}
			return(0);
		case SING:
			error(inst.thing.linno,inst.thing.stmtno, 67);		/* LOG of 0 */
			errno = SING;
			x->retval = -MAXDOUBLE;
			return(1);
		case OVERFLOW:
			error(inst.thing.linno,inst.thing.stmtno, 60);		/* OVERFLOW */
			x->retval = MAXDOUBLE;
			return(1);
		case UNDERFLOW:
		case TLOSS:
		case PLOSS:
			return(0);
	}
	return(0);
}



 /* Plant a random seed (RANDOMIZE COMMAND) */
_random()
{
	short	seed;
	double	dseed;
	char	*nptr,
		number[LINMAX+1];

	skip00(&expr);
	if (*expr == '\0')
	{
		do {
			printf("Random Number Seed (-32768 to 32767)? ");
			lclfgets(number,LINMAX,stdin);
			number[strlen(number)-1] = '\0';
			nptr = number;
		} while (getnum(&nptr,&dseed) < 0);
		if (dseed < 0)
			dseed -= 0.5;
		else
			dseed += 0.5;
		if (dseed < -32768. || dseed > 32767.)
		{
			error(inst.thing.linno,inst.thing.stmtno, 60);
			return(-1);
		}
		seed = (int) dseed;
	}
	else
	{
		if (eval(expr) < 0)
			return(-1);
		if (exprtype == STRING)
		{
			error(inst.thing.linno,inst.thing.stmtno, 49);
			return(-1);
		}
		else if (exprtype == DOUBLE)
		{
			if (exprval.dval < 0)	/* round off the number */
				exprval.dval -= 0.5;
			else
				exprval.dval += 0.5;
			if (exprval.dval < -32768. || exprval.dval > 32767.)
			{
				error(inst.thing.linno,inst.thing.stmtno, 60);
				return(-1);
			}
			seed = (int) exprval.dval;
		}
		else if (exprtype == SINGLE)
		{
			if (exprval.fval < 0)	/* round off the number */
				exprval.fval -= 0.5;
			else
				exprval.fval += 0.5;
			if (exprval.fval < -32768. || exprval.fval > 32767.)
			{
				error(inst.thing.linno,inst.thing.stmtno, 60);
				return(-1);
			}
			seed = (int) exprval.fval;
		}
		else
			seed = exprval.ival;
	}
	srand48((long) seed);		/* seed random gen with system time */
	return(0);
}


extern char	*hicore;

corleft()
{
	preprint();
	printf("Core used: %d bytes.\tCore remaining: %d bytes.\n",
			PGSIZ*NMPAG-(hicore-endofprog-ENCLINMAX),
			hicore-endofprog-ENCLINMAX);
	return(0);
}



fre(numargs)
{
	if (numargs != 1)
	{
		error(inst.thing.linno,inst.thing.stmtno, 52);
		return(-1);
	}
	if (semtop->type == STRING)
		strtop = semtop->val.sval;
	semtop->type = INTEGER;
	semtop->val.ival = hicore - endofprog - ENCLINMAX;
	return(0);
}

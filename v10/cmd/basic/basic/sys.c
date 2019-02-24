/*	"@(#) sys.c: V 1.14 6/6/83"	*/

/*	sys.c - system calls 	*/

#include	<stdio.h>
#include	<bas.h>
#include	<sys/types.h>
#include	<sys/stat.h>

extern char	*getenv(),
		*shpath,
		*shname,
		errbuf[];

#define	NMROFFILES	10

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

extern struct	FILTBL	filtbl[];

/*
 *	changedir command
 */
syscd()
{
	char	*dir;

	findsh(shpath = getenv("SHELL"));		/* get shell */
	if (*shname == 'r')				/* restricted shell */
	{						/* cd: restricted */
		error(inst.thing.linno,inst.thing.stmtno,64);
		return(-1);
	}
	else
	{
		skip00(&expr);		/* skip preceding blanks and tabs */
		if (*expr == '\0')		/* change to $HOME */
			dir = getenv("HOME");
		else
		{
			if (eval(expr) < 0)
				return(-1);
			if (exprtype != STRING)
			{ 				/* numeric in str*/
				error(inst.thing.linno,inst.thing.stmtno, 51);
				return(-1);
			}
			dir = exprval.sval;
		}
		if (chdir(dir) == 0)
			return(0);
		else
		{
			sprintf(errbuf, " %s\n",dir);
			error2(inst.thing.linno,inst.thing.stmtno,63,errbuf);	
						/* cannot change to directory */
			return(-1);
		}
	}
}



/* FILES command */
sysls()
{
	char	cmdlin[LINMAX+1];
	char	*space = "\" \"";

	cmdlin[0] = '\0';
	skip00(&expr);
	if (*expr == '\0')
		expr = space;
	if (eval(expr) < 0)
		return(-1);
	if (exprtype != STRING)
	{						/* numeric in str. exp*/
		error(inst.thing.linno,inst.thing.stmtno, 51);
		return(-1);
	}
	strcat(cmdlin,"ls ");
	strcat(cmdlin,exprval.sval);
	strcat(cmdlin," 2>/dev/null|pr -5 -t -w80 2>/dev/null");
	if (system(cmdlin) != 0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 15);
		return(-1);
	}
	skip00(&eoexpr);
	if (*eoexpr != '\0')
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	return(0);
}


/* KILL command */
sysrm()
{
	char	cmdlin[LINMAX+1];
	struct stat	file,
			name;
	int		i;

	cmdlin[0] = '\0';
	skip00(&expr);			/* skip preceding blanks and tabs */
	if (*expr == '\0')
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	if (eval(expr) < 0)
		return(-1);
	if (exprtype != STRING)
	{						/* numeric in str. exp*/
		error(inst.thing.linno,inst.thing.stmtno, 51);
		return(-1);
	}
	stat(exprval.sval, &name);
	for (i = 0; i<NMROFFILES; ++i)
	{
		if (filtbl[i].fildes == NULL)
			continue;
		fstat(fileno(filtbl[i].fildes), &file);
		if (name.st_dev == file.st_dev && name.st_ino == file.st_ino)
		{
			error(inst.thing.linno,inst.thing.stmtno, 1);
			return(-1);
		}
	}
	strcat(cmdlin,"rm ");
	strcat(cmdlin,exprval.sval);
	strcat(cmdlin," 2>/dev/null");
	if (system(cmdlin) != 0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 15);
		return(-1);
	}
	skip00(&eoexpr);
	if (*eoexpr != '\0')
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	return(0);
}



/* NAME command */
sysmv()
{
	char	cmdlin[BUFSIZ],
		oldname[STRMAX+1],
		newname[STRMAX+1];

	cmdlin[0] = '\0';
	if (eval(expr) < 0)
		return(-1);
	if (exprtype != STRING)
	{					/* numeric in string */
		error(inst.thing.linno,inst.thing.stmtno,51);
		return(-1);
	}
	strcpy(oldname, exprval.sval);
	expr = eoexpr;
	skip00(&expr);
	if ((*expr & 0377) != AScode)
	{						/* statement syntax */
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	++expr;
	if (eval(expr) < 0)
		return(-1);
	if (exprtype != STRING)
	{						/* numeric in string */
		error(inst.thing.linno,inst.thing.stmtno,51);
		return(-1);
	}
	strcpy(newname, exprval.sval);
	sprintf(cmdlin,"mv %s %s 2>/dev/null",oldname,newname);
	if (system(cmdlin) != 0)
	{						/* file not found */
		error(inst.thing.linno,inst.thing.stmtno, 15);
		return(-1);
	}
	return(0);
}

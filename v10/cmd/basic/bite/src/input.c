/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// input.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Sun Aug 26 07:55:00 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *
 * BASIC INPUT routine
 * PART OF BITE
 *
 * MOD,J.P.Hawkins,3-FEB-81, String variable handling.
 * MOD,J.P.Hawkins,21-FEB-81, fgetlin moved to seperate module
 *    to implement "if more" EOF test.
 * MOD,J.P.Hawkins,28-FEB-81, added string variable array assignment.
 */
/*   "@(#) input.c:  V 1.8  9/9/81" */
#include	"bas.h"
__input()
{
	register char i;
	register char *ptr;	/* pointer to program string */
	char	*prncpy();
	int	k;
	int	fildes;		/* file designator for BASIC */
	extern char ascop[];	/*use for variable name string*/
	extern struct FILTBL filtbl[];
	char linbuf[LINMAX];	/*use for input string*/
	extern int stpflg;
	int pflg;
	double	evalx();
	int	filflg;
	char	field[20];
	int	type;		/* variable type */
	char	*cptr;		/* pointer for class to play with */
	char	armpit[LINMAX];	/* gets buf */
	ptr = expr;	/* point to command operand string */
	pflg=1; /*prompt flag*/
	filflg = 0;
	if(*ptr == '_' && (*(ptr+1) >= '0' && *(ptr+1) <= '8'))
	{
		filflg = 1;
		*ptr++;
		fildes = *ptr - '1';	/* get BASIC file desig */
		*ptr++;
		if(filtbl[fildes].mode != 'r')	/* file must be open
						     for reading */
		{
			error(inst.thing.linno, 37); /* FILE NOT OPEN FOR READ */
			return(-1);
		}
	}
	while(stpflg == 0)
	{
		ptr=prncpy(ascop,ptr);		/* get field betw. ',' */
		cptr=ascop;			/* point to varname */
		type = class(&cptr, field);
		if(pflg==1)
		{
			if(filflg)
				fgetlin(armpit,fildes);
			else
			{
				printf("?");	/* input PROMPT */
				gets(armpit);
			}
			k = 0;
			pflg=0;
		}
		for(i=0;i<LINMAX;i++)
		{
			linbuf[i]=armpit[k++];
			if(linbuf[i] == '\0')
			{
				pflg=1;
				linbuf[i]='\0';
				break;
			}
			if(linbuf[i] == ',')
			{
				linbuf[i]='\0';
				break;
			}
			if(linbuf[i] == ' ')
			{
#ifdef	STRINGS
				if(type < STCLASS)
#endif
					i--;
				continue;
			}
		}
		if(linbuf[0]=='s' && type < STCLASS)
		{
				stpflg=1;
				return(0);
		}
		switch(type)
		{
			case VRCLASS:	/* regular variable name */
				putvar(field, evalx(linbuf));
				break;
			case VACLASS: /* subscripted variable class */
				aputvar(field, evalx(linbuf));
				break;
#ifdef	STRINGS
			case SVCLASS:
				sputvar(field,linbuf);
				break;
			case SACLASS:
				saputvar(field,linbuf);
				break;
#endif
			default:
				break;
		}

		if(*ptr == '\0')	/* if end of string */
		{
			if(pflg !=1)
				while(armpit[k++] != '\n'); /* eat up extra */
			return(0);	/* we're done */
		}
		 if(*ptr++ != ',')	/* else it better be a , */
		{
			error(inst.thing.linno, 13); /* delimiter error */
			return(-1);
		}
	}
	return 0;
}

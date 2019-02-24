/*	"@(#) getdims.c: V 2.17 11/10/83"	*/

/*
*   getinds.c: 	routines for making a list of indices or dimensions
*		from an array reference or dimension.
*/


#include	<values.h>
#include	<bas.h>


/*
*	makes a list of array index values given array reference string
*/

getinds(aptr, arname, numdim, dimlist)
char	**aptr,				/* pointer to pointer to array ref */	
        arname[];			/* array name */
short 	*numdim,			/* number of dimensions */
	dimlist[];			/* list of dimensions */
{
	char	expfld[LINMAX+1];		/* expression field */
	double		dm;
	short		pcnt,
			qflg;
	int		type;
	register	i;

	i = 0;
	skip00(aptr);		/* skip preceding blanks and tabs */
	
	if ((type = getid(aptr, arname)) < 0)
		return(-1);
	(*aptr)++;			/* bump past '(' */

	/*
	 * CALCULATE NUMBER AND VALUE OF EACH DIMENSION
	 */

	*numdim = 0;
	i = 0;
	pcnt = 0;
	qflg = 0;
	skip00(aptr);
	while (!((pcnt == 0) && (**aptr == ')')))
	{
		switch (**aptr)
		{
		case '(' : 	if (!qflg)
					pcnt++;
				expfld[i++] = *(*aptr)++;
				break;
		case ')' :	if (!qflg)
					pcnt--;
				expfld[i++] = *(*aptr)++;
				break;
		case '"' :	if (qflg)
					qflg = 0;
				else
					qflg = 1;
				expfld[i++] = *(*aptr)++;
				break;
		case '\0':	error(inst.thing.linno,inst.thing.stmtno, 16);
				return(-1);
		case ',' :	if (pcnt > 0)
					expfld[i++] = *(*aptr)++;
				else
				{
					(*aptr)++;	
					expfld[i] = '\0';
					i = 0;		

					/*
					 * Evaluate dimension
					 */

					if (eval(expfld) < 0)
						return(-1);
					if (exprtype == STRING)
					{
						error(inst.thing.linno,inst.thing.stmtno, 49);
						return(-1);
					}
					if (exprtype == DOUBLE)
						dm = exprval.dval;
					else if (exprtype == SINGLE)
						dm = exprval.fval;
					else
						dm = (double) exprval.ival;
					if (dm < -.5) 
					{
						error(inst.thing.linno,inst.thing.stmtno, 28);
						return(-1);
					}
					if (dm >= MAXSHORT)
					{
						error(inst.thing.linno,inst.thing.stmtno, 86);
						return(-1);
					}
					dimlist[*numdim] = dm + .5;
					/* round real index */
					(*numdim)++;  
					if (*numdim > MAXDIM)
					{
						error(inst.thing.linno,inst.thing.stmtno, 22);
						return(-1);
					}
				}
				break;
			default:	expfld[i++] = *(*aptr)++;
		}
	}
	(*aptr)++;			
	expfld[i] = '\0';
	i = 0;		
	if (eval(expfld) < 0)
		return(-1);
	if (exprtype == STRING)
	{
		error(inst.thing.linno,inst.thing.stmtno, 49);
		return(-1);
	}
	if (exprtype == DOUBLE)
		dm = exprval.dval;
	else if (exprtype == SINGLE)
		dm = exprval.fval;
	else
		dm = (double) exprval.ival;
	if (dm < -.5) 
	{
		error(inst.thing.linno,inst.thing.stmtno, 28);	/* NEG DIM */
		return(-1);
	}
	if (dm >= MAXSHORT)
	{
		error(inst.thing.linno,inst.thing.stmtno, 86);
		return(-1);
	}
	dimlist[*numdim] = dm + .5; 	/* round real to int */
	(*numdim)++;  
	if (*numdim > MAXDIM)
	{
		error(inst.thing.linno,inst.thing.stmtno, 22); /* TOO MANY DIMS */
		return(-1);
	}
	return(type);
}

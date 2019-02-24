#include	"draw_dag.h"

#include	<sys/types.h>
#include	<sys/times.h>
#define TIC 60

/*
	simplex algorithm to solve LP
*/

static int	Rows, Cols;
static long	*Auxf, *Objf, **Arrp;


static void readresults(int n_elts, int *array)
{
	for (int i = 0; i < n_elts; i++)
	{
		int row = -1;
		for(int j = 0; j < Rows; j++)
		{
			long val = Arrp[j][i];
			if(val != 0)
			{
				if(row >= 0)
				{
					row = -1;
					break;
				}
				else if(val != 1)
					break;
				else	row = j;
			}
		}
		array[i] = (row >= 0 ? Arrp[row][Cols-1] : 0);
	}
}



static int FindRow (int col)
{
	long	quot;
	int	minrow = -1;
	long	minval = Maxint;
	register int maxcol = Cols-1;

	for (register int i = 0; i < Rows; i++)
	{
		if (Arrp[i][col] <= 0)
			continue;
		quot = Arrp[i][maxcol];
		if (quot < minval)
		{
			minrow = i;
			minval = quot;
		}
	}
	return (minrow);
}

static int FindCol()
{
	long	minval = Maxint;
	int	mincol = -1;
	register int maxcol = Cols-1;

	for (register int i = 0; i < maxcol; i++)
	{
		if (Auxf[i] < minval)
		{
			mincol = i;
			minval = Auxf[i];
		}
	}
	return (mincol);
}

static void MergeRows (register long *row1, register long *row2, int col)
{
	register long fact, *maxrow1 = row1+Cols;

	switch (fact = row1[col])
	{
	case 0 :
		break;
	case 1 :
		while(row1 < maxrow1)
			*row1++ -= *row2++;
		break;
	case -1 :
		while(row1 < maxrow1)
			*row1++ += *row2++;
		break;
	default :
		while(row1 < maxrow1)
			*row1++ -= (*row2++)*fact;
		break;
	}
}

void dag_simplex(int rows, int cols,
	long **arrp, long *auxf, long *objf, int n_elts, int *array)
{
	struct tms begtm, endtm;
	int	col, row;
	long	val;

	Rows = rows;
	Cols = cols;
	Arrp = arrp;
	Auxf = auxf;
	Objf = objf;

	if(Verbose)
	{
		fprintf(stderr,"In simplex: rows = %d, Columns = %d\n",Rows,Cols-1);
		times(&begtm);
	}

	register int maxcol = Cols-1;
	for(int cnt = 0;; cnt++)
	{
		/* Find least coeff. in auxf. */
		col = FindCol();
		val = Auxf[col];
		if (val >= 0)
		{
			if (!Objf)
			{
				if (Verbose)
					fprintf (stderr, "Passes = %d\n",cnt);
				break;
			}
			Auxf = Objf;
			Objf = NULL;
			continue;
		}

		row = FindRow(col);
		val = Arrp[row][col];
		if (val != 1)
			panic("non-unit pivot");
		for(register int i = 0; i < Rows; i++)
			if(i != row)
				MergeRows(Arrp[i], Arrp[row], col);
		if(Objf != NULL)
			MergeRows(Objf, Arrp[row], col);
		MergeRows(Auxf, Arrp[row], col);
		for(i = 0; i < Rows; i++)
			if (Arrp[i][maxcol] < 0)
				panic("Non-canonical form\n");
	}

	readresults(n_elts,array);

	if(Verbose)
	{
		times(&endtm);
		int total = (endtm.tms_utime-begtm.tms_utime) +
				(endtm.tms_stime-begtm.tms_stime);
		int percent = (total - (total/TIC)*TIC)*100/TIC;
		fprintf(stderr,"Time in simplex: %d.%02ds\n",
				total/TIC, percent);
	}
}

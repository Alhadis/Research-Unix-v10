#include	"update.h"


/*
**	Find the longest prefix of tar that match some substring of src
**	This uses an inverted index for speed.
*/
#define ALPHA	(256)	/* alphabet size */

static void freemem(n_index,index)
long	*n_index;
char	***index;
{
	register int i;
	if(n_index && index)
	{
		for(i = 0; i < ALPHA; ++i)
			if(n_index[i] > 0)
				free(index[i]);
		free(index);
		free(n_index);
	}
}

/* initial assumptions: src[0] == tar[0] && src+n_match <= endsrc */
static long domatch(src,endsrc,tar,endtar,n_match)
char	*src, *endsrc, *tar, *endtar;
long	n_match;
{
	register char	*sp, *tp;

	/* see if this really improves on the current match */
	for(sp = src+n_match, tp = tar+n_match; sp > src; --sp, --tp)
		if(*sp != *tp)
			break;

	/* got an improvement, match as many more as we can */
	if(sp == src)
	{
		sp = src+n_match+1;
		tp = tar+n_match+1;
		for(; sp < endsrc && tp < endtar; ++sp, ++tp)
			if(*sp != *tp)
				break;
	}
	return tp-tar;
}

/* the real thing */
long	mtchstring(src,n_src,tar,n_tar,match)
char	*src;	/* the source string to match from */
long	n_src;	/* the length of src */
char	*tar;	/* the string a prefix of which is matched */
long	n_tar;	/* the length of tar */
char	**match; /* to return the matched address in src */
{
	char		*endsrc, *endtar;
	long		n_match;
	register int	i;
	register long	n_ind;
	register char	**ind;
	static long	*N_index = NIL(long);
	static char	*Cursrc = NIL(char), ***Index = NIL(char**);
	static int	Alloced = 0;
	char		**malloc(), **realloc();

	/* free old inverted indices if necessary */
	if(src != Cursrc)
	{
		if(Alloced)
			freemem(N_index,Index);
		Alloced = 0;
		Cursrc = NIL(char);
	}

	/* nothing to do */
	if(!src || n_src <= 0 || !tar || n_tar <= 0)
		return 0;

	endsrc = src + n_src;
	endtar = tar + n_tar;

	/* build new index structure */
	if(src != Cursrc)
	{
		Cursrc = src;
		Alloced = 1;
		if(N_index = (long*) malloc(ALPHA*sizeof(long)))
		{
			register char	*sp;

			memzero((char*)N_index,ALPHA*sizeof(long));
			if(!(Index = (char ***) malloc(ALPHA*sizeof(char**))))
			{
				free(N_index);
				N_index = NIL(long);
				Alloced = 0;
			}
			else for(sp = src; sp < endsrc; ++sp)
			{
				i = (int)(*((unsigned char *)(sp)));
				ind = Index[i];
				n_ind = N_index[i];

				/* make sure we have space */
				if((n_ind%ALPHA) == 0)
				{
					ind = n_ind == 0 ?
					      malloc(ALPHA*sizeof(char *)) :
					      realloc(ind,(n_ind+ALPHA)*sizeof(char*));
					Index[i] = ind;
				}
				if(ind)
				{
					ind[n_ind] = sp;
					N_index[i] += 1;
				}
				else
				{
					freemem(N_index,Index);
					N_index = NIL(long);
					Index = NIL(char**);
					Alloced = 0;
					break;
				}
			}
		}
	}

	/* find longest matching prefix */
	i = (int)(*((unsigned char *)(tar)));
	ind   = Alloced ? Index[i] : NULL;
	n_ind = Alloced ? N_index[i] : -1;
	n_match = 0;
	while(1)
	{
		register long m;

		if(ind)
		{
			src = n_ind > 0 ? *ind++ : endsrc;
			n_ind -= 1;
		}
		else for(; src+n_match < endsrc; ++src)
			if(*src == *tar)
				break;
		if(src+n_match >= endsrc)
			break;

		if((m = domatch(src,endsrc,tar,endtar,n_match)) > n_match)
		{
			n_match = m;
			*match = src;
			if(tar+n_match >= endtar)
				break;
		}
	}

	return n_match;
}

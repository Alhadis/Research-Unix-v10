#include	<jerq.h>
#include	<font.h>
#include	<jerqio.h>

static FILE *inf;

static
cget()
{
	return(getc(inf));
}

Font *
getfont(s)
	char *s;
{
	Font *f;

	if((inf = fopen(s, "r")) == (FILE *)0)
	{
		return((Font *)0);
	}

	f = infont(cget);
	fclose(inf);
	return(f);
}

#include <stdio.h>
#include "jerq.h"
#include "commands.h"
#include "io.h"

#define ISIZE(n)	((n+1)*sizeof(Fontchar))

#define FNTsize 20
Font *fntab[FNTsize];
FILE *inf;
extern int Jin, Jout;

int lookUpFont(f)  Font *f;
{	int i;
	for (i=0; i<FNTsize && fntab[i]!=(Font *)NULL && fntab[i]!=f; i++) ;
	if (i >= FNTsize) {
		fprintf(stderr,"out of room in font table\n");
		exit();
	}
	else if (fntab[i] == (Font *) NULL) {
		fprintf(stderr,"unknown font referenced.  DEFONT substituted.\n");
		return(0);
	}
	else return(i);
}

void ffree(f) Font *f;
{	int i,j;
	sendCommand(CCFFREE);
	i = lookUpFont(f);
	sendInt(i);
	/*close the hole*/
	for (j=i+1; j<FNTsize; j++) fntab[j-1] = fntab[j];
	fntab[FNTsize] = (Font *) NULL;
}

static ngetc(n, base) int n; char **base;
{	int x, i;

	i = 0;
	do {
		x = getc(inf);
		(*base)[i++] = x;
		if(x == -1)
			return(1);
	} while (--n > 0);
	return(0);
}


Font *loadFont(fname) char *fname;
{	short n, in;
	Font *f;
	char *temp;

	if((inf = fopen(fname, "r")) == (FILE *)NULL) return((Font *) NULL);

	temp = (char *)&in;
	if (ngetc(2, &temp)) f = (Font *) NULL;
	else {
		swab(temp, (char*)&n, 2);
		if ((f = (Font *) malloc(sizeof(Font)+ISIZE(n)))!=(Font *)NULL) {
			f->n = n;
			temp = 2 + (char *)f;
			if (ngetc(6, &temp)) {
				free(f);
				f = (Font *) NULL;
			}
			else {
				temp = (char *)f->info;
				if (ngetc(ISIZE(n), &temp)) {
					free(f);
					f = (Font *) NULL;
				}
			}
		}
	}

	fclose(inf);
	return(f);
}

Font *getfont(fname) char *fname;
{	int i, j, n; 
	FILE *fp;
	char fbuf[4096];

	if ((fp=fopen(fname,"r"))==(FILE *)NULL) {
		fprintf(stderr,"font not found: %s\n",fname);
		return((Font *)NULL);
	}
	sendCommand(CCGETFONT);
	for (i=0; i<FNTsize && fntab[i]!=(Font *) NULL; i++) ;
	if (i>=FNTsize) i=1;
	sendInt(i);
	flush();
	while((n=fread(fbuf,sizeof(char),sizeof(fbuf),fp)) > 0) 
/*		for (j=0; j<n; j++) put(fbuf[j]); */
		write(Jout,fbuf,n); 
	fclose(fp);
	fntab[i] = loadFont(fname);
	return(fntab[i]);
}

strwidth(f,s) Font *f; unsigned char *s;
{
	int wid=0;
	int n = f->n;
	Fontchar *info;

	info = f->info;
	for(; *s; s++)
		if(*s < n) wid+=info[*s].width;
	return(wid);
}

jstrwidth(s)
	char *s;
{
	return(strwidth(&defont,s));
}




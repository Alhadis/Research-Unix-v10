#include "tdef.h"
#include "fns.h"
#include "ext.h"

#define	MAXCH NCHARS		/* maximum number of global char names */
char	*chnames[MAXCH];	/* chnames[n-ALPHABET] -> name of char n */
int	nchnames;		/* number of Cxy names currently seen */

#define	MAXPS	100		/* max number of point sizes */
int	pstab[MAXPS];		/* point sizes */
int	nsizes;			/* number in DESC */

Font	fonts[MAXFONTS+1];	/* font info + ptr to width info */


#define	skipline(f)	while (getc(f) != '\n')

getdesc(char *name)
{
	FILE *fin;
	char cmd[100], s[100];
	int i, v;

	if ((fin = fopen(name, "r")) == NULL)
		return -1;
	while (fscanf(fin, "%s", cmd) != EOF) {
		if (strcmp(cmd, "res") == 0) {
			fscanf(fin, "%d", &Inch);
		} else if (strcmp(cmd, "hor") == 0) {
			fscanf(fin, "%d", &Hor);
		} else if (strcmp(cmd, "vert") == 0) {
			fscanf(fin, "%d", &Vert);
		} else if (strcmp(cmd, "unitwidth") == 0) {
			fscanf(fin, "%d", &Unitwidth);
		} else if (strcmp(cmd, "sizes") == 0) {
			nsizes = 0;
			while (fscanf(fin, "%d", &v) != EOF && v != 0 && nsizes < MAXPS)
				pstab[nsizes++] = v;
		} else if (strcmp(cmd, "fonts") == 0) {
			fscanf(fin, "%d", &nfonts);
			for (i = 1; i <= nfonts; i++) {
				fscanf(fin, "%s", s);
				fontlab[i] = PAIR(s[0], s[1]);
			}
		} else if (strcmp(cmd, "charset") == 0) {	/* add any names */
			while (fscanf(fin, "%s", s) != EOF)
				chadd(s);
			break;
		}
		/* else 
			just skip anything else */
		skipline(fin);
	}
	fclose(fin);
	return 1;
}

#define	eq(s1, s2)	(strcmp(s1, s2) == 0)

static int checkfont(char *name)
{		/* in case it's not really a font description file */
		/* really paranoid, but consider \f. */
	FILE *fp;
	char buf[300], buf2[300];
	int i, status = -1;

	if ((fp = fopen(name, "r")) == NULL)
		return -1;
	for (i = 1; i <= 10; i++) {
		if (fgets(buf, sizeof buf, fp) == NULL)
			break;
		sscanf(buf, "%s", buf2);
		if (eq(buf2, "name") || eq(buf2, "fontname") ||
		    eq(buf2, "special") || eq(buf2, "charset")) {
			status = 1;
			break;
		}
	}
	fclose(fp);
	return status;
	
}

getfont(char *name, int pos)	/* create width tab for font */
{
	FILE *fin;
	Font *ftemp = &fonts[pos];
	Chwid chtemp[MAXCH];
	static Chwid chinit;
	int i, nw, n, wid, kern, code;
	char buf[100], ch[100], s1[100], s2[100], s3[100], cmd[300];

	/* fprintf(stderr, "read font %s onto %d\n", name, pos); */
	if (checkfont(name) == -1)
		return -1;
	if ((fin = fopen(name, "r")) == NULL)
		return -1;
	for (i = 0; i < ALPHABET; i++)
		chtemp[i] = chinit;	/* zero out to begin with */
	ftemp->specfont = ftemp->ligfont = 0;
	ftemp->defaultwidth = ftemp->spacewidth = Inch * Unitwidth / 72 / 3; /* should be rounded */
	while (fscanf(fin, "%s", cmd) != EOF) {
		if (strcmp(cmd, "name") == 0)
			fscanf(fin, "%s", ftemp->longname);
		else if (strcmp(cmd, "special") == 0)
			ftemp->specfont = 1;
		else if (strcmp(cmd, "ligatures") == 0) {
			ftemp->ligfont = getlig(fin);
		} else if (strcmp(cmd, "spacewidth") == 0) {
			fscanf(fin, "%d", &ftemp->spacewidth);
		} else if (strcmp(cmd, "defaultwidth") == 0) {
			fscanf(fin, "%d", &ftemp->defaultwidth);
		} else if (strcmp(cmd, "charset") == 0) {
			skipline(fin);
			nw = ALPHABET;
			while (fgets(buf, sizeof buf, fin) != NULL) {
				sscanf(buf, "%s %s %s %s", ch, s1, s2, s3);
				if (s1[0] != '"') {	/* genuine new character */
					sscanf(s1, "%d", &wid);
					sscanf(s2, "%d", &kern);
					code = strtol(s3, 0, 0);	/* dec/oct/hex */
				}
				/* otherwise it's a synonym for prev character, */
				/* so leave previous values intact */


				/* decide what kind of alphabet it might come from here */


				if (strlen(ch) == 1) {	/* it's ascii */
					n = ch[0];	/* origin includes non-graphics */
					chtemp[n].num = ch[0];
					chtemp[n].wid = wid;
					chtemp[n].kern = kern;
					chtemp[n].code = code;
				} else if (ch[0] == '\\' && ch[1] == '0') {
					/* \0octal or \0xhex */
					n = strtol(ch+1, 0, 0);
					chtemp[n].num = n;
					chtemp[n].wid = wid;
					chtemp[n].kern = kern;
					chtemp[n].code = code;
				} else {
					if (strcmp(ch, "---") == 0) /* no name */
						sprintf(ch, "#%d", code);
					if ((n = chindex(ch)) == -1) /* global? */
						n = chadd(ch);
					chtemp[nw].num = n;
					chtemp[nw].wid = wid;
					chtemp[nw].kern = kern;
					chtemp[nw].code = code;
					nw++;
				}
				/* fprintf(stderr, "font %2.2s char %4.4s num %3d wid %2d code %3d\n",
					ftemp->longname, ch, n, wid, code);
				*/
			}
			break;
		}
		skipline(fin);
	}
	fclose(fin);
	chtemp[' '].wid = ftemp->spacewidth;	/* width of space on this font */
	ftemp->nchars = nw;
	if (ftemp->wp)
		free(ftemp->wp);	/* god help us if this wasn't allocated */
	ftemp->wp = (Chwid *) malloc(nw * sizeof(Chwid));
	if (ftemp->wp == NULL)
		return -1;
	for (i = 0; i < nw; i++)
		ftemp->wp[i] = chtemp[i];
/*
 *	printf("%d chars: ", nw);
 *	for (i = 0; i < nw; i++) {
 *		if (ftemp->wp[i].num > 0 && ftemp->wp[i].num < ALPHABET)
 *			printf("%c %d ", ftemp->wp[i].num, ftemp->wp[i].wid);
 *		else if (i >= ALPHABET)
 *			printf("%d (%s) %d ", ftemp->wp[i].num,
 *				chnames[ftemp->wp[i].num-ALPHABET], ftemp->wp[i].wid);
 *	}
 *	printf("\n");
 */
	return 1;
}

chindex(char *s)	/* look for s in global character name table */
{
	int i;

	for (i = 0; i < nchnames; i++)
		if (strcmp(s, chnames[i]) == 0)
			return i + ALPHABET;	/* offset by ALPHABET... */
	return -1;
}

chadd(char *s)	/* add s to global character name table;  assume not there */
{		/* changes when table format changes */
	char *p;

	chnames[nchnames] = p = (char *) malloc(strlen(s)+1);
	if (p == NULL) {
		ERROR "out of space adding character %s\n", s WARN;
		return LEFTHAND;
	}
	if (nchnames >= NCHARS) {
		ERROR "out ot table space adding character %s\n", s WARN;
		return LEFTHAND;
	}
	strcpy(chnames[nchnames], s);
	return nchnames++ + ALPHABET;
}

char *chname(int n)	/* return string for char with index n */
{			/* changes when table format changes */
	return chnames[n-ALPHABET];
}

getlig(FILE *fin)	/* pick up ligature list */
{
	int lig;
	char temp[200];

	lig = 0;
	while (fscanf(fin, "%s", temp) != EOF && strcmp(temp, "0") != 0) {
		if (strcmp(temp, "fi") == 0)
			lig |= LFI;
		else if (strcmp(temp, "fl") == 0)
			lig |= LFL;
		else if (strcmp(temp, "ff") == 0)
			lig |= LFF;
		else if (strcmp(temp, "ffi") == 0)
			lig |= LFFI;
		else if (strcmp(temp, "ffl") == 0)
			lig |= LFFL;
		else
			fprintf(stderr, "illegal ligature %s ignored\n", temp);
	}
	return lig;
}

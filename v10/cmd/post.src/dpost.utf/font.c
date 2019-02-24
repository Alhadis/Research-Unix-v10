/*
 *
 * Typesetter font tables routines - for postprocessors.
 *
 */

#include <stdio.h>
#include <ctype.h>

#include "gen.h"
#include "rune.h"
#include "ext.h"
#include "font.h"

Font	*mount[MAXFONTS+1];		/* mount table - pointers into fonts[] */
Font	fonts[MAXFONTS+2];		/* font data - guarantee one empty slot */

int	fcount;				/* entries in fonts[] */
int	mcount;				/* fonts currently in memory */
int	mlimit = MAXFONTS+1;		/* and the most we'll allow */

char	*chnames[SPECIALCHARS];		/* special character hash table */
int	nchnames;			/* number of entries in chnames[] */

extern int	devres;
extern int	unitwidth;
extern int	nfonts;

/*****************************************************************************/

checkdesc(path)

    char	*path;

{

    char	buf[150];
    FILE	*fp;
    int		val = 0;

/*
 *
 * Return non-zero if the typesetter description file path includes,
 *
 *	PDL PostScript
 *
 * before the charset table.
 *
 */

    if ( (fp = fopen(path, "r")) != NULL ) {
	while ( fscanf(fp, "%s", buf) != EOF ) {
	    if ( strcmp(buf, "PDL") == 0 ) {
		fscanf(fp, "%s", buf);
		val = strcmp(buf, "PostScript") == 0;
		break;
	    } else if ( strcmp(buf, "charset") == 0 )
		break;
	    skipline(fp);
	}   /* End while */
	fclose(fp);
    }	/* End if */

    return(val);

}   /* End of checkdesc */

/*****************************************************************************/

getdesc(path)

    char	*path;

{

    char	buf[150];
    FILE	*fp;
    int		n;

    if ( (fp = fopen(path, "r")) == NULL )
	return(-1);

    while ( fscanf(fp, "%s", buf) != EOF ) {
	if ( strcmp(buf, "res") == 0 )
	    fscanf(fp, "%d", &devres);
	else if ( strcmp(buf, "unitwidth") == 0 )
	    fscanf(fp, "%d", &unitwidth);
	else if ( strcmp(buf, "sizes") == 0 )
	    while ( fscanf(fp, "%d", &n) != EOF && n != 0 ) ;
	else if ( strcmp(buf, "inmemory") == 0 )
	    fscanf(fp, "%d", &mlimit);
	else if ( strcmp(buf, "Encoding") == 0 ) {
	    fscanf(fp, "%s", buf);
	    fontencoding = strsave(buf);
	} else if ( strcmp(buf, "fonts") == 0 ) {
	    fscanf(fp, "%d", &nfonts);
	    for ( n = 0; n < nfonts; n++ )
		fscanf(fp, "%s", buf);
	} else if ( strcmp(buf, "charset") == 0 ) {
	    while ( fscanf(fp, "%s", buf) != EOF )
		chadd(buf);
	    break;
	}   /* End if */
	skipline(fp);
    }	/* End while */

    fclose(fp);
    return(1);

}   /* End of getdesc */

/*****************************************************************************/

getfont(path, fpos)

    char	*path;
    Font	*fpos;

{

    FILE	*fin;
    Chwid	chtemp[LARGESTFONT];
    int		next;
    int		i, n, num, wid, code;
    char	buf[300], ch[100], s1[100], s2[100], s3[100], cmd[100];


/*
 *
 * Read a font width table. Skip unnamed characters, spacewidth, ligatures,
 * ascender/descender entries, and anything else not recognized. Charset
 * entries for a new font are first stacked in chtemp[] and later copied
 * to correct slots in a newly allocated wp array. All calls should come
 * through mountfont().
 *
 */

    if ( fpos->state == INMEMORY )
	return(1);

    if ( (fin = fopen(path, "r")) == NULL )
	return(-1);

    if ( fpos->state == NEWFONT ) {
	if ( ++fcount > MAXFONTS+1 )
	    return(-1);
	fpos->path = strsave(path);
    }	/* End if */

    if ( ++mcount > mlimit && mcount > nfonts+1 )
	freefonts();

    while ( fscanf(fin, "%s", cmd) != EOF ) {
	if ( strcmp(cmd, "name") == 0 ) {
	    release(fpos->name);
	    fscanf(fin, "%s", buf);
	    fpos->name = strsave(buf);
	} else if ( strcmp(cmd, "fontname") == 0 ) {
	    release(fpos->fontname);
	    fscanf(fin, "%s", buf);
	    fpos->fontname = strsave(buf);
	} else if ( strcmp(cmd, "special") == 0 )
	    fpos->specfont = 1;
	else if ( strcmp(cmd, "named") == 0 )	/* in prologue or somewhere else */
	    fpos->flags |= NAMED;
	else if ( strcmp(cmd, "charset") == 0 ) {
	    skipline(fin);
	    next = 0;
	    fpos->nchars = 0;		/* special characters */
	    fpos->first = LASTCODE;
	    fpos->last = FIRSTCODE;
	    while ( fgets(buf, sizeof(buf), fin) != NULL ) {
		num = -1;
		sscanf(buf, "%s %s %s %s", ch, s1, s2, s3);
		if ( s1[0] != '"' ) {		/* not a synonym */
		    sscanf(s1, "%d", &wid);
		    code = strtol(s3, 0, 0);	/* dec/oct/hex */
		}   /* End if */
		if ( strlen(ch) == 1 )		/* it's ascii */
		    num = ch[0];
		else if ( ch[0] == '\\' && ch[1] == '0' )
		    num = strtol(ch+1, 0, 0);
		/*
		 * Eventually consider something like,
		 *
		 * else if ( strlen(ch) == chartorune(&r, ch) )
		 *	num = r;
		 */
		else if ( strcmp(ch, "---") != 0 ) {	/* ignore unnamed chars */
		    if ( (num = chindex(ch)) == INVALIDCODE )
			num = chadd(ch);
		}   /* End else */
		if ( ValidChar(num) ) {
		    if ( next < LARGESTFONT ) {
			chtemp[next].num = num;
                	chtemp[next].wid = wid;
                	chtemp[next++].code = code;
			if ( ValidCode(num) ) {
			    fpos->first = (num < fpos->first) ? num : fpos->first;
			    fpos->last = (num > fpos->last) ? num : fpos->last;
			} else fpos->nchars++;
		    } else error(FATAL, "font %s too large", path);
		} else if ( num != -1 )
		    error(FATAL, "invalid character in font %s\n", path);
	    }	/* End while */
	    break;
	}   /* End else */
	skipline(fin);
    }	/* End while */

    fclose(fin);

    if ( fpos->first > fpos->last )
	fpos->last = fpos->first - 1;
    fpos->nchars += fpos->last - fpos->first + 1;
    fpos->wp = (Chwid *)allocate(fpos->nchars * sizeof(Chwid));

    for ( i = 0; i < fpos->nchars; i++ )
	fpos->wp[i].num = INVALIDCODE;

    for ( i = 0, n = 1; i < next; i++ ) {
	if ( chtemp[i].num <= fpos->last )
	    fpos->wp[chtemp[i].num - fpos->first] = chtemp[i];
	else fpos->wp[fpos->last - fpos->first + n++] = chtemp[i];
    }	/* End for */

    fpos->state = INMEMORY;
    return(1);

}   /* End of getfont */

/*****************************************************************************/

mountfont(path, m)

    char	*path;
    int		m;

{

    Font	*fpos;

    if ( m < 0 || m > MAXFONTS )
	return(-1);

    if ( mount[m] != NULL ) {
	if ( mount[m]->path != NULL && strcmp(path, mount[m]->path) == 0 ) {
	    if ( mount[m]->state == INMEMORY )
		return(1);
	} else {
	    mount[m]->mounted--;
	    mount[m] = NULL;
	}   /* End else */
    }	/* End if */

    mount[m] = fpos = &fonts[findfont(path)];
    mount[m]->mounted++;
    return(getfont(path, fpos));

}   /* End of mountfont */

/*****************************************************************************/

freefonts()

{

    int		n;

/*
 *
 * Don't release the path without resetting state to NEWFONT - findfont()
 * assumes path is available.
 *
 */

    for ( n = 0; n < MAXFONTS+2; n++ )
	if ( fonts[n].state == INMEMORY && fonts[n].mounted == 0 ) {
	    release(fonts[n].wp);
	    fonts[n].wp = NULL;
	    fonts[n].state = RELEASED;
	    mcount--;
	}   /* End if */

}   /* End of freefonts */

/*****************************************************************************/

findfont(path)

    char	*path;

{

    register	n;

    for ( n = hash(path, MAXFONTS+2); fonts[n].state != NEWFONT; n = (n+1) % (MAXFONTS+2) )
	if ( strcmp(path, fonts[n].path) == 0 )
	    break;
    return(n);

}   /* End of findfont */

/*****************************************************************************/

mounted(m)

    int		m;

{

    return(m >= 0 && m <= MAXFONTS && mount[m] != NULL);

}   /* End of mounted */

/*****************************************************************************/

onfont(c, m)

    int		c;
    int		m;

{

    Font	*fp;
    Chwid	*cp, *ep;

/*
 *
 * Returns the position of character c in the font mounted at m, or -1 if the
 * character is not found.
 *
 */

    if ( mounted(m) ) {
	fp = mount[m];
	if ( c >= fp->first && c <= fp->last ) {
	    if ( fp->wp[c-fp->first].num == c )
		return(c - fp->first);
	    else return(-1);
	}   /* End if */

	if ( ValidSpecial(c) ) {
	    cp = &fp->wp[fp->last - fp->first + 1];
	    ep = &fp->wp[fp->nchars];
	    for ( ; cp < ep; cp++ )			/* search others */
    		if ( cp->num == c )
		    return(cp - &fp->wp[0]);
	}   /* End if */
    }	/* End if */

    return(-1);

}   /* End of onfont */

/*****************************************************************************/

chindex(s)

    char	*s;

{

    register	i;

/*
 *
 * Return the code assigned to special character s or INVALIDCODE if s
 * is not currently defined.
 *
 */

    for ( i = hash(s, SPECIALCHARS); chnames[i] != NULL; i = (i+1) % SPECIALCHARS )
	if ( strcmp(s, chnames[i]) == 0 )
	    return(i+FIRSTSPECIAL);
    return(INVALIDCODE);

}   /* End of chindex */

/*****************************************************************************/

chadd(s)

    char	*s;

{

    register	i;

    if ( nchnames >= SPECIALCHARS - 1 )		/* guarantee one empty slot */
	error(FATAL, "out of table space adding character %s", s);

    for ( i = hash(s, SPECIALCHARS); chnames[i] != NULL; i = (i+1) % SPECIALCHARS ) ;

    nchnames++;
    chnames[i] = strsave(s);
    return(i+FIRSTSPECIAL);

}   /* End of chadd */

/*****************************************************************************/

char *chname(n)

    int		n;

{

    return(chnames[n-FIRSTSPECIAL]);

}   /* End of chname */

/*****************************************************************************/

hash(s, l)

    char	*s;
    int		l;

{

    unsigned	i;

    for ( i = 0; *s; s++ )
	i = i*10 + *s;
    return(i % l);

}   /* End of hash */

/*****************************************************************************/

char *strsave(s)

    char	*s;

{

    char	*ptr = NULL;

    if ( s != NULL ) {
	ptr = (char *)allocate(strlen(s)+1);
	strcpy(ptr, s);
    }	/* End if */
    return(ptr);

}   /* End of strsave */

/*****************************************************************************/

char *allocate(count)

    int		count;

{

    char	*ptr;

    if ( (ptr = (char *)malloc(count)) == NULL ) {
	freefonts();
	if ( (ptr = (char *)malloc(count)) == NULL )
	    error(FATAL, "no memory");
    }	/* End if */
    return(ptr);

}   /* End of allocate */

/*****************************************************************************/

release(ptr)

    char	*ptr;

{

    if ( ptr != NULL )
	free(ptr);

}   /* End of release */

/*****************************************************************************/

dumpmount(m)

    int		m;

{

    if ( mount[m] != NULL )
	dumpfont((mount[m] - &fonts[0]));
    else fprintf(stderr, "no font mounted at %d\n", m);

}   /* End of dumpmount */

/*****************************************************************************/

dumpfont(n)

    int		n;

{

    int		i;
    Font	*fpos;
    char	*str;

    fpos = &fonts[n];

    if ( fpos->state ) {
	fprintf(stderr, "path %s\n", fpos->path);
	fprintf(stderr, "state %d\n", fpos->state);
	fprintf(stderr, "flags %d\n", fpos->flags);
	fprintf(stderr, "mounted %d\n", fpos->mounted);
	fprintf(stderr, "first %d\n", fpos->first);
	fprintf(stderr, "last %d\n", fpos->last);
	fprintf(stderr, "nchars %d\n", fpos->nchars);
	fprintf(stderr, "special %d\n", fpos->specfont);
	fprintf(stderr, "name %s\n", fpos->name);
	fprintf(stderr, "fontname %s\n", fpos->fontname);
	if ( fpos->state == INMEMORY ) {
	    fprintf(stderr, "charset\n");
	    for ( i = 0; i < fpos->nchars; i++ ) {
		if ( fpos->wp[i].num > 0 ) {
		    if ( fpos->wp[i].num <= fpos->last )
			fprintf(stderr, "%c\t%d\t%d\n", fpos->wp[i].num,
				fpos->wp[i].wid, fpos->wp[i].code);
		    else {
			str = chname(fpos->wp[i].num);
			if ( *str == '#' && isdigit(*(str+1)) && isdigit(*(str+2)) )
			    str = "---";
			fprintf(stderr, "%s\t%d\t%d\n", str, fpos->wp[i].wid,
				fpos->wp[i].code);
		    }	/* End else */
		}   /* End if */
	    }	/* End for */
	} else fprintf(stderr, "charset: not in memory\n");
    } else fprintf(stderr, "empty font: %d\n", n);

    putc('\n', stderr);

}   /* End of dumpfont */

/*****************************************************************************/


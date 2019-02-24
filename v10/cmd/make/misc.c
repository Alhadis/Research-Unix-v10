static	char *sccsid = "@(#)misc.c	8th Edition (Bell Labs) 85/05/25";
#include "defs"
#include <ctype.h>



/* simple linear hash.  hash function is sum of
   characters mod hash table size.
*/
hashloc(s)
char *s;
{
register int i;
register int hashval;
register char *t;

hashval = 0;

for(t=s; *t!='\0' ; ++t)
	hashval += *t;

hashval %= hashsize;

for(i=hashval;
	hashtab[i]!=0 && !equal(s,hashtab[i]->namep);
	i = i >= hashsize-1 ? 0 : i+1) ;

return i;
}


nameblkp srchname(s)
char *s;
{
return  hashtab[hashloc(s)] ;
}



nameblkp makename(s)
char *s;
{
register nameblkp p;

if(nhashed > hashthresh)
	rehash();

++nhashed;
hashtab[hashloc(s)] = p = ALLOC(nameblock);
p->nxtnameblock = firstname;
p->namep = copys(s);	/* make a fresh copy of the string s */
/* p->linep = 0; p->done = 0; p->septype = 0; p->modtime = 0; */

firstname = p;
if(mainname==NULL && !haspercent(s) && (*s!='.' || hasslash(s)) )
	mainname = p;

return p;
}



hasslash(s)
register char *s;
{
for( ; *s ; ++s)
	if(*s == '/')
		return YES;
return NO;
}


haspercent(s)
register char *s;
{
for( ; *s ; ++s)
	if(*s == '%')
		return YES;
return NO;
}


hasparen(s)
register char *s;
{
for( ; *s ; ++s)
	if(*s == '(')
		return YES;
return NO;
}


rehash()
{
nameblkp *ohash;
register nameblkp p, *hp, *endohash;
hp = ohash = hashtab;
endohash = hashtab + hashsize;

newhash(2*hashsize);

while( hp<endohash )
	if(p = *hp++)
		hashtab[hashloc(p->namep)] = p;

free( (char *) ohash);
}



newhash(newsize)
int newsize;
{
hashsize = newsize;
hashtab = (nameblkp *) ckalloc(hashsize * sizeof(nameblkp));
hashthresh = (2*hashsize)/3;
}



nameblkp chkname(s)
char *s;
{
nameblkp p;
TIMETYPE k;
/*TEMP NEW */
if(hasparen(s))
	{
	k = lookarch(s);
/*TEMP	fprintf(stderr, "chkname(%s): look=%d\n", s, k); */
	if(k == 0)
		return NULL;
	}
if(p = srchname(s))
	return p;
dirsrch(s);
return srchname(s);
}



char *copys(s)
register char *s;
{
char *malloc();
register char *t, *t0;

if( (t = t0 = malloc( strlen(s)+1 ) ) == NULL)
	fatal("out of memory");
while(*t++ = *s++)
	;
return t0;
}



char *concat(a,b,c)   /* c = concatenation of a and b */
register char *a,*b;
char *c;
{
register char *t;
t = c;

while(*t = *a++) t++;
while(*t++ = *b++);
return c;
}



suffix(a,b,p)  /* is b the suffix of a?  if so, set p = prefix */
register char *a,*b,*p;
{
char *a0,*b0;
a0 = a;
b0 = b;

while(*a++);
while(*b++);

if( (a-a0) < (b-b0) ) return 0;

while(b>b0)
	if(*--a != *--b) return 0;

while(a0<a) *p++ = *a0++;
*p = '\0';

return 1;
}






int *ckalloc(n)
register int n;
{
register int *p;

if( p = (int *) calloc(1,n) )
	return p;

fatal("out of memory");
/* NOTREACHED */
}

/* copy string a into b, substituting for arguments */
char *subst(a,b)
register char *a,*b;
{
static depth	= 0;
register char *s;
char vname[100];
struct varblock *varptr(), *vbp;
char closer;

if(++depth > 100)
	fatal("infinitely recursive macro?");
if(a)  while(*a)
	{
	if(*a!='$' || a[1]=='\0' || *++a=='$')
		/* if a non-macro character copy it.  if $$ or $\0, copy $ */
		*b++ = *a++;
	else	{
		s = vname;
		if( *a=='(' || *a=='{' )
			{
			closer = ( *a=='(' ? ')' : '}');
			++a;
			while(*a == ' ') ++a;
			while(*a!=' ' && *a!=closer && *a!='\0') *s++ = *a++;
			while(*a!=closer && *a!='\0') ++a;
			if(*a == closer) ++a;
			}
		else	*s++ = *a++;

		*s = '\0';
		if( (vbp = varptr(vname)) ->varval != 0)
			{
			b = subst(vbp->varval, b);
			vbp->used = YES;
			}
		}
	}

*b = '\0';
--depth;
return b;
}


setvar(v, s, dyn)
char *v, *s;
int dyn;	/* 1 if dynamic macro */
{
struct varblock *varptr();
register struct varblock *p;

p = varptr(v);
if( ! p->noreset )
	{
	p->varval = s;
	p->noreset = inarglist;
	if(p->used && !dyn)
		fprintf(stderr, "Warning: %s changed after being used\n",v);
	if(p->export)
		{
		/* change string pointed to by environment to new v=s */
		register char *t;
		int lenv;
		lenv = strlen(v);
		*(p->export) = t = (char *) ckalloc(lenv + strlen(s) + 2);
		strcpy(t,v);
		t[lenv] = '=';
		strcpy(t+lenv+1, s);
		}
	else
		p->export = envpp;
	}
}


/* for setting Bradford's *D and *F family of macros whens setting * etc */
set3var(macro, value)
char *macro, *value;
{
register char *s;
char macjunk[8], *lastslash, *dirpart, *filepart;

setvar(macro, value, YES);
if(value == CHNULL)
	dirpart = filepart = CHNULL;
else
	{
	lastslash = CHNULL;
	for(s = value; *s; ++s)
		if(*s == '/')
			lastslash = s;
	if(lastslash)
		{
		dirpart = copys(value);
		filepart = dirpart + (lastslash-value);
		filepart[-1] = '\0';
		}
	else
		{
		dirpart = "";
		filepart = value;
		}
	}
setvar(concat(macro, "D", macjunk), dirpart, YES);
setvar(concat(macro, "F", macjunk), filepart, YES);
}



eqsign(a)   /*look for arguments with equal signs but not colons */
char *a;
{
register char *s, *t;
char c;

while(*a == ' ') ++a;
for(s=a  ;   *s!='\0' && *s!=':'  ; ++s)
	if(*s == '=')
		{
		for(t = a ; *t!='=' && *t!=' ' && *t!='\t' ;  ++t );
		c = *t;
		*t = '\0';

		for(++s; *s==' ' || *s=='\t' ; ++s);
		setvar(a, copys(s), NO);
		*t = c;
		return YES;
		}

return NO;
}


struct varblock *varptr(v)
char *v;
{
register struct varblock *vp;

/* for compatibility, $(TGS) = $^ */
if(equal(v, "TGS") )
	v = "^";
for(vp = firstvar; vp ; vp = vp->nxtvarblock)
	if(equal(v , vp->varname))
		return vp;

vp = ALLOC(varblock);
vp->nxtvarblock = firstvar;
firstvar = vp;
vp->varname = copys(v);
vp->varval = 0;
return vp;
}


dynmacro(line)
char *line;
{
register char *s;
char endc, *endp;
if(!isalpha(line[0]))
	return NO;
for(s=line+1 ; *s && (isalpha(*s) | isdigit(*s)) ; ++s)
	;
endp = s;
while( isspace(*s) )
	++s;
if(s[0]!=':' || s[1]!='=')
	return NO;

endc = *endp;
*endp = '\0';
setvar(line, copys(s+2), YES);
*endp = endc;

return YES;
}


	
fatal1(s, t)
char *s, *t;
{
char buf[100];
sprintf(buf, s, t);
fatal(buf);
}



fatal(s)
char *s;
{
fflush(stdout);
if(s)
	fprintf(stderr, "Make: %s.  Stop.\n", s);
else
	fprintf(stderr, "\nStop.\n");

waitstack(0);
exit(1);
}



/* appends to the chain for $? and $^ */
chainp appendq(head, tail)
chainp head;
char *tail;
{
register chainp p, q;

p = ALLOC(chain);
p->datap = tail;

if(head)
	{
	for(q = head ; q->nextp ; q = q->nextp)
		;
	q->nextp = p;
	return head;
	}
else
	return p;
}





/* builds the value for $? and $^ */
char *mkqlist(p,qbuf)
struct chain *p;
char *qbuf;
{
register char *qbufp, *s;

if(p == NULL)
	return "";

qbufp = qbuf;

for( ; p ; p = p->nextp)
	{
	s = p->datap;
	if(qbufp+strlen(s) > &qbuf[QBUFMAX-3])
		{
		fprintf(stderr, "$? list too long\n");
		break;
		}
	while (*s)
		*qbufp++ = *s++;
	*qbufp++ = ' ';
	}
*--qbufp = '\0';
return qbuf;
}

wildp iswild(name)
char *name;
{
register char *s;
register wildp p;

for(s=name; *s; ++s)
	if(*s == '%')
		{
		p = ALLOC(wild);
		*s = '\0';
		p->left = copys(name);
		*s = '%';
		p->right = copys(s+1);
		p->llen = strlen(p->left);
		p->rlen = strlen(p->right);
		p->totlen = p->llen + p->rlen;
		return p;
		}
return NULL;
}


char *wildmatch(p, name, len)
register wildp p;
char *name;
int len;
{
char *stem;
register char *s;
char c;

if(len < p->totlen ||
   strncmp(name, p->left, p->llen) ||
   strncmp(s = name+len-p->rlen, p->right, p->rlen) )
	return CHNULL;

/*TEMP fprintf(stderr, "wildmatch(%s)=%s%%%s)\n", name,p->left,p->right); */
c = *s;
*s = '\0';
stem = copys(name + p->llen);
*s = c;
return stem;
}



/* substitute stem for any % marks */
char *wildsub(pat, stem)
char *pat, *stem;
{
static char temp[100];
register char *s, *t;

s = temp;
for(; *pat; ++pat)
	if(*pat == '%')
		for(t = stem ; *t; )
			*s++ = *t++;
	else
		*s++ = *pat;
*s = '\0';
return temp;
}

/*	@(#)name.c	1.5	*/
/*
 * UNIX shell
 *
 * Bell Telephone Laboratories
 *
 */

#include	"defs.h"
#include	"sym.h"

extern BOOL	chkid();
extern char	*simple();
static int	namwalk();

struct namnod ps2nod =
{
	(struct namnod *)NIL,
	&acctnod,
	ps2name
};
struct namnod cdpnod = 
{
	(struct namnod *)NIL,
	(struct namnod *)NIL,
	cdpname
};
struct namnod ifsnod =
{
	(struct namnod *)NIL,
	(struct namnod *)NIL,
	ifsname
};
struct namnod ps1nod =
{
	&pathnod,
	&ps2nod,
	ps1name
};
struct namnod histnod =
{
	&cdpnod,
	(struct namnod *)NIL,
	histname
};
struct namnod homenod =
{
	&histnod,
	&ifsnod,
	homename
};
struct namnod pathnod =
{
	(struct namnod *)NIL,
	(struct namnod *)NIL,
	pathname
};
struct namnod mailnod =
{
	&homenod,
	&ps1nod,
	mailname
};
struct namnod acctnod =
{
	(struct namnod *)NIL,
	(struct namnod *)NIL,
	acctname
};	
struct namnod *namep = &mailnod;

/* ========	variable and string handling	======== */

syslook(w, syswds, n)
	register char *w;
	register struct sysnod syswds[];
	int n;
{
	int	low;
	int	high;
	int	mid;
	register int cond;

	if (w == 0 || *w == 0)
		return(0);

	low = 0;
	high = n - 1;

	while (low <= high)
	{
		mid = (low + high) / 2;

		if ((cond = cf(w, syswds[mid].sysnam)) < 0)
			high = mid - 1;
		else if (cond > 0)
			low = mid + 1;
		else
			return(syswds[mid].sysval);
	}
	return(0);
}

setlist(arg, xp)
register struct argnod *arg;
int	xp;
{
	if (flags & exportflg)
		xp |= N_EXPORT;

	while (arg)
	{
		register char *s = mactrim(arg->argval);
		setname(s, xp);
		arg = arg->argnxt;
		if (flags & execpr)
		{
			prs(s);
			if (arg)
				blank();
			else
				newline();
		}
	}
}


setname(argi, xp)	/* does parameter assignments */
char	*argi;
int	xp;
{
	register char *argscan = argi;
	register struct namnod *n;

	while(!ctrlchar(*argscan))
	{
		if (*argscan == '=')
		{
			*argscan = 0;	/* make name a cohesive string */

			n = lookup(argi);
			*argscan++ = '=';
			attrib(n, xp);
			if (xp & N_ENVNAM)
				n->namenv.val = n->namval.val = argscan;
			else
				assign(n, argscan);
			return;
		}
		if ((xp & N_ENVNAM) && *argscan == '(')
		{
			/*
			 * flags==0 when we are called, so flags&ttyflg==0, so
			 * if there's an error we will exit
			 */
			execexp(argi, 0);
			*argscan = 0;
			n = lookup(argi);
			attrib(n, xp);
			return;
		}
		argscan++;
	}
	failed(argi, notid, 0);
}

replace(a, v)
register char	**a;
char	*v;
{
	shfree(*a);
	*a = make(v);
}

dfault(n, v)
struct namnod *n;
char	*v;
{
	if (n->namval.val == 0)
		assign(n, v);
}

assign(n, v)
struct namnod *n;
char	*v;
{
	if (!(n->namflg & N_ENVNAM))
		free_val(&n->namval);
	else
		n->namflg &= ~N_ENVNAM;

	n->namval.val = make(v);
	n->namval.flg = N_VAR;

	if (flags & prompt)
		if (n == &mailnod)
			setmail(n->namval.val);
}

readvar(names)
char	**names;
{
	struct fileblk	fb;
	register struct fileblk *f = &fb;
	register char	c;
	register int	rc = 0;
	struct namnod *n = lookup(*names++);	/* done now to avoid storage mess */
	char	*rel = (char *)relstak();

	push(f);
	initf(dup(0));

	if (lseek(0, 0L, 1) == -1)
		f->fsiz = 1;

	/*
	 * strip leading IFS characters
	 */
	while ((any((c = nextc(0)), ifsnod.namval.val)) && !(eolchar(c)))
			;
	for (;;)
	{
		if ((*names && any(c, ifsnod.namval.val)) || eolchar(c))
		{
			zerostak();
			assign(n, absstak(rel));
			setstak(rel);
			if (*names)
				n = lookup(*names++);
			else
				n = 0;
			if (eolchar(c))
			{
				break;
			}
			else		/* strip imbedded IFS characters */
			{
				while ((any((c = nextc(0)), ifsnod.namval.val)) &&
					!(eolchar(c)))
					;
			}
		}
		else
		{
			pushstak(c);
			c = nextc(0);

			if (eolchar(c))
			{
				char *top = staktop;
			
				while (any(*(--top), ifsnod.namval.val))
					;
				staktop = top + 1;
			}


		}
	}
	while (n)
	{
		assign(n, nullstr);
		if (*names)
			n = lookup(*names++);
		else
			n = 0;
	}

	if (eof)
		rc = 1;
	lseek(0, (long)(f->fnxt - f->fend), 1);
	pop();
	return(rc);
}

assnum(p, i)
char	**p;
int	i;
{
	itos(i);
	replace(p, numbuf);
}

char *
make(v)
char	*v;
{
	register char	*p;

	if (v)
	{
		movstr(v, p = shalloc(length(v)));
		return(p);
	}
	else
		return(0);
}


struct namnod *
lookup(nam)
	register char	*nam;
{
	register struct namnod *nscan = namep;
	register struct namnod **prev;
	int		LR;

	if (!chkid(nam))
		failed(nam, notid, 0);
	while (nscan)
	{
		if ((LR = cf(nam, nscan->namid)) == 0)
			return(nscan);

		else if (LR < 0)
			prev = &(nscan->namlft);
		else
			prev = &(nscan->namrgt);
		nscan = *prev;
	}
	/*
	 * add name node
	 */
	nscan = (struct namnod *)shalloc(sizeof *nscan);
	nscan->namlft = nscan->namrgt = (struct namnod *)NIL;
	nscan->namid = make(nam);
	nscan->namval.val = 0;
	nscan->namval.flg = N_VAR;
	nscan->namenv.val = 0;
	nscan->namenv.flg = N_VAR;
	nscan->namflg = N_DEFAULT;

	return(*prev = nscan);
}

BOOL
chkid(nam)
register char	*nam;
{
	if(!nam) return 0;
	while(!ctrlchar(*nam) && (*nam&QUOTE)==0 && *nam!='(' && *nam!='=')
		nam++;
	return *nam==0;
}

static int (*namfn)();
namscan(fn)
	int	(*fn)();
{
	namfn = fn;
	namwalk(namep);
}

static int
namwalk(np)
register struct namnod *np;
{
	if (np)
	{
		namwalk(np->namlft);
		(*namfn)(np);
		namwalk(np->namrgt);
	}
}

printnam(n)
struct namnod *n;
{
	register char	*s;

	sigchk();

	if (n->namval.flg & N_FUNCTN) {
		prs_buff(strf(n));
		prc_buff(NL);
	}
	else if (s = n->namval.val)
	{
		prs_buff(n->namid);
		prc_buff('=');
		prs_buff(quotedstring(s));
		prc_buff(NL);
	}
}
pushstr(s)
register char *s;
{
	do
		pushstak(*s);
	while(*s++);
	--staktop;
}
char *
staknam(n)
register struct namnod *n;
{
	register char	*p;
	register char	*q;

	if (n->namval.flg & N_FUNCTN)
		pushstr(strf(n));
	else
	{
		pushstr(n->namid);
		pushstak('=');
		pushstr(n->namval.val);
	}
	return(getstak(staktop + 1 - (char *)(stakbot)));
}

static int namec;

exname(n)
	register struct namnod *n;
{
	register int 	flg = n->namflg;

	if (!(flg & N_ENVNAM))
	{
		n->namflg |= N_ENVNAM;

		if (flg & N_EXPORT)
		{
			free_val(&n->namenv);
			n->namenv = n->namval;
		}
		else
		{
			free_val(&n->namval);
			n->namval = n->namenv;
		}
	}

	
	n->namflg &= N_ENVNAM;

	if (n->namval.val)
		namec++;
}

printexp(n)
register struct namnod *n;
{
	if (n->namflg & N_EXPORT)
	{
		prs_buff(export);
		prc_buff(SP);
		prs_buff(n->namid);
		prc_buff(NL);
	}
}

setup_env()
{
	register char **e = environ;

	while (*e)
		setname(*e++, N_ENVNAM);
}

killenvfn(n)
struct namnod *n;
{
	if ((n->namflg & N_ENVNAM) && (n->namval.flg & N_FUNCTN))
	{
		freefunc(n->namval.val);
		n->namval.val = N_VAR;
		n->namval.flg = 0;
	}
}

prot_env()
{
	register struct namnod *n;

	namscan(killenvfn);
	if (ifsnod.namval.val)
		shfree(ifsnod.namval.val);
	ifsnod.namval.val = 0;
	dfault(&ifsnod, sptbnl);
}

static char **argnam;

pushnam(n)
struct namnod *n;
{
	if (n->namval.val || (n->namval.flg & N_FUNCTN))
		*argnam++ = staknam(n);
}

char **
setenv()
{
	register char	**er;

	namec = 0;
	namscan(exname);

	argnam = er = (char **)getstak(namec * BYTESPERWORD + BYTESPERWORD);
	namscan(pushnam);
	*argnam++ = 0;
	return(er);
}

struct namnod *
findnam(nam)
	register char	*nam;
{
	register struct namnod *nscan = namep;
	int		LR;

	if (!chkid(nam))
		return(0);
	while (nscan)
	{
		if ((LR = cf(nam, nscan->namid)) == 0)
			return(nscan);
		else if (LR < 0)
			nscan = nscan->namlft;
		else
			nscan = nscan->namrgt;
	}
	return(0); 
}


special(s)
	char *s;
{
	return !cf(s, "builtin") || !cf(s, ifsname) || !cf(s, pathname) || !cf(s, ps1name) || !cf(s, ps2name);
}

unset_name(name)
	register char 	*name;
{
	register struct namnod	*n;

	if (n = findnam(name))
	{
		if (special(name))
			failed(name, badunset, 0);

		if (!(n->namflg & N_ENVNAM))
		{
			if (n->namval.flg & N_FUNCTN)
				freefunc(n->namval.val);
			else
				shfree(n->namval.val);
		}

		if (n->namenv.flg & N_FUNCTN)
			freefunc(n->namenv.val);
		else
			shfree(n->namenv.val);

		n->namval.val = N_VAR;
		n->namval.flg = 0;
		n->namenv = n->namval;
		n->namflg = N_DEFAULT;

		if (flags & prompt)
		{
			if (n == &mailnod)
				setmail(0);
		}
	}
}

free_val(nx)
	struct namx	*nx;
{
		if (nx->flg & N_FUNCTN)
			freefunc(nx->val);
		else
			shfree(nx->val);
}

#include	"mk.h"

static Rule *lr, *lmr;
static rcmp();
static int nrules = 0;

addrule(head, tail, body, ahead, attr, hline, override, prog)
	Word *tail, *ahead;
	char *head, *body, *prog;
{
	register Rule *r;
	Rule *rr;
	Symtab *sym;
	int reuse;

	if(sym = symlook(head, S_TARGET, (char *)0)){
		for(r = (Rule *)sym->value; r; r = r->chain)
			if(rcmp(r, head, tail) == 0) break;
		if(r && !override)
			return;
	} else
		r = 0;
	reuse = r != 0;
	if(r == 0)
		r = (Rule *)Malloc(sizeof(Rule));
	r->target = head;
	r->tail = tail;
	r->recipe = body;
	r->line = hline;
	r->file = infile;
	r->attr = attr;
	r->alltargets = ahead;
	r->prog = prog;
	r->rule = nrules++;
	if(!reuse){
		rr = (Rule *)symlook(head, S_TARGET, (char *)r)->value;
		if(rr != r){
			r->chain = rr->chain;
			rr->chain = r;
		} else
			r->chain = 0;
	}
	if(strchr(head, '%') || strchr(head, '&') || (attr&REGEXP))
		goto meta;
	if(reuse)
		return;
	r->next = 0;
	r->pat = 0;
	if(rules == 0)
		rules = lr = r;
	else {
		lr->next = r;
		lr = r;
	}
	return;
meta:
	r->attr |= META;
	if(reuse)
		return;
	r->next = 0;
	if(r->attr&REGEXP){
		patrule = r;
		r->pat = regcomp(head);
	}
	if(metarules == 0)
		metarules = lmr = r;
	else {
		lmr->next = r;
		lmr = r;
	}
}

dumpr(s, r)
	char *s;
	register Rule *r;
{
	Fprint(1, "%s: start=%ld\n", s, r);
	for(; r; r = r->next){
		Fprint(1, "\tRule %ld: %s[%d] attr=%x next=%ld chain=%ld alltarget='%s'",
			r, r->file, r->line, r->attr, r->next, r->chain, wtos(r->alltargets));
		if(r->prog)
			Fprint(1, " prog='%s'", r->prog);
		Fprint(1, "\n\ttarget=%s: %s\n", r->target, wtos(r->tail));
		Fprint(1, "\trecipe@%ld='%s'\n", r->recipe, r->recipe);
	}
}

frule(w)
	Word *w;
{
	extern Word *target1;
	register Word *ww;
	register char *s;

#define	ADD(s)	{if(target1==0)target1=ww=newword(s);else ww=ww->next=newword(s);}

	for(ww = w; ww; ww = ww->next)
		if(strchr(w->s, '%') || strchr(w->s, '&'))
			return;	/* no metarule targets */
	while(w){
		if(s = strchr(w->s, '+')){
			*s++ = 0;
			if(*w->s)
				ADD(w->s);
			if(*s)
				ADD(s);
			s[-1] = '+';
		} else
			ADD(w->s);
		w = w->next;
	}
}

static
rcmp(r, target, tail)
	register Rule *r;
	char *target;
	register Word *tail;
{
	register Word *w;

	if(strcmp(r->target, target))
		return(1);
	for(w = r->tail; w && tail; w = w->next, tail = tail->next)
		if(strcmp(w->s, tail->s))
			return(1);
	return(w || tail);
}

char *
rulecnt()
{
	register char *s;

	s = Malloc(nrules);
	memset(s, 0, nrules);
	return(s);
}

ismeta(s)
	register char *s;
{
	for(; *s; s++)
		switch(*s)
		{
		case '\\':
			if(s[1])
				s++;
			break;
		case '[':
		case '*':
		case '?':
			return(1);
			break;
		}
	return(0);
}

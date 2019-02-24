#include	"mk.h"

static Node *newnode(), *applyrules();
static cyclechk(), vacuous(), ambiguous(), attribute();

Node *
graph(target)
	char *target;
{
	Node *node;
	char *cnt;

	node = applyrules(target, cnt = rulecnt());
	free(cnt);
	cyclechk(node);
	node->flags |= PROBABLE;	/* make sure it doesn't get deleted */
	(void)vacuous(node);
	(void)ambiguous(node);
	(void)attribute(node);
	return(node);
}

static Node *
applyrules(target, cnt)
	char *target, *cnt;
{
	Symtab *sym;
	Node *node;
	Rule *r;
	Arc head, *a = &head;
	Word *w;
	char stem[NAMEBLOCK], buf[NAMEBLOCK];
	regsubexp rmatch[NREGEXP];

/*	print("appplyrules(%ld='%s')\n", target, target);/**/
	if(sym = symlook(target, S_NODE, (char *)0)){
		node = (Node *)(sym->value);
		return(node);
	}
	target = strdup(target);
	node = newnode(target);
	head.n = 0;
	head.next = 0;
	sym = symlook(target, S_TARGET, (char *)0);
	for(r = sym? (Rule *)(sym->value):0; r; r = r->chain){
		if(r->attr&META) continue;
		if(strcmp(target, r->target)) continue;
		if(cnt[r->rule] >= nreps) continue;
		cnt[r->rule]++;
		node->flags |= PROBABLE;
		if(r->attr&VIR)
			node->flags |= VIRTUAL;
		if(r->attr&NOREC)
			node->flags |= NORECIPE;
		if(r->attr&DEL)
			node->flags |= DELETE;
		if(r->tail == 0)
			a = a->next = newarc((Node *)0, r, "", rmatch);
		else
			for(w = r->tail; w; w = w->next){
				a = a->next = newarc(applyrules(w->s, cnt), r, "", rmatch);
		}
		cnt[r->rule]--;
		head.n = node;
	}
	for(r = metarules; r; r = r->next){
		if(r->attr&REGEXP){
			stem[0] = 0;
			patrule = r;
			if(regexec(r->pat, node->name, rmatch, NREGEXP) == 0)
				continue;
		} else {
			if(!match(node->name, r->target, stem)) continue;
		}
		if(cnt[r->rule] >= nreps) continue;
		cnt[r->rule]++;
		if(r->attr&VIR)
			node->flags |= VIRTUAL;
		if(r->attr&NOREC)
			node->flags |= NORECIPE;
		if(r->attr&DEL)
			node->flags |= DELETE;
		if(r->tail == 0)
			a = a->next = newarc((Node *)0, r, strdup(stem), rmatch);
		else
			for(w = r->tail; w; w = w->next){
				if(r->attr&REGEXP)
					regsub(w->s, buf, rmatch, NREGEXP);
				else
					subst(stem, w->s, buf);
				a = a->next = newarc(applyrules(buf, cnt), r, strdup(stem), rmatch);
			}
		cnt[r->rule]--;
	}
	a->next = node->prereqs;
	node->prereqs = head.next;
	return(node);
}

static
togo(node)
	register Node *node;
{
	register Arc *la, *a;

	/* delete them now */
	for(a = node->prereqs; a; la = a, a = a->next)
		if(a->flag&TOGO){
			if(a == node->prereqs)
				node->prereqs = a->next;
			else
				la->next = a->next, a = la;
		}
}

static
vacuous(node)
	register Node *node;
{
	register Arc *la, *a;
	int vac = !(node->flags&PROBABLE);

	if(node->flags&READY)
		return(node->flags&VACUOUS);
	node->flags |= READY;
	for(a = node->prereqs; a; a = a->next)
		if(a->n && vacuous(a->n) && (a->r->attr&META))
			a->flag |= TOGO;
		else
			vac = 0;
	/* if a rule generated arcs that DON'T go; no others from that rule go */
	for(a = node->prereqs; a; a = a->next)
		if((a->flag&TOGO) == 0)
			for(la = node->prereqs; la; la = la->next)
				if((la->flag&TOGO) && (la->r == a->r)){
					la->flag &= ~TOGO;
				}
	togo(node);
	if(vac)
		node->flags |= VACUOUS;
	return(vac);
}

static Node *
newnode(name)
	char *name;
{
	register Node *node;

	node = (Node *)Malloc(sizeof(Node));
	symlook(name, S_NODE, (char *)node);
	node->name = name;
	node->time = timeof(name, 0);
	node->prereqs = 0;
	node->flags = node->time? PROBABLE : 0;
	node->next = 0;
	return(node);
}

dumpn(s, n)
	char *s;
	register Node *n;
{
	char buf[1024];
	register Arc *a;

	sprint(buf, "%s   ", (*s == ' ')? s:"");
	Fprint(1, "%s%s@%ld: time=%ld flags=0x%x next=%ld\n",
		s, n->name, n, n->time, n->flags, n->next);
	for(a = n->prereqs; a; a = a->next)
		dumpa(buf, a);
}

static
trace(s, a)
	char *s;
	register Arc *a;
{
	Fprint(2, "\t%s", s);
	while(a){
		Fprint(2, " <-(%s:%d)- %s", a->r->file, a->r->line,
			a->n? a->n->name:"");
		if(a->n){
			for(a = a->n->prereqs; a; a = a->next)
				if(*a->r->recipe) break;
		} else
			a = 0;
	}
	Fputc(2, '\n');
}

static
cyclechk(n)
	register Node *n;
{
	register Arc *a;

	if((n->flags&CYCLE) && n->prereqs){
		Fprint(2, "mk: cycle in graph detected at target %s\n", n->name);
		Exit();
	}
	n->flags |= CYCLE;
	for(a = n->prereqs; a; a = a->next)
		if(a->n)
			cyclechk(a->n);
	n->flags &= ~CYCLE;
}

static
ambiguous(n)
	register Node *n;
{
	register Arc *a;
	register Rule *r = 0;
	Arc *la;
	int bad = 0;

	for(a = n->prereqs; a; a = a->next){
		if(a->n)
			ambiguous(a->n);
		if(*a->r->recipe == 0) continue;
		if(r == 0)
			r = a->r, la = a;
		else{
			if(r->recipe != a->r->recipe){
				if((r->attr&META) && !(a->r->attr&META)){
					la->flag |= TOGO;
					r = a->r, la = a;
				} else if(!(r->attr&META) && (a->r->attr&META)){
					a->flag |= TOGO;
					continue;
				}
			}
			if(r->recipe != a->r->recipe){
				if(bad == 0){
					Fprint(2, "mk: ambiguous recipes for %s:\n", n->name);
					bad = 1;
					trace(n->name, la);
				}
				trace(n->name, a);
			}
		}
	}
	if(bad)
		Exit();
	togo(n);
}

static
attribute(n)
	register Node *n;
{
	register Arc *a;

	for(a = n->prereqs; a; a = a->next){
		if(a->r->attr&VIR)
			n->flags |= VIRTUAL;
		if(a->r->attr&NOREC)
			n->flags |= NORECIPE;
		if(a->r->attr&DEL)
			n->flags |= DELETE;
		if(a->n)
			attribute(a->n);
	}
	if(n->flags&VIRTUAL)
		n->time = 0;
}

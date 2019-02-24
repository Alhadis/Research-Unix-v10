#include <stdio.h>
#include "efix.h"
#define Hlen 4091
#define HGULP 1020
#define SGULP 8092
 extern char *strcpy();

 char *
alloc(x)
 unsigned x;
{
	extern char *malloc();
	char *s;
	s = malloc(x);
	if (!s) {
		fprintf(stderr, "%s: malloc failure!\n", progname);
		exit(1);
		}
	return s;
	}

 struct HashHeader {
	struct Hash *next, *prev;
	} maintab[Hlen], memtab[Hlen];

 struct Hash {
	struct HashHeader hh;
	char *name;
	char *type;
	struct Hash *dup;
	int lev;
	} *halloc_next, *halloc_last, *lasthash;

/* type values:
 *	static, auto, extern, register: i
 *	char, double, float, int, long, short, unsigned, void: i
 *	enum: e
 *	struct, union: a
 *	typedef: t
 *	struct tag, union tag, typedefed name: T{type}
 */

 char *salloc_next, *salloc_last;
 struct SALLOC {
	struct SALLOC *next;
	char *last;
	char block[SGULP];
	} *Salloc_block;
 struct HALLOC {
	struct HALLOC *next, *prev;
	struct Hash block[HGULP];
	} *Halloc_block;
	
 struct Context {
	struct Context *next, *prev;
	struct SALLOC *Sblock;
	char *snext, *slast;
	struct HALLOC *Hblock;
	struct Hash *hnext;
	} *context;
 int levnow;

new_context()
{
	struct Context *C;
	++levnow;
	if (!context || !(C = context->next)) {
		C = (struct Context *)alloc(sizeof(struct Context));
		C->next = 0;
		if (context)
			context->next = C;
		C->prev = context;
		}
	context = C;
	C->Sblock = Salloc_block;
	C->Hblock = Halloc_block;
	C->snext = salloc_next;
	C->slast = salloc_last;
	C->hnext = halloc_next;
	}

end_context()
{
	struct Hash *h, *h0, *h00, *h1;
	struct HALLOC *H;
	struct Context *C;

	--levnow;
	C = context;
	context = C->prev;
	if (!C || levnow < 0)
		scream("too many end_context's");
	Salloc_block = C->Sblock;
	salloc_next = C->snext;
	salloc_last = C->slast;
	h00 = C->hnext;
	if (H = Halloc_block) {
		h0 = H->block;
		h = halloc_next;
		while(h != h00) {
			if (--h == h0) {
				H = H->prev;
				h0 = H->block;
				h = h0 + HGULP;
				continue;
				}
			h1 = h->hh.next;
			h1->hh.prev = h->hh.prev;
			h->hh.prev->hh.next = h1;
			}
		}
	Halloc_block = C->Hblock;
	halloc_next = h00;
	}

 struct Hash *
hash(s, ht)
 register char *s;
 struct HashHeader *ht;
{
	register x = 0, c;
	register struct Hash *h, *h0;
	char *s0 = s;

	while(c = *s++) {
		if (x & 1)
			x = 0x40000000 | (x>>1) ^ c;
		else
			x = (x>>1) ^ c;
		}
	x %= Hlen;
	lasthash = h0 = (struct Hash *) (ht + x);
	s = s0;
	for(h = h0->hh.next; h != h0; h = h->hh.next)
		if (!strcmp(s,h->name))
			return h;
	return 0;
	}

 char *
salloc(s)
 char *s;
{
	unsigned x;
	char *t;
	struct SALLOC *S;

	x = strlen(s) + 1;
	t = salloc_next;
	if ((salloc_next += x) > salloc_last) {
		if (Salloc_block && (S = Salloc_block->next)) {
			if (S->last - S->block < x)
				scream("Huge string in salloc");
			}
		else {
			S = (struct SALLOC *)alloc(x + sizeof(struct SALLOC));
			if (Salloc_block)
				Salloc_block->next = S;
			S->last = S->block + x + SGULP;
			S->next = 0;
			}
		t = S->block;
		salloc_next = t + x;
		salloc_last = S->last;
		Salloc_block = S;
		}
	return strcpy(t, s);
	}

 struct Hash *
newhash(s,t)
 char *s, *t;
{
	struct Hash *h, *h1;
	struct HALLOC *H;

	if ((h = halloc_next++) >= halloc_last) {
		if (!Halloc_block || !(H = Halloc_block->next)) {
			H = (struct HALLOC *)
				alloc(sizeof(struct HALLOC));
			H->next = 0;
			H->prev = Halloc_block;
			}
		Halloc_block = H;
		h = H->block;
		halloc_next = h + 1;
		halloc_last = h + HGULP;
		}
	h->name = s;
	h->type = t;
	h->lev = levnow;
	h->hh.next = h1 = lasthash->hh.next;
	h1->hh.prev = h;
	h->hh.prev = lasthash;
	lasthash->hh.next = h;
	return h;
	}

 struct Tok Toks[3];
 char isdcl[128];

setup()
{
	int i;
	struct HashHeader *h;
	static char *keywds[] = {
		"auto", "i",
		"char", "i",
		"double", "i",
		"enum", "e",
		"extern", "i",
		"float", "i",
		"int", "i",
		"long", "i",
		"register", "i",
		"short", "i",
		"static", "i",
		"struct", "a",
		"typedef", "t",
		"union", "a",
		"unsigned", "i",
		"void", "i",
		0, 0};
	char **k, *s;

	Toks[0].next = Toks + 1;	Toks[0].prev = Toks + 2;
	Toks[1].next = Toks + 2;	Toks[1].prev = Toks;
	Toks[2].next = Toks;		Toks[2].prev = Toks + 1;
	for(i = 0; i < 3; i++) Toks[i].inlevel = 1;
	T = Toks;
	for(h = maintab, i = 0; i < Hlen; ++i, ++h)
		h->next = h->prev = (struct Hash *)h;
	for(h = memtab, i = 0; i < Hlen; ++i, ++h)
		h->next = h->prev = (struct Hash *)h;

	for(k = keywds; *k; k += 2) {
		hash(*k, maintab);
		newhash(k[0], k[1]);
		}
	for(s = "aeitT"; *s; ++s)
		isdcl[*s] = 1;
	}

process()
{
	int t;
	struct Tok *T1;

	setup();

	while((t = token(0)) != FEND) {
		if (t == SYMBOL)
			dcl();
		else if (t != ';') {
			squawk("expected declaration");
			do token(2);
				while(t != ';');
			}
		}
 done:
	for(T1 = T;;) {
		T1 = T1->next;
		if (!T1->inlevel)
			printf("%s%s", T1->white, T1->tok);
		if (T1 == T)
			break;
		}
	}

 char *
memname()
{
	int pars = 0;
	char *s;
	int t;

	for(t = T->type; ; t = token(2)) {
		switch(t) {
			case '(': ++pars;
			case '*': continue;
			case ':':
				if (pars)
					scream("unexpected :");
				getexp();
				return 0;
			default: break;
			}
		break;
		}
	if (t != SYMBOL)
		scream("expected * ( : or symbol") /*)*/;
	s = salloc(T->tok);
	for(;;) {
		switch(token(2)) {
			case '(':
			case '[':
				++pars;
				continue;
			case ')':
			case ']':
				--pars;
				continue;
			case ':':
				if (!pars) {
					getexp();
					break;
					}
			case ';':
				break;
			case ',':
				if (pars <= 0)
					break;
			default:
				continue;
			}
		break;
		}
	if (pars)
		squawk("bad paren count in memname");
	return s;
	}

 char *
sdcl(tag)
char *tag;
{
	int t, typed;
	char buf[256], *m, *s;
	struct Hash *h, *h1;
	static int anon;
	char *b, *be = buf + sizeof(buf);

	b = adjoin(buf,be,tag);
	*b++ = *T->tok;	/* 's' or 'u' */
	t = token(2);
	if (t == SYMBOL) {
		adjoin(b,be,T->tok);
		t = token(2);
		}
	else
		sprintf(b, "#%d", ++anon);
	s = salloc(buf);
	if (t != '{') /*}*/ {
		return s;
		}

	/* struct foo {...} -- process the ... */

	for(t = token(2); /*{*/ t != '}'; t = token(2)) {
		for(typed = 0; t == SYMBOL; t = token(2), typed = 1) {
			h = hash(T->tok, maintab);
			if (h) switch(*h->type) {
				case 'a':
				case 'e':
					t = token(2);
					if (t != SYMBOL)
						scream("expected tag");
				case 'i':
				case 'T':
					continue;
				case 't':
					scream("typedef within struct!");
				}
			break;
			}
		if (!typed)
			scream("no type!");
		if (t != ';')
		    for(; ; token(2)) {
			if (m = memname()) {
				h = hash(m, memtab);
				h1 = newhash(m, s);
				h1->dup = h;
				}
			switch(T->type) {
				case ',': continue;
				case ';': break;
				default:
					scream("expected ; or ,");
				}
			break;
			}
		}
	token(2);
	return s;
	}

dcl()
{
	struct Hash *h;
	char buf[256], *sym;
	int pars = 0, t, typed = 0;
	char *b = buf, *buf1 = buf, *be = buf + sizeof(buf), *tag = "";

	for(t = SYMBOL; t == SYMBOL; t = token(2)) {
		h = hash(T->tok, maintab);
		if (h) switch(*h->type) {
			case 'T':
				tag = h->type + 1;
				typed = 1;
				continue;
			case 'a':	/* aggregate -- struct or union */
				tag = sdcl(tag);
				typed = 1;
				t = T->type;
				goto havetag;
			case 'e':
				do t = token(2);
					while(t != ';');
				return;
			case 'i':
				typed = 1;
				continue;
			case 't':
				typed = 0;
				*buf = 'T';
				b = buf1 = buf + 1;
				continue;
			}
		break;
		}
 havetag:
	for(; t != ';'; t = token(2)) {
		if (t != SYMBOL)
		    switch(t) {
			case '(':
				++pars;
			case '*':
				*b++ = t;
				continue;
			default:
				scream("expected symbol, ( or *" /*)*/);
			}
		sym = salloc(T->tok);
		b = adjoin(b,be,tag);
		while(pars > 0) {
			switch(t = token(2)) {
				case '(':
				case '[':
					++pars;
					break;
				case ')':
				case ']':
					--pars;
					break;
				default:
					scream("expected ( [ ] or )");
				}
			*b++ = t;
			}
		if (pars < 0)
			scream("negative paren count!");
		t = token(2);
		if (t == '(' /*)*/) {
			adjoin(b,be,"()");
			if ((h = hash(sym, maintab))) {
				if (h->lev == levnow && strcmp(buf,h->type)) {
 badtype:
					squawk("wrong type!");
					fprintf(stderr, "expected %s, got %s\n",
						h->type, buf);
					exit(1);
					}
				}
			else newhash(sym,salloc(buf));

			/* start of a function body? */

			switch(t = token(2)) {
				case SYMBOL:
					function();
					return;
				case /*(*/ ')':
					t = token(2);
					if (t == '{'/*}*/) {
						function();
						return;
						}
					break;
				default:
					scream(/*(*/"expected symbol or )");
				}
			}
		else {
			if (!typed)
				scream("expected type or function prologue");
			if (t == '['/*]*/)
				for(; ; t = token(2)) {
					switch(t) {
						case '[':
							if (!pars)
								adjoin(b,be,"[]");
						case '(':
							++pars;
							continue;
						case ']':
						case ')':
							--pars;
							continue;
						default:
							if (pars)
								continue;
						}
					break;
					}
			if ((h = hash(sym, maintab))) {
				if (h->lev == levnow && strcmp(buf,h->type))
					goto badtype;
				}
			else newhash(sym,salloc(buf));
			}
		switch(t) {
			case ',':
				break;
			case ';':
				return;
			case '=':
				t = getexp();
				if (t == ';')
					return;
				break;
			default:
				scream("expected , ; or =");
			}
		b = buf1;	
		}
	}

doarrow()
{
	struct Tok *T0, *T1;
	int t;
	struct Hash *h, *h0;
	char *s, *what;

	T0 = T->prev;
	if (T0->type != SYMBOL || T0->inlevel)
		return;
	untok = t = token(2);
	if (t != SYMBOL)
		return;
	T1 = T;
	if (!(h = hash(T1->tok, memtab))
		|| !(h0 = hash(T0->tok, maintab)))
			return;
	if (*h0->type == '*' && !strcmp(h0->type+1, h->type))
		return;
	T0->inlevel = 1;
	s = h->type;
	what = *s++ == 's' ? "struct" : "union";
	if (!h->dup)
		printf("%s((%s %s *)%s)", T0->white, what, s, T0->tok);
	else {
		printf("%s((%s %s /*"/*))*/, T0->white, what, s);
		if (verbose) {
			squawk("ambiguous tag");
			fprintf(stderr, "%s %s", what, s);
			}
		while(h = h->dup) {
			s = h->type;
			what = *s++ == 's' ? "struct" : "union";
			printf("|| %s %s", what, s);
			if (verbose)
				fprintf(stderr, " || %s %s", what, s);
			}
		printf(/*((*/" */ *)%s)", T0->tok);
		}
	}

getexp()
{
	int brcnt = 0, t;

	for(;;) switch(t = token(2)) {
		case ';':
			if (brcnt)
				scream("unclosed braces!");
			/* no break */
		case ',':
			if (!brcnt)
				return t;
			break;
		case ARROW:
			doarrow();
			break;
		case '(':
		case '{':
			++brcnt;
			break;
		case '}':
		case ')':
			--brcnt;
			break;
		}
	}

function()
{
	int paren, t;
	struct Hash *h;
	/*debug*/static int zork; extern int lineno;

	if (levnow)
		scream("inner function def!");
	new_context();
	if (T->type == SYMBOL) {
		while((t = token(2)) != /*(*/ ')') {
			if (t != ',')
				scream(/*(*/"expected ) or ,");
			if (token(2) != SYMBOL)
				scream("expected dummy arg");
			}
		while((t = token(2)) == SYMBOL)
			dcl();
		if (t != '{'/*}*/)
			scream(/*{*/"expected }");
		}
	new_context();
	paren = 0;
 top:
	for(;;) {
		switch(t = token(2)) {
			case ';':
				continue;
			case '{':
				new_context();
				continue;
			case '}':
				end_context();
				if (levnow == 1) {
					end_context();
					return;
					}
				continue;
			case '(': /*)*/
				paren++;
				break;
			case SYMBOL:
				if ((h = hash(T->tok,maintab))
					&& isdcl[*h->type]) {
						dcl();
						continue;
						}
			}
		/* fall thru for non declaration stmt */
		for(;;) {
			/*debug*/ if (lineno == zork)
			/*debug*/	printf("");
			switch(t = token(2)) {
				case ARROW:
					doarrow();
					continue;
				case ';':
					if (!paren)
						goto top;
					continue;
				case '(':
					++paren;
					continue;
				case ')':
					if (--paren < 0)
						scream("bad paren count");
					continue;
				case '{':
					if (paren)
						scream("unexpected {");
					new_context();
					goto top;
				case '}':
					scream("unexpected }");
				}
			}
		}
	}

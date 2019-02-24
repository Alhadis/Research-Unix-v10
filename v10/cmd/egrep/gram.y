/*
 * egrep -- print lines containing (or not containing) a regular expression
 *
 *	status returns:
 *		0 - ok, and some matches
 *		1 - ok, but no matches
 *		2 - some error; matches irrelevant
 */
%token CHAR DOT CCL NCCL OR CAT STAR PLUS QUEST
%left OR
%left CHAR DOT CCL NCCL '('
%left CAT
%left STAR PLUS QUEST

%{
#include "hdr.h"
%}

%%
s:	t
		{ unary(FINAL, $1);
		  line--;
		}
	;
t:	b r
		{ $$ = node(CAT, $1, $2); }
	| OR b r OR
		{ $$ = node(CAT, $2, $3); }
	| OR b r
		{ $$ = node(CAT, $2, $3); }
	| b r OR
		{ $$ = node(CAT, $1, $2); }
	;
b:
		{ $$ = enter(DOT);
		   $$ = unary(STAR, $$); }
	;
r:	CHAR
		{ $$ = iflag?node(OR, enter(tolower($1)), enter(toupper($1))):enter($1); }
	| DOT
		{ $$ = enter(DOT); }
	| CCL
		{ $$ = cclenter(CCL); }
	| NCCL
		{ $$ = cclenter(NCCL); }
	;

r:	r OR r
		{ $$ = node(OR, $1, $3); }
	| r r %prec CAT
		{ $$ = node(CAT, $1, $2); }
	| r STAR
		{ $$ = unary(STAR, $1); }
	| r PLUS
		{ $$ = unary(PLUS, $1); }
	| r QUEST
		{ $$ = unary(QUEST, $1); }
	| '(' r ')'
		{ $$ = $2; }
	| error 
	;

%%
yyerror(s) {
	fprint(2, "%s: %s\n", progname, s);
	exit(2);
}

yylex() {
	extern int yylval;
	int cclcnt, x;
	register char c, d;
	switch(c = nextch()) {
		case '^': c = LEFT;
			goto defchar;
		case '$': c = RIGHT;
			goto defchar;
		case '|': return (OR);
		case '*': return (STAR);
		case '+': return (PLUS);
		case '?': return (QUEST);
		case '(': return (c);
		case ')': return (c);
		case '.': return (DOT);
		case '\0': return (0);
		case RIGHT: return (OR);
		case '[': 
			x = CCL;
			cclcnt = 0;
			count = nxtchar++;
			if ((c = nextch()) == '^') {
				x = NCCL;
				c = nextch();
			}
			do {
				if (c == '\0') synerror();
				if (c == '-' && cclcnt > 0 && chars[nxtchar-1] != 0) {
					if ((d = nextch()) != 0) {
						c = chars[nxtchar-1];
						while (c < d) {
							if (iflag && isalpha(c)) {
								if (nxtchar >= MAXLIN-1) overflo();
									chars[nxtchar++] = isupper(++c)?tolower(c):toupper(c);
									chars[nxtchar++] = c;
									cclcnt += 2;
							}
							else {
								if (nxtchar >= MAXLIN) overflo();
								chars[nxtchar++] = ++c;
								cclcnt++;
							}
						}
						continue;
					}
				}
				if (iflag&&isalpha(c)) {
					if (nxtchar >= MAXLIN-1) overflo();
					chars[nxtchar++] = isupper(c)?tolower(c):toupper(c);
					chars[nxtchar++] = c;
					cclcnt += 2;
				}
				else {
					if (nxtchar >= MAXLIN) overflo();
					chars[nxtchar++] = c;
					cclcnt++;
				}
			} while ((c = nextch()) != ']');
			chars[count] = cclcnt;
			return (x);
		case '\\':
			if ((c = nextch()) == '\0') synerror();
			else if (c == '\n') c = nextch();
		defchar:
		default: yylval = c; return (CHAR);
	}
}

nextch() {
	register c;
	if (fflag) {
		if ((c = Fgetc(expfile)) < 0)
			c = 0;
	}
	else c = *input++;
#ifdef DOSTATS
	*statsexpr++ = c;
#endif
	return(c);
}

synerror() {
	fprint(2, "%s: syntax error\n", progname);
	exit(2);
}

enter(x) int x; {
	if(line >= MAXLIN) overflo();
	name[line] = x;
	left[line] = 0;
	right[line] = 0;
	return(line++);
}

cclenter(x) int x; {
	register linno;
	linno = enter(x);
	right[linno] = count;
	return (linno);
}

node(x, l, r) {
	if(line >= MAXLIN) overflo();
	name[line] = x;
	left[line] = l;
	right[line] = r;
	parent[l] = line;
	parent[r] = line;
	return(line++);
}

unary(x, d) {
	if(line >= MAXLIN) overflo();
	name[line] = x;
	left[line] = d;
	right[line] = 0;
	parent[d] = line;
	return(line++);
}
overflo() {
	fprint(2, "%s: regular expression too long\n", progname);
	exit(2);
}

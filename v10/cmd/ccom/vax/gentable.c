#include "stdio.h"

char line[10240];
struct arg {
	char *val;
	int len;
} args[1000];
int narg;
struct arg words[2000];
extern char *wordskip(), *matchskip();

main()
{	char *p, *q;
	for(;;) {
		(void) fgets(line, sizeof(line), stdin);
		if(feof(stdin))
			exit(0);
		narg = 0;
		p = wordskip(line, '\t');
		q = matchskip(p);
		if(!findargs(p))
			continue;
		if(!uniqargs())
			continue;
		if(!matchargs(q))
			continue;
		if(!checktemp(q))
			continue;
		fixtree();
		prleft(q);
	}
}

fixtree()
{	int i, j;
	for(i = 0; i < narg; i++)
	for(j = 0; j < args[i].len; j++)
		args[i].val[j] = i;
}


checktemp(q)
char *q;	/* [^\].(fp) is an unusable temporary, and ,$n */
{	char *p;
	for(p = q; *p != '\n'; p++)
		if(*p == 'f' && *(p-1) == '(' && *(p-2) > ' ')
			return(0);
	for(p = q; *p != '\n'; p++)
		if(*p == ',' && *(p+1) == '$' && *(p+2) > ' ')
			return(0);
	for(p = q; *p != '\n'; p++)
		if(*p == '$' && *(p+1) == '-')
			return(0);
	return(1);
}

prleft(q)
char *q;
{	char *p, *s;
	for(p = line; *p != '\t'; p++)
		;
	*p = ' ';
	for(p = line; *p != '\n'; ) {
		if(*p >= ' ') {
			putchar(*p++);
			continue;
		}
		for(s = p; *s == *p; s++)
			;
		if(*p < 10)
			printf("\\%d", *p);
		else
			printf("\\%c", *p - 9 + 'a');
		p = s;
	}
	putchar('\n');
}
int alens[33];	/* if an arg > 32, forget it */
matchargs(s)
char *s;
{	char *p;
	int i, j, ok;
	for(i = 0; i < 33; i++)
		alens[i] = 0;
	for(i = 0; i < narg; i++)
		if(args[i].len <= 32)
			alens[args[i].len]++;
		else
			return(0);
	for(j = 32; j >= 1; j--) {
		if(alens[j] == 0)
			continue;
		for(i = 0; i < narg; i++) {
			if(args[i].len != j)
				continue;
			ok = 0;
			for(p = s; *p != '\n'; p++)
				if(*p == args[i].val[0])
					ok += matchit(p, i);
			if(ok == 0)
				return(0);
		}
	}
	return(1);
}

matchit(p, i)
char *p;
{	int j, c;
	for(j = 0; j < args[i].len; j++)
		if(p[j] != args[i].val[j])
			return(0);
	if((c = p[j]) <= '9' && c >= '0')
		return(0);
	if(c <= 'z' && c >= 'a')
		return(0);
	if(c <= 'Z' && c >= 'A')
		return(0);
	for(j = 0; j < args[i].len; j++)
		p[j] = i;	/* assumes narg < 32!!!!*/
	return(1);
}

findargs(p)
char *p;
{	int cnt, i, j;
	struct arg *a;
	if(*p != '(') {
		fprintf(stderr, "findargs fmt %s", line);
		return(0);
	}
	cnt = setwords(p);
	for(i = 0; i <= cnt; i++)
		stripcrud(words + i);
	for(i = 0; i <= cnt; i++) {
		a = &words[i];
		if(a->len == 0)
			continue;
		switch(numargs(a->val)) {
		case 0:
			continue;
		case 2:
			args[narg++] = words[++i];
		case 1:
			args[narg++] = words[++i];
			continue;
		default:
			fprintf(stderr, "unk op %s|%s", a->val, line);
			return(0);
		}
	}
	return(1);
}

struct {
	char *val;
	int num, len;
} ops[] = {
	{"and", 0},
	{"asg", 0},	/* asg ops are binary*/
	{"assign", 0},
	{"call", 1},
	{"cm", 0},
	{"comop", 0},
	{"compl", 0},
	{"conv", 0},
	{"decr", 0},
	{"div", 0},
	{"er", 0},
	{"fld", 2},
	{"funarg", 0},
	{"genbr", 2},
	{"genlab", 1},
	{"genubr", 1},
	{"icon", 1},
	{"init", 0},
	{"incr", 0},
	{"ls", 0},
	{"minus", 0},
	{"mod", 0},
	{"mul", 0},
	{"name", 1},
	{"not", 0},
	{"or", 0},
	{"plus", 0},
	{"reg", 1},
	{"return", 0},
	{"unary", 0},	/* common prefix for unary ops */
	{"rnode", 0},
	{"qnode", 0},
	{"snode", 0},
	{"rs", 0},
	{"star", 0},
	{"starg", 2},
	{"stasg", 1},
	{"stcall", 2},
	{"vauto", 1},
	{"vparam", 1},
	{0,0}
};

numargs(s)
char *s;
{	int i;
	static int init = 0;
	if(!init++)
		for(i = 0; ops[i].val; i++)
			ops[i].len = strlen(ops[i].val);
	for(i = 0; ops[i].val; i++)
		if(strncmp(s+1, ops[i].val,ops[i].len) == 0)
			return(ops[i].num);
	return(-1);
}

stripcrud(a)
struct arg *a;
{	char *p;
	for(p = a->val + a->len - 1; *p == ' ' || *p == ')'; p--)
		;
	if(p < a->val)
		p = a->val - 1;
	a->len = p - a->val + 1;
}

setwords(p)
char *p;
{	int i;
	i = 0;
loop:
	words[i].val = p;
	p = wordskip(p, ' ');
	words[i].len = p - words[i].val;
	if(*p == '\\')
		return(i);
	i++;
	goto loop;
}

argcmp(a, b)
struct arg *a, *b;
{	int n, i;
	n = a->len;
	if(b->len < n)
		n = b->len;
	i = strncmp(a->val, b->val, n);
	if(i != 0)
		return(i);
	if(a->len < b->len)
		return(-1);
	else if(a->len > b->len)
		return(1);
	return(0);
}

uniqargs()
{	int i, j;
	for(i = 1; i < narg; i++) {
		if(args[i].len <= 0)
			continue;
		for(j = 0; j < i; j++) {
			if(args[j].len <= 0)
				continue;
			if(argcmp(args + i, args + j) == 0) {
				/*printf("%5.5s %5.5s ", args[i].val, args[j].val);
				*/return(0);
			}
		}
	}
	return(1);
}

char *
wordskip(p, c)
char *p;
{
	for(; *p != c; p++)
		;
	for(; *p == c; p++)
		;
	return(p);
}

char *
matchskip(p)
char *p;
{	int cnt;
	for(cnt = 0; ; p++)
		if(*p == '(')
			cnt++;
		else if(*p == ')')
			if(--cnt <= 0)
				return(p+1);
}

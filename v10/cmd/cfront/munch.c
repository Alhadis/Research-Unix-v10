/*
	scan nm output and detect constructors and destructors for static objects.
	the name on an nm output line is expected to be in the right hand margin.
	the name is expected to be on the form __STD*_ or __STI*_ and less than
	100 characters long.
	nm output lines are assumed to be less than 256 characters long.
	constructors found are called by _main() called from main().
	destructors found is called by exit().
	return 0 if no constructor or destructor is found otherwise.
	The output is ?
	
*/

#include <stdio.h>
//extern int strcpy(char*, char*);
//extern char * strtok(char*, char*);

struct sbuf {
	sbuf* next;
	char str[100];
	sbuf(sbuf* l, char* p);
};

sbuf::sbuf(sbuf* l, char* p)
{
	next=l;
	// strcpy(str,strtok(p," |"));
	// ``unrolled'' since strtok() is not on bsd systems
	register char* s = str;
	for (register char c = *p++; c && c!=' ' && c!='|'; c = *p++) *s++ = c;
	*s = 0;	
}

sbuf* dtor;	// list of constructors
sbuf* ctor;	// list of destructors

void _main() { };	// make munch.c independent of libC.a
char* malloc(unsigned);
void* operator new(long s) { return malloc(s); }

main (int, char* argv[])
{
	char buf[256];
	register char* p;

newline:
	p = buf;
	for(;;) {
		int c;
		switch (c=getchar()) {
		case EOF:
			goto done;
		case '\n':
		{	if (p == buf) goto newline;	// ignore empty lines
			*p = 0;				// terminate string
			p = buf;
			while (*p++!='_') if (*p == 0) goto newline;
			if (p[-2] == '.') goto newline;	// RT: ignore ._STI
			for (p--; *p == '_'; p++) ;	// accept _STI and __STI
			register char* st = p-1;
			if (st[0]!='_' || 
				(st[1]!='S' && st[1]!='s') ||
				(st[2]!='T' && st[2]!='t') ||
				(st[1]=='s' && st[2]=='t' &&
				    (st[4]!='_' || st[5]!='_' || st[-1]!='_'))) goto newline;
			switch (st[3]) {
			case 'd':
				st--;
			case 'D':
				dtor = new sbuf(dtor,st);
				goto newline;
			case 'i':
				st--;
			case 'I':
				ctor = new sbuf(ctor,st);
			default:
				goto newline;
			}
		}
		default:
			*p++ = c;
		}
	}

done:
	int cond = dtor||ctor;

	if (cond == 0) exit(0);

	printf("typedef int (*PFV)();\n");	// "int" to dodge bsd4.2 bug
	if (ctor) {
		for (sbuf* p = ctor; p; p=p->next) printf("int %s();\n",p->str);
		printf("extern PFV _ctors[];\nPFV _ctors[] = {\n");
		for (sbuf* q = ctor; q; q=q->next) printf("\t%s,\n",q->str);
		printf("\t0\n};\n");
	}

	if (dtor) {
		for (sbuf* p = dtor; p; p=p->next) printf("int %s();\n",p->str);
		printf("extern PFV _dtors[];\nPFV _dtors[] = {\n");
		for (sbuf* q = dtor; q; q=q->next) printf("\t%s,\n",q->str);
		printf("\t0\n};\n");
	}

	exit(1);
}

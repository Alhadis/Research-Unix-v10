#include	"mk.h"

Word *
newword(s)
	char *s;
{
	register Word *w = (Word *)Malloc(sizeof(Word));

	w->s = strdup(s);
	w->next = 0;
	return(w);
}

Word *
stow(s)
	char *s;
{
	char save;
	register char *t;
	Word *head, *w;

	w = head = newword("");
	while(*s){
		while(SEP(*s))
			s++;
		if(*s == 0) break;
		for(t = s; *t && !SEP(*t); t++)
			;
		save = *t;
		*t = 0;
		w->next = newword(s);
		w = w->next;
		*t = save;
		s = t;
	}
	w = head->next;
	delword(head);
	return(w);
}

char *
wtos(w)
	register Word *w;
{
	static char buf[BIGBLOCK];
	register char *s = buf;
	register char *t;

	for(; w; w = w->next){
		for(t = w->s; *t;)
			*s++ = *t++;
		if(w->next)
			*s++ = ' ';
	}
	*s = 0;
	return(strdup(buf));
}

delword(w)
	Word *w;
{
	free(w->s);
	free((char *)w);
}

dumpw(s, w)
	char *s;
	Word *w;
{
	Fprint(1, "%s", s);
	for(; w; w = w->next)
		Fprint(1, " '%s'", w->s);
	Fputc(1, '\n');
}

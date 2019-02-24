#include	"mk.h"
#include	<ctype.h>

static Word *bollard;

static void
append(s)
	char *s;
{
	register Word *w;
	register char *p;

	if(bollard){
		for(w = bollard; w->next; w = w->next)
			;
		w = w->next = stow(s);
	} else
		w = bollard = stow(s);
	if(w)	/* if we got a word */
		for(p = w->s; *p;)
			*p++ &= ~EBIT;
}

Word *
expand(s)
	register char *s;
{
	char save;
	register char *t;
	Word *head, *w;

	w = head = newword("");
	while(*s){
		while(SEP(*s))
			s++;
		if(*s == 0) break;
		for(t = s+1; *t && !SEP(*t); t++)
			;
		save = *t;
		*t = 0;
		bollard = 0;
		glob(s, append);
		w->next = bollard;
		while(w->next)
			w = w->next;
		*t = save;
		s = t;
	}
	w = head->next;
	delword(head);
	return(w);
}

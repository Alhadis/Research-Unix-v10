#include	"mk.h"

char *infile;
int inline;
static longline();
extern Word *target1;

parse(f, fd, varoverride, ruleoverride)
	char *f;
{
	int hline;
	char *s, *body;
	Word *head, *tail;
	int attr, set;
	char *prog, *inc;

	if(fd < 0){
		perror(f);
		Exit();
	}
	ipush();
	infile = strdup(f);
	inline = 1;
	Finit(fd, (char *)0);
	while(s = Frdline(fd)){
		if((*s == '#') || (*s == 0)){
			inline++;
			continue;
		}
		hline = inline;
		switch(rhead(s, fd, &head, &tail, &attr, &prog))
		{
		case '<':
			if((tail == 0) || ((inc = wtos(tail)) == 0)){
				SYNERR(-1);
				Fprint(2, "missing include file name\n");
				Exit();
			}
			parse(inc, open(inc, 0), 0, 1);
			break;
		case ':':
			rbody(fd, &body);
			addrules(head, tail, body, attr, hline, ruleoverride, prog);
			break;
		case '=':
			if(head->next){
				SYNERR(-1);
				Fprint(2, "multiple vars on left side of assignment\n");
				Exit();
			}
			if(symlook(head->s, S_OVERRIDE, (char *)0)){
				set = varoverride;
				symdel(head->s, S_OVERRIDE);
			} else {
				set = 1;
				if(varoverride)
					symlook(head->s, S_OVERRIDE, "");
			}
			if(set)
				setvar(head->s, wtos(tail));
			if(attr)
				(void)symlook(head->s, S_NOEXPORT, "");
			break;
		default:
			SYNERR(hline);
			Fprint(2, "expected : or =\n");
			Exit();
			break;
		}
	}
	close(fd);
	ipop();
}

addrules(head, tail, body, attr, hline, override, prog)
	Word *head, *tail;
	char *body, *prog;
{
	register Word *w;

	assert("addrules args", head && body);
	if((target1 == 0) && !(attr&REGEXP))
		frule(head);
	for(w = head; w; w = w->next)
		addrule(w->s, tail, body, head, attr, hline, override, prog);
}

rhead(s, fd, h, t, attr, prog)
	char *s;
	Word **h, **t;
	int *attr;
	char **prog;
{
	char buf[BIGBLOCK];
	register char *p;
	char *pp;
	int sep;

	longline(fd, s, buf);		/* cover \\n guys */
	lex(fd, buf);
	if(p = strchr(buf, '#'))	/* comment ? */
		*p = 0;
	for(p = buf; *p; p++)
		if((*p == ':') || (*p == '=') || (*p == '<')) break;
	if(*p == 0){
		return('?');
	}
	sep = *p;
	*p++ = 0;
	*attr = 0;
	*prog = 0;
	if(sep == '='){
		char *le, *e;

		le = strchr(p, ' ');
		if(e = strchr(p, '\t'))
			if((le == 0) || (e < le))
				le = e;
		if((e = strchr(p, '=')) && ((le == 0) || (e < le))){
			while(*p != '='){
				switch(*p++)
				{
				case 0:
					SYNERR(-1);
					Fprint(2, "missing trailing =\n");
					Exit();
				default:
					SYNERR(-1);
					Fprint(2, "unknown attribute '%c'\n", p[-1]);
					Exit();
				case 'U':
					*attr = 1;
					break;
				}
			}
			p++;
		}
	}
	if((sep == ':') && *p && (*p != ' ') && (*p != '\t')){
		while(*p != ':')
			switch(*p++)
			{
			case 0:
		eos:
				SYNERR(-1);
				Fprint(2, "missing trailing :\n");
				Exit();
			default:
				SYNERR(-1);
				Fprint(2, "unknown attribute '%c'\n", p[-1]);
				Exit();
			case '<':
				*attr |= RED;
				break;
			case 'D':
				*attr |= DEL;
				break;
			case 'N':
				*attr |= NOREC;
				break;
			case 'P':
				pp = p;
				while(*p && (*p != ':'))
					p++;
				if(*p == 0)
					goto eos;
				*p = 0;
				*prog = strdup(pp);
				*p = ':';
				break;
			case 'Q':
				*attr |= QUIET;
				break;
			case 'R':
				*attr |= REGEXP;
				break;
			case 'U':
				*attr |= UPD;
				break;
			case 'V':
				*attr |= VIR;
				break;
			}
		p++;
	}
	*h = expand(buf);
	if((*h == 0) && (sep != '<')){
		SYNERR(inline-1);
		Fprint(2, "no var on left side of assignment/rule\n");
		Exit();
	}
	while((*p == ' ') || (*p == '\t'))
		p++;
	*t = expand(p);
	return(sep);
}

rbody(fd, b)
	char **b;
{
	char buf[BIGBLOCK];
	char *cp, *s;
	register n;

	cp = buf;
	while(s = Frdline(fd)){
		inline++;
		if((*s != '\t') && (*s != ' ')){
			inline--;
			Fundo(fd);
			break;
		}
		memcpy(cp, s+1, n = FIOLINELEN(fd)-1);
		cp += n;
		*cp++ = '\n';
	}
	*cp++ = 0;
	assert("rbody overflow", cp <= &buf[BIGBLOCK]);
	*b = (buf[0])? strndup(buf, (int)(cp-buf)) : strdup("");
}

static
longline(fd, line, dest)
	register char *line, *dest;
{
	for(;;){
		inline++;
		while(*dest++ = *line++);
		dest -= 2;
		if(*dest == '\\'){
			*dest = 0;
			if((line = Frdline(fd)) == 0){
				Fundo(fd);
				break;
			}
		} else
			break;
	}
}

struct input
{
	char *file;
	int line;
	struct input *next;
};
static struct input *inputs = 0;

ipush()
{
	register struct input *in, *me;

	me = (struct input *)Malloc(sizeof(*me));
	me->file = infile;
	me->line = inline;
	me->next = 0;
	if(inputs == 0)
		inputs = me;
	else {
		for(in = inputs; in->next; )
			in = in->next;
		in->next = me;
	}
}

ipop()
{
	register struct input *in, *me;

	assert("pop input list", inputs != 0);
	if(inputs->next == 0){
		me = inputs;
		inputs = 0;
	} else {
		for(in = inputs; in->next->next; )
			in = in->next;
		me = in->next;
		in->next = 0;
	}
	infile = me->file;
	inline = me->line;
	free((char *)me);
}

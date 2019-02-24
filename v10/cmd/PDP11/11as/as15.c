/* a5 -- pdp-11 assembler pass 1 */
#include "as1.h"

OP savop;
OP lastop;
int noputw;

OP
readop()
{
	if (savop.xp) {lastop=savop; savop.xp=0; return(lastop);}
	noputw=0;
	lastop=_readop(rch());
	if (noputw==0) putw(lastop.v);
	return(lastop);
}

extern int fbflag;
int rschflg;

OP
_readop(t)
register int t;
{
	OP s;

	for (;;) {
	switch (chartab[t]) {
	case C_IOR:
		s.v=037; return(s);
	case C_ESC:
		switch (t=rch()) {
			case '/': s.v='/'; return(s);
			case '<': s.v=035; return(s);
			case '>': s.v=036; return(s);
			case '%': s.v=037; return(s);
		}
		s.v=t; return(s);
	case C_WHITE:
		break;
	case C_TERM:
		s.v=t; return(s);
	case C_SQUO:
		t=rsch(); goto ccon;
	case C_DQUO:
		t=rsch(); t |= rsch()<<8;
	 ccon:
		numval=t; putw(T_ABS); putw(numval); s.v=T_ABS; noputw=1; return(s);
	case C_GARB:
		error('g'); break;
	case C_SKIP:
		for (;;) {
			t=rch();
			if (t=='\004' || t=='\n') {s.v=t; return(s);}
		}
	case C_EOL:
		s.v=t; return(s);
	case C_STR:
		putw('<'); numval=0;
		for (;;) {
			t=rsch();
			if (rschflg) break;
			putw(t|0400);
			numval++;
		}
		putw(-1);
		s.v='<'; noputw=1; return(s);
	default:
		ch=t;
		if ('0'<=t && t<='9') {
			t=number();
			if (fbflag==0) goto ccon;
			s.v=t; return(s);
		}
		s.xp=rname(t); return(s);
	}
	t=rch();
	}
}

rsch()
{
	register int t=rch();

	if (t=='\004' || t=='\n') {error('<'); aexit();}
	rschflg=0;
	if (t=='\\') {
		switch (t=rch()) {
			case 'n': return(012);	
			case 's': return(040);
			case 't': return(011);
			case 'e': return(004);
			case '0': return(000);
			case 'r': return(015);
			case 'a': return(006);
			case 'p': return(033);
		}
		return(t);
	}
	if (t=='>') rschflg=1;
	return(t);
}

%{
#include	"hdr.h"
#include	"x.tab.h"
#undef	getc
#undef	output
#define	output(x)	(void)(x)
extern debug;
extern char *strdup();
%}
%%

audio1		return(AUDIO1);
audio2		return(AUDIO2);
audio3		return(AUDIO3);
auto		return(AUTO);
cue		return(CUE);
edit		return(EDIT);
frame		return(FRAME);
fwd		return(FWD);
go		return(GO);
help		return(HELP);
insert		return(INSERT);
ltc		return(LTC);
loop		return(LOOP);
mode		return(MODE);
off		return(OFF);
on		return(ON);
play		return(PLAY);
preroll		return(PREROLL);
rec		return(REC);
remote		return(REMOTE);
rev		return(REV);
set		return(SET);
shoot		return(SHOOT);
sleep		return(SLEEP);
snap		return(SNAP);
standby		return(STANDBY);
status		return(STATUS);
step		return(STEP);
still		return(STILL);
stop		return(STOP);
t1		return(T1);
t2		return(T2);
tcr		return(TCR);
time		return(TIME);
ub		return(UB);
video		return(VIDEO);
vidsync		return(VIDSYNC);
view		return(VIEW);
vitc		return(VITC);
wait		return(WAIT);
[0123456789]+	{ yylval.ival = atoi(yytext); return(NUMBER); }
\n		{ yylineno++; return('\n'); }
#.*		{ ; /* comment */ }
[ \t]+		{ ; /* white space */ }
^!.*		{ Fflush(logfd);if(!debug) system(yytext+1); print("!\n"); }
\"[^"]*\"	{ yytext[yyleng-1] = 0; yylval.str = strdup(yytext+1); return(STRING); }
.		{ return(yytext[0]); }

%%

%start		input

%{

#define		LTIME(t, hr, mi, se, fr)	(t.h=hr, t.m=mi, t.s=se, t.f=fr)

#define		YYDEBUG		1

#include	"hdr.h"

%}

%token		AUTO CUE EDIT HELP INSERT MODE PLAY REC SET SNAP STATUS STEP STOP
%token		LOOP VIEW WAIT FRAME STANDBY REMOTE SHOOT SLEEP
%token		FWD REV OFF ON PREROLL STILL GO
%token		VIDSYNC AUDIO1 AUDIO2 AUDIO3 VIDEO
%token		TIME LTC VITC T1 T2 TCR UB
%token		NUMBER STRING

%type <time>	time
%type <ival>	NUMBER timertype channel channels
%type <str>	STRING

%union{
	int ival;
	Time time;
	char *str;
}

%%

input	:	/* nothing */
	|	input line '\n'
	|	input line ';'
	|	input line error '\n'
	|	input line error ';'
	;
	/*
		help can be done explicitly by :help info::
	*/

line	:		/* nothing */
	|	AUTO ON  { two("auto on", 0x40, 0x41); } /*:::*/
	|	AUTO OFF  { two("auto off", 0x40, 0x40); } /*:AUTO [ON|OFF]::*/
	|	CUE time { cue($2); }
	|	EDIT OFF  { two("edit off", 0x20, 0x64); }
	|	FRAME OFF { frame(0); } /*:::*/
	|	FRAME ON { frame(1); } /*:FRAME [ON|OFF]::*/
	|	HELP  { help(); }
	|	EDIT SET time time channels { editset($3, $4, $5); }/*:EDIT SET in_time out_time channels::*/
	|	EDIT GO { editgo(); }
	|	INSERT time  { insert($2, 1); }/*:INSERT time [nframes=1]::*/
	|	INSERT time NUMBER  { insert($2, $3); } /*:::*/
	|	LOOP time time NUMBER   { loop($2, $3, $4); } /*:LOOP tbegin tend frames/s:: */
	|	PLAY  { two("play", 0x20, 0x01); }
	|	PREROLL TIME time  { prerollt($3); }
	|	REC  { two("rec", 0x20, 0x02); }
	|	REC STEP  { three("rec step", 0x41, 0x3F, 2); }
	|	REMOTE NUMBER { remiframe($2); } /*:REMOTE [frame|str]::*/
	|	REMOTE STRING { remsframe($2); } /*:::*/
	|	REMOTE { remiframe(-1); } /*:::*/
	|	REMOTE STRING STRING { remsetup($2, $3, (char *)0); } /*:REMOTE system prog [relay]::*/
	|	REMOTE STRING STRING STRING { remsetup($2, $3, $4); } /*:REMOTE system prog [relay]::*/
	|	SET TIME T1 time { settimer($4, 0); } /*:SET TIME [T1|TCR] time::*/
	|	SET TIME TCR time { settimer($4, 4); } /*:::*/
	|	SHOOT NUMBER NUMBER { shoot($2, $3, 1); } /*:SHOOT start nframes::*/
	|	SLEEP NUMBER { sleep($2); }
	|	snap { snap(1); } /*:SNAP [nframes=1]::*/
	|	snap NUMBER { snap($2); } /*:::*/
	|	STANDBY OFF  { two("stop", 0x20, 0x00); two("standby off", 0x20, 0x04); }
	|	STATUS  { status(0); }
	|	STATUS STATUS  { status(1); }
	|	STEP FWD  { two("step fwd", 0x20, 0x14); } /*:STEP [FWD|REV]::*/
	|	STEP REV  { two("step rev", 0x20, 0x24); } /*:::*/
	|	STILL time  { still($2); }
	|	STILL MODE ON { three("", 0x41, 0x3F, 0x01); } /*:STILL MODE [ON|OFF]::*/
	|	STILL MODE OFF { three("", 0x41, 0x3F, 0x00); } /*:::*/
	|	STOP  { two("stop", 0x20, 0x00); printclock(); }
	|	TIME timertype  { timer($2); }
	|	TIME MODE TCR  { three("time mode tcr", 0x41, 0x36, 0); } /*:TIME MODE [T1|T2|TCR]::*/
	|	TIME MODE T1  { three("time mode t1", 0x41, 0x36, 1); } /*:::*/
	|	TIME MODE T2  { three("time mode t2", 0x41, 0x36, 2); } /*:::*/
	|	VIEW time time { view($2,$3); } /*:VIEW tbegin tend:: */
	|	WAIT  {  swait(); }
	;

/*DO NOT MOVE, used to generate helptab.c*/

snap	:	SNAP
	|	EDIT ON
	;

time	:	NUMBER  { LTIME($$, 0, 0, $1, 0); }
	|	NUMBER '.' NUMBER  { LTIME($$, 0, 0, $1, $3); }
	|	NUMBER '.' NUMBER '.' NUMBER  { LTIME($$, 0, $1, $3, $5); }
	|	NUMBER '.' NUMBER '.' NUMBER '.' NUMBER  { LTIME($$, $1, $3, $5, $7); }
	;

timertype	:	LTC  { $$ = 0x01; }
	|	 { $$ = 0x01; }
	|	VITC  { $$ = 0x02; }
	|	T1  { $$ = 0x04; }
	|	T2  { $$ = 0x08; }
	;

channels	:	channel	{ $$ = $1; }
	|	channels channel { $$ = $1|$2; }
	;

channel	:	AUDIO1	{ $$ = 0x01; }
	|	AUDIO2	{ $$ = 0x02; }
	|	AUDIO3	{ $$ = 0x04; }
	|	VIDEO	{ $$ = 0x18; }
	;

%%

yywrap()
{
	return(1);
}

yyerror(s1, s2)
	char *s1;
{
	fprint(2, s1, s2);
}

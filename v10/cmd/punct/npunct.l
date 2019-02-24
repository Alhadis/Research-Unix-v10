%{
/* NOTICE-NOT TO BE DISCLOSED OUTSIDE BELL SYS EXCEPT UNDER WRITTEN AGRMT */
/* Writer's Workbench version x.x, date */

#include <ctype.h>
#undef YYLMAX
#define YYLMAX 500
#undef input
#undef unput
#define OFF 0
int ineqn = 0;
int low = 0;
%}
 int flag,c,n,i,bracket,slash,save;
 flag=0;
 bracket=0;
 slash=0;
 save=0;
%e 1500
%p 4000
%a 3000
%START ENDM
U       [A-Z]
A	[A-Za-z]
O	[^A-Za-z]
L       [a-z]
N       [0-9i]
B       [\ \n\t]
S	[\ \t]
D	[0-9.]
%%
(et{B}+al|a"."d|A"."M|a"."m|b"."c|Ch|{B}ch|{B}ckts|Corp|dB|Dept|dept|Depts|depts)"."     |
etc"..."	|
(Dr|Drs|e"."g|Eq|eq|etc|Fig|Figs|fig|figs)"."   |
({B}ft|i"."e|{B}in|Inc|Jr|jr|lb|lbs|{B}mi|Mr|Mrs|Ms|No|{B}no|Nos|nos)"."    |
(P"."{B}*M|p"."m|Ph"."D|Ph"."d|PhD|Ref|ref|Refs|refs|sec|Sec|St|vs|v|yr)"." |
(Proc|Trans|Conf|Symp)"." |
(Colo|Calif|Ltd|Ga|Md|Va|Co|Fla)"." |
(I|II|III|IV|V|VI|VII|VIII|IX|X)"."	|
(mm|\(pp|pp|cf|Cf|hrs|{B}+pl|{B}+al|conj|fem|masc)"."	ECHO;


\"\./{B}	{
			/*---------------PERIODS------------------------*/
			/* ONE: periods go inside quotes before a blank */
		printf(".\"");
		BEGIN ENDM;
		}
"''."/{B}	{printf(".''"); BEGIN ENDM;}

\".\"\.	{			/*leave examples as is "a". */
	ECHO;
	BEGIN ENDM;
	}
"``"."''."	{
		ECHO;
		BEGIN ENDM;
		}
{D}+\"	{			/* leave 3". for 3 inches */
		ECHO;
		}
\""..."\"\.	ECHO;
"``...''."	ECHO;
\"{D}+\"\.	{		/* but fix "3.0". */
		yytext[yyleng-2] = '\0';
		printf("%s.\"",yytext);
		BEGIN ENDM;
		}
"``"{D}+"''."	{
		yytext[yyleng-3] = '\0';
		printf("%s.''",yytext);
		BEGIN ENDM;
		}


\.\.	{
			/* TWO: two periods changed to one. */
		printf(".");
		BEGIN ENDM;
			/* exceptions */
		}
\.\.[.]+	ECHO;
". . ."	ECHO;
"..."\"\.	ECHO;
"...''."	ECHO;

^\..*\n		{
			/* THREE: to handle troff & preprocessors */
		if(yytext[1] == 'T' && yytext[2] == 'S'){
			ECHO;
			skip('T','E',0);
		}
		else if(yytext[1] == 'P' && yytext[2] == 'S'){
			ECHO;
			skip('P','E',0);
		}
		else if(yytext[1] == 'D' && yytext[2] == 'S'){
			ECHO;
			skip('D', 'E', 0);
		}
		else if(yytext[1] == 'E' && yytext[2] == 'Q'){
			ECHO;
			skip('E','N',1);
		}
		else {
			for (i=0;i<yyleng-1;i++) output(yytext[i]);
			yyless(yyleng-1);
		}
		BEGIN 0;
		}


{L}\.{B}+{L}[^).]	{ /* FOUR: Capitalize first word of sentence,
				but not a. or a). **/
		yytext[yyleng-2]=toupper(yytext[yyleng-2]);
		ECHO;
		}


{L}\.{B}*\n\.P.*\n{L}[^).]	{/* Cap after .P */
			yytext[yyleng-2]=toupper(yytext[yyleng-2]);
			ECHO;
		}

<ENDM>{B}+\"{L}	|
<ENDM>{B}+"``"{L}	|
<ENDM>{B}+{L}	{	/* other sent caps */
		yytext[yyleng-1] = toupper(yytext[yyleng-1]);
		ECHO;
		BEGIN 0;
		}
<ENDM>.		{
		unput(yytext[0]);
		BEGIN 0;
		}


{B}+{L}\.{B}+	{ /* Exceptions:
			abbreviations at top not counted as sentences
			(deleted periods in references code)
			don't understand this rule */
		ECHO;	/* a. many is the time...*/
		}


{L}\./{U}{L}	{ /* FIVE: Put space after sentence end if omitted */
		ECHO; putchar(' ');}

\.{A}\.{B}+	{ /* Exception: the first 2 rules handle i.d. & i.i.d. */
		ECHO;
		}
\.{A}\. {
	yyless(yyleng-1);
	ECHO;
	}
{L}\.{U}{U}	ECHO;	  /*comm.SYS */



\.{S}*\)/{B}+{U} {	/* SIX: incomplete sentences in parens have . outside*/
		if(--flag < 0)flag = 0;
		if(flag == 0 && low == 1){
			printf("\)\.");
		}
		else ECHO;
		low = 0;
	}
\){S}*\./{B}+{U}	{
		if(--flag < 0)flag = 0;
		if(flag == 0 && low == 2)
			printf(".) ");
		else ECHO;
		low = 0;
		}
\.{S}*\) 	{
		if(--flag < 0)flag = 0;
		if(flag == 0 && low != 2)
			printf("). ");
		else ECHO;
		low = 0;
		}

et\.{B}*al	{	/* SEVEN: other */
		printf("et al.");
		if((c=input()) != '.')unput(c);
		}
\?\"\.	{		/* ... transimission?". */
		ECHO;
		BEGIN ENDM;
	}
\?"''."		{
		ECHO;
		BEGIN ENDM;
		}



\"\,    {	/*------------------------------COMMAS----------------------*/
		/* ONE: commas go inside double quotes */
		/*  leave alone if inside (), may be code */
		if(flag == 0)printf(",\"");
		else ECHO;
	}
"'',"	{
	if(flag == 0)printf(",''");
	else ECHO;
	}


\".\"","	{ /* Exception */
		ECHO;		/*leave examples as is "a", */ 
		}
"``"."'',"	ECHO;


\,{B}*\(	{/* TWO: no commas before ( */
		if(flag == 0){
			printf(" ");
			yyless(1);
			save = 1;
		}
		else {
			yyless(1);
			ECHO;
		}
		}


\,{B}*\({N}+{A}*\)	{ /* Exceptions */
		ECHO;  /* ,  (2) or ,  (34b) */
		}
\,{B}*\({A}\)	ECHO;  /* , (b) */
\)\,{B}*\(	ECHO;   /* (1), (2),..*/
i\.e\.\,	|
e\.g\.\,	ECHO;



\,\,	{		/* THREE: No ,, except inside (), [], // */
		if (flag!=0|bracket==1|slash==1) ECHO;
	else printf(",");
	}



{L}\,/{L}	{	/* FOUR: put blank after comma */
			/* except in (), [], // (x,y,,a,3) */
		if(flag!=0|slash==1|bracket==1) ECHO;
		else {putchar(yytext[0]); printf(", ");}
	}
{L}\,/{U}{L}	{if(flag!=0|slash==1|bracket==1) ECHO;
	else {putchar(yytext[0]); printf(", ");}
	}

\,\.\.\.\,	{	/* Exception: math */
		ECHO;
		}
i\,j    |
x\,y    |
p\,q    ECHO;



;\"	{	/*-----------------COLONS & SEMI-COLONS--------------------*/
		/* ONE: : and ; go outside of quotes */
	printf("\";");
	}
;"''"	printf("'';");
:\"	{
	if(flag == 0)printf("\":");
	else ECHO;
	}
:"''"	printf("'':");


\"[;:]\"	{ /* Exception: when : or ; is quoted, i.e. ";" */
		ECHO;
		}
"``"[;:]"''"	ECHO;


;;	{	/* TWO: no double ;; */
	yyless(1);
	}
::	{	/* no double :: */
	yyless(1);
	}

\(;;\)	{ /* Exception: C-code for(;;) */
	ECHO;
	}

;{B}*\(	{	/* THREE: no semicolons before ( */
		if(flag == 0){
			printf(" ");
			yyless(1);
			save = 2;
		}
		else {
			yyless(1);
			ECHO;
		}
		}


;{B}*\({N}+\)   {	/* Exceptions */
		ECHO;  /* ;  (2) */
		}
;{B}*\({A}\)    ECHO;  /* ; (b) */
\);{B}*\(       ECHO;   /* (1); (2);..*/


[;:]/{A}	{	/* FOUR: put space after ; & : */
		if(flag == 0){
			putchar(yytext[0]); putchar(' ');
		}
		else ECHO;
		}



\?\,	{	/*----------------------QUESTION MARKS-----------------------*/
		/* ONE: ?, becomes ? */
	printf("?");
	}
\?\"\,	printf("?\"");
\?"'',"	printf("?''");


\?\?	{	/* TWO: ?? becomes ? */
	yyless(1);
	}


{L}\?{B}+{L}  	{	/* THREE: capitalize new sentence after ? */
		yytext[yyleng-1]=toupper(yytext[yyleng-1]);
		ECHO;
		}


{L}\?/{U}{L}    { /* FOUR: put space after ? between sentences */
		ECHO; putchar(' ');}



\({B}*/{L}	{ /*----------------------PARENTHESES-----------------------*/
		/* NOTE: The value of flag tells if you are inside a pair
		of parentheses, low is set if first word started in lower case
		at the first level of parens
		  flag=1 and low = 1 if ([a-z]
		  flag = 1 and low = 0 if ([^a-z]
		  flag = 0 if )
		  bracket & slash toggle between 0 & 1 on \[\] \& \/
		ONE: for left parens */
		if(flag == 0) low = 1;
		flag++;
		ECHO;
		}
\({B}*/{U}	{
		if(flag == 0)low = 2;
		flag++;
		ECHO;
		}
\(		{
		if(flag == 0)low = 0;
		flag++;
		ECHO;
		}
\\\(..	ECHO;		/* troff characters */
\[	{bracket=1; ECHO;}
\\	{if(slash==0) slash=1;
	 else slash=0;
	 ECHO;
				/* TWO: next for right parens */
	}


\?{B}*\)	|
{B}+{A}\.\)	|	/* ...Jones, A.)  */
\.{A}\.\)	|		/* (i.i.d.) */
\)	{
		if(--flag < 0)flag = 0;
		ECHO;
		if(save == 1)putchar(',');
		if(save==2) putchar(';');
		save=0;
	}

\)+[,;]	{
		if(--flag < 0)flag = 0;
		ECHO;
		save = 0;
		}

\]	{bracket=0; ECHO;}

\.	{ /* all other .'s */
	ECHO;
	BEGIN ENDM;
	}

%%
int rdelim = 0;
int ldelim = 0;
char del[] = "elim";
skip(c1,c2,eqn)
char c1,c2;
{
	int c,first;
	char *s;
	first = 1;
cont:
	while((c=input()) != '.'){
ck:
		if(c == 0)return;
		if(c == '\n')first = 1;
		else first = 0;
		putchar(c);
		if(eqn && c == 'd'){
			for(s=del;*s != '\0';s++){
				if((c=input()) != *s)goto ck;
				putchar(c);
			}
			while((c=input()) == ' ')putchar(c);
			if(c == '\n' || c == 'o'){
				ldelim = rdelim = OFF;
				first = 1;
				putchar(c);
				if(c != '\n')
					while((c=input()) != '\n')putchar(c);
				continue;
			}
			putchar(c);
			ldelim = c;
			rdelim = getchar();
			putchar(c);
			continue;
		}
	}
	if(first != 1){
		putchar('.');
		goto cont;
	}
	if((c=input()) != c1){
		if(c == 0)return;
		putchar('.'); putchar(c);
		if(c == '\n')first = 1;
		else first = 0;
		goto cont;
	}
	if((c=input()) != c2){
		if(c == 0)return;
		if(c == '\n')first = 1;
		else first = 0;
		printf(".%c%c",c1,c);
		goto cont;
	}
	printf(".%c%c",c1,c2);
	while((c=input()) != '\n'){
		if(c == 0)return;
		putchar(c);
	}
	unput('\n');
}
char buf[YYLMAX];
char *ptr = buf;
input(){
	int cc;
	if(ptr != buf){
		cc = *ptr--;
		if(ldelim == 0)return(cc);
		if(cc != ldelim && ineqn == 0)return(cc);
		if(ineqn == 0){
			ineqn = 1;
			return(ldelim);
		}
		putchar(cc);
		if(cc == rdelim)goto gotit;
		while(ptr != buf){
			cc = *ptr--;
			putchar(cc);
			if(cc == rdelim){
gotit:
				ineqn = 0;
				if(ptr != buf)return(*ptr--);
				else {
					cc=getchar();
					return(cc==EOF?0:cc);
				}
			}
		}
		goto more2;
	}
	if(ldelim == 0){
		cc=getchar();
		return(cc==EOF?0:cc);
	}
more:
	if((cc=getchar()) != ldelim && ineqn == 0){
		return(cc==EOF?0:cc);
	}
	if(ineqn == 0){
		ineqn = 1;
		return(ldelim);
	}
	putchar(cc);
	if(cc == rdelim){
		ineqn = 0;
		goto more;
	}
more2:
	while((cc=getchar()) != rdelim)
		if(cc == EOF){ineqn = 0; return(0);}
		else putchar(cc);
	putchar(cc);
	ineqn = 0;
	goto more;
}
unput(cc)
char cc;
{
	*(++ptr) = cc;
	if(ineqn && cc == ldelim)ineqn = 0;
}

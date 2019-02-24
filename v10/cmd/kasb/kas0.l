/*	@(#)kas0.l	1.1	*/
%%
%{
	struct symtab *op;
	static struct symtab tmplab;
%}
[ \t]	;
"/*"	{
	char c;

	while(c=input()) {
		if (c=='\n')
			lineno++;
		else if (c=='*')
			if ((c=input())=='/')
				break;
			else
				unput(c);
	}
	}
[\._a-zA-Z][0-9\._a-zA-Z]*	{
	if ((op = *lookup(1))->type&XTAG) {
		yylval.sym = op;
		return(op->type&~XTAG);
	} else {
		yylval.sym = op;
		if ((op->type&XTYPE)==XUNDEF)
			return(UNAME);
		else	return(DNAME);
	}
	}
[0-9][bf]	{
	op = &loclab[yytext[0]-'0'];
	if (yytext[1]=='b') {
		if (op->name[0]==0)
			yyerror("undefined numeric label");
		tmplab.value = op->type;
		tmplab.type = XTEXT;
		yylval.sym = &tmplab;
		return(DNAME);
	} else {
		yylval.sym = op;
		return(TNAME);
	}
	}
0[0-7]*	{
	sscanf(yytext, "%o", &yylval.ival);
	return(INT);
	}
0[xX][0-9a-fA-F]+	{
	sscanf(&yytext[2], "%x", &yylval.ival);
	return(INT);
	}
[1-9][0-9]*	{
	sscanf(yytext, "%d", &yylval.ival);
	return(INT);
	}
"\n"	return(NL);
"%"	return(POP);
"("	return(LP);
")"	return(RP);
"*"	return(MUL);
"/"	return(DIV);
"++"	return(INC);
"+"	return(PLUS);
"-"	return(MINUS);
","	return(CM);
":"	return(COLON);
";"	return(SEMI);
"|"	return(BAR);
"~"	return(NOT);
"&"	return(AND);
">>"	return(RS);
"<<"	return(LS);
"^"	return(XOR);
^!.*	{
	system(&yytext[1]);
	printf("!\n");
	}
^#.*\n	{
	if (sscanf(&yytext[1], "%d%*[\" ]%[^\"\n]", &lineno, infile = instr)<1)
		yyerror("illegal preprocessor line");
	}
'\\.'	{
	switch(yytext[2]) {
	case  '0':
		yylval.ival = 0;
		break;
	case 'b':
		yylval.ival = 010;
		break;
	case 't':
		yylval.ival = 011;
		break;
	case 'n':
		yylval.ival = 012;
		break;
	case 'f':
		yylval.ival = 014;
		break;
	case 'r':
		yylval.ival = 015;
		break;
	default:
		yylval.ival = yytext[2];
	}
	return(INT);
	}
'.'	{
	yylval.ival = yytext[1];
	return(INT);
	}
\"[^"\n]*\"	{
	yytext[yyleng-1] = '\0';
	strcpy(dbstr, &yytext[1]);
	yylval.str = dbstr;
	return(STR);
	}
.	{
	yyerror("Illegal character");
	return(NOCHAR);
	}
%%

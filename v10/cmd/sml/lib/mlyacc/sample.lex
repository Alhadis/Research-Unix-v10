open Calc.LexValue
type lexresult=V
val eof = fn () => EOF
%%
alpha=[A-Za-z];
digit=[0-9];
ws = [\ \t\n];
%%
{ws}+	 => (lex());
{digit}+ => (NUM (revfold (fn (a,r) => ord(a)-ord("0")+10*r) (explode yytext) 0));
"+"	 => (PLUS);
"*"	 => (TIMES);
";"	 => (EOS);
{alpha}+ => (if yytext="print" then PRINT else ID yytext);
"-"	 => (SUB);
"^"	 => (CARAT);
"/"	 => (DIV);
.	 => (Calc.HDR.error ("ignoring bad character " ^ yytext); lex());

open C.LexValue
type lexresult = V

val pcount = ref 0;
val commentLevel = ref 0
val lineno = C.HDR.lineno
val actionstart = ref 0
val eof = fn () => (if (!pcount)>0 then
			 error " eof encountered in action beginning here !"
				(!actionstart)
		      else (); EOF)

val text = ref (nil : string list)
val Add = fn s => (text := s::(!text))
val error = C.HDR.error
val inc = fn (i:int ref) => i := (!i) +1

fun lookup s =
let val dict = [("%prec",PREC_TAG),("%term",Term),
		  ("%nonterm",Nonterm),("%nonassoc",PREC NONASSOC),
		  ("%left",PREC LEFT),("%right",PREC RIGHT),
		  ("%eof",PERCENT_EOF),("%start",START),
		  ("%prefer",PREFER),("%insert_prefer",IPREFER),
		  ("%keyword",KEYWORD),("%structure",STRUCT),
		  ("%verbose",VERBOSE)]

     fun find ((a,d)::b) = if a=s then d else find(b)
       | find nil = (UNKNOWN s)
in find dict
end

%%
%s A B C D F COMMENT STRING;
ws = [\t\ ]*;
idchars = [A-Za-z_'0-9];
id=[A-Za-z]{idchars}*;
tyvar="'"{idchars}*;
qualid ={id}".";
%%
<INITIAL>.	=> (YYBEGIN C; lineno := 1; text := [yytext]; lex());
<INITIAL>\n	=> (YYBEGIN C; lineno := 2; text := [yytext]; lex());
<INITIAL>"%%"	=> (YYBEGIN D; HEADER "");

<C>[^%\n]+	=> (Add yytext; lex());
<C>"%%"		=> (YYBEGIN D; HEADER (implode (rev (!text))));
<C>\n		=> (Add yytext; inc lineno; lex());
<C>.		=> (Add yytext; lex());

<D>"("		=> (LPAREN);
<D>")"		=> (RPAREN);

<D,A>of		=> (OF);
<D,A>for	=> (FOR);
<D,A>val	=> (VAL);
<D,A>"="	=> (EQUAL);
<D,A>"%%"	=> (YYBEGIN A; DELIMITER);
<D,A>{ws}+	=> (lex());
<D,A>\n+	=> (lineno := !lineno + (size yytext); lex());
<D,A>":"	=> (COLON);
<D,A>"|"	=> (BAR);
<D,A>"{"	=> (LBRACE);
<D,A>"}"	=> (RBRACE);
<D,A>","	=> (COMMA);
<D,A>"*"	=> (ASTERISK);
<D,A>"->"	=> (ARROW);
<D,A>"%"[a-z_]+	=> (lookup yytext);
<D,A>{id}	=> (ID yytext);
<D,A>[0-9]+	=> (INT yytext);
<D,A>{tyvar}	=> (TYVAR yytext);
<D,A>{qualid}	=> (IDDOT yytext);
<A>"("		=> (pcount := 1; actionstart := (!C.HDR.lineno);
		    text := nil; YYBEGIN B; lex());
<D,A>.		=> (UNKNOWN yytext);

<B>"("		=> (pcount := (!pcount) + 1; Add yytext; lex());
<B>")"		=> (pcount := (!pcount) - 1; 
		    if (!pcount = 0) then
			 (YYBEGIN A; PROG (implode (rev (!text))))
		    else (Add yytext; lex()));
<B>"\""		=> (Add yytext; YYBEGIN STRING; lex());
<B>[^()"\n]+	=> (Add yytext; lex());
<B>\n		=> (Add yytext; inc lineno; lex());
<B>"(*"		=> (Add yytext; YYBEGIN COMMENT; inc commentLevel; lex());

<COMMENT>[(*)]	=> (Add yytext; lex());
<COMMENT>"(*"	=> (Add yytext; inc commentLevel; lex());
<COMMENT>[^*()\n]+ => (Add yytext; lex());
<COMMENT>\n	=> (Add yytext; inc lineno; lex());
<COMMENT>"*)"	=> (Add yytext; dec commentLevel;
		    if !commentLevel=0 then YYBEGIN B else (); lex());
<STRING>"\""	=> (Add yytext; YYBEGIN B; lex());
<STRING>[^"\\\n]+ => (Add yytext; lex());
<STRING>\\	=> (Add yytext; lex());
<STRING>\\\"	=> (Add yytext; lex());
<STRING>\n	=> (Add yytext; error "unclosed string" (!lineno); 
		    inc lineno; YYBEGIN B; lex());
<STRING>\\[\ \t\n]   => (Add yytext;
			if substring(yytext,1,1)="\n" then inc lineno else ();
		     	YYBEGIN F; lex());

<F>\n		=> (Add yytext; inc lineno; lex());
<F>[\ \t]+	=> (Add yytext; lex());
<F>\\		=> (Add yytext; YYBEGIN STRING; lex());
<F>.		=> (Add yytext; error "unclosed string" (!lineno);
		    YYBEGIN B; lex());
%%

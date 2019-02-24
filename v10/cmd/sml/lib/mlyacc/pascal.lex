open C.LexValue
type lexresult=V
val eof = fn () => EOF
val lineno = C.G.lineno

structure KeyWord : sig
	     		val find : string -> V option
	  	    end =
  struct

	val TableSize = 211
	val HashFactor = 5

	val hash = fn s =>
	   fold (fn (c,v)=>(v*HashFactor+(ord c)) mod TableSize) (explode s) 0


	val HashTable = array(TableSize,nil) : (string * V) list array


	val add = fn (s,v) =>
	 let val i = hash s
	 in update(HashTable,i,(s,v) :: (HashTable sub i))
	 end

        val find = fn s =>
	  let val i = hash s
	      fun f ((key,v)::r) = if s=key then SOME v else f r
	        | f nil = NONE
	  in  f (HashTable sub i)
	  end
 
	val _ = 
	    (List.app add
	[("and",YAND),
	 ("array",YARRAY),
	 ("begin",YBEGIN),
	 ("case",YCASE),
	 ("const",YCONST),
	 ("div",YDIV),
	 ("do",YDO),
	 ("downto",YDOWNTO),
	 ("else",YELSE),
	 ("end",YEND),
	 ("extern",YEXTERN),
	 ("file",YFILE),
	 ("for",YFOR),
	 ("forward",YFORWARD),
	 ("function",YFUNCTION),
	 ("goto",YGOTO),
	 ("hex",YHEX),
	 ("if",YIF),
	 ("in",YIN),
	 ("label",YLABEL),
	 ("mod",YMOD),
	 ("nil",YNIL),
	 ("not",YNOT),
	 ("oct",YOCT),
	 ("of",YOF),
	 ("or",YOR),
	 ("packed",YPACKED),
	 ("procedure",YPROCEDURE),
	 ("program",YPROG),
	 ("record",YRECORD),
	 ("repeat",YREPEAT),
	 ("set",YSET),
	 ("then",YTHEN),
	 ("to",YTO),
	 ("type",YTYPE),
	 ("until",YUNTIL),
	 ("var",YVAR),
	 ("while",YWHILE),
	 ("with",YWITH)
	])
   end
   open KeyWord

%%

%s C B;
alpha=[A-Za-z];
digit=[0-9];
optsign=("+"|"-")?;
integer={digit}+;
frac="."{digit}+;
exp=(e|E){optsign}{digit}+;
octdigit=[0-7];
ws = [\ \t];
%%
<INITIAL>{ws}+	=> (lex());
<INITIAL>\n	=> (inc lineno; lex());
<INITIAL>{alpha}+ => (case find yytext of SOME v => v | _ => YID);
<INITIAL>{alpha}({alpha}|{digit})*  => (YID);
<INITIAL>{optsign}{integer}({frac}{exp}?|{frac}?{exp}) => (YNUMB);
<INITIAL>{optsign}{integer} => (YINT);
<INITIAL>{octdigit}+(b|B) => (YBINT);
<INITIAL>"'"([^']|"''")*"'" => (YSTRING);
<INITIAL>"(*" =>   (YYBEGIN C; lex());
<INITIAL>".."	=> (YDOTDOT);
<INITIAL>"."	=> (YDOT);
<INITIAL>"("	=> (YLPAR);
<INITIAL>")"	=> (YRPAR);
<INITIAL>";"	=> (YSEMI);
<INITIAL>","	=> (YCOMMA);
<INITIAL>":"	=> (YCOLON);
<INITIAL>"^"	=> (YCARET);
<INITIAL>"["	=> (YLBRA);
<INITIAL>"]"	=> (YRBRA);
<INITIAL>"~"	=> (YTILDE);
<INITIAL>"<"	=> (YLESS);
<INITIAL>"="	=> (YEQUAL);
<INITIAL>">"	=> (YGREATER);
<INITIAL>"+"	=> (YPLUS);
<INITIAL>"-"	=> (YMINUS);
<INITIAL>"|"	=> (YBAR);
<INITIAL>"*"	=> (YSTAR);
<INITIAL>"/"	=> (YSLASH);
<INITIAL>"{"	=> (YYBEGIN B; lex());
<INITIAL>.	=> (YILLCH);
<C>\n		=> (inc lineno; lex());
<C>[^()*\n]+	=> (lex());
<C>"(*"		=> (lex());
<C>"*)"		=> (YYBEGIN INITIAL; lex());
<C>[*()]	=> (lex());
<B>\n		=> (inc lineno; lex());
<B>[^{}\n]+	=> (lex());
<B>"{"		=> (lex());
<B>"}"		=> (YYBEGIN INITIAL; lex());

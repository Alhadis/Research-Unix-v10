(* Copyright 1989 by AT&T Bell Laboratories *)
type lexresult = Token.token
type lexarg = {comLevel : int ref, lineNum : int ref, complain : string->unit}
val eof = fn {comLevel,lineNum,complain} => 
           (if !comLevel>0 then complain "unclosed comment" else ();
            Token.EOF)
val charlist = ref (nil : string list)
fun addString (s:string) = charlist := s :: (!charlist)
fun makeInt s = revfold (fn (c,a) => a*10 + (ord c - Ascii.zero)) (explode s) 0
%% 
%s A S F;
%arg (arg as {comLevel,lineNum,complain});
idchars=[A-Za-z'_0-9];
id=[A-Za-z'_]{idchars}*;
qualid={id}".";
ws=[\t\ ]*;
sym=[!%&$+/:<=>?@~|#*`]|\\|\-|\^;
num=[0-9]+;
frac="."{num};
exp="E"(~?){num};
real=(~?)(({num}{frac}?{exp})|({num}{frac}{exp}?));
%%
<INITIAL>{ws}	=> (continue());
<INITIAL>\n	=> (inc lineNum; continue());
<INITIAL>"*"	=> (Token.ASTERISK);
<INITIAL>"|"	=> (Token.BAR);
<INITIAL>":"	=> (Token.COLON);
<INITIAL>"="	=> (Token.EQUAL);
<INITIAL>"_"	=> (Token.WILD);
<INITIAL>"#"	=> (Token.HASH);
<INITIAL>","	=> (Token.COMMA);
<INITIAL>"{"	=> (Token.LBRACE);
<INITIAL>"}"	=> (Token.RBRACE);
<INITIAL>"["	=> (Token.LBRACKET);
<INITIAL>"]"	=> (Token.RBRACKET);
<INITIAL>";"	=> (Token.SEMICOLON);
<INITIAL>"("	=> (Token.LPAREN);
<INITIAL>")"	=> (Token.RPAREN);
<INITIAL>"and"	=> (Token.AND);
<INITIAL>"abstraction"	=> (Token.ABSTRACTION);
<INITIAL>"abstype"	=> (Token.ABSTYPE);
<INITIAL>"->"		=> (Token.ARROW);
<INITIAL>"as"		=> (Token.AS);
<INITIAL>"case"		=> (Token.CASE);
<INITIAL>"datatype"	=> (Token.DATATYPE);
<INITIAL>"..."		=> (Token.DOTDOTDOT);
<INITIAL>"else"		=> (Token.ELSE);
<INITIAL>"end"		=> (Token.END);
<INITIAL>"eqtype"	=> (Token.EQTYPE);
<INITIAL>"exception"	=> (Token.EXCEPTION);
<INITIAL>"do"		=> (Token.DO);
<INITIAL>"=>"		=> (Token.DARROW);
<INITIAL>"fn"		=> (Token.FN);
<INITIAL>"fun"		=> (Token.FUN);
<INITIAL>"functor"	=> (Token.FUNCTOR);
<INITIAL>"handle"	=> (Token.HANDLE);
<INITIAL>"if"		=> (Token.IF);
<INITIAL>"in"		=> (Token.IN);
<INITIAL>"include"	=> (Token.INCLUDE);
<INITIAL>"infix"	=> (Token.INFIX);
<INITIAL>"infixr"	=> (Token.INFIXR);
<INITIAL>"let"		=> (Token.LET);
<INITIAL>"local"	=> (Token.LOCAL);
<INITIAL>"nonfix"	=> (Token.NONFIX);
<INITIAL>"of"		=> (Token.OF);
<INITIAL>"op"		=> (Token.OP);
<INITIAL>"open"		=> (Token.OPEN);
<INITIAL>"overload"	=> (Token.OVERLOAD);
<INITIAL>"raise"	=> (Token.RAISE);
<INITIAL>"rec"		=> (Token.REC);
<INITIAL>"sharing"	=> (Token.SHARING);
<INITIAL>"sig"		=> (Token.SIG);
<INITIAL>"signature"	=> (Token.SIGNATURE);
<INITIAL>"struct"	=> (Token.STRUCT);
<INITIAL>"structure"	=> (Token.STRUCTURE);
<INITIAL>"then"		=> (Token.THEN);
<INITIAL>"type"		=> (Token.TYPE);
<INITIAL>"val"		=> (Token.VAL);
<INITIAL>"while"	=> (Token.WHILE);
<INITIAL>"with"		=> (Token.WITH);
<INITIAL>"withtype"	=> (Token.WITHTYPE);
<INITIAL>"orelse"	=> (Token.ORELSE);
<INITIAL>"andalso"	=> (Token.ANDALSO);
<INITIAL>"import"	=> (Token.IMPORT);
<INITIAL>{qualid} =>
  (Token.IDDOT (Symbol.symbol(substring(yytext,0,size(yytext)-1))));
<INITIAL>"..."	=> (Token.DOTDOTDOT);
<INITIAL>"'"{idchars}+	=> (Token.TYVAR(Symbol.symbol yytext));
<INITIAL>({sym}+|{id})	=> (Token.ID(Symbol.symbol yytext));
<INITIAL>{real}	=> (Token.REAL yytext);
<INITIAL>{num}	=> (Token.INT(makeInt yytext
		    handle Overflow => (complain "integer too large"; 0)));
<INITIAL>~{num}	=> (Token.INT(~(makeInt(substring(yytext,1,size(yytext)-1)))
		    handle Overflow => (complain "integer too large"; 0)));
<INITIAL>\"	=> (charlist := nil; YYBEGIN S; continue());
<INITIAL>"(*"	=> (YYBEGIN A; comLevel := 1; continue());
<INITIAL>.	=> (complain "illegal token"; continue());
<A>"(*"		=> (inc comLevel; continue());
<A>\n		=> (inc lineNum; continue());
<A>"*)" => (dec comLevel; if !comLevel=0 then YYBEGIN INITIAL else (); continue());
<A>.		=> (continue());
<S>\"	        => (YYBEGIN INITIAL; Token.STRING(implode(rev(!charlist))));
<S>\n		=> (complain "unclosed string"; YYBEGIN INITIAL; Token.STRING "");
<S>[^"\\\n]*	=> (addString yytext; continue());
<S>\\\n	       	=> (inc lineNum; YYBEGIN F; continue());
<S>\\[\ \t]   	=> (YYBEGIN F; continue());
<F>\n		=> (inc lineNum; continue());
<F>{ws}		=> (continue());
<F>\\		=> (YYBEGIN S; continue());
<F>.		=> (complain "unclosed string"; YYBEGIN INITIAL; Token.STRING "");
<S>\\t		=> (addString "\t"; continue());
<S>\\n		=> (addString "\n"; continue());
<S>\\\\		=> (addString "\\"; continue());
<S>\\\"		=> (addString(chr(Ascii.dquote)); continue());
<S>\\\^[@-_]	=> (addString(chr(ordof(yytext,2)-ord("@"))); continue());
<S>\\[0-9]{3}	=>
 (let val x = ordof(yytext,1)*100
	     +ordof(yytext,2)*10
	     +ordof(yytext,3)
	     -(Ascii.zero*111)
  in (if x>255
      then complain ("illegal ascii escape '"^yytext^"'")
      else addString (chr x);
      continue())
  end);
<S>\\		=> (complain "illegal string escape"; continue());


(* November 1988, Jussi Rintanen, Helsinki University of Technology *)

(* This is the specification of ML-Twig lexical analyzer.

   This lexer correctly recognizes all Standard ML tokens, as specified
   in [Harper, MacQueen and Milner, 1986].
*)

datatype lexresult =
      IDENTIFIER of string
      | INT of string
      | EQ
      | RPAREN
      | LPAREN
      | COLON
      | SEMICOLON
      | COMMA
      | TREEREF of int list
      | OTHER of string
      | SPACE of string
      | EOF

local
  fun digit c = ("0" <= c) andalso (c <= "9")
  fun str2 (a,c::r) =
    if digit c
      then str2 (a*10 + ord c - ord "0" ,r)
    else (a,c::r)
    | str2 r = r
in
  fun str0int s = str2 (0,s)
end

local
  fun parse_treeref' nil = nil
    | parse_treeref' ["$"] = nil
    | parse_treeref' s =
      let val (i,r) = str0int s
      in
	i :: parse_treeref' (tl r)
      end
in  
  val parse_treeref = (parse_treeref' o tl o explode)
end

val current_line_number : int ref = ref 1
val commentlevel : int ref = ref 0

fun current_line () = !current_line_number

fun eof() = EOF

%%

%structure TwigLexer

%s C;

idbegin=[a-zA-Z'_];
idchar=[a-zA-Z0-9'_];

sidbegin=[!%&$+/:<=>?@~'|*\\^-];
sidchar=[!%&$+/:<=>?@~'|#*\\^-];

digit=[0-9];
whitespace=[\ \t];
newline=[\n];
controllable=[a-zA-Z];

%%

<INITIAL>"$" ({digit}+("."{digit}+)*)? "$" => (TREEREF (parse_treeref yytext));

<INITIAL>{digit}+ => (INT yytext);

<INITIAL>{digit}+("."{digit}+)?(E"~"?{digit}+)? => (OTHER yytext);

<INITIAL>{sidbegin}{sidchar}* => (case yytext of
      	      	      	  ":" => COLON
      	      	      	  | "=" => EQ
      	      	      	  | _ => OTHER yytext);

<INITIAL>{idbegin}{idchar}* => (IDENTIFIER yytext);

<INITIAL>(({idbegin}{idchar}*) | ({sidbegin}{sidchar}*))
("."(({idbegin}{idchar}*) | ({sidchar}+)))+
                    => (OTHER yytext);

<INITIAL>"(" => (LPAREN);
<INITIAL>")" => (RPAREN);
<INITIAL>";" => (SEMICOLON);
<INITIAL>"," => (COMMA);
<INITIAL>"]" => (OTHER yytext);
<INITIAL>"[" => (OTHER yytext);    
<INITIAL>"..." => (OTHER yytext);
<INITIAL>"#"{digit}+ => (OTHER yytext);

<INITIAL>\"((\\([nt\\"]|([0-9]{3})|("^"[a-zA-Z])|([\ \t\n]+\\)))|[^\n\\"])*\"
 => (let val dummy = (current_line_number :=
		      fold (fn (a,b) => b+(if a="\n" then 1 else 0))
		      (explode yytext) (!current_line_number))
	in
	  OTHER yytext
	end);

<INITIAL>"{"|"}"|"_" => (OTHER yytext);

<INITIAL>{whitespace}+ => (SPACE yytext);

<INITIAL>{newline} => ( inc current_line_number; SPACE yytext);
							   
<INITIAL>"(*" => ( YYBEGIN C; inc commentlevel; SPACE yytext );

<C>"(*" => ( inc commentlevel; SPACE yytext );
<C>"*)" => ( dec commentlevel; if !commentlevel = 0 then YYBEGIN INITIAL else (); SPACE yytext );
<C>{newline} => ( inc current_line_number; SPACE yytext );
<C>"*"{newline} => ( inc current_line_number; SPACE yytext );
<C>(("("[^*\n])|[^(*\n]|("*"[^)\n]))+ => ( SPACE yytext );

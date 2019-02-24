.so ../ADM/mac
.XX 22 359 "Yacc: A Parser Generator"
.\".nr PD 1u	\" bwk:  little bit of paddability
.\".nr PI 2n
.rn SH sH
.fp 9 CB CW
.de SH
.SP 1.5
.sH
\\$1.  \\$2
.PP
.nr H5 0
.nr H4 0
.nr H3 0
.nr H2 0
.nr H1 \\$1
..
.de SS
.NH 2
\\$1
.PP
..
.de CB
.nr Ft \\n(.f
\&\\$3\&\\f(CB\\$1\&\\f\\n(Ft\\$2
..
.de Q{
.in +20p
.X{ \\$1
..
.de P{
.Q{ \\$1
.tr _\(ru
.ft CB
.lg 0
..
.de Q}
.X} \\$1
.in -20p
..
.de P}
.lg
.Q} \\$1
..
.de X{
.KS
.lg 0
.ie \\n(.$ .SP \\$1
.el .SP .5
.nf
.ft 1
.nr t 20p
.ta \\ntu +\\ntu +\\ntu +\\ntu +\\ntu +\\ntu +\\ntu +\\ntu +\\ntu +\\ntu +\\ntu +\\ntu +\\ntu +\\ntu +\\ntu
..
.de X}
.ft 1
.fi
.lg
.ie \\n(.$ .SP \\$1
.el .SP .5
.KE
..
.de FI
.LP
.DS B
.fi
.nr PS 9
.ps 9
\\f3Figure \\$1\\$2.\\f1
..
.\"		EF - end figure
.de EF
.DE
.fi
.nr PS 10
.ps 10
.if \\n(.$ .SP \\$1
.if !\\n(.$ .SP
.LP
..
.PS
	arrowht = .08; arrowwid = .04	# default values are .1 and .05
	vs = 1/6			# 1/6 = 12p; for vertical distances
	pagewid = 6; fillval = .98

	# ln --		line to $1, $2 chop elliptically
	#		chopw and choph are the chop ellipse width and height
	define ln %
	 {	[
			x = $1*hu; y = $2*vu
			xsq = x*x
			rsq = xsq + y*y; r = sqrt(rsq);
			asq = chopw*chopw/4;
			bsq = choph*choph/4
			esq = (asq - bsq)/asq
			p = sqrt(bsq/(1-esq*(xsq/rsq)))/r;

		O:	""
		B:	O + p*x, p*y
		E:	O + (1-p)*x, (1-p)*y
		P:	O + x,y
		line $3 from B to E
		] with .O at Here
	}
	move to Here + $1*hu, $2*vu
	%
	# usage: adirection([e|n|w|s], [right|up|...], [1|0|1|0], Object)
	define adirection %
		axdir = $3
		move to $4
		$2
	%
	define aright % adirection(e, right, 1, $1) %
	define aleft  % adirection(w, left,  1, $1) %
	define anup   % adirection(n, up,    0, $1) %
	define adown  % adirection(s, down,  0, $1) %
	define anell %
		ax = $1.x - Here.x; ay = $1.y - Here.y
	Anell:	Here + ax,ay
		if axdir then|
			line $3 from Here to Here + ax,0 chop 0 chop arcrad
			if ax*ay > 0 then@ arc @else@ arc cw @
			axdir = 0
		|else|
			line $3 from Here to Here + 0,ay chop 0 chop arcrad
			if ax*ay < 0 then@ arc @else@ arc cw @
			axdir = 1
		|
		line $2 to Anell
	% axdir = 1
.PE
.fp 8 SS S
.EQ
define Small	% size 7 "$1" %
define f2	% "\&" font 2 "$1" %
define cdot	% "\s7\f8\N'183'\fP\s0" %
define =>	% "\v'-.15m'\f8\N'222'\fP\v'.15m'" %
define C++	% "\f1C\h'-.14m'+\h'-.18'+\fP" %
define <!	% "\f8\N'225'\fP" %
define >!	% "\f8\N'241'\fP" %
delim $$
.EN
.ND "July 1, 1989"
.TL
Yacc: A Parser Generator\(dg
.AU
Stephen C. Johnson
Ravi Sethi
.AI
.MH
.AB
Since the early 1970s, Yacc has been used to implement
hundreds of languages, big and small.
Its applications range from small desk calculators
to medium-sized preprocessors for typesetting
to large compiler front ends for complete programming languages.
.PP
A Yacc specification is based on a collection of grammar rules
describing the syntax of a language; Yacc turns the
specification into a syntax analyzer.
A pure syntax analyzer merely checks whether or not an input string
conforms to the syntax of the language.
.PP
We can go beyond pure syntax analysis by attaching code in C
or $C++$ to
a grammar rule; such code is called an action, and is executed
whenever the rule is applied during syntax analysis.
Thus, a desk calculator might use actions to evaluate an expression,
and a compiler front end might use actions to
emit intermediate code.
.PP
Yacc allows us to build parsers from LALR(1) grammars
without necessarily learning the underlying theory.
.AE
.@tag SH _SH1_
.FS
\(dg Prepared by R. Sethi from\&|reference(v7yacc)\&.
S. C. Johnson is presently with Ardent Computer,
880 West Maude Ave.,
Sunnyvale, California 94086.
.FE
.SH _SH1_  "Introduction"
.LP
Yacc is a tool for building syntax analyzers, also known as
.I parsers .
This section introduces the basic features of Yacc.
We review grammars, build a pure parser
for real numbers, and then augment the parser to evaluate
numbers during parsing.
The language of real numbers is a toy;
realistic examples appear in Section _SH2_.
.PP
Uppercase letters are distinct from lowercase letters
in Yacc specifications.
Thus,
.CW digit
and
.CW DIGIT
are distinct names.
The font of a name is chosen purely for readability, so
.CW fraction ,
and $fraction$ (within diagrams)
refer to the same name.
.SS "Further Reading"
Yacc is designed to handle a single but significant part
of the total job of building a translator or interpreter
for a language.
The remaining parts of the job must be implemented
in a host programming language,
presumed to be C|reference(Kernighan Ritchie 1988) or
$C++$|reference(Stroustrup book 1986).
.PP
Another progam generator, designed to work in harmony with Yacc,
is Lex|reference(latest lex), a tool developed for
building lexical analyzers.
Lex can be easily used to produce quite complicated lexical analyzers,
but there remain some languages (such as Fortran) that do not
fit any theoretical framework, and whose lexical analyzers
must be crafted by hand.
.PP
Kernighan and Pike illustrate program development
using Yacc and Lex by gradually extending an expression evaluator
into an interpreter for a language comparable to Basic|reference(Kernighan Pike).
Schreiner and Friedman conduct a book-length case
study of how to create a compiler using Yacc and Lex|reference(Schreiner Friedman).
.PP
Textbooks on compilers often provide more information on the behavior and
construction of parsers than will be covered here; see for example
\&|reference(Aho Sethi Ullman 1986)\&.
The algorithms underlying Yacc are also discussed in the survey
of LR parsing by
\&|reference(Aho Johnson surveys)\&.
A feature that sets Yacc apart \(em its ability to build fast compact
LR parsers from ambiguous grammars \(em is based on the theory
developed by
\&|reference(Aho Johnson Ullman ambiguous)\&.
.PP
Among the earliest applications of Yacc are
EQN|reference(Kernighan Cherry 1975), a language for typesetting mathematics, and
PCC, the Portable C Compiler|reference(Johnson portable acm).
.SS "Grammars, Reviewed"
The
.I syntax
of a language imposes a hierarchical structure,
called a
.I "parse tree" ,
on strings in the language.
The following is a parse tree for the string
.CW 3.14
in a language of real numbers:
.KS
.ps 9
.ft CB
.PS
[
	hu = .5; vu = 1.5*vs; choph = vs; chopw = .7
	boxht = vs; boxwid = .8*vs

	"$realNumber$"
	 {
		ln(-1,-1); "$integerPart$"
		ln( 0,-1); "$digit$"
		ln( 0,-2); "3"
	}{
		ln( 0,-4); "."
	}{
		ln( 1.5,-1); "$fraction$"
		 {
			ln(-0.5,-1); "$digit$"
			ln( 0,-2); "1"
		}{
			ln( 0.5,-1); "$fraction$"
			ln( 0,-1); "$digit$"
			ln( 0,-1); "4"
		}
	}
]
.PE
.KE
.PP
The leaves at the bottom of a parse tree are labeled with
.I terminals
or
.I tokens ;
tokens represent themselves.
By contrast, the other nodes of a parse tree are labeled with
.I nonterminals .
Each node in the parse tree is based on a rule
that defines a nonterminal in terms of a sequence of
terminals and nonterminals.
Such rules are called
.I productions .
The root of the preceding parse tree is based on the
following informally stated production:
.Q{
A real number consists of an integer part, a point, and a fraction.
.Q}
This production is written as follows in the notation accepted by Yacc:
.P{
realNumber :  integerPart '.' fraction
           ;
.P}
.PP
Together, the tokens, the nonterminals, the productions, and a
distinguished nonterminal, called the
.I "start symbol" ,
constitute a
.I grammar
for a language.
Both tokens and nonterminals can be referred to as
.I "grammar symbols" ,
or simply
.I symbols .
.SS "Grammars in Yacc Specifications"
A Yacc specification has three sections for
optional declarations, productions, and optional user-supplied programs.
The productions are the heart of a specification; they are
darker for emphasis in Figure _FI2_.
The sections are separated by
double percent
.CW %%
marks \(em the percent symbol
is generally used by Yacc as an escape character.
If the programs section is omitted, the second
.CW %%
mark can be omitted as well.
.KF bottom
.ps 9
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.PS
[
	define text % [
	B:	box ht vs wid pagewid invis
		$1 ljust at B.w + 20/72,0
	] %
 down
.ft CW
 text("%start lines")
 text("%%")
.ft CB
 text("lines       : /* empty */")
 text("            | lines realNumber '\en'")
 text("            ;")
 text("realNumber  : integerPart '.' fraction")
 text("            ;")
 text("integerPart : digit")
 text("            | integerPart digit")
 text("            ;")
 text("fraction    : digit")
 text("            | digit fraction")
 text("            ;")
 text("digit       : '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'")
 text("            ;")
.ft CW
 text("%%")
 text("#include <stdio.h>")
 text("int  yylex() { return getchar(); }")
 text("int  main()  { return yyparse(); }")
 text("void yyerror(s) char *s; { fprintf(stderr, \"%s\en\", s); }")
]
.PE
.@tag FI _FI2_
.FI _FI2_
A complete Yacc specification for sequences of real numbers, one per line.
.EF 0
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.SP
.KE
.PP
Blanks, tabs, and newlines are ignored.
Comments can appear wherever a name can; they are enclosed
between
.CW /*
and
.CW */ ,
as in C.
.PP
It is possible, and desirable, for the start symbol of a grammar
to be declared explicitly, using the
.CW %start
keyword, as in
.P{
%start lines
.P}
Otherwise, the start symbol is taken from the first production in the
specification.
.PP
A name in the production section is presumed to represent a nonterminal
unless it is explicitly declared to represent a token.
There are no token declarations in Figure _FI2_;
later in this section,
.CW DIGIT
will be declared to be a token by writing
.P{
%token DIGIT
.P}
.PP
Single-character tokens need not be declared; a
.I literal
is a character enclosed in single quotes.
As in C, the backslash
.CW \e
is an escape character within literals, and the following C escapes are
recognized:
.ft CB
.TS
lw4 0 l l.
	'\en'	\f1newline\fP
	'\er'	\f1return\fP
	'\e''	\f1single quote \fP'\f1\fP
	'\et'	\f1tab\fP
	'\eb'	\f1backspace\fP
	'\ef'	\f1form feed\fP
	'\exxx'	\f1``\fPxxx\f1'' in octal
.TE
For technical reasons, the
.CW NUL
character,
.CW '\e0'
or
.CW 0 ,
should never be used in productions.
.PP
A production
.P{
realNumber :  integerPart '.' fraction
           ;
.P}
defines a nonterminal, called its
.I "left side" ,
in terms of a sequence of grammar symbols, called its
.I "right side" .
A colon separates the two sides, and a semicolon marks the end
of the production.
The left side of this production is
.CW realNumber .
Its right side consists of
.CW integerPart ,
the literal
.CW '.' ,
and
.CW fraction .
.PP
The right side of a production can be empty, as in the following
production with no symbols between the colon and the
terminating semicolon:
.P{
lines : ;
.P}
.PP
Productions with the same left side can be combined, and written with
a vertical bar separating the right sides.
The productions
.P{
integerPart :  digit
            ;
integerPart :  integerPart digit
            ;
.P}
can be rewritten equivalently as
.P{
integerPart :  digit
            |  integerPart digit
            ;
.P}
In words, an integer part is either a single digit or a
(smaller) integer part followed by a digit.
Thus,
an integer part consists of
a string of one or more digits.
.PP
A fraction also consists of a string of one or more digits;
however, the productions for
.CW fraction
impose a different hierarchical
structure on strings of digits.
The productions are
.P{
fraction :  digit
         |  digit fraction
         ;
.P}
Note how a tree for
.CW integerPart
grows down to the left, whereas a tree for
.CW fraction
grows down to the right:
.KS
.ps 9
.ft CB
.PS
[
	hu = .5; vu = 1.5*vs; choph = vs; chopw = .7
	boxht = vs

IntegerPart:\
	[
		"$integerPart$"
		{
			ln( 1,-1); "$digit$"
			ln( 0,-1); "3"
		}
		ln(-1,-1); "$integerPart$"
		{
			ln( 1,-1); "$digit$"
			ln( 0,-1); "2"
		}
		ln(-1,-1); "$integerPart$"
		{
			ln( 0,-1); "$digit$"
			ln( 0,-1); "1"
		}{
			box invis wid .8 at Here
		}
	]
Fraction:\
	[
		"$fraction$"
		{
			ln(-1,-1); "$digit$"
			ln( 0,-1); "7"
		}
		ln( 1,-1); "$fraction$"
		{
			ln(-1,-1); "$digit$"
			ln( 0,-1); "8"
		}
		ln( 1,-1); "$fraction$"
		{
			ln( 0,-1); "$digit$"
			ln( 0,-1); "9"
		}{
			box invis wid .6 at Here
		}
	] with .e at IntegerPart.w + pagewid-2*20/72,0
]
.PE
.KE
.LP
The different hierarchical structures imposed by
.CW integerPart
and
.CW fraction
on strings of digits facilitate the evaluation of real numbers,
as we shall see later in this section.
.SS "Using Yacc"
Parsers generated by Yacc need to be supplemented
before a self-contained application is built.
Only four lines are devoted to such supplementary
code at the end of Figure _FI2_.
In other words,
Yacc confines itself to generating fast parsers, leaving
us with both the control and responsibility
for all other aspects of an application.
.PP
The use of Yacc is illustrated in Figure _FI3_.
The code of a Yacc-generated parser is a function,
.CW yyparse ,
which returns 0
if the entire input is parsed successfully;
otherwise, it returns 1.
The function
.CW yyparse
gets tokens by repeatedly calling
.CW yylex ,
a lexical analyzer.
.KF bottom
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.nr PS 9
.ps 9
.vs 11
.PS
[
	fillval = 1
	boxht = 2.5*vs; boxwid = .6
	lineht = vs; linewid = .25

	right
	box invis "character" "stream" wid .7
	line ->
	box "lexical" "analyzer" fill
	line ->
	box invis "token" "stream"
	line ->
	box "syntax" "analyzer" fill
	{
		move to last box.n
		up
		line <-
		box "C" "compiler"
		line <-
		box "Yacc" fill
		line <-
		box invis ht 1.5*vs "grammar"
	}
	line ->
	box invis "optional" "output"
]
.PE
.@tag FI _FI3_
.FI _FI3_
Yacc handles the syntax analysis part of an application.
.EF 0
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.SP
.KE
.PP
A
.I "lexical analyzer"
reads input characters and returns tokens.
The simplest lexical analyzer returns each individual character
as a token; it is defined by
.P{
int yylex() { return getchar(); }
.P}
.PP
Execution of a C program begins in a function called
.CW main ,
so
.CW main
must call
.CW yyparse.
The code for
.CW main
in Figure _FI2_
.P{
int main() { return yyparse(); }
.P}
simply returns the result obtained from
.CW yyparse ;
more generally,
.CW main
might read command-line arguments and options
before calling
.CW yyparse .
.PP
If a syntax error occurs during parsing,
.CW yyparse
calls a user-supplied function
.CW yyerror
with a terse message, usually
.CW "syntax error" .'' ``
The following implementation of
.CW yyerror
prints the message :
.P{
void yyerror(s) char *s; { fprintf(stderr, "%s\en", s); }
.P}
Parsing terminates when an error is detected unless ``error productions''
are used as described in Section _SH5_.
.PP
We have now examined all the pieces of the specification in
Figure _FI2_ except
.P{
#include <stdio.h>
.P}
which provides access to input/output facilities from a
standard library. 
.PP
When Yacc is applied to a specification, the output
is a file of C programs, called
.CW y.tab.c
on most systems
(the name might differ due to local file-system conventions).
Suppose that the specification in Figure _FI2_ appears in a file
.CW real.y .
A program for reading a sequence of real numbers can then be
constructed by the following
.UX
system commands:
.ft CB
.TS
lw4 0 l8 l.
	yacc real.y	\f2generates C code into file \&\fPy.tab.c
	cc y.tab.c	\f2compiles executable program into file \&\fPa.out
.TE
.LP
The compiled program
.CW a.out
silently reads real numbers, one per line, until the end of the input
is reached, or until a syntax error occurs.
The specification will now be extended to evaluate real numbers
as they are read.
.EQ
delim off
.EN
.SS "Actions and Attributes"
An action attached to a production is
executed each time the production is applied during parsing.
An
.I action
consists of one or more C statements, enclosed in curly braces
.CW {
and
.CW } .
Within an action, pseudo-variables starting with
.CW $
signs refer to values associated with the symbols in the production.
The pseudo-variable for the left side is
.CW $$ ;
the pseudo-variable for a symbol on the right side
is formed by prefixing a
.CW $
sign to its name or to its position.
.PP
The action in
.P{
realNumber : integerPart fraction { $$ = $integerPart + $fraction; } ;
.P}
is the single statement
.P{
$$ = $integerPart + $fraction;
.P}
.EQ
delim @@
.EN
It defines the value associated with the left side to be the sum of the
values associated with the two symbols on the right side.@"" sup _FS#_@
.FS
.@tag FS _FS#_
@"" sup _FS#_@
When the same symbol @s@ appears several times on the right side,
its pseudo-variable can be written as
.CW $@s@#1 ,
.CW $@s@#2 ,
.CW $@s@#3 .
.FE
Using pseudo-variables formed from positions on the right side, instead of
names, the action can be rewritten equivalently as
.EQ
delim off
.EN
.P{
realNumber :  integerPart fraction  { $$ = $1 + $2; }  ;
.P}
This production and action are from the complete specification
in Figure _FI2A_.
.PP
Earlier versions of Yacc support only positional pseudo-variables
like
.CW $1
and
.CW $2 .
.PP
The type of the values associated with grammar symbols is given by
.CW YYSTYPE ,
defined to be
.CW double
in Figure _FI2A_.
The default type for
.CW YYSTYPE
is
.CW int .
Any C code in the declarations section must be enclosed between
.CW %{
and
.CW %} .
See Section _SH6_ for how to associate different types of values with
different grammar symbols.
.KF
.ps 9
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.PS
[
	define text % [
	B:	box ht vs wid pagewid invis
		$1 ljust at B.w + 20/72,0
	] %
 down
.ft CW
  text("%token DIGIT")
  text("%start lines")
  text("%{")
  text("#def\&ine YYSTYPE double")
  text("%}")
  text("%%")
.ft CB
  text("lines       : lines realNumber '\en'")
.ft CW
  text("                { printf(\"%g\en\", $realNumber); }")
.ft CB
  text("            | /* empty */")
  text("            ;")
  text("realNumber  : integerPart '.' fraction")
.ft CW
  text("                { $$ = $integerPart + $fraction; }")
.ft CB
  text("            ;")
  text("integerPart : DIGIT")
  text("            | integerPart DIGIT")
.ft CW
  text("                { $$ = $integerPart*10 + $DIGIT; }")
.ft CB
  text("            ;")
  text("fraction    : DIGIT")
.ft CW
  text("                { $$ = $DIGIT*0.1; }")
.ft CB
  text("            | DIGIT fraction")
.ft CW
  text("                { $$ = ($DIGIT + $fraction)*0.1; }")
.ft CB
  text("            ;")
.ft CW
  text("%%")
  text("#include <stdio.h>")
  text("#include <ctype.h>")
  text("int  yylex()   {")
  text("    int c;")
  text("    c = getchar();")
  text("    if( ! isdigit(c) ) return c;")
  text("    yylval = c - '0';")
  text("    return DIGIT;")
  text("}")
  text("int  main() { return yyparse(); }")
  text("void yyerror(s) char *s; { fprintf(stderr, \"%s\en\", s); }")
]
.PE
.@tag FI _FI2A_
.FI _FI2A_
The actions in this Yacc specification evaluate real numbers during parsing.
.EF 0
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.SP
.KE
.PP
The tree in Figure _FI2C_ illustrates the evaluation of the real number
.CW 321.789 .
(The tree is technically not a parse tree because its root is not
for the start symbol
.CW lines ,
and the nodes are labeled with values rather than grammar symbols.)
.KF
.EQ
delim $$
.EN
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.ps 9
.ft CB
.PS
[
	define intp % {
		"$f2($integerPart)^=^$1$"
	} %
	define frac % {
		"$f2($fraction)^=^$1$"
	} %
	define DIGIT % {
		"$Small($DIGIT)^=^$1$"
	} %

	hu = .6; vu = 2.5*vs; choph = vs; chopw = .7
	boxht = vs

	"$f2($realNumber)^=^321.789$"
	{
		ln(-2,-1); intp(321)
		{
			ln( 1,-1); DIGIT(1)
		}
		ln(-1,-1); intp(32)
		{
			ln( 1,-1); DIGIT(2)
		}
		ln(-1,-1); intp(3)
		{
			ln( 0,-1); DIGIT(3)
		}{
			box invis wid .85 at Here
		}
	}{
		ln( 2,-1); frac(0.789)
		{
			ln(-1,-1); DIGIT(7)
		}
		ln( 1,-1); frac(0.89)
		{
			ln(-1,-1); DIGIT(8)
		}
		ln( 1,-1); frac(0.9)
		{
			ln( 0,-1); DIGIT(9)
		}{
			box invis wid .8 at Here
		}
	}
]
.PE
.@tag FI _FI2C_
.FI _FI2C_
Attribute values during the evaluation of
.CB 321.789 .
.EF 0
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.SP
.EQ
delim off
.EN
.KE
.PP
A value associated with a node in a parse tree is called an
.I attribute .
For the moment, the attribute at a node will be defined solely in
terms of the attributes at the children of the node.
.PP
Attributes for tokens are defined by the lexical analyzer.
Conceptually, a lexical analyzer returns a pair, consisting
of a token and an associated attribute value.
For example, in Figure _FI2A_,
.CW DIGIT
is a token.
When the lexical analyzer reads the character
.CW 1
it returns
.CW DIGIT
with attribute value 1, when it reads
.CW 2
it returns
.CW DIGIT
with attribute value 2, and so on.
.PP
Specifically, the parser
.CW yyparse
expects the lexical analyzer
.CW yylex
to leave the attribute value in a global variable
.CW yylval .
The function
.CW yylex
in Figure _FI2A_ assigns a value to
.CW yylval
just before it returns the token
.CW DIGIT .
.PP
Starting in the bottom-left corner of Figure _FI2C_,
the lexical analyzer sets the attribute value 3 at the
leftmost leaf for the token
.CW DIGIT .
The parent of this leaf is based on the production and action
.P{
integerPart :  DIGIT  { $$ = $1; }  ;
.P}
This action is omitted in Figure _FI2A_ because, by default,
the parser sets the attribute of the left side to that of the
first symbol on the right side.
.PP
Working up the tree, the next node is based on
.P{
integerPart :  integerPart DIGIT  { $$ = $integerPart + $DIGIT; }  ;
.P}
.PP
The effect of the actions is perhaps easier to see at the nodes
for
.CW fraction .
At the only node based on
.P{
fraction :  DIGIT  { $$ = $DIGIT*0.1; }  ;
.P}
the value of
.CW $DIGIT
is 9 and the value of
.CW $fraction
is 0.9.
.PP
Since Yacc generates bottom-up parsers, bottom-up evaluation of
attribute values fits naturally with parsing.
Actions attached to productions are examined further in Section _SH2_;
their execution order becomes significant when
they do input/output, assign values to variables, call functions
with side effects, or otherwise affect the state of a computation.
.SS "A Style for Specifications"
Choose a style that makes the productions
visible through the morass of action code.
.SP .5
.IP a. 4
Use all capital letters for token names, all lower case letters for
nonterminal names.
This hint comes under the heading of ``knowing who to blame when
things go wrong.''
.SP .25
.IP b.
Put productions and actions on separate lines.
Either can then be changed independently.
.SP .25
.IP c.
Put all productions with the same left side together.
Put the left side in only once, and let all
following productions begin with a vertical bar.
.SP .25
.IP d.
Put a semicolon only after the last production with a given left side,
and put the semicolon on a separate line.
New productions can then be easily added.
.SP .25
.IP e.
Indent production bodies by one tab stop, and action bodies by two
tab stops.
.PP
These style hints owe much to Brian Kernighan.
The examples in this paper sometimes deviate from this style to conserve space.
.EQ
delim $$
.EN
.@tag SH _SH2_
.SH _SH2_  "Evaluation And Translation Of Expressions"
Both actions and productions must be considered
when a Yacc specification is designed.
Without actions, a parser would silently analyze input strings,
complaining only if it detects an error.
With suitable actions, a parser can become an evaluator or
a translator.
Typically, the desired actions
influence the choice of productions.
.PP
For example, the actions for evaluating the integer and fractional
parts of a real number motivate different productions for
the sequences of digits
represented by
.CW integerPart
and
.CW fraction
in Section _SH1_.
In the integer part, the contribution of a digit depends on the
number of digits to its right; the contribution of
.CW 3
in
.CW 321.789
is 300, where the number of zeros depends on the number of
digits to its right.
In the fractional part, however, the contribution of a digit
depends on the number of digits to its left; the contribution
of
.CW 9
in
.CW 321.789
is 0.009, where the number of zeros depends on the number of
digits to its left.
.PP
Arithmetic expressions are a fertile source of examples for Yacc
because the specifications of expression evaluators and translators are
quite short and the ideas carry over to richer languages.
This section begins with a specification for an
expression evaluator.
The evaluator benefits from Yacc's facilities for
specifying the associativity and precedence of operators
within expressions.
The next example, a translator from infix into postfix notation,
illustrates parsing order.
.SS "Grammars for Expressions"
Expressions are characterized by the operators within them;
the choice of productions for expressions
depends on the associativity and precedence of operators.
.PP
An operator
.CW OP
is
.I "left associative"
if an expression
.P{
expr$"" sub 1$ OP expr$"" sub 2$ OP expr$"" sub 3$
.P}
is evaluated as if it were parenthesized as
.P{
(expr$"" sub 1$ OP expr$"" sub 2$) OP expr$"" sub 3$
.P}
Similarly, the operator is
.I "right associative"
if the expression is evaluated as if it were parenthesized as
.P{
expr$"" sub 1$ OP (expr$"" sub 2$ OP expr$"" sub 3$)
.P}
.PP
An operator
.CW OPA
has
.I "lower precedence"
than an operator
.CW OPB
if the following equivalences hold (that is, the expressions to the
left and right of the $==$ signs have the same values):
.ft CB
.TS
lw4 0 r2 c2 l.
	expr$"" sub 1$ OPA expr$"" sub 2$ OPB expr$"" sub 3$	$==$	expr$"" sub 1$ OPA (expr$"" sub 2$ OPB expr$"" sub 3$)
	expr$"" sub 1$ OPB expr$"" sub 2$ OPA expr$"" sub 3$	$==$	(expr$"" sub 1$ OPB expr$"" sub 2$) OPA expr$"" sub 3$
.TE
.PP
The traditional grammar for arithmetic expressions uses three nonterminals
.CW expr ,
.CW term ,
and
.CW factor ,
where
.CW expr
represents an expression and
.CW term
and
.CW factor
represent subexpressions:
.P{
%token NUMBER VAR
%%
expr   :  expr '+' term  |  expr '-' term  |  term
       ;
term   :  term '*' factor  |  term '/' factor  |  factor
       ;
factor :  NUMBER  |  VAR  |  '(' expr ')'
       ;
.P}
In words, an expression is a sequence of terms separated
by
.CW +
or
.CW -
signs.
A term is a sequence of factors separated by
.CW *
or
.CW /
signs.
Thus,
.P{
b*b - 4*a*c
.P}
is an expression containing two terms
.CW b*b
and
.CW 4*a*c .
The term
.CW 4*a*c
has three factors
.CW 4 ,
.CW a ,
and
.CW c .
A factor is either a number, a variable, or a parenthesized expression.
.PP
Ths grammar for expressions
dates back to 1960 when Backus\&|reference(Backus 1960 %no_cite)\&
introduced BNF, a notation for writing grammars.
.PP
A desk calculator can be based on this grammar
by adding actions, as in
.EQ
delim off
.EN
.P{
expr :  expr '-' term  { $$ = $expr - $term; }
.P}
.EQ
delim $$
.EN
This production and action respect the left associativity of
the minus operator, and correctly evaluate
.CW 7-1-2
to 4 \(em check it by drawing a parse tree.
.PP
The traditional grammar generalizes to operators at $n^>=^1$ precedence levels;
all operators at the same level have the same associativity and precedence.
Set up a nonterminal
.CW expr$"" sub i$
for precedence level $i$, with level $1$ being the lowest.
In the traditional grammar, the nonterminals
.CW expr ,
.CW term ,
and
.CW factor
correspond to
.CW expr$"" sub 1$ ,
.CW expr$"" sub 2$ ,
and
.CW expr$"" sub 3$ ,
respectively.
If the operators at level $i^<^n$ are left associative,
then the productions for
.CW expr$"" sub i$
have the form
.P{
expr$"" sub i$ :  expr$"" sub i$ OP expr$"" sub i+1$  ;
.P}
Here,
.CW OP
represents an operator at level $i$.
Otherwise, if the operators at level $i$ are right associative,
then the productions have the form
.P{
expr$"" sub i$ :  expr$"" sub i+1$ OP expr$"" sub i$  ;
.P}
.PP
At each precedence level $i^<^n$, there is an additional production of the
form
.P{
expr$"" sub i$ :  expr$"" sub i+1$  ;
.P}
.SS "Associativity and Precedence Declarations"
Yacc has special facilities for declaring the associativity and
precedence of operators.
They will be introduced by considering the
specification in Figure _PREC_.
.KF
.EQ
delim off
.EN
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.ps 9
.ft CB
.PS
[
	define text @ [
	B:	box ht vs wid pagewid invis
		$1 ljust at B.w + 20/72,0
	] @
 down
 text("%{")
 text("#def\&ine YYSTYPE double")
 text("%}")
 text("%token NUMBER")
 text("%left '+' '-'")
 text("%left '*' '/'")
 text("%right '^'")
 text("%left '~' UMINUS")
 text("%%")
 text("lines :  lines expr '\en'        { printf(\"%g\en\", $expr); }")
 text("      |  lines '\en'")
 text("      |  /* empty */")
 text("      ;")
 text("expr  :  expr '+' expr          { $$ = $1 + $3; }")
 text("      |  expr '-' expr          { $$ = $1 - $3; }")
 text("      |  expr '*' expr          { $$ = $1 * $3; }")
 text("      |  expr '/' expr          { $$ = $1 / $3; }")
 text("      |  '-' expr %prec UMINUS  { $$ = - $expr; }")
 text("      |  '~' expr               { $$ = - $expr; }")
 text("      |  expr '^' expr          { $$ = pow($1, $3); }")
 text("      |  '(' expr ')'           { $$ = $expr; }")
 text("      |  NUMBER")
 text("      ;")
]
.PE
.@tag FI _PREC_
.FI _PREC_
An expression evaluator based on precedence declarations for tokens.
.EF 0
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.SP
.EQ
delim $$
.EN
.KE
.PP
The tokens of the evaluator in Figure _PREC_ are
parentheses,
.CW NUMBER ,
and the operators
.P{
\&'+' '-' '*' '/' '^' '~' UMINUS
.P}
(Ignore
.CW ~
and
.CW UMINUS
for the moment.)
.PP
The associativity and precedence of tokens are declared on
lines beginning with one of the keywords
.CW %left ,
.CW %right ,
or
.CW %nonassoc .
All of the tokens on a line have the same associativity and
precedence; they have lower precedence than operators on successive lines.
These declarations will be referred to as
.I precedence
declarations.
.PP
(The keyword
.CW %nonassoc
is used to describe operators that must not associate with themselves;
for example,
.P{
A .LT. B .LT. C
.P}
is illegal in Fortran because
.CW .LT.
is nonassociative.)
.PP
The precedence declarations
.P{
%left  '+' '-'
%left  '*' '/'
%right '^'
.P}
specify that
.CW +
and
.CW -
are left associative and have lower
precedence than the left-associative operators
.CW *
and
.CW / ,
which in turn have lower precedence than the right-associative operator
.CW ^ .
.PP
The operator
.CW ^
in the grammar
represents exponentiation, as in
.CW 2^3 ,
which evaluates to 8.
This operator is right associative;
thus,
.CW 2^2^3
is equivalent to
.CW 2^(2^3) .
.PP
The nonterminals of the grammar are
.CW lines
and
.CW expr .
The grammar expects a sequence of expressions, on separate lines.
A typical production for
.CW expr
has the form
.EQ
delim off
.EN
.P{
expr :  expr '+' expr   { $$ = $1 + $3; }
.P}
Alternatively, we can write the action as
.P{
expr :  expr '+' expr   { $$ = $expr#1 + $expr#2; }
.P}
.EQ
delim $$
.EN
.PP
With these precedence declarations, the expression
.P{
2 ^ 2 ^ 3 * 4 - 5 * 6 - 7 * 8
.P}
is evaluated as if it were parenthesized as
.P{
( (2^(2^3))*4  -  5*6 )  -  7*8
.P}
.PP
The programs section for the evaluator is in Figure _PRECB_.
The lexical analyzer
.CW yylex
returns a token each time it is called.
It skips blanks.
If it sees a digit or a decimal point, it
returns the token
.CW NUMBER
after using the
C library function
.CW scanf
to read a number into the global variable
.CW yylval
(as mentioned in Section 1, the attribute value, if any,
associated with a token
must be left in
.CW yylval ).
Otherwise, the lexical analyzer returns a character as a token.
.KF
.EQ
delim off
.EN
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.ps 9
.ft CB
.PS
[
	define text @ [
	B:	box ht vs wid pagewid invis
		$1 ljust at B.w + 20/72,0
	] @
 down
 text("%%")
 text("#include <stdio.h>")
 text("#include <ctype.h>")
 text("#include <math.h>")
 text("int yylex() {")
 text("	int c;")
 text("	while ( ( c = getchar() ) == ' ' );")
 text("	if ( (c == '.') || (isdigit(c)) ) {")
 text("		ungetc(c, stdin);")
 text("		scanf(\"%le\", &yylval);")
 text("		return NUMBER;")
 text("	}")
 text("	return c;")
 text("}")
 text("int main() { return yyparse(); }")
 text("void yyerror(s) char * s; { fprintf(stderr, \"%s\en\", s); }")
]
.PE
.@tag FI _PRECB_
.FI _PRECB_
Programs section for the evaluator in Figure _PREC_.
.EF 0
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.SP
.EQ
delim $$
.EN
.KE
.PP
The parser uses precedence declarations to decide when to apply a production.
Suppose that the input has the form
.P{
expr * expr \&\f1$...$\fP
.P}
and the parser has to decide whether or not to apply the
multiplication production
.P{
expr :  expr '*' expr
.P}
If the next symbol in the input is
.CW + ,
as in
.P{
expr * expr  + \&\f1$...$\fP
.P}
the parser applies the multiplication production because the
token
.CW +
has lower precedence than
.CW * .
However, if the next symbol in the input is
.CW ^ ,
the parser defers the multiplication production because
.CW ^
has higher precedence than
.CW * .
.PP
The treatment of the unary minus operator deserves special mention.
The evaluator in Figure _PREC_ accepts the expression
.CW 10^-1
in lieu of $10 sup -1~==~0.1$.
In other words,
.CW 10^-1
is treated like
.CW 10^(-1) ,
with unary minus having higher precedence than
.CW ^ .
To help distinguish between the two uses of the minus operator,
the evaluator recognizes
.CW ~
as a synonym for unary minus.
Unary
.CW ~
has the highest precedence and binary
.CW -
has the lowest precedence in Figure _PREC_.
The expression
.CW 3-10^~1
is therefore equivalent to
.CW 3-(10^(~1)) .
.PP
The keyword
.CW %prec
in the production
.P{
expr :  '-' expr  %prec UMINUS
.P}
specifies that the precedence of the unary minus operator is
taken from that of the token
.CW UMINUS .
Since
.CW ~
has this same precedence,
.CW 3-10^-1
is equivalent to
.CW 3-(10^(-1)) .
.PP
The
.CW %prec
keyword is used to override the precedence of the tokens
on the right side of a production.
Without
.CW %prec
the parser uses the precedence of the last token on the right
side to decide whether to apply a production.
Thus, the
.CW %prec
in
.P{
expr :  '(' TYPENAME ')' expr   %prec TYPENAME
.P}
is necessary for the production to take the precedence of
token
.CW TYPENAME ;
otherwise the production takes its precedence from the
closing parenthesis.
.PP
It is recommended that precedence declarations
be used in a ``cookbook'' fashion, until some experience is gained.
How Yacc uses precedence declarations is examined further in
Section _SH4_.
.SS "Execution Order for Actions"
The execution order of actions is significant because actions can
have side effects.
One way to visualize the order is to imagine a traversal of
a parse tree in which the children of each node
are visited depth-first from left to right, starting at the root.
Suppose a node has two children $c$ and $d$, with $c$ to the left of $d$.
.I Depth-first
implies that all the nodes in the subtree for $c$
are visited before any nodes are visited in the subtree for $d$.
The tree in Figure _FI.ACTIONS_ includes actions as pseudo-symbols,
attached by dashed lines.
Actions are executed in the order they would be visited
in a depth-first left-to-right traversal.
.KF
.EQ
define Small % size 7 "$1" %
.EN
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.nr PS 9
.ps 9
.PS
[
	define NUM % {
		"$Small(NUM)$"
		"$Small($NUM)^=^$1$" at Here + 0,-vs
	} %

	hu = .3; vu = vs; choph = vs; chopw = .7
	boxht = vs

	"$expr$"
	 {
		ln(-5,-3); "$expr$"
G1:""
		 {	ln(-2,-2); NUM(2)
G2:""
		}{	ln( 1,-2, dashed .04); "{print $Small($NUM)$}"
G3:""
		}
	}{
		ln(-1,-3); "+"
	}{
		ln( 2,-3); "$expr$"
		 {
			ln(-3,-4); "$expr$"
			 {	ln(-2,-2); NUM(3)
			}{	ln( 1,-2, dashed .04); "{print $Small($NUM)$}"
			}
		}{
			ln(-.5,-4); "\(**"
		}{
			ln( 2.5,-4); "$expr$"
			 {	ln(-2,-4); NUM(5)
			}{	ln( 1,-4, dashed .04); "{print $Small($NUM)$}"
			}
		}{
			ln( 5,-4, dashed .04);  "{print \(**}"
			box invis wid .6 ht vs at Here
		}
	}{
		ln( 6,-3, dashed .04);  "{print +}"
	}

H1:	G1 + -3*hu,0
H2:	G2 + -5*hu,-3*vu
H3:	G2.x, H2.y
.ps 36
	spline -> from H1 to H2 to H3
.ps\n(PS
]
.PE
.@tag FI _FI.ACTIONS_
.FI _FI.ACTIONS_
Actions are executed in a depth-first left-to-right order.
.EF 0
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.SP
.KE
.PP
The parse tree in Figure _FI.ACTIONS_
is based on the following specification of an
infix-to-postfix translator:
.EQ
delim off
.EN
.P{
%token NUM
%left  '+'
%left  '*'
%%
expr : expr '+' expr    { printf(" +"); }
     | expr '*' expr    { printf(" *"); }
     | '(' expr ')'
     | NUM              { printf(" %d", $NUM); }
     ;
.P}
.EQ
delim $$
.EN
The translation is emitted incrementally during parsing, so
the execution order of the print statements is critical.
The translation of
.CW 2+3*5
is
.P{
 2 3 5 * +
.P}
.SS "Actions Embedded Within Rules"
Yacc permits an action to be written in the middle of a production
as well as at the end; such actions are called
.I embedded
actions.
.PP
Embedded actions are useful for keeping track of context information.
In the EQN grammar, the context includes the current point size.
The EQN input
.P{
E sub 1
.P}
is typeset as $E sub 1$.
Note that the subscript $1$ has a smaller point size than $E$.
Nonterminal
.CW box
in the following production represents an EQN construct.
The current point size is maintained in variable
.CW ps .
The embedded action
.CW ps$^$-=$^$del
reduces the point size before the subscript box
is processed; the other action
.CW ps$^$+=$^$del
restores the point size.
Token
.CW SUB
represents the input characters
.CW sub .
.P{
box :  box  { ps -= del; }  SUB  box  { ps += del; }
.P}
.PP
Embedded actions are implemented by manufacturing a fresh
nonterminal with one production deriving the empty string.
Yacc actually treats this EQN example as if it had been
written:
.EQ
delim off
.EN
.P{
box  :  box  $ACT  SUB  box
            { ps +=del; }
     ;
$ACT :  /* empty */
            { ps -= del; }
.P}
Here
.CW $ACT
is a fresh nonterminal.
.PP
Section _SH6_ considers the use of embedded actions
to pass left-to-right context in a Yacc specification.
.PP
Embedded actions can have associated attribute values;
within an embedded action,
.CW $$
refers to its attribute value, not to the attribute of the
left side of the production.
Thus, 1 is the attribute value for the embedded action in
.P{
a : b  { $$ = 1; }  c  { x = $2; y = $c; }
.P}
The embedded action is at the second position in the right
side, so its value is referred to as
.CW $2
in the assignment
.CW x=$2 .
Note that
.CW c
is at position 3, so the above production can be
rewritten using
.CW $3
instead of
.CW $c :
.P{
a : b  { $$ = 1; }  c  { x = $2; y = $3; }
.P}
.EQ
delim $$
.EN
.@tag SH _SH3_
.SH _SH3_  "How The Parser Works"
Some familiarity with parsing is helpful
in deciphering messages from Yacc
about ``shift/reduce'' and ``reduce/reduce'' conflicts.
Such messages merit investigation; they are a warning that
the generated parser will make choices that may or may not be
consistent with the grammar designer's intent.
.PP
When invoked with the
.CW -v
(for verbose) option,
Yacc places a human-readable description of the generated parser
into the file
.CW y.output .
This section deals with the parsing background behind
.CW y.output
files; parsing conflicts themselves are considered in Section _SH4_.
.PP
The running example in this section is the following grammar for
real numbers:
.P{
%token D P
%%
real :  intp P frac    ;
intp :  D  |  intp D   ;
frac :  D  |  D frac   ;
.P}
The abbreviated names
.CW intp
for integer part,
.CW frac
for fraction,
and
.CW D
for digit, conserve space in diagrams.
The use of token
.CW P
for a decimal point
.CW '.'
avoids confusion with other uses of dots within
.CW y.output
files.
.SS "Shift-Reduce Parsing"
A
.I "bottom-up"
parser works from the leaves (bottom) of a parse tree towards the
root.
The following sequence of tree snapshots illustrates a bottom-up
parse of the token stream
.CW DDPDD ,
corresponding to the real number
.CW 21.89 .
For the moment, the digit represented by a token
.CW D
appears below the token.
The trees are
.KS
.ps 9
.ft CB
.PS
[
	define D % {
		"\f2\s8D\s0\fP"
		"\s6$1\s0" at Here + 0,-.5*vs
	} %
	define Pt % {
		"\f2\s8P\s0\fP"
		"." at Here + 0,-.5*vs
	} %

	hu = .125; vu = 1.5*vs; choph = vs; chopw = .4
	boxht = vs; boxwid = .8*vs; movewid = .5

S1:	[
		# "$real$"
		 {
			ln(-1,-1, invis); # "$intp$"
			 {
				ln(-1,-1, invis); # "$intp$"
				ln( 0,-1, invis); D(2)
			}{
				ln( 0,-2, invis); D(1)
			}
		}{
			ln( 0,-3, invis); Pt
		}{
			ln( 1,-1, invis); # "$frac$"
			 {
				ln( 0,-2, invis); D(8)
			}{
				ln( 1,-1, invis); # "$frac$"
				ln( 0,-1, invis); D(9)
			}
		}
	]
	move
S2:	[
		# "$real$"
		 {
			ln(-1,-1, invis); # "$intp$"
			 {
				ln(-1,-1, invis); "$intp$"
				ln( 0,-1); D(2)
			}{
				ln( 0,-2, invis); D(1)
			}
		}{
			ln( 0,-3, invis); Pt
		}{
			ln( 1,-1, invis); # "$~~frac$"
			 {
				ln( 0,-2, invis); D(8)
			}{
				ln( 1,-1, invis); # "$frac$"
				ln( 0,-1, invis); D(9)
			}
		}
	]
	move
S3:	[
		# "$real$"
		 {
			ln(-1,-1, invis); "$intp~$"
			 {
				ln(-1,-1); "$intp$"
				ln( 0,-1); D(2)
			}{
				ln( 0,-2); D(1)
			}
		}{
			ln( 0,-3, invis); Pt
		}{
			ln( 1,-1, invis); # "$~~frac$"
			 {
				ln( 0,-2, invis); D(8)
			}{
				ln( 1,-1, invis); # "$frac$"
				ln( 0,-1, invis); D(9)
			}
		}
	]
	move
S4:	[
		# "$real$"
		 {
			ln(-1,-1, invis); "$intp~$"
			 {
				ln(-1,-1); "$intp$"
				ln( 0,-1); D(2)
			}{
				ln( 0,-2); D(1)
			}
		}{
			ln( 0,-3, invis); Pt
		}{
			ln( 1,-1, invis); # "$frac$"
			 {
				ln( 0,-2, invis); D(8)
			}{
				ln( 1,-1, invis); "$frac$"
				ln( 0,-1); D(9)
			}
		}
	]
	move
S5:	[
		 # "$real$"
		 {
			ln(-1,-1, invis); "$intp~~$"
			 {
				ln(-1,-1); "$intp$"
				ln( 0,-1); D(2)
			}{
				ln( 0,-2); D(1)
			}
		}{
			ln( 0,-3, invis); Pt
		}{
			ln( 1,-1, invis); "$~~frac$"
			 {
				ln( 0,-2); D(8)
			}{
				ln( 1,-1); "$frac$"
				ln( 0,-1); D(9)
			}
		}
	]
	move
S6:	[
		"$real$"
		 {
			ln(-1,-1); "$intp~~$"
			 {
				ln(-1,-1); "$intp$"
				ln( 0,-1); D(2)
			}{
				ln( 0,-2); D(1)
			}
		}{
			ln( 0,-3); Pt
		}{
			ln( 1,-1); "$~~frac$"
			 {
				ln( 0,-2); D(8)
			}{
				ln( 1,-1); "$frac$"
				ln( 0,-1); D(9)
			}
		}
	]

	"$=>$" at .5<S1.e, S2.w>
	"$=>$" at .5<S2.e, S3.w>
	"$=>$" at .5<S3.e, S4.w>
	"$=>$" at .5<S4.e, S5.w>
	"$=>$" at .5<S5.e, S6.w>
]
.PE
.KE
.LP
Let us redraw these trees to line up their uncovered portions;
that is, the roots of the completed subtrees.
The redrawn trees are
.KS
.ps 9
.PS
[
	define D % {
		"\f2\s8D\s0\fP"
	} %
	define Pt % {
		"\f2\s8P\s0\fP"
	#	"." at Here + 0,-.5*vs
	} %

	hu = .125; vu = 1.5*vs; choph = vs; chopw = .4
	boxht = vs; boxwid = .8*vs; movewid = hu; sep = .5


S1:	[ 
		D(1); move; D(2); move; Pt; move; D(8); move; D(9)
	]

S2:	[
		{
			"$intp~~$"; ln( 0,-1); D(1)
		}
		move
		D(2); move; Pt; move; D(8); move; D(9)
	] with .nw at S1.ne + sep,0

S3:	[
		{
			"$intp~~$"
			 {
				ln(-1,-1); "$intp$"
				ln( 0,-1); D(1)
			}{
				ln( 0,-2); D(2)
			}
		}
		move
		Pt; move; D(8); move; D(9)
	] with .nw at S2.ne + sep,0

S4:	[
		{
			"$intp~~$"
			 {
				ln(-1,-1); "$intp$"
				ln( 0,-1); D(1)
			}{
				ln( 0,-2); D(2)
			}
		}
		move
		Pt; move; D(8)
		move
		{
			"$~~frac$"; ln( 0,-1); D(9)
		}
	] with .nw at S3.ne + sep,0

S5:	[
		{
			"$intp~~$"
			 {
				ln(-1,-1); "$intp$"
				ln( 0,-1); D(1)
			}{
				ln( 0,-2); D(2)
			}
		}
		move
		Pt
		move
		{
			"$~~frac$"
			 {
				ln( 0,-2); D(8)
			}{
				ln( 1,-1); "$frac$"
				ln( 0,-1); D(9)
			}
		}
	] with .nw at S4.ne + sep,0

S6:	[
		"$real$"
		 {
			ln(-1,-1); "$intp~~$"
			 {
				ln(-1,-1); "$intp$"
				ln( 0,-1); D(1)
			}{
				ln( 0,-2); D(2)
			}
		}{
			ln( 0,-3); Pt
		}{
			ln( 1,-1); "$~~frac$"
			 {
				ln( 0,-2); D(8)
			}{
				ln( 1,-1); "$frac$"
				ln( 0,-1); D(9)
			}
		}
	] with .nw at S5.ne + sep,0

	"$=>~$" at .5<S1.ne, S2.nw>
	"$=>$" at .5<S2.ne, S3.nw>
	"$=>$" at .5<S3.ne, S4.nw>
	"$~~=>$" at .5<S4.ne, S5.nw>
	"$=>$" at .5<S5.ne, S6.nw>
]
.PE
.KE
.PP
Shift-reduce parsers store only the uncovered portions of a
parse tree, as in
the following
snapshots, obtained from the
preceding sequence of trees:
.KS
.ps 9
.PS
[
	define N % box invis $1 wid .25 %
	define D % box invis "\f2\s8D\s0\fP" wid .125 %
	define P % box invis "\f2\s8P\s0\fP" wid .125 %
	define derives % box invis wid .4 "$=>$" %

	boxht = vs; boxwid = .8*vs


		[ D; D; P; D; D ]
	derives
		[ N("$intp$"); D; P; D; D ]
	derives
		[ N("$intp$"); P; D; D ]
	derives
		[ N("$intp$"); P; D; N("$frac$") ]
	derives
		[ N("$intp$"); P; N("$frac$") ]
	derives
		[ N("$real$") ]
]
.PE
.KE
.PP
Such derivations can be broken down
into sequences of shift and
reduce actions.
A
.I shift
action advances the parser to the next unexamined input token;
such tokens are called
.I lookahead
symbols.
A
.I reduce
action replaces the right side of a production by its left side.
The key problem of shift-reduce parsing is that of deciding
when to shift and when to reduce; Yacc generates tables for
this purpose that are explained later in this section.
.EQ
delim off
.EN
.PP
A shift-reduce parse of the input token stream
.CW DDPDD
appears in Figure _FI.SHIFTREDUCE_.
A special token
.CW $end
marks the end of the input.
Within each snapshot, a pointer appears before the
current lookahead symbol.
The first action, a shift, advances the pointer past the
leftmost
.CW D :
.EQ
delim $$
.EN
.KF
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.ps 9
.PS
[
	define N % box invis $1 wid .25 %
	define D % box invis "\f2\s8D\s0\fP" wid .125 %
	define P % box invis "\f2\s8P\s0\fP" wid .125 %
	define E % box invis "$f2($end)$" wid .3 %
	define action % Act: [
		right
	B:	box invis wid actionwid
	PTR:	box invis wid .1
		$1 ljust at B.w
	] with .PTR.n at S.PTR.s %
	define shift % action("shift") %
	define reduce % action("reduce") %
	define ptr %
		PTR: box invis wid .1; { move left .025; up; line <- }
	%
	define handle % { line from last box.sw to last box.se } %

	boxht = vs
	arrowht = .05; arrowwid = .025; lineht = .75*vs; dy = vs

	down

	actionwid = .9
T1:	S:[ right
		ptr; D; D; P; D; D; E
	]
 shift
	S:[ right
		D; handle; ptr; D; P; D; D; E
	] with .PTR.n at Act.PTR.s
 reduce
	S:[ right
		N("$intp$"); ptr; D; P; D; D; E
	] with .PTR.n at Act.PTR.s
 shift
	S:[ right
		N("$intp$"); handle; D; handle; ptr; P; D; D; E
	] with .PTR.n at Act.PTR.s
 reduce
	S:[ right
		N("$intp$"); ptr; P; D; D; E
	] with .PTR.n at Act.PTR.s
 shift
B1:	S:[ right
		N("$intp$"); P; ptr; D; D; E
	] with .PTR.n at Act.PTR.s



	actionwid = 1.2
T2:	S:[ right
		box invis wid actionwid
	  PTR:	box invis wid .05
	] with .sw at T1.se + 1.2,0
 shift
	S:[ right
		N("$intp$"); P; D; ptr; D; E
	] with .PTR.n at Act.PTR.s
 shift
	S:[ right
		N("$intp$"); P; D; D; handle; ptr; E
	] with .PTR.n at Act.PTR.s
 reduce
	S:[ right
		N("$intp$"); P; D; handle; N("$frac$"); handle; ptr; E
	] with .PTR.n at Act.PTR.s
 reduce
	S:[ right
		N("$intp$"); handle; P; handle; N("$frac$"); handle; ptr; E
	] with .PTR.n at Act.PTR.s
 reduce
B2:	S:[ right
		N("$real$"); ptr; E
	] with .PTR.n at Act.PTR.s

G:	.5<T1.se, T2.sw> + 0,vs
	line from G to G.x, B2.s.y
]
.PE
.@tag FI _FI.SHIFTREDUCE_
.FI _FI.SHIFTREDUCE_
Shift-reduce parsing of
.CB DDPDD .
.EF
.nf
\s5\l'\n(LLu\&\(ul'\s0
.EF 0
.fi
.SP
.ps 9
.PS

 define N % box invis $1 wid .25 %
 define D % box invis "\f2\s8D\s0\fP" wid .125 %
 define P % box invis "\f2\s8P\s0\fP" wid .125 %
 define E % box invis wid .05; box invis "$f2($end)$" wid .25 %
 define shift  % box invis wid .8 "shift" %
 define reduce % box invis wid .8 "reduce" %
 define lookahead % box invis wid .1; { move left .05; up; line <- } %
 define handle % { line from last box.sw to last box.se } %

[
	boxht = vs
	arrowht = .05; arrowwid = .025; lineht = .75*vs

	lookahead; D; D; P; D; D; E
 shift
	D; lookahead; D; P; D; D; E
]
.PE
.LP
The second action reduces the token
.CW D
immediately to the left of the pointer
(right sides to be reduced are underlined for clarity).
The reduction replaces the right side
.CW D
by the left side
.CW intp :
.ps 9
.PS
[
	boxht = vs
	arrowht = .05; arrowwid = .025; lineht = .75*vs

	D; handle; lookahead; D; P; D; D; E
 reduce
	N("$intp$"); lookahead; D; P; D; D; E
]
.PE
.LP
After the next
.CW D
is shifted, the right side
.CW intp$~$D
is reduced to the left side
.CW intp :
.ps 9
.PS
[
	boxht = vs
	arrowht = .05; arrowwid = .025; lineht = .75*vs

	N("$intp$"); handle; D; handle; lookahead; P; D; D; E
 reduce
	N("$intp$"); lookahead; P; D; D; E
]
.PE
.LP
Successive shift actions now advance the lookahead pointer all the way
to the endmarker.
Finally, a sequence of reduce actions completes the parse.
.PP
It is no accident that a right side to be reduced always appears
immediately to the left of the pointer in Figure _FI.SHIFTREDUCE_.
This observation is the basis for a stack-implementation of shift-reduce
parsing.
Informally, the symbols to the left of the pointer
are held on a stack, so a right side to
be reduced appears at the top of the stack.
.PP
Parsers generated by Yacc use a stack; however, instead of symbols
the stack holds states.
It is these states that are displayed in a
.CW y.output
file.
.SS "Parser States"
How does a parser know that a digit to the left of a decimal point
reduces to
.CW intp ,
but that a digit to the right reduces to
.CW frac ?
The parser uses states to summarize prior parsing actions.
Some of
the states of the real-number parser are (informally)
.SP .5
.IP 0. 4
.I "The starting state" .
The very first token must be a
.CW D ;
it reduces to
.CW intp .
.SP .5
.IP 2.
.I "Within the integer part" .
An
.CW intp
has been seen.
The lookahead token must either be a
.CW D
(another digit in the integer part)
or a
.CW P
(the decimal point).
.SP .5
.IP 7.
.I "Within the fraction part" .
Shift as long as the lookahead token is a
.CW D .
Otherwise, reduce the last
.CW D
to
.CW frac .
.SP .5
.PP
State 2 is displayed as follows in the
.CW y.output
file for the real-number grammar:
.P{
state 2
	real :  intp.P frac 
	intp :  intp.D 
.sp .5
.ft CW
	D  shift 5
	P  shift 4
	.  error
.P}
.LP
A
.I state
consists of a collection of items, where an
.I item
is a production with a dot inserted in the right side\(emsome
versions of Yacc use an underscore to mark the position of the dot.
The items of state 2 are
.P{
real :  intp.P frac 
intp :  intp.D 
.P}
In this state, the parser shifts on lookahead
.CW P .
The shift is recorded by (conceptually) moving the dot past
.CW P
to obtain the item
.P{
real :  intp P.frac 
.P}
This item is the sole item in state 4.
In words, state 4 records that a
.CW P
has been seen and that the incoming tokens are now
expected to match
.CW frac .
.PP
The states and their transitions constitute an automaton.
The automaton for the real-number grammar appears in
Figure _FI.GOTO_.
The solid arrows are for shift transitions, due to tokens.
The dashed arrows, for transitions due to nonterminals, are used
during reductions.
.KF
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.nr PS 9
.ps 9
.PS
[
	define state %
		if "$4"!="" then 'nitems=$4' else 'nitems=1'
	S$1:	box fill at O + $2*hu, $3*vu ht nitems*vs
		"$1" at S$1.n + 0,vs/2
	%

	arcrad = vs/3; boxht = vs; boxwid = 1.2; fillval = 1
	dashwid = .03
	hu = .7; vu = 4*vs

O: ""
	state(0, 0, 0)
	box invis "$f2($accept)^:~cdot~real~f2($end)$" at S0

	state(1, 3, 0)
	box invis "$f2($accept)^:~real~cdot~f2($end)$" at S1

	state(2, 1,-1, 2)
	[ down
	B1:	box invis
	B2:	box invis
		"$real^:~intp~cdot~P~frac$" ljust at B1.w + .075,0
		"$intp^:~intp~cdot~D$" ljust at B2.w + .075,0
	] at S2

	state(5, 4,-1)
	box invis "$intp^:~intp~D~cdot$" at S5

	state(4, 2,-2)
	box invis "$real^:~intp~P~cdot~frac$" at S4

	state(6, 5,-2)
	box invis "$real^:~intp~P~frac~cdot$" at S6

	state(7, 3,-3, 2)
	[ down
	B1:	box invis
	B2:	box invis
		box invis "$frac^:~D~cdot$" ljust at B1.w + .1,0
		box invis "$frac^:~D~cdot~frac$" ljust at B2.w + .1,0
	] at S7

	state(8, 6,-3)
	box invis "$frac^:~D~frac~cdot$" at S8

	state(3, 0,-3)
	box invis "$intp^:~D~cdot$" at S3

S9:	box invis at O + 6*hu, 0
	"\0\0\f3accept\fP" ljust at S9.w

G02:	.66<S0.sw,S0.s>
G03:	.33<S0.sw,S0.s>
G2:	.5<S2.sw,S2.s>
G4:	.5<S4.sw,S4.s>
G7a:	.5<S7.sw,S7.s>
G7b:	.5<S7.s,S7.se>
G7c:	S7.s + 0,-1.5*vs

	line -> from S0.e to S1.w dashed
	line -> from S2.e to S5.w
	line -> from S4.e to S6.w dashed
	line -> from S7.e to S8.w dashed
	line -> from S1.e to S9.w
	line -> from G03.s to G03.x, S3.n.y

	adown(G02); anell(S2.w, -> dashed, dashed)
	adown(G2); anell(S4.w, ->)
	adown(G4); anell(S7.w, ->)
	adown(G7a); anell(G7c); anell(G7b, ->)

	"$real$"	at .5<S0.e, S1.w> + 0, vs/2
	"$intp$"	at (G02.x+S2.w.x)/2, S2.y + vs/2
	"$~D$" ljust	at G03.x,S4.y
	"$f2($end)$"	at .5<S1.e, S9.w> + 0, vs/2
	"$D$"		at .5<S2.e, S5.w> + 0, vs/2
	"$P$"		at (G2.x+S4.w.x)/2, S4.y + vs/2
	"$frac$"	at .5<S4.e, S6.w> + 0, vs/2
	"$D$"		at (G4.x+S7.w.x)/2, S7.y + vs/2
	"$frac$"	at .5<S7.e, S8.w> + 0, vs/2
	"$D$"		at G7c + 0, vs/2
]
.PE
.@tag FI _FI.GOTO_
.FI _FI.GOTO_
States and transitions for the real-number grammar.
.EF 0
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.SP
.KE
.PP
The parser holds states on a stack, with the current state on
top.
The starting state of the automaton in Figure _FI.GOTO_ is state 0.
With lookahead
.CW D ,
the automaton shifts to state 3 (in the bottom-left corner of
the figure)
by pushing 3 onto the stack and removing the lookahead
.CW D
from the input.
For ease of comparison with Figure _FI.SHIFTREDUCE_, the symbol
.CW D
appears below the state in the following diagram:
.KS
.ps 9
.PS
[
	define Disp %
		box invis $2
		{ move to last box + 0,-del; $1 }
	%
	define N % Disp($1, wid .25) %
	define D % Disp("\f2\s8D\s0\fP", wid .1) %
	define P % Disp("\f2\s8P\s0\fP", wid .1) %
	define E % Disp("$f2($end)$", wid .3) %
	define action % Act: [
		right
	B:	box invis wid 1
	PTR:	box invis wid .1
		$1 ljust at B.w
	] with .PTR.n at S.PTR.s %
	define shift % action("shift") %
	define reduce % action("$1") %
	define ptr %
	PTR:	box invis wid .05
			{ move left .025; move up boxht/2; line <- }
		box invis wid .3
			{ move left .15; $1 }
		box invis wid .05
			{ move left .025; move up boxht/2; line <- }
	%
	define handle % { line from last box.sw to last box.se } %
	define state % [
		down
	ELEM:	box "$1"
		$2
		$3
	] with .ELEM.w at Here; move to last [].ELEM.e; right %

	boxht = vs; boxwid = .2
	arrowht = .05; arrowwid = .025; lineht = .5*vs; dy = vs
	movewid = .1; del = .015

L1:	S:[ right
		state(0)
		ptr()
		D; D; P; D; D; E
	]
 shift
L2:	S:[ right
		state(0); state(3, D)
		ptr()
		D; P; D; D; E
	] with .PTR.n at Act.PTR.s + 0,-boxht/2

]
.PE
.KE
.LP
The top of the state stack has its own pointer, separate from the
lookahead pointer to the next input symbol.
.PP
The only item in state 3 is
.P{
intp :  D.
.P}
Whenever the dot in an item is at the end of the production, one of the
possible actions is a reduction by that production.
Since the state has no other actions, the parser chooses to reduce.
.PP
A reduce action has two phases:
(a) pop the states corresponding to the right side,
and (b) push a state corresponding to the left side.
The following diagram illustrates the reduction of the
leading
.CW D
in
.CW DDPDD
to
.CW intp :
.KS
.ps 9
.PS
[
	define Disp %
		box invis $2
		{ move to last box + 0,-del; $1 }
	%
	define N % Disp($1, wid .25) %
	define D % Disp("\f2\s8D\s0\fP", wid .1) %
	define P % Disp("\f2\s8P\s0\fP", wid .1) %
	define E % Disp("$f2($end)$", wid .3) %
	define action % Act: [
		right
	B:	box invis wid 1
	PTR:	box invis wid .1
		$1 ljust at B.w
	] with .PTR.n at S.PTR.s  + 0,-boxht %
	define shift % action("shift") %
	define reduce % action("$1") %
	define ptr %
	PTR:	box invis wid .05
			{ move left .025; move up boxht/2; line <- }
		box invis wid .3
			{ move left .15; $1 }
		box invis wid .05
			{ move left .025; move up boxht/2; line <- }
	%
	define handle % { line from last box.sw to last box.se } %
	define state % [
		down
	ELEM:	box "$1"
		$2
		$3
	] with .ELEM.w at Here; move to last [].ELEM.e; right %

	boxht = vs; boxwid = .2
	arrowht = .05; arrowwid = .025; lineht = .5*vs; dy = vs
	movewid = .1; del = .015

L1:	S:[ right
		state(0); state(3, D, handle)
		ptr()
		D; P; D; D; E
	]
 reduce(pop)
L2:	S:[ right
		state(0)
		ptr("$intp$")
		D; P; D; D; E
	] with .PTR.n at Act.PTR.s + 0,-boxht
 reduce(goto)
L3:	S:[ right
		state(0); state(2, N("$intp$"))
		ptr()
		D; P; D; D; E
	] with .PTR.n at Act.PTR.s + 0,-boxht

	dx = .75; dy = -lineht-vs/2-del
	"Pop the states corresponding to the right side \f2\s8D\s0\fP."\
		ljust at L1.ne + dx,dy
	"Prepare to push a state for the left side $intp$."\
		ljust at L2.ne + dx,dy
	"State 0 goes to 2 under $intp$."\
		ljust at L3.ne + dx,dy

	box invis wid 2.75 with .nw at L1.ne + dx,dy
]
.PE
.KE
.SS "Parsing Actions"
The parser has only four actions available to it, called
shift, reduce, accept, and error.
A move of the parser is as follows:
.SP .5
.IP 1. 4
Based on its current state, the parser decides whether it needs a lookahead
token to choose the next action.
If it needs one, and does not already have one, it calls
.CW yylex
to obtain the next token.
.SP .5
.IP 2.
Using the current state $p$, and the lookahead token $t$
if needed, the parser chooses an action.
.RS
.SP .25
.IP a) 4
A
.I shift
action to state $q$ is done by pushing state $q$ onto the
state stack and clearing the lookahead token.
.SP .25
.IP b)
A
.I reduce
action by a production is done in two phases.
In the first phase, the parser pops from the stack a number of states
equal to the number of symbols on the right side.
Let $q$ be the
state uncovered after the states are popped,
and let the nonterminal on the left side of the production take $q$ to $r$.
In the second phase, the parser pushes state $r$
onto the stack.
.SP .25
.EQ
delim off
.EN
.IP c)
The
.I accept
action occurs after the entire input has been seen and matched.
This action occurs only when the lookahead token is the endmarker
.CW $end .
.EQ
delim $$
.EN
.SP .25
.IP d)
An
.I error
action occurs when the parser can no longer continue parsing
according to the productions.
That is, the input tokens seen so far and the current lookahead
cannot possibly be followed by anything that would result
in a legal input.
Error recovery is covered in Section _SH5_.
.RE
.@tag SH _SH4_
.SH _SH4_  "Ambiguity and Conflicts"
A
.I "shift/reduce conflict"
occurs if the parser cannot decide between
a shift action and a reduce action.
Similarly, a
.I "reduce/reduce conflict"
occurs if the parser cannot decide between
two legal reductions.
.PP
Conflicts definitely occur when a grammar is
.I ambiguous ;
that is, if some input string has more than one parse tree.
Conflicts can also occur when a grammar, although consistent,
requires a more complex parser than Yacc is capable of
constructing.
Finally, conflicts are sometimes introduced when an action is embedded
into the middle of a production.
.PP
Yacc produces a parser even when conflicts occur.
Rules used to choose between two competing actions is
called
.I "disambiguating rules" .
The default disambiguating rules are:
.SP .5
.IP 1. 4
In a shift/reduce conflict, the default is to shift.
.SP .25
.IP 2.
In a reduce/reduce conflict, the default is to reduce by
the earlier production in the specification.
.SP .5
.PP
Although the effect of disambiguating rules can often be achieved
by rewriting the grammar to avoid conflicts,
experience suggests that this rewriting is somewhat unnatural
and produces slower parsers.
.PP
The rest of this section considers two situations;
one in which the default disambiguating rules do not
have the intended effect, and one in which they do.
It is recommended that shift/reduce conflicts be
investigated using the
.CW y.output
file created by running Yacc with the
.CW -v
option.
.SS "To Shift or To Reduce"
Yacc reports 2 shift/reduce conflicts when it is applied to
.EQ
delim off
.EN
.P{
%token NUM
%%
expr  :  expr '\en'       { printf("\en"); return(0); }
      |  expr '-' expr   { printf(" -"); }
      |  NUM             { printf(" %g", $NUM); }
      ;
.P}
.EQ
delim $$
.EN
This specification marks the end of an expression
by a newline character;
the
.CW return
in the associated action terminates parsing immediately after
echoing the newline.
.PP
The hope here is to translate an infix expression into postfix
notation.
Thus we want
.P{
2 - 1 - 1
.P}
to be translated into
.P{
2 1 - 1 -
.P}
The answer would be 0 if the expression were evaluated by
subtracting 1 from 2, and then subtracting 1 from the result.
Unfortunately, the output of the parser (with a suitable program
section) is
.P{
 2 1 1
.P}
Whatever happened to the minus operators?
The problem can be traced to the preference for shift in a shift/reduce
conflict, which makes
.CW -
right associative, and gives
.CW \en
higher precedence than
.CW - .
.KF
.EQ
delim off
.EN
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.ps 9
.ft CW
.PS
[
	define text % [
		boxht = vs
	B:	box $2 invis
		$1 ljust at B.w
	] %

	boxht = vs

C1:	[
		boxwid = 2; down

		text("state 0")
		text("    $accept : .expr $end ")
		text("", ht vs/3)
		text("    NUM  shift 2")
		text("    .  error")
		text("    expr  goto 1")
		text("", ht vs/2)
		text("state 1")
		text("", ht vs/3)
		text("    $accept :  expr.$end ")
		text("    expr :  expr.\en ")
		text("    expr :  expr.- expr ")
		text("    $end  accept")
		text("    \en  shift 3")
		text("    -  shift 4")
		text("    .  error")
		text("", ht vs/2)
		text("state 2")
		text("", ht vs/3)
		text("    expr :  NUM.    (3)")
		text("    .  reduce 3")
	]

C2:	[
		boxwid = 2.5; down

		text("state 3")
		text("    expr :  expr \en.    (1)")
		text("", ht vs/3)
		text("    .  reduce 1")
		text("", ht vs/2)
		text("state 4")
		text("    expr :  expr -.expr ")
		text("", ht vs/3)
		text("    NUM  shift 2")
		text("    .  error")
		text("    expr  goto 5")
		text("", ht vs/2)
.ft CB
		text("5: conflict (shift \en, reduce 2)")
		text("5: conflict (shift -, reduce 2)")
		text("state 5")
		text("    expr :  expr.\en ")
		text("    expr :  expr.- expr ")
		text("    expr :  expr - expr.    (2)")
		text("", ht vs/3)
		text("    \en  shift 3")
		text("    -  shift 4")
		text("    .  reduce 2")
.ft CW
	] with .ne at C1.nw + pagewid-.75,0
]
.PE
.EQ
delim $$
.EN
.@tag FI _FI.Y.OUTPUT_
.FI _FI.Y.OUTPUT_
A slightly edited
.CW y.output
file.
.EF 0
.nf
\s5\l'\n(LLu\&\(ul'\s0
.fi
.SP
.KE
.PP
A slightly edited version of the
.CW y.output
file for this grammar appears in Figure _FI.Y.OUTPUT_.
The 2 shift/reduce conflicts are in state 5.
In this state,
by default, the parser shifts the lookahead symbols
.CW \en
and
.CW -
instead of using the item
.P{
expr :  expr - expr.    (2)
.P}
to reduce by production (2).
A reduction occurs only when a right side is at the top of the
parser stack, so the stack must contain states corresponding to
.P{
\f1$...$\fP expr$"" sub a$ - expr$"" sub b$
.P}
(The subscripts merely distinguish between the
two occurrences of
.CW expr .)
.PP
The other items in state 5 tell us more about the choices
faced by the parser.
With lookahead
.CW \en
the parser shifts, by default.
Thus, it treats
.CW expr$"" sub b$
on top of the stack as if it were in the item
.P{
expr :  expr$"" sub b$.\en
.P}
instead of the item
.P{
expr :  expr$"" sub a$ - expr$"" sub b$.    (2)
.P}
Similarly, with lookahead
.CW - ,
the parser treats
.CW expr$"" sub b$
as if it were the first subexpression in
.P{
expr :  expr$"" sub b$.- expr
.P}
.PP
Putting these observations together, the input
.CW 2-1-1\en
will result in the stack eventually containing
.P{
expr - expr - expr \en
.P}
.PP
Now, when the right side
.CW expr$^$\en
is reduced, its action will print a newline and return,
abandoning the parse in midstream.
.PP
The addition of the precedence declarations
.P{
%nonassoc '\en'
%left     '-'
.P}
eliminates the conflicts in state 5, resulting in the
new state
.P{
state 5
    expr :  expr.\en 
    expr :  expr.- expr 
    expr :  expr - expr.    (2)
.sp .5
    .  reduce 2
.P}
Here, the reduction takes precedence over the potential shifts,
thereby implementing the left associativity of
.CW -
and its higher precedence over
.CW \en .
.SS "Precedence Declarations"
Precedence declarations give rise to disambiguating
rules for resolving parsing conflicts.
As mentioned in Section _SH2_, a precedence declaration
starts with
.CW %left ,
.CW %right ,
or
.CW %nonassoc .
These keywords specify the associativity of the tokens in a declaration.
All the tokens in a declaration have the same precedence; tokens
in successive declarations have higher precedence.
A token in a precedence declaration need not be, but can be,
declared by
.CW %token
as well.
.PP
The disambiguating rules are as follows.
.SP .5
.IP 1. 4
Although declared only for tokens and literals, precedence
information is attached to each production as well.
The precedence and associativity of a production
is that of the last token or literal
on its right side.
The
.CW %prec
construction overrides this default.
.SP .5
.IP 2.
If there is a shift/reduce conflict, and both
the lookahead symbol (to be shifted)
and the production (to be reduced)
have precedence declarations, then
the conflict is resolved in favor of the action (shift or reduce)
with the higher precedence.
If the precedences are the same, then the associativity is used;
left associative implies reduce, right associative
implies shift, and nonassociative implies error.
.SP .5
.IP 3.
In the absence of precedence information, the default
disambiguating rules given earlier in this section are used.
More precisely, suppose that there is no
precedence information for either the lookahead symbol or
a production to be reduced by.
A shift/reduce conflict is resolved by shifting.
A reduce/reduce conflict is resolved by reducing by the
production that appears earlier in the specification.
Conflicts resolved by these default rules are reported.
.SP .5
.LP
Conflicts resolved by precedence are not counted in the shift/reduce
and reduce/reduce conflicts reported by Yacc.
Thus, mistakes in precedence declarations can mask errors in
the design of the productions.
.SS "Shift a Dangling Else"
As an example of the power of the default disambiguating rules
consider a fragment from a programming language involving an
if-then-else construction:
.P{
stmt :  IF '(' expr ')' stmt
     |  IF '(' expr ')' stmt ELSE stmt
     ;
.P}
Here,
.CW IF
and
.CW ELSE
are tokens,
.CW stmt
is a nonterminal for statements, and
.CW expr
is a nonterminal for expressions.
Call the first production the
.I "simple-if"
production and the second the
.I "else-if"
production.
.PP
These two productions are ambiguous, since
.P{
IF ( expr$"" sub 1$ ) IF ( expr$"" sub 2$ ) stmt$"" sub a$ ELSE stmt$"" sub b$
.P}
can be structured in two ways
.ft CW
.TS
lw4 0 l7 | l.
	IF ( expr$"" sub 1$ ) {	IF ( expr$"" sub 1$ ) {
.ft CB
	    IF ( expr$"" sub 2$ ) stmt$"" sub a$	    IF ( expr$"" sub 2$ ) stmt$"" sub a$
	\f(CW}\fP	    ELSE stmt$"" sub b$
.ft CW
	ELSE stmt$"" sub b$	}
.TE
.LP
The second interpretation, with an
.CW ELSE
matching the nearest preceding unmatched
.CW IF ,
is the one taken by most programming languages.
It is the interpretation obtained when shift/reduce conflicts are
resolved in favor of shift actions.
.PP
The
.CW y.output
file for a grammar containing the simple-if and if-else productions
contains a shift/reduce conflict, illustrated by
.P{
.ft CW
8: shift/reduce conflict (shift 9, red'n 1) on ELSE
.ft CB
state 8
    stmt :  IF ( expr ) stmt.    (1)
    stmt :  IF ( expr ) stmt.ELSE stmt 
.ft CW
.sp .5
    ELSE  shift 9
    .  reduce 1
.P}
This conflict occurs when the lookahead symbol is
.CW ELSE
and the parser stack contains the right side of the simple-if production
.P{
\f1$...$\fP IF ( expr ) stmt
.P}
The parser chooses to shift the
.CW ELSE
rather than reduce by the simple-if production.
This choice allows
.P{
\f1$...$\fP IF ( expr ) stmt ELSE stmt
.P}
to be successfully reduced by the if-else production.
Note that a premature application of the simple-if rule
(with lookahead
.CW ELSE )
would lead to the stack contents
.P{
\f1$...$\fP stmt ELSE stmt
.P}
which cannot be parsed further.
.PP
A shift on lookahead
.CW ELSE
also matches an
.CW ELSE
with the nearest preceding unmatched
.CW IF .
With stack contents
.P{
\f1$...$\fP \f(CWIF ( expr )\fP IF ( expr ) stmt
.P}
and lookahead
.CW ELSE ,
the parser shifts, so the stack eventually holds
.P{
\f1$...$\fP \f(CWIF ( expr )\fP IF ( expr ) stmt ELSE stmt
.P}
A reduction by the if-else production now yields
.P{
\f1$...$\fP \f(CWIF ( expr )\fP stmt
.P}
which can be reduced by the simple-if production.
.@tag SH _SH5_
.SH _SH5_  "Error Handling"
Error handling is an extremely difficult area, and many of the problems are
semantic ones.
When an error is found, it may be necessary to undo the effect of
actions\(emfor example, to reclaim parse-tree storage, to delete or alter
symbol-table entries\(emand, typically, set flags to avoid generation of
further output.
.PP
It is seldom acceptable to stop all processing when an error is found;
further syntax errors might be found if parsing continues.
But, how do we get the parser ``restarted'' after an error is detected.
One approach is to discard tokens from the input until parsing can be
continued.
.PP
Yacc provides a simple, but reasonably general, feature for discarding
tokens.
The token name
.CW error
is reserved for error handling.
On the right side of a production,
.CW error
suggests a place where error recovery is planned.
When an error occurs, the parser pops its stack until it enters a
state where the token
.CW error
is legal.
It then behaves as if
.CW error
were the current lookahead, and performs the action encountered.
The lookahead is then reset to the token that caused the error.
If no error productions are specified, parsing halts when an
error is detected.
.PP
In order to prevent a cascade of error messages, the parser, after
detecting an error, remains in the error state until
three tokens have been successfully
read and shifted.
If an error is detected when the parser is already in error state,
no message is given, and the input token is quietly deleted.
.PP
For example, a production
.P{
stmt :  error
.P}
would, in effect, mean that on a syntax error
the parser would attempt to skip over the statement
in which the error was seen.
More precisely, the parser will
scan ahead, looking for three tokens that might legally follow
a statement, and start processing at the first of these; if
the beginnings of statements are not sufficiently distinctive, it may make a
false start in the middle of a statement, and end up reporting a
second error where there is in fact no error.
.PP
Actions can be used with these special error rules.
These actions might attempt to reinitialize tables, reclaim symbol table space, etc.
.PP
Productions with just
.CW error
on the right side are very general, but difficult to control.
Somewhat easier are productions such as
.P{
stmt :  error  ';'
.P}
Here, upon error, the parser attempts to skip over the statement, but
will do so by skipping to the next semicolon
All tokens after the error and before the next semicolon
cannot be shifted, and are discarded.
When the semicolon is seen, this right side will be reduced,
and any ``cleanup''
action associated with it performed.
.PP
Another form of error production arises in interactive applications, where
it may be desirable to permit a line to be reentered after an error.
A possible error production might be
.EQ
delim off
.EN
.P{
input :  error '\en'
             {  printf("Reenter last line: ");  }
         input
             { $$ = $input;  }
      ;
.P}
One potential difficulty with this approach is that
the parser must correctly process three input tokens before it
admits that it has correctly resynchronized after the error.
If the reentered line contains an error
in the first two tokens, the parser deletes the offending tokens,
and gives no message; this is clearly unacceptable.
For this reason, there is a mechanism that
can be used to force the parser
to believe that an error has been fully recovered from.
The statement
.P{
yyerrok;
.P}
in an action
resets the parser to its normal mode.
The last example is better written
.P{
input :  error '\en'
             { yyerrok; printf("Reenter last line: "); }
         input
             { $$ = $input; }
      ;
.P}
.EQ
delim $$
.EN
.PP
As mentioned above, the token seen immediately
after the
.CW error
symbol is the input token at which the
error was discovered.
Sometimes, this is inappropriate; for example, an
error recovery action might
take upon itself the job of finding the correct place to resume input.
In this case,
the previous lookahead token must be cleared.
The statement
.P{
yyclearin;
.P}
in an action has this effect.
For example, suppose the action after error
is to call some sophisticated resynchronization routine,
supplied by the user, that attempts to advance the input to the
beginning of the next valid statement.
After this routine is called, the next token returned by
.CW yylex
is presumably the first token in a legal statement;
the old, illegal token must be discarded, and the error state reset.
This can be done by a rule like
.P{
stmt :  error 
            { resynch(); yyerrok; yyclearin; }
     ;
.P}
.PP
These mechanisms are admittedly crude, but do allow for a simple,
fairly effective recovery of the parser
from many errors;
moreover, the user can get control to deal with
the error actions required by other portions of the program.
.@tag SH _SH6_
.SH _SH6_  "The Yacc Environment"
From a specification, Yacc creates a file of C programs, called
.CW y.tab.c
on most systems.
.SS "Program Organization"
User-code in a specification file
.P{
%{
  $<!$\f2user supplied code within declarations\fP$>!$
#define YYSTYPE $<!$\f1desired type\fP$>!$
%}
  $<!$\f2declarations section\fP$>!$
%%
  $<!$\f2productions\fP$>!$
%%
  $<!$  \f1program section\fP$>!$
.P}
leads to a
.CW y.tab.c
file organized as follows:
.P{
  $<!$\&\f2user supplied code within declarations\fP$>!$
#define YYSTYPE $<!$\f1desired type\fP$>!$
  $<!$\f2token and other declarations\fP$>!$
  $<!$\f2program section\fP$>!$
  $<!$\f2parser tables\fP$>!$
yyparse() {  \&\f1$...$\fP }
.P}
Actions are incorporated into
.CW yyparse .
.PP
As mentioned in Section _SH1_,
.CW yyparse ,
the code for the parser, expects the following functions
to be supplied:
.P{
int yylex() {
	\f2a lexical analyzer; returns a token\fP
}
int main(\f1$...$\fP) {
	\f1$...$\fP yyparse(); \f1$...$\fP
}
void yyerror(s) char *s; {
	\f2print an error message pointed to by\fP s
}
.P}
.PP
The function
.CW main
decides when it wants to call
.CW yyparse ,
which returns 0 if the parser accepts, and 1 if an error is detected
and no error recovery is possible.
.PP
A user-supplied function
.CW yyerror
is called with a string containing an error message.
The average application will want to do more than simply print the
message; for example, the message might be
accompanied by the input line number on which the error was detected.
The external integer variable
.CW yychar
contains the lookahead token number at the time an error is
detected; this may be of some interest in giving better
diagnostics.
.PP
The external variable
.CW yydebug
is normally set to 0.
It it is set to a nonzero value, the parser will output a
verbose description of its actions, including the tokens
read and the parser actions.
Depending on the operating environment, it may be possible
to set this variable by using a debugging system.
.SS "Lexical Tie-Ins"
The lexical analyzer
.CW yylex
must return an integer, the token number, representing
the lookahead token.
An attribute value associated with the token must be
placed in the global variable
.CW yylval .
.PP
The parser and the lexical analyzer must agree on the
token numbers in order for communication between them to
take place.
Token numbers may be chosen by Yacc, or chosen by the user.
In either case, the
.CW #def\&ine
mechanism of C is used to allow the lexical analyzer
to refer to these numbers symbolically.
For example, the declarations section
.P{
%token IF ELSE
.P}
leads to the following definitions in the file
.CW y.tab.c :
.P{
# define IF 257
# define ELSE 258
.P}
.PP
If
.CW yylex
is included in the programs section, it is within the scope
of these definitions, so
.CW IF
and
.CW ELSE
can be used as the names of token numbers in
.CW yylex .
.PP
A file
.CW y.tab.h
containing the definition of token numbers can be
created by running Yacc with the
.CW -d
option.
.PP
The approach of treating token names as defined constants
leads to clear, easily modified lexical analyzers; the only
pitfall is the need to avoid using any token names that are
reserved or significant in C or the parser.
For example, the use of the token names
.CW if
and
.CW while
will almost certainly cause severe difficulties when
the lexical analyzer is compiled.
The token name
.CW error
is reserved for error handling; see Section _SH5_.
.PP
Yacc chooses token numbers if the user does not.
The default token number for a literal character
is the numerical value of the character in the
local character set.
Other names are assigned token numbers starting at
257.
.PP
To assign a number to a token (including a literal), the first
appearance of the token name or literal in the declarations section
can be immediately followed by a nonnegative integer.
This integer is taken to be the token number of the name or literal.
Names and literals not defined by this mechanism retain their default
definition.
It is important that all token numbers be distinct.
.PP
For historical reasons, the endmarker must have token number
0 or negative.
This token number cannot be redefined by the user;
thus, all lexical analyzers must be prepared to return
0 or negative as a token number upon reaching the end of
their input.
.SS "Communicating Context to the Lexical Analyzer"
Some lexical decisions depend on context.
For example, the lexical analyzer might want to
delete blanks normally, but not within quoted strings.
Or names might be entered into a symbol table in declarations,
but not in expressions.
.PP
One way of handling this situation is
to create a global flag that is
examined by the lexical analyzer, and set by actions.
For example, suppose a program
consists of zero or more declarations,
followed by zero or more statements.
Consider:
.P{
%{
    int dflag;
%}
\&\f2$...$  other declarations $...$\f(CB
%%
prog  :  decls stmts
      ;
decls :  /* empty */          { dflag = 1; }
      |  decls  declaration
      ;
stmts :  /* empty */          { dflag = 0; }
      |  stmts  statement
      ;
\&\f2$...$  other productions $...$\f(CB
.P}
.PP
The flag
.CW dflag
is now 1 when reading declarations and 0 when reading statements,
.ul
except for the first token in the first statement.
This token must be seen by the parser before it can tell that
the declarations have ended and the productions have
begun.
In many cases, this single token exception does not
affect the lexical scan.
.PP
This kind of ``backdoor'' approach can be elaborated
to a noxious degree.
Nevertheless, it represents a way of doing some things
that are difficult, if not impossible, to
do otherwise.
.PP
Some programming languages
permit the user to
use words like
.CW if ,
which are normally reserved,
as label or variable names, provided that such use does not
conflict with the legal use of these names in the programming language.
This is extremely hard to do in the framework of Yacc;
it is difficult to pass information to the lexical analyzer
telling it ``this instance of 
.CW if
is a keyword, and that instance is a variable''.
The user can make a stab at it, using flags like
.CW dflag ,
above, but it is difficult.
It is better that the keywords be
.I reserved ;
that is, be forbidden for use as variable names.
There are powerful stylistic reasons for preferring this, anyway.
.SS "Support for Arbitrary Attribute Types"
By default, the values returned by actions and
the lexical analyzer are integers.
Other types can be supported by defining
.CW YYSTYPE
in the declarations section, as in
.P{
%{
#define YYSTYPE double
%}
.P}
.CW YYSTYPE
is not a normal variable, because the parser contains the following
lines to define it to be
.CW int
if it has not already been defined by the user:
.P{
#ifndef YYSTYPE
#define YYSTYPE int
#endif
.P}
.PP
Clearly,
.CW YYSTYPE
can be defined to be any type, including a union type.
.PP
The
.CW %union
mechanism of Yacc attempts to make the underlying union
transparent, in all but a few places where Yacc needs
help in determining which field of the union is intended.
.PP
Unions are declared in the declarations section, an example being
.P{
%union {
    int    ival;
    double dval;
    char * sval;
}
.P}
A union type with these members is created for the Yacc value stack,
and for the global variables
.CW yylval
and
.CW yyval .
With the
.CW -d
option, Yacc copies the union type into the
.CW y.tab.h
file.
The type of the union can be referred to as
.CW YYSTYPE .
.PP
The type of each attribute must now correspond to one of
the union members.
The construction
.P{
<name>
.P}
indicates a union member name.
If the construction follows
.CW %token ,
.CW %left ,
.CW %right ,
or
.CW %nonassoc ,
then the union member name is associated with the tokens in
that declaration.
Another keyword
.CW %type
is used similarly to associate union member names with nonterminals.
Thus, we might use
.P{
%type <dval> expr term
.P}
.EQ
delim off
.EN
.PP
There remain a couple of cases where these mechanisms are insufficient.
If there is an action within a rule, the value returned
by this action has no a priori type.
Similarly, reference to left context values
leaves Yacc with no easy way of knowing the type.
In this case, a type can be imposed on the reference by inserting
a union member name, between
.CW <
and
.CW > , immediately after
the first
.CW $ .
An example of this usage is
.P{
rule :  aaa  {  $<intval>$  =  3;  }
        bbb  {  fun( $<intval>2, $<other>0 );  }
     ;
.P}
This syntax has little to recommend it, but the situation arises rarely.
.PP
The facilities in this subsection are not triggered until they are used:
in particular, the use of
.CW %type
will turn on these mechanisms.
When they are used, there is a fairly strict level of checking.
For example, use of
.CW $\f2n\fP
or
.CW $$
to refer to something with no defined type
is diagnosed.
If these facilities are not triggered, the Yacc value stack is used to
hold
.CW int s,
as was true historically.
.EQ
delim $$
.EN
.NH 1
References
.LP
|reference_placement

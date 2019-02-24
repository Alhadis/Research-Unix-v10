%token WORD COMMA DOT NEG APPEND HEAD PARA STORY PIPE WRITE NL
%{
/* garbage to let this compile */
typedef char * charptr;
# define YYSTYPE charptr
extern char * svc();
%}
%%
S: T R;
 | R
T: which =defdest();
R: ;
 | R req;
req: what where newline which;
what: HEAD  =svsz(1);
    | PARA  =svsz(2);
    | STORY =svsz(3);
    |  =svsz(3);
where: PIPE =svwh($1);
     | APPEND = svwh($1);
     | WRITE = svwh($1);
which: story newline =mkstory();
     | which story newline = mkstory();
seq: wkey ;
   | seq wkey ;
sent: seq = mkseq();
   | sent COMMA seq = mkseq();
story: sent = mksent();
    | story DOT sent = mksent();
wkey: WORD ={mkwrd($1, 0);}
   | NEG WORD ={mkwrd($2, 1);}
newline: NL;
  | newline NL;
%%
# include "stdio.h"
# include "ctype.h"
# include "assert.h"
# include "lex.yy.c"

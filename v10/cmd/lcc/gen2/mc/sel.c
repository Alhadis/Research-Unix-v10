/* "lea sp@(%c),a1; movl #%0/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b" => ="lea %z,a1; movl %x,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b" C="sp@(%c)" B="#%c/2" b=a */
/* "movl #%0/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b" => ="movl %x,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b" B="#%c/2" b=a */
/* "movl a%0,a%c" => ="mov%t %y,%z" E="l" A="a%n" 0=0 C="a%c" */
/* "jbsr a%0@" => ="jbsr %y" A="a%n@" 0=0 */
/* "andl #0xffff,d%c" => ="%f%t %x,%z" D="and" E="l" B="#%c" b="0xffff" C="d%c" */
/* "andl #0xff,d%c" => ="%f%t %x,%z" D="and" E="l" B="#%c" b="0xff" C="d%c" */
/* "movb d%0,d%c" => ="mov%t %y,%z" E="b" A="d%n" 0=0 C="d%c" */
/* "movw d%0,d%c" => ="mov%t %y,%z" E="w" A="d%n" 0=0 C="d%c" */
/* "movb d%0,d%c" => ="mov%t %y,%z" E="b" A="d%n" 0=0 C="d%c" */
/* "movw d%0,d%c" => ="mov%t %y,%z" E="w" A="d%n" 0=0 C="d%c" */
/* "movl d%0,d%c" => ="mov%t %y,%z" E="l" A="d%n" 0=0 C="d%c" */
/* "movl d%0,d%c" => ="mov%t %y,%z" E="l" A="d%n" 0=0 C="d%c" */
/* "movl d%0,d%c" => ="mov%t %y,%z" E="l" A="d%n" 0=0 C="d%c" */
/* "movl a%0,d%c" => ="mov%t %y,%z" E="l" A="a%n" 0=0 C="d%c" */
/* "movl d%0,d%c" => ="mov%t %y,%z" E="l" A="d%n" 0=0 C="d%c" */
/* "movl d%0,d%c" => ="mov%t %y,%z" E="l" A="d%n" 0=0 C="d%c" */
/* "movl d%0,d%c" => ="mov%t %y,%z" E="l" A="d%n" 0=0 C="d%c" */
/* "movl d%0,a%c" => ="mov%t %y,%z" E="l" A="d%n" 0=0 C="a%c" */
/* "movl d%0,d%c" => ="mov%t %y,%z" E="l" A="d%n" 0=0 C="d%c" */
/* "movl d%0,d%c" => ="mov%t %y,%z" E="l" A="d%n" 0=0 C="d%c" */
/* "movl d%0,d%c" => ="mov%t %y,%z" E="l" A="d%n" 0=0 C="d%c" */
/* "movl a%0,a%c" => ="mov%t %y,%z" E="l" A="a%n" 0=0 C="a%c" */
/* "movl d%0,d%c" => ="mov%t %y,%z" E="l" A="d%n" 0=0 C="d%c" */
/* "movl a%0,a%c" => ="mov%t %y,%z" E="l" A="a%n" 0=0 C="a%c" */
/* "jbsr a%0@" => ="jbsr %y" A="a%n@" 0=0 */
/* "jbsr a%0@" => ="jbsr %y" A="a%n@" 0=0 */
/* "jbsr a%0@; movl d0,sp@-; fmoves sp@+,fp%c" => ="jbsr %y; movl d0,sp@-; fmoves sp@+,%z" A="a%n@" 0=0 C="fp%c" */
/* "jbsr a%0@; movl d1,sp@-; movl d0,sp@-; fmoved sp@+,fp%c" => ="jbsr %y; movl d1,sp@-; movl d0,sp@-; fmoved sp@+,%z" A="a%n@" 0=0 C="fp%c" */
/* "jra a%0@" => ="jra %z" C="a%n@" 2=0 */
/* "%0:" => ="%z:" C="%c" c=a */
/* "lea %0:l,a%c" => ="lea %y,%z" A="%c:l" a=a C="a%c" */
/* "lea a6@(%0),a%c" => ="lea %y,%z" A="a6@(%c)" a=a C="a%c" */
/* "lea a6@(%0),a%c" => ="lea %y,%z" A="a6@(%c)" a=a C="a%c" */
/* "subl d%1,a%c" => ="%f%t %x,%z" D="sub" E="l" B="d%n" 1=1 C="a%c" */
/* "addl d%1,a%c" => ="%f%t %x,%z" D="add" E="l" B="d%n" 1=1 C="a%c" */
/* "subl d%1,d%c" => ="%f%t %x,%z" D="sub" E="l" B="d%n" 1=1 C="d%c" */
/* "subl d%1,d%c" => ="%f%t %x,%z" D="sub" E="l" B="d%n" 1=1 C="d%c" */
/* "eorl d%1,d%c" => ="%f%t %x,%z" D="eor" E="l" B="d%n" 1=1 C="d%c" */
/* "andl d%1,d%c" => ="%f%t %x,%z" D="and" E="l" B="d%n" 1=1 C="d%c" */
/* "addl d%1,d%c" => ="%f%t %x,%z" D="add" E="l" B="d%n" 1=1 C="d%c" */
/* "addl d%1,d%c" => ="%f%t %x,%z" D="add" E="l" B="d%n" 1=1 C="d%c" */
/* "orl d%1,d%c" => ="%f%t %x,%z" D="or" E="l" B="d%n" 1=1 C="d%c" */
/* "lsrl d%1,d%c" => ="%f%t %x,%z" D="lsr" E="l" B="d%n" 1=1 C="d%c" */
/* "asll d%1,d%c" => ="%f%t %x,%z" D="asl" E="l" B="d%n" 1=1 C="d%c" */
/* "divul d%1,d%c" => ="%f%t %x,%z" D="divu" E="l" B="d%n" 1=1 C="d%c" */
/* "mulul d%1,d%c" => ="%f%t %x,%z" D="mulu" E="l" B="d%n" 1=1 C="d%c" */
/* "lsll d%1,d%c" => ="%f%t %x,%z" D="lsl" E="l" B="d%n" 1=1 C="d%c" */
/* "divsl d%1,d%c" => ="%f%t %x,%z" D="divs" E="l" B="d%n" 1=1 C="d%c" */
/* "asrl d%1,d%c" => ="%f%t %x,%z" D="asr" E="l" B="d%n" 1=1 C="d%c" */
/* "mulsl d%1,d%c" => ="%f%t %x,%z" D="muls" E="l" B="d%n" 1=1 C="d%c" */
/* "movl d%0,sp@(%c)" => ="mov%t %y,%z" E="l" A="d%n" 0=0 C="sp@(%c)" */
/* "movl a%0,sp@(%c)" => ="mov%t %y,%z" E="l" A="a%n" 0=0 C="sp@(%c)" */
/* "fmoved fp%1,a%0@" => ="fmove%t %y,%z" E="d" A="fp%n" 0=1 C="a%n@" 2=0 */
/* "fmoves fp%1,a%0@" => ="fmove%t %y,%z" E="s" A="fp%n" 0=1 C="a%n@" 2=0 */
/* "movb d%1,a%0@" => ="mov%t %y,%z" E="b" A="d%n" 0=1 C="a%n@" 2=0 */
/* "movl d%1,a%0@" => ="mov%t %y,%z" E="l" A="d%n" 0=1 C="a%n@" 2=0 */
/* "movw d%1,a%0@" => ="mov%t %y,%z" E="w" A="d%n" 0=1 C="a%n@" 2=0 */
/* "movl a%1,a%0@" => ="mov%t %y,%z" E="l" A="a%n" 0=1 C="a%n@" 2=0 */
/* "fmoved a%0@,fp%c" => ="fmove%t %y,%z" E="d" A="a%n@" 0=0 C="fp%c" */
/* "fmoves a%0@,fp%c" => ="fmove%t %y,%z" E="s" A="a%n@" 0=0 C="fp%c" */
/* "movb a%0@,d%c" => ="mov%t %y,%z" E="b" A="a%n@" 0=0 C="d%c" */
/* "movl a%0@,d%c" => ="mov%t %y,%z" E="l" A="a%n@" 0=0 C="d%c" */
/* "movw a%0@,d%c" => ="mov%t %y,%z" E="w" A="a%n@" 0=0 C="d%c" */
/* "movl a%0@,a%c" => ="mov%t %y,%z" E="l" A="a%n@" 0=0 C="a%c" */
/* "cmpl d%1,d%0; jgt %0" => ="cmp%t %x,%y; j%f %z" E="l" B="d%n" 1=1 A="d%n" 0=0 D="gt" C="%c" c=a */
/* "cmpl d%1,d%0; jne %0" => ="cmp%t %x,%y; j%f %z" E="l" B="d%n" 1=1 A="d%n" 0=0 D="ne" C="%c" c=a */
/* "cmpl d%1,d%0; jeq %0" => ="cmp%t %x,%y; j%f %z" E="l" B="d%n" 1=1 A="d%n" 0=0 D="eq" C="%c" c=a */
/* "cmpl d%1,d%0; jle %0" => ="cmp%t %x,%y; j%f %z" E="l" B="d%n" 1=1 A="d%n" 0=0 D="le" C="%c" c=a */
/* "cmpl d%1,d%0; jge %0" => ="cmp%t %x,%y; j%f %z" E="l" B="d%n" 1=1 A="d%n" 0=0 D="ge" C="%c" c=a */
/* "cmpl d%1,d%0; jlt %0" => ="cmp%t %x,%y; j%f %z" E="l" B="d%n" 1=1 A="d%n" 0=0 D="lt" C="%c" c=a */
/* "cmpl d%1,d%0; jcc %0" => ="cmp%t %x,%y; j%f %z" E="l" B="d%n" 1=1 A="d%n" 0=0 D="cc" C="%c" c=a */
/* "cmpl d%1,d%0; jcs %0" => ="cmp%t %x,%y; j%f %z" E="l" B="d%n" 1=1 A="d%n" 0=0 D="cs" C="%c" c=a */
/* "cmpl d%1,d%0; jhi %0" => ="cmp%t %x,%y; j%f %z" E="l" B="d%n" 1=1 A="d%n" 0=0 D="hi" C="%c" c=a */
/* "cmpl d%1,d%0; jls %0" => ="cmp%t %x,%y; j%f %z" E="l" B="d%n" 1=1 A="d%n" 0=0 D="ls" C="%c" c=a */
/* "movb #%0,d%c" => ="mov%t %y,%z" E="b" A="#%c" a=a C="d%c" */
/* "movl #%0,d%c" => ="mov%t %y,%z" E="l" A="#%c" a=a C="d%c" */
/* "movw #%0,d%c" => ="mov%t %y,%z" E="w" A="#%c" a=a C="d%c" */
/* "movl #%0,d%c" => ="mov%t %y,%z" E="l" A="#%c" a=a C="d%c" */
/* "movl #%0,a%c" => ="mov%t %y,%z" E="l" A="#%c" a=a C="a%c" */
/* "extbl d%c" => ="%f%t %z" D="extb" E="l" C="d%c" */
/* "extl d%c" => ="%f%t %z" D="ext" E="l" C="d%c" */
/* "notl d%c" => ="%f%t %z" D="not" E="l" C="d%c" */
/* "negl d%c" => ="%f%t %z" D="neg" E="l" C="d%c" */
/* "divull d%1,d%c:d0 |mod" => ="%f%t %x,%z |mod" D="divul" E="l" B="d%n" 1=1 C="d%c:d0" */
/* "divsll d%1,d%c:d0 |mod" => ="%f%t %x,%z |mod" D="divsl" E="l" B="d%n" 1=1 C="d%c:d0" */
/* "fsubx fp%1,fp%c" => ="f%f%t %x,%z" D="sub" E="x" B="fp%n" 1=1 C="fp%c" */
/* "fsubx fp%1,fp%c" => ="f%f%t %x,%z" D="sub" E="x" B="fp%n" 1=1 C="fp%c" */
/* "fmulx fp%1,fp%c" => ="f%f%t %x,%z" D="mul" E="x" B="fp%n" 1=1 C="fp%c" */
/* "fmulx fp%1,fp%c" => ="f%f%t %x,%z" D="mul" E="x" B="fp%n" 1=1 C="fp%c" */
/* "fdivx fp%1,fp%c" => ="f%f%t %x,%z" D="div" E="x" B="fp%n" 1=1 C="fp%c" */
/* "fdivx fp%1,fp%c" => ="f%f%t %x,%z" D="div" E="x" B="fp%n" 1=1 C="fp%c" */
/* "faddx fp%1,fp%c" => ="f%f%t %x,%z" D="add" E="x" B="fp%n" 1=1 C="fp%c" */
/* "faddx fp%1,fp%c" => ="f%f%t %x,%z" D="add" E="x" B="fp%n" 1=1 C="fp%c" */
/* "fcmpx fp%1,fp%0; fjgt %0" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" 1=1 A="fp%n" 0=0 D="gt" C="%c" c=a */
/* "fcmpx fp%1,fp%0; fjgt %0" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" 1=1 A="fp%n" 0=0 D="gt" C="%c" c=a */
/* "fcmpx fp%1,fp%0; fjne %0" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" 1=1 A="fp%n" 0=0 D="ne" C="%c" c=a */
/* "fcmpx fp%1,fp%0; fjne %0" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" 1=1 A="fp%n" 0=0 D="ne" C="%c" c=a */
/* "fcmpx fp%1,fp%0; fjeq %0" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" 1=1 A="fp%n" 0=0 D="eq" C="%c" c=a */
/* "fcmpx fp%1,fp%0; fjeq %0" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" 1=1 A="fp%n" 0=0 D="eq" C="%c" c=a */
/* "fcmpx fp%1,fp%0; fjle %0" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" 1=1 A="fp%n" 0=0 D="le" C="%c" c=a */
/* "fcmpx fp%1,fp%0; fjle %0" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" 1=1 A="fp%n" 0=0 D="le" C="%c" c=a */
/* "fcmpx fp%1,fp%0; fjge %0" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" 1=1 A="fp%n" 0=0 D="ge" C="%c" c=a */
/* "fcmpx fp%1,fp%0; fjge %0" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" 1=1 A="fp%n" 0=0 D="ge" C="%c" c=a */
/* "fcmpx fp%1,fp%0; fjlt %0" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" 1=1 A="fp%n" 0=0 D="lt" C="%c" c=a */
/* "fcmpx fp%1,fp%0; fjlt %0" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" 1=1 A="fp%n" 0=0 D="lt" C="%c" c=a */
/* "fmovex fp%0,fp%c" => ="fmove%t %y,%z" E="x" A="fp%n" 0=0 C="fp%c" */
/* "fmovex fp%0,fp%c" => ="fmove%t %y,%z" E="x" A="fp%n" 0=0 C="fp%c" */
/* "fmovex fp%0,fp%c" => ="fmove%t %y,%z" E="x" A="fp%n" 0=0 C="fp%c" */
/* "fmoves fp%0,fp%c" => ="fmove%t %y,%z" E="s" A="fp%n" 0=0 C="fp%c" */
/* "fmovel d%0,fp%c" => ="fmove%t %y,%z" E="l" A="d%n" 0=0 C="fp%c" */
/* "fintrzx fp%0,fp0; fmovel fp0,d%c" => ="fintrzx %y,fp0; fmovel fp0,%z" A="fp%n" 0=0 C="d%c" */
/* "fmoved fp%0,sp@(%c)" => ="fmove%t %y,%z" E="d" A="fp%n" 0=0 C="sp@(%c)" */
/* "fmoves fp%0,sp@(%c)" => ="fmove%t %y,%z" E="s" A="fp%n" 0=0 C="sp@(%c)" */
/* "fnegx fp%0,fp%c" => ="f%f%t %x,%z" D="neg" E="x" B="fp%n" 1=0 C="fp%c" */
/* "fnegx fp%0,fp%c" => ="f%f%t %x,%z" D="neg" E="x" B="fp%n" 1=0 C="fp%c" */
/* "addl d%n,a%c" => ="%f%t %x,%z" D="add" E="l" B="d%n" C="a%c" */
/* "subl d%n,a%c" => ="%f%t %x,%z" D="sub" E="l" B="d%n" C="a%c" */
/* "mov%t d%c,d%c" => ="mov%t %y,%z" A="d%c" C="d%c" */
/* "mov%t a%c,a%c" => ="mov%t %y,%z" A="a%c" C="a%c" */
/* "fmove%t fp%c,fp%c" => ="fmove%t %y,%z" A="fp%c" C="fp%c" */
/* "mov%t a%n,a%c" => ="mov%t %y,%z" A="a%n" C="a%c" */
/* "mov%t %y,sp@(0)" => ="mov%t %y,%z" C="sp@(%c)" c="0" */
/* "addl #%c,a%c" => ="%f%t %x,%z" D="add" E="l" B="#%c" C="a%c" */
/* "lea a%n@(%c),%z" => ="lea %y,%z" A="a%n@(%c)" */
/* "addl a%n,a%c" => ="%f%t %x,%z" D="add" E="l" B="a%n" C="a%c" */
/* "lea %c:l,%z" => ="lea %y,%z" 1A="%c:l" */
/* "lea @(%c:l,d%i:l:1),%z" => ="lea %y,%z" A="@(%c:l,d%i:l:1)" */
/* "addl a%n,a%c" => ="%f%t %x,%z" D="add" E="l" B="a%n" C="a%c" */
/* "lea a6@(%c),%z" => ="lea %y,%z" 1A="a6@(%c)" */
/* "lea a6@(%c,d%i:l:1),%z" => ="lea %y,%z" A="a6@(%c,d%i:l:1)" */
/* "addl a%n,a%c" => ="%f%t %x,%z" D="add" E="l" B="a%n" C="a%c" */
/* "asll #2,%z" => ="%f%t %x,%z" 0D="asl" 0E="l" 0B="#%c" 0b="2" */
/* "lea a%n@(0,d%i:l:4),%z" => ="lea %y,%z" A="a%n@(%c,d%i:l:4)" a="0" */
/* "addl a%n,a%c" => ="%f%t %x,%z" D="add" E="l" B="a%n" C="a%c" */
/* "asll #1,%z" => ="%f%t %x,%z" 0D="asl" 0E="l" 0B="#%c" 0b="1" */
/* "lea a%n@(0,d%i:l:2),%z" => ="lea %y,%z" A="a%n@(%c,d%i:l:2)" a="0" */
/* "lea @(%c:l,d%i:l:1),%z" => ="lea %y,%z" A="@(%c:l,d%i:l:1)" */
/* "asll #2,%z" => ="%f%t %x,%z" 3D="asl" 3E="l" 3B="#%c" 3b="2" */
/* "lea @(%c:l,d%i:l:1),%z" => ="lea %y,%z" A="@(%c:l,d%i:l:1)" */
/* "asll #1,%z" => ="%f%t %x,%z" 3D="asl" 3E="l" 3B="#%c" 3b="1" */
/* "movl a%n,%z" => ="mov%t %y,%z" E="l" A="a%n" */
/* "lea %c:l,%z" => ="lea %y,%z" 0A="%c:l" */
/* "jra a%n@" => ="jra %z" C="a%n@" */
/* "lea %c:l,%z" => ="lea %y,%z" 2A="%c:l" */
/* "jra %c" => ="jra %z" C="%c" */
/* "lea a%n@(%c),a%c" => ="lea %y,%z" A="a%n@(%c)" C="a%c" */
/* "mov%t a%c,%z" => ="mov%t %y,%z" 0A="a%c" */
/* "addl #%c,%z" => ="%f%t %x,%z" D="add" E="l" B="#%c" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" A="d%n" */
/* "movl d%n,%z" => ="mov%t %y,%z" 0E="l" 0A="d%n" */
/* "mov%t a%n,%z" => ="mov%t %y,%z" A="a%n" */
/* "movl a%n,%z" => ="mov%t %y,%z" 0E="l" 0A="a%n" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" A="d%n" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" A="d%n" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" A="d%n" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" A="d%n" */
/* "mov%t a%n,%z" => ="mov%t %y,%z" A="a%n" */
/* "mov%t a%n,%z" => ="mov%t %y,%z" A="a%n" */
/* "mov%t a%n,%z" => ="mov%t %y,%z" A="a%n" */
/* "mov%t a%n,%z" => ="mov%t %y,%z" A="a%n" */
/* "sub%t d%n,d%c" => ="%f%t %x,%z" D="sub" B="d%n" C="d%c" */
/* "sub%t d%n,d%c" => ="%f%t %x,%z" D="sub" B="d%n" C="d%c" */
/* "sub%t d%n,d%c" => ="%f%t %x,%z" D="sub" B="d%n" C="d%c" */
/* "sub%t d%n,d%c" => ="%f%t %x,%z" D="sub" B="d%n" C="d%c" */
/* "sub%t d%n,a%c" => ="%f%t %x,%z" D="sub" B="d%n" C="a%c" */
/* "sub%t d%n,a%c" => ="%f%t %x,%z" D="sub" B="d%n" C="a%c" */
/* "sub%t d%n,a%c" => ="%f%t %x,%z" D="sub" B="d%n" C="a%c" */
/* "sub%t d%n,a%c" => ="%f%t %x,%z" D="sub" B="d%n" C="a%c" */
/* "sub%t a%n,d%c" => ="%f%t %x,%z" D="sub" B="a%n" C="d%c" */
/* "sub%t a%n,d%c" => ="%f%t %x,%z" D="sub" B="a%n" C="d%c" */
/* "sub%t a%n,d%c" => ="%f%t %x,%z" D="sub" B="a%n" C="d%c" */
/* "sub%t a%n,d%c" => ="%f%t %x,%z" D="sub" B="a%n" C="d%c" */
/* "sub%t a%n,a%c" => ="%f%t %x,%z" D="sub" B="a%n" C="a%c" */
/* "sub%t a%n,a%c" => ="%f%t %x,%z" D="sub" B="a%n" C="a%c" */
/* "sub%t a%n,a%c" => ="%f%t %x,%z" D="sub" B="a%n" C="a%c" */
/* "sub%t a%n,a%c" => ="%f%t %x,%z" D="sub" B="a%n" C="a%c" */
/* "add%t d%n,d%c" => ="%f%t %x,%z" D="add" B="d%n" C="d%c" */
/* "add%t d%n,d%c" => ="%f%t %x,%z" D="add" B="d%n" C="d%c" */
/* "add%t d%n,d%c" => ="%f%t %x,%z" D="add" B="d%n" C="d%c" */
/* "add%t d%n,d%c" => ="%f%t %x,%z" D="add" B="d%n" C="d%c" */
/* "add%t d%n,a%c" => ="%f%t %x,%z" D="add" B="d%n" C="a%c" */
/* "add%t d%n,a%c" => ="%f%t %x,%z" D="add" B="d%n" C="a%c" */
/* "add%t d%n,a%c" => ="%f%t %x,%z" D="add" B="d%n" C="a%c" */
/* "add%t d%n,a%c" => ="%f%t %x,%z" D="add" B="d%n" C="a%c" */
/* "add%t a%n,d%c" => ="%f%t %x,%z" D="add" B="a%n" C="d%c" */
/* "add%t a%n,d%c" => ="%f%t %x,%z" D="add" B="a%n" C="d%c" */
/* "add%t a%n,d%c" => ="%f%t %x,%z" D="add" B="a%n" C="d%c" */
/* "add%t a%n,d%c" => ="%f%t %x,%z" D="add" B="a%n" C="d%c" */
/* "add%t a%n,a%c" => ="%f%t %x,%z" D="add" B="a%n" C="a%c" */
/* "add%t a%n,a%c" => ="%f%t %x,%z" D="add" B="a%n" C="a%c" */
/* "add%t a%n,a%c" => ="%f%t %x,%z" D="add" B="a%n" C="a%c" */
/* "add%t a%n,a%c" => ="%f%t %x,%z" D="add" B="a%n" C="a%c" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" 1A="d%n" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" 1A="d%n" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" 1A="d%n" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" 1A="d%n" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t #%c,%z" => ="mov%t %y,%z" 1A="#%c" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t #%c,%z" => ="mov%t %y,%z" 1A="#%c" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t #%c,%z" => ="mov%t %y,%z" 1A="#%c" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t #%c,%z" => ="mov%t %y,%z" 1A="#%c" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@,%z" => ="mov%t %y,%z" 1A="a%n@" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@,%z" => ="mov%t %y,%z" 1A="a%n@" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@,%z" => ="mov%t %y,%z" 1A="a%n@" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@,%z" => ="mov%t %y,%z" 1A="a%n@" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a6@(%c),%z" => ="mov%t %y,%z" 1A="a6@(%c)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a6@(%c),%z" => ="mov%t %y,%z" 1A="a6@(%c)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a6@(%c),%z" => ="mov%t %y,%z" 1A="a6@(%c)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a6@(%c),%z" => ="mov%t %y,%z" 1A="a6@(%c)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:1),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:1)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:1),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:1)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:1),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:1)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:1),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:1)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:2),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:2)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:2),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:2)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:2),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:2)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:2),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:2)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:4),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:4)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:4),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:4)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:4),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:4)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:4),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:4)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t sp@(%c),%z" => ="mov%t %y,%z" 1A="sp@(%c)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t sp@(%c),%z" => ="mov%t %y,%z" 1A="sp@(%c)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t sp@(%c),%z" => ="mov%t %y,%z" 1A="sp@(%c)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t sp@(%c),%z" => ="mov%t %y,%z" 1A="sp@(%c)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:1)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:1)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:1)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:1)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:2)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:2)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:2)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:2)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:4)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:4)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:4)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:4)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t d%c,%z" => ="mov%t %y,%z" 1A="d%c" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t d%c,%z" => ="mov%t %y,%z" 1A="d%c" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t d%c,%z" => ="mov%t %y,%z" 1A="d%c" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t d%c,%z" => ="mov%t %y,%z" 1A="d%c" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c),%z" => ="mov%t %y,%z" 1A="a%n@(%c)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c),%z" => ="mov%t %y,%z" 1A="a%n@(%c)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c),%z" => ="mov%t %y,%z" 1A="a%n@(%c)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c),%z" => ="mov%t %y,%z" 1A="a%n@(%c)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c:l),%z" => ="mov%t %y,%z" 1A="a%n@(%c:l)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c:l),%z" => ="mov%t %y,%z" 1A="a%n@(%c:l)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c:l),%z" => ="mov%t %y,%z" 1A="a%n@(%c:l)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c:l),%z" => ="mov%t %y,%z" 1A="a%n@(%c:l)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:1)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:1)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:1)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:1)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:2)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:2)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:2)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:2)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:4)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:4)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:4)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:4)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t %c:l,%z" => ="mov%t %y,%z" 1A="%c:l" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t %c:l,%z" => ="mov%t %y,%z" 1A="%c:l" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t %c:l,%z" => ="mov%t %y,%z" 1A="%c:l" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t %c:l,%z" => ="mov%t %y,%z" 1A="%c:l" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:1)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:1)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:1)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:1)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:2)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:2)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:2)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:2)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:4)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:4)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:4)" */
/* "and%t d%n,d%c" => ="%f%t %x,%z" D="and" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:4)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" 1A="d%n" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" 1A="d%n" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" 1A="d%n" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" 1A="d%n" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t #%c,%z" => ="mov%t %y,%z" 1A="#%c" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t #%c,%z" => ="mov%t %y,%z" 1A="#%c" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t #%c,%z" => ="mov%t %y,%z" 1A="#%c" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t #%c,%z" => ="mov%t %y,%z" 1A="#%c" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@,%z" => ="mov%t %y,%z" 1A="a%n@" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@,%z" => ="mov%t %y,%z" 1A="a%n@" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@,%z" => ="mov%t %y,%z" 1A="a%n@" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@,%z" => ="mov%t %y,%z" 1A="a%n@" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a6@(%c),%z" => ="mov%t %y,%z" 1A="a6@(%c)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a6@(%c),%z" => ="mov%t %y,%z" 1A="a6@(%c)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a6@(%c),%z" => ="mov%t %y,%z" 1A="a6@(%c)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a6@(%c),%z" => ="mov%t %y,%z" 1A="a6@(%c)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:1),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:1)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:1),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:1)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:1),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:1)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:1),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:1)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:2),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:2)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:2),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:2)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:2),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:2)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:2),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:2)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:4),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:4)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:4),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:4)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:4),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:4)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c:l,d%i:l:4),%z" => ="mov%t %y,%z" 1A="@(%c:l,d%i:l:4)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t sp@(%c),%z" => ="mov%t %y,%z" 1A="sp@(%c)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t sp@(%c),%z" => ="mov%t %y,%z" 1A="sp@(%c)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t sp@(%c),%z" => ="mov%t %y,%z" 1A="sp@(%c)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t sp@(%c),%z" => ="mov%t %y,%z" 1A="sp@(%c)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:1)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:1)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:1)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:1)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:2)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:2)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:2)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:2)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:4)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:4)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:4)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a6@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="a6@(%c,d%i:l:4)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t d%c,%z" => ="mov%t %y,%z" 1A="d%c" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t d%c,%z" => ="mov%t %y,%z" 1A="d%c" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t d%c,%z" => ="mov%t %y,%z" 1A="d%c" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t d%c,%z" => ="mov%t %y,%z" 1A="d%c" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c),%z" => ="mov%t %y,%z" 1A="a%n@(%c)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c),%z" => ="mov%t %y,%z" 1A="a%n@(%c)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c),%z" => ="mov%t %y,%z" 1A="a%n@(%c)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c),%z" => ="mov%t %y,%z" 1A="a%n@(%c)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c:l),%z" => ="mov%t %y,%z" 1A="a%n@(%c:l)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c:l),%z" => ="mov%t %y,%z" 1A="a%n@(%c:l)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c:l),%z" => ="mov%t %y,%z" 1A="a%n@(%c:l)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c:l),%z" => ="mov%t %y,%z" 1A="a%n@(%c:l)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:1)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:1)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:1)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:1)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:2)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:2)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:2)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:2)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:4)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:4)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:4)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t a%n@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="a%n@(%c,d%i:l:4)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t %c:l,%z" => ="mov%t %y,%z" 1A="%c:l" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t %c:l,%z" => ="mov%t %y,%z" 1A="%c:l" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t %c:l,%z" => ="mov%t %y,%z" 1A="%c:l" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t %c:l,%z" => ="mov%t %y,%z" 1A="%c:l" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:1)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:1)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:1)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:1),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:1)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:2)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:2)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:2)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:2),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:2)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:4)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:4)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:4)" */
/* "or%t d%n,d%c" => ="%f%t %x,%z" D="or" B="d%n" C="d%c" */
/* "mov%t @(%c,d%i:l:4),%z" => ="mov%t %y,%z" 1A="@(%c,d%i:l:4)" */
/* "cmp%t d%n,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="d%n" A="d%n" */
/* "cmp%t d%n,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="d%n" A="d%n" */
/* "cmp%t d%n,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="d%n" A="d%n" */
/* "cmp%t d%n,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="d%n" A="d%n" */
/* "cmp%t d%n,d%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="d%n" A="d%c" */
/* "cmp%t d%n,d%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="d%n" A="d%c" */
/* "cmp%t d%n,d%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="d%n" A="d%c" */
/* "cmp%t d%n,d%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="d%n" A="d%c" */
/* "cmp%t d%n,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="d%n" A="a%n" */
/* "cmp%t d%n,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="d%n" A="a%n" */
/* "cmp%t d%n,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="d%n" A="a%n" */
/* "cmp%t d%n,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="d%n" A="a%n" */
/* "cmp%t d%n,a%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="d%n" A="a%c" */
/* "cmp%t d%n,a%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="d%n" A="a%c" */
/* "cmp%t d%n,a%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="d%n" A="a%c" */
/* "cmp%t d%n,a%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="d%n" A="a%c" */
/* "cmp%t a%n,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="a%n" A="d%n" */
/* "cmp%t a%n,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="a%n" A="d%n" */
/* "cmp%t a%n,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="a%n" A="d%n" */
/* "cmp%t a%n,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="a%n" A="d%n" */
/* "cmp%t a%n,d%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="a%n" A="d%c" */
/* "cmp%t a%n,d%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="a%n" A="d%c" */
/* "cmp%t a%n,d%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="a%n" A="d%c" */
/* "cmp%t a%n,d%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="a%n" A="d%c" */
/* "cmp%t a%n,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="a%n" A="a%n" */
/* "cmp%t a%n,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="a%n" A="a%n" */
/* "cmp%t a%n,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="a%n" A="a%n" */
/* "cmp%t a%n,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="a%n" A="a%n" */
/* "cmp%t a%n,a%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="a%n" A="a%c" */
/* "cmp%t a%n,a%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="a%n" A="a%c" */
/* "cmp%t a%n,a%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="a%n" A="a%c" */
/* "cmp%t a%n,a%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="a%n" A="a%c" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" 0A="d%n" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a%n@,%z" => ="mov%t %y,%z" 0A="a%n@" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a6@(%c),%z" => ="mov%t %y,%z" 0A="a6@(%c)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t @(%c:l,d%i:l:1),%z" => ="mov%t %y,%z" 0A="@(%c:l,d%i:l:1)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t @(%c:l,d%i:l:2),%z" => ="mov%t %y,%z" 0A="@(%c:l,d%i:l:2)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t @(%c:l,d%i:l:4),%z" => ="mov%t %y,%z" 0A="@(%c:l,d%i:l:4)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t sp@(%c),%z" => ="mov%t %y,%z" 0A="sp@(%c)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a6@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 0A="a6@(%c,d%i:l:1)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a6@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 0A="a6@(%c,d%i:l:2)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a6@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 0A="a6@(%c,d%i:l:4)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t d%c,%z" => ="mov%t %y,%z" 0A="d%c" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a%n@(%c),%z" => ="mov%t %y,%z" 0A="a%n@(%c)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a%n@(%c:l),%z" => ="mov%t %y,%z" 0A="a%n@(%c:l)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a%n,%z" => ="mov%t %y,%z" 0A="a%n" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a%n@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 0A="a%n@(%c,d%i:l:1)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a%n@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 0A="a%n@(%c,d%i:l:2)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a%n@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 0A="a%n@(%c,d%i:l:4)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a%c,%z" => ="mov%t %y,%z" 0A="a%c" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t %c:l,%z" => ="mov%t %y,%z" 0A="%c:l" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t @(%c,d%i:l:1),%z" => ="mov%t %y,%z" 0A="@(%c,d%i:l:1)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t @(%c,d%i:l:2),%z" => ="mov%t %y,%z" 0A="@(%c,d%i:l:2)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t @(%c,d%i:l:4),%z" => ="mov%t %y,%z" 0A="@(%c,d%i:l:4)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" 0A="d%n" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a%n@,%z" => ="mov%t %y,%z" 0A="a%n@" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a6@(%c),%z" => ="mov%t %y,%z" 0A="a6@(%c)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t @(%c:l,d%i:l:1),%z" => ="mov%t %y,%z" 0A="@(%c:l,d%i:l:1)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t @(%c:l,d%i:l:2),%z" => ="mov%t %y,%z" 0A="@(%c:l,d%i:l:2)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t @(%c:l,d%i:l:4),%z" => ="mov%t %y,%z" 0A="@(%c:l,d%i:l:4)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t sp@(%c),%z" => ="mov%t %y,%z" 0A="sp@(%c)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a6@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 0A="a6@(%c,d%i:l:1)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a6@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 0A="a6@(%c,d%i:l:2)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a6@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 0A="a6@(%c,d%i:l:4)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t d%c,%z" => ="mov%t %y,%z" 0A="d%c" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a%n@(%c),%z" => ="mov%t %y,%z" 0A="a%n@(%c)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a%n@(%c:l),%z" => ="mov%t %y,%z" 0A="a%n@(%c:l)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a%n,%z" => ="mov%t %y,%z" 0A="a%n" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a%n@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 0A="a%n@(%c,d%i:l:1)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a%n@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 0A="a%n@(%c,d%i:l:2)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a%n@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 0A="a%n@(%c,d%i:l:4)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a%c,%z" => ="mov%t %y,%z" 0A="a%c" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t %c:l,%z" => ="mov%t %y,%z" 0A="%c:l" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t @(%c,d%i:l:1),%z" => ="mov%t %y,%z" 0A="@(%c,d%i:l:1)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t @(%c,d%i:l:2),%z" => ="mov%t %y,%z" 0A="@(%c,d%i:l:2)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t @(%c,d%i:l:4),%z" => ="mov%t %y,%z" 0A="@(%c,d%i:l:4)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" 0A="d%n" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a%n@,%z" => ="mov%t %y,%z" 0A="a%n@" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a6@(%c),%z" => ="mov%t %y,%z" 0A="a6@(%c)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t @(%c:l,d%i:l:1),%z" => ="mov%t %y,%z" 0A="@(%c:l,d%i:l:1)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t @(%c:l,d%i:l:2),%z" => ="mov%t %y,%z" 0A="@(%c:l,d%i:l:2)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t @(%c:l,d%i:l:4),%z" => ="mov%t %y,%z" 0A="@(%c:l,d%i:l:4)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t sp@(%c),%z" => ="mov%t %y,%z" 0A="sp@(%c)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a6@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 0A="a6@(%c,d%i:l:1)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a6@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 0A="a6@(%c,d%i:l:2)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a6@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 0A="a6@(%c,d%i:l:4)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t d%c,%z" => ="mov%t %y,%z" 0A="d%c" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a%n@(%c),%z" => ="mov%t %y,%z" 0A="a%n@(%c)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a%n@(%c:l),%z" => ="mov%t %y,%z" 0A="a%n@(%c:l)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a%n,%z" => ="mov%t %y,%z" 0A="a%n" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a%n@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 0A="a%n@(%c,d%i:l:1)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a%n@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 0A="a%n@(%c,d%i:l:2)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a%n@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 0A="a%n@(%c,d%i:l:4)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t a%c,%z" => ="mov%t %y,%z" 0A="a%c" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t %c:l,%z" => ="mov%t %y,%z" 0A="%c:l" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t @(%c,d%i:l:1),%z" => ="mov%t %y,%z" 0A="@(%c,d%i:l:1)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t @(%c,d%i:l:2),%z" => ="mov%t %y,%z" 0A="@(%c,d%i:l:2)" */
/* "cmp%t #%c,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="d%n" */
/* "mov%t @(%c,d%i:l:4),%z" => ="mov%t %y,%z" 0A="@(%c,d%i:l:4)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" 0A="d%n" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a%n@,%z" => ="mov%t %y,%z" 0A="a%n@" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a6@(%c),%z" => ="mov%t %y,%z" 0A="a6@(%c)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t @(%c:l,d%i:l:1),%z" => ="mov%t %y,%z" 0A="@(%c:l,d%i:l:1)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t @(%c:l,d%i:l:2),%z" => ="mov%t %y,%z" 0A="@(%c:l,d%i:l:2)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t @(%c:l,d%i:l:4),%z" => ="mov%t %y,%z" 0A="@(%c:l,d%i:l:4)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t sp@(%c),%z" => ="mov%t %y,%z" 0A="sp@(%c)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a6@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 0A="a6@(%c,d%i:l:1)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a6@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 0A="a6@(%c,d%i:l:2)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a6@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 0A="a6@(%c,d%i:l:4)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t d%c,%z" => ="mov%t %y,%z" 0A="d%c" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a%n@(%c),%z" => ="mov%t %y,%z" 0A="a%n@(%c)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a%n@(%c:l),%z" => ="mov%t %y,%z" 0A="a%n@(%c:l)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a%n,%z" => ="mov%t %y,%z" 0A="a%n" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a%n@(%c,d%i:l:1),%z" => ="mov%t %y,%z" 0A="a%n@(%c,d%i:l:1)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a%n@(%c,d%i:l:2),%z" => ="mov%t %y,%z" 0A="a%n@(%c,d%i:l:2)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a%n@(%c,d%i:l:4),%z" => ="mov%t %y,%z" 0A="a%n@(%c,d%i:l:4)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t a%c,%z" => ="mov%t %y,%z" 0A="a%c" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t %c:l,%z" => ="mov%t %y,%z" 0A="%c:l" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t @(%c,d%i:l:1),%z" => ="mov%t %y,%z" 0A="@(%c,d%i:l:1)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t @(%c,d%i:l:2),%z" => ="mov%t %y,%z" 0A="@(%c,d%i:l:2)" */
/* "cmp%t #%c,a%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" A="a%n" */
/* "mov%t @(%c,d%i:l:4),%z" => ="mov%t %y,%z" 0A="@(%c,d%i:l:4)" */
/* "cmpl d%n,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" E="l" B="d%n" A="d%n" */
/* "extbl %z" => ="%f%t %z" 0D="extb" 0E="l" */
/* "extbl %z" => ="%f%t %z" 1D="extb" 1E="l" */
/* "cmpl d%n,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" E="l" B="d%n" A="d%n" */
/* "extl %z" => ="%f%t %z" 0D="ext" 0E="l" */
/* "extl %z" => ="%f%t %z" 1D="ext" 1E="l" */
/* "cmpl #%c,d%n; jgt %z" => ="cmp%t %x,%y; j%f %z" E="l" B="#%c" A="d%n" D="gt" */
/* "extbl %z" => ="%f%t %z" 0D="extb" 0E="l" */
/* "cmpl #%c,d%n; jne %z" => ="cmp%t %x,%y; j%f %z" E="l" B="#%c" A="d%n" D="ne" */
/* "extbl %z" => ="%f%t %z" 0D="extb" 0E="l" */
/* "cmpl #%c,d%n; jeq %z" => ="cmp%t %x,%y; j%f %z" E="l" B="#%c" A="d%n" D="eq" */
/* "extbl %z" => ="%f%t %z" 0D="extb" 0E="l" */
/* "cmpl #%c,d%n; jle %z" => ="cmp%t %x,%y; j%f %z" E="l" B="#%c" A="d%n" D="le" */
/* "extbl %z" => ="%f%t %z" 0D="extb" 0E="l" */
/* "cmpl #%c,d%n; jge %z" => ="cmp%t %x,%y; j%f %z" E="l" B="#%c" A="d%n" D="ge" */
/* "extbl %z" => ="%f%t %z" 0D="extb" 0E="l" */
/* "cmpl #%c,d%n; jlt %z" => ="cmp%t %x,%y; j%f %z" E="l" B="#%c" A="d%n" D="lt" */
/* "extbl %z" => ="%f%t %z" 0D="extb" 0E="l" */
/* "cmpl #%c,d%n; jgt %z" => ="cmp%t %x,%y; j%f %z" E="l" B="#%c" A="d%n" D="gt" */
/* "extl %z" => ="%f%t %z" 0D="ext" 0E="l" */
/* "cmpl #%c,d%n; jne %z" => ="cmp%t %x,%y; j%f %z" E="l" B="#%c" A="d%n" D="ne" */
/* "extl %z" => ="%f%t %z" 0D="ext" 0E="l" */
/* "cmpl #%c,d%n; jeq %z" => ="cmp%t %x,%y; j%f %z" E="l" B="#%c" A="d%n" D="eq" */
/* "extl %z" => ="%f%t %z" 0D="ext" 0E="l" */
/* "cmpl #%c,d%n; jle %z" => ="cmp%t %x,%y; j%f %z" E="l" B="#%c" A="d%n" D="le" */
/* "extl %z" => ="%f%t %z" 0D="ext" 0E="l" */
/* "cmpl #%c,d%n; jge %z" => ="cmp%t %x,%y; j%f %z" E="l" B="#%c" A="d%n" D="ge" */
/* "extl %z" => ="%f%t %z" 0D="ext" 0E="l" */
/* "cmpl #%c,d%n; jlt %z" => ="cmp%t %x,%y; j%f %z" E="l" B="#%c" A="d%n" D="lt" */
/* "extl %z" => ="%f%t %z" 0D="ext" 0E="l" */
/* "lsr%t d%n,d%c" => ="%f%t %x,%z" D="lsr" B="d%n" C="d%c" */
/* "mov%t #4,%z" => ="mov%t %y,%z" 1A="#%c" 1a="4" */
/* "lsr%t d%n,d%c" => ="%f%t %x,%z" D="lsr" B="d%n" C="d%c" */
/* "mov%t #8,%z" => ="mov%t %y,%z" 1A="#%c" 1a="8" */
/* "lsr%t d%n,d%c" => ="%f%t %x,%z" D="lsr" B="d%n" C="d%c" */
/* "mov%t #1,%z" => ="mov%t %y,%z" 1A="#%c" 1a="1" */
/* "lsr%t d%n,d%c" => ="%f%t %x,%z" D="lsr" B="d%n" C="d%c" */
/* "mov%t #5,%z" => ="mov%t %y,%z" 1A="#%c" 1a="5" */
/* "lsr%t d%n,d%c" => ="%f%t %x,%z" D="lsr" B="d%n" C="d%c" */
/* "mov%t #2,%z" => ="mov%t %y,%z" 1A="#%c" 1a="2" */
/* "lsr%t d%n,d%c" => ="%f%t %x,%z" D="lsr" B="d%n" C="d%c" */
/* "mov%t #6,%z" => ="mov%t %y,%z" 1A="#%c" 1a="6" */
/* "lsr%t d%n,d%c" => ="%f%t %x,%z" D="lsr" B="d%n" C="d%c" */
/* "mov%t #3,%z" => ="mov%t %y,%z" 1A="#%c" 1a="3" */
/* "lsr%t d%n,d%c" => ="%f%t %x,%z" D="lsr" B="d%n" C="d%c" */
/* "mov%t #7,%z" => ="mov%t %y,%z" 1A="#%c" 1a="7" */
/* "asl%t d%n,d%c" => ="%f%t %x,%z" D="asl" B="d%n" C="d%c" */
/* "mov%t #4,%z" => ="mov%t %y,%z" 1A="#%c" 1a="4" */
/* "asl%t d%n,d%c" => ="%f%t %x,%z" D="asl" B="d%n" C="d%c" */
/* "mov%t #8,%z" => ="mov%t %y,%z" 1A="#%c" 1a="8" */
/* "asl%t d%n,d%c" => ="%f%t %x,%z" D="asl" B="d%n" C="d%c" */
/* "mov%t #1,%z" => ="mov%t %y,%z" 1A="#%c" 1a="1" */
/* "asl%t d%n,d%c" => ="%f%t %x,%z" D="asl" B="d%n" C="d%c" */
/* "mov%t #5,%z" => ="mov%t %y,%z" 1A="#%c" 1a="5" */
/* "asl%t d%n,d%c" => ="%f%t %x,%z" D="asl" B="d%n" C="d%c" */
/* "mov%t #2,%z" => ="mov%t %y,%z" 1A="#%c" 1a="2" */
/* "asl%t d%n,d%c" => ="%f%t %x,%z" D="asl" B="d%n" C="d%c" */
/* "mov%t #6,%z" => ="mov%t %y,%z" 1A="#%c" 1a="6" */
/* "asl%t d%n,d%c" => ="%f%t %x,%z" D="asl" B="d%n" C="d%c" */
/* "mov%t #3,%z" => ="mov%t %y,%z" 1A="#%c" 1a="3" */
/* "asl%t d%n,d%c" => ="%f%t %x,%z" D="asl" B="d%n" C="d%c" */
/* "mov%t #7,%z" => ="mov%t %y,%z" 1A="#%c" 1a="7" */
/* "lsl%t d%n,d%c" => ="%f%t %x,%z" D="lsl" B="d%n" C="d%c" */
/* "mov%t #4,%z" => ="mov%t %y,%z" 1A="#%c" 1a="4" */
/* "lsl%t d%n,d%c" => ="%f%t %x,%z" D="lsl" B="d%n" C="d%c" */
/* "mov%t #8,%z" => ="mov%t %y,%z" 1A="#%c" 1a="8" */
/* "lsl%t d%n,d%c" => ="%f%t %x,%z" D="lsl" B="d%n" C="d%c" */
/* "mov%t #1,%z" => ="mov%t %y,%z" 1A="#%c" 1a="1" */
/* "lsl%t d%n,d%c" => ="%f%t %x,%z" D="lsl" B="d%n" C="d%c" */
/* "mov%t #5,%z" => ="mov%t %y,%z" 1A="#%c" 1a="5" */
/* "lsl%t d%n,d%c" => ="%f%t %x,%z" D="lsl" B="d%n" C="d%c" */
/* "mov%t #2,%z" => ="mov%t %y,%z" 1A="#%c" 1a="2" */
/* "lsl%t d%n,d%c" => ="%f%t %x,%z" D="lsl" B="d%n" C="d%c" */
/* "mov%t #6,%z" => ="mov%t %y,%z" 1A="#%c" 1a="6" */
/* "lsl%t d%n,d%c" => ="%f%t %x,%z" D="lsl" B="d%n" C="d%c" */
/* "mov%t #3,%z" => ="mov%t %y,%z" 1A="#%c" 1a="3" */
/* "lsl%t d%n,d%c" => ="%f%t %x,%z" D="lsl" B="d%n" C="d%c" */
/* "mov%t #7,%z" => ="mov%t %y,%z" 1A="#%c" 1a="7" */
/* "asr%t d%n,d%c" => ="%f%t %x,%z" D="asr" B="d%n" C="d%c" */
/* "mov%t #4,%z" => ="mov%t %y,%z" 1A="#%c" 1a="4" */
/* "asr%t d%n,d%c" => ="%f%t %x,%z" D="asr" B="d%n" C="d%c" */
/* "mov%t #8,%z" => ="mov%t %y,%z" 1A="#%c" 1a="8" */
/* "asr%t d%n,d%c" => ="%f%t %x,%z" D="asr" B="d%n" C="d%c" */
/* "mov%t #1,%z" => ="mov%t %y,%z" 1A="#%c" 1a="1" */
/* "asr%t d%n,d%c" => ="%f%t %x,%z" D="asr" B="d%n" C="d%c" */
/* "mov%t #5,%z" => ="mov%t %y,%z" 1A="#%c" 1a="5" */
/* "asr%t d%n,d%c" => ="%f%t %x,%z" D="asr" B="d%n" C="d%c" */
/* "mov%t #2,%z" => ="mov%t %y,%z" 1A="#%c" 1a="2" */
/* "asr%t d%n,d%c" => ="%f%t %x,%z" D="asr" B="d%n" C="d%c" */
/* "mov%t #6,%z" => ="mov%t %y,%z" 1A="#%c" 1a="6" */
/* "asr%t d%n,d%c" => ="%f%t %x,%z" D="asr" B="d%n" C="d%c" */
/* "mov%t #3,%z" => ="mov%t %y,%z" 1A="#%c" 1a="3" */
/* "asr%t d%n,d%c" => ="%f%t %x,%z" D="asr" B="d%n" C="d%c" */
/* "mov%t #7,%z" => ="mov%t %y,%z" 1A="#%c" 1a="7" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" A="d%n" */
/* "sub%t d%n,%z" => ="%f%t %x,%z" 0D="sub" 0B="d%n" */
/* "sub%t %x,%z" => ="%f%t %x,%z" D="sub" */
/* "mov%t a%n,%z" => ="mov%t %y,%z" A="a%n" */
/* "sub%t d%n,%z" => ="%f%t %x,%z" 0D="sub" 0B="d%n" */
/* "sub%t %x,%z" => ="%f%t %x,%z" D="sub" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" A="d%n" */
/* "add%t d%n,%z" => ="%f%t %x,%z" 0D="add" 0B="d%n" */
/* "add%t %x,%z" => ="%f%t %x,%z" D="add" */
/* "mov%t a%n,%z" => ="mov%t %y,%z" A="a%n" */
/* "add%t d%n,%z" => ="%f%t %x,%z" 0D="add" 0B="d%n" */
/* "add%t %x,%z" => ="%f%t %x,%z" D="add" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" A="d%n" */
/* "sub%t #%c,%z" => ="%f%t %x,%z" 0D="sub" 0B="#%c" */
/* "sub%t %x,%z" => ="%f%t %x,%z" D="sub" */
/* "mov%t a%n,%z" => ="mov%t %y,%z" A="a%n" */
/* "sub%t #%c,%z" => ="%f%t %x,%z" 0D="sub" 0B="#%c" */
/* "sub%t %x,%z" => ="%f%t %x,%z" D="sub" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" A="d%n" */
/* "add%t #%c,%z" => ="%f%t %x,%z" 0D="add" 0B="#%c" */
/* "add%t %x,%z" => ="%f%t %x,%z" D="add" */
/* "mov%t a%n,%z" => ="mov%t %y,%z" A="a%n" */
/* "add%t #%c,%z" => ="%f%t %x,%z" 0D="add" 0B="#%c" */
/* "add%t %x,%z" => ="%f%t %x,%z" D="add" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" A="d%n" */
/* "sub%t a%n,%z" => ="%f%t %x,%z" 0D="sub" 0B="a%n" */
/* "sub%t %x,%z" => ="%f%t %x,%z" D="sub" */
/* "mov%t a%n,%z" => ="mov%t %y,%z" A="a%n" */
/* "sub%t a%n,%z" => ="%f%t %x,%z" 0D="sub" 0B="a%n" */
/* "sub%t %x,%z" => ="%f%t %x,%z" D="sub" */
/* "mov%t d%n,%z" => ="mov%t %y,%z" A="d%n" */
/* "add%t a%n,%z" => ="%f%t %x,%z" 0D="add" 0B="a%n" */
/* "add%t %x,%z" => ="%f%t %x,%z" D="add" */
/* "mov%t a%n,%z" => ="mov%t %y,%z" A="a%n" */
/* "add%t a%n,%z" => ="%f%t %x,%z" 0D="add" 0B="a%n" */
/* "add%t %x,%z" => ="%f%t %x,%z" D="add" */
/* "fmovex fp%n,fp%c" => ="fmove%t %y,%z" E="x" A="fp%n" C="fp%c" */
/* "fmovex fp%n,fp%c" => ="fmove%t %y,%z" E="x" A="fp%n" C="fp%c" */
/* "fmovex fp%n,fp%c" => ="fmove%t %y,%z" E="x" A="fp%n" C="fp%c" */
/* "fmovex fp%n,fp%c" => ="fmove%t %y,%z" E="x" A="fp%n" C="fp%c" */
/* "fmove%t fp%n,%z" => ="fmove%t %y,%z" A="fp%n" */
/* "fmove%t fp%n,%z" => ="fmove%t %y,%z" 0A="fp%n" */
/* "fmove%t fp%n,%z" => ="fmove%t %y,%z" A="fp%n" */
/* "fmove%t fp%n,%z" => ="fmove%t %y,%z" 0A="fp%n" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t d%n,%z" => ="fmove%t %y,%z" 1A="d%n" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t d%n,%z" => ="fmove%t %y,%z" 1A="d%n" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t d%n,%z" => ="fmove%t %y,%z" 1A="d%n" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t d%n,%z" => ="fmove%t %y,%z" 1A="d%n" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@,%z" => ="fmove%t %y,%z" 1A="a%n@" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@,%z" => ="fmove%t %y,%z" 1A="a%n@" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@,%z" => ="fmove%t %y,%z" 1A="a%n@" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@,%z" => ="fmove%t %y,%z" 1A="a%n@" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a6@(%c),%z" => ="fmove%t %y,%z" 1A="a6@(%c)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a6@(%c),%z" => ="fmove%t %y,%z" 1A="a6@(%c)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a6@(%c),%z" => ="fmove%t %y,%z" 1A="a6@(%c)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a6@(%c),%z" => ="fmove%t %y,%z" 1A="a6@(%c)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c:l,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:1)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c:l,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:1)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c:l,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:1)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c:l,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:1)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c:l,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:2)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c:l,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:2)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c:l,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:2)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c:l,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:2)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c:l,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:4)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c:l,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:4)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c:l,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:4)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c:l,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:4)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t sp@(%c),%z" => ="fmove%t %y,%z" 1A="sp@(%c)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t sp@(%c),%z" => ="fmove%t %y,%z" 1A="sp@(%c)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t sp@(%c),%z" => ="fmove%t %y,%z" 1A="sp@(%c)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t sp@(%c),%z" => ="fmove%t %y,%z" 1A="sp@(%c)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a6@(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:1)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a6@(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:1)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a6@(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:1)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a6@(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:1)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a6@(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:2)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a6@(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:2)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a6@(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:2)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a6@(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:2)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a6@(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:4)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a6@(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:4)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a6@(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:4)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a6@(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:4)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t d%c,%z" => ="fmove%t %y,%z" 1A="d%c" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t d%c,%z" => ="fmove%t %y,%z" 1A="d%c" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t d%c,%z" => ="fmove%t %y,%z" 1A="d%c" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t d%c,%z" => ="fmove%t %y,%z" 1A="d%c" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c),%z" => ="fmove%t %y,%z" 1A="a%n@(%c)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c),%z" => ="fmove%t %y,%z" 1A="a%n@(%c)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c),%z" => ="fmove%t %y,%z" 1A="a%n@(%c)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c),%z" => ="fmove%t %y,%z" 1A="a%n@(%c)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c:l),%z" => ="fmove%t %y,%z" 1A="a%n@(%c:l)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c:l),%z" => ="fmove%t %y,%z" 1A="a%n@(%c:l)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c:l),%z" => ="fmove%t %y,%z" 1A="a%n@(%c:l)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c:l),%z" => ="fmove%t %y,%z" 1A="a%n@(%c:l)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:1)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:1)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:1)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:1)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:2)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:2)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:2)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:2)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:4)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:4)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:4)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t a%n@(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:4)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t %c:l,%z" => ="fmove%t %y,%z" 1A="%c:l" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t %c:l,%z" => ="fmove%t %y,%z" 1A="%c:l" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t %c:l,%z" => ="fmove%t %y,%z" 1A="%c:l" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t %c:l,%z" => ="fmove%t %y,%z" 1A="%c:l" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:1)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:1)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:1)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:1)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:2)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:2)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:2)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:2)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:4)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:4)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:4)" */
/* "fcmpx fp%n,fp%n; fj%f %z" => ="fcmp%t %x,%y; fj%f %z" E="x" B="fp%n" A="fp%n" */
/* "fmove%t @(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:4)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t d%n,%z" => ="fmove%t %y,%z" 1A="d%n" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t d%n,%z" => ="fmove%t %y,%z" 1A="d%n" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t d%n,%z" => ="fmove%t %y,%z" 1A="d%n" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t d%n,%z" => ="fmove%t %y,%z" 1A="d%n" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@,%z" => ="fmove%t %y,%z" 1A="a%n@" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@,%z" => ="fmove%t %y,%z" 1A="a%n@" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@,%z" => ="fmove%t %y,%z" 1A="a%n@" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@,%z" => ="fmove%t %y,%z" 1A="a%n@" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a6@(%c),%z" => ="fmove%t %y,%z" 1A="a6@(%c)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a6@(%c),%z" => ="fmove%t %y,%z" 1A="a6@(%c)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a6@(%c),%z" => ="fmove%t %y,%z" 1A="a6@(%c)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a6@(%c),%z" => ="fmove%t %y,%z" 1A="a6@(%c)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c:l,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:1)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c:l,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:1)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c:l,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:1)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c:l,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:1)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c:l,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:2)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c:l,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:2)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c:l,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:2)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c:l,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:2)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c:l,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:4)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c:l,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:4)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c:l,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:4)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c:l,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="@(%c:l,d%i:l:4)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t sp@(%c),%z" => ="fmove%t %y,%z" 1A="sp@(%c)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t sp@(%c),%z" => ="fmove%t %y,%z" 1A="sp@(%c)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t sp@(%c),%z" => ="fmove%t %y,%z" 1A="sp@(%c)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t sp@(%c),%z" => ="fmove%t %y,%z" 1A="sp@(%c)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a6@(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:1)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a6@(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:1)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a6@(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:1)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a6@(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:1)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a6@(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:2)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a6@(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:2)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a6@(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:2)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a6@(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:2)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a6@(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:4)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a6@(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:4)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a6@(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:4)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a6@(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="a6@(%c,d%i:l:4)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t d%c,%z" => ="fmove%t %y,%z" 1A="d%c" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t d%c,%z" => ="fmove%t %y,%z" 1A="d%c" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t d%c,%z" => ="fmove%t %y,%z" 1A="d%c" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t d%c,%z" => ="fmove%t %y,%z" 1A="d%c" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c),%z" => ="fmove%t %y,%z" 1A="a%n@(%c)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c),%z" => ="fmove%t %y,%z" 1A="a%n@(%c)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c),%z" => ="fmove%t %y,%z" 1A="a%n@(%c)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c),%z" => ="fmove%t %y,%z" 1A="a%n@(%c)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c:l),%z" => ="fmove%t %y,%z" 1A="a%n@(%c:l)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c:l),%z" => ="fmove%t %y,%z" 1A="a%n@(%c:l)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c:l),%z" => ="fmove%t %y,%z" 1A="a%n@(%c:l)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c:l),%z" => ="fmove%t %y,%z" 1A="a%n@(%c:l)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:1)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:1)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:1)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:1)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:2)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:2)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:2)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:2)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:4)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:4)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:4)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t a%n@(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="a%n@(%c,d%i:l:4)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t %c:l,%z" => ="fmove%t %y,%z" 1A="%c:l" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t %c:l,%z" => ="fmove%t %y,%z" 1A="%c:l" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t %c:l,%z" => ="fmove%t %y,%z" 1A="%c:l" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t %c:l,%z" => ="fmove%t %y,%z" 1A="%c:l" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:1)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:1)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:1)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c,d%i:l:1),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:1)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:2)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:2)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:2)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c,d%i:l:2),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:2)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:4)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:4)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:4)" */
/* "f%fx fp%n,fp%c" => ="f%f%t %x,%z" E="x" B="fp%n" C="fp%c" */
/* "fmove%t @(%c,d%i:l:4),%z" => ="fmove%t %y,%z" 1A="@(%c,d%i:l:4)" */
/* "mov%t #0,d%n" => ="mov%t %y,%z" A="#%c" a="0" C="d%n" */
/* "mov%t #0,a%n@" => ="mov%t %y,%z" A="#%c" a="0" C="a%n@" */
/* "mov%t #0,a6@(%c)" => ="mov%t %y,%z" A="#%c" a="0" C="a6@(%c)" */
/* "mov%t #0,@(%c:l,d%i:l:1)" => ="mov%t %y,%z" A="#%c" a="0" C="@(%c:l,d%i:l:1)" */
/* "mov%t #0,@(%c:l,d%i:l:2)" => ="mov%t %y,%z" A="#%c" a="0" C="@(%c:l,d%i:l:2)" */
/* "mov%t #0,@(%c:l,d%i:l:4)" => ="mov%t %y,%z" A="#%c" a="0" C="@(%c:l,d%i:l:4)" */
/* "mov%t #0,sp@(%c)" => ="mov%t %y,%z" A="#%c" a="0" C="sp@(%c)" */
/* "mov%t #0,a6@(%c,d%i:l:1)" => ="mov%t %y,%z" A="#%c" a="0" C="a6@(%c,d%i:l:1)" */
/* "mov%t #0,a6@(%c,d%i:l:2)" => ="mov%t %y,%z" A="#%c" a="0" C="a6@(%c,d%i:l:2)" */
/* "mov%t #0,a6@(%c,d%i:l:4)" => ="mov%t %y,%z" A="#%c" a="0" C="a6@(%c,d%i:l:4)" */
/* "mov%t #0,d%c" => ="mov%t %y,%z" A="#%c" a="0" C="d%c" */
/* "mov%t #0,a%n@(%c)" => ="mov%t %y,%z" A="#%c" a="0" C="a%n@(%c)" */
/* "mov%t #0,a%n@(%c:l)" => ="mov%t %y,%z" A="#%c" a="0" C="a%n@(%c:l)" */
/* "mov%t #0,a%n@(%c,d%i:l:1)" => ="mov%t %y,%z" A="#%c" a="0" C="a%n@(%c,d%i:l:1)" */
/* "mov%t #0,a%n@(%c,d%i:l:2)" => ="mov%t %y,%z" A="#%c" a="0" C="a%n@(%c,d%i:l:2)" */
/* "mov%t #0,a%n@(%c,d%i:l:4)" => ="mov%t %y,%z" A="#%c" a="0" C="a%n@(%c,d%i:l:4)" */
/* "mov%t #0,%c:l" => ="mov%t %y,%z" A="#%c" a="0" C="%c:l" */
/* "mov%t #0,@(%c,d%i:l:1)" => ="mov%t %y,%z" A="#%c" a="0" C="@(%c,d%i:l:1)" */
/* "mov%t #0,@(%c,d%i:l:2)" => ="mov%t %y,%z" A="#%c" a="0" C="@(%c,d%i:l:2)" */
/* "mov%t #0,@(%c,d%i:l:4)" => ="mov%t %y,%z" A="#%c" a="0" C="@(%c,d%i:l:4)" */
/* "cmp%t #0,d%n; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="d%n" */
/* "cmp%t #0,a%n@; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="a%n@" */
/* "cmp%t #0,a6@(%c); j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="a6@(%c)" */
/* "cmp%t #0,@(%c:l,d%i:l:1); j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="@(%c:l,d%i:l:1)" */
/* "cmp%t #0,@(%c:l,d%i:l:2); j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="@(%c:l,d%i:l:2)" */
/* "cmp%t #0,@(%c:l,d%i:l:4); j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="@(%c:l,d%i:l:4)" */
/* "cmp%t #0,sp@(%c); j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="sp@(%c)" */
/* "cmp%t #0,a6@(%c,d%i:l:1); j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="a6@(%c,d%i:l:1)" */
/* "cmp%t #0,a6@(%c,d%i:l:2); j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="a6@(%c,d%i:l:2)" */
/* "cmp%t #0,a6@(%c,d%i:l:4); j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="a6@(%c,d%i:l:4)" */
/* "cmp%t #0,d%c; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="d%c" */
/* "cmp%t #0,a%n@(%c); j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="a%n@(%c)" */
/* "cmp%t #0,a%n@(%c:l); j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="a%n@(%c:l)" */
/* "cmp%t #0,a%n@(%c,d%i:l:1); j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="a%n@(%c,d%i:l:1)" */
/* "cmp%t #0,a%n@(%c,d%i:l:2); j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="a%n@(%c,d%i:l:2)" */
/* "cmp%t #0,a%n@(%c,d%i:l:4); j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="a%n@(%c,d%i:l:4)" */
/* "cmp%t #0,%c:l; j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="%c:l" */
/* "cmp%t #0,@(%c,d%i:l:1); j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="@(%c,d%i:l:1)" */
/* "cmp%t #0,@(%c,d%i:l:2); j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="@(%c,d%i:l:2)" */
/* "cmp%t #0,@(%c,d%i:l:4); j%f %z" => ="cmp%t %x,%y; j%f %z" B="#%c" b="0" A="@(%c,d%i:l:4)" */
/* "sub%t #4,%z" => ="%f%t %x,%z" D="sub" B="#%c" b="4" */
/* "sub%t #8,%z" => ="%f%t %x,%z" D="sub" B="#%c" b="8" */
/* "sub%t #1,%z" => ="%f%t %x,%z" D="sub" B="#%c" b="1" */
/* "sub%t #5,%z" => ="%f%t %x,%z" D="sub" B="#%c" b="5" */
/* "sub%t #2,%z" => ="%f%t %x,%z" D="sub" B="#%c" b="2" */
/* "sub%t #6,%z" => ="%f%t %x,%z" D="sub" B="#%c" b="6" */
/* "sub%t #3,%z" => ="%f%t %x,%z" D="sub" B="#%c" b="3" */
/* "sub%t #7,%z" => ="%f%t %x,%z" D="sub" B="#%c" b="7" */
/* "add%t #4,%z" => ="%f%t %x,%z" D="add" B="#%c" b="4" */
/* "add%t #8,%z" => ="%f%t %x,%z" D="add" B="#%c" b="8" */
/* "add%t #1,%z" => ="%f%t %x,%z" D="add" B="#%c" b="1" */
/* "add%t #5,%z" => ="%f%t %x,%z" D="add" B="#%c" b="5" */
/* "add%t #2,%z" => ="%f%t %x,%z" D="add" B="#%c" b="2" */
/* "add%t #6,%z" => ="%f%t %x,%z" D="add" B="#%c" b="6" */
/* "add%t #3,%z" => ="%f%t %x,%z" D="add" B="#%c" b="3" */
/* "add%t #7,%z" => ="%f%t %x,%z" D="add" B="#%c" b="7" */
/* "f%f%t %x,fp%c" => ="f%f%t %x,%z" C="fp%c" */
/* "fmovex %y,%z" => ="fmove%t %y,%z" E="x" */
/* "fmoves fp%n,fp%c" => ="fmove%t %y,%z" E="s" A="fp%n" C="fp%c" */
/* "fmoves fp%n,d0; fmoves d0,fp%c" => ="fmoves %y,d0; fmoves d0,%z" A="fp%n" C="fp%c" */
#include "pseudos.c"

static char *shortopcodes[] = {
/* 608 */  "fmoves %y,d0; fmoves d0,%z",
/* 609 */  "%f%t %x,%z |mod",
/* 610 */  "fintrzx %y,fp0; fmovel fp0,%z",
/* 611 */  "jbsr %y",
/* 612 */  "jbsr %y; movl d0,sp@-; fmoves sp@+,%z",
/* 613 */  "jbsr %y; movl d1,sp@-; movl d0,sp@-; fmoved sp@+,%z",
/* 614 */  "lea %z,a1; movl %x,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b",
/* 615 */  "movl %x,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b",
/* 616 */  "# label",
/* 617 */  "# nop",
/* 618 */  "# ret",
/* 619 */  "%f%t %x,%z",
/* 620 */  "%f%t %z",
/* 621 */  "clr%t %z",
/* 622 */  "cmp%t %x,%y; j%f %z",
/* 623 */  "f%f%t %x,%z",
/* 624 */  "fcmp%t %x,%y; fj%f %z",
/* 625 */  "fmove%t %y,%z",
/* 626 */  "fmovex %y,%z; f%f%t %x,%z",
/* 627 */  "jra %z",
/* 628 */  "lea %y,%z",
/* 629 */  "mov%t %y,%z",
/* 630 */  "mov%t %y,%z; %f%t %x,%z",
/* 631 */  "mov%t %y,%z; %f%t %z",
/* 632 */  "tst%t %y; j%f %z",
/* 633 */  "%z:",
};

static char *asmstr0[] = {
	"(DUMMY)",
	"a%n",
	"a%c",
	"d%n",
	"d%c",
	"fp%n",
	"fp%c",
	"d%c:d0",
	"%c:l",
	"@(%c:l,d%i:l:1)",
	"@(%c:l,d%i:l:2)",
	"@(%c:l,d%i:l:4)",
	"@(%c,d%i:l:1)",
	"@(%c,d%i:l:2)",
	"@(%c,d%i:l:4)",
	"a%n@(%c,d%i:l:1)",
	"a%n@(%c,d%i:l:2)",
	"a%n@(%c,d%i:l:4)",
	"a6@(%c,d%i:l:1)",
	"a6@(%c,d%i:l:2)",
	"a6@(%c,d%i:l:4)",
	"a%n@",
	"sp@",
	"a6@(%c)",
	"sp@(%c)",
	"a%n@(%c:l)",
	"a%n@(%c)",
	"#%c",
	"#%c/2",
	"%c",
};

static char *asmstr3[] = {
	"(DUMMY)",
	"add",
	"addq",
	"and",
	"asl",
	"asr",
	"cc",
	"cs",
	"div",
	"divs",
	"divsl",
	"divu",
	"divul",
	"eor",
	"eq",
	"ext",
	"extb",
	"ge",
	"gt",
	"hi",
	"le",
	"ls",
	"lsl",
	"lsr",
	"lt",
	"mul",
	"muls",
	"mulu",
	"ne",
	"neg",
	"not",
	"or",
	"sub",
	"subq",
};

static char *asmstr4[] = {
	"(DUMMY)",
	"b",
	"w",
	"l",
	"d",
	"s",
	"x",
};

static struct symbol sym[] = {
{{"(DUMMY)"}},
{{"0xff"}},
{{"0xffff"}},
{{"0"}},
{{"1"}},
{{"2"}},
{{"3"}},
{{"4"}},
{{"5"}},
{{"6"}},
{{"7"}},
{{"8"}},
};

dclproto(static void rewrite,(Node));

dclproto(static void emit,(Node));

static void initgen() {
	opcodes = shortopcodes-608;
	IR->x.rewrite = rewrite;
	IR->emit = emit;
	asmstr[0] = asmstr0;
	asmstr[1] = asmstr0;
	asmstr[2] = asmstr0;
	asmstr[3] = asmstr3;
	asmstr[4] = asmstr4;
	sym[1].x.name = string("0xff");
	sym[2].x.name = string("0xffff");
	sym[3].x.name = string("0");
	sym[4].x.name = string("1");
	sym[5].x.name = string("2");
	sym[6].x.name = string("3");
	sym[7].x.name = string("4");
	sym[8].x.name = string("5");
	sym[9].x.name = string("6");
	sym[10].x.name = string("7");
	sym[11].x.name = string("8");
}

dclproto(static char *emit0,(char *, Node, int));
static char *emit0(rbp, a, n)
register char *rbp;
register Node a;
register int n;
{
	register char *cp;

	assert(a->x.ints[n]);
	switch (a->x.ints[n]) {
	case 0:   /* (DUMMY) */
		*rbp++ = '(';
		*rbp++ = 'D';
		*rbp++ = 'U';
		*rbp++ = 'M';
		*rbp++ = 'M';
		*rbp++ = 'Y';
		*rbp++ = ')';
		return rbp;
	case 1:   /* a%n */
		*rbp++ = 'a';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 2:   /* a%c */
		*rbp++ = 'a';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 3:   /* d%n */
		*rbp++ = 'd';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 4:   /* d%c */
		*rbp++ = 'd';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 5:   /* fp%n */
		*rbp++ = 'f';
		*rbp++ = 'p';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 6:   /* fp%c */
		*rbp++ = 'f';
		*rbp++ = 'p';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 7:   /* d%c:d0 */
		*rbp++ = 'd';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'd';
		*rbp++ = '0';
		return rbp;
	case 8:   /* %c:l */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		return rbp;
	case 9:   /* @(%c:l,d%i:l:1) */
		*rbp++ = '@';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		*rbp++ = ',';
		*rbp++ = 'd';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		*rbp++ = ':';
		*rbp++ = '1';
		*rbp++ = ')';
		return rbp;
	case 10:   /* @(%c:l,d%i:l:2) */
		*rbp++ = '@';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		*rbp++ = ',';
		*rbp++ = 'd';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		*rbp++ = ':';
		*rbp++ = '2';
		*rbp++ = ')';
		return rbp;
	case 11:   /* @(%c:l,d%i:l:4) */
		*rbp++ = '@';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		*rbp++ = ',';
		*rbp++ = 'd';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		*rbp++ = ':';
		*rbp++ = '4';
		*rbp++ = ')';
		return rbp;
	case 12:   /* @(%c,d%i:l:1) */
		*rbp++ = '@';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ',';
		*rbp++ = 'd';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		*rbp++ = ':';
		*rbp++ = '1';
		*rbp++ = ')';
		return rbp;
	case 13:   /* @(%c,d%i:l:2) */
		*rbp++ = '@';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ',';
		*rbp++ = 'd';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		*rbp++ = ':';
		*rbp++ = '2';
		*rbp++ = ')';
		return rbp;
	case 14:   /* @(%c,d%i:l:4) */
		*rbp++ = '@';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ',';
		*rbp++ = 'd';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		*rbp++ = ':';
		*rbp++ = '4';
		*rbp++ = ')';
		return rbp;
	case 15:   /* a%n@(%c,d%i:l:1) */
		*rbp++ = 'a';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '@';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ',';
		*rbp++ = 'd';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		*rbp++ = ':';
		*rbp++ = '1';
		*rbp++ = ')';
		return rbp;
	case 16:   /* a%n@(%c,d%i:l:2) */
		*rbp++ = 'a';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '@';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ',';
		*rbp++ = 'd';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		*rbp++ = ':';
		*rbp++ = '2';
		*rbp++ = ')';
		return rbp;
	case 17:   /* a%n@(%c,d%i:l:4) */
		*rbp++ = 'a';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '@';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ',';
		*rbp++ = 'd';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		*rbp++ = ':';
		*rbp++ = '4';
		*rbp++ = ')';
		return rbp;
	case 18:   /* a6@(%c,d%i:l:1) */
		*rbp++ = 'a';
		*rbp++ = '6';
		*rbp++ = '@';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ',';
		*rbp++ = 'd';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		*rbp++ = ':';
		*rbp++ = '1';
		*rbp++ = ')';
		return rbp;
	case 19:   /* a6@(%c,d%i:l:2) */
		*rbp++ = 'a';
		*rbp++ = '6';
		*rbp++ = '@';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ',';
		*rbp++ = 'd';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		*rbp++ = ':';
		*rbp++ = '2';
		*rbp++ = ')';
		return rbp;
	case 20:   /* a6@(%c,d%i:l:4) */
		*rbp++ = 'a';
		*rbp++ = '6';
		*rbp++ = '@';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ',';
		*rbp++ = 'd';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		*rbp++ = ':';
		*rbp++ = '4';
		*rbp++ = ')';
		return rbp;
	case 21:   /* a%n@ */
		*rbp++ = 'a';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '@';
		return rbp;
	case 22:   /* sp@ */
		*rbp++ = 's';
		*rbp++ = 'p';
		*rbp++ = '@';
		return rbp;
	case 23:   /* a6@(%c) */
		*rbp++ = 'a';
		*rbp++ = '6';
		*rbp++ = '@';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
		return rbp;
	case 24:   /* sp@(%c) */
		*rbp++ = 's';
		*rbp++ = 'p';
		*rbp++ = '@';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
		return rbp;
	case 25:   /* a%n@(%c:l) */
		*rbp++ = 'a';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '@';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ':';
		*rbp++ = 'l';
		*rbp++ = ')';
		return rbp;
	case 26:   /* a%n@(%c) */
		*rbp++ = 'a';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '@';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
		return rbp;
	case 27:   /* #%c */
		*rbp++ = '#';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 28:   /* #%c/2 */
		*rbp++ = '#';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '/';
		*rbp++ = '2';
		return rbp;
	case 29:   /* %c */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	}
	return rbp;
}

static void emit(a)
register Node a;
{
	register char *cp, *rbp;

	for (; a; a = a->x.next) {
		switch (a->op) {
		case 608: L608:   /* fmoves %y,d0; fmoves d0,%z */
			rbp = bp;
			*rbp++ = 'f';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'e';
			*rbp++ = 's';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = 'd';
			*rbp++ = '0';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'f';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'e';
			*rbp++ = 's';
			*rbp++ = ' ';
			*rbp++ = 'd';
			*rbp++ = '0';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 609: L609:   /* %f%t %x,%z |mod */
			rbp = bp;
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			*rbp++ = ' ';
			*rbp++ = '|';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'd';
			bp = rbp;
			outs("\n");
			break;
		case 610: L610:   /* fintrzx %y,fp0; fmovel fp0,%z */
			rbp = bp;
			*rbp++ = 'f';
			*rbp++ = 'i';
			*rbp++ = 'n';
			*rbp++ = 't';
			*rbp++ = 'r';
			*rbp++ = 'z';
			*rbp++ = 'x';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = 'f';
			*rbp++ = 'p';
			*rbp++ = '0';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'f';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'e';
			*rbp++ = 'l';
			*rbp++ = ' ';
			*rbp++ = 'f';
			*rbp++ = 'p';
			*rbp++ = '0';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 611: L611:   /* jbsr %y */
			rbp = bp;
			*rbp++ = 'j';
			*rbp++ = 'b';
			*rbp++ = 's';
			*rbp++ = 'r';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 612: L612:   /* jbsr %y; movl d0,sp@-; fmoves sp@+,%z */
			rbp = bp;
			*rbp++ = 'j';
			*rbp++ = 'b';
			*rbp++ = 's';
			*rbp++ = 'r';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'l';
			*rbp++ = ' ';
			*rbp++ = 'd';
			*rbp++ = '0';
			*rbp++ = ',';
			*rbp++ = 's';
			*rbp++ = 'p';
			*rbp++ = '@';
			*rbp++ = '-';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'f';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'e';
			*rbp++ = 's';
			*rbp++ = ' ';
			*rbp++ = 's';
			*rbp++ = 'p';
			*rbp++ = '@';
			*rbp++ = '+';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 613: L613:   /* jbsr %y; movl d1,sp@-; movl d0,sp@-; fmoved sp@+,%z */
			rbp = bp;
			*rbp++ = 'j';
			*rbp++ = 'b';
			*rbp++ = 's';
			*rbp++ = 'r';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'l';
			*rbp++ = ' ';
			*rbp++ = 'd';
			*rbp++ = '1';
			*rbp++ = ',';
			*rbp++ = 's';
			*rbp++ = 'p';
			*rbp++ = '@';
			*rbp++ = '-';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'l';
			*rbp++ = ' ';
			*rbp++ = 'd';
			*rbp++ = '0';
			*rbp++ = ',';
			*rbp++ = 's';
			*rbp++ = 'p';
			*rbp++ = '@';
			*rbp++ = '-';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'f';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'e';
			*rbp++ = 'd';
			*rbp++ = ' ';
			*rbp++ = 's';
			*rbp++ = 'p';
			*rbp++ = '@';
			*rbp++ = '+';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 614: L614:   /* lea %z,a1; movl %x,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b */
			rbp = bp;
			*rbp++ = 'l';
			*rbp++ = 'e';
			*rbp++ = 'a';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			*rbp++ = ',';
			*rbp++ = 'a';
			*rbp++ = '1';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'l';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			*rbp++ = 'd';
			*rbp++ = '0';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = '2';
			*rbp++ = ':';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'w';
			*rbp++ = ' ';
			*rbp++ = 'a';
			*rbp++ = '0';
			*rbp++ = '@';
			*rbp++ = '+';
			*rbp++ = ',';
			*rbp++ = 'a';
			*rbp++ = '1';
			*rbp++ = '@';
			*rbp++ = '+';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 's';
			*rbp++ = 'u';
			*rbp++ = 'b';
			*rbp++ = 'q';
			*rbp++ = 'l';
			*rbp++ = ' ';
			*rbp++ = '#';
			*rbp++ = '1';
			*rbp++ = ',';
			*rbp++ = 'd';
			*rbp++ = '0';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'j';
			*rbp++ = 'g';
			*rbp++ = 't';
			*rbp++ = ' ';
			*rbp++ = '2';
			*rbp++ = 'b';
			bp = rbp;
			outs("\n");
			break;
		case 615: L615:   /* movl %x,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'l';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			*rbp++ = 'd';
			*rbp++ = '0';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = '2';
			*rbp++ = ':';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'w';
			*rbp++ = ' ';
			*rbp++ = 'a';
			*rbp++ = '0';
			*rbp++ = '@';
			*rbp++ = '+';
			*rbp++ = ',';
			*rbp++ = 'a';
			*rbp++ = '1';
			*rbp++ = '@';
			*rbp++ = '+';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 's';
			*rbp++ = 'u';
			*rbp++ = 'b';
			*rbp++ = 'q';
			*rbp++ = 'l';
			*rbp++ = ' ';
			*rbp++ = '#';
			*rbp++ = '1';
			*rbp++ = ',';
			*rbp++ = 'd';
			*rbp++ = '0';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'j';
			*rbp++ = 'g';
			*rbp++ = 't';
			*rbp++ = ' ';
			*rbp++ = '2';
			*rbp++ = 'b';
			bp = rbp;
			outs("\n");
			break;
		case 616: L616:   /* # label */
			(*IR->x.emitspecial)(a);
			break;
		case 617: L617:   /* # nop */
			(*IR->x.emitspecial)(a);
			break;
		case 618: L618:   /* # ret */
			(*IR->x.emitspecial)(a);
			break;
		case 619: L619:   /* %f%t %x,%z */
			rbp = bp;
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 620: L620:   /* %f%t %z */
			rbp = bp;
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 621: L621:   /* clr%t %z */
			rbp = bp;
			*rbp++ = 'c';
			*rbp++ = 'l';
			*rbp++ = 'r';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 622: L622:   /* cmp%t %x,%y; j%f %z */
			rbp = bp;
			*rbp++ = 'c';
			*rbp++ = 'm';
			*rbp++ = 'p';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'j';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 623: L623:   /* f%f%t %x,%z */
			rbp = bp;
			*rbp++ = 'f';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 624: L624:   /* fcmp%t %x,%y; fj%f %z */
			rbp = bp;
			*rbp++ = 'f';
			*rbp++ = 'c';
			*rbp++ = 'm';
			*rbp++ = 'p';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'f';
			*rbp++ = 'j';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 625: L625:   /* fmove%t %y,%z */
			rbp = bp;
			*rbp++ = 'f';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'e';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 626: L626:   /* fmovex %y,%z; f%f%t %x,%z */
			rbp = bp;
			*rbp++ = 'f';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'e';
			*rbp++ = 'x';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'f';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 627: L627:   /* jra %z */
			rbp = bp;
			*rbp++ = 'j';
			*rbp++ = 'r';
			*rbp++ = 'a';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 628: L628:   /* lea %y,%z */
			rbp = bp;
			*rbp++ = 'l';
			*rbp++ = 'e';
			*rbp++ = 'a';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 629: L629:   /* mov%t %y,%z */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 630: L630:   /* mov%t %y,%z; %f%t %x,%z */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 631: L631:   /* mov%t %y,%z; %f%t %z */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 632: L632:   /* tst%t %y; j%f %z */
			rbp = bp;
			*rbp++ = 't';
			*rbp++ = 's';
			*rbp++ = 't';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'j';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 633: L633:   /* %z: */
			rbp = bp;
			rbp = emit0(rbp, a, 2);
			*rbp++ = ':';
			bp = rbp;
			outs("\n");
			break;
		}
	}
}

static void final(a)
register Node a;
{
	register int s;
	switch (a->op) {
	case 608: L608:   /* fmoves %y,d0; fmoves d0,%z */
		dumptree(a, 608);
		spill(1, DREG, a);  
		a->op = 608;
		break;
	case 609: L609:   /* %f%t %x,%z |mod */
		dumptree(a, 609);
		spill(1, DREG, a);  
		a->op = 609;
		break;
	case 610: L610:   /* fintrzx %y,fp0; fmovel fp0,%z */
		dumptree(a, 610);
		spill(1, FREG, a);  
		a->op = 610;
		break;
	case 611: L611:   /* jbsr %y */
		dumptree(a, 611);
		spill(3, AREG, a);  spill(2, DREG, a);  
		a->op = 611;
		break;
	case 612: L612:   /* jbsr %y; movl d0,sp@-; fmoves sp@+,%z */
		dumptree(a, 612);
		spill(3, DREG, a);  spill(3, AREG, a);  
		a->op = 612;
		break;
	case 613: L613:   /* jbsr %y; movl d1,sp@-; movl d0,sp@-; fmoved sp@+,%z */
		dumptree(a, 613);
		spill(3, DREG, a);  spill(3, AREG, a);  
		a->op = 613;
		break;
	case 614: L614:   /* lea %z,a1; movl %x,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b */
		dumptree(a, 614);
		spill(1, DREG, a);  spill(3, AREG, a);  
		a->op = 614;
		break;
	case 615: L615:   /* movl %x,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b */
		dumptree(a, 615);
		spill(1, DREG, a);  spill(3, AREG, a);  
		a->op = 615;
		break;
	case 616: L616:   /* # label */
		dumptree(a, 616);
		a->op = 616;
		return;
	case 617: L617:   /* # nop */
		dumptree(a, 617);
		a->op = 617;
		return;
	case 618: L618:   /* # ret */
		dumptree(a, 618);
		a->op = 618;
		return;
	case 619: L619:   /* %f%t %x,%z */
		dumptree(a, 619);
		switch (a->x.ints[3]) {
		case 1:   /* add */
			if (
			a->x.ints[1] == 27  /* #%c */
			) {
				if (
				a->syms[1]->x.name == sym[7].x.name  /* 4 */
				) {
					a->x.ints[3] = 2;  /* addq */
					goto L619;   /* %f%t %x,%z */
				}
				if (
				a->syms[1]->x.name == sym[11].x.name  /* 8 */
				) {
					a->x.ints[3] = 2;  /* addq */
					goto L619;   /* %f%t %x,%z */
				}
				if (
				a->syms[1]->x.name == sym[4].x.name  /* 1 */
				) {
					a->x.ints[3] = 2;  /* addq */
					goto L619;   /* %f%t %x,%z */
				}
				if (
				a->syms[1]->x.name == sym[8].x.name  /* 5 */
				) {
					a->x.ints[3] = 2;  /* addq */
					goto L619;   /* %f%t %x,%z */
				}
				if (
				a->syms[1]->x.name == sym[5].x.name  /* 2 */
				) {
					a->x.ints[3] = 2;  /* addq */
					goto L619;   /* %f%t %x,%z */
				}
				if (
				a->syms[1]->x.name == sym[9].x.name  /* 6 */
				) {
					a->x.ints[3] = 2;  /* addq */
					goto L619;   /* %f%t %x,%z */
				}
				if (
				a->syms[1]->x.name == sym[6].x.name  /* 3 */
				) {
					a->x.ints[3] = 2;  /* addq */
					goto L619;   /* %f%t %x,%z */
				}
				if (
				a->syms[1]->x.name == sym[10].x.name  /* 7 */
				) {
					a->x.ints[3] = 2;  /* addq */
					goto L619;   /* %f%t %x,%z */
				}
			}
			break;
		case 32:   /* sub */
			if (
			a->x.ints[1] == 27  /* #%c */
			) {
				if (
				a->syms[1]->x.name == sym[7].x.name  /* 4 */
				) {
					a->x.ints[3] = 33;  /* subq */
					goto L619;   /* %f%t %x,%z */
				}
				if (
				a->syms[1]->x.name == sym[11].x.name  /* 8 */
				) {
					a->x.ints[3] = 33;  /* subq */
					goto L619;   /* %f%t %x,%z */
				}
				if (
				a->syms[1]->x.name == sym[4].x.name  /* 1 */
				) {
					a->x.ints[3] = 33;  /* subq */
					goto L619;   /* %f%t %x,%z */
				}
				if (
				a->syms[1]->x.name == sym[8].x.name  /* 5 */
				) {
					a->x.ints[3] = 33;  /* subq */
					goto L619;   /* %f%t %x,%z */
				}
				if (
				a->syms[1]->x.name == sym[5].x.name  /* 2 */
				) {
					a->x.ints[3] = 33;  /* subq */
					goto L619;   /* %f%t %x,%z */
				}
				if (
				a->syms[1]->x.name == sym[9].x.name  /* 6 */
				) {
					a->x.ints[3] = 33;  /* subq */
					goto L619;   /* %f%t %x,%z */
				}
				if (
				a->syms[1]->x.name == sym[6].x.name  /* 3 */
				) {
					a->x.ints[3] = 33;  /* subq */
					goto L619;   /* %f%t %x,%z */
				}
				if (
				a->syms[1]->x.name == sym[10].x.name  /* 7 */
				) {
					a->x.ints[3] = 33;  /* subq */
					goto L619;   /* %f%t %x,%z */
				}
			}
			break;
		}
		/* a%c */
		/* d%c */
		if ("\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[2]]) {
			if (
			!regchain(a)
			) {
				goto L630;   /* mov%t %y,%z; %f%t %x,%z */
			}
		}
		a->op = 619;
		break;
	case 620: L620:   /* %f%t %z */
		dumptree(a, 620);
		/* a%c */
		/* d%c */
		if ("\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[2]]) {
			if (
			!regchain(a)
			) {
				goto L631;   /* mov%t %y,%z; %f%t %z */
			}
		}
		a->op = 620;
		break;
	case 621: L621:   /* clr%t %z */
		dumptree(a, 621);
		a->op = 621;
		break;
	case 622: L622:   /* cmp%t %x,%y; j%f %z */
		dumptree(a, 622);
		if (
		a->x.ints[1] == 27  /* #%c */
		&& a->syms[1]->x.name == sym[3].x.name  /* 0 */
		) {
			/* d%n */
			/* d%c */
			/* %c:l */
			/* @(%c:l,d%i:l:1) */
			/* @(%c:l,d%i:l:2) */
			/* @(%c:l,d%i:l:4) */
			/* @(%c,d%i:l:1) */
			/* @(%c,d%i:l:2) */
			/* @(%c,d%i:l:4) */
			/* a%n@(%c,d%i:l:1) */
			/* a%n@(%c,d%i:l:2) */
			/* a%n@(%c,d%i:l:4) */
			/* a6@(%c,d%i:l:1) */
			/* a6@(%c,d%i:l:2) */
			/* a6@(%c,d%i:l:4) */
			/* a%n@ */
			/* a6@(%c) */
			/* sp@(%c) */
			/* a%n@(%c:l) */
			/* a%n@(%c) */
			if ("\0\0\0\1\1\0\0\0\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\1\1\1\1\0\0\0"[a->x.ints[0]]) {
				goto L632;   /* tst%t %y; j%f %z */
			}
		}
		a->op = 622;
		break;
	case 623: L623:   /* f%f%t %x,%z */
		dumptree(a, 623);
		if (
		a->x.ints[2] == 6  /* fp%c */
		&& !regchain(a)
		) {
			goto L626;   /* fmovex %y,%z; f%f%t %x,%z */
		}
		a->op = 623;
		break;
	case 624: L624:   /* fcmp%t %x,%y; fj%f %z */
		dumptree(a, 624);
		a->op = 624;
		break;
	case 625: L625:   /* fmove%t %y,%z */
		dumptree(a, 625);
		switch (a->x.ints[4]) {
		case 5:   /* s */
			if (
			a->x.ints[0] == 5  /* fp%n */
			&& a->x.ints[2] == 6  /* fp%c */
			) {
				a->x.ints[2] = 6;  /* fp%c */
				a->x.ints[0] = 5;  /* fp%n */
				goto L608;   /* fmoves %y,d0; fmoves d0,%z */
			}
			break;
		case 6:   /* x */
			if (
			a->x.ints[0] == a->x.ints[2]
			&& a->kids[0] == a->kids[2]
			&& a->kids[3] == a->kids[5]
			&& a->syms[0] == a->syms[2]
			) {
				goto L617;   /* # nop */
			}
			break;
		}
		a->op = 625;
		break;
	case 626: L626:   /* fmovex %y,%z; f%f%t %x,%z */
		dumptree(a, 626);
		a->op = 626;
		break;
	case 627: L627:   /* jra %z */
		dumptree(a, 627);
		a->op = 627;
		break;
	case 628: L628:   /* lea %y,%z */
		dumptree(a, 628);
		a->op = 628;
		break;
	case 629: L629:   /* mov%t %y,%z */
		dumptree(a, 629);
		if (
		a->x.ints[0] == 27  /* #%c */
		&& a->syms[0]->x.name == sym[3].x.name  /* 0 */
		) {
			/* d%n */
			/* d%c */
			/* %c:l */
			/* @(%c:l,d%i:l:1) */
			/* @(%c:l,d%i:l:2) */
			/* @(%c:l,d%i:l:4) */
			/* @(%c,d%i:l:1) */
			/* @(%c,d%i:l:2) */
			/* @(%c,d%i:l:4) */
			/* a%n@(%c,d%i:l:1) */
			/* a%n@(%c,d%i:l:2) */
			/* a%n@(%c,d%i:l:4) */
			/* a6@(%c,d%i:l:1) */
			/* a6@(%c,d%i:l:2) */
			/* a6@(%c,d%i:l:4) */
			/* a%n@ */
			/* a6@(%c) */
			/* sp@(%c) */
			/* a%n@(%c:l) */
			/* a%n@(%c) */
			if ("\0\0\0\1\1\0\0\0\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\1\1\1\1\0\0\0"[a->x.ints[2]]) {
				goto L621;   /* clr%t %z */
			}
		}
		if (
		a->x.ints[0] == a->x.ints[2]
		&& a->kids[0] == a->kids[2]
		&& a->kids[3] == a->kids[5]
		&& a->syms[0] == a->syms[2]
		) {
			goto L617;   /* # nop */
		}
		a->op = 629;
		break;
	case 630: L630:   /* mov%t %y,%z; %f%t %x,%z */
		dumptree(a, 630);
		a->op = 630;
		break;
	case 631: L631:   /* mov%t %y,%z; %f%t %z */
		dumptree(a, 631);
		a->op = 631;
		break;
	case 632: L632:   /* tst%t %y; j%f %z */
		dumptree(a, 632);
		a->op = 632;
		break;
	case 633: L633:   /* %z: */
		dumptree(a, 633);
		a->op = 633;
		break;
	}
}

static void rewrite(a)
register Node a;
{
	register Node b, c;
	register int s;
	assert(a);
	switch (a->op) {
	case 19: L19:   /* CNSTC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[0] = 27;  /* #%c */
		a->x.ints[4] = 1;  /* b */
		goto L629;   /* mov%t %y,%z */
	case 20: L20:   /* CNSTS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[0] = 27;  /* #%c */
		a->x.ints[4] = 2;  /* w */
		goto L629;   /* mov%t %y,%z */
	case 21: L21:   /* CNSTI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[0] = 27;  /* #%c */
		a->x.ints[4] = 3;  /* l */
		goto L629;   /* mov%t %y,%z */
	case 22: L22:   /* CNSTU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[0] = 27;  /* #%c */
		a->x.ints[4] = 3;  /* l */
		goto L629;   /* mov%t %y,%z */
	case 23: L23:   /* CNSTP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[2] = 2;  /* a%c */
		a->x.ints[0] = 27;  /* #%c */
		a->x.ints[4] = 3;  /* l */
		goto L629;   /* mov%t %y,%z */
	case 33: L33:   /* ARGF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 24;  /* sp@(%c) */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.ints[4] = 5;  /* s */
		goto L625;   /* fmove%t %y,%z */
	case 34: L34:   /* ARGD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 24;  /* sp@(%c) */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.ints[4] = 4;  /* d */
		goto L625;   /* fmove%t %y,%z */
	case 37: L37:   /* ARGI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 24;  /* sp@(%c) */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		goto L629;   /* mov%t %y,%z */
	case 39: L39:   /* ARGP */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 24;  /* sp@(%c) */
		a->x.ints[0] = 1;  /* a%n */
		a->x.ints[4] = 3;  /* l */
		goto L629;   /* mov%t %y,%z */
	case 41: L41:   /* ARGB */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->syms[1] = a->syms[0];
		a->x.ints[1] = 28;  /* #%c/2 */
		a->x.ints[2] = 24;  /* sp@(%c) */
		rtarget(a,0,a0);
		goto L614;   /* lea %z,a1; movl %x,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b */
	case 49: L49:   /* ASGNF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 21;  /* a%n@ */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.ints[4] = 5;  /* s */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L625;   /* fmove%t %y,%z */
	case 50: L50:   /* ASGND */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 21;  /* a%n@ */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.ints[4] = 4;  /* d */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L625;   /* fmove%t %y,%z */
	case 51: L51:   /* ASGNC */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 21;  /* a%n@ */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[4] = 1;  /* b */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L629;   /* mov%t %y,%z */
	case 52: L52:   /* ASGNS */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 21;  /* a%n@ */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[4] = 2;  /* w */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L629;   /* mov%t %y,%z */
	case 53: L53:   /* ASGNI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 21;  /* a%n@ */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L629;   /* mov%t %y,%z */
	case 55: L55:   /* ASGNP */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 21;  /* a%n@ */
		a->x.ints[0] = 1;  /* a%n */
		a->x.ints[4] = 3;  /* l */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L629;   /* mov%t %y,%z */
	case 57: L57:   /* ASGNB */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[1] = a->syms[0];
		a->x.ints[1] = 28;  /* #%c/2 */
		rtarget(a,0,a1), rtarget(a,1,a0);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L615;   /* movl %x,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b */
	case 65: L65:   /* INDIRF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 6;  /* fp%c */
		a->x.ints[0] = 21;  /* a%n@ */
		a->x.ints[4] = 5;  /* s */
		goto L625;   /* fmove%t %y,%z */
	case 66: L66:   /* INDIRD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 6;  /* fp%c */
		a->x.ints[0] = 21;  /* a%n@ */
		a->x.ints[4] = 4;  /* d */
		goto L625;   /* fmove%t %y,%z */
	case 67: L67:   /* INDIRC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[0] = 21;  /* a%n@ */
		a->x.ints[4] = 1;  /* b */
		goto L629;   /* mov%t %y,%z */
	case 68: L68:   /* INDIRS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[0] = 21;  /* a%n@ */
		a->x.ints[4] = 2;  /* w */
		goto L629;   /* mov%t %y,%z */
	case 69: L69:   /* INDIRI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[0] = 21;  /* a%n@ */
		a->x.ints[4] = 3;  /* l */
		goto L629;   /* mov%t %y,%z */
	case 71: L71:   /* INDIRP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* a%c */
		a->x.ints[0] = 21;  /* a%n@ */
		a->x.ints[4] = 3;  /* l */
		goto L629;   /* mov%t %y,%z */
	case 73: L73:   /* INDIRB */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* a%c */
		a->x.ints[0] = 1;  /* a%n */
		a->x.ints[4] = 3;  /* l */
		goto L629;   /* mov%t %y,%z */
	case 85: L85:   /* CVCI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 16;  /* extb */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L620;   /* %f%t %z */
	case 86: L86:   /* CVCU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->syms[1] = &sym[1];  /* 0xff */
		a->x.ints[1] = 27;  /* #%c */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 3;  /* and */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 97: L97:   /* CVDF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 6;  /* fp%c */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.ints[4] = 5;  /* s */
		goto L625;   /* fmove%t %y,%z */
	case 101: L101:   /* CVDI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[0] = 5;  /* fp%n */
		goto L610;   /* fintrzx %y,fp0; fmovel fp0,%z */
	case 114: L114:   /* CVFD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 6;  /* fp%c */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		goto L625;   /* fmove%t %y,%z */
	case 130: L130:   /* CVID */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 6;  /* fp%c */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		goto L625;   /* fmove%t %y,%z */
	case 131: L131:   /* CVIC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[4] = 1;  /* b */
		goto L629;   /* mov%t %y,%z */
	case 132: L132:   /* CVIS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[4] = 2;  /* w */
		goto L629;   /* mov%t %y,%z */
	case 134: L134:   /* CVIU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		goto L629;   /* mov%t %y,%z */
	case 150: L150:   /* CVPU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[0] = 1;  /* a%n */
		a->x.ints[4] = 3;  /* l */
		goto L629;   /* mov%t %y,%z */
	case 165: L165:   /* CVSI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 15;  /* ext */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L620;   /* %f%t %z */
	case 166: L166:   /* CVSU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->syms[1] = &sym[2];  /* 0xffff */
		a->x.ints[1] = 27;  /* #%c */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 3;  /* and */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 179: L179:   /* CVUC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[4] = 1;  /* b */
		goto L629;   /* mov%t %y,%z */
	case 180: L180:   /* CVUS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[4] = 2;  /* w */
		goto L629;   /* mov%t %y,%z */
	case 181: L181:   /* CVUI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		goto L629;   /* mov%t %y,%z */
	case 183: L183:   /* CVUP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* a%c */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		goto L629;   /* mov%t %y,%z */
	case 193: L193:   /* NEGF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 6;  /* fp%c */
		a->kids[1] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[1] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		a->x.ints[3] = 29;  /* neg */
		goto L623;   /* f%f%t %x,%z */
	case 194: L194:   /* NEGD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 6;  /* fp%c */
		a->kids[1] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[1] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		a->x.ints[3] = 29;  /* neg */
		goto L623;   /* f%f%t %x,%z */
	case 197: L197:   /* NEGI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 29;  /* neg */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L620;   /* %f%t %z */
	case 209: L209:   /* CALLF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 6;  /* fp%c */
		a->x.ints[0] = 21;  /* a%n@ */
		setreg(a,fp0);
		goto L612;   /* jbsr %y; movl d0,sp@-; fmoves sp@+,%z */
	case 210: L210:   /* CALLD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 6;  /* fp%c */
		a->x.ints[0] = 21;  /* a%n@ */
		setreg(a,fp0);
		goto L613;   /* jbsr %y; movl d1,sp@-; movl d0,sp@-; fmoved sp@+,%z */
	case 213: L213:   /* CALLI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 21;  /* a%n@ */
		setreg(a,d0);
		goto L611;   /* jbsr %y */
	case 216: L216:   /* CALLV */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[0] = 21;  /* a%n@ */
		setreg(a,d0);
		goto L611;   /* jbsr %y */
	case 217: L217:   /* CALLB */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[0] = 21;  /* a%n@ */
		rtarget(a,1,a1), setreg(a,d0);
		goto L611;   /* jbsr %y */
	case 225: L225:   /* LOADF */
	case 226: L226:   /* LOADD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 6;  /* fp%c */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		goto L625;   /* fmove%t %y,%z */
	case 227: L227:   /* LOADC */
	case 228: L228:   /* LOADS */
	case 229: L229:   /* LOADI */
	case 230: L230:   /* LOADU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		goto L629;   /* mov%t %y,%z */
	case 231: L231:   /* LOADP */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* a%c */
		a->x.ints[0] = 1;  /* a%n */
		a->x.ints[4] = 3;  /* l */
		goto L629;   /* mov%t %y,%z */
	case 233: L233:   /* LOADB */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* a%c */
		a->x.ints[0] = 1;  /* a%n */
		a->x.ints[4] = 3;  /* l */
		goto L629;   /* mov%t %y,%z */
	case 241: L241:   /* RETF */
	case 242: L242:   /* RETD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rtarget(a,0,fp0);
		goto L248;   /* RETV */
	case 245: L245:   /* RETI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rtarget(a,0,d0);
		goto L618;   /* # ret */
	case 248: L248:   /* RETV */
		dumptree(a, a->op);
		goto L618;   /* # ret */
	case 263: L263:   /* ADDRGP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[2] = 2;  /* a%c */
		a->x.ints[0] = 8;  /* %c:l */
		goto L628;   /* lea %y,%z */
	case 279: L279:   /* ADDRFP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[2] = 2;  /* a%c */
		a->x.ints[0] = 23;  /* a6@(%c) */
		goto L628;   /* lea %y,%z */
	case 295: L295:   /* ADDRLP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[2] = 2;  /* a%c */
		a->x.ints[0] = 23;  /* a6@(%c) */
		goto L628;   /* lea %y,%z */
	case 305: L305:   /* ADDF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 6;  /* fp%c */
		a->x.ints[1] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		a->x.ints[3] = 1;  /* add */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.zap0=1;
		goto L623;   /* f%f%t %x,%z */
	case 306: L306:   /* ADDD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 6;  /* fp%c */
		a->x.ints[1] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		a->x.ints[3] = 1;  /* add */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.zap0=1;
		goto L623;   /* f%f%t %x,%z */
	case 309: L309:   /* ADDI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 1;  /* add */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 310: L310:   /* ADDU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 1;  /* add */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 311: L311:   /* ADDP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* a%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 1;  /* add */
		a->x.ints[0] = 1;  /* a%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 321: L321:   /* SUBF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 6;  /* fp%c */
		a->x.ints[1] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		a->x.ints[3] = 32;  /* sub */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.zap0=1;
		goto L623;   /* f%f%t %x,%z */
	case 322: L322:   /* SUBD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 6;  /* fp%c */
		a->x.ints[1] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		a->x.ints[3] = 32;  /* sub */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.zap0=1;
		goto L623;   /* f%f%t %x,%z */
	case 325: L325:   /* SUBI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 32;  /* sub */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 326: L326:   /* SUBU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 32;  /* sub */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 327: L327:   /* SUBP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* a%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 32;  /* sub */
		a->x.ints[0] = 1;  /* a%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 341: L341:   /* LSHI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 4;  /* asl */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 342: L342:   /* LSHU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 22;  /* lsl */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 357: L357:   /* MODI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 7;  /* d%c:d0 */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 10;  /* divsl */
		rtarget(a,0,d0),setreg(a,d1234);
		a->x.ints[0] = 3;  /* d%n */
		goto L609;   /* %f%t %x,%z |mod */
	case 358: L358:   /* MODU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 7;  /* d%c:d0 */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 12;  /* divul */
		rtarget(a,0,d0),setreg(a,d1234);
		a->x.ints[0] = 3;  /* d%n */
		goto L609;   /* %f%t %x,%z |mod */
	case 373: L373:   /* RSHI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 5;  /* asr */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 374: L374:   /* RSHU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 23;  /* lsr */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 390: L390:   /* BANDU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 3;  /* and */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 406: L406:   /* BCOMU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 30;  /* not */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L620;   /* %f%t %z */
	case 422: L422:   /* BORU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 31;  /* or */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 438: L438:   /* BXORU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 13;  /* eor */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 449: L449:   /* DIVF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 6;  /* fp%c */
		a->x.ints[1] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		a->x.ints[3] = 8;  /* div */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.zap0=1;
		goto L623;   /* f%f%t %x,%z */
	case 450: L450:   /* DIVD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 6;  /* fp%c */
		a->x.ints[1] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		a->x.ints[3] = 8;  /* div */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.zap0=1;
		goto L623;   /* f%f%t %x,%z */
	case 453: L453:   /* DIVI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 9;  /* divs */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 454: L454:   /* DIVU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 11;  /* divu */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 465: L465:   /* MULF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 6;  /* fp%c */
		a->x.ints[1] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		a->x.ints[3] = 25;  /* mul */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.zap0=1;
		goto L623;   /* f%f%t %x,%z */
	case 466: L466:   /* MULD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 6;  /* fp%c */
		a->x.ints[1] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		a->x.ints[3] = 25;  /* mul */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.zap0=1;
		goto L623;   /* f%f%t %x,%z */
	case 469: L469:   /* MULI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 26;  /* muls */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 470: L470:   /* MULU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* d%c */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 27;  /* mulu */
		a->x.ints[0] = 3;  /* d%n */
		a->x.zap0=1;
		goto L619;   /* %f%t %x,%z */
	case 481: L481:   /* EQF */
	case 482: L482:   /* EQD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		a->x.ints[3] = 14;  /* eq */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.ints[1] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		goto L624;   /* fcmp%t %x,%y; fj%f %z */
	case 485: L485:   /* EQI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		a->x.ints[3] = 14;  /* eq */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* cmp%t %x,%y; j%f %z */
	case 497: L497:   /* GEF */
	case 498: L498:   /* GED */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		a->x.ints[3] = 17;  /* ge */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.ints[1] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		goto L624;   /* fcmp%t %x,%y; fj%f %z */
	case 501: L501:   /* GEI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		a->x.ints[3] = 17;  /* ge */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* cmp%t %x,%y; j%f %z */
	case 502: L502:   /* GEU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		a->x.ints[3] = 6;  /* cc */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* cmp%t %x,%y; j%f %z */
	case 513: L513:   /* GTF */
	case 514: L514:   /* GTD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		a->x.ints[3] = 18;  /* gt */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.ints[1] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		goto L624;   /* fcmp%t %x,%y; fj%f %z */
	case 517: L517:   /* GTI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		a->x.ints[3] = 18;  /* gt */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* cmp%t %x,%y; j%f %z */
	case 518: L518:   /* GTU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		a->x.ints[3] = 19;  /* hi */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* cmp%t %x,%y; j%f %z */
	case 529: L529:   /* LEF */
	case 530: L530:   /* LED */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		a->x.ints[3] = 20;  /* le */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.ints[1] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		goto L624;   /* fcmp%t %x,%y; fj%f %z */
	case 533: L533:   /* LEI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		a->x.ints[3] = 20;  /* le */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* cmp%t %x,%y; j%f %z */
	case 534: L534:   /* LEU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		a->x.ints[3] = 21;  /* ls */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* cmp%t %x,%y; j%f %z */
	case 545: L545:   /* LTF */
	case 546: L546:   /* LTD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		a->x.ints[3] = 24;  /* lt */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.ints[1] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		goto L624;   /* fcmp%t %x,%y; fj%f %z */
	case 549: L549:   /* LTI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		a->x.ints[3] = 24;  /* lt */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* cmp%t %x,%y; j%f %z */
	case 550: L550:   /* LTU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		a->x.ints[3] = 7;  /* cs */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* cmp%t %x,%y; j%f %z */
	case 561: L561:   /* NEF */
	case 562: L562:   /* NED */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		a->x.ints[3] = 28;  /* ne */
		a->x.ints[0] = 5;  /* fp%n */
		a->x.ints[1] = 5;  /* fp%n */
		a->x.ints[4] = 6;  /* x */
		goto L624;   /* fcmp%t %x,%y; fj%f %z */
	case 565: L565:   /* NEI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		a->x.ints[3] = 28;  /* ne */
		a->x.ints[0] = 3;  /* d%n */
		a->x.ints[1] = 3;  /* d%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* cmp%t %x,%y; j%f %z */
	case 584: L584:   /* JUMPV */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 21;  /* a%n@ */
		goto L627;   /* jra %z */
	case 600: L600:   /* LABELV */
		dumptree(a, a->op);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 29;  /* %c */
		goto L633;   /* %z: */
	case 608: L608:   /* fmoves %y,d0; fmoves d0,%z */
		dumptree(a, 608);
		a->op = 608;
		break;
	case 609: L609:   /* %f%t %x,%z |mod */
		dumptree(a, 609);
		a->op = 609;
		break;
	case 610: L610:   /* fintrzx %y,fp0; fmovel fp0,%z */
		dumptree(a, 610);
		a->op = 610;
		break;
	case 611: L611:   /* jbsr %y */
		dumptree(a, 611);
		b = a->kids[0];
		if (
		a->x.ints[0] == 21  /* a%n@ */
		&& b->op == 628  /* lea %y,%z */
		) {
			/* %c:l */
			/* a6@(%c) */
			if ("\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0"[b->x.ints[0]]) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L611;   /* jbsr %y */
			}
			if (
			b->count == 1
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L611;   /* jbsr %y */
			}
		}
		a->op = 611;
		break;
	case 612: L612:   /* jbsr %y; movl d0,sp@-; fmoves sp@+,%z */
		dumptree(a, 612);
		b = a->kids[0];
		if (
		a->x.ints[0] == 21  /* a%n@ */
		&& b->op == 628  /* lea %y,%z */
		) {
			/* %c:l */
			/* a6@(%c) */
			if ("\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0"[b->x.ints[0]]) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L612;   /* jbsr %y; movl d0,sp@-; fmoves sp@+,%z */
			}
			if (
			b->count == 1
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L612;   /* jbsr %y; movl d0,sp@-; fmoves sp@+,%z */
			}
		}
		a->op = 612;
		break;
	case 613: L613:   /* jbsr %y; movl d1,sp@-; movl d0,sp@-; fmoved sp@+,%z */
		dumptree(a, 613);
		b = a->kids[0];
		if (
		a->x.ints[0] == 21  /* a%n@ */
		&& b->op == 628  /* lea %y,%z */
		) {
			/* %c:l */
			/* a6@(%c) */
			if ("\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0"[b->x.ints[0]]) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L613;   /* jbsr %y; movl d1,sp@-; movl d0,sp@-; fmoved sp@+,%z */
			}
			if (
			b->count == 1
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L613;   /* jbsr %y; movl d1,sp@-; movl d0,sp@-; fmoved sp@+,%z */
			}
		}
		a->op = 613;
		break;
	case 614: L614:   /* lea %z,a1; movl %x,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b */
		dumptree(a, 614);
		a->op = 614;
		break;
	case 615: L615:   /* movl %x,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b */
		dumptree(a, 615);
		a->op = 615;
		break;
	case 616: L616:   /* # label */
		dumptree(a, 616);
		a->op = 616;
		break;
	case 617: L617:   /* # nop */
		dumptree(a, 617);
		a->op = 617;
		break;
	case 618: L618:   /* # ret */
		dumptree(a, 618);
		a->op = 618;
		break;
	case 619: L619:   /* %f%t %x,%z */
		dumptree(a, 619);
		switch (a->x.ints[3]) {
		case 1:   /* add */
			if (
			a->x.ints[4] == 3  /* l */
			&& a->x.ints[1] == 3  /* d%n */
			&& a->x.ints[2] == 2  /* a%c */
			&& a->kids[1]->x.ints[2] == 2  /* a%c */
			) {
				a->x.ints[1] = 1;  /* a%n */
				a->x.ints[0] = 3;  /* d%n */
				goto L619;   /* %f%t %x,%z */
			}
			if (
			a->x.ints[4] == 3  /* l */
			) {
				switch (a->x.ints[1]) {
				case 1:   /* a%n */
					if (
					a->x.ints[2] == 2  /* a%c */
					&& a->x.ints[0] == 3  /* d%n */
					) {
						b = a->kids[1];
						if (
						b->op == 628  /* lea %y,%z */
						) {
							switch (b->x.ints[0]) {
							case 8:   /* %c:l */
								if (
								!kflag
								) {
									--b->count;
									a->x.ints[0] = 9;  /* @(%c:l,d%i:l:1) */
									a->syms[0] = b->syms[0];
									a->kids[3] = a->kids[0];
									a->kids[0] = 0;
									a->x.zap0=0;
									a->kids[1] = 0;
									goto L628;   /* lea %y,%z */
								}
								break;
							case 23:   /* a6@(%c) */
								--b->count;
								a->x.ints[0] = 18;  /* a6@(%c,d%i:l:1) */
								a->syms[0] = b->syms[0];
								a->kids[3] = a->kids[0];
								a->kids[0] = 0;
								a->x.zap0=0;
								a->kids[1] = 0;
								goto L628;   /* lea %y,%z */
							}
						}
						b = a->kids[0];
						if (
						b->op == 619  /* %f%t %x,%z */
						&& b->x.ints[3] == 4  /* asl */
						&& b->x.ints[4] == 3  /* l */
						&& b->x.ints[1] == 27  /* #%c */
						) {
							if (
							b->syms[1]->x.name == sym[5].x.name  /* 2 */
							) {
								if (--b->count > 0)
									if (c=b->kids[0]) ++c->count;
								a->syms[0] = &sym[3];  /* 0 */
								a->x.ints[0] = 17;  /* a%n@(%c,d%i:l:4) */
								a->kids[3] = b->kids[0];
								a->kids[0] = a->kids[1];
								a->kids[1] = 0;
								a->x.zap0=0;
								goto L628;   /* lea %y,%z */
							}
							if (
							b->syms[1]->x.name == sym[4].x.name  /* 1 */
							) {
								if (--b->count > 0)
									if (c=b->kids[0]) ++c->count;
								a->syms[0] = &sym[3];  /* 0 */
								a->x.ints[0] = 16;  /* a%n@(%c,d%i:l:2) */
								a->kids[3] = b->kids[0];
								a->kids[0] = a->kids[1];
								a->kids[1] = 0;
								a->x.zap0=0;
								goto L628;   /* lea %y,%z */
							}
						}
					}
					break;
				case 27:   /* #%c */
					if (
					a->x.ints[2] == 2  /* a%c */
					&& a->x.ints[0] == 1  /* a%n */
					) {
						a->x.ints[0] = 26;  /* a%n@(%c) */
						a->syms[0] = a->syms[1];
						a->x.zap0=0;
						goto L628;   /* lea %y,%z */
					}
					break;
				}
			}
			/* a%n */
			/* d%n */
			if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[1]]) {
				/* a%c */
				/* d%c */
				if ("\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[2]]) {
					b = a->kids[1];
					if (
					b->op == 629  /* mov%t %y,%z */
					&& a->x.ints[4] == b->x.ints[4]
					) {
						/* a%n */
						/* #%c */
						if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->x.ints[0] == 3  /* d%n */
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
					}
				}
			}
			break;
		case 3:   /* and */
			b = a->kids[1];
			if (
			a->x.ints[1] == 3  /* d%n */
			&& a->x.ints[2] == 4  /* d%c */
			&& b->op == 629  /* mov%t %y,%z */
			) {
				/* d%n */
				/* d%c */
				/* %c:l */
				/* @(%c:l,d%i:l:1) */
				/* @(%c:l,d%i:l:2) */
				/* @(%c:l,d%i:l:4) */
				/* @(%c,d%i:l:1) */
				/* @(%c,d%i:l:2) */
				/* @(%c,d%i:l:4) */
				/* a%n@(%c,d%i:l:1) */
				/* a%n@(%c,d%i:l:2) */
				/* a%n@(%c,d%i:l:4) */
				/* a6@(%c,d%i:l:1) */
				/* a6@(%c,d%i:l:2) */
				/* a6@(%c,d%i:l:4) */
				/* a%n@ */
				/* a6@(%c) */
				/* sp@(%c) */
				/* a%n@(%c:l) */
				/* a%n@(%c) */
				/* #%c */
				if ("\0\0\0\1\1\0\0\0\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\1\1\1\1\1\0\0"[b->x.ints[0]]) {
					if (
					a->x.ints[4] == b->x.ints[4]
					) {
						/* a%n */
						/* #%c */
						if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->x.ints[0] == 3  /* d%n */
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
					}
				}
			}
			break;
		case 4:   /* asl */
		case 5:   /* asr */
		case 22:   /* lsl */
		case 23:   /* lsr */
			b = a->kids[1];
			if (
			a->x.ints[1] == 3  /* d%n */
			&& a->x.ints[2] == 4  /* d%c */
			&& b->op == 629  /* mov%t %y,%z */
			&& b->x.ints[0] == 27  /* #%c */
			) {
				if (
				b->syms[0]->x.name == sym[7].x.name  /* 4 */
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					goto L619;   /* %f%t %x,%z */
				}
				if (
				b->syms[0]->x.name == sym[11].x.name  /* 8 */
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					goto L619;   /* %f%t %x,%z */
				}
				if (
				b->syms[0]->x.name == sym[4].x.name  /* 1 */
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					goto L619;   /* %f%t %x,%z */
				}
				if (
				b->syms[0]->x.name == sym[8].x.name  /* 5 */
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					goto L619;   /* %f%t %x,%z */
				}
				if (
				b->syms[0]->x.name == sym[5].x.name  /* 2 */
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					goto L619;   /* %f%t %x,%z */
				}
				if (
				b->syms[0]->x.name == sym[9].x.name  /* 6 */
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					goto L619;   /* %f%t %x,%z */
				}
				if (
				b->syms[0]->x.name == sym[6].x.name  /* 3 */
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					goto L619;   /* %f%t %x,%z */
				}
				if (
				b->syms[0]->x.name == sym[10].x.name  /* 7 */
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					goto L619;   /* %f%t %x,%z */
				}
			}
			break;
		case 31:   /* or */
			b = a->kids[1];
			if (
			a->x.ints[1] == 3  /* d%n */
			&& a->x.ints[2] == 4  /* d%c */
			&& b->op == 629  /* mov%t %y,%z */
			) {
				/* d%n */
				/* d%c */
				/* %c:l */
				/* @(%c:l,d%i:l:1) */
				/* @(%c:l,d%i:l:2) */
				/* @(%c:l,d%i:l:4) */
				/* @(%c,d%i:l:1) */
				/* @(%c,d%i:l:2) */
				/* @(%c,d%i:l:4) */
				/* a%n@(%c,d%i:l:1) */
				/* a%n@(%c,d%i:l:2) */
				/* a%n@(%c,d%i:l:4) */
				/* a6@(%c,d%i:l:1) */
				/* a6@(%c,d%i:l:2) */
				/* a6@(%c,d%i:l:4) */
				/* a%n@ */
				/* a6@(%c) */
				/* sp@(%c) */
				/* a%n@(%c:l) */
				/* a%n@(%c) */
				/* #%c */
				if ("\0\0\0\1\1\0\0\0\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\1\1\1\1\1\0\0"[b->x.ints[0]]) {
					if (
					a->x.ints[4] == b->x.ints[4]
					) {
						/* a%n */
						/* #%c */
						if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->x.ints[0] == 3  /* d%n */
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
					}
				}
			}
			break;
		case 32:   /* sub */
			if (
			a->x.ints[4] == 3  /* l */
			&& a->x.ints[1] == 3  /* d%n */
			&& a->x.ints[2] == 2  /* a%c */
			&& a->kids[1]->x.ints[2] == 2  /* a%c */
			) {
				a->x.ints[1] = 1;  /* a%n */
				a->x.ints[2] = 4;  /* d%c */
				setreg(a, rmap[I]);
				goto L619;   /* %f%t %x,%z */
			}
			/* a%n */
			/* d%n */
			if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[1]]) {
				/* a%c */
				/* d%c */
				if ("\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[2]]) {
					b = a->kids[1];
					if (
					b->op == 629  /* mov%t %y,%z */
					&& a->x.ints[4] == b->x.ints[4]
					) {
						/* a%n */
						/* #%c */
						if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						/* a%n */
						/* d%n */
						/* #%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->count == 1
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
						if (
						b->x.ints[0] == 3  /* d%n */
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L619;   /* %f%t %x,%z */
						}
					}
				}
			}
			break;
		}
		a->op = 619;
		break;
	case 620: L620:   /* %f%t %z */
		dumptree(a, 620);
		a->op = 620;
		break;
	case 621: L621:   /* clr%t %z */
		dumptree(a, 621);
		a->op = 621;
		break;
	case 622: L622:   /* cmp%t %x,%y; j%f %z */
		dumptree(a, 622);
		switch (a->x.ints[1]) {
		case 1:   /* a%n */
		case 3:   /* d%n */
			/* a%n */
			/* a%c */
			/* d%n */
			/* d%c */
			if ("\0\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[0]]) {
				b = a->kids[1];
				if (
				b->op == 629  /* mov%t %y,%z */
				&& a->x.ints[4] == b->x.ints[4]
				) {
					/* a%n */
					/* #%c */
					if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L622;   /* cmp%t %x,%y; j%f %z */
					}
					if (
					b->count == 1
					) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L622;   /* cmp%t %x,%y; j%f %z */
					}
					/* a%n */
					/* d%n */
					/* #%c */
					if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L622;   /* cmp%t %x,%y; j%f %z */
					}
					if (
					b->count == 1
					) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L622;   /* cmp%t %x,%y; j%f %z */
					}
					/* a%n */
					/* d%n */
					/* #%c */
					if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L622;   /* cmp%t %x,%y; j%f %z */
					}
					if (
					b->count == 1
					) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L622;   /* cmp%t %x,%y; j%f %z */
					}
					/* a%n */
					/* d%n */
					/* #%c */
					if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L622;   /* cmp%t %x,%y; j%f %z */
					}
					if (
					b->count == 1
					) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L622;   /* cmp%t %x,%y; j%f %z */
					}
					if (
					b->x.ints[0] == 3  /* d%n */
					) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L622;   /* cmp%t %x,%y; j%f %z */
					}
				}
			}
			break;
		case 27:   /* #%c */
			/* a%n */
			/* d%n */
			if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[0]]) {
				b = a->kids[0];
				if (
				b->op == 629  /* mov%t %y,%z */
				) {
					/* a%n */
					/* a%c */
					/* d%n */
					/* d%c */
					/* %c:l */
					/* @(%c:l,d%i:l:1) */
					/* @(%c:l,d%i:l:2) */
					/* @(%c:l,d%i:l:4) */
					/* @(%c,d%i:l:1) */
					/* @(%c,d%i:l:2) */
					/* @(%c,d%i:l:4) */
					/* a%n@(%c,d%i:l:1) */
					/* a%n@(%c,d%i:l:2) */
					/* a%n@(%c,d%i:l:4) */
					/* a6@(%c,d%i:l:1) */
					/* a6@(%c,d%i:l:2) */
					/* a6@(%c,d%i:l:4) */
					/* a%n@ */
					/* a6@(%c) */
					/* sp@(%c) */
					/* a%n@(%c:l) */
					/* a%n@(%c) */
					if ("\0\1\1\1\1\0\0\0\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\1\1\1\1\0\0\0"[b->x.ints[0]]) {
						if (
						a->x.ints[4] == b->x.ints[4]
						) {
							if (
							b->count == 1
							) {
								if (--b->count > 0) {
									if (c=b->kids[0]) ++c->count;
									if (c=b->kids[3]) ++c->count;
								}
								a->syms[0] = b->syms[0];
								a->kids[3] = b->kids[3];
								a->x.ints[0] = b->x.ints[0];
								a->kids[0] = b->kids[0];
								goto L622;   /* cmp%t %x,%y; j%f %z */
							}
							if (
							b->x.ints[0] == 3  /* d%n */
							) {
								if (--b->count > 0) {
									if (c=b->kids[0]) ++c->count;
									if (c=b->kids[3]) ++c->count;
								}
								a->syms[0] = b->syms[0];
								a->kids[3] = b->kids[3];
								a->x.ints[0] = b->x.ints[0];
								a->kids[0] = b->kids[0];
								goto L622;   /* cmp%t %x,%y; j%f %z */
							}
						}
					}
				}
			}
			break;
		}
		if (
		a->x.ints[4] == 3  /* l */
		) {
			switch (a->x.ints[1]) {
			case 3:   /* d%n */
				b = a->kids[0];
				if (
				a->x.ints[0] == 3  /* d%n */
				&& b->op == 620  /* %f%t %z */
				) {
					switch (b->x.ints[3]) {
					case 15:   /* ext */
						b = a->kids[1];
						if (
						a->kids[0]->x.ints[4] == 3  /* l */
						&& b->op == 620  /* %f%t %z */
						&& b->x.ints[3] == 15  /* ext */
						&& b->x.ints[4] == 3  /* l */
						) {
							if (--a->kids[0]->count > 0) {
								if (c=a->kids[0]->kids[0]) ++c->count;
								if (c=a->kids[0]->kids[3]) ++c->count;
							}
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[0] = a->kids[0]->syms[0];
							a->kids[3] = a->kids[0]->kids[3];
							a->x.ints[0] = a->kids[0]->x.ints[0];
							a->kids[0] = a->kids[0]->kids[0];
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							a->x.ints[4] = 2;  /* w */
							goto L622;   /* cmp%t %x,%y; j%f %z */
						}
						break;
					case 16:   /* extb */
						b = a->kids[1];
						if (
						a->kids[0]->x.ints[4] == 3  /* l */
						&& b->op == 620  /* %f%t %z */
						&& b->x.ints[3] == 16  /* extb */
						&& b->x.ints[4] == 3  /* l */
						) {
							if (--a->kids[0]->count > 0) {
								if (c=a->kids[0]->kids[0]) ++c->count;
								if (c=a->kids[0]->kids[3]) ++c->count;
							}
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[0] = a->kids[0]->syms[0];
							a->kids[3] = a->kids[0]->kids[3];
							a->x.ints[0] = a->kids[0]->x.ints[0];
							a->kids[0] = a->kids[0]->kids[0];
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							a->x.ints[4] = 1;  /* b */
							goto L622;   /* cmp%t %x,%y; j%f %z */
						}
						break;
					}
				}
				break;
			case 27:   /* #%c */
				if (
				a->x.ints[0] == 3  /* d%n */
				) {
					/* eq */
					/* ge */
					/* gt */
					/* le */
					/* lt */
					/* ne */
					if ("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\1\1\0\1\0\0\0\1\0\0\0\1\0\0\0\0\0"[a->x.ints[3]]) {
						b = a->kids[0];
						if (
						b->op == 620  /* %f%t %z */
						) {
							switch (b->x.ints[3]) {
							case 15:   /* ext */
								if (
								b->x.ints[4] == 3  /* l */
								&& b->count == 1
								) {
									if (--b->count > 0) {
										if (c=b->kids[0]) ++c->count;
										if (c=b->kids[3]) ++c->count;
									}
									a->syms[0] = b->syms[0];
									a->kids[3] = b->kids[3];
									a->x.ints[0] = b->x.ints[0];
									a->kids[0] = b->kids[0];
									a->x.ints[4] = 2;  /* w */
									goto L622;   /* cmp%t %x,%y; j%f %z */
								}
								break;
							case 16:   /* extb */
								if (
								b->x.ints[4] == 3  /* l */
								&& b->count == 1
								) {
									if (--b->count > 0) {
										if (c=b->kids[0]) ++c->count;
										if (c=b->kids[3]) ++c->count;
									}
									a->syms[0] = b->syms[0];
									a->kids[3] = b->kids[3];
									a->x.ints[0] = b->x.ints[0];
									a->kids[0] = b->kids[0];
									a->x.ints[4] = 1;  /* b */
									goto L622;   /* cmp%t %x,%y; j%f %z */
								}
								break;
							}
						}
					}
				}
				break;
			}
		}
		a->op = 622;
		break;
	case 623: L623:   /* f%f%t %x,%z */
		dumptree(a, 623);
		b = a->kids[1];
		if (
		a->x.ints[4] == 6  /* x */
		&& a->x.ints[1] == 5  /* fp%n */
		&& a->x.ints[2] == 6  /* fp%c */
		&& b->op == 625  /* fmove%t %y,%z */
		) {
			/* d%n */
			/* d%c */
			/* %c:l */
			/* @(%c:l,d%i:l:1) */
			/* @(%c:l,d%i:l:2) */
			/* @(%c:l,d%i:l:4) */
			/* @(%c,d%i:l:1) */
			/* @(%c,d%i:l:2) */
			/* @(%c,d%i:l:4) */
			/* a%n@(%c,d%i:l:1) */
			/* a%n@(%c,d%i:l:2) */
			/* a%n@(%c,d%i:l:4) */
			/* a6@(%c,d%i:l:1) */
			/* a6@(%c,d%i:l:2) */
			/* a6@(%c,d%i:l:4) */
			/* a%n@ */
			/* a6@(%c) */
			/* sp@(%c) */
			/* a%n@(%c:l) */
			/* a%n@(%c) */
			if ("\0\0\0\1\1\0\0\0\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\1\1\1\1\0\0\0"[b->x.ints[0]]) {
				if (
				b->x.ints[0] == 27  /* #%c */
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
				/* d%n */
				/* fp%n */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L623;   /* f%f%t %x,%z */
				}
			}
		}
		a->op = 623;
		break;
	case 624: L624:   /* fcmp%t %x,%y; fj%f %z */
		dumptree(a, 624);
		b = a->kids[1];
		if (
		a->x.ints[4] == 6  /* x */
		&& a->x.ints[1] == 5  /* fp%n */
		&& a->x.ints[0] == 5  /* fp%n */
		&& b->op == 625  /* fmove%t %y,%z */
		) {
			/* d%n */
			/* d%c */
			/* %c:l */
			/* @(%c:l,d%i:l:1) */
			/* @(%c:l,d%i:l:2) */
			/* @(%c:l,d%i:l:4) */
			/* @(%c,d%i:l:1) */
			/* @(%c,d%i:l:2) */
			/* @(%c,d%i:l:4) */
			/* a%n@(%c,d%i:l:1) */
			/* a%n@(%c,d%i:l:2) */
			/* a%n@(%c,d%i:l:4) */
			/* a6@(%c,d%i:l:1) */
			/* a6@(%c,d%i:l:2) */
			/* a6@(%c,d%i:l:4) */
			/* a%n@ */
			/* a6@(%c) */
			/* sp@(%c) */
			/* a%n@(%c:l) */
			/* a%n@(%c) */
			if ("\0\0\0\1\1\0\0\0\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\1\1\1\1\0\0\0"[b->x.ints[0]]) {
				if (
				b->x.ints[0] == 27  /* #%c */
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				/* #%c */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
				/* d%n */
				/* fp%n */
				if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[1] = b->syms[0];
					a->kids[4] = b->kids[3];
					a->x.ints[1] = b->x.ints[0];
					a->kids[1] = b->kids[0];
					a->x.ints[4] = b->x.ints[4];
					goto L624;   /* fcmp%t %x,%y; fj%f %z */
				}
			}
		}
		a->op = 624;
		break;
	case 625: L625:   /* fmove%t %y,%z */
		dumptree(a, 625);
		if (
		a->x.ints[2] == 23  /* a6@(%c) */
		&& getregset(a->syms[2])==FREG
		) {
			a->x.ints[4] = 6;  /* x */
			a->x.ints[2] = 6;  /* fp%c */
			goto L625;   /* fmove%t %y,%z */
		}
		switch (a->x.ints[0]) {
		case 6:   /* fp%c */
			if (
			a->x.ints[2] == 6  /* fp%c */
			&& a->syms[RX] == rmap[D] && rmcopy(a)
			) {
				a->syms[2] = a->syms[0];
				goto L625;   /* fmove%t %y,%z */
			}
			break;
		case 21:   /* a%n@ */
			b = a->kids[0];
			if (
			b->op == 628  /* lea %y,%z */
			) {
				/* %c:l */
				/* a6@(%c) */
				if ("\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L625;   /* fmove%t %y,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L625;   /* fmove%t %y,%z */
				}
			}
			break;
		case 23:   /* a6@(%c) */
			if (
			getregset(a->syms[0])==FREG
			) {
				a->x.ints[4] = 6;  /* x */
				a->x.ints[0] = 6;  /* fp%c */
				goto L625;   /* fmove%t %y,%z */
			}
			break;
		}
		b = a->kids[2];
		if (
		a->x.ints[2] == 21  /* a%n@ */
		&& b->op == 628  /* lea %y,%z */
		) {
			/* %c:l */
			/* a6@(%c) */
			if ("\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0"[b->x.ints[0]]) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[2] = b->syms[0];
				a->kids[5] = b->kids[3];
				a->x.ints[2] = b->x.ints[0];
				a->kids[2] = b->kids[0];
				goto L625;   /* fmove%t %y,%z */
			}
			if (
			b->count == 1
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[2] = b->syms[0];
				a->kids[5] = b->kids[3];
				a->x.ints[2] = b->x.ints[0];
				a->kids[2] = b->kids[0];
				goto L625;   /* fmove%t %y,%z */
			}
		}
		b = a->kids[0];
		if (
		a->x.ints[4] == 6  /* x */
		&& a->x.ints[0] == 5  /* fp%n */
		&& a->x.ints[2] == 6  /* fp%c */
		&& b->op == 625  /* fmove%t %y,%z */
		) {
			if (
			b->x.ints[0] == 27  /* #%c */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				a->x.ints[4] = b->x.ints[4];
				goto L625;   /* fmove%t %y,%z */
			}
			if (
			b->count == 1
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				a->x.ints[4] = b->x.ints[4];
				goto L625;   /* fmove%t %y,%z */
			}
			/* d%n */
			/* fp%n */
			if ("\0\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				a->x.ints[4] = b->x.ints[4];
				goto L625;   /* fmove%t %y,%z */
			}
		}
		if (
		a->x.ints[0] == 5  /* fp%n */
		&& b->op == 625  /* fmove%t %y,%z */
		&& b->x.ints[0] == 5  /* fp%n */
		) {
			if (
			b->x.ints[4] == a->x.ints[4]
			) {
				if (--b->count > 0)
					if (c=b->kids[0]) ++c->count;
				a->kids[0] = b->kids[0];
				goto L625;   /* fmove%t %y,%z */
			}
			if (
			b->x.ints[4] == 6  /* x */
			) {
				if (--b->count > 0)
					if (c=b->kids[0]) ++c->count;
				a->kids[0] = b->kids[0];
				goto L625;   /* fmove%t %y,%z */
			}
		}
		a->op = 625;
		break;
	case 626: L626:   /* fmovex %y,%z; f%f%t %x,%z */
		dumptree(a, 626);
		a->op = 626;
		break;
	case 627: L627:   /* jra %z */
		dumptree(a, 627);
		b = a->kids[2];
		if (
		a->x.ints[2] == 21  /* a%n@ */
		&& b->count == 1
		&& b->op == 628  /* lea %y,%z */
		&& b->x.ints[0] == 8  /* %c:l */
		) {
			--b->count;
			a->x.ints[2] = 29;  /* %c */
			a->syms[2] = b->syms[0];
			a->kids[2] = 0;
			goto L627;   /* jra %z */
		}
		a->op = 627;
		break;
	case 628: L628:   /* lea %y,%z */
		dumptree(a, 628);
		switch (a->x.ints[0]) {
		case 9:   /* @(%c:l,d%i:l:1) */
			b = a->kids[3];
			if (
			b->op == 619  /* %f%t %x,%z */
			&& b->x.ints[3] == 4  /* asl */
			&& b->x.ints[4] == 3  /* l */
			&& b->x.ints[1] == 27  /* #%c */
			) {
				if (
				b->syms[1]->x.name == sym[5].x.name  /* 2 */
				) {
					if (--b->count > 0)
						if (c=b->kids[0]) ++c->count;
					a->x.ints[0] = 11;  /* @(%c:l,d%i:l:4) */
					a->kids[3] = b->kids[0];
					goto L628;   /* lea %y,%z */
				}
				if (
				b->syms[1]->x.name == sym[4].x.name  /* 1 */
				) {
					if (--b->count > 0)
						if (c=b->kids[0]) ++c->count;
					a->x.ints[0] = 10;  /* @(%c:l,d%i:l:2) */
					a->kids[3] = b->kids[0];
					goto L628;   /* lea %y,%z */
				}
			}
			break;
		case 26:   /* a%n@(%c) */
			b = a->kids[0];
			if (
			a->x.ints[2] == 2  /* a%c */
			&& b->op == 629  /* mov%t %y,%z */
			&& b->x.ints[0] == 2  /* a%c */
			&& b->count == 1
			&& a->syms[2] == b->syms[0]
			) {
				--b->count;
				a->x.ints[1] = 27;  /* #%c */
				a->x.ints[4] = 3;  /* l */
				a->x.ints[3] = 1;  /* add */
				a->syms[1] = a->syms[0];
				a->kids[0] = 0;
				goto L619;   /* %f%t %x,%z */
			}
			break;
		}
		a->op = 628;
		break;
	case 629: L629:   /* mov%t %y,%z */
		dumptree(a, 629);
		if (
		a->x.ints[2] == 23  /* a6@(%c) */
		) {
			if (
			getregset(a->syms[2])==DREG
			) {
				a->x.ints[2] = 4;  /* d%c */
				goto L629;   /* mov%t %y,%z */
			}
			if (
			getregset(a->syms[2])==AREG
			) {
				a->x.ints[2] = 2;  /* a%c */
				goto L629;   /* mov%t %y,%z */
			}
		}
		switch (a->x.ints[0]) {
		case 1:   /* a%n */
			b = a->kids[0];
			if (
			a->x.ints[2] == 2  /* a%c */
			&& b->op == 628  /* lea %y,%z */
			&& b->count == 1
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L628;   /* lea %y,%z */
			}
			break;
		case 2:   /* a%c */
			if (
			a->x.ints[2] == 2  /* a%c */
			&& a->syms[RX] == rmap[P] && rmcopy(a)
			) {
				a->syms[2] = a->syms[0];
				goto L629;   /* mov%t %y,%z */
			}
			break;
		case 4:   /* d%c */
			if (
			a->x.ints[2] == 4  /* d%c */
			&& a->syms[RX] == rmap[I] && rmcopy(a)
			) {
				a->syms[2] = a->syms[0];
				goto L629;   /* mov%t %y,%z */
			}
			break;
		case 23:   /* a6@(%c) */
			if (
			getregset(a->syms[0])==DREG
			) {
				a->x.ints[0] = 4;  /* d%c */
				goto L629;   /* mov%t %y,%z */
			}
			if (
			getregset(a->syms[0])==AREG
			) {
				a->x.ints[0] = 2;  /* a%c */
				goto L629;   /* mov%t %y,%z */
			}
			break;
		}
		if (
		a->x.ints[2] == 24  /* sp@(%c) */
		&& a->syms[2]->x.name == sym[3].x.name  /* 0 */
		) {
			a->x.ints[2] = 22;  /* sp@ */
			goto L629;   /* mov%t %y,%z */
		}
		b = a->kids[0];
		if (
		a->x.ints[0] == 21  /* a%n@ */
		&& b->op == 628  /* lea %y,%z */
		) {
			/* %c:l */
			/* a6@(%c) */
			if ("\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0"[b->x.ints[0]]) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L629;   /* mov%t %y,%z */
			}
			if (
			b->count == 1
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L629;   /* mov%t %y,%z */
			}
		}
		b = a->kids[2];
		if (
		a->x.ints[2] == 21  /* a%n@ */
		&& b->op == 628  /* lea %y,%z */
		) {
			/* %c:l */
			/* a6@(%c) */
			if ("\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0"[b->x.ints[0]]) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[2] = b->syms[0];
				a->kids[5] = b->kids[3];
				a->x.ints[2] = b->x.ints[0];
				a->kids[2] = b->kids[0];
				goto L629;   /* mov%t %y,%z */
			}
			if (
			b->count == 1
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[2] = b->syms[0];
				a->kids[5] = b->kids[3];
				a->x.ints[2] = b->x.ints[0];
				a->kids[2] = b->kids[0];
				goto L629;   /* mov%t %y,%z */
			}
		}
		b = a->kids[0];
		if (
		a->x.ints[4] == 3  /* l */
		&& a->x.ints[0] == 1  /* a%n */
		&& b->op == 628  /* lea %y,%z */
		&& b->x.ints[0] == 8  /* %c:l */
		) {
			--b->count;
			a->x.ints[0] = 27;  /* #%c */
			a->syms[0] = b->syms[0];
			a->kids[0] = 0;
			goto L629;   /* mov%t %y,%z */
		}
		switch (a->x.ints[0]) {
		case 1:   /* a%n */
			if (
			b->op == 629  /* mov%t %y,%z */
			&& b->x.ints[4] == 3  /* l */
			&& b->x.ints[0] == 1  /* a%n */
			) {
				if (--b->count > 0)
					if (c=b->kids[0]) ++c->count;
				a->kids[0] = b->kids[0];
				goto L629;   /* mov%t %y,%z */
			}
			if (
			b->op == 629  /* mov%t %y,%z */
			&& a->x.ints[4] == b->x.ints[4]
			) {
				/* a%n */
				/* #%c */
				if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L629;   /* mov%t %y,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L629;   /* mov%t %y,%z */
				}
				if (
				b->x.ints[0] == 3  /* d%n */
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L629;   /* mov%t %y,%z */
				}
			}
			if (
			b->op == 619  /* %f%t %x,%z */
			&& b->x.ints[3] == 32  /* sub */
			&& b->x.ints[1] == 3  /* d%n */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 629  /* mov%t %y,%z */
			&& a->x.ints[4] == b->kids[0]->x.ints[4]
			&& b->kids[0]->count == 1
			&& b->kids[0]->x.ints[0] == a->x.ints[2]
			&& b->kids[0]->kids[0] == a->kids[2]
			&& b->kids[0]->kids[3] == a->kids[5]
			&& b->kids[0]->syms[0] == a->syms[2]
			) {
				if (--b->count > 0) {
					if (c=b->kids[1]) ++c->count;
					if (c=b->kids[4]) ++c->count;
				}
				--b->kids[0]->count;
				b->kids[0]->kids[0] && --b->kids[0]->kids[0]->count;
				b->kids[0]->kids[3] && --b->kids[0]->kids[3]->count;
				a->x.ints[3] = 32;  /* sub */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L619;   /* %f%t %x,%z */
			}
			if (
			b->op == 619  /* %f%t %x,%z */
			&& b->x.ints[3] == 1  /* add */
			&& b->x.ints[1] == 3  /* d%n */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 629  /* mov%t %y,%z */
			&& a->x.ints[4] == b->kids[0]->x.ints[4]
			&& b->kids[0]->count == 1
			&& b->kids[0]->x.ints[0] == a->x.ints[2]
			&& b->kids[0]->kids[0] == a->kids[2]
			&& b->kids[0]->kids[3] == a->kids[5]
			&& b->kids[0]->syms[0] == a->syms[2]
			) {
				if (--b->count > 0) {
					if (c=b->kids[1]) ++c->count;
					if (c=b->kids[4]) ++c->count;
				}
				--b->kids[0]->count;
				b->kids[0]->kids[0] && --b->kids[0]->kids[0]->count;
				b->kids[0]->kids[3] && --b->kids[0]->kids[3]->count;
				a->x.ints[3] = 1;  /* add */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L619;   /* %f%t %x,%z */
			}
			if (
			b->op == 619  /* %f%t %x,%z */
			&& b->x.ints[3] == 32  /* sub */
			&& b->x.ints[1] == 27  /* #%c */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 629  /* mov%t %y,%z */
			&& a->x.ints[4] == b->kids[0]->x.ints[4]
			&& b->kids[0]->count == 1
			&& b->kids[0]->x.ints[0] == a->x.ints[2]
			&& b->kids[0]->kids[0] == a->kids[2]
			&& b->kids[0]->kids[3] == a->kids[5]
			&& b->kids[0]->syms[0] == a->syms[2]
			) {
				if (--b->count > 0) {
					if (c=b->kids[1]) ++c->count;
					if (c=b->kids[4]) ++c->count;
				}
				--b->kids[0]->count;
				b->kids[0]->kids[0] && --b->kids[0]->kids[0]->count;
				b->kids[0]->kids[3] && --b->kids[0]->kids[3]->count;
				a->x.ints[3] = 32;  /* sub */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L619;   /* %f%t %x,%z */
			}
			if (
			b->op == 619  /* %f%t %x,%z */
			&& b->x.ints[3] == 1  /* add */
			&& b->x.ints[1] == 27  /* #%c */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 629  /* mov%t %y,%z */
			&& a->x.ints[4] == b->kids[0]->x.ints[4]
			&& b->kids[0]->count == 1
			&& b->kids[0]->x.ints[0] == a->x.ints[2]
			&& b->kids[0]->kids[0] == a->kids[2]
			&& b->kids[0]->kids[3] == a->kids[5]
			&& b->kids[0]->syms[0] == a->syms[2]
			) {
				if (--b->count > 0) {
					if (c=b->kids[1]) ++c->count;
					if (c=b->kids[4]) ++c->count;
				}
				--b->kids[0]->count;
				b->kids[0]->kids[0] && --b->kids[0]->kids[0]->count;
				b->kids[0]->kids[3] && --b->kids[0]->kids[3]->count;
				a->x.ints[3] = 1;  /* add */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L619;   /* %f%t %x,%z */
			}
			if (
			b->op == 619  /* %f%t %x,%z */
			&& b->x.ints[3] == 32  /* sub */
			&& b->x.ints[1] == 1  /* a%n */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 629  /* mov%t %y,%z */
			&& a->x.ints[4] == b->kids[0]->x.ints[4]
			&& b->kids[0]->count == 1
			&& b->kids[0]->x.ints[0] == a->x.ints[2]
			&& b->kids[0]->kids[0] == a->kids[2]
			&& b->kids[0]->kids[3] == a->kids[5]
			&& b->kids[0]->syms[0] == a->syms[2]
			) {
				if (--b->count > 0) {
					if (c=b->kids[1]) ++c->count;
					if (c=b->kids[4]) ++c->count;
				}
				--b->kids[0]->count;
				b->kids[0]->kids[0] && --b->kids[0]->kids[0]->count;
				b->kids[0]->kids[3] && --b->kids[0]->kids[3]->count;
				a->x.ints[3] = 32;  /* sub */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L619;   /* %f%t %x,%z */
			}
			if (
			b->op == 619  /* %f%t %x,%z */
			&& b->x.ints[3] == 1  /* add */
			&& b->x.ints[1] == 1  /* a%n */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 629  /* mov%t %y,%z */
			&& a->x.ints[4] == b->kids[0]->x.ints[4]
			&& b->kids[0]->count == 1
			&& b->kids[0]->x.ints[0] == a->x.ints[2]
			&& b->kids[0]->kids[0] == a->kids[2]
			&& b->kids[0]->kids[3] == a->kids[5]
			&& b->kids[0]->syms[0] == a->syms[2]
			) {
				if (--b->count > 0) {
					if (c=b->kids[1]) ++c->count;
					if (c=b->kids[4]) ++c->count;
				}
				--b->kids[0]->count;
				b->kids[0]->kids[0] && --b->kids[0]->kids[0]->count;
				b->kids[0]->kids[3] && --b->kids[0]->kids[3]->count;
				a->x.ints[3] = 1;  /* add */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L619;   /* %f%t %x,%z */
			}
			break;
		case 3:   /* d%n */
			if (
			b->op == 629  /* mov%t %y,%z */
			&& b->x.ints[4] == 3  /* l */
			&& b->x.ints[0] == 3  /* d%n */
			) {
				if (--b->count > 0)
					if (c=b->kids[0]) ++c->count;
				a->kids[0] = b->kids[0];
				goto L629;   /* mov%t %y,%z */
			}
			if (
			b->op == 629  /* mov%t %y,%z */
			&& a->x.ints[4] == b->x.ints[4]
			) {
				/* a%n */
				/* #%c */
				if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L629;   /* mov%t %y,%z */
				}
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L629;   /* mov%t %y,%z */
				}
				if (
				b->x.ints[0] == 3  /* d%n */
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L629;   /* mov%t %y,%z */
				}
			}
			if (
			b->op == 619  /* %f%t %x,%z */
			&& b->x.ints[3] == 32  /* sub */
			&& b->x.ints[1] == 3  /* d%n */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 629  /* mov%t %y,%z */
			&& a->x.ints[4] == b->kids[0]->x.ints[4]
			&& b->kids[0]->count == 1
			&& b->kids[0]->x.ints[0] == a->x.ints[2]
			&& b->kids[0]->kids[0] == a->kids[2]
			&& b->kids[0]->kids[3] == a->kids[5]
			&& b->kids[0]->syms[0] == a->syms[2]
			) {
				if (--b->count > 0) {
					if (c=b->kids[1]) ++c->count;
					if (c=b->kids[4]) ++c->count;
				}
				--b->kids[0]->count;
				b->kids[0]->kids[0] && --b->kids[0]->kids[0]->count;
				b->kids[0]->kids[3] && --b->kids[0]->kids[3]->count;
				a->x.ints[3] = 32;  /* sub */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L619;   /* %f%t %x,%z */
			}
			if (
			b->op == 619  /* %f%t %x,%z */
			&& b->x.ints[3] == 1  /* add */
			&& b->x.ints[1] == 3  /* d%n */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 629  /* mov%t %y,%z */
			&& a->x.ints[4] == b->kids[0]->x.ints[4]
			&& b->kids[0]->count == 1
			&& b->kids[0]->x.ints[0] == a->x.ints[2]
			&& b->kids[0]->kids[0] == a->kids[2]
			&& b->kids[0]->kids[3] == a->kids[5]
			&& b->kids[0]->syms[0] == a->syms[2]
			) {
				if (--b->count > 0) {
					if (c=b->kids[1]) ++c->count;
					if (c=b->kids[4]) ++c->count;
				}
				--b->kids[0]->count;
				b->kids[0]->kids[0] && --b->kids[0]->kids[0]->count;
				b->kids[0]->kids[3] && --b->kids[0]->kids[3]->count;
				a->x.ints[3] = 1;  /* add */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L619;   /* %f%t %x,%z */
			}
			if (
			b->op == 619  /* %f%t %x,%z */
			&& b->x.ints[3] == 32  /* sub */
			&& b->x.ints[1] == 27  /* #%c */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 629  /* mov%t %y,%z */
			&& a->x.ints[4] == b->kids[0]->x.ints[4]
			&& b->kids[0]->count == 1
			&& b->kids[0]->x.ints[0] == a->x.ints[2]
			&& b->kids[0]->kids[0] == a->kids[2]
			&& b->kids[0]->kids[3] == a->kids[5]
			&& b->kids[0]->syms[0] == a->syms[2]
			) {
				if (--b->count > 0) {
					if (c=b->kids[1]) ++c->count;
					if (c=b->kids[4]) ++c->count;
				}
				--b->kids[0]->count;
				b->kids[0]->kids[0] && --b->kids[0]->kids[0]->count;
				b->kids[0]->kids[3] && --b->kids[0]->kids[3]->count;
				a->x.ints[3] = 32;  /* sub */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L619;   /* %f%t %x,%z */
			}
			if (
			b->op == 619  /* %f%t %x,%z */
			&& b->x.ints[3] == 1  /* add */
			&& b->x.ints[1] == 27  /* #%c */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 629  /* mov%t %y,%z */
			&& a->x.ints[4] == b->kids[0]->x.ints[4]
			&& b->kids[0]->count == 1
			&& b->kids[0]->x.ints[0] == a->x.ints[2]
			&& b->kids[0]->kids[0] == a->kids[2]
			&& b->kids[0]->kids[3] == a->kids[5]
			&& b->kids[0]->syms[0] == a->syms[2]
			) {
				if (--b->count > 0) {
					if (c=b->kids[1]) ++c->count;
					if (c=b->kids[4]) ++c->count;
				}
				--b->kids[0]->count;
				b->kids[0]->kids[0] && --b->kids[0]->kids[0]->count;
				b->kids[0]->kids[3] && --b->kids[0]->kids[3]->count;
				a->x.ints[3] = 1;  /* add */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L619;   /* %f%t %x,%z */
			}
			if (
			b->op == 619  /* %f%t %x,%z */
			&& b->x.ints[3] == 32  /* sub */
			&& b->x.ints[1] == 1  /* a%n */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 629  /* mov%t %y,%z */
			&& a->x.ints[4] == b->kids[0]->x.ints[4]
			&& b->kids[0]->count == 1
			&& b->kids[0]->x.ints[0] == a->x.ints[2]
			&& b->kids[0]->kids[0] == a->kids[2]
			&& b->kids[0]->kids[3] == a->kids[5]
			&& b->kids[0]->syms[0] == a->syms[2]
			) {
				if (--b->count > 0) {
					if (c=b->kids[1]) ++c->count;
					if (c=b->kids[4]) ++c->count;
				}
				--b->kids[0]->count;
				b->kids[0]->kids[0] && --b->kids[0]->kids[0]->count;
				b->kids[0]->kids[3] && --b->kids[0]->kids[3]->count;
				a->x.ints[3] = 32;  /* sub */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L619;   /* %f%t %x,%z */
			}
			if (
			b->op == 619  /* %f%t %x,%z */
			&& b->x.ints[3] == 1  /* add */
			&& b->x.ints[1] == 1  /* a%n */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 629  /* mov%t %y,%z */
			&& a->x.ints[4] == b->kids[0]->x.ints[4]
			&& b->kids[0]->count == 1
			&& b->kids[0]->x.ints[0] == a->x.ints[2]
			&& b->kids[0]->kids[0] == a->kids[2]
			&& b->kids[0]->kids[3] == a->kids[5]
			&& b->kids[0]->syms[0] == a->syms[2]
			) {
				if (--b->count > 0) {
					if (c=b->kids[1]) ++c->count;
					if (c=b->kids[4]) ++c->count;
				}
				--b->kids[0]->count;
				b->kids[0]->kids[0] && --b->kids[0]->kids[0]->count;
				b->kids[0]->kids[3] && --b->kids[0]->kids[3]->count;
				a->x.ints[3] = 1;  /* add */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L619;   /* %f%t %x,%z */
			}
			break;
		}
		a->op = 629;
		break;
	case 630: L630:   /* mov%t %y,%z; %f%t %x,%z */
		dumptree(a, 630);
		a->op = 630;
		break;
	case 631: L631:   /* mov%t %y,%z; %f%t %z */
		dumptree(a, 631);
		a->op = 631;
		break;
	case 632: L632:   /* tst%t %y; j%f %z */
		dumptree(a, 632);
		a->op = 632;
		break;
	case 633: L633:   /* %z: */
		dumptree(a, 633);
		a->op = 633;
		break;
	default:
	assert(0);
	}
}


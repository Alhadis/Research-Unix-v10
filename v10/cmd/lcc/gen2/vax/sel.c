/* "MOV %f,x,$%0,(r%0),-(sp)" => ="MOV %f,%t,%x,%y,%z" E="x" B="$%c" b=a A="(r%n)" 0=0 C="-(sp)" */
/* "MOV %f,l,%x,r%0,-(sp)" => ="MOV %f,%t,%x,%y,%z" E="l" A="r%n" 0=0 C="-(sp)" */
/* "MOV %f,x,$%0,(r%1),(r%0)" => ="MOV %f,%t,%x,%y,%z" E="x" B="$%c" b=a A="(r%n)" 0=1 C="(r%n)" 2=0 */
/* "CALL %f,%t,$%0/4,(r%0),%z" => ="CALL %f,%t,%x,%y,%z" B="$%c/4" b=a A="(r%n)" 0=0 */
/* "CALL %f,%t,$%0/4,(r%0),%z" => ="CALL %f,%t,%x,%y,%z" B="$%c/4" b=a A="(r%n)" 0=0 */
/* "CALL %f,%t,$%0/4,(r%0),%z" => ="CALL %f,%t,%x,%y,%z" B="$%c/4" b=a A="(r%n)" 0=0 */
/* "MOV %f,l,%x,r%0,r%c" => ="MOV %f,%t,%x,%y,%z" E="l" A="r%n" 0=0 C="r%c" */
/* "jmp (r%0)" => ="jmp %z" C="(r%n)" 2=0 */
/* "EXT %f,l,r%1,r%0,r%c" => ="EXT %f,%t,%x,%y,%z" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "MOVA %f,l,%x,%0,r%c" => ="MOVA %f,%t,%x,%y,%z" E="l" A="%c" a=a C="r%c" */
/* "MOVA %f,l,%x,%0(fp),r%c" => ="MOVA %f,%t,%x,%y,%z" E="l" A="%c(fp)" a=a C="r%c" */
/* "MOVA %f,l,%x,%0(ap),r%c" => ="MOVA %f,%t,%x,%y,%z" E="l" A="%c(ap)" a=a C="r%c" */
/* "pushl r%1\npushl r%0\ncalls $2,udiv" => ="pushl %x\npushl %y\ncalls $2,%f" B="r%n" 1=1 A="r%n" 0=0 D="udiv" */
/* "pushl r%1\npushl r%0\ncalls $2,urem" => ="pushl %x\npushl %y\ncalls $2,%f" B="r%n" 1=1 A="r%n" 0=0 D="urem" */
/* "%0:" => ="%z:" C="%c" c=a */
/* "MOV %f,d,%x,r%1,(r%0)" => ="MOV %f,%t,%x,%y,%z" E="d" A="r%n" 0=1 C="(r%n)" 2=0 */
/* "MOV %f,b,%x,r%1,(r%0)" => ="MOV %f,%t,%x,%y,%z" E="b" A="r%n" 0=1 C="(r%n)" 2=0 */
/* "MOV %f,l,%x,r%1,(r%0)" => ="MOV %f,%t,%x,%y,%z" E="l" A="r%n" 0=1 C="(r%n)" 2=0 */
/* "MOV %f,w,%x,r%1,(r%0)" => ="MOV %f,%t,%x,%y,%z" E="w" A="r%n" 0=1 C="(r%n)" 2=0 */
/* "MOV %f,f,%x,r%1,(r%0)" => ="MOV %f,%t,%x,%y,%z" E="f" A="r%n" 0=1 C="(r%n)" 2=0 */
/* "MOV %f,l,%x,r%1,(r%0)" => ="MOV %f,%t,%x,%y,%z" E="l" A="r%n" 0=1 C="(r%n)" 2=0 */
/* "CALL %f,%t,$%0/4,(r%0),%z" => ="CALL %f,%t,%x,%y,%z" B="$%c/4" b=a A="(r%n)" 0=0 */
/* "CALL %f,%t,$%0/4,(r%0),%z" => ="CALL %f,%t,%x,%y,%z" B="$%c/4" b=a A="(r%n)" 0=0 */
/* "CALL %f,%t,$%0/4,(r%0),%z" => ="CALL %f,%t,%x,%y,%z" B="$%c/4" b=a A="(r%n)" 0=0 */
/* "CALL %f,%t,$%0/4,(r%0),%z" => ="CALL %f,%t,%x,%y,%z" B="$%c/4" b=a A="(r%n)" 0=0 */
/* "MOV %f,l,%x,$%0,r%c" => ="MOV %f,%t,%x,%y,%z" E="l" A="$%c" a=a C="r%c" */
/* "MOV %f,l,%x,$%0,r%c" => ="MOV %f,%t,%x,%y,%z" E="l" A="$%c" a=a C="r%c" */
/* "MOV %f,l,%x,$%0,r%c" => ="MOV %f,%t,%x,%y,%z" E="l" A="$%c" a=a C="r%c" */
/* "MOV %f,l,%x,$%0,r%c" => ="MOV %f,%t,%x,%y,%z" E="l" A="$%c" a=a C="r%c" */
/* "MOV %f,l,%x,$%0,r%c" => ="MOV %f,%t,%x,%y,%z" E="l" A="$%c" a=a C="r%c" */
/* "MOV %f,l,%x,$%0,r%c" => ="MOV %f,%t,%x,%y,%z" E="l" A="$%c" a=a C="r%c" */
/* "MOV %f,l,%x,$%0,r%c" => ="MOV %f,%t,%x,%y,%z" E="l" A="$%c" a=a C="r%c" */
/* "MOV %f,d,%x,(r%0),r%c" => ="MOV %f,%t,%x,%y,%z" E="d" A="(r%n)" 0=0 C="r%c" */
/* "MOV %f,b,%x,(r%0),r%c" => ="MOV %f,%t,%x,%y,%z" E="b" A="(r%n)" 0=0 C="r%c" */
/* "MOV %f,l,%x,(r%0),r%c" => ="MOV %f,%t,%x,%y,%z" E="l" A="(r%n)" 0=0 C="r%c" */
/* "MOV %f,w,%x,(r%0),r%c" => ="MOV %f,%t,%x,%y,%z" E="w" A="(r%n)" 0=0 C="r%c" */
/* "MOV %f,f,%x,(r%0),r%c" => ="MOV %f,%t,%x,%y,%z" E="f" A="(r%n)" 0=0 C="r%c" */
/* "MOV %f,l,%x,(r%0),r%c" => ="MOV %f,%t,%x,%y,%z" E="l" A="(r%n)" 0=0 C="r%c" */
/* "MOV %f,d,%x,r%0,-(sp)" => ="MOV %f,%t,%x,%y,%z" E="d" A="r%n" 0=0 C="-(sp)" */
/* "MOV %f,l,%x,r%0,-(sp)" => ="MOV %f,%t,%x,%y,%z" E="l" A="r%n" 0=0 C="-(sp)" */
/* "MOV %f,f,%x,r%0,-(sp)" => ="MOV %f,%t,%x,%y,%z" E="f" A="r%n" 0=0 C="-(sp)" */
/* "MOV %f,l,%x,r%0,-(sp)" => ="MOV %f,%t,%x,%y,%z" E="l" A="r%n" 0=0 C="-(sp)" */
/* "MOV %f,d,%x,r%0,r%c" => ="MOV %f,%t,%x,%y,%z" E="d" A="r%n" 0=0 C="r%c" */
/* "MOV %f,b,%x,r%0,r%c" => ="MOV %f,%t,%x,%y,%z" E="b" A="r%n" 0=0 C="r%c" */
/* "MOV %f,l,%x,r%0,r%c" => ="MOV %f,%t,%x,%y,%z" E="l" A="r%n" 0=0 C="r%c" */
/* "MOV %f,w,%x,r%0,r%c" => ="MOV %f,%t,%x,%y,%z" E="w" A="r%n" 0=0 C="r%c" */
/* "MOV %f,f,%x,r%0,r%c" => ="MOV %f,%t,%x,%y,%z" E="f" A="r%n" 0=0 C="r%c" */
/* "MOV %f,l,%x,r%0,r%c" => ="MOV %f,%t,%x,%y,%z" E="l" A="r%n" 0=0 C="r%c" */
/* "MOV %f,l,%x,r%0,r%c" => ="MOV %f,%t,%x,%y,%z" E="l" A="r%n" 0=0 C="r%c" */
/* "MOVZ b,l,%x,r%0,r%c" => ="MOVZ %f,%t,%x,%y,%z" D="b" E="l" A="r%n" 0=0 C="r%c" */
/* "MOVZ w,l,%x,r%0,r%c" => ="MOVZ %f,%t,%x,%y,%z" D="w" E="l" A="r%n" 0=0 C="r%c" */
/* "ASH %f,l,r%1,r%0,r%c" => ="ASH %f,%t,%x,%y,%z" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "ASH %f,l,r%1,r%0,r%c" => ="ASH %f,%t,%x,%y,%z" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "ASH %f,l,r%1,r%0,r%c" => ="ASH %f,%t,%x,%y,%z" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 bic,l,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="bic" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 sub,d,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="sub" E="d" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 sub,l,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="sub" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 sub,f,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="sub" E="f" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 sub,l,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="sub" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 sub,l,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="sub" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 mul,d,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="mul" E="d" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 mul,l,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="mul" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 mul,f,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="mul" E="f" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 mul,l,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="mul" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 xor,l,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="xor" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 bis,l,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="bis" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 div,d,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="div" E="d" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 div,l,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="div" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 div,f,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="div" E="f" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 div,l,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="div" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 mod,l,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="mod" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 mod,l,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="mod" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 add,d,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="add" E="d" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 add,l,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="add" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 add,f,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="add" E="f" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 add,l,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="add" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "OP2 add,l,r%1,r%0,r%c" => ="OP2 %f,%t,%x,%y,%z" D="add" E="l" B="r%n" 1=1 A="r%n" 0=0 C="r%c" */
/* "CMP gequ,l,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="gequ" E="l" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP lssu,l,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="lssu" E="l" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP lequ,l,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="lequ" E="l" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP gtru,l,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="gtru" E="l" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP geq,d,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="geq" E="d" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP geq,l,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="geq" E="l" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP geq,f,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="geq" E="f" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP geq,l,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="geq" E="l" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP lss,d,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="lss" E="d" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP lss,l,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="lss" E="l" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP lss,f,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="lss" E="f" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP lss,l,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="lss" E="l" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP neq,d,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="neq" E="d" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP neq,l,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="neq" E="l" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP neq,f,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="neq" E="f" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP eql,d,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="eql" E="d" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP eql,l,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="eql" E="l" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP eql,f,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="eql" E="f" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP leq,d,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="leq" E="d" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP leq,l,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="leq" E="l" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP leq,f,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="leq" E="f" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP leq,l,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="leq" E="l" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP gtr,d,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="gtr" E="d" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP gtr,l,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="gtr" E="l" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP gtr,f,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="gtr" E="f" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CMP gtr,l,r%1,r%0,%0" => ="CMP %f,%t,%x,%y,%z" D="gtr" E="l" B="r%n" 1=1 A="r%n" 0=0 C="%c" c=a */
/* "CVT d,l,%x,r%0,r%c" => ="CVT %f,%t,%x,%y,%z" D="d" E="l" A="r%n" 0=0 C="r%c" */
/* "CVT d,f,%x,r%0,r%c" => ="CVT %f,%t,%x,%y,%z" D="d" E="f" A="r%n" 0=0 C="r%c" */
/* "CVT b,l,%x,r%0,r%c" => ="CVT %f,%t,%x,%y,%z" D="b" E="l" A="r%n" 0=0 C="r%c" */
/* "CVT b,l,%x,r%0,r%c" => ="CVT %f,%t,%x,%y,%z" D="b" E="l" A="r%n" 0=0 C="r%c" */
/* "CVT l,d,%x,r%0,r%c" => ="CVT %f,%t,%x,%y,%z" D="l" E="d" A="r%n" 0=0 C="r%c" */
/* "CVT l,b,%x,r%0,r%c" => ="CVT %f,%t,%x,%y,%z" D="l" E="b" A="r%n" 0=0 C="r%c" */
/* "CVT l,w,%x,r%0,r%c" => ="CVT %f,%t,%x,%y,%z" D="l" E="w" A="r%n" 0=0 C="r%c" */
/* "CVT l,l,%x,r%0,r%c" => ="CVT %f,%t,%x,%y,%z" D="l" E="l" A="r%n" 0=0 C="r%c" */
/* "CVT w,l,%x,r%0,r%c" => ="CVT %f,%t,%x,%y,%z" D="w" E="l" A="r%n" 0=0 C="r%c" */
/* "CVT w,l,%x,r%0,r%c" => ="CVT %f,%t,%x,%y,%z" D="w" E="l" A="r%n" 0=0 C="r%c" */
/* "CVT f,d,%x,r%0,r%c" => ="CVT %f,%t,%x,%y,%z" D="f" E="d" A="r%n" 0=0 C="r%c" */
/* "CVT l,l,%x,r%0,r%c" => ="CVT %f,%t,%x,%y,%z" D="l" E="l" A="r%n" 0=0 C="r%c" */
/* "CVT l,b,%x,r%0,r%c" => ="CVT %f,%t,%x,%y,%z" D="l" E="b" A="r%n" 0=0 C="r%c" */
/* "CVT l,l,%x,r%0,r%c" => ="CVT %f,%t,%x,%y,%z" D="l" E="l" A="r%n" 0=0 C="r%c" */
/* "CVT l,w,%x,r%0,r%c" => ="CVT %f,%t,%x,%y,%z" D="l" E="w" A="r%n" 0=0 C="r%c" */
/* "CVT l,l,%x,r%0,r%c" => ="CVT %f,%t,%x,%y,%z" D="l" E="l" A="r%n" 0=0 C="r%c" */
/* "OP1 mneg,d,%x,r%0,r%c" => ="OP1 %f,%t,%x,%y,%z" D="mneg" E="d" A="r%n" 0=0 C="r%c" */
/* "OP1 mneg,l,%x,r%0,r%c" => ="OP1 %f,%t,%x,%y,%z" D="mneg" E="l" A="r%n" 0=0 C="r%c" */
/* "OP1 mneg,f,%x,r%0,r%c" => ="OP1 %f,%t,%x,%y,%z" D="mneg" E="f" A="r%n" 0=0 C="r%c" */
/* "OP1 mcom,l,%x,r%0,r%c" => ="OP1 %f,%t,%x,%y,%z" D="mcom" E="l" A="r%n" 0=0 C="r%c" */
/* "ASH %f,l,$1,r%n,r%c" => ="ASH %f,%t,%x,%y,%z" E="l" B="$%c" b="1" A="r%n" C="r%c" */
/* "MOVA %f,w,%x,0[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" E="w" A="%c[r%i]" a="0" */
/* "ASH %f,l,$3,r%n,r%c" => ="ASH %f,%t,%x,%y,%z" E="l" B="$%c" b="3" A="r%n" C="r%c" */
/* "MOVA %f,d,%x,0[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" E="d" A="%c[r%i]" a="0" */
/* "ASH %f,l,$2,r%n,r%c" => ="ASH %f,%t,%x,%y,%z" E="l" B="$%c" b="2" A="r%n" C="r%c" */
/* "MOVA %f,l,%x,0[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" E="l" A="%c[r%i]" a="0" */
/* "ASH %f,{t},r%n,%y,%z" => ="ASH %f,%t,%x,%y,%z" E=@TMP1026 B="r%n" */
/* "MOV %f,?1,%x,{lit},%z" => ="MOV %f,%t,%x,%y,%z" 1E=E 1A=@TMP1032 */
/* "ASH %f,{t},%x,r%n,%z" => ="ASH %f,%t,%x,%y,%z" E=@TMP1037 A="r%n" */
/* "MOV %f,?1,%x,{lit},%z" => ="MOV %f,%t,%x,%y,%z" 0E=E 0A=@TMP1043 */
/* "ASH %f,{t},%x,r%n,%z" => ="ASH %f,%t,%x,%y,%z" E=@TMP1048 A="r%n" */
/* "MOV %f,?1,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 0E=E */
/* "CALL %f,%t,%x,(r%n),%z" => ="CALL %f,%t,%x,%y,%z" A="(r%n)" */
/* "MOVA %f,%t,%x,{nox},%z" => ="MOVA %f,%t,%x,%y,%z" 0A=@TMP1063 */
/* "CALL %f,%t,%x,(r%n),%z" => ="CALL %f,%t,%x,%y,%z" A="(r%n)" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="(r%n)" */
/* "CALL %f,%t,%x,(r%n),%z" => ="CALL %f,%t,%x,%y,%z" A="(r%n)" */
/* "MOV %f,%t,%x,%c(fp),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(fp)" */
/* "CALL %f,%t,%x,(r%n),%z" => ="CALL %f,%t,%x,%y,%z" A="(r%n)" */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" 0A="r%n" */
/* "CALL %f,%t,%x,(r%n),%z" => ="CALL %f,%t,%x,%y,%z" A="(r%n)" */
/* "MOV %f,%t,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="$%c" */
/* "CALL %f,%t,%x,(r%n),%z" => ="CALL %f,%t,%x,%y,%z" A="(r%n)" */
/* "MOV %f,%t,%x,%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c" */
/* "CALL %f,%t,%x,(r%n),%z" => ="CALL %f,%t,%x,%y,%z" A="(r%n)" */
/* "MOV %f,%t,%x,%c(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(r%n)" */
/* "CALL %f,%t,%x,(r%n),%z" => ="CALL %f,%t,%x,%y,%z" A="(r%n)" */
/* "MOV %f,%t,%x,%c(ap),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(ap)" */
/* "CMP %f,{t},r%n,%y,%z" => ="CMP %f,%t,%x,%y,%z" E=@TMP1138 B="r%n" */
/* "MOV %f,?1,%x,{lit},%z" => ="MOV %f,%t,%x,%y,%z" 1E=E 1A=@TMP1144 */
/* "CMP %f,{t},r%n,%y,%z" => ="CMP %f,%t,%x,%y,%z" E=@TMP1149 B="r%n" */
/* "MOV %f,?1,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 1E=E */
/* "CMP %f,b,r%n,%y,%z" => ="CMP %f,%t,%x,%y,%z" E="b" B="r%n" */
/* "CVT b,l,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 1D="b" 1E="l" */
/* "CMP %f,w,r%n,%y,%z" => ="CMP %f,%t,%x,%y,%z" E="w" B="r%n" */
/* "CVT w,l,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 1D="w" 1E="l" */
/* "CMP %f,{t},%x,r%n,%z" => ="CMP %f,%t,%x,%y,%z" E=@TMP1183 A="r%n" */
/* "MOV %f,?1,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 0E=E */
/* "CMP %f,b,%x,r%n,%z" => ="CMP %f,%t,%x,%y,%z" E="b" A="r%n" */
/* "CVT b,l,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 0D="b" 0E="l" */
/* "CMP %f,w,%x,r%n,%z" => ="CMP %f,%t,%x,%y,%z" E="w" A="r%n" */
/* "CVT w,l,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 0D="w" 0E="l" */
/* "CVT %f,%t,%x,r%n,%z" => ="CVT %f,%t,%x,%y,%z" A="r%n" */
/* "CVT b,%t,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 0D="b" */
/* "CVT b,%t,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" D="b" */
/* "CVT %f,%t,%x,r%n,%z" => ="CVT %f,%t,%x,%y,%z" A="r%n" */
/* "CVT w,{l|f|d},%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 0D="w" 0E=@TMP1238 */
/* "CVT w,%t,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" D="w" */
/* "CVT %f,%t,%x,r%n,%z" => ="CVT %f,%t,%x,%y,%z" A="r%n" */
/* "CVT {l|f},d,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 0D=@TMP1253 0E="d" */
/* "CVT ?1,%t,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" D=0D */
/* "CVT l,w,%x,r%n,%z" => ="CVT %f,%t,%x,%y,%z" D="l" E="w" A="r%n" */
/* "MOVZ b,l,%x,{noxrc},%z" => ="MOVZ %f,%t,%x,%y,%z" 0D="b" 0E="l" 0A=@TMP1272 */
/* "MOVZ b,%t,%x,%y,%z" => ="MOVZ %f,%t,%x,%y,%z" D="b" */
/* "CVT l,{b|w},%x,r%n,%z" => ="CVT %f,%t,%x,%y,%z" D="l" E=@TMP1283 A="r%n" */
/* "MOVZ %f,l,%x,{noxrc},%z" => ="MOVZ %f,%t,%x,%y,%z" 0E="l" 0A=@TMP1290 */
/* "CVT d,d,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" D="d" E="d" */
/* "CVT l,l,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" D="l" E="l" */
/* "CVT b,b,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" D="b" E="b" */
/* "CVT f,f,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" D="f" E="f" */
/* "CVT w,w,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" D="w" E="w" */
/* "CVT l,{b|w},%x,{noxrc},%z" => ="CVT %f,%t,%x,%y,%z" D="l" E=@TMP1326 A=@TMP1327 */
/* "CVT w,b,%x,{noxrc},%z" => ="CVT %f,%t,%x,%y,%z" D="w" E="b" A=@TMP1334 */
/* "CVT %f,{t},%x,r%n,%z" => ="CVT %f,%t,%x,%y,%z" E=@TMP1339 A="r%n" */
/* "MOV %f,?1,%x,{lit},%z" => ="MOV %f,%t,%x,%y,%z" 0E=E 0A=@TMP1345 */
/* "CVT d,%t,%x,r%n,%z" => ="CVT %f,%t,%x,%y,%z" D="d" A="r%n" */
/* "MOV %f,d,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 0E="d" */
/* "CVT l,%t,%x,r%n,%z" => ="CVT %f,%t,%x,%y,%z" D="l" A="r%n" */
/* "MOV %f,l,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 0E="l" */
/* "CVT b,%t,%x,r%n,%z" => ="CVT %f,%t,%x,%y,%z" D="b" A="r%n" */
/* "MOV %f,b,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 0E="b" */
/* "CVT f,%t,%x,r%n,%z" => ="CVT %f,%t,%x,%y,%z" D="f" A="r%n" */
/* "MOV %f,f,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 0E="f" */
/* "CVT w,%t,%x,r%n,%z" => ="CVT %f,%t,%x,%y,%z" D="w" A="r%n" */
/* "MOV %f,w,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 0E="w" */
/* "EXT %f,{t},r%n,%y,%z" => ="EXT %f,%t,%x,%y,%z" E=@TMP1405 B="r%n" */
/* "MOV %f,?1,%x,{lit},%z" => ="MOV %f,%t,%x,%y,%z" 1E=E 1A=@TMP1411 */
/* "EXT %f,{t},r%n,%y,%z" => ="EXT %f,%t,%x,%y,%z" E=@TMP1416 B="r%n" */
/* "MOV %f,?1,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 1E=E */
/* "jmp (r%n)" => ="jmp %z" C="(r%n)" */
/* "MOV %f,l,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 2E="l" 2A="$%c" */
/* "jbr %c" => ="jbr %z" C="%c" */
/* "MOV %f,%t,%x,%y,{"%c(ap)"|"%c(fp)"}" => ="MOV %f,%t,%x,%y,%z" C=@TMP1444 */
/* "MOV %f,%t,%x,%y,(r%n)" => ="MOV %f,%t,%x,%y,%z" C="(r%n)" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 2A="(r%n)" */
/* "MOV %f,%t,%x,%y,(r%n)" => ="MOV %f,%t,%x,%y,%z" C="(r%n)" */
/* "MOV %f,%t,%x,%c(fp),%z" => ="MOV %f,%t,%x,%y,%z" 2A="%c(fp)" */
/* "MOV %f,%t,%x,%y,(r%n)" => ="MOV %f,%t,%x,%y,%z" C="(r%n)" */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" 2A="r%n" */
/* "MOV %f,%t,%x,%y,(r%n)" => ="MOV %f,%t,%x,%y,%z" C="(r%n)" */
/* "MOV %f,%t,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 2A="$%c" */
/* "MOV %f,%t,%x,%y,(r%n)" => ="MOV %f,%t,%x,%y,%z" C="(r%n)" */
/* "MOV %f,%t,%x,%c,%z" => ="MOV %f,%t,%x,%y,%z" 2A="%c" */
/* "MOV %f,%t,%x,%y,(r%n)" => ="MOV %f,%t,%x,%y,%z" C="(r%n)" */
/* "MOV %f,%t,%x,%c(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 2A="%c(r%n)" */
/* "MOV %f,%t,%x,%y,(r%n)" => ="MOV %f,%t,%x,%y,%z" C="(r%n)" */
/* "MOV %f,%t,%x,%c(ap),%z" => ="MOV %f,%t,%x,%y,%z" 2A="%c(ap)" */
/* "MOV %f,%t,%x,%y,(r%n)" => ="MOV %f,%t,%x,%y,%z" C="(r%n)" */
/* "MOVA %f,%t,%x,{nox},%z" => ="MOVA %f,%t,%x,%y,%z" 2A=@TMP1524 */
/* "MOV %f,{t},%x,%y,(r%n)" => ="MOV %f,%t,%x,%y,%z" E=@TMP1529 C="(r%n)" */
/* "MOVA %f,?1,%x,{nox"[r%i]"},%z" => ="MOVA %f,%t,%x,%y,%z" 2E=E 2A=@TMP1535 */
/* "MOV %f,{t},%x,%y,(r%n)" => ="MOV %f,%t,%x,%y,%z" E=@TMP1540 C="(r%n)" */
/* "MOVA %f,?1,%x,%y,%z" => ="MOVA %f,%t,%x,%y,%z" 2E=E */
/* "MOV %f,f,%x,%y,(r%n)" => ="MOV %f,%t,%x,%y,%z" E="f" C="(r%n)" */
/* "MOVA %f,l,%x,%y,%z" => ="MOVA %f,%t,%x,%y,%z" 2E="l" */
/* "MOV %f,f,%x,%y,(r%n)" => ="MOV %f,%t,%x,%y,%z" E="f" C="(r%n)" */
/* "MOVA %f,l,%x,{nox"[r%i]"},%z" => ="MOVA %f,%t,%x,%y,%z" 2E="l" 2A=@TMP1568 */
/* "MOV %f,%t,%x,{"%c(ap)"|"%c(fp)"},%z" => ="MOV %f,%t,%x,%y,%z" A=@TMP1573 */
/* "MOV %f,%t,%x,r%c,r%c" => ="MOV %f,%t,%x,%y,%z" A="r%c" C="r%c" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" A="(r%n)" */
/* "MOVA %f,%t,%x,{nox},%z" => ="MOVA %f,%t,%x,%y,%z" 0A=@TMP1589 */
/* "MOV %f,{t},%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" E=@TMP1594 A="(r%n)" */
/* "MOVA %f,?1,%x,{nox"[r%i]"},%z" => ="MOVA %f,%t,%x,%y,%z" 0E=E 0A=@TMP1600 */
/* "MOV %f,{t},%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" E=@TMP1605 A="(r%n)" */
/* "MOVA %f,?1,%x,%y,%z" => ="MOVA %f,%t,%x,%y,%z" 0E=E */
/* "MOV %f,f,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" E="f" A="(r%n)" */
/* "MOVA %f,l,%x,%y,%z" => ="MOVA %f,%t,%x,%y,%z" 0E="l" */
/* "MOV %f,f,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" E="f" A="(r%n)" */
/* "MOVA %f,l,%x,{nox"[r%i]"},%z" => ="MOVA %f,%t,%x,%y,%z" 0E="l" 0A=@TMP1633 */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" A="(r%n)" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="(r%n)" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" A="(r%n)" */
/* "MOV %f,%t,%x,%c(fp),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(fp)" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" A="(r%n)" */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" 0A="r%n" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" A="(r%n)" */
/* "MOV %f,%t,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="$%c" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" A="(r%n)" */
/* "MOV %f,%t,%x,%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" A="(r%n)" */
/* "MOV %f,%t,%x,%c(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(r%n)" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" A="(r%n)" */
/* "MOV %f,%t,%x,%c(ap),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(ap)" */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" A="r%n" */
/* "MOV %f,%t,%x,{lit},%z" => ="MOV %f,%t,%x,%y,%z" 0A=@TMP1713 */
/* "MOV %f,l,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" E="l" A="r%n" */
/* "MOVA %f,{t},%x,%y,%z" => ="MOVA %f,%t,%x,%y,%z" 0E=@TMP1724 */
/* "MOVA %f,?1,%x,%y,%z" => ="MOVA %f,%t,%x,%y,%z" E=0E */
/* "MOV %f,{t},%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" E=@TMP1733 A="r%n" */
/* "MOV {f},?1,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 0D=@TMP1739 0E=E */
/* "MOV ?2,%t,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" D=0D */
/* "MOV %f,{t},%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" E=@TMP1748 A="r%n" */
/* "MOVZ {f},?1,%x,%y,%z" => ="MOVZ %f,%t,%x,%y,%z" 0D=@TMP1754 0E=E */
/* "MOVZ ?2,%t,%x,%y,%z" => ="MOVZ %f,%t,%x,%y,%z" D=0D */
/* "MOV %f,{t},%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" E=@TMP1763 A="r%n" */
/* "OP1 {f},?1,%x,%y,%z" => ="OP1 %f,%t,%x,%y,%z" 0D=@TMP1769 0E=E */
/* "OP1 ?2,%t,%x,%y,%z" => ="OP1 %f,%t,%x,%y,%z" D=0D */
/* "MOV %f,{t},%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" E=@TMP1778 A="r%n" */
/* "CVT {f},?1,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 0D=@TMP1784 0E=E */
/* "CVT ?2,%t,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" D=0D */
/* "MOV %f,{b|w},%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" E=@TMP1793 A="r%n" */
/* "MOV {f},l,%x,{noxrc},%z" => ="MOV %f,%t,%x,%y,%z" 0D=@TMP1799 0E="l" 0A=@TMP1801 */
/* "MOV ?2,%t,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" D=0D */
/* "MOV %f,{b|w},%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" E=@TMP1810 A="r%n" */
/* "MOVZ {f},l,%x,{noxrc},%z" => ="MOVZ %f,%t,%x,%y,%z" 0D=@TMP1816 0E="l" 0A=@TMP1818 */
/* "MOVZ ?2,%t,%x,%y,%z" => ="MOVZ %f,%t,%x,%y,%z" D=0D */
/* "MOV %f,{b|w},%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" E=@TMP1827 A="r%n" */
/* "OP1 {f},l,%x,{noxrc},%z" => ="OP1 %f,%t,%x,%y,%z" 0D=@TMP1833 0E="l" 0A=@TMP1835 */
/* "OP1 ?2,%t,%x,%y,%z" => ="OP1 %f,%t,%x,%y,%z" D=0D */
/* "MOV %f,{b|w},%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" E=@TMP1844 A="r%n" */
/* "CVT {f},l,%x,{noxrc},%z" => ="CVT %f,%t,%x,%y,%z" 0D=@TMP1850 0E="l" 0A=@TMP1852 */
/* "CVT ?2,%t,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" D=0D */
/* "MOV %f,{t},%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" E=@TMP1861 A="r%n" */
/* "ASH %f,?1,%x,%y,%z" => ="ASH %f,%t,%x,%y,%z" 0E=E */
/* "MOV %f,{t},%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" E=@TMP1871 A="r%n" */
/* "EXT %f,?1,%x,%y,%z" => ="EXT %f,%t,%x,%y,%z" 0E=E */
/* "MOV %f,{t},%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" E=@TMP1881 A="r%n" */
/* "OP2 {f},?1,%x,%y,%z" => ="OP2 %f,%t,%x,%y,%z" 0D=@TMP1887 0E=E */
/* "OP2 ?2,%t,%x,%y,%z" => ="OP2 %f,%t,%x,%y,%z" D=0D */
/* "MOV %f,{b|w},%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" E=@TMP1896 A="r%n" */
/* "OP2 {add|bic|bis|mul|sub|xor},l,{noxrc},{noxrc},%z" => ="OP2 %f,%t,%x,%y,%z" 0D=@TMP1902 0E="l" 0B=@TMP1904 0A=@TMP1905 */
/* "OP2 ?2,%t,%x,%y,%z" => ="OP2 %f,%t,%x,%y,%z" D=0D */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" A="r%n" */
/* "MOVZ %f,%t,%x,{noxrc},%z" => ="MOVZ %f,%t,%x,%y,%z" 0A=@TMP1919 */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" A="r%n" */
/* "CVT %f,%t,%x,{noxrc},%z" => ="CVT %f,%t,%x,%y,%z" 0A=@TMP1929 */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" A="r%n" */
/* "MOV %f,l,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" 0E="l" 0A="r%n" */
/* "MOVA %f,%t,%x,%c,r%c" => ="MOVA %f,%t,%x,%y,%z" A="%c" C="r%c" */
/* "MOV %f,l,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" E="l" A="$%c" */
/* "MOVA %f,%t,%x,0(r%n),%z" => ="MOVA %f,%t,%x,%y,%z" A="%c(r%n)" a="0" */
/* "MOVA %f,%t,%x,0(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="%c(r%n)[r%i]" a="0" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOVA %f,%t,%x,*%c,%z" => ="MOVA %f,%t,%x,%y,%z" 0A="*%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOVA %f,%t,%x,*%c(fp),%z" => ="MOVA %f,%t,%x,%y,%z" 0A="*%c(fp)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOVA %f,%t,%x,*%c(ap),%z" => ="MOVA %f,%t,%x,%y,%z" 0A="*%c(ap)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOVA %f,%t,%x,*%c(r%n),%z" => ="MOVA %f,%t,%x,%y,%z" 0A="*%c(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOVA %f,%t,%x,%c(ap),%z" => ="MOVA %f,%t,%x,%y,%z" 0A="%c(ap)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOVA %f,%t,%x,%c(fp),%z" => ="MOVA %f,%t,%x,%y,%z" 0A="%c(fp)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOVA %f,%t,%x,%c(r%n),%z" => ="MOVA %f,%t,%x,%y,%z" 0A="%c(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOVA %f,%t,%x,*(r%n),%z" => ="MOVA %f,%t,%x,%y,%z" 0A="*(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOVA %f,%t,%x,*$%c,%z" => ="MOVA %f,%t,%x,%y,%z" 0A="*$%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOVA %f,%t,%x,%c,%z" => ="MOVA %f,%t,%x,%y,%z" 0A="%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOVA %f,%t,%x,(r%n),%z" => ="MOVA %f,%t,%x,%y,%z" 0A="(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(fp),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(fp)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(fp),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(fp)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(fp),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(fp)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(fp),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(fp)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(fp),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(fp)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(fp),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(fp)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(fp),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(fp)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(fp),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(fp)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(fp),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(fp)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(fp),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(fp)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(fp),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(fp)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" 0A="r%n" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" 0A="r%n" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" 0A="r%n" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" 0A="r%n" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" 0A="r%n" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" 0A="r%n" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" 0A="r%n" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" 0A="r%n" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" 0A="r%n" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" 0A="r%n" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" 0A="r%n" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="$%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="$%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="$%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="$%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="$%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="$%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="$%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="$%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="$%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="$%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="$%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(r%n),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(r%n)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(ap),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(ap)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(ap),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(ap)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(ap),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(ap)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(ap),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(ap)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(ap),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(ap)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(ap),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(ap)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(ap),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(ap)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(ap),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(ap)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(ap),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(ap)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(ap),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(ap)" */
/* "MOVA %f,%t,%x,(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="(r%n)[r%i]" */
/* "MOV %f,%t,%x,%c(ap),%z" => ="MOV %f,%t,%x,%y,%z" 0A="%c(ap)" */
/* "MOVZ d,d,%x,%y,%z" => ="MOVZ %f,%t,%x,%y,%z" D="d" E="d" */
/* "MOVZ l,l,%x,%y,%z" => ="MOVZ %f,%t,%x,%y,%z" D="l" E="l" */
/* "MOVZ b,b,%x,%y,%z" => ="MOVZ %f,%t,%x,%y,%z" D="b" E="b" */
/* "MOVZ f,f,%x,%y,%z" => ="MOVZ %f,%t,%x,%y,%z" D="f" E="f" */
/* "MOVZ w,w,%x,%y,%z" => ="MOVZ %f,%t,%x,%y,%z" D="w" E="w" */
/* "MOVZ w,b,%x,%y,%z" => ="MOVZ %f,%t,%x,%y,%z" D="w" E="b" */
/* "MOVZ l,{b|w},%x,%y,%z" => ="MOVZ %f,%t,%x,%y,%z" D="l" E=@TMP2884 */
/* "MOVZ d,%t,%x,r%n,%z" => ="MOVZ %f,%t,%x,%y,%z" D="d" A="r%n" */
/* "MOV %f,d,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 0E="d" */
/* "MOVZ l,%t,%x,r%n,%z" => ="MOVZ %f,%t,%x,%y,%z" D="l" A="r%n" */
/* "MOV %f,l,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 0E="l" */
/* "MOVZ b,%t,%x,r%n,%z" => ="MOVZ %f,%t,%x,%y,%z" D="b" A="r%n" */
/* "MOV %f,b,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 0E="b" */
/* "MOVZ f,%t,%x,r%n,%z" => ="MOVZ %f,%t,%x,%y,%z" D="f" A="r%n" */
/* "MOV %f,f,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 0E="f" */
/* "MOVZ w,%t,%x,r%n,%z" => ="MOVZ %f,%t,%x,%y,%z" D="w" A="r%n" */
/* "MOV %f,w,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 0E="w" */
/* "MOVZ %f,{t},%x,r%n,%z" => ="MOVZ %f,%t,%x,%y,%z" E=@TMP2944 A="r%n" */
/* "MOV %f,?1,%x,{lit},%z" => ="MOV %f,%t,%x,%y,%z" 0E=E 0A=@TMP2950 */
/* "MOVZ %f,%t,%x,r%n,%z" => ="MOVZ %f,%t,%x,%y,%z" A="r%n" */
/* "MOVZ b,%t,%x,%y,%z" => ="MOVZ %f,%t,%x,%y,%z" 0D="b" */
/* "MOVZ b,%t,%x,%y,%z" => ="MOVZ %f,%t,%x,%y,%z" D="b" */
/* "MOVZ %f,%t,%x,r%n,%z" => ="MOVZ %f,%t,%x,%y,%z" A="r%n" */
/* "MOVZ w,l,%x,%y,%z" => ="MOVZ %f,%t,%x,%y,%z" 0D="w" 0E="l" */
/* "MOVZ w,%t,%x,%y,%z" => ="MOVZ %f,%t,%x,%y,%z" D="w" */
/* "OP1 %f,{t},%x,r%n,%z" => ="OP1 %f,%t,%x,%y,%z" E=@TMP2986 A="r%n" */
/* "MOV %f,?1,%x,{lit},%z" => ="MOV %f,%t,%x,%y,%z" 0E=E 0A=@TMP2992 */
/* "OP1 %f,{t},%x,r%n,%z" => ="OP1 %f,%t,%x,%y,%z" E=@TMP2997 A="r%n" */
/* "MOV %f,?1,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 0E=E */
/* "OP2 {div|mod|mul},%t,%x,%y,%z" => ="OP2 %f,%t,%x,%y,%z" D=@TMP3007 */
/* "OP2 add,l,r%n,r%n,r%c" => ="OP2 %f,%t,%x,%y,%z" D="add" E="l" B="r%n" A="r%n" C="r%c" */
/* "MOVA %f,%t,%x,%c[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" 0A="%c[r%i]" */
/* "MOVA %f,%t,%x,%c(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="%c(r%n)[r%i]" */
/* "OP2 add,l,r%n,r%n,r%c" => ="OP2 %f,%t,%x,%y,%z" D="add" E="l" B="r%n" A="r%n" C="r%c" */
/* "MOV %f,%t,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 0A="$%c" */
/* "MOVA %f,%t,%x,%c(r%n),%z" => ="MOVA %f,%t,%x,%y,%z" A="%c(r%n)" */
/* "OP2 add,l,r%n,r%n,r%c" => ="OP2 %f,%t,%x,%y,%z" D="add" E="l" B="r%n" A="r%n" C="r%c" */
/* "MOVA %f,%t,%x,%c,%z" => ="MOVA %f,%t,%x,%y,%z" 0A="%c" */
/* "MOVA %f,%t,%x,%c(r%n),%z" => ="MOVA %f,%t,%x,%y,%z" A="%c(r%n)" */
/* "OP2 add,l,r%n,r%n,r%c" => ="OP2 %f,%t,%x,%y,%z" D="add" E="l" B="r%n" A="r%n" C="r%c" */
/* "MOVA %f,%t,%x,%c[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" 1A="%c[r%i]" */
/* "MOVA %f,%t,%x,%c(r%n)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" A="%c(r%n)[r%i]" */
/* "OP2 add,l,r%n,r%n,r%c" => ="OP2 %f,%t,%x,%y,%z" D="add" E="l" B="r%n" A="r%n" C="r%c" */
/* "MOV %f,%t,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" 1A="$%c" */
/* "MOVA %f,%t,%x,%c(r%n),%z" => ="MOVA %f,%t,%x,%y,%z" A="%c(r%n)" */
/* "OP2 add,l,r%n,r%n,r%c" => ="OP2 %f,%t,%x,%y,%z" D="add" E="l" B="r%n" A="r%n" C="r%c" */
/* "MOVA %f,%t,%x,%c,%z" => ="MOVA %f,%t,%x,%y,%z" 1A="%c" */
/* "MOVA %f,%t,%x,%c(r%n),%z" => ="MOVA %f,%t,%x,%y,%z" A="%c(r%n)" */
/* "OP2 add,l,r%n,r%n,%z" => ="OP2 %f,%t,%x,%y,%z" D="add" E="l" B="r%n" A="r%n" */
/* "MOVA %f,%t,%x,%c(fp),%z" => ="MOVA %f,%t,%x,%y,%z" 1A="%c(fp)" */
/* "MOVA %f,b,%x,%c(fp)[r%i],%z" => ="MOVA %f,%t,%x,%y,%z" E="b" A="%c(fp)[r%i]" */
/* "OP2 %f,{t},r%n,%y,%z" => ="OP2 %f,%t,%x,%y,%z" E=@TMP3145 B="r%n" */
/* "MOV %f,?1,%x,{lit},%z" => ="MOV %f,%t,%x,%y,%z" 1E=E 1A=@TMP3151 */
/* "OP2 %f,{t},r%n,%y,%z" => ="OP2 %f,%t,%x,%y,%z" E=@TMP3156 B="r%n" */
/* "MOV %f,?1,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 1E=E */
/* "OP2 %f,b,r%n,%y,%z" => ="OP2 %f,%t,%x,%y,%z" E="b" B="r%n" */
/* "CVT b,l,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 1D="b" 1E="l" */
/* "OP2 %f,w,r%n,%y,%z" => ="OP2 %f,%t,%x,%y,%z" E="w" B="r%n" */
/* "CVT w,l,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 1D="w" 1E="l" */
/* "OP2 %f,{t},%x,r%n,%z" => ="OP2 %f,%t,%x,%y,%z" E=@TMP3190 A="r%n" */
/* "MOV %f,?1,%x,{lit},%z" => ="MOV %f,%t,%x,%y,%z" 0E=E 0A=@TMP3196 */
/* "OP2 %f,{t},%x,r%n,%z" => ="OP2 %f,%t,%x,%y,%z" E=@TMP3201 A="r%n" */
/* "MOV %f,?1,%x,%y,%z" => ="MOV %f,%t,%x,%y,%z" 0E=E */
/* "OP2 %f,b,%x,r%n,%z" => ="OP2 %f,%t,%x,%y,%z" E="b" A="r%n" */
/* "CVT b,l,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 0D="b" 0E="l" */
/* "OP2 %f,w,%x,r%n,%z" => ="OP2 %f,%t,%x,%y,%z" E="w" A="r%n" */
/* "CVT w,l,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 0D="w" 0E="l" */
/* "OP2 add,l,$4,r%n,r%c" => ="OP2 %f,%t,%x,%y,%z" D="add" E="l" B="$%c" b="4" A="r%n" C="r%c" */
/* "MOV %f,l,%x,r%c,%z" => ="MOV %f,%t,%x,%y,%z" 0E="l" 0A="r%c" */
/* "OP2 add,l,$2,r%n,r%c" => ="OP2 %f,%t,%x,%y,%z" D="add" E="l" B="$%c" b="2" A="r%n" C="r%c" */
/* "MOV %f,l,%x,r%c,%z" => ="MOV %f,%t,%x,%y,%z" 0E="l" 0A="r%c" */
/* "OP2 add,l,$1,r%n,r%c" => ="OP2 %f,%t,%x,%y,%z" D="add" E="l" B="$%c" b="1" A="r%n" C="r%c" */
/* "MOV %f,l,%x,r%c,%z" => ="MOV %f,%t,%x,%y,%z" 0E="l" 0A="r%c" */
/* "OP2K %f,{t},r%n,%y,%z" => ="OP2K %f,%t,%x,%y,%z" E=@TMP3280 B="r%n" */
/* "MOV %f,?1,%x,{lit},%z" => ="MOV %f,%t,%x,%y,%z" 1E=E 1A=@TMP3286 */
/* "CMP %f,b,$%c,%y,%z" => ="CMP %f,%t,%x,%y,%z" E="b" B="$%c" */
/* "CMP %f,w,$%c,%y,%z" => ="CMP %f,%t,%x,%y,%z" E="w" B="$%c" */
/* "OP2 %f,b,$%c,%y,%z" => ="OP2 %f,%t,%x,%y,%z" E="b" B="$%c" */
/* "OP2 %f,w,$%c,%y,%z" => ="OP2 %f,%t,%x,%y,%z" E="w" B="$%c" */
/* "MOV %f,b,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" E="b" A="$%c" */
/* "MOV %f,w,%x,$%c,%z" => ="MOV %f,%t,%x,%y,%z" E="w" A="$%c" */
/* "MOVZ %f,b,%x,$%c,%z" => ="MOVZ %f,%t,%x,%y,%z" E="b" A="$%c" */
/* "MOVZ %f,w,%x,$%c,%z" => ="MOVZ %f,%t,%x,%y,%z" E="w" A="$%c" */
/* "CMP %f,b,%x,$%c,%z" => ="CMP %f,%t,%x,%y,%z" E="b" A="$%c" */
/* "CMP %f,w,%x,$%c,%z" => ="CMP %f,%t,%x,%y,%z" E="w" A="$%c" */
/* "OP1 %f,b,%x,$%c,%z" => ="OP1 %f,%t,%x,%y,%z" E="b" A="$%c" */
/* "OP1 %f,w,%x,$%c,%z" => ="OP1 %f,%t,%x,%y,%z" E="w" A="$%c" */
/* "CVT %f,b,%x,$%c,%z" => ="CVT %f,%t,%x,%y,%z" E="b" A="$%c" */
/* "CVT %f,w,%x,$%c,%z" => ="CVT %f,%t,%x,%y,%z" E="w" A="$%c" */
/* "OP2 %f,b,%x,$%c,%z" => ="OP2 %f,%t,%x,%y,%z" E="b" A="$%c" */
/* "OP2 %f,w,%x,$%c,%z" => ="OP2 %f,%t,%x,%y,%z" E="w" A="$%c" */
/* "CMP %f,l,$%c,r%n,%z" => ="CMP %f,%t,%x,%y,%z" E="l" B="$%c" A="r%n" */
/* "CVT b,l,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 0D="b" 0E="l" */
/* "CMP %f,l,$%c,r%n,%z" => ="CMP %f,%t,%x,%y,%z" E="l" B="$%c" A="r%n" */
/* "CVT w,l,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 0D="w" 0E="l" */
/* "CMP %f,l,r%n,r%n,%z" => ="CMP %f,%t,%x,%y,%z" E="l" B="r%n" A="r%n" */
/* "CVT b,l,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 0D="b" 0E="l" */
/* "CVT b,l,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 1D="b" 1E="l" */
/* "CMP %f,l,r%n,r%n,%z" => ="CMP %f,%t,%x,%y,%z" E="l" B="r%n" A="r%n" */
/* "CVT w,l,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 0D="w" 0E="l" */
/* "CVT w,l,%x,%y,%z" => ="CVT %f,%t,%x,%y,%z" 1D="w" 1E="l" */
/* "CMP %f,%t,$0,r%n,%z" => ="CMP %f,%t,%x,%y,%z" B="$%c" b="0" A="r%n" */
/* "MOV %f,%t,%x,r%n,%z" => ="MOV %f,%t,%x,%y,%z" $A="r%n" */
/* "CMP %f,%t,$0,%y,%z" => ="CMP %f,%t,%x,%y,%z" B="$%c" b="0" */
/* "EXT %f,%t,$%c,%y,%z" => ="EXT %f,%t,%x,%y,%z" B="$%c" */
/* "extzv $%c,$32-%c,%y,%z" => ="extzv %x,%y,%z" B="$%c,$32-%c" */
/* "MOV %f,%t,%x,$0,%z" => ="MOV %f,%t,%x,%y,%z" A="$%c" a="0" */
/* "MOV %f,l,%x,%y,-(sp)" => ="MOV %f,%t,%x,%y,%z" E="l" C="-(sp)" */
/* "MOVA %f,%t,%x,%y,-(sp)" => ="MOVA %f,%t,%x,%y,%z" C="-(sp)" */
/* "OP2 add,l,$%c,r%n,-(sp)" => ="OP2 %f,%t,%x,%y,%z" D="add" E="l" B="$%c" A="r%n" C="-(sp)" */
/* "pusha%t %c(r%n)" => ="pusha%t %y" A="%c(r%n)" */
/* "OP2 mod,%t,%x,%y,%z" => ="OP2 %f,%t,%x,%y,%z" D="mod" */
/* "OP2 {add|sub},l,$%c,r%n,%z" => ="OP2 %f,%t,%x,%y,%z" D=@TMP3513 E="l" B="$%c" A="r%n" */
/* "movl %y,%z" => ="mov%t %y,%z" $E="l" */
/* "OP2 %f,%t,%x,r%n,r%c" => ="OP2 %f,%t,%x,%y,%z" A="r%n" C="r%c" */
/* "OP2 {add|bis|xor|mul},%t,%x,%y,%z" => ="OP2 %f,%t,%x,%y,%z" D=@TMP3532 */
/* "OP2 {add|bis|xor|mul},%t,r%n,%y,r%c" => ="OP2 %f,%t,%x,%y,%z" D=@TMP3537 B="r%n" C="r%c" */
/* "add%t2 $-1,%z" => ="%f%t2 %x,%z" D="add" B="$%c" b="-1" */
/* "sub%t2 $1,%z" => ="%f%t2 %x,%z" D="sub" B="$%c" b="1" */
/* "sub%t2 $1,%z" => ="%f%t2 %x,%z" D="sub" B="$%c" b="1" */
/* "dec%t %z" => ="%f%t %z" D="dec" */
/* "add%t2 $1,%z" => ="%f%t2 %x,%z" D="add" B="$%c" b="1" */
/* "inc%t %z" => ="%f%t %z" D="inc" */
/* "movx %y,%z" => ="mov%t %y,%z" E="x" */
/* "movx %y,%z" => ="mov%t %y,%z" E="x" */
/* "movx %y,-(sp)" => ="mov%t %y,%z" E="x" C="-(sp)" */
/* "subl2 $(%c+3)&-4,sp\nmovc3 $%c,%y,(sp)" => ="subl2 %x,sp\nmovc3 %z,%y,(sp)" B="$(%c+3)&-4" C="$%c" */
/* "movx %y,%z" => ="mov%t %y,%z" E="x" */
/* "movc3 $%c,%y,%z" => ="movc3 %x,%y,%z" B="$%c" */
/* "mov%t r%n,r%c" => ="mov%t %y,%z" A="r%n" C="r%c" */
/* "tst%t r%n\nj%f %z" => ="tst%t %y\nj%f %z" A="r%n" */
#include "pseudos.c"

static char *shortopcodes[] = {
/* 608 */  "subl3 %x,$32,r0\nextzv %x,r0,%y,%z",
/* 609 */  "calls %x,%y",
/* 610 */  "pushl %x\npushl %y\ncalls $2,%f",
/* 611 */  "movc3 %x,%y,%z",
/* 612 */  "subl2 %x,sp\nmovc3 %z,%y,(sp)",
/* 613 */  "%f%t %y,%z",
/* 614 */  "%f%t %z",
/* 615 */  "%f%t2 %x,%z",
/* 616 */  "%f%t3 %x,%y,%z",
/* 617 */  "ash%t %x,%y,%z",
/* 618 */  "clr%t %z",
/* 619 */  "cmp%t %y,%x\nj%f %z",
/* 620 */  "cvt%f%t %y,%z",
/* 621 */  "div%t3 %x,%y,-(sp)\nmul%t2 %x,(sp)\nsub%t3 (sp)+,%y,%z",
/* 622 */  "extzv %x,%y,%z",
/* 623 */  "mov%t %y,%z",
/* 624 */  "mova%t %y,%z",
/* 625 */  "movz%f%t %y,%z",
/* 626 */  "pusha%t %y",
/* 627 */  "pushl %y",
/* 628 */  "tst%t %y\nj%f %z",
/* 629 */  "bit%t %y,%x\nj%f %z",
/* 630 */  "ASH %f,%t,%x,%y,%z",
/* 631 */  "BIT %f,%t,%x,%y,%z",
/* 632 */  "CALL %f,%t,%x,%y,%z",
/* 633 */  "CMP %f,%t,%x,%y,%z",
/* 634 */  "CVT %f,%t,%x,%y,%z",
/* 635 */  "EXT %f,%t,%x,%y,%z",
/* 636 */  "MOV %f,%t,%x,%y,%z",
/* 637 */  "MOVA %f,%t,%x,%y,%z",
/* 638 */  "MOVZ %f,%t,%x,%y,%z",
/* 639 */  "OP1 %f,%t,%x,%y,%z",
/* 640 */  "OP2 %f,%t,%x,%y,%z",
/* 641 */  "OP2K %f,%t,%x,%y,%z",
/* 642 */  "j%f %z",
/* 643 */  "jbr %z",
/* 644 */  "jmp %z",
/* 645 */  "ret",
/* 646 */  "# label",
/* 647 */  "# nop",
/* 648 */  "%z:",
};

static char *asmstr0[] = {
	"(DUMMY)",
	"*%c(ap)",
	"*%c(fp)",
	"*%c(r%n)",
	"*(r%n)",
	"*%c",
	"%c(ap)",
	"%c(fp)",
	"%c(r%n)",
	"(r%n)",
	"%c",
	"*$%c",
	"r%n",
	"r%c",
	"$%c",
	"*%c(ap)[r%i]",
	"*%c(fp)[r%i]",
	"*%c(r%n)[r%i]",
	"*(r%n)[r%i]",
	"*%c[r%i]",
	"%c(ap)[r%i]",
	"%c(fp)[r%i]",
	"%c(r%n)[r%i]",
	"(r%n)[r%i]",
	"%c[r%i]",
	"*$%c[r%i]",
	"(r%c)+",
	"(sp)",
	"-(sp)",
	"$%c/4",
	"$%c,$32-%c",
	"$(%c+3)&-4",
};

static char *asmstr3[] = {
	"(DUMMY)",
	"eqlu",
	"eql",
	"gequ",
	"geq",
	"gtru",
	"gtr",
	"lequ",
	"leq",
	"lssu",
	"lss",
	"nequ",
	"neq",
	"b",
	"w",
	"l",
	"f",
	"d",
	"add",
	"bic",
	"bis",
	"div",
	"mod",
	"mul",
	"sub",
	"xor",
	"dec",
	"inc",
	"mcom",
	"mneg",
	"udiv",
	"urem",
};

static char *asmstr4[] = {
	"(DUMMY)",
	"b",
	"w",
	"l",
	"f",
	"d",
	"x",
	"q",
};

static struct symbol sym[] = {
{{"(DUMMY)"}},
{{"0"}},
{{"1"}},
{{"2"}},
{{"3"}},
{{"4"}},
{{"8"}},
{{"-1"}},
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
	sym[1].x.name = string("0");
	sym[2].x.name = string("1");
	sym[3].x.name = string("2");
	sym[4].x.name = string("3");
	sym[5].x.name = string("4");
	sym[6].x.name = string("8");
	sym[7].x.name = string("-1");
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
	case 1:   /* *%c(ap) */
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = 'a';
		*rbp++ = 'p';
		*rbp++ = ')';
		return rbp;
	case 2:   /* *%c(fp) */
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = 'f';
		*rbp++ = 'p';
		*rbp++ = ')';
		return rbp;
	case 3:   /* *%c(r%n) */
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = 'r';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
		return rbp;
	case 4:   /* *(r%n) */
		*rbp++ = '*';
		*rbp++ = '(';
		*rbp++ = 'r';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
		return rbp;
	case 5:   /* *%c */
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 6:   /* %c(ap) */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = 'a';
		*rbp++ = 'p';
		*rbp++ = ')';
		return rbp;
	case 7:   /* %c(fp) */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = 'f';
		*rbp++ = 'p';
		*rbp++ = ')';
		return rbp;
	case 8:   /* %c(r%n) */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = 'r';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
		return rbp;
	case 9:   /* (r%n) */
		*rbp++ = '(';
		*rbp++ = 'r';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
		return rbp;
	case 10:   /* %c */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 11:   /* *$%c */
		*rbp++ = '*';
		*rbp++ = '$';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 12:   /* r%n */
		*rbp++ = 'r';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 13:   /* r%c */
		*rbp++ = 'r';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 14:   /* $%c */
		*rbp++ = '$';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 15:   /* *%c(ap)[r%i] */
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = 'a';
		*rbp++ = 'p';
		*rbp++ = ')';
		*rbp++ = '[';
		*rbp++ = 'r';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ']';
		return rbp;
	case 16:   /* *%c(fp)[r%i] */
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = 'f';
		*rbp++ = 'p';
		*rbp++ = ')';
		*rbp++ = '[';
		*rbp++ = 'r';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ']';
		return rbp;
	case 17:   /* *%c(r%n)[r%i] */
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = 'r';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
		*rbp++ = '[';
		*rbp++ = 'r';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ']';
		return rbp;
	case 18:   /* *(r%n)[r%i] */
		*rbp++ = '*';
		*rbp++ = '(';
		*rbp++ = 'r';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
		*rbp++ = '[';
		*rbp++ = 'r';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ']';
		return rbp;
	case 19:   /* *%c[r%i] */
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '[';
		*rbp++ = 'r';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ']';
		return rbp;
	case 20:   /* %c(ap)[r%i] */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = 'a';
		*rbp++ = 'p';
		*rbp++ = ')';
		*rbp++ = '[';
		*rbp++ = 'r';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ']';
		return rbp;
	case 21:   /* %c(fp)[r%i] */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = 'f';
		*rbp++ = 'p';
		*rbp++ = ')';
		*rbp++ = '[';
		*rbp++ = 'r';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ']';
		return rbp;
	case 22:   /* %c(r%n)[r%i] */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = 'r';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
		*rbp++ = '[';
		*rbp++ = 'r';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ']';
		return rbp;
	case 23:   /* (r%n)[r%i] */
		*rbp++ = '(';
		*rbp++ = 'r';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
		*rbp++ = '[';
		*rbp++ = 'r';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ']';
		return rbp;
	case 24:   /* %c[r%i] */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '[';
		*rbp++ = 'r';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ']';
		return rbp;
	case 25:   /* *$%c[r%i] */
		*rbp++ = '*';
		*rbp++ = '$';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '[';
		*rbp++ = 'r';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ']';
		return rbp;
	case 26:   /* (r%c)+ */
		*rbp++ = '(';
		*rbp++ = 'r';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
		*rbp++ = '+';
		return rbp;
	case 27:   /* (sp) */
		*rbp++ = '(';
		*rbp++ = 's';
		*rbp++ = 'p';
		*rbp++ = ')';
		return rbp;
	case 28:   /* -(sp) */
		*rbp++ = '-';
		*rbp++ = '(';
		*rbp++ = 's';
		*rbp++ = 'p';
		*rbp++ = ')';
		return rbp;
	case 29:   /* $%c/4 */
		*rbp++ = '$';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '/';
		*rbp++ = '4';
		return rbp;
	case 30:   /* $%c,$32-%c */
		*rbp++ = '$';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ',';
		*rbp++ = '$';
		*rbp++ = '3';
		*rbp++ = '2';
		*rbp++ = '-';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 31:   /* $(%c+3)&-4 */
		*rbp++ = '$';
		*rbp++ = '(';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '3';
		*rbp++ = ')';
		*rbp++ = '&';
		*rbp++ = '-';
		*rbp++ = '4';
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
		case 608: L608:   /* subl3 %x,$32,r0\nextzv %x,r0,%y,%z */
			rbp = bp;
			*rbp++ = 's';
			*rbp++ = 'u';
			*rbp++ = 'b';
			*rbp++ = 'l';
			*rbp++ = '3';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			*rbp++ = '$';
			*rbp++ = '3';
			*rbp++ = '2';
			*rbp++ = ',';
			*rbp++ = 'r';
			*rbp++ = '0';
			bp = rbp; outs("\n"); rbp = bp;
			*rbp++ = 'e';
			*rbp++ = 'x';
			*rbp++ = 't';
			*rbp++ = 'z';
			*rbp++ = 'v';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			*rbp++ = 'r';
			*rbp++ = '0';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 609: L609:   /* calls %x,%y */
			rbp = bp;
			*rbp++ = 'c';
			*rbp++ = 'a';
			*rbp++ = 'l';
			*rbp++ = 'l';
			*rbp++ = 's';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 610: L610:   /* pushl %x\npushl %y\ncalls $2,%f */
			rbp = bp;
			*rbp++ = 'p';
			*rbp++ = 'u';
			*rbp++ = 's';
			*rbp++ = 'h';
			*rbp++ = 'l';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			bp = rbp; outs("\n"); rbp = bp;
			*rbp++ = 'p';
			*rbp++ = 'u';
			*rbp++ = 's';
			*rbp++ = 'h';
			*rbp++ = 'l';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			bp = rbp; outs("\n"); rbp = bp;
			*rbp++ = 'c';
			*rbp++ = 'a';
			*rbp++ = 'l';
			*rbp++ = 'l';
			*rbp++ = 's';
			*rbp++ = ' ';
			*rbp++ = '$';
			*rbp++ = '2';
			*rbp++ = ',';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			bp = rbp;
			outs("\n");
			break;
		case 611: L611:   /* movc3 %x,%y,%z */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'c';
			*rbp++ = '3';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 612: L612:   /* subl2 %x,sp\nmovc3 %z,%y,(sp) */
			rbp = bp;
			*rbp++ = 's';
			*rbp++ = 'u';
			*rbp++ = 'b';
			*rbp++ = 'l';
			*rbp++ = '2';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			*rbp++ = 's';
			*rbp++ = 'p';
			bp = rbp; outs("\n"); rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'c';
			*rbp++ = '3';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = '(';
			*rbp++ = 's';
			*rbp++ = 'p';
			*rbp++ = ')';
			bp = rbp;
			outs("\n");
			break;
		case 613: L613:   /* %f%t %y,%z */
			rbp = bp;
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
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
		case 614: L614:   /* %f%t %z */
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
		case 615: L615:   /* %f%t2 %x,%z */
			rbp = bp;
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = '2';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 616: L616:   /* %f%t3 %x,%y,%z */
			rbp = bp;
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = '3';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 617: L617:   /* ash%t %x,%y,%z */
			rbp = bp;
			*rbp++ = 'a';
			*rbp++ = 's';
			*rbp++ = 'h';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 618: L618:   /* clr%t %z */
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
		case 619: L619:   /* cmp%t %y,%x\nj%f %z */
			rbp = bp;
			*rbp++ = 'c';
			*rbp++ = 'm';
			*rbp++ = 'p';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			bp = rbp; outs("\n"); rbp = bp;
			*rbp++ = 'j';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 620: L620:   /* cvt%f%t %y,%z */
			rbp = bp;
			*rbp++ = 'c';
			*rbp++ = 'v';
			*rbp++ = 't';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
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
		case 621: L621:   /* div%t3 %x,%y,-(sp)\nmul%t2 %x,(sp)\nsub%t3 (sp)+,%y,%z */
			rbp = bp;
			*rbp++ = 'd';
			*rbp++ = 'i';
			*rbp++ = 'v';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = '3';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = '-';
			*rbp++ = '(';
			*rbp++ = 's';
			*rbp++ = 'p';
			*rbp++ = ')';
			bp = rbp; outs("\n"); rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'u';
			*rbp++ = 'l';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = '2';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			*rbp++ = '(';
			*rbp++ = 's';
			*rbp++ = 'p';
			*rbp++ = ')';
			bp = rbp; outs("\n"); rbp = bp;
			*rbp++ = 's';
			*rbp++ = 'u';
			*rbp++ = 'b';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = '3';
			*rbp++ = ' ';
			*rbp++ = '(';
			*rbp++ = 's';
			*rbp++ = 'p';
			*rbp++ = ')';
			*rbp++ = '+';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 622: L622:   /* extzv %x,%y,%z */
			rbp = bp;
			*rbp++ = 'e';
			*rbp++ = 'x';
			*rbp++ = 't';
			*rbp++ = 'z';
			*rbp++ = 'v';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 623: L623:   /* mov%t %y,%z */
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
		case 624: L624:   /* mova%t %y,%z */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'a';
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
		case 625: L625:   /* movz%f%t %y,%z */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'z';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
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
		case 626: L626:   /* pusha%t %y */
			rbp = bp;
			*rbp++ = 'p';
			*rbp++ = 'u';
			*rbp++ = 's';
			*rbp++ = 'h';
			*rbp++ = 'a';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 627: L627:   /* pushl %y */
			rbp = bp;
			*rbp++ = 'p';
			*rbp++ = 'u';
			*rbp++ = 's';
			*rbp++ = 'h';
			*rbp++ = 'l';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 628: L628:   /* tst%t %y\nj%f %z */
			rbp = bp;
			*rbp++ = 't';
			*rbp++ = 's';
			*rbp++ = 't';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			bp = rbp; outs("\n"); rbp = bp;
			*rbp++ = 'j';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 629: L629:   /* bit%t %y,%x\nj%f %z */
			rbp = bp;
			*rbp++ = 'b';
			*rbp++ = 'i';
			*rbp++ = 't';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			bp = rbp; outs("\n"); rbp = bp;
			*rbp++ = 'j';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 630: L630:   /* ASH %f,%t,%x,%y,%z */
			rbp = bp;
			*rbp++ = 'A';
			*rbp++ = 'S';
			*rbp++ = 'H';
			*rbp++ = ' ';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 631: L631:   /* BIT %f,%t,%x,%y,%z */
			rbp = bp;
			*rbp++ = 'B';
			*rbp++ = 'I';
			*rbp++ = 'T';
			*rbp++ = ' ';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 632: L632:   /* CALL %f,%t,%x,%y,%z */
			rbp = bp;
			*rbp++ = 'C';
			*rbp++ = 'A';
			*rbp++ = 'L';
			*rbp++ = 'L';
			*rbp++ = ' ';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 633: L633:   /* CMP %f,%t,%x,%y,%z */
			rbp = bp;
			*rbp++ = 'C';
			*rbp++ = 'M';
			*rbp++ = 'P';
			*rbp++ = ' ';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 634: L634:   /* CVT %f,%t,%x,%y,%z */
			rbp = bp;
			*rbp++ = 'C';
			*rbp++ = 'V';
			*rbp++ = 'T';
			*rbp++ = ' ';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 635: L635:   /* EXT %f,%t,%x,%y,%z */
			rbp = bp;
			*rbp++ = 'E';
			*rbp++ = 'X';
			*rbp++ = 'T';
			*rbp++ = ' ';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 636: L636:   /* MOV %f,%t,%x,%y,%z */
			rbp = bp;
			*rbp++ = 'M';
			*rbp++ = 'O';
			*rbp++ = 'V';
			*rbp++ = ' ';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 637: L637:   /* MOVA %f,%t,%x,%y,%z */
			rbp = bp;
			*rbp++ = 'M';
			*rbp++ = 'O';
			*rbp++ = 'V';
			*rbp++ = 'A';
			*rbp++ = ' ';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 638: L638:   /* MOVZ %f,%t,%x,%y,%z */
			rbp = bp;
			*rbp++ = 'M';
			*rbp++ = 'O';
			*rbp++ = 'V';
			*rbp++ = 'Z';
			*rbp++ = ' ';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 639: L639:   /* OP1 %f,%t,%x,%y,%z */
			rbp = bp;
			*rbp++ = 'O';
			*rbp++ = 'P';
			*rbp++ = '1';
			*rbp++ = ' ';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 640: L640:   /* OP2 %f,%t,%x,%y,%z */
			rbp = bp;
			*rbp++ = 'O';
			*rbp++ = 'P';
			*rbp++ = '2';
			*rbp++ = ' ';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 641: L641:   /* OP2K %f,%t,%x,%y,%z */
			rbp = bp;
			*rbp++ = 'O';
			*rbp++ = 'P';
			*rbp++ = '2';
			*rbp++ = 'K';
			*rbp++ = ' ';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 642: L642:   /* j%f %z */
			rbp = bp;
			*rbp++ = 'j';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 643: L643:   /* jbr %z */
			rbp = bp;
			*rbp++ = 'j';
			*rbp++ = 'b';
			*rbp++ = 'r';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 644: L644:   /* jmp %z */
			rbp = bp;
			*rbp++ = 'j';
			*rbp++ = 'm';
			*rbp++ = 'p';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 645: L645:   /* ret */
			rbp = bp;
			*rbp++ = 'r';
			*rbp++ = 'e';
			*rbp++ = 't';
			bp = rbp;
			outs("\n");
			break;
		case 646: L646:   /* # label */
			(*IR->x.emitspecial)(a);
			break;
		case 647: L647:   /* # nop */
			(*IR->x.emitspecial)(a);
			break;
		case 648: L648:   /* %z: */
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
	case 608: L608:   /* subl3 %x,$32,r0\nextzv %x,r0,%y,%z */
		dumptree(a, 608);
		spill(0x01, 0, a);  
		a->op = 608;
		break;
	case 609: L609:   /* calls %x,%y */
		dumptree(a, 609);
		spill(0x3e, 0, a);  
		a->op = 609;
		break;
	case 610: L610:   /* pushl %x\npushl %y\ncalls $2,%f */
		dumptree(a, 610);
		spill(0x3e, 0, a);  
		a->op = 610;
		break;
	case 611: L611:   /* movc3 %x,%y,%z */
		dumptree(a, 611);
		spill(0x3f, 0, a);  
		a->op = 611;
		break;
	case 612: L612:   /* subl2 %x,sp\nmovc3 %z,%y,(sp) */
		dumptree(a, 612);
		spill(0x3f, 0, a);  
		a->op = 612;
		break;
	case 613: L613:   /* %f%t %y,%z */
		dumptree(a, 613);
		a->op = 613;
		break;
	case 614: L614:   /* %f%t %z */
		dumptree(a, 614);
		a->op = 614;
		break;
	case 615: L615:   /* %f%t2 %x,%z */
		dumptree(a, 615);
		switch (a->x.ints[3]) {
		case 18:   /* add */
			if (
			a->x.ints[1] == 14  /* $%c */
			&& a->syms[1]->x.name == sym[7].x.name  /* -1 */
			) {
				a->syms[1] = &sym[2];  /* 1 */
				a->x.ints[1] = 14;  /* $%c */
				a->x.ints[3] = 24;  /* sub */
				goto L615;   /* %f%t2 %x,%z */
			}
			if (
			a->x.ints[1] == 14  /* $%c */
			&& a->syms[1]->x.name == sym[2].x.name  /* 1 */
			) {
				a->x.ints[3] = 27;  /* inc */
				goto L614;   /* %f%t %z */
			}
			break;
		case 24:   /* sub */
			if (
			a->x.ints[1] == 14  /* $%c */
			&& a->syms[1]->x.name == sym[2].x.name  /* 1 */
			) {
				a->x.ints[3] = 26;  /* dec */
				goto L614;   /* %f%t %z */
			}
			break;
		}
		a->op = 615;
		break;
	case 616: L616:   /* %f%t3 %x,%y,%z */
		dumptree(a, 616);
		a->op = 616;
		break;
	case 617: L617:   /* ash%t %x,%y,%z */
		dumptree(a, 617);
		a->op = 617;
		break;
	case 618: L618:   /* clr%t %z */
		dumptree(a, 618);
		a->op = 618;
		break;
	case 619: L619:   /* cmp%t %y,%x\nj%f %z */
		dumptree(a, 619);
		a->op = 619;
		break;
	case 620: L620:   /* cvt%f%t %y,%z */
		dumptree(a, 620);
		a->op = 620;
		break;
	case 621: L621:   /* div%t3 %x,%y,-(sp)\nmul%t2 %x,(sp)\nsub%t3 (sp)+,%y,%z */
		dumptree(a, 621);
		a->op = 621;
		break;
	case 622: L622:   /* extzv %x,%y,%z */
		dumptree(a, 622);
		a->op = 622;
		break;
	case 623: L623:   /* mov%t %y,%z */
		dumptree(a, 623);
		if (
		a->x.ints[4] == 6  /* x */
		) {
			if (
			a->syms[1]->x.name == sym[5].x.name  /* 4 */
			) {
				a->x.ints[4] = 3;  /* l */
				goto L623;   /* mov%t %y,%z */
			}
			if (
			a->syms[1]->x.name == sym[6].x.name  /* 8 */
			) {
				a->x.ints[4] = 7;  /* q */
				goto L623;   /* mov%t %y,%z */
			}
			if (
			a->x.ints[2] == 28  /* -(sp) */
			) {
				a->x.ints[2] = 14;  /* $%c */
				a->x.ints[1] = 31;  /* $(%c+3)&-4 */
				a->syms[2] = a->syms[1];
				goto L612;   /* subl2 %x,sp\nmovc3 %z,%y,(sp) */
			}
			a->x.ints[1] = 14;  /* $%c */
			goto L611;   /* movc3 %x,%y,%z */
		}
		if (
		a->x.ints[0] == 12  /* r%n */
		&& a->x.ints[2] == 13  /* r%c */
		&& a->kids[0]->syms[2] == a->syms[2]
		) {
			goto L647;   /* # nop */
		}
		a->op = 623;
		break;
	case 624: L624:   /* mova%t %y,%z */
		dumptree(a, 624);
		a->op = 624;
		break;
	case 625: L625:   /* movz%f%t %y,%z */
		dumptree(a, 625);
		a->op = 625;
		break;
	case 626: L626:   /* pusha%t %y */
		dumptree(a, 626);
		a->op = 626;
		break;
	case 627: L627:   /* pushl %y */
		dumptree(a, 627);
		a->op = 627;
		break;
	case 628: L628:   /* tst%t %y\nj%f %z */
		dumptree(a, 628);
		if (
		a->x.ints[0] == 12  /* r%n */
		&& last && "\1\0\0\0\0\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[last->op-608]  /* <SetsCC> */
		&& last && last == a->kids[0]
		) {
			goto L642;   /* j%f %z */
		}
		a->op = 628;
		break;
	case 629: L629:   /* bit%t %y,%x\nj%f %z */
		dumptree(a, 629);
		a->op = 629;
		break;
	case 630: L630:   /* ASH %f,%t,%x,%y,%z */
		dumptree(a, 630);
		goto L617;   /* ash%t %x,%y,%z */
	case 631: L631:   /* BIT %f,%t,%x,%y,%z */
		dumptree(a, 631);
		goto L629;   /* bit%t %y,%x\nj%f %z */
	case 632: L632:   /* CALL %f,%t,%x,%y,%z */
		dumptree(a, 632);
		goto L609;   /* calls %x,%y */
	case 633: L633:   /* CMP %f,%t,%x,%y,%z */
		dumptree(a, 633);
		if (
		a->x.ints[1] == 14  /* $%c */
		&& a->syms[1]->x.name == sym[1].x.name  /* 0 */
		) {
			goto L628;   /* tst%t %y\nj%f %z */
		}
		goto L619;   /* cmp%t %y,%x\nj%f %z */
	case 634: L634:   /* CVT %f,%t,%x,%y,%z */
		dumptree(a, 634);
		goto L620;   /* cvt%f%t %y,%z */
	case 635: L635:   /* EXT %f,%t,%x,%y,%z */
		dumptree(a, 635);
		if (
		a->x.ints[1] == 14  /* $%c */
		) {
			a->x.ints[1] = 30;  /* $%c,$32-%c */
			goto L622;   /* extzv %x,%y,%z */
		}
		goto L608;   /* subl3 %x,$32,r0\nextzv %x,r0,%y,%z */
	case 636: L636:   /* MOV %f,%t,%x,%y,%z */
		dumptree(a, 636);
		if (
		a->x.ints[0] == 14  /* $%c */
		&& a->syms[0]->x.name == sym[1].x.name  /* 0 */
		) {
			goto L618;   /* clr%t %z */
		}
		if (
		a->x.ints[4] == 3  /* l */
		&& a->x.ints[2] == 28  /* -(sp) */
		) {
			goto L627;   /* pushl %y */
		}
		goto L623;   /* mov%t %y,%z */
	case 637: L637:   /* MOVA %f,%t,%x,%y,%z */
		dumptree(a, 637);
		if (
		a->x.ints[2] == 28  /* -(sp) */
		) {
			goto L626;   /* pusha%t %y */
		}
		goto L624;   /* mova%t %y,%z */
	case 638: L638:   /* MOVZ %f,%t,%x,%y,%z */
		dumptree(a, 638);
		goto L625;   /* movz%f%t %y,%z */
	case 639: L639:   /* OP1 %f,%t,%x,%y,%z */
		dumptree(a, 639);
		goto L613;   /* %f%t %y,%z */
	case 640: L640:   /* OP2 %f,%t,%x,%y,%z */
		dumptree(a, 640);
		switch (a->x.ints[3]) {
		case 18:   /* add */
			if (
			a->x.ints[4] == 3  /* l */
			&& a->x.ints[1] == 14  /* $%c */
			&& a->x.ints[0] == 12  /* r%n */
			&& a->x.ints[2] == 28  /* -(sp) */
			) {
				a->x.ints[0] = 8;  /* %c(r%n) */
				a->syms[0] = a->syms[1];
				goto L626;   /* pusha%t %y */
			}
			break;
		case 22:   /* mod */
			goto L621;   /* div%t3 %x,%y,-(sp)\nmul%t2 %x,(sp)\nsub%t3 (sp)+,%y,%z */
		}
		if (
		"\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\1\0\0\0\0\0\0\0"[a->x.ints[3]]  /* <TMP3513> */
		&& a->x.ints[4] == 3  /* l */
		&& a->x.ints[1] == 14  /* $%c */
		&& a->x.ints[0] == 12  /* r%n */
		&& last && last->op == 623  /* mov%t %y,%z */
		&& last && last->x.ints[4] == 3  /* l */
		&& last && a->kids[0]->syms[2] == last->syms[2]
		&& last && a->x.ints[2] == last->x.ints[0]
		&& last && a->kids[2] == last->kids[0]
		&& last && a->kids[5] == last->kids[3]
		&& last && a->syms[2] == last->syms[0]
		) {
			a->syms[0] = last->syms[0];
			a->kids[3] = last->kids[3];
			a->kids[0] = last->kids[0];
			a->x.ints[0] = last->x.ints[0];
			goto L640;   /* OP2 %f,%t,%x,%y,%z */
		}
		if (
		a->x.ints[2] == a->x.ints[0]
		&& a->kids[2] == a->kids[0]
		&& a->kids[5] == a->kids[3]
		&& a->syms[2] == a->syms[0]
		) {
			goto L615;   /* %f%t2 %x,%z */
		}
		if (
		a->x.ints[0] == 12  /* r%n */
		&& a->x.ints[2] == 13  /* r%c */
		&& a->syms[2] == a->kids[0]->syms[2]
		) {
			goto L615;   /* %f%t2 %x,%z */
		}
		if (
		"\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0\1\0\1\0\0\0\0\0\0"[a->x.ints[3]]  /* <TMP3532> */
		&& a->x.ints[2] == a->x.ints[1]
		&& a->kids[2] == a->kids[1]
		&& a->kids[5] == a->kids[4]
		&& a->syms[2] == a->syms[1]
		) {
			a->syms[1] = a->syms[0];
			a->kids[4] = a->kids[3];
			a->kids[3] = 0;
			a->kids[1] = a->kids[0];
			a->kids[0] = 0;
			a->x.ints[1] = a->x.ints[0];
			goto L615;   /* %f%t2 %x,%z */
		}
		if (
		"\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0\1\0\1\0\0\0\0\0\0"[a->x.ints[3]]  /* <TMP3537> */
		&& a->x.ints[1] == 12  /* r%n */
		&& a->x.ints[2] == 13  /* r%c */
		&& a->syms[2] == a->kids[1]->syms[2]
		) {
			a->syms[1] = a->syms[0];
			a->kids[4] = a->kids[3];
			a->kids[3] = 0;
			a->kids[1] = a->kids[0];
			a->kids[0] = 0;
			a->x.ints[1] = a->x.ints[0];
			goto L615;   /* %f%t2 %x,%z */
		}
		goto L616;   /* %f%t3 %x,%y,%z */
	case 641: L641:   /* OP2K %f,%t,%x,%y,%z */
		dumptree(a, 641);
		goto L616;   /* %f%t3 %x,%y,%z */
	case 642: L642:   /* j%f %z */
		dumptree(a, 642);
		a->op = 642;
		break;
	case 643: L643:   /* jbr %z */
		dumptree(a, 643);
		a->op = 643;
		break;
	case 644: L644:   /* jmp %z */
		dumptree(a, 644);
		a->op = 644;
		break;
	case 645: L645:   /* ret */
		dumptree(a, 645);
		if (
		glevel>1
		) {
			goto L647;   /* # nop */
		}
		a->op = 645;
		break;
	case 646: L646:   /* # label */
		dumptree(a, 646);
		a->op = 646;
		return;
	case 647: L647:   /* # nop */
		dumptree(a, 647);
		a->op = 647;
		return;
	case 648: L648:   /* %z: */
		dumptree(a, 648);
		a->op = 648;
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
	case 17: L17:   /* CNSTF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 14;  /* $%c */
		a->x.ints[4] = 3;  /* l */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 18: L18:   /* CNSTD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 14;  /* $%c */
		a->x.ints[4] = 3;  /* l */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 19: L19:   /* CNSTC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 14;  /* $%c */
		a->x.ints[4] = 3;  /* l */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 20: L20:   /* CNSTS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 14;  /* $%c */
		a->x.ints[4] = 3;  /* l */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 21: L21:   /* CNSTI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 14;  /* $%c */
		a->x.ints[4] = 3;  /* l */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 22: L22:   /* CNSTU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 14;  /* $%c */
		a->x.ints[4] = 3;  /* l */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 23: L23:   /* CNSTP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 14;  /* $%c */
		a->x.ints[4] = 3;  /* l */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 33: L33:   /* ARGF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 28;  /* -(sp) */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 4;  /* f */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 34: L34:   /* ARGD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 28;  /* -(sp) */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 5;  /* d */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 37: L37:   /* ARGI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 28;  /* -(sp) */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 39: L39:   /* ARGP */
		dumptree(a, a->op);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[2] = 28;  /* -(sp) */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 41: L41:   /* ARGB */
		dumptree(a, a->op);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[2] = 28;  /* -(sp) */
		a->x.ints[0] = 9;  /* (r%n) */
		a->syms[1] = a->syms[0];
		a->x.ints[1] = 14;  /* $%c */
		a->x.ints[4] = 6;  /* x */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 49: L49:   /* ASGNF */
		dumptree(a, a->op);
		aset(a,0);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 9;  /* (r%n) */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 4;  /* f */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 50: L50:   /* ASGND */
		dumptree(a, a->op);
		aset(a,0);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 9;  /* (r%n) */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 5;  /* d */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 51: L51:   /* ASGNC */
		dumptree(a, a->op);
		aset(a,0);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 9;  /* (r%n) */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 1;  /* b */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 52: L52:   /* ASGNS */
		dumptree(a, a->op);
		aset(a,0);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 9;  /* (r%n) */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 2;  /* w */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 53: L53:   /* ASGNI */
		dumptree(a, a->op);
		aset(a,0);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 9;  /* (r%n) */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 55: L55:   /* ASGNP */
		dumptree(a, a->op);
		aset(a,0);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 9;  /* (r%n) */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 57: L57:   /* ASGNB */
		dumptree(a, a->op);
		aset(a,0);
		aset(a,1);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 9;  /* (r%n) */
		a->x.ints[0] = 9;  /* (r%n) */
		a->syms[1] = a->syms[0];
		a->x.ints[1] = 14;  /* $%c */
		a->x.ints[4] = 6;  /* x */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 65: L65:   /* INDIRF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 9;  /* (r%n) */
		a->x.ints[4] = 4;  /* f */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 66: L66:   /* INDIRD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 9;  /* (r%n) */
		a->x.ints[4] = 5;  /* d */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 67: L67:   /* INDIRC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 9;  /* (r%n) */
		a->x.ints[4] = 1;  /* b */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 68: L68:   /* INDIRS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 9;  /* (r%n) */
		a->x.ints[4] = 2;  /* w */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 69: L69:   /* INDIRI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 9;  /* (r%n) */
		a->x.ints[4] = 3;  /* l */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 71: L71:   /* INDIRP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 9;  /* (r%n) */
		a->x.ints[4] = 3;  /* l */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 73: L73:   /* INDIRB */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 85: L85:   /* CVCI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 13;  /* b */
		goto L634;   /* CVT %f,%t,%x,%y,%z */
	case 86: L86:   /* CVCU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 13;  /* b */
		goto L638;   /* MOVZ %f,%t,%x,%y,%z */
	case 97: L97:   /* CVDF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 4;  /* f */
		a->x.ints[3] = 17;  /* d */
		goto L634;   /* CVT %f,%t,%x,%y,%z */
	case 101: L101:   /* CVDI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 17;  /* d */
		goto L634;   /* CVT %f,%t,%x,%y,%z */
	case 114: L114:   /* CVFD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 5;  /* d */
		a->x.ints[3] = 16;  /* f */
		goto L634;   /* CVT %f,%t,%x,%y,%z */
	case 130: L130:   /* CVID */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 5;  /* d */
		a->x.ints[3] = 15;  /* l */
		goto L634;   /* CVT %f,%t,%x,%y,%z */
	case 131: L131:   /* CVIC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 1;  /* b */
		a->x.ints[3] = 15;  /* l */
		goto L634;   /* CVT %f,%t,%x,%y,%z */
	case 132: L132:   /* CVIS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 2;  /* w */
		a->x.ints[3] = 15;  /* l */
		goto L634;   /* CVT %f,%t,%x,%y,%z */
	case 134: L134:   /* CVIU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 15;  /* l */
		goto L634;   /* CVT %f,%t,%x,%y,%z */
	case 150: L150:   /* CVPU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 15;  /* l */
		goto L634;   /* CVT %f,%t,%x,%y,%z */
	case 165: L165:   /* CVSI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 14;  /* w */
		goto L634;   /* CVT %f,%t,%x,%y,%z */
	case 166: L166:   /* CVSU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 14;  /* w */
		goto L638;   /* MOVZ %f,%t,%x,%y,%z */
	case 179: L179:   /* CVUC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 1;  /* b */
		a->x.ints[3] = 15;  /* l */
		goto L634;   /* CVT %f,%t,%x,%y,%z */
	case 180: L180:   /* CVUS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 2;  /* w */
		a->x.ints[3] = 15;  /* l */
		goto L634;   /* CVT %f,%t,%x,%y,%z */
	case 181: L181:   /* CVUI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 15;  /* l */
		goto L634;   /* CVT %f,%t,%x,%y,%z */
	case 183: L183:   /* CVUP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 15;  /* l */
		goto L634;   /* CVT %f,%t,%x,%y,%z */
	case 193: L193:   /* NEGF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 4;  /* f */
		a->x.ints[3] = 29;  /* mneg */
		goto L639;   /* OP1 %f,%t,%x,%y,%z */
	case 194: L194:   /* NEGD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 5;  /* d */
		a->x.ints[3] = 29;  /* mneg */
		goto L639;   /* OP1 %f,%t,%x,%y,%z */
	case 197: L197:   /* NEGI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 29;  /* mneg */
		goto L639;   /* OP1 %f,%t,%x,%y,%z */
	case 209: L209:   /* CALLF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[0] = 9;  /* (r%n) */
		a->syms[1] = a->syms[0];
		a->x.ints[1] = 29;  /* $%c/4 */
		setreg(a,f0);
		goto L632;   /* CALL %f,%t,%x,%y,%z */
	case 210: L210:   /* CALLD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[0] = 9;  /* (r%n) */
		a->syms[1] = a->syms[0];
		a->x.ints[1] = 29;  /* $%c/4 */
		setreg(a,d0);
		goto L632;   /* CALL %f,%t,%x,%y,%z */
	case 213: L213:   /* CALLI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[0] = 9;  /* (r%n) */
		a->syms[1] = a->syms[0];
		a->x.ints[1] = 29;  /* $%c/4 */
		setreg(a,r0);
		goto L632;   /* CALL %f,%t,%x,%y,%z */
	case 216: L216:   /* CALLV */
		dumptree(a, a->op);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[0] = 9;  /* (r%n) */
		a->syms[1] = a->syms[0];
		a->x.ints[1] = 29;  /* $%c/4 */
		setreg(a,r0);
		goto L632;   /* CALL %f,%t,%x,%y,%z */
	case 217: L217:   /* CALLB */
		dumptree(a, a->op);
		aset(a,0);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[0] = 9;  /* (r%n) */
		a->syms[1] = a->syms[0];
		a->x.ints[1] = 29;  /* $%c/4 */
		setreg(a,r0), rtarget(a,1,r1);
		goto L632;   /* CALL %f,%t,%x,%y,%z */
	case 225: L225:   /* LOADF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 4;  /* f */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 226: L226:   /* LOADD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 5;  /* d */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 227: L227:   /* LOADC */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 1;  /* b */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 228: L228:   /* LOADS */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 2;  /* w */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 229: L229:   /* LOADI */
	case 230: L230:   /* LOADU */
	case 231: L231:   /* LOADP */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		goto L636;   /* MOV %f,%t,%x,%y,%z */
	case 241: L241:   /* RETF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rtarget(a,0,f0);
		goto L645;   /* ret */
	case 242: L242:   /* RETD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rtarget(a,0,d0);
		goto L645;   /* ret */
	case 245: L245:   /* RETI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rtarget(a,0,r0);
		goto L645;   /* ret */
	case 248: L248:   /* RETV */
		dumptree(a, a->op);
		goto L645;   /* ret */
	case 263: L263:   /* ADDRGP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 10;  /* %c */
		a->x.ints[4] = 3;  /* l */
		goto L637;   /* MOVA %f,%t,%x,%y,%z */
	case 279: L279:   /* ADDRFP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 6;  /* %c(ap) */
		a->x.ints[4] = 3;  /* l */
		goto L637;   /* MOVA %f,%t,%x,%y,%z */
	case 295: L295:   /* ADDRLP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 7;  /* %c(fp) */
		a->x.ints[4] = 3;  /* l */
		goto L637;   /* MOVA %f,%t,%x,%y,%z */
	case 305: L305:   /* ADDF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 4;  /* f */
		a->x.ints[3] = 18;  /* add */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 306: L306:   /* ADDD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 5;  /* d */
		a->x.ints[3] = 18;  /* add */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 309: L309:   /* ADDI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 18;  /* add */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 310: L310:   /* ADDU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 18;  /* add */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 311: L311:   /* ADDP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 18;  /* add */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 321: L321:   /* SUBF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 4;  /* f */
		a->x.ints[3] = 24;  /* sub */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 322: L322:   /* SUBD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 5;  /* d */
		a->x.ints[3] = 24;  /* sub */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 325: L325:   /* SUBI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 24;  /* sub */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 326: L326:   /* SUBU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 24;  /* sub */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 327: L327:   /* SUBP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 24;  /* sub */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 341: L341:   /* LSHI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		goto L630;   /* ASH %f,%t,%x,%y,%z */
	case 342: L342:   /* LSHU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		goto L630;   /* ASH %f,%t,%x,%y,%z */
	case 357: L357:   /* MODI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 22;  /* mod */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 358: L358:   /* MODU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[3] = 31;  /* urem */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		setreg(a,r0);
		goto L610;   /* pushl %x\npushl %y\ncalls $2,%f */
	case 373: L373:   /* RSHI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		goto L630;   /* ASH %f,%t,%x,%y,%z */
	case 374: L374:   /* RSHU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		goto L635;   /* EXT %f,%t,%x,%y,%z */
	case 390: L390:   /* BANDU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 19;  /* bic */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 406: L406:   /* BCOMU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 28;  /* mcom */
		goto L639;   /* OP1 %f,%t,%x,%y,%z */
	case 422: L422:   /* BORU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 20;  /* bis */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 438: L438:   /* BXORU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 25;  /* xor */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 449: L449:   /* DIVF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 4;  /* f */
		a->x.ints[3] = 21;  /* div */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 450: L450:   /* DIVD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 5;  /* d */
		a->x.ints[3] = 21;  /* div */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 453: L453:   /* DIVI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 21;  /* div */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 454: L454:   /* DIVU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[3] = 30;  /* udiv */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		setreg(a,r0);
		goto L610;   /* pushl %x\npushl %y\ncalls $2,%f */
	case 465: L465:   /* MULF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 4;  /* f */
		a->x.ints[3] = 23;  /* mul */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 466: L466:   /* MULD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 5;  /* d */
		a->x.ints[3] = 23;  /* mul */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 469: L469:   /* MULI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 23;  /* mul */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 470: L470:   /* MULU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 13;  /* r%c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 23;  /* mul */
		goto L640;   /* OP2 %f,%t,%x,%y,%z */
	case 481: L481:   /* EQF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 4;  /* f */
		a->x.ints[3] = 2;  /* eql */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 482: L482:   /* EQD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 5;  /* d */
		a->x.ints[3] = 2;  /* eql */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 485: L485:   /* EQI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 2;  /* eql */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 497: L497:   /* GEF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 4;  /* f */
		a->x.ints[3] = 4;  /* geq */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 498: L498:   /* GED */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 5;  /* d */
		a->x.ints[3] = 4;  /* geq */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 501: L501:   /* GEI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 4;  /* geq */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 502: L502:   /* GEU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 3;  /* gequ */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 513: L513:   /* GTF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 4;  /* f */
		a->x.ints[3] = 6;  /* gtr */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 514: L514:   /* GTD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 5;  /* d */
		a->x.ints[3] = 6;  /* gtr */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 517: L517:   /* GTI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 6;  /* gtr */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 518: L518:   /* GTU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 5;  /* gtru */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 529: L529:   /* LEF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 4;  /* f */
		a->x.ints[3] = 8;  /* leq */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 530: L530:   /* LED */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 5;  /* d */
		a->x.ints[3] = 8;  /* leq */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 533: L533:   /* LEI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 8;  /* leq */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 534: L534:   /* LEU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 7;  /* lequ */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 545: L545:   /* LTF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 4;  /* f */
		a->x.ints[3] = 10;  /* lss */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 546: L546:   /* LTD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 5;  /* d */
		a->x.ints[3] = 10;  /* lss */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 549: L549:   /* LTI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 10;  /* lss */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 550: L550:   /* LTU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 9;  /* lssu */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 561: L561:   /* NEF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 4;  /* f */
		a->x.ints[3] = 12;  /* neq */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 562: L562:   /* NED */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 5;  /* d */
		a->x.ints[3] = 12;  /* neq */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 565: L565:   /* NEI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		a->x.ints[0] = 12;  /* r%n */
		a->x.ints[1] = 12;  /* r%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 12;  /* neq */
		goto L633;   /* CMP %f,%t,%x,%y,%z */
	case 584: L584:   /* JUMPV */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 9;  /* (r%n) */
		goto L644;   /* jmp %z */
	case 600: L600:   /* LABELV */
		dumptree(a, a->op);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 10;  /* %c */
		goto L648;   /* %z: */
	case 608: L608:   /* subl3 %x,$32,r0\nextzv %x,r0,%y,%z */
		dumptree(a, 608);
		a->op = 608;
		break;
	case 609: L609:   /* calls %x,%y */
		dumptree(a, 609);
		a->op = 609;
		break;
	case 610: L610:   /* pushl %x\npushl %y\ncalls $2,%f */
		dumptree(a, 610);
		a->op = 610;
		break;
	case 611: L611:   /* movc3 %x,%y,%z */
		dumptree(a, 611);
		a->op = 611;
		break;
	case 612: L612:   /* subl2 %x,sp\nmovc3 %z,%y,(sp) */
		dumptree(a, 612);
		a->op = 612;
		break;
	case 613: L613:   /* %f%t %y,%z */
		dumptree(a, 613);
		a->op = 613;
		break;
	case 614: L614:   /* %f%t %z */
		dumptree(a, 614);
		a->op = 614;
		break;
	case 615: L615:   /* %f%t2 %x,%z */
		dumptree(a, 615);
		a->op = 615;
		break;
	case 616: L616:   /* %f%t3 %x,%y,%z */
		dumptree(a, 616);
		a->op = 616;
		break;
	case 617: L617:   /* ash%t %x,%y,%z */
		dumptree(a, 617);
		a->op = 617;
		break;
	case 618: L618:   /* clr%t %z */
		dumptree(a, 618);
		a->op = 618;
		break;
	case 619: L619:   /* cmp%t %y,%x\nj%f %z */
		dumptree(a, 619);
		a->op = 619;
		break;
	case 620: L620:   /* cvt%f%t %y,%z */
		dumptree(a, 620);
		a->op = 620;
		break;
	case 621: L621:   /* div%t3 %x,%y,-(sp)\nmul%t2 %x,(sp)\nsub%t3 (sp)+,%y,%z */
		dumptree(a, 621);
		a->op = 621;
		break;
	case 622: L622:   /* extzv %x,%y,%z */
		dumptree(a, 622);
		a->op = 622;
		break;
	case 623: L623:   /* mov%t %y,%z */
		dumptree(a, 623);
		a->op = 623;
		break;
	case 624: L624:   /* mova%t %y,%z */
		dumptree(a, 624);
		a->op = 624;
		break;
	case 625: L625:   /* movz%f%t %y,%z */
		dumptree(a, 625);
		a->op = 625;
		break;
	case 626: L626:   /* pusha%t %y */
		dumptree(a, 626);
		a->op = 626;
		break;
	case 627: L627:   /* pushl %y */
		dumptree(a, 627);
		a->op = 627;
		break;
	case 628: L628:   /* tst%t %y\nj%f %z */
		dumptree(a, 628);
		a->op = 628;
		break;
	case 629: L629:   /* bit%t %y,%x\nj%f %z */
		dumptree(a, 629);
		a->op = 629;
		break;
	case 630: L630:   /* ASH %f,%t,%x,%y,%z */
		dumptree(a, 630);
		if (
		a->x.ints[4] == 3  /* l */
		&& a->x.ints[1] == 14  /* $%c */
		) {
			if (
			a->syms[1]->x.name == sym[2].x.name  /* 1 */
			&& a->x.ints[0] == 12  /* r%n */
			&& a->x.ints[2] == 13  /* r%c */
			) {
				a->syms[0] = &sym[1];  /* 0 */
				a->x.ints[0] = 24;  /* %c[r%i] */
				a->x.ints[4] = 2;  /* w */
				a->kids[3] = a->kids[0];
				a->kids[0] = 0;
				goto L637;   /* MOVA %f,%t,%x,%y,%z */
			}
			if (
			a->syms[1]->x.name == sym[4].x.name  /* 3 */
			&& a->x.ints[0] == 12  /* r%n */
			&& a->x.ints[2] == 13  /* r%c */
			) {
				a->syms[0] = &sym[1];  /* 0 */
				a->x.ints[0] = 24;  /* %c[r%i] */
				a->x.ints[4] = 5;  /* d */
				a->kids[3] = a->kids[0];
				a->kids[0] = 0;
				goto L637;   /* MOVA %f,%t,%x,%y,%z */
			}
			if (
			a->syms[1]->x.name == sym[3].x.name  /* 2 */
			&& a->x.ints[0] == 12  /* r%n */
			&& a->x.ints[2] == 13  /* r%c */
			) {
				a->syms[0] = &sym[1];  /* 0 */
				a->x.ints[0] = 24;  /* %c[r%i] */
				a->x.ints[4] = 3;  /* l */
				a->kids[3] = a->kids[0];
				a->kids[0] = 0;
				goto L637;   /* MOVA %f,%t,%x,%y,%z */
			}
		}
		b = a->kids[1];
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1026> */
		&& a->x.ints[1] == 12  /* r%n */
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		&& "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1032> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[1] = b->syms[0];
			a->kids[4] = b->kids[3];
			a->x.ints[1] = b->x.ints[0];
			a->kids[1] = b->kids[0];
			goto L630;   /* ASH %f,%t,%x,%y,%z */
		}
		b = a->kids[0];
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1037> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		&& "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1043> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L630;   /* ASH %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1048> */
		&& a->x.ints[0] == 12  /* r%n */
		&& !kflag
		&& b->count == 1
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L630;   /* ASH %f,%t,%x,%y,%z */
		}
		a->op = 630;
		break;
	case 631: L631:   /* BIT %f,%t,%x,%y,%z */
		dumptree(a, 631);
		a->op = 631;
		break;
	case 632: L632:   /* CALL %f,%t,%x,%y,%z */
		dumptree(a, 632);
		if (
		a->x.ints[0] == 9  /* (r%n) */
		) {
			b = a->kids[0];
			if (
			b->op == 637  /* MOVA %f,%t,%x,%y,%z */
			&& "\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1063> */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L632;   /* CALL %f,%t,%x,%y,%z */
			}
			if (
			!kflag
			&& b->count == 1
			&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
			) {
				/* %c(ap) => *%c(ap) */
				/* %c(fp) => *%c(fp) */
				/* %c(r%n) => *%c(r%n) */
				/* (r%n) => *(r%n) */
				/* %c => *%c */
				/* r%n => (r%n) */
				/* $%c => *$%c */
				if (s="\0\0\0\0\0\0\1\2\3\4\5\0\11\0\13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]) {
					if (--b->count > 0)
						if (c=b->kids[0]) ++c->count;
					a->x.ints[0] = s;
					a->syms[0] = b->syms[0];
					a->kids[0] = b->kids[0];
					goto L632;   /* CALL %f,%t,%x,%y,%z */
				}
			}
		}
		a->op = 632;
		break;
	case 633: L633:   /* CMP %f,%t,%x,%y,%z */
		dumptree(a, 633);
		b = a->kids[1];
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1138> */
		&& a->x.ints[1] == 12  /* r%n */
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		&& "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1144> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[1] = b->syms[0];
			a->kids[4] = b->kids[3];
			a->x.ints[1] = b->x.ints[0];
			a->kids[1] = b->kids[0];
			goto L633;   /* CMP %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1149> */
		&& a->x.ints[1] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[1] = b->syms[0];
			a->kids[4] = b->kids[3];
			a->x.ints[1] = b->x.ints[0];
			a->kids[1] = b->kids[0];
			goto L633;   /* CMP %f,%t,%x,%y,%z */
		}
		/* b => b */
		/* w => w */
		if (s="\0\15\16\0\0\0\0\0"[a->x.ints[4]]) {
			if (
			a->x.ints[1] == 12  /* r%n */
			&& b->count == 1
			&& b->op == 634  /* CVT %f,%t,%x,%y,%z */
			&& b->x.ints[3] == s
			&& b->x.ints[4] == 3  /* l */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[1] = b->syms[0];
				a->kids[4] = b->kids[3];
				a->x.ints[1] = b->x.ints[0];
				a->kids[1] = b->kids[0];
				goto L633;   /* CMP %f,%t,%x,%y,%z */
			}
		}
		b = a->kids[0];
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1183> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L633;   /* CMP %f,%t,%x,%y,%z */
		}
		switch (a->x.ints[4]) {
		case 1:   /* b */
			if (
			a->x.ints[0] == 12  /* r%n */
			&& b->count == 1
			&& b->op == 634  /* CVT %f,%t,%x,%y,%z */
			&& b->x.ints[3] == 13  /* b */
			&& b->x.ints[4] == 3  /* l */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L633;   /* CMP %f,%t,%x,%y,%z */
			}
			if (
			a->x.ints[1] == 14  /* $%c */
			&& lop(a,1,8)
			) {
				goto L633;   /* CMP %f,%t,%x,%y,%z */
			}
			if (
			a->x.ints[0] == 14  /* $%c */
			&& lop(a,0,8)
			) {
				goto L633;   /* CMP %f,%t,%x,%y,%z */
			}
			break;
		case 2:   /* w */
			if (
			a->x.ints[0] == 12  /* r%n */
			&& b->count == 1
			&& b->op == 634  /* CVT %f,%t,%x,%y,%z */
			&& b->x.ints[3] == 14  /* w */
			&& b->x.ints[4] == 3  /* l */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L633;   /* CMP %f,%t,%x,%y,%z */
			}
			if (
			a->x.ints[1] == 14  /* $%c */
			&& lop(a,1,16)
			) {
				goto L633;   /* CMP %f,%t,%x,%y,%z */
			}
			if (
			a->x.ints[0] == 14  /* $%c */
			&& lop(a,0,16)
			) {
				goto L633;   /* CMP %f,%t,%x,%y,%z */
			}
			break;
		case 3:   /* l */
			switch (a->x.ints[1]) {
			case 12:   /* r%n */
				if (
				a->x.ints[0] == 12  /* r%n */
				&& b->op == 634  /* CVT %f,%t,%x,%y,%z */
				) {
					switch (b->x.ints[3]) {
					case 13:   /* b */
						b = a->kids[1];
						if (
						a->kids[0]->x.ints[4] == 3  /* l */
						&& a->kids[0]->count == 1
						&& b->op == 634  /* CVT %f,%t,%x,%y,%z */
						&& b->x.ints[3] == 13  /* b */
						&& b->x.ints[4] == 3  /* l */
						&& b->count == 1
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
							goto L633;   /* CMP %f,%t,%x,%y,%z */
						}
						break;
					case 14:   /* w */
						b = a->kids[1];
						if (
						a->kids[0]->x.ints[4] == 3  /* l */
						&& a->kids[0]->count == 1
						&& b->op == 634  /* CVT %f,%t,%x,%y,%z */
						&& b->x.ints[3] == 14  /* w */
						&& b->x.ints[4] == 3  /* l */
						&& b->count == 1
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
							goto L633;   /* CMP %f,%t,%x,%y,%z */
						}
						break;
					}
				}
				break;
			case 14:   /* $%c */
				if (
				a->x.ints[0] == 12  /* r%n */
				) {
					if (
					range(getint(a->syms[1]),8)
					&& b->count == 1
					&& b->op == 634  /* CVT %f,%t,%x,%y,%z */
					&& b->x.ints[3] == 13  /* b */
					&& b->x.ints[4] == 3  /* l */
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
						goto L633;   /* CMP %f,%t,%x,%y,%z */
					}
					if (
					range(getint(a->syms[1]),16)
					&& b->count == 1
					&& b->op == 634  /* CVT %f,%t,%x,%y,%z */
					&& b->x.ints[3] == 14  /* w */
					&& b->x.ints[4] == 3  /* l */
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
						goto L633;   /* CMP %f,%t,%x,%y,%z */
					}
				}
				break;
			}
			break;
		}
		if (
		a->x.ints[1] == 14  /* $%c */
		&& a->syms[1]->x.name == sym[1].x.name  /* 0 */
		&& a->x.ints[0] == 12  /* r%n */
		&& last && last->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& last && last->x.ints[0] == 12  /* r%n */
		&& last && a->kids[0] == last->kids[0]
		) {
			a->kids[0] && --b->count;
			a->kids[0] = 0;
			rewrite(last);
			goto L642;   /* j%f %z */
		}
		a->op = 633;
		break;
	case 634: L634:   /* CVT %f,%t,%x,%y,%z */
		dumptree(a, 634);
		b = a->kids[0];
		if (
		a->x.ints[0] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 634  /* CVT %f,%t,%x,%y,%z */
		) {
			switch (b->x.ints[3]) {
			case 13:   /* b */
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->x.ints[3] = 13;  /* b */
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L634;   /* CVT %f,%t,%x,%y,%z */
			case 14:   /* w */
				if (
				"\1\1\1\1\1\1\0\0"[b->x.ints[4]]  /* <TMP1238> */
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->x.ints[3] = 14;  /* w */
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L634;   /* CVT %f,%t,%x,%y,%z */
				}
				break;
			}
			if (
			"\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1"[b->x.ints[3]]  /* <TMP1253> */
			&& b->x.ints[4] == 5  /* d */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->x.ints[3] = b->x.ints[3];
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L634;   /* CVT %f,%t,%x,%y,%z */
			}
		}
		switch (a->x.ints[3]) {
		case 13:   /* b */
			if (
			a->x.ints[4] == 1  /* b */
			) {
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			break;
		case 14:   /* w */
			if (
			a->x.ints[4] == 2  /* w */
			) {
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			if (
			a->x.ints[4] == 1  /* b */
			&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[0]]  /* <TMP1334> */
			) {
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			break;
		case 15:   /* l */
			if (
			a->x.ints[4] == 2  /* w */
			&& a->x.ints[0] == 12  /* r%n */
			&& b->count == 1
			&& b->op == 638  /* MOVZ %f,%t,%x,%y,%z */
			&& b->x.ints[3] == 13  /* b */
			&& b->x.ints[4] == 3  /* l */
			&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1272> */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->x.ints[3] = 13;  /* b */
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L638;   /* MOVZ %f,%t,%x,%y,%z */
			}
			if (
			"\1\1\1\0\0\0\0\0"[a->x.ints[4]]  /* <TMP1283> */
			&& a->x.ints[0] == 12  /* r%n */
			&& b->count == 1
			&& b->op == 638  /* MOVZ %f,%t,%x,%y,%z */
			&& b->x.ints[4] == 3  /* l */
			&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1290> */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			if (
			a->x.ints[4] == 3  /* l */
			) {
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			if (
			"\1\1\1\0\0\0\0\0"[a->x.ints[4]]  /* <TMP1326> */
			&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[0]]  /* <TMP1327> */
			) {
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			break;
		case 16:   /* f */
			if (
			a->x.ints[4] == 4  /* f */
			) {
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			break;
		case 17:   /* d */
			if (
			a->x.ints[4] == 5  /* d */
			) {
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			break;
		}
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1339> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		&& "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1345> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L634;   /* CVT %f,%t,%x,%y,%z */
		}
		/* b => b */
		/* w => w */
		/* l => l */
		/* f => f */
		/* d => d */
		if (s="\0\0\0\0\0\0\0\0\0\0\0\0\0\1\2\3\4\5\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[3]]) {
			if (
			a->x.ints[0] == 12  /* r%n */
			&& b->count == 1
			&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
			&& b->x.ints[4] == s
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L634;   /* CVT %f,%t,%x,%y,%z */
			}
		}
		switch (a->x.ints[4]) {
		case 1:   /* b */
			if (
			a->x.ints[0] == 14  /* $%c */
			&& lop(a,0,8)
			) {
				goto L634;   /* CVT %f,%t,%x,%y,%z */
			}
			break;
		case 2:   /* w */
			if (
			a->x.ints[0] == 14  /* $%c */
			&& lop(a,0,16)
			) {
				goto L634;   /* CVT %f,%t,%x,%y,%z */
			}
			break;
		}
		a->op = 634;
		break;
	case 635: L635:   /* EXT %f,%t,%x,%y,%z */
		dumptree(a, 635);
		b = a->kids[1];
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1405> */
		&& a->x.ints[1] == 12  /* r%n */
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		&& "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1411> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[1] = b->syms[0];
			a->kids[4] = b->kids[3];
			a->x.ints[1] = b->x.ints[0];
			a->kids[1] = b->kids[0];
			goto L635;   /* EXT %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1416> */
		&& a->x.ints[1] == 12  /* r%n */
		&& !kflag
		&& b->count == 1
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[1] = b->syms[0];
			a->kids[4] = b->kids[3];
			a->x.ints[1] = b->x.ints[0];
			a->kids[1] = b->kids[0];
			goto L635;   /* EXT %f,%t,%x,%y,%z */
		}
		a->op = 635;
		break;
	case 636: L636:   /* MOV %f,%t,%x,%y,%z */
		dumptree(a, 636);
		if (
		"\1\0\0\0\0\0\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[2]]  /* <TMP1444> */
		&& isregvar(a->syms[2])
		) {
			a->x.ints[2] = 13;  /* r%c */
			goto L636;   /* MOV %f,%t,%x,%y,%z */
		}
		if (
		a->x.ints[2] == 9  /* (r%n) */
		) {
			b = a->kids[2];
			if (
			!kflag
			&& b->count == 1
			&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
			) {
				/* %c(ap) => *%c(ap) */
				/* %c(fp) => *%c(fp) */
				/* %c(r%n) => *%c(r%n) */
				/* (r%n) => *(r%n) */
				/* %c => *%c */
				/* r%n => (r%n) */
				/* $%c => *$%c */
				if (s="\0\0\0\0\0\0\1\2\3\4\5\0\11\0\13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]) {
					if (--b->count > 0)
						if (c=b->kids[0]) ++c->count;
					a->x.ints[2] = s;
					a->syms[2] = b->syms[0];
					a->kids[2] = b->kids[0];
					goto L636;   /* MOV %f,%t,%x,%y,%z */
				}
			}
			if (
			b->op == 637  /* MOVA %f,%t,%x,%y,%z */
			&& "\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1524> */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[2] = b->syms[0];
				a->kids[5] = b->kids[3];
				a->x.ints[2] = b->x.ints[0];
				a->kids[2] = b->kids[0];
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
		}
		b = a->kids[2];
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1529> */
		&& a->x.ints[2] == 9  /* (r%n) */
		&& b->op == 637  /* MOVA %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		&& "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1535> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[2] = b->syms[0];
			a->kids[5] = b->kids[3];
			a->x.ints[2] = b->x.ints[0];
			a->kids[2] = b->kids[0];
			goto L636;   /* MOV %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1540> */
		&& a->x.ints[2] == 9  /* (r%n) */
		&& b->count == 1
		&& b->op == 637  /* MOVA %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[2] = b->syms[0];
			a->kids[5] = b->kids[3];
			a->x.ints[2] = b->x.ints[0];
			a->kids[2] = b->kids[0];
			goto L636;   /* MOV %f,%t,%x,%y,%z */
		}
		if (
		a->x.ints[4] == 4  /* f */
		&& a->x.ints[2] == 9  /* (r%n) */
		) {
			if (
			b->count == 1
			&& b->op == 637  /* MOVA %f,%t,%x,%y,%z */
			&& b->x.ints[4] == 3  /* l */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[2] = b->syms[0];
				a->kids[5] = b->kids[3];
				a->x.ints[2] = b->x.ints[0];
				a->kids[2] = b->kids[0];
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			if (
			b->op == 637  /* MOVA %f,%t,%x,%y,%z */
			&& b->x.ints[4] == 3  /* l */
			&& "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1568> */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[2] = b->syms[0];
				a->kids[5] = b->kids[3];
				a->x.ints[2] = b->x.ints[0];
				a->kids[2] = b->kids[0];
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
		}
		if (
		"\1\0\0\0\0\0\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[0]]  /* <TMP1573> */
		&& isregvar(a->syms[0])
		) {
			a->x.ints[0] = 13;  /* r%c */
			goto L636;   /* MOV %f,%t,%x,%y,%z */
		}
		switch (a->x.ints[0]) {
		case 9:   /* (r%n) */
			b = a->kids[0];
			if (
			b->op == 637  /* MOVA %f,%t,%x,%y,%z */
			&& "\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1589> */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			break;
		case 13:   /* r%c */
			if (
			a->x.ints[2] == 13  /* r%c */
			&& rmcopy(a)
			) {
				a->syms[2] = a->syms[0];
				goto L647;   /* # nop */
			}
			break;
		}
		b = a->kids[0];
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1594> */
		&& a->x.ints[0] == 9  /* (r%n) */
		&& b->op == 637  /* MOVA %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		&& "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1600> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L636;   /* MOV %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1605> */
		&& a->x.ints[0] == 9  /* (r%n) */
		&& b->count == 1
		&& b->op == 637  /* MOVA %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L636;   /* MOV %f,%t,%x,%y,%z */
		}
		if (
		a->x.ints[4] == 4  /* f */
		&& a->x.ints[0] == 9  /* (r%n) */
		) {
			if (
			b->count == 1
			&& b->op == 637  /* MOVA %f,%t,%x,%y,%z */
			&& b->x.ints[4] == 3  /* l */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			if (
			b->op == 637  /* MOVA %f,%t,%x,%y,%z */
			&& b->x.ints[4] == 3  /* l */
			&& "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1633> */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
		}
		switch (a->x.ints[0]) {
		case 9:   /* (r%n) */
			if (
			!kflag
			&& b->count == 1
			&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
			) {
				/* %c(ap) => *%c(ap) */
				/* %c(fp) => *%c(fp) */
				/* %c(r%n) => *%c(r%n) */
				/* (r%n) => *(r%n) */
				/* %c => *%c */
				/* r%n => (r%n) */
				/* $%c => *$%c */
				if (s="\0\0\0\0\0\0\1\2\3\4\5\0\11\0\13\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]) {
					if (--b->count > 0)
						if (c=b->kids[0]) ++c->count;
					a->x.ints[0] = s;
					a->syms[0] = b->syms[0];
					a->kids[0] = b->kids[0];
					goto L636;   /* MOV %f,%t,%x,%y,%z */
				}
			}
			break;
		case 12:   /* r%n */
			if (
			b->op == 636  /* MOV %f,%t,%x,%y,%z */
			&& "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1713> */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			break;
		}
		if (
		a->x.ints[4] == 3  /* l */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 637  /* MOVA %f,%t,%x,%y,%z */
		&& "\1\1\1\1\1\1\1\1"[b->x.ints[4]]  /* <TMP1724> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->x.ints[4] = b->x.ints[4];
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L637;   /* MOVA %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1733> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1"[b->x.ints[3]]  /* <TMP1739> */
		&& b->x.ints[4] == a->x.ints[4]
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->x.ints[3] = b->x.ints[3];
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L636;   /* MOV %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1748> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 638  /* MOVZ %f,%t,%x,%y,%z */
		&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1"[b->x.ints[3]]  /* <TMP1754> */
		&& b->x.ints[4] == a->x.ints[4]
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->x.ints[3] = b->x.ints[3];
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L638;   /* MOVZ %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1763> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 639  /* OP1 %f,%t,%x,%y,%z */
		&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1"[b->x.ints[3]]  /* <TMP1769> */
		&& b->x.ints[4] == a->x.ints[4]
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->x.ints[3] = b->x.ints[3];
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L639;   /* OP1 %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1778> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 634  /* CVT %f,%t,%x,%y,%z */
		&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1"[b->x.ints[3]]  /* <TMP1784> */
		&& b->x.ints[4] == a->x.ints[4]
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->x.ints[3] = b->x.ints[3];
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L634;   /* CVT %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\0\0\0\0\0"[a->x.ints[4]]  /* <TMP1793> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1"[b->x.ints[3]]  /* <TMP1799> */
		&& b->x.ints[4] == 3  /* l */
		&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1801> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->x.ints[3] = b->x.ints[3];
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L636;   /* MOV %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\0\0\0\0\0"[a->x.ints[4]]  /* <TMP1810> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 638  /* MOVZ %f,%t,%x,%y,%z */
		&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1"[b->x.ints[3]]  /* <TMP1816> */
		&& b->x.ints[4] == 3  /* l */
		&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1818> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->x.ints[3] = b->x.ints[3];
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L638;   /* MOVZ %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\0\0\0\0\0"[a->x.ints[4]]  /* <TMP1827> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 639  /* OP1 %f,%t,%x,%y,%z */
		&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1"[b->x.ints[3]]  /* <TMP1833> */
		&& b->x.ints[4] == 3  /* l */
		&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1835> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->x.ints[3] = b->x.ints[3];
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L639;   /* OP1 %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\0\0\0\0\0"[a->x.ints[4]]  /* <TMP1844> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 634  /* CVT %f,%t,%x,%y,%z */
		&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1"[b->x.ints[3]]  /* <TMP1850> */
		&& b->x.ints[4] == 3  /* l */
		&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1852> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->x.ints[3] = b->x.ints[3];
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L634;   /* CVT %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1861> */
		&& a->x.ints[0] == 12  /* r%n */
		&& !kflag
		&& b->count == 1
		&& b->op == 630  /* ASH %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
				if (c=b->kids[1]) ++c->count;
				if (c=b->kids[4]) ++c->count;
			}
			a->syms[1] = b->syms[1];
			a->kids[4] = b->kids[4];
			a->kids[1] = b->kids[1];
			a->x.ints[1] = b->x.ints[1];
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L630;   /* ASH %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1871> */
		&& a->x.ints[0] == 12  /* r%n */
		&& !kflag
		&& b->count == 1
		&& b->op == 635  /* EXT %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
				if (c=b->kids[1]) ++c->count;
				if (c=b->kids[4]) ++c->count;
			}
			a->syms[1] = b->syms[1];
			a->kids[4] = b->kids[4];
			a->kids[1] = b->kids[1];
			a->x.ints[1] = b->x.ints[1];
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L635;   /* EXT %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP1881> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 640  /* OP2 %f,%t,%x,%y,%z */
		&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1"[b->x.ints[3]]  /* <TMP1887> */
		&& b->x.ints[4] == a->x.ints[4]
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
				if (c=b->kids[1]) ++c->count;
				if (c=b->kids[4]) ++c->count;
			}
			a->x.ints[3] = b->x.ints[3];
			a->syms[1] = b->syms[1];
			a->kids[4] = b->kids[4];
			a->kids[1] = b->kids[1];
			a->x.ints[1] = b->x.ints[1];
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L640;   /* OP2 %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\0\0\0\0\0"[a->x.ints[4]]  /* <TMP1896> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 640  /* OP2 %f,%t,%x,%y,%z */
		&& "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\0\0\1\1\1\0\0\0\0\0\0"[b->x.ints[3]]  /* <TMP1902> */
		&& b->x.ints[4] == 3  /* l */
		&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[1]]  /* <TMP1904> */
		&& "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1905> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
				if (c=b->kids[1]) ++c->count;
				if (c=b->kids[4]) ++c->count;
			}
			a->x.ints[3] = b->x.ints[3];
			a->syms[1] = b->syms[1];
			a->kids[4] = b->kids[4];
			a->kids[1] = b->kids[1];
			a->x.ints[1] = b->x.ints[1];
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L640;   /* OP2 %f,%t,%x,%y,%z */
		}
		if (
		a->x.ints[0] == 12  /* r%n */
		) {
			if (
			b->count == 1
			) {
				switch (b->op) {
				case 634:   /* CVT %f,%t,%x,%y,%z */
					if (
					"\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1929> */
					) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[0] = b->syms[0];
						a->kids[3] = b->kids[3];
						a->x.ints[0] = b->x.ints[0];
						a->kids[0] = b->kids[0];
						goto L634;   /* CVT %f,%t,%x,%y,%z */
					}
					break;
				case 638:   /* MOVZ %f,%t,%x,%y,%z */
					if (
					"\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP1919> */
					) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[0] = b->syms[0];
						a->kids[3] = b->kids[3];
						a->x.ints[0] = b->x.ints[0];
						a->kids[0] = b->kids[0];
						goto L638;   /* MOVZ %f,%t,%x,%y,%z */
					}
					break;
				}
			}
			if (
			b->op == 636  /* MOV %f,%t,%x,%y,%z */
			&& b->x.ints[4] == 3  /* l */
			&& b->x.ints[0] == 12  /* r%n */
			) {
				if (--b->count > 0)
					if (c=b->kids[0]) ++c->count;
				a->kids[0] = b->kids[0];
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
		}
		switch (a->x.ints[4]) {
		case 1:   /* b */
			if (
			a->x.ints[0] == 14  /* $%c */
			&& lop(a,0,8)
			) {
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			break;
		case 2:   /* w */
			if (
			a->x.ints[0] == 14  /* $%c */
			&& lop(a,0,16)
			) {
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			break;
		}
		a->op = 636;
		break;
	case 637: L637:   /* MOVA %f,%t,%x,%y,%z */
		dumptree(a, 637);
		switch (a->x.ints[0]) {
		case 8:   /* %c(r%n) */
			if (
			a->syms[0]->x.name == sym[1].x.name  /* 0 */
			) {
				a->x.ints[0] = 9;  /* (r%n) */
				goto L637;   /* MOVA %f,%t,%x,%y,%z */
			}
			break;
		case 10:   /* %c */
			if (
			a->x.ints[2] == 13  /* r%c */
			&& !atst(a)
			) {
				a->x.ints[0] = 14;  /* $%c */
				a->x.ints[4] = 3;  /* l */
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			break;
		case 22:   /* %c(r%n)[r%i] */
			if (
			a->syms[0]->x.name == sym[1].x.name  /* 0 */
			) {
				a->x.ints[0] = 23;  /* (r%n)[r%i] */
				goto L637;   /* MOVA %f,%t,%x,%y,%z */
			}
			break;
		case 23:   /* (r%n)[r%i] */
			b = a->kids[0];
			if (
			b->op == 637  /* MOVA %f,%t,%x,%y,%z */
			) {
				/* *%c(ap) => *%c(ap)[r%i] */
				/* *%c(fp) => *%c(fp)[r%i] */
				/* *%c(r%n) => *%c(r%n)[r%i] */
				/* *(r%n) => *(r%n)[r%i] */
				/* *%c => *%c[r%i] */
				/* %c(ap) => %c(ap)[r%i] */
				/* %c(fp) => %c(fp)[r%i] */
				/* %c(r%n) => %c(r%n)[r%i] */
				/* (r%n) => (r%n)[r%i] */
				/* %c => %c[r%i] */
				/* *$%c => *$%c[r%i] */
				if (s="\0\17\20\21\22\23\24\25\26\27\30\31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]) {
					if (--b->count > 0)
						if (c=b->kids[0]) ++c->count;
					a->x.ints[0] = s;
					a->syms[0] = b->syms[0];
					a->kids[0] = b->kids[0];
					goto L637;   /* MOVA %f,%t,%x,%y,%z */
				}
			}
			if (
			!kflag
			&& b->count == 1
			&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
			) {
				/* %c(ap) => *%c(ap)[r%i] */
				/* %c(fp) => *%c(fp)[r%i] */
				/* %c(r%n) => *%c(r%n)[r%i] */
				/* (r%n) => *(r%n)[r%i] */
				/* %c => *%c[r%i] */
				/* r%n => (r%n)[r%i] */
				/* $%c => *$%c[r%i] */
				if (s="\0\0\0\0\0\0\17\20\21\22\23\0\27\0\31\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]) {
					if (--b->count > 0)
						if (c=b->kids[0]) ++c->count;
					a->x.ints[0] = s;
					a->syms[0] = b->syms[0];
					a->kids[0] = b->kids[0];
					goto L637;   /* MOVA %f,%t,%x,%y,%z */
				}
			}
			break;
		}
		a->op = 637;
		break;
	case 638: L638:   /* MOVZ %f,%t,%x,%y,%z */
		dumptree(a, 638);
		switch (a->x.ints[3]) {
		case 13:   /* b */
			if (
			a->x.ints[4] == 1  /* b */
			) {
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			b = a->kids[0];
			if (
			a->x.ints[0] == 12  /* r%n */
			&& b->count == 1
			&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
			&& b->x.ints[4] == 1  /* b */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L638;   /* MOVZ %f,%t,%x,%y,%z */
			}
			break;
		case 14:   /* w */
			switch (a->x.ints[4]) {
			case 1:   /* b */
				goto L634;   /* CVT %f,%t,%x,%y,%z */
			case 2:   /* w */
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			b = a->kids[0];
			if (
			a->x.ints[0] == 12  /* r%n */
			&& b->count == 1
			&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
			&& b->x.ints[4] == 2  /* w */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L638;   /* MOVZ %f,%t,%x,%y,%z */
			}
			break;
		case 15:   /* l */
			if (
			a->x.ints[4] == 3  /* l */
			) {
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			if (
			"\1\1\1\0\0\0\0\0"[a->x.ints[4]]  /* <TMP2884> */
			) {
				goto L634;   /* CVT %f,%t,%x,%y,%z */
			}
			b = a->kids[0];
			if (
			a->x.ints[0] == 12  /* r%n */
			&& b->count == 1
			&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
			&& b->x.ints[4] == 3  /* l */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L638;   /* MOVZ %f,%t,%x,%y,%z */
			}
			break;
		case 16:   /* f */
			if (
			a->x.ints[4] == 4  /* f */
			) {
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			b = a->kids[0];
			if (
			a->x.ints[0] == 12  /* r%n */
			&& b->count == 1
			&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
			&& b->x.ints[4] == 4  /* f */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L638;   /* MOVZ %f,%t,%x,%y,%z */
			}
			break;
		case 17:   /* d */
			if (
			a->x.ints[4] == 5  /* d */
			) {
				goto L636;   /* MOV %f,%t,%x,%y,%z */
			}
			b = a->kids[0];
			if (
			a->x.ints[0] == 12  /* r%n */
			&& b->count == 1
			&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
			&& b->x.ints[4] == 5  /* d */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L638;   /* MOVZ %f,%t,%x,%y,%z */
			}
			break;
		}
		b = a->kids[0];
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP2944> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		&& "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP2950> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L638;   /* MOVZ %f,%t,%x,%y,%z */
		}
		if (
		a->x.ints[0] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 638  /* MOVZ %f,%t,%x,%y,%z */
		) {
			switch (b->x.ints[3]) {
			case 13:   /* b */
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->x.ints[3] = 13;  /* b */
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L638;   /* MOVZ %f,%t,%x,%y,%z */
			case 14:   /* w */
				if (
				b->x.ints[4] == 3  /* l */
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->x.ints[3] = 14;  /* w */
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L638;   /* MOVZ %f,%t,%x,%y,%z */
				}
				break;
			}
		}
		switch (a->x.ints[4]) {
		case 1:   /* b */
			if (
			a->x.ints[0] == 14  /* $%c */
			&& lop(a,0,8)
			) {
				goto L638;   /* MOVZ %f,%t,%x,%y,%z */
			}
			break;
		case 2:   /* w */
			if (
			a->x.ints[0] == 14  /* $%c */
			&& lop(a,0,16)
			) {
				goto L638;   /* MOVZ %f,%t,%x,%y,%z */
			}
			break;
		}
		a->op = 638;
		break;
	case 639: L639:   /* OP1 %f,%t,%x,%y,%z */
		dumptree(a, 639);
		b = a->kids[0];
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP2986> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		&& "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP2992> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L639;   /* OP1 %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP2997> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L639;   /* OP1 %f,%t,%x,%y,%z */
		}
		switch (a->x.ints[4]) {
		case 1:   /* b */
			if (
			a->x.ints[0] == 14  /* $%c */
			&& lop(a,0,8)
			) {
				goto L639;   /* OP1 %f,%t,%x,%y,%z */
			}
			break;
		case 2:   /* w */
			if (
			a->x.ints[0] == 14  /* $%c */
			&& lop(a,0,16)
			) {
				goto L639;   /* OP1 %f,%t,%x,%y,%z */
			}
			break;
		}
		a->op = 639;
		break;
	case 640: L640:   /* OP2 %f,%t,%x,%y,%z */
		dumptree(a, 640);
		if (
		"\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\1\0\0\0\0\0\0\0\0"[a->x.ints[3]]  /* <TMP3007> */
		&& kflag
		) {
			goto L641;   /* OP2K %f,%t,%x,%y,%z */
		}
		if (
		a->x.ints[3] == 18  /* add */
		&& a->x.ints[4] == 3  /* l */
		&& a->x.ints[1] == 12  /* r%n */
		&& a->x.ints[0] == 12  /* r%n */
		) {
			if (
			a->x.ints[2] == 13  /* r%c */
			&& atst(a)
			) {
				b = a->kids[0];
				switch (b->op) {
				case 636:   /* MOV %f,%t,%x,%y,%z */
					if (
					b->x.ints[0] == 14  /* $%c */
					) {
						if (--b->count > 0)
							if (c=b->kids[3]) ++c->count;
						a->x.ints[0] = 8;  /* %c(r%n) */
						a->x.ints[4] = b->x.ints[4];
						a->syms[0] = b->syms[0];
						a->kids[3] = b->kids[3];
						a->kids[0] = a->kids[1];
						a->kids[1] = 0;
						goto L637;   /* MOVA %f,%t,%x,%y,%z */
					}
					break;
				case 637:   /* MOVA %f,%t,%x,%y,%z */
					if (
					b->x.ints[0] == 24  /* %c[r%i] */
					) {
						if (--b->count > 0)
							if (c=b->kids[3]) ++c->count;
						a->x.ints[0] = 22;  /* %c(r%n)[r%i] */
						a->x.ints[4] = b->x.ints[4];
						a->syms[0] = b->syms[0];
						a->kids[3] = b->kids[3];
						a->kids[0] = a->kids[1];
						a->kids[1] = 0;
						goto L637;   /* MOVA %f,%t,%x,%y,%z */
					}
					if (
					b->x.ints[0] == 10  /* %c */
					) {
						if (--b->count > 0)
							if (c=b->kids[3]) ++c->count;
						a->x.ints[0] = 8;  /* %c(r%n) */
						a->x.ints[4] = b->x.ints[4];
						a->syms[0] = b->syms[0];
						a->kids[3] = b->kids[3];
						a->kids[0] = a->kids[1];
						a->kids[1] = 0;
						goto L637;   /* MOVA %f,%t,%x,%y,%z */
					}
					break;
				}
				b = a->kids[1];
				switch (b->op) {
				case 636:   /* MOV %f,%t,%x,%y,%z */
					if (
					b->x.ints[0] == 14  /* $%c */
					) {
						if (--b->count > 0)
							if (c=b->kids[3]) ++c->count;
						a->x.ints[0] = 8;  /* %c(r%n) */
						a->x.ints[4] = b->x.ints[4];
						a->syms[0] = b->syms[0];
						a->kids[3] = b->kids[3];
						a->kids[1] = 0;
						goto L637;   /* MOVA %f,%t,%x,%y,%z */
					}
					break;
				case 637:   /* MOVA %f,%t,%x,%y,%z */
					if (
					b->x.ints[0] == 24  /* %c[r%i] */
					) {
						if (--b->count > 0)
							if (c=b->kids[3]) ++c->count;
						a->x.ints[0] = 22;  /* %c(r%n)[r%i] */
						a->x.ints[4] = b->x.ints[4];
						a->syms[0] = b->syms[0];
						a->kids[3] = b->kids[3];
						a->kids[1] = 0;
						goto L637;   /* MOVA %f,%t,%x,%y,%z */
					}
					if (
					b->x.ints[0] == 10  /* %c */
					) {
						if (--b->count > 0)
							if (c=b->kids[3]) ++c->count;
						a->x.ints[0] = 8;  /* %c(r%n) */
						a->x.ints[4] = b->x.ints[4];
						a->syms[0] = b->syms[0];
						a->kids[3] = b->kids[3];
						a->kids[1] = 0;
						goto L637;   /* MOVA %f,%t,%x,%y,%z */
					}
					break;
				}
			}
			b = a->kids[1];
			if (
			b->op == 637  /* MOVA %f,%t,%x,%y,%z */
			&& b->x.ints[0] == 7  /* %c(fp) */
			) {
				--b->count;
				a->x.ints[0] = 21;  /* %c(fp)[r%i] */
				a->x.ints[4] = 1;  /* b */
				a->syms[0] = b->syms[0];
				a->kids[3] = a->kids[0];
				a->kids[0] = 0;
				a->kids[1] = 0;
				goto L637;   /* MOVA %f,%t,%x,%y,%z */
			}
		}
		b = a->kids[1];
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP3145> */
		&& a->x.ints[1] == 12  /* r%n */
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		&& "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP3151> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[1] = b->syms[0];
			a->kids[4] = b->kids[3];
			a->x.ints[1] = b->x.ints[0];
			a->kids[1] = b->kids[0];
			goto L640;   /* OP2 %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP3156> */
		&& a->x.ints[1] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[1] = b->syms[0];
			a->kids[4] = b->kids[3];
			a->x.ints[1] = b->x.ints[0];
			a->kids[1] = b->kids[0];
			goto L640;   /* OP2 %f,%t,%x,%y,%z */
		}
		/* b => b */
		/* w => w */
		if (s="\0\15\16\0\0\0\0\0"[a->x.ints[4]]) {
			if (
			a->x.ints[1] == 12  /* r%n */
			&& b->count == 1
			&& b->op == 634  /* CVT %f,%t,%x,%y,%z */
			&& b->x.ints[3] == s
			&& b->x.ints[4] == 3  /* l */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[1] = b->syms[0];
				a->kids[4] = b->kids[3];
				a->x.ints[1] = b->x.ints[0];
				a->kids[1] = b->kids[0];
				goto L640;   /* OP2 %f,%t,%x,%y,%z */
			}
		}
		b = a->kids[0];
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP3190> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		&& "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP3196> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L640;   /* OP2 %f,%t,%x,%y,%z */
		}
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP3201> */
		&& a->x.ints[0] == 12  /* r%n */
		&& b->count == 1
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L640;   /* OP2 %f,%t,%x,%y,%z */
		}
		/* b => b */
		/* w => w */
		if (s="\0\15\16\0\0\0\0\0"[a->x.ints[4]]) {
			if (
			a->x.ints[0] == 12  /* r%n */
			&& b->count == 1
			&& b->op == 634  /* CVT %f,%t,%x,%y,%z */
			&& b->x.ints[3] == s
			&& b->x.ints[4] == 3  /* l */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L640;   /* OP2 %f,%t,%x,%y,%z */
			}
		}
		if (
		a->x.ints[3] == 18  /* add */
		&& a->x.ints[4] == 3  /* l */
		&& a->x.ints[1] == 14  /* $%c */
		) {
			if (
			a->syms[1]->x.name == sym[5].x.name  /* 4 */
			&& a->x.ints[0] == 12  /* r%n */
			&& a->x.ints[2] == 13  /* r%c */
			&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
			&& b->x.ints[4] == 3  /* l */
			&& b->x.ints[0] == 13  /* r%c */
			&& a->syms[2] == b->syms[0]
			&& last && last == a->kids[0]
			&& b->count == 2
			) {
				--b->count;
				last->op = 637;  /* MOVA %f,%t,%x,%y,%z */
				last->x.ints[0] = 26;  /* (r%c)+ */
				last->x.ints[4] = 3;  /* l */
				a->kids[0] = 0;
				rewrite(last);
				goto L647;   /* # nop */
			}
			if (
			a->syms[1]->x.name == sym[3].x.name  /* 2 */
			&& a->x.ints[0] == 12  /* r%n */
			&& a->x.ints[2] == 13  /* r%c */
			&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
			&& b->x.ints[4] == 3  /* l */
			&& b->x.ints[0] == 13  /* r%c */
			&& a->syms[2] == b->syms[0]
			&& last && last == a->kids[0]
			&& b->count == 2
			) {
				--b->count;
				last->op = 637;  /* MOVA %f,%t,%x,%y,%z */
				last->x.ints[0] = 26;  /* (r%c)+ */
				last->x.ints[4] = 2;  /* w */
				a->kids[0] = 0;
				rewrite(last);
				goto L647;   /* # nop */
			}
			if (
			a->syms[1]->x.name == sym[2].x.name  /* 1 */
			&& a->x.ints[0] == 12  /* r%n */
			&& a->x.ints[2] == 13  /* r%c */
			&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
			&& b->x.ints[4] == 3  /* l */
			&& b->x.ints[0] == 13  /* r%c */
			&& a->syms[2] == b->syms[0]
			&& last && last == a->kids[0]
			&& b->count == 2
			) {
				--b->count;
				last->op = 637;  /* MOVA %f,%t,%x,%y,%z */
				last->x.ints[0] = 26;  /* (r%c)+ */
				last->x.ints[4] = 1;  /* b */
				a->kids[0] = 0;
				rewrite(last);
				goto L647;   /* # nop */
			}
		}
		switch (a->x.ints[4]) {
		case 1:   /* b */
			if (
			a->x.ints[1] == 14  /* $%c */
			&& lop(a,1,8)
			) {
				goto L640;   /* OP2 %f,%t,%x,%y,%z */
			}
			if (
			a->x.ints[0] == 14  /* $%c */
			&& lop(a,0,8)
			) {
				goto L640;   /* OP2 %f,%t,%x,%y,%z */
			}
			break;
		case 2:   /* w */
			if (
			a->x.ints[1] == 14  /* $%c */
			&& lop(a,1,16)
			) {
				goto L640;   /* OP2 %f,%t,%x,%y,%z */
			}
			if (
			a->x.ints[0] == 14  /* $%c */
			&& lop(a,0,16)
			) {
				goto L640;   /* OP2 %f,%t,%x,%y,%z */
			}
			break;
		}
		a->op = 640;
		break;
	case 641: L641:   /* OP2K %f,%t,%x,%y,%z */
		dumptree(a, 641);
		b = a->kids[1];
		if (
		"\1\1\1\1\1\1\1\1"[a->x.ints[4]]  /* <TMP3280> */
		&& a->x.ints[1] == 12  /* r%n */
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == a->x.ints[4]
		&& "\1\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[b->x.ints[0]]  /* <TMP3286> */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[1] = b->syms[0];
			a->kids[4] = b->kids[3];
			a->x.ints[1] = b->x.ints[0];
			a->kids[1] = b->kids[0];
			goto L641;   /* OP2K %f,%t,%x,%y,%z */
		}
		a->op = 641;
		break;
	case 642: L642:   /* j%f %z */
		dumptree(a, 642);
		a->op = 642;
		break;
	case 643: L643:   /* jbr %z */
		dumptree(a, 643);
		a->op = 643;
		break;
	case 644: L644:   /* jmp %z */
		dumptree(a, 644);
		b = a->kids[2];
		if (
		a->x.ints[2] == 9  /* (r%n) */
		&& b->op == 636  /* MOV %f,%t,%x,%y,%z */
		&& b->x.ints[4] == 3  /* l */
		&& b->x.ints[0] == 14  /* $%c */
		) {
			--b->count;
			a->x.ints[2] = 10;  /* %c */
			a->syms[2] = b->syms[0];
			a->kids[2] = 0;
			goto L643;   /* jbr %z */
		}
		a->op = 644;
		break;
	case 645: L645:   /* ret */
		dumptree(a, 645);
		a->op = 645;
		break;
	case 646: L646:   /* # label */
		dumptree(a, 646);
		a->op = 646;
		break;
	case 647: L647:   /* # nop */
		dumptree(a, 647);
		a->op = 647;
		break;
	case 648: L648:   /* %z: */
		dumptree(a, 648);
		a->op = 648;
		break;
	default:
	assert(0);
	}
}


/* "set ^0,%g1; add %fp,%g1,%^c" => ="set ^y,%g1; add %fp,%g1,^z" A="^c" a=a C="%^c" */
/* "set _^0,%^c" => ="set ^y,^z" A="_^c" a=a C="%^c" */
/* "set ^0,%g1; add %fp,%g1,%^c" => ="set ^y,%g1; add %fp,%g1,^z" A="^c" a=a C="%^c" */
/* "std %f^0,[%sp+4*^c+68]" => ="st^t ^y,[^z]" E="d" A="%f^n" 0=0 C="%sp+4*^c+68" */
/* "st %f^0,[%sp+4*^c+68]" => ="st^t ^y,[^z]" E="" A="%f^n" 0=0 C="%sp+4*^c+68" */
/* "st %^0,[%sp+4*^c+68]" => ="st^t ^y,[^z]" E="" A="%^n" 0=0 C="%sp+4*^c+68" */
/* "st %^0,[%sp+4*^c+68]" => ="st^t ^y,[^z]" E="" A="%^n" 0=0 C="%sp+4*^c+68" */
/* "std %f^1,[%^0]" => ="st^t ^y,[^z]" E="d" A="%f^n" 0=1 C="%^n" 2=0 */
/* "st %f^1,[%^0]" => ="st^t ^y,[^z]" E="" A="%f^n" 0=1 C="%^n" 2=0 */
/* "stb %^1,[%^0]" => ="st^t ^y,[^z]" E="b" A="%^n" 0=1 C="%^n" 2=0 */
/* "st %^1,[%^0]" => ="st^t ^y,[^z]" E="" A="%^n" 0=1 C="%^n" 2=0 */
/* "st %^1,[%^0]" => ="st^t ^y,[^z]" E="" A="%^n" 0=1 C="%^n" 2=0 */
/* "sth %^1,[%^0]" => ="st^t ^y,[^z]" E="h" A="%^n" 0=1 C="%^n" 2=0 */
/* "xnor %^0,%g0,%^c" => ="^f ^y,^x,^z" D="xnor" A="%^n" 0=0 B="%g0" C="%^c" */
/* "call %^0; st %^1,[%sp+64]" => ="call ^y; st ^z,[%sp+64]" A="%^n" 0=0 C="%^n" 2=1 */
/* "call %^0; nop" => ="call ^y; nop" A="%^n" 0=0 */
/* "call %^0; nop" => ="call ^y; nop" A="%^n" 0=0 */
/* "call %^0; nop" => ="call ^y; nop" A="%^n" 0=0 */
/* "call %^0; nop" => ="call ^y; nop" A="%^n" 0=0 */
/* "set ^0,%^c" => ="set ^y,^z" A="^c" a=a C="%^c" */
/* "set ^0,%^c" => ="set ^y,^z" A="^c" a=a C="%^c" */
/* "set ^0,%^c" => ="set ^y,^z" A="^c" a=a C="%^c" */
/* "set ^0,%^c" => ="set ^y,^z" A="^c" a=a C="%^c" */
/* "set ^0,%^c" => ="set ^y,^z" A="^c" a=a C="%^c" */
/* "sll %^0,24,%g1; sra %g1,24,%^c" => ="sll ^y,^x,%g1; sra %g1,^x,^z" A="%^n" 0=0 B="^c" b="24" B="^c" b="24" C="%^c" */
/* "sll %^0,16,%g1; sra %g1,16,%^c" => ="sll ^y,^x,%g1; sra %g1,^x,^z" A="%^n" 0=0 B="^c" b="16" B="^c" b="16" C="%^c" */
/* "and %^0,0xff,%^c" => ="^f ^y,^x,^z" D="and" A="%^n" 0=0 B="^c" b="0xff" C="%^c" */
/* "set 0xffff,%g1; and %^0,%g1,%^c" => ="set 0xffff,%g1; and ^y,%g1,^z" A="%^n" 0=0 C="%^c" */
/* "fdtos %f^n,%f^c" => ="fdtos ^y,^z" A="%f^n" C="%f^c" */
/* "fstod %f^n,%f^c" => ="fstod ^y,^z" A="%f^n" C="%f^c" */
/* "st %^0,[%sp+64]; ld [%sp+64],^z; fitod ^z,%f^c" => ="st ^y,[%sp+64]; ld [%sp+64],^z; fitod ^z,^z" A="%^n" 0=0 C="%f^c" */
/* "fdtoi %f^0,%f0; st %f0,[%sp+64]; ld [%sp+64],%^c" => ="fdtoi ^y,%f0; st %f0,[%sp+64]; ld [%sp+64],^z" A="%f^n" 0=0 C="%^c" */
/* "mov %^0,%^c" => ="mov ^y,^z" A="%^n" 0=0 C="%^c" */
/* "mov %^0,%^c" => ="mov ^y,^z" A="%^n" 0=0 C="%^c" */
/* "mov %^0,%^c" => ="mov ^y,^z" A="%^n" 0=0 C="%^c" */
/* "mov %^0,%^c" => ="mov ^y,^z" A="%^n" 0=0 C="%^c" */
/* "mov %^0,%^c" => ="mov ^y,^z" A="%^n" 0=0 C="%^c" */
/* "mov %^0,%^c" => ="mov ^y,^z" A="%^n" 0=0 C="%^c" */
/* "mov %^0,%^c" => ="mov ^y,^z" A="%^n" 0=0 C="%^c" */
/* "mov %^0,%^c" => ="mov ^y,^z" A="%^n" 0=0 C="%^c" */
/* "mov %^1,%o1; call .rem,2; mov %^0,%o0; mov %o0,%^c" => ="mov ^x,%o1; call ^f,2; mov ^y,%o0; mov %o0,^z" B="%^n" 1=1 D=".rem" A="%^n" 0=0 C="%^c" */
/* "mov %^1,%o1; call .mul,2; mov %^0,%o0; mov %o0,%^c" => ="mov ^x,%o1; call ^f,2; mov ^y,%o0; mov %o0,^z" B="%^n" 1=1 D=".mul" A="%^n" 0=0 C="%^c" */
/* "mov %^1,%o1; call .div,2; mov %^0,%o0; mov %o0,%^c" => ="mov ^x,%o1; call ^f,2; mov ^y,%o0; mov %o0,^z" B="%^n" 1=1 D=".div" A="%^n" 0=0 C="%^c" */
/* "mov %^1,%o1; call .urem,2; mov %^0,%o0; mov %o0,%^c" => ="mov ^x,%o1; call ^f,2; mov ^y,%o0; mov %o0,^z" B="%^n" 1=1 D=".urem" A="%^n" 0=0 C="%^c" */
/* "mov %^1,%o1; call .umul,2; mov %^0,%o0; mov %o0,%^c" => ="mov ^x,%o1; call ^f,2; mov ^y,%o0; mov %o0,^z" B="%^n" 1=1 D=".umul" A="%^n" 0=0 C="%^c" */
/* "mov %^1,%o1; call .udiv,2; mov %^0,%o0; mov %o0,%^c" => ="mov ^x,%o1; call ^f,2; mov ^y,%o0; mov %o0,^z" B="%^n" 1=1 D=".udiv" A="%^n" 0=0 C="%^c" */
/* "mov %^0,%^c" => ="mov ^y,^z" A="%^n" 0=0 C="%^c" */
/* "ldd [%^0],%f^c" => ="ld^t [^y],^z" E="d" A="%^n" 0=0 C="%f^c" */
/* "ld [%^0],%f^c" => ="ld^t [^y],^z" E="" A="%^n" 0=0 C="%f^c" */
/* "ld [%^0],%^c" => ="ld^t [^y],^z" E="" A="%^n" 0=0 C="%^c" */
/* "ldsh [%^0],%^c" => ="ld^t [^y],^z" E="sh" A="%^n" 0=0 C="%^c" */
/* "ld [%^0],%^c" => ="ld^t [^y],^z" E="" A="%^n" 0=0 C="%^c" */
/* "ldsb [%^0],%^c" => ="ld^t [^y],^z" E="sb" A="%^n" 0=0 C="%^c" */
/* "jmp %^0; nop" => ="jmp ^y; nop" A="%^n" 0=0 */
/* "^0:" => ="^z:" C="^c" c=a */
/* "mov %^0,%^c" => ="mov ^y,^z" A="%^n" 0=0 C="%^c" */
/* "fmovs %f^0,%f^c" => ="fmov^t ^y,^z" E="s" A="%f^n" 0=0 C="%f^c" */
/* "mov %^0,%^c" => ="mov ^y,^z" A="%^n" 0=0 C="%^c" */
/* "mov %^0,%^c" => ="mov ^y,^z" A="%^n" 0=0 C="%^c" */
/* "mov %^0,%^c" => ="mov ^y,^z" A="%^n" 0=0 C="%^c" */
/* "mov %^0,%^c" => ="mov ^y,^z" A="%^n" 0=0 C="%^c" */
/* "mov %^0,%^c" => ="mov ^y,^z" A="%^n" 0=0 C="%^c" */
/* "sll %^0,%^1,%^c" => ="^f ^y,^x,^z" D="sll" A="%^n" 0=0 B="%^n" 1=1 C="%^c" */
/* "sra %^0,%^1,%^c" => ="^f ^y,^x,^z" D="sra" A="%^n" 0=0 B="%^n" 1=1 C="%^c" */
/* "sll %^0,%^1,%^c" => ="^f ^y,^x,^z" D="sll" A="%^n" 0=0 B="%^n" 1=1 C="%^c" */
/* "srl %^0,%^1,%^c" => ="^f ^y,^x,^z" D="srl" A="%^n" 0=0 B="%^n" 1=1 C="%^c" */
/* "fnegs %f^n,%f^c" => ="fnegs ^y,^z" A="%f^n" C="%f^c" */
/* "sub %g0,%^0,%^c" => ="^f ^y,^x,^z" D="sub" A="%g0" B="%^n" 1=0 C="%^c" */
/* "fsubd %f^0,%f^1,%f^c" => ="f^f^t ^y,^x,^z" D="sub" E="d" A="%f^n" 0=0 B="%f^n" 1=1 C="%f^c" */
/* "fsubs %f^0,%f^1,%f^c" => ="f^f^t ^y,^x,^z" D="sub" E="s" A="%f^n" 0=0 B="%f^n" 1=1 C="%f^c" */
/* "fmuld %f^0,%f^1,%f^c" => ="f^f^t ^y,^x,^z" D="mul" E="d" A="%f^n" 0=0 B="%f^n" 1=1 C="%f^c" */
/* "fmuls %f^0,%f^1,%f^c" => ="f^f^t ^y,^x,^z" D="mul" E="s" A="%f^n" 0=0 B="%f^n" 1=1 C="%f^c" */
/* "fdivd %f^0,%f^1,%f^c" => ="f^f^t ^y,^x,^z" D="div" E="d" A="%f^n" 0=0 B="%f^n" 1=1 C="%f^c" */
/* "fdivs %f^0,%f^1,%f^c" => ="f^f^t ^y,^x,^z" D="div" E="s" A="%f^n" 0=0 B="%f^n" 1=1 C="%f^c" */
/* "faddd %f^0,%f^1,%f^c" => ="f^f^t ^y,^x,^z" D="add" E="d" A="%f^n" 0=0 B="%f^n" 1=1 C="%f^c" */
/* "fadds %f^0,%f^1,%f^c" => ="f^f^t ^y,^x,^z" D="add" E="s" A="%f^n" 0=0 B="%f^n" 1=1 C="%f^c" */
/* "sub %^0,%^1,%^c" => ="^f ^y,^x,^z" D="sub" A="%^n" 0=0 B="%^n" 1=1 C="%^c" */
/* "sub %^0,%^1,%^c" => ="^f ^y,^x,^z" D="sub" A="%^n" 0=0 B="%^n" 1=1 C="%^c" */
/* "sub %^0,%^1,%^c" => ="^f ^y,^x,^z" D="sub" A="%^n" 0=0 B="%^n" 1=1 C="%^c" */
/* "xor %^0,%^1,%^c" => ="^f ^y,^x,^z" D="xor" A="%^n" 0=0 B="%^n" 1=1 C="%^c" */
/* "and %^0,%^1,%^c" => ="^f ^y,^x,^z" D="and" A="%^n" 0=0 B="%^n" 1=1 C="%^c" */
/* "add %^0,%^1,%^c" => ="^f ^y,^x,^z" D="add" A="%^n" 0=0 B="%^n" 1=1 C="%^c" */
/* "add %^0,%^1,%^c" => ="^f ^y,^x,^z" D="add" A="%^n" 0=0 B="%^n" 1=1 C="%^c" */
/* "add %^0,%^1,%^c" => ="^f ^y,^x,^z" D="add" A="%^n" 0=0 B="%^n" 1=1 C="%^c" */
/* "or %^0,%^1,%^c" => ="^f ^y,^x,^z" D="or" A="%^n" 0=0 B="%^n" 1=1 C="%^c" */
/* "cmp %^0,%^1; bleu ^0; nop" => ="cmp ^y,^x; b^f ^z; nop" A="%^n" 0=0 B="%^n" 1=1 D="leu" C="^c" c=a */
/* "cmp %^0,%^1; blu ^0; nop" => ="cmp ^y,^x; b^f ^z; nop" A="%^n" 0=0 B="%^n" 1=1 D="lu" C="^c" c=a */
/* "cmp %^0,%^1; bgeu ^0; nop" => ="cmp ^y,^x; b^f ^z; nop" A="%^n" 0=0 B="%^n" 1=1 D="geu" C="^c" c=a */
/* "cmp %^0,%^1; bgu ^0; nop" => ="cmp ^y,^x; b^f ^z; nop" A="%^n" 0=0 B="%^n" 1=1 D="gu" C="^c" c=a */
/* "fcmped %f^0,%f^1; nop; fbne ^0; nop" => ="fcmpe^t ^y,^x; nop; fb^f ^z; nop" E="d" A="%f^n" 0=0 B="%f^n" 1=1 D="ne" C="^c" c=a */
/* "fcmpes %f^0,%f^1; nop; fbne ^0; nop" => ="fcmpe^t ^y,^x; nop; fb^f ^z; nop" E="s" A="%f^n" 0=0 B="%f^n" 1=1 D="ne" C="^c" c=a */
/* "fcmped %f^0,%f^1; nop; fbuge ^0; nop" => ="fcmpe^t ^y,^x; nop; fb^f ^z; nop" E="d" A="%f^n" 0=0 B="%f^n" 1=1 D="uge" C="^c" c=a */
/* "fcmpes %f^0,%f^1; nop; fbuge ^0; nop" => ="fcmpe^t ^y,^x; nop; fb^f ^z; nop" E="s" A="%f^n" 0=0 B="%f^n" 1=1 D="uge" C="^c" c=a */
/* "fcmped %f^0,%f^1; nop; fbug ^0; nop" => ="fcmpe^t ^y,^x; nop; fb^f ^z; nop" E="d" A="%f^n" 0=0 B="%f^n" 1=1 D="ug" C="^c" c=a */
/* "fcmpes %f^0,%f^1; nop; fbug ^0; nop" => ="fcmpe^t ^y,^x; nop; fb^f ^z; nop" E="s" A="%f^n" 0=0 B="%f^n" 1=1 D="ug" C="^c" c=a */
/* "fcmped %f^0,%f^1; nop; fbue ^0; nop" => ="fcmpe^t ^y,^x; nop; fb^f ^z; nop" E="d" A="%f^n" 0=0 B="%f^n" 1=1 D="ue" C="^c" c=a */
/* "fcmpes %f^0,%f^1; nop; fbue ^0; nop" => ="fcmpe^t ^y,^x; nop; fb^f ^z; nop" E="s" A="%f^n" 0=0 B="%f^n" 1=1 D="ue" C="^c" c=a */
/* "fcmped %f^0,%f^1; nop; fbule ^0; nop" => ="fcmpe^t ^y,^x; nop; fb^f ^z; nop" E="d" A="%f^n" 0=0 B="%f^n" 1=1 D="ule" C="^c" c=a */
/* "fcmpes %f^0,%f^1; nop; fbule ^0; nop" => ="fcmpe^t ^y,^x; nop; fb^f ^z; nop" E="s" A="%f^n" 0=0 B="%f^n" 1=1 D="ule" C="^c" c=a */
/* "fcmped %f^0,%f^1; nop; fbul ^0; nop" => ="fcmpe^t ^y,^x; nop; fb^f ^z; nop" E="d" A="%f^n" 0=0 B="%f^n" 1=1 D="ul" C="^c" c=a */
/* "fcmpes %f^0,%f^1; nop; fbul ^0; nop" => ="fcmpe^t ^y,^x; nop; fb^f ^z; nop" E="s" A="%f^n" 0=0 B="%f^n" 1=1 D="ul" C="^c" c=a */
/* "cmp %^0,%^1; bne ^0; nop" => ="cmp ^y,^x; b^f ^z; nop" A="%^n" 0=0 B="%^n" 1=1 D="ne" C="^c" c=a */
/* "cmp %^0,%^1; be ^0; nop" => ="cmp ^y,^x; b^f ^z; nop" A="%^n" 0=0 B="%^n" 1=1 D="e" C="^c" c=a */
/* "cmp %^0,%^1; ble ^0; nop" => ="cmp ^y,^x; b^f ^z; nop" A="%^n" 0=0 B="%^n" 1=1 D="le" C="^c" c=a */
/* "cmp %^0,%^1; ble ^0; nop" => ="cmp ^y,^x; b^f ^z; nop" A="%^n" 0=0 B="%^n" 1=1 D="le" C="^c" c=a */
/* "cmp %^0,%^1; bl ^0; nop" => ="cmp ^y,^x; b^f ^z; nop" A="%^n" 0=0 B="%^n" 1=1 D="l" C="^c" c=a */
/* "cmp %^0,%^1; bl ^0; nop" => ="cmp ^y,^x; b^f ^z; nop" A="%^n" 0=0 B="%^n" 1=1 D="l" C="^c" c=a */
/* "cmp %^0,%^1; bge ^0; nop" => ="cmp ^y,^x; b^f ^z; nop" A="%^n" 0=0 B="%^n" 1=1 D="ge" C="^c" c=a */
/* "cmp %^0,%^1; bge ^0; nop" => ="cmp ^y,^x; b^f ^z; nop" A="%^n" 0=0 B="%^n" 1=1 D="ge" C="^c" c=a */
/* "cmp %^0,%^1; bg ^0; nop" => ="cmp ^y,^x; b^f ^z; nop" A="%^n" 0=0 B="%^n" 1=1 D="g" C="^c" c=a */
/* "cmp %^0,%^1; bg ^0; nop" => ="cmp ^y,^x; b^f ^z; nop" A="%^n" 0=0 B="%^n" 1=1 D="g" C="^c" c=a */
/* "call %^n; nop" => ="call ^y; nop" A="%^n" */
/* "set _^c,^z" => ="set ^y,^z" 0A="_^c" */
/* "call %^n; st ^z,[%sp+64]" => ="call ^y; st ^z,[%sp+64]" A="%^n" */
/* "set _^c,^z" => ="set ^y,^z" 0A="_^c" */
/* "cmp ^y,%^n; b^f ^z; nop" => ="cmp ^y,^x; b^f ^z; nop" B="%^n" */
/* "set ^c,^z" => ="set ^y,^z" 1A="^c" */
/* "cmp ^y,%^n; b^f ^z; nop" => ="cmp ^y,^x; b^f ^z; nop" B="%^n" */
/* "mov %^n,^z" => ="mov ^y,^z" 1A="%^n" */
/* "cmp %^n,^x; b^f ^z; nop" => ="cmp ^y,^x; b^f ^z; nop" A="%^n" */
/* "mov %^n,^z" => ="mov ^y,^z" 0A="%^n" */
/* "^f ^y,%^n,^z" => ="^f ^y,^x,^z" B="%^n" */
/* "set ^c,^z" => ="set ^y,^z" 1A="^c" */
/* "^f ^y,%^n,^z" => ="^f ^y,^x,^z" B="%^n" */
/* "mov %^n,^z" => ="mov ^y,^z" 1A="%^n" */
/* "^f %^n,^x,^z" => ="^f ^y,^x,^z" A="%^n" */
/* "mov %^n,^z" => ="mov ^y,^z" 0A="%^n" */
/* "jmp %^n; nop" => ="jmp ^y; nop" A="%^n" */
/* "set _^c,^z" => ="set ^y,^z" 0A="_^c" */
/* "ba ^c; nop" => ="ba ^y; nop" A="^c" */
/* "ld^t [%^n],^z" => ="ld^t [^y],^z" A="%^n" */
/* "add ^c,%fp,^z" => ="^f ^y,^x,^z" 0D="add" 0A="^c" 0B="%fp" */
/* "ld^t [%fp+^c],^z" => ="ld^t [^y],^z" A="%fp+^c" */
/* "ld^t [%^n],^z" => ="ld^t [^y],^z" A="%^n" */
/* "set ^c,%g1; ld^t [%fp+%g1],^z" => ="set ^y,%g1; ld^t [%fp+%g1],^z" A="^c" */
/* "ld^t [%^n],^z" => ="ld^t [^y],^z" A="%^n" */
/* "add %^n,^c,^z" => ="^f ^y,^x,^z" 0D="add" 0A="%^n" 0B="^c" */
/* "ld^t [%^n+^c],^z" => ="ld^t [^y],^z" A="%^n+^c" */
/* "ld{sb|sh|ub|uh|""|s} [%^n],^z" => ="ld^t [^y],^z" E=@TMP717 A="%^n" */
/* "add %^n,%^n,^z" => ="^f ^y,^x,^z" 0D="add" 0A="%^n" 0B="%^n" */
/* "ld^t [%^n+%^i],^z" => ="ld^t [^y],^z" A="%^n+%^i" */
/* "ld^t [%^n],^z" => ="ld^t [^y],^z" A="%^n" */
/* "mov %^n,^z" => ="mov ^y,^z" 0A="%^n" */
/* "ld^t [%fp+^c],^z" => ="ld^t [^y],^z" A="%fp+^c" */
/* "mov %^c,%^c" => ="mov ^y,^z" A="%^c" C="%^c" */
/* "mov %^c,%^c" => ="mov ^y,^z" A="%^c" C="%^c" */
/* "mov %^n,^z" => ="mov ^y,^z" A="%^n" */
/* "mov %^n,^z" => ="mov ^y,^z" A="%^n" */
/* "mov %^n,^z" => ="mov ^y,^z" A="%^n" */
/* "mov %^n,^z" => ="mov ^y,^z" A="%^n" */
/* "mov %^n,^z" => ="mov ^y,^z" A="%^n" */
/* "mov %^n,^z" => ="mov ^y,^z" 0A="%^n" */
/* "set 0,^z" => ="set ^y,^z" A="^c" a="0" */
/* "set ^y,%g1; add %fp,%g1,%^c" => ="set ^y,%g1; add %fp,%g1,^z" C="%^c" */
/* "add ^y,%fp,^z" => ="^f ^y,^x,^z" D="add" B="%fp" */
/* "sll %^n,16,%g1; sra %g1,16,^z" => ="sll ^y,^x,%g1; sra %g1,^x,^z" A="%^n" B="^c" b="16" B="^c" b="16" */
/* "ldsh [^y],^z" => ="ld^t [^y],^z" 0E="sh" */
/* "ldsh [^y],^z" => ="ld^t [^y],^z" E="sh" */
/* "sll %^n,24,%g1; sra %g1,24,^z" => ="sll ^y,^x,%g1; sra %g1,^x,^z" A="%^n" B="^c" b="24" B="^c" b="24" */
/* "ldsb [^y],^z" => ="ld^t [^y],^z" 0E="sb" */
/* "ldsb [^y],^z" => ="ld^t [^y],^z" E="sb" */
/* "and %^n,0xff,^z" => ="^f ^y,^x,^z" D="and" A="%^n" B="^c" b="0xff" */
/* "ldsb [^y],^z" => ="ld^t [^y],^z" 0E="sb" */
/* "ldub [^y],^z" => ="ld^t [^y],^z" E="ub" */
/* "set 0xffff,%g1; and %^n,%g1,^z" => ="set 0xffff,%g1; and ^y,%g1,^z" A="%^n" */
/* "ldsh [^y],^z" => ="ld^t [^y],^z" 0E="sh" */
/* "lduh [^y],^z" => ="ld^t [^y],^z" E="uh" */
/* "st^t %^n,[%sp+4*^c+68]" => ="st^t ^y,[^z]" A="%^n" C="%sp+4*^c+68" */
/* "mov %^n,%^c" => ="mov ^y,^z" A="%^n" C="%^c" */
/* "st^t ^y,[%^n]" => ="st^t ^y,[^z]" C="%^n" */
/* "add ^c,%fp,^z" => ="^f ^y,^x,^z" 2D="add" 2A="^c" 2B="%fp" */
/* "st^t ^y,[%fp+^c]" => ="st^t ^y,[^z]" C="%fp+^c" */
/* "st^t ^y,[%^n]" => ="st^t ^y,[^z]" C="%^n" */
/* "set ^c,%g1; st^t ^y,[%fp+%g1]" => ="set ^z,%g1; st^t ^y,[%fp+%g1]" C="^c" */
/* "st^t ^y,[%^n]" => ="st^t ^y,[^z]" C="%^n" */
/* "add %^n,^c,^z" => ="^f ^y,^x,^z" 2D="add" 2A="%^n" 2B="^c" */
/* "st^t ^y,[%^n+^c]" => ="st^t ^y,[^z]" C="%^n+^c" */
/* "st{b|h|""|s} ^y,[%^n]" => ="st^t ^y,[^z]" E=@TMP851 C="%^n" */
/* "add %^n,%^n,^z" => ="^f ^y,^x,^z" 2D="add" 2A="%^n" 2B="%^n" */
/* "st^t ^y,[%^n+%^i]" => ="st^t ^y,[^z]" C="%^n+%^i" */
/* "st^t ^y,[%^n]" => ="st^t ^y,[^z]" C="%^n" */
/* "st^t %^n,[^z]" => ="st^t ^y,[^z]" A="%^n" */
/* "st^t ^y,[%fp+^c]" => ="st^t ^y,[^z]" C="%fp+^c" */
/* "mov ^y,%^c" => ="mov ^y,^z" C="%^c" */
/* "ldd [^y],^z" => ="ld^t [^y],^z" E="d" */
/* "ld [^y],^z; ld [^y+4],%f^c" => ="ld [^y],^z; ld [^y+4],^x" B="%f^c" */
/* "mov %^n,%^c" => ="mov ^y,^z" A="%^n" C="%^c" */
/* "st %f^n,[%sp+4*^c+68]" => ="st^t ^y,[^z]" E="" A="%f^n" C="%sp+4*^c+68" */
/* "st %f^n,[%sp+4*^c+68]; ld [%sp+4*^c+68],%o^c" => ="st ^y,[%sp+4*^z+68]; ld [%sp+4*^z+68],%o^x" A="%f^n" C="^c" C="^c" B="^c" */
/* "std %f^n,[%sp+4*^c+68]" => ="st^t ^y,[^z]" E="d" A="%f^n" C="%sp+4*^c+68" */
/* "std ^y,[^z]" => ="st^t ^y,[^z]" E="d" */
/* "st ^y,[^z]; st %f^c,[^z+4]" => ="st ^y,[^z]; st ^x,[^z+4]" B="%f^c" */
/* "set _^c,^z" => ="set ^y,^z" A="_^c" */
/* "call _^c; nop" => ="call ^y; nop" A="_^c" */
/* "call _^c; st ^z,[%sp+64]" => ="call ^y; st ^z,[%sp+64]" A="_^c" */
#include "pseudos.c"

static char *shortopcodes[] = {
/* 608 */  "mov ^x,%o1; call ^f,2; mov ^y,%o0; mov %o0,^z",
/* 609 */  "call ^y; nop",
/* 610 */  "call ^y; st ^z,[%sp+64]",
/* 611 */  "fdtoi ^y,%f0; st %f0,[%sp+64]; ld [%sp+64],^z",
/* 612 */  "sll ^y,^x,%g1; sra %g1,^x,^z",
/* 613 */  "^z:",
/* 614 */  "nop",
/* 615 */  "# argd",
/* 616 */  "# movd",
/* 617 */  "# negd",
/* 618 */  "# asgnb",
/* 619 */  "# reg",
/* 620 */  "# ret",
/* 621 */  "# label",
/* 622 */  "^f ^y,^x,^z",
/* 623 */  "f^f^t ^y,^x,^z",
/* 624 */  "call ^y,2; nop",
/* 625 */  "cmp ^y,^x; b^f ^z; nop",
/* 626 */  "fcmpe^t ^y,^x; nop; fb^f ^z; nop",
/* 627 */  "fstod ^y,^z",
/* 628 */  "fdtos ^y,^z",
/* 629 */  "fnegs ^y,^z",
/* 630 */  "fmovd ^y,^z; fnegs ^z,^z",
/* 631 */  "st ^y,[%sp+64]; ld [%sp+64],^z; fitod ^z,^z",
/* 632 */  "jmp ^y; nop",
/* 633 */  "ba ^y; nop",
/* 634 */  "ld^t [^y],^z",
/* 635 */  "ld [^y],^z; ld [^y+4],^x",
/* 636 */  "mov ^y,^z",
/* 637 */  "fmov^t ^y,^z",
/* 638 */  "set ^y,^z",
/* 639 */  "set 0xffff,%g1; and ^y,%g1,^z",
/* 640 */  "set ^z,%g1; st^t ^y,[%fp+%g1]",
/* 641 */  "set ^y,%g1; ld^t [%fp+%g1],^z",
/* 642 */  "st^t ^y,[^z]",
/* 643 */  "st ^y,[^z]",
/* 644 */  "st ^y,[^z]; st ^x,[^z+4]",
/* 645 */  "st ^y,[%sp+4*^z+68]; ld [%sp+4*^z+68],%o^x",
/* 646 */  "set ^y,%g1; add %fp,%g1,^z",
/* 647 */  "call ^f,2; nop",
};

static char *asmstr0[] = {
	"(DUMMY)",
	"%^n",
	"%^c",
	"%fp",
	"%sp",
	"%g0",
	"%f^n",
	"%f^c",
	"%^n+%^i",
	"%^c+%^i",
	"%fp+%^i",
	"%sp+%^i",
	"%g0+%^i",
	"%f^n+%^i",
	"%f^c+%^i",
	"%^n+^c",
	"%^n+_^c",
	"%^n+4*^c+68",
	"%^n-^c",
	"%^n-_^c",
	"%^n-4*^c+68",
	"%^c+^c",
	"%^c+_^c",
	"%^c+4*^c+68",
	"%^c-^c",
	"%^c-_^c",
	"%^c-4*^c+68",
	"%fp+^c",
	"%fp+_^c",
	"%fp+4*^c+68",
	"%fp-^c",
	"%fp-_^c",
	"%fp-4*^c+68",
	"%sp+^c",
	"%sp+_^c",
	"%sp+4*^c+68",
	"%sp-^c",
	"%sp-_^c",
	"%sp-4*^c+68",
	"%g0+^c",
	"%g0+_^c",
	"%g0+4*^c+68",
	"%g0-^c",
	"%g0-_^c",
	"%g0-4*^c+68",
	"%f^n+^c",
	"%f^n+_^c",
	"%f^n+4*^c+68",
	"%f^n-^c",
	"%f^n-_^c",
	"%f^n-4*^c+68",
	"%f^c+^c",
	"%f^c+_^c",
	"%f^c+4*^c+68",
	"%f^c-^c",
	"%f^c-_^c",
	"%f^c-4*^c+68",
	"^c",
	"_^c",
	"4*^c+68",
};

static char *asmstr3[] = {
	"(DUMMY)",
	"add",
	"and",
	"div",
	"mul",
	"or",
	"sll",
	"sra",
	"srl",
	"sub",
	"xor",
	"xnor",
	".udiv",
	".umul",
	".urem",
	".div",
	".mul",
	".rem",
	"e",
	"ne",
	"gu",
	"g",
	"geu",
	"ge",
	"lu",
	"l",
	"leu",
	"le",
	"ug",
	"uge",
	"ul",
	"ule",
	"ue",
};

static char *asmstr4[] = {
	"(DUMMY)",
	"sb",
	"sh",
	"ub",
	"uh",
	"b",
	"h",
	"s",
	"d",
	"",
};

static struct symbol sym[] = {
{{"(DUMMY)"}},
{{"0"}},
{{"0xff"}},
{{"0xffff"}},
{{"16"}},
{{"24"}},
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
	sym[2].x.name = string("0xff");
	sym[3].x.name = string("0xffff");
	sym[4].x.name = string("16");
	sym[5].x.name = string("24");
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
	case 1:   /* %^n */
		*rbp++ = '%';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 2:   /* %^c */
		*rbp++ = '%';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 3:   /* %fp */
		*rbp++ = '%';
		*rbp++ = 'f';
		*rbp++ = 'p';
		return rbp;
	case 4:   /* %sp */
		*rbp++ = '%';
		*rbp++ = 's';
		*rbp++ = 'p';
		return rbp;
	case 5:   /* %g0 */
		*rbp++ = '%';
		*rbp++ = 'g';
		*rbp++ = '0';
		return rbp;
	case 6:   /* %f^n */
		*rbp++ = '%';
		*rbp++ = 'f';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 7:   /* %f^c */
		*rbp++ = '%';
		*rbp++ = 'f';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 8:   /* %^n+%^i */
		*rbp++ = '%';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '%';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 9:   /* %^c+%^i */
		*rbp++ = '%';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '%';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 10:   /* %fp+%^i */
		*rbp++ = '%';
		*rbp++ = 'f';
		*rbp++ = 'p';
		*rbp++ = '+';
		*rbp++ = '%';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 11:   /* %sp+%^i */
		*rbp++ = '%';
		*rbp++ = 's';
		*rbp++ = 'p';
		*rbp++ = '+';
		*rbp++ = '%';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 12:   /* %g0+%^i */
		*rbp++ = '%';
		*rbp++ = 'g';
		*rbp++ = '0';
		*rbp++ = '+';
		*rbp++ = '%';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 13:   /* %f^n+%^i */
		*rbp++ = '%';
		*rbp++ = 'f';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '%';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 14:   /* %f^c+%^i */
		*rbp++ = '%';
		*rbp++ = 'f';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '%';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 15:   /* %^n+^c */
		*rbp++ = '%';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 16:   /* %^n+_^c */
		*rbp++ = '%';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 17:   /* %^n+4*^c+68 */
		*rbp++ = '%';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '4';
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '6';
		*rbp++ = '8';
		return rbp;
	case 18:   /* %^n-^c */
		*rbp++ = '%';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '-';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 19:   /* %^n-_^c */
		*rbp++ = '%';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '-';
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 20:   /* %^n-4*^c+68 */
		*rbp++ = '%';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '-';
		*rbp++ = '4';
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '6';
		*rbp++ = '8';
		return rbp;
	case 21:   /* %^c+^c */
		*rbp++ = '%';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 22:   /* %^c+_^c */
		*rbp++ = '%';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 23:   /* %^c+4*^c+68 */
		*rbp++ = '%';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '4';
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '6';
		*rbp++ = '8';
		return rbp;
	case 24:   /* %^c-^c */
		*rbp++ = '%';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '-';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 25:   /* %^c-_^c */
		*rbp++ = '%';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '-';
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 26:   /* %^c-4*^c+68 */
		*rbp++ = '%';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '-';
		*rbp++ = '4';
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '6';
		*rbp++ = '8';
		return rbp;
	case 27:   /* %fp+^c */
		*rbp++ = '%';
		*rbp++ = 'f';
		*rbp++ = 'p';
		*rbp++ = '+';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 28:   /* %fp+_^c */
		*rbp++ = '%';
		*rbp++ = 'f';
		*rbp++ = 'p';
		*rbp++ = '+';
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 29:   /* %fp+4*^c+68 */
		*rbp++ = '%';
		*rbp++ = 'f';
		*rbp++ = 'p';
		*rbp++ = '+';
		*rbp++ = '4';
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '6';
		*rbp++ = '8';
		return rbp;
	case 30:   /* %fp-^c */
		*rbp++ = '%';
		*rbp++ = 'f';
		*rbp++ = 'p';
		*rbp++ = '-';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 31:   /* %fp-_^c */
		*rbp++ = '%';
		*rbp++ = 'f';
		*rbp++ = 'p';
		*rbp++ = '-';
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 32:   /* %fp-4*^c+68 */
		*rbp++ = '%';
		*rbp++ = 'f';
		*rbp++ = 'p';
		*rbp++ = '-';
		*rbp++ = '4';
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '6';
		*rbp++ = '8';
		return rbp;
	case 33:   /* %sp+^c */
		*rbp++ = '%';
		*rbp++ = 's';
		*rbp++ = 'p';
		*rbp++ = '+';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 34:   /* %sp+_^c */
		*rbp++ = '%';
		*rbp++ = 's';
		*rbp++ = 'p';
		*rbp++ = '+';
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 35:   /* %sp+4*^c+68 */
		*rbp++ = '%';
		*rbp++ = 's';
		*rbp++ = 'p';
		*rbp++ = '+';
		*rbp++ = '4';
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '6';
		*rbp++ = '8';
		return rbp;
	case 36:   /* %sp-^c */
		*rbp++ = '%';
		*rbp++ = 's';
		*rbp++ = 'p';
		*rbp++ = '-';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 37:   /* %sp-_^c */
		*rbp++ = '%';
		*rbp++ = 's';
		*rbp++ = 'p';
		*rbp++ = '-';
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 38:   /* %sp-4*^c+68 */
		*rbp++ = '%';
		*rbp++ = 's';
		*rbp++ = 'p';
		*rbp++ = '-';
		*rbp++ = '4';
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '6';
		*rbp++ = '8';
		return rbp;
	case 39:   /* %g0+^c */
		*rbp++ = '%';
		*rbp++ = 'g';
		*rbp++ = '0';
		*rbp++ = '+';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 40:   /* %g0+_^c */
		*rbp++ = '%';
		*rbp++ = 'g';
		*rbp++ = '0';
		*rbp++ = '+';
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 41:   /* %g0+4*^c+68 */
		*rbp++ = '%';
		*rbp++ = 'g';
		*rbp++ = '0';
		*rbp++ = '+';
		*rbp++ = '4';
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '6';
		*rbp++ = '8';
		return rbp;
	case 42:   /* %g0-^c */
		*rbp++ = '%';
		*rbp++ = 'g';
		*rbp++ = '0';
		*rbp++ = '-';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 43:   /* %g0-_^c */
		*rbp++ = '%';
		*rbp++ = 'g';
		*rbp++ = '0';
		*rbp++ = '-';
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 44:   /* %g0-4*^c+68 */
		*rbp++ = '%';
		*rbp++ = 'g';
		*rbp++ = '0';
		*rbp++ = '-';
		*rbp++ = '4';
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '6';
		*rbp++ = '8';
		return rbp;
	case 45:   /* %f^n+^c */
		*rbp++ = '%';
		*rbp++ = 'f';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 46:   /* %f^n+_^c */
		*rbp++ = '%';
		*rbp++ = 'f';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 47:   /* %f^n+4*^c+68 */
		*rbp++ = '%';
		*rbp++ = 'f';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '4';
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '6';
		*rbp++ = '8';
		return rbp;
	case 48:   /* %f^n-^c */
		*rbp++ = '%';
		*rbp++ = 'f';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '-';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 49:   /* %f^n-_^c */
		*rbp++ = '%';
		*rbp++ = 'f';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '-';
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 50:   /* %f^n-4*^c+68 */
		*rbp++ = '%';
		*rbp++ = 'f';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '-';
		*rbp++ = '4';
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '6';
		*rbp++ = '8';
		return rbp;
	case 51:   /* %f^c+^c */
		*rbp++ = '%';
		*rbp++ = 'f';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 52:   /* %f^c+_^c */
		*rbp++ = '%';
		*rbp++ = 'f';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 53:   /* %f^c+4*^c+68 */
		*rbp++ = '%';
		*rbp++ = 'f';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '4';
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '6';
		*rbp++ = '8';
		return rbp;
	case 54:   /* %f^c-^c */
		*rbp++ = '%';
		*rbp++ = 'f';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '-';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 55:   /* %f^c-_^c */
		*rbp++ = '%';
		*rbp++ = 'f';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '-';
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 56:   /* %f^c-4*^c+68 */
		*rbp++ = '%';
		*rbp++ = 'f';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '-';
		*rbp++ = '4';
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '6';
		*rbp++ = '8';
		return rbp;
	case 57:   /* ^c */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 58:   /* _^c */
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 59:   /* 4*^c+68 */
		*rbp++ = '4';
		*rbp++ = '*';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '+';
		*rbp++ = '6';
		*rbp++ = '8';
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
		case 608: L608:   /* mov ^x,%o1; call ^f,2; mov ^y,%o0; mov %o0,^z */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			*rbp++ = '%';
			*rbp++ = 'o';
			*rbp++ = '1';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'c';
			*rbp++ = 'a';
			*rbp++ = 'l';
			*rbp++ = 'l';
			*rbp++ = ' ';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			*rbp++ = '2';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = '%';
			*rbp++ = 'o';
			*rbp++ = '0';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = ' ';
			*rbp++ = '%';
			*rbp++ = 'o';
			*rbp++ = '0';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 609: L609:   /* call ^y; nop */
			rbp = bp;
			*rbp++ = 'c';
			*rbp++ = 'a';
			*rbp++ = 'l';
			*rbp++ = 'l';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'n';
			*rbp++ = 'o';
			*rbp++ = 'p';
			bp = rbp;
			outs("\n");
			break;
		case 610: L610:   /* call ^y; st ^z,[%sp+64] */
			rbp = bp;
			*rbp++ = 'c';
			*rbp++ = 'a';
			*rbp++ = 'l';
			*rbp++ = 'l';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 's';
			*rbp++ = 't';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			*rbp++ = ',';
			*rbp++ = '[';
			*rbp++ = '%';
			*rbp++ = 's';
			*rbp++ = 'p';
			*rbp++ = '+';
			*rbp++ = '6';
			*rbp++ = '4';
			*rbp++ = ']';
			bp = rbp;
			outs("\n");
			break;
		case 611: L611:   /* fdtoi ^y,%f0; st %f0,[%sp+64]; ld [%sp+64],^z */
			rbp = bp;
			*rbp++ = 'f';
			*rbp++ = 'd';
			*rbp++ = 't';
			*rbp++ = 'o';
			*rbp++ = 'i';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = '%';
			*rbp++ = 'f';
			*rbp++ = '0';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 's';
			*rbp++ = 't';
			*rbp++ = ' ';
			*rbp++ = '%';
			*rbp++ = 'f';
			*rbp++ = '0';
			*rbp++ = ',';
			*rbp++ = '[';
			*rbp++ = '%';
			*rbp++ = 's';
			*rbp++ = 'p';
			*rbp++ = '+';
			*rbp++ = '6';
			*rbp++ = '4';
			*rbp++ = ']';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'l';
			*rbp++ = 'd';
			*rbp++ = ' ';
			*rbp++ = '[';
			*rbp++ = '%';
			*rbp++ = 's';
			*rbp++ = 'p';
			*rbp++ = '+';
			*rbp++ = '6';
			*rbp++ = '4';
			*rbp++ = ']';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 612: L612:   /* sll ^y,^x,%g1; sra %g1,^x,^z */
			rbp = bp;
			*rbp++ = 's';
			*rbp++ = 'l';
			*rbp++ = 'l';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			*rbp++ = '%';
			*rbp++ = 'g';
			*rbp++ = '1';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 's';
			*rbp++ = 'r';
			*rbp++ = 'a';
			*rbp++ = ' ';
			*rbp++ = '%';
			*rbp++ = 'g';
			*rbp++ = '1';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 613: L613:   /* ^z: */
			rbp = bp;
			rbp = emit0(rbp, a, 2);
			*rbp++ = ':';
			bp = rbp;
			outs("\n");
			break;
		case 614: L614:   /* nop */
			rbp = bp;
			*rbp++ = 'n';
			*rbp++ = 'o';
			*rbp++ = 'p';
			bp = rbp;
			outs("\n");
			break;
		case 615: L615:   /* # argd */
			(*IR->x.emitspecial)(a);
			break;
		case 616: L616:   /* # movd */
			(*IR->x.emitspecial)(a);
			break;
		case 617: L617:   /* # negd */
			(*IR->x.emitspecial)(a);
			break;
		case 618: L618:   /* # asgnb */
			(*IR->x.emitspecial)(a);
			break;
		case 619: L619:   /* # reg */
			(*IR->x.emitspecial)(a);
			break;
		case 620: L620:   /* # ret */
			(*IR->x.emitspecial)(a);
			break;
		case 621: L621:   /* # label */
			(*IR->x.emitspecial)(a);
			break;
		case 622: L622:   /* ^f ^y,^x,^z */
			rbp = bp;
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 623: L623:   /* f^f^t ^y,^x,^z */
			rbp = bp;
			*rbp++ = 'f';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			assert(a->x.ints[4]);
			for (cp = asmstr4[a->x.ints[4]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 624: L624:   /* call ^y,2; nop */
			rbp = bp;
			*rbp++ = 'c';
			*rbp++ = 'a';
			*rbp++ = 'l';
			*rbp++ = 'l';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = '2';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'n';
			*rbp++ = 'o';
			*rbp++ = 'p';
			bp = rbp;
			outs("\n");
			break;
		case 625: L625:   /* cmp ^y,^x; b^f ^z; nop */
			rbp = bp;
			*rbp++ = 'c';
			*rbp++ = 'm';
			*rbp++ = 'p';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'b';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'n';
			*rbp++ = 'o';
			*rbp++ = 'p';
			bp = rbp;
			outs("\n");
			break;
		case 626: L626:   /* fcmpe^t ^y,^x; nop; fb^f ^z; nop */
			rbp = bp;
			*rbp++ = 'f';
			*rbp++ = 'c';
			*rbp++ = 'm';
			*rbp++ = 'p';
			*rbp++ = 'e';
			assert(a->x.ints[4]);
			for (cp = asmstr4[a->x.ints[4]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'n';
			*rbp++ = 'o';
			*rbp++ = 'p';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'f';
			*rbp++ = 'b';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'n';
			*rbp++ = 'o';
			*rbp++ = 'p';
			bp = rbp;
			outs("\n");
			break;
		case 627: L627:   /* fstod ^y,^z */
			rbp = bp;
			*rbp++ = 'f';
			*rbp++ = 's';
			*rbp++ = 't';
			*rbp++ = 'o';
			*rbp++ = 'd';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 628: L628:   /* fdtos ^y,^z */
			rbp = bp;
			*rbp++ = 'f';
			*rbp++ = 'd';
			*rbp++ = 't';
			*rbp++ = 'o';
			*rbp++ = 's';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 629: L629:   /* fnegs ^y,^z */
			rbp = bp;
			*rbp++ = 'f';
			*rbp++ = 'n';
			*rbp++ = 'e';
			*rbp++ = 'g';
			*rbp++ = 's';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 630: L630:   /* fmovd ^y,^z; fnegs ^z,^z */
			rbp = bp;
			*rbp++ = 'f';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'd';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'f';
			*rbp++ = 'n';
			*rbp++ = 'e';
			*rbp++ = 'g';
			*rbp++ = 's';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 631: L631:   /* st ^y,[%sp+64]; ld [%sp+64],^z; fitod ^z,^z */
			rbp = bp;
			*rbp++ = 's';
			*rbp++ = 't';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = '[';
			*rbp++ = '%';
			*rbp++ = 's';
			*rbp++ = 'p';
			*rbp++ = '+';
			*rbp++ = '6';
			*rbp++ = '4';
			*rbp++ = ']';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'l';
			*rbp++ = 'd';
			*rbp++ = ' ';
			*rbp++ = '[';
			*rbp++ = '%';
			*rbp++ = 's';
			*rbp++ = 'p';
			*rbp++ = '+';
			*rbp++ = '6';
			*rbp++ = '4';
			*rbp++ = ']';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'f';
			*rbp++ = 'i';
			*rbp++ = 't';
			*rbp++ = 'o';
			*rbp++ = 'd';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 632: L632:   /* jmp ^y; nop */
			rbp = bp;
			*rbp++ = 'j';
			*rbp++ = 'm';
			*rbp++ = 'p';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'n';
			*rbp++ = 'o';
			*rbp++ = 'p';
			bp = rbp;
			outs("\n");
			break;
		case 633: L633:   /* ba ^y; nop */
			rbp = bp;
			*rbp++ = 'b';
			*rbp++ = 'a';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'n';
			*rbp++ = 'o';
			*rbp++ = 'p';
			bp = rbp;
			outs("\n");
			break;
		case 634: L634:   /* ld^t [^y],^z */
			rbp = bp;
			*rbp++ = 'l';
			*rbp++ = 'd';
			assert(a->x.ints[4]);
			for (cp = asmstr4[a->x.ints[4]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			*rbp++ = '[';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ']';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 635: L635:   /* ld [^y],^z; ld [^y+4],^x */
			rbp = bp;
			*rbp++ = 'l';
			*rbp++ = 'd';
			*rbp++ = ' ';
			*rbp++ = '[';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ']';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'l';
			*rbp++ = 'd';
			*rbp++ = ' ';
			*rbp++ = '[';
			rbp = emit0(rbp, a, 0);
			*rbp++ = '+';
			*rbp++ = '4';
			*rbp++ = ']';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			bp = rbp;
			outs("\n");
			break;
		case 636: L636:   /* mov ^y,^z */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 637: L637:   /* fmov^t ^y,^z */
			rbp = bp;
			*rbp++ = 'f';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			assert(a->x.ints[4]);
			for (cp = asmstr4[a->x.ints[4]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 638: L638:   /* set ^y,^z */
			rbp = bp;
			*rbp++ = 's';
			*rbp++ = 'e';
			*rbp++ = 't';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 639: L639:   /* set 0xffff,%g1; and ^y,%g1,^z */
			rbp = bp;
			*rbp++ = 's';
			*rbp++ = 'e';
			*rbp++ = 't';
			*rbp++ = ' ';
			*rbp++ = '0';
			*rbp++ = 'x';
			*rbp++ = 'f';
			*rbp++ = 'f';
			*rbp++ = 'f';
			*rbp++ = 'f';
			*rbp++ = ',';
			*rbp++ = '%';
			*rbp++ = 'g';
			*rbp++ = '1';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'a';
			*rbp++ = 'n';
			*rbp++ = 'd';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = '%';
			*rbp++ = 'g';
			*rbp++ = '1';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 640: L640:   /* set ^z,%g1; st^t ^y,[%fp+%g1] */
			rbp = bp;
			*rbp++ = 's';
			*rbp++ = 'e';
			*rbp++ = 't';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			*rbp++ = ',';
			*rbp++ = '%';
			*rbp++ = 'g';
			*rbp++ = '1';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 's';
			*rbp++ = 't';
			assert(a->x.ints[4]);
			for (cp = asmstr4[a->x.ints[4]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = '[';
			*rbp++ = '%';
			*rbp++ = 'f';
			*rbp++ = 'p';
			*rbp++ = '+';
			*rbp++ = '%';
			*rbp++ = 'g';
			*rbp++ = '1';
			*rbp++ = ']';
			bp = rbp;
			outs("\n");
			break;
		case 641: L641:   /* set ^y,%g1; ld^t [%fp+%g1],^z */
			rbp = bp;
			*rbp++ = 's';
			*rbp++ = 'e';
			*rbp++ = 't';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = '%';
			*rbp++ = 'g';
			*rbp++ = '1';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'l';
			*rbp++ = 'd';
			assert(a->x.ints[4]);
			for (cp = asmstr4[a->x.ints[4]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			*rbp++ = '[';
			*rbp++ = '%';
			*rbp++ = 'f';
			*rbp++ = 'p';
			*rbp++ = '+';
			*rbp++ = '%';
			*rbp++ = 'g';
			*rbp++ = '1';
			*rbp++ = ']';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 642: L642:   /* st^t ^y,[^z] */
			rbp = bp;
			*rbp++ = 's';
			*rbp++ = 't';
			assert(a->x.ints[4]);
			for (cp = asmstr4[a->x.ints[4]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = '[';
			rbp = emit0(rbp, a, 2);
			*rbp++ = ']';
			bp = rbp;
			outs("\n");
			break;
		case 643: L643:   /* st ^y,[^z] */
			rbp = bp;
			*rbp++ = 's';
			*rbp++ = 't';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = '[';
			rbp = emit0(rbp, a, 2);
			*rbp++ = ']';
			bp = rbp;
			outs("\n");
			break;
		case 644: L644:   /* st ^y,[^z]; st ^x,[^z+4] */
			rbp = bp;
			*rbp++ = 's';
			*rbp++ = 't';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = '[';
			rbp = emit0(rbp, a, 2);
			*rbp++ = ']';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 's';
			*rbp++ = 't';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			*rbp++ = '[';
			rbp = emit0(rbp, a, 2);
			*rbp++ = '+';
			*rbp++ = '4';
			*rbp++ = ']';
			bp = rbp;
			outs("\n");
			break;
		case 645: L645:   /* st ^y,[%sp+4*^z+68]; ld [%sp+4*^z+68],%o^x */
			rbp = bp;
			*rbp++ = 's';
			*rbp++ = 't';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = '[';
			*rbp++ = '%';
			*rbp++ = 's';
			*rbp++ = 'p';
			*rbp++ = '+';
			*rbp++ = '4';
			*rbp++ = '*';
			rbp = emit0(rbp, a, 2);
			*rbp++ = '+';
			*rbp++ = '6';
			*rbp++ = '8';
			*rbp++ = ']';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'l';
			*rbp++ = 'd';
			*rbp++ = ' ';
			*rbp++ = '[';
			*rbp++ = '%';
			*rbp++ = 's';
			*rbp++ = 'p';
			*rbp++ = '+';
			*rbp++ = '4';
			*rbp++ = '*';
			rbp = emit0(rbp, a, 2);
			*rbp++ = '+';
			*rbp++ = '6';
			*rbp++ = '8';
			*rbp++ = ']';
			*rbp++ = ',';
			*rbp++ = '%';
			*rbp++ = 'o';
			rbp = emit0(rbp, a, 1);
			bp = rbp;
			outs("\n");
			break;
		case 646: L646:   /* set ^y,%g1; add %fp,%g1,^z */
			rbp = bp;
			*rbp++ = 's';
			*rbp++ = 'e';
			*rbp++ = 't';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = '%';
			*rbp++ = 'g';
			*rbp++ = '1';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'a';
			*rbp++ = 'd';
			*rbp++ = 'd';
			*rbp++ = ' ';
			*rbp++ = '%';
			*rbp++ = 'f';
			*rbp++ = 'p';
			*rbp++ = ',';
			*rbp++ = '%';
			*rbp++ = 'g';
			*rbp++ = '1';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 647: L647:   /* call ^f,2; nop */
			rbp = bp;
			*rbp++ = 'c';
			*rbp++ = 'a';
			*rbp++ = 'l';
			*rbp++ = 'l';
			*rbp++ = ' ';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			*rbp++ = '2';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'n';
			*rbp++ = 'o';
			*rbp++ = 'p';
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
	case 608: L608:   /* mov ^x,%o1; call ^f,2; mov ^y,%o0; mov %o0,^z */
		dumptree(a, 608);
		spill(0x0000ff00, IREG, a);  
		a->op = 608;
		break;
	case 609: L609:   /* call ^y; nop */
		dumptree(a, 609);
		if (
		a->x.ints[0] == 58  /* _^c */
		) {
			a->x.ints[0] = 57;  /* ^c */
			goto L609;   /* call ^y; nop */
		}
		spill(0xfffffffc, FREG, a);  
		a->op = 609;
		break;
	case 610: L610:   /* call ^y; st ^z,[%sp+64] */
		dumptree(a, 610);
		if (
		a->x.ints[0] == 58  /* _^c */
		) {
			a->x.ints[0] = 57;  /* ^c */
			goto L610;   /* call ^y; st ^z,[%sp+64] */
		}
		spill(0xfffffffc, FREG, a);  
		a->op = 610;
		break;
	case 611: L611:   /* fdtoi ^y,%f0; st %f0,[%sp+64]; ld [%sp+64],^z */
		dumptree(a, 611);
		spill(0x00000001, FREG, a);  
		a->op = 611;
		break;
	case 612: L612:   /* sll ^y,^x,%g1; sra %g1,^x,^z */
		dumptree(a, 612);
		a->op = 612;
		break;
	case 613: L613:   /* ^z: */
		dumptree(a, 613);
		a->op = 613;
		break;
	case 614: L614:   /* nop */
		dumptree(a, 614);
		a->op = 614;
		break;
	case 615: L615:   /* # argd */
		dumptree(a, 615);
		a->op = 615;
		return;
	case 616: L616:   /* # movd */
		dumptree(a, 616);
		a->op = 616;
		return;
	case 617: L617:   /* # negd */
		dumptree(a, 617);
		a->op = 617;
		return;
	case 618: L618:   /* # asgnb */
		dumptree(a, 618);
		a->op = 618;
		return;
	case 619: L619:   /* # reg */
		dumptree(a, 619);
		a->op = 619;
		return;
	case 620: L620:   /* # ret */
		dumptree(a, 620);
		a->op = 620;
		return;
	case 621: L621:   /* # label */
		dumptree(a, 621);
		a->op = 621;
		return;
	case 622: L622:   /* ^f ^y,^x,^z */
		dumptree(a, 622);
		a->op = 622;
		break;
	case 623: L623:   /* f^f^t ^y,^x,^z */
		dumptree(a, 623);
		a->op = 623;
		break;
	case 624: L624:   /* call ^y,2; nop */
		dumptree(a, 624);
		a->op = 624;
		break;
	case 625: L625:   /* cmp ^y,^x; b^f ^z; nop */
		dumptree(a, 625);
		a->op = 625;
		break;
	case 626: L626:   /* fcmpe^t ^y,^x; nop; fb^f ^z; nop */
		dumptree(a, 626);
		a->op = 626;
		break;
	case 627: L627:   /* fstod ^y,^z */
		dumptree(a, 627);
		a->op = 627;
		break;
	case 628: L628:   /* fdtos ^y,^z */
		dumptree(a, 628);
		a->op = 628;
		break;
	case 629: L629:   /* fnegs ^y,^z */
		dumptree(a, 629);
		a->op = 629;
		break;
	case 630: L630:   /* fmovd ^y,^z; fnegs ^z,^z */
		dumptree(a, 630);
		a->op = 630;
		break;
	case 631: L631:   /* st ^y,[%sp+64]; ld [%sp+64],^z; fitod ^z,^z */
		dumptree(a, 631);
		a->op = 631;
		break;
	case 632: L632:   /* jmp ^y; nop */
		dumptree(a, 632);
		a->op = 632;
		break;
	case 633: L633:   /* ba ^y; nop */
		dumptree(a, 633);
		a->op = 633;
		break;
	case 634: L634:   /* ld^t [^y],^z */
		dumptree(a, 634);
		if (
		a->x.ints[4] == 8  /* d */
		) {
			a->x.ints[1] = 7;  /* %f^c */
			a->syms[1]=nextreg(a->syms[2]);
			goto L635;   /* ld [^y],^z; ld [^y+4],^x */
		}
		a->op = 634;
		break;
	case 635: L635:   /* ld [^y],^z; ld [^y+4],^x */
		dumptree(a, 635);
		a->op = 635;
		break;
	case 636: L636:   /* mov ^y,^z */
		dumptree(a, 636);
		if (
		a->x.ints[0] == 1  /* %^n */
		&& a->x.ints[2] == 2  /* %^c */
		&& a->kids[0]->syms[RX]->x.name==a->syms[RX]->x.name
		) {
			goto L619;   /* # reg */
		}
		a->op = 636;
		break;
	case 637: L637:   /* fmov^t ^y,^z */
		dumptree(a, 637);
		a->op = 637;
		break;
	case 638: L638:   /* set ^y,^z */
		dumptree(a, 638);
		if (
		a->x.ints[0] == 58  /* _^c */
		) {
			a->x.ints[0] = 57;  /* ^c */
			goto L638;   /* set ^y,^z */
		}
		a->op = 638;
		break;
	case 639: L639:   /* set 0xffff,%g1; and ^y,%g1,^z */
		dumptree(a, 639);
		a->op = 639;
		break;
	case 640: L640:   /* set ^z,%g1; st^t ^y,[%fp+%g1] */
		dumptree(a, 640);
		a->op = 640;
		break;
	case 641: L641:   /* set ^y,%g1; ld^t [%fp+%g1],^z */
		dumptree(a, 641);
		a->op = 641;
		break;
	case 642: L642:   /* st^t ^y,[^z] */
		dumptree(a, 642);
		switch (a->x.ints[4]) {
		case 8:   /* d */
			if (
			a->x.ints[0] == 6  /* %f^n */
			&& a->x.ints[2] == 35  /* %sp+4*^c+68 */
			) {
				spill(5>=a->syms[1]->u.c.v.i ? 256*("\003\006\014\030\060\040"[a->syms[1]->u.c.v.i]) : 0, IREG, a);
				goto L615;   /* # argd */
			}
			a->x.ints[1] = 7;  /* %f^c */
			a->syms[1]=nextreg(a->kids[0]->syms[2]);
			goto L644;   /* st ^y,[^z]; st ^x,[^z+4] */
		case 9:   /*  */
			if (
			a->x.ints[0] == 6  /* %f^n */
			&& a->x.ints[2] == 35  /* %sp+4*^c+68 */
			&& 6>a->syms[2]->u.c.v.i
			) {
				a->x.ints[1] = 57;  /* ^c */
				a->x.ints[2] = 57;  /* ^c */
				a->x.ints[2] = 57;  /* ^c */
				a->x.ints[0] = 6;  /* %f^n */
				goto L645;   /* st ^y,[%sp+4*^z+68]; ld [%sp+4*^z+68],%o^x */
			}
			break;
		}
		a->op = 642;
		break;
	case 643: L643:   /* st ^y,[^z] */
		dumptree(a, 643);
		a->op = 643;
		break;
	case 644: L644:   /* st ^y,[^z]; st ^x,[^z+4] */
		dumptree(a, 644);
		a->op = 644;
		break;
	case 645: L645:   /* st ^y,[%sp+4*^z+68]; ld [%sp+4*^z+68],%o^x */
		dumptree(a, 645);
		a->op = 645;
		break;
	case 646: L646:   /* set ^y,%g1; add %fp,%g1,^z */
		dumptree(a, 646);
		a->op = 646;
		break;
	case 647: L647:   /* call ^f,2; nop */
		dumptree(a, 647);
		a->op = 647;
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
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 57;  /* ^c */
		goto L638;   /* set ^y,^z */
	case 20: L20:   /* CNSTS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 57;  /* ^c */
		goto L638;   /* set ^y,^z */
	case 21: L21:   /* CNSTI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 57;  /* ^c */
		goto L638;   /* set ^y,^z */
	case 22: L22:   /* CNSTU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 57;  /* ^c */
		goto L638;   /* set ^y,^z */
	case 23: L23:   /* CNSTP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 57;  /* ^c */
		goto L638;   /* set ^y,^z */
	case 33: L33:   /* ARGF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 35;  /* %sp+4*^c+68 */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 9;  /*  */
		goto L642;   /* st^t ^y,[^z] */
	case 34: L34:   /* ARGD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 35;  /* %sp+4*^c+68 */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 8;  /* d */
		goto L642;   /* st^t ^y,[^z] */
	case 37: L37:   /* ARGI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 35;  /* %sp+4*^c+68 */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[4] = 9;  /*  */
		goto L642;   /* st^t ^y,[^z] */
	case 39: L39:   /* ARGP */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 35;  /* %sp+4*^c+68 */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[4] = 9;  /*  */
		goto L642;   /* st^t ^y,[^z] */
	case 49: L49:   /* ASGNF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 1;  /* %^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 9;  /*  */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L642;   /* st^t ^y,[^z] */
	case 50: L50:   /* ASGND */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 1;  /* %^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 8;  /* d */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L642;   /* st^t ^y,[^z] */
	case 51: L51:   /* ASGNC */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[4] = 5;  /* b */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L642;   /* st^t ^y,[^z] */
	case 52: L52:   /* ASGNS */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[4] = 6;  /* h */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L642;   /* st^t ^y,[^z] */
	case 53: L53:   /* ASGNI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[4] = 9;  /*  */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L642;   /* st^t ^y,[^z] */
	case 55: L55:   /* ASGNP */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[4] = 9;  /*  */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L642;   /* st^t ^y,[^z] */
	case 57: L57:   /* ASGNB */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		goto L618;   /* # asgnb */
	case 65: L65:   /* INDIRF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 7;  /* %f^c */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[4] = 9;  /*  */
		goto L634;   /* ld^t [^y],^z */
	case 66: L66:   /* INDIRD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 7;  /* %f^c */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[4] = 8;  /* d */
		goto L634;   /* ld^t [^y],^z */
	case 67: L67:   /* INDIRC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[4] = 1;  /* sb */
		goto L634;   /* ld^t [^y],^z */
	case 68: L68:   /* INDIRS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[4] = 2;  /* sh */
		goto L634;   /* ld^t [^y],^z */
	case 69: L69:   /* INDIRI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[4] = 9;  /*  */
		goto L634;   /* ld^t [^y],^z */
	case 71: L71:   /* INDIRP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[4] = 9;  /*  */
		goto L634;   /* ld^t [^y],^z */
	case 73: L73:   /* INDIRB */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		goto L636;   /* mov ^y,^z */
	case 85: L85:   /* CVCI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->syms[1] = &sym[5];  /* 24 */
		a->x.ints[1] = 57;  /* ^c */
		a->syms[1] = &sym[5];  /* 24 */
		a->x.ints[1] = 57;  /* ^c */
		a->x.ints[0] = 1;  /* %^n */
		goto L612;   /* sll ^y,^x,%g1; sra %g1,^x,^z */
	case 86: L86:   /* CVCU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->syms[1] = &sym[2];  /* 0xff */
		a->x.ints[1] = 57;  /* ^c */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 2;  /* and */
		goto L622;   /* ^f ^y,^x,^z */
	case 97: L97:   /* CVDF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 7;  /* %f^c */
		a->x.ints[0] = 6;  /* %f^n */
		goto L628;   /* fdtos ^y,^z */
	case 101: L101:   /* CVDI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 6;  /* %f^n */
		goto L611;   /* fdtoi ^y,%f0; st %f0,[%sp+64]; ld [%sp+64],^z */
	case 114: L114:   /* CVFD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 7;  /* %f^c */
		a->x.ints[0] = 6;  /* %f^n */
		goto L627;   /* fstod ^y,^z */
	case 130: L130:   /* CVID */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 7;  /* %f^c */
		a->x.ints[0] = 1;  /* %^n */
		goto L631;   /* st ^y,[%sp+64]; ld [%sp+64],^z; fitod ^z,^z */
	case 131: L131:   /* CVIC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		goto L636;   /* mov ^y,^z */
	case 132: L132:   /* CVIS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		goto L636;   /* mov ^y,^z */
	case 134: L134:   /* CVIU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		goto L636;   /* mov ^y,^z */
	case 150: L150:   /* CVPU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		goto L636;   /* mov ^y,^z */
	case 165: L165:   /* CVSI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->syms[1] = &sym[4];  /* 16 */
		a->x.ints[1] = 57;  /* ^c */
		a->syms[1] = &sym[4];  /* 16 */
		a->x.ints[1] = 57;  /* ^c */
		a->x.ints[0] = 1;  /* %^n */
		goto L612;   /* sll ^y,^x,%g1; sra %g1,^x,^z */
	case 166: L166:   /* CVSU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		goto L639;   /* set 0xffff,%g1; and ^y,%g1,^z */
	case 179: L179:   /* CVUC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		goto L636;   /* mov ^y,^z */
	case 180: L180:   /* CVUS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		goto L636;   /* mov ^y,^z */
	case 181: L181:   /* CVUI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		goto L636;   /* mov ^y,^z */
	case 183: L183:   /* CVUP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		goto L636;   /* mov ^y,^z */
	case 193: L193:   /* NEGF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 7;  /* %f^c */
		a->x.ints[0] = 6;  /* %f^n */
		goto L629;   /* fnegs ^y,^z */
	case 194: L194:   /* NEGD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		goto L617;   /* # negd */
	case 197: L197:   /* NEGI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->kids[1] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 5;  /* %g0 */
		a->x.ints[3] = 9;  /* sub */
		goto L622;   /* ^f ^y,^x,^z */
	case 209: L209:   /* CALLF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* %^n */
		setreg(a,f0);
		goto L609;   /* call ^y; nop */
	case 210: L210:   /* CALLD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* %^n */
		setreg(a,d0);
		goto L609;   /* call ^y; nop */
	case 213: L213:   /* CALLI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* %^n */
		setreg(a,oreg[0]);
		goto L609;   /* call ^y; nop */
	case 216: L216:   /* CALLV */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* %^n */
		setreg(a,oreg[0]);
		goto L609;   /* call ^y; nop */
	case 217: L217:   /* CALLB */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[1];
		a->kids[1] = 0;
		a->x.ints[2] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		setreg(a,oreg[0]);
		goto L610;   /* call ^y; st ^z,[%sp+64] */
	case 225: L225:   /* LOADF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 7;  /* %f^c */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 7;  /* s */
		goto L637;   /* fmov^t ^y,^z */
	case 226: L226:   /* LOADD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		goto L616;   /* # movd */
	case 227: L227:   /* LOADC */
	case 228: L228:   /* LOADS */
	case 229: L229:   /* LOADI */
	case 230: L230:   /* LOADU */
	case 231: L231:   /* LOADP */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		goto L636;   /* mov ^y,^z */
	case 233: L233:   /* LOADB */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		goto L636;   /* mov ^y,^z */
	case 241: L241:   /* RETF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rtarget(a,0,f0);
		goto L620;   /* # ret */
	case 242: L242:   /* RETD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rtarget(a,0,d0);
		goto L620;   /* # ret */
	case 245: L245:   /* RETI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rtarget(a,0,i0);
		goto L620;   /* # ret */
	case 248: L248:   /* RETV */
		dumptree(a, a->op);
		goto L620;   /* # ret */
	case 263: L263:   /* ADDRGP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 58;  /* _^c */
		goto L638;   /* set ^y,^z */
	case 279: L279:   /* ADDRFP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 57;  /* ^c */
		goto L646;   /* set ^y,%g1; add %fp,%g1,^z */
	case 295: L295:   /* ADDRLP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 57;  /* ^c */
		goto L646;   /* set ^y,%g1; add %fp,%g1,^z */
	case 305: L305:   /* ADDF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 7;  /* %f^c */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 7;  /* s */
		a->x.ints[3] = 1;  /* add */
		goto L623;   /* f^f^t ^y,^x,^z */
	case 306: L306:   /* ADDD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 7;  /* %f^c */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 8;  /* d */
		a->x.ints[3] = 1;  /* add */
		goto L623;   /* f^f^t ^y,^x,^z */
	case 309: L309:   /* ADDI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 1;  /* add */
		goto L622;   /* ^f ^y,^x,^z */
	case 310: L310:   /* ADDU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 1;  /* add */
		goto L622;   /* ^f ^y,^x,^z */
	case 311: L311:   /* ADDP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 1;  /* add */
		goto L622;   /* ^f ^y,^x,^z */
	case 321: L321:   /* SUBF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 7;  /* %f^c */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 7;  /* s */
		a->x.ints[3] = 9;  /* sub */
		goto L623;   /* f^f^t ^y,^x,^z */
	case 322: L322:   /* SUBD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 7;  /* %f^c */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 8;  /* d */
		a->x.ints[3] = 9;  /* sub */
		goto L623;   /* f^f^t ^y,^x,^z */
	case 325: L325:   /* SUBI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 9;  /* sub */
		goto L622;   /* ^f ^y,^x,^z */
	case 326: L326:   /* SUBU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 9;  /* sub */
		goto L622;   /* ^f ^y,^x,^z */
	case 327: L327:   /* SUBP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 9;  /* sub */
		goto L622;   /* ^f ^y,^x,^z */
	case 341: L341:   /* LSHI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 6;  /* sll */
		goto L622;   /* ^f ^y,^x,^z */
	case 342: L342:   /* LSHU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 6;  /* sll */
		goto L622;   /* ^f ^y,^x,^z */
	case 357: L357:   /* MODI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 17;  /* .rem */
		a->x.ints[1] = 1;  /* %^n */
		ncalls++;
		goto L608;   /* mov ^x,%o1; call ^f,2; mov ^y,%o0; mov %o0,^z */
	case 358: L358:   /* MODU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 14;  /* .urem */
		a->x.ints[1] = 1;  /* %^n */
		ncalls++;
		goto L608;   /* mov ^x,%o1; call ^f,2; mov ^y,%o0; mov %o0,^z */
	case 373: L373:   /* RSHI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 7;  /* sra */
		goto L622;   /* ^f ^y,^x,^z */
	case 374: L374:   /* RSHU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 8;  /* srl */
		goto L622;   /* ^f ^y,^x,^z */
	case 390: L390:   /* BANDU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 2;  /* and */
		goto L622;   /* ^f ^y,^x,^z */
	case 406: L406:   /* BCOMU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[1] = 5;  /* %g0 */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 11;  /* xnor */
		goto L622;   /* ^f ^y,^x,^z */
	case 422: L422:   /* BORU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 5;  /* or */
		goto L622;   /* ^f ^y,^x,^z */
	case 438: L438:   /* BXORU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 10;  /* xor */
		goto L622;   /* ^f ^y,^x,^z */
	case 449: L449:   /* DIVF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 7;  /* %f^c */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 7;  /* s */
		a->x.ints[3] = 3;  /* div */
		goto L623;   /* f^f^t ^y,^x,^z */
	case 450: L450:   /* DIVD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 7;  /* %f^c */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 8;  /* d */
		a->x.ints[3] = 3;  /* div */
		goto L623;   /* f^f^t ^y,^x,^z */
	case 453: L453:   /* DIVI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 15;  /* .div */
		a->x.ints[1] = 1;  /* %^n */
		ncalls++;
		goto L608;   /* mov ^x,%o1; call ^f,2; mov ^y,%o0; mov %o0,^z */
	case 454: L454:   /* DIVU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 12;  /* .udiv */
		a->x.ints[1] = 1;  /* %^n */
		ncalls++;
		goto L608;   /* mov ^x,%o1; call ^f,2; mov ^y,%o0; mov %o0,^z */
	case 465: L465:   /* MULF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 7;  /* %f^c */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 7;  /* s */
		a->x.ints[3] = 4;  /* mul */
		goto L623;   /* f^f^t ^y,^x,^z */
	case 466: L466:   /* MULD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 7;  /* %f^c */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 8;  /* d */
		a->x.ints[3] = 4;  /* mul */
		goto L623;   /* f^f^t ^y,^x,^z */
	case 469: L469:   /* MULI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 16;  /* .mul */
		a->x.ints[1] = 1;  /* %^n */
		ncalls++;
		goto L608;   /* mov ^x,%o1; call ^f,2; mov ^y,%o0; mov %o0,^z */
	case 470: L470:   /* MULU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* %^c */
		a->x.ints[0] = 1;  /* %^n */
		a->x.ints[3] = 13;  /* .umul */
		a->x.ints[1] = 1;  /* %^n */
		ncalls++;
		goto L608;   /* mov ^x,%o1; call ^f,2; mov ^y,%o0; mov %o0,^z */
	case 481: L481:   /* EQF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 32;  /* ue */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 7;  /* s */
		goto L626;   /* fcmpe^t ^y,^x; nop; fb^f ^z; nop */
	case 482: L482:   /* EQD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 32;  /* ue */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 8;  /* d */
		goto L626;   /* fcmpe^t ^y,^x; nop; fb^f ^z; nop */
	case 485: L485:   /* EQI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 18;  /* e */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		goto L625;   /* cmp ^y,^x; b^f ^z; nop */
	case 497: L497:   /* GEF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 29;  /* uge */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 7;  /* s */
		goto L626;   /* fcmpe^t ^y,^x; nop; fb^f ^z; nop */
	case 498: L498:   /* GED */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 29;  /* uge */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 8;  /* d */
		goto L626;   /* fcmpe^t ^y,^x; nop; fb^f ^z; nop */
	case 501: L501:   /* GEI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 23;  /* ge */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		goto L625;   /* cmp ^y,^x; b^f ^z; nop */
	case 502: L502:   /* GEU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 22;  /* geu */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		goto L625;   /* cmp ^y,^x; b^f ^z; nop */
	case 513: L513:   /* GTF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 28;  /* ug */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 7;  /* s */
		goto L626;   /* fcmpe^t ^y,^x; nop; fb^f ^z; nop */
	case 514: L514:   /* GTD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 28;  /* ug */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 8;  /* d */
		goto L626;   /* fcmpe^t ^y,^x; nop; fb^f ^z; nop */
	case 517: L517:   /* GTI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 21;  /* g */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		goto L625;   /* cmp ^y,^x; b^f ^z; nop */
	case 518: L518:   /* GTU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 20;  /* gu */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		goto L625;   /* cmp ^y,^x; b^f ^z; nop */
	case 529: L529:   /* LEF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 31;  /* ule */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 7;  /* s */
		goto L626;   /* fcmpe^t ^y,^x; nop; fb^f ^z; nop */
	case 530: L530:   /* LED */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 31;  /* ule */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 8;  /* d */
		goto L626;   /* fcmpe^t ^y,^x; nop; fb^f ^z; nop */
	case 533: L533:   /* LEI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 27;  /* le */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		goto L625;   /* cmp ^y,^x; b^f ^z; nop */
	case 534: L534:   /* LEU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 26;  /* leu */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		goto L625;   /* cmp ^y,^x; b^f ^z; nop */
	case 545: L545:   /* LTF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 30;  /* ul */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 7;  /* s */
		goto L626;   /* fcmpe^t ^y,^x; nop; fb^f ^z; nop */
	case 546: L546:   /* LTD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 30;  /* ul */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 8;  /* d */
		goto L626;   /* fcmpe^t ^y,^x; nop; fb^f ^z; nop */
	case 549: L549:   /* LTI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 25;  /* l */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		goto L625;   /* cmp ^y,^x; b^f ^z; nop */
	case 550: L550:   /* LTU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 24;  /* lu */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		goto L625;   /* cmp ^y,^x; b^f ^z; nop */
	case 561: L561:   /* NEF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 19;  /* ne */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 7;  /* s */
		goto L626;   /* fcmpe^t ^y,^x; nop; fb^f ^z; nop */
	case 562: L562:   /* NED */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 19;  /* ne */
		a->x.ints[1] = 6;  /* %f^n */
		a->x.ints[0] = 6;  /* %f^n */
		a->x.ints[4] = 8;  /* d */
		goto L626;   /* fcmpe^t ^y,^x; nop; fb^f ^z; nop */
	case 565: L565:   /* NEI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		a->x.ints[3] = 19;  /* ne */
		a->x.ints[1] = 1;  /* %^n */
		a->x.ints[0] = 1;  /* %^n */
		goto L625;   /* cmp ^y,^x; b^f ^z; nop */
	case 584: L584:   /* JUMPV */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* %^n */
		goto L632;   /* jmp ^y; nop */
	case 600: L600:   /* LABELV */
		dumptree(a, a->op);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 57;  /* ^c */
		goto L613;   /* ^z: */
	case 608: L608:   /* mov ^x,%o1; call ^f,2; mov ^y,%o0; mov %o0,^z */
		dumptree(a, 608);
		a->op = 608;
		break;
	case 609: L609:   /* call ^y; nop */
		dumptree(a, 609);
		b = a->kids[0];
		if (
		a->x.ints[0] == 1  /* %^n */
		&& b->op == 638  /* set ^y,^z */
		&& b->x.ints[0] == 58  /* _^c */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L609;   /* call ^y; nop */
		}
		a->op = 609;
		break;
	case 610: L610:   /* call ^y; st ^z,[%sp+64] */
		dumptree(a, 610);
		b = a->kids[0];
		if (
		a->x.ints[0] == 1  /* %^n */
		&& b->op == 638  /* set ^y,^z */
		&& b->x.ints[0] == 58  /* _^c */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L610;   /* call ^y; st ^z,[%sp+64] */
		}
		a->op = 610;
		break;
	case 611: L611:   /* fdtoi ^y,%f0; st %f0,[%sp+64]; ld [%sp+64],^z */
		dumptree(a, 611);
		a->op = 611;
		break;
	case 612: L612:   /* sll ^y,^x,%g1; sra %g1,^x,^z */
		dumptree(a, 612);
		if (
		a->x.ints[0] == 1  /* %^n */
		&& a->x.ints[1] == 57  /* ^c */
		) {
			b = a->kids[0];
			if (
			a->syms[1]->x.name == sym[4].x.name  /* 16 */
			&& a->x.ints[1] == 57  /* ^c */
			&& a->syms[1]->x.name == sym[4].x.name  /* 16 */
			&& b->count == 1
			&& b->op == 634  /* ld^t [^y],^z */
			&& b->x.ints[4] == 2  /* sh */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->x.ints[4] = 2;  /* sh */
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L634;   /* ld^t [^y],^z */
			}
			if (
			a->syms[1]->x.name == sym[5].x.name  /* 24 */
			&& a->x.ints[1] == 57  /* ^c */
			&& a->syms[1]->x.name == sym[5].x.name  /* 24 */
			&& b->count == 1
			&& b->op == 634  /* ld^t [^y],^z */
			&& b->x.ints[4] == 1  /* sb */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->x.ints[4] = 1;  /* sb */
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L634;   /* ld^t [^y],^z */
			}
		}
		a->op = 612;
		break;
	case 613: L613:   /* ^z: */
		dumptree(a, 613);
		a->op = 613;
		break;
	case 614: L614:   /* nop */
		dumptree(a, 614);
		a->op = 614;
		break;
	case 615: L615:   /* # argd */
		dumptree(a, 615);
		a->op = 615;
		break;
	case 616: L616:   /* # movd */
		dumptree(a, 616);
		a->op = 616;
		break;
	case 617: L617:   /* # negd */
		dumptree(a, 617);
		a->op = 617;
		break;
	case 618: L618:   /* # asgnb */
		dumptree(a, 618);
		a->op = 618;
		break;
	case 619: L619:   /* # reg */
		dumptree(a, 619);
		a->op = 619;
		break;
	case 620: L620:   /* # ret */
		dumptree(a, 620);
		a->op = 620;
		break;
	case 621: L621:   /* # label */
		dumptree(a, 621);
		a->op = 621;
		break;
	case 622: L622:   /* ^f ^y,^x,^z */
		dumptree(a, 622);
		if (
		a->x.ints[1] == 1  /* %^n */
		) {
			b = a->kids[1];
			if (
			b->op == 638  /* set ^y,^z */
			&& b->x.ints[0] == 57  /* ^c */
			&& range(getint(a->kids[1]->syms[0]),13)
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[1] = b->syms[0];
				a->kids[4] = b->kids[3];
				a->x.ints[1] = b->x.ints[0];
				a->kids[1] = b->kids[0];
				goto L622;   /* ^f ^y,^x,^z */
			}
			if (
			b->count == 1
			&& b->op == 636  /* mov ^y,^z */
			&& b->x.ints[0] == 1  /* %^n */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[1] = b->syms[0];
				a->kids[4] = b->kids[3];
				a->x.ints[1] = b->x.ints[0];
				a->kids[1] = b->kids[0];
				goto L622;   /* ^f ^y,^x,^z */
			}
		}
		b = a->kids[0];
		if (
		a->x.ints[0] == 1  /* %^n */
		&& b->count == 1
		&& b->op == 636  /* mov ^y,^z */
		&& b->x.ints[0] == 1  /* %^n */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L622;   /* ^f ^y,^x,^z */
		}
		if (
		a->x.ints[3] == 2  /* and */
		&& a->x.ints[0] == 1  /* %^n */
		&& a->x.ints[1] == 57  /* ^c */
		&& a->syms[1]->x.name == sym[2].x.name  /* 0xff */
		&& b->count == 1
		&& b->op == 634  /* ld^t [^y],^z */
		&& b->x.ints[4] == 1  /* sb */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->x.ints[4] = 3;  /* ub */
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L634;   /* ld^t [^y],^z */
		}
		a->op = 622;
		break;
	case 623: L623:   /* f^f^t ^y,^x,^z */
		dumptree(a, 623);
		a->op = 623;
		break;
	case 624: L624:   /* call ^y,2; nop */
		dumptree(a, 624);
		a->op = 624;
		break;
	case 625: L625:   /* cmp ^y,^x; b^f ^z; nop */
		dumptree(a, 625);
		if (
		a->x.ints[1] == 1  /* %^n */
		) {
			b = a->kids[1];
			if (
			b->op == 638  /* set ^y,^z */
			&& b->x.ints[0] == 57  /* ^c */
			&& range(getint(a->kids[1]->syms[0]),13)
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[1] = b->syms[0];
				a->kids[4] = b->kids[3];
				a->x.ints[1] = b->x.ints[0];
				a->kids[1] = b->kids[0];
				goto L625;   /* cmp ^y,^x; b^f ^z; nop */
			}
			if (
			b->count == 1
			&& b->op == 636  /* mov ^y,^z */
			&& b->x.ints[0] == 1  /* %^n */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[1] = b->syms[0];
				a->kids[4] = b->kids[3];
				a->x.ints[1] = b->x.ints[0];
				a->kids[1] = b->kids[0];
				goto L625;   /* cmp ^y,^x; b^f ^z; nop */
			}
		}
		b = a->kids[0];
		if (
		a->x.ints[0] == 1  /* %^n */
		&& b->count == 1
		&& b->op == 636  /* mov ^y,^z */
		&& b->x.ints[0] == 1  /* %^n */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L625;   /* cmp ^y,^x; b^f ^z; nop */
		}
		a->op = 625;
		break;
	case 626: L626:   /* fcmpe^t ^y,^x; nop; fb^f ^z; nop */
		dumptree(a, 626);
		a->op = 626;
		break;
	case 627: L627:   /* fstod ^y,^z */
		dumptree(a, 627);
		a->op = 627;
		break;
	case 628: L628:   /* fdtos ^y,^z */
		dumptree(a, 628);
		a->op = 628;
		break;
	case 629: L629:   /* fnegs ^y,^z */
		dumptree(a, 629);
		a->op = 629;
		break;
	case 630: L630:   /* fmovd ^y,^z; fnegs ^z,^z */
		dumptree(a, 630);
		a->op = 630;
		break;
	case 631: L631:   /* st ^y,[%sp+64]; ld [%sp+64],^z; fitod ^z,^z */
		dumptree(a, 631);
		a->op = 631;
		break;
	case 632: L632:   /* jmp ^y; nop */
		dumptree(a, 632);
		b = a->kids[0];
		if (
		a->x.ints[0] == 1  /* %^n */
		&& b->op == 638  /* set ^y,^z */
		&& b->x.ints[0] == 58  /* _^c */
		) {
			--b->count;
			a->x.ints[0] = 57;  /* ^c */
			a->syms[0] = b->syms[0];
			a->kids[0] = 0;
			goto L633;   /* ba ^y; nop */
		}
		a->op = 632;
		break;
	case 633: L633:   /* ba ^y; nop */
		dumptree(a, 633);
		a->op = 633;
		break;
	case 634: L634:   /* ld^t [^y],^z */
		dumptree(a, 634);
		if (
		a->x.ints[0] == 1  /* %^n */
		) {
			b = a->kids[0];
			if (
			b->op == 622  /* ^f ^y,^x,^z */
			&& b->x.ints[3] == 1  /* add */
			&& b->x.ints[0] == 57  /* ^c */
			&& b->x.ints[1] == 3  /* %fp */
			) {
				--b->count;
				a->x.ints[0] = 27;  /* %fp+^c */
				a->syms[0] = b->syms[0];
				a->kids[0] = 0;
				goto L634;   /* ld^t [^y],^z */
			}
			if (
			b->count == 1
			) {
				switch (b->op) {
				case 622:   /* ^f ^y,^x,^z */
					if (
					b->x.ints[3] == 1  /* add */
					&& b->x.ints[0] == 1  /* %^n */
					&& b->x.ints[1] == 57  /* ^c */
					) {
						if (--b->count > 0)
							if (c=b->kids[0]) ++c->count;
						a->x.ints[0] = 15;  /* %^n+^c */
						a->kids[0] = b->kids[0];
						a->syms[0] = b->syms[1];
						goto L634;   /* ld^t [^y],^z */
					}
					break;
				case 646:   /* set ^y,%g1; add %fp,%g1,^z */
					--b->count;
					a->x.ints[0] = 57;  /* ^c */
					a->syms[0] = b->syms[0];
					a->kids[0] = 0;
					goto L641;   /* set ^y,%g1; ld^t [%fp+%g1],^z */
				}
			}
		}
		b = a->kids[0];
		if (
		"\1\1\1\1\1\0\0\1\0\1"[a->x.ints[4]]  /* <TMP717> */
		&& a->x.ints[0] == 1  /* %^n */
		&& b->count == 1
		&& b->op == 622  /* ^f ^y,^x,^z */
		&& b->x.ints[3] == 1  /* add */
		&& b->x.ints[0] == 1  /* %^n */
		&& b->x.ints[1] == 1  /* %^n */
		) {
			if (--b->count > 0) {
				if (c=b->kids[1]) ++c->count;
				if (c=b->kids[0]) ++c->count;
			}
			a->x.ints[0] = 8;  /* %^n+%^i */
			a->kids[0] = b->kids[0];
			a->kids[3] = b->kids[1];
			goto L634;   /* ld^t [^y],^z */
		}
		switch (a->x.ints[0]) {
		case 1:   /* %^n */
			if (
			b->count == 1
			&& b->op == 636  /* mov ^y,^z */
			&& b->x.ints[0] == 1  /* %^n */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L634;   /* ld^t [^y],^z */
			}
			break;
		case 27:   /* %fp+^c */
			if (
			isregvar(a->syms[0])
			) {
				a->x.ints[2] = 2;  /* %^c */
				a->x.ints[0] = 2;  /* %^c */
				goto L636;   /* mov ^y,^z */
			}
			break;
		}
		a->op = 634;
		break;
	case 635: L635:   /* ld [^y],^z; ld [^y+4],^x */
		dumptree(a, 635);
		a->op = 635;
		break;
	case 636: L636:   /* mov ^y,^z */
		dumptree(a, 636);
		switch (a->x.ints[0]) {
		case 1:   /* %^n */
			if (
			a->kids[0]->count == 1
			) {
				b = a->kids[0];
				switch (b->op) {
				case 622:   /* ^f ^y,^x,^z */
					if (--b->count > 0) {
						if (c=b->kids[1]) ++c->count;
						if (c=b->kids[4]) ++c->count;
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->x.ints[3] = b->x.ints[3];
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					a->syms[1] = b->syms[1];
					a->kids[4] = b->kids[4];
					a->kids[1] = b->kids[1];
					a->x.ints[1] = b->x.ints[1];
					goto L622;   /* ^f ^y,^x,^z */
				case 634:   /* ld^t [^y],^z */
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->x.ints[4] = b->x.ints[4];
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L634;   /* ld^t [^y],^z */
				case 636:   /* mov ^y,^z */
					if (
					b->x.ints[0] == 1  /* %^n */
					) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[0] = b->syms[0];
						a->kids[3] = b->kids[3];
						a->x.ints[0] = b->x.ints[0];
						a->kids[0] = b->kids[0];
						goto L636;   /* mov ^y,^z */
					}
					break;
				case 638:   /* set ^y,^z */
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L638;   /* set ^y,^z */
				case 641:   /* set ^y,%g1; ld^t [%fp+%g1],^z */
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->x.ints[4] = b->x.ints[4];
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L641;   /* set ^y,%g1; ld^t [%fp+%g1],^z */
				}
			}
			break;
		case 2:   /* %^c */
			if (
			a->x.ints[2] == 2  /* %^c */
			&& rmcopy(a)
			) {
				a->syms[2] = a->syms[0];
				goto L619;   /* # reg */
			}
			break;
		}
		a->op = 636;
		break;
	case 637: L637:   /* fmov^t ^y,^z */
		dumptree(a, 637);
		a->op = 637;
		break;
	case 638: L638:   /* set ^y,^z */
		dumptree(a, 638);
		if (
		a->x.ints[0] == 57  /* ^c */
		&& a->syms[0]->x.name == sym[1].x.name  /* 0 */
		) {
			setreg(a,g0);
			goto L619;   /* # reg */
		}
		a->op = 638;
		break;
	case 639: L639:   /* set 0xffff,%g1; and ^y,%g1,^z */
		dumptree(a, 639);
		b = a->kids[0];
		if (
		a->x.ints[0] == 1  /* %^n */
		&& b->count == 1
		&& b->op == 634  /* ld^t [^y],^z */
		&& b->x.ints[4] == 2  /* sh */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->x.ints[4] = 4;  /* uh */
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L634;   /* ld^t [^y],^z */
		}
		a->op = 639;
		break;
	case 640: L640:   /* set ^z,%g1; st^t ^y,[%fp+%g1] */
		dumptree(a, 640);
		a->op = 640;
		break;
	case 641: L641:   /* set ^y,%g1; ld^t [%fp+%g1],^z */
		dumptree(a, 641);
		a->op = 641;
		break;
	case 642: L642:   /* st^t ^y,[^z] */
		dumptree(a, 642);
		if (
		a->x.ints[0] == 1  /* %^n */
		&& a->x.ints[2] == 35  /* %sp+4*^c+68 */
		&& 6>a->syms[1]->u.c.v.i
		) {
			a->x.ints[2] = 2;  /* %^c */
			a->x.ints[0] = 1;  /* %^n */
			a->syms[2]=oreg[a->syms[1]->u.c.v.i];
			goto L636;   /* mov ^y,^z */
		}
		if (
		a->x.ints[2] == 1  /* %^n */
		) {
			b = a->kids[2];
			if (
			b->op == 622  /* ^f ^y,^x,^z */
			&& b->x.ints[3] == 1  /* add */
			&& b->x.ints[0] == 57  /* ^c */
			&& b->x.ints[1] == 3  /* %fp */
			) {
				--b->count;
				a->x.ints[2] = 27;  /* %fp+^c */
				a->syms[2] = b->syms[0];
				a->kids[2] = 0;
				goto L642;   /* st^t ^y,[^z] */
			}
			if (
			b->count == 1
			) {
				switch (b->op) {
				case 622:   /* ^f ^y,^x,^z */
					if (
					b->x.ints[3] == 1  /* add */
					&& b->x.ints[0] == 1  /* %^n */
					&& b->x.ints[1] == 57  /* ^c */
					) {
						if (--b->count > 0)
							if (c=b->kids[0]) ++c->count;
						a->x.ints[2] = 15;  /* %^n+^c */
						a->kids[2] = b->kids[0];
						a->syms[2] = b->syms[1];
						goto L642;   /* st^t ^y,[^z] */
					}
					break;
				case 646:   /* set ^y,%g1; add %fp,%g1,^z */
					--b->count;
					a->x.ints[2] = 57;  /* ^c */
					a->syms[2] = b->syms[0];
					a->kids[2] = 0;
					goto L640;   /* set ^z,%g1; st^t ^y,[%fp+%g1] */
				}
			}
		}
		b = a->kids[2];
		if (
		"\1\0\0\0\0\1\1\1\0\1"[a->x.ints[4]]  /* <TMP851> */
		&& a->x.ints[2] == 1  /* %^n */
		&& b->count == 1
		&& b->op == 622  /* ^f ^y,^x,^z */
		&& b->x.ints[3] == 1  /* add */
		&& b->x.ints[0] == 1  /* %^n */
		&& b->x.ints[1] == 1  /* %^n */
		) {
			if (--b->count > 0) {
				if (c=b->kids[1]) ++c->count;
				if (c=b->kids[0]) ++c->count;
			}
			a->x.ints[2] = 8;  /* %^n+%^i */
			a->kids[2] = b->kids[0];
			a->kids[5] = b->kids[1];
			goto L642;   /* st^t ^y,[^z] */
		}
		if (
		a->x.ints[2] == 1  /* %^n */
		&& b->count == 1
		&& b->op == 636  /* mov ^y,^z */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[2] = b->syms[0];
			a->kids[5] = b->kids[3];
			a->x.ints[2] = b->x.ints[0];
			a->kids[2] = b->kids[0];
			goto L642;   /* st^t ^y,[^z] */
		}
		b = a->kids[0];
		if (
		a->x.ints[0] == 1  /* %^n */
		&& b->count == 1
		&& b->op == 636  /* mov ^y,^z */
		) {
			if (--b->count > 0) {
				if (c=b->kids[0]) ++c->count;
				if (c=b->kids[3]) ++c->count;
			}
			a->syms[0] = b->syms[0];
			a->kids[3] = b->kids[3];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L642;   /* st^t ^y,[^z] */
		}
		if (
		a->x.ints[2] == 27  /* %fp+^c */
		&& isregvar(a->syms[2])
		) {
			a->x.ints[2] = 2;  /* %^c */
			goto L636;   /* mov ^y,^z */
		}
		a->op = 642;
		break;
	case 643: L643:   /* st ^y,[^z] */
		dumptree(a, 643);
		a->op = 643;
		break;
	case 644: L644:   /* st ^y,[^z]; st ^x,[^z+4] */
		dumptree(a, 644);
		a->op = 644;
		break;
	case 645: L645:   /* st ^y,[%sp+4*^z+68]; ld [%sp+4*^z+68],%o^x */
		dumptree(a, 645);
		a->op = 645;
		break;
	case 646: L646:   /* set ^y,%g1; add %fp,%g1,^z */
		dumptree(a, 646);
		if (
		a->x.ints[2] == 2  /* %^c */
		&& isregvar(a->syms[0])||range(-a->syms[0]->x.offset,13)
		) {
			a->x.ints[1] = 3;  /* %fp */
			a->x.ints[3] = 1;  /* add */
			goto L622;   /* ^f ^y,^x,^z */
		}
		a->op = 646;
		break;
	case 647: L647:   /* call ^f,2; nop */
		dumptree(a, 647);
		a->op = 647;
		break;
	default:
	assert(0);
	}
}


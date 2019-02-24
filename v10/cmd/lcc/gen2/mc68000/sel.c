/* "lea %c($sp),$a1; mov.l &%0/2,$d0; mov.w ($a0)+,($a1)+; sub.l &1,$d0; byte 0x6e,-6" => ="lea %z,$a1; mov.l %x,$d0; mov.w ($a0)+,($a1)+; sub.l &1,$d0; byte 0x6e,-6" C="%c($sp)" B="&%c/2" b=a */
/* "mov.l &%0/2,$d0; mov.w ($a0)+,($a1)+; sub.l &1,$d0; byte 0x6e,-6" => ="mov.l %x,$d0; mov.w ($a0)+,($a1)+; sub.l &1,$d0; byte 0x6e,-6" B="&%c/2" b=a */
/* "mov.l $a%0,$a%c" => ="mov.%t %y,%z" E="l" A="$a%n" 0=0 C="$a%c" */
/* "jsr ($a%0)" => ="jsr %y" A="($a%n)" 0=0 */
/* "and.l &0xff,$d%c" => ="%f.%t %x,%z" D="and" E="l" B="&%c" b="0xff" C="$d%c" */
/* "and.l &0xffff,$d%c" => ="%f.%t %x,%z" D="and" E="l" B="&%c" b="0xffff" C="$d%c" */
/* "mov.b $d%0,$d%c" => ="mov.%t %y,%z" E="b" A="$d%n" 0=0 C="$d%c" */
/* "mov.w $d%0,$d%c" => ="mov.%t %y,%z" E="w" A="$d%n" 0=0 C="$d%c" */
/* "mov.b $d%0,$d%c" => ="mov.%t %y,%z" E="b" A="$d%n" 0=0 C="$d%c" */
/* "mov.w $d%0,$d%c" => ="mov.%t %y,%z" E="w" A="$d%n" 0=0 C="$d%c" */
/* "mov.l $d%0,$d%c" => ="mov.%t %y,%z" E="l" A="$d%n" 0=0 C="$d%c" */
/* "mov.l $d%0,$d%c" => ="mov.%t %y,%z" E="l" A="$d%n" 0=0 C="$d%c" */
/* "mov.l $d%0,$d%c" => ="mov.%t %y,%z" E="l" A="$d%n" 0=0 C="$d%c" */
/* "mov.l $a%0,$d%c" => ="mov.%t %y,%z" E="l" A="$a%n" 0=0 C="$d%c" */
/* "mov.l $d%0,$d%c" => ="mov.%t %y,%z" E="l" A="$d%n" 0=0 C="$d%c" */
/* "mov.l $d%0,$d%c" => ="mov.%t %y,%z" E="l" A="$d%n" 0=0 C="$d%c" */
/* "mov.l $d%0,$d%c" => ="mov.%t %y,%z" E="l" A="$d%n" 0=0 C="$d%c" */
/* "mov.l $d%0,$a%c" => ="mov.%t %y,%z" E="l" A="$d%n" 0=0 C="$a%c" */
/* "mov.l $d%0,$d%c" => ="mov.%t %y,%z" E="l" A="$d%n" 0=0 C="$d%c" */
/* "mov.l $d%0,$d%c" => ="mov.%t %y,%z" E="l" A="$d%n" 0=0 C="$d%c" */
/* "mov.l $d%0,$d%c" => ="mov.%t %y,%z" E="l" A="$d%n" 0=0 C="$d%c" */
/* "mov.l $a%0,$a%c" => ="mov.%t %y,%z" E="l" A="$a%n" 0=0 C="$a%c" */
/* "mov.l $a%0,$a%c" => ="mov.%t %y,%z" E="l" A="$a%n" 0=0 C="$a%c" */
/* "mov.l $d%0,$d%c" => ="mov.%t %y,%z" E="l" A="$d%n" 0=0 C="$d%c" */
/* "jsr ($a%0)" => ="jsr %y" A="($a%n)" 0=0 */
/* "jsr ($a%0)" => ="jsr %y" A="($a%n)" 0=0 */
/* "bra ($a%0)" => ="bra %z" C="($a%n)" 2=0 */
/* "L%0:" => ="%z:" C="L%c" c=a */
/* "lea _%0,$a%c" => ="lea %y,%z" A="_%c" a=a C="$a%c" */
/* "lea %0($a6),$a%c" => ="lea %y,%z" A="%c($a6)" a=a C="$a%c" */
/* "lea %0($a6),$a%c" => ="lea %y,%z" A="%c($a6)" a=a C="$a%c" */
/* "add.l $d%1,$a%c" => ="%f.%t %x,%z" D="add" E="l" B="$d%n" 1=1 C="$a%c" */
/* "sub.l $d%1,$a%c" => ="%f.%t %x,%z" D="sub" E="l" B="$d%n" 1=1 C="$a%c" */
/* "and.l $d%1,$d%c" => ="%f.%t %x,%z" D="and" E="l" B="$d%n" 1=1 C="$d%c" */
/* "add.l $d%1,$d%c" => ="%f.%t %x,%z" D="add" E="l" B="$d%n" 1=1 C="$d%c" */
/* "add.l $d%1,$d%c" => ="%f.%t %x,%z" D="add" E="l" B="$d%n" 1=1 C="$d%c" */
/* "or.l $d%1,$d%c" => ="%f.%t %x,%z" D="or" E="l" B="$d%n" 1=1 C="$d%c" */
/* "sub.l $d%1,$d%c" => ="%f.%t %x,%z" D="sub" E="l" B="$d%n" 1=1 C="$d%c" */
/* "sub.l $d%1,$d%c" => ="%f.%t %x,%z" D="sub" E="l" B="$d%n" 1=1 C="$d%c" */
/* "eor.l $d%1,$d%c" => ="%f.%t %x,%z" D="eor" E="l" B="$d%n" 1=1 C="$d%c" */
/* "asl.l $d%1,$d%c" => ="%f.%t %x,%z" D="asl" E="l" B="$d%n" 1=1 C="$d%c" */
/* "asr.l $d%1,$d%c" => ="%f.%t %x,%z" D="asr" E="l" B="$d%n" 1=1 C="$d%c" */
/* "lsl.l $d%1,$d%c" => ="%f.%t %x,%z" D="lsl" E="l" B="$d%n" 1=1 C="$d%c" */
/* "lsr.l $d%1,$d%c" => ="%f.%t %x,%z" D="lsr" E="l" B="$d%n" 1=1 C="$d%c" */
/* "muls.w $d%1,$d%c" => ="%f.%t %x,%z" D="muls" E="w" B="$d%n" 1=1 C="$d%c" */
/* "mulu.w $d%1,$d%c" => ="%f.%t %x,%z" D="mulu" E="w" B="$d%n" 1=1 C="$d%c" */
/* "divs.w $d%1,$d%c; ext.w %z" => ="%f.%t %x,%z; ext.w %z" D="divs" E="w" B="$d%n" 1=1 C="$d%c" */
/* "divu.w $d%1,$d%c; and &0xffff,%z" => ="%f.%t %x,%z; and &0xffff,%z" D="divu" E="w" B="$d%n" 1=1 C="$d%c" */
/* "divs.w $d%1,$d%c; swap; ext.w %z" => ="%f.%t %x,%z; swap; ext.w %z" D="divs" E="w" B="$d%n" 1=1 C="$d%c" */
/* "divu.w $d%1,$d%c; swap; and &0xffff,%z" => ="%f.%t %x,%z; swap; and &0xffff,%z" D="divu" E="w" B="$d%n" 1=1 C="$d%c" */
/* "mov.l $d%0,%c($sp)" => ="mov.%t %y,%z" E="l" A="$d%n" 0=0 C="%c($sp)" */
/* "mov.l $a%0,%c($sp)" => ="mov.%t %y,%z" E="l" A="$a%n" 0=0 C="%c($sp)" */
/* "mov.b $d%1,($a%0)" => ="mov.%t %y,%z" E="b" A="$d%n" 0=1 C="($a%n)" 2=0 */
/* "mov.l $d%1,($a%0)" => ="mov.%t %y,%z" E="l" A="$d%n" 0=1 C="($a%n)" 2=0 */
/* "mov.w $d%1,($a%0)" => ="mov.%t %y,%z" E="w" A="$d%n" 0=1 C="($a%n)" 2=0 */
/* "mov.l $a%1,($a%0)" => ="mov.%t %y,%z" E="l" A="$a%n" 0=1 C="($a%n)" 2=0 */
/* "mov.b ($a%0),$d%c" => ="mov.%t %y,%z" E="b" A="($a%n)" 0=0 C="$d%c" */
/* "mov.l ($a%0),$d%c" => ="mov.%t %y,%z" E="l" A="($a%n)" 0=0 C="$d%c" */
/* "mov.w ($a%0),$d%c" => ="mov.%t %y,%z" E="w" A="($a%n)" 0=0 C="$d%c" */
/* "mov.l ($a%0),$a%c" => ="mov.%t %y,%z" E="l" A="($a%n)" 0=0 C="$a%c" */
/* "cmp.l $d%0,$d%1; beq L%0" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" 0=0 B="$d%n" 1=1 D="eq" C="L%c" c=a */
/* "cmp.l $d%0,$d%1; bge L%0" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" 0=0 B="$d%n" 1=1 D="ge" C="L%c" c=a */
/* "cmp.l $d%0,$d%1; bgt L%0" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" 0=0 B="$d%n" 1=1 D="gt" C="L%c" c=a */
/* "cmp.l $d%0,$d%1; ble L%0" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" 0=0 B="$d%n" 1=1 D="le" C="L%c" c=a */
/* "cmp.l $d%0,$d%1; blt L%0" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" 0=0 B="$d%n" 1=1 D="lt" C="L%c" c=a */
/* "cmp.l $d%0,$d%1; bne L%0" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" 0=0 B="$d%n" 1=1 D="ne" C="L%c" c=a */
/* "cmp.l $d%0,$d%1; beq L%0" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" 0=0 B="$d%n" 1=1 D="eq" C="L%c" c=a */
/* "cmp.l $d%0,$d%1; bcs L%0" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" 0=0 B="$d%n" 1=1 D="cs" C="L%c" c=a */
/* "cmp.l $d%0,$d%1; bne L%0" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" 0=0 B="$d%n" 1=1 D="ne" C="L%c" c=a */
/* "cmp.l $d%0,$d%1; bls L%0" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" 0=0 B="$d%n" 1=1 D="ls" C="L%c" c=a */
/* "cmp.l $d%0,$d%1; bhi L%0" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" 0=0 B="$d%n" 1=1 D="hi" C="L%c" c=a */
/* "cmp.l $d%0,$d%1; bcc L%0" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" 0=0 B="$d%n" 1=1 D="cc" C="L%c" c=a */
/* "mov.b &%0,$d%c" => ="mov.%t %y,%z" E="b" A="&%c" a=a C="$d%c" */
/* "mov.l &%0,$d%c" => ="mov.%t %y,%z" E="l" A="&%c" a=a C="$d%c" */
/* "mov.w &%0,$d%c" => ="mov.%t %y,%z" E="w" A="&%c" a=a C="$d%c" */
/* "mov.l &%0,$d%c" => ="mov.%t %y,%z" E="l" A="&%c" a=a C="$d%c" */
/* "mov.l &%0,$a%c" => ="mov.%t %y,%z" E="l" A="&%c" a=a C="$a%c" */
/* "ext.w $d%c; ext.l $d%c" => ="ext.w %z; ext.l %z" C="$d%c" C="$d%c" */
/* "ext.l $d%c" => ="%f.%t %z" D="ext" E="l" C="$d%c" */
/* "neg.l $d%c" => ="%f.%t %z" D="neg" E="l" C="$d%c" */
/* "not.l $d%c" => ="%f.%t %z" D="not" E="l" C="$d%c" */
/* "add.l $d%n,$a%c" => ="%f.%t %x,%z" D="add" E="l" B="$d%n" C="$a%c" */
/* "sub.l $d%n,$a%c" => ="%f.%t %x,%z" D="sub" E="l" B="$d%n" C="$a%c" */
/* "mov.%t $d%c,$d%c" => ="mov.%t %y,%z" A="$d%c" C="$d%c" */
/* "mov.%t $a%c,$a%c" => ="mov.%t %y,%z" A="$a%c" C="$a%c" */
/* "mov.%t $a%n,$a%c" => ="mov.%t %y,%z" A="$a%n" C="$a%c" */
/* "mov.%t %y,0($sp)" => ="mov.%t %y,%z" C="%c($sp)" c="0" */
/* "add.l &%c,$a%c" => ="%f.%t %x,%z" D="add" E="l" B="&%c" C="$a%c" */
/* "lea %c($a%n),%z" => ="lea %y,%z" A="%c($a%n)" */
/* "mov.l $a%n,%z" => ="mov.%t %y,%z" E="l" A="$a%n" */
/* "lea _%c,%z" => ="lea %y,%z" 0A="_%c" */
/* "bra ($a%n)" => ="bra %z" C="($a%n)" */
/* "lea _%c,%z" => ="lea %y,%z" 2A="_%c" */
/* "bra L%c" => ="bra %z" C="L%c" */
/* "add.l $a%n,$a%c" => ="%f.%t %x,%z" D="add" E="l" B="$a%n" C="$a%c" */
/* "lea 0($a%n,$d%i.l),%z" => ="lea %y,%z" A="0($a%n,$d%i.l)" */
/* "lea %c($a%n),$a%c" => ="lea %y,%z" A="%c($a%n)" C="$a%c" */
/* "mov.%t $a%c,%z" => ="mov.%t %y,%z" 0A="$a%c" */
/* "add.l &%c,%z" => ="%f.%t %x,%z" D="add" E="l" B="&%c" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" A="$d%n" */
/* "mov.l $d%n,%z" => ="mov.%t %y,%z" 0E="l" 0A="$d%n" */
/* "mov.%t $a%n,%z" => ="mov.%t %y,%z" A="$a%n" */
/* "mov.l $a%n,%z" => ="mov.%t %y,%z" 0E="l" 0A="$a%n" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" A="$d%n" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" A="$d%n" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" A="$d%n" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" A="$d%n" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" A="$d%n" */
/* "mov.%t $a%n,%z" => ="mov.%t %y,%z" A="$a%n" */
/* "mov.%t $a%n,%z" => ="mov.%t %y,%z" A="$a%n" */
/* "mov.%t $a%n,%z" => ="mov.%t %y,%z" A="$a%n" */
/* "mov.%t $a%n,%z" => ="mov.%t %y,%z" A="$a%n" */
/* "mov.%t $a%n,%z" => ="mov.%t %y,%z" A="$a%n" */
/* "add.%t $d%n,$d%c" => ="%f.%t %x,%z" D="add" B="$d%n" C="$d%c" */
/* "add.%t $d%n,$d%c" => ="%f.%t %x,%z" D="add" B="$d%n" C="$d%c" */
/* "add.%t $d%n,$d%c" => ="%f.%t %x,%z" D="add" B="$d%n" C="$d%c" */
/* "add.%t $d%n,$d%c" => ="%f.%t %x,%z" D="add" B="$d%n" C="$d%c" */
/* "add.%t $d%n,$d%c" => ="%f.%t %x,%z" D="add" B="$d%n" C="$d%c" */
/* "add.%t $d%n,$a%c" => ="%f.%t %x,%z" D="add" B="$d%n" C="$a%c" */
/* "add.%t $d%n,$a%c" => ="%f.%t %x,%z" D="add" B="$d%n" C="$a%c" */
/* "add.%t $d%n,$a%c" => ="%f.%t %x,%z" D="add" B="$d%n" C="$a%c" */
/* "add.%t $d%n,$a%c" => ="%f.%t %x,%z" D="add" B="$d%n" C="$a%c" */
/* "add.%t $d%n,$a%c" => ="%f.%t %x,%z" D="add" B="$d%n" C="$a%c" */
/* "add.%t $a%n,$d%c" => ="%f.%t %x,%z" D="add" B="$a%n" C="$d%c" */
/* "add.%t $a%n,$d%c" => ="%f.%t %x,%z" D="add" B="$a%n" C="$d%c" */
/* "add.%t $a%n,$d%c" => ="%f.%t %x,%z" D="add" B="$a%n" C="$d%c" */
/* "add.%t $a%n,$d%c" => ="%f.%t %x,%z" D="add" B="$a%n" C="$d%c" */
/* "add.%t $a%n,$d%c" => ="%f.%t %x,%z" D="add" B="$a%n" C="$d%c" */
/* "add.%t $a%n,$a%c" => ="%f.%t %x,%z" D="add" B="$a%n" C="$a%c" */
/* "add.%t $a%n,$a%c" => ="%f.%t %x,%z" D="add" B="$a%n" C="$a%c" */
/* "add.%t $a%n,$a%c" => ="%f.%t %x,%z" D="add" B="$a%n" C="$a%c" */
/* "add.%t $a%n,$a%c" => ="%f.%t %x,%z" D="add" B="$a%n" C="$a%c" */
/* "add.%t $a%n,$a%c" => ="%f.%t %x,%z" D="add" B="$a%n" C="$a%c" */
/* "sub.%t $d%n,$d%c" => ="%f.%t %x,%z" D="sub" B="$d%n" C="$d%c" */
/* "sub.%t $d%n,$d%c" => ="%f.%t %x,%z" D="sub" B="$d%n" C="$d%c" */
/* "sub.%t $d%n,$d%c" => ="%f.%t %x,%z" D="sub" B="$d%n" C="$d%c" */
/* "sub.%t $d%n,$d%c" => ="%f.%t %x,%z" D="sub" B="$d%n" C="$d%c" */
/* "sub.%t $d%n,$d%c" => ="%f.%t %x,%z" D="sub" B="$d%n" C="$d%c" */
/* "sub.%t $d%n,$a%c" => ="%f.%t %x,%z" D="sub" B="$d%n" C="$a%c" */
/* "sub.%t $d%n,$a%c" => ="%f.%t %x,%z" D="sub" B="$d%n" C="$a%c" */
/* "sub.%t $d%n,$a%c" => ="%f.%t %x,%z" D="sub" B="$d%n" C="$a%c" */
/* "sub.%t $d%n,$a%c" => ="%f.%t %x,%z" D="sub" B="$d%n" C="$a%c" */
/* "sub.%t $d%n,$a%c" => ="%f.%t %x,%z" D="sub" B="$d%n" C="$a%c" */
/* "sub.%t $a%n,$d%c" => ="%f.%t %x,%z" D="sub" B="$a%n" C="$d%c" */
/* "sub.%t $a%n,$d%c" => ="%f.%t %x,%z" D="sub" B="$a%n" C="$d%c" */
/* "sub.%t $a%n,$d%c" => ="%f.%t %x,%z" D="sub" B="$a%n" C="$d%c" */
/* "sub.%t $a%n,$d%c" => ="%f.%t %x,%z" D="sub" B="$a%n" C="$d%c" */
/* "sub.%t $a%n,$d%c" => ="%f.%t %x,%z" D="sub" B="$a%n" C="$d%c" */
/* "sub.%t $a%n,$a%c" => ="%f.%t %x,%z" D="sub" B="$a%n" C="$a%c" */
/* "sub.%t $a%n,$a%c" => ="%f.%t %x,%z" D="sub" B="$a%n" C="$a%c" */
/* "sub.%t $a%n,$a%c" => ="%f.%t %x,%z" D="sub" B="$a%n" C="$a%c" */
/* "sub.%t $a%n,$a%c" => ="%f.%t %x,%z" D="sub" B="$a%n" C="$a%c" */
/* "sub.%t $a%n,$a%c" => ="%f.%t %x,%z" D="sub" B="$a%n" C="$a%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t %c($a%n),%z" => ="mov.%t %y,%z" 1A="%c($a%n)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t %c($a%n),%z" => ="mov.%t %y,%z" 1A="%c($a%n)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t %c($a%n),%z" => ="mov.%t %y,%z" 1A="%c($a%n)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t %c($a%n),%z" => ="mov.%t %y,%z" 1A="%c($a%n)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t %c($a%n),%z" => ="mov.%t %y,%z" 1A="%c($a%n)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t ($a%n),%z" => ="mov.%t %y,%z" 1A="($a%n)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t ($a%n),%z" => ="mov.%t %y,%z" 1A="($a%n)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t ($a%n),%z" => ="mov.%t %y,%z" 1A="($a%n)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t ($a%n),%z" => ="mov.%t %y,%z" 1A="($a%n)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t ($a%n),%z" => ="mov.%t %y,%z" 1A="($a%n)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t _%c,%z" => ="mov.%t %y,%z" 1A="_%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t _%c,%z" => ="mov.%t %y,%z" 1A="_%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t _%c,%z" => ="mov.%t %y,%z" 1A="_%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t _%c,%z" => ="mov.%t %y,%z" 1A="_%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t _%c,%z" => ="mov.%t %y,%z" 1A="_%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t &%c,%z" => ="mov.%t %y,%z" 1A="&%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t &%c,%z" => ="mov.%t %y,%z" 1A="&%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t &%c,%z" => ="mov.%t %y,%z" 1A="&%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t &%c,%z" => ="mov.%t %y,%z" 1A="&%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t &%c,%z" => ="mov.%t %y,%z" 1A="&%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t $d%c,%z" => ="mov.%t %y,%z" 1A="$d%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t $d%c,%z" => ="mov.%t %y,%z" 1A="$d%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t $d%c,%z" => ="mov.%t %y,%z" 1A="$d%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t $d%c,%z" => ="mov.%t %y,%z" 1A="$d%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t $d%c,%z" => ="mov.%t %y,%z" 1A="$d%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" 1A="$d%n" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" 1A="$d%n" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" 1A="$d%n" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" 1A="$d%n" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" 1A="$d%n" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t &_%c,%z" => ="mov.%t %y,%z" 1A="&_%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t &_%c,%z" => ="mov.%t %y,%z" 1A="&_%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t &_%c,%z" => ="mov.%t %y,%z" 1A="&_%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t &_%c,%z" => ="mov.%t %y,%z" 1A="&_%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t &_%c,%z" => ="mov.%t %y,%z" 1A="&_%c" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t %c($sp),%z" => ="mov.%t %y,%z" 1A="%c($sp)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t %c($sp),%z" => ="mov.%t %y,%z" 1A="%c($sp)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t %c($sp),%z" => ="mov.%t %y,%z" 1A="%c($sp)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t %c($sp),%z" => ="mov.%t %y,%z" 1A="%c($sp)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t %c($sp),%z" => ="mov.%t %y,%z" 1A="%c($sp)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t %c($a6),%z" => ="mov.%t %y,%z" 1A="%c($a6)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t %c($a6),%z" => ="mov.%t %y,%z" 1A="%c($a6)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t %c($a6),%z" => ="mov.%t %y,%z" 1A="%c($a6)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t %c($a6),%z" => ="mov.%t %y,%z" 1A="%c($a6)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t %c($a6),%z" => ="mov.%t %y,%z" 1A="%c($a6)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t _%c($a%n),%z" => ="mov.%t %y,%z" 1A="_%c($a%n)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t _%c($a%n),%z" => ="mov.%t %y,%z" 1A="_%c($a%n)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t _%c($a%n),%z" => ="mov.%t %y,%z" 1A="_%c($a%n)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t _%c($a%n),%z" => ="mov.%t %y,%z" 1A="_%c($a%n)" */
/* "and.%t $d%n,$d%c" => ="%f.%t %x,%z" D="and" B="$d%n" C="$d%c" */
/* "mov.%t _%c($a%n),%z" => ="mov.%t %y,%z" 1A="_%c($a%n)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t %c($a%n),%z" => ="mov.%t %y,%z" 1A="%c($a%n)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t %c($a%n),%z" => ="mov.%t %y,%z" 1A="%c($a%n)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t %c($a%n),%z" => ="mov.%t %y,%z" 1A="%c($a%n)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t %c($a%n),%z" => ="mov.%t %y,%z" 1A="%c($a%n)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t %c($a%n),%z" => ="mov.%t %y,%z" 1A="%c($a%n)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t ($a%n),%z" => ="mov.%t %y,%z" 1A="($a%n)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t ($a%n),%z" => ="mov.%t %y,%z" 1A="($a%n)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t ($a%n),%z" => ="mov.%t %y,%z" 1A="($a%n)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t ($a%n),%z" => ="mov.%t %y,%z" 1A="($a%n)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t ($a%n),%z" => ="mov.%t %y,%z" 1A="($a%n)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t _%c,%z" => ="mov.%t %y,%z" 1A="_%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t _%c,%z" => ="mov.%t %y,%z" 1A="_%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t _%c,%z" => ="mov.%t %y,%z" 1A="_%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t _%c,%z" => ="mov.%t %y,%z" 1A="_%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t _%c,%z" => ="mov.%t %y,%z" 1A="_%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t &%c,%z" => ="mov.%t %y,%z" 1A="&%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t &%c,%z" => ="mov.%t %y,%z" 1A="&%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t &%c,%z" => ="mov.%t %y,%z" 1A="&%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t &%c,%z" => ="mov.%t %y,%z" 1A="&%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t &%c,%z" => ="mov.%t %y,%z" 1A="&%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t $d%c,%z" => ="mov.%t %y,%z" 1A="$d%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t $d%c,%z" => ="mov.%t %y,%z" 1A="$d%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t $d%c,%z" => ="mov.%t %y,%z" 1A="$d%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t $d%c,%z" => ="mov.%t %y,%z" 1A="$d%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t $d%c,%z" => ="mov.%t %y,%z" 1A="$d%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" 1A="$d%n" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" 1A="$d%n" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" 1A="$d%n" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" 1A="$d%n" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" 1A="$d%n" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t &_%c,%z" => ="mov.%t %y,%z" 1A="&_%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t &_%c,%z" => ="mov.%t %y,%z" 1A="&_%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t &_%c,%z" => ="mov.%t %y,%z" 1A="&_%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t &_%c,%z" => ="mov.%t %y,%z" 1A="&_%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t &_%c,%z" => ="mov.%t %y,%z" 1A="&_%c" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t %c($sp),%z" => ="mov.%t %y,%z" 1A="%c($sp)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t %c($sp),%z" => ="mov.%t %y,%z" 1A="%c($sp)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t %c($sp),%z" => ="mov.%t %y,%z" 1A="%c($sp)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t %c($sp),%z" => ="mov.%t %y,%z" 1A="%c($sp)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t %c($sp),%z" => ="mov.%t %y,%z" 1A="%c($sp)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t %c($a6),%z" => ="mov.%t %y,%z" 1A="%c($a6)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t %c($a6),%z" => ="mov.%t %y,%z" 1A="%c($a6)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t %c($a6),%z" => ="mov.%t %y,%z" 1A="%c($a6)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t %c($a6),%z" => ="mov.%t %y,%z" 1A="%c($a6)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t %c($a6),%z" => ="mov.%t %y,%z" 1A="%c($a6)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t _%c($a%n),%z" => ="mov.%t %y,%z" 1A="_%c($a%n)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t _%c($a%n),%z" => ="mov.%t %y,%z" 1A="_%c($a%n)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t _%c($a%n),%z" => ="mov.%t %y,%z" 1A="_%c($a%n)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t _%c($a%n),%z" => ="mov.%t %y,%z" 1A="_%c($a%n)" */
/* "or.%t $d%n,$d%c" => ="%f.%t %x,%z" D="or" B="$d%n" C="$d%c" */
/* "mov.%t _%c($a%n),%z" => ="mov.%t %y,%z" 1A="_%c($a%n)" */
/* "cmp.%t $a%c,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%c" B="$d%n" */
/* "cmp.%t $a%c,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%c" B="$d%n" */
/* "cmp.%t $a%c,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%c" B="$d%n" */
/* "cmp.%t $a%c,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%c" B="$d%n" */
/* "cmp.%t $a%c,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%c" B="$d%n" */
/* "cmp.%t $a%c,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%c" B="$a%n" */
/* "cmp.%t $a%c,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%c" B="$a%n" */
/* "cmp.%t $a%c,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%c" B="$a%n" */
/* "cmp.%t $a%c,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%c" B="$a%n" */
/* "cmp.%t $a%c,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%c" B="$a%n" */
/* "cmp.%t $a%n,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="$d%n" */
/* "cmp.%t $a%n,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="$d%n" */
/* "cmp.%t $a%n,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="$d%n" */
/* "cmp.%t $a%n,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="$d%n" */
/* "cmp.%t $a%n,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="$d%n" */
/* "cmp.%t $a%n,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="$a%n" */
/* "cmp.%t $a%n,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="$a%n" */
/* "cmp.%t $a%n,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="$a%n" */
/* "cmp.%t $a%n,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="$a%n" */
/* "cmp.%t $a%n,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="$a%n" */
/* "cmp.%t $d%c,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%c" B="$d%n" */
/* "cmp.%t $d%c,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%c" B="$d%n" */
/* "cmp.%t $d%c,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%c" B="$d%n" */
/* "cmp.%t $d%c,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%c" B="$d%n" */
/* "cmp.%t $d%c,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%c" B="$d%n" */
/* "cmp.%t $d%c,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%c" B="$a%n" */
/* "cmp.%t $d%c,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%c" B="$a%n" */
/* "cmp.%t $d%c,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%c" B="$a%n" */
/* "cmp.%t $d%c,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%c" B="$a%n" */
/* "cmp.%t $d%c,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%c" B="$a%n" */
/* "cmp.%t $d%n,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="$d%n" */
/* "cmp.%t $d%n,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="$d%n" */
/* "cmp.%t $d%n,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="$d%n" */
/* "cmp.%t $d%n,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="$d%n" */
/* "cmp.%t $d%n,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="$d%n" */
/* "cmp.%t $d%n,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="$a%n" */
/* "cmp.%t $d%n,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="$a%n" */
/* "cmp.%t $d%n,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="$a%n" */
/* "cmp.%t $d%n,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="$a%n" */
/* "cmp.%t $d%n,$a%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="$a%n" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t %c($a%n),%z" => ="mov.%t %y,%z" 0A="%c($a%n)" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t ($a%n),%z" => ="mov.%t %y,%z" 0A="($a%n)" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t _%c,%z" => ="mov.%t %y,%z" 0A="_%c" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t $a%c,%z" => ="mov.%t %y,%z" 0A="$a%c" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t $a%n,%z" => ="mov.%t %y,%z" 0A="$a%n" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t $d%c,%z" => ="mov.%t %y,%z" 0A="$d%c" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" 0A="$d%n" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t %c($sp),%z" => ="mov.%t %y,%z" 0A="%c($sp)" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t %c($a6),%z" => ="mov.%t %y,%z" 0A="%c($a6)" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t _%c($a%n),%z" => ="mov.%t %y,%z" 0A="_%c($a%n)" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t %c($a%n),%z" => ="mov.%t %y,%z" 0A="%c($a%n)" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t ($a%n),%z" => ="mov.%t %y,%z" 0A="($a%n)" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t _%c,%z" => ="mov.%t %y,%z" 0A="_%c" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t $a%c,%z" => ="mov.%t %y,%z" 0A="$a%c" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t $a%n,%z" => ="mov.%t %y,%z" 0A="$a%n" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t $d%c,%z" => ="mov.%t %y,%z" 0A="$d%c" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" 0A="$d%n" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t %c($sp),%z" => ="mov.%t %y,%z" 0A="%c($sp)" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t %c($a6),%z" => ="mov.%t %y,%z" 0A="%c($a6)" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t _%c($a%n),%z" => ="mov.%t %y,%z" 0A="_%c($a%n)" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t %c($a%n),%z" => ="mov.%t %y,%z" 0A="%c($a%n)" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t ($a%n),%z" => ="mov.%t %y,%z" 0A="($a%n)" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t _%c,%z" => ="mov.%t %y,%z" 0A="_%c" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t $a%c,%z" => ="mov.%t %y,%z" 0A="$a%c" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t $a%n,%z" => ="mov.%t %y,%z" 0A="$a%n" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t $d%c,%z" => ="mov.%t %y,%z" 0A="$d%c" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" 0A="$d%n" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t %c($sp),%z" => ="mov.%t %y,%z" 0A="%c($sp)" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t %c($a6),%z" => ="mov.%t %y,%z" 0A="%c($a6)" */
/* "cmp.%t $d%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" */
/* "mov.%t _%c($a%n),%z" => ="mov.%t %y,%z" 0A="_%c($a%n)" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t %c($a%n),%z" => ="mov.%t %y,%z" 0A="%c($a%n)" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t ($a%n),%z" => ="mov.%t %y,%z" 0A="($a%n)" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t _%c,%z" => ="mov.%t %y,%z" 0A="_%c" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t $a%c,%z" => ="mov.%t %y,%z" 0A="$a%c" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t $a%n,%z" => ="mov.%t %y,%z" 0A="$a%n" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t $d%c,%z" => ="mov.%t %y,%z" 0A="$d%c" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" 0A="$d%n" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t %c($sp),%z" => ="mov.%t %y,%z" 0A="%c($sp)" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t %c($a6),%z" => ="mov.%t %y,%z" 0A="%c($a6)" */
/* "cmp.%t $a%n,&%c; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$a%n" B="&%c" */
/* "mov.%t _%c($a%n),%z" => ="mov.%t %y,%z" 0A="_%c($a%n)" */
/* "cmp.l $d%n,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" B="$d%n" */
/* "cmp.l $d%n,$d%n; b%f %z" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" B="$d%n" */
/* "ext.l %z" => ="%f.%t %z" 0D="ext" 0E="l" */
/* "ext.l %z" => ="%f.%t %z" 1D="ext" 1E="l" */
/* "cmp.l $d%n,&%c; beq %z" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" B="&%c" D="eq" */
/* "cmp.l $d%n,&%c; bge %z" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" B="&%c" D="ge" */
/* "cmp.l $d%n,&%c; bgt %z" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" B="&%c" D="gt" */
/* "cmp.l $d%n,&%c; ble %z" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" B="&%c" D="le" */
/* "cmp.l $d%n,&%c; blt %z" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" B="&%c" D="lt" */
/* "cmp.l $d%n,&%c; bne %z" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" B="&%c" D="ne" */
/* "cmp.l $d%n,&%c; beq %z" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" B="&%c" D="eq" */
/* "ext.l %z" => ="%f.%t %z" 0D="ext" 0E="l" */
/* "cmp.l $d%n,&%c; bge %z" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" B="&%c" D="ge" */
/* "ext.l %z" => ="%f.%t %z" 0D="ext" 0E="l" */
/* "cmp.l $d%n,&%c; bgt %z" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" B="&%c" D="gt" */
/* "ext.l %z" => ="%f.%t %z" 0D="ext" 0E="l" */
/* "cmp.l $d%n,&%c; ble %z" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" B="&%c" D="le" */
/* "ext.l %z" => ="%f.%t %z" 0D="ext" 0E="l" */
/* "cmp.l $d%n,&%c; blt %z" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" B="&%c" D="lt" */
/* "ext.l %z" => ="%f.%t %z" 0D="ext" 0E="l" */
/* "cmp.l $d%n,&%c; bne %z" => ="cmp.%t %y,%x; b%f %z" E="l" A="$d%n" B="&%c" D="ne" */
/* "ext.l %z" => ="%f.%t %z" 0D="ext" 0E="l" */
/* "asl.%t $d%n,$d%c" => ="%f.%t %x,%z" D="asl" B="$d%n" C="$d%c" */
/* "mov.%t &4,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="4" */
/* "asl.%t $d%n,$d%c" => ="%f.%t %x,%z" D="asl" B="$d%n" C="$d%c" */
/* "mov.%t &8,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="8" */
/* "asl.%t $d%n,$d%c" => ="%f.%t %x,%z" D="asl" B="$d%n" C="$d%c" */
/* "mov.%t &3,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="3" */
/* "asl.%t $d%n,$d%c" => ="%f.%t %x,%z" D="asl" B="$d%n" C="$d%c" */
/* "mov.%t &7,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="7" */
/* "asl.%t $d%n,$d%c" => ="%f.%t %x,%z" D="asl" B="$d%n" C="$d%c" */
/* "mov.%t &2,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="2" */
/* "asl.%t $d%n,$d%c" => ="%f.%t %x,%z" D="asl" B="$d%n" C="$d%c" */
/* "mov.%t &6,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="6" */
/* "asl.%t $d%n,$d%c" => ="%f.%t %x,%z" D="asl" B="$d%n" C="$d%c" */
/* "mov.%t &1,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="1" */
/* "asl.%t $d%n,$d%c" => ="%f.%t %x,%z" D="asl" B="$d%n" C="$d%c" */
/* "mov.%t &5,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="5" */
/* "asr.%t $d%n,$d%c" => ="%f.%t %x,%z" D="asr" B="$d%n" C="$d%c" */
/* "mov.%t &4,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="4" */
/* "asr.%t $d%n,$d%c" => ="%f.%t %x,%z" D="asr" B="$d%n" C="$d%c" */
/* "mov.%t &8,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="8" */
/* "asr.%t $d%n,$d%c" => ="%f.%t %x,%z" D="asr" B="$d%n" C="$d%c" */
/* "mov.%t &3,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="3" */
/* "asr.%t $d%n,$d%c" => ="%f.%t %x,%z" D="asr" B="$d%n" C="$d%c" */
/* "mov.%t &7,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="7" */
/* "asr.%t $d%n,$d%c" => ="%f.%t %x,%z" D="asr" B="$d%n" C="$d%c" */
/* "mov.%t &2,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="2" */
/* "asr.%t $d%n,$d%c" => ="%f.%t %x,%z" D="asr" B="$d%n" C="$d%c" */
/* "mov.%t &6,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="6" */
/* "asr.%t $d%n,$d%c" => ="%f.%t %x,%z" D="asr" B="$d%n" C="$d%c" */
/* "mov.%t &1,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="1" */
/* "asr.%t $d%n,$d%c" => ="%f.%t %x,%z" D="asr" B="$d%n" C="$d%c" */
/* "mov.%t &5,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="5" */
/* "lsl.%t $d%n,$d%c" => ="%f.%t %x,%z" D="lsl" B="$d%n" C="$d%c" */
/* "mov.%t &4,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="4" */
/* "lsl.%t $d%n,$d%c" => ="%f.%t %x,%z" D="lsl" B="$d%n" C="$d%c" */
/* "mov.%t &8,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="8" */
/* "lsl.%t $d%n,$d%c" => ="%f.%t %x,%z" D="lsl" B="$d%n" C="$d%c" */
/* "mov.%t &3,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="3" */
/* "lsl.%t $d%n,$d%c" => ="%f.%t %x,%z" D="lsl" B="$d%n" C="$d%c" */
/* "mov.%t &7,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="7" */
/* "lsl.%t $d%n,$d%c" => ="%f.%t %x,%z" D="lsl" B="$d%n" C="$d%c" */
/* "mov.%t &2,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="2" */
/* "lsl.%t $d%n,$d%c" => ="%f.%t %x,%z" D="lsl" B="$d%n" C="$d%c" */
/* "mov.%t &6,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="6" */
/* "lsl.%t $d%n,$d%c" => ="%f.%t %x,%z" D="lsl" B="$d%n" C="$d%c" */
/* "mov.%t &1,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="1" */
/* "lsl.%t $d%n,$d%c" => ="%f.%t %x,%z" D="lsl" B="$d%n" C="$d%c" */
/* "mov.%t &5,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="5" */
/* "lsr.%t $d%n,$d%c" => ="%f.%t %x,%z" D="lsr" B="$d%n" C="$d%c" */
/* "mov.%t &4,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="4" */
/* "lsr.%t $d%n,$d%c" => ="%f.%t %x,%z" D="lsr" B="$d%n" C="$d%c" */
/* "mov.%t &8,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="8" */
/* "lsr.%t $d%n,$d%c" => ="%f.%t %x,%z" D="lsr" B="$d%n" C="$d%c" */
/* "mov.%t &3,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="3" */
/* "lsr.%t $d%n,$d%c" => ="%f.%t %x,%z" D="lsr" B="$d%n" C="$d%c" */
/* "mov.%t &7,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="7" */
/* "lsr.%t $d%n,$d%c" => ="%f.%t %x,%z" D="lsr" B="$d%n" C="$d%c" */
/* "mov.%t &2,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="2" */
/* "lsr.%t $d%n,$d%c" => ="%f.%t %x,%z" D="lsr" B="$d%n" C="$d%c" */
/* "mov.%t &6,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="6" */
/* "lsr.%t $d%n,$d%c" => ="%f.%t %x,%z" D="lsr" B="$d%n" C="$d%c" */
/* "mov.%t &1,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="1" */
/* "lsr.%t $d%n,$d%c" => ="%f.%t %x,%z" D="lsr" B="$d%n" C="$d%c" */
/* "mov.%t &5,%z" => ="mov.%t %y,%z" 1A="&%c" 1a="5" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" A="$d%n" */
/* "add.%t $a%n,%z" => ="%f.%t %x,%z" 0D="add" 0B="$a%n" */
/* "add.%t %x,%z" => ="%f.%t %x,%z" D="add" */
/* "mov.%t $a%n,%z" => ="mov.%t %y,%z" A="$a%n" */
/* "add.%t $a%n,%z" => ="%f.%t %x,%z" 0D="add" 0B="$a%n" */
/* "add.%t %x,%z" => ="%f.%t %x,%z" D="add" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" A="$d%n" */
/* "sub.%t $a%n,%z" => ="%f.%t %x,%z" 0D="sub" 0B="$a%n" */
/* "sub.%t %x,%z" => ="%f.%t %x,%z" D="sub" */
/* "mov.%t $a%n,%z" => ="mov.%t %y,%z" A="$a%n" */
/* "sub.%t $a%n,%z" => ="%f.%t %x,%z" 0D="sub" 0B="$a%n" */
/* "sub.%t %x,%z" => ="%f.%t %x,%z" D="sub" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" A="$d%n" */
/* "add.%t &%c,%z" => ="%f.%t %x,%z" 0D="add" 0B="&%c" */
/* "add.%t %x,%z" => ="%f.%t %x,%z" D="add" */
/* "mov.%t $a%n,%z" => ="mov.%t %y,%z" A="$a%n" */
/* "add.%t &%c,%z" => ="%f.%t %x,%z" 0D="add" 0B="&%c" */
/* "add.%t %x,%z" => ="%f.%t %x,%z" D="add" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" A="$d%n" */
/* "sub.%t &%c,%z" => ="%f.%t %x,%z" 0D="sub" 0B="&%c" */
/* "sub.%t %x,%z" => ="%f.%t %x,%z" D="sub" */
/* "mov.%t $a%n,%z" => ="mov.%t %y,%z" A="$a%n" */
/* "sub.%t &%c,%z" => ="%f.%t %x,%z" 0D="sub" 0B="&%c" */
/* "sub.%t %x,%z" => ="%f.%t %x,%z" D="sub" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" A="$d%n" */
/* "add.%t $d%n,%z" => ="%f.%t %x,%z" 0D="add" 0B="$d%n" */
/* "add.%t %x,%z" => ="%f.%t %x,%z" D="add" */
/* "mov.%t $a%n,%z" => ="mov.%t %y,%z" A="$a%n" */
/* "add.%t $d%n,%z" => ="%f.%t %x,%z" 0D="add" 0B="$d%n" */
/* "add.%t %x,%z" => ="%f.%t %x,%z" D="add" */
/* "mov.%t $d%n,%z" => ="mov.%t %y,%z" A="$d%n" */
/* "sub.%t $d%n,%z" => ="%f.%t %x,%z" 0D="sub" 0B="$d%n" */
/* "sub.%t %x,%z" => ="%f.%t %x,%z" D="sub" */
/* "mov.%t $a%n,%z" => ="mov.%t %y,%z" A="$a%n" */
/* "sub.%t $d%n,%z" => ="%f.%t %x,%z" 0D="sub" 0B="$d%n" */
/* "sub.%t %x,%z" => ="%f.%t %x,%z" D="sub" */
/* "mov.%t &0,%c($a%n)" => ="mov.%t %y,%z" A="&%c" a="0" C="%c($a%n)" */
/* "mov.%t &0,($a%n)" => ="mov.%t %y,%z" A="&%c" a="0" C="($a%n)" */
/* "mov.%t &0,_%c" => ="mov.%t %y,%z" A="&%c" a="0" C="_%c" */
/* "mov.%t &0,$d%c" => ="mov.%t %y,%z" A="&%c" a="0" C="$d%c" */
/* "mov.%t &0,$d%n" => ="mov.%t %y,%z" A="&%c" a="0" C="$d%n" */
/* "mov.%t &0,%c($sp)" => ="mov.%t %y,%z" A="&%c" a="0" C="%c($sp)" */
/* "mov.%t &0,%c($a6)" => ="mov.%t %y,%z" A="&%c" a="0" C="%c($a6)" */
/* "mov.%t &0,_%c($a%n)" => ="mov.%t %y,%z" A="&%c" a="0" C="_%c($a%n)" */
/* "cmp.%t %c($a%n),&0; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="%c($a%n)" B="&%c" b="0" */
/* "cmp.%t ($a%n),&0; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="($a%n)" B="&%c" b="0" */
/* "cmp.%t _%c,&0; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="_%c" B="&%c" b="0" */
/* "cmp.%t $d%c,&0; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%c" B="&%c" b="0" */
/* "cmp.%t $d%n,&0; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="$d%n" B="&%c" b="0" */
/* "cmp.%t %c($sp),&0; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="%c($sp)" B="&%c" b="0" */
/* "cmp.%t %c($a6),&0; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="%c($a6)" B="&%c" b="0" */
/* "cmp.%t _%c($a%n),&0; b%f %z" => ="cmp.%t %y,%x; b%f %z" A="_%c($a%n)" B="&%c" b="0" */
#include "c.h"

#ifdef DEBUG
#define dumptree(x,y) dumptree1(x,y,-1,0)
#else
#define dumptree(x,y)
#endif

static char *shortopcodes[] = {
/* 608 */  "%f%t %x,%z |mod",
/* 609 */  "jsr %y",
/* 610 */  "lea %z,$a1; mov.l %x,$d0; mov.w ($a0)+,($a1)+; sub.l &1,$d0; byte 0x6e,-6",
/* 611 */  "mov.l %x,$d0; mov.w ($a0)+,($a1)+; sub.l &1,$d0; byte 0x6e,-6",
/* 612 */  "# label",
/* 613 */  "# nop",
/* 614 */  "# ret",
/* 615 */  "%f.%t %x,%z",
/* 616 */  "%f.%t %x,%z |mod",
/* 617 */  "%f.%t %z",
/* 618 */  "clr.%t %z",
/* 619 */  "cmp.%t %y,%x; b%f %z",
/* 620 */  "bra %z",
/* 621 */  "lea %y,%z",
/* 622 */  "mov.%t %y,%z",
/* 623 */  "mov.%t %y,%z; %f.%t %x,%z",
/* 624 */  "mov.%t %y,%z; %f.%t %z",
/* 625 */  "tst.%t %y; b%f %z",
/* 626 */  "%z:",
/* 627 */  "ext.w %z; ext.l %z",
/* 628 */  "%f.%t %x,%z; ext.w %z",
/* 629 */  "%f.%t %x,%z; and &0xffff,%z",
/* 630 */  "%f.%t %x,%z; swap; ext.w %z",
/* 631 */  "%f.%t %x,%z; swap; and &0xffff,%z",
};
char **opcodes = shortopcodes-608;

char *asmstr0[] = {
	"(DUMMY)",
	"$a%n",
	"$a%c",
	"$d%n",
	"$d%c",
	"_%c",
	"_%c($a%n)",
	"_%c($sp)",
	"_%c($a6)",
	"%c($a%n)",
	"%c($sp)",
	"%c($a6)",
	"($a%n)",
	"($sp)",
	"($a6)",
	"&%c",
	"&%c/2",
	"&_%c",
	"L%c",
	"0($a%n,$d%i.l)",
};

char *asmstr3[] = {
	"(DUMMY)",
	"add",
	"and",
	"asl",
	"asr",
	"cc",
	"cs",
	"divs",
	"divu",
	"eor",
	"eq",
	"ext",
	"ge",
	"gt",
	"hi",
	"le",
	"ls",
	"lsl",
	"lsr",
	"lt",
	"muls",
	"mulu",
	"ne",
	"neg",
	"not",
	"or",
	"sub",
};

char *asmstr4[] = {
	"(DUMMY)",
	"b",
	"w",
	"l",
	"d",
	"s",
	"x",
};

struct symbol sym[] = {
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

char **asmstr[MAXINTS] = {
	asmstr0,
	asmstr0,
	asmstr0,
	asmstr3,
	asmstr4,
};

void initgen()
{
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

dclproto(static char *emit0,(char *, Node, int))
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
	case 1:   /* $a%n */
		*rbp++ = '$';
		*rbp++ = 'a';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 2:   /* $a%c */
		*rbp++ = '$';
		*rbp++ = 'a';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 3:   /* $d%n */
		*rbp++ = '$';
		*rbp++ = 'd';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 4:   /* $d%c */
		*rbp++ = '$';
		*rbp++ = 'd';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 5:   /* _%c */
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 6:   /* _%c($a%n) */
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = '$';
		*rbp++ = 'a';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
		return rbp;
	case 7:   /* _%c($sp) */
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = '$';
		*rbp++ = 's';
		*rbp++ = 'p';
		*rbp++ = ')';
		return rbp;
	case 8:   /* _%c($a6) */
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = '$';
		*rbp++ = 'a';
		*rbp++ = '6';
		*rbp++ = ')';
		return rbp;
	case 9:   /* %c($a%n) */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = '$';
		*rbp++ = 'a';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
		return rbp;
	case 10:   /* %c($sp) */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = '$';
		*rbp++ = 's';
		*rbp++ = 'p';
		*rbp++ = ')';
		return rbp;
	case 11:   /* %c($a6) */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = '$';
		*rbp++ = 'a';
		*rbp++ = '6';
		*rbp++ = ')';
		return rbp;
	case 12:   /* ($a%n) */
		*rbp++ = '(';
		*rbp++ = '$';
		*rbp++ = 'a';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
		return rbp;
	case 13:   /* ($sp) */
		*rbp++ = '(';
		*rbp++ = '$';
		*rbp++ = 's';
		*rbp++ = 'p';
		*rbp++ = ')';
		return rbp;
	case 14:   /* ($a6) */
		*rbp++ = '(';
		*rbp++ = '$';
		*rbp++ = 'a';
		*rbp++ = '6';
		*rbp++ = ')';
		return rbp;
	case 15:   /* &%c */
		*rbp++ = '&';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 16:   /* &%c/2 */
		*rbp++ = '&';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '/';
		*rbp++ = '2';
		return rbp;
	case 17:   /* &_%c */
		*rbp++ = '&';
		*rbp++ = '_';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 18:   /* L%c */
		*rbp++ = 'L';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 19:   /* 0($a%n,$d%i.l) */
		*rbp++ = '0';
		*rbp++ = '(';
		*rbp++ = '$';
		*rbp++ = 'a';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ',';
		*rbp++ = '$';
		*rbp++ = 'd';
		assert(a->kids[n+3]);
		assert(a->kids[n+3]->x.registered);
		assert(a->kids[n+3]->syms[RX]);
		for (cp = a->kids[n+3]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '.';
		*rbp++ = 'l';
		*rbp++ = ')';
		return rbp;
	}
	return rbp;
}

void emit(a)
register Node a;
{
	register char *cp, *rbp;

	for (; a; a = a->x.next) {
		switch (a->op) {
		case 608: L608:   /* %f%t %x,%z |mod */
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
		case 609: L609:   /* jsr %y */
			rbp = bp;
			*rbp++ = 'j';
			*rbp++ = 's';
			*rbp++ = 'r';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 610: L610:   /* lea %z,$a1; mov.l %x,$d0; mov.w ($a0)+,($a1)+; sub.l &1,$d0; byte 0x6e,-6 */
			rbp = bp;
			*rbp++ = 'l';
			*rbp++ = 'e';
			*rbp++ = 'a';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			*rbp++ = ',';
			*rbp++ = '$';
			*rbp++ = 'a';
			*rbp++ = '1';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = '.';
			*rbp++ = 'l';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			*rbp++ = '$';
			*rbp++ = 'd';
			*rbp++ = '0';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = '.';
			*rbp++ = 'w';
			*rbp++ = ' ';
			*rbp++ = '(';
			*rbp++ = '$';
			*rbp++ = 'a';
			*rbp++ = '0';
			*rbp++ = ')';
			*rbp++ = '+';
			*rbp++ = ',';
			*rbp++ = '(';
			*rbp++ = '$';
			*rbp++ = 'a';
			*rbp++ = '1';
			*rbp++ = ')';
			*rbp++ = '+';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 's';
			*rbp++ = 'u';
			*rbp++ = 'b';
			*rbp++ = '.';
			*rbp++ = 'l';
			*rbp++ = ' ';
			*rbp++ = '&';
			*rbp++ = '1';
			*rbp++ = ',';
			*rbp++ = '$';
			*rbp++ = 'd';
			*rbp++ = '0';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'b';
			*rbp++ = 'y';
			*rbp++ = 't';
			*rbp++ = 'e';
			*rbp++ = ' ';
			*rbp++ = '0';
			*rbp++ = 'x';
			*rbp++ = '6';
			*rbp++ = 'e';
			*rbp++ = ',';
			*rbp++ = '-';
			*rbp++ = '6';
			bp = rbp;
			outs("\n");
			break;
		case 611: L611:   /* mov.l %x,$d0; mov.w ($a0)+,($a1)+; sub.l &1,$d0; byte 0x6e,-6 */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = '.';
			*rbp++ = 'l';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			*rbp++ = '$';
			*rbp++ = 'd';
			*rbp++ = '0';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = '.';
			*rbp++ = 'w';
			*rbp++ = ' ';
			*rbp++ = '(';
			*rbp++ = '$';
			*rbp++ = 'a';
			*rbp++ = '0';
			*rbp++ = ')';
			*rbp++ = '+';
			*rbp++ = ',';
			*rbp++ = '(';
			*rbp++ = '$';
			*rbp++ = 'a';
			*rbp++ = '1';
			*rbp++ = ')';
			*rbp++ = '+';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 's';
			*rbp++ = 'u';
			*rbp++ = 'b';
			*rbp++ = '.';
			*rbp++ = 'l';
			*rbp++ = ' ';
			*rbp++ = '&';
			*rbp++ = '1';
			*rbp++ = ',';
			*rbp++ = '$';
			*rbp++ = 'd';
			*rbp++ = '0';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'b';
			*rbp++ = 'y';
			*rbp++ = 't';
			*rbp++ = 'e';
			*rbp++ = ' ';
			*rbp++ = '0';
			*rbp++ = 'x';
			*rbp++ = '6';
			*rbp++ = 'e';
			*rbp++ = ',';
			*rbp++ = '-';
			*rbp++ = '6';
			bp = rbp;
			outs("\n");
			break;
		case 612: L612:   /* # label */
			emitspecial(a);
			break;
		case 613: L613:   /* # nop */
			emitspecial(a);
			break;
		case 614: L614:   /* # ret */
			emitspecial(a);
			break;
		case 615: L615:   /* %f.%t %x,%z */
			rbp = bp;
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = '.';
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
		case 616: L616:   /* %f.%t %x,%z |mod */
			rbp = bp;
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = '.';
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
		case 617: L617:   /* %f.%t %z */
			rbp = bp;
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = '.';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 618: L618:   /* clr.%t %z */
			rbp = bp;
			*rbp++ = 'c';
			*rbp++ = 'l';
			*rbp++ = 'r';
			*rbp++ = '.';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 619: L619:   /* cmp.%t %y,%x; b%f %z */
			rbp = bp;
			*rbp++ = 'c';
			*rbp++ = 'm';
			*rbp++ = 'p';
			*rbp++ = '.';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
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
			bp = rbp;
			outs("\n");
			break;
		case 620: L620:   /* bra %z */
			rbp = bp;
			*rbp++ = 'b';
			*rbp++ = 'r';
			*rbp++ = 'a';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 621: L621:   /* lea %y,%z */
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
		case 622: L622:   /* mov.%t %y,%z */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = '.';
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
		case 623: L623:   /* mov.%t %y,%z; %f.%t %x,%z */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = '.';
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
			*rbp++ = '.';
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
		case 624: L624:   /* mov.%t %y,%z; %f.%t %z */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = '.';
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
			*rbp++ = '.';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 625: L625:   /* tst.%t %y; b%f %z */
			rbp = bp;
			*rbp++ = 't';
			*rbp++ = 's';
			*rbp++ = 't';
			*rbp++ = '.';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'b';
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 626: L626:   /* %z: */
			rbp = bp;
			rbp = emit0(rbp, a, 2);
			*rbp++ = ':';
			bp = rbp;
			outs("\n");
			break;
		case 627: L627:   /* ext.w %z; ext.l %z */
			rbp = bp;
			*rbp++ = 'e';
			*rbp++ = 'x';
			*rbp++ = 't';
			*rbp++ = '.';
			*rbp++ = 'w';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'e';
			*rbp++ = 'x';
			*rbp++ = 't';
			*rbp++ = '.';
			*rbp++ = 'l';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 628: L628:   /* %f.%t %x,%z; ext.w %z */
			rbp = bp;
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = '.';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'e';
			*rbp++ = 'x';
			*rbp++ = 't';
			*rbp++ = '.';
			*rbp++ = 'w';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 629: L629:   /* %f.%t %x,%z; and &0xffff,%z */
			rbp = bp;
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = '.';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'a';
			*rbp++ = 'n';
			*rbp++ = 'd';
			*rbp++ = ' ';
			*rbp++ = '&';
			*rbp++ = '0';
			*rbp++ = 'x';
			*rbp++ = 'f';
			*rbp++ = 'f';
			*rbp++ = 'f';
			*rbp++ = 'f';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 630: L630:   /* %f.%t %x,%z; swap; ext.w %z */
			rbp = bp;
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = '.';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 's';
			*rbp++ = 'w';
			*rbp++ = 'a';
			*rbp++ = 'p';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'e';
			*rbp++ = 'x';
			*rbp++ = 't';
			*rbp++ = '.';
			*rbp++ = 'w';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		case 631: L631:   /* %f.%t %x,%z; swap; and &0xffff,%z */
			rbp = bp;
			assert(a->x.ints[3]);
			for (cp = asmstr3[a->x.ints[3]]; *cp; *rbp++ = *cp++);
			*rbp++ = '.';
			assert(a->x.ints[4]);
			cp = asmstr4[a->x.ints[4]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 's';
			*rbp++ = 'w';
			*rbp++ = 'a';
			*rbp++ = 'p';
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'a';
			*rbp++ = 'n';
			*rbp++ = 'd';
			*rbp++ = ' ';
			*rbp++ = '&';
			*rbp++ = '0';
			*rbp++ = 'x';
			*rbp++ = 'f';
			*rbp++ = 'f';
			*rbp++ = 'f';
			*rbp++ = 'f';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 2);
			bp = rbp;
			outs("\n");
			break;
		}
	}
}

void final(a)
register Node a;
{
	register int s;
	switch (a->op) {
	case 608: L608:   /* %f%t %x,%z |mod */
		dumptree(a, 608);
		spill(1, DREG, a);  
		a->op = 608;
		break;
	case 609: L609:   /* jsr %y */
		dumptree(a, 609);
		spill(2, DREG, a);  spill(3, AREG, a);  
		a->op = 609;
		break;
	case 610: L610:   /* lea %z,$a1; mov.l %x,$d0; mov.w ($a0)+,($a1)+; sub.l &1,$d0; byte 0x6e,-6 */
		dumptree(a, 610);
		spill(1, DREG, a);  spill(3, AREG, a);  
		a->op = 610;
		break;
	case 611: L611:   /* mov.l %x,$d0; mov.w ($a0)+,($a1)+; sub.l &1,$d0; byte 0x6e,-6 */
		dumptree(a, 611);
		spill(1, DREG, a);  spill(3, AREG, a);  
		a->op = 611;
		break;
	case 612: L612:   /* # label */
		dumptree(a, 612);
		a->op = 612;
		return;
	case 613: L613:   /* # nop */
		dumptree(a, 613);
		a->op = 613;
		return;
	case 614: L614:   /* # ret */
		dumptree(a, 614);
		a->op = 614;
		return;
	case 615: L615:   /* %f.%t %x,%z */
		dumptree(a, 615);
		/* $a%c */
		/* $d%c */
		if ("\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[2]]) {
			if (
			!regchain(a)
			) {
				goto L623;   /* mov.%t %y,%z; %f.%t %x,%z */
			}
		}
		a->op = 615;
		break;
	case 616: L616:   /* %f.%t %x,%z |mod */
		dumptree(a, 616);
		a->op = 616;
		break;
	case 617: L617:   /* %f.%t %z */
		dumptree(a, 617);
		/* $a%c */
		/* $d%c */
		if ("\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[2]]) {
			if (
			!regchain(a)
			) {
				goto L624;   /* mov.%t %y,%z; %f.%t %z */
			}
		}
		a->op = 617;
		break;
	case 618: L618:   /* clr.%t %z */
		dumptree(a, 618);
		a->op = 618;
		break;
	case 619: L619:   /* cmp.%t %y,%x; b%f %z */
		dumptree(a, 619);
		/* $d%n */
		/* $d%c */
		/* _%c */
		/* _%c($a%n) */
		/* %c($a%n) */
		/* %c($sp) */
		/* %c($a6) */
		/* ($a%n) */
		if ("\0\0\0\1\1\1\1\0\0\1\1\1\1\0\0\0\0\0\0\0"[a->x.ints[0]]) {
			if (
			a->x.ints[1] == 15  /* &%c */
			&& a->syms[1]->x.name == sym[3].x.name  /* 0 */
			) {
				goto L625;   /* tst.%t %y; b%f %z */
			}
		}
		a->op = 619;
		break;
	case 620: L620:   /* bra %z */
		dumptree(a, 620);
		a->op = 620;
		break;
	case 621: L621:   /* lea %y,%z */
		dumptree(a, 621);
		a->op = 621;
		break;
	case 622: L622:   /* mov.%t %y,%z */
		dumptree(a, 622);
		if (
		a->x.ints[0] == 15  /* &%c */
		&& a->syms[0]->x.name == sym[3].x.name  /* 0 */
		) {
			/* $d%n */
			/* $d%c */
			/* _%c */
			/* _%c($a%n) */
			/* %c($a%n) */
			/* %c($sp) */
			/* %c($a6) */
			/* ($a%n) */
			if ("\0\0\0\1\1\1\1\0\0\1\1\1\1\0\0\0\0\0\0\0"[a->x.ints[2]]) {
				goto L618;   /* clr.%t %z */
			}
		}
		if (
		a->x.ints[0] == a->x.ints[2]
		&& a->kids[0] == a->kids[2]
		&& a->kids[3] == a->kids[5]
		&& a->syms[0] == a->syms[2]
		) {
			goto L613;   /* # nop */
		}
		a->op = 622;
		break;
	case 623: L623:   /* mov.%t %y,%z; %f.%t %x,%z */
		dumptree(a, 623);
		a->op = 623;
		break;
	case 624: L624:   /* mov.%t %y,%z; %f.%t %z */
		dumptree(a, 624);
		a->op = 624;
		break;
	case 625: L625:   /* tst.%t %y; b%f %z */
		dumptree(a, 625);
		a->op = 625;
		break;
	case 626: L626:   /* %z: */
		dumptree(a, 626);
		a->op = 626;
		break;
	case 627: L627:   /* ext.w %z; ext.l %z */
		dumptree(a, 627);
		a->op = 627;
		break;
	case 628: L628:   /* %f.%t %x,%z; ext.w %z */
		dumptree(a, 628);
		a->op = 628;
		break;
	case 629: L629:   /* %f.%t %x,%z; and &0xffff,%z */
		dumptree(a, 629);
		a->op = 629;
		break;
	case 630: L630:   /* %f.%t %x,%z; swap; ext.w %z */
		dumptree(a, 630);
		a->op = 630;
		break;
	case 631: L631:   /* %f.%t %x,%z; swap; and &0xffff,%z */
		dumptree(a, 631);
		a->op = 631;
		break;
	}
}

void rewrite(a)
register Node a;
{
	register Node b, c;
	register int s;
	assert(a);
	switch (a->op) {
	case 19: L19:   /* CNSTC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[0] = 15;  /* &%c */
		a->x.ints[4] = 1;  /* b */
		goto L622;   /* mov.%t %y,%z */
	case 20: L20:   /* CNSTS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[0] = 15;  /* &%c */
		a->x.ints[4] = 2;  /* w */
		goto L622;   /* mov.%t %y,%z */
	case 21: L21:   /* CNSTI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[0] = 15;  /* &%c */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* mov.%t %y,%z */
	case 22: L22:   /* CNSTU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[0] = 15;  /* &%c */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* mov.%t %y,%z */
	case 23: L23:   /* CNSTP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[2] = 2;  /* $a%c */
		a->x.ints[0] = 15;  /* &%c */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* mov.%t %y,%z */
	case 37: L37:   /* ARGI */
		dumptree(a, a->op);
		doarg(a);
		rewrite(a->kids[0]);
		a->x.ints[2] = 10;  /* %c($sp) */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* mov.%t %y,%z */
	case 39: L39:   /* ARGP */
		dumptree(a, a->op);
		doarg(a);
		rewrite(a->kids[0]);
		a->x.ints[2] = 10;  /* %c($sp) */
		a->x.ints[0] = 1;  /* $a%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* mov.%t %y,%z */
	case 41: L41:   /* ARGB */
		dumptree(a, a->op);
		doarg(a);
		rewrite(a->kids[0]);
		a->syms[1] = a->syms[0];
		a->x.ints[1] = 16;  /* &%c/2 */
		a->x.ints[2] = 10;  /* %c($sp) */
		rtarget(a,0,a0);
		goto L610;   /* lea %z,$a1; mov.l %x,$d0; mov.w ($a0)+,($a1)+; sub.l &1,$d0; byte 0x6e,-6 */
	case 51: L51:   /* ASGNC */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 12;  /* ($a%n) */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 1;  /* b */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L622;   /* mov.%t %y,%z */
	case 52: L52:   /* ASGNS */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 12;  /* ($a%n) */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 2;  /* w */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L622;   /* mov.%t %y,%z */
	case 53: L53:   /* ASGNI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 12;  /* ($a%n) */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L622;   /* mov.%t %y,%z */
	case 55: L55:   /* ASGNP */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 12;  /* ($a%n) */
		a->x.ints[0] = 1;  /* $a%n */
		a->x.ints[4] = 3;  /* l */
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L622;   /* mov.%t %y,%z */
	case 57: L57:   /* ASGNB */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[1] = a->syms[0];
		a->x.ints[1] = 16;  /* &%c/2 */
		rtarget(a,0,a1), rtarget(a,1,a0);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->kids[0] = a->kids[1];
		a->kids[1] = 0;
		goto L611;   /* mov.l %x,$d0; mov.w ($a0)+,($a1)+; sub.l &1,$d0; byte 0x6e,-6 */
	case 67: L67:   /* INDIRC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[0] = 12;  /* ($a%n) */
		a->x.ints[4] = 1;  /* b */
		goto L622;   /* mov.%t %y,%z */
	case 68: L68:   /* INDIRS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[0] = 12;  /* ($a%n) */
		a->x.ints[4] = 2;  /* w */
		goto L622;   /* mov.%t %y,%z */
	case 69: L69:   /* INDIRI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[0] = 12;  /* ($a%n) */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* mov.%t %y,%z */
	case 71: L71:   /* INDIRP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* $a%c */
		a->x.ints[0] = 12;  /* ($a%n) */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* mov.%t %y,%z */
	case 73: L73:   /* INDIRB */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* $a%c */
		a->x.ints[0] = 1;  /* $a%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* mov.%t %y,%z */
	case 85: L85:   /* CVCI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L627;   /* ext.w %z; ext.l %z */
	case 86: L86:   /* CVCU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->syms[1] = &sym[1];  /* 0xff */
		a->x.ints[1] = 15;  /* &%c */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 2;  /* and */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 131: L131:   /* CVIC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 1;  /* b */
		goto L622;   /* mov.%t %y,%z */
	case 132: L132:   /* CVIS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 2;  /* w */
		goto L622;   /* mov.%t %y,%z */
	case 134: L134:   /* CVIU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* mov.%t %y,%z */
	case 150: L150:   /* CVPU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[0] = 1;  /* $a%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* mov.%t %y,%z */
	case 165: L165:   /* CVSI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 11;  /* ext */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L617;   /* %f.%t %z */
	case 166: L166:   /* CVSU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->syms[1] = &sym[2];  /* 0xffff */
		a->x.ints[1] = 15;  /* &%c */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 2;  /* and */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 179: L179:   /* CVUC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 1;  /* b */
		goto L622;   /* mov.%t %y,%z */
	case 180: L180:   /* CVUS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 2;  /* w */
		goto L622;   /* mov.%t %y,%z */
	case 181: L181:   /* CVUI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* mov.%t %y,%z */
	case 183: L183:   /* CVUP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* $a%c */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* mov.%t %y,%z */
	case 197: L197:   /* NEGI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 23;  /* neg */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L617;   /* %f.%t %z */
	case 213: L213:   /* CALLI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		docall(a);
		rewrite(a->kids[0]);
		a->x.ints[0] = 12;  /* ($a%n) */
		setreg(a,d0);
		goto L609;   /* jsr %y */
	case 216: L216:   /* CALLV */
		dumptree(a, a->op);
		docall(a);
		rewrite(a->kids[0]);
		a->x.ints[0] = 12;  /* ($a%n) */
		setreg(a,d0);
		goto L609;   /* jsr %y */
	case 217: L217:   /* CALLB */
		dumptree(a, a->op);
		docall(a);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[0] = 12;  /* ($a%n) */
		rtarget(a,1,a1), setreg(a,d0);
		goto L609;   /* jsr %y */
	case 227: L227:   /* LOADC */
	case 228: L228:   /* LOADS */
	case 229: L229:   /* LOADI */
	case 230: L230:   /* LOADU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* mov.%t %y,%z */
	case 231: L231:   /* LOADP */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* $a%c */
		a->x.ints[0] = 1;  /* $a%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* mov.%t %y,%z */
	case 233: L233:   /* LOADB */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[2] = 2;  /* $a%c */
		a->x.ints[0] = 1;  /* $a%n */
		a->x.ints[4] = 3;  /* l */
		goto L622;   /* mov.%t %y,%z */
	case 245: L245:   /* RETI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rtarget(a,0,d0);
		goto L614;   /* # ret */
	case 248: L248:   /* RETV */
		dumptree(a, a->op);
		goto L614;   /* # ret */
	case 263: L263:   /* ADDRGP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[2] = 2;  /* $a%c */
		a->x.ints[0] = 5;  /* _%c */
		goto L621;   /* lea %y,%z */
	case 279: L279:   /* ADDRFP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[2] = 2;  /* $a%c */
		a->x.ints[0] = 11;  /* %c($a6) */
		goto L621;   /* lea %y,%z */
	case 295: L295:   /* ADDRLP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[2] = 2;  /* $a%c */
		a->x.ints[0] = 11;  /* %c($a6) */
		goto L621;   /* lea %y,%z */
	case 309: L309:   /* ADDI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 1;  /* add */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 310: L310:   /* ADDU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 1;  /* add */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 311: L311:   /* ADDP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* $a%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 1;  /* add */
		a->x.ints[0] = 1;  /* $a%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 325: L325:   /* SUBI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 26;  /* sub */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 326: L326:   /* SUBU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 26;  /* sub */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 327: L327:   /* SUBP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 2;  /* $a%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 26;  /* sub */
		a->x.ints[0] = 1;  /* $a%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 341: L341:   /* LSHI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 3;  /* asl */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 342: L342:   /* LSHU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 17;  /* lsl */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 357: L357:   /* MODI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 2;  /* w */
		a->x.ints[3] = 7;  /* divs */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L630;   /* %f.%t %x,%z; swap; ext.w %z */
	case 358: L358:   /* MODU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 2;  /* w */
		a->x.ints[3] = 8;  /* divu */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L631;   /* %f.%t %x,%z; swap; and &0xffff,%z */
	case 373: L373:   /* RSHI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 4;  /* asr */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 374: L374:   /* RSHU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 18;  /* lsr */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 390: L390:   /* BANDU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 2;  /* and */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 406: L406:   /* BCOMU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 24;  /* not */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L617;   /* %f.%t %z */
	case 422: L422:   /* BORU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 25;  /* or */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 438: L438:   /* BXORU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		a->x.ints[3] = 9;  /* eor */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 453: L453:   /* DIVI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 2;  /* w */
		a->x.ints[3] = 7;  /* divs */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L628;   /* %f.%t %x,%z; ext.w %z */
	case 454: L454:   /* DIVU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 2;  /* w */
		a->x.ints[3] = 8;  /* divu */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L629;   /* %f.%t %x,%z; and &0xffff,%z */
	case 469: L469:   /* MULI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 2;  /* w */
		a->x.ints[3] = 20;  /* muls */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 470: L470:   /* MULU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[2] = 4;  /* $d%c */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[4] = 2;  /* w */
		a->x.ints[3] = 21;  /* mulu */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.zap0=1;
		goto L615;   /* %f.%t %x,%z */
	case 485: L485:   /* EQI */
	case 486: L486:   /* EQU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 18;  /* L%c */
		a->x.ints[3] = 10;  /* eq */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		goto L619;   /* cmp.%t %y,%x; b%f %z */
	case 501: L501:   /* GEI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 18;  /* L%c */
		a->x.ints[3] = 12;  /* ge */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		goto L619;   /* cmp.%t %y,%x; b%f %z */
	case 502: L502:   /* GEU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 18;  /* L%c */
		a->x.ints[3] = 5;  /* cc */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		goto L619;   /* cmp.%t %y,%x; b%f %z */
	case 517: L517:   /* GTI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 18;  /* L%c */
		a->x.ints[3] = 13;  /* gt */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		goto L619;   /* cmp.%t %y,%x; b%f %z */
	case 518: L518:   /* GTU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 18;  /* L%c */
		a->x.ints[3] = 14;  /* hi */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		goto L619;   /* cmp.%t %y,%x; b%f %z */
	case 533: L533:   /* LEI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 18;  /* L%c */
		a->x.ints[3] = 15;  /* le */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		goto L619;   /* cmp.%t %y,%x; b%f %z */
	case 534: L534:   /* LEU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 18;  /* L%c */
		a->x.ints[3] = 16;  /* ls */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		goto L619;   /* cmp.%t %y,%x; b%f %z */
	case 549: L549:   /* LTI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 18;  /* L%c */
		a->x.ints[3] = 19;  /* lt */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		goto L619;   /* cmp.%t %y,%x; b%f %z */
	case 550: L550:   /* LTU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 18;  /* L%c */
		a->x.ints[3] = 6;  /* cs */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		goto L619;   /* cmp.%t %y,%x; b%f %z */
	case 565: L565:   /* NEI */
	case 566: L566:   /* NEU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 18;  /* L%c */
		a->x.ints[3] = 22;  /* ne */
		a->x.ints[1] = 3;  /* $d%n */
		a->x.ints[0] = 3;  /* $d%n */
		a->x.ints[4] = 3;  /* l */
		goto L619;   /* cmp.%t %y,%x; b%f %z */
	case 584: L584:   /* JUMPV */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[2] = 12;  /* ($a%n) */
		goto L620;   /* bra %z */
	case 600: L600:   /* LABELV */
		dumptree(a, a->op);
		a->syms[2] = a->syms[0];
		a->x.ints[2] = 18;  /* L%c */
		goto L626;   /* %z: */
	case 608: L608:   /* %f%t %x,%z |mod */
		dumptree(a, 608);
		a->op = 608;
		break;
	case 609: L609:   /* jsr %y */
		dumptree(a, 609);
		b = a->kids[0];
		if (
		a->x.ints[0] == 12  /* ($a%n) */
		&& b->op == 621  /* lea %y,%z */
		) {
			if (
			b->x.ints[0] == 5  /* _%c */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L609;   /* jsr %y */
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
				goto L609;   /* jsr %y */
			}
			if (
			b->x.ints[0] == 11  /* %c($a6) */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L609;   /* jsr %y */
			}
		}
		a->op = 609;
		break;
	case 610: L610:   /* lea %z,$a1; mov.l %x,$d0; mov.w ($a0)+,($a1)+; sub.l &1,$d0; byte 0x6e,-6 */
		dumptree(a, 610);
		a->op = 610;
		break;
	case 611: L611:   /* mov.l %x,$d0; mov.w ($a0)+,($a1)+; sub.l &1,$d0; byte 0x6e,-6 */
		dumptree(a, 611);
		a->op = 611;
		break;
	case 612: L612:   /* # label */
		dumptree(a, 612);
		a->op = 612;
		break;
	case 613: L613:   /* # nop */
		dumptree(a, 613);
		a->op = 613;
		break;
	case 614: L614:   /* # ret */
		dumptree(a, 614);
		a->op = 614;
		break;
	case 615: L615:   /* %f.%t %x,%z */
		dumptree(a, 615);
		switch (a->x.ints[3]) {
		case 1:   /* add */
			if (
			a->x.ints[4] == 3  /* l */
			&& a->x.ints[1] == 3  /* $d%n */
			&& a->x.ints[2] == 2  /* $a%c */
			&& a->kids[1]->x.ints[2] == 2  /* $a%c */
			) {
				a->x.ints[1] = 1;  /* $a%n */
				a->x.ints[0] = 3;  /* $d%n */
				goto L615;   /* %f.%t %x,%z */
			}
			if (
			a->x.ints[4] == 3  /* l */
			) {
				switch (a->x.ints[1]) {
				case 1:   /* $a%n */
					if (
					a->x.ints[2] == 2  /* $a%c */
					&& a->x.ints[0] == 3  /* $d%n */
					) {
						a->x.ints[0] = 19;  /* 0($a%n,$d%i.l) */
						a->kids[3] = a->kids[0];
						a->kids[0] = 0;
						a->kids[0] = a->kids[1];
						a->kids[1] = 0;
						goto L621;   /* lea %y,%z */
					}
					break;
				case 15:   /* &%c */
					if (
					a->x.ints[2] == 2  /* $a%c */
					&& a->x.ints[0] == 1  /* $a%n */
					&& range(getint(a->syms[1]),16)
					) {
						a->x.ints[0] = 9;  /* %c($a%n) */
						a->syms[0] = a->syms[1];
						a->x.zap0=0;
						goto L621;   /* lea %y,%z */
					}
					break;
				}
			}
			/* $a%n */
			/* $d%n */
			if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[1]]) {
				/* $a%c */
				/* $d%c */
				if ("\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[2]]) {
					b = a->kids[1];
					if (
					b->op == 622  /* mov.%t %y,%z */
					&& a->x.ints[4] == b->x.ints[4]
					) {
						/* $d%n */
						/* &%c */
						if ("\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* &_%c */
						if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
						}
					}
				}
			}
			break;
		case 2:   /* and */
			b = a->kids[1];
			if (
			a->x.ints[1] == 3  /* $d%n */
			&& a->x.ints[2] == 4  /* $d%c */
			&& b->op == 622  /* mov.%t %y,%z */
			) {
				/* $d%n */
				/* $d%c */
				/* _%c */
				/* _%c($a%n) */
				/* %c($a%n) */
				/* %c($sp) */
				/* %c($a6) */
				/* ($a%n) */
				/* &%c */
				/* &_%c */
				if ("\0\0\0\1\1\1\1\0\0\1\1\1\1\0\0\1\0\1\0\0"[b->x.ints[0]]) {
					if (
					a->x.ints[4] == b->x.ints[4]
					) {
						/* $d%n */
						/* &%c */
						if ("\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* &_%c */
						if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
						}
					}
				}
			}
			break;
		case 3:   /* asl */
		case 4:   /* asr */
		case 17:   /* lsl */
		case 18:   /* lsr */
			b = a->kids[1];
			if (
			a->x.ints[1] == 3  /* $d%n */
			&& a->x.ints[2] == 4  /* $d%c */
			&& b->op == 622  /* mov.%t %y,%z */
			&& b->x.ints[0] == 15  /* &%c */
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
					goto L615;   /* %f.%t %x,%z */
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
					goto L615;   /* %f.%t %x,%z */
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
					goto L615;   /* %f.%t %x,%z */
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
					goto L615;   /* %f.%t %x,%z */
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
					goto L615;   /* %f.%t %x,%z */
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
					goto L615;   /* %f.%t %x,%z */
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
					goto L615;   /* %f.%t %x,%z */
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
					goto L615;   /* %f.%t %x,%z */
				}
			}
			break;
		case 25:   /* or */
			b = a->kids[1];
			if (
			a->x.ints[1] == 3  /* $d%n */
			&& a->x.ints[2] == 4  /* $d%c */
			&& b->op == 622  /* mov.%t %y,%z */
			) {
				/* $d%n */
				/* $d%c */
				/* _%c */
				/* _%c($a%n) */
				/* %c($a%n) */
				/* %c($sp) */
				/* %c($a6) */
				/* ($a%n) */
				/* &%c */
				/* &_%c */
				if ("\0\0\0\1\1\1\1\0\0\1\1\1\1\0\0\1\0\1\0\0"[b->x.ints[0]]) {
					if (
					a->x.ints[4] == b->x.ints[4]
					) {
						/* $d%n */
						/* &%c */
						if ("\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* &_%c */
						if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
						}
					}
				}
			}
			break;
		case 26:   /* sub */
			if (
			a->x.ints[4] == 3  /* l */
			&& a->x.ints[1] == 3  /* $d%n */
			&& a->x.ints[2] == 2  /* $a%c */
			&& a->kids[1]->x.ints[2] == 2  /* $a%c */
			) {
				a->x.ints[1] = 1;  /* $a%n */
				a->x.ints[2] = 4;  /* $d%c */
				setreg(a, rmap[I]);
				goto L615;   /* %f.%t %x,%z */
			}
			/* $a%n */
			/* $d%n */
			if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[1]]) {
				/* $a%c */
				/* $d%c */
				if ("\0\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[2]]) {
					b = a->kids[1];
					if (
					b->op == 622  /* mov.%t %y,%z */
					&& a->x.ints[4] == b->x.ints[4]
					) {
						/* $d%n */
						/* &%c */
						if ("\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* $d%n */
						/* &%c */
						/* &_%c */
						if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
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
							goto L615;   /* %f.%t %x,%z */
						}
						/* $a%n */
						/* &_%c */
						if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[1] = b->syms[0];
							a->kids[4] = b->kids[3];
							a->x.ints[1] = b->x.ints[0];
							a->kids[1] = b->kids[0];
							goto L615;   /* %f.%t %x,%z */
						}
					}
				}
			}
			break;
		}
		a->op = 615;
		break;
	case 616: L616:   /* %f.%t %x,%z |mod */
		dumptree(a, 616);
		a->op = 616;
		break;
	case 617: L617:   /* %f.%t %z */
		dumptree(a, 617);
		a->op = 617;
		break;
	case 618: L618:   /* clr.%t %z */
		dumptree(a, 618);
		a->op = 618;
		break;
	case 619: L619:   /* cmp.%t %y,%x; b%f %z */
		dumptree(a, 619);
		switch (a->x.ints[0]) {
		case 1:   /* $a%n */
			/* $a%n */
			/* $d%n */
			if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[1]]) {
				b = a->kids[1];
				if (
				b->op == 622  /* mov.%t %y,%z */
				&& a->x.ints[4] == b->x.ints[4]
				) {
					/* $d%n */
					/* &%c */
					if ("\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0"[b->x.ints[0]]) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L619;   /* cmp.%t %y,%x; b%f %z */
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
						goto L619;   /* cmp.%t %y,%x; b%f %z */
					}
					/* $a%n */
					/* $d%n */
					/* &%c */
					/* &_%c */
					if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L619;   /* cmp.%t %y,%x; b%f %z */
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
						goto L619;   /* cmp.%t %y,%x; b%f %z */
					}
					/* $a%n */
					/* &_%c */
					if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L619;   /* cmp.%t %y,%x; b%f %z */
					}
				}
			}
			b = a->kids[0];
			if (
			a->x.ints[1] == 15  /* &%c */
			&& b->op == 622  /* mov.%t %y,%z */
			) {
				/* $a%n */
				/* $a%c */
				/* $d%n */
				/* $d%c */
				/* _%c */
				/* _%c($a%n) */
				/* %c($a%n) */
				/* %c($sp) */
				/* %c($a6) */
				/* ($a%n) */
				if ("\0\1\1\1\1\1\1\0\0\1\1\1\1\0\0\0\0\0\0\0"[b->x.ints[0]]) {
					if (
					a->x.ints[4] == b->x.ints[4]
					&& b->x.ints[0] == 3  /* $d%n */
					) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[0] = b->syms[0];
						a->kids[3] = b->kids[3];
						a->x.ints[0] = b->x.ints[0];
						a->kids[0] = b->kids[0];
						goto L619;   /* cmp.%t %y,%x; b%f %z */
					}
				}
			}
			b = a->kids[0];
			if (
			a->x.ints[1] == 15  /* &%c */
			&& b->op == 622  /* mov.%t %y,%z */
			) {
				/* $a%n */
				/* $a%c */
				/* $d%n */
				/* $d%c */
				/* _%c */
				/* _%c($a%n) */
				/* %c($a%n) */
				/* %c($sp) */
				/* %c($a6) */
				/* ($a%n) */
				if ("\0\1\1\1\1\1\1\0\0\1\1\1\1\0\0\0\0\0\0\0"[b->x.ints[0]]) {
					if (
					a->x.ints[4] == b->x.ints[4]
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
						goto L619;   /* cmp.%t %y,%x; b%f %z */
					}
				}
			}
			break;
		case 2:   /* $a%c */
			/* $a%n */
			/* $d%n */
			if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[1]]) {
				b = a->kids[1];
				if (
				b->op == 622  /* mov.%t %y,%z */
				&& a->x.ints[4] == b->x.ints[4]
				) {
					/* $d%n */
					/* &%c */
					if ("\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0"[b->x.ints[0]]) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L619;   /* cmp.%t %y,%x; b%f %z */
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
						goto L619;   /* cmp.%t %y,%x; b%f %z */
					}
					/* $a%n */
					/* $d%n */
					/* &%c */
					/* &_%c */
					if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L619;   /* cmp.%t %y,%x; b%f %z */
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
						goto L619;   /* cmp.%t %y,%x; b%f %z */
					}
					/* $a%n */
					/* &_%c */
					if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L619;   /* cmp.%t %y,%x; b%f %z */
					}
				}
			}
			break;
		case 3:   /* $d%n */
			switch (a->x.ints[1]) {
			case 1:   /* $a%n */
			case 3:   /* $d%n */
				b = a->kids[1];
				if (
				b->op == 622  /* mov.%t %y,%z */
				&& a->x.ints[4] == b->x.ints[4]
				) {
					/* $d%n */
					/* &%c */
					if ("\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0"[b->x.ints[0]]) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L619;   /* cmp.%t %y,%x; b%f %z */
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
						goto L619;   /* cmp.%t %y,%x; b%f %z */
					}
					/* $a%n */
					/* &_%c */
					if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L619;   /* cmp.%t %y,%x; b%f %z */
					}
				}
				break;
			case 15:   /* &%c */
				b = a->kids[0];
				if (
				b->op == 622  /* mov.%t %y,%z */
				) {
					/* $a%n */
					/* $a%c */
					/* $d%n */
					/* $d%c */
					/* _%c */
					/* _%c($a%n) */
					/* %c($a%n) */
					/* %c($sp) */
					/* %c($a6) */
					/* ($a%n) */
					if ("\0\1\1\1\1\1\1\0\0\1\1\1\1\0\0\0\0\0\0\0"[b->x.ints[0]]) {
						if (
						a->x.ints[4] == b->x.ints[4]
						&& b->x.ints[0] == 3  /* $d%n */
						) {
							if (--b->count > 0) {
								if (c=b->kids[0]) ++c->count;
								if (c=b->kids[3]) ++c->count;
							}
							a->syms[0] = b->syms[0];
							a->kids[3] = b->kids[3];
							a->x.ints[0] = b->x.ints[0];
							a->kids[0] = b->kids[0];
							goto L619;   /* cmp.%t %y,%x; b%f %z */
						}
					}
				}
				break;
			}
			b = a->kids[0];
			if (
			a->x.ints[1] == 15  /* &%c */
			&& b->op == 622  /* mov.%t %y,%z */
			) {
				/* $a%n */
				/* $a%c */
				/* $d%n */
				/* $d%c */
				/* _%c */
				/* _%c($a%n) */
				/* %c($a%n) */
				/* %c($sp) */
				/* %c($a6) */
				/* ($a%n) */
				if ("\0\1\1\1\1\1\1\0\0\1\1\1\1\0\0\0\0\0\0\0"[b->x.ints[0]]) {
					if (
					a->x.ints[4] == b->x.ints[4]
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
						goto L619;   /* cmp.%t %y,%x; b%f %z */
					}
				}
			}
			break;
		case 4:   /* $d%c */
			/* $a%n */
			/* $d%n */
			if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[1]]) {
				b = a->kids[1];
				if (
				b->op == 622  /* mov.%t %y,%z */
				&& a->x.ints[4] == b->x.ints[4]
				) {
					/* $d%n */
					/* &%c */
					if ("\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0"[b->x.ints[0]]) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L619;   /* cmp.%t %y,%x; b%f %z */
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
						goto L619;   /* cmp.%t %y,%x; b%f %z */
					}
					/* $a%n */
					/* $d%n */
					/* &%c */
					/* &_%c */
					if ("\0\1\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\1\0\0"[b->x.ints[0]]) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L619;   /* cmp.%t %y,%x; b%f %z */
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
						goto L619;   /* cmp.%t %y,%x; b%f %z */
					}
					/* $a%n */
					/* &_%c */
					if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
						if (--b->count > 0) {
							if (c=b->kids[0]) ++c->count;
							if (c=b->kids[3]) ++c->count;
						}
						a->syms[1] = b->syms[0];
						a->kids[4] = b->kids[3];
						a->x.ints[1] = b->x.ints[0];
						a->kids[1] = b->kids[0];
						goto L619;   /* cmp.%t %y,%x; b%f %z */
					}
				}
			}
			break;
		}
		if (
		a->x.ints[4] == 3  /* l */
		&& a->x.ints[0] == 3  /* $d%n */
		) {
			switch (a->x.ints[1]) {
			case 3:   /* $d%n */
				b = a->kids[0];
				switch (b->op) {
				case 617:   /* %f.%t %z */
					b = a->kids[1];
					if (
					a->kids[0]->x.ints[3] == 11  /* ext */
					&& a->kids[0]->x.ints[4] == 3  /* l */
					&& b->op == 617  /* %f.%t %z */
					&& b->x.ints[3] == 11  /* ext */
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
						goto L619;   /* cmp.%t %y,%x; b%f %z */
					}
					break;
				case 627:   /* ext.w %z; ext.l %z */
					b = a->kids[1];
					if (
					b->op == 627  /* ext.w %z; ext.l %z */
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
						goto L619;   /* cmp.%t %y,%x; b%f %z */
					}
					break;
				}
				break;
			case 15:   /* &%c */
				/* eq */
				/* ge */
				/* gt */
				/* le */
				/* lt */
				/* ne */
				if ("\0\0\0\0\0\0\0\0\0\0\1\0\1\1\0\1\0\0\0\1\0\0\1\0\0\0\0"[a->x.ints[3]]) {
					b = a->kids[0];
					switch (b->op) {
					case 617:   /* %f.%t %z */
						if (
						b->x.ints[3] == 11  /* ext */
						&& b->x.ints[4] == 3  /* l */
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
							goto L619;   /* cmp.%t %y,%x; b%f %z */
						}
						break;
					case 627:   /* ext.w %z; ext.l %z */
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
							a->x.ints[4] = 1;  /* b */
							goto L619;   /* cmp.%t %y,%x; b%f %z */
						}
						break;
					}
				}
				break;
			}
		}
		a->op = 619;
		break;
	case 620: L620:   /* bra %z */
		dumptree(a, 620);
		b = a->kids[2];
		if (
		a->x.ints[2] == 12  /* ($a%n) */
		&& b->count == 1
		&& b->op == 621  /* lea %y,%z */
		&& b->x.ints[0] == 5  /* _%c */
		) {
			--b->count;
			a->x.ints[2] = 18;  /* L%c */
			a->syms[2] = b->syms[0];
			a->kids[2] = 0;
			goto L620;   /* bra %z */
		}
		a->op = 620;
		break;
	case 621: L621:   /* lea %y,%z */
		dumptree(a, 621);
		b = a->kids[0];
		if (
		a->x.ints[0] == 9  /* %c($a%n) */
		&& a->x.ints[2] == 2  /* $a%c */
		&& b->op == 622  /* mov.%t %y,%z */
		&& b->x.ints[0] == 2  /* $a%c */
		&& b->count == 1
		&& a->syms[2] == b->syms[0]
		) {
			--b->count;
			a->x.ints[1] = 15;  /* &%c */
			a->x.ints[4] = 3;  /* l */
			a->x.ints[3] = 1;  /* add */
			a->syms[1] = a->syms[0];
			a->kids[0] = 0;
			goto L615;   /* %f.%t %x,%z */
		}
		a->op = 621;
		break;
	case 622: L622:   /* mov.%t %y,%z */
		dumptree(a, 622);
		if (
		a->x.ints[2] == 11  /* %c($a6) */
		) {
			if (
			getregset(a->syms[2])==DREG
			) {
				a->x.ints[2] = 4;  /* $d%c */
				goto L622;   /* mov.%t %y,%z */
			}
			if (
			getregset(a->syms[2])==AREG
			) {
				a->x.ints[2] = 2;  /* $a%c */
				goto L622;   /* mov.%t %y,%z */
			}
		}
		switch (a->x.ints[0]) {
		case 1:   /* $a%n */
			b = a->kids[0];
			if (
			a->x.ints[2] == 2  /* $a%c */
			&& b->op == 621  /* lea %y,%z */
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
				goto L621;   /* lea %y,%z */
			}
			break;
		case 2:   /* $a%c */
			if (
			a->x.ints[2] == 2  /* $a%c */
			&& a->syms[RX] == rmap[P] && rmcopy(a)
			) {
				a->syms[2] = a->syms[0];
				goto L622;   /* mov.%t %y,%z */
			}
			break;
		case 4:   /* $d%c */
			if (
			a->x.ints[2] == 4  /* $d%c */
			&& a->syms[RX] == rmap[I] && rmcopy(a)
			) {
				a->syms[2] = a->syms[0];
				goto L622;   /* mov.%t %y,%z */
			}
			break;
		case 11:   /* %c($a6) */
			if (
			getregset(a->syms[0])==DREG
			) {
				a->x.ints[0] = 4;  /* $d%c */
				goto L622;   /* mov.%t %y,%z */
			}
			if (
			getregset(a->syms[0])==AREG
			) {
				a->x.ints[0] = 2;  /* $a%c */
				goto L622;   /* mov.%t %y,%z */
			}
			break;
		}
		if (
		a->x.ints[2] == 10  /* %c($sp) */
		&& a->syms[2]->x.name == sym[3].x.name  /* 0 */
		) {
			a->x.ints[2] = 13;  /* ($sp) */
			goto L622;   /* mov.%t %y,%z */
		}
		b = a->kids[0];
		if (
		a->x.ints[0] == 12  /* ($a%n) */
		&& b->op == 621  /* lea %y,%z */
		) {
			if (
			b->x.ints[0] == 5  /* _%c */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L622;   /* mov.%t %y,%z */
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
				goto L622;   /* mov.%t %y,%z */
			}
			if (
			b->x.ints[0] == 11  /* %c($a6) */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[0] = b->syms[0];
				a->kids[3] = b->kids[3];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L622;   /* mov.%t %y,%z */
			}
		}
		b = a->kids[2];
		if (
		a->x.ints[2] == 12  /* ($a%n) */
		&& b->op == 621  /* lea %y,%z */
		) {
			if (
			b->x.ints[0] == 5  /* _%c */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[2] = b->syms[0];
				a->kids[5] = b->kids[3];
				a->x.ints[2] = b->x.ints[0];
				a->kids[2] = b->kids[0];
				goto L622;   /* mov.%t %y,%z */
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
				goto L622;   /* mov.%t %y,%z */
			}
			if (
			b->x.ints[0] == 11  /* %c($a6) */
			) {
				if (--b->count > 0) {
					if (c=b->kids[0]) ++c->count;
					if (c=b->kids[3]) ++c->count;
				}
				a->syms[2] = b->syms[0];
				a->kids[5] = b->kids[3];
				a->x.ints[2] = b->x.ints[0];
				a->kids[2] = b->kids[0];
				goto L622;   /* mov.%t %y,%z */
			}
		}
		b = a->kids[0];
		if (
		a->x.ints[4] == 3  /* l */
		&& a->x.ints[0] == 1  /* $a%n */
		&& b->op == 621  /* lea %y,%z */
		&& b->x.ints[0] == 5  /* _%c */
		) {
			--b->count;
			a->x.ints[0] = 17;  /* &_%c */
			a->syms[0] = b->syms[0];
			a->kids[0] = 0;
			goto L622;   /* mov.%t %y,%z */
		}
		switch (a->x.ints[0]) {
		case 1:   /* $a%n */
			if (
			b->op == 622  /* mov.%t %y,%z */
			&& b->x.ints[4] == 3  /* l */
			&& b->x.ints[0] == 1  /* $a%n */
			) {
				if (--b->count > 0)
					if (c=b->kids[0]) ++c->count;
				a->kids[0] = b->kids[0];
				goto L622;   /* mov.%t %y,%z */
			}
			if (
			b->op == 622  /* mov.%t %y,%z */
			&& a->x.ints[4] == b->x.ints[4]
			) {
				/* $d%n */
				/* &%c */
				if ("\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L622;   /* mov.%t %y,%z */
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
					goto L622;   /* mov.%t %y,%z */
				}
				/* $a%n */
				/* &_%c */
				if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L622;   /* mov.%t %y,%z */
				}
			}
			if (
			b->op == 615  /* %f.%t %x,%z */
			&& b->x.ints[3] == 1  /* add */
			&& b->x.ints[1] == 1  /* $a%n */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 622  /* mov.%t %y,%z */
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
				goto L615;   /* %f.%t %x,%z */
			}
			if (
			b->op == 615  /* %f.%t %x,%z */
			&& b->x.ints[3] == 26  /* sub */
			&& b->x.ints[1] == 1  /* $a%n */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 622  /* mov.%t %y,%z */
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
				a->x.ints[3] = 26;  /* sub */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L615;   /* %f.%t %x,%z */
			}
			if (
			b->op == 615  /* %f.%t %x,%z */
			&& b->x.ints[3] == 1  /* add */
			&& b->x.ints[1] == 15  /* &%c */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 622  /* mov.%t %y,%z */
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
				goto L615;   /* %f.%t %x,%z */
			}
			if (
			b->op == 615  /* %f.%t %x,%z */
			&& b->x.ints[3] == 26  /* sub */
			&& b->x.ints[1] == 15  /* &%c */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 622  /* mov.%t %y,%z */
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
				a->x.ints[3] = 26;  /* sub */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L615;   /* %f.%t %x,%z */
			}
			if (
			b->op == 615  /* %f.%t %x,%z */
			&& b->x.ints[3] == 1  /* add */
			&& b->x.ints[1] == 3  /* $d%n */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 622  /* mov.%t %y,%z */
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
				goto L615;   /* %f.%t %x,%z */
			}
			if (
			b->op == 615  /* %f.%t %x,%z */
			&& b->x.ints[3] == 26  /* sub */
			&& b->x.ints[1] == 3  /* $d%n */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 622  /* mov.%t %y,%z */
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
				a->x.ints[3] = 26;  /* sub */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L615;   /* %f.%t %x,%z */
			}
			break;
		case 3:   /* $d%n */
			if (
			b->op == 622  /* mov.%t %y,%z */
			&& b->x.ints[4] == 3  /* l */
			&& b->x.ints[0] == 3  /* $d%n */
			) {
				if (--b->count > 0)
					if (c=b->kids[0]) ++c->count;
				a->kids[0] = b->kids[0];
				goto L622;   /* mov.%t %y,%z */
			}
			if (
			b->op == 622  /* mov.%t %y,%z */
			&& a->x.ints[4] == b->x.ints[4]
			) {
				/* $d%n */
				/* &%c */
				if ("\0\0\0\1\0\0\0\0\0\0\0\0\0\0\0\1\0\0\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L622;   /* mov.%t %y,%z */
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
					goto L622;   /* mov.%t %y,%z */
				}
				/* $a%n */
				/* &_%c */
				if ("\0\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\0\0"[b->x.ints[0]]) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[3]) ++c->count;
					}
					a->syms[0] = b->syms[0];
					a->kids[3] = b->kids[3];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L622;   /* mov.%t %y,%z */
				}
			}
			if (
			b->op == 615  /* %f.%t %x,%z */
			&& b->x.ints[3] == 1  /* add */
			&& b->x.ints[1] == 1  /* $a%n */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 622  /* mov.%t %y,%z */
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
				goto L615;   /* %f.%t %x,%z */
			}
			if (
			b->op == 615  /* %f.%t %x,%z */
			&& b->x.ints[3] == 26  /* sub */
			&& b->x.ints[1] == 1  /* $a%n */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 622  /* mov.%t %y,%z */
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
				a->x.ints[3] = 26;  /* sub */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L615;   /* %f.%t %x,%z */
			}
			if (
			b->op == 615  /* %f.%t %x,%z */
			&& b->x.ints[3] == 1  /* add */
			&& b->x.ints[1] == 15  /* &%c */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 622  /* mov.%t %y,%z */
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
				goto L615;   /* %f.%t %x,%z */
			}
			if (
			b->op == 615  /* %f.%t %x,%z */
			&& b->x.ints[3] == 26  /* sub */
			&& b->x.ints[1] == 15  /* &%c */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 622  /* mov.%t %y,%z */
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
				a->x.ints[3] = 26;  /* sub */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L615;   /* %f.%t %x,%z */
			}
			if (
			b->op == 615  /* %f.%t %x,%z */
			&& b->x.ints[3] == 1  /* add */
			&& b->x.ints[1] == 3  /* $d%n */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 622  /* mov.%t %y,%z */
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
				goto L615;   /* %f.%t %x,%z */
			}
			if (
			b->op == 615  /* %f.%t %x,%z */
			&& b->x.ints[3] == 26  /* sub */
			&& b->x.ints[1] == 3  /* $d%n */
			&& a->x.ints[4] == b->x.ints[4]
			&& b->count == 1
			&& b->kids[0]->op == 622  /* mov.%t %y,%z */
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
				a->x.ints[3] = 26;  /* sub */
				a->syms[1] = b->syms[1];
				a->kids[4] = b->kids[4];
				a->kids[1] = b->kids[1];
				a->x.ints[1] = b->x.ints[1];
				b->kids[0]->kids[3] = 0;
				b->kids[0]->kids[0] = 0;
				b->kids[0] = 0;
				a->kids[0] = 0;
				goto L615;   /* %f.%t %x,%z */
			}
			break;
		}
		a->op = 622;
		break;
	case 623: L623:   /* mov.%t %y,%z; %f.%t %x,%z */
		dumptree(a, 623);
		a->op = 623;
		break;
	case 624: L624:   /* mov.%t %y,%z; %f.%t %z */
		dumptree(a, 624);
		a->op = 624;
		break;
	case 625: L625:   /* tst.%t %y; b%f %z */
		dumptree(a, 625);
		a->op = 625;
		break;
	case 626: L626:   /* %z: */
		dumptree(a, 626);
		a->op = 626;
		break;
	case 627: L627:   /* ext.w %z; ext.l %z */
		dumptree(a, 627);
		a->op = 627;
		break;
	case 628: L628:   /* %f.%t %x,%z; ext.w %z */
		dumptree(a, 628);
		a->op = 628;
		break;
	case 629: L629:   /* %f.%t %x,%z; and &0xffff,%z */
		dumptree(a, 629);
		a->op = 629;
		break;
	case 630: L630:   /* %f.%t %x,%z; swap; ext.w %z */
		dumptree(a, 630);
		a->op = 630;
		break;
	case 631: L631:   /* %f.%t %x,%z; swap; and &0xffff,%z */
		dumptree(a, 631);
		a->op = 631;
		break;
	default:
	assert(0);
	}
}


/* "jal $%0  # int" => ="jal %y  # int" A="$%n" 0=0 */
/* "move $%r,$%0" => ="move $%r,%y" A="$%n" 0=0 */
/* "j $%0" => ="j %y" A="$%n" 0=0 */
/* "%0:" => ="%y:" A="%c" a=a */
/* "la $%r,%0" => ="la $%r,%y" A="%c" a=a */
/* "la $%r,%0($sp)" => ="la $%r,%y" A="%c($sp)" a=a */
/* "la $%r,%0($sp)" => ="la $%r,%y" A="%c($sp)" a=a */
/* "move $%r,$%0" => ="move $%r,%y" A="$%n" 0=0 */
/* "move $%r,$%0" => ="move $%r,%y" A="$%n" 0=0 */
/* "move $%r,$%0" => ="move $%r,%y" A="$%n" 0=0 */
/* "move $%r,$%0" => ="move $%r,%y" A="$%n" 0=0 */
/* "move $%r,$%0" => ="move $%r,%y" A="$%n" 0=0 */
/* "move $%r,$%0" => ="move $%r,%y" A="$%n" 0=0 */
/* "move $%r,$%0" => ="move $%r,%y" A="$%n" 0=0 */
/* "move $%r,$%0" => ="move $%r,%y" A="$%n" 0=0 */
/* "trunc.w.d $f2,$f%0,$%r; mfc1 $%r,$f2" => ="%f.%t $f2,%y,$%r; mfc1 $%r,$f2" C="trunc.w" D="d" A="$f%n" 0=0 */
/* "mtc1 $%0,$f%r; cvt.d.w $f%r,$f%r" => ="mtc1 %y,$f%r; %f.%t $f%r,$f%r" A="$%n" 0=0 C="cvt.d" D="w" */
/* "cvt.s.d $f%r,$f%0" => ="%f.%t $f%r,%y" C="cvt.s" D="d" A="$f%n" 0=0 */
/* "cvt.d.s $f%r,$f%0" => ="%f.%t $f%r,%y" C="cvt.d" D="s" A="$f%n" 0=0 */
/* "s.d $f%0,%c($sp)" => ="s.%t $f%k,%y" D="d" 2=0 A="%c($sp)" */
/* "s.s $f%0,%c($sp)" => ="s.%t $f%k,%y" D="s" 2=0 A="%c($sp)" */
/* "sw $%0,%c($sp)" => ="s%t $%k,%y" D="w" 2=0 A="%c($sp)" */
/* "sw $%0,%c($sp)" => ="s%t $%k,%y" D="w" 2=0 A="%c($sp)" */
/* "s.d $f%1,($%0)" => ="s.%t $f%k,%y" D="d" 2=1 A="($%n)" 0=0 */
/* "s.s $f%1,($%0)" => ="s.%t $f%k,%y" D="s" 2=1 A="($%n)" 0=0 */
/* "sw $%1,($%0)" => ="s%t $%k,%y" D="w" 2=1 A="($%n)" 0=0 */
/* "sb $%1,($%0)" => ="s%t $%k,%y" D="b" 2=1 A="($%n)" 0=0 */
/* "sw $%1,($%0)" => ="s%t $%k,%y" D="w" 2=1 A="($%n)" 0=0 */
/* "sh $%1,($%0)" => ="s%t $%k,%y" D="h" 2=1 A="($%n)" 0=0 */
/* "jal $%0  # real" => ="jal %y  # real" A="$%n" 0=0 */
/* "jal $%0  # real" => ="jal %y  # real" A="$%n" 0=0 */
/* "jal $%0  # int" => ="jal %y  # int" A="$%n" 0=0 */
/* "jal $%0  # int" => ="jal %y  # int" A="$%n" 0=0 */
/* "li.d $f%r,%0" => ="li.%t $f%r,%y" D="d" A="%c" a=a */
/* "li.s $f%r,%0" => ="li.%t $f%r,%y" D="s" A="%c" a=a */
/* "li $%r,%0" => ="li $%r,%y" A="%c" a=a */
/* "li $%r,%0" => ="li $%r,%y" A="%c" a=a */
/* "li $%r,%0" => ="li $%r,%y" A="%c" a=a */
/* "li $%r,%0" => ="li $%r,%y" A="%c" a=a */
/* "li $%r,%0" => ="li $%r,%y" A="%c" a=a */
/* "l.d $f%r,($%0)" => ="l.%t $f%r,%y" D="d" A="($%n)" 0=0 */
/* "l.s $f%r,($%0)" => ="l.%t $f%r,%y" D="s" A="($%n)" 0=0 */
/* "lw $%r,($%0)" => ="l%t $%r,%y" D="w" A="($%n)" 0=0 */
/* "lb $%r,($%0)" => ="l%t $%r,%y" D="b" A="($%n)" 0=0 */
/* "lw $%r,($%0)" => ="l%t $%r,%y" D="w" A="($%n)" 0=0 */
/* "lh $%r,($%0)" => ="l%t $%r,%y" D="h" A="($%n)" 0=0 */
/* "mov.d $f%r,$f%0" => ="mov.%t $f%r,%y" D="d" A="$f%n" 0=0 */
/* "mov.s $f%r,$f%0" => ="mov.%t $f%r,%y" D="s" A="$f%n" 0=0 */
/* "move $%r,$%0" => ="move $%r,%y" A="$%n" 0=0 */
/* "move $%r,$%0" => ="move $%r,%y" A="$%n" 0=0 */
/* "move $%r,$%0" => ="move $%r,%y" A="$%n" 0=0 */
/* "move $%r,$%0" => ="move $%r,%y" A="$%n" 0=0 */
/* "move $%r,$%0" => ="move $%r,%y" A="$%n" 0=0 */
/* "neg.d $f%r,$f%0" => ="%f.%t $f%r,%y" C="neg" D="d" A="$f%n" 0=0 */
/* "neg.s $f%r,$f%0" => ="%f.%t $f%r,%y" C="neg" D="s" A="$f%n" 0=0 */
/* "sub.d $f%r,$f%0,$f%1" => ="%f.%t $f%r,%y,%x" C="sub" D="d" A="$f%n" 0=0 B="$f%n" 1=1 */
/* "sub.s $f%r,$f%0,$f%1" => ="%f.%t $f%r,%y,%x" C="sub" D="s" A="$f%n" 0=0 B="$f%n" 1=1 */
/* "mul.d $f%r,$f%0,$f%1" => ="%f.%t $f%r,%y,%x" C="mul" D="d" A="$f%n" 0=0 B="$f%n" 1=1 */
/* "mul.s $f%r,$f%0,$f%1" => ="%f.%t $f%r,%y,%x" C="mul" D="s" A="$f%n" 0=0 B="$f%n" 1=1 */
/* "div.d $f%r,$f%0,$f%1" => ="%f.%t $f%r,%y,%x" C="div" D="d" A="$f%n" 0=0 B="$f%n" 1=1 */
/* "div.s $f%r,$f%0,$f%1" => ="%f.%t $f%r,%y,%x" C="div" D="s" A="$f%n" 0=0 B="$f%n" 1=1 */
/* "add.d $f%r,$f%0,$f%1" => ="%f.%t $f%r,%y,%x" C="add" D="d" A="$f%n" 0=0 B="$f%n" 1=1 */
/* "add.s $f%r,$f%0,$f%1" => ="%f.%t $f%r,%y,%x" C="add" D="s" A="$f%n" 0=0 B="$f%n" 1=1 */
/* "negu $%r,$%0" => ="%f $%r,%y" C="negu" A="$%n" 0=0 */
/* "not $%r,$%0" => ="%f $%r,%y" C="not" A="$%n" 0=0 */
/* "sll $%r,$%0,24; sra $%r,$%r,%c" => ="sll $%r,%y,%x; %f $%r,$%r,%x" A="$%n" 0=0 B="%c" b="24" C="sra" B="%c" */
/* "sll $%r,$%0,16; sra $%r,$%r,%c" => ="sll $%r,%y,%x; %f $%r,$%r,%x" A="$%n" 0=0 B="%c" b="16" C="sra" B="%c" */
/* "and $%r,$%0,255" => ="%f $%r,%y,%x" C="and" A="$%n" 0=0 B="%c" b="255" */
/* "and $%r,$%0,65535" => ="%f $%r,%y,%x" C="and" A="$%n" 0=0 B="%c" b="65535" */
/* "divu $%r,$%0,$%1" => ="%f $%r,%y,%x" C="divu" A="$%n" 0=0 B="$%n" 1=1 */
/* "srl $%r,$%0,$%1" => ="%f $%r,%y,%x" C="srl" A="$%n" 0=0 B="$%n" 1=1 */
/* "remu $%r,$%0,$%1" => ="%f $%r,%y,%x" C="remu" A="$%n" 0=0 B="$%n" 1=1 */
/* "c.eq.d $f%0,$f%1; bc1t %0" => ="c.%f.%t %y,%x; bc1t %r" C="eq" D="d" A="$f%n" 0=0 B="$f%n" 1=1 c=a */
/* "c.eq.s $f%0,$f%1; bc1t %0" => ="c.%f.%t %y,%x; bc1t %r" C="eq" D="s" A="$f%n" 0=0 B="$f%n" 1=1 c=a */
/* "c.le.d $f%0,$f%1; bc1t %0" => ="c.%f.%t %y,%x; bc1t %r" C="le" D="d" A="$f%n" 0=0 B="$f%n" 1=1 c=a */
/* "c.le.s $f%0,$f%1; bc1t %0" => ="c.%f.%t %y,%x; bc1t %r" C="le" D="s" A="$f%n" 0=0 B="$f%n" 1=1 c=a */
/* "c.lt.d $f%0,$f%1; bc1t %0" => ="c.%f.%t %y,%x; bc1t %r" C="lt" D="d" A="$f%n" 0=0 B="$f%n" 1=1 c=a */
/* "c.lt.s $f%0,$f%1; bc1t %0" => ="c.%f.%t %y,%x; bc1t %r" C="lt" D="s" A="$f%n" 0=0 B="$f%n" 1=1 c=a */
/* "c.le.d $f%0,$f%1; bc1f %0" => ="c.%f.%t %y,%x; bc1f %r" C="le" D="d" A="$f%n" 0=0 B="$f%n" 1=1 c=a */
/* "c.le.s $f%0,$f%1; bc1f %0" => ="c.%f.%t %y,%x; bc1f %r" C="le" D="s" A="$f%n" 0=0 B="$f%n" 1=1 c=a */
/* "c.eq.d $f%0,$f%1; bc1f %0" => ="c.%f.%t %y,%x; bc1f %r" C="eq" D="d" A="$f%n" 0=0 B="$f%n" 1=1 c=a */
/* "c.eq.s $f%0,$f%1; bc1f %0" => ="c.%f.%t %y,%x; bc1f %r" C="eq" D="s" A="$f%n" 0=0 B="$f%n" 1=1 c=a */
/* "c.lt.d $f%0,$f%1; bc1f %0" => ="c.%f.%t %y,%x; bc1f %r" C="lt" D="d" A="$f%n" 0=0 B="$f%n" 1=1 c=a */
/* "c.lt.s $f%0,$f%1; bc1f %0" => ="c.%f.%t %y,%x; bc1f %r" C="lt" D="s" A="$f%n" 0=0 B="$f%n" 1=1 c=a */
/* "bleu $%0,$%1,%0" => ="b%f %y,%x,%r" C="leu" A="$%n" 0=0 B="$%n" 1=1 c=a */
/* "bge $%0,$%1,%0" => ="b%f %y,%x,%r" C="ge" A="$%n" 0=0 B="$%n" 1=1 c=a */
/* "blt $%0,$%1,%0" => ="b%f %y,%x,%r" C="lt" A="$%n" 0=0 B="$%n" 1=1 c=a */
/* "ble $%0,$%1,%0" => ="b%f %y,%x,%r" C="le" A="$%n" 0=0 B="$%n" 1=1 c=a */
/* "beq $%0,$%1,%0" => ="b%f %y,%x,%r" C="eq" A="$%n" 0=0 B="$%n" 1=1 c=a */
/* "bgtu $%0,$%1,%0" => ="b%f %y,%x,%r" C="gtu" A="$%n" 0=0 B="$%n" 1=1 c=a */
/* "bltu $%0,$%1,%0" => ="b%f %y,%x,%r" C="ltu" A="$%n" 0=0 B="$%n" 1=1 c=a */
/* "bne $%0,$%1,%0" => ="b%f %y,%x,%r" C="ne" A="$%n" 0=0 B="$%n" 1=1 c=a */
/* "bgt $%0,$%1,%0" => ="b%f %y,%x,%r" C="gt" A="$%n" 0=0 B="$%n" 1=1 c=a */
/* "bgeu $%0,$%1,%0" => ="b%f %y,%x,%r" C="geu" A="$%n" 0=0 B="$%n" 1=1 c=a */
/* "rem $%r,$%0,$%1" => ="%f $%r,%y,%x" C="rem" A="$%n" 0=0 B="$%n" 1=1 */
/* "rem $%r,$%0,$%1" => ="%f $%r,%y,%x" C="rem" A="$%n" 0=0 B="$%n" 1=1 */
/* "mul $%r,$%0,$%1" => ="%f $%r,%y,%x" C="mul" A="$%n" 0=0 B="$%n" 1=1 */
/* "mul $%r,$%0,$%1" => ="%f $%r,%y,%x" C="mul" A="$%n" 0=0 B="$%n" 1=1 */
/* "xor $%r,$%0,$%1" => ="%f $%r,%y,%x" C="xor" A="$%n" 0=0 B="$%n" 1=1 */
/* "div $%r,$%0,$%1" => ="%f $%r,%y,%x" C="div" A="$%n" 0=0 B="$%n" 1=1 */
/* "div $%r,$%0,$%1" => ="%f $%r,%y,%x" C="div" A="$%n" 0=0 B="$%n" 1=1 */
/* "sll $%r,$%0,$%1" => ="%f $%r,%y,%x" C="sll" A="$%n" 0=0 B="$%n" 1=1 */
/* "sll $%r,$%0,$%1" => ="%f $%r,%y,%x" C="sll" A="$%n" 0=0 B="$%n" 1=1 */
/* "addu $%r,$%0,$%1" => ="%f $%r,%y,%x" C="addu" A="$%n" 0=0 B="$%n" 1=1 */
/* "addu $%r,$%0,$%1" => ="%f $%r,%y,%x" C="addu" A="$%n" 0=0 B="$%n" 1=1 */
/* "addu $%r,$%0,$%1" => ="%f $%r,%y,%x" C="addu" A="$%n" 0=0 B="$%n" 1=1 */
/* "and $%r,$%0,$%1" => ="%f $%r,%y,%x" C="and" A="$%n" 0=0 B="$%n" 1=1 */
/* "subu $%r,$%0,$%1" => ="%f $%r,%y,%x" C="subu" A="$%n" 0=0 B="$%n" 1=1 */
/* "subu $%r,$%0,$%1" => ="%f $%r,%y,%x" C="subu" A="$%n" 0=0 B="$%n" 1=1 */
/* "subu $%r,$%0,$%1" => ="%f $%r,%y,%x" C="subu" A="$%n" 0=0 B="$%n" 1=1 */
/* "sra $%r,$%0,$%1" => ="%f $%r,%y,%x" C="sra" A="$%n" 0=0 B="$%n" 1=1 */
/* "sra $%r,$%0,$%1" => ="%f $%r,%y,%x" C="sra" A="$%n" 0=0 B="$%n" 1=1 */
/* "or $%r,$%0,$%1" => ="%f $%r,%y,%x" C="or" A="$%n" 0=0 B="$%n" 1=1 */
/* "j $%0" => ="j %y" A="$%n" 0=0 */
/* "la $%r,%c" => ="la $%r,%y" 0A="%c" */
/* "move $%r,$%c" => ="move $%r,%y" A="$%c" */
/* "mov.%t $f%r,$f%c" => ="mov.%t $f%r,%y" A="$f%c" */
/* "move $%r,$%n" => ="move $%r,%y" A="$%n" */
/* "mov.%t $f%r,$f%n" => ="mov.%t $f%r,%y" A="$f%n" */
/* "sw $%k,0($sp)" => ="s%t $%k,%y" D="w" A="%c($sp)" a="0" */
/* "move $4,$%n" => ="move $%r,%y" c="4" A="$%n" */
/* "sw $%k,4($sp)" => ="s%t $%k,%y" D="w" A="%c($sp)" a="4" */
/* "move $5,$%n" => ="move $%r,%y" c="5" A="$%n" */
/* "sw $%k,8($sp)" => ="s%t $%k,%y" D="w" A="%c($sp)" a="8" */
/* "move $6,$%n" => ="move $%r,%y" c="6" A="$%n" */
/* "sw $%k,12($sp)" => ="s%t $%k,%y" D="w" A="%c($sp)" a="12" */
/* "move $7,$%n" => ="move $%r,%y" c="7" A="$%n" */
/* "s.d $f%k,0($sp)" => ="s.%t $f%k,%y" D="d" A="%c($sp)" a="0" */
/* "mfc1.d $4,$f%n" => ="mfc1.d $%r,%y" c="4" A="$f%n" */
/* "s.s $f%k,0($sp)" => ="s.%t $f%k,%y" D="s" A="%c($sp)" a="0" */
/* "mfc1 $4,$f%n" => ="mfc1 $%r,%y" c="4" A="$f%n" */
/* "s.%t $f%k,0($sp)" => ="s.%t $f%k,%y" A="%c($sp)" a="0" */
/* "mov.%t $f12,$f%n" => ="mov.%t $f%r,%y" c="12" A="$f%n" */
/* "s.%t $f%k,4($sp)" => ="s.%t $f%k,%y" A="%c($sp)" a="4" */
/* "mov.%t $f14,$f%n" => ="mov.%t $f%r,%y" c="14" A="$f%n" */
/* "s.%t $f%k,4($sp)" => ="s.%t $f%k,%y" A="%c($sp)" a="4" */
/* "mov.%t $f14,$f%n" => ="mov.%t $f%r,%y" c="14" A="$f%n" */
/* "s.%t $f%k,4($sp)" => ="s.%t $f%k,%y" A="%c($sp)" a="4" */
/* "mov.%t $f14,$f%n" => ="mov.%t $f%r,%y" c="14" A="$f%n" */
/* "s.%t $f%k,4($sp)" => ="s.%t $f%k,%y" A="%c($sp)" a="4" */
/* "mov.%t $f14,$f%n" => ="mov.%t $f%r,%y" c="14" A="$f%n" */
/* "s.%t $f%k,4($sp)" => ="s.%t $f%k,%y" A="%c($sp)" a="4" */
/* "mov.%t $f14,$f%n" => ="mov.%t $f%r,%y" c="14" A="$f%n" */
/* "s.%t $f%k,4($sp)" => ="s.%t $f%k,%y" A="%c($sp)" a="4" */
/* "mov.%t $f14,$f%n" => ="mov.%t $f%r,%y" c="14" A="$f%n" */
/* "s.%t $f%k,8($sp)" => ="s.%t $f%k,%y" A="%c($sp)" a="8" */
/* "mov.%t $f14,$f%n" => ="mov.%t $f%r,%y" c="14" A="$f%n" */
/* "s.%t $f%k,8($sp)" => ="s.%t $f%k,%y" A="%c($sp)" a="8" */
/* "mov.%t $f14,$f%n" => ="mov.%t $f%r,%y" c="14" A="$f%n" */
/* "s.%t $f%k,8($sp)" => ="s.%t $f%k,%y" A="%c($sp)" a="8" */
/* "mov.%t $f14,$f%n" => ="mov.%t $f%r,%y" c="14" A="$f%n" */
/* "s.%t $f%k,8($sp)" => ="s.%t $f%k,%y" A="%c($sp)" a="8" */
/* "mov.%t $f14,$f%n" => ="mov.%t $f%r,%y" c="14" A="$f%n" */
/* "s.%t $f%k,8($sp)" => ="s.%t $f%k,%y" A="%c($sp)" a="8" */
/* "mov.%t $f14,$f%n" => ="mov.%t $f%r,%y" c="14" A="$f%n" */
/* "s.%t $f%k,8($sp)" => ="s.%t $f%k,%y" A="%c($sp)" a="8" */
/* "mov.%t $f14,$f%n" => ="mov.%t $f%r,%y" c="14" A="$f%n" */
/* "s.d $f%k,8($sp)" => ="s.%t $f%k,%y" D="d" A="%c($sp)" a="8" */
/* "mfc1.d $6,$f%n" => ="mfc1.d $%r,%y" c="6" A="$f%n" */
/* "s.s $f%k,4($sp)" => ="s.%t $f%k,%y" D="s" A="%c($sp)" a="4" */
/* "mfc1 $5,$f%n" => ="mfc1 $%r,%y" c="5" A="$f%n" */
/* "s.s $f%k,8($sp)" => ="s.%t $f%k,%y" D="s" A="%c($sp)" a="8" */
/* "mfc1 $6,$f%n" => ="mfc1 $%r,%y" c="6" A="$f%n" */
/* "s.s $f%k,12($sp)" => ="s.%t $f%k,%y" D="s" A="%c($sp)" a="12" */
/* "mfc1 $7,$f%n" => ="mfc1 $%r,%y" c="7" A="$f%n" */
/* "mov.%t $f%r,$f%c" => ="mov.%t $f%r,%y" A="$f%c" */
/* "move $%r,$%c" => ="move $%r,%y" A="$%c" */
/* "move $%r,$%n" => ="move $%r,%y" A="$%n" */
/* "mov.%t $f%r,$f%n" => ="mov.%t $f%r,%y" A="$f%n" */
/* "move $%r,$%n" => ="move $%r,%y" A="$%n" */
/* "move $%r,$%n" => ="move $%r,%y" A="$%n" */
/* "move $%r,$%n" => ="move $%r,%y" A="$%n" */
/* "move $%r,$%n" => ="move $%r,%y" A="$%n" */
/* "move $%r,$%n" => ="move $%r,%y" A="$%n" */
/* "move $%r,$%n" => ="move $%r,%y" A="$%n" */
/* "move $%r,$%n" => ="move $%r,%y" A="$%n" */
/* "move $%r,$%n" => ="move $%r,%y" A="$%n" */
/* "move $%r,$%n" => ="move $%r,%y" A="$%n" */
/* "ld $%r,$f%n" => ="l%t $%r,%y" D="d" A="$f%n" */
/* "mov.%t $f%r,$f%n" => ="mov.%t $f%r,%y" A="$f%n" */
/* "mov.%t $f%r,$f%n" => ="mov.%t $f%r,%y" A="$f%n" */
/* "mov.%t $f%r,$f%n" => ="mov.%t $f%r,%y" A="$f%n" */
/* "mov.%t $f%r,$f%n" => ="mov.%t $f%r,%y" A="$f%n" */
/* "mov.%t $f%r,$f%n" => ="mov.%t $f%r,%y" A="$f%n" */
/* "mov.%t $f%r,$f%n" => ="mov.%t $f%r,%y" A="$f%n" */
/* "jal $%n  # real" => ="jal %y  # real" A="$%n" */
/* "la $%r,%c" => ="la $%r,%y" 0A="%c" */
/* "jal $%n  # int" => ="jal %y  # int" A="$%n" */
/* "la $%r,%c" => ="la $%r,%y" 0A="%c" */
/* "addu $%r,%y,$%n" => ="%f $%r,%y,%x" C="addu" B="$%n" */
/* "la $%r,%c" => ="la $%r,%y" 1A="%c" */
/* "la $%r,%c($%n)" => ="la $%r,%y" A="%c($%n)" */
/* "addu $%r,%y,$%n" => ="%f $%r,%y,%x" C="addu" B="$%n" */
/* "li $%r,%c" => ="li $%r,%y" 1A="%c" */
/* "la $%r,%c($%n)" => ="la $%r,%y" A="%c($%n)" */
/* "li $%r,%c" => ="li $%r,%y" 1A="%c" */
/* "li $%r,%c" => ="li $%r,%y" 1A="%c" */
/* "li $%r,0" => ="li $%r,%y" A="%c" a="0" */
/* "sll $%r,$%n,%x; sra $%r,$%r,%c" => ="sll $%r,%y,%x; %f $%r,$%r,%x" A="$%n" C="sra" B="%c" */
/* "lh $%r,%y" => ="l%t $%r,%y" 0D="h" */
/* "sll $%r,$%n,%x; sra $%r,$%r,%c" => ="sll $%r,%y,%x; %f $%r,$%r,%x" A="$%n" C="sra" B="%c" */
/* "lb $%r,%y" => ="l%t $%r,%y" 0D="b" */
/* "and $%r,$%n,65535" => ="%f $%r,%y,%x" C="and" A="$%n" B="%c" b="65535" */
/* "lh $%r,%y" => ="l%t $%r,%y" 0D="h" */
/* "and $%r,$%n,255" => ="%f $%r,%y,%x" C="and" A="$%n" B="%c" b="255" */
/* "lb $%r,%y" => ="l%t $%r,%y" 0D="b" */
/* "trunc.w.d $f2,$f%n,$%r; mfc1 $%r,$f2" => ="%f.%t $f2,%y,$%r; mfc1 $%r,$f2" C="trunc.w" D="d" A="$f%n" */
/* "cvt.d.s $f%r,%y" => ="%f.%t $f%r,%y" 0C="cvt.d" 0D="s" */
/* "move $%r,$%n" => ="move $%r,%y" A="$%n" */
/* "{sll|srl|sra} $%r,%y,%c" => ="%f $%r,%y,%x" C=@TMP1168 B="%c" */
#include "pseudos.c"

static char *shortopcodes[] = {
/* 608 */  "jal %y  # int",
/* 609 */  "jal %y  # real",
/* 610 */  "# asgnb",
/* 611 */  "# argb",
/* 612 */  "la $%r,%y",
/* 613 */  "mfc1 $%r,%y",
/* 614 */  "move $%r,%y",
/* 615 */  "%f $%r,%y",
/* 616 */  "li $%r,%y",
/* 617 */  "l%t $%r,%y",
/* 618 */  "%f.%t $f2,%y,$%r; mfc1 $%r,$f2",
/* 619 */  "%f $%r,%y,%x",
/* 620 */  "l%tu $%r,%y",
/* 621 */  "# nop",
/* 622 */  "# ret",
/* 623 */  "%f.%t $f%r,%y",
/* 624 */  "%f.%t $f%r,%y,%x",
/* 625 */  "sll $%r,%y,%x; %f $%r,$%r,%x",
/* 626 */  "# label",
/* 627 */  "b%f %y,%x,%r",
/* 628 */  "b %r",
/* 629 */  "j %y",
/* 630 */  "c.%f.%t %y,%x; bc1f %r",
/* 631 */  "c.%f.%t %y,%x; bc1t %r",
/* 632 */  "l.%t $f%r,%y",
/* 633 */  "li.%t $f%r,%y",
/* 634 */  "mfc1.d $%r,%y",
/* 635 */  "mov.%t $f%r,%y",
/* 636 */  "mtc1 %y,$f%r; %f.%t $f%r,$f%r",
/* 637 */  "s%t $%k,%y",
/* 638 */  "s.%t $f%k,%y",
/* 639 */  "%y:",
};

static char *asmstr0[] = {
	"(DUMMY)",
	"$%n",
	"$%c",
	"$f%n",
	"$f%c",
	"%c",
	"%c($%n)",
	"%c($sp)",
	"($%n)",
};

static char *asmstr2[] = {
	"(DUMMY)",
	"and",
	"cvt.d",
	"cvt.s",
	"eq",
	"mul",
	"ne",
	"not",
	"or",
	"sll",
	"sra",
	"srl",
	"trunc.w",
	"xor",
	"add",
	"div",
	"ge",
	"gt",
	"le",
	"lt",
	"neg",
	"rem",
	"sub",
	"addu",
	"divu",
	"geu",
	"gtu",
	"leu",
	"ltu",
	"negu",
	"remu",
	"subu",
};

static char *asmstr3[] = {
	"(DUMMY)",
	"s",
	"d",
	"b",
	"h",
	"w",
};

static struct symbol sym[] = {
{{"(DUMMY)"}},
{{"0"}},
{{"4"}},
{{"5"}},
{{"6"}},
{{"7"}},
{{"8"}},
{{"12"}},
{{"14"}},
{{"16"}},
{{"24"}},
{{"255"}},
{{"65535"}},
};

dclproto(static void rewrite,(Node));

dclproto(static void emit,(Node));

static void initgen() {
	opcodes = shortopcodes-608;
	IR->x.rewrite = rewrite;
	IR->emit = emit;
	asmstr[0] = asmstr0;
	asmstr[1] = asmstr0;
	asmstr[2] = asmstr2;
	asmstr[3] = asmstr3;
	sym[1].x.name = string("0");
	sym[2].x.name = string("4");
	sym[3].x.name = string("5");
	sym[4].x.name = string("6");
	sym[5].x.name = string("7");
	sym[6].x.name = string("8");
	sym[7].x.name = string("12");
	sym[8].x.name = string("14");
	sym[9].x.name = string("16");
	sym[10].x.name = string("24");
	sym[11].x.name = string("255");
	sym[12].x.name = string("65535");
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
	case 1:   /* $%n */
		*rbp++ = '$';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 2:   /* $%c */
		*rbp++ = '$';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 3:   /* $f%n */
		*rbp++ = '$';
		*rbp++ = 'f';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 4:   /* $f%c */
		*rbp++ = '$';
		*rbp++ = 'f';
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 5:   /* %c */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		return rbp;
	case 6:   /* %c($%n) */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = '$';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
		return rbp;
	case 7:   /* %c($sp) */
		assert(a->syms[n+0]);
		for (cp = a->syms[n+0]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = '(';
		*rbp++ = '$';
		*rbp++ = 's';
		*rbp++ = 'p';
		*rbp++ = ')';
		return rbp;
	case 8:   /* ($%n) */
		*rbp++ = '(';
		*rbp++ = '$';
		assert(a->kids[n+0]);
		assert(a->kids[n+0]->x.registered);
		assert(a->kids[n+0]->syms[RX]);
		for (cp = a->kids[n+0]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
		*rbp++ = ')';
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
		case 608: L608:   /* jal %y  # int */
			rbp = bp;
			*rbp++ = 'j';
			*rbp++ = 'a';
			*rbp++ = 'l';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ' ';
			*rbp++ = ' ';
			*rbp++ = '#';
			*rbp++ = ' ';
			*rbp++ = 'i';
			*rbp++ = 'n';
			*rbp++ = 't';
			bp = rbp;
			outs("\n");
			break;
		case 609: L609:   /* jal %y  # real */
			rbp = bp;
			*rbp++ = 'j';
			*rbp++ = 'a';
			*rbp++ = 'l';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ' ';
			*rbp++ = ' ';
			*rbp++ = '#';
			*rbp++ = ' ';
			*rbp++ = 'r';
			*rbp++ = 'e';
			*rbp++ = 'a';
			*rbp++ = 'l';
			bp = rbp;
			outs("\n");
			break;
		case 610: L610:   /* # asgnb */
			(*IR->x.emitspecial)(a);
			break;
		case 611: L611:   /* # argb */
			(*IR->x.emitspecial)(a);
			break;
		case 612: L612:   /* la $%r,%y */
			rbp = bp;
			*rbp++ = 'l';
			*rbp++ = 'a';
			*rbp++ = ' ';
			*rbp++ = '$';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 613: L613:   /* mfc1 $%r,%y */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'f';
			*rbp++ = 'c';
			*rbp++ = '1';
			*rbp++ = ' ';
			*rbp++ = '$';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 614: L614:   /* move $%r,%y */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = 'e';
			*rbp++ = ' ';
			*rbp++ = '$';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 615: L615:   /* %f $%r,%y */
			rbp = bp;
			assert(a->x.ints[2]);
			for (cp = asmstr2[a->x.ints[2]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			*rbp++ = '$';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 616: L616:   /* li $%r,%y */
			rbp = bp;
			*rbp++ = 'l';
			*rbp++ = 'i';
			*rbp++ = ' ';
			*rbp++ = '$';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 617: L617:   /* l%t $%r,%y */
			rbp = bp;
			*rbp++ = 'l';
			assert(a->x.ints[3]);
			cp = asmstr3[a->x.ints[3]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			*rbp++ = '$';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 618: L618:   /* %f.%t $f2,%y,$%r; mfc1 $%r,$f2 */
			rbp = bp;
			assert(a->x.ints[2]);
			for (cp = asmstr2[a->x.ints[2]]; *cp; *rbp++ = *cp++);
			*rbp++ = '.';
			assert(a->x.ints[3]);
			cp = asmstr3[a->x.ints[3]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			*rbp++ = '$';
			*rbp++ = 'f';
			*rbp++ = '2';
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = '$';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'm';
			*rbp++ = 'f';
			*rbp++ = 'c';
			*rbp++ = '1';
			*rbp++ = ' ';
			*rbp++ = '$';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			*rbp++ = '$';
			*rbp++ = 'f';
			*rbp++ = '2';
			bp = rbp;
			outs("\n");
			break;
		case 619: L619:   /* %f $%r,%y,%x */
			rbp = bp;
			assert(a->x.ints[2]);
			for (cp = asmstr2[a->x.ints[2]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			*rbp++ = '$';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			bp = rbp;
			outs("\n");
			break;
		case 620: L620:   /* l%tu $%r,%y */
			rbp = bp;
			*rbp++ = 'l';
			assert(a->x.ints[3]);
			cp = asmstr3[a->x.ints[3]];
			*rbp++ = cp[0];
			*rbp++ = 'u';
			*rbp++ = ' ';
			*rbp++ = '$';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 621: L621:   /* # nop */
			(*IR->x.emitspecial)(a);
			break;
		case 622: L622:   /* # ret */
			(*IR->x.emitspecial)(a);
			break;
		case 623: L623:   /* %f.%t $f%r,%y */
			rbp = bp;
			assert(a->x.ints[2]);
			for (cp = asmstr2[a->x.ints[2]]; *cp; *rbp++ = *cp++);
			*rbp++ = '.';
			assert(a->x.ints[3]);
			cp = asmstr3[a->x.ints[3]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			*rbp++ = '$';
			*rbp++ = 'f';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 624: L624:   /* %f.%t $f%r,%y,%x */
			rbp = bp;
			assert(a->x.ints[2]);
			for (cp = asmstr2[a->x.ints[2]]; *cp; *rbp++ = *cp++);
			*rbp++ = '.';
			assert(a->x.ints[3]);
			cp = asmstr3[a->x.ints[3]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			*rbp++ = '$';
			*rbp++ = 'f';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			bp = rbp;
			outs("\n");
			break;
		case 625: L625:   /* sll $%r,%y,%x; %f $%r,$%r,%x */
			rbp = bp;
			*rbp++ = 's';
			*rbp++ = 'l';
			*rbp++ = 'l';
			*rbp++ = ' ';
			*rbp++ = '$';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			assert(a->x.ints[2]);
			for (cp = asmstr2[a->x.ints[2]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			*rbp++ = '$';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			*rbp++ = '$';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			bp = rbp;
			outs("\n");
			break;
		case 626: L626:   /* # label */
			(*IR->x.emitspecial)(a);
			break;
		case 627: L627:   /* b%f %y,%x,%r */
			rbp = bp;
			*rbp++ = 'b';
			assert(a->x.ints[2]);
			for (cp = asmstr2[a->x.ints[2]]; *cp; *rbp++ = *cp++);
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			*rbp++ = ',';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			bp = rbp;
			outs("\n");
			break;
		case 628: L628:   /* b %r */
			rbp = bp;
			*rbp++ = 'b';
			*rbp++ = ' ';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			bp = rbp;
			outs("\n");
			break;
		case 629: L629:   /* j %y */
			rbp = bp;
			*rbp++ = 'j';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 630: L630:   /* c.%f.%t %y,%x; bc1f %r */
			rbp = bp;
			*rbp++ = 'c';
			*rbp++ = '.';
			assert(a->x.ints[2]);
			for (cp = asmstr2[a->x.ints[2]]; *cp; *rbp++ = *cp++);
			*rbp++ = '.';
			assert(a->x.ints[3]);
			cp = asmstr3[a->x.ints[3]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'b';
			*rbp++ = 'c';
			*rbp++ = '1';
			*rbp++ = 'f';
			*rbp++ = ' ';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			bp = rbp;
			outs("\n");
			break;
		case 631: L631:   /* c.%f.%t %y,%x; bc1t %r */
			rbp = bp;
			*rbp++ = 'c';
			*rbp++ = '.';
			assert(a->x.ints[2]);
			for (cp = asmstr2[a->x.ints[2]]; *cp; *rbp++ = *cp++);
			*rbp++ = '.';
			assert(a->x.ints[3]);
			cp = asmstr3[a->x.ints[3]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 1);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			*rbp++ = 'b';
			*rbp++ = 'c';
			*rbp++ = '1';
			*rbp++ = 't';
			*rbp++ = ' ';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			bp = rbp;
			outs("\n");
			break;
		case 632: L632:   /* l.%t $f%r,%y */
			rbp = bp;
			*rbp++ = 'l';
			*rbp++ = '.';
			assert(a->x.ints[3]);
			cp = asmstr3[a->x.ints[3]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			*rbp++ = '$';
			*rbp++ = 'f';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 633: L633:   /* li.%t $f%r,%y */
			rbp = bp;
			*rbp++ = 'l';
			*rbp++ = 'i';
			*rbp++ = '.';
			assert(a->x.ints[3]);
			cp = asmstr3[a->x.ints[3]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			*rbp++ = '$';
			*rbp++ = 'f';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 634: L634:   /* mfc1.d $%r,%y */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'f';
			*rbp++ = 'c';
			*rbp++ = '1';
			*rbp++ = '.';
			*rbp++ = 'd';
			*rbp++ = ' ';
			*rbp++ = '$';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 635: L635:   /* mov.%t $f%r,%y */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 'o';
			*rbp++ = 'v';
			*rbp++ = '.';
			assert(a->x.ints[3]);
			cp = asmstr3[a->x.ints[3]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			*rbp++ = '$';
			*rbp++ = 'f';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 636: L636:   /* mtc1 %y,$f%r; %f.%t $f%r,$f%r */
			rbp = bp;
			*rbp++ = 'm';
			*rbp++ = 't';
			*rbp++ = 'c';
			*rbp++ = '1';
			*rbp++ = ' ';
			rbp = emit0(rbp, a, 0);
			*rbp++ = ',';
			*rbp++ = '$';
			*rbp++ = 'f';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			bp = rbp; outs(";"); rbp = bp;
			*rbp++ = ' ';
			assert(a->x.ints[2]);
			for (cp = asmstr2[a->x.ints[2]]; *cp; *rbp++ = *cp++);
			*rbp++ = '.';
			assert(a->x.ints[3]);
			cp = asmstr3[a->x.ints[3]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			*rbp++ = '$';
			*rbp++ = 'f';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			*rbp++ = '$';
			*rbp++ = 'f';
		assert(a->syms[2]);
		for (cp = a->syms[2]->x.name; *cp; *rbp++ = *cp++);
			bp = rbp;
			outs("\n");
			break;
		case 637: L637:   /* s%t $%k,%y */
			rbp = bp;
			*rbp++ = 's';
			assert(a->x.ints[3]);
			cp = asmstr3[a->x.ints[3]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			*rbp++ = '$';
			assert(a->kids[2]);
			assert(a->kids[2]->x.registered);
		assert(a->kids[2]->syms[RX]);
		for (cp = a->kids[2]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 638: L638:   /* s.%t $f%k,%y */
			rbp = bp;
			*rbp++ = 's';
			*rbp++ = '.';
			assert(a->x.ints[3]);
			cp = asmstr3[a->x.ints[3]];
			*rbp++ = cp[0];
			*rbp++ = ' ';
			*rbp++ = '$';
			*rbp++ = 'f';
			assert(a->kids[2]);
			assert(a->kids[2]->x.registered);
		assert(a->kids[2]->syms[RX]);
		for (cp = a->kids[2]->syms[RX]->x.name; *cp; *rbp++ = *cp++);
			*rbp++ = ',';
			rbp = emit0(rbp, a, 0);
			bp = rbp;
			outs("\n");
			break;
		case 639: L639:   /* %y: */
			rbp = bp;
			rbp = emit0(rbp, a, 0);
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
	case 608: L608:   /* jal %y  # int */
		dumptree(a, 608);
		spill(0x000f0ff3, FREG, a);  spill(0x0300ff00, IREG, a);  
		a->op = 608;
		break;
	case 609: L609:   /* jal %y  # real */
		dumptree(a, 609);
		spill(0x0300ff04, IREG, a);  spill(0x000f0ff0, FREG, a);  
		a->op = 609;
		break;
	case 610: L610:   /* # asgnb */
		dumptree(a, 610);
		spill(0x10c, IREG, a);  
		a->op = 610;
		return;
	case 611: L611:   /* # argb */
		dumptree(a, 611);
		spill(0x10c, IREG, a);  
		a->op = 611;
		return;
	case 612: L612:   /* la $%r,%y */
		dumptree(a, 612);
		a->op = 612;
		break;
	case 613: L613:   /* mfc1 $%r,%y */
		dumptree(a, 613);
		a->op = 613;
		break;
	case 614: L614:   /* move $%r,%y */
		dumptree(a, 614);
		if (
		a->x.ints[0] == 1  /* $%n */
		&& a->syms[2] == a->kids[0]->syms[2]
		) {
			goto L621;   /* # nop */
		}
		a->op = 614;
		break;
	case 615: L615:   /* %f $%r,%y */
		dumptree(a, 615);
		a->op = 615;
		break;
	case 616: L616:   /* li $%r,%y */
		dumptree(a, 616);
		a->op = 616;
		break;
	case 617: L617:   /* l%t $%r,%y */
		dumptree(a, 617);
		a->op = 617;
		break;
	case 618: L618:   /* %f.%t $f2,%y,$%r; mfc1 $%r,$f2 */
		dumptree(a, 618);
		a->op = 618;
		break;
	case 619: L619:   /* %f $%r,%y,%x */
		dumptree(a, 619);
		if (
		"\1\0\0\0\0\0\0\0\0\1\1\1\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"[a->x.ints[2]]  /* <TMP1168> */
		&& a->x.ints[1] == 5  /* %c */
		&& lop(a,1,5)
		) {
			goto L619;   /* %f $%r,%y,%x */
		}
		a->op = 619;
		break;
	case 620: L620:   /* l%tu $%r,%y */
		dumptree(a, 620);
		a->op = 620;
		break;
	case 621: L621:   /* # nop */
		dumptree(a, 621);
		a->op = 621;
		return;
	case 622: L622:   /* # ret */
		dumptree(a, 622);
		a->op = 622;
		return;
	case 623: L623:   /* %f.%t $f%r,%y */
		dumptree(a, 623);
		a->op = 623;
		break;
	case 624: L624:   /* %f.%t $f%r,%y,%x */
		dumptree(a, 624);
		a->op = 624;
		break;
	case 625: L625:   /* sll $%r,%y,%x; %f $%r,$%r,%x */
		dumptree(a, 625);
		a->op = 625;
		break;
	case 626: L626:   /* # label */
		dumptree(a, 626);
		a->op = 626;
		return;
	case 627: L627:   /* b%f %y,%x,%r */
		dumptree(a, 627);
		a->op = 627;
		break;
	case 628: L628:   /* b %r */
		dumptree(a, 628);
		a->op = 628;
		break;
	case 629: L629:   /* j %y */
		dumptree(a, 629);
		a->op = 629;
		break;
	case 630: L630:   /* c.%f.%t %y,%x; bc1f %r */
		dumptree(a, 630);
		a->op = 630;
		break;
	case 631: L631:   /* c.%f.%t %y,%x; bc1t %r */
		dumptree(a, 631);
		a->op = 631;
		break;
	case 632: L632:   /* l.%t $f%r,%y */
		dumptree(a, 632);
		a->op = 632;
		break;
	case 633: L633:   /* li.%t $f%r,%y */
		dumptree(a, 633);
		a->op = 633;
		break;
	case 634: L634:   /* mfc1.d $%r,%y */
		dumptree(a, 634);
		a->op = 634;
		break;
	case 635: L635:   /* mov.%t $f%r,%y */
		dumptree(a, 635);
		a->op = 635;
		break;
	case 636: L636:   /* mtc1 %y,$f%r; %f.%t $f%r,$f%r */
		dumptree(a, 636);
		a->op = 636;
		break;
	case 637: L637:   /* s%t $%k,%y */
		dumptree(a, 637);
		a->op = 637;
		break;
	case 638: L638:   /* s.%t $f%k,%y */
		dumptree(a, 638);
		a->op = 638;
		break;
	case 639: L639:   /* %y: */
		dumptree(a, 639);
		a->op = 639;
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
		a->x.ints[0] = 5;  /* %c */
		a->x.ints[3] = 1;  /* s */
		goto L633;   /* li.%t $f%r,%y */
	case 18: L18:   /* CNSTD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		a->x.ints[0] = 5;  /* %c */
		a->x.ints[3] = 2;  /* d */
		goto L633;   /* li.%t $f%r,%y */
	case 19: L19:   /* CNSTC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		a->x.ints[0] = 5;  /* %c */
		goto L616;   /* li $%r,%y */
	case 20: L20:   /* CNSTS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		a->x.ints[0] = 5;  /* %c */
		goto L616;   /* li $%r,%y */
	case 21: L21:   /* CNSTI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		a->x.ints[0] = 5;  /* %c */
		goto L616;   /* li $%r,%y */
	case 22: L22:   /* CNSTU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		a->x.ints[0] = 5;  /* %c */
		goto L616;   /* li $%r,%y */
	case 23: L23:   /* CNSTP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[0] = 5;  /* %c */
		goto L616;   /* li $%r,%y */
	case 33: L33:   /* ARGF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[0] = 7;  /* %c($sp) */
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[3] = 1;  /* s */
		goto L638;   /* s.%t $f%k,%y */
	case 34: L34:   /* ARGD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[0] = 7;  /* %c($sp) */
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[3] = 2;  /* d */
		goto L638;   /* s.%t $f%k,%y */
	case 37: L37:   /* ARGI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[0] = 7;  /* %c($sp) */
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[3] = 5;  /* w */
		goto L637;   /* s%t $%k,%y */
	case 39: L39:   /* ARGP */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[0] = 7;  /* %c($sp) */
		a->kids[2] = a->kids[0];
		a->kids[0] = 0;
		a->x.ints[3] = 5;  /* w */
		goto L637;   /* s%t $%k,%y */
	case 41: L41:   /* ARGB */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		goto L611;   /* # argb */
	case 49: L49:   /* ASGNF */
		dumptree(a, a->op);
		aset(a,0);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[0] = 8;  /* ($%n) */
		a->kids[2] = a->kids[1];
		a->kids[1] = 0;
		a->x.ints[3] = 1;  /* s */
		goto L638;   /* s.%t $f%k,%y */
	case 50: L50:   /* ASGND */
		dumptree(a, a->op);
		aset(a,0);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[0] = 8;  /* ($%n) */
		a->kids[2] = a->kids[1];
		a->kids[1] = 0;
		a->x.ints[3] = 2;  /* d */
		goto L638;   /* s.%t $f%k,%y */
	case 51: L51:   /* ASGNC */
		dumptree(a, a->op);
		aset(a,0);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[0] = 8;  /* ($%n) */
		a->kids[2] = a->kids[1];
		a->kids[1] = 0;
		a->x.ints[3] = 3;  /* b */
		goto L637;   /* s%t $%k,%y */
	case 52: L52:   /* ASGNS */
		dumptree(a, a->op);
		aset(a,0);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[0] = 8;  /* ($%n) */
		a->kids[2] = a->kids[1];
		a->kids[1] = 0;
		a->x.ints[3] = 4;  /* h */
		goto L637;   /* s%t $%k,%y */
	case 53: L53:   /* ASGNI */
		dumptree(a, a->op);
		aset(a,0);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[0] = 8;  /* ($%n) */
		a->kids[2] = a->kids[1];
		a->kids[1] = 0;
		a->x.ints[3] = 5;  /* w */
		goto L637;   /* s%t $%k,%y */
	case 55: L55:   /* ASGNP */
		dumptree(a, a->op);
		aset(a,0);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[0] = 8;  /* ($%n) */
		a->kids[2] = a->kids[1];
		a->kids[1] = 0;
		a->x.ints[3] = 5;  /* w */
		goto L637;   /* s%t $%k,%y */
	case 57: L57:   /* ASGNB */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		goto L610;   /* # asgnb */
	case 65: L65:   /* INDIRF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[0] = 8;  /* ($%n) */
		a->x.ints[3] = 1;  /* s */
		goto L632;   /* l.%t $f%r,%y */
	case 66: L66:   /* INDIRD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[0] = 8;  /* ($%n) */
		a->x.ints[3] = 2;  /* d */
		goto L632;   /* l.%t $f%r,%y */
	case 67: L67:   /* INDIRC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[0] = 8;  /* ($%n) */
		a->x.ints[3] = 3;  /* b */
		goto L617;   /* l%t $%r,%y */
	case 68: L68:   /* INDIRS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[0] = 8;  /* ($%n) */
		a->x.ints[3] = 4;  /* h */
		goto L617;   /* l%t $%r,%y */
	case 69: L69:   /* INDIRI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[0] = 8;  /* ($%n) */
		a->x.ints[3] = 5;  /* w */
		goto L617;   /* l%t $%r,%y */
	case 71: L71:   /* INDIRP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[0] = 8;  /* ($%n) */
		a->x.ints[3] = 5;  /* w */
		goto L617;   /* l%t $%r,%y */
	case 73: L73:   /* INDIRB */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		aset(a,0);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		goto L614;   /* move $%r,%y */
	case 85: L85:   /* CVCI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[1] = 5;  /* %c */
		a->x.ints[2] = 10;  /* sra */
		a->syms[1] = &sym[10];  /* 24 */
		a->x.ints[1] = 5;  /* %c */
		a->x.ints[0] = 1;  /* $%n */
		goto L625;   /* sll $%r,%y,%x; %f $%r,$%r,%x */
	case 86: L86:   /* CVCU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->syms[1] = &sym[11];  /* 255 */
		a->x.ints[1] = 5;  /* %c */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 1;  /* and */
		goto L619;   /* %f $%r,%y,%x */
	case 97: L97:   /* CVDF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 2;  /* d */
		a->x.ints[2] = 3;  /* cvt.s */
		goto L623;   /* %f.%t $f%r,%y */
	case 101: L101:   /* CVDI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 2;  /* d */
		a->x.ints[2] = 12;  /* trunc.w */
		goto L618;   /* %f.%t $f2,%y,$%r; mfc1 $%r,$f2 */
	case 114: L114:   /* CVFD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 1;  /* s */
		a->x.ints[2] = 2;  /* cvt.d */
		goto L623;   /* %f.%t $f%r,%y */
	case 130: L130:   /* CVID */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		a->x.ints[3] = 5;  /* w */
		a->x.ints[2] = 2;  /* cvt.d */
		a->x.ints[0] = 1;  /* $%n */
		goto L636;   /* mtc1 %y,$f%r; %f.%t $f%r,$f%r */
	case 131: L131:   /* CVIC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		goto L614;   /* move $%r,%y */
	case 132: L132:   /* CVIS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		goto L614;   /* move $%r,%y */
	case 134: L134:   /* CVIU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		goto L614;   /* move $%r,%y */
	case 150: L150:   /* CVPU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		goto L614;   /* move $%r,%y */
	case 165: L165:   /* CVSI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[1] = 5;  /* %c */
		a->x.ints[2] = 10;  /* sra */
		a->syms[1] = &sym[9];  /* 16 */
		a->x.ints[1] = 5;  /* %c */
		a->x.ints[0] = 1;  /* $%n */
		goto L625;   /* sll $%r,%y,%x; %f $%r,$%r,%x */
	case 166: L166:   /* CVSU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->syms[1] = &sym[12];  /* 65535 */
		a->x.ints[1] = 5;  /* %c */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 1;  /* and */
		goto L619;   /* %f $%r,%y,%x */
	case 179: L179:   /* CVUC */
		dumptree(a, a->op);
		setreg(a, rmap[C]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		goto L614;   /* move $%r,%y */
	case 180: L180:   /* CVUS */
		dumptree(a, a->op);
		setreg(a, rmap[S]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		goto L614;   /* move $%r,%y */
	case 181: L181:   /* CVUI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		goto L614;   /* move $%r,%y */
	case 183: L183:   /* CVUP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		goto L614;   /* move $%r,%y */
	case 193: L193:   /* NEGF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 1;  /* s */
		a->x.ints[2] = 20;  /* neg */
		goto L623;   /* %f.%t $f%r,%y */
	case 194: L194:   /* NEGD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 2;  /* d */
		a->x.ints[2] = 20;  /* neg */
		goto L623;   /* %f.%t $f%r,%y */
	case 197: L197:   /* NEGI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 29;  /* negu */
		goto L615;   /* %f $%r,%y */
	case 209: L209:   /* CALLF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		setreg(a,f0);
		goto L609;   /* jal %y  # real */
	case 210: L210:   /* CALLD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		setreg(a,f0);
		goto L609;   /* jal %y  # real */
	case 213: L213:   /* CALLI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		setreg(a,ireg[2]);
		goto L608;   /* jal %y  # int */
	case 216: L216:   /* CALLV */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		setreg(a,ireg[2]);
		goto L608;   /* jal %y  # int */
	case 217: L217:   /* CALLB */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[0] = 1;  /* $%n */
		setreg(a,ireg[2]),rtarget(a,1,ireg[4]);
		goto L608;   /* jal %y  # int */
	case 225: L225:   /* LOADF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 1;  /* s */
		goto L635;   /* mov.%t $f%r,%y */
	case 226: L226:   /* LOADD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 2;  /* d */
		goto L635;   /* mov.%t $f%r,%y */
	case 227: L227:   /* LOADC */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[3] = 3;  /* b */
		goto L614;   /* move $%r,%y */
	case 228: L228:   /* LOADS */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[3] = 4;  /* h */
		goto L614;   /* move $%r,%y */
	case 229: L229:   /* LOADI */
	case 230: L230:   /* LOADU */
	case 231: L231:   /* LOADP */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[3] = 5;  /* w */
		goto L614;   /* move $%r,%y */
	case 241: L241:   /* RETF */
	case 242: L242:   /* RETD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rtarget(a,0,f0);
		goto L622;   /* # ret */
	case 245: L245:   /* RETI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rtarget(a,0,ireg[2]);
		goto L622;   /* # ret */
	case 248: L248:   /* RETV */
		dumptree(a, a->op);
		goto L622;   /* # ret */
	case 263: L263:   /* ADDRGP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[0] = 5;  /* %c */
		goto L612;   /* la $%r,%y */
	case 279: L279:   /* ADDRFP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[0] = 7;  /* %c($sp) */
		goto L612;   /* la $%r,%y */
	case 295: L295:   /* ADDRLP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		a->x.ints[0] = 7;  /* %c($sp) */
		goto L612;   /* la $%r,%y */
	case 305: L305:   /* ADDF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 1;  /* s */
		a->x.ints[2] = 14;  /* add */
		goto L624;   /* %f.%t $f%r,%y,%x */
	case 306: L306:   /* ADDD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 2;  /* d */
		a->x.ints[2] = 14;  /* add */
		goto L624;   /* %f.%t $f%r,%y,%x */
	case 309: L309:   /* ADDI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 23;  /* addu */
		goto L619;   /* %f $%r,%y,%x */
	case 310: L310:   /* ADDU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 23;  /* addu */
		goto L619;   /* %f $%r,%y,%x */
	case 311: L311:   /* ADDP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 23;  /* addu */
		goto L619;   /* %f $%r,%y,%x */
	case 321: L321:   /* SUBF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 1;  /* s */
		a->x.ints[2] = 22;  /* sub */
		goto L624;   /* %f.%t $f%r,%y,%x */
	case 322: L322:   /* SUBD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 2;  /* d */
		a->x.ints[2] = 22;  /* sub */
		goto L624;   /* %f.%t $f%r,%y,%x */
	case 325: L325:   /* SUBI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 31;  /* subu */
		goto L619;   /* %f $%r,%y,%x */
	case 326: L326:   /* SUBU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 31;  /* subu */
		goto L619;   /* %f $%r,%y,%x */
	case 327: L327:   /* SUBP */
		dumptree(a, a->op);
		setreg(a, rmap[P]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 31;  /* subu */
		goto L619;   /* %f $%r,%y,%x */
	case 341: L341:   /* LSHI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 9;  /* sll */
		goto L619;   /* %f $%r,%y,%x */
	case 342: L342:   /* LSHU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 9;  /* sll */
		goto L619;   /* %f $%r,%y,%x */
	case 357: L357:   /* MODI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 21;  /* rem */
		goto L619;   /* %f $%r,%y,%x */
	case 358: L358:   /* MODU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 30;  /* remu */
		goto L619;   /* %f $%r,%y,%x */
	case 373: L373:   /* RSHI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 10;  /* sra */
		goto L619;   /* %f $%r,%y,%x */
	case 374: L374:   /* RSHU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 11;  /* srl */
		goto L619;   /* %f $%r,%y,%x */
	case 390: L390:   /* BANDU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 1;  /* and */
		goto L619;   /* %f $%r,%y,%x */
	case 406: L406:   /* BCOMU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 7;  /* not */
		goto L615;   /* %f $%r,%y */
	case 422: L422:   /* BORU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 8;  /* or */
		goto L619;   /* %f $%r,%y,%x */
	case 438: L438:   /* BXORU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 13;  /* xor */
		goto L619;   /* %f $%r,%y,%x */
	case 449: L449:   /* DIVF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 1;  /* s */
		a->x.ints[2] = 15;  /* div */
		goto L624;   /* %f.%t $f%r,%y,%x */
	case 450: L450:   /* DIVD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 2;  /* d */
		a->x.ints[2] = 15;  /* div */
		goto L624;   /* %f.%t $f%r,%y,%x */
	case 453: L453:   /* DIVI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 15;  /* div */
		goto L619;   /* %f $%r,%y,%x */
	case 454: L454:   /* DIVU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 24;  /* divu */
		goto L619;   /* %f $%r,%y,%x */
	case 465: L465:   /* MULF */
		dumptree(a, a->op);
		setreg(a, rmap[F]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 1;  /* s */
		a->x.ints[2] = 5;  /* mul */
		goto L624;   /* %f.%t $f%r,%y,%x */
	case 466: L466:   /* MULD */
		dumptree(a, a->op);
		setreg(a, rmap[D]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 2;  /* d */
		a->x.ints[2] = 5;  /* mul */
		goto L624;   /* %f.%t $f%r,%y,%x */
	case 469: L469:   /* MULI */
		dumptree(a, a->op);
		setreg(a, rmap[I]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 5;  /* mul */
		goto L619;   /* %f $%r,%y,%x */
	case 470: L470:   /* MULU */
		dumptree(a, a->op);
		setreg(a, rmap[U]);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 5;  /* mul */
		goto L619;   /* %f $%r,%y,%x */
	case 481: L481:   /* EQF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 1;  /* s */
		a->x.ints[2] = 4;  /* eq */
		goto L631;   /* c.%f.%t %y,%x; bc1t %r */
	case 482: L482:   /* EQD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 2;  /* d */
		a->x.ints[2] = 4;  /* eq */
		goto L631;   /* c.%f.%t %y,%x; bc1t %r */
	case 485: L485:   /* EQI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 4;  /* eq */
		goto L627;   /* b%f %y,%x,%r */
	case 497: L497:   /* GEF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 1;  /* s */
		a->x.ints[2] = 19;  /* lt */
		goto L630;   /* c.%f.%t %y,%x; bc1f %r */
	case 498: L498:   /* GED */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 2;  /* d */
		a->x.ints[2] = 19;  /* lt */
		goto L630;   /* c.%f.%t %y,%x; bc1f %r */
	case 501: L501:   /* GEI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 16;  /* ge */
		goto L627;   /* b%f %y,%x,%r */
	case 502: L502:   /* GEU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 25;  /* geu */
		goto L627;   /* b%f %y,%x,%r */
	case 513: L513:   /* GTF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 1;  /* s */
		a->x.ints[2] = 18;  /* le */
		goto L630;   /* c.%f.%t %y,%x; bc1f %r */
	case 514: L514:   /* GTD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 2;  /* d */
		a->x.ints[2] = 18;  /* le */
		goto L630;   /* c.%f.%t %y,%x; bc1f %r */
	case 517: L517:   /* GTI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 17;  /* gt */
		goto L627;   /* b%f %y,%x,%r */
	case 518: L518:   /* GTU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 26;  /* gtu */
		goto L627;   /* b%f %y,%x,%r */
	case 529: L529:   /* LEF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 1;  /* s */
		a->x.ints[2] = 18;  /* le */
		goto L631;   /* c.%f.%t %y,%x; bc1t %r */
	case 530: L530:   /* LED */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 2;  /* d */
		a->x.ints[2] = 18;  /* le */
		goto L631;   /* c.%f.%t %y,%x; bc1t %r */
	case 533: L533:   /* LEI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 18;  /* le */
		goto L627;   /* b%f %y,%x,%r */
	case 534: L534:   /* LEU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 27;  /* leu */
		goto L627;   /* b%f %y,%x,%r */
	case 545: L545:   /* LTF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 1;  /* s */
		a->x.ints[2] = 19;  /* lt */
		goto L631;   /* c.%f.%t %y,%x; bc1t %r */
	case 546: L546:   /* LTD */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 2;  /* d */
		a->x.ints[2] = 19;  /* lt */
		goto L631;   /* c.%f.%t %y,%x; bc1t %r */
	case 549: L549:   /* LTI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 19;  /* lt */
		goto L627;   /* b%f %y,%x,%r */
	case 550: L550:   /* LTU */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 28;  /* ltu */
		goto L627;   /* b%f %y,%x,%r */
	case 561: L561:   /* NEF */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 1;  /* s */
		a->x.ints[2] = 4;  /* eq */
		goto L630;   /* c.%f.%t %y,%x; bc1f %r */
	case 562: L562:   /* NED */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 3;  /* $f%n */
		a->x.ints[0] = 3;  /* $f%n */
		a->x.ints[3] = 2;  /* d */
		a->x.ints[2] = 4;  /* eq */
		goto L630;   /* c.%f.%t %y,%x; bc1f %r */
	case 565: L565:   /* NEI */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		rewrite(a->kids[1]);
		a->syms[2] = a->syms[0];
		a->x.ints[1] = 1;  /* $%n */
		a->x.ints[0] = 1;  /* $%n */
		a->x.ints[2] = 6;  /* ne */
		goto L627;   /* b%f %y,%x,%r */
	case 584: L584:   /* JUMPV */
		dumptree(a, a->op);
		rewrite(a->kids[0]);
		a->x.ints[0] = 1;  /* $%n */
		goto L629;   /* j %y */
	case 600: L600:   /* LABELV */
		dumptree(a, a->op);
		a->x.ints[0] = 5;  /* %c */
		goto L639;   /* %y: */
	case 608: L608:   /* jal %y  # int */
		dumptree(a, 608);
		b = a->kids[0];
		if (
		a->x.ints[0] == 1  /* $%n */
		&& b->op == 612  /* la $%r,%y */
		&& b->x.ints[0] == 5  /* %c */
		) {
			if (--b->count > 0)
				if (c=b->kids[0]) ++c->count;
			a->syms[0] = b->syms[0];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L608;   /* jal %y  # int */
		}
		a->op = 608;
		break;
	case 609: L609:   /* jal %y  # real */
		dumptree(a, 609);
		b = a->kids[0];
		if (
		a->x.ints[0] == 1  /* $%n */
		&& b->op == 612  /* la $%r,%y */
		&& b->x.ints[0] == 5  /* %c */
		) {
			if (--b->count > 0)
				if (c=b->kids[0]) ++c->count;
			a->syms[0] = b->syms[0];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L609;   /* jal %y  # real */
		}
		a->op = 609;
		break;
	case 610: L610:   /* # asgnb */
		dumptree(a, 610);
		a->op = 610;
		break;
	case 611: L611:   /* # argb */
		dumptree(a, 611);
		a->op = 611;
		break;
	case 612: L612:   /* la $%r,%y */
		dumptree(a, 612);
		a->op = 612;
		break;
	case 613: L613:   /* mfc1 $%r,%y */
		dumptree(a, 613);
		a->op = 613;
		break;
	case 614: L614:   /* move $%r,%y */
		dumptree(a, 614);
		switch (a->x.ints[0]) {
		case 1:   /* $%n */
			b = a->kids[0];
			switch (b->op) {
			case 612:   /* la $%r,%y */
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[1]) ++c->count;
					}
					a->syms[1] = b->syms[1];
					a->kids[1] = b->kids[1];
					a->x.ints[1] = b->x.ints[1];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					a->x.ints[2] = b->x.ints[2];
					a->x.ints[3] = b->x.ints[3];
					goto L612;   /* la $%r,%y */
				}
				break;
			case 613:   /* mfc1 $%r,%y */
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[1]) ++c->count;
					}
					a->syms[1] = b->syms[1];
					a->kids[1] = b->kids[1];
					a->x.ints[1] = b->x.ints[1];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					a->x.ints[2] = b->x.ints[2];
					a->x.ints[3] = b->x.ints[3];
					goto L613;   /* mfc1 $%r,%y */
				}
				break;
			case 614:   /* move $%r,%y */
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[1]) ++c->count;
					}
					a->syms[1] = b->syms[1];
					a->kids[1] = b->kids[1];
					a->x.ints[1] = b->x.ints[1];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					a->x.ints[2] = b->x.ints[2];
					a->x.ints[3] = b->x.ints[3];
					goto L614;   /* move $%r,%y */
				}
				break;
			case 615:   /* %f $%r,%y */
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[1]) ++c->count;
					}
					a->syms[1] = b->syms[1];
					a->kids[1] = b->kids[1];
					a->x.ints[1] = b->x.ints[1];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					a->x.ints[2] = b->x.ints[2];
					a->x.ints[3] = b->x.ints[3];
					goto L615;   /* %f $%r,%y */
				}
				break;
			case 616:   /* li $%r,%y */
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[1]) ++c->count;
					}
					a->syms[1] = b->syms[1];
					a->kids[1] = b->kids[1];
					a->x.ints[1] = b->x.ints[1];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					a->x.ints[2] = b->x.ints[2];
					a->x.ints[3] = b->x.ints[3];
					goto L616;   /* li $%r,%y */
				}
				break;
			case 617:   /* l%t $%r,%y */
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[1]) ++c->count;
					}
					a->syms[1] = b->syms[1];
					a->kids[1] = b->kids[1];
					a->x.ints[1] = b->x.ints[1];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					a->x.ints[2] = b->x.ints[2];
					a->x.ints[3] = b->x.ints[3];
					goto L617;   /* l%t $%r,%y */
				}
				break;
			case 618:   /* %f.%t $f2,%y,$%r; mfc1 $%r,$f2 */
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[1]) ++c->count;
					}
					a->syms[1] = b->syms[1];
					a->kids[1] = b->kids[1];
					a->x.ints[1] = b->x.ints[1];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					a->x.ints[2] = b->x.ints[2];
					a->x.ints[3] = b->x.ints[3];
					goto L618;   /* %f.%t $f2,%y,$%r; mfc1 $%r,$f2 */
				}
				break;
			case 619:   /* %f $%r,%y,%x */
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[1]) ++c->count;
					}
					a->syms[1] = b->syms[1];
					a->kids[1] = b->kids[1];
					a->x.ints[1] = b->x.ints[1];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					a->x.ints[2] = b->x.ints[2];
					a->x.ints[3] = b->x.ints[3];
					goto L619;   /* %f $%r,%y,%x */
				}
				break;
			case 620:   /* l%tu $%r,%y */
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[1]) ++c->count;
					}
					a->syms[1] = b->syms[1];
					a->kids[1] = b->kids[1];
					a->x.ints[1] = b->x.ints[1];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					a->x.ints[2] = b->x.ints[2];
					a->x.ints[3] = b->x.ints[3];
					goto L620;   /* l%tu $%r,%y */
				}
				break;
			case 621:   /* # nop */
				if (
				a->syms[RX] == rmap[I]
				) {
					--b->count;
					a->syms[2] = b->syms[2];
					a->kids[0] = 0;
					goto L621;   /* # nop */
				}
				break;
			}
			break;
		case 2:   /* $%c */
			if (
			rmcopy(a)
			) {
				a->syms[2] = a->syms[0];
				goto L621;   /* # nop */
			}
			break;
		}
		a->op = 614;
		break;
	case 615: L615:   /* %f $%r,%y */
		dumptree(a, 615);
		a->op = 615;
		break;
	case 616: L616:   /* li $%r,%y */
		dumptree(a, 616);
		if (
		a->x.ints[0] == 5  /* %c */
		&& a->syms[0]->x.name == sym[1].x.name  /* 0 */
		) {
			setreg(a,ireg[0]);
			goto L621;   /* # nop */
		}
		a->op = 616;
		break;
	case 617: L617:   /* l%t $%r,%y */
		dumptree(a, 617);
		switch (a->x.ints[0]) {
		case 7:   /* %c($sp) */
			if (
			isregvar(a->syms[0])
			) {
				a->x.ints[0] = 2;  /* $%c */
				goto L614;   /* move $%r,%y */
			}
			break;
		case 8:   /* ($%n) */
			b = a->kids[0];
			if (
			b->op == 612  /* la $%r,%y */
			) {
				if (--b->count > 0)
					if (c=b->kids[0]) ++c->count;
				a->syms[0] = b->syms[0];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L617;   /* l%t $%r,%y */
			}
			break;
		}
		b = a->kids[0];
		if (
		a->x.ints[3] == 2  /* d */
		&& a->x.ints[0] == 3  /* $f%n */
		&& b->op == 632  /* l.%t $f%r,%y */
		&& b->count == 1
		) {
			if (--b->count > 0)
				if (c=b->kids[0]) ++c->count;
			a->syms[0] = b->syms[0];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			a->x.ints[3] = b->x.ints[3];
			goto L617;   /* l%t $%r,%y */
		}
		a->op = 617;
		break;
	case 618: L618:   /* %f.%t $f2,%y,$%r; mfc1 $%r,$f2 */
		dumptree(a, 618);
		b = a->kids[0];
		if (
		a->x.ints[2] == 12  /* trunc.w */
		&& a->x.ints[3] == 2  /* d */
		&& a->x.ints[0] == 3  /* $f%n */
		&& b->op == 623  /* %f.%t $f%r,%y */
		&& b->x.ints[2] == 2  /* cvt.d */
		&& b->x.ints[3] == 1  /* s */
		) {
			if (--b->count > 0)
				if (c=b->kids[0]) ++c->count;
			a->x.ints[3] = 1;  /* s */
			a->syms[0] = b->syms[0];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L618;   /* %f.%t $f2,%y,$%r; mfc1 $%r,$f2 */
		}
		a->op = 618;
		break;
	case 619: L619:   /* %f $%r,%y,%x */
		dumptree(a, 619);
		if (
		a->x.ints[2] == 23  /* addu */
		&& a->x.ints[1] == 1  /* $%n */
		) {
			b = a->kids[1];
			if (
			b->op == 612  /* la $%r,%y */
			&& b->x.ints[0] == 5  /* %c */
			) {
				--b->count;
				a->x.ints[0] = 6;  /* %c($%n) */
				a->syms[0] = b->syms[0];
				a->kids[1] = 0;
				goto L612;   /* la $%r,%y */
			}
			if (
			atst(a)
			&& b->op == 616  /* li $%r,%y */
			&& b->x.ints[0] == 5  /* %c */
			) {
				--b->count;
				a->x.ints[0] = 6;  /* %c($%n) */
				a->syms[0] = b->syms[0];
				a->kids[1] = 0;
				goto L612;   /* la $%r,%y */
			}
		}
		b = a->kids[1];
		if (
		a->x.ints[1] == 1  /* $%n */
		&& b->op == 616  /* li $%r,%y */
		&& b->x.ints[0] == 5  /* %c */
		) {
			if (--b->count > 0)
				if (c=b->kids[0]) ++c->count;
			a->syms[1] = b->syms[0];
			a->x.ints[1] = b->x.ints[0];
			a->kids[1] = b->kids[0];
			goto L619;   /* %f $%r,%y,%x */
		}
		if (
		a->x.ints[2] == 1  /* and */
		&& a->x.ints[0] == 1  /* $%n */
		&& a->x.ints[1] == 5  /* %c */
		) {
			b = a->kids[0];
			if (
			a->syms[1]->x.name == sym[12].x.name  /* 65535 */
			&& b->op == 617  /* l%t $%r,%y */
			&& b->x.ints[3] == 4  /* h */
			&& b->count == 1
			) {
				if (--b->count > 0)
					if (c=b->kids[0]) ++c->count;
				a->x.ints[3] = b->x.ints[3];
				a->syms[0] = b->syms[0];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L620;   /* l%tu $%r,%y */
			}
			if (
			a->syms[1]->x.name == sym[11].x.name  /* 255 */
			&& b->op == 617  /* l%t $%r,%y */
			&& b->x.ints[3] == 3  /* b */
			&& b->count == 1
			) {
				if (--b->count > 0)
					if (c=b->kids[0]) ++c->count;
				a->x.ints[3] = b->x.ints[3];
				a->syms[0] = b->syms[0];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L620;   /* l%tu $%r,%y */
			}
		}
		a->op = 619;
		break;
	case 620: L620:   /* l%tu $%r,%y */
		dumptree(a, 620);
		a->op = 620;
		break;
	case 621: L621:   /* # nop */
		dumptree(a, 621);
		a->op = 621;
		break;
	case 622: L622:   /* # ret */
		dumptree(a, 622);
		a->op = 622;
		break;
	case 623: L623:   /* %f.%t $f%r,%y */
		dumptree(a, 623);
		a->op = 623;
		break;
	case 624: L624:   /* %f.%t $f%r,%y,%x */
		dumptree(a, 624);
		a->op = 624;
		break;
	case 625: L625:   /* sll $%r,%y,%x; %f $%r,$%r,%x */
		dumptree(a, 625);
		b = a->kids[0];
		if (
		a->x.ints[0] == 1  /* $%n */
		&& a->x.ints[2] == 10  /* sra */
		&& a->x.ints[1] == 5  /* %c */
		&& b->op == 617  /* l%t $%r,%y */
		) {
			switch (b->x.ints[3]) {
			case 3:   /* b */
				if (
				b->count == 1
				&& a->syms[1]->x.name == sym[10].x.name  /* 24 */
				) {
					if (--b->count > 0)
						if (c=b->kids[0]) ++c->count;
					a->x.ints[3] = b->x.ints[3];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L617;   /* l%t $%r,%y */
				}
				break;
			case 4:   /* h */
				if (
				b->count == 1
				&& a->syms[1]->x.name == sym[9].x.name  /* 16 */
				) {
					if (--b->count > 0)
						if (c=b->kids[0]) ++c->count;
					a->x.ints[3] = b->x.ints[3];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					goto L617;   /* l%t $%r,%y */
				}
				break;
			}
		}
		a->op = 625;
		break;
	case 626: L626:   /* # label */
		dumptree(a, 626);
		a->op = 626;
		break;
	case 627: L627:   /* b%f %y,%x,%r */
		dumptree(a, 627);
		b = a->kids[1];
		if (
		a->x.ints[1] == 1  /* $%n */
		&& b->op == 616  /* li $%r,%y */
		&& b->x.ints[0] == 5  /* %c */
		) {
			if (--b->count > 0)
				if (c=b->kids[0]) ++c->count;
			a->syms[1] = b->syms[0];
			a->x.ints[1] = b->x.ints[0];
			a->kids[1] = b->kids[0];
			goto L627;   /* b%f %y,%x,%r */
		}
		a->op = 627;
		break;
	case 628: L628:   /* b %r */
		dumptree(a, 628);
		a->op = 628;
		break;
	case 629: L629:   /* j %y */
		dumptree(a, 629);
		b = a->kids[0];
		if (
		a->x.ints[0] == 1  /* $%n */
		&& a->kids[0] == a->kids[0]
		&& b->count == 1
		&& b->op == 612  /* la $%r,%y */
		&& b->x.ints[0] == 5  /* %c */
		) {
			--b->count;
			--b->count;
			a->syms[2] = b->syms[0];
			a->kids[0] = 0;
			a->kids[0] = 0;
			goto L628;   /* b %r */
		}
		a->op = 629;
		break;
	case 630: L630:   /* c.%f.%t %y,%x; bc1f %r */
		dumptree(a, 630);
		a->op = 630;
		break;
	case 631: L631:   /* c.%f.%t %y,%x; bc1t %r */
		dumptree(a, 631);
		a->op = 631;
		break;
	case 632: L632:   /* l.%t $f%r,%y */
		dumptree(a, 632);
		switch (a->x.ints[0]) {
		case 7:   /* %c($sp) */
			if (
			isregvar(a->syms[0])
			) {
				a->x.ints[0] = 4;  /* $f%c */
				goto L635;   /* mov.%t $f%r,%y */
			}
			break;
		case 8:   /* ($%n) */
			b = a->kids[0];
			if (
			b->op == 612  /* la $%r,%y */
			) {
				if (--b->count > 0)
					if (c=b->kids[0]) ++c->count;
				a->syms[0] = b->syms[0];
				a->x.ints[0] = b->x.ints[0];
				a->kids[0] = b->kids[0];
				goto L632;   /* l.%t $f%r,%y */
			}
			break;
		}
		a->op = 632;
		break;
	case 633: L633:   /* li.%t $f%r,%y */
		dumptree(a, 633);
		a->op = 633;
		break;
	case 634: L634:   /* mfc1.d $%r,%y */
		dumptree(a, 634);
		a->op = 634;
		break;
	case 635: L635:   /* mov.%t $f%r,%y */
		dumptree(a, 635);
		switch (a->x.ints[0]) {
		case 3:   /* $f%n */
			b = a->kids[0];
			switch (b->op) {
			case 621:   /* # nop */
				if (
				a->syms[RX] == rmap[D]
				) {
					--b->count;
					a->syms[2] = b->syms[2];
					a->kids[0] = 0;
					goto L621;   /* # nop */
				}
				break;
			case 623:   /* %f.%t $f%r,%y */
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[1]) ++c->count;
					}
					a->syms[1] = b->syms[1];
					a->kids[1] = b->kids[1];
					a->x.ints[1] = b->x.ints[1];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					a->x.ints[2] = b->x.ints[2];
					a->x.ints[3] = b->x.ints[3];
					goto L623;   /* %f.%t $f%r,%y */
				}
				break;
			case 624:   /* %f.%t $f%r,%y,%x */
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[1]) ++c->count;
					}
					a->syms[1] = b->syms[1];
					a->kids[1] = b->kids[1];
					a->x.ints[1] = b->x.ints[1];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					a->x.ints[2] = b->x.ints[2];
					a->x.ints[3] = b->x.ints[3];
					goto L624;   /* %f.%t $f%r,%y,%x */
				}
				break;
			case 632:   /* l.%t $f%r,%y */
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[1]) ++c->count;
					}
					a->syms[1] = b->syms[1];
					a->kids[1] = b->kids[1];
					a->x.ints[1] = b->x.ints[1];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					a->x.ints[2] = b->x.ints[2];
					a->x.ints[3] = b->x.ints[3];
					goto L632;   /* l.%t $f%r,%y */
				}
				break;
			case 633:   /* li.%t $f%r,%y */
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[1]) ++c->count;
					}
					a->syms[1] = b->syms[1];
					a->kids[1] = b->kids[1];
					a->x.ints[1] = b->x.ints[1];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					a->x.ints[2] = b->x.ints[2];
					a->x.ints[3] = b->x.ints[3];
					goto L633;   /* li.%t $f%r,%y */
				}
				break;
			case 635:   /* mov.%t $f%r,%y */
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[1]) ++c->count;
					}
					a->syms[1] = b->syms[1];
					a->kids[1] = b->kids[1];
					a->x.ints[1] = b->x.ints[1];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					a->x.ints[2] = b->x.ints[2];
					a->x.ints[3] = b->x.ints[3];
					goto L635;   /* mov.%t $f%r,%y */
				}
				break;
			case 636:   /* mtc1 %y,$f%r; %f.%t $f%r,$f%r */
				if (
				b->count == 1
				) {
					if (--b->count > 0) {
						if (c=b->kids[0]) ++c->count;
						if (c=b->kids[1]) ++c->count;
					}
					a->syms[1] = b->syms[1];
					a->kids[1] = b->kids[1];
					a->x.ints[1] = b->x.ints[1];
					a->syms[0] = b->syms[0];
					a->x.ints[0] = b->x.ints[0];
					a->kids[0] = b->kids[0];
					a->x.ints[2] = b->x.ints[2];
					a->x.ints[3] = b->x.ints[3];
					goto L636;   /* mtc1 %y,$f%r; %f.%t $f%r,$f%r */
				}
				break;
			}
			break;
		case 4:   /* $f%c */
			if (
			rmcopy(a)
			) {
				a->syms[2] = a->syms[0];
				goto L621;   /* # nop */
			}
			break;
		}
		a->op = 635;
		break;
	case 636: L636:   /* mtc1 %y,$f%r; %f.%t $f%r,$f%r */
		dumptree(a, 636);
		a->op = 636;
		break;
	case 637: L637:   /* s%t $%k,%y */
		dumptree(a, 637);
		if (
		a->x.ints[0] == 7  /* %c($sp) */
		&& isregvar(a->syms[0])
		) {
			a->x.ints[0] = 1;  /* $%n */
			a->kids[0] = a->kids[2];
			a->kids[2] = 0;
			a->syms[2] = a->syms[0];
			goto L614;   /* move $%r,%y */
		}
		if (
		a->x.ints[3] == 5  /* w */
		&& a->x.ints[0] == 7  /* %c($sp) */
		) {
			if (
			a->syms[0]->x.name == sym[1].x.name  /* 0 */
			) {
				a->x.ints[0] = 1;  /* $%n */
				a->syms[2] = &sym[2];  /* 4 */
				a->kids[0] = a->kids[2];
				a->kids[2] = 0;
				goto L614;   /* move $%r,%y */
			}
			if (
			a->syms[0]->x.name == sym[2].x.name  /* 4 */
			) {
				a->x.ints[0] = 1;  /* $%n */
				a->syms[2] = &sym[3];  /* 5 */
				a->kids[0] = a->kids[2];
				a->kids[2] = 0;
				goto L614;   /* move $%r,%y */
			}
			if (
			a->syms[0]->x.name == sym[6].x.name  /* 8 */
			) {
				a->x.ints[0] = 1;  /* $%n */
				a->syms[2] = &sym[4];  /* 6 */
				a->kids[0] = a->kids[2];
				a->kids[2] = 0;
				goto L614;   /* move $%r,%y */
			}
			if (
			a->syms[0]->x.name == sym[7].x.name  /* 12 */
			) {
				a->x.ints[0] = 1;  /* $%n */
				a->syms[2] = &sym[5];  /* 7 */
				a->kids[0] = a->kids[2];
				a->kids[2] = 0;
				goto L614;   /* move $%r,%y */
			}
		}
		b = a->kids[0];
		if (
		a->x.ints[0] == 8  /* ($%n) */
		&& b->op == 612  /* la $%r,%y */
		) {
			if (--b->count > 0)
				if (c=b->kids[0]) ++c->count;
			a->syms[0] = b->syms[0];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L637;   /* s%t $%k,%y */
		}
		a->op = 637;
		break;
	case 638: L638:   /* s.%t $f%k,%y */
		dumptree(a, 638);
		if (
		a->x.ints[0] == 7  /* %c($sp) */
		&& isregvar(a->syms[0])
		) {
			a->x.ints[0] = 3;  /* $f%n */
			a->kids[0] = a->kids[2];
			a->kids[2] = 0;
			a->syms[2] = a->syms[0];
			goto L635;   /* mov.%t $f%r,%y */
		}
		switch (a->x.ints[3]) {
		case 1:   /* s */
			if (
			a->x.ints[0] == 7  /* %c($sp) */
			&& a->syms[0]->x.name == sym[1].x.name  /* 0 */
			&& a->x.isvararg
			) {
				a->x.ints[0] = 3;  /* $f%n */
				a->syms[2] = &sym[2];  /* 4 */
				a->kids[0] = a->kids[2];
				a->kids[2] = 0;
				goto L613;   /* mfc1 $%r,%y */
			}
			break;
		case 2:   /* d */
			if (
			a->x.ints[0] == 7  /* %c($sp) */
			&& a->syms[0]->x.name == sym[1].x.name  /* 0 */
			&& a->x.isvararg
			) {
				a->x.ints[0] = 3;  /* $f%n */
				a->syms[2] = &sym[2];  /* 4 */
				a->kids[0] = a->kids[2];
				a->kids[2] = 0;
				goto L634;   /* mfc1.d $%r,%y */
			}
			break;
		}
		if (
		a->x.ints[0] == 7  /* %c($sp) */
		) {
			if (
			a->syms[0]->x.name == sym[1].x.name  /* 0 */
			) {
				a->x.ints[0] = 3;  /* $f%n */
				a->syms[2] = &sym[7];  /* 12 */
				a->kids[0] = a->kids[2];
				a->kids[2] = 0;
				goto L635;   /* mov.%t $f%r,%y */
			}
			if (
			a->syms[0]->x.name == sym[2].x.name  /* 4 */
			) {
				if (last)
					/* %f.%t $f%r,%y */
					/* %f.%t $f%r,%y,%x */
					/* l.%t $f%r,%y */
					/* li.%t $f%r,%y */
					/* mov.%t $f%r,%y */
					/* mtc1 %y,$f%r; %f.%t $f%r,$f%r */
					if ("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\0\0\0\0\0\0\0\1\1\0\1\1\0\0\0"[last->op-608]) {
						if (
						last && last->syms[2]->x.name == sym[7].x.name  /* 12 */
						) {
							a->x.ints[0] = 3;  /* $f%n */
							a->syms[2] = &sym[8];  /* 14 */
							a->kids[0] = a->kids[2];
							a->kids[2] = 0;
							rewrite(last);
							goto L635;   /* mov.%t $f%r,%y */
						}
					}
			}
			if (
			a->syms[0]->x.name == sym[6].x.name  /* 8 */
			) {
				if (last)
					/* %f.%t $f%r,%y */
					/* %f.%t $f%r,%y,%x */
					/* l.%t $f%r,%y */
					/* li.%t $f%r,%y */
					/* mov.%t $f%r,%y */
					/* mtc1 %y,$f%r; %f.%t $f%r,$f%r */
					if ("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\1\1\0\0\0\0\0\0\0\1\1\0\1\1\0\0\0"[last->op-608]) {
						if (
						last && last->syms[2]->x.name == sym[7].x.name  /* 12 */
						) {
							a->x.ints[0] = 3;  /* $f%n */
							a->syms[2] = &sym[8];  /* 14 */
							a->kids[0] = a->kids[2];
							a->kids[2] = 0;
							rewrite(last);
							goto L635;   /* mov.%t $f%r,%y */
						}
					}
			}
		}
		switch (a->x.ints[3]) {
		case 1:   /* s */
			if (
			a->x.ints[0] == 7  /* %c($sp) */
			) {
				if (
				a->syms[0]->x.name == sym[2].x.name  /* 4 */
				) {
					a->x.ints[0] = 3;  /* $f%n */
					a->syms[2] = &sym[3];  /* 5 */
					a->kids[0] = a->kids[2];
					a->kids[2] = 0;
					goto L613;   /* mfc1 $%r,%y */
				}
				if (
				a->syms[0]->x.name == sym[6].x.name  /* 8 */
				) {
					a->x.ints[0] = 3;  /* $f%n */
					a->syms[2] = &sym[4];  /* 6 */
					a->kids[0] = a->kids[2];
					a->kids[2] = 0;
					goto L613;   /* mfc1 $%r,%y */
				}
				if (
				a->syms[0]->x.name == sym[7].x.name  /* 12 */
				) {
					a->x.ints[0] = 3;  /* $f%n */
					a->syms[2] = &sym[5];  /* 7 */
					a->kids[0] = a->kids[2];
					a->kids[2] = 0;
					goto L613;   /* mfc1 $%r,%y */
				}
			}
			break;
		case 2:   /* d */
			if (
			a->x.ints[0] == 7  /* %c($sp) */
			&& a->syms[0]->x.name == sym[6].x.name  /* 8 */
			) {
				a->x.ints[0] = 3;  /* $f%n */
				a->syms[2] = &sym[4];  /* 6 */
				a->kids[0] = a->kids[2];
				a->kids[2] = 0;
				goto L634;   /* mfc1.d $%r,%y */
			}
			break;
		}
		b = a->kids[0];
		if (
		a->x.ints[0] == 8  /* ($%n) */
		&& b->op == 612  /* la $%r,%y */
		) {
			if (--b->count > 0)
				if (c=b->kids[0]) ++c->count;
			a->syms[0] = b->syms[0];
			a->x.ints[0] = b->x.ints[0];
			a->kids[0] = b->kids[0];
			goto L638;   /* s.%t $f%k,%y */
		}
		a->op = 638;
		break;
	case 639: L639:   /* %y: */
		dumptree(a, 639);
		a->op = 639;
		break;
	default:
	assert(0);
	}
}


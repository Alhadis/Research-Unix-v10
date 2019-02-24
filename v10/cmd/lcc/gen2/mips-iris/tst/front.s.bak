.set reorder
.text
.globl main
.text
.align 2; .ent main
main:
$fp1=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0x80000000,-8
sw $31,16($sp)
move $4,$0
jal exit  # int
L.1:
lw $31,16($sp)
addu $sp,$sp,24
j $31
.end main
.globl nested
.text
.align 2; .ent nested
nested:
$fp2=0
.frame $sp,0,$31
bge $4,4,L.6
beq $5,114,L.9
L.6:
bne $4,1,L.8
beq $5,104,L.9
beq $5,105,L.9
L.8:
bne $4,2,L.3
beq $5,111,L.9
bne $5,121,L.3
L.9:
move $4,$5
L.3:
L.2:
j $31
.end nested
.globl s
.text
.align 2; .ent s
s:
$fp3=0
.frame $sp,0,$31
L.10:
j $31
.end s
.globl Dy
.sdata; .align 2; Dy:.word 0x0
.space 4
.globl Dz
.sdata; .align 2; Dz:.word 0x1
.space 4
.text
.globl Dfunc
.text
.align 2; .ent Dfunc
Dfunc:
$fp4=8
.frame $sp,8,$31
addu $sp,$sp,-8
.mask 0x80000000,-8
sw $31,0($sp)
lw $31,0($sp)
addu $sp,$sp,8
j $31
.end Dfunc
.globl f
.text
.align 2; .ent f
f:
$fp5=0
.frame $sp,0,$31
j $31
.end f
.globl f1
.text
.align 2; .ent f1
f1:
$fp6=0
.frame $sp,0,$31
j $31
.end f1
.globl f2
.text
.align 2; .ent f2
f2:
$fp7=8
.frame $sp,8,$31
addu $sp,$sp,-8
.mask 0x80000000,-8
sw $31,0($sp)
sw $4,8($sp)
sw $5,12($sp)
lw $31,0($sp)
addu $sp,$sp,8
j $31
.end f2
.globl g
.text
.align 2; .ent g
g:
$fp8=8
.frame $sp,8,$31
addu $sp,$sp,-8
.mask 0x80000000,-8
sw $31,0($sp)
sw $4,8($sp)
lw $31,0($sp)
addu $sp,$sp,8
j $31
.end g
.globl h
.text
.align 2; .ent h
h:
$fp9=8
.frame $sp,8,$31
addu $sp,$sp,-8
.mask 0x80000000,-8
sw $31,0($sp)
sw $4,8($sp)
lw $31,0($sp)
addu $sp,$sp,8
j $31
.end h
.globl h1
.text
.align 2; .ent h1
h1:
$fp10=8
.frame $sp,8,$31
addu $sp,$sp,-8
.mask 0x80000000,-8
sw $31,0($sp)
sw $4,8($sp)
sw $5,12($sp)
lw $31,0($sp)
addu $sp,$sp,8
j $31
.end h1
.globl h2
.text
.align 2; .ent h2
h2:
$fp11=0
.frame $sp,0,$31
j $31
.end h2
.sdata; .align 2; L.21:.word 0x1
.text
.globl set1
.text
.align 2; .ent set1
set1:
$fp12=0
.frame $sp,0,$31
j $31
.end set1
.sdata; .align 2; L.23:.word 0x2
.text
.globl set2
.text
.align 2; .ent set2
set2:
$fp13=0
.frame $sp,0,$31
j $31
.end set2
.text
.align 2; .ent goo
goo:
$fp14=0
.frame $sp,0,$31
j $31
.end goo
.globl sss
.text
.align 2; .ent sss
sss:
$fp15=0
.frame $sp,0,$31
j $31
.end sss
.lcomm L.27,4
.text
.globl rrr
.text
.align 2; .ent rrr
rrr:
$fp16=0
.frame $sp,0,$31
j $31
.end rrr
.globl setstatic
.text
.align 2; .ent setstatic
setstatic:
$fp17=0
.frame $sp,0,$31
j $31
.end setstatic
.globl gx1
.text
.align 2; .ent gx1
gx1:
$fp18=8
.frame $sp,8,$31
addu $sp,$sp,-8
.mask 0x80000000,-8
sw $31,0($sp)
s.d $f12,8($sp)
lw $31,0($sp)
addu $sp,$sp,8
j $31
.end gx1
.globl ff1
.text
.align 2; .ent ff1
ff1:
$fp19=0
.frame $sp,0,$31
j $31
.end ff1
.globl gg1
.text
.align 2; .ent gg1
gg1:
$fp20=0
.frame $sp,0,$31
j $31
.end gg1
.globl hh1
.text
.align 2; .ent hh1
hh1:
$fp21=0
.frame $sp,0,$31
j $31
.end hh1
.globl cmp
.text
.align 2; .ent cmp
cmp:
$fp22=8
.frame $sp,8,$31
addu $sp,$sp,-8
.mask 0x80000000,-8
sw $31,0($sp)
sw $4,8($sp)
sw $5,12($sp)
lw $31,0($sp)
addu $sp,$sp,8
j $31
.end cmp
.globl sort
.text
.align 2; .ent sort
sort:
$fp23=8
.frame $sp,8,$31
addu $sp,$sp,-8
.mask 0x80000000,-8
sw $31,0($sp)
lw $31,0($sp)
addu $sp,$sp,8
j $31
.end sort
.globl onearg
.text
.align 2; .ent onearg
onearg:
$fp24=8
.frame $sp,8,$31
addu $sp,$sp,-8
.mask 0x80000000,-8
sw $31,0($sp)
lw $31,0($sp)
addu $sp,$sp,8
j $31
.end onearg
.globl ss4
.comm ss4,4
.lcomm ss2,4
.lcomm ss5,4
.globl ss3
.comm ss3,4
.lcomm ss1,4
.lcomm yy,4
.globl z
.comm z,4
.globl y
.comm y,4
.globl x
.comm x,4
.globl b
.comm b,4
.globl a
.comm a,4
.extern xr 4

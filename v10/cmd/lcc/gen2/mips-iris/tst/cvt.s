.set reorder
.text
.globl main
.text
.align 2; .ent main
main:
$fp1=80
.frame $sp,80,$31
addu $sp,$sp,-80
.fmask 0xf0000000,-24
s.d $f28,48($sp)
s.d $f30,56($sp)
.mask 0xc0c00000,-4
sw $22,64($sp)
sw $23,68($sp)
sw $30,72($sp)
sw $31,76($sp)
li $25,1
sb $25,c
lb $25,c
move $24,$25
sh $24,s
sw $25,i
move $24,$25
move $15,$24
sb $15,C
move $15,$24
sh $15,S
sw $24,I
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f16,$f18
s.s $f16,f
s.d $f18,d
la $4,L.2
move $5,$25
lh $6,s
lw $7,i
lbu $25,C
sw $25,16($sp)
lhu $25,S
sw $25,20($sp)
lw $25,I
sw $25,24($sp)
l.s $f18,f
cvt.d.s $f18,$f18
s.d $f18,32($sp)
l.d $f18,d
s.d $f18,40($sp)
jal printf  # int
li $25,2
sh $25,s
lh $25,s
move $24,$25
sb $24,c
sw $25,i
move $24,$25
move $15,$24
sb $15,C
move $15,$24
sh $15,S
sw $24,I
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f16,$f18
s.s $f16,f
s.d $f18,d
la $4,L.2
lb $5,c
move $6,$25
lw $7,i
lbu $25,C
sw $25,16($sp)
lhu $25,S
sw $25,20($sp)
lw $25,I
sw $25,24($sp)
l.s $f18,f
cvt.d.s $f18,$f18
s.d $f18,32($sp)
l.d $f18,d
s.d $f18,40($sp)
jal printf  # int
li $25,3
sw $25,i
lw $25,i
move $24,$25
sb $24,c
move $24,$25
sh $24,s
move $24,$25
move $15,$24
sb $15,C
move $15,$24
sh $15,S
sw $24,I
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f16,$f18
s.s $f16,f
s.d $f18,d
la $4,L.2
lb $5,c
lh $6,s
move $7,$25
lbu $25,C
sw $25,16($sp)
lhu $25,S
sw $25,20($sp)
lw $25,I
sw $25,24($sp)
l.s $f18,f
cvt.d.s $f18,$f18
s.d $f18,32($sp)
l.d $f18,d
s.d $f18,40($sp)
jal printf  # int
li $25,4
sb $25,C
lbu $25,C
move $24,$25
move $15,$24
sb $15,c
move $15,$24
sh $15,s
sw $24,i
move $24,$25
sh $24,S
sw $25,I
lbu $25,C
blt $25,$0,L.4
mtc1 $25,$f30; cvt.d.w $f30,$f30
b L.5
L.4:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.6
add.d $f30,$f18,$f16
L.5:
cvt.s.d $f18,$f30
s.s $f18,f
lbu $25,C
blt $25,$0,L.8
mtc1 $25,$f28; cvt.d.w $f28,$f28
b L.9
L.8:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.6
add.d $f28,$f18,$f16
L.9:
s.d $f28,d
la $4,L.2
lb $5,c
lh $6,s
lw $7,i
lbu $25,C
sw $25,16($sp)
lhu $25,S
sw $25,20($sp)
lw $25,I
sw $25,24($sp)
l.s $f18,f
cvt.d.s $f18,$f18
s.d $f18,32($sp)
l.d $f18,d
s.d $f18,40($sp)
jal printf  # int
li $25,5
sh $25,S
lhu $25,S
move $24,$25
move $15,$24
sb $15,c
move $15,$24
sh $15,s
sw $24,i
move $24,$25
sb $24,C
sw $25,I
lhu $25,S
blt $25,$0,L.11
mtc1 $25,$f30; cvt.d.w $f30,$f30
b L.12
L.11:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.6
add.d $f30,$f18,$f16
L.12:
cvt.s.d $f18,$f30
s.s $f18,f
lhu $25,S
blt $25,$0,L.14
mtc1 $25,$f28; cvt.d.w $f28,$f28
b L.15
L.14:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.6
add.d $f28,$f18,$f16
L.15:
s.d $f28,d
la $4,L.2
lb $5,c
lh $6,s
lw $7,i
lbu $25,C
sw $25,16($sp)
lhu $25,S
sw $25,20($sp)
lw $25,I
sw $25,24($sp)
l.s $f18,f
cvt.d.s $f18,$f18
s.d $f18,32($sp)
l.d $f18,d
s.d $f18,40($sp)
jal printf  # int
li $25,6
sw $25,I
lw $25,I
move $24,$25
move $15,$24
sb $15,c
move $15,$24
sh $15,s
sw $24,i
move $24,$25
sb $24,C
sh $25,S
lw $25,I
blt $25,$0,L.17
mtc1 $25,$f30; cvt.d.w $f30,$f30
b L.18
L.17:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.6
add.d $f30,$f18,$f16
L.18:
cvt.s.d $f18,$f30
s.s $f18,f
lw $25,I
blt $25,$0,L.20
mtc1 $25,$f28; cvt.d.w $f28,$f28
b L.21
L.20:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.6
add.d $f28,$f18,$f16
L.21:
s.d $f28,d
la $4,L.2
lb $5,c
lh $6,s
lw $7,i
lbu $25,C
sw $25,16($sp)
lhu $25,S
sw $25,20($sp)
lw $25,I
sw $25,24($sp)
l.s $f18,f
cvt.d.s $f18,$f18
s.d $f18,32($sp)
l.d $f18,d
s.d $f18,40($sp)
jal printf  # int
l.s $f18,L.22
s.s $f18,f
l.s $f18,f
trunc.w.s $f2,$f18,$25; mfc1 $25,$f2
move $24,$25
sb $24,c
move $24,$25
sh $24,s
sw $25,i
l.s $f18,f
cvt.d.s $f16,$f18
l.d $f10,L.26
c.lt.d $f16,$f10; bc1t L.24
sub.d $f16,$f16,$f10
trunc.w.d $f2,$f16,$25; mfc1 $25,$f2
addu $30,$25,0x80000000
b L.25
L.24:
trunc.w.s $f2,$f18,$30; mfc1 $30,$f2
L.25:
sb $30,C
l.s $f18,f
cvt.d.s $f16,$f18
l.d $f10,L.26
c.lt.d $f16,$f10; bc1t L.28
sub.d $f16,$f16,$f10
trunc.w.d $f2,$f16,$25; mfc1 $25,$f2
addu $23,$25,0x80000000
b L.29
L.28:
trunc.w.s $f2,$f18,$23; mfc1 $23,$f2
L.29:
sh $23,S
l.s $f18,f
cvt.d.s $f16,$f18
l.d $f10,L.26
c.lt.d $f16,$f10; bc1t L.31
sub.d $f16,$f16,$f10
trunc.w.d $f2,$f16,$25; mfc1 $25,$f2
addu $22,$25,0x80000000
b L.32
L.31:
trunc.w.s $f2,$f18,$22; mfc1 $22,$f2
L.32:
sw $22,I
l.s $f18,f
cvt.d.s $f18,$f18
s.d $f18,d
la $4,L.2
lb $5,c
lh $6,s
lw $7,i
lbu $25,C
sw $25,16($sp)
lhu $25,S
sw $25,20($sp)
lw $25,I
sw $25,24($sp)
s.d $f18,32($sp)
l.d $f18,d
s.d $f18,40($sp)
jal printf  # int
l.d $f18,L.33
s.d $f18,d
l.d $f18,d
trunc.w.d $f2,$f18,$25; mfc1 $25,$f2
move $24,$25
sb $24,c
move $24,$25
sh $24,s
sw $25,i
l.d $f18,d
l.d $f16,L.26
c.lt.d $f18,$f16; bc1t L.35
sub.d $f16,$f18,$f16
trunc.w.d $f2,$f16,$25; mfc1 $25,$f2
addu $30,$25,0x80000000
b L.36
L.35:
trunc.w.d $f2,$f18,$30; mfc1 $30,$f2
L.36:
sb $30,C
l.d $f18,d
l.d $f16,L.26
c.lt.d $f18,$f16; bc1t L.38
sub.d $f16,$f18,$f16
trunc.w.d $f2,$f16,$25; mfc1 $25,$f2
addu $23,$25,0x80000000
b L.39
L.38:
trunc.w.d $f2,$f18,$23; mfc1 $23,$f2
L.39:
sh $23,S
l.d $f18,d
l.d $f16,L.26
c.lt.d $f18,$f16; bc1t L.41
sub.d $f16,$f18,$f16
trunc.w.d $f2,$f16,$25; mfc1 $25,$f2
addu $22,$25,0x80000000
b L.42
L.41:
trunc.w.d $f2,$f18,$22; mfc1 $22,$f2
L.42:
sw $22,I
l.d $f18,d
cvt.s.d $f16,$f18
s.s $f16,f
la $4,L.2
lb $5,c
lh $6,s
lw $7,i
lbu $25,C
sw $25,16($sp)
lhu $25,S
sw $25,20($sp)
lw $25,I
sw $25,24($sp)
l.s $f16,f
cvt.d.s $f16,$f16
s.d $f16,32($sp)
s.d $f18,40($sp)
jal printf  # int
L.1:
l.d $f28,48($sp)
l.d $f30,56($sp)
lw $22,64($sp)
lw $23,68($sp)
lw $30,72($sp)
lw $31,76($sp)
addu $sp,$sp,80
j $31
.end main
.globl d2
.comm d2,8
.globl d1
.comm d1,8
.globl d
.comm d,8
.globl f2
.comm f2,4
.globl f1
.comm f1,4
.globl f
.comm f,4
.globl I
.comm I,4
.globl S
.comm S,2
.globl C
.comm C,1
.globl i2
.comm i2,4
.globl i1
.comm i1,4
.globl i
.comm i,4
.globl s
.comm s,2
.globl c2
.comm c2,1
.globl c1
.comm c1,1
.globl c
.comm c,1
.rdata
.align 3; L.33:.word 0x40200000
.word 0x0
.align 3; L.26:.word 0x41e00000
.word 0x0
.align 2; L.22:.word 0x40e00000
.align 3; L.6:.word 0x41f00000
.word 0x0
L.2:.ascii "%d %d %d %d %d %d %f %f\012\000"

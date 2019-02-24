.set reorder
.text
.globl main
.text
.align 2; .ent main
main:
$fp1=0
.frame $sp,0,$31
L.1:
j $31
.end main
.globl f
.text
.align 2; .ent f
f:
$fp2=8
.frame $sp,8,$31
addu $sp,$sp,-8
.mask 0xc0000000,-4
sw $30,0($sp)
sw $31,4($sp)
sw $4,8($sp)
jal f  # int
move $30,$2
jal f  # int
addu $25,$30,$2
sw $25,$fp2+0($sp)
L.2:
lw $30,0($sp)
lw $31,4($sp)
addu $sp,$sp,8
j $31
.end f
.globl f2
.text
.align 2; .ent f2
f2:
$fp3=16
.frame $sp,16,$31
addu $sp,$sp,-16
.mask 0xc0800000,-8
sw $23,0($sp)
sw $30,4($sp)
sw $31,8($sp)
sw $4,16($sp)
jal f  # int
move $23,$2
lw $25,$fp3+0($sp)
beq $25,$0,L.6
jal f  # int
move $30,$2
b L.7
L.6:
li $30,1
L.7:
addu $25,$23,$30
sw $25,$fp3+0($sp)
L.4:
lw $23,0($sp)
lw $30,4($sp)
lw $31,8($sp)
addu $sp,$sp,16
j $31
.end f2
.globl f3
.text
.align 2; .ent f3
f3:
$fp4=56
.frame $sp,56,$31
addu $sp,$sp,-56
.mask 0xc0ff0000,-20
sw $16,0($sp)
sw $17,4($sp)
sw $18,8($sp)
sw $19,12($sp)
sw $20,16($sp)
sw $21,20($sp)
sw $22,24($sp)
sw $23,28($sp)
sw $30,32($sp)
sw $31,36($sp)
sw $4,56($sp)
sw $5,60($sp)
move $30,$0
move $23,$0
move $22,$0
move $21,$0
move $20,$0
move $19,$0
move $18,$0
move $17,$0
move $16,$0
sw $0,$fp4-4($sp)
lw $25,$fp4+4($sp)
sw $25,$fp4-12($sp)
addu $24,$25,4
sw $24,$fp4+4($sp)
lw $24,$fp4+0($sp)
sw $24,$fp4-8($sp)
lw $24,$fp4-8($sp)
beq $24,$0,L.11
jal f  # int
sw $2,$fp4-8($sp)
L.11:
lw $25,$fp4-8($sp)
lw $24,$fp4-12($sp)
sw $25,($24)
L.9:
lw $16,0($sp)
lw $17,4($sp)
lw $18,8($sp)
lw $19,12($sp)
lw $20,16($sp)
lw $21,20($sp)
lw $22,24($sp)
lw $23,28($sp)
lw $30,32($sp)
lw $31,36($sp)
addu $sp,$sp,56
j $31
.end f3
.globl f4
.text
.align 2; .ent f4
f4:
$fp5=0
.frame $sp,0,$31
move $9,$0
move $10,$0
move $11,$0
move $12,$0
move $13,$0
move $14,$0
lw $25,i
sll $24,$25,3
l.d $f18,a($24)
l.d $f16,b($24)
add.d $f10,$f18,$f16
l.d $f8,L.18
c.eq.d $f10,$f8; bc1t L.16
beq $25,$0,L.16
sub.d $f18,$f18,$f16
c.eq.d $f18,$f8; bc1t L.16
li $15,1
b L.17
L.16:
move $15,$0
L.17:
sw $15,i
L.14:
j $31
.end f4
.globl f5
.text
.align 2; .ent f5
f5:
$fp6=0
.frame $sp,0,$31
lw $25,k
lw $24,m
mul $15,$25,$24
sll $15,$15,3
lw $14,A
addu $15,$15,$14
l.d $f18,($15)
lw $13,j
mul $24,$13,$24
sll $24,$24,3
addu $24,$24,$14
l.d $f16,($24)
mul.d $f18,$f18,$f16
lw $14,n
mul $25,$25,$14
sll $25,$25,3
lw $12,B
addu $25,$25,$12
l.d $f16,($25)
mul $14,$13,$14
sll $14,$14,3
addu $14,$14,$12
l.d $f10,($14)
mul.d $f16,$f16,$f10
add.d $f18,$f18,$f16
s.d $f18,x
l.d $f18,($15)
l.d $f16,($14)
mul.d $f18,$f18,$f16
l.d $f16,($25)
l.d $f10,($24)
mul.d $f16,$f16,$f10
sub.d $f18,$f18,$f16
s.d $f18,x
L.19:
j $31
.end f5
.globl x
.comm x,8
.globl B
.comm B,4
.globl A
.comm A,4
.globl n
.comm n,4
.globl m
.comm m,4
.globl k
.comm k,4
.globl j
.comm j,4
.globl i
.comm i,4
.globl b
.comm b,80
.globl a
.comm a,80
.rdata
.align 3; L.18:.word 0x0
.word 0x0

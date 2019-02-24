.set reorder
.globl in
.data; .align 2; in:.word 0xa
.word 0x20
.word 0xffffffff
.word 0x237
.word 0x3
.word 0x12
.word 0x1
.word 0xffffffcd
.word 0x315
.word 0x0
.text
.globl main
.text
.align 2; .ent main
main:
$fp1=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0xc0000000,-4
sw $30,16($sp)
sw $31,20($sp)
la $4,in
li $5,10
jal sort  # int
move $30,$0
b L.5
L.2:
sll $25,$30,2
lw $4,in($25)
jal putd  # int
li $4,10
jal putchar  # int
L.3:
addu $30,$30,1
L.5:
bltu $30,10,L.2
move $2,$0
L.1:
lw $30,16($sp)
lw $31,20($sp)
addu $sp,$sp,24
j $31
.end main
.globl putd
.text
.align 2; .ent putd
putd:
$fp2=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0xc0000000,-4
sw $30,16($sp)
sw $31,20($sp)
move $30,$4
bge $30,$0,L.7
li $4,45
jal putchar  # int
negu $30,$30
L.7:
div $25,$30,10
beq $25,$0,L.9
div $4,$30,10
jal putd  # int
L.9:
rem $25,$30,10
addu $4,$25,48
jal putchar  # int
L.6:
lw $30,16($sp)
lw $31,20($sp)
addu $sp,$sp,24
j $31
.end putd
.globl sort
.text
.align 2; .ent sort
sort:
$fp3=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0x80000000,-8
sw $31,16($sp)
sw $4,24($sp)
sw $5,28($sp)
lw $25,$fp3+0($sp)
sw $25,xx
move $4,$25
move $5,$0
lw $25,$fp3+4($sp)
subu $25,$25,1
sw $25,$fp3+4($sp)
move $6,$25
jal quick  # int
L.11:
lw $31,16($sp)
addu $sp,$sp,24
j $31
.end sort
.globl quick
.text
.align 2; .ent quick
quick:
$fp4=40
.frame $sp,40,$31
addu $sp,$sp,-40
.mask 0xc0e00000,-8
sw $21,16($sp)
sw $22,20($sp)
sw $23,24($sp)
sw $30,28($sp)
sw $31,32($sp)
move $30,$4
move $23,$5
move $22,$6
blt $23,$22,L.13
b L.12
L.13:
move $4,$30
move $5,$23
move $6,$22
jal partition  # int
move $21,$2
move $4,$30
move $5,$23
subu $6,$21,1
jal quick  # int
move $4,$30
addu $5,$21,1
move $6,$22
jal quick  # int
L.12:
lw $21,16($sp)
lw $22,20($sp)
lw $23,24($sp)
lw $30,28($sp)
lw $31,32($sp)
addu $sp,$sp,40
j $31
.end quick
.globl partition
.text
.align 2; .ent partition
partition:
$fp5=40
.frame $sp,40,$31
addu $sp,$sp,-40
.mask 0xc0f00000,-4
sw $20,16($sp)
sw $21,20($sp)
sw $22,24($sp)
sw $23,28($sp)
sw $30,32($sp)
sw $31,36($sp)
move $30,$4
move $23,$5
move $22,$6
addu $22,$22,1
move $20,$23
sll $25,$20,2
addu $25,$25,$30
lw $21,($25)
b L.17
L.16:
addu $23,$23,1
b L.20
L.19:
addu $23,$23,1
L.20:
sll $25,$23,2
addu $25,$25,$30
lw $25,($25)
blt $25,$21,L.19
subu $22,$22,1
b L.23
L.22:
subu $22,$22,1
L.23:
sll $25,$22,2
addu $25,$25,$30
lw $25,($25)
bgt $25,$21,L.22
bge $23,$22,L.25
sll $25,$23,2
addu $4,$25,$30
sll $25,$22,2
addu $5,$25,$30
jal exchange  # int
L.25:
L.17:
blt $23,$22,L.16
sll $25,$20,2
addu $4,$25,$30
sll $25,$22,2
addu $5,$25,$30
jal exchange  # int
move $2,$22
L.15:
lw $20,16($sp)
lw $21,20($sp)
lw $22,24($sp)
lw $23,28($sp)
lw $30,32($sp)
lw $31,36($sp)
addu $sp,$sp,40
j $31
.end partition
.globl exchange
.text
.align 2; .ent exchange
exchange:
$fp6=32
.frame $sp,32,$31
addu $sp,$sp,-32
.mask 0xc0800000,-8
sw $23,16($sp)
sw $30,20($sp)
sw $31,24($sp)
move $30,$4
move $23,$5
la $4,L.28
lw $25,xx
subu $24,$30,$25
div $5,$24,4
subu $25,$23,$25
div $6,$25,4
jal printf  # int
lw $25,($30)
sw $25,$fp6-4($sp)
lw $25,($23)
sw $25,($30)
lw $25,$fp6-4($sp)
sw $25,($23)
L.27:
lw $23,16($sp)
lw $30,20($sp)
lw $31,24($sp)
addu $sp,$sp,32
j $31
.end exchange
.globl xx
.comm xx,4
.rdata
L.28:.ascii "exchange(%d,%d)\012\000"

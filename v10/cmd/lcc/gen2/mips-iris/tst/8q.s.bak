.set reorder
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
move $30,$0
L.2:
sll $25,$30,2
li $24,1
sw $24,down($25)
sw $24,up($25)
L.3:
addu $30,$30,1
blt $30,15,L.2
move $30,$0
L.6:
sll $25,$30,2
li $24,1
sw $24,rows($25)
L.7:
addu $30,$30,1
blt $30,8,L.6
move $4,$0
jal queens  # int
move $2,$0
L.1:
lw $30,16($sp)
lw $31,20($sp)
addu $sp,$sp,24
j $31
.end main
.globl queens
.text
.align 2; .ent queens
queens:
$fp2=32
.frame $sp,32,$31
addu $sp,$sp,-32
.mask 0xc0800000,-8
sw $23,16($sp)
sw $30,20($sp)
sw $31,24($sp)
move $30,$4
move $23,$0
L.11:
sll $25,$23,2
lw $25,rows($25)
beq $25,$0,L.15
subu $25,$23,$30
sll $25,$25,2
lw $25,up+28($25)
beq $25,$0,L.15
addu $25,$23,$30
sll $25,$25,2
lw $25,down($25)
beq $25,$0,L.15
addu $25,$23,$30
sll $25,$25,2
sw $0,down($25)
subu $25,$23,$30
sll $25,$25,2
sw $0,up+28($25)
sll $25,$23,2
sw $0,rows($25)
sll $25,$30,2
sw $23,x($25)
bne $30,7,L.19
jal print  # int
b L.20
L.19:
addu $4,$30,1
jal queens  # int
L.20:
addu $25,$23,$30
sll $25,$25,2
li $24,1
sw $24,down($25)
subu $25,$23,$30
sll $25,$25,2
sw $24,up+28($25)
sll $25,$23,2
sw $24,rows($25)
L.15:
L.12:
addu $23,$23,1
blt $23,8,L.11
L.10:
lw $23,16($sp)
lw $30,20($sp)
lw $31,24($sp)
addu $sp,$sp,32
j $31
.end queens
.globl print
.text
.align 2; .ent print
print:
$fp3=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0xc0000000,-4
sw $30,16($sp)
sw $31,20($sp)
move $30,$0
L.23:
la $4,L.27
sll $25,$30,2
lw $25,x($25)
addu $5,$25,49
jal printf  # int
L.24:
addu $30,$30,1
blt $30,8,L.23
la $4,L.28
jal printf  # int
L.22:
lw $30,16($sp)
lw $31,20($sp)
addu $sp,$sp,24
j $31
.end print
.globl x
.comm x,32
.globl rows
.comm rows,32
.globl down
.comm down,60
.globl up
.comm up,60
.rdata
L.28:.byte 10,0
L.27:.ascii "%c \000"

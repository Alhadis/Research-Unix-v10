.set reorder
.globl x
.data; .align 2; x:.word 0x1
.word 0x2
.word 0x3
.word 0x4
.text
.globl main
.text
.align 2; .ent main
main:
$fp1=104
.frame $sp,104,$31
addu $sp,$sp,-104
.mask 0x80000000,-4
sw $31,100($sp)
la $4,L.2
jal print  # int
la $4,L.3
la $5,L.4
jal print  # int
la $4,L.5
li $5,3
li $6,10
jal print  # int
la $4,L.6
la $5,L.7
la $6,L.8
li $7,4
li $25,10
sw $25,16($sp)
jal print  # int
la $4,L.9
la $5,L.7
la $6,L.8
l.d $f18,L.10
s.d $f18,16($sp)
li $25,10
sw $25,24($sp)
jal print  # int
la $4,L.11
la $25,x
lw $2,0($25)
lw $3,4($25)
sw $2,4($sp)
lw $2,8($25)
sw $3,8($sp)
lw $3,12($25)
sw $2,12($sp)
sw $3,16($sp)
lw $5,4($sp)
lw $6,8($sp)
lw $7,12($sp)
lw $2,0($25)
lw $3,4($25)
sw $2,20($sp)
lw $2,8($25)
sw $3,24($sp)
lw $3,12($25)
sw $2,28($sp)
sw $3,32($sp)
lw $2,0($25)
lw $3,4($25)
sw $2,36($sp)
lw $2,8($25)
sw $3,40($sp)
lw $3,12($25)
sw $2,44($sp)
sw $3,48($sp)
lw $2,0($25)
lw $3,4($25)
sw $2,52($sp)
lw $2,8($25)
sw $3,56($sp)
lw $3,12($25)
sw $2,60($sp)
sw $3,64($sp)
lw $2,0($25)
lw $3,4($25)
sw $2,68($sp)
lw $2,8($25)
sw $3,72($sp)
lw $3,12($25)
sw $2,76($sp)
sw $3,80($sp)
lw $2,0($25)
lw $3,4($25)
sw $2,84($sp)
lw $2,8($25)
sw $3,88($sp)
lw $3,12($25)
sw $2,92($sp)
sw $3,96($sp)
jal print  # int
L.1:
lw $31,100($sp)
addu $sp,$sp,104
j $31
.end main
.globl print
.text
.align 2; .ent print
print:
$fp2=48
.frame $sp,48,$31
addu $sp,$sp,-48
.mask 0xc0800000,-20
sw $23,20($sp)
sw $30,24($sp)
sw $31,28($sp)
sw $4,48($sp)
sw $5,52($sp)
sw $6,56($sp)
sw $7,60($sp)
la $30,$fp2+4($sp)
b L.18
L.15:
lw $25,$fp2+0($sp)
lb $25,($25)
bne $25,37,L.19
lw $25,$fp2+0($sp)
addu $25,$25,1
sw $25,$fp2+0($sp)
lb $23,($25)
beq $23,115,L.35
bgt $23,115,L.39
blt $23,98,L.21
bgt $23,102,L.21
sll $25,$23,2
lw $25,L.41-392($25)
j $25
L.41:
.align 2; .word L.24
.align 2; .word L.29
.align 2; .word L.31
.align 2; .word L.21
.align 2; .word L.37
L.40:
L.39:
beq $23,119,L.33
b L.21
L.24:
addu $25,$30,16
move $30,$25
la $24,$fp2-16($sp)
la $25,-16($25)
lw $2,0($25)
lw $3,4($25)
sw $2,0($24)
lw $2,8($25)
sw $3,4($24)
lw $3,12($25)
sw $2,8($24)
sw $3,12($24)
la $4,L.25
lw $5,$fp2-16($sp)
lw $6,$fp2-12($sp)
lw $7,$fp2-8($sp)
lw $25,$fp2-4($sp)
sw $25,16($sp)
jal printf  # int
b L.20
L.29:
la $4,L.30
addu $25,$30,4
move $30,$25
lb $5,-1($25)
jal printf  # int
b L.20
L.31:
la $4,L.32
addu $25,$30,4
move $30,$25
lw $5,-4($25)
jal printf  # int
b L.20
L.33:
la $4,L.34
addu $25,$30,4
move $30,$25
lh $5,-2($25)
jal printf  # int
b L.20
L.35:
la $4,L.36
addu $25,$30,4
move $30,$25
lw $5,-4($25)
jal printf  # int
b L.20
L.37:
la $4,L.38
addu $25,$30,15
and $25,$25,0xfffffff8
move $30,$25
l.d $f18,-8($25)
mfc1.d $6,$f18
jal printf  # int
b L.20
L.21:
la $4,L.30
lw $25,$fp2+0($sp)
lb $5,($25)
jal printf  # int
b L.20
L.19:
la $4,L.30
lw $25,$fp2+0($sp)
lb $5,($25)
jal printf  # int
L.20:
L.16:
lw $25,$fp2+0($sp)
addu $25,$25,1
sw $25,$fp2+0($sp)
L.18:
lw $25,$fp2+0($sp)
lb $25,($25)
bne $25,$0,L.15
L.12:
lw $23,20($sp)
lw $30,24($sp)
lw $31,28($sp)
addu $sp,$sp,48
j $31
.end print
.rdata
L.38:.byte 37,102,0
L.36:.byte 37,115,0
L.34:.byte 37,120,0
L.32:.byte 37,100,0
L.30:.byte 37,99,0
L.25:.ascii "{%d %d %d %d}\000"
L.11:.ascii "%b %b %b %b %b %b\012\000"
.align 3; L.10:.word 0x40140000
.word 0x0
L.9:.ascii "%s%s %f%c\000"
L.8:.ascii "st\000"
L.7:.ascii "te\000"
L.6:.ascii "%s%s %w%c\000"
L.5:.ascii "test %d%c\000"
L.4:.byte 50,0
L.3:.ascii "test %s\012\000"
L.2:.ascii "test 1\012\000"

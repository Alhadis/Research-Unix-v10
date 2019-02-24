.set reorder
.text
.globl addpoint
.text
.align 2; .ent addpoint
addpoint:
$fp1=8
.frame $sp,8,$31
addu $sp,$sp,-8
sw $4,4($sp)
sw $6,16($sp)
sw $5,12($sp)
sw $7,20($sp)
lw $25,$fp1+4($sp)
lw $24,$fp1+12($sp)
addu $25,$25,$24
sw $25,$fp1+4($sp)
lw $25,$fp1+8($sp)
lw $24,$fp1+16($sp)
addu $25,$25,$24
sw $25,$fp1+8($sp)
lw $25,$fp1-4($sp)
la $24,$fp1+4($sp)
lw $2,0($24)
lw $3,4($24)
sw $2,0($25)
sw $3,4($25)
L.1:
addu $sp,$sp,8
j $31
.end addpoint
.globl canonrect
.text
.align 2; .ent canonrect
canonrect:
$fp2=24
.frame $sp,24,$31
addu $sp,$sp,-24
sw $4,20($sp)
sw $7,36($sp)
sw $6,32($sp)
sw $5,28($sp)
lw $25,$fp2+4($sp)
lw $24,$fp2+12($sp)
bge $25,$24,L.10
lw $12,$fp2+4($sp)
b L.11
L.10:
lw $12,$fp2+12($sp)
L.11:
sw $12,$fp2-20($sp)
lw $25,$fp2+8($sp)
lw $24,$fp2+16($sp)
bge $25,$24,L.20
lw $13,$fp2+8($sp)
b L.21
L.20:
lw $13,$fp2+16($sp)
L.21:
sw $13,$fp2-16($sp)
lw $25,$fp2+4($sp)
lw $24,$fp2+12($sp)
ble $25,$24,L.26
lw $14,$fp2+4($sp)
b L.27
L.26:
lw $14,$fp2+12($sp)
L.27:
sw $14,$fp2-12($sp)
lw $25,$fp2+8($sp)
lw $24,$fp2+16($sp)
ble $25,$24,L.37
lw $15,$fp2+8($sp)
b L.38
L.37:
lw $15,$fp2+16($sp)
L.38:
sw $15,$fp2-8($sp)
lw $25,$fp2-4($sp)
la $24,$fp2-20($sp)
lw $2,0($24)
lw $3,4($24)
sw $2,0($25)
lw $2,8($24)
sw $3,4($25)
lw $3,12($24)
sw $2,8($25)
sw $3,12($25)
L.5:
addu $sp,$sp,24
j $31
.end canonrect
.globl makepoint
.text
.align 2; .ent makepoint
makepoint:
$fp3=16
.frame $sp,16,$31
addu $sp,$sp,-16
sw $4,12($sp)
sw $5,$fp3-12($sp)
sw $6,$fp3-8($sp)
lw $25,$fp3-4($sp)
la $24,$fp3-12($sp)
lw $2,0($24)
lw $3,4($24)
sw $2,0($25)
sw $3,4($25)
L.39:
addu $sp,$sp,16
j $31
.end makepoint
.globl makerect
.text
.align 2; .ent makerect
makerect:
$fp4=48
.frame $sp,48,$31
addu $sp,$sp,-48
.mask 0x80000000,-28
sw $31,20($sp)
sw $4,44($sp)
sw $6,56($sp)
sw $5,52($sp)
sw $7,60($sp)
la $25,$fp4-20($sp)
la $24,$fp4+4($sp)
lw $2,0($24)
lw $3,4($24)
sw $2,0($25)
sw $3,4($25)
la $25,$fp4-12($sp)
la $24,$fp4+12($sp)
lw $2,0($24)
lw $3,4($24)
sw $2,0($25)
sw $3,4($25)
la $25,$fp4-20($sp)
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
lw $4,$fp4-4($sp)
jal canonrect  # int
L.42:
lw $31,20($sp)
addu $sp,$sp,48
j $31
.end makerect
.globl ptinrect
.text
.align 2; .ent ptinrect
ptinrect:
$fp5=0
.frame $sp,0,$31
sw $5,4($sp)
sw $4,0($sp)
sw $7,12($sp)
sw $6,8($sp)
lw $25,$fp5+0($sp)
lw $24,$fp5+8($sp)
blt $25,$24,L.54
lw $24,$fp5+16($sp)
bge $25,$24,L.54
lw $25,$fp5+4($sp)
lw $24,$fp5+12($sp)
blt $25,$24,L.54
lw $25,$fp5+4($sp)
lw $24,$fp5+20($sp)
bge $25,$24,L.54
li $15,1
b L.55
L.54:
move $15,$0
L.55:
move $2,$15
L.46:
j $31
.end ptinrect
.rdata
.align 2; L.57:.word 0x0
.word 0x0
.align 2; L.58:.word 0x140
.word 0x140
.align 2; L.59:.word 0xffffffff
.word 0xffffffff
.word 0x1
.word 0x1
.word 0x14
.word 0x12c
.word 0x1f4
.word 0x190
.text
.globl main
.text
.align 2; .ent main
main:
$fp6=136
.frame $sp,136,$31
addu $sp,$sp,-136
.mask 0xc0000000,-108
sw $30,24($sp)
sw $31,28($sp)
la $25,$fp6-64($sp)
la $24,L.57
lw $2,0($24)
lw $3,4($24)
sw $2,0($25)
sw $3,4($25)
la $25,$fp6-72($sp)
la $24,L.58
lw $2,0($24)
lw $3,4($24)
sw $2,0($25)
sw $3,4($25)
la $25,$fp6-32($sp)
la $24,L.59
addu $24,$24,32
addu $8,$25,32
L.81:
addu $24,$24,-8
addu $8,$8,-8
lw $2,0($24)
lw $3,4($24)
sw $2,0($8)
sw $3,4($8)
.set noat
sltu $1,$25,$8
bgtz $1,L.81
.set at
li $25,-10
move $5,$25
move $6,$25
la $4,$fp6-80($sp)
jal makepoint  # int
la $25,$fp6-72($sp)
lw $2,0($25)
lw $3,4($25)
sw $2,4($sp)
sw $3,8($sp)
lw $5,4($sp)
lw $6,8($sp)
la $25,$fp6-80($sp)
lw $2,0($25)
lw $3,4($25)
sw $2,12($sp)
sw $3,16($sp)
lw $7,12($sp)
la $4,$fp6-88($sp)
jal addpoint  # int
li $25,10
move $5,$25
move $6,$25
la $4,$fp6-96($sp)
jal makepoint  # int
la $25,$fp6-64($sp)
lw $2,0($25)
lw $3,4($25)
sw $2,4($sp)
sw $3,8($sp)
lw $5,4($sp)
lw $6,8($sp)
la $25,$fp6-96($sp)
lw $2,0($25)
lw $3,4($25)
sw $2,12($sp)
sw $3,16($sp)
lw $7,12($sp)
la $4,$fp6-104($sp)
jal addpoint  # int
la $25,$fp6-88($sp)
lw $2,0($25)
lw $3,4($25)
sw $2,4($sp)
sw $3,8($sp)
lw $5,4($sp)
lw $6,8($sp)
la $25,$fp6-104($sp)
lw $2,0($25)
lw $3,4($25)
sw $2,12($sp)
sw $3,16($sp)
lw $7,12($sp)
la $4,$fp6-48($sp)
jal makerect  # int
move $30,$0
b L.68
L.65:
sll $25,$30,3
la $24,$fp6-32($sp)
addu $24,$25,$24
lw $5,($24)
la $24,$fp6-28($sp)
addu $25,$25,$24
lw $6,($25)
la $4,$fp6-56($sp)
jal makepoint  # int
la $4,L.69
sll $25,$30,3
la $24,$fp6-32($sp)
addu $25,$25,$24
lw $5,($25)
lw $6,$fp6-52($sp)
jal printf  # int
la $25,$fp6-56($sp)
lw $2,0($25)
lw $3,4($25)
sw $2,0($sp)
sw $3,4($sp)
lw $4,0($sp)
lw $5,4($sp)
la $25,$fp6-48($sp)
lw $2,0($25)
lw $3,4($25)
sw $2,8($sp)
lw $2,8($25)
sw $3,12($sp)
lw $3,12($25)
sw $2,16($sp)
sw $3,20($sp)
lw $6,8($sp)
lw $7,12($sp)
jal ptinrect  # int
bne $2,$0,L.73
la $4,L.75
jal printf  # int
L.73:
la $4,L.76
lw $5,$fp6-48($sp)
lw $6,$fp6-44($sp)
lw $7,$fp6-40($sp)
lw $25,$fp6-36($sp)
sw $25,16($sp)
jal printf  # int
L.66:
addu $30,$30,1
L.68:
bltu $30,4,L.65
move $4,$0
jal exit  # int
L.56:
lw $30,24($sp)
lw $31,28($sp)
addu $sp,$sp,136
j $31
.end main
.rdata
L.76:.ascii "within [%d,%d; %d,%d]\012\000"
L.75:.ascii "not \000"
L.69:.ascii "(%d,%d) is \000"

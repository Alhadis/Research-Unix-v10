.set reorder
.text
.globl main
.text
.align 2; .ent main
main:
$fp1=32
.frame $sp,32,$31
addu $sp,$sp,-32
.mask 0xc0800000,-8
sw $23,16($sp)
sw $30,20($sp)
sw $31,24($sp)
la $30,L.6
b L.5
L.2:
lb $4,($30)
jal backslash  # int
la $4,L.7
lb $5,($30)
move $6,$2
jal printf  # int
L.3:
addu $30,$30,1
L.5:
lb $25,($30)
bne $25,$0,L.2
jal f  # int
jal g  # int
jal h  # int
li $23,16777216
b L.11
L.8:
move $4,$23
jal big  # int
L.9:
addu $23,$23,16777216
L.11:
and $25,$23,0x7000000
bne $25,$0,L.8
jal limit  # int
move $2,$0
L.1:
lw $23,16($sp)
lw $30,20($sp)
lw $31,24($sp)
addu $sp,$sp,32
j $31
.end main
.globl backslash
.text
.align 2; .ent backslash
backslash:
$fp2=0
.frame $sp,0,$31
beq $4,110,L.17
bgt $4,110,L.21
beq $4,102,L.16
bgt $4,102,L.13
beq $4,98,L.15
b L.13
L.21:
beq $4,114,L.18
beq $4,116,L.19
beq $4,118,L.20
b L.13
L.15:
li $2,8
b L.12
L.16:
li $2,12
b L.12
L.17:
li $2,10
b L.12
L.18:
li $2,13
b L.12
L.19:
li $2,9
b L.12
L.20:
li $2,11
b L.12
L.13:
move $2,$4
L.12:
j $31
.end backslash
.globl f
.text
.align 2; .ent f
f:
$fp3=32
.frame $sp,32,$31
addu $sp,$sp,-32
.mask 0xc0c00000,-4
sw $22,16($sp)
sw $23,20($sp)
sw $30,24($sp)
sw $31,28($sp)
move $23,$0
la $4,L.23
jal printf  # int
move $30,$0
L.24:
move $22,$30
blt $30,1,L.28
bgt $30,20,L.28
sll $25,$30,2
lw $25,L.41-4($25)
j $25
L.41:
.align 2; .word L.30
.align 2; .word L.31
.align 2; .word L.28
.align 2; .word L.28
.align 2; .word L.28
.align 2; .word L.28
.align 2; .word L.32
.align 2; .word L.33
.align 2; .word L.34
.align 2; .word L.28
.align 2; .word L.28
.align 2; .word L.28
.align 2; .word L.28
.align 2; .word L.28
.align 2; .word L.28
.align 2; .word L.35
.align 2; .word L.36
.align 2; .word L.37
.align 2; .word L.38
.align 2; .word L.39
L.30:
move $23,$30
b L.29
L.31:
move $23,$30
b L.29
L.32:
move $23,$30
b L.29
L.33:
move $23,$30
b L.29
L.34:
move $23,$30
b L.29
L.35:
move $23,$30
b L.29
L.36:
move $23,$30
b L.29
L.37:
move $23,$30
b L.29
L.38:
move $23,$30
b L.29
L.39:
move $23,$30
L.28:
L.29:
la $4,L.43
move $5,$23
jal printf  # int
L.25:
addu $30,$30,1
ble $30,20,L.24
L.22:
lw $22,16($sp)
lw $23,20($sp)
lw $30,24($sp)
lw $31,28($sp)
addu $sp,$sp,32
j $31
.end f
.globl g
.text
.align 2; .ent g
g:
$fp4=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0xc0000000,-4
sw $30,16($sp)
sw $31,20($sp)
la $4,L.45
jal printf  # int
li $30,1
L.46:
blt $30,1001,L.64
bgt $30,1004,L.63
sll $25,$30,2
lw $25,L.65-4004($25)
j $25
L.65:
.align 2; .word L.59
.align 2; .word L.59
.align 2; .word L.59
.align 2; .word L.59
L.64:
blt $30,1,L.50
bgt $30,8,L.50
sll $25,$30,2
lw $25,L.68-4($25)
j $25
L.68:
.align 2; .word L.52
.align 2; .word L.52
.align 2; .word L.54
.align 2; .word L.54
.align 2; .word L.54
.align 2; .word L.56
.align 2; .word L.56
.align 2; .word L.56
L.67:
L.63:
blt $30,3001,L.50
bgt $30,3004,L.50
sll $25,$30,2
lw $25,L.71-12004($25)
j $25
L.71:
.align 2; .word L.61
.align 2; .word L.61
.align 2; .word L.61
.align 2; .word L.61
L.52:
la $4,L.53
move $5,$30
jal printf  # int
b L.51
L.54:
la $4,L.55
move $5,$30
jal printf  # int
b L.51
L.56:
la $4,L.57
move $5,$30
jal printf  # int
L.50:
la $4,L.58
move $5,$30
jal printf  # int
b L.51
L.59:
la $4,L.60
move $5,$30
jal printf  # int
b L.51
L.61:
la $4,L.62
move $5,$30
jal printf  # int
L.51:
L.47:
addu $30,$30,1
ble $30,10,L.46
L.44:
lw $30,16($sp)
lw $31,20($sp)
addu $sp,$sp,24
j $31
.end g
.globl h
.text
.align 2; .ent h
h:
$fp5=32
.frame $sp,32,$31
addu $sp,$sp,-32
.mask 0xc0800000,-8
sw $23,16($sp)
sw $30,20($sp)
sw $31,24($sp)
move $23,$0
la $4,L.74
jal printf  # int
li $30,1
L.75:
beq $30,128,L.81
bgt $30,128,L.94
beq $30,16,L.83
bgt $30,16,L.95
beq $30,8,L.84
b L.79
L.95:
beq $30,120,L.85
b L.79
L.94:
beq $30,280,L.86
bgt $30,280,L.96
beq $30,264,L.87
bgt $30,264,L.97
beq $30,248,L.88
b L.79
L.97:
beq $30,272,L.89
b L.79
L.96:
beq $30,304,L.90
bgt $30,304,L.98
beq $30,296,L.91
bgt $30,296,L.79
beq $30,288,L.92
b L.79
L.98:
beq $30,312,L.93
b L.79
L.79:
addu $23,$23,1
b L.76
L.81:
la $4,L.82
move $5,$30
jal printf  # int
b L.80
L.83:
la $4,L.82
move $5,$30
jal printf  # int
b L.80
L.84:
la $4,L.82
move $5,$30
jal printf  # int
b L.80
L.85:
la $4,L.82
move $5,$30
jal printf  # int
b L.80
L.86:
la $4,L.82
move $5,$30
jal printf  # int
b L.80
L.87:
la $4,L.82
move $5,$30
jal printf  # int
b L.80
L.88:
la $4,L.82
move $5,$30
jal printf  # int
b L.80
L.89:
la $4,L.82
move $5,$30
jal printf  # int
b L.80
L.90:
la $4,L.82
move $5,$30
jal printf  # int
b L.80
L.91:
la $4,L.82
move $5,$30
jal printf  # int
b L.80
L.92:
la $4,L.82
move $5,$30
jal printf  # int
b L.80
L.93:
la $4,L.82
move $5,$30
jal printf  # int
L.80:
L.76:
addu $30,$30,1
ble $30,500,L.75
la $4,L.99
move $5,$23
jal printf  # int
L.73:
lw $23,16($sp)
lw $30,20($sp)
lw $31,24($sp)
addu $sp,$sp,32
j $31
.end h
.globl big
.text
.align 2; .ent big
big:
$fp6=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0xc0000000,-4
sw $30,16($sp)
sw $31,20($sp)
sw $4,24($sp)
lw $25,$fp6+0($sp)
and $30,$25,0x6000000
beq $30,33554432,L.106
bgt $30,33554432,L.109
beq $30,-2,L.104
beq $30,-1,L.104
beq $30,$0,L.104
b L.101
L.109:
beq $30,67108864,L.107
b L.101
L.104:
la $4,L.105
lw $5,$fp6+0($sp)
jal printf  # int
b L.102
L.106:
la $4,L.105
lw $5,$fp6+0($sp)
jal printf  # int
b L.102
L.107:
la $4,L.105
lw $5,$fp6+0($sp)
jal printf  # int
b L.102
L.101:
la $4,L.108
lw $5,$fp6+0($sp)
jal printf  # int
L.102:
L.100:
lw $30,16($sp)
lw $31,20($sp)
addu $sp,$sp,24
j $31
.end big
.globl limit
.text
.align 2; .ent limit
limit:
$fp7=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0xc0000000,-4
sw $30,16($sp)
sw $31,20($sp)
li $30,-2147483648
L.111:
blt $30,-2147483648,L.115
bgt $30,-2147483644,L.115
sll $25,$30,2
li $24,-2147483648
sll $24,$24,2
subu $25,$25,$24
lw $25,L.129($25)
j $25
L.129:
.align 2; .word L.117
.align 2; .word L.119
.align 2; .word L.121
.align 2; .word L.123
.align 2; .word L.125
L.117:
la $4,L.118
jal printf  # int
b L.116
L.119:
la $4,L.120
jal printf  # int
b L.116
L.121:
la $4,L.122
jal printf  # int
b L.116
L.123:
la $4,L.124
jal printf  # int
b L.116
L.125:
la $4,L.126
jal printf  # int
b L.116
L.115:
la $4,L.127
jal printf  # int
L.116:
L.112:
addu $30,$30,1
ble $30,-2147483643,L.111
li $30,2147483647
L.130:
blt $30,2147483643,L.134
bgt $30,2147483647,L.134
sll $25,$30,2
li $24,2147483643
sll $24,$24,2
subu $25,$25,$24
lw $25,L.142($25)
j $25
L.142:
.align 2; .word L.140
.align 2; .word L.139
.align 2; .word L.138
.align 2; .word L.137
.align 2; .word L.136
L.136:
la $4,L.118
jal printf  # int
b L.135
L.137:
la $4,L.120
jal printf  # int
b L.135
L.138:
la $4,L.122
jal printf  # int
b L.135
L.139:
la $4,L.124
jal printf  # int
b L.135
L.140:
la $4,L.126
jal printf  # int
b L.135
L.134:
la $4,L.127
jal printf  # int
L.135:
L.131:
subu $30,$30,1
bge $30,2147483642,L.130
L.110:
lw $30,16($sp)
lw $31,20($sp)
addu $sp,$sp,24
j $31
.end limit
.rdata
L.127:.byte 53,10,0
L.126:.byte 52,10,0
L.124:.byte 51,10,0
L.122:.byte 50,10,0
L.120:.byte 49,10,0
L.118:.byte 48,10,0
L.108:.ascii "x = 0x%x (default)\012\000"
L.105:.ascii "x = 0x%x\012\000"
L.99:.ascii "%d defaults\012\000"
L.82:.ascii "i = %d\012\000"
L.74:.byte 104,58,10,0
L.62:.ascii "6 %d\012\000"
L.60:.ascii "5 %d\012\000"
L.58:.ascii "d %d\012\000"
L.57:.ascii "3 %d\012\000"
L.55:.ascii "2 %d\012\000"
L.53:.ascii "1 %d\012\000"
L.45:.byte 103,58,10,0
L.43:.ascii "x = %d\012\000"
L.23:.byte 102,58,10,0
L.7:.ascii "%c = 0x%x\012\000"
L.6:.ascii "bfnrtvx\000"

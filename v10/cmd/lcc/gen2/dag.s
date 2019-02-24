.set reorder
.globl codehead
.data; .align 2; codehead:.word 0x6
.space 44
.globl codelist
.sdata; .align 2; codelist:.word codehead
.text
.globl addlocal
.text
.align 2; .ent addlocal
addlocal:
$fp1=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0x80000000,-8
sw $31,16($sp)
sw $4,24($sp)
lw $25,$fp1+0($sp)
lw $25,32($25)
and $25,$25,0x80000000
bne $25,$0,L.24
li $4,2
jal code  # int
move $25,$2
lw $24,$fp1+0($sp)
sw $24,12($25)
lw $25,$fp1+0($sp)
lw $24,32($25)
or $24,$24,0x80000000
sw $24,32($25)
lw $25,$fp1+0($sp)
lw $24,level
sh $24,28($25)
L.24:
L.23:
lw $31,16($sp)
addu $sp,$sp,24
j $31
.end addlocal
.globl btot
.text
.align 2; .ent btot
btot:
$fp2=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0xc0000000,-4
sw $30,16($sp)
sw $31,20($sp)
sw $4,24($sp)
lw $25,$fp2+0($sp)
and $30,$25,15
blt $30,1,L.27
bgt $30,7,L.27
sll $25,$30,2
lw $25,L.40-4($25)
j $25
L.40:
.align 2; .word L.30
.align 2; .word L.31
.align 2; .word L.32
.align 2; .word L.33
.align 2; .word L.34
.align 2; .word L.35
.align 2; .word L.36
L.30:
lw $2,floattype
b L.26
L.31:
lw $2,doubletype
b L.26
L.32:
lw $2,chartype
b L.26
L.33:
lw $2,shorttype
b L.26
L.34:
lw $2,inttype
b L.26
L.35:
lw $2,unsignedtype
b L.26
L.36:
lw $2,voidptype
b L.26
L.27:
la $4,L.37
la $5,L.38
li $6,46
jal fatal  # int
move $2,$0
L.26:
lw $30,16($sp)
lw $31,20($sp)
addu $sp,$sp,24
j $31
.end btot
.globl code
.text
.align 2; .ent code
code:
$fp3=32
.frame $sp,32,$31
addu $sp,$sp,-32
.mask 0xc0800000,-8
sw $23,16($sp)
sw $30,20($sp)
sw $31,24($sp)
sw $4,32($sp)
lw $25,$fp3+0($sp)
ble $25,5,L.43
lw $30,codelist
b L.48
L.45:
lw $30,4($30)
L.46:
L.48:
lw $25,($30)
blt $25,5,L.45
lw $25,($30)
bne $25,9,L.49
la $4,L.51
jal warning  # int
L.49:
L.43:
lw $25,transient
li $24,48
sw $24,($25)
lw $25,transient
lw $24,($25)
lw $15,4($25)
addu $24,$24,$15
lw $25,8($25)
bltu $24,$25,L.53
lw $25,transient
lw $4,($25)
la $5,transient
jal allocate  # int
move $23,$2
b L.54
L.53:
lw $25,transient
lw $24,($25)
lw $15,4($25)
addu $24,$24,$15
sw $24,4($25)
lw $25,transient
lw $24,4($25)
lw $25,($25)
subu $23,$24,$25
L.54:
move $30,$23
lw $25,$fp3+0($sp)
sw $25,($30)
lw $25,codelist
sw $25,4($30)
sw $0,8($30)
lw $25,codelist
sw $30,8($25)
sw $30,codelist
move $2,$30
L.42:
lw $23,16($sp)
lw $30,20($sp)
lw $31,24($sp)
addu $sp,$sp,32
j $31
.end code
.text
.align 2; .ent dagnode
dagnode:
$fp4=32
.frame $sp,32,$31
addu $sp,$sp,-32
.mask 0xc0c00000,-4
sw $22,16($sp)
sw $23,20($sp)
sw $30,24($sp)
sw $31,28($sp)
sw $4,32($sp)
sw $5,36($sp)
sw $6,40($sp)
sw $7,44($sp)
lw $25,transient
li $24,88
sw $24,($25)
lw $25,transient
lw $24,($25)
lw $15,4($25)
addu $24,$24,$15
lw $25,8($25)
bltu $24,$25,L.57
lw $25,transient
lw $4,($25)
la $5,transient
jal allocate  # int
move $23,$2
b L.58
L.57:
lw $25,transient
lw $24,($25)
lw $15,4($25)
addu $24,$24,$15
sw $24,4($25)
lw $25,transient
lw $24,4($25)
lw $25,($25)
subu $23,$24,$25
L.58:
move $30,$23
move $23,$30
addu $22,$23,64
b L.62
L.59:
sw $0,28($23)
sw $0,24($23)
sw $0,20($23)
sw $0,16($23)
sw $0,12($23)
sw $0,8($23)
sw $0,4($23)
sw $0,($23)
L.60:
addu $23,$23,32
L.62:
bltu $23,$22,L.59
sw $0,($23)
sw $0,4($23)
sw $0,8($23)
sw $0,12($23)
sw $0,16($23)
lw $25,$fp4+0($sp)
sw $25,($30)
lw $25,$fp4+4($sp)
sw $25,24($30)
beq $25,$0,L.63
lw $25,$fp4+4($sp)
lh $24,4($25)
addu $24,$24,1
sh $24,4($25)
L.63:
lw $25,$fp4+8($sp)
sw $25,28($30)
beq $25,$0,L.65
lw $25,$fp4+8($sp)
lh $24,4($25)
addu $24,$24,1
sh $24,4($25)
L.65:
lw $25,$fp4+12($sp)
sw $25,8($30)
move $2,$30
L.55:
lw $22,16($sp)
lw $23,20($sp)
lw $30,24($sp)
lw $31,28($sp)
addu $sp,$sp,32
j $31
.end dagnode
.globl emitcode
.text
.align 2; .ent emitcode
emitcode:
$fp5=48
.frame $sp,48,$31
addu $sp,$sp,-48
.mask 0xc0f00000,-12
sw $20,16($sp)
sw $21,20($sp)
sw $22,24($sp)
sw $23,28($sp)
sw $30,32($sp)
sw $31,36($sp)
la $25,$fp5-8($sp)
la $24,src
lw $2,0($24)
lw $3,4($24)
sw $2,0($25)
sw $3,4($25)
move $23,$0
la $30,codehead
b L.71
L.68:
lw $22,($30)
blt $22,$0,L.72
bgt $22,10,L.72
sll $25,$22,2
lw $25,L.100($25)
j $25
L.100:
.align 2; .word L.75
.align 2; .word L.78
.align 2; .word L.81
.align 2; .word L.72
.align 2; .word L.84
.align 2; .word L.90
.align 2; .word L.72
.align 2; .word L.72
.align 2; .word L.90
.align 2; .word L.87
.align 2; .word L.91
L.75:
sw $23,12($30)
move $23,$30
lw $25,glevel
beq $25,$0,L.73
lw $25,IR
lw $25,104($25)
beq $25,$0,L.73
li $4,123
lh $25,18($23)
subu $5,$25,5
lw $6,20($23)
lw $25,IR
lw $25,104($25)
jal $25  # int
li $4,1
jal swtoseg  # int
b L.73
L.78:
lw $25,glevel
beq $25,$0,L.79
lw $25,IR
lw $25,104($25)
beq $25,$0,L.79
lw $4,24($23)
lh $5,18($23)
la $6,typestab
move $7,$0
jal foreach  # int
lw $4,28($23)
lh $5,18($23)
la $6,typestab
move $7,$0
jal foreach  # int
li $4,125
lh $25,18($23)
subu $5,$25,5
lw $6,20($23)
lw $25,IR
lw $25,104($25)
jal $25  # int
li $4,1
jal swtoseg  # int
L.79:
lw $23,12($23)
b L.73
L.81:
lw $25,glevel
beq $25,$0,L.73
lw $25,IR
lw $25,124($25)
beq $25,$0,L.73
lw $4,12($30)
lw $25,IR
lw $25,124($25)
jal $25  # int
li $4,1
jal swtoseg  # int
b L.73
L.84:
la $25,src
la $24,12($30)
lw $2,0($24)
lw $3,4($24)
sw $2,0($25)
sw $3,4($25)
lw $25,glevel
beq $25,$0,L.73
lw $25,IR
lw $25,120($25)
beq $25,$0,L.73
addu $4,$30,12
lw $25,IR
lw $25,120($25)
jal $25  # int
li $4,1
jal swtoseg  # int
b L.73
L.87:
lw $25,12($30)
bne $25,$0,L.88
b L.73
L.88:
L.90:
lw $4,12($30)
lw $25,IR
lw $25,60($25)
jal $25  # int
b L.73
L.91:
lw $25,28($30)
lw $20,($25)
move $21,$0
b L.95
L.96:
lw $25,20($30)
lw $4,68($25)
lw $25,IR
lw $25,44($25)
jal $25  # int
L.97:
move $25,$20
addu $20,$25,1
sll $24,$21,2
lw $15,28($30)
addu $24,$24,$15
lw $24,($24)
blt $25,$24,L.96
sll $25,$21,2
lw $24,32($30)
addu $25,$25,$24
lw $25,($25)
lw $4,68($25)
lw $25,IR
lw $25,44($25)
jal $25  # int
L.93:
addu $21,$21,1
L.95:
lw $25,24($30)
blt $21,$25,L.97
L.72:
L.73:
L.69:
lw $30,8($30)
L.71:
lw $25,errcnt
bgt $25,$0,L.101
bne $30,$0,L.68
L.101:
la $25,src
la $24,$fp5-8($sp)
lw $2,0($24)
lw $3,4($24)
sw $2,0($25)
sw $3,4($25)
L.67:
lw $20,16($sp)
lw $21,20($sp)
lw $22,24($sp)
lw $23,28($sp)
lw $30,32($sp)
lw $31,36($sp)
addu $sp,$sp,48
j $31
.end emitcode
.text
.align 2; .ent fixup
fixup:
$fp6=32
.frame $sp,32,$31
addu $sp,$sp,-32
.mask 0xc0800000,-8
sw $23,16($sp)
sw $30,20($sp)
sw $31,24($sp)
move $30,$4
b L.106
L.103:
lw $25,($30)
and $23,$25,0xfffffff0
beq $23,544,L.113
beq $23,560,L.113
bgt $23,560,L.115
beq $23,512,L.113
beq $23,528,L.113
bgt $23,528,L.107
beq $23,480,L.113
beq $23,496,L.113
b L.107
L.115:
beq $23,576,L.110
beq $23,592,L.113
b L.107
L.110:
lw $25,24($30)
lw $25,($25)
bne $25,263,L.108
lw $25,24($30)
lw $24,8($25)
lw $24,68($24)
sw $24,8($25)
b L.108
L.113:
lw $25,8($30)
bne $25,$0,L.114
la $4,L.37
la $5,L.38
li $6,151
jal fatal  # int
L.114:
lw $25,8($30)
lw $25,68($25)
sw $25,8($30)
L.107:
L.108:
L.104:
lw $30,48($30)
L.106:
bne $30,$0,L.103
L.102:
lw $23,16($sp)
lw $30,20($sp)
lw $31,24($sp)
addu $sp,$sp,32
j $31
.end fixup
.globl gencode
.text
.align 2; .ent gencode
gencode:
$fp7=64
.frame $sp,64,$31
addu $sp,$sp,-64
.mask 0xc0ff0000,-12
sw $16,16($sp)
sw $17,20($sp)
sw $18,24($sp)
sw $19,28($sp)
sw $20,32($sp)
sw $21,36($sp)
sw $22,40($sp)
sw $23,44($sp)
sw $30,48($sp)
sw $31,52($sp)
move $30,$4
move $23,$5
la $25,$fp7-8($sp)
la $24,src
lw $2,0($24)
lw $3,4($24)
sw $2,0($25)
sw $3,4($25)
lw $25,codehead+8
lw $22,8($25)
lw $25,codehead+8
sw $25,codelist
move $19,$0
b L.122
L.119:
lbu $25,30($21)
lbu $24,30($20)
bne $25,$24,L.125
lw $25,40($21)
lw $24,40($20)
beq $25,$24,L.123
L.125:
move $4,$20
jal idnode  # int
move $4,$21
move $5,$2
jal asgn  # int
move $4,$2
move $5,$0
move $6,$0
jal walk  # int
lw $25,glevel
beq $25,$0,L.124
lw $25,IR
lw $25,124($25)
beq $25,$0,L.124
move $4,$21
lw $25,IR
lw $25,124($25)
jal $25  # int
move $4,$20
lw $25,IR
lw $25,124($25)
jal $25  # int
li $4,1
jal swtoseg  # int
b L.124
L.123:
lw $25,glevel
beq $25,$0,L.128
lw $25,IR
lw $25,124($25)
beq $25,$0,L.128
move $4,$21
lw $25,IR
lw $25,124($25)
jal $25  # int
li $4,1
jal swtoseg  # int
L.128:
L.124:
L.120:
addu $19,$19,1
L.122:
sll $25,$19,2
addu $24,$25,$23
lw $24,($24)
move $21,$24
beq $24,$0,L.130
addu $25,$25,$30
lw $25,($25)
move $20,$25
bne $25,$0,L.119
L.130:
lw $25,codelist
sw $22,8($25)
lw $25,codelist
sw $25,4($22)
move $18,$0
la $22,codehead
b L.134
L.131:
lw $17,($22)
blt $17,$0,L.135
bgt $17,10,L.135
sll $25,$17,2
lw $25,L.160($25)
j $25
L.160:
.align 2; .word L.140
.align 2; .word L.149
.align 2; .word L.150
.align 2; .word L.152
.align 2; .word L.139
.align 2; .word L.156
.align 2; .word L.136
.align 2; .word L.135
.align 2; .word L.156
.align 2; .word L.153
.align 2; .word L.136
b L.136
L.139:
la $25,src
la $24,12($22)
lw $2,0($24)
lw $3,4($24)
sw $2,0($25)
sw $3,4($25)
b L.136
L.140:
lw $16,20($22)
sw $18,12($22)
move $18,$22
addu $4,$18,32
lw $25,IR
lw $25,36($25)
jal $25  # int
b L.144
L.141:
lw $25,($16)
l.s $f18,36($25)
l.s $f16,L.148
c.eq.s $f18,$f16; bc1f L.147
lw $25,glevel
beq $25,$0,L.145
L.147:
lw $4,($16)
lw $25,IR
lw $25,84($25)
jal $25  # int
L.145:
L.142:
addu $16,$16,4
L.144:
lw $25,($16)
bne $25,$0,L.141
b L.136
L.149:
addu $4,$18,32
lw $25,IR
lw $25,40($25)
jal $25  # int
lw $18,12($18)
b L.136
L.150:
lw $25,12($22)
lhu $25,28($25)
lh $24,18($18)
beq $25,$24,L.151
la $4,L.37
la $5,L.38
li $6,202
jal fatal  # int
L.151:
lw $4,12($22)
lw $25,IR
lw $25,84($25)
jal $25  # int
b L.136
L.152:
lw $4,12($22)
lw $5,16($22)
lw $6,20($22)
lw $25,IR
lw $25,32($25)
jal $25  # int
b L.136
L.153:
lw $25,12($22)
bne $25,$0,L.154
b L.136
L.154:
L.156:
lw $4,12($22)
jal fixup  # int
lw $25,IR
lw $25,28($25)
and $25,$25,0x2000000
beq $25,$0,L.157
lw $4,12($22)
jal undag  # int
move $25,$2
sw $25,12($22)
L.157:
lw $4,12($22)
lw $25,IR
lw $25,72($25)
jal $25  # int
move $25,$2
sw $25,12($22)
b L.136
L.135:
la $4,L.37
la $5,L.38
li $6,218
jal fatal  # int
L.136:
L.132:
lw $22,8($22)
L.134:
lw $25,errcnt
bgt $25,$0,L.161
bne $22,$0,L.131
L.161:
la $25,src
la $24,$fp7-8($sp)
lw $2,0($24)
lw $3,4($24)
sw $2,0($25)
sw $3,4($25)
L.116:
lw $16,16($sp)
lw $17,20($sp)
lw $18,24($sp)
lw $19,28($sp)
lw $20,32($sp)
lw $21,36($sp)
lw $22,40($sp)
lw $23,44($sp)
lw $30,48($sp)
lw $31,52($sp)
addu $sp,$sp,64
j $31
.end gencode
.text
.align 2; .ent haskid
haskid:
$fp8=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0xc0000000,-4
sw $30,16($sp)
sw $31,20($sp)
sw $4,24($sp)
sw $5,28($sp)
lw $25,$fp8+4($sp)
bne $25,$0,L.163
move $2,$0
b L.162
L.163:
lw $25,$fp8+0($sp)
lw $24,$fp8+4($sp)
bne $25,$24,L.165
li $2,1
b L.162
L.165:
lw $4,$fp8+0($sp)
lw $25,$fp8+4($sp)
lw $5,24($25)
jal haskid  # int
bne $2,$0,L.170
lw $4,$fp8+0($sp)
lw $25,$fp8+4($sp)
lw $5,28($25)
jal haskid  # int
beq $2,$0,L.168
L.170:
li $30,1
b L.169
L.168:
move $30,$0
L.169:
move $2,$30
L.162:
lw $30,16($sp)
lw $31,20($sp)
addu $sp,$sp,24
j $31
.end haskid
.text
.align 2; .ent labelnode
labelnode:
$fp9=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0x80000000,-8
sw $31,16($sp)
sw $4,24($sp)
lw $25,$fp9+0($sp)
bne $25,$0,L.172
la $4,L.37
la $5,L.38
li $6,235
jal fatal  # int
L.172:
lw $25,nodelist
move $24,$25
beq $24,$0,L.173
lw $24,($25)
bne $24,600,L.173
lw $25,8($25)
beq $25,$0,L.173
lw $4,$fp9+0($sp)
jal findlabel  # int
move $4,$2
lw $25,nodelist
lw $5,8($25)
jal equatelab  # int
lw $2,nodelist
b L.171
L.173:
lw $4,$fp9+0($sp)
jal findlabel  # int
li $4,600
move $5,$0
move $6,$0
move $7,$2
jal newnode  # int
move $4,$2
jal list  # int
L.171:
lw $31,16($sp)
addu $sp,$sp,24
j $31
.end labelnode
.text
.align 2; .ent list
list:
$fp10=0
.frame $sp,0,$31
beq $4,$0,L.176
lw $25,48($4)
bne $25,$0,L.176
lw $25,nodelist
beq $25,$0,L.178
lw $25,nodelist
lw $25,48($25)
sw $25,48($4)
lw $25,nodelist
sw $4,48($25)
b L.179
L.178:
sw $4,48($4)
L.179:
sw $4,nodelist
L.176:
move $2,$4
L.175:
j $31
.end list
.globl listnodes
.text
.align 2; .ent listnodes
listnodes:
$fp11=80
.frame $sp,80,$31
addu $sp,$sp,-80
.mask 0xc0f80000,-40
sw $19,16($sp)
sw $20,20($sp)
sw $21,24($sp)
sw $22,28($sp)
sw $23,32($sp)
sw $30,36($sp)
sw $31,40($sp)
move $30,$4
sw $5,84($sp)
sw $6,88($sp)
bne $30,$0,L.181
move $2,$0
b L.180
L.181:
lw $25,16($30)
beq $25,$0,L.183
lw $2,16($30)
b L.180
L.183:
lw $25,($30)
and $25,$25,0xfffffff0
sw $25,$fp11-4($sp)
lw $22,$fp11-4($sp)
beq $22,336,L.323
beq $22,352,L.323
bgt $22,352,L.376
beq $22,176,L.336
beq $22,192,L.336
bgt $22,192,L.377
beq $22,80,L.336
beq $22,96,L.336
bgt $22,96,L.378
beq $22,48,L.292
beq $22,64,L.339
bgt $22,64,L.185
beq $22,16,L.211
beq $22,32,L.266
b L.185
L.378:
beq $22,144,L.336
beq $22,160,L.336
bgt $22,160,L.185
beq $22,112,L.336
beq $22,128,L.336
b L.185
L.377:
beq $22,272,L.371
beq $22,288,L.366
bgt $22,288,L.379
beq $22,240,L.333
beq $22,256,L.371
bgt $22,256,L.185
beq $22,208,L.248
b L.185
L.379:
beq $22,304,L.323
beq $22,320,L.323
b L.185
L.376:
beq $22,528,L.273
beq $22,544,L.273
bgt $22,544,L.380
beq $22,432,L.323
beq $22,448,L.323
bgt $22,448,L.381
beq $22,400,L.336
beq $22,416,L.323
bgt $22,416,L.185
beq $22,368,L.318
beq $22,384,L.313
b L.185
L.381:
beq $22,496,L.273
beq $22,512,L.273
bgt $22,512,L.185
beq $22,464,L.323
beq $22,480,L.273
b L.185
L.380:
beq $22,608,L.188
beq $22,624,L.191
bgt $22,624,L.382
beq $22,560,L.273
beq $22,576,L.243
b L.185
L.382:
beq $22,672,L.226
beq $22,688,L.363
bgt $22,688,L.185
beq $22,640,L.192
beq $22,656,L.195
b L.185
L.188:
lw $25,$fp11+8($sp)
beq $25,$0,L.189
lw $4,8($30)
move $5,$0
lw $6,$fp11+8($sp)
jal listnodes  # int
lw $4,12($30)
move $5,$0
lw $6,$fp11+8($sp)
jal listnodes  # int
move $25,$2
sw $25,$fp11-12($sp)
b L.190
L.189:
li $4,1
jal genlabel  # int
sw $2,$fp11+8($sp)
lw $4,8($30)
move $5,$0
move $6,$2
jal listnodes  # int
lw $4,12($30)
lw $5,$fp11+4($sp)
move $6,$0
jal listnodes  # int
lw $4,$fp11+8($sp)
jal labelnode  # int
move $25,$2
sw $25,$fp11-12($sp)
L.190:
move $4,$0
jal trash  # int
lw $2,$fp11-12($sp)
b L.180
L.191:
lw $4,8($30)
lw $5,$fp11+8($sp)
lw $6,$fp11+4($sp)
jal listnodes  # int
b L.180
L.192:
lw $25,$fp11+4($sp)
beq $25,$0,L.193
lw $4,8($30)
lw $5,$fp11+4($sp)
move $6,$0
jal listnodes  # int
lw $4,12($30)
lw $5,$fp11+4($sp)
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-12($sp)
b L.194
L.193:
li $4,1
jal genlabel  # int
sw $2,$fp11+4($sp)
lw $4,8($30)
move $5,$2
move $6,$0
jal listnodes  # int
lw $4,12($30)
move $5,$0
lw $6,$fp11+8($sp)
jal listnodes  # int
lw $4,$fp11+4($sp)
jal labelnode  # int
move $25,$2
sw $25,$fp11-12($sp)
L.194:
move $4,$0
jal trash  # int
lw $2,$fp11-12($sp)
b L.180
L.195:
lw $25,$fp11+4($sp)
bne $25,$0,L.197
lw $25,$fp11+8($sp)
beq $25,$0,L.196
L.197:
la $4,L.37
la $5,L.38
li $6,296
jal fatal  # int
L.196:
lw $25,24($30)
beq $25,$0,L.198
lw $4,24($30)
jal addlocal  # int
L.198:
move $4,$0
jal trash  # int
li $4,2
jal genlabel  # int
sw $2,$fp11+8($sp)
lw $4,8($30)
move $5,$0
move $6,$2
jal listnodes  # int
move $4,$0
jal trash  # int
lw $25,12($30)
sw $25,$fp11-16($sp)
beq $25,$0,L.200
lw $25,$fp11-16($sp)
lw $25,($25)
beq $25,672,L.202
la $4,L.37
la $5,L.38
li $6,303
jal fatal  # int
L.202:
lw $25,$fp11-16($sp)
lw $4,8($25)
move $5,$0
move $6,$0
jal listnodes  # int
lw $25,nodelist
move $24,$25
beq $24,$0,L.203
lw $24,($25)
bne $24,600,L.203
lw $25,8($25)
beq $25,$0,L.203
lw $25,$fp11+8($sp)
addu $4,$25,1
jal findlabel  # int
lw $25,nodelist
lw $4,8($25)
move $5,$2
jal equatelab  # int
lw $4,nodelist
jal remove  # int
L.203:
move $4,$0
jal trash  # int
L.200:
lw $25,$fp11-16($sp)
move $24,$25
beq $24,$0,L.205
lw $25,12($25)
beq $25,$0,L.205
lw $25,$fp11+8($sp)
addu $4,$25,1
jal jump  # int
move $4,$2
jal list  # int
lw $4,$fp11+8($sp)
jal labelnode  # int
lw $25,$fp11-16($sp)
lw $4,12($25)
move $5,$0
move $6,$0
jal listnodes  # int
lw $25,nodelist
move $24,$25
beq $24,$0,L.206
lw $24,($25)
bne $24,600,L.206
lw $25,8($25)
beq $25,$0,L.206
lw $25,$fp11+8($sp)
addu $4,$25,1
jal findlabel  # int
lw $25,nodelist
lw $4,8($25)
move $5,$2
jal equatelab  # int
lw $4,nodelist
jal remove  # int
b L.206
L.205:
lw $4,$fp11+8($sp)
jal labelnode  # int
L.206:
lw $25,$fp11+8($sp)
addu $4,$25,1
jal labelnode  # int
move $23,$2
move $4,$0
jal trash  # int
lw $25,24($30)
beq $25,$0,L.186
lw $4,24($30)
jal idnode  # int
move $25,$2
sw $25,$fp11-20($sp)
lw $25,24($30)
l.s $f18,L.148
s.s $f18,36($25)
lw $4,$fp11-20($sp)
move $5,$0
move $6,$0
jal listnodes  # int
move $23,$2
b L.186
L.211:
lw $25,4($30)
lw $25,($25)
blt $25,15,L.213
lw $25,4($30)
lw $21,12($25)
b L.214
L.213:
lw $21,4($30)
L.214:
sw $21,$fp11-16($sp)
lw $25,$fp11-4($sp)
beq $25,20,L.215
lw $25,$fp11-16($sp)
lw $25,16($25)
bne $25,$0,L.215
la $4,L.37
la $5,L.38
li $6,332
jal fatal  # int
L.215:
lw $25,$fp11-4($sp)
beq $25,20,L.218
lw $25,$fp11-16($sp)
lw $25,16($25)
lw $25,32($25)
and $25,$25,0x8000000
beq $25,$0,L.216
L.218:
move $4,$30
jal cvtconst  # int
move $4,$2
lw $5,$fp11+4($sp)
lw $6,$fp11+8($sp)
jal listnodes  # int
move $23,$2
b L.186
L.216:
lw $25,$fp11+4($sp)
bne $25,$0,L.219
lw $25,$fp11+8($sp)
bne $25,$0,L.219
lw $4,$fp11-16($sp)
la $25,24($30)
lw $2,0($25)
lw $3,4($25)
sw $2,8($sp)
sw $3,12($sp)
lw $6,8($sp)
lw $7,12($sp)
jal constant  # int
lw $4,($30)
move $5,$0
move $6,$0
move $7,$2
jal node  # int
move $23,$2
b L.186
L.219:
lw $25,$fp11-16($sp)
lw $24,inttype
beq $25,$24,L.221
la $4,L.37
la $5,L.38
li $6,338
jal fatal  # int
L.221:
lw $25,$fp11+4($sp)
beq $25,$0,L.222
lw $25,24($30)
beq $25,$0,L.222
lw $4,$fp11+4($sp)
jal jump  # int
move $4,$2
jal list  # int
move $23,$2
b L.186
L.222:
lw $25,$fp11+8($sp)
beq $25,$0,L.224
lw $25,24($30)
bne $25,$0,L.224
lw $4,$fp11+8($sp)
jal jump  # int
move $4,$2
jal list  # int
move $23,$2
b L.186
L.224:
move $23,$0
b L.186
L.226:
lw $25,8($30)
move $24,$25
beq $24,$0,L.227
lw $24,($25)
and $24,$24,0xfffffff0
bne $24,64,L.227
lw $24,12($30)
move $15,$24
beq $15,$0,L.227
lw $15,($24)
and $15,$15,0xfffffff0
bne $15,48,L.227
lw $25,8($25)
lw $24,8($24)
bne $25,$24,L.227
lw $4,8($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $23,$2
lw $25,nodelist
beq $25,$0,L.229
lw $25,nodelist
sw $25,$fp11-16($sp)
L.231:
move $4,$23
lw $25,$fp11-16($sp)
lw $5,48($25)
jal haskid  # int
beq $2,$0,L.235
lw $25,$fp11-16($sp)
lw $25,48($25)
sw $25,48($23)
lw $25,$fp11-16($sp)
sw $23,48($25)
b L.230
L.235:
lw $25,$fp11-16($sp)
lw $25,48($25)
lw $24,nodelist
bne $25,$24,L.237
move $4,$23
jal list  # int
b L.230
L.237:
L.232:
lw $25,$fp11-16($sp)
lw $25,48($25)
sw $25,$fp11-16($sp)
b L.231
L.229:
move $4,$23
jal list  # int
L.230:
lw $4,12($30)
move $5,$0
move $6,$0
jal listnodes  # int
b L.186
L.227:
lw $25,12($30)
beq $25,$0,L.239
lw $25,8($30)
move $24,$25
beq $24,$0,L.241
lw $25,($25)
and $25,$25,0xfffffff0
bne $25,16,L.241
sw $0,8($30)
L.241:
lw $4,8($30)
move $5,$0
move $6,$0
jal listnodes  # int
lw $4,12($30)
lw $5,$fp11+4($sp)
lw $6,$fp11+8($sp)
jal listnodes  # int
move $23,$2
b L.186
L.239:
lw $4,8($30)
lw $5,$fp11+4($sp)
lw $6,$fp11+8($sp)
jal listnodes  # int
move $23,$2
b L.186
L.243:
lw $25,$fp11+4($sp)
bne $25,$0,L.245
lw $25,$fp11+8($sp)
beq $25,$0,L.244
L.245:
la $4,L.37
la $5,L.38
li $6,376
jal fatal  # int
L.244:
lw $25,24($30)
beq $25,$0,L.246
la $4,L.37
la $5,L.38
li $6,377
jal fatal  # int
L.246:
lw $25,8($30)
bne $25,$0,L.247
la $4,L.37
la $5,L.38
li $6,378
jal fatal  # int
L.247:
lw $4,8($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-8($sp)
li $4,584
lw $5,$fp11-8($sp)
move $6,$0
move $7,$0
jal newnode  # int
move $23,$2
move $4,$0
jal trash  # int
move $4,$23
jal list  # int
b L.186
L.248:
lw $25,$fp11+4($sp)
bne $25,$0,L.250
lw $25,$fp11+8($sp)
beq $25,$0,L.249
L.250:
la $4,L.37
la $5,L.38
li $6,385
jal fatal  # int
L.249:
lw $4,8($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-8($sp)
lw $4,12($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-12($sp)
lw $4,($30)
lw $5,$fp11-8($sp)
lw $6,$fp11-12($sp)
move $7,$0
jal newnode  # int
move $23,$2
lw $25,transient
li $24,104
sw $24,($25)
lw $25,transient
lw $24,($25)
lw $15,4($25)
addu $24,$24,$15
lw $25,8($25)
bltu $24,$25,L.252
lw $25,transient
lw $4,($25)
la $5,transient
jal allocate  # int
move $21,$2
b L.253
L.252:
lw $25,transient
lw $24,($25)
lw $15,4($25)
addu $24,$24,$15
sw $24,4($25)
lw $25,transient
lw $24,4($25)
lw $25,($25)
subu $21,$24,$25
L.253:
sw $21,8($23)
lw $21,8($23)
addu $25,$21,96
sw $25,$fp11-16($sp)
b L.257
L.254:
sw $0,28($21)
sw $0,24($21)
sw $0,20($21)
sw $0,16($21)
sw $0,12($21)
sw $0,8($21)
sw $0,4($21)
sw $0,($21)
L.255:
addu $21,$21,32
L.257:
lw $25,$fp11-16($sp)
bltu $21,$25,L.254
sw $0,($21)
sw $0,4($21)
lw $25,8($30)
lw $25,4($25)
lw $25,($25)
blt $25,15,L.262
lw $25,8($30)
lw $25,4($25)
lw $21,12($25)
b L.263
L.262:
lw $25,8($30)
lw $21,4($25)
L.263:
lw $25,($21)
bne $25,7,L.258
lw $25,8($30)
lw $25,4($25)
lw $25,12($25)
lw $25,($25)
blt $25,15,L.264
lw $25,8($30)
lw $25,4($25)
lw $25,12($25)
lw $20,12($25)
b L.265
L.264:
lw $25,8($30)
lw $25,4($25)
lw $20,12($25)
L.265:
lw $25,($20)
bne $25,11,L.258
lw $25,8($23)
lw $24,8($30)
lw $24,4($24)
lw $24,12($24)
sw $24,40($25)
b L.259
L.258:
lw $4,voidtype
move $5,$0
li $6,1
jal func  # int
lw $25,8($23)
move $24,$2
sw $24,40($25)
L.259:
move $4,$0
jal trash  # int
move $4,$23
jal list  # int
lw $25,cfunc
lw $24,92($25)
addu $24,$24,1
sw $24,92($25)
b L.186
L.266:
lw $25,$fp11+4($sp)
bne $25,$0,L.268
lw $25,$fp11+8($sp)
beq $25,$0,L.267
L.268:
la $4,L.37
la $5,L.38
li $6,400
jal fatal  # int
L.267:
lw $25,IR
lw $25,28($25)
and $25,$25,0x80000000
beq $25,$0,L.269
lw $4,12($30)
move $5,$0
move $6,$0
jal listnodes  # int
L.269:
lw $4,8($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-8($sp)
lw $4,($30)
lw $5,$fp11-8($sp)
move $6,$0
move $7,$0
jal newnode  # int
move $23,$2
lw $25,4($30)
lw $4,8($25)
jal intconst  # int
move $25,$2
sw $25,8($23)
lw $25,4($30)
lh $4,4($25)
jal intconst  # int
move $25,$2
sw $25,12($23)
move $4,$23
jal list  # int
lw $25,IR
lw $25,28($25)
and $25,$25,0x80000000
bne $25,$0,L.271
lw $4,12($30)
move $5,$0
move $6,$0
jal listnodes  # int
L.271:
move $2,$0
b L.180
L.273:
lw $25,24($30)
beq $25,$0,L.274
la $4,L.37
la $5,L.38
li $6,413
jal fatal  # int
L.274:
lw $25,errcnt
bne $25,$0,L.275
lw $25,$fp11+4($sp)
bne $25,$0,L.275
lw $25,$fp11+8($sp)
bne $25,$0,L.275
la $4,L.37
la $5,L.38
li $6,414
jal fatal  # int
L.275:
lw $25,$fp11+8($sp)
sw $25,$fp11-16($sp)
beq $25,$0,L.276
lw $25,$fp11+4($sp)
beq $25,$0,L.278
la $4,L.37
la $5,L.38
li $6,416
jal fatal  # int
L.278:
lw $21,$fp11-4($sp)
beq $21,512,L.284
beq $21,528,L.287
bgt $21,528,L.288
beq $21,480,L.282
beq $21,496,L.286
b L.277
L.288:
beq $21,544,L.285
beq $21,560,L.283
b L.277
L.282:
lw $25,($30)
and $25,$25,15
addu $25,$25,560
sw $25,$fp11-4($sp)
b L.277
L.283:
lw $25,($30)
and $25,$25,15
addu $25,$25,480
sw $25,$fp11-4($sp)
b L.277
L.284:
lw $25,($30)
and $25,$25,15
addu $25,$25,528
sw $25,$fp11-4($sp)
b L.277
L.285:
lw $25,($30)
and $25,$25,15
addu $25,$25,496
sw $25,$fp11-4($sp)
b L.277
L.286:
lw $25,($30)
and $25,$25,15
addu $25,$25,544
sw $25,$fp11-4($sp)
b L.277
L.287:
lw $25,($30)
and $25,$25,15
addu $25,$25,512
sw $25,$fp11-4($sp)
b L.277
L.276:
lw $25,$fp11+4($sp)
sw $25,$fp11-16($sp)
beq $25,$0,L.289
lw $25,($30)
sw $25,$fp11-4($sp)
L.289:
L.277:
lw $4,8($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-8($sp)
lw $4,12($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-12($sp)
lw $4,$fp11-16($sp)
jal findlabel  # int
lw $4,$fp11-4($sp)
lw $5,$fp11-8($sp)
lw $6,$fp11-12($sp)
move $7,$2
jal newnode  # int
move $23,$2
lw $25,8($23)
l.s $f18,36($25)
l.s $f16,L.291
add.s $f18,$f18,$f16
s.s $f18,36($25)
move $4,$23
jal list  # int
b L.186
L.292:
lw $25,$fp11+4($sp)
bne $25,$0,L.294
lw $25,$fp11+8($sp)
beq $25,$0,L.293
L.294:
la $4,L.37
la $5,L.38
li $6,435
jal fatal  # int
L.293:
lw $25,8($30)
lw $25,($25)
bne $25,688,L.295
lw $25,8($30)
lw $25,8($25)
sw $25,$fp11-20($sp)
lw $25,8($30)
lw $25,24($25)
sw $25,$fp11-16($sp)
lw $25,$fp11-20($sp)
lw $25,($25)
and $25,$25,0xfffffff0
beq $25,64,L.297
la $4,L.37
la $5,L.38
li $6,439
jal fatal  # int
L.297:
move $4,$0
jal trash  # int
lw $4,$fp11-20($sp)
jal lvalue  # int
move $4,$2
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-8($sp)
lw $25,$fp11-16($sp)
lh $24,12($25)
lw $25,4($25)
lw $25,8($25)
sll $25,$25,3
bge $24,$25,L.298
li $25,0xffffffff
lw $24,$fp11-16($sp)
lh $24,12($24)
sll $25,$25,$24
not $25,$25
sw $25,$fp11-28($sp)
lw $25,$fp11-28($sp)
lw $24,$fp11-16($sp)
lh $24,14($24)
subu $24,$24,1
sll $25,$25,$24
sw $25,$fp11-32($sp)
lw $25,12($30)
sw $25,$fp11-24($sp)
lw $25,$fp11-24($sp)
lw $24,($25)
bne $24,21,L.303
lw $25,24($25)
beq $25,$0,L.302
L.303:
lw $25,$fp11-24($sp)
lw $24,($25)
bne $24,22,L.300
lw $25,24($25)
bne $25,$0,L.300
L.302:
lw $25,$fp11-32($sp)
not $4,$25
lw $5,unsignedtype
jal constnode  # int
li $4,384
lw $5,$fp11-20($sp)
move $6,$2
jal bitnode  # int
move $25,$2
sw $25,$fp11-24($sp)
b L.301
L.300:
lw $25,$fp11-24($sp)
lw $24,($25)
bne $24,21,L.307
lw $25,24($25)
lw $24,$fp11-28($sp)
and $25,$25,$24
beq $25,$24,L.306
L.307:
lw $25,$fp11-24($sp)
lw $24,($25)
bne $24,22,L.304
lw $25,24($25)
lw $24,$fp11-28($sp)
and $25,$25,$24
bne $25,$24,L.304
L.306:
lw $4,$fp11-32($sp)
lw $5,unsignedtype
jal constnode  # int
li $4,416
lw $5,$fp11-20($sp)
move $6,$2
jal bitnode  # int
move $25,$2
sw $25,$fp11-24($sp)
b L.305
L.304:
lw $25,$fp11-32($sp)
not $4,$25
lw $5,unsignedtype
jal constnode  # int
li $4,384
lw $5,$fp11-20($sp)
move $6,$2
jal bitnode  # int
move $21,$2
lw $4,$fp11-24($sp)
lw $5,unsignedtype
jal cast  # int
move $20,$2
lw $25,$fp11-16($sp)
lh $25,14($25)
subu $4,$25,1
lw $5,inttype
jal constnode  # int
li $4,336
move $5,$20
move $6,$2
jal shnode  # int
move $19,$2
lw $4,$fp11-32($sp)
lw $5,unsignedtype
jal constnode  # int
li $4,384
move $5,$19
move $6,$2
jal bitnode  # int
li $4,416
move $5,$21
move $6,$2
jal bitnode  # int
move $25,$2
sw $25,$fp11-24($sp)
L.305:
L.301:
lw $4,$fp11-24($sp)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-12($sp)
b L.296
L.298:
lw $4,12($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-12($sp)
b L.296
L.295:
lw $4,8($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-8($sp)
lw $4,12($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-12($sp)
L.296:
lw $25,8($30)
lw $25,($25)
beq $25,263,L.312
beq $25,295,L.312
bne $25,279,L.309
L.312:
lw $25,8($30)
lw $25,24($25)
lw $25,32($25)
and $25,$25,0x10000000
bne $25,$0,L.309
lw $21,$fp11-8($sp)
b L.310
L.309:
move $21,$0
L.310:
move $4,$21
jal trash  # int
lw $4,($30)
lw $5,$fp11-8($sp)
lw $6,$fp11-12($sp)
move $7,$0
jal newnode  # int
move $23,$2
lw $25,12($30)
lw $25,4($25)
lw $4,8($25)
jal intconst  # int
move $25,$2
sw $25,8($23)
lw $25,12($30)
lw $25,4($25)
lh $4,4($25)
jal intconst  # int
move $25,$2
sw $25,12($23)
move $4,$23
jal list  # int
lw $4,12($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $23,$2
b L.186
L.313:
lw $25,$fp11+4($sp)
bne $25,$0,L.315
lw $25,$fp11+8($sp)
beq $25,$0,L.314
L.315:
la $4,L.37
la $5,L.38
li $6,473
jal fatal  # int
L.314:
lw $4,8($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-8($sp)
lw $25,IR
lw $25,28($25)
and $25,$25,0x8000000
beq $25,$0,L.316
lw $4,12($30)
lw $5,unsignedtype
jal cast  # int
li $4,406
lw $5,unsignedtype
move $6,$2
move $7,$0
jal simplify  # int
move $4,$2
lw $25,12($30)
lw $5,4($25)
jal cast  # int
move $4,$2
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-12($sp)
b L.317
L.316:
lw $4,12($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-12($sp)
L.317:
lw $4,($30)
lw $5,$fp11-8($sp)
lw $6,$fp11-12($sp)
move $7,$0
jal node  # int
move $23,$2
b L.186
L.318:
lw $25,$fp11+4($sp)
bne $25,$0,L.320
lw $25,$fp11+8($sp)
beq $25,$0,L.319
L.320:
la $4,L.37
la $5,L.38
li $6,485
jal fatal  # int
L.319:
lw $4,8($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-8($sp)
lw $25,IR
lw $25,28($25)
and $25,$25,0x8000000
beq $25,$0,L.321
lw $25,($30)
bne $25,373,L.321
li $4,197
lw $5,inttype
lw $6,12($30)
move $7,$0
jal simplify  # int
move $4,$2
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-12($sp)
b L.322
L.321:
lw $4,12($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-12($sp)
L.322:
lw $4,($30)
lw $5,$fp11-8($sp)
lw $6,$fp11-12($sp)
move $7,$0
jal node  # int
move $23,$2
b L.186
L.323:
lw $25,$fp11+4($sp)
bne $25,$0,L.325
lw $25,$fp11+8($sp)
beq $25,$0,L.324
L.325:
la $4,L.37
la $5,L.38
li $6,495
jal fatal  # int
L.324:
lw $4,8($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-8($sp)
lw $4,12($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-12($sp)
lw $4,($30)
lw $5,$fp11-8($sp)
lw $6,$fp11-12($sp)
move $7,$0
jal node  # int
move $23,$2
lw $25,IR
lw $25,28($25)
and $25,$25,0x10000000
beq $25,$0,L.186
lw $25,($23)
beq $25,453,L.332
beq $25,357,L.332
beq $25,469,L.332
beq $25,454,L.332
beq $25,358,L.332
bne $25,470,L.186
L.332:
move $4,$23
jal list  # int
b L.186
L.333:
lw $25,$fp11+4($sp)
bne $25,$0,L.335
lw $25,$fp11+8($sp)
beq $25,$0,L.334
L.335:
la $4,L.37
la $5,L.38
li $6,505
jal fatal  # int
L.334:
lw $4,8($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-8($sp)
lw $4,($30)
lw $5,$fp11-8($sp)
move $6,$0
move $7,$0
jal newnode  # int
move $23,$2
move $4,$23
jal list  # int
b L.186
L.336:
lw $25,$fp11+4($sp)
bne $25,$0,L.338
lw $25,$fp11+8($sp)
beq $25,$0,L.337
L.338:
la $4,L.37
la $5,L.38
li $6,512
jal fatal  # int
L.337:
lw $4,8($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-8($sp)
lw $4,($30)
lw $5,$fp11-8($sp)
move $6,$0
move $7,$0
jal node  # int
move $23,$2
b L.186
L.339:
lw $25,8($30)
lw $25,4($25)
sw $25,$fp11-16($sp)
lw $25,$fp11-16($sp)
lw $25,($25)
blt $25,15,L.343
lw $25,$fp11-16($sp)
lw $21,12($25)
b L.344
L.343:
lw $21,$fp11-16($sp)
L.344:
lw $25,($21)
bne $25,7,L.340
lw $25,$fp11-16($sp)
lw $25,($25)
blt $25,15,L.346
lw $25,$fp11-16($sp)
lw $21,12($25)
b L.347
L.346:
lw $21,$fp11-16($sp)
L.347:
lw $25,12($21)
sw $25,$fp11-16($sp)
L.340:
lw $25,$fp11+4($sp)
bne $25,$0,L.349
lw $25,$fp11+8($sp)
beq $25,$0,L.348
L.349:
la $4,L.37
la $5,L.38
li $6,520
jal fatal  # int
L.348:
lw $4,8($30)
move $5,$0
move $6,$0
jal listnodes  # int
move $25,$2
sw $25,$fp11-8($sp)
lw $25,$fp11-16($sp)
lw $25,($25)
beq $25,16,L.355
beq $25,31,L.355
lw $25,$fp11-16($sp)
lw $25,($25)
blt $25,15,L.357
lw $25,$fp11-16($sp)
lw $21,12($25)
b L.358
L.357:
lw $21,$fp11-16($sp)
L.358:
lw $25,($21)
beq $25,9,L.356
lw $25,$fp11-16($sp)
lw $25,($25)
blt $25,15,L.359
lw $25,$fp11-16($sp)
lw $20,12($25)
b L.360
L.359:
lw $20,$fp11-16($sp)
L.360:
lw $25,($20)
bne $25,10,L.350
L.356:
lw $25,$fp11-16($sp)
lw $25,($25)
blt $25,15,L.361
lw $25,$fp11-16($sp)
lw $19,12($25)
b L.362
L.361:
lw $19,$fp11-16($sp)
L.362:
lw $25,16($19)
lw $25,64($25)
and $25,$25,0x40000000
beq $25,$0,L.350
L.355:
lw $4,($30)
lw $5,$fp11-8($sp)
move $6,$0
move $7,$0
jal newnode  # int
move $23,$2
b L.186
L.350:
lw $4,($30)
lw $5,$fp11-8($sp)
move $6,$0
move $7,$0
jal node  # int
move $23,$2
b L.186
L.363:
lw $25,$fp11+4($sp)
bne $25,$0,L.365
lw $25,$fp11+8($sp)
beq $25,$0,L.364
L.365:
la $4,L.37
la $5,L.38
li $6,530
jal fatal  # int
L.364:
lw $25,24($30)
lw $24,4($25)
lw $24,8($24)
sll $24,$24,3
lh $15,12($25)
subu $24,$24,$15
lh $25,14($25)
subu $25,$25,1
subu $4,$24,$25
lw $5,inttype
jal constnode  # int
li $4,336
lw $5,8($30)
move $6,$2
jal shnode  # int
move $21,$2
lw $25,4($30)
lw $25,8($25)
sll $25,$25,3
lw $24,24($30)
lh $24,12($24)
subu $4,$25,$24
lw $5,inttype
jal constnode  # int
li $4,368
move $5,$21
move $6,$2
jal shnode  # int
move $25,$2
sw $25,$fp11-16($sp)
lw $4,$fp11-16($sp)
move $5,$0
move $6,$0
jal listnodes  # int
move $23,$2
b L.186
L.366:
lw $25,$fp11+4($sp)
bne $25,$0,L.368
lw $25,$fp11+8($sp)
beq $25,$0,L.367
L.368:
la $4,L.37
la $5,L.38
li $6,539
jal fatal  # int
L.367:
lw $25,24($30)
lw $25,32($25)
and $25,$25,0x40000000
beq $25,$0,L.369
lw $4,24($30)
jal addlocal  # int
lw $4,24($30)
jal release  # int
L.369:
lw $4,($30)
move $5,$0
move $6,$0
lw $7,24($30)
jal node  # int
move $23,$2
b L.186
L.371:
lw $25,$fp11+4($sp)
bne $25,$0,L.373
lw $25,$fp11+8($sp)
beq $25,$0,L.372
L.373:
la $4,L.37
la $5,L.38
li $6,547
jal fatal  # int
L.372:
lw $25,24($30)
lhu $25,28($25)
bne $25,2,L.374
lw $25,24($30)
l.s $f18,36($25)
l.s $f16,L.291
add.s $f18,$f18,$f16
s.s $f18,36($25)
L.374:
lw $4,($30)
move $5,$0
move $6,$0
lw $7,24($30)
jal node  # int
move $23,$2
b L.186
L.185:
la $4,L.37
la $5,L.38
li $6,552
jal fatal  # int
L.186:
sw $23,16($30)
move $2,$23
L.180:
lw $19,16($sp)
lw $20,20($sp)
lw $21,24($sp)
lw $22,28($sp)
lw $23,32($sp)
lw $30,36($sp)
lw $31,40($sp)
addu $sp,$sp,80
j $31
.end listnodes
.globl jump
.text
.align 2; .ent jump
jump:
$fp12=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0xc0000000,-4
sw $30,16($sp)
sw $31,20($sp)
sw $4,24($sp)
lw $4,$fp12+0($sp)
jal findlabel  # int
move $30,$2
l.s $f18,36($30)
l.s $f16,L.291
add.s $f18,$f18,$f16
s.s $f18,36($30)
li $4,263
move $5,$0
move $6,$0
move $7,$30
jal node  # int
li $4,584
move $5,$2
move $6,$0
move $7,$0
jal newnode  # int
L.387:
lw $30,16($sp)
lw $31,20($sp)
addu $sp,$sp,24
j $31
.end jump
.globl newnode
.text
.align 2; .ent newnode
newnode:
$fp13=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0x80000000,-8
sw $31,16($sp)
sw $4,24($sp)
sw $5,28($sp)
sw $6,32($sp)
sw $7,36($sp)
lw $4,$fp13+0($sp)
lw $5,$fp13+4($sp)
lw $6,$fp13+8($sp)
lw $7,$fp13+12($sp)
jal dagnode  # int
L.388:
lw $31,16($sp)
addu $sp,$sp,24
j $31
.end newnode
.globl node
.text
.align 2; .ent node
node:
$fp14=32
.frame $sp,32,$31
addu $sp,$sp,-32
.mask 0xc0800000,-8
sw $23,16($sp)
sw $30,20($sp)
sw $31,24($sp)
sw $4,32($sp)
sw $5,36($sp)
sw $6,40($sp)
sw $7,44($sp)
lw $25,$fp14+0($sp)
sra $25,$25,4
lw $24,$fp14+12($sp)
srl $24,$24,2
xor $25,$25,$24
and $23,$25,15
sll $25,$23,2
lw $30,buckets($25)
b L.393
L.390:
lw $25,($30)
lw $24,$fp14+0($sp)
bne $25,$24,L.394
lw $25,8($30)
lw $24,$fp14+12($sp)
bne $25,$24,L.394
lw $25,24($30)
lw $24,$fp14+4($sp)
bne $25,$24,L.394
lw $25,28($30)
lw $24,$fp14+8($sp)
bne $25,$24,L.394
move $2,$30
b L.389
L.394:
L.391:
lw $30,80($30)
L.393:
bne $30,$0,L.390
lw $4,$fp14+0($sp)
lw $5,$fp14+4($sp)
lw $6,$fp14+8($sp)
lw $7,$fp14+12($sp)
jal dagnode  # int
move $30,$2
sll $25,$23,2
lw $25,buckets($25)
sw $25,80($30)
sll $25,$23,2
sw $30,buckets($25)
lw $25,nodecount
addu $25,$25,1
sw $25,nodecount
move $2,$30
L.389:
lw $23,16($sp)
lw $30,20($sp)
lw $31,24($sp)
addu $sp,$sp,32
j $31
.end node
.globl printdag
.text
.align 2; .ent printdag
printdag:
$fp15=32
.frame $sp,32,$31
addu $sp,$sp,-32
.mask 0xc0800000,-8
sw $23,16($sp)
sw $30,20($sp)
sw $31,24($sp)
move $30,$4
move $23,$5
move $4,$0
jal printed  # int
bne $30,$0,L.397
lw $25,nodelist
move $30,$25
beq $25,$0,L.398
L.401:
lw $30,48($30)
move $4,$30
move $5,$23
move $6,$0
jal printdag1  # int
L.402:
lw $25,nodelist
bne $30,$25,L.401
b L.398
L.397:
move $4,$30
jal nodeid  # int
move $4,$2
jal printed  # int
move $25,$2
lw $25,($25)
beq $25,$0,L.404
move $4,$30
jal nodeid  # int
move $4,$23
la $5,L.406
move $6,$2
jal fprint  # int
b L.405
L.404:
move $4,$30
move $5,$23
move $6,$0
jal printdag1  # int
L.405:
L.398:
L.396:
lw $23,16($sp)
lw $30,20($sp)
lw $31,24($sp)
addu $sp,$sp,32
j $31
.end printdag
.text
.align 2; .ent printdag1
printdag1:
$fp16=40
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
beq $30,$0,L.410
move $4,$30
jal nodeid  # int
sw $2,$fp16-4($sp)
move $4,$2
jal printed  # int
move $25,$2
lw $25,($25)
beq $25,$0,L.408
L.410:
b L.407
L.408:
lw $4,$fp16-4($sp)
jal printed  # int
move $25,$2
li $24,1
sw $24,($25)
move $21,$0
L.411:
sll $25,$21,2
la $24,24($30)
addu $25,$25,$24
lw $4,($25)
move $5,$23
addu $6,$22,1
jal printdag1  # int
L.412:
addu $21,$21,1
blt $21,6,L.411
move $4,$30
move $5,$23
move $6,$22
jal printnode  # int
L.407:
lw $21,16($sp)
lw $22,20($sp)
lw $23,24($sp)
lw $30,28($sp)
lw $31,32($sp)
addu $sp,$sp,40
j $31
.end printdag1
.text
.align 2; .ent printnode
printnode:
$fp17=56
.frame $sp,56,$31
addu $sp,$sp,-56
.mask 0xc0f80000,-12
sw $19,20($sp)
sw $20,24($sp)
sw $21,28($sp)
sw $22,32($sp)
sw $23,36($sp)
sw $30,40($sp)
sw $31,44($sp)
move $30,$4
move $23,$5
sw $6,64($sp)
beq $30,$0,L.416
move $4,$30
jal nodeid  # int
sw $2,$fp17-4($sp)
move $4,$23
la $5,L.418
lw $25,$fp17+8($sp)
bne $25,$0,L.423
li $21,39
b L.424
L.423:
li $21,35
L.424:
move $6,$21
lw $7,$fp17-4($sp)
lw $25,$fp17-4($sp)
bge $25,10,L.425
move $20,$0
b L.426
L.425:
lw $25,$fp17-4($sp)
bge $25,100,L.427
li $19,1
b L.428
L.427:
li $19,2
L.428:
move $20,$19
L.426:
la $25,L.420($20)
sw $25,16($sp)
jal fprint  # int
lw $4,($30)
jal opname  # int
move $4,$23
la $5,L.429
move $6,$2
lh $7,4($30)
jal fprint  # int
move $22,$0
b L.433
L.430:
sll $25,$22,2
la $24,24($30)
addu $25,$25,$24
lw $4,($25)
jal nodeid  # int
move $4,$23
la $5,L.434
move $6,$2
jal fprint  # int
L.431:
addu $22,$22,1
L.433:
bge $22,6,L.435
sll $25,$22,2
la $24,24($30)
addu $25,$25,$24
lw $25,($25)
bne $25,$0,L.430
L.435:
move $22,$0
b L.439
L.436:
move $4,$23
la $5,L.440
sll $25,$22,2
la $24,8($30)
addu $25,$25,$24
lw $25,($25)
lw $6,24($25)
jal fprint  # int
L.437:
addu $22,$22,1
L.439:
bge $22,4,L.441
sll $25,$22,2
la $24,8($30)
addu $25,$25,$24
lw $25,($25)
bne $25,$0,L.436
L.441:
move $4,$23
la $5,L.442
jal fprint  # int
L.416:
L.415:
lw $19,20($sp)
lw $20,24($sp)
lw $21,28($sp)
lw $22,32($sp)
lw $23,36($sp)
lw $30,40($sp)
lw $31,44($sp)
addu $sp,$sp,56
j $31
.end printnode
.text
.align 2; .ent remove
remove:
$fp18=32
.frame $sp,32,$31
addu $sp,$sp,-32
.mask 0xc0800000,-8
sw $23,16($sp)
sw $30,20($sp)
sw $31,24($sp)
move $30,$4
lw $25,nodelist
beq $25,$0,L.444
lw $23,nodelist
b L.449
L.446:
L.447:
lw $23,48($23)
L.449:
lw $25,48($23)
beq $25,$30,L.450
lw $24,nodelist
bne $25,$24,L.446
L.450:
lw $25,48($23)
beq $25,$30,L.451
la $4,L.37
la $5,L.38
li $6,637
jal fatal  # int
L.451:
lw $25,48($30)
sw $25,48($23)
lw $25,nodelist
bne $30,$25,L.452
sw $23,nodelist
L.452:
L.444:
L.443:
lw $23,16($sp)
lw $30,20($sp)
lw $31,24($sp)
addu $sp,$sp,32
j $31
.end remove
.text
.align 2; .ent reset
reset:
$fp19=0
.frame $sp,0,$31
la $24,buckets
addu $25,$24,64
b L.458
L.455:
sw $0,28($24)
sw $0,24($24)
sw $0,20($24)
sw $0,16($24)
sw $0,12($24)
sw $0,8($24)
sw $0,4($24)
sw $0,($24)
L.456:
addu $24,$24,32
L.458:
bltu $24,$25,L.455
sw $0,nodecount
L.454:
j $31
.end reset
.text
.align 2; .ent trash
trash:
$fp20=16
.frame $sp,16,$31
addu $sp,$sp,-16
.mask 0xc0c00000,-4
sw $22,0($sp)
sw $23,4($sp)
sw $30,8($sp)
sw $31,12($sp)
sw $4,16($sp)
lw $25,$fp20+0($sp)
beq $25,$0,L.460
move $30,$0
L.462:
sll $25,$30,2
la $22,buckets($25)
b L.469
L.466:
lw $25,($23)
and $25,$25,0xfffffff0
bne $25,64,L.470
lw $25,24($23)
lw $25,($25)
beq $25,263,L.474
beq $25,295,L.474
bne $25,279,L.472
L.474:
lw $25,24($23)
lw $25,8($25)
lw $24,$fp20+0($sp)
lw $24,8($24)
bne $25,$24,L.470
L.472:
lw $25,80($23)
sw $25,($22)
lw $25,nodecount
subu $25,$25,1
sw $25,nodecount
b L.471
L.470:
addu $22,$23,80
L.471:
L.467:
L.469:
lw $25,($22)
move $23,$25
bne $25,$0,L.466
L.463:
addu $30,$30,1
blt $30,16,L.462
b L.461
L.460:
lw $25,nodecount
ble $25,$0,L.475
jal reset  # int
L.475:
L.461:
L.459:
lw $22,0($sp)
lw $23,4($sp)
lw $30,8($sp)
lw $31,12($sp)
addu $sp,$sp,16
j $31
.end trash
.text
.align 2; .ent typestab
typestab:
$fp21=32
.frame $sp,32,$31
addu $sp,$sp,-32
.mask 0xc0800000,-8
sw $23,16($sp)
sw $30,20($sp)
sw $31,24($sp)
move $30,$4
sw $5,36($sp)
lw $25,40($30)
lw $25,($25)
blt $25,15,L.481
lw $25,40($30)
lw $23,12($25)
b L.482
L.481:
lw $23,40($30)
L.482:
lw $25,($23)
beq $25,11,L.478
lbu $25,30($30)
beq $25,74,L.483
bne $25,81,L.478
L.483:
lw $25,IR
lw $25,124($25)
beq $25,$0,L.478
move $4,$30
lw $25,IR
lw $25,124($25)
jal $25  # int
b L.479
L.478:
lbu $25,30($30)
beq $25,83,L.486
bne $25,$0,L.484
L.486:
lw $25,IR
lw $25,128($25)
beq $25,$0,L.484
move $4,$30
lw $25,IR
lw $25,128($25)
jal $25  # int
L.484:
L.479:
L.477:
lw $23,16($sp)
lw $30,20($sp)
lw $31,24($sp)
addu $sp,$sp,32
j $31
.end typestab
.text
.align 2; .ent undag
undag:
$fp22=112
.frame $sp,112,$31
addu $sp,$sp,-112
.mask 0xc0c00000,-84
sw $22,16($sp)
sw $23,20($sp)
sw $30,24($sp)
sw $31,28($sp)
sw $4,112($sp)
lw $25,$fp22+0($sp)
sw $25,$fp22-32($sp)
la $23,$fp22-80($sp)
b L.492
L.489:
lw $22,48($30)
sw $23,48($30)
lw $25,($30)
and $25,$25,0xfffffff0
bne $25,64,L.493
lh $25,4($30)
bge $25,1,L.495
la $4,L.37
la $5,L.38
li $6,695
jal fatal  # int
L.495:
move $4,$30
move $5,$30
jal undag1  # int
lw $23,48($30)
sw $22,48($23)
sw $0,48($30)
b L.494
L.493:
lw $25,($30)
move $24,$25
and $24,$24,0xfffffff0
beq $24,208,L.503
lw $24,IR
lw $24,28($24)
and $24,$24,0x10000000
beq $24,$0,L.496
beq $25,453,L.503
beq $25,357,L.503
beq $25,469,L.503
beq $25,454,L.503
beq $25,358,L.503
bne $25,470,L.496
L.503:
lh $25,4($30)
blt $25,1,L.496
move $4,$30
move $5,$30
jal undag1  # int
lw $25,48($30)
move $24,$25
beq $24,$0,L.506
lw $24,($25)
and $24,$24,0xfffffff0
bne $24,48,L.506
lw $25,28($25)
beq $25,$30,L.504
L.506:
la $4,L.37
la $5,L.38
li $6,711
jal fatal  # int
L.504:
lw $23,48($30)
sw $22,48($23)
sw $0,48($30)
b L.497
L.496:
lh $25,4($30)
beq $25,$0,L.507
la $4,L.37
la $5,L.38
li $6,730
jal fatal  # int
L.507:
move $4,$30
move $5,$30
jal undag1  # int
sw $22,48($30)
move $23,$30
L.497:
L.494:
L.490:
L.492:
lw $25,48($23)
move $30,$25
bne $25,$0,L.489
move $4,$0
li $5,5
jal rmtemps  # int
lw $2,$fp22-32($sp)
L.487:
lw $22,16($sp)
lw $23,20($sp)
lw $30,24($sp)
lw $31,28($sp)
addu $sp,$sp,112
j $31
.end undag
.text
.align 2; .ent undag1
undag1:
$fp23=40
.frame $sp,40,$31
addu $sp,$sp,-40
.mask 0xc0c00000,-12
sw $22,16($sp)
sw $23,20($sp)
sw $30,24($sp)
sw $31,28($sp)
move $30,$4
sw $5,44($sp)
bne $30,$0,L.510
b L.511
L.510:
lw $25,16($30)
beq $25,$0,L.512
lw $25,16($30)
lw $25,40($25)
lw $25,($25)
blt $25,15,L.518
lw $25,16($30)
lw $25,40($25)
lw $22,12($25)
b L.519
L.518:
lw $25,16($30)
lw $22,40($25)
L.519:
lw $25,($22)
bne $25,6,L.516
li $23,5
b L.517
L.516:
lw $25,16($30)
lw $4,40($25)
jal ttob  # int
move $23,$2
L.517:
li $4,295
move $5,$0
move $6,$0
lw $7,16($30)
jal newnode  # int
addu $4,$23,64
move $5,$2
move $6,$0
move $7,$0
jal newnode  # int
move $25,$2
sw $25,$fp23-4($sp)
lw $25,$fp23-4($sp)
li $24,1
sh $24,4($25)
lh $25,4($30)
subu $25,$25,1
sh $25,4($30)
sll $25,$25,16; sra $25,$25,16
bne $25,1,L.520
sw $0,16($30)
L.520:
lw $30,$fp23-4($sp)
b L.513
L.512:
lh $25,4($30)
bgt $25,1,L.531
lw $25,($30)
move $24,$25
and $24,$24,0xfffffff0
beq $24,208,L.531
lw $24,IR
lw $24,28($24)
and $24,$24,0x10000000
beq $24,$0,L.537
beq $25,453,L.531
beq $25,357,L.531
beq $25,469,L.531
beq $25,454,L.531
beq $25,358,L.531
bne $25,470,L.537
L.531:
lh $25,4($30)
bne $25,$0,L.522
lw $25,($30)
move $24,$25
and $24,$24,0xfffffff0
beq $24,208,L.537
lw $24,IR
lw $24,28($24)
and $24,$24,0x10000000
beq $24,$0,L.522
beq $25,453,L.537
beq $25,357,L.537
beq $25,469,L.537
beq $25,454,L.537
beq $25,358,L.537
bne $25,470,L.522
L.537:
lw $4,24($30)
lw $5,$fp23+4($sp)
jal undag1  # int
move $25,$2
sw $25,24($30)
lw $4,28($30)
lw $5,$fp23+4($sp)
jal undag1  # int
move $25,$2
sw $25,28($30)
b L.523
L.522:
lw $25,($30)
beq $25,295,L.540
bne $25,279,L.538
L.540:
lw $25,$fp23+4($sp)
bne $30,$25,L.541
la $4,L.37
la $5,L.38
li $6,765
jal fatal  # int
L.541:
lw $4,($30)
move $5,$0
move $6,$0
lw $7,8($30)
jal newnode  # int
move $30,$2
li $25,1
sh $25,4($30)
b L.539
L.538:
lw $25,($30)
and $25,$25,0xfffffff0
bne $25,64,L.542
lw $25,24($30)
lw $25,($25)
beq $25,295,L.544
bne $25,279,L.542
L.544:
lw $25,24($30)
lw $25,8($25)
lbu $25,30($25)
bne $25,78,L.542
lw $25,$fp23+4($sp)
beq $30,$25,L.542
lw $25,24($30)
lw $4,($25)
move $5,$0
move $6,$0
lw $7,8($25)
jal newnode  # int
lw $4,($30)
move $5,$2
move $6,$0
move $7,$0
jal newnode  # int
move $30,$2
li $25,1
sh $25,4($30)
b L.543
L.542:
lw $25,($30)
bne $25,73,L.545
lh $25,4($30)
subu $25,$25,1
sh $25,4($30)
lw $4,($30)
lw $5,24($30)
move $6,$0
move $7,$0
jal newnode  # int
move $30,$2
li $25,1
sh $25,4($30)
lw $4,24($30)
lw $5,$fp23+4($sp)
jal undag1  # int
move $25,$2
sw $25,24($30)
b L.546
L.545:
lw $4,($30)
jal btot  # int
li $4,78
move $5,$2
jal temporary  # int
move $25,$2
sw $25,16($30)
lw $25,16($30)
lw $25,32($25)
and $25,$25,0x80000000
bne $25,$0,L.547
lw $25,16($30)
li $24,5
sh $24,28($25)
lw $25,16($30)
l.s $f18,L.291
s.s $f18,36($25)
lw $4,16($30)
lw $25,IR
lw $25,84($25)
jal $25  # int
lw $25,16($30)
lw $24,32($25)
or $24,$24,0x80000000
sw $24,32($25)
L.547:
lw $4,24($30)
lw $5,$fp23+4($sp)
jal undag1  # int
move $25,$2
sw $25,24($30)
lw $4,28($30)
lw $5,$fp23+4($sp)
jal undag1  # int
move $25,$2
sw $25,28($30)
lw $25,16($30)
lw $25,40($25)
lw $25,($25)
blt $25,15,L.553
lw $25,16($30)
lw $25,40($25)
lw $22,12($25)
b L.554
L.553:
lw $25,16($30)
lw $22,40($25)
L.554:
lw $25,($22)
bne $25,6,L.551
li $23,5
b L.552
L.551:
lw $25,16($30)
lw $4,40($25)
jal ttob  # int
move $23,$2
L.552:
li $4,295
move $5,$0
move $6,$0
lw $7,16($30)
jal newnode  # int
addu $4,$23,48
move $5,$2
move $6,$30
move $7,$0
jal newnode  # int
move $25,$2
sw $25,$fp23-4($sp)
lw $25,16($30)
lw $25,40($25)
lw $4,8($25)
jal intconst  # int
lw $25,$fp23-4($sp)
move $24,$2
sw $24,8($25)
lw $25,16($30)
lw $25,40($25)
lh $4,4($25)
jal intconst  # int
lw $25,$fp23-4($sp)
move $24,$2
sw $24,12($25)
lw $25,$fp23+4($sp)
lw $24,48($25)
lw $15,$fp23-4($sp)
sw $15,48($24)
sw $15,48($25)
lw $25,$fp23-4($sp)
lw $24,$fp23+4($sp)
sw $24,48($25)
lw $25,$fp23+4($sp)
beq $30,$25,L.555
move $4,$30
lw $5,$fp23+4($sp)
jal undag1  # int
move $30,$2
L.555:
L.546:
L.543:
L.539:
L.523:
L.513:
L.511:
move $2,$30
L.509:
lw $22,16($sp)
lw $23,20($sp)
lw $30,24($sp)
lw $31,28($sp)
addu $sp,$sp,40
j $31
.end undag1
.globl walk
.text
.align 2; .ent walk
walk:
$fp24=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0x80000000,-8
sw $31,16($sp)
sw $4,24($sp)
sw $5,28($sp)
sw $6,32($sp)
lw $4,$fp24+0($sp)
lw $5,$fp24+4($sp)
lw $6,$fp24+8($sp)
jal listnodes  # int
lw $25,nodelist
beq $25,$0,L.558
move $4,$0
jal trash  # int
li $4,8
jal code  # int
lw $25,codelist
lw $24,nodelist
lw $24,48($24)
sw $24,12($25)
lw $25,nodelist
sw $0,48($25)
sw $0,nodelist
li $4,78
move $5,$0
jal rmtemps  # int
L.558:
jal reset  # int
sw $0,ntree
L.557:
lw $31,16($sp)
addu $sp,$sp,24
j $31
.end walk
.lcomm nodelist,4
.lcomm buckets,64
.extern voidtype 4
.extern voidptype 4
.extern unsignedtype 4
.extern unsignedshort 4
.extern unsignedlong 4
.extern unsignedchar 4
.extern signedchar 4
.extern shorttype 4
.extern longtype 4
.extern longdouble 4
.extern inttype 4
.extern floattype 4
.extern doubletype 4
.extern chartype 4
.extern transient 4
.extern permanent 4
.extern refinc 4
.extern density 4
.extern needconst 4
.extern opnode 0
.extern wflag 4
.extern errlimit 4
.extern errcnt 4
.extern bp 4
.extern events 24
.extern npoints 4
.extern ncalled 4
.extern xref 4
.extern glevel 4
.extern YYnull 4
.extern Pflag 4
.extern Aflag 4
.extern tsym 4
.extern token 4
.extern t 4
.extern src 8
.extern kind 0
.extern lineno 4
.extern line 4
.extern limit 4
.extern firstfile 4
.extern file 4
.extern cp 4
.extern ntree 4
.extern retv 4
.extern fname 4
.extern cfunc 4
.globl nodecount
.comm nodecount,4
.extern symbols 4
.extern level 4
.extern types 4
.extern labels 0
.extern identifiers 4
.extern globals 4
.extern externals 4
.extern constants 4
.extern bnumber 4
.extern IR 4
.extern interfaces 0
.extern genx 4
.extern salign 4
.extern dalign 4
.extern vmask 0
.extern usedmask 0
.extern tmask 0
.extern tempmask 0
.extern savemask 0
.extern savebase 0
.extern rmap 0
.extern regvars 4
.extern regs 4
.extern opcodes 4
.extern nregsets 4
.extern rflag 4
.extern pflag 4
.extern kflag 4
.extern dflag 4
.extern bflag 4
.extern offset 4
.extern ncalls 4
.extern maxoffset 4
.extern last 4
.extern freemask 0
.extern eemask 0
.extern asmstr 0
.extern argoffset 4
.extern argbuildsize 4
.rdata
L.442:.byte 10,0
L.440:.ascii " %s\000"
L.434:.ascii " #%d\000"
L.429:.ascii "%s count=%d\000"
L.420:.ascii "   \000"
L.418:.ascii "%c%d%s\000"
L.406:.ascii "node'%d printed above\012\000"
.align 2; L.291:.word 0x3f800000
.align 2; L.148:.word 0x0
L.51:.ascii "unreachable code\012\000"
L.38:.ascii "assertion failure at line %d\012\000"
L.37:.ascii "../c/dag.c\000"

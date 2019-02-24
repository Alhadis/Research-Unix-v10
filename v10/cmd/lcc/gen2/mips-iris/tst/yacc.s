.set reorder
.globl yyin
.sdata; .align 2; yyin:.word _iob
.globl yyout
.sdata; .align 2; yyout:.word _iob+16
.text
.globl yylex
.text
.align 2; .ent yylex
yylex:
$fp1=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0xc0000000,-4
sw $30,16($sp)
sw $31,20($sp)
b L.4
L.3:
L.6:
blt $30,-1,L.7
bgt $30,4,L.7
sll $25,$30,2
lw $25,L.19+4($25)
j $25
L.19:
.align 2; .word L.8
.align 2; .word L.9
.align 2; .word L.12
.align 2; .word L.13
.align 2; .word L.8
.align 2; .word L.15
L.9:
jal yywrap  # int
beq $2,$0,L.8
move $2,$0
b L.2
L.12:
li $2,257
b L.2
L.13:
li $2,258
b L.2
L.15:
lb $2,yytext
b L.2
L.7:
lw $4,yyout
la $5,L.17
move $6,$30
jal fprintf  # int
L.8:
L.4:
jal yylook  # int
move $30,$2
bge $2,$0,L.3
move $2,$0
L.2:
lw $30,16($sp)
lw $31,20($sp)
addu $sp,$sp,24
j $31
.end yylex
.globl yyvstop
.data; .align 2; yyvstop:.word 0x0
.word 0x4
.word 0x0
.word 0x3
.word 0x4
.word 0x0
.word 0x2
.word 0x4
.word 0x0
.word 0x1
.word 0x4
.word 0x0
.word 0x2
.word 0x0
.word 0x1
.word 0x0
.word 0x0
.globl yycrank
.data; yycrank:.byte 0
.byte 0
.byte 0
.byte 0
.byte 1
.byte 3
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 1
.byte 4
.byte 1
.byte 3
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 1
.byte 5
.byte 5
.byte 7
.byte 5
.byte 7
.byte 5
.byte 7
.byte 5
.byte 7
.byte 5
.byte 7
.byte 5
.byte 7
.byte 5
.byte 7
.byte 5
.byte 7
.byte 5
.byte 7
.byte 5
.byte 7
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 1
.byte 6
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 6
.byte 8
.byte 0
.byte 0
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 6
.byte 8
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.byte 0
.globl yysvec
.data; .align 2; yysvec:.word 0x0
.word 0x0
.word 0x0
.word yycrank-2
.word 0x0
.word 0x0
.word yycrank
.word yysvec+12
.word 0x0
.word yycrank
.word 0x0
.word yyvstop+4
.word yycrank
.word 0x0
.word yyvstop+12
.word yycrank+4
.word 0x0
.word yyvstop+24
.word yycrank+38
.word 0x0
.word yyvstop+36
.word yycrank
.word yysvec+60
.word yyvstop+48
.word yycrank
.word yysvec+72
.word yyvstop+56
.word 0x0
.word 0x0
.word 0x0
.globl yytop
.sdata; .align 2; yytop:.word yycrank+282
.globl yybgin
.sdata; .align 2; yybgin:.word yysvec+12
.globl yymatch
.data; yymatch:.byte 0,1,1,1
.byte 1,1,1,1
.byte 1,9,10,1
.byte 1,1,1,1
.byte 1,1,1,1
.byte 1,1,1,1
.byte 1,1,1,1
.byte 1,1,1,1
.byte 9,1,1,1
.byte 1,1,1,1
.byte 1,1,1,1
.byte 1,1,1,1
.ascii "0000"
.ascii "0000"
.byte 48,48,1,1
.byte 1,1,1,1
.ascii "\001AAA"
.ascii "AAAA"
.ascii "AAAA"
.ascii "AAAA"
.ascii "AAAA"
.ascii "AAAA"
.ascii "AAA\001"
.byte 1,1,1,65
.ascii "\001AAA"
.ascii "AAAA"
.ascii "AAAA"
.ascii "AAAA"
.ascii "AAAA"
.ascii "AAAA"
.ascii "AAA\001"
.byte 1,1,1,1
.byte 0
.globl yyextra
.data; yyextra:.byte 0,0,0,0
.byte 0,0,0,0
.byte 0
.globl yylineno
.sdata; .align 2; yylineno:.word 0x1
.globl yysptr
.sdata; .align 2; yysptr:.word yysbuf
.globl yyprevious
.sdata; .align 2; yyprevious:.word 0xa
.text
.globl yylook
.text
.align 2; .ent yylook
yylook:
$fp2=72
.frame $sp,72,$31
addu $sp,$sp,-72
.mask 0xc0ff0000,-20
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
lw $25,yymorfg
bne $25,$0,L.36
la $21,yytext
b L.37
L.36:
sw $0,yymorfg
lw $25,yyleng
la $21,yytext($25)
L.37:
L.38:
la $23,yylstate
lw $25,yybgin
move $30,$25
sw $25,yyestate
lw $25,yyprevious
bne $25,10,L.42
addu $30,$30,12
L.42:
L.44:
lw $22,($30)
la $25,yycrank
bne $22,$25,L.48
lw $18,4($30)
bne $18,$0,L.50
b L.95
L.50:
lw $25,($18)
la $24,yycrank
bne $25,$24,L.52
b L.95
L.52:
L.48:
move $25,$21
sw $25,$fp2-12($sp)
addu $21,$25,1
lw $24,yysptr
la $15,yysbuf
bleu $24,$15,L.62
lw $24,yysptr
addu $24,$24,-1
sw $24,yysptr
lb $24,($24)
sw $24,$fp2-4($sp)
b L.63
L.62:
lw $24,yyin
lw $15,($24)
subu $15,$15,1
sw $15,($24)
bge $15,$0,L.64
lw $4,yyin
jal _filbuf  # int
sw $2,$fp2-8($sp)
b L.65
L.64:
lw $25,yyin
lw $24,4($25)
addu $15,$24,1
sw $15,4($25)
lbu $25,($24)
sw $25,$fp2-8($sp)
L.65:
lw $25,$fp2-8($sp)
sw $25,$fp2-4($sp)
L.63:
lw $25,$fp2-4($sp)
sw $25,yytchar
bne $25,10,L.60
lw $25,yylineno
addu $25,$25,1
sw $25,yylineno
lw $16,yytchar
b L.61
L.60:
lw $16,yytchar
L.61:
bne $16,-1,L.58
move $17,$0
b L.59
L.58:
lw $17,yytchar
L.59:
move $20,$17
lw $25,$fp2-12($sp)
sb $17,($25)
L.66:
move $19,$22
la $25,yycrank
ble $22,$25,L.67
sll $25,$20,1
addu $22,$25,$19
lw $25,yytop
bgtu $22,$25,L.68
li $25,12
lb $24,($22)
mul $25,$25,$24
la $25,yysvec($25)
bne $25,$30,L.68
li $25,12
lb $24,1($22)
mul $25,$25,$24
la $25,yysvec($25)
la $24,yysvec
bne $25,$24,L.71
addu $25,$21,-1
move $21,$25
lb $25,($25)
sw $25,yytchar
lw $25,yytchar
bne $25,10,L.73
lw $25,yylineno
subu $25,$25,1
sw $25,yylineno
L.73:
lw $25,yysptr
addu $24,$25,1
sw $24,yysptr
lw $24,yytchar
sb $24,($25)
b L.95
L.71:
move $25,$23
addu $23,$25,4
li $24,12
lb $15,1($22)
mul $24,$24,$15
la $24,yysvec($24)
move $30,$24
sw $24,($25)
b L.44
L.67:
la $25,yycrank
bge $22,$25,L.76
la $25,yycrank
subu $25,$25,$22
div $25,$25,2
sll $25,$25,1
la $25,yycrank($25)
move $19,$25
move $22,$25
sll $25,$20,1
addu $22,$25,$22
lw $25,yytop
bgtu $22,$25,L.78
li $25,12
lb $24,($22)
mul $25,$25,$24
la $25,yysvec($25)
bne $25,$30,L.78
li $25,12
lb $24,1($22)
mul $25,$25,$24
la $25,yysvec($25)
la $24,yysvec
bne $25,$24,L.80
addu $25,$21,-1
move $21,$25
lb $25,($25)
sw $25,yytchar
lw $25,yytchar
bne $25,10,L.82
lw $25,yylineno
subu $25,$25,1
sw $25,yylineno
L.82:
lw $25,yysptr
addu $24,$25,1
sw $24,yysptr
lw $24,yytchar
sb $24,($25)
b L.95
L.80:
move $25,$23
addu $23,$25,4
li $24,12
lb $15,1($22)
mul $24,$24,$15
la $24,yysvec($24)
move $30,$24
sw $24,($25)
b L.44
L.78:
lb $25,yymatch($20)
sll $25,$25,1
addu $22,$25,$19
lw $25,yytop
bgtu $22,$25,L.84
li $25,12
lb $24,($22)
mul $25,$25,$24
la $25,yysvec($25)
bne $25,$30,L.84
li $25,12
lb $24,1($22)
mul $25,$25,$24
la $25,yysvec($25)
la $24,yysvec
bne $25,$24,L.86
addu $25,$21,-1
move $21,$25
lb $25,($25)
sw $25,yytchar
lw $25,yytchar
bne $25,10,L.88
lw $25,yylineno
subu $25,$25,1
sw $25,yylineno
L.88:
lw $25,yysptr
addu $24,$25,1
sw $24,yysptr
lw $24,yytchar
sb $24,($25)
b L.95
L.86:
move $25,$23
addu $23,$25,4
li $24,12
lb $15,1($22)
mul $24,$24,$15
la $24,yysvec($24)
move $30,$24
sw $24,($25)
b L.44
L.84:
L.76:
L.68:
lw $25,4($30)
move $30,$25
beq $25,$0,L.90
lw $25,($30)
move $22,$25
la $24,yycrank
beq $25,$24,L.90
b L.66
L.90:
addu $25,$21,-1
move $21,$25
lb $25,($25)
sw $25,yytchar
lw $25,yytchar
bne $25,10,L.92
lw $25,yylineno
subu $25,$25,1
sw $25,yylineno
L.92:
lw $25,yysptr
addu $24,$25,1
sw $24,yysptr
lw $24,yytchar
sb $24,($25)
b L.95
L.94:
move $25,$21
addu $21,$25,-1
sb $0,($25)
lw $25,($23)
move $24,$25
beq $24,$0,L.97
lw $25,8($25)
sw $25,yyfnd
beq $25,$0,L.97
lw $25,yyfnd
lw $25,($25)
ble $25,$0,L.97
sw $23,yyolsp
lw $25,yyfnd
lw $25,($25)
lb $25,yyextra($25)
beq $25,$0,L.99
b L.102
L.101:
addu $23,$23,-4
move $25,$21
addu $21,$25,-1
lb $25,($25)
sw $25,yytchar
lw $25,yytchar
bne $25,10,L.104
lw $25,yylineno
subu $25,$25,1
sw $25,yylineno
L.104:
lw $25,yysptr
addu $24,$25,1
sw $24,yysptr
lw $24,yytchar
sb $24,($25)
L.102:
lw $25,($23)
lw $4,8($25)
lw $25,yyfnd
lw $25,($25)
negu $5,$25
jal yyback  # int
beq $2,1,L.106
la $25,yylstate
bgtu $23,$25,L.101
L.106:
L.99:
lb $25,($21)
sw $25,yyprevious
sw $23,yylsp
la $25,yytext
subu $25,$21,$25
addu $25,$25,1
sw $25,yyleng
lw $25,yyleng
sb $0,yytext($25)
lw $25,yyfnd
addu $24,$25,4
sw $24,yyfnd
lw $2,($25)
b L.35
L.97:
lb $25,($21)
sw $25,yytchar
lw $25,yytchar
bne $25,10,L.107
lw $25,yylineno
subu $25,$25,1
sw $25,yylineno
L.107:
lw $25,yysptr
addu $24,$25,1
sw $24,yysptr
lw $24,yytchar
sb $24,($25)
L.95:
move $25,$23
addu $23,$25,-4
la $24,yylstate
bgtu $25,$24,L.94
lb $25,yytext
bne $25,$0,L.109
la $25,yysbuf
sw $25,yysptr
move $2,$0
b L.35
L.109:
lw $25,yysptr
la $24,yysbuf
bleu $25,$24,L.119
lw $25,yysptr
addu $25,$25,-1
sw $25,yysptr
lb $25,($25)
sw $25,$fp2-4($sp)
b L.120
L.119:
lw $25,yyin
lw $24,($25)
subu $24,$24,1
sw $24,($25)
bge $24,$0,L.121
lw $4,yyin
jal _filbuf  # int
sw $2,$fp2-8($sp)
b L.122
L.121:
lw $25,yyin
lw $24,4($25)
addu $15,$24,1
sw $15,4($25)
lbu $25,($24)
sw $25,$fp2-8($sp)
L.122:
lw $25,$fp2-8($sp)
sw $25,$fp2-4($sp)
L.120:
lw $25,$fp2-4($sp)
sw $25,yytchar
bne $25,10,L.117
lw $25,yylineno
addu $25,$25,1
sw $25,yylineno
lw $16,yytchar
b L.118
L.117:
lw $16,yytchar
L.118:
bne $16,-1,L.115
move $17,$0
b L.116
L.115:
lw $17,yytchar
L.116:
sb $17,yytext
sll $25,$17,24; sra $25,$25,24
sw $25,yyprevious
lw $25,yyprevious
ble $25,$0,L.123
lw $25,yyout
lw $24,($25)
subu $24,$24,1
sw $24,($25)
bge $24,$0,L.126
lw $25,yyprevious
and $4,$25,255
lw $5,yyout
jal _flsbuf  # int
b L.127
L.126:
lw $25,yyout
lw $24,4($25)
addu $15,$24,1
sw $15,4($25)
lw $25,yyprevious
sb $25,($24)
L.127:
L.123:
la $21,yytext
b L.38
L.35:
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
addu $sp,$sp,72
j $31
.end yylook
.globl yyback
.text
.align 2; .ent yyback
yyback:
$fp3=0
.frame $sp,0,$31
bne $4,$0,L.133
move $2,$0
b L.129
L.132:
move $25,$4
addu $4,$25,4
lw $25,($25)
bne $25,$5,L.135
li $2,1
b L.129
L.135:
L.133:
lw $25,($4)
bne $25,$0,L.132
move $2,$0
L.129:
j $31
.end yyback
.globl yyinput
.text
.align 2; .ent yyinput
yyinput:
$fp4=40
.frame $sp,40,$31
addu $sp,$sp,-40
.mask 0xc0e00000,-8
sw $21,16($sp)
sw $22,20($sp)
sw $23,24($sp)
sw $30,28($sp)
sw $31,32($sp)
lw $25,yysptr
la $24,yysbuf
bleu $25,$24,L.146
lw $25,yysptr
addu $25,$25,-1
sw $25,yysptr
lb $22,($25)
b L.147
L.146:
lw $25,yyin
lw $24,($25)
subu $24,$24,1
sw $24,($25)
bge $24,$0,L.148
lw $4,yyin
jal _filbuf  # int
move $21,$2
b L.149
L.148:
lw $25,yyin
lw $24,4($25)
addu $15,$24,1
sw $15,4($25)
lbu $21,($24)
L.149:
move $22,$21
L.147:
sw $22,yytchar
bne $22,10,L.144
lw $25,yylineno
addu $25,$25,1
sw $25,yylineno
lw $23,yytchar
b L.145
L.144:
lw $23,yytchar
L.145:
bne $23,-1,L.142
move $30,$0
b L.143
L.142:
lw $30,yytchar
L.143:
move $2,$30
L.137:
lw $21,16($sp)
lw $22,20($sp)
lw $23,24($sp)
lw $30,28($sp)
lw $31,32($sp)
addu $sp,$sp,40
j $31
.end yyinput
.globl yyoutput
.text
.align 2; .ent yyoutput
yyoutput:
$fp5=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0x80000000,-8
sw $31,16($sp)
sw $4,24($sp)
lw $25,yyout
lw $24,($25)
subu $24,$24,1
sw $24,($25)
bge $24,$0,L.152
lw $25,$fp5+0($sp)
and $4,$25,255
lw $5,yyout
jal _flsbuf  # int
b L.153
L.152:
lw $25,yyout
lw $24,4($25)
addu $15,$24,1
sw $15,4($25)
lw $25,$fp5+0($sp)
sb $25,($24)
L.153:
L.150:
lw $31,16($sp)
addu $sp,$sp,24
j $31
.end yyoutput
.globl yyunput
.text
.align 2; .ent yyunput
yyunput:
$fp6=0
.frame $sp,0,$31
sw $4,yytchar
lw $25,yytchar
bne $25,10,L.155
lw $25,yylineno
subu $25,$25,1
sw $25,yylineno
L.155:
lw $25,yysptr
addu $24,$25,1
sw $24,yysptr
lw $24,yytchar
sb $24,($25)
L.154:
j $31
.end yyunput
.globl main
.text
.align 2; .ent main
main:
$fp7=8
.frame $sp,8,$31
addu $sp,$sp,-8
.mask 0x80000000,-8
sw $31,0($sp)
jal yyparse  # int
L.157:
lw $31,0($sp)
addu $sp,$sp,8
j $31
.end main
.globl yyerror
.text
.align 2; .ent yyerror
yyerror:
$fp8=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0x80000000,-8
sw $31,16($sp)
la $4,L.159
jal printf  # int
L.158:
lw $31,16($sp)
addu $sp,$sp,24
j $31
.end yyerror
.globl yyexca
.data; .align 1; yyexca:.half -1
.half 1
.half 0
.half -1
.half -2
.half 0
.globl yyact
.data; .align 1; yyact:.half 12
.half 2
.half 9
.half 8
.half 17
.half 11
.half 25
.half 17
.half 15
.half 18
.half 16
.half 10
.half 18
.half 17
.half 15
.half 7
.half 16
.half 13
.half 18
.half 5
.half 3
.half 1
.half 0
.half 19
.half 20
.half 0
.half 0
.half 21
.half 22
.half 23
.half 24
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 6
.half 14
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 0
.half 4
.half 6
.globl yypact
.data; .align 1; yypact:.half -1000
.half -9
.half -1000
.half 5
.half -7
.half -59
.half -1000
.half -1000
.half -1000
.half -40
.half -29
.half -40
.half -40
.half -1000
.half -1000
.half -40
.half -40
.half -40
.half -40
.half -38
.half -35
.half -38
.half -38
.half -1000
.half -1000
.half -1000
.globl yypgo
.data; .align 1; yypgo:.half 0
.half 21
.half 20
.half 17
.half 11
.globl yyr1
.data; .align 1; yyr1:.half 0
.half 1
.half 1
.half 1
.half 1
.half 2
.half 4
.half 4
.half 4
.half 4
.half 4
.half 4
.half 4
.half 4
.half 3
.globl yyr2
.data; .align 1; yyr2:.half 0
.half 0
.half 2
.half 3
.half 3
.half 3
.half 3
.half 3
.half 3
.half 3
.half 2
.half 3
.half 1
.half 1
.half 1
.globl yychk
.data; .align 1; yychk:.half -1000
.half -1
.half 10
.half -2
.half 256
.half -3
.half 257
.half 10
.half 10
.half 61
.half -4
.half 45
.half 40
.half -3
.half 258
.half 43
.half 45
.half 42
.half 47
.half -4
.half -4
.half -4
.half -4
.half -4
.half -4
.half 41
.globl yydef
.data; .align 1; yydef:.half 1
.half -2
.half 2
.half 0
.half 0
.half 0
.half 14
.half 3
.half 4
.half 0
.half 5
.half 0
.half 0
.half 12
.half 13
.half 0
.half 0
.half 0
.half 0
.half 10
.half 0
.half 6
.half 7
.half 8
.half 9
.half 11
.globl yychar
.sdata; .align 2; yychar:.word 0xffffffff
.globl yynerrs
.sdata; .align 2; yynerrs:.word 0x0
.globl yyerrflag
.sdata; .align 1; yyerrflag:.half 0
.text
.globl yyparse
.text
.align 2; .ent yyparse
yyparse:
$fp9=360
.frame $sp,360,$31
addu $sp,$sp,-360
.mask 0xc0fe0000,-312
sw $17,16($sp)
sw $18,20($sp)
sw $19,24($sp)
sw $20,28($sp)
sw $21,32($sp)
sw $22,36($sp)
sw $23,40($sp)
sw $30,44($sp)
sw $31,48($sp)
move $17,$0
move $23,$0
li $25,-1
sw $25,yychar
sw $0,yynerrs
sh $17,yyerrflag
la $22,$fp9-302($sp)
la $20,yyv-4
L.163:
addu $25,$22,2
move $22,$25
la $24,$fp9+0($sp)
bleu $25,$24,L.164
la $4,L.167
jal yyerror  # int
li $2,1
b L.160
L.164:
sh $23,($22)
addu $20,$20,4
lw $25,yyval
sw $25,($20)
L.168:
sll $25,$23,16; sra $25,$25,16
sll $25,$25,1
lh $21,yypact($25)
sll $25,$21,16; sra $25,$25,16
bgt $25,-1000,L.169
b L.171
L.169:
lw $25,yychar
bge $25,$0,L.172
jal yylex  # int
sw $2,yychar
bge $2,$0,L.174
sw $0,yychar
L.174:
L.172:
sll $25,$21,16; sra $25,$25,16
lw $24,yychar
addu $25,$25,$24
move $21,$25
sll $25,$25,16; sra $25,$25,16
blt $25,$0,L.178
sll $25,$21,16; sra $25,$25,16
blt $25,249,L.176
L.178:
b L.171
L.176:
sll $25,$21,16; sra $25,$25,16
sll $25,$25,1
lh $25,yyact($25)
move $21,$25
sll $25,$25,16; sra $25,$25,16
sll $25,$25,1
lh $25,yychk($25)
lw $24,yychar
bne $25,$24,L.179
li $25,-1
sw $25,yychar
lw $25,yylval
sw $25,yyval
move $23,$21
lh $25,yyerrflag
ble $25,$0,L.163
lh $25,yyerrflag
subu $25,$25,1
sh $25,yyerrflag
b L.163
L.179:
L.171:
sll $25,$23,16; sra $25,$25,16
sll $25,$25,1
lh $25,yydef($25)
move $21,$25
sll $25,$25,16; sra $25,$25,16
bne $25,-2,L.183
lw $25,yychar
bge $25,$0,L.185
jal yylex  # int
sw $2,yychar
bge $2,$0,L.187
sw $0,yychar
L.187:
L.185:
la $19,yyexca
b L.192
L.189:
L.190:
addu $19,$19,4
L.192:
lh $25,($19)
bne $25,-1,L.189
lh $25,2($19)
sll $24,$23,16; sra $24,$24,16
bne $25,$24,L.189
b L.194
L.193:
lh $25,($19)
lw $24,yychar
bne $25,$24,L.196
b L.195
L.196:
L.194:
addu $25,$19,4
move $19,$25
lh $25,($25)
bge $25,$0,L.193
L.195:
lh $25,2($19)
move $21,$25
sll $25,$25,16; sra $25,$25,16
bge $25,$0,L.198
move $2,$0
b L.160
L.198:
L.183:
sll $25,$21,16; sra $25,$25,16
bne $25,$0,L.200
lh $17,yyerrflag
blt $17,$0,L.202
bgt $17,3,L.202
sll $25,$17,2
lw $25,L.219($25)
j $25
L.219:
.align 2; .word L.205
.align 2; .word L.208
.align 2; .word L.208
.align 2; .word L.215
L.205:
la $4,L.206
jal yyerror  # int
L.207:
lw $25,yynerrs
addu $25,$25,1
sw $25,yynerrs
L.208:
li $25,3
sh $25,yyerrflag
b L.210
L.209:
lh $25,($22)
sll $25,$25,1
lh $25,yypact($25)
addu $21,$25,256
sll $25,$21,16; sra $25,$25,16
blt $25,$0,L.212
bge $25,249,L.212
sll $25,$25,1
lh $25,yyact($25)
sll $25,$25,1
lh $25,yychk($25)
bne $25,256,L.212
sll $25,$21,16; sra $25,$25,16
sll $25,$25,1
lh $23,yyact($25)
b L.163
L.212:
lh $25,($22)
sll $25,$25,1
lh $21,yypact($25)
addu $22,$22,-2
addu $20,$20,-4
L.210:
la $25,$fp9-300($sp)
bgeu $22,$25,L.209
L.214:
li $2,1
b L.160
L.215:
lw $25,yychar
bne $25,$0,L.216
b L.214
L.216:
li $25,-1
sw $25,yychar
b L.168
L.202:
L.200:
sll $25,$21,16; sra $25,$25,16
sll $25,$25,1
lh $24,yyr2($25)
sll $24,$24,1
subu $22,$22,$24
move $30,$20
lh $24,yyr2($25)
sll $24,$24,2
subu $20,$20,$24
lw $24,4($20)
sw $24,yyval
sh $21,$fp9-302($sp)
lh $21,yyr1($25)
sll $25,$21,16; sra $25,$25,16
sll $24,$25,1
lh $24,yypgo($24)
lh $15,($22)
addu $24,$24,$15
addu $18,$24,1
sll $24,$18,16; sra $24,$24,16
bge $24,249,L.222
sll $24,$24,1
lh $24,yyact($24)
move $23,$24
sll $24,$24,16; sra $24,$24,16
sll $24,$24,1
lh $24,yychk($24)
negu $25,$25
beq $24,$25,L.220
L.222:
sll $25,$21,16; sra $25,$25,16
sll $25,$25,1
lh $25,yypgo($25)
sll $25,$25,1
lh $23,yyact($25)
L.220:
lh $17,$fp9-302($sp)
blt $17,4,L.163
bgt $17,14,L.163
sll $25,$17,2
lw $25,L.246-16($25)
j $25
L.246:
.align 2; .word L.226
.align 2; .word L.227
.align 2; .word L.229
.align 2; .word L.231
.align 2; .word L.233
.align 2; .word L.235
.align 2; .word L.237
.align 2; .word L.163
.align 2; .word L.239
.align 2; .word L.241
.align 2; .word L.243
L.226:
sh $0,yyerrflag
b L.163
L.227:
la $4,L.228
jal printf  # int
b L.163
L.229:
la $4,L.230
jal printf  # int
b L.163
L.231:
la $4,L.232
jal printf  # int
b L.163
L.233:
la $4,L.234
jal printf  # int
b L.163
L.235:
la $4,L.236
jal printf  # int
b L.163
L.237:
la $4,L.238
jal printf  # int
b L.163
L.239:
la $4,L.240
jal printf  # int
b L.163
L.241:
la $4,L.242
la $5,yytext
jal printf  # int
b L.163
L.243:
la $4,L.244
la $5,yytext
jal printf  # int
b L.163
L.160:
lw $17,16($sp)
lw $18,20($sp)
lw $19,24($sp)
lw $20,28($sp)
lw $21,32($sp)
lw $22,36($sp)
lw $23,40($sp)
lw $30,44($sp)
lw $31,48($sp)
addu $sp,$sp,360
j $31
.end yyparse
.globl yywrap
.text
.align 2; .ent yywrap
yywrap:
$fp10=0
.frame $sp,0,$31
li $2,1
L.249:
j $31
.end yywrap
.globl yyv
.comm yyv,600
.globl yyfnd
.comm yyfnd,4
.globl yyolsp
.comm yyolsp,4
.globl yylsp
.comm yylsp,4
.globl yylstate
.comm yylstate,800
.globl yyestate
.comm yyestate,4
.globl yytchar
.comm yytchar,4
.globl yysbuf
.comm yysbuf,200
.globl yymorfg
.comm yymorfg,4
.globl yytext
.comm yytext,200
.globl yyleng
.comm yyleng,4
.extern _iob 0
.globl yyval
.comm yyval,4
.globl yylval
.comm yylval,4
.rdata
L.244:.byte 37,115,10,0
L.242:.ascii "push %s\012\000"
L.240:.ascii "load\012\000"
L.238:.ascii "negate\012\000"
L.236:.ascii "divide\012\000"
L.234:.ascii "multiply\012\000"
L.232:.ascii "negate\012add\012\000"
L.230:.ascii "add\012\000"
L.228:.ascii "store\012\000"
L.206:.ascii "syntax error\000"
L.167:.ascii "yacc stack overflow\000"
L.159:.ascii "syntax error\012\000"
L.17:.ascii "bad switch yylook %d\000"

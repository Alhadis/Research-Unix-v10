.data
.globl _yyin
.align 2; _yyin:.long __iob
.globl _yyout
.align 2; _yyout:.long __iob+20
.text
.globl _yylex
.align 2
_yylex:link a6,#-24
moveml #0x490,a6@(-12)
jra L4
L3:
L6:
cmpl #-1,d7; jlt L7
cmpl #4,d7; jgt L7
movl @(L19+4:l,d7:l:4),a2
jra a2@
L19:
.align 2; .long L8
.align 2; .long L9
.align 2; .long L12
.align 2; .long L13
.align 2; .long L8
.align 2; .long L15
L9:
jbsr _yywrap:l
tstl d0; jeq L8
clrl d0
jra L2
L12:
movl #257,d0
jra L2
L13:
movl #258,d0
jra L2
L15:
movb _yytext:l,d4
movl d4,d0; extbl d0
jra L2
L7:
movl _yyout:l,sp@
movl #L17,sp@(4)
movl d7,sp@(8)
jbsr _fprintf:l
L8:
L4:
jbsr _yylook:l
movl d0,d7
tstl d0; jge L3
clrl d0
L2:
moveml a6@(-12),#0x490
unlk a6
rts
.data
.globl _yyvstop
.align 2; _yyvstop:.long 0
.long 4
.long 0
.long 3
.long 4
.long 0
.long 2
.long 4
.long 0
.long 1
.long 4
.long 0
.long 2
.long 0
.long 1
.long 0
.long 0
.globl _yycrank
.align 2; _yycrank:.byte 0
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
.globl _yysvec
.align 2; _yysvec:.long 0x0
.long 0x0
.long 0x0
.long _yycrank-2
.long 0x0
.long 0x0
.long _yycrank
.long _yysvec+12
.long 0x0
.long _yycrank
.long 0x0
.long _yyvstop+4
.long _yycrank
.long 0x0
.long _yyvstop+12
.long _yycrank+4
.long 0x0
.long _yyvstop+24
.long _yycrank+38
.long 0x0
.long _yyvstop+36
.long _yycrank
.long _yysvec+60
.long _yyvstop+48
.long _yycrank
.long _yysvec+72
.long _yyvstop+56
.long 0x0
.long 0x0
.long 0x0
.globl _yytop
.align 2; _yytop:.long _yycrank+282
.globl _yybgin
.align 2; _yybgin:.long _yysvec+12
.globl _yymatch
_yymatch:.byte 0,1,1,1
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
.globl _yyextra
_yyextra:.byte 0,0,0,0
.byte 0,0,0,0
.byte 0
.globl _yylineno
.align 2; _yylineno:.long 1
.globl _yysptr
.align 2; _yysptr:.long _yysbuf
.globl _yyprevious
.align 2; _yyprevious:.long 10
.text
.globl _yylook
.align 2
_yylook:link a6,#-68
moveml #0x3cf8,a6@(-60)
tstl _yymorfg:l; jne L36
movl #_yytext,a6@(-4)
jra L37
L36:
clrl _yymorfg:l
movl _yyleng:l,d4
lea @(_yytext:l,d4:l:1),a2
movl a2,a6@(-4)
L37:
L38:
lea _yylstate:l,a4
movl _yybgin:l,a2
movl a2,a5
movl a2,_yyestate:l
cmpl #10,_yyprevious:l; jne L42
addl #12,a5
L42:
L44:
movl a5@,a3
cmpl #_yycrank,a3; jne L48
movl a5@(4),a6@(-12)
tstl a6@(-12); jne L50
jra L95
L50:
movl a6@(-12),a2
cmpl #_yycrank,a2@; jne L52
jra L95
L52:
L48:
movl a6@(-4),a2
movl a2,a6@(-24)
lea a2@(1),a1
movl a1,a6@(-4)
cmpl #_yysbuf,_yysptr:l; jls L62
movl _yysptr:l,a1
lea a1@(-1),a1
movl a1,_yysptr:l
movb a1@,d4
extbl d4
movl d4,a6@(-16)
jra L63
L62:
movl _yyin:l,a1
movl a1@,d4
subql #1,d4
movl d4,a1@
tstl d4; jlt L64
movl _yyin:l,a1
lea a1@(4),a1
movl a1@,a0
lea a0@(1),a2
movl a2,a1@
movb a0@,d4
andl #0xff,d4
movl d4,a6@(-20)
jra L65
L64:
movl _yyin:l,sp@
jbsr __filbuf:l
movl d0,a6@(-20)
L65:
movl a6@(-20),a6@(-16)
L63:
movl a6@(-16),d4
movl d4,_yytchar:l
cmpl #10,d4; jne L60
addql #1,_yylineno:l
movl _yytchar:l,d5
jra L61
L60:
movl _yytchar:l,d5
L61:
cmpl #-1,d5; jne L58
clrl d6
jra L59
L58:
movl _yytchar:l,d6
L59:
movl d6,d7
movl a6@(-24),a2
movb d6,a2@
L66:
movl a3,a6@(-8)
cmpl #_yycrank,a3; jle L67
movl a6@(-8),a2
lea a2@(0,d7:l:2),a3
movl a3,d4
cmpl _yytop:l,d4; jhi L68
movl #12,d4
movb a3@,d3
extbl d3
mulsl d3,d4
lea @(_yysvec:l,d4:l:1),a2
movl a2,d4
cmpl a5,d4; jne L68
movl #12,d4
movb a3@(1),d3
extbl d3
mulsl d3,d4
lea @(_yysvec:l,d4:l:1),a2
cmpl #_yysvec,a2; jne L71
movl a6@(-4),a2
lea a2@(-1),a2
movl a2,a6@(-4)
movb a2@,d4
extbl d4
movl d4,_yytchar:l
cmpl #10,_yytchar:l; jne L73
subql #1,_yylineno:l
L73:
movl _yysptr:l,a2
lea a2@(1),a1
movl a1,_yysptr:l
movl _yytchar:l,d4
movb d4,a2@
jra L95
L71:
movl a4,a2
lea a2@(4),a4
movl #12,d4
movb a3@(1),d3
extbl d3
mulsl d3,d4
lea @(_yysvec:l,d4:l:1),a1
movl a1,a5
movl a1,a2@
jra L44
L67:
cmpl #_yycrank,a3; jge L76
movl #_yycrank,d4
subl a3,d4
movl d4,d4
movl #2,d3
divsl d3,d4
lea @(_yycrank:l,d4:l:2),a2
movl a2,a6@(-8)
movl a2,a3
lea a3@(0,d7:l:2),a3
movl a3,d4
cmpl _yytop:l,d4; jhi L78
movl #12,d4
movb a3@,d3
extbl d3
mulsl d3,d4
lea @(_yysvec:l,d4:l:1),a2
movl a2,d4
cmpl a5,d4; jne L78
movl #12,d4
movb a3@(1),d3
extbl d3
mulsl d3,d4
lea @(_yysvec:l,d4:l:1),a2
cmpl #_yysvec,a2; jne L80
movl a6@(-4),a2
lea a2@(-1),a2
movl a2,a6@(-4)
movb a2@,d4
extbl d4
movl d4,_yytchar:l
cmpl #10,_yytchar:l; jne L82
subql #1,_yylineno:l
L82:
movl _yysptr:l,a2
lea a2@(1),a1
movl a1,_yysptr:l
movl _yytchar:l,d4
movb d4,a2@
jra L95
L80:
movl a4,a2
lea a2@(4),a4
movl #12,d4
movb a3@(1),d3
extbl d3
mulsl d3,d4
lea @(_yysvec:l,d4:l:1),a1
movl a1,a5
movl a1,a2@
jra L44
L78:
movl a6@(-8),a2
movb @(_yymatch:l,d7:l:1),d4
extbl d4
lea a2@(0,d4:l:2),a3
movl a3,d4
cmpl _yytop:l,d4; jhi L84
movl #12,d4
movb a3@,d3
extbl d3
mulsl d3,d4
lea @(_yysvec:l,d4:l:1),a2
movl a2,d4
cmpl a5,d4; jne L84
movl #12,d4
movb a3@(1),d3
extbl d3
mulsl d3,d4
lea @(_yysvec:l,d4:l:1),a2
cmpl #_yysvec,a2; jne L86
movl a6@(-4),a2
lea a2@(-1),a2
movl a2,a6@(-4)
movb a2@,d4
extbl d4
movl d4,_yytchar:l
cmpl #10,_yytchar:l; jne L88
subql #1,_yylineno:l
L88:
movl _yysptr:l,a2
lea a2@(1),a1
movl a1,_yysptr:l
movl _yytchar:l,d4
movb d4,a2@
jra L95
L86:
movl a4,a2
lea a2@(4),a4
movl #12,d4
movb a3@(1),d3
extbl d3
mulsl d3,d4
lea @(_yysvec:l,d4:l:1),a1
movl a1,a5
movl a1,a2@
jra L44
L84:
L76:
L68:
movl a5@(4),a2
movl a2,a5
cmpl #0,a2; jeq L90
movl a5@,a2
movl a2,a3
cmpl #_yycrank,a2; jeq L90
jra L66
L90:
movl a6@(-4),a2
lea a2@(-1),a2
movl a2,a6@(-4)
movb a2@,d4
extbl d4
movl d4,_yytchar:l
cmpl #10,_yytchar:l; jne L92
subql #1,_yylineno:l
L92:
movl _yysptr:l,a2
lea a2@(1),a1
movl a1,_yysptr:l
movl _yytchar:l,d4
movb d4,a2@
jra L95
L94:
movl a6@(-4),a2
lea a2@(-1),a1
movl a1,a6@(-4)
clrb a2@
movl a4@,a2
cmpl #0,a2; jeq L97
movl a2@(8),a2
movl a2,_yyfnd:l
cmpl #0,a2; jeq L97
movl _yyfnd:l,a2
tstl a2@; jle L97
movl a4,_yyolsp:l
movl _yyfnd:l,a2
movl a2@,d4
tstb @(_yyextra:l,d4:l:1); jeq L99
jra L102
L101:
addl #-4,a4
movl a6@(-4),a2
lea a2@(-1),a1
movl a1,a6@(-4)
movb a2@,d4
extbl d4
movl d4,_yytchar:l
cmpl #10,_yytchar:l; jne L104
subql #1,_yylineno:l
L104:
movl _yysptr:l,a2
lea a2@(1),a1
movl a1,_yysptr:l
movl _yytchar:l,d4
movb d4,a2@
L102:
movl a4@,a2
movl a2@(8),sp@
movl _yyfnd:l,a2
movl a2@,d4
negl d4
movl d4,sp@(4)
jbsr _yyback:l
cmpl #1,d0; jeq L106
cmpl #_yylstate,a4; jhi L101
L106:
L99:
movl a6@(-4),a2
movb a2@,d4
extbl d4
movl d4,_yyprevious:l
movl a4,_yylsp:l
movl a2,d4
subl #_yytext,d4
movl d4,d4
addql #1,d4
movl d4,_yyleng:l
movl _yyleng:l,d4
clrb @(_yytext:l,d4:l:1)
movl _yyfnd:l,a2
lea a2@(4),a1
movl a1,_yyfnd:l
movl a2@,d0
jra L35
L97:
movl a6@(-4),a2
movb a2@,d4
extbl d4
movl d4,_yytchar:l
cmpl #10,_yytchar:l; jne L107
subql #1,_yylineno:l
L107:
movl _yysptr:l,a2
lea a2@(1),a1
movl a1,_yysptr:l
movl _yytchar:l,d4
movb d4,a2@
L95:
movl a4,a2
lea a2@(-4),a4
cmpl #_yylstate,a2; jhi L94
tstb _yytext:l; jne L109
movl #_yysbuf,_yysptr:l
clrl d0
jra L35
L109:
cmpl #_yysbuf,_yysptr:l; jls L119
movl _yysptr:l,a2
lea a2@(-1),a2
movl a2,_yysptr:l
movb a2@,d4
extbl d4
movl d4,a6@(-16)
jra L120
L119:
movl _yyin:l,a2
movl a2@,d4
subql #1,d4
movl d4,a2@
tstl d4; jlt L121
movl _yyin:l,a2
lea a2@(4),a2
movl a2@,a1
lea a1@(1),a0
movl a0,a2@
movb a1@,d4
andl #0xff,d4
movl d4,a6@(-20)
jra L122
L121:
movl _yyin:l,sp@
jbsr __filbuf:l
movl d0,a6@(-20)
L122:
movl a6@(-20),a6@(-16)
L120:
movl a6@(-16),d4
movl d4,_yytchar:l
cmpl #10,d4; jne L117
addql #1,_yylineno:l
movl _yytchar:l,d5
jra L118
L117:
movl _yytchar:l,d5
L118:
cmpl #-1,d5; jne L115
clrl d6
jra L116
L115:
movl _yytchar:l,d6
L116:
movb d6,_yytext:l
movb d6,d4
extbl d4
movl d4,_yyprevious:l
tstl _yyprevious:l; jle L123
movl _yyout:l,a2
movl a2@,d4
subql #1,d4
movl d4,a2@
tstl d4; jlt L128
movl _yyout:l,a2
lea a2@(4),a2
movl a2@,a1
lea a1@(1),a0
movl a0,a2@
movl _yyprevious:l,d4
movb d4,a1@
jra L129
L128:
movl _yyout:l,a2
movw a2@(16),d4
extl d4
movl d4,d4
andl #128,d4
tstl d4; jeq L130
movl a2@,d4
negl d4
cmpl a2@(12),d4; jge L130
movl _yyprevious:l,d4
movl _yyout:l,a2
movl a2@(4),a2
movb d4,a2@
movb d4,d4
andl #0xff,d4
cmpl #10,d4; jeq L132
movl _yyout:l,a2
lea a2@(4),a2
movl a2@,a1
lea a1@(1),a1
movl a1,a2@
jra L129
L132:
movl _yyout:l,a2
movl a2@(4),a1
movb a1@,d4
andl #0xff,d4
movl d4,sp@
movl a2,sp@(4)
jbsr __flsbuf:l
jra L129
L130:
movl _yyprevious:l,d4
movb d4,d4
andl #0xff,d4
movl d4,sp@
movl _yyout:l,sp@(4)
jbsr __flsbuf:l
L129:
L123:
movl #_yytext,a6@(-4)
jra L38
L35:
moveml a6@(-60),#0x3cf8
unlk a6
rts
.globl _yyback
.align 2
_yyback:link a6,#-8
moveml #0x410,a6@(-8)
movl a6@(8),a1
movl a6@(12),d1
cmpl #0,a1; jne L139
clrl d0
jra L135
L138:
movl a1,a2
lea a2@(4),a1
movl a2@,d4
cmpl d1,d4; jne L141
movl #1,d0
jra L135
L141:
L139:
tstl a1@; jne L138
clrl d0
L135:
moveml a6@(-8),#0x410
unlk a6
rts
.globl _yyinput
.align 2
_yyinput:link a6,#-28
moveml #0x4f0,a6@(-24)
cmpl #_yysbuf,_yysptr:l; jls L152
movl _yysptr:l,a2
lea a2@(-1),a2
movl a2,_yysptr:l
movb a2@,d4
extbl d4
movl d4,d5
jra L153
L152:
movl _yyin:l,a2
movl a2@,d4
subql #1,d4
movl d4,a2@
tstl d4; jlt L154
movl _yyin:l,a2
lea a2@(4),a2
movl a2@,a1
lea a1@(1),a0
movl a0,a2@
movb a1@,d4
andl #0xff,d4
movl d4,a6@(-4)
jra L155
L154:
movl _yyin:l,sp@
jbsr __filbuf:l
movl d0,a6@(-4)
L155:
movl a6@(-4),d5
L153:
movl d5,_yytchar:l
cmpl #10,d5; jne L150
addql #1,_yylineno:l
movl _yytchar:l,d6
jra L151
L150:
movl _yytchar:l,d6
L151:
cmpl #-1,d6; jne L148
clrl d7
jra L149
L148:
movl _yytchar:l,d7
L149:
movl d7,d0
L143:
moveml a6@(-24),#0x4f0
unlk a6
rts
.globl _yyoutput
.align 2
_yyoutput:link a6,#-20
moveml #0x490,a6@(-12)
movl a6@(8),d7
movl _yyout:l,a2
movl a2@,d4
subql #1,d4
movl d4,a2@
tstl d4; jlt L160
movl _yyout:l,a2
lea a2@(4),a2
movl a2@,a1
lea a1@(1),a0
movl a0,a2@
movb d7,a1@
jra L161
L160:
movl _yyout:l,a2
movw a2@(16),d4
extl d4
movl d4,d4
andl #128,d4
tstl d4; jeq L162
movl a2@,d4
negl d4
cmpl a2@(12),d4; jge L162
movl _yyout:l,a2
movl a2@(4),a2
movb d7,a2@
movb d7,d4
andl #0xff,d4
cmpl #10,d4; jeq L164
movl _yyout:l,a2
lea a2@(4),a2
movl a2@,a1
lea a1@(1),a1
movl a1,a2@
jra L161
L164:
movl _yyout:l,a2
movl a2@(4),a1
movb a1@,d4
andl #0xff,d4
movl d4,sp@
movl a2,sp@(4)
jbsr __flsbuf:l
jra L161
L162:
movb d7,d4
andl #0xff,d4
movl d4,sp@
movl _yyout:l,sp@(4)
jbsr __flsbuf:l
L161:
L156:
moveml a6@(-12),#0x490
unlk a6
rts
.globl _yyunput
.align 2
_yyunput:link a6,#-8
moveml #0x410,a6@(-8)
movl a6@(8),_yytchar:l
cmpl #10,_yytchar:l; jne L167
subql #1,_yylineno:l
L167:
movl _yysptr:l,a2
lea a2@(1),a1
movl a1,_yysptr:l
movl _yytchar:l,d4
movb d4,a2@
L166:
moveml a6@(-8),#0x410
unlk a6
rts
.globl _main
.align 2
_main:link a6,#-0
jbsr _yyparse:l
L169:
unlk a6
rts
.globl _yyerror
.align 2
_yyerror:link a6,#-4
movl #L171,sp@
jbsr _printf:l
L170:
unlk a6
rts
.data
.globl _yyexca
.align 2; _yyexca:.word -1
.word 1
.word 0
.word -1
.word -2
.word 0
.globl _yyact
.align 2; _yyact:.word 12
.word 2
.word 9
.word 8
.word 17
.word 11
.word 25
.word 17
.word 15
.word 18
.word 16
.word 10
.word 18
.word 17
.word 15
.word 7
.word 16
.word 13
.word 18
.word 5
.word 3
.word 1
.word 0
.word 19
.word 20
.word 0
.word 0
.word 21
.word 22
.word 23
.word 24
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 6
.word 14
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 4
.word 6
.globl _yypact
.align 2; _yypact:.word -1000
.word -9
.word -1000
.word 5
.word -7
.word -59
.word -1000
.word -1000
.word -1000
.word -40
.word -29
.word -40
.word -40
.word -1000
.word -1000
.word -40
.word -40
.word -40
.word -40
.word -38
.word -35
.word -38
.word -38
.word -1000
.word -1000
.word -1000
.globl _yypgo
.align 2; _yypgo:.word 0
.word 21
.word 20
.word 17
.word 11
.globl _yyr1
.align 2; _yyr1:.word 0
.word 1
.word 1
.word 1
.word 1
.word 2
.word 4
.word 4
.word 4
.word 4
.word 4
.word 4
.word 4
.word 4
.word 3
.globl _yyr2
.align 2; _yyr2:.word 0
.word 0
.word 2
.word 3
.word 3
.word 3
.word 3
.word 3
.word 3
.word 3
.word 2
.word 3
.word 1
.word 1
.word 1
.globl _yychk
.align 2; _yychk:.word -1000
.word -1
.word 10
.word -2
.word 256
.word -3
.word 257
.word 10
.word 10
.word 61
.word -4
.word 45
.word 40
.word -3
.word 258
.word 43
.word 45
.word 42
.word 47
.word -4
.word -4
.word -4
.word -4
.word -4
.word -4
.word 41
.globl _yydef
.align 2; _yydef:.word 1
.word -2
.word 2
.word 0
.word 0
.word 0
.word 14
.word 3
.word 4
.word 0
.word 5
.word 0
.word 0
.word 12
.word 13
.word 0
.word 0
.word 0
.word 0
.word 10
.word 0
.word 6
.word 7
.word 8
.word 9
.word 11
.globl _yychar
.align 2; _yychar:.long -1
.globl _yynerrs
.align 2; _yynerrs:.long 0
.globl _yyerrflag
.align 2; _yyerrflag:.word 0
.text
.globl _yyparse
.align 2
_yyparse:link a6,#-360
moveml #0x3cfc,a6@(-352)
clrw d7
movl #-1,_yychar:l
clrl _yynerrs:l
clrw _yyerrflag:l
lea a6@(-306),a4
lea _yyv-4:l,a3
L175:
lea a4@(2),a2
movl a2,a4
movl a2,d4
lea a6@(-4),a2
cmpl a2,d4; jls L176
movl #L179,sp@
jbsr _yyerror:l
movl #1,d0
jra L172
L176:
movw d7,a4@
addql #4,a3
movl _yyval:l,a3@
L180:
movl d7,d4; extl d4
movw @(_yypact:l,d4:l:2),d6
cmpw #-1000,d6; jgt L181
jra L183
L181:
tstl _yychar:l; jge L184
jbsr _yylex:l
movl d0,_yychar:l
tstl d0; jge L186
clrl _yychar:l
L186:
L184:
movl d6,d4; extl d4
addl _yychar:l,d4
movw d4,d6
tstw d4; jlt L190
cmpw #249,d6; jlt L188
L190:
jra L183
L188:
movl d6,d4; extl d4
movw @(_yyact:l,d4:l:2),d4
movw d4,d6
extl d4
movw @(_yychk:l,d4:l:2),d4
extl d4
cmpl _yychar:l,d4; jne L191
movl #-1,_yychar:l
movl _yylval:l,_yyval:l
movw d6,d7
tstw _yyerrflag:l; jle L175
movw _yyerrflag:l,d4
extl d4
subql #1,d4
movw d4,_yyerrflag:l
jra L175
L191:
L183:
movl d7,d4; extl d4
movw @(_yydef:l,d4:l:2),d4
movw d4,d6
cmpw #-2,d4; jne L195
tstl _yychar:l; jge L197
jbsr _yylex:l
movl d0,_yychar:l
tstl d0; jge L199
clrl _yychar:l
L199:
L197:
movl #_yyexca,a6@(-4)
jra L204
L201:
L202:
movl a6@(-4),a2
lea a2@(4),a2
movl a2,a6@(-4)
L204:
movl a6@(-4),a2
cmpw #-1,a2@; jne L201
movw a2@(2),d4
cmpw d7,d4; jne L201
jra L206
L205:
movl a6@(-4),a2
movw a2@,d4
extl d4
cmpl _yychar:l,d4; jne L208
jra L207
L208:
L206:
movl a6@(-4),a2
lea a2@(4),a2
movl a2,a6@(-4)
tstw a2@; jge L205
L207:
movl a6@(-4),a2
movw a2@(2),d4
movw d4,d6
tstw d4; jge L210
clrl d0
jra L172
L210:
L195:
tstw d6; jne L212
movw _yyerrflag:l,d4
extl d4
movl d4,a6@(-312)
movl a6@(-312),d4
tstl d4; jlt L214
cmpl #3,d4; jgt L214
movl @(L231:l,d4:l:4),a2
jra a2@
L231:
.align 2; .long L217
.align 2; .long L220
.align 2; .long L220
.align 2; .long L227
L217:
movl #L218,sp@
jbsr _yyerror:l
L219:
addql #1,_yynerrs:l
L220:
movw #3,_yyerrflag:l
jra L222
L221:
movw a4@,d4
extl d4
movw @(_yypact:l,d4:l:2),d4
extl d4
addl #256,d4
movw d4,d6
movl d6,d4; extl d4
tstl d4; jlt L224
cmpl #249,d4; jge L224
movw @(_yyact:l,d4:l:2),d4
extl d4
cmpw #256,@(_yychk:l,d4:l:2); jne L224
movl d6,d4; extl d4
movw @(_yyact:l,d4:l:2),d7
jra L175
L224:
movw a4@,d4
extl d4
movw @(_yypact:l,d4:l:2),d6
lea a4@(-2),a4
addl #-4,a3
L222:
movl a4,d4
lea a6@(-304),a2
cmpl a2,d4; jcc L221
L226:
movl #1,d0
jra L172
L227:
tstl _yychar:l; jne L228
jra L226
L228:
movl #-1,_yychar:l
jra L180
L214:
L212:
movl d6,d4; extl d4
lea @(_yyr2:l,d4:l:2),a2
movw a2@,d3
extl d3
asll #1,d3
subl d3,a4
movl a3,a5
movw a2@,d3
extl d3
asll #2,d3
movl a3,a2; subl d3,a2
movl a2,a3
movl a3@(4),_yyval:l
movw d6,a6@(-308)
movw @(_yyr1:l,d4:l:2),d6
movl d6,d4; extl d4
movw @(_yypgo:l,d4:l:2),d3
extl d3
movw a4@,d2
extl d2
addl d2,d3
addql #1,d3
movw d3,d5
movl d5,d3; extl d3
cmpl #249,d3; jge L234
movw @(_yyact:l,d3:l:2),d3
movw d3,d7
extl d3
movw @(_yychk:l,d3:l:2),d3
extl d3
negl d4
cmpl d4,d3; jeq L232
L234:
movl d6,d4; extl d4
movw @(_yypgo:l,d4:l:2),d4
extl d4
movw @(_yyact:l,d4:l:2),d7
L232:
movw a6@(-308),d4
extl d4
movl d4,a6@(-312)
movl a6@(-312),d4
cmpl #4,d4; jlt L175
cmpl #14,d4; jgt L175
movl @(L258-16:l,d4:l:4),a2
jra a2@
L258:
.align 2; .long L238
.align 2; .long L239
.align 2; .long L241
.align 2; .long L243
.align 2; .long L245
.align 2; .long L247
.align 2; .long L249
.align 2; .long L175
.align 2; .long L251
.align 2; .long L253
.align 2; .long L255
L238:
clrw _yyerrflag:l
jra L175
L239:
movl #L240,sp@
jbsr _printf:l
jra L175
L241:
movl #L242,sp@
jbsr _printf:l
jra L175
L243:
movl #L244,sp@
jbsr _printf:l
jra L175
L245:
movl #L246,sp@
jbsr _printf:l
jra L175
L247:
movl #L248,sp@
jbsr _printf:l
jra L175
L249:
movl #L250,sp@
jbsr _printf:l
jra L175
L251:
movl #L252,sp@
jbsr _printf:l
jra L175
L253:
movl #L254,sp@
movl #_yytext,sp@(4)
jbsr _printf:l
jra L175
L255:
movl #L256,sp@
movl #_yytext,sp@(4)
jbsr _printf:l
jra L175
L172:
moveml a6@(-352),#0x3cfc
unlk a6
rts
.globl _yywrap
.align 2
_yywrap:link a6,#-0
movl #1,d0
L260:
unlk a6
rts
.bss
.globl _yyv
.comm _yyv,600
.globl _yyfnd
.comm _yyfnd,4
.globl _yyolsp
.comm _yyolsp,4
.globl _yylsp
.comm _yylsp,4
.globl _yylstate
.comm _yylstate,800
.globl _yyestate
.comm _yyestate,4
.globl _yytchar
.comm _yytchar,4
.globl _yysbuf
.comm _yysbuf,200
.globl _yymorfg
.comm _yymorfg,4
.globl _yytext
.comm _yytext,200
.globl _yyleng
.comm _yyleng,4
.globl _yyval
.comm _yyval,4
.globl _yylval
.comm _yylval,4
.text
.align 2; L256:.byte 37,115,10,0
.align 2; L254:.ascii "push %s\012\000"
.align 2; L252:.ascii "load\012\000"
.align 2; L250:.ascii "negate\012\000"
.align 2; L248:.ascii "divide\012\000"
.align 2; L246:.ascii "multiply\012\000"
.align 2; L244:.ascii "negate\012add\012\000"
.align 2; L242:.ascii "add\012\000"
.align 2; L240:.ascii "store\012\000"
.align 2; L218:.ascii "syntax error\000"
.align 2; L179:.ascii "yacc stack overflow\000"
.align 2; L171:.ascii "syntax error\012\000"
.align 2; L17:.ascii "bad switch yylook %d\000"

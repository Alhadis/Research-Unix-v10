.text
.globl _main
.align 2
_main:link a6,#-8
moveml #0x80,a6@(-4)
clrl d7
L2:
movl #1,@(_down:l,d7:l:4)
movl #1,@(_up:l,d7:l:4)
L3:
addql #1,d7
cmpl #15,d7; jlt L2
clrl d7
L6:
movl #1,@(_rows:l,d7:l:4)
L7:
addql #1,d7
cmpl #8,d7; jlt L6
movl #0,sp@
jbsr _queens:l
clrl d0
L1:
moveml a6@(-4),#0x80
unlk a6
rts
.globl _queens
.align 2
_queens:link a6,#-16
moveml #0xd0,a6@(-12)
movl a6@(8),d7
clrl d6
L11:
tstl @(_rows:l,d6:l:4); jeq L15
movl d6,d4; subl d7,d4
tstl @(_up+28:l,d4:l:4); jeq L15
movl d6,d4; addl d7,d4
tstl @(_down:l,d4:l:4); jeq L15
movl d6,d4; addl d7,d4
clrl @(_down:l,d4:l:4)
movl d6,d4; subl d7,d4
clrl @(_up+28:l,d4:l:4)
clrl @(_rows:l,d6:l:4)
movl d6,@(_x:l,d7:l:4)
cmpl #7,d7; jne L19
jbsr _print:l
jra L20
L19:
movl d7,d4; addql #1,d4
movl d4,sp@
jbsr _queens:l
L20:
movl d6,d4; addl d7,d4
movl #1,@(_down:l,d4:l:4)
movl d6,d4; subl d7,d4
movl #1,@(_up+28:l,d4:l:4)
movl #1,@(_rows:l,d6:l:4)
L15:
L12:
addql #1,d6
cmpl #8,d6; jlt L11
L10:
moveml a6@(-12),#0xd0
unlk a6
rts
.globl _print
.align 2
_print:link a6,#-16
moveml #0x90,a6@(-8)
clrl d7
L23:
movl #L27,sp@
movl @(_x:l,d7:l:4),d4
addl #49,d4
movl d4,sp@(4)
jbsr _printf:l
L24:
addql #1,d7
cmpl #8,d7; jlt L23
movl #L28,sp@
jbsr _printf:l
L22:
moveml a6@(-8),#0x90
unlk a6
rts
.bss
.globl _x
.comm _x,32
.globl _rows
.comm _rows,32
.globl _down
.comm _down,60
.globl _up
.comm _up,60
.text
.align 2; L28:.byte 10,0
.align 2; L27:.ascii "%c \000"

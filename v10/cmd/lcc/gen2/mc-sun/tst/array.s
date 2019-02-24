.text
.globl _main
.align 2
_main:link a6,#-80
moveml #0x24d8,a6@(-72)
clrl d6
L2:
clrl d7
L6:
movl #1000,d4
mulsl d6,d4
addl d7,d4
movl d6,d3; asll #4,d3
lea @(_x:l,d3:l:1),a2
movl d4,a2@(0,d7:l:4)
L7:
addql #1,d7
cmpl #4,d7; jlt L6
movl d6,d4; asll #4,d4
lea @(_x:l,d4:l:1),a2
movl a2,@(_y:l,d6:l:4)
L3:
addql #1,d6
cmpl #3,d6; jlt L2
jbsr _f:l
clrl d6
L10:
movl d6,d4; asll #4,d4
lea a6@(-48,d4:l:1),a2
movl a2,a5
movl a2,@(_y:l,d6:l:4)
clrl d7
L14:
movl d6,d4; asll #4,d4
lea @(_x:l,d4:l:1),a2
movl a2@(0,d7:l:4),a5@(0,d7:l:4)
L15:
addql #1,d7
cmpl #4,d7; jlt L14
L11:
addql #1,d6
cmpl #3,d6; jlt L10
lea a6@(-48),a2
movl a2,sp@
movl #_y,sp@(4)
jbsr _g:l
clrl d0
L1:
moveml a6@(-72),#0x24d8
unlk a6
rts
.globl _f
.align 2
_f:link a6,#-24
moveml #0x4d0,a6@(-16)
clrl d6
L19:
clrl d7
L23:
movl #L27,sp@
movl d6,d4; asll #4,d4
lea @(_x:l,d4:l:1),a2
movl a2@(0,d7:l:4),sp@(4)
jbsr _printf:l
L24:
addql #1,d7
cmpl #4,d7; jlt L23
L20:
addql #1,d6
cmpl #3,d6; jlt L19
movl #L28,sp@
jbsr _printf:l
clrl d6
L29:
clrl d7
L33:
movl #L27,sp@
movl @(_y:l,d6:l:4),a2
movl a2@(0,d7:l:4),sp@(4)
jbsr _printf:l
L34:
addql #1,d7
cmpl #4,d7; jlt L33
L30:
addql #1,d6
cmpl #3,d6; jlt L29
movl #L28,sp@
jbsr _printf:l
L18:
moveml a6@(-16),#0x4d0
unlk a6
rts
.globl _g
.align 2
_g:link a6,#-32
moveml #0x34d0,a6@(-24)
movl a6@(8),a5
movl a6@(12),a4
clrl d6
L38:
clrl d7
L42:
movl #L27,sp@
movl d6,d4; asll #4,d4
movl d4,a2; addl a5,a2
movl a2@(0,d7:l:4),sp@(4)
jbsr _printf:l
L43:
addql #1,d7
cmpl #4,d7; jlt L42
L39:
addql #1,d6
cmpl #3,d6; jlt L38
movl #L28,sp@
jbsr _printf:l
clrl d6
L46:
clrl d7
L50:
movl #L27,sp@
movl a4@(0,d6:l:4),a2
movl a2@(0,d7:l:4),sp@(4)
jbsr _printf:l
L51:
addql #1,d7
cmpl #4,d7; jlt L50
L47:
addql #1,d6
cmpl #3,d6; jlt L46
movl #L28,sp@
jbsr _printf:l
L37:
moveml a6@(-24),#0x34d0
unlk a6
rts
.bss
.globl _y
.comm _y,12
.globl _x
.comm _x,48
.text
.align 2; L28:.byte 10,0
.align 2; L27:.ascii " %d\000"

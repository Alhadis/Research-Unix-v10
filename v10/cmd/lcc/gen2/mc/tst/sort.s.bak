.data
.globl _in
.align 2; _in:.long 10
.long 32
.long -1
.long 567
.long 3
.long 18
.long 1
.long -51
.long 789
.long 0
.text
.globl _main
.align 2
_main:link a6,#-12
moveml #0x80,a6@(-4)
movl #_in,sp@
movl #10,sp@(4)
jbsr _sort:l
clrl d7
jra L5
L2:
movl @(_in:l,d7:l:4),sp@
jbsr _putd:l
movl #10,sp@
jbsr _putchar:l
L3:
addql #1,d7
L5:
cmpl #10,d7; jcs L2
clrl d0
L1:
moveml a6@(-4),#0x80
unlk a6
rts
.globl _putd
.align 2
_putd:link a6,#-16
moveml #0x98,a6@(-12)
movl a6@(8),d7
tstl d7; jge L7
movl #45,sp@
jbsr _putchar:l
movl d7,d4; negl d4
movl d4,d7
L7:
movl #10,d4
movl d7,d3; divsl d4,d3
tstl d3; jeq L9
movl #10,d4
movl d7,d3; divsl d4,d3
movl d3,sp@
jbsr _putd:l
L9:
movl d7,d0
movl #10,d4
divsll d4,d4:d0 |mod
movl d4,d4; addl #48,d4
movl d4,sp@
jbsr _putchar:l
L6:
moveml a6@(-12),#0x98
unlk a6
rts
.globl _sort
.align 2
_sort:link a6,#-20
moveml #0x410,a6@(-8)
movl a6@(8),a2
movl a2,_xx:l
movl a2,sp@
clrl sp@(4)
movl a6@(12),d4
subql #1,d4
movl d4,a6@(12)
movl d4,sp@(8)
jbsr _quick:l
L11:
moveml a6@(-8),#0x410
unlk a6
rts
.globl _quick
.align 2
_quick:link a6,#-32
moveml #0x20f0,a6@(-20)
movl a6@(8),a5
movl a6@(12),d7
movl a6@(16),d6
cmpl d6,d7; jlt L13
jra L12
L13:
movl a5,sp@
movl d7,sp@(4)
movl d6,sp@(8)
jbsr _partition:l
movl d0,d5
movl a5,sp@
movl d7,sp@(4)
movl d5,d4; subql #1,d4
movl d4,sp@(8)
jbsr _quick:l
movl a5,sp@
movl d5,d4; addql #1,d4
movl d4,sp@(4)
movl d6,sp@(8)
jbsr _quick:l
L12:
moveml a6@(-20),#0x20f0
unlk a6
rts
.globl _partition
.align 2
_partition:link a6,#-36
moveml #0x24f0,a6@(-28)
movl a6@(8),a5
movl a6@(12),d7
movl a6@(16),d6
addql #1,d6
movl d7,a6@(-4)
movl a6@(-4),d4
movl a5@(0,d4:l:4),d5
jra L17
L16:
addql #1,d7
jra L20
L19:
addql #1,d7
L20:
movl a5@(0,d7:l:4),d4
cmpl d5,d4; jlt L19
subql #1,d6
jra L23
L22:
subql #1,d6
L23:
movl a5@(0,d6:l:4),d4
cmpl d5,d4; jgt L22
cmpl d6,d7; jge L25
lea a5@(0,d7:l:4),a2
movl a2,sp@
lea a5@(0,d6:l:4),a2
movl a2,sp@(4)
jbsr _exchange:l
L25:
L17:
cmpl d6,d7; jlt L16
movl a6@(-4),d4
lea a5@(0,d4:l:4),a2
movl a2,sp@
lea a5@(0,d6:l:4),a2
movl a2,sp@(4)
jbsr _exchange:l
movl d6,d0
L15:
moveml a6@(-28),#0x24f0
unlk a6
rts
.globl _exchange
.align 2
_exchange:link a6,#-36
moveml #0x301c,a6@(-24)
movl a6@(8),a5
movl a6@(12),a4
movl #L28,sp@
movl a5,d4
movl _xx:l,d3
subl d3,d4
movl d4,d4
movl #4,d2
divsl d2,d4
movl d4,sp@(4)
movl a4,d4
subl d3,d4
movl d4,d4
divsl d2,d4
movl d4,sp@(8)
jbsr _printf:l
movl a5@,a6@(-4)
movl a4@,a5@
movl a6@(-4),a4@
L27:
moveml a6@(-24),#0x301c
unlk a6
rts
.bss
.globl _xx
.comm _xx,4
.text
.align 2; L28:.ascii "exchange(%d,%d)\012\000"

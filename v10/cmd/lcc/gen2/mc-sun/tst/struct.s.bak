.text
.globl _addpoint
.align 2
_addpoint:link a6,#-8
moveml #0x10,a6@(-8)
movl a1,a6@(-4)
movl a6@(8),d4
addl a6@(16),d4
movl d4,a6@(8)
movl a6@(12),d4
addl a6@(20),d4
movl d4,a6@(12)
lea a6@(8),a0
movl a6@(-4),a1
movl #8/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
L1:
moveml a6@(-8),#0x10
unlk a6
rts
.globl _canonrect
.align 2
_canonrect:link a6,#-32
moveml #0xd0,a6@(-32)
movl a1,a6@(-4)
movl a6@(8),d4
cmpl a6@(16),d4; jge L10
movl a6@(8),d7
jra L11
L10:
movl a6@(16),d7
L11:
movl d7,a6@(-20)
movl a6@(12),d4
cmpl a6@(20),d4; jge L20
movl a6@(12),d6
jra L21
L20:
movl a6@(20),d6
L21:
movl d6,a6@(-16)
movl a6@(8),d4
cmpl a6@(16),d4; jle L26
movl a6@(8),d7
jra L27
L26:
movl a6@(16),d7
L27:
movl d7,a6@(-12)
movl a6@(12),d4
cmpl a6@(20),d4; jle L37
movl a6@(12),d1
jra L38
L37:
movl a6@(20),d1
L38:
movl d1,a6@(-8)
lea a6@(-20),a0
movl a6@(-4),a1
movl #16/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
L5:
moveml a6@(-32),#0xd0
unlk a6
rts
.globl _makepoint
.align 2
_makepoint:link a6,#-12
movl a1,a6@(-4)
movl a6@(8),a6@(-12)
movl a6@(12),a6@(-8)
lea a6@(-12),a0
movl a6@(-4),a1
movl #8/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
L39:
unlk a6
rts
.globl _makerect
.align 2
_makerect:link a6,#-36
movl a1,a6@(-4)
lea a6@(8),a0
lea a6@(-20),a1
movl #8/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
lea a6@(16),a0
lea a6@(-12),a1
movl #8/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
lea a6@(-20),a0
lea sp@(0),a1; movl #16/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
movl a6@(-4),a1
jbsr _canonrect:l
L42:
unlk a6
rts
.globl _ptinrect
.align 2
_ptinrect:link a6,#-4
moveml #0x10,a6@(-4)
movl a6@(8),d4
cmpl a6@(16),d4; jlt L54
cmpl a6@(24),d4; jge L54
movl a6@(12),d4
cmpl a6@(20),d4; jlt L54
movl a6@(12),d4
cmpl a6@(28),d4; jge L54
movl #1,d1
jra L55
L54:
clrl d1
L55:
movl d1,d0
L46:
moveml a6@(-4),#0x10
unlk a6
rts
.text
.align 2; L57:.long 0
.long 0
.align 2; L58:.long 320
.long 320
.align 2; L59:.long -1
.long -1
.long 1
.long 1
.long 20
.long 300
.long 500
.long 400
.text
.globl _main
.align 2
_main:link a6,#-136
moveml #0x90,a6@(-112)
lea L57:l,a0
lea a6@(-64),a1
movl #8/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
lea L58:l,a0
lea a6@(-72),a1
movl #8/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
lea L59:l,a0
lea a6@(-32),a1
movl #32/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
movl #-10,sp@
movl #-10,sp@(4)
lea a6@(-80),a1
jbsr _makepoint:l
lea a6@(-72),a0
lea sp@(0),a1; movl #8/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
lea a6@(-80),a0
lea sp@(8),a1; movl #8/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
lea a6@(-88),a1
jbsr _addpoint:l
movl #10,sp@
movl #10,sp@(4)
lea a6@(-96),a1
jbsr _makepoint:l
lea a6@(-64),a0
lea sp@(0),a1; movl #8/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
lea a6@(-96),a0
lea sp@(8),a1; movl #8/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
lea a6@(-104),a1
jbsr _addpoint:l
lea a6@(-88),a0
lea sp@(0),a1; movl #8/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
lea a6@(-104),a0
lea sp@(8),a1; movl #8/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
lea a6@(-48),a1
jbsr _makerect:l
clrl d7
jra L68
L65:
movl d7,d4; asll #3,d4
movl a6@(-32,d4:l:1),sp@
movl a6@(-28,d4:l:1),sp@(4)
lea a6@(-56),a1
jbsr _makepoint:l
movl #L69,sp@
movl d7,d4; asll #3,d4
movl a6@(-32,d4:l:1),sp@(4)
movl a6@(-52),sp@(8)
jbsr _printf:l
lea a6@(-56),a0
lea sp@(0),a1; movl #8/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
lea a6@(-48),a0
lea sp@(8),a1; movl #16/2,d0; 2:movw a0@+,a1@+; subql #1,d0; jgt 2b
jbsr _ptinrect:l
tstl d0; jne L73
movl #L75,sp@
jbsr _printf:l
L73:
movl #L76,sp@
movl a6@(-48),sp@(4)
movl a6@(-44),sp@(8)
movl a6@(-40),sp@(12)
movl a6@(-36),sp@(16)
jbsr _printf:l
L66:
addql #1,d7
L68:
cmpl #4,d7; jcs L65
movl #0,sp@
jbsr _exit:l
L56:
moveml a6@(-112),#0x90
unlk a6
rts
.text
.align 2; L76:.ascii "within [%d,%d; %d,%d]\012\000"
.align 2; L75:.ascii "not \000"
.align 2; L69:.ascii "(%d,%d) is \000"

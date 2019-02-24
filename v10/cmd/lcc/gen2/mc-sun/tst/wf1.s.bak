.text
.globl _main
.align 2
_main:link a6,#-36
moveml #0x400,a6@(-28)
clrl a6@(-24)
clrl _next:l
jra L3
L2:
lea a6@(-20),a2
movl a2,sp@
lea a6@(-24),a2
movl a2,sp@(4)
jbsr _lookup:l
movl d0,a2
addql #1,a2@
L3:
lea a6@(-20),a2
movl a2,sp@
jbsr _getword:l
tstl d0; jne L2
movl a6@(-24),sp@
jbsr _tprint:l
clrl d0
L1:
moveml a6@(-28),#0x400
unlk a6
rts
.globl _err
.align 2
_err:link a6,#-8
movl #L6,sp@
movl a6@(8),sp@(4)
jbsr _printf:l
movl #1,sp@
jbsr _exit:l
L5:
unlk a6
rts
.globl _getword
.align 2
_getword:link a6,#-20
moveml #0x2490,a6@(-16)
L8:
L9:
jbsr _getchar:l
movl d0,d7
cmpl #-1,d0; jeq L11
movl d7,sp@
jbsr _isletter:l
tstl d0; jeq L8
L11:
movl a6@(8),a5
jra L15
L12:
movl a5,a2
lea a2@(1),a5
movb d7,a2@
L13:
jbsr _getchar:l
movl d0,d7
L15:
movl d7,sp@
jbsr _isletter:l
movl d0,d7
tstl d0; jne L12
clrb a5@
movl a5,d4
cmpl a6@(8),d4; jls L16
movl #1,d0
jra L7
L16:
clrl d0
L7:
moveml a6@(-16),#0x2490
unlk a6
rts
.globl _isletter
.align 2
_isletter:link a6,#-0
movl a6@(8),d1
cmpl #65,d1; jlt L19
cmpl #90,d1; jgt L19
addl #32,d1
L19:
cmpl #97,d1; jlt L21
cmpl #122,d1; jgt L21
movl d1,d0
jra L18
L21:
clrl d0
L18:
unlk a6
rts
.globl _lookup
.align 2
_lookup:link a6,#-28
moveml #0x2418,a6@(-20)
movl a6@(12),a5
tstl a5@; jeq L24
movl a6@(8),sp@
movl a5@,a2
movl a2@(12),sp@(4)
jbsr _strcmp:l
movl d0,a6@(-4)
tstl a6@(-4); jge L26
movl a6@(8),sp@
movl a5@,a2
lea a2@(4),a2
movl a2,sp@(4)
jbsr _lookup:l
movl d0,d0
jra L23
L26:
tstl a6@(-4); jle L28
movl a6@(8),sp@
movl a5@,a2
lea a2@(8),a2
movl a2,sp@(4)
jbsr _lookup:l
movl d0,d0
jra L23
L28:
movl a5@,d0
jra L23
L24:
cmpl #2000,_next:l; jlt L30
movl #L32,sp@
jbsr _err:l
L30:
movl _next:l,d4
asll #4,d4
clrl @(_words:l,d4:l:1)
movl _next:l,d4
asll #4,d4
clrl @(_words+8:l,d4:l:1)
clrl @(_words+4:l,d4:l:1)
movl a6@(8),sp@
jbsr _strlen:l
movl d0,d4; addql #1,d4
movl d4,sp@
jbsr _malloc:l
movl _next:l,d4
asll #4,d4
movl d0,@(_words+12:l,d4:l:1)
movl _next:l,d4
asll #4,d4
tstl @(_words+12:l,d4:l:1); jne L36
movl #L39,sp@
jbsr _err:l
L36:
movl _next:l,d4
asll #4,d4
movl @(_words+12:l,d4:l:1),sp@
movl a6@(8),sp@(4)
jbsr _strcpy:l
movl _next:l,d4
movl d4,d3; addql #1,d3
movl d3,_next:l
asll #4,d4
lea @(_words:l,d4:l:1),a2
movl a2,a5@
movl a2,d0
L23:
moveml a6@(-20),#0x2418
unlk a6
rts
.globl _tprint
.align 2
_tprint:link a6,#-16
moveml #0x2000,a6@(-4)
movl a6@(8),a5
cmpl #0,a5; jeq L42
movl a5@(4),sp@
jbsr _tprint:l
movl #L44,sp@
movl a5@,sp@(4)
movl a5@(12),sp@(8)
jbsr _printf:l
movl a5@(8),sp@
jbsr _tprint:l
L42:
L41:
moveml a6@(-4),#0x2000
unlk a6
rts
.globl _strcmp
.align 2
_strcmp:link a6,#-12
moveml #0x418,a6@(-12)
movl a6@(8),a0
movl a6@(12),a1
jra L47
L46:
movl a0,a2
lea a2@(1),a0
tstb a2@; jne L49
clrl d0
jra L45
L49:
addql #1,a1
L47:
movb a0@,d4
cmpb a1@,d4; jeq L46
tstb a0@; jne L51
movl #-1,d0
jra L45
L51:
tstb a1@; jne L53
movl #1,d0
jra L45
L53:
movb a0@,d4
extbl d4
movb a1@,d3
extbl d3
movl d4,d0; subl d3,d0
L45:
moveml a6@(-12),#0x418
unlk a6
rts
.bss
.globl _next
.comm _next,4
.globl _words
.comm _words,32000
.text
.align 2; L44:.ascii "%d\011%s\012\000"
.align 2; L39:.ascii "out of word storage\000"
.align 2; L32:.ascii "out of node storage\000"
.align 2; L6:.ascii "? %s\012\000"

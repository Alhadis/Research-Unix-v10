.text
.globl _main
.align 2
_main:link a6,#-84
fmovemx #0xb1,a6@(-48)
moveml #0x4f8,a6@(-72)
cmpl #1,a6@(8); jgt L2
fmoves L4:l,fp7
jra L3
L2:
movl a6@(12),a2
movl a2@(4),sp@
jbsr _atof:l; movl d0,sp@-; fmoves sp@+,fp0
fmovex fp0,fp3; fdivs L5:l,fp3
fmovex fp3,fp7
L3:
clrl d7
L6:
movl d7,d4
movl d4,d3; addql #1,d3
movl d3,d7
fmoves L4:l,fp3
fmoves fp3,@(_f:l,d4:l:4)
L7:
cmpl #127,d7; jle L6
clrl d6
jra L11
L10:
lea @(_f:l,d5:l:4),a2
fmoves a2@,fp3
fadds L13:l,fp3
fmoves fp3,a2@
addql #1,d6
L11:
jbsr _getchar:l
movl d0,d5
cmpl #-1,d0; jne L10
movl #L14,sp@
jbsr _printf:l
clrl d7
L15:
fmoves @(_f:l,d7:l:4),fp3
fcmps L4:l,fp3; fjeq L19
fmovel d6,fp2
fmoves fp2,d0; fmoves d0,fp2
fdivx fp2,fp3
fcmpx fp7,fp3; fjlt L19
cmpl #32,d7; jgt L21
movl #L23,sp@
movl d7,sp@(4)
jbsr _printf:l
jra L22
L21:
movl #L24,sp@
movl d7,sp@(4)
jbsr _printf:l
L22:
movl #L25,sp@
fmoves L5:l,fp3
fmuls @(_f:l,d7:l:4),fp3
fmovel d6,fp2
fmoves fp2,d0; fmoves d0,fp2
fdivx fp2,fp3
fmoved fp3,sp@(4)
jbsr _printf:l
L19:
L16:
addql #1,d7
cmpl #127,d7; jle L15
clrl d0
L1:
moveml a6@(-72),#0x4f8
fmovemx a6@(-48),#0xb1
unlk a6
rts
.bss
.globl _f
.comm _f,512
.text
.align 2; L25:.ascii "\011%.1f\012\000"
.align 2; L24:.byte 37,99,0
.align 2; L23:.ascii "%03o\000"
.align 2; L14:.ascii "char\011freq\012\000"
.align 2; L13:.long 0x3f800000
.align 2; L5:.long 0x42c80000
.align 2; L4:.long 0x0

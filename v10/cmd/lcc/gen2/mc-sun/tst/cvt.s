.text
.globl _main
.align 2
_main:link a6,#-136
fmovemx #0xf3,a6@(-72)
moveml #0xf8,a6@(-92)
movb #1,_c:l
movb _c:l,d4
extbl d4
movw d4,_s:l
movl d4,_i:l
movb d4,_C:l
movw d4,_S:l
movl d4,_I:l
fmovel d4,fp3
fmoves fp3,_f:l
fmoved fp3,_d:l
movl #L2,sp@
movl d4,sp@(4)
movw _s:l,d4
extl d4
movl d4,sp@(8)
movl _i:l,sp@(12)
movb _C:l,d4
andl #0xff,d4
movl d4,sp@(16)
movw _S:l,d4
andl #0xffff,d4
movl d4,sp@(20)
movl _I:l,sp@(24)
fmoves _f:l,fp3
fmoved fp3,sp@(28)
fmoved _d:l,fp3
fmoved fp3,sp@(36)
jbsr _printf:l
movw #2,_s:l
movw _s:l,d4
extl d4
movb d4,_c:l
movl d4,_i:l
movb d4,_C:l
movw d4,_S:l
movl d4,_I:l
fmovel d4,fp3
fmoves fp3,_f:l
fmoved fp3,_d:l
movl #L2,sp@
movb _c:l,d3
extbl d3
movl d3,sp@(4)
movl d4,sp@(8)
movl _i:l,sp@(12)
movb _C:l,d4
andl #0xff,d4
movl d4,sp@(16)
movw _S:l,d4
andl #0xffff,d4
movl d4,sp@(20)
movl _I:l,sp@(24)
fmoves _f:l,fp3
fmoved fp3,sp@(28)
fmoved _d:l,fp3
fmoved fp3,sp@(36)
jbsr _printf:l
movl #3,_i:l
movl _i:l,d4
movb d4,_c:l
movw d4,_s:l
movb d4,_C:l
movw d4,_S:l
movl d4,_I:l
fmovel d4,fp3
fmoves fp3,_f:l
fmoved fp3,_d:l
movl #L2,sp@
movb _c:l,d3
extbl d3
movl d3,sp@(4)
movw _s:l,d3
extl d3
movl d3,sp@(8)
movl d4,sp@(12)
movb _C:l,d4
andl #0xff,d4
movl d4,sp@(16)
movw _S:l,d4
andl #0xffff,d4
movl d4,sp@(20)
movl _I:l,sp@(24)
fmoves _f:l,fp3
fmoved fp3,sp@(28)
fmoved _d:l,fp3
fmoved fp3,sp@(36)
jbsr _printf:l
movb #4,_C:l
movb _C:l,d4
andl #0xff,d4
movb d4,_c:l
movw d4,_s:l
movl d4,_i:l
movw d4,_S:l
movl d4,_I:l
movb _C:l,d4
andl #0xff,d4
tstl d4; jlt L4
movl d4,d4
fmovel d4,fp7
jra L5
L4:
fmovel d4,fp3
faddd L6:l,fp3
fmovex fp3,fp7
L5:
fmoves fp7,_f:l
movb _C:l,d4
andl #0xff,d4
tstl d4; jlt L8
movl d4,d4
fmovel d4,fp6
jra L9
L8:
fmovel d4,fp3
faddd L6:l,fp3
fmovex fp3,fp6
L9:
fmoved fp6,_d:l
movl #L2,sp@
movb _c:l,d4
extbl d4
movl d4,sp@(4)
movw _s:l,d4
extl d4
movl d4,sp@(8)
movl _i:l,sp@(12)
movb _C:l,d4
andl #0xff,d4
movl d4,sp@(16)
movw _S:l,d4
andl #0xffff,d4
movl d4,sp@(20)
movl _I:l,sp@(24)
fmoves _f:l,fp3
fmoved fp3,sp@(28)
fmoved _d:l,fp3
fmoved fp3,sp@(36)
jbsr _printf:l
movw #5,_S:l
movw _S:l,d4
andl #0xffff,d4
movb d4,_c:l
movw d4,_s:l
movl d4,_i:l
movb d4,_C:l
movl d4,_I:l
movw _S:l,d4
andl #0xffff,d4
tstl d4; jlt L11
movl d4,d4
fmovel d4,fp7
jra L12
L11:
fmovel d4,fp3
faddd L6:l,fp3
fmovex fp3,fp7
L12:
fmoves fp7,_f:l
movw _S:l,d4
andl #0xffff,d4
tstl d4; jlt L14
movl d4,d4
fmovel d4,fp6
jra L15
L14:
fmovel d4,fp3
faddd L6:l,fp3
fmovex fp3,fp6
L15:
fmoved fp6,_d:l
movl #L2,sp@
movb _c:l,d4
extbl d4
movl d4,sp@(4)
movw _s:l,d4
extl d4
movl d4,sp@(8)
movl _i:l,sp@(12)
movb _C:l,d4
andl #0xff,d4
movl d4,sp@(16)
movw _S:l,d4
andl #0xffff,d4
movl d4,sp@(20)
movl _I:l,sp@(24)
fmoves _f:l,fp3
fmoved fp3,sp@(28)
fmoved _d:l,fp3
fmoved fp3,sp@(36)
jbsr _printf:l
movl #6,_I:l
movl _I:l,d4
movb d4,_c:l
movw d4,_s:l
movl d4,_i:l
movb d4,_C:l
movw d4,_S:l
movl _I:l,d4
tstl d4; jlt L17
fmovel d4,fp7
jra L18
L17:
fmovel d4,fp3
faddd L6:l,fp3
fmovex fp3,fp7
L18:
fmoves fp7,_f:l
movl _I:l,d4
tstl d4; jlt L20
fmovel d4,fp6
jra L21
L20:
fmovel d4,fp3
faddd L6:l,fp3
fmovex fp3,fp6
L21:
fmoved fp6,_d:l
movl #L2,sp@
movb _c:l,d4
extbl d4
movl d4,sp@(4)
movw _s:l,d4
extl d4
movl d4,sp@(8)
movl _i:l,sp@(12)
movb _C:l,d4
andl #0xff,d4
movl d4,sp@(16)
movw _S:l,d4
andl #0xffff,d4
movl d4,sp@(20)
movl _I:l,sp@(24)
fmoves _f:l,fp3
fmoved fp3,sp@(28)
fmoved _d:l,fp3
fmoved fp3,sp@(36)
jbsr _printf:l
fmoves L22:l,fp3
fmoves fp3,_f:l
fmoves _f:l,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movb d4,_c:l
movw d4,_s:l
movl d4,_i:l
fmoves _f:l,fp3
fmoved L26:l,fp2
fcmpx fp2,fp3; fjlt L24
fmovex fp3,fp1; fsubx fp2,fp1
fintrzx fp1,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,d7
jra L25
L24:
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,d7
L25:
movb d7,_C:l
fmoves _f:l,fp3
fmoved L26:l,fp2
fcmpx fp2,fp3; fjlt L28
fmovex fp3,fp1; fsubx fp2,fp1
fintrzx fp1,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,d6
jra L29
L28:
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,d6
L29:
movw d6,_S:l
fmoves _f:l,fp3
fmoved L26:l,fp2
fcmpx fp2,fp3; fjlt L31
fmovex fp3,fp1; fsubx fp2,fp1
fintrzx fp1,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,d5
jra L32
L31:
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,d5
L32:
movl d5,_I:l
fmoves _f:l,fp3
fmoved fp3,_d:l
movl #L2,sp@
movb _c:l,d4
extbl d4
movl d4,sp@(4)
movw _s:l,d4
extl d4
movl d4,sp@(8)
movl _i:l,sp@(12)
movb _C:l,d4
andl #0xff,d4
movl d4,sp@(16)
movw _S:l,d4
andl #0xffff,d4
movl d4,sp@(20)
movl _I:l,sp@(24)
fmoved fp3,sp@(28)
fmoved _d:l,fp3
fmoved fp3,sp@(36)
jbsr _printf:l
fmoved L33:l,fp3
fmoved fp3,_d:l
fmoved _d:l,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movb d4,_c:l
movw d4,_s:l
movl d4,_i:l
fmoved _d:l,fp3
fmoved L26:l,fp2
fcmpx fp2,fp3; fjlt L35
fmovex fp3,fp1; fsubx fp2,fp1
fintrzx fp1,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,d7
jra L36
L35:
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,d7
L36:
movb d7,_C:l
fmoved _d:l,fp3
fmoved L26:l,fp2
fcmpx fp2,fp3; fjlt L38
fmovex fp3,fp1; fsubx fp2,fp1
fintrzx fp1,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,d6
jra L39
L38:
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,d6
L39:
movw d6,_S:l
fmoved _d:l,fp3
fmoved L26:l,fp2
fcmpx fp2,fp3; fjlt L41
fmovex fp3,fp1; fsubx fp2,fp1
fintrzx fp1,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,d5
jra L42
L41:
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,d5
L42:
movl d5,_I:l
fmoved _d:l,fp3
fmoves fp3,_f:l
movl #L2,sp@
movb _c:l,d4
extbl d4
movl d4,sp@(4)
movw _s:l,d4
extl d4
movl d4,sp@(8)
movl _i:l,sp@(12)
movb _C:l,d4
andl #0xff,d4
movl d4,sp@(16)
movw _S:l,d4
andl #0xffff,d4
movl d4,sp@(20)
movl _I:l,sp@(24)
fmoves _f:l,fp2
fmoved fp2,sp@(28)
fmoved fp3,sp@(36)
jbsr _printf:l
L1:
moveml a6@(-92),#0xf8
fmovemx a6@(-72),#0xf3
unlk a6
rts
.bss
.globl _d2
.comm _d2,8
.globl _d1
.comm _d1,8
.globl _d
.comm _d,8
.globl _f2
.comm _f2,4
.globl _f1
.comm _f1,4
.globl _f
.comm _f,4
.globl _I
.comm _I,4
.globl _S
.comm _S,2
.globl _C
.comm _C,1
.globl _i2
.comm _i2,4
.globl _i1
.comm _i1,4
.globl _i
.comm _i,4
.globl _s
.comm _s,2
.globl _c2
.comm _c2,1
.globl _c1
.comm _c1,1
.globl _c
.comm _c,1
.text
.align 2; L33:.long 0x40200000,0x0
.align 2; L26:.long 0x41e00000,0x0
.align 2; L22:.long 0x40e00000
.align 2; L6:.long 0x41f00000,0x0
.align 2; L2:.ascii "%d %d %d %d %d %d %f %f\012\000"

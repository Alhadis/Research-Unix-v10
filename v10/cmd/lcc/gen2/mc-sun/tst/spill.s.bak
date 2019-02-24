.text
.globl _main
.align 2
_main:link a6,#-0
L1:
unlk a6
rts
.globl _f
.align 2
_f:link a6,#-8
moveml #0x90,a6@(-8)
jbsr _f:l
movl d0,d7
jbsr _f:l
movl d7,d4; addl d0,d4
movl d4,a6@(8)
L2:
moveml a6@(-8),#0x90
unlk a6
rts
.globl _f2
.align 2
_f2:link a6,#-12
moveml #0xd0,a6@(-12)
jbsr _f:l
movl d0,d6
tstl a6@(8); jeq L6
jbsr _f:l
movl d0,d7
jra L7
L6:
movl #1,d7
L7:
movl d6,d4; addl d7,d4
movl d4,a6@(8)
L4:
moveml a6@(-12),#0xd0
unlk a6
rts
.globl _f3
.align 2
_f3:link a6,#-48
moveml #0x4e0,a6@(-48)
clrl d7
clrl d6
clrl d5
clrl a6@(-4)
clrl a6@(-8)
clrl a6@(-12)
clrl a6@(-16)
clrl a6@(-20)
clrl a6@(-24)
clrl a6@(-28)
movl a6@(12),a2
lea a2@(4),a1
movl a1,a6@(12)
movl a6@(8),a6@(-32)
tstl a6@(-32); jeq L11
jbsr _f:l
movl d0,a6@(-32)
L11:
movl a6@(-32),a2@
L9:
moveml a6@(-48),#0x4e0
unlk a6
rts
.globl _f4
.align 2
_f4:link a6,#-76
fmovemx #0xf0,a6@(-64)
moveml #0x98,a6@(-76)
clrl d7
clrl d0
clrl d1
clrl a6@(-4)
clrl a6@(-8)
clrl a6@(-12)
movl _i:l,d4
movl d4,d3; asll #3,d3
fmoved @(_a:l,d3:l:1),fp3
fmoved @(_b:l,d3:l:1),fp2
fmovex fp3,fp1; faddx fp2,fp1
fmoved L17:l,fp0
fcmpx fp0,fp1; fjeq L15
tstl d4; jeq L15
fsubx fp2,fp3
fcmpx fp0,fp3; fjeq L15
movl #1,a6@(-16)
jra L16
L15:
clrl a6@(-16)
L16:
movl a6@(-16),_i:l
L13:
moveml a6@(-76),#0x98
fmovemx a6@(-64),#0xf0
unlk a6
rts
.globl _f5
.align 2
_f5:link a6,#-48
fmovemx #0x30,a6@(-24)
moveml #0x341c,a6@(-48)
movl _k:l,d4
movl _m:l,d3
movl d4,d2; mulsl d3,d2
asll #3,d2
movl _A:l,a2
movl d2,a1; addl a2,a1
fmoved a1@,fp3
movl _j:l,d2
movl d2,d1; mulsl d3,d1
movl d1,d3; asll #3,d3
movl d3,a0; addl a2,a0
movl a0,a5
fmuld a0@,fp3
movl _n:l,d3
mulsl d3,d4
asll #3,d4
movl _B:l,a2
movl d4,a0; addl a2,a0
movl a0,a4
fmoved a0@,fp2
movl d2,d4; mulsl d3,d4
asll #3,d4
movl d4,a0; addl a2,a0
fmuld a0@,fp2
faddx fp2,fp3
fmoved fp3,_x:l
fmoved a1@,fp3
fmuld a0@,fp3
fmoved a4@,fp2
fmuld a5@,fp2
fsubx fp2,fp3
fmoved fp3,_x:l
L18:
moveml a6@(-48),#0x341c
fmovemx a6@(-24),#0x30
unlk a6
rts
.bss
.globl _x
.comm _x,8
.globl _B
.comm _B,4
.globl _A
.comm _A,4
.globl _n
.comm _n,4
.globl _m
.comm _m,4
.globl _k
.comm _k,4
.globl _j
.comm _j,4
.globl _i
.comm _i,4
.globl _b
.comm _b,80
.globl _a
.comm _a,80
.text
.align 2; L17:.long 0x0,0x0

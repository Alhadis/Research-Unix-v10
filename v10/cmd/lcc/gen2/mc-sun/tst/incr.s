.text
.globl _main
.align 2
_main:link a6,#-0
L1:
unlk a6
rts
.globl _memchar
.align 2
_memchar:link a6,#-12
moveml #0x400,a6@(-12)
movl a6@(-8),a2
lea a2@(1),a1
movl a1,a6@(-8)
movb a2@,a6@(-4)
movl a6@(-8),a2
lea a2@(1),a2
movl a2,a6@(-8)
movb a2@,a6@(-4)
movl a6@(-8),a2
lea a2@(-1),a1
movl a1,a6@(-8)
movb a2@,a6@(-4)
movl a6@(-8),a2
lea a2@(-1),a2
movl a2,a6@(-8)
movb a2@,a6@(-4)
L2:
moveml a6@(-12),#0x400
unlk a6
rts
.globl _memint
.align 2
_memint:link a6,#-12
moveml #0x400,a6@(-12)
movl a6@(-8),a2
lea a2@(4),a1
movl a1,a6@(-8)
movl a2@,a6@(-4)
movl a6@(-8),a2
lea a2@(4),a2
movl a2,a6@(-8)
movl a2@,a6@(-4)
movl a6@(-8),a2
lea a2@(-4),a1
movl a1,a6@(-8)
movl a2@,a6@(-4)
movl a6@(-8),a2
lea a2@(-4),a2
movl a2,a6@(-8)
movl a2@,a6@(-4)
L3:
moveml a6@(-12),#0x400
unlk a6
rts
.globl _regchar
.align 2
_regchar:link a6,#-4
moveml #0x400,a6@(-4)
movl a1,a2
lea a2@(1),a1
movb a2@,d1
lea a1@(1),a2
movl a2,a1
movb a2@,d1
movl a1,a2
lea a2@(-1),a1
movb a2@,d1
lea a1@(-1),a2
movl a2,a1
movb a2@,d1
L4:
moveml a6@(-4),#0x400
unlk a6
rts
.globl _regint
.align 2
_regint:link a6,#-4
moveml #0x400,a6@(-4)
movl a1,a2
lea a2@(4),a1
movl a2@,d1
lea a1@(4),a2
movl a2,a1
movl a2@,d1
movl a1,a2
lea a2@(-4),a1
movl a2@,d1
lea a1@(-4),a2
movl a2,a1
movl a2@,d1
L5:
moveml a6@(-4),#0x400
unlk a6
rts

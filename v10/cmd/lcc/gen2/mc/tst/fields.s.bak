.data
.globl _x
.align 2; _x:.long 1
.byte 2
.skip 1
.byte 0
.byte 52
.byte 10
.byte 6
.globl _i
.align 2; _i:.long 16
.globl _y
.align 2; _y:.byte 224
.skip 1
.byte 0
.byte 0
.byte 0
.byte 9
.text
.globl _main
.align 2
_main:link a6,#-40
moveml #0x1c,a6@(-12)
movl #L4,sp@
movl _x:l,sp@(4)
movb _x+4:l,d4
extbl d4
movl d4,sp@(8)
movl _x+6:l,d4
movl #20,d3
asrl d3,d4
movl d4,sp@(12)
movl _x+6:l,d4
movl #12,d3
asll d3,d4
movl #28,d3
asrl d3,d4
movl d4,sp@(16)
movl _x+8:l,d4
asll #4,d4
movl #29,d3
asrl d3,d4
movl d4,sp@(20)
movb _x+9:l,d4
extbl d4
movl d4,sp@(24)
jbsr _printf:l
movl #L10,sp@
movl _y:l,d4
movl #30,d3
movl d4,d2; lsrl d3,d2
movl d2,sp@(4)
lsll #2,d4
movl #28,d3
lsrl d3,d4
movl d4,sp@(8)
movl _y+2:l,sp@(12)
jbsr _printf:l
movl _x+6:l,d4
andl #0xfff0ffff,d4
movl _i:l,d3
movl #28,d2
asll d2,d3
asrl d2,d3
movl d3,d3
movl #16,d2
lsll d2,d3
andl #0xf0000,d3
orl d3,d4
movl d4,_x+6:l
movl _x+8:l,d4
andl #0xf1ffffff,d4
movl d4,_x+8:l
movl #L4,sp@
movl _x:l,sp@(4)
movb _x+4:l,d4
extbl d4
movl d4,sp@(8)
movl _x+6:l,d4
movl #20,d3
asrl d3,d4
movl d4,sp@(12)
movl _x+6:l,d4
movl #12,d3
asll d3,d4
movl #28,d3
asrl d3,d4
movl d4,sp@(16)
movl _x+8:l,d4
asll #4,d4
movl #29,d3
asrl d3,d4
movl d4,sp@(20)
movb _x+9:l,d4
extbl d4
movl d4,sp@(24)
jbsr _printf:l
movl _y:l,d4
andl #0x3fffffff,d4
orl #0x80000000,d4
movl d4,_y:l
movl _i:l,_y+2:l
movl #L10,sp@
movl _y:l,d4
movl #30,d3
movl d4,d2; lsrl d3,d2
movl d2,sp@(4)
lsll #2,d4
movl #28,d3
lsrl d3,d4
movl d4,sp@(8)
movl _y+2:l,sp@(12)
jbsr _printf:l
movl #_x,sp@
jbsr _f2:l
L3:
moveml a6@(-12),#0x1c
unlk a6
rts
.globl _f1
.align 2
_f1:link a6,#-28
moveml #0x201c,a6@(-16)
movl a6@(8),a5
movl a5@,d4
andl #0xc3ffffff,d4
movl d4,a5@
movl a5@,d4
andl #0x3fffffff,d4
clrl d3
andl #3,d3
movl #30,d2
lsll d2,d3
andl #0xc0000000,d3
orl d3,d4
movl d4,a5@
movl a5@,d4
andl #0x3c000000,d4
tstl d4; jeq L22
movl #L24,sp@
jbsr _printf:l
L22:
movl a5@,d4
orl #0xc0000000,d4
movl d4,a5@
movl a5@,d4
orl #0x3c000000,d4
movl d4,a5@
movl #L25,sp@
movl a5@,d4
movl #30,d3
movl d4,d2; lsrl d3,d2
movl d2,sp@(4)
lsll #2,d4
movl #28,d3
lsrl d3,d4
movl d4,sp@(8)
jbsr _printf:l
L21:
moveml a6@(-16),#0x201c
unlk a6
rts
.globl _f2
.align 2
_f2:link a6,#-24
moveml #0x209c,a6@(-20)
movl a6@(8),a5
tstl _i:l; jne L28
movl #1,d7
jra L29
L28:
clrl d7
L29:
movl a5@,d4
andl #0x3fffffff,d4
movl d7,d3; andl #3,d3
movl #30,d2
lsll d2,d3
andl #0xc0000000,d3
orl d3,d4
movl d4,a5@
movl a5,sp@
jbsr _f1:l
movl a5@,d4
andl #0xc3ffffff,d4
clrl d3
andl #15,d3
movl #26,d2
lsll d2,d3
andl #0x3c000000,d3
orl d3,d4
movl d4,a5@
L26:
moveml a6@(-20),#0x209c
unlk a6
rts
.text
.align 2; L25:.ascii "p->a = 0x%x, p->b = 0x%x\012\000"
.align 2; L24:.ascii "p->b != 0!\012\000"
.align 2; L10:.ascii "y = %d %d %d\012\000"
.align 2; L4:.ascii "x = %d %d %d %d %d %d\012\000"

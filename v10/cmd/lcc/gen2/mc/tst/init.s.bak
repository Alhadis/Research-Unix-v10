.data
.globl _words
.align 2; _words:.long 1
.long 2
.long 3
.ascii "if\000"
.skip 3
.long 4
.long 5
.skip 4
.ascii "for"
.skip 3
.long 6
.long 7
.long 8
.ascii "else\000"
.skip 1
.long 9
.long 10
.long 11
.ascii "whil"
.byte 101
.skip 1
.long 0
.skip 8
.skip 6
.globl _wordlist
.align 2; _wordlist:.long _words
.globl _x
.align 2; _x:.long 1
.long 2
.long 3
.long 4
.long 0
.long 5
.long 6
.skip 12
.long 7
.skip 16
.globl _y
.align 2; _y:.long _x
.long _x+20
.long _x+40
.long 0x0
.text
.globl _main
.align 2
_main:link a6,#-20
moveml #0x4c0,a6@(-12)
clrl d6
jra L8
L5:
clrl d7
jra L12
L9:
movl #L13,sp@
movl @(_y:l,d6:l:4),a2
movl a2@(0,d7:l:4),sp@(4)
jbsr _printf:l
L10:
addql #1,d7
L12:
movl @(_y:l,d6:l:4),a2
tstl a2@(0,d7:l:4); jne L9
movl #L14,sp@
jbsr _printf:l
L6:
addql #1,d6
L8:
tstl @(_y:l,d6:l:4); jne L5
jbsr _f:l
movl _wordlist:l,sp@
jbsr _g:l
clrl d0
L4:
moveml a6@(-12),#0x4c0
unlk a6
rts
.data
.align 2; L16:.long L17
.long L18
.long L19
.long L20
.long 0x0
.text
.globl _f
.align 2
_f:link a6,#-12
moveml #0x2000,a6@(-4)
lea L16:l,a5
jra L24
L21:
movl #L25,sp@
movl a5@,sp@(4)
jbsr _printf:l
L22:
addql #4,a5
L24:
tstl a5@; jne L21
L15:
moveml a6@(-4),#0x2000
unlk a6
rts
.globl _g
.align 2
_g:link a6,#-20
moveml #0x2480,a6@(-12)
movl a6@(8),a5
jra L30
L27:
clrl d7
jra L34
L31:
movl #L35,sp@
movl a5@(0,d7:l:4),sp@(4)
jbsr _printf:l
L32:
addql #1,d7
L34:
cmpl #3,d7; jcs L31
movl #L25,sp@
lea a5@(12),a2
movl a2,sp@(4)
jbsr _printf:l
L28:
addl #18,a5
L30:
tstl a5@; jne L27
jbsr _h:l
L26:
moveml a6@(-12),#0x2480
unlk a6
rts
.globl _h
.align 2
_h:link a6,#-32
moveml #0x490,a6@(-12)
clrl d7
jra L40
L37:
movl #L41,sp@
movl #18,d4
mulsl d7,d4
movl @(_words:l,d4:l:1),sp@(4)
movl @(_words+4:l,d4:l:1),sp@(8)
movl @(_words+8:l,d4:l:1),sp@(12)
lea @(_words+12:l,d4:l:1),a2
movl a2,sp@(16)
jbsr _printf:l
L38:
addql #1,d7
L40:
cmpl #5,d7; jcs L37
L36:
moveml a6@(-12),#0x490
unlk a6
rts
.text
.align 2; L41:.ascii "%d %d %d %s\012\000"
.align 2; L35:.ascii "%d \000"
.align 2; L25:.byte 37,115,10,0
.align 2; L20:.ascii "while\000"
.align 2; L19:.ascii "else\000"
.align 2; L18:.ascii "for\000"
.align 2; L17:.ascii "if\000"
.align 2; L14:.byte 10,0
.align 2; L13:.ascii " %d\000"

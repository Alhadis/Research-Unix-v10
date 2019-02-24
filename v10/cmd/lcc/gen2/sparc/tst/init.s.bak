.seg "data"
.global _words
.align 4; _words:.word 1
.word 2
.word 3
.ascii "if\000"
.skip 3
.skip 2
.word 4
.word 5
.skip 4
.ascii "for"
.skip 3
.skip 2
.word 6
.word 7
.word 8
.ascii "else\000"
.skip 1
.skip 2
.word 9
.word 10
.word 11
.ascii "whil"
.byte 101
.skip 1
.skip 2
.word 0
.skip 8
.skip 8
.global _wordlist
.align 4; _wordlist:.word _words
.global _x
.align 4; _x:.word 1
.word 2
.word 3
.word 4
.word 0
.word 5
.word 6
.skip 12
.word 7
.skip 16
.global _y
.align 4; _y:.word _x
.word _x+20
.word _x+40
.word 0x0
.seg "text"
.global _main
.align 4
.proc 4
_main:save %sp,-96,%sp
mov %r0,%r28
ba L8; nop
L5:
mov %r0,%r29
ba L12; nop
L9:
set L13,%r8
sll %r29,2,%r27
sll %r28,2,%r26
set _y,%r25
ld [%r26+%r25],%r26
ld [%r27+%r26],%r9
call _printf; nop
L10:
add %r29,1,%r29
L12:
sll %r29,2,%r27
sll %r28,2,%r26
set _y,%r25
ld [%r26+%r25],%r26
ld [%r27+%r26],%r27
cmp %r27,%r0; bne L9; nop
set L14,%r8
call _printf; nop
L6:
add %r28,1,%r28
L8:
sll %r28,2,%r27
set _y,%r26
ld [%r27+%r26],%r27
cmp %r27,%r0; bne L5; nop
call _f; nop
set _wordlist,%r27
ld [%r27],%r8
call _g; nop
mov %r0,%r24
L4:
ret; restore
.seg "data"
.align 4; _16:.word L17
.word L18
.word L19
.word L20
.word 0x0
.seg "text"
.global _f
.align 4
.proc 4
_f:save %sp,-96,%sp
set _16,%r29
ba L24; nop
L21:
set L25,%r8
ld [%r29],%r9
call _printf; nop
L22:
add %r29,4,%r29
L24:
ld [%r29],%r28
cmp %r28,%r0; bne L21; nop
L15:
ret; restore
.global _g
.align 4
.proc 4
_g:save %sp,-96,%sp
ba L30; nop
L27:
mov %r0,%r29
ba L34; nop
L31:
set L35,%r8
sll %r29,2,%r28
ld [%r28+%r24],%r9
call _printf; nop
L32:
add %r29,1,%r29
L34:
cmp %r29,3; blu L31; nop
set L25,%r8
add %r24,12,%r9
call _printf; nop
L28:
add %r24,20,%r24
L30:
ld [%r24],%r28
cmp %r28,%r0; bne L27; nop
call _h; nop
L26:
ret; restore
.global _h
.align 4
.proc 4
_h:save %sp,-96,%sp
mov %r0,%r29
ba L40; nop
L37:
set 20,%r28
mov %r29,%o1; call .mul,2; mov %r28,%o0; mov %o0,%r28
set L41,%r8
set _words,%r27
ld [%r28+%r27],%r9
set _words+4,%r27
ld [%r28+%r27],%r10
set _words+8,%r27
ld [%r28+%r27],%r11
set _words+12,%r27
add %r28,%r27,%r12
call _printf; nop
L38:
add %r29,1,%r29
L40:
cmp %r29,5; blu L37; nop
L36:
ret; restore
.seg "text"
L41:.ascii "%d %d %d %s\012\000"
L35:.ascii "%d \000"
L25:.byte 37,115,10,0
L20:.ascii "while\000"
L19:.ascii "else\000"
L18:.ascii "for\000"
L17:.ascii "if\000"
L14:.byte 10,0
L13:.ascii " %d\000"

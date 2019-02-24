.seg "data"
.global _x
.align 4; _x:.word 1
.word 2
.word 3
.word 4
.seg "text"
.global _main
.align 4
.proc 4
_main:save %sp,-192,%sp
set L2,%r8
call _print; nop
set L3,%r8
set L4,%r9
call _print; nop
set L5,%r8
set 3,%r9
set 10,%r10
call _print; nop
set L6,%r8
set L7,%r9
set L8,%r10
set 4,%r11
set 10,%r12
call _print; nop
set L9,%r8
set L7,%r9
set L8,%r10
set L10,%r29
ld [%r29],%f30; ld [%r29+4],%f31
st %f30,[%sp+4*3+68]
st %f31,[%sp+4*4+68]
ld [%sp+4*3+68],%o3
ld [%sp+4*4+68],%o4
set 10,%r13
call _print; nop
set L11,%r8
add -16,%fp,%r29
set _x,%r28
ld [%r28+0],%r1
ld [%r28+4],%r2
st %r1,[%r29+0]
ld [%r28+8],%r1
st %r2,[%r29+4]
ld [%r28+12],%r2
st %r1,[%r29+8]
st %r2,[%r29+12]
mov %r29,%r9
add -32,%fp,%r29
ld [%r28+0],%r1
ld [%r28+4],%r2
st %r1,[%r29+0]
ld [%r28+8],%r1
st %r2,[%r29+4]
ld [%r28+12],%r2
st %r1,[%r29+8]
st %r2,[%r29+12]
mov %r29,%r10
add -48,%fp,%r29
ld [%r28+0],%r1
ld [%r28+4],%r2
st %r1,[%r29+0]
ld [%r28+8],%r1
st %r2,[%r29+4]
ld [%r28+12],%r2
st %r1,[%r29+8]
st %r2,[%r29+12]
mov %r29,%r11
add -64,%fp,%r29
ld [%r28+0],%r1
ld [%r28+4],%r2
st %r1,[%r29+0]
ld [%r28+8],%r1
st %r2,[%r29+4]
ld [%r28+12],%r2
st %r1,[%r29+8]
st %r2,[%r29+12]
mov %r29,%r12
add -80,%fp,%r29
ld [%r28+0],%r1
ld [%r28+4],%r2
st %r1,[%r29+0]
ld [%r28+8],%r1
st %r2,[%r29+4]
ld [%r28+12],%r2
st %r1,[%r29+8]
st %r2,[%r29+12]
mov %r29,%r13
add -96,%fp,%r29
ld [%r28+0],%r1
ld [%r28+4],%r2
st %r1,[%r29+0]
ld [%r28+8],%r1
st %r2,[%r29+4]
ld [%r28+12],%r2
st %r1,[%r29+8]
st %r2,[%r29+12]
st %r29,[%sp+4*6+68]
call _print; nop
L1:
ret; restore
.global _print
.align 4
.proc 4
_print:save %sp,-112,%sp
st %i1,[%fp+72]
st %i2,[%fp+76]
st %i3,[%fp+80]
st %i4,[%fp+84]
st %i5,[%fp+88]
st %r24,[%fp+68]
add 72,%fp,%r29
ba L24; nop
L21:
ld [%fp+68],%r28
ldsb [%r28],%r28
cmp %r28,37; bne L25; nop
ld [%fp+68],%r27
add %r27,1,%r27
st %r27,[%fp+68]
ldsb [%r27],%r28
cmp %r28,115; be L41; nop
cmp %r28,115; bg L45; nop
cmp %r28,98; bl L27; nop
cmp %r28,102; bg L27; nop
sll %r28,2,%r27
set L47-392,%r26
ld [%r27+%r26],%r27
jmp %r27; nop
L47:
.align 4; .word L30
.align 4; .word L35
.align 4; .word L37
.align 4; .word L27
.align 4; .word L43
L46:
L45:
cmp %r28,119; be L39; nop
ba L27; nop
L30:
add %r29,4,%r27
mov %r27,%r29
add -16,%fp,%r26
ld [%r27+-4],%r27
ld [%r27+0],%r1
ld [%r27+4],%r2
st %r1,[%r26+0]
ld [%r27+8],%r1
st %r2,[%r26+4]
ld [%r27+12],%r2
st %r1,[%r26+8]
st %r2,[%r26+12]
set L31,%r8
ld [%fp+-16],%r9
ld [%fp+-12],%r10
ld [%fp+-8],%r11
ld [%fp+-4],%r12
call _printf; nop
ba L26; nop
L35:
set L36,%r8
add %r29,4,%r27
mov %r27,%r29
ldsb [%r27+-1],%r9
call _printf; nop
ba L26; nop
L37:
set L38,%r8
add %r29,4,%r27
mov %r27,%r29
ld [%r27+-4],%r9
call _printf; nop
ba L26; nop
L39:
set L40,%r8
add %r29,4,%r27
mov %r27,%r29
ldsh [%r27+-2],%r9
call _printf; nop
ba L26; nop
L41:
set L42,%r8
add %r29,4,%r27
mov %r27,%r29
ld [%r27+-4],%r9
call _printf; nop
ba L26; nop
L43:
set L44,%r8
add %r29,8,%r27
mov %r27,%r29
ld [%r27+-8],%f30; ld [%r27+-8+4],%f31
st %f30,[%sp+4*1+68]
st %f31,[%sp+4*2+68]
ld [%sp+4*1+68],%o1
ld [%sp+4*2+68],%o2
call _printf; nop
ba L26; nop
L27:
set L36,%r8
ld [%fp+68],%r27
ldsb [%r27],%r9
call _printf; nop
ba L26; nop
L25:
set L36,%r8
ld [%fp+68],%r27
ldsb [%r27],%r9
call _printf; nop
L26:
L22:
ld [%fp+68],%r27
add %r27,1,%r27
st %r27,[%fp+68]
L24:
ld [%fp+68],%r27
ldsb [%r27],%r27
cmp %r27,%r0; bne L21; nop
L18:
ret; restore
.seg "text"
L44:.byte 37,102,0
L42:.byte 37,115,0
L40:.byte 37,120,0
L38:.byte 37,100,0
L36:.byte 37,99,0
L31:.ascii "{%d %d %d %d}\000"
L11:.ascii "%b %b %b %b %b %b\012\000"
.align 8; L10:.word 0x40140000,0x0
L9:.ascii "%s%s %f%c\000"
L8:.ascii "st\000"
L7:.ascii "te\000"
L6:.ascii "%s%s %w%c\000"
L5:.ascii "test %d%c\000"
L4:.byte 50,0
L3:.ascii "test %s\012\000"
L2:.ascii "test 1\012\000"

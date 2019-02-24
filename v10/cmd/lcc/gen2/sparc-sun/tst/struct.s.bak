.seg "text"
.global _addpoint
.align 4
.proc 4
_addpoint:mov %fp,%g7; mov %sp,%fp
ld [%r8],%r13
ld [%r9],%r12
add %r13,%r12,%r13
st %r13,[%r8]
add %r8,4,%r13
ld [%r13],%r12
ld [%r9+4],%r11
add %r12,%r11,%r12
st %r12,[%r13]
ld [%fp+64],%r13
mov %r8,%r12
ld [%r12+0],%r1
ld [%r12+4],%r2
st %r1,[%r13+0]
st %r2,[%r13+4]
L1:
retl; mov %g7,%fp
.global _canonrect
.align 4
.proc 4
_canonrect:save %sp,-112,%sp
ld [%r24],%r27
ld [%r24+8],%r26
cmp %r27,%r26; bge L6; nop
ld [%r24],%r29
ba L7; nop
L6:
ld [%r24+8],%r29
L7:
st %r29,[%fp+-16]
ld [%r24+4],%r27
ld [%r24+12],%r26
cmp %r27,%r26; bge L10; nop
ld [%r24+4],%r28
ba L11; nop
L10:
ld [%r24+12],%r28
L11:
st %r28,[%fp+-12]
ld [%r24],%r28
ld [%r24+8],%r27
cmp %r28,%r27; ble L14; nop
ld [%r24],%r29
ba L15; nop
L14:
ld [%r24+8],%r29
L15:
st %r29,[%fp+-8]
ld [%r24+4],%r28
ld [%r24+12],%r27
cmp %r28,%r27; ble L19; nop
ld [%r24+4],%r29
ba L20; nop
L19:
ld [%r24+12],%r29
L20:
st %r29,[%fp+-4]
ld [%fp+64],%r29
add -16,%fp,%r28
ld [%r28+0],%r1
ld [%r28+4],%r2
st %r1,[%r29+0]
ld [%r28+8],%r1
st %r2,[%r29+4]
ld [%r28+12],%r2
st %r1,[%r29+8]
st %r2,[%r29+12]
L3:
ret; restore
.global _makepoint
.align 4
.proc 4
_makepoint:save %sp,-104,%sp
st %r24,[%fp+-8]
st %r25,[%fp+-4]
ld [%fp+64],%r29
add -8,%fp,%r28
ld [%r28+0],%r1
ld [%r28+4],%r2
st %r1,[%r29+0]
st %r2,[%r29+4]
L21:
ret; restore
.global _makerect
.align 4
.proc 4
_makerect:save %sp,-128,%sp
add -16,%fp,%r29
mov %r24,%r28
ld [%r28+0],%r1
ld [%r28+4],%r2
st %r1,[%r29+0]
st %r2,[%r29+4]
add -8,%fp,%r29
mov %r25,%r28
ld [%r28+0],%r1
ld [%r28+4],%r2
st %r1,[%r29+0]
st %r2,[%r29+4]
add -32,%fp,%r29
add -16,%fp,%r28
ld [%r28+0],%r1
ld [%r28+4],%r2
st %r1,[%r29+0]
ld [%r28+8],%r1
st %r2,[%r29+4]
ld [%r28+12],%r2
st %r1,[%r29+8]
st %r2,[%r29+12]
mov %r29,%r8
ld [%fp+64],%r29
call _canonrect; st %r29,[%sp+64]
L24:
ret; restore
.global _ptinrect
.align 4
.proc 4
_ptinrect:
ld [%r8],%r12
ld [%r9],%r11
cmp %r12,%r11; bl L31; nop
ld [%r9+8],%r11
cmp %r12,%r11; bge L31; nop
ld [%r8+4],%r12
ld [%r9+4],%r11
cmp %r12,%r11; bl L31; nop
ld [%r9+12],%r11
cmp %r12,%r11; bge L31; nop
set 1,%r13
ba L32; nop
L31:
mov %r0,%r13
L32:
mov %r13,%r8
L29:
retl; nop
.seg "text"
.align 4; L34:.word 0
.word 0
.align 4; L35:.word 320
.word 320
.align 4; L36:.word -1
.word -1
.word 1
.word 1
.word 20
.word 300
.word 500
.word 400
.seg "text"
.global _main
.align 4
.proc 4
_main:save %sp,-240,%sp
add -64,%fp,%r28
set L34,%r27
ld [%r27+0],%r1
ld [%r27+4],%r2
st %r1,[%r28+0]
st %r2,[%r28+4]
add -72,%fp,%r28
set L35,%r27
ld [%r27+0],%r1
ld [%r27+4],%r2
st %r1,[%r28+0]
st %r2,[%r28+4]
add -32,%fp,%r28
set L36,%r27
add %r27,32,%r27
add %r28,32,%r3
1: dec 8,%r3
ld [%r27+-8],%r1
ld [%r27+-4],%r2
st %r1,[%r3+0]
st %r2,[%r3+4]
cmp %r3,%r28; bgt 1b; dec 8,%r27
set -10,%r28
mov %r28,%r8
mov %r28,%r9
add -80,%fp,%r28
call _makepoint; st %r28,[%sp+64]
add -88,%fp,%r28
add -72,%fp,%r27
ld [%r27+0],%r1
ld [%r27+4],%r2
st %r1,[%r28+0]
st %r2,[%r28+4]
mov %r28,%r8
add -80,%fp,%r9
add -96,%fp,%r28
call _addpoint; st %r28,[%sp+64]
set 10,%r28
mov %r28,%r8
mov %r28,%r9
add -104,%fp,%r28
call _makepoint; st %r28,[%sp+64]
add -112,%fp,%r28
add -64,%fp,%r27
ld [%r27+0],%r1
ld [%r27+4],%r2
st %r1,[%r28+0]
st %r2,[%r28+4]
mov %r28,%r8
add -104,%fp,%r9
add -120,%fp,%r28
call _addpoint; st %r28,[%sp+64]
add -96,%fp,%r8
add -120,%fp,%r9
add -48,%fp,%r28
call _makerect; st %r28,[%sp+64]
mov %r0,%r29
ba L47; nop
L44:
sll %r29,3,%r28
add -32,%fp,%r27
ld [%r28+%r27],%r8
add -28,%fp,%r27
ld [%r28+%r27],%r9
add -56,%fp,%r28
call _makepoint; st %r28,[%sp+64]
set L48,%r8
sll %r29,3,%r28
add -32,%fp,%r27
ld [%r28+%r27],%r9
ld [%fp+-52],%r10
call _printf; nop
add -128,%fp,%r28
add -56,%fp,%r27
ld [%r27+0],%r1
ld [%r27+4],%r2
st %r1,[%r28+0]
st %r2,[%r28+4]
mov %r28,%r8
add -144,%fp,%r28
add -48,%fp,%r27
ld [%r27+0],%r1
ld [%r27+4],%r2
st %r1,[%r28+0]
ld [%r27+8],%r1
st %r2,[%r28+4]
ld [%r27+12],%r2
st %r1,[%r28+8]
st %r2,[%r28+12]
mov %r28,%r9
call _ptinrect; nop
cmp %r8,%r0; bne L52; nop
set L56,%r8
call _printf; nop
L52:
set L57,%r8
ld [%fp+-48],%r9
ld [%fp+-44],%r10
ld [%fp+-40],%r11
ld [%fp+-36],%r12
call _printf; nop
L45:
add %r29,1,%r29
L47:
cmp %r29,4; blu L44; nop
mov %r0,%r8
call _exit; nop
L33:
ret; restore
.seg "text"
L57:.ascii "within [%d,%d; %d,%d]\012\000"
L56:.ascii "not \000"
L48:.ascii "(%d,%d) is \000"

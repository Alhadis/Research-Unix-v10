.seg "text"
.global _main
.align 4
.proc 4
_main:save %sp,-120,%sp
st %r0,[%fp+-24]
set _next,%r29
st %r0,[%r29]
ba L3; nop
L2:
add -20,%fp,%r8
add -24,%fp,%r9
call _lookup; nop
mov %r8,%r29
ld [%r29],%r28
add %r28,1,%r28
st %r28,[%r29]
L3:
add -20,%fp,%r8
call _getword; nop
cmp %r8,%r0; bne L2; nop
ld [%fp+-24],%r8
call _tprint; nop
mov %r0,%r24
L1:
ret; restore
.global _err
.align 4
.proc 4
_err:save %sp,-96,%sp
set L6,%r8
mov %r24,%r9
call _printf; nop
set 1,%r8
call _exit; nop
L5:
ret; restore
.global _getword
.align 4
.proc 4
_getword:save %sp,-96,%sp
L8:
L9:
call _getchar; nop
mov %r8,%r29
cmp %r8,-1; be L11; nop
mov %r29,%r8
call _isletter; nop
cmp %r8,%r0; be L8; nop
L11:
mov %r24,%r28
ba L15; nop
L12:
mov %r28,%r27
add %r27,1,%r28
stb %r29,[%r27]
L13:
call _getchar; nop
mov %r8,%r29
L15:
mov %r29,%r8
call _isletter; nop
mov %r8,%r29
cmp %r8,%r0; bne L12; nop
stb %r0,[%r28]
cmp %r28,%r24; bleu L16; nop
set 1,%r24
ba L7; nop
L16:
mov %r0,%r24
L7:
ret; restore
.global _isletter
.align 4
.proc 4
_isletter:
cmp %r8,65; bl L19; nop
cmp %r8,90; bg L19; nop
add %r8,32,%r8
L19:
cmp %r8,97; bl L21; nop
cmp %r8,122; bg L21; nop
ba L18; nop
L21:
mov %r0,%r8
L18:
retl; nop
.global _lookup
.align 4
.proc 4
_lookup:save %sp,-96,%sp
ld [%r25],%r28
cmp %r28,%r0; be L24; nop
mov %r24,%r8
ld [%r25],%r28
ld [%r28+12],%r9
call _strcmp; nop
mov %r8,%r29
cmp %r29,%r0; bge L26; nop
mov %r24,%r8
ld [%r25],%r28
add %r28,4,%r9
call _lookup; nop
mov %r8,%r24
ba L23; nop
L26:
cmp %r29,%r0; ble L28; nop
mov %r24,%r8
ld [%r25],%r28
add %r28,8,%r9
call _lookup; nop
mov %r8,%r24
ba L23; nop
L28:
ld [%r25],%r24
ba L23; nop
L24:
set _next,%r28
ld [%r28],%r28
cmp %r28,2000; bl L30; nop
set L32,%r8
call _err; nop
L30:
set _next,%r28
ld [%r28],%r28
sll %r28,4,%r28
set _words,%r27
st %r0,[%r28+%r27]
set _next,%r28
ld [%r28],%r28
sll %r28,4,%r28
set _words+8,%r27
st %r0,[%r28+%r27]
set _words+4,%r27
st %r0,[%r28+%r27]
mov %r24,%r8
call _strlen; nop
add %r8,1,%r8
call _malloc; nop
set _next,%r28
ld [%r28],%r28
sll %r28,4,%r28
set _words+12,%r27
st %r8,[%r28+%r27]
set _next,%r28
ld [%r28],%r28
sll %r28,4,%r28
set _words+12,%r27
ld [%r28+%r27],%r28
cmp %r28,%r0; bne L36; nop
set L39,%r8
call _err; nop
L36:
set _next,%r28
ld [%r28],%r28
sll %r28,4,%r28
set _words+12,%r27
ld [%r28+%r27],%r8
mov %r24,%r9
call _strcpy; nop
set _next,%r28
ld [%r28],%r27
add %r27,1,%r26
st %r26,[%r28]
sll %r27,4,%r28
set _words,%r27
add %r28,%r27,%r28
st %r28,[%r25]
mov %r28,%r24
L23:
ret; restore
.global _tprint
.align 4
.proc 4
_tprint:save %sp,-96,%sp
cmp %r24,%r0; be L42; nop
ld [%r24+4],%r8
call _tprint; nop
set L44,%r8
ld [%r24],%r9
ld [%r24+12],%r10
call _printf; nop
ld [%r24+8],%r8
call _tprint; nop
L42:
L41:
ret; restore
.global _strcmp
.align 4
.proc 4
_strcmp:
ba L47; nop
L46:
mov %r8,%r13
add %r13,1,%r8
ldsb [%r13],%r13
cmp %r13,%r0; bne L49; nop
mov %r0,%r8
ba L45; nop
L49:
add %r9,1,%r9
L47:
ldsb [%r8],%r13
ldsb [%r9],%r12
cmp %r13,%r12; be L46; nop
ldsb [%r8],%r13
cmp %r13,%r0; bne L51; nop
set -1,%r8
ba L45; nop
L51:
ldsb [%r9],%r13
cmp %r13,%r0; bne L53; nop
set 1,%r8
ba L45; nop
L53:
ldsb [%r8],%r13
ldsb [%r9],%r12
sub %r13,%r12,%r8
L45:
retl; nop
.seg "bss"
.global _next
.align 4; .common _next,4
.global _words
.align 4; .common _words,32000
.seg "text"
L44:.ascii "%d\011%s\012\000"
L39:.ascii "out of word storage\000"
L32:.ascii "out of node storage\000"
L6:.ascii "? %s\012\000"

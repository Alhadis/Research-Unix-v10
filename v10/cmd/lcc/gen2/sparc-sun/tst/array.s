.seg "text"
.global _main
.align 4
.proc 4
_main:save %sp,-144,%sp
mov %r0,%r28
L2:
mov %r0,%r29
L6:
set 1000,%r26
mov %r28,%o1; call .mul,2; mov %r26,%o0; mov %o0,%r26
add %r26,%r29,%r26
sll %r29,2,%r25
sll %r28,4,%r24
set _x,%r23
add %r24,%r23,%r24
st %r26,[%r25+%r24]
L7:
add %r29,1,%r29
cmp %r29,4; bl L6; nop
sll %r28,4,%r26
set _x,%r25
add %r26,%r25,%r26
sll %r28,2,%r25
set _y,%r24
st %r26,[%r25+%r24]
L3:
add %r28,1,%r28
cmp %r28,3; bl L2; nop
call _f; nop
mov %r0,%r28
L10:
sll %r28,4,%r26
add -48,%fp,%r25
add %r26,%r25,%r26
mov %r26,%r27
sll %r28,2,%r25
set _y,%r24
st %r26,[%r25+%r24]
mov %r0,%r29
L14:
sll %r29,2,%r26
sll %r28,4,%r25
set _x,%r24
add %r25,%r24,%r25
ld [%r26+%r25],%r25
st %r25,[%r26+%r27]
L15:
add %r29,1,%r29
cmp %r29,4; bl L14; nop
L11:
add %r28,1,%r28
cmp %r28,3; bl L10; nop
add -48,%fp,%r8
set _y,%r9
call _g; nop
mov %r0,%r24
L1:
ret; restore
.global _f
.align 4
.proc 4
_f:save %sp,-96,%sp
mov %r0,%r28
L19:
mov %r0,%r29
L23:
set L27,%r8
sll %r29,2,%r27
sll %r28,4,%r26
set _x,%r25
add %r26,%r25,%r26
ld [%r27+%r26],%r9
call _printf; nop
L24:
add %r29,1,%r29
cmp %r29,4; bl L23; nop
L20:
add %r28,1,%r28
cmp %r28,3; bl L19; nop
set L28,%r8
call _printf; nop
mov %r0,%r28
L29:
mov %r0,%r29
L33:
set L27,%r8
sll %r29,2,%r27
sll %r28,2,%r26
set _y,%r25
ld [%r26+%r25],%r26
ld [%r27+%r26],%r9
call _printf; nop
L34:
add %r29,1,%r29
cmp %r29,4; bl L33; nop
L30:
add %r28,1,%r28
cmp %r28,3; bl L29; nop
set L28,%r8
call _printf; nop
L18:
ret; restore
.global _g
.align 4
.proc 4
_g:save %sp,-96,%sp
mov %r0,%r28
L38:
mov %r0,%r29
L42:
set L27,%r8
sll %r29,2,%r27
sll %r28,4,%r26
add %r26,%r24,%r26
ld [%r27+%r26],%r9
call _printf; nop
L43:
add %r29,1,%r29
cmp %r29,4; bl L42; nop
L39:
add %r28,1,%r28
cmp %r28,3; bl L38; nop
set L28,%r8
call _printf; nop
mov %r0,%r28
L46:
mov %r0,%r29
L50:
set L27,%r8
sll %r29,2,%r27
sll %r28,2,%r26
ld [%r26+%r25],%r26
ld [%r27+%r26],%r9
call _printf; nop
L51:
add %r29,1,%r29
cmp %r29,4; bl L50; nop
L47:
add %r28,1,%r28
cmp %r28,3; bl L46; nop
set L28,%r8
call _printf; nop
L37:
ret; restore
.seg "bss"
.global _y
.align 4; .common _y,12
.global _x
.align 4; .common _x,48
.seg "text"
L28:.byte 10,0
L27:.ascii " %d\000"

.seg "text"
.global _main
.align 4
.proc 4
_main:save %sp,-96,%sp
mov %r0,%r29
L2:
set 1,%r28
sll %r29,2,%r27
set _down,%r26
st %r28,[%r27+%r26]
set _up,%r26
st %r28,[%r27+%r26]
L3:
add %r29,1,%r29
cmp %r29,15; bl L2; nop
mov %r0,%r29
L6:
set 1,%r28
sll %r29,2,%r27
set _rows,%r26
st %r28,[%r27+%r26]
L7:
add %r29,1,%r29
cmp %r29,8; bl L6; nop
mov %r0,%r8
call _queens; nop
mov %r0,%r24
L1:
ret; restore
.global _queens
.align 4
.proc 4
_queens:save %sp,-96,%sp
mov %r0,%r29
L11:
sll %r29,2,%r28
set _rows,%r27
ld [%r28+%r27],%r28
cmp %r28,%r0; be L15; nop
sub %r29,%r24,%r28
sll %r28,2,%r28
set _up+28,%r27
ld [%r28+%r27],%r28
cmp %r28,%r0; be L15; nop
add %r29,%r24,%r28
sll %r28,2,%r28
set _down,%r27
ld [%r28+%r27],%r28
cmp %r28,%r0; be L15; nop
add %r29,%r24,%r28
sll %r28,2,%r28
set _down,%r27
st %r0,[%r28+%r27]
sub %r29,%r24,%r28
sll %r28,2,%r28
set _up+28,%r27
st %r0,[%r28+%r27]
sll %r29,2,%r28
set _rows,%r27
st %r0,[%r28+%r27]
sll %r24,2,%r28
set _x,%r27
st %r29,[%r28+%r27]
cmp %r24,7; bne L19; nop
call _print; nop
ba L20; nop
L19:
add %r24,1,%r8
call _queens; nop
L20:
set 1,%r28
add %r29,%r24,%r27
sll %r27,2,%r27
set _down,%r26
st %r28,[%r27+%r26]
sub %r29,%r24,%r27
sll %r27,2,%r27
set _up+28,%r26
st %r28,[%r27+%r26]
sll %r29,2,%r27
set _rows,%r26
st %r28,[%r27+%r26]
L15:
L12:
add %r29,1,%r29
cmp %r29,8; bl L11; nop
L10:
ret; restore
.global _print
.align 4
.proc 4
_print:save %sp,-96,%sp
mov %r0,%r29
L23:
set L27,%r8
sll %r29,2,%r28
set _x,%r27
ld [%r28+%r27],%r28
add %r28,49,%r9
call _printf; nop
L24:
add %r29,1,%r29
cmp %r29,8; bl L23; nop
set L28,%r8
call _printf; nop
L22:
ret; restore
.seg "bss"
.global _x
.align 4; .common _x,32
.global _rows
.align 4; .common _rows,32
.global _down
.align 4; .common _down,60
.global _up
.align 4; .common _up,60
.seg "text"
L28:.byte 10,0
L27:.ascii "%c \000"

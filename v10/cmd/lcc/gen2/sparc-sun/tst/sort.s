.seg "data"
.global _in
.align 4; _in:.word 10
.word 32
.word -1
.word 567
.word 3
.word 18
.word 1
.word -51
.word 789
.word 0
.seg "text"
.global _main
.align 4
.proc 4
_main:save %sp,-96,%sp
set _in,%r8
set 10,%r9
call _sort; nop
mov %r0,%r29
ba L5; nop
L2:
sll %r29,2,%r28
set _in,%r27
ld [%r28+%r27],%r8
call _putd; nop
set 10,%r8
call _putchar; nop
L3:
add %r29,1,%r29
L5:
cmp %r29,10; blu L2; nop
mov %r0,%r24
L1:
ret; restore
.global _putd
.align 4
.proc 4
_putd:save %sp,-96,%sp
cmp %r24,%r0; bge L7; nop
set 45,%r8
call _putchar; nop
sub %g0,%r24,%r24
L7:
set 10,%r29
mov %r29,%o1; call .div,2; mov %r24,%o0; mov %o0,%r29
cmp %r29,%r0; be L9; nop
set 10,%r29
mov %r29,%o1; call .div,2; mov %r24,%o0; mov %o0,%r29
mov %r29,%r8
call _putd; nop
L9:
set 10,%r29
mov %r29,%o1; call .rem,2; mov %r24,%o0; mov %o0,%r29
add %r29,48,%r8
call _putchar; nop
L6:
ret; restore
.global _sort
.align 4
.proc 4
_sort:save %sp,-96,%sp
set _xx,%r29
st %r24,[%r29]
mov %r24,%r8
mov %r0,%r9
sub %r25,1,%r29
mov %r29,%r25
mov %r29,%r10
call _quick; nop
L11:
ret; restore
.global _quick
.align 4
.proc 4
_quick:save %sp,-96,%sp
cmp %r25,%r26; bl L13; nop
ba L12; nop
L13:
mov %r24,%r8
mov %r25,%r9
mov %r26,%r10
call _partition; nop
mov %r8,%r29
mov %r24,%r8
mov %r25,%r9
sub %r29,1,%r10
call _quick; nop
mov %r24,%r8
add %r29,1,%r9
mov %r26,%r10
call _quick; nop
L12:
ret; restore
.global _partition
.align 4
.proc 4
_partition:save %sp,-96,%sp
add %r26,1,%r26
mov %r25,%r28
sll %r28,2,%r27
ld [%r27+%r24],%r29
ba L17; nop
L16:
add %r25,1,%r25
ba L20; nop
L19:
add %r25,1,%r25
L20:
sll %r25,2,%r27
ld [%r27+%r24],%r27
cmp %r27,%r29; bl L19; nop
sub %r26,1,%r26
ba L23; nop
L22:
sub %r26,1,%r26
L23:
sll %r26,2,%r27
ld [%r27+%r24],%r27
cmp %r27,%r29; bg L22; nop
cmp %r25,%r26; bge L25; nop
sll %r25,2,%r27
add %r27,%r24,%r8
sll %r26,2,%r27
add %r27,%r24,%r9
call _exchange; nop
L25:
L17:
cmp %r25,%r26; bl L16; nop
sll %r28,2,%r27
add %r27,%r24,%r8
sll %r26,2,%r27
add %r27,%r24,%r9
call _exchange; nop
mov %r26,%r24
L15:
ret; restore
.global _exchange
.align 4
.proc 4
_exchange:save %sp,-96,%sp
set _xx,%r28
ld [%r28],%r28
sub %r24,%r28,%r27
set 4,%r26
mov %r26,%o1; call .div,2; mov %r27,%o0; mov %o0,%r27
sub %r25,%r28,%r28
mov %r26,%o1; call .div,2; mov %r28,%o0; mov %o0,%r28
set L28,%r8
mov %r27,%r9
mov %r28,%r10
call _printf; nop
ld [%r24],%r29
ld [%r25],%r28
st %r28,[%r24]
st %r29,[%r25]
L27:
ret; restore
.seg "bss"
.global _xx
.align 4; .common _xx,4
.seg "text"
L28:.ascii "exchange(%d,%d)\012\000"

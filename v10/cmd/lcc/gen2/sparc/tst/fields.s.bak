.seg "data"
.global _x
.align 4; _x:.word 1
.byte 2
.skip 3
.byte 0
.byte 52
.skip 2
.byte 10
.byte 6
.skip 2
.global _i
.align 4; _i:.word 16
.global _y
.align 4; _y:.byte 224
.skip 3
.byte 0
.byte 0
.byte 0
.byte 9
.seg "text"
.global _main
.align 4
.proc 4
_main:save %sp,-96,%sp
set L4,%r8
set _x,%r29
ld [%r29],%r9
set _x+4,%r29
ldsb [%r29],%r10
set _x+8,%r29
ld [%r29],%r29
sra %r29,20,%r11
set _x+8,%r29
ld [%r29],%r29
sll %r29,12,%r29
sra %r29,28,%r12
set _x+12,%r29
ld [%r29],%r29
sll %r29,4,%r29
sra %r29,29,%r13
set _x+13,%r29
ldsb [%r29],%r29
st %r29,[%sp+4*6+68]
call _printf; nop
set L10,%r8
set _y,%r29
ld [%r29],%r29
srl %r29,30,%r9
sll %r29,2,%r29
srl %r29,28,%r10
set _y+4,%r29
ld [%r29],%r11
call _printf; nop
set _x+8,%r29
ld [%r29],%r28
set 0xfff0ffff,%r27
and %r28,%r27,%r28
set _i,%r27
ld [%r27],%r27
sll %r27,28,%r27
sra %r27,28,%r27
sll %r27,16,%r27
set 0xf0000,%r26
and %r27,%r26,%r27
or %r28,%r27,%r28
st %r28,[%r29]
set _x+12,%r29
ld [%r29],%r28
set 0xf1ffffff,%r27
and %r28,%r27,%r28
st %r28,[%r29]
set L4,%r8
set _x,%r29
ld [%r29],%r9
set _x+4,%r29
ldsb [%r29],%r10
set _x+8,%r29
ld [%r29],%r29
sra %r29,20,%r11
set _x+8,%r29
ld [%r29],%r29
sll %r29,12,%r29
sra %r29,28,%r12
set _x+12,%r29
ld [%r29],%r29
sll %r29,4,%r29
sra %r29,29,%r13
set _x+13,%r29
ldsb [%r29],%r29
st %r29,[%sp+4*6+68]
call _printf; nop
set _y,%r29
ld [%r29],%r28
set 0x3fffffff,%r27
and %r28,%r27,%r28
set 0x80000000,%r27
or %r28,%r27,%r28
st %r28,[%r29]
set _i,%r29
ld [%r29],%r29
set _y+4,%r28
st %r29,[%r28]
set L10,%r8
set _y,%r29
ld [%r29],%r29
srl %r29,30,%r9
sll %r29,2,%r29
srl %r29,28,%r10
set _y+4,%r29
ld [%r29],%r11
call _printf; nop
set _x,%r8
call _f2; nop
L3:
ret; restore
.global _f1
.align 4
.proc 4
_f1:save %sp,-96,%sp
ld [%r24],%r29
set 0xc3ffffff,%r28
and %r29,%r28,%r29
st %r29,[%r24]
ld [%r24],%r29
set 0x3fffffff,%r28
and %r29,%r28,%r29
and %r0,3,%r28
sll %r28,30,%r28
set 0xc0000000,%r27
and %r28,%r27,%r28
or %r29,%r28,%r29
st %r29,[%r24]
ld [%r24],%r29
set 0x3c000000,%r28
and %r29,%r28,%r29
cmp %r29,%r0; be L22; nop
set L24,%r8
call _printf; nop
L22:
ld [%r24],%r29
set 0xc0000000,%r28
or %r29,%r28,%r29
st %r29,[%r24]
ld [%r24],%r29
set 0x3c000000,%r28
or %r29,%r28,%r29
st %r29,[%r24]
set L25,%r8
ld [%r24],%r29
srl %r29,30,%r9
sll %r29,2,%r29
srl %r29,28,%r10
call _printf; nop
L21:
ret; restore
.global _f2
.align 4
.proc 4
_f2:save %sp,-96,%sp
set _i,%r28
ld [%r28],%r28
cmp %r28,%r0; bne L28; nop
set 1,%r29
ba L29; nop
L28:
mov %r0,%r29
L29:
ld [%r24],%r28
set 0x3fffffff,%r27
and %r28,%r27,%r28
and %r29,3,%r27
sll %r27,30,%r27
set 0xc0000000,%r26
and %r27,%r26,%r27
or %r28,%r27,%r28
st %r28,[%r24]
mov %r24,%r8
call _f1; nop
ld [%r24],%r29
set 0xc3ffffff,%r28
and %r29,%r28,%r29
and %r0,15,%r28
sll %r28,26,%r28
set 0x3c000000,%r27
and %r28,%r27,%r28
or %r29,%r28,%r29
st %r29,[%r24]
L26:
ret; restore
.seg "text"
L25:.ascii "p->a = 0x%x, p->b = 0x%x\012\000"
L24:.ascii "p->b != 0!\012\000"
L10:.ascii "y = %d %d %d\012\000"
L4:.ascii "x = %d %d %d %d %d %d\012\000"

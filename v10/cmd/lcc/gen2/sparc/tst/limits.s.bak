.seg "text"
.global _main
.align 4
.proc 4
_main:save %sp,-96,%sp
set L2,%r8
set 255,%r29
mov %r29,%r9
mov %r29,%r10
call _printf; nop
set L3,%r8
set 65535,%r29
mov %r29,%r9
mov %r29,%r10
call _printf; nop
set L4,%r8
set 0xffffffff,%r29
mov %r29,%r9
mov %r29,%r10
call _printf; nop
set L5,%r8
set 0xffffffff,%r29
mov %r29,%r9
mov %r29,%r10
call _printf; nop
set L6,%r8
set 127,%r29
mov %r29,%r9
mov %r29,%r10
call _printf; nop
set L7,%r8
set 127,%r29
mov %r29,%r9
mov %r29,%r10
call _printf; nop
set L8,%r8
set 32767,%r29
mov %r29,%r9
mov %r29,%r10
call _printf; nop
set L9,%r8
set 2147483647,%r29
mov %r29,%r9
mov %r29,%r10
call _printf; nop
set L10,%r8
set 2147483647,%r29
mov %r29,%r9
mov %r29,%r10
call _printf; nop
set L11,%r8
set -128,%r29
mov %r29,%r9
mov %r29,%r10
call _printf; nop
set L12,%r8
set -128,%r29
mov %r29,%r9
mov %r29,%r10
call _printf; nop
set L13,%r8
set -32768,%r29
mov %r29,%r9
mov %r29,%r10
call _printf; nop
set L14,%r8
set -2147483648,%r29
mov %r29,%r9
mov %r29,%r10
call _printf; nop
set L15,%r8
set -2147483648,%r29
mov %r29,%r9
mov %r29,%r10
call _printf; nop
L1:
ret; restore
.seg "text"
L15:.ascii "LONG_MIN:\011%08x=%d\012\000"
L14:.ascii "INT_MIN:\011%08x=%d\012\000"
L13:.ascii "SHRT_MIN:\011%08x=%d\012\000"
L12:.ascii "SCHAR_MIN:\011%08x=%d\012\000"
L11:.ascii "CHAR_MIN:\011%08x=%d\012\000"
L10:.ascii "LONG_MAX:\011%08x=%d\012\000"
L9:.ascii "INT_MAX:\011%08x=%d\012\000"
L8:.ascii "SHRT_MAX:\011%08x=%d\012\000"
L7:.ascii "SCHAR_MAX:\011%08x=%d\012\000"
L6:.ascii "CHAR_MAX:\011%08x=%d\012\000"
L5:.ascii "ULONG_MAX:\011%08x=%d\012\000"
L4:.ascii "UINT_MAX:\011%08x=%d\012\000"
L3:.ascii "USHRT_MAX:\011%08x=%d\012\000"
L2:.ascii "UCHAR_MAX:\011%08x=%d\012\000"

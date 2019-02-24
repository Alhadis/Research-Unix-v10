.text
.globl _main
.align 2
_main:link a6,#-12
movl #L2,sp@
movl #255,sp@(4)
movl #255,sp@(8)
jbsr _printf:l
movl #L3,sp@
movl #65535,sp@(4)
movl #65535,sp@(8)
jbsr _printf:l
movl #L4,sp@
movl #0xffffffff,sp@(4)
movl #0xffffffff,sp@(8)
jbsr _printf:l
movl #L5,sp@
movl #0xffffffff,sp@(4)
movl #0xffffffff,sp@(8)
jbsr _printf:l
movl #L6,sp@
movl #127,sp@(4)
movl #127,sp@(8)
jbsr _printf:l
movl #L7,sp@
movl #127,sp@(4)
movl #127,sp@(8)
jbsr _printf:l
movl #L8,sp@
movl #32767,sp@(4)
movl #32767,sp@(8)
jbsr _printf:l
movl #L9,sp@
movl #2147483647,sp@(4)
movl #2147483647,sp@(8)
jbsr _printf:l
movl #L10,sp@
movl #2147483647,sp@(4)
movl #2147483647,sp@(8)
jbsr _printf:l
movl #L11,sp@
movl #-128,sp@(4)
movl #-128,sp@(8)
jbsr _printf:l
movl #L12,sp@
movl #-128,sp@(4)
movl #-128,sp@(8)
jbsr _printf:l
movl #L13,sp@
movl #-32768,sp@(4)
movl #-32768,sp@(8)
jbsr _printf:l
movl #L14,sp@
movl #-2147483648,sp@(4)
movl #-2147483648,sp@(8)
jbsr _printf:l
movl #L15,sp@
movl #-2147483648,sp@(4)
movl #-2147483648,sp@(8)
jbsr _printf:l
L1:
unlk a6
rts
.text
.align 2; L15:.ascii "LONG_MIN:\011%08x=%d\012\000"
.align 2; L14:.ascii "INT_MIN:\011%08x=%d\012\000"
.align 2; L13:.ascii "SHRT_MIN:\011%08x=%d\012\000"
.align 2; L12:.ascii "SCHAR_MIN:\011%08x=%d\012\000"
.align 2; L11:.ascii "CHAR_MIN:\011%08x=%d\012\000"
.align 2; L10:.ascii "LONG_MAX:\011%08x=%d\012\000"
.align 2; L9:.ascii "INT_MAX:\011%08x=%d\012\000"
.align 2; L8:.ascii "SHRT_MAX:\011%08x=%d\012\000"
.align 2; L7:.ascii "SCHAR_MAX:\011%08x=%d\012\000"
.align 2; L6:.ascii "CHAR_MAX:\011%08x=%d\012\000"
.align 2; L5:.ascii "ULONG_MAX:\011%08x=%d\012\000"
.align 2; L4:.ascii "UINT_MAX:\011%08x=%d\012\000"
.align 2; L3:.ascii "USHRT_MAX:\011%08x=%d\012\000"
.align 2; L2:.ascii "UCHAR_MAX:\011%08x=%d\012\000"

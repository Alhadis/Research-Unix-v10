.set reorder
.text
.globl main
.text
.align 2; .ent main
main:
$fp1=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0x80000000,-8
sw $31,16($sp)
la $4,L.2
li $25,255
move $5,$25
move $6,$25
jal printf  # int
la $4,L.3
li $25,65535
move $5,$25
move $6,$25
jal printf  # int
la $4,L.4
li $25,0xffffffff
move $5,$25
move $6,$25
jal printf  # int
la $4,L.5
li $25,0xffffffff
move $5,$25
move $6,$25
jal printf  # int
la $4,L.6
li $25,127
move $5,$25
move $6,$25
jal printf  # int
la $4,L.7
li $25,127
move $5,$25
move $6,$25
jal printf  # int
la $4,L.8
li $25,32767
move $5,$25
move $6,$25
jal printf  # int
la $4,L.9
li $25,2147483647
move $5,$25
move $6,$25
jal printf  # int
la $4,L.10
li $25,2147483647
move $5,$25
move $6,$25
jal printf  # int
la $4,L.11
li $25,-128
move $5,$25
move $6,$25
jal printf  # int
la $4,L.12
li $25,-128
move $5,$25
move $6,$25
jal printf  # int
la $4,L.13
li $25,-32768
move $5,$25
move $6,$25
jal printf  # int
la $4,L.14
li $25,-2147483648
move $5,$25
move $6,$25
jal printf  # int
la $4,L.15
li $25,-2147483648
move $5,$25
move $6,$25
jal printf  # int
L.1:
lw $31,16($sp)
addu $sp,$sp,24
j $31
.end main
.rdata
L.15:.ascii "LONG_MIN:\011%08x=%d\012\000"
L.14:.ascii "INT_MIN:\011%08x=%d\012\000"
L.13:.ascii "SHRT_MIN:\011%08x=%d\012\000"
L.12:.ascii "SCHAR_MIN:\011%08x=%d\012\000"
L.11:.ascii "CHAR_MIN:\011%08x=%d\012\000"
L.10:.ascii "LONG_MAX:\011%08x=%d\012\000"
L.9:.ascii "INT_MAX:\011%08x=%d\012\000"
L.8:.ascii "SHRT_MAX:\011%08x=%d\012\000"
L.7:.ascii "SCHAR_MAX:\011%08x=%d\012\000"
L.6:.ascii "CHAR_MAX:\011%08x=%d\012\000"
L.5:.ascii "ULONG_MAX:\011%08x=%d\012\000"
L.4:.ascii "UINT_MAX:\011%08x=%d\012\000"
L.3:.ascii "USHRT_MAX:\011%08x=%d\012\000"
L.2:.ascii "UCHAR_MAX:\011%08x=%d\012\000"

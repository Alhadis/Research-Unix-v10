# 1 "doprnt.nS"

	# C library -- conversions

.globl	__doprnt
.globl	__flsbuf






















	.set ch.zer,'0			# cpp doesn't like single appostrophes
	.set ch.per,'%			# I'm convinced BE

	.align 2


















strfoo:
	clrl r4					# fix interrupt race
	jbr strok				# and try again
strmore:
	movzbl (r1)+,r2			# one char

	jeql stresc2			# BE
	cmpl r2,$ch.per			# BE
	jeql stresc2			# bad guy in disguise (outbuf is full)
strout2:		# enter here to force out r2; r0,r1 must be set

	movq r0,-(sp)	# BE
	pushl -4(fp)				# FILE
	pushl r2				# the char
	calls $2,__flsbuf		# please empty the buffer and handle 1 char
	tstl r0					# successful?
	jgeq strm1				# yes
	jbcs $31,-16(fp),strm1	# turn on -17(fp) bit of -16(fp) to signify error
strm1:
	incl -16(fp)				# count the char

	movq r0,(sp)+		# BE
strout:			# enter via bsb with (r0,r1)=input descriptor

	movq *-4(fp),r4			# output descriptor
	jbs $31,r4,strfoo		# negative count is a no no
strok:
	addl2 r0,-16(fp)			# we intend to move this many chars

	clrl r2
	tstl    r0
	bleq    movdon
movlp:
	tstl    r4
	bleq    movdon
	movzbl  (r1)+,r3

	beql	2f		# BE
	cmpl	r3,$ch.per	# BE
	bneq    1f
2:				# BE
	mnegl   $1,r2
	decl    r1
	brb     movdon
1:
	movb    r3,(r5)+
	decl    r4
	sobgtr  r0,movlp
  


  
movdon: movq r4,*-4(fp)                  
	subl2 r0,-16(fp)			# some chars not moved
	jbs $1,r2,stresc		# terminated by escape?
	sobgeq r0,strmore		# no; but out buffer might be full
stresc:
	rsb
stresc2:
	incl r0					# fix the length
	decl r1					# and the addr
	movl $1<1,r2			# fake condition codes
	rsb

errdone:
	jbcs $31,-16(fp),prdone	# set error bit
prdone:
	movl -16(fp),r0
	ret

	.align	1
__doprnt:
	.word	0xfc0			# uses r11-r6
	movab -256(sp),sp		# work space
	movl 4(ap),r11			# addr of format string
	movl 12(ap),-4(fp)		# output FILE ptr
	movl 8(ap),ap			# addr of first arg
	clrl -16(fp)				# number of chars transferred
loop:
	movzwl $65535,r0		# pseudo length
	movl r11,r1				# fmt addr
		# comet sucks.
	movq *-4(fp),r4
	subl3 r1,r5,r2
	jlss lp1
	cmpl r0,r2
	jleq lp1
	movl r2,r0
lp1:
		#
	bsbw strout				# copy to output, stop at null or percent
	movl r1,r11				# new fmt
	jbc $1,r2,loop		# if no escape, then very long fmt
	tstb (r11)+				# escape; null or percent?
	jeql prdone				# null means end of fmt

	movl sp,r5			# reset output buffer pointer
	clrq r9				# r9; r10
	clrq r6				# r6,r7
longorunsg:				# we can ignore both of these distinctions
short:
L4a:
	movzbl (r11)+,r0		# so capital letters can tail merge
L4:	caseb r0,$' ,$'x-' 		# format char
L5:
	.word space-L5			# space
	.word fmtbad-L5			# !
	.word fmtbad-L5			# "
	.word sharp-L5			# #
	.word fmtbad-L5			# $
	.word fmtbad-L5			# %
	.word fmtbad-L5			# &
	.word fmtbad-L5			# '
	.word fmtbad-L5			# (
	.word fmtbad-L5			# )
	.word indir-L5			# *
	.word plus-L5			# +
	.word fmtbad-L5			# ,
	.word minus-L5			# -
	.word dot-L5			# .
	.word fmtbad-L5			# /
	.word gnum0-L5			# 0
	.word gnum-L5			# 1
	.word gnum-L5			# 2
	.word gnum-L5			# 3
	.word gnum-L5			# 4
	.word gnum-L5			# 5
	.word gnum-L5			# 6
	.word gnum-L5			# 7
	.word gnum-L5			# 8
	.word gnum-L5			# 9
	.word fmtbad-L5			# :
	.word fmtbad-L5			# ;
	.word fmtbad-L5			# <
	.word fmtbad-L5			# =
	.word fmtbad-L5			# >
	.word fmtbad-L5			# ?
	.word fmtbad-L5			# @
	.word fmtbad-L5			# A
	.word fmtbad-L5			# B
	.word fmtbad-L5			# C
	.word decimal-L5		# D
	.word capital-L5		# E
	.word fmtbad-L5			# F
	.word capital-L5		# G
	.word fmtbad-L5			# H
	.word fmtbad-L5			# I
	.word fmtbad-L5			# J
	.word fmtbad-L5			# K
	.word fmtbad-L5			# L
	.word fmtbad-L5			# M
	.word fmtbad-L5			# N
	.word octal-L5			# O
	.word fmtbad-L5			# P
	.word fmtbad-L5			# Q
	.word fmtbad-L5			# R
	.word fmtbad-L5			# S
	.word fmtbad-L5			# T
	.word unsigned-L5		# U
	.word fmtbad-L5			# V
	.word fmtbad-L5			# W
	.word capital-L5		# X
	.word fmtbad-L5			# Y
	.word fmtbad-L5			# Z
	.word fmtbad-L5			# [
	.word fmtbad-L5			# \
	.word fmtbad-L5			# ]
	.word fmtbad-L5			# ^
	.word fmtbad-L5			# _
	.word fmtbad-L5			# `
	.word fmtbad-L5			# a
	.word fmtbad-L5			# b
	.word charac-L5			# c
	.word decimal-L5		# d
	.word scien-L5			# e
	.word float-L5			# f
	.word general-L5		# g
	.word short-L5			# h
	.word fmtbad-L5			# i
	.word fmtbad-L5			# j
	.word fmtbad-L5			# k
	.word longorunsg-L5		# l
	.word fmtbad-L5			# m
	.word fmtbad-L5			# n
	.word octal-L5			# o
	.word fmtbad-L5			# p
	.word fmtbad-L5			# q
	.word fmtbad-L5			# r
	.word string-L5			# s
	.word fmtbad-L5			# t
	.word unsigned-L5		# u
	.word fmtbad-L5			# v
	.word fmtbad-L5			# w
	.word hex-L5			# x
fmtbad:
	movb r0,(r5)+			# print the unfound character
	jeql errdone			# dumb users who end the format with a %
	jbr prbuf
capital:
	bisl2 $1<6,r10		# note that it was capitalized
	xorb2 $'a^'A,r0			# make it small
	jbr L4					# and try again

string:
	movl r8,r0
	jbs $1,r10,L20		# max length was specified
	mnegl $1,r0			# default max length
L20:	movl (ap)+,r2			# addr first byte
	locc $0,r0,(r2)			# find the zero at the end
	movl r1,r5			# addr last byte +1
	movl r2,r1			# addr first byte
	jbr prstr

htab:	.byte	'0,'1,'2,'3,'4,'5,'6,'7,'8,'9,'a,'b,'c,'d,'e,'f
Htab:	.byte	'0,'1,'2,'3,'4,'5,'6,'7,'8,'9,'A,'B,'C,'D,'E,'F

octal:
	movl $30,r2			# init position
	movl $3,r3			# field r9
	movab htab,r7	# translate table
	jbr L10

hex:
	movl $28,r2			# init position
	movl $4,r3			# field r9
	movab htab,r7	# translate table
	jbc $6,r10,L10
	movab Htab,r7
L10:	mnegl r3,r6			# increment
	clrl r1
	addl2 $4,r5			# room for left affix (2) and slop [forced -17(fp)?]
	movl (ap)+,r0			# fetch arg
L11:	extzv r2,r3,r0,r1		# pull out a digit
	movb (r7)[r1],(r5)+		# convert to character
L12:	acbl $0,r6,r2,L11		# continue until done
	clrq r6				# r6, r7
	clrb (r5)			# flag end
	skpc $'0,$11,4(sp)		# skip over leading zeroes
	jbc $5,r10,prn3	# easy if no left affix
	tstl -4(ap)				# original value
	jeql prn3			# no affix on 0, for some reason
	cmpl r3,$4			# were we doing hex or octal?
	jneq L12a			# octal
	movb $'x,r0
	jbc $6,r10,L12b
	movb $'X,r0
L12b:	movb r0,-(r1)
	movl $2,r7		# leading 0x for hex is an affix
L12a:	movb $'0,-(r1)	# leading zero for octal is a digit, not an affix
	jbr prn3			# omit -17(fp) (plus, 7) massaging

unsigned:
lunsigned:
	bicl2 $1<4|1<7,r10	# omit -17(fp) (plus, 7) massaging
	extzv $1,$31,(ap),r0		# right shift logical 1 bit
	cvtlp r0,$10,(sp)		# convert [n/2] to packed
	movp $10,(sp),8(sp)		# copy packed
	addp4 $10,8(sp),$10,(sp)	# 2*[n/2] in packed, at (sp)
	blbc (ap)+,L14			# n was even
	addp4 $1,pone,$10,(sp)		# n was odd
	jbr L14

patdec:					# editpc pattern for decimal printing
	.byte 0xAA			# eo$float 10
	.byte 0x01			# eo$end_float
	.byte 0				# eo$end

decimal:
	cvtlp (ap)+,$10,(sp)		# 10 digits max
	jgeq L14
	incl r7			# minus -17(fp) is a left affix
L14:	editpc $10,(sp),patdec,8(sp)	# ascii at 8(sp); r5=end+1
	skpc $' ,$11,8(sp)		# skip leading blanks; r1=first

prnum:			# r1=addr first byte, r5=addr last byte +1, r7=size of signs
				# -1(r1) vacant, for forced -17(fp)
	tstl r7
	jneq prn3			# already some left affix, dont fuss
	jbc $4,r10,prn2
	movb $'+,-(r1)		# needs a plus sign
	jbr prn4
prn2:	jbc $7,r10,prn3
	movb $' ,-(r1)		# needs a blank sign
prn4:	incl r7
prn3:	jbs $1,r10,prn1
	movl $1,r8		# default precision is 1
prn1:	subl3 r1,r5,r6	# raw r9
	subl2 r7,r6	# number of digits
	subl2 r6,r8	# number of leading zeroes needed
	jleq prstr			# none
	addl2 r7,r1		# where current digits start
	pushl r1			# movcx gobbles registers
		# check bounds on users who say %.300d
	movab 32(r5)[r8],r2
	subl2 fp,r2
	jlss prn5
	subl2 r2,r8
prn5:
		#
	movc3 r6,(r1),(r1)[r8]	# make room in middle
	movc5 $0,(r1),$ch.zer,r8,*(sp)	# '0 fill
	subl3 r7,(sp)+,r1	# first byte addr
	addl3 r6,r3,r5	# last byte addr +1

prstr:			# r1=addr first byte; r5=addr last byte +1
				# r9=minimum r9; r7=len. left affix
				# r8=<avail>
	subl3 r1,r5,r8		# raw r9
	subl3 r8,r9,r0	# pad length
	jleq padlno				# in particular, no left padding
	jbs $3,r10,padlno
			# extension for %0 flag causing left zero padding to field r9
	jbs $2,r10,padlz
			# this bsbb needed even if %0 flag extension is removed
	bsbb padb				# 7 pad on left
	jbr padnlz
padlz:
	movl r7,r0
	jleq padnlx				# left zero pad requires left affix first
	subl2 r0,r8			# part of total length will be transferred
	subl2 r0,r9			# and will account for part of minimum r9
	bsbw strout				# left affix
padnlx:
	subl3 r8,r9,r0	# pad length
	bsbb padz				# zero pad on left
padnlz:
			# end of extension for left zero padding
padlno:			# remaining: root, possible right padding
	subl2 r8,r9		# root reduces minimum r9
	movl r8,r0			# root length
p1:	bsbw strout				# transfer to output buffer
p3:	jbc $1,r2,padnpct	# percent -17(fp) (or null byte via %c) ?
	decl r0					# yes; adjust count
	movzbl (r1)+,r2			# fetch byte
	movq *-4(fp),r4			# output buffer descriptor
	sobgeq r4,p2			# room at the out [inn] ?
	bsbw strout2			# no; force it, then try rest
	jbr p3					# here we go 'round the mullberry bush, ...
p2:	movb r2,(r5)+			# hand-deposit the percent or null
	incl -16(fp)				# count it
	movq r4,*-4(fp)			# store output descriptor
	jbr p1					# what an expensive hiccup!
padnpct:
	movl r9,r0	# size of pad
	jleq loop
	bsbb padb
	jbr loop

padz:
	movb $'0,r2
	jbr pad
padb:
	movb $' ,r2
pad:
	subl2 r0,r9			# pad r9 decreases minimum r9
	pushl r1				# save non-pad addr
	movl r0,r7			# remember r9 of pad
	subl2 r0,sp				# allocate
	movc5 $0,(r0),r2,r7,(sp)	# create pad string
	movl r7,r0			# length
	movl sp,r1				# addr
	bsbw strout
	addl2 r7,sp			# deallocate
	movl (sp)+,r1			# recover non-pad addr
	rsb

pone:	.byte	0x1C			# packed 1
	
charac:
	movl (ap)+,r0		# word containing the char
	movb r0,(r5)+		# one byte, that's all

prbuf:
	movl sp,r1			# addr first byte
	jbr prstr

space:	bisl2 $1<7,r10		# constant r9 e fmt, no plus -17(fp)
	jbr L4a
sharp:	bisl2 $1<5,r10		# 'self identifying', please
	jbr L4a
plus:	bisl2 $1<4,r10		# always print -17(fp) for floats
	jbr L4a
minus:	bisl2 $1<3,r10		# left justification, please
	jbr L4a
gnum0:	jbs $0,r10,gnum
	jbs $1,r10,gnump		# ignore when reading precision
	bisl2 $1<2,r10		# leading zero fill, please
gnum:	jbs $1,r10,gnump
	moval (r9)[r9],r9	# r9 *= 5;
	movaw -ch.zer(r0)[r9],r9	# r9 = 2*witdh + r0 - '0';
	jbr gnumd
gnump:	moval (r8)[r8],r8	# r8 *= 5;
	movaw -ch.zer(r0)[r8],r8 # r8 = 2*r8 + r0 - '0';
gnumd:	bisl2 $1<0,r10		# digit seen
	jbr L4a
dot:	clrl r8			# start on the precision
	bisl2 $1<1,r10
	bicl2 $1<0,r10
	jbr L4a
indir:
	jbs $1,r10,in1
	movl (ap)+,r9		# r9 specified by parameter
	jgeq gnumd
	xorl2 $1<3,r10		# parameterized left adjustment
	mnegl r9,r9
	jbr gnumd
in1:
	movl (ap)+,r8		# precision specified by paratmeter
	jgeq gnumd
	mnegl r8,r8
	jbr gnumd

float:
	jbs $1,r10,float1
	movl $6,r8			# default # digits to right of decpt.
float1:	bsbw fltcvt
	addl3 -8(fp),r8,r7
	movl r7,r6			# for later "underflow" checking
	bgeq fxplrd
	clrl r7				# poor programmer planning
fxplrd:	cmpl r7,$31			# expressible in packed decimal?
	bleq fnarro			# yes
	movl $31,r7
fnarro:	subl3 $17,r7,r0			# where to round
	ashp r0,$17,(sp),$5,r7,16(sp)	# do it
	bvc fnovfl
		# band-aid for microcode error (spurious overflow)
	#	clrl r0				# assume even length result
	#	jlbc r7,fleven			# right
	#	movl $4,r0			# odd length result
	#fleven:	cmpv r0,$4,16(sp),$0		# top digit zero iff true overflow
	#	bneq fnovfl
		# end band-aid
	aobleq $0,r6,fnovfl		# if "underflow" then jump
	movl r7,r0
	incl -8(fp)
	incl r7
	ashp r0,$1,pone,$0,r7,16(sp)
	ashl $-1,r7,r0			# displ to last byte
	bisb2 -17(fp),16(sp)[r0]		# insert -17(fp)
fnovfl:
	movab 16(sp),r1		# packed source
	movl r7,r6		# packed length
	pushab prnum	# goto prnum after fall-through call to fedit


	# enter via bsb
	#	r1=addr of packed source
	#	   16(r1) used to unpack source
	#	   48(r1) used to construct pattern to unpack source
	#	   48(r1) used to hold result
	#	r6=length of packed source (destroyed)
	#	-8(fp)=# digits to left of decimal point (destroyed)
	#	r8=# digits to right of decimal point (destroyed)
	#	-17(fp)=1 if negative, 0 otherwise
	# stack will be used for work space for pattern and unpacked source 
	# exits with
	#	r1=addr of punctuated result
	#	r5=addr of last byte +1
	#	r7=1 if minus -17(fp) inserted, 0 otherwise
fedit:
	pushab 48(r1)			# save result addr
	movab 48(r1),r3			# pattern addr
	movb $0x03,(r3)+		# eo$set_signif
	movc5 $0,(r1),$0x91,r6,(r3)	# eo$move 1
	clrb (r3)				# eo$end
	editpc r6,(r1),48(r1),16(r1)	# unpack 'em all
	subl3 r6,r5,r1			# addr unpacked source
	movl (sp),r3			# punctuated output placed here
	clrl r7
	jlbc -17(fp),f1
	movb $'-,(r3)+		# negative
	incl r7
f1:	movl -8(fp),r0
	jgtr f2
	movb $'0,(r3)+		# must have digit before decimal point
	jbr f3
f2:	cmpl r0,r6			# limit on packed length
	jleq f4
	movl r6,r0
f4:	subl2 r0,r6			# eat some digits
	subl2 r0,-8(fp)		# from the exponent
	movc3 r0,(r1),(r3)	# (most of the) digits to left of decimal point
	movl -8(fp),r0			# need any more?
	jleq f3
	movc5 $0,(r1),$'0,r0,(r3)	# '0 fill
f3:	movl r8,r0		# # digits to right of decimal point
	jgtr f5
	jbs $5,r10,f5	# no decimal point unless forced
	jbcs $9,r10,f6	# no decimal point
f5:	movb $'.,(r3)+		# the decimal point
f6:	mnegl -8(fp),r0		# "leading" zeroes to right of decimal point
	jleq f9
	cmpl r0,r8		# cant exceed this many
	jleq fa
	movl r8,r0
fa:	subl2 r0,r8
	movc5 $0,(r1),$'0,r0,(r3)
f9:	movl r8,r0
	cmpl r0,r6			# limit on packed length
	jleq f7
	movl r6,r0
f7:	subl2 r0,r8		# eat some digits from the fraction
	movc3 r0,(r1),(r3)	# (most of the) digits to right of decimal point
	movl r8,r0			# need any more?
	jleq f8
		# check bounds on users who say %.300f
	movab 32(r3)[r0],r2
	subl2 fp,r2
	jlss fb
	subl2 r2,r0			# truncate, willy-nilly
	movl r0,r8		# and no more digits later, either
fb:
		#
	subl2 r0,r8		# eat some digits from the fraction
	movc5 $0,(r1),$'0,r0,(r3)	# '0 fill
f8:	movl r3,r5			# addr last byte +1
	popr $1<1			# [movl (sp)+,r1] addr first byte
	rsb

patexp:	.byte	0x03			# eo$set_signif
	.byte	0x44,'e			# eo$insert 'e
	.byte	0x42,'+			# eo$load_plus '+
	.byte	0x04			# eo$store_sign
	.byte	0x92			# eo$move 2
	.byte	0			# eo$end

scien:
	incl r8
	jbs $1,r10,L23
	movl $7,r8
L23:	bsbw fltcvt			# get packed digits
	movl r8,r7
	cmpl r7,$31				# expressible in packed decimal?
	jleq snarro				# yes
	movl $31,r7
snarro:	subl3 $17,r7,r0		# rounding position
	ashp r0,$17,(sp),$5,r7,16(sp) # shift and round
	bvc snovfl
		# band-aid for microcode error (spurious overflow)
	#	clrl r0				# assume even length result
	#	jlbc r8,sceven		# right
	#	movl $4,r0			# odd length result
	#sceven:	cmpv r0,$4,16(sp),$0		# top digit zero iff true overflow
	#	bneq snovfl
		# end band-aid
	incl -8(fp)			# rounding overflowed to 100...
	subl3 $1,r7,r0
	ashp r0,$1,pone,$0,r7,16(sp)
	ashl $-1,r7,r0		# displ to last byte
	bisb2 -17(fp),16(sp)[r0]		# insert -17(fp)
snovfl:
	jbs $8,r10,gfmt		# %g format
	movab 16(sp),r1
	bsbb eedit
eexp:
	movl r1,r6		# save fwa from destruction by cvtlp
	subl3 $1,-12(fp),r0	# 1P exponent
	cvtlp r0,$2,(sp)	# packed
	editpc $2,(sp),patexp,(r5)
	movl r6,r1		# fwa
	jbc $6,r10,prnum
	xorb2 $'e^'E,-4(r5)
	jbr prnum

eedit:
	movl r7,r6		# packed length
	decl r8		# 1 digit before decimal point
	movl -8(fp),-12(fp)	# save from destruction
	movl $1,-8(fp)		# and pretend
	jbr fedit

gfmt:
	addl3 $3,-8(fp),r0		# -8(fp) is 1 more than e
	jlss gfmte		# (e+1)+3<0, e+4<=-1, e<=-5
	subl2 $3,r0		# -8(fp) [==(e+1)]
	cmpl r0,r8
	jgtr gfmte		# e+1>n, e>=n
gfmtf:
	movl r7,r6
	subl2 r0,r8		# n-e-1
	movab 16(sp),r1
	bsbw fedit
g1:	jbs $5,r10,g2
	jbs $9,r10,g2	# dont strip if no decimal point
g3:	cmpb -(r5),$'0		# strip trailing zeroes
	jeql g3
	cmpb (r5),$'.		# and trailing decimal point
	jeql g2
	incl r5
g2:	jbc $8,r10,eexp
	jbr prnum
gfmte:
	movab 16(sp),r1		# packed source
	bsbw eedit
	jbsc $8,r10,g1	# 8 now means "use %f" [hence no exponent]

general:
	jbs $1,r10,gn1
	movl $6,r8		# default precision is 6 significant digits
gn1:	tstl r8		# cannot allow precision of 0
	jgtr gn2
	movl $1,r8		# change 0 to 1, willy-nilly
gn2:	jbcs $8,r10,L23
	jbr L23			# safety net

	# convert double-floating at (ap) to 17-digit packed at (sp),
	# set 'sign' and 'exp', advance ap.
fltcvt:
	clrb -17(fp)
	movd (ap)+,r5
	jeql fzero
	bgtr fpos
	mnegd r5,r5
	incb -17(fp)
fpos:
	extzv $7,$8,r5,r2		# exponent of 2
	movab -0200(r2),r2		# unbias
	mull2 $59,r2			# 59/196: 3rd convergent continued frac of log10(2)
	jlss eneg
	movab 196(r2),r2
eneg:
	movab -98(r2),r2
	divl2 $196,r2
	bsbw expten
	cmpd r0,r5
	bgtr ceil
	incl r2
ceil:	movl r2,-8(fp)
	mnegl r2,r2
	cmpl r2,$29			# 10^(29+9) is all we can handle
	bleq getman
	muld2 ten16,r5
	subl2 $16,r2
getman:	addl2 $9,r2			# -ceil(log10(x)) + 9
	jsb expten
	emodd r0,r4,r5,r0,r5		# (r0+r4)*r5; r0=int, r5=frac
fz1:	cvtlp r0,$9,16(sp)		# leading 9 digits
	ashp $8,$9,16(sp),$0,$17,4(sp)	# as top 9 of 17
	emodd ten8,$0,r5,r0,r5
	cvtlp r0,$8,16(sp)		# trailing 8 digits
		# if precision >= 17, must round here
	movl r8,r7			# so figure out what precision is
	pushab scien
	cmpl (sp)+,(sp)
	jleq gm1			# who called us?
	addl2 -8(fp),r7			# float; adjust for exponent
gm1:	cmpl r7,$17
	jlss gm2
	cmpd r5,$0d0.5			# must round here; check fraction
	jlss gm2
	bisb2 $0x10,8+4(sp)		# increment l.s. digit
gm2:		# end of "round here" code
	addp4 $8,16(sp),$17,4(sp)	# combine leading and trailing
	bisb2 -17(fp),12(sp)		# and insert -17(fp)
	rsb
fzero:	clrl r0
	movl $1,-8(fp)		# 0.000e+00 and 0.000 rather than 0.000e-01 and .000
	jbr fz1

	.align 2
lsb: .long 0x00010000		# lsb in the crazy floating-point format

	# return 10^r2 as a double float in r0||r1 and 8 extra bits of precision in r4
	# preserve r2, r5||r6
expten:
	movd $0d1.0,r0			# begin computing 10^exp10
	clrl r4				# bit counter
	movad ten1,r3			# table address
	tstl r2
	bgeq e10lp
	mnegl r2,r2			# get absolute value
	jbss $6,r2,e10lp		# flag as negative
e10lp:	jbc r4,r2,el1			# want this power?
	muld2 (r3),r0			# yes
el1:	addl2 $8,r3			# advance to next power
	aobleq $5,r4,e10lp		# through 10^32
	jbcc $6,r2,el2			# correct for negative exponent
	divd3 r0,$0d1.0,r0		# by taking reciprocal
	cmpl $28,r2
	jneq enm28
	addl2 lsb,r1			# 10**-28 needs lsb incremented
enm28:	mnegl r2,r2			# original exponent of 10
el2:	addl3 $5*8,r2,r3		# negative bit positions are illegal?
	jbc r3,xlsbh-5,eoklsb
	subl2 lsb,r1			# lsb was too high
eoklsb:
	movzbl xprec[r2],r4		# 8 extra bits
	rsb

	# powers of ten
	.align	2
ten1:	.word	0x4220,0,0,0
ten2:	.word	0x43c8,0,0,0
ten4:	.word	0x471c,0x4000,0,0
ten8:	.word	0x4dbe,0xbc20,0,0
ten16:	.word	0x5b0e,0x1bc9,0xbf04,0
ten32:	.word	0x759d,0xc5ad,0xa82b,0x70b6

	# whether lsb is too high or not
	.byte 1:0,1:0,1:0,1:0,1:1,1:0,1:1,1:0	# -40 thru -33
	.byte 1:0,1:1,1:0,1:0,1:0,1:0,1:1,1:0	# -32 thru -25
	.byte 1:0,1:0,1:1,1:1,1:1,1:1,1:0,1:0	# -24 thru -17
	.byte 1:0,1:1,1:0,1:0,1:1,1:1,1:1,1:1	# -16 thru -9
	.byte 1:1,1:1,1:1,1:0,1:0,1:0,1:0,1:1	# -8  thru -1
xlsbh:
	.byte 1:0,1:0,1:0,1:0,1:0,1:0,1:0,1:0	# 0 thru 7
	.byte 1:0,1:0,1:0,1:0,1:0,1:0,1:0,1:0	# 8 thru 15
	.byte 1:0,1:0,1:0,1:0,1:0,1:0,1:0,1:0	# 16 thru 23
	.byte 1:0,1:1,1:1,1:0,1:1,1:1,1:1,1:1	# 24 thru 31
	.byte 1:1,1:1,1:1,1:1,1:1,1:1,1:1    	# 32 thru 38

	# bytes of extra precision
	.byte           0x56,0x76,0xd3,0x88,0xb5,0x62	# -38 thru -33
	.byte 0xba,0xf5,0x32,0x3e,0x0e,0x48,0xdb,0x51	# -32 thru -25
	.byte 0x53,0x27,0xb1,0xef,0xeb,0xa5,0x07,0x49	# -24 thru -17
	.byte 0x5b,0xd9,0x0f,0x13,0xcd,0xff,0xbf,0x97	# -16 thru -9
	.byte 0xfd,0xbc,0xb6,0x23,0x2c,0x3b,0x0a,0xcd	# -8  thru -1
xprec:
	.byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	# 0  thru 7
	.byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	# 8  thru 15
	.byte 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	# 16 thru 23
	.byte 0x00,0xa0,0xc8,0x3a,0x84,0xe4,0xdc,0x92	# 24 thru 31
	.byte 0x9b,0x00,0xc0,0x58,0xae,0x18,0xef     	# 32 thru 38

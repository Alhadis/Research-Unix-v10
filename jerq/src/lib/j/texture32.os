	.text
	.globl	texture32
texture32:	
	cmph	8(%ap),4(%ap)		# width in bits
	BGB	dxpos			# width positive ??
	RET				# NO!, return
dxpos:	SAVE	%r3			# lets save all the registers
	addw2	&8,%sp			# local vars

	PUSHW	0(%ap)			# push pointer to bitmap
	PUSHW	4(%ap)			# push rect.origin
	call	&2,addr			# calculate the address of the point
	MOVW	%r0,%r8			# %r8 = addr(bitmap,rect.origin)

	movw	0(%ap),%r7		# pointer to bitmap
	llsw3	&2,4(%r7),%r7		# width of bitmap in bytes -> r7

	movw	12(%ap),%r6		# r6 = pointer to texture

	subh3	6(%ap),10(%ap),%r4	# dy

	andh3	&0x1f,6(%ap),%r0
	llsw3	&2,%r0,%r3		# bit index into texture = r3

	movh	4(%ap),%r2		# rec.origin.x -> r2
	lrsw3	%r2,&-1,0(%fp)		# generate mask1 -> 0(%fp)

	subh3	&1,8(%ap),%r5
	arsw3	%r5,&0x80000000,4(%fp)		# generate mask2 -> 4(%fp)

	lrsw3	&5,%r5,%r5		# (x + dx -1) >> WORDSHIFT
	lrsw3	&5,%r2,%r2		# x >> WORDSHIFT
	subw2	%r2,%r5		
	DECW	%r5			# inner word count ii
	BGEB	notnarrow 		# if ii == -1 then narrow
	andw2	4(%fp),0(%fp)		# narrow, combine both masks here
	mcomw	0(%fp),4(%fp)		# mask2 = ~mask1
	subw2	&4,%r7			# amount to get to next line
	BRB	restcode
notnarrow:
	addw3	&1,%r5,%r0		# offset in words
	llsw3	&2,%r0,%r0		# offset in bytes
	subw2	%r0,%r7			# amount to get to next line
restcode:
	ALSW3	&2,16(%ap),%r0		# code * 4 -> r0
	BLB	end			# bad if code less than 0
	cmpw	%r0,&12			# see if code greater than 3
	jle	*jmptbl(%r0)		# jump to correct routine if ok
end:	subw2	&8,%sp
	RESTORE	%r3
	RET


	.data
jmptbl:	.word	FSTORE
	.word	FOR
	.word	FCLEAR
	.word	FXOR
	.text
FXOR:	DECW	%r4			# number of lines
	jl	end			# if neg. then done
	addw3	%r6,%r3,%r2
	movw	0(%r2),%r2		# bits = map[bitindex]
	addw2	&4,%r3			#bitindex += 1;
	andw2	&0x7f,%r3		# bitindex = ( bitindex + 1) & WORDMASK
	andw3	0(%fp),%r2,%r0		#
	xorw2	%r0,0(%r8)		#screenp ^= bits & mask1
	addw2	&4,%r8			# screenp++
	movw	%r5,%r1			# i = ii
	BLB	restloop		# if i = -1 then narrow
	BEB	mask2ok			# if i = 0 then no inner words
	lrsw3	&2,%r1,%r0		# i/4
	BLEB	less4			# less than 4 inner words
more4:	xorw2	%r2,0(%r8)		# screenp ^= bits
	xorw2	%r2,4(%r8)		# screenp ^= bits
	xorw2	%r2,8(%r8)		# screenp ^= bits
	xorw2	%r2,12(%r8)		# screenp ^= bits
	addw2	&16,%r8			# screenp += 4
	DECW	%r0			
	BGB	more4			# more inner words
	andw2	&3,%r1			# just how many are left
	BLEB	mask2ok			# no more to do
less4:	xorw2	%r2,0(%r8)		# screenp ^= bits
	addw2	&4,%r8			# screenp++
	DECW	%r1			
	BGB	less4			# more inner words
mask2ok:
	andw3	4(%fp),%r2,%r0
	xorw2	%r0,0(%r8)		# screenp ^= (bits & mask2)
restloop:
	addw2	%r7,%r8			# screenp += width
	BRB	FXOR

FCLEAR:	mcomw	0(%fp),0(%fp)
	mcomw	4(%fp),4(%fp)
FCLR: DECW	%r4			# number of lines
	jl	end			# if neg. then done
	addw3	%r6,%r3,%r2
	mcomw	0(%r2),%r2		# bits = ~map[bitindex]
	addw2	&4,%r3			#bitindex += 1;
	andw2	&0x7f,%r3		# bitindex = ( bitindex + 1) & WORDMASK
	orw3	0(%fp),%r2,%r0		#
	andw2	%r0,0(%r8)		#screenp &= bits & mask1
	addw2	&4,%r8			# screenp++
	movw	%r5,%r1			# i = ii
	BLB	restclr		# if i = -1 then narrow
	BEB	msk2clr			# if i = 0 then no inner words
	lrsw3	&2,%r1,%r0		# i/4
	BLEB	less4clr			# less than 4 inner words
Fclr_4:	andw2	%r2,0(%r8)		# screenp ^= bits
	andw2	%r2,4(%r8)		# screenp ^= bits
	andw2	%r2,8(%r8)		# screenp ^= bits
	andw2	%r2,12(%r8)		# screenp ^= bits
	addw2	&16,%r8			# screenp += 4
	DECW	%r0			
	BGB	Fclr_4			# more inner words
	andw2	&3,%r1			# just how many are left
	BLEB	msk2clr			# no more to do
less4clr:
	andw2	%r2,0(%r8)		# screenp ^= bits
	addw2	&4,%r8			# screenp++
	DECW	%r1			
	BGB	less4clr			# more inner words
msk2clr:
	orw3	4(%fp),%r2,%r0
	andw2	%r0,0(%r8)		# screenp ^= (bits & mask2)
restclr:
	addw2	%r7,%r8			# screenp += width
	BRB	FCLR

FOR:	DECW	%r4			# number of lines
	jl	end			# if neg. then done
	addw3	%r6,%r3,%r2
	movw	0(%r2),%r2		# bits = map[bitindex]
	addw2	&4,%r3			#bitindex += 1;
	andw2	&0x7f,%r3		# bitindex = ( bitindex + 1) & WORDMASK
	andw3	0(%fp),%r2,%r0		#
	orw2	%r0,0(%r8)		#screenp ^= bits & mask1
	addw2	&4,%r8			# screenp++
	movw	%r5,%r1			# i = ii
	BLB	rest_or		# if i = -1 then narrow
	BEB	mask2or			# if i = 0 then no inner words
	lrsw3	&2,%r1,%r0		# i/4
	BLEB	less4or			# less than 4 inner words
For_4:	orw2	%r2,0(%r8)		# screenp ^= bits
	orw2	%r2,4(%r8)		# screenp ^= bits
	orw2	%r2,8(%r8)		# screenp ^= bits
	orw2	%r2,12(%r8)		# screenp ^= bits
	addw2	&16,%r8			# screenp += 4
	DECW	%r0			
	BGB	For_4			# more inner words
	andw2	&3,%r1			# just how many are left
	BLEB	mask2or			# no more to do
less4or:
	orw2	%r2,0(%r8)		# screenp ^= bits
	addw2	&4,%r8			# screenp++
	DECW	%r1			
	BGB	less4or			# more inner words
mask2or:
	andw3	4(%fp),%r2,%r0
	orw2	%r0,0(%r8)		# screenp ^= (bits & mask2)
rest_or:
	addw2	%r7,%r8			# screenp += width
	BRB	FOR

FSTORE: DECW	%r4			# number of lines
	jl	end			# if neg. then done
	addw3	%r6,%r3,%r2
	movw	0(%r2),%r2		# bits = map[bitindex]
	addw2	&4,%r3			#bitindex += 1;
	andw2	&0x7f,%r3		# bitindex = ( bitindex + 1) & WORDMASK
	andw3	0(%fp),%r2,%r0		#
	xorw3	0(%r8),%r2,%r0		#screenp = bits & mask1
	andw2	0(%fp),%r0
	xorw2	%r0,0(%r8)		#
	addw2	&4,%r8			# screenp++
	movw	%r5,%r1			# i = ii
	BLB	stoloop			# if i = -1 then narrow
	BEB	sto2ok			# if i = 0 then no inner words
	lrsw3	&2,%r1,%r0		# i/4
	BLEB	st4less			# less than 4 inner words
store4:	movw	%r2,0(%r8)		# screenp = bits
	movw	%r2,4(%r8)		# screenp = bits
	movw	%r2,8(%r8)		# screenp = bits
	movw	%r2,12(%r8)		# screenp = bits
	addw2	&16,%r8			# screenp += 4
	DECW	%r0			
	BGB	store4			# more inner words
	andw2	&3,%r1			# just how many are left
	BLEB	sto2ok			# no more to do
st4less:
	movw	%r2,0(%r8)		# screenp ^= bits
	addw2	&4,%r8			# screenp++
	DECW	%r1			
	BGB	st4less			# more inner words
sto2ok:
	xorw3	0(%r8),%r2,%r0		#screenp = bits & mask2
	andw2	4(%fp),%r0
	xorw2	%r0,0(%r8)		#
stoloop:
	addw2	%r7,%r8			# screenp += width
	BRB	FSTORE

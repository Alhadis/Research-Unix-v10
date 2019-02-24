	# cursblt draws the cursor.  It is assumed that
	#	the cursor is 16x16 in a texture16
	#	the move is in XOR mode
	#	the destination is on the screen
	# arguments are &display, &texture, destination point

	.globl  cursblt
	.align	8
cursblt:
	SAVE  %r6
	MOVW  &16,%r7	# scan count and source position value
	MOVW 0(%ap),%r2		# get screen bitmap pointer
	ARSH3 &4,8(%ap),%r0	# p.x >> 4 -> r0  (half word address)
	MOVH  10(%ap),%r1	# p.y -> r1
	MULW2 &50,%r1		# p.y * 50 = # of words from base of display
				# to start of scan line
	ADDW2 %r1,%r0		# x + y
	ADDW2 %r0,%r0		# convert words to bytes
	ADDW2 0(%r2),%r0	# add in the base of display for address of dest
	MOVW  4(%ap),%r1	# start of cursor to move
	ANDH3 &31,8(%ap),%r2	# need the shift distance
	SUBW2 %r7,%r2		# r2 - 16
	BEB  CB_ALIGNED		# No shift at all, will we be quick
	MOVW  &0xFFFF0000,%r6	# load a mask needed in loop
	BITB  &2,%r0		# if set, we are dest straddling
	BNEB   CB_DEST_ST
	.align 4
CB_NO_STRADDLE:			# just rotate the 16 bits, other bits are 0's
	ROTW %r2,{uhalf}0(%r1),%r8
	XORW2 %r8,0(%r0)	# and store
	ADDW2 &100,%r0		# next scanline 100bytes=display width=800 bits
	ADDW2 &2,%r1
	DECW  %r7		# for count of 16
	BNEB  CB_NO_STRADDLE
	RESTORE %r6
	RET
	.align 4
CB_ALIGNED:			# no shift, so just xor mem to mem..
	XORH2 0(%r1),0(%r0)	# and store
	ADDW2 &100,%r0		# next scanline 100bytes=display width=800 bits
	ADDW2 &2,%r1
	DECW  %r7		# for count of 16
	BNEB  CB_ALIGNED
	RESTORE %r6
	RET
	.align 4
CB_DEST_ST:			# we straddle, so rotw the halfword...
	ROTW %r2,{uhalf}0(%r1),%r8
	XORH2 %r8,0(%r0)	# xor the left half of the straddling dest
	ANDW2 %r6,%r8		# now mask out what you just wrote &
	XORW2 %r8,2(%r0)	# xor the right half as a full word (faster)
	ADDW2 &2,%r1
	ADDW2 &100,%r0		# next scanline 100bytes=display width=800 bits
	DECW  %r7
	BNEB   CB_DEST_ST
	RESTORE %r6
	RET

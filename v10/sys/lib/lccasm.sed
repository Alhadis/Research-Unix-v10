/calls/{
s/calls[	 ]$[^,]*,_spl0/mfpr	$18,r0\
	mtpr	$0,$18/
s/calls[	 ]$[^,]*,_spl4/mfpr	$18,r0\
	mtpr	$0x14,$18/
s/calls[	 ]$[^,]*,_spl5/mfpr	$18,r0\
	mtpr	$0x15,$18/
s/calls[	 ]$[^,]*,_spl6/mfpr	$18,r0\
	mtpr	$0x18,$18/
s/calls[	 ]$[^,]*,_spl7/mfpr	$18,r0\
	mtpr	$0x1f,$18/
s/calls[	 ]$[^,]*,_splx/mfpr	$18,r0\
	mtpr	(sp)+,$18/
s/calls[	 ]$[^,]*,_mfpr/mfpr	(sp)+,r0/
s/calls[	 ]$[^,]*,_mtpr/mtpr	4(sp),(sp)\
	addl2	$8,sp/
s/calls[	 ]$[^,]*,_resume/ashl	$9,(sp)+,r0 \
	movpsl	-(sp) \
	jsb	_Resume/
s/calls[	 ]$[^,]*,_bcopy/movc3	8(sp),*(sp),*4(sp)\
	addl2	$12,sp/
s/calls[	 ]$[^,]*,_copyin/jsb	_Copyin\
	addl2	$12,sp/
s/calls[	 ]$[^,]*,_copyout/jsb	_Copyout\
	addl2	$12,sp/
s/calls[	 ]$[^,]*,_fubyte/movl	(sp)+,r0 \
	jsb	_Fubyte/
s/calls[	 ]$[^,]*,_fuibyte/movl (sp)+,r0 \
	jsb	_Fubyte/
s/calls[	 ]$[^,]*,_fuword/movl (sp)+,r0 \
	jsb	_Fuword/
s/calls[	 ]$[^,]*,_fuiword/movl (sp)+,r0 \
	jsb	_Fuword/
s/calls[	 ]$[^,]*,_subyte/movl	(sp)+,r0 \
	movl	(sp)+,r1 \
	jsb	_Subyte/
s/calls[	 ]$[^,]*,_suibyte/movl (sp)+,r0 \
	movl	(sp)+,r1 \
	jsb	_Subyte/
s/calls[	 ]$[^,]*,_suword/movl (sp)+,r0 \
	movl	(sp)+,r1 \
	jsb	_Suword/
s/calls[	 ]$[^,]*,_suiword/movl (sp)+,r0 \
	movl	(sp)+,r1 \
	jsb	_Suword/
s/calls[	 ]$[^,]*,_setrq/movl	(sp)+,r0 \
	jsb	_Setrq/
s/calls[	 ]$[^,]*,_remrq/movl	(sp)+,r0 \
	jsb	_Remrq/
s/calls[	 ]$[^,]*,_swtch/movpsl	-(sp)\
	jsb	_Swtch/
s/calls[	 ]$[^,]*,_setjmp/	movl	(sp)+,r1\
	clrl	r0\
	movl	fp,(r1)+\
	moval	1(pc),(r1)/
s/calls[	 ]$[^,]*,_longjmp/movl	(sp)+,r0 \
	jsb	_Longjmp/
s/calls[	 ]$[^,]*,_ffs/ffs	$0,$32,(sp)+,r0 \
	bneq	1f \
	mnegl	$1,r0 \
1: \
	incl	r0/
s/calls[	 ]$[^,]*,_insque/insque	*(sp)+,*(sp)+/
s/calls[	 ]$[^,]*,_remque/remque	*(sp)+,r0/
s/calls[	 ]$[^,]*,_bzero/movc5	$0,(r0),$0,4(sp),*(sp)\
	addl2	$8,sp/
s/calls[	 ]$[^,]*,_htons/rotl	$8,(sp),r0\
	movb	1(sp),r0\
	movzwl	r0,r0\
	addl2	$4,sp/
s/calls[	 ]$[^,]*,_ntohs/rotl	$8,(sp),r0\
	movb	1(sp),r0\
	movzwl	r0,r0\
	addl2	$4,sp/
s/calls[	 ]$[^,]*,_htonl/rotl	$-8,(sp),r0\
	insv	r0,$16,$8,r0\
	movb	3(sp),r0\
	addl2	$4,sp/
s/calls[	 ]$[^,]*,_ntohl/rotl	$-8,(sp),r0\
	insv	r0,$16,$8,r0\
	movb	3(sp),r0\
	addl2	$4,sp/
}

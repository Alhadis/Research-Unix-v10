	.file	"dline.c"
	.data
	.comm	Jdmajor,2
	.comm	Jdminor,2
	.comm	Jxmajor,2
	.comm	Jslopeneg,2
	.comm	PtCurrent,4
	.comm	DotMask,4
	.text
	.align	4
	.def	_dline;	.val	_dline;	.scl	2;	.type	044;	.endef
	.globl	_dline
_dline:
	save	&.R1
	addw2	&.F1,%sp
	movw	0(%ap),%r8
	cmph	Jxmajor,&0
	jne	.L53
	cmph	6(%ap),10(%ap)
	jle	.L54
	movw	4(%ap),20(%fp)
	movw	8(%ap),4(%ap)
	movw	20(%fp),8(%ap)
.L54:
	cmph	4(%ap),8(%ap)
	jle	.L55
	orw2	&1024,12(%ap)
.L55:
	jmp	.L56
.L53:
	cmph	4(%ap),8(%ap)
	jle	.L57
	movw	4(%ap),20(%fp)
	movw	8(%ap),4(%ap)
	movw	20(%fp),8(%ap)
.L57:
.L56:
	movh	4(%ap),4(%fp)
	movh	6(%ap),6(%fp)
	pushw	DotMask
	andh3	&15,6(%fp),%r0
	pushw	%r0
	call	&2,rol
	movw	%r0,24(%fp)
	subh3	4(%fp),8(%ap),%r0
	movh	%r0,0(%fp)
	subh3	6(%fp),10(%ap),%r0
	movh	%r0,2(%fp)
	cmph	2(%fp),&0
	jle	.L59
	movw	&1,%r6
	movw	%r6,%r0
	jmp	.L60
.L59:
	movw	&-1,%r6
	movw	%r6,%r0
.L60:
	pushw	%r8
	pushw	4(%ap)
	call	&2,*7464716
	movw	%r0,%r4
	mulw3	4(%r8),%r6,%r0
	movw	%r0,%r3
	andh3	&31,4(%fp),%r0
	subw3	%r0,&31,%r0
	LLSW3	%r0,&1,%r0
	movw	%r0,%r5
	pushw	24(%fp)
	andh3	&31,4(%fp),%r0
	subw3	%r0,&31,%r0
	pushw	%r0
	call	&2,rol
	movw	%r0,24(%fp)
	movbhw	0(%fp),%r0
	pushw	%r0
	call	&1,abs
	movh	%r0,0(%fp)
	movbhw	2(%fp),%r0
	pushw	%r0
	call	&1,abs
	movh	%r0,2(%fp)
	cmph	Jxmajor,&0
	jne	.L62
	jmp	.L63
.L62:
	addh3	&1,0(%fp),%r0
	movw	%r0,16(%fp)
	cmph	2(%fp),&0
	jne	.L64
	addh3	18(%fp),4(%fp),%r0
	movh	%r0,6(%fp)
	bith	4(%fp),&31
	jz	.L65
	andh3	&31,4(%fp),%r0
	LLSW3	&2,%r0,%r0
	movw	topbits(%r0),%r7
	movw	12(%ap),%r0
	jmp	.L67
.L68:
.L69:
	andh3	&-32,4(%fp),%r0
	andh3	&-32,6(%fp),%r1
	cmpw	%r0,%r1
	jne	.L70
	andh3	&31,6(%fp),%r0
	LLSW3	&2,%r0,%r0
	xorw2	topbits(%r0),%r7
	andw3	DotMask,%r7,%r0
	orw2	%r0,0(%r4)
	jmp	.L52
.L70:
	movw	%r4,%r0
	addw2	&4,%r4
	mcomw	%r7,%r1
	andw2	DotMask,%r1
	orw2	%r1,0(%r0)
	orh3	&31,4(%fp),%r0
	addh2	&1,%r0
	movh	%r0,4(%fp)
	jmp	.L66
.L71:
	andh3	&-32,4(%fp),%r0
	andh3	&-32,6(%fp),%r1
	cmpw	%r0,%r1
	jne	.L72
	andh3	&31,6(%fp),%r0
	LLSW3	&2,%r0,%r0
	xorw2	topbits(%r0),%r7
	andw3	DotMask,%r7,%r0
	mcomw	%r0,%r0
	andw2	%r0,0(%r4)
	jmp	.L52
.L72:
	movw	%r4,%r0
	addw2	&4,%r4
	mcomw	DotMask,%r1
	orw2	%r7,%r1
	andw2	%r1,0(%r0)
	orh3	&31,4(%fp),%r0
	addh2	&1,%r0
	movh	%r0,4(%fp)
	jmp	.L66
.L73:
	andh3	&-32,4(%fp),%r0
	andh3	&-32,6(%fp),%r1
	cmpw	%r0,%r1
	jne	.L74
	andh3	&31,6(%fp),%r0
	LLSW3	&2,%r0,%r0
	xorw2	topbits(%r0),%r7
	andw3	DotMask,%r7,%r0
	xorw2	%r0,0(%r4)
	jmp	.L52
.L74:
	movw	%r4,%r0
	addw2	&4,%r4
	mcomw	%r7,%r1
	andw2	DotMask,%r1
	xorw2	%r1,0(%r0)
	orh3	&31,4(%fp),%r0
	addh2	&1,%r0
	movh	%r0,4(%fp)
	jmp	.L66
.L67:
	cmpw	%r0,&3
	jg	.L75
	ALSW3	&2,%r0,%r0
	cmpw	%r0,&0 
	jneg	.L75
	jmp	*.L76(%r0)
	.data
	.align	4
#SWBEG
.L76:
	.word	.L68
	.word	.L69
	.word	.L71
	.word	.L73
#SWEND
	.text
.L75:
.L66:
.L65:
	movw	12(%ap),%r0
	jmp	.L78
.L79:
.L80:
	jmp	.L82
.L83:
	movw	%r4,%r0
	addw2	&4,%r4
	orw2	DotMask,0(%r0)
.L82:
	addh2	&32,4(%fp)
	cmph	4(%fp),6(%fp)
	jle	.L83
.L81:
	bith	6(%fp),&31
	jz	.L84
	andh3	&31,6(%fp),%r0
	LLSW3	&2,%r0,%r0
	andw3	DotMask,topbits(%r0),%r0
	orw2	%r0,0(%r4)
.L84:
	jmp	.L77
.L85:
	jmp	.L87
.L88:
	movw	%r4,%r0
	addw2	&4,%r4
	mcomw	DotMask,%r1
	andw2	%r1,0(%r0)
.L87:
	addh2	&32,4(%fp)
	cmph	4(%fp),6(%fp)
	jle	.L88
.L86:
	bith	6(%fp),&31
	jz	.L89
	andh3	&31,6(%fp),%r0
	LLSW3	&2,%r0,%r0
	andw3	DotMask,topbits(%r0),%r0
	mcomw	%r0,%r0
	andw2	%r0,0(%r4)
.L89:
	jmp	.L77
.L90:
	jmp	.L92
.L93:
	movw	%r4,%r0
	addw2	&4,%r4
	xorw2	DotMask,0(%r0)
.L92:
	addh2	&32,4(%fp)
	cmph	4(%fp),6(%fp)
	jle	.L93
.L91:
	bith	6(%fp),&31
	jz	.L94
	andh3	&31,6(%fp),%r0
	LLSW3	&2,%r0,%r0
	andw3	DotMask,topbits(%r0),%r0
	xorw2	%r0,0(%r4)
.L94:
	jmp	.L77
.L78:
	cmpw	%r0,&3
	jg	.L95
	ALSW3	&2,%r0,%r0
	cmpw	%r0,&0 
	jneg	.L95
	jmp	*.L96(%r0)
	.data
	.align	4
#SWBEG
.L96:
	.word	.L79
	.word	.L80
	.word	.L85
	.word	.L90
#SWEND
	.text
.L95:
.L77:
	jmp	.L52
.L64:
	movbhw	4(%fp),%r0
	pushw	%r0
	call	&1,Jsetdda
	movw	%r0,%r7
	movbhw	Jdminor,%r0
	LLSW3	&1,%r0,%r0
	movw	%r0,8(%fp)
	movbhw	Jdmajor,%r0
	LLSW3	&1,%r0,%r0
	subw3	%r0,8(%fp),%r0
	movw	%r0,12(%fp)
	movw	12(%ap),%r0
	jmp	.L99
.L100:
.L101:
.L104:
	andw3	DotMask,%r5,%r0
	orw2	%r0,0(%r4)
	subw2	&1,16(%fp)
	jnz	.L105
	jmp	.L52
.L105:
	LRSW3	&1,%r5,%r5
	jnz	.L106
	addw2	&4,%r4
	movw	&-2147483648,%r5
.L106:
	cmpw	%r7,&0
	jge	.L107
	addw2	8(%fp),%r7
	jmp	.L108
.L107:
	addw2	12(%fp),%r7
	LLSW3	&2,%r3,%r0
	addw2	%r0,%r4
.L108:
	jmp	.L104
.L103:
.L109:
.L112:
	andw3	%r5,DotMask,%r0
	mcomw	%r0,%r0
	andw2	%r0,0(%r4)
	subw2	&1,16(%fp)
	jnz	.L113
	jmp	.L52
.L113:
	LRSW3	&1,%r5,%r5
	jnz	.L114
	addw2	&4,%r4
	movw	&-2147483648,%r5
.L114:
	cmpw	%r7,&0
	jge	.L115
	addw2	8(%fp),%r7
	jmp	.L116
.L115:
	addw2	12(%fp),%r7
	LLSW3	&2,%r3,%r0
	addw2	%r0,%r4
.L116:
	jmp	.L112
.L111:
.L117:
.L120:
	andw3	DotMask,%r5,%r0
	xorw2	%r0,0(%r4)
	subw2	&1,16(%fp)
	jnz	.L121
	jmp	.L52
.L121:
	LRSW3	&1,%r5,%r5
	jnz	.L122
	addw2	&4,%r4
	movw	&-2147483648,%r5
.L122:
	cmpw	%r7,&0
	jge	.L123
	addw2	8(%fp),%r7
	jmp	.L124
.L123:
	addw2	12(%fp),%r7
	LLSW3	&2,%r3,%r0
	addw2	%r0,%r4
.L124:
	jmp	.L120
.L119:
.L99:
	cmpw	%r0,&3
	jg	.L125
	ALSW3	&2,%r0,%r0
	cmpw	%r0,&0 
	jneg	.L125
	jmp	*.L126(%r0)
	.data
	.align	4
#SWBEG
.L126:
	.word	.L100
	.word	.L101
	.word	.L109
	.word	.L117
#SWEND
	.text
.L125:
.L98:
.L63:
	addh3	&1,2(%fp),%r0
	movw	%r0,16(%fp)
	cmph	0(%fp),&0
	jne	.L127
	movw	12(%ap),%r0
	jmp	.L129
.L130:
.L131:
.L134:
	andw3	24(%fp),%r5,%r0
	orw2	%r0,0(%r4)
	pushw	24(%fp)
	pushw	&1
	call	&2,rol
	movw	%r0,24(%fp)
	subw2	&1,16(%fp)
	jnz	.L135
	jmp	.L52
.L135:
	LLSW3	&2,%r3,%r0
	addw2	%r0,%r4
	jmp	.L134
.L133:
.L136:
	mcomw	%r5,%r0
	movw	%r0,%r5
	mcomw	24(%fp),%r0
	movw	%r0,24(%fp)
.L139:
	orw3	24(%fp),%r5,%r0
	andw2	%r0,0(%r4)
	pushw	24(%fp)
	pushw	&1
	call	&2,rol
	movw	%r0,24(%fp)
	subw2	&1,16(%fp)
	jnz	.L140
	jmp	.L52
.L140:
	LLSW3	&2,%r3,%r0
	addw2	%r0,%r4
	jmp	.L139
.L138:
.L141:
.L144:
	andw3	24(%fp),%r5,%r0
	xorw2	%r0,0(%r4)
	pushw	24(%fp)
	pushw	&1
	call	&2,rol
	movw	%r0,24(%fp)
	subw2	&1,16(%fp)
	jnz	.L145
	jmp	.L52
.L145:
	LLSW3	&2,%r3,%r0
	addw2	%r0,%r4
	jmp	.L144
.L143:
.L129:
	cmpw	%r0,&3
	jg	.L146
	ALSW3	&2,%r0,%r0
	cmpw	%r0,&0 
	jneg	.L146
	jmp	*.L147(%r0)
	.data
	.align	4
#SWBEG
.L147:
	.word	.L130
	.word	.L131
	.word	.L136
	.word	.L141
#SWEND
	.text
.L146:
.L128:
.L127:
	movbhw	6(%fp),%r0
	pushw	%r0
	call	&1,Jsetdda
	movw	%r0,%r7
	movbhw	Jdminor,%r0
	LLSW3	&1,%r0,%r0
	movw	%r0,8(%fp)
	movbhw	Jdmajor,%r0
	LLSW3	&1,%r0,%r0
	subw3	%r0,8(%fp),%r0
	movw	%r0,12(%fp)
	movw	12(%ap),%r0
	jmp	.L149
.L150:
.L151:
.L154:
	andw3	24(%fp),%r5,%r0
	orw2	%r0,0(%r4)
	pushw	24(%fp)
	pushw	&1
	call	&2,rol
	movw	%r0,24(%fp)
	subw2	&1,16(%fp)
	jnz	.L155
	jmp	.L52
.L155:
	cmpw	%r7,&0
	jge	.L156
	addw2	8(%fp),%r7
	jmp	.L157
.L156:
	addw2	12(%fp),%r7
	pushw	24(%fp)
	pushw	&31
	call	&2,rol
	movw	%r0,24(%fp)
	LRSW3	&1,%r5,%r5
	jnz	.L158
	addw2	&4,%r4
	movw	&-2147483648,%r5
.L158:
.L157:
	LLSW3	&2,%r3,%r0
	addw2	%r0,%r4
	jmp	.L154
.L153:
.L159:
.L160:
.L163:
	andw3	24(%fp),%r5,%r0
	orw2	%r0,0(%r4)
	pushw	24(%fp)
	pushw	&1
	call	&2,rol
	movw	%r0,24(%fp)
	subw2	&1,16(%fp)
	jnz	.L164
	jmp	.L52
.L164:
	cmpw	%r7,&0
	jge	.L165
	addw2	8(%fp),%r7
	jmp	.L166
.L165:
	addw2	12(%fp),%r7
	pushw	24(%fp)
	pushw	&1
	call	&2,rol
	movw	%r0,24(%fp)
	LLSW3	&1,%r5,%r5
	jnz	.L167
	subw2	&4,%r4
	movw	&1,%r5
.L167:
.L166:
	LLSW3	&2,%r3,%r0
	addw2	%r0,%r4
	jmp	.L163
.L162:
.L168:
.L171:
	andw3	24(%fp),%r5,%r0
	mcomw	%r0,%r0
	andw2	%r0,0(%r4)
	pushw	24(%fp)
	pushw	&1
	call	&2,rol
	movw	%r0,24(%fp)
	subw2	&1,16(%fp)
	jnz	.L172
	jmp	.L52
.L172:
	cmpw	%r7,&0
	jge	.L173
	addw2	8(%fp),%r7
	jmp	.L174
.L173:
	addw2	12(%fp),%r7
	pushw	24(%fp)
	pushw	&31
	call	&2,rol
	movw	%r0,24(%fp)
	LRSW3	&1,%r5,%r5
	jnz	.L175
	addw2	&4,%r4
	movw	&-2147483648,%r5
.L175:
.L174:
	LLSW3	&2,%r3,%r0
	addw2	%r0,%r4
	jmp	.L171
.L170:
.L176:
.L179:
	andw3	24(%fp),%r5,%r0
	mcomw	%r0,%r0
	andw2	%r0,0(%r4)
	pushw	24(%fp)
	pushw	&1
	call	&2,rol
	movw	%r0,24(%fp)
	subw2	&1,16(%fp)
	jnz	.L180
	jmp	.L52
.L180:
	cmpw	%r7,&0
	jge	.L181
	addw2	8(%fp),%r7
	jmp	.L182
.L181:
	addw2	12(%fp),%r7
	pushw	24(%fp)
	pushw	&1
	call	&2,rol
	movw	%r0,24(%fp)
	LLSW3	&1,%r5,%r5
	jnz	.L183
	subw2	&4,%r4
	movw	&1,%r5
.L183:
.L182:
	LLSW3	&2,%r3,%r0
	addw2	%r0,%r4
	jmp	.L179
.L178:
.L184:
.L187:
	andw3	24(%fp),%r5,%r0
	xorw2	%r0,0(%r4)
	pushw	24(%fp)
	pushw	&1
	call	&2,rol
	movw	%r0,24(%fp)
	subw2	&1,16(%fp)
	jnz	.L188
	jmp	.L52
.L188:
	cmpw	%r7,&0
	jge	.L189
	addw2	8(%fp),%r7
	jmp	.L190
.L189:
	addw2	12(%fp),%r7
	pushw	24(%fp)
	pushw	&31
	call	&2,rol
	movw	%r0,24(%fp)
	LRSW3	&1,%r5,%r5
	jnz	.L191
	addw2	&4,%r4
	movw	&-2147483648,%r5
.L191:
.L190:
	LLSW3	&2,%r3,%r0
	addw2	%r0,%r4
	jmp	.L187
.L186:
.L192:
.L195:
	andw3	24(%fp),%r5,%r0
	xorw2	%r0,0(%r4)
	pushw	24(%fp)
	pushw	&1
	call	&2,rol
	movw	%r0,24(%fp)
	subw2	&1,16(%fp)
	jnz	.L196
	jmp	.L52
.L196:
	cmpw	%r7,&0
	jge	.L197
	addw2	8(%fp),%r7
	jmp	.L198
.L197:
	addw2	12(%fp),%r7
	pushw	24(%fp)
	pushw	&1
	call	&2,rol
	movw	%r0,24(%fp)
	LLSW3	&1,%r5,%r5
	jnz	.L199
	subw2	&4,%r4
	movw	&1,%r5
.L199:
.L198:
	LLSW3	&2,%r3,%r0
	addw2	%r0,%r4
	jmp	.L195
.L194:
.L149:
	cmpw	&0,%r0
	je	.L150
	cmpw	&1,%r0
	je	.L151
	cmpw	&2,%r0
	je	.L168
	cmpw	&3,%r0
	je	.L184
	cmpw	&1024,%r0
	je	.L159
	cmpw	&1025,%r0
	je	.L160
	cmpw	&1026,%r0
	je	.L176
	cmpw	&1027,%r0
	je	.L192
.L148:
.L52:
	.def	.ef;	.val	.;	.scl	101;	.line	297;	.endef
	.ln	297
	.set	.F1,28
	.set	.R1,6
	ret	&.R1
	.def	_dline;	.val	.;	.scl	-1;	.endef
	.data

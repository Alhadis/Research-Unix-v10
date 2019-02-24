# C return sequence which
# sets errno, returns -1.
#
	global cerror
	comm errno,2

cerror:
	mov.w	%d0,errno
	mov.l	&-1,%d0
	unlk	%fp
	rts


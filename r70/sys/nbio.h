/* netb ioctls */
/* ioctl(fd, cmd, buf)  buf is always 64 bytes, and is both read and written */

/* used in the client kernel */
#define NBIOCON		(('a'<<8)|1)	/* turn debugging prints on */
#define NBIOCOFF	(('a'<<8)|2)	/* turn debugging prints off */
#define NBIOCBSZ	(('a'<<8)|3)	/* first word of buf is nbbsz */

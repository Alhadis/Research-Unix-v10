/*
 *	Sydney C Compiler.
 *
 *	Copyright 1984, Bruce Ellis.
 *
 *	Unauthorised possesion, sale or use prohibited.
 */

extern char	*err_ptr;
extern char	*put_ptr;
extern char	*get_end;
extern char	*get_ptr;
extern char	err_buff[];
extern char	put_buff[];
extern char	get_buff[];
extern void	err_flush();
extern void	put_flush();

#define	put(c)	if (*put_ptr++ = (c), put_ptr == &put_buff[BUFFZ]) \
			put_flush(); \
		else

#ifdef	PUT_CALLS
extern void	putz();
extern void	vput();
extern void	uput();
#else	PUT_CALLS
/*
 *	vput and uput pack longs into 1, 2 or 4 bytes.
 *	putz() is equivalent to [uv]put(0) but less optimisable.
 *
 *	If packing is not required we can replace them with
 *	four put()s.  The argument to the [uv]put macros should
 *	be a register (look at how often it is accessed).
 */
#define	putz()	put(0)

#define	vput(i)	if ((unsigned long)(i) < 0x80) \
			put(i); \
		else if ((unsigned long)(i) < 0x4000) \
		{ \
			put((((int)(i) >> 8) & 0x3F) | 0x80); \
			put((int)(i) & 0xFF); \
		} \
		else \
		{ \
			put((((i) >> 24) & 0x3F) | 0xC0); \
			put(((i) >> 16) & 0xFF); \
			put(((int)(i) >> 8) & 0xFF); \
			put((int)(i) & 0xFF); \
		}

#define	uput(i)	if ((i) < 0x40) \
		{ \
			if ((i) < -0x40) \
			{ \
				if ((i) < -0x2000) \
				{ \
					put((((i) >> 24) & 0x3F) | 0xC0); \
					put(((i) >> 16) & 0xFF); \
					put(((int)(i) >> 8) & 0xFF); \
					put((int)(i) & 0xFF); \
				} \
				else \
				{ \
					put((((int)(i) >> 8) & 0x3F) | 0x80); \
					put((int)(i) & 0xFF); \
				} \
			} \
			else \
				put(i); \
		} \
		else if ((i) < 0x2000) \
		{ \
			put((((int)(i) >> 8) & 0x3F) | 0x80); \
			put((int)(i) & 0xFF); \
		} \
		else \
		{ \
			put((((i) >> 24) & 0x3F) | 0xC0); \
			put(((i) >> 16) & 0xFF); \
			put(((int)(i) >> 8) & 0xFF); \
			put((int)(i) & 0xFF); \
		}
#endif	PUT_CALLS

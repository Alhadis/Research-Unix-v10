/*
**	Packet check bytes calculation
*/

#ifdef	vax
/*
**	Vax "crc" instruction look-up table for polynomial = 0120001
*/

unsigned long	crc16t[] = 
{
	0, 0146001, 0154001,  012000, 0170001,  036000,  024000, 0162001
	, 0120001,  066000,  074000, 0132001,  050000, 0116001, 0104001,  042000
};


/*ARGSUSED*/
int
crc(s, n)
	unsigned char	*s;
	int	n;
{
	asm("	crc	_crc16t,$0,8(ap),*4(ap)	");
	asm("	cmpw	r0,(r3)			");
	asm("	beqlu	OK			");
	asm("	movw	r0,(r3)			");
	asm("	movl	$1,r0			");
	asm("	ret				");
	asm("OK:movw	r0,(r3)			");
	return (0);
}


#else


/*
**	crc-16:  x**16 + x**15 + x**2 + 1
*/

typedef unsigned char	uchar;
typedef unsigned short	ushort;

#define	lobyte(X)	(X&0xff)
#define	hibyte(X)	((X>>8)&0xff)

#ifdef	ROMCRC
extern ushort crc16t_32[2] [16];
#else
ushort	crc16t_32[2][16]	= 
{
	0, 0140301, 0140601, 0500, 0141401, 01700, 01200, 0141101,
	    0143001, 03300, 003600, 0143501, 02400, 0142701, 0142201, 02100,
	    0, 0146001, 0154001, 012000, 0170001, 036000, 024000, 0162001,
	    0120001, 066000, 074000, 0132001, 050000, 0116001, 0104001, 042000
};
#endif

int
crc(buffer, nbytes)
	register uchar *buffer;
	int	nbytes;
{
	register ushort	tcrc = 0;
	register int	temp;
	register int	i;

	if ( (i = nbytes) > 0 )
		do {
			temp = tcrc ^ *buffer++;
			tcrc = crc16t_32[0][temp & 017]
			     ^ crc16t_32[1][(temp>>4) & 017]
			     ^ (tcrc >> 8);
		} while ( --i > 0 );

	if ( lobyte(tcrc) != *buffer )
		i++;
	*buffer++ = lobyte(tcrc);

	if ( hibyte(tcrc) != *buffer )
		i++;
	*buffer++ = hibyte(tcrc);

	return(i);
}


#endif

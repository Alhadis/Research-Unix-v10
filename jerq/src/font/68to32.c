/*
	68to32:

		convert a blit 68000 font to a 5620 font.
	this runs on the host!!

	usage:	conv < 68000.font > mac32.font
*/
#include	<stdio.h>

#define	M16	0xF
#define	M32	0x1F

main()
{
	char buf[80000], zero[2];
	int m68, m32, right, height;
	register i;

	zero[0] = zero[1] = 0;
	read(0, buf, 8);
	write(1, buf, 8);
	height = buf[2];
	i = buf[1]&0xff;
	fprintf(stderr, "%d chars height=%d\n", i, height);
	for(i++; i; i--)
	{
		read(0, buf, 6);
		write(1, buf, 6);
		write(1, zero, 2);
	}
	right = (buf[0]&0xff)*256 + (buf[1]&0xff);
	m68 = 2*((right+M16) >> 4);
	m32 = 4*((right+M32) >> 5);
	fprintf(stderr, "m32=%d m68=%d\n", m32, m68);
	for(i = 0; i < height; i++)
	{
		if(read(0, buf, m68) != m68) perror("read");
		if(write(1, buf, m32) != m32) perror("write");
	}
	exit(0);
}

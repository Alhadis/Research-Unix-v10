typedef	unsigned short	ushort;
typedef	unsigned char	uchar;
typedef unsigned long	ulong;
typedef	long		vlong;
typedef union
{
	char	clength[8];
	vlong	vlength;
	struct
	{
		long	length;
		long	hlength;
	};
} Length;


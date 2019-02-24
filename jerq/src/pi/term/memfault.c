#define	DMD_ROM	0
#define	DMD_RAM	7

#define DMD_SEGMENT(a)	( (a)>>20 )

char *AlignErr(addr,size)
long addr;
{
	if( !addr ) return "zero address";
	return addr&(size-1) ? "alignment error" : 0;
}

char *ReadErr(addr,size)
register long addr;
{
	register seg = DMD_SEGMENT(addr);
	return (seg == DMD_RAM || seg == DMD_ROM )
	      ? AlignErr(addr,size)
	      : "memory read error";
}

char *WriteErr(addr,size)
register long addr;
{
	return DMD_SEGMENT(addr) == DMD_RAM
	      ? AlignErr(addr,size)
	      : "memory write error";
}

#include "master.pri"
#include "m68kcore.h"

const long stack_size = 12*1024;
const long rom_lo = 0x200000;
const long rom_size = 16*1024;
const long rom_hi = rom_lo+rom_size;

SnetCore.SnetCore(Process *p, SnetMaster *m):(p,m)
{
	stack_hi = 0x1b1000;
	stack_lo = stack_hi - stack_size;
}

long SnetCore.savearea()	// boot.s: a_fault: mov.l %sp,0x1f94 (savesp+4)
{
	return peek(0x1f94)->lng;
}

char *SnetCore.seekto(int &fd, long &offset)
{
	if( offset >= rom_lo ){
		if( offset >= rom_hi )
			return "address beyond top of ROM";
		offset -= rom_lo;
		static int rom_fd = -1;
		extern char *SNETROM;
		if( rom_fd < 0 ){		// rom must be swabbed
			int fd = ::open(SNETROM, 0);
			if( fd < 0 ) return sf("ROM: cannot open %s", SNETROM);
			char swab[rom_size+1];
			int got = ::read(fd, (char*)swab+1, rom_size);
			::close(fd);
			if( got <= 0 ) return sf("ROM: cannot read %s", SNETROM);
			for( char *p = swab; p < swab+got; p += 2 )
				p[0] = p[2];
			char *tmp_rom = sf("rom_%d", getpid());
			creat(tmp_rom, 0666);
			rom_fd = ::open(tmp_rom, 2);
			::unlink(tmp_rom);
			if( rom_fd<0 ) return sf("ROM: cannot open %s", tmp_rom);
			::write(rom_fd, swab, got);
		}
		fd = rom_fd;
		return 0;
	}
	if( offset >= stack_hi )
		return "address beyond stack top";
	if( offset > stack_lo )
		offset -= stack_lo;
	else
		offset += stack_size;
	return 0;	
}

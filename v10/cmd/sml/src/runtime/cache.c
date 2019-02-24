/* Copyright 1989 by AT&T Bell Laboratories */

#include <syscall.h>
#include <sys/sysmips.h>
#include <machine/cachectl.h>

/* Flush the instruction cache, starting at addr for size bytes */
FlushInstructionCache(addr,size)
int addr, size;
{
  syscall(SYS_sysmips, MIPS_CACHEFLUSH, addr, size, ICACHE, 0);
}

cachectl(addr,nbytes,op) 
    char *addr;
    int nbytes, op;
{
    return syscall(SYS_sysmips,MIPS_CACHECTL,addr,nbytes,op,0);
}

cacheflush(addr, nbytes, cache)
     char *addr;
     int nbytes, cache;
{
     return syscall(SYS_sysmips,MIPS_CACHEFLUSH,addr,nbytes,cache,0);
}

/*
 * return number of blocks, including indirect block count
 * given stat info
 *
 * for systems not supporting stat.st_blocks
 */

#include <lclparam.h>
#include <fsinfo.h>

#ifndef stat_blocks

#ifndef B_DIRECT
#define B_DIRECT	10
#endif

#ifdef BITFS

#define B_SIZE		BSIZE(st->st_dev)
#define B_INDIRECT	NINDIR(st->st_dev)

#else

#ifdef BSIZE
#define B_SIZE		BSIZE
#else
#define B_SIZE		1024
#endif

#ifdef NINDIR
#define B_INDIRECT	NINDIR
#else
#define B_INDIRECT	128
#endif

#endif

#endif

long
iblocks(st)
struct stat*	st;
{
#ifdef stat_blocks
	return((st->st_blocks + 1) / 2);
#else
	long	b;
	long	t;

	t = b = (st->st_size + B_SIZE - 1) / B_SIZE;
	if ((b -= B_DIRECT) > 0)
	{
		t += (b - 1) / B_INDIRECT + 1;
		if ((b -= B_INDIRECT) > 0)
		{
			t += (b - 1) / (B_INDIRECT * B_INDIRECT) + 1;
			if (b > B_INDIRECT * B_INDIRECT) t++;
		}
	}
	return(t);
#endif
}

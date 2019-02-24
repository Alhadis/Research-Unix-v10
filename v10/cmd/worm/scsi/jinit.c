#define	_POSIX_SOURCE
#include	<stddef.h>
#include	<stdio.h>
#include	<string.h>
#include	"jukeface.h"
#include	"jukebox.h"

void
j_init(Jukebox *j)
{
	j->nshelves = 0;
	j->nluns = 0;
	j->ndrives = 0;
	j->luns = 0;
	j->shelves = 0;
	j->names = 0;
}

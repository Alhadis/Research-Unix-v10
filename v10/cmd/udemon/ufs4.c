#
#include "ufs.h"
#include "sys/types.h"
#include "sys/stat.h"
/*
 * Converts stat command to intermediate form to
 * decrease filesystem dependency
 */
filstat(name,b)
char	*name;
struct ustat	*b;
{
	struct stat s;
	long l;
	int	i;

	if(stat(name,&s)<0)
		return(-1);
	uconv(&s,b);
	return(1);
}
uconv(ip,b)
char	*ip;
struct ustat	*b;
{
	long l;
	int	i;

	b->mode = ip->st_mode&0777;
	switch(ip->st_mode&S_IFMT){
	case S_IFDIR:
		b->type = DIR;
		break;
	case S_IFCHR:
		b->type = CHR;
		break;
	case S_IFBLK:
		b->type = BLK;
		break;
	case S_IFREG:
		b->type = ORDFILE;
		break;
	}
	b->size = ip->st_size;
	b->uid = ip->st_uid;
	b->gid = ip->st_gid;
	b->modtime = ip->st_mtime;
	b->acctime = ip->st_atime;
	return(1);
}
desstat(f,b)
char	*b;
{
	struct stat s;
	if(fstat(f,&s)<0)
		return(-1);
	uconv(&s,b);
	return(1);
}

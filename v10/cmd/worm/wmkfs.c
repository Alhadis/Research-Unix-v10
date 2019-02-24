#include	<libc.h>
#include	"worm.h"
#include	<sys/types.h>
#include	<sys/udaioc.h>

usage()
{
	fprint(2, "Usage: worm mkfs [-fdevice] [-ccomments] [-bblksize] [-nnblks] [-vnewvol_id] vol_id\n");
	fprint(2, "e.g. worm mkfs -f1 -c\"512x512x24 movies\" tdmovies1a\n");
	exit(1);
}

main(argc, argv)
	char **argv;
{
	Superblock s, os;
	char *b;
	long sb;
	int c;
	char *volid;
	char *dev = "/dev/worm0";
	char *nblks = 0;
	char *bsize = 0;
	char *nvolid = 0;
	char *comments = 0;
	char *e;
	int virgin;
	extern optind;
	extern char *optarg;

	while((c = getopt(argc, argv, "f:n:b:c:v:")) != -1)
		switch(c)
		{
		case 'b':	bsize = optarg; break;
		case 'c':	comments = optarg; break;
		case 'f':	dev = optarg; break;
		case 'n':	nblks = optarg; break;
		case 'v':	nvolid = optarg; break;
		case '?':	usage();
		}
	if(optind != argc-1)
		usage();
	volid = argv[optind];
	if(strlen(volid) > sizeof(s.vol_id)-1)
		volid[sizeof(s.vol_id)-1] = 0;
	c = volid[strlen(volid)-1];
	if((c != 'a') && (c != 'b')){
		if(nvolid == 0){
			fprint(2, "worm mkfs: vol_id '%s' must end in 'a' or 'b'\n", volid);
			exit(1);
		}
		fprint(2, "worm mkfs: warning: vol_id '%s' should end in 'a' or 'b'\n", volid);
	}
	if(nvolid){
		if(strlen(nvolid) > sizeof(s.vol_id)-1)
			nvolid[sizeof(s.vol_id)-1] = 0;
		c = nvolid[strlen(nvolid)-1];
		if((c != 'a') && (c != 'b')){
			fprint(2, "worm mkfs: vol_id '%s' must end in 'a' or 'b'\n", nvolid);
			exit(1);
		}
	}
	dev = mapdev(dev);
	if((s.fd = open(dev, 2)) < 0){
		perror(dev);
		exit(1);
	}
	/*
		now, do we read the current superblock or make one up?
		this is hard to answer in general, push the answer off to virginal()
	*/
	virgin = virginal(&s);
	if(virgin){
		setdefaults(&s, nblks);
		if((s.blocksize < 512) || (s.blocksize%512)){
			fprint(2, "worm mkfs: bad blocksize '%s'\n", bsize);
			exit(1);
		}
		strcpy(s.vol_id, volid);
	} else {
		if(strcmp(volid, s.vol_id)){
			fprint(2, "worm mkfs: volid mismatch; expected %s, got %s\n",
				volid, s.vol_id);
			exit(1);
		}
	}
	/* set any new parameters */
	if(nvolid)
		strcpy(s.vol_id, nvolid);
	if(bsize)
		s.blocksize = atoi(bsize);
	if(s.blocksize < 512){
		fprint(2, "wormmkfs: bad nblocks = '%s'\n", nblks);
		exit(1);
	}
	if(s.blocksize % sizeof(Inode)){
		fprint(2, "worm mkfs: sizeof(Inode)=%d does not divide blocksize %d\n",
			sizeof(Inode), s.blocksize);
		exit(1);
	}
	if(comments){
		if(strlen(comments) > sizeof(s.comment)-1)
			comments[sizeof(s.comment)-1] = 0;
		strcpy(s.comment, comments);
	}
	/* only check if we are changing it */
	if(nblks && !virgin){
		s.nblocks = atoi(nblks);
		s.nfree = s.nblocks - s.nextffree;
		if(s.nfree <= 1){
			fprint(2, "worm mkfs: new nblocks(%d) is too small\n", s.nblocks);
			exit(1);
		}
	}
	/* now allocate the new superblock */
	sb = s.nextsb;
	s.myblock = sb;
	s.nextsb = sb+1;
	s.nextffree = sb+2;
	s.nfree -= 1;
	s.ninodes = 0;
	s.ninochars = 0;
	s.binodes = 0;
	time(&s.ctime);
	/* write it */
	if((b = malloc(s.blocksize)) == 0){
		fprint(2, "worm mkfs: cannot malloc buffer %d bytes\n", s.blocksize);
		exit(1);
	}
	memset(b, 0, s.blocksize);
	memcpy(b, (char *)&s, sizeof(s));
	Seek(&s, sb);
	if(Write(&s, b, 1L))
		exit(1);
	ioctl(s.fd, UIOSPDW);
	exit(0);
}

setdefaults(s, nblks)
	Superblock *s;
	char *nblks;
{
	struct ud_unit sz;
	char buf[1024];

	s->magic = SMAGIC;
	s->blocksize = 1024;
	s->version = VLINK;
	if(nblks){
		s->nblocks = atoi(nblks);
		if(s->nblocks <= 2){
			fprint(2, "worm mkfs: nblocks(%d) too small\n", s->nblocks);
			exit(1);
		}
	} else {
		read(s->fd, buf, sizeof buf);	/* ignore error */
		if(ioctl(s->fd, UIOCHAR, &sz) >= 0){
			switch(sz.radsize)
			{		/* note below figures/2 used in scsi/volid.c */
			case 3275999:		/* sony 12in clv single density */
				s->nblocks = 1600000;
				break;
			case 6551999:		/* sony 12in clv double density */
				s->nblocks = 3250000;
				break;
			default:
				fprint(2, "worm mkfs: unknown size %d\n", sz.radsize);
				exit(1);
			}
		} else
			s->nblocks = 1600000;
		fprint(2, "worm mkfs: using disk size %d\n", s->nblocks);
	}
	s->zero = 0;
	s->nfree = s->nblocks-1;
	s->nextffree = 0;
	s->nextsb = 1;
	s->vol_id[0] = 0;
	s->comment[0] = 0;
	s->myblock = 0;
	s->ctime = 0;
}

virginal(s)
	Superblock *s;
{
	char buf[1024];
	static char zeros[1024];
	long sb;
	char *e;
	extern char *getenv();

	if(e = getenv("WORMZERO"))
		sb = atoi(e);
	else
		sb = 1;
	bigseek(s->fd, sb, 1024, 0);
	errno = 0;
	if(read(s->fd, buf, 1024) == 1024)
		goto valid;
	if((errno != ENXIO) && (errno != 0) && memcmp(buf, zeros, 1024))
		goto invalid;
	errno = 0;
	if(read(s->fd, buf, 1024) == 1024){	/* try next block */
valid:
		if(e = openinode(s, SPIN_DOWN)){
			fprint(2, "worm mkfs: %s\n", e);
			exit(1);
		}
		return(0);
	} else {
		if((errno == ENXIO) || (errno == 0) || (memcmp(buf, zeros, 1024) == 0))
			return(1);
invalid:
		perror("worm mkfs: I/O errors probing for superblock");
		exit(1);
	}
}

#include	<stdio.h>
#include	<limits.h>
#include	"scsi.h"
#include	"juke.h"

main(int argc, char *argv[])
{
	int c;
	int aflag = 0, eflag = 0, mflag = 0, pflag = 0;
	int rflag = 0, sflag = 0, uflag = 0, Uflag = 0;
	long secs = 3600L*24*183;	/* half a year is enough */
	char *vol_id;
	char errbuf[1024];
	extern int optind;
	extern char *optarg;

	setbuf(stdout, (char *)0);	/* turn off buffering */
	while((c = getopt(argc, argv, "aemprsuUw:")) != -1)
		switch (c)
     		{
		case 'a':       aflag = 1; break;
		case 'e':       eflag = 1; break;
		case 'm':	mflag = 1; break;
		case 'p':       pflag = 1; break;
		case 'r':       rflag = 1; break;
		case 's':       sflag = 1; break;
		case 'u':       uflag = 1; break;
		case 'U':       Uflag = 1; break;
		case 'w':	secs = atol(optarg); break;
		default:	usage(); break;
		}
	s_id = 2;
	setnlun();
	if(!aflag&&!eflag&&!mflag&&!pflag&&!rflag&&!uflag&&!Uflag)
		sflag = 1;
	vol_id = (optind < argc)? argv[optind] : 0;
	if(uflag || Uflag)
		unload(Uflag);
	if(eflag){
		if(vol_id == 0){
			strcpy(errbuf, "-e needs a vol_id");
			goto scram;
		}
		if(eject(vol_id, errbuf))
			goto scram;
	}
	if(rflag){
		unload(1);
		sleep(1);
		if(cold_inv(vol_id? *vol_id : 'u', errbuf) < 0)
			goto scram;
	}
	if(aflag){
		if(vol_id == 0){
			strcpy(errbuf, "-a needs a vol_id");
			goto scram;
		}
		if(allocate(vol_id, errbuf))
			goto scram;
	}
	if(mflag){
		if((c = j_load(vol_id, errbuf, secs)) < 0)
			goto scram;
		if(s_start(c, errbuf) < 0)
			fprintf(stderr, "jukebox: %s\n", errbuf);
		if(s_stop(c, errbuf) < 0)
			fprintf(stderr, "jukebox: %s\n", errbuf);
		printf("%d\n", c);
	}
	if(sflag)
		prstatus();
	if(pflag){
		if(j_rdshelves(errbuf) < 0)
			goto scram;
		for(c = 0; c < NSHELF; c++)
			if(j_shelf[c])
				printf("%d: %s\n", c, j_shelf[c]);
	}
	if(j_wrshelf)
		if(j_wrshelves(errbuf))
			errexit(errbuf);
	exit(0);
scram:
	if(j_wrshelf)
		j_wrshelves(errbuf);
	errexit(errbuf);
}

usage()
{
	fprintf(stderr, "Usage: jukebox [-aemprsuU] [-w secs] [vol_id\n");
	exit(1);
}

errexit(char *errbuf)
{
	fprintf(stderr, "jukebox: %s\n", errbuf);
	exit(1);
}

prstatus()
{
	struct Lunstatus *l;
	int c;
	char errbuf[1024];
	static char *spin[2] = { "offline", "online" };

	if(j_getstatus(errbuf)){
		fprintf(stderr, "jukebox: %s\n", errbuf);
		exit(1);
	}
	if(j_rdshelves(errbuf)){
		fprintf(stderr, "jukebox: %s\n", errbuf);
		exit(1);
	}
	for(c = 0; c < 8; c++){
		l = &j_status.lun[c];
		if(!l->diskin)
			continue;
		printf("lun %d(", c);
		if(j_status.udrive == (0x80|c))
			printf("upper,%s", spin[l->ready]);
		else if(j_status.ldrive == (0x80|c))
			printf("lower,%s", spin[l->ready]);
		else
			printf("in shelf");
		printf("): ");
		if(l->shelfvalid){
			if(j_shelf[l->retshelf>>1])
				printf("%s%c", j_shelf[l->retshelf>>1], "ab"[l->retshelf&1]);
			else
				printf("unallocated shelf number %d??", l->retshelf);
		} else
			printf("<unknown shelf??>");
		printf("\n");
	}
}

unload(int force)
{
	struct Lunstatus *l;
	int c;
	char errbuf[1024];

	if(j_getstatus(errbuf)){
		fprintf(stderr, "jukebox: %s\n", errbuf);
		exit(1);
	}
	if(j_rdshelves(errbuf)){
		fprintf(stderr, "jukebox: %s\n", errbuf);
		exit(1);
	}
	for(c = 0; c < 8; c++){
		l = &j_status.lun[c];
		if(!l->diskin)
			continue;
		if(force || !l->ready)
			if(j_drive_to_shelf(c, -1, SIDEA, errbuf))
				fprintf(stderr, "jukebox: %s\n", errbuf);
	}
}

eject(char *vol_id, char *errbuf)
{
	int sh;
	int dr;

	if(j_rdshelves(errbuf)){
		fprintf(stderr, "jukebox: %s\n", errbuf);
		exit(1);
	}
	sh = j_shelfof(vol_id);
	if(sh < 0){
		sprintf(errbuf, "xcan't find vol_id %s", vol_id);
		return(-1);
	}
	j_wrshelf = 1;
	if((dr = j_driveof(vol_id)) >= 0){
		j_shelf[sh] = 0;
		return(s_eject(dr, errbuf));
	}
	dr = NLUN-1;
	if(j_shelf_to_drive(sh, SIDEA, dr, errbuf) < 0)
		return(-1);
	if(s_eject(dr, errbuf))
		return(-1);
	j_shelf[sh] = 0;
	return(0);
}

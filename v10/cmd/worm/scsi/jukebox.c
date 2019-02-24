#define	_POSIX_SOURCE
#include	<stddef.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<string.h>
#include	"scsi.h"
#include	"jukeface.h"
#include	"jukebox.h"
#include	"arg.h"

Jukebox jukebox;
char *argv0;
static void prstatus(void);
static void usage(void);
static void errexit(char *errbuf);
static void unload(int);
static eject(Jukebox *j, char *vol_id, char *errbuf);

main(int argc, char *argv[])
{
	int c;
	int aflag = 0, eflag = 0, mflag = 0, pflag = 0;
	int rflag = 0, sflag = 0, uflag = 0, Uflag = 0;
	int reset = 0;
	int warm = 0;
	long secs = 3600L*24*183;	/* half a year is enough */
	char *vol_id;
	char errbuf[1024];

	setvbuf(stderr, (char *)0, _IOLBF, 4096);
	setvbuf(stdout, (char *)0, _IOLBF, 4096);
	ARGBEGIN{
	case 'a':       aflag = 1; break;
	case 'e':       eflag = 1; break;
	case 'm':	mflag = 1; break;
	case 'p':       pflag = 1; break;
	case 'r':       rflag = 1; break;
	case 'R':       reset = 1; break;
	case 's':       sflag = 1; break;
	case 'u':       uflag = 1; break;
	case 'U':       Uflag = 1; break;
	case 'w':	secs = atol(ARGF()); break;
	case 'W':	warm = 1; break;
	default:	usage(); break;
	}ARGEND
	s_id = 2;
	j_init(&jukebox);
	if(j_config(&jukebox, errbuf) < 0)
		goto scram;
	if(!aflag&&!eflag&&!mflag&&!pflag&&!rflag&&!uflag&&!Uflag)
		sflag = 1;
	vol_id = argc? argv[0] : 0;
	if(reset)
		soft_reset();
	if(uflag || Uflag)
		unload(Uflag);
	if(eflag){
		if(vol_id == 0){
			strcpy(errbuf, "-e needs a vol_id");
			goto scram;
		}
		if(eject(&jukebox, vol_id, errbuf))
			goto scram;
	}
	if(rflag){
		if(j_cold(&jukebox, vol_id? vol_id : "u", errbuf) < 0)
			goto scram;
	}
	if(warm){
		if(j_warm(&jukebox, errbuf) < 0)
			goto scram;
	}
	if(aflag){
		if(vol_id == 0){
			strcpy(errbuf, "-a needs a vol_id");
			goto scram;
		}
		if(allocate(&jukebox, vol_id, errbuf))
			goto scram;
	}
	if(mflag){
		if((c = j_load(&jukebox, vol_id, errbuf, secs)) < 0)
			goto scram;
		if(j_start(c, errbuf) < 0)
			fprintf(stderr, "jukebox: %s\n", errbuf);
		if(j_stop(c, errbuf) < 0)
			fprintf(stderr, "jukebox: %s\n", errbuf);
		printf("%d\n", c);
	}
	if(sflag)
		prstatus();
	if(pflag){
		if(j_rdshelves(&jukebox, errbuf) < 0)
			goto scram;
		for(c = 0; c < jukebox.nshelves; c++)
			if(jukebox.names[c])
				printf("%d: %s\n", c, jukebox.names[c]);
	}
	if(j_wrshelf)
		if(j_wrshelves(&jukebox, errbuf))
			errexit(errbuf);
	exit(0);
scram:
	if(j_wrshelf)
		j_wrshelves(&jukebox, errbuf);
	errexit(errbuf);
	return(1);			/* shut up compiler */
}

static void
usage(void)
{
	fprintf(stderr, "Usage: jukebox [-aemprsuU] [-w secs] [vol_id\n");
	exit(1);
}

static void
errexit(char *errbuf)
{
	fprintf(stderr, "jukebox: %s\n", errbuf);
	exit(1);
}

static void
prstatus(void)
{
	struct lun *lun;
	int c;
	char errbuf[1024];

	if(j_drstatus(&jukebox, errbuf)){
		fprintf(stderr, "jukebox: %s\n", errbuf);
		exit(1);
	}
	for(c = 0, lun = jukebox.luns; c < jukebox.nluns; c++, lun++){
		if(!lun->occupied)
			continue;
		printf("lun %d(%s,%sline): ", c, lun->desc, lun->spunup?"on":"off");
		if(lun->shelf >= 0){
			printf("%s%c", j_name(&jukebox, lun->shelf), "ab"[lun->side]);
		} else
			printf("<unknown shelf??>");
		printf("\n");
	}
}

static void
unload(int force)
{
	struct lun *l;
	int c;
	char errbuf[1024];

	if(j_drstatus(&jukebox, errbuf)){
		fprintf(stderr, "jukebox: %s\n", errbuf);
		exit(1);
	}
	for(c = 0, l = jukebox.luns; c < 8; c++, l++){
		if(l->occupied && (force || !l->spunup))
			if(j_dr_to_sh(c, -1, SIDEA, errbuf))
				fprintf(stderr, "jukebox: %s\n", errbuf);
	}
}

static
eject(Jukebox *j, char *vol_id, char *errbuf)
{
	int sh;
	int dr;

	if(j_rdshelves(j, errbuf))
		return(-1);
	if(j_drstatus(j, errbuf)){
		fprintf(stderr, "jukebox: %s\n", errbuf);
		exit(1);
	}
	sh = j_shelfof(j, vol_id);
	if(sh < 0){
		sprintf(errbuf, "no vol_id %s", vol_id);
		return(-1);
	}
	j_wrshelf = 1;
	if((dr = j_driveof(j, vol_id)) >= 0){
		j_wrshelf = 1;
		j->shelves[sh] = 0;
		j->names[sh] = 0;
		return(j_eject(dr, errbuf));
	}
	dr = j->nluns-1;
	if(j_sh_to_dr(sh, SIDEA, dr, errbuf) < 0)
		return(-1);
	if(j_eject(dr, errbuf))
		return(-1);
	j_wrshelf = 1;
	j->shelves[sh] = 0;
	j->names[sh] = 0;
	return(0);
}

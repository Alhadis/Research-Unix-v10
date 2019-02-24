/*
 * I/O routines to retrieve files from GCOS via Spider
 */

#include	<stdio.h>

#define	SPIDER	1
#define	PHONE	0
#define	LPD	0
#define	FGET	1

char	dname[] = "FGETS";

#include	"gcos.c"
#include	"fgdemon.c"

/* nak codes --
 * I illegal
 * B dest busy
 * S illegal gcos command
 * G gcos down/no resp
 */

#define	SETSIG	0
#define	GETSIG	1
#define	GETCHN	2
#define	GETTRB	3
#define	SETEOF	4
#define	CLREOF	5

int	COMM[]	= {3};
int	DATA[]	= {1};
int	SPEOF[]	= {2};
int	TERM[]	= {4};

char	sndev[]	= "/dev/tiu/d2";
#define	LIDMES	4
char	idmes[LIDMES+1]	= "gXXa";

int	sfd = -1;	/*spider file*/
int	rathole[1];
int	errno;
int	ioerr;
char	sbuf[BUFSIZ];

fgd_read()
{
	int nr, i;
	extern hungup();

	/* allow 50 seconds to open the file and get going */

	signal(SIGALRM, hungup);
	alarm(50);

	if ((sfd = open(sndev, 2)) < 0) {
		ioerr = errno;
		if (ioerr == EBUSY)
			trouble(OKTRB, 10, "unix chan bsy");
		else
			trouble(FATTRB, 0, "TIU down %d, 201 used", ioerr);
		}

	snstat(sfd, rathole, GETTRB);
	snstat(sfd, COMM, SETSIG);

	idmes[1] = fget_id[0];
	idmes[2] = fget_id[1];
	if (nr = write(sfd, idmes, LIDMES) != LIDMES) {
		if((nr=write(sfd,idmes,LIDMES))!=LIDMES){  /*try once again*/
		    snstat(sfd, rathole, GETTRB);
		    trouble(BADTRB, 20, "wrote 4, sent %d, error %o, 201 used",
		      nr,rathole[0]&0377);
		}
	}

#ifdef	DEBUG
	fprintf(stderr, "sent %s\n", idmes); /* PRINTF */
#endif

	if (read(sfd, sbuf, BUFSIZ) <= 0) {
		ioerr = errno;
		snstat(sfd, rathole, GETTRB);
		trouble(BADTRB, 20, "read1 err %d tbyte %o, 201 used", ioerr, rathole[0]&0377);
		}

#ifdef	DEBUG
	fprintf(stderr, "acknowledgement: %c\n", sbuf[0]); /* PRINTF */
#endif

	if (sbuf[0] != '+') {
		if (sbuf[0] == 'B')
			trouble(OKTRB, 10, "pdp8 busy    ");

		if (sbuf[0] == 'G')
			trouble(OKTRB, -120, "gcos down");

		trouble(FATTRB, 0, "nak %c, 201 used", sbuf[0]);

		}

	while ( (nr = read(sfd, sbuf, BUFSIZ)) > 0 ) {

	    alarm(240);
	    	
#ifdef	DEBUG
	    fprintf(stderr, "chars read = %d\n", nr); /* PRINTF */
#endif
	    for ( i = 0; i < nr; i += sbuf[i+1]+2 ) {

#ifdef	DEBUG
		/* fprintf(stderr, "sbuf[i], sbuf[i+1] = %d %d\n",
			sbuf[i],sbuf[i+1]); /* PRINTF */
#endif
		switch ( sbuf[i] ) {

		    case 0:
			(*output)(&sbuf[i+2], sbuf[i+1]);
			break;

		    case 1: 
		    case 2:
#ifdef	DEBUG
			sbuf[i+sbuf[i+1]+1] = '\0'; /* PRINTF */
			fprintf(stderr, "misc card - %s\n", &sbuf[i+2]); /* PRINTF */
#endif
			output = ignore;
			break;

		    case 3:
#ifdef	DEBUG
			fprintf(stderr, "binary card - %o\n",sbuf[i+2]); /* PRINTF */
#endif
			if (((sbuf[i+2]&0377) == 0377)
			   && ((sbuf[i+3]&0377) == 0360)) endfile();
			else output = ignore;
		            break;

		    default:
			if((tfd = fopen(badbuf, "w")) != NULL){
				fwrite(sbuf, 1, nr, tfd);
				FCLOSE(tfd);
			}
			trouble(FATTRB, 0, "bad record type %d, 201 used", sbuf[i]);

		}

	    }

	}

	if (nr < 0) {
		ioerr = errno;
		snstat(sfd, rathole, GETTRB);
		gotit = 0;
		trouble(BADTRB, 20, "read2 err %d tbyte %o, 201 used", ioerr, rathole[0]&0377);
		}

	snstat(sfd, rathole, GETSIG);
	if ((rathole[0]&0377) != 4)
		trouble(FATTRB, 0, "unexpected signal byte %d, 201 used", rathole[0]&377);
}


dem_dis()
{
	FCLOSE(ofd);
	FCLOSE(tfd);
	if(sfd >= 0){
		close(sfd);
		sfd = -1;
	}
}


hungup()
{
	trouble(BADTRB, 10, "timeout, 201 used");
}

/*
 * Retrieve files from GCOS via dataphone
 */

#define	SPIDER	0
#define	PHONE	1
#define	LPD	0
#define	FGET	1

char	dname[] = "FGETP";

#include	"gcos.c"
#include	"fgdemon.c"

#define	FLUSH	(*output)(inp, lbuf-inp);  lbuf = inp;
#define	BADCHAR	cgset[cp[1] - 040] & 040

#define	NCONERR	4
#define	BUSY	-1
#define	TIMEOUT	-2
#define	BADID	-3
#define	ERROR	-4
char	conerr[NCONERR+1][8] = {"BUSY","TIMEOUT","BAD ID","ERROR","UNKNOWN"};
char	conmes[] = "Can't connect to GCOS: %-13s";

char	inp[BUFSIZ];
char	*ebuf;
char	*lbuf	= inp;
char	savc;
int	state;
int	ioc;
char	ibuf[BUFSIZ];


fgd_read()
{
	register i, n;
	register char *cp;
	extern char cgset[];

	while((i = connect(fget_id)) < 0) {
		if(i == TIMEOUT)
			trouble(BADTRB, 60,"TIMEOUT");
		if((i = -i) > NCONERR+1)  i = NCONERR+1;
		trouble(BADTRB, 60, conmes, conerr[i-1]);
	}

	if((ioc = send("$*$out", ibuf)) != 0103) {
#ifdef DEBUG
		fprintf(stderr, "ioc = %o; *ibuf = %o\n", ioc, *ibuf);	/*DEBUG*/
		fprintf(stderr, "out\n");	/*DEBUG*/
#endif
		trouble(BADTRB, 60,"out: ioc=%o", ioc);
	}

	if((ioc = gerts(0102, 0100, (char *)0, ibuf)) != 0102) {
#ifdef DEBUG
		fprintf(stderr, "sack\n");	/*DEBUG*/
#endif
		trouble(BADTRB, 60,"sack: ioc=%o", ioc);
	}

	if((ioc = gerts(0102, 0100, (char *)0, ibuf)) != 0100) {
#ifdef DEBUG
		fprintf(stderr, "data\n");	/*DEBUG*/
#endif
		trouble(BADTRB, 60,"data: ioc=%o", ioc);
	}

	ebuf = inp + BUFSIZ;
loop:
	if(ibuf[1] != 0127)	goto rrec;
	if(ibuf[2] == '!' && ibuf[3] == '!') {
		FLUSH
		endfile();
		state = 0;
		goto rrec;
	}
	cp = ibuf+1;
	if(BADCHAR)    goto rrec;
	state = 0;
	cp = ibuf+2;
cloop:
	if(*cp == 036) {
		if(*++cp != 0127)	goto rrec;
		if(cp[1] == '!' && cp[2] == '!') {
			FLUSH
			endfile();
			goto rrec;
		}
		if(BADCHAR){
#ifdef DEBUG
			fprintf(stderr, "unexpected high bits\n");
#endif
			trouble(BADTRB, 60, "unexpected high bits");
		}
		cp++;
	}
	if(cp >= ibuf+BUFSIZ) {
#ifdef DEBUG
		fprintf(stderr, "overflow\n");	/*DEBUG*/
#endif
		trouble(BADTRB, 60,"overflow");
	}
	if(cp[1] == 037) {
		n = cgset[cp[2]-040]+1;
		while(n--)
			if(putcod(*cp)){
				FLUSH
			}
		cp += 3;
		goto cloop;
	}
	if(putcod(*cp++)){
		FLUSH
	}
	goto cloop;
rrec:
	if((ioc = gerts(0110, 0100, (char *)0, ibuf)) == 0100) {
#ifdef DEBUG
		fprintf(stderr, "%o %o %o %o\n", ibuf[0], ibuf[1],
			ibuf[2], ibuf[3]);	/*DEBUG*/
#endif
		goto loop;
	}
	return;
}


dem_dis()
{
	FCLOSE(ofd);
	FCLOSE(tfd);
	gerts_dis();
}


putcod(ch)
{
	extern char cgset[];
	register c, code;

	if(ch < 040 || ch >= 0140) {
#ifdef DEBUG
		fprintf(stderr, "%o: ", ch);		/*DEBUG*/
#endif
		fgderr("Bad code: %o, in file %s", ch, fname);
	}
	c = cgset[ch-040];
	switch(state) {

	case 0:
		state++;
		savc = c;
		return(0);

	case 1:
		state++;
		code = (savc<<3)|(c>>3);
		savc = c&07;
		break;

	case 2:
		state = 0;
		code = c | ( savc<<6);
		break;
	}

#ifdef DEBUG
	putc(code, stderr);	/*DEBUG*/
#endif
	*lbuf = code;
	if(++lbuf >= ebuf || code == '\n')
		return(1);
	return(0);
}

fgderr(s, ch, f)
char	*s;
char	ch;
char	*f;
{

	if(ofd != NULL){
		fprintf(ofd, s, ch, f);
		putc('\n', ofd);
	}
	trouble(BADTRB, 60,s, ch, f);
}

send(card, gbuf)
char	*card;
char	*gbuf;
{

	char	image[200];
	register char	*cp;
	int	fc;

	if(*card == '$')	fc = 0104;
	else			fc = 0110;

	cp = image;
	*cp++ = 'H';
	while(*cp++ = *card++);

	cp[-1] = '\36';
	*cp = '\3';


	return(gerts(fc, 0100, image, gbuf));
}

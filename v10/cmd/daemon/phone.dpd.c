#
/*
 * dpd -- data-phone daemon dispatcher
 *
 */

#define	SPIDER	0
#define	PHONE	1
#define	LPD	0
#define	FGET	0

char	dname[] = "DPDP";
char	dpd[]	= "/usr/spool/dpd";
char	dfname[30] = "/usr/spool/dpd/";
char	lock[]	= "/usr/spool/dpd/lock";
char	error[]	= "/usr/spool/dpd/errors";

#include	"daemon.c"
#include	"gcos.c"

/*
 * The remaining part is the data phone interface.
 */

#define	NCONERR	4
#define	BUSY	-1
#define	TIMEOUT	-2
#define	BADID	-3
#define	ERROR	-4
char	conerr[NCONERR+1][8] = {"BUSY","TIMEOUT","BAD ID","ERROR","UNKNOWN"};

#define	FC4	0104
#define	FC10	0110
#define	OC	0100
#define	ACKOC	0102
#define	MCBCD	0110
#define	MCBIN	0120
#define	RS	036
#define	ETX	03
#define	MASK6	077

char	snumbf[]	= "/usr/spool/dpd/snumb";
char	scard[]	= "H$      snumb   XXxxx,31\036\003";
char	rcd[]	= "H$*$rcd\036\003";

FILE	*cfb = NULL;
int	snum	= 0;
int	ioc;

dem_con()
{
	int i;

	if((i = connect(phone_id)) < 0) {
		if((i = -i) > NCONERR+1)  i = NCONERR+1;
		logerr("Can't reach GCOS: %-8s", conerr[i-1]);
		return(i);
	}
	if((ioc = gerts(FC4, OC, rcd, NULL)) != ACKOC){
		dem_dis();
		logerr("Bad ACK on rcd: ACK-OC = %-11o", ioc);
		return(1);
	}
	return(0);
}

dem_dis()
{
	gerts_dis();
	FCLOSE(dfb);
	FCLOSE(cfb);
}

dem_open(file)
char	*file;
{
}

dem_close()
{
}

get_snumb()
{
	FILE *f;
	int i, temp;

		if((f = fopen(snumbf, "r")) != NULL){
			fread((char *)&snum, 2, 1, f);
			fclose(f);
		}
		snum++;
		scard[16] = phone_id[0];
		scard[17] = phone_id[1];
		temp = snum;
		for(i = 20; i > 17; i--){
			scard[i] = (temp % 10) | '0';
			temp = temp/10;
		}
		scard[21] = ',';
		if(temp > 0)
			snum = 0;
		if((f = fopen(snumbf, "w")) == NULL)
			trouble("Can't write %s", snumbf);
		fwrite((char *)&snum, 2, 1, f);
		fclose(f);
		gwrite(FC4, scard, 25);
		snumb = &scard[16];
		scard[21] = 0;
}

lwrite()
{
		line[0] = MCBCD;
		line[linel++] = RS;
		if(line[1] == '$')
			gwrite(FC4, line, linel);
		else
			gwrite(FC10, line, linel);
}


sascii(fff)
{
	static char obuf[400];
	int io, j1, j2;
	int c1, c2, eof;
	int temp;
	extern char gcset[];

	if((cfb = fopen(&line[1], "r")) == NULL){
		logerr("Can't open %s", &line[1]);
		return(0);
	}
	io = eof = 0;
	if(fff)
		c1 = ff;
	else
	    if((c1 = getc(cfb)) == EOF)
		eof = 1;
	while(eof == 0){
	    for(j1 = 1; j1 <= 2; j1++){
		obuf[io++] = MCBIN;
		for(j2 = 1; j2 <= 52; j2++){
			obuf[io++] = gcset[c1>>3 & MASK6];
			if((c2 = getc(cfb)) == EOF){
			    c2 = 0;
			    eof = 1;
			}
			temp = (c1<<3 | (c2>>6 & 07));
			obuf[io++] = gcset[temp & MASK6];
			obuf[io++] = gcset[c2 & MASK6];
			if((c1 = getc(cfb)) == EOF)
			    eof = 1;
			if(eof)
			    break;
		}
		obuf[io++] = RS;
		if(eof)
			break;
	    }
	    gwrite(FC10, obuf, io);
	    io = 0;
	}
	FCLOSE(cfb);
	return(0);
}

gwrite(fc, b, n)
int	fc;
char	*b;
int	n;
{
	register i;
	static char outbuf[402];

	b[n] = ETX;
	ioc = gerts(fc, OC, b, outbuf);
	for(i = 0; i <= 10; i++){
		if(ioc == ACKOC){
			snsum += n-2;
			return;
		}
#ifdef DEBUG
		for(i = 1; outbuf[i] != ETX && i < 400; i++) ;
		outbuf[i+1] = 0;
		fprintf(stderr, "ioc=%o, fc=%o, buf='%s'.\n",
			ioc, outbuf[0], &outbuf[1]);
#endif
		if(ioc != 0100)
			break;
		ioc = gerts(0115, OC, NULL, outbuf);
	}
	trouble("write error: ioc =%6o, pid %-6s",
		ioc, dfname+LPID);
}

/* VARARGS */
trouble(s, a1, a2, a3, a4)
char	*s;
int	a1, a2, a3, a4;
{

	FCLOSE(dfb);
	FCLOSE(cfb);
	if(retcode != 0)
		gerts_dis();
	logerr(s, a1, a2, a3, a4);
	longjmp(env, 1);
}

/*
|| dslib.c - library routines for /dev/scsi
||
|| Copyright 1988, 1989, by
||   Gene Dronek (Vulcan Laboratory) and
||   Rich Morin  (Canta Forda Computer Laboratory).
|| All rights reserved.
*/
#ident	"dslib.c: $Revision: 1.4 $"

#include <stdio.h>
#include <sys/types.h>

#include "dslib.h"
#ifdef aux
#include <sys/vio.h>
#include <sys/scsireq.h>
#endif aux

int dsdebug=0;
long dsreqflags;	/* flag bits always set by filldsreq */

#define min(i,j)  ( (i) < (j) ? (i) : (j) )


/*
|| Startup/shutdown -----------------------------------------------
*/

static struct context *dsc[FDSIZ];


/*
|| dsopen - open device, set up structures
*/

struct dsreq *
dsopen(opath, oflags)
  char *opath;
  int   oflags;
{
    
  struct dsreq *dsp;
  struct context *cp;
  int fd;
  DSDBG(fprintf(stderr,"dsopen(%s,%x) ", opath, oflags));

  fd = open(opath, oflags);
  if (fd < 0)						
    return NULL;  			/* can't open	*/
  if (dsc[fd] != NULL)		        /* already in use */
    ds_zot("dsopen: fd already in use");

  cp = (struct context *) calloc(1, sizeof(struct context));
  if (cp == NULL)				      /* can't allocate	*/
    ds_zot("dsopen: can't allocate space");
  dsc[fd] = cp;
  cp->dsc_fd = fd;
  dsp = &(cp->dsc_dsreq);

  dsp->ds_flags =	0;
  dsp->ds_time =	10 * 1000;	/* 10 second default timeout */
  dsp->ds_private =	(ulong) cp;	/* pointer back to context */
  dsp->ds_cmdbuf = 	cp->dsc_cmd;
  dsp->ds_cmdlen = 	sizeof cp->dsc_cmd;
  dsp->ds_databuf = 	0;
  dsp->ds_datalen = 	0;
  dsp->ds_sensebuf =	cp->dsc_sense;
  dsp->ds_senselen = 	sizeof cp->dsc_sense;
  DSDBG(fprintf(stderr,"=>cp %x, dsp %x\n", cp, dsp));
  return dsp;
}


/*
|| dsclose - close device, release context struct.
*/

dsclose(dsp)
  struct dsreq *dsp;
{
  int fd;
  struct context *cp;

  if (dsp == NULL)
    ds_zot("dsclose: dsp is NULL");

  cp = (struct context *)dsp->ds_private;
  fd = getfd(dsp);
  if ( cp == NULL )
    ds_zot("dsclose: private is NULL");

  cfree(cp);
  dsc[fd] = (struct context *)NULL;
  return;
}


/*
|| Generic SCSI CCS Command functions ------------------------------------
||
|| dsp		dsreq pointer
|| data		data buffer pointer
|| datalen	data buffer length
|| lba		logical block address
|| vu		vendor unique bits
*/

/*
|| testunitready00 - issue group 0 "Test Unit Ready" command (0x00)
*/

testunitready00(dsp)
  struct dsreq *dsp;
{
  fillg0cmd(dsp, CMDBUF(dsp), G0_TEST, 0, 0, 0, 0, 0);
  filldsreq(dsp, 0, 0, DSRQ_READ|DSRQ_SENSE);
  return(doscsireq(getfd(dsp), dsp));
}


/*
|| requestsense03 - issue group 0 "Request Sense" command (0x03)
*/

requestsense03(dsp, data, datalen, vu)
  struct dsreq *dsp;
  caddr_t data;
  long datalen;
  char vu;
{
  fillg0cmd(dsp, CMDBUF(dsp), G0_REQU, 0, 0, 0, B1(datalen), B1(vu<<6));
  filldsreq(dsp, data, datalen, DSRQ_READ);
  return(doscsireq(getfd(dsp), dsp));
}


/*
|| write0a - issue group 0 "Write" command (0x0a)
*/

write0a(dsp, data, datalen, lba, vu)
  struct dsreq *dsp;
  caddr_t data;
  long datalen, lba;
  char vu;
{
  fillg0cmd(dsp, CMDBUF(dsp), G0_WRIT, B3(lba), B1(datalen), B1(vu<<6));
  filldsreq(dsp, data, datalen, DSRQ_READ);
  return(doscsireq(getfd(dsp), dsp));
}


/*
|| inquiry12 - issue group 0 "Inquiry" command (0x12)
*/

inquiry12(dsp, data, datalen, vu)
  struct dsreq *dsp;
  caddr_t data;
  long datalen;
  char vu;
{
  fillg0cmd(dsp, CMDBUF(dsp), G0_INQU, 0, 0, 0, B1(datalen), B1(vu<<6));
  filldsreq(dsp, data, datalen, DSRQ_READ|DSRQ_SENSE);
  return(doscsireq(getfd(dsp), dsp));
}


/*
|| modeselect15 - issue group 0 "Mode Select" command (0x15)
||
|| save		0 - don't save saveable pages
|| 		1 - save saveable pages
*/

modeselect15(dsp, data, datalen, save, vu)
  struct dsreq *dsp;
  caddr_t data;
  long datalen;
  char save, vu;
{
  fillg0cmd(dsp, CMDBUF(dsp), G0_MSEL, save&1, 0, 0, B1(datalen), B1(vu<<6));
  filldsreq(dsp, data, datalen, DSRQ_WRITE|DSRQ_SENSE);
  return(doscsireq(getfd(dsp), dsp));
}


/*
|| modesense1a - issue group 0 "Mode Sense" command (0x1a)
||
|| pagectrl	0 - current values
||		1 - changeable values
||		2 - default values
||		3 - saved values
||
|| pagecode	0   - vendor unique
||		1   - error recovery
||		2   - disconnect/reconnect
||		3   - direct access dev. fmt.
||		4   - rigid disk geometry
||		5   - flexible disk
||		6-9 - see specific dev. types
||		0a  - implemented options
||		0b  - medium types supported
||		3f  - return all pages
*/

modesense1a(dsp, data, datalen, pagectrl, pagecode, vu)
  struct dsreq *dsp;
  caddr_t data;
  long datalen;
  char pagectrl, pagecode, vu;
{
  fillg0cmd(dsp, CMDBUF(dsp), G0_MSEN, 0x10,
    ((pagectrl&3)<<6) | (pagecode&0x3F),
    0, B1(datalen), B1(vu<<6));
  filldsreq(dsp, data, datalen, DSRQ_READ|DSRQ_SENSE);
  return(doscsireq(getfd(dsp), dsp));
}


/*
|| senddiagnostic1d - issue group 0 "Send Diagnostic" command (0x1d)
||
|| self		0 - run test, hold results
||		1 - run test, return status
||
|| dofl		0 - device online
||		1 - device offline
||
|| uofl		0 - unit online
||		1 - unit offline
*/

senddiagnostic1d(dsp, data, datalen, self, dofl, uofl, vu)
  struct dsreq *dsp;
  caddr_t data;
  long datalen;
  char self, dofl, uofl, vu;
{
  fillg0cmd(dsp, CMDBUF(dsp), G0_MSEN,
    (self&1)<<2 | (dofl&1)<<1 | (uofl&1),
    0, B2(datalen), B1(vu<<6));
  filldsreq(dsp, data, datalen, DSRQ_READ|DSRQ_SENSE);
  return(doscsireq(getfd(dsp), dsp));
}


/*
|| readcapacity25 - issue group 1 "Read Capacity" command (0x25)
||
|| pmi		0 - return last logical block, entire unit
||		1 - return last logical block, current track
*/

readcapacity25(dsp, data, datalen, lba, pmi, vu)
  struct dsreq *dsp;
  caddr_t data;
  long datalen, lba;
  char pmi, vu;
{
  fillg1cmd(dsp, CMDBUF(dsp), G1_RCAP, 0, B4(lba), 0, 0, pmi&1, B1(vu<<6));
  filldsreq(dsp, data, datalen, DSRQ_READ|DSRQ_SENSE
    /* |DSRQ_CTRL2 */ );
  /* dsp->ds_time = 100;	/* often takes a while */
  return(doscsireq(getfd(dsp), dsp));
}


/*
|| readextended28 - issue group 1 "Read Extended" command (0x28)
*/

readextended28(dsp, data, datalen, lba, vu)
  struct dsreq *dsp;
  caddr_t data;
  long datalen, lba;
  char vu;
{
  fillg1cmd(dsp, CMDBUF(dsp), G1_READ, 0, B4(lba), 0, B2(datalen), B1(vu<<6));
  filldsreq(dsp, data, datalen, DSRQ_READ|DSRQ_SENSE
    /* |DSRQ_CTRL2 */ );
  /* dsp->ds_time = 100;	/* often takes a while */
  return(doscsireq(getfd(dsp), dsp));
}


/*
|| writeextended2a - issue group 1 "Write Extended" command (0x2a)
*/

writeextended2a(dsp, data, datalen, lba, vu)
  struct dsreq *dsp;
  caddr_t data;
  long datalen, lba;
  char vu;
{
  fillg1cmd(dsp, CMDBUF(dsp), G1_WRIT, 0, B4(lba), 0, B2(datalen), B1(vu<<6));
  filldsreq(dsp, data, datalen, DSRQ_READ|DSRQ_SENSE
    /* |DSRQ_CTRL2 */ );
  /* dsp->ds_time = 100;	/* often takes a while */
  return(doscsireq(getfd(dsp), dsp));
}


/*
|| Support functions ----------------------------------------------------
*/

/*
|| fillg0cmd - Fill a Group 0 command buffer
*/

fillg0cmd(dsp, cmd, b0,b1,b2,b3,b4,b5)
  struct dsreq *dsp;
  uchar_t *cmd, b0,b1,b2,b3,b4,b5;
{
  uchar_t *c = cmd;
  DSDBG(fprintf(stderr,"fillg0cmd(%x,%x, %02x %02x %02x %02x %02x %02x)\n",
		dsp, cmd, b0,b1,b2,b3,b4,b5));
  *c++ = b0, *c++ = b1, *c++ = b2, *c++ = b3, *c++ = b4, *c++ = b5;
	
  CMDBUF(dsp) = (caddr_t) cmd;
  CMDLEN(dsp) = 6;
}


/*
|| fillg1cmd - Fill a Group 1 command buffer
*/

fillg1cmd(dsp, cmd, b0,b1,b2,b3,b4,b5,b6,b7,b8,b9)
  struct dsreq *dsp;
  uchar_t *cmd, b0,b1,b2,b3,b4,b5,b6,b7,b8,b9;
{
  uchar_t *c = cmd;
  DSDBG(fprintf(stderr,
    "fillg1cmd(%x,%x, %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x)\n",
		dsp, cmd, b0,b1,b2,b3,b4,b5,b6,b7,b8,b9));

  *c++ = b0, *c++ = b1, *c++ = b2, *c++ = b3, *c++ = b4, *c++ = b5;
  *c++ = b6, *c++ = b7, *c++ = b8, *c++ = b9;
	
  CMDBUF(dsp) = (caddr_t) cmd;
  CMDLEN(dsp) = 10;
}


/*
|| filldsreq - Fill a dsreq structure
*/

filldsreq(dsp,data,datalen,flags)
  struct dsreq		*dsp;
  uchar_t		*data;
{
  DSDBG(fprintf(stderr,"filldsreq(%x,%x,%d,%x) cmdlen %d\n",
		dsp,data,datalen,flags,CMDLEN(dsp)));
  dsp->ds_flags	= flags | dsreqflags |
	  (((dsdebug&1) ? DSRQ_TRACE : 0) |
	  ((dsdebug&2) ? DSRQ_PRINT : 0));
  dsp->ds_time	= 10 * 1000;	/* default to 10 seconds */
  dsp->ds_link	= 0;
  dsp->ds_synch	= 0;
  dsp->ds_ret  	= 0;

  DATABUF(dsp) 	= (caddr_t) data;
  DATALEN(dsp)	= datalen;
}


/*
|| bprint - print array of bytes, in hex.
*/

#define hex(x) "0123456789ABCDEF" [ (x) & 0xF ]

bprint(s,n,nperline,space)
	char *s;
{
	int   i, x;
	char  *sp = (space) ? " ": "";

	for(i=0;i<n;i++)  {
		x = s[i];
		fprintf(stderr,((i%4==3)?"%c%c%s%s":"%c%c%s"),
			hex(x>>4), hex(x), sp, sp);
		if ( i%nperline == (nperline - 1) )
			fprintf(stderr,"\n");
	}
	if ( space )
		fprintf(stderr,"\n");
}


/*
|| doscsireq - issue scsi command, return status or -1 error.
*/

doscsireq( fd, dsp)
  int	fd;		/* ioctl file descriptor */
  struct dsreq *dsp;	/* devscsi request packet */
{
  int	cc;
  int	retries = 4;
  uchar_t	sbyte;

  DSDBG(fprintf(stderr,"doscsireq(%d,%x) %x ---- %s\n",fd,dsp,
    (CMDBUF(dsp))[0],
    ds_vtostr( (CMDBUF(dsp))[0], cmdnametab)));

  /*
   *  loop, issuing command
   *    until done, or further retry pointless
   */

  while ( --retries > 0 )  {

   caddr_t sp;

    sp =  SENSEBUF(dsp);
    DSDBG(fprintf(stderr,"cmdbuf   =  ");
		bprint(CMDBUF(dsp),CMDLEN(dsp),16,1));
    if ( (dsp->ds_flags & DSRQ_WRITE) )
      DSDBG(bprint( DATABUF(dsp), min(50,DATALEN(dsp)),16,1 ));
  	
DSDBG(fprintf(stderr,"databuf datalen %x %d\n",DATABUF(dsp), DATALEN(dsp)));
    cc = ioctl( fd, DS_ENTER, dsp);
    if ( cc < 0)  {
      ds_panic(dsp, "cannot ioctl fd %d\n",fd);
    }
  	
	DSDBG(fprintf(stderr,"cmdlen after ioctl=%d\n",CMDLEN(dsp)));
    DSDBG(fprintf(stderr,"ioctl=%d ret=%x %s",
      cc, RET(dsp), 
      RET(dsp) ? ds_vtostr(RET(dsp),dsrtnametab) : ""));
    DSDBG(if (SENSESENT(dsp)) fprintf(stderr," sensesent=%d",
      SENSESENT(dsp)));

    DSDBG(fprintf(stderr,
      " cmdsent=%d datasent=%d sbyte=%x %s\n",
      CMDSENT(dsp), DATASENT(dsp), STATUS(dsp),
      ds_vtostr(STATUS(dsp), cmdstatustab)));
    DSDBG(if ( FLAGS(dsp) & DSRQ_READ )
      bprint( DATABUF(dsp), min(16*16,DATASENT(dsp)), 16,1));

#ifdef aux
  /*
   *  check for AUX bus-error 
   *  we retry with poll-dma
   */
    if ( RET(dsp) == DSRT_AGAIN )  {
      int n = SDC_RDPOLL|SDC_WRPOLL;
      DSDBG(fprintf(stderr,"setting rd/wr-poll"));
      cc = ioctl( fd, DS_SET, n);	/* set bits */
      if ( cc != 0 )
        return -1;
    }
#endif aux

    if ( RET(dsp) == DSRT_NOSEL )
      continue;		/* retry noselect 3X */

    /* decode sense data returned */
    if ( SENSESENT(dsp) )  {
      DSDBG(
        fprintf(stderr, "sense key %x - %s\n",
          SENSEKEY(sp),
          ds_vtostr( SENSEKEY(sp), sensekeytab));
        bprint( SENSEBUF(dsp),
          min(100, SENSESENT(dsp)),
          16,1);
      );
    }
    DSDBG(fprintf(stderr, "sbyte %x\n", STATUS(dsp)));

    /* decode scsi command status byte */
    sbyte = STATUS(dsp);
    switch (sbyte)  {
      case 0x08:		/*  BUSY */
      case 0x18:		/*  RESERV CONFLICT */
    	sleep(2);
    	continue;
      case 0x00:		/*  GOOD */
      case 0x02:		/*  CHECK CONDITION */
      case 0x10:		/*  INTERM/GOOD */
      default:
    	return sbyte;
    }
  }
  return -1;	/* fail retry limit */
}


/*
|| opttovar - lookup option in table, return var addr (NULL if fail)
*/

int *
opttovar( ostr, table)
  char *ostr;
  struct opttab{
    char *opt;
    int  *var;
  } *table;
{
  register struct opttab *tp;

  for (tp=table; (tp->var); tp++)
    if ( strncmp( ostr, tp->opt, 3) == 0 )
      break;

  if ( !tp->var )
    fprintf(stderr,"unknown option %s", ostr);
	
  return (tp->var);
}


/*
|| ds_vtostr - lookup value in table to return string pointer
*/

char *
ds_vtostr( v, table)
  long v;
  struct vtab *table;
{
  register struct vtab *tp;

  for (tp=table; (tp->string); tp++)
    if ( v == tp->val )
      break;
	
  return (tp->string) ? tp->string : "";
}


/*
|| ds_panic - yelp, leave...
*/

ds_panic( fmt, v)
  char *fmt;
  int v;
{
  extern errno;

  fprintf(stderr,fmt,v);
  fprintf(stderr,"\nerrno = %d\n",errno);
  exit(1);
}


/*
|| ds_zot - go away, with a message.
*/

ds_zot(message)
  char *message;
{
  fprintf(stderr, "%s\n", message);
  exit(1);
}

#ifndef	LOADER
#define	LOADER	"/usr/DMD/bin/32ld"
#endif	LOADER

/*
 * sysint.c - this guy sits on the host end and interprets
 *	library calls.
 */

#include <stdio.h>
#include <sgtty.h>
#include "comm.h"

int jerqno;
char jerqtty[] = "/dev/tty";
char *getenv(),*cttyname();
FILE *files[15];
FILE *tojerq,*fromjerq;
char obuf[256];
char *load = LOADER;
FILE *popen();
extern char *strcmp();

main()
{
	int i;

	fromjerq = fopen(jerqtty,"r");
	if ((tojerq = fopen(jerqtty,"w")) == NULL){
		fprintf(stderr,"can't write to %s\n",jerqtty);
		return 1;
	}
	jerqno = fileno(tojerq);
	(void) ioctl( tojerq, TIOCEXCL, 0);
	setbuf(tojerq,obuf);
	setbuf(fromjerq,NULL);
	listen();
	(void) ioctl( tojerq, TIOCNXCL , 0);	
	return 0;
}

jputchar(c)
{
	putc(c,tojerq);
}

jputshort(s)
short	s;
{
	jputchar(s>>8);
	jputchar(s&0xFF);
}

jputbuf(p,n)
register char *p;
register short n;
{
	jputshort(n);
	while (n-- > 0)
		jputchar(*p++);
}

jputstr(s)
register char *s;
{
	register char c;
	do {
		jputchar(c = *s++);
	} while (c != '\0');
}

jgetchar()
{
	int c;
	c = getc(fromjerq);
	return(c);
}

short
jgetshort()
{
	short	s;
	s = jgetchar() << 8;
	s += jgetchar();
	return s;
}

jgetbuf(p)
register char *p;
{
	register short i,n;
	n = jgetshort();
	for (i = 0; i < n; i++)
		*p++ = jgetchar();
	return(n);
}

jgetstr(s)
register char *s;
{
	while (*s++ = jgetchar());
}

listen()
{
	int n,c,i;
	char filename[50],buf[1000],mode[2];
	char vname[60];
	char *xname;
	files[0] = stdin;
	files[1] = stdout;
	files[2] = stderr;
	mode[1] = '\0';
	jputchar(_SYSINIT);
	fflush(tojerq);
	while ((c = jgetchar()) != EOF) {
		while (c != _SYSTAG) {	/* I KNOW it's awful */
			if (c == EOF || c == 0177)
				return(0);
			c = jgetchar();
		}
		c = jgetchar();
		switch (c) {
		case _SYSSYNC:
			jputchar(_SYSSYNC);
			break;
		case _SYSOPEN:
			i = jgetshort();
			jgetstr(filename);
			*mode = jgetchar();
			if ((files[i] = fopen(filename,mode)) == NULL)
				jputshort(-1);
			else
				jputshort(i);
			break;
		case _SYSPOPEN:
			i = jgetshort();
			jgetstr(filename);
			*mode = jgetchar();
			if ((files[i] = popen(filename,mode)) == NULL)
				jputshort(-1);
			else
				jputshort(i);
			break;
		case _SYSCLOSE:
			i = jgetshort();
			jputshort(fclose(files[i]));
			break;
		case _SYSPCLOSE:
			i = jgetshort();
			jputshort(pclose(files[i]));
			break;
		case _SYSWRITE:
			i = jgetshort();
			n = jgetbuf(buf);
			jputshort(fwrite(buf,1,n,files[i]));
			fflush(files[i]);
			break;
		case _SYSEXIT:
			i = jgetshort();
			jputshort(i);
			return(i);
		case _SYSREAD:
			i = jgetshort();
			n = jgetshort();
			n = fread(buf,1,n,files[i]);
			jputbuf(buf,n);
			break;
		case _SYSACCESS:
			jgetstr(filename);
			i = jgetshort();
			jputshort(access(filename,i));
			break;
/*
		case _GETENV:
			jgetstr( vname ); 
			xname = getenv( vname );
			jputstr( xname );
			break;
*/
		default:
			fprintf(stderr,"unrecognized system call: %d\n",c);
		}
		fflush(tojerq);
	}
}




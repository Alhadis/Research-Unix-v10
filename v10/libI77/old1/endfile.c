#include "f2c.h"
#include "fio.h"
#include "sys/types.h"
#include "fcntl.h"
#include "rawio.h"
#ifndef O_RDONLY
#define O_RDONLY 0
#endif

#ifdef KR_headers
extern char *strcpy();
#else
#undef abs
#undef min
#undef max
#include "stdlib.h"
#include "string.h"
#endif


#ifdef KR_headers
integer f_end(a) alist *a;
#else
integer f_end(alist *a)
#endif
{
	unit *b;
	if(a->aunit>=MXUNIT || a->aunit<0) err(a->aerr,101,"endfile");
	b = &f__units[a->aunit];
	if(b->ufd==NULL) {
		char nbuf[10];
		(void) sprintf(nbuf,"fort.%ld",a->aunit);
		close(creat(nbuf, 0666));
		return(0);
		}
	b->uend=1;
	return(b->useek ? t_runc(a) : 0);
}

 static int
#ifdef KR_headers
copy(from, len, to) char *from, *to; register long len;
#else
copy(char *from, register long len, char *to)
#endif
{
	register int n;
	int k, rc = 0, tmp;
	char buf[BUFSIZ];

	if ((k = open(from, O_RDONLY)) < 0)
		return 1;
	if ((tmp = creat(to,0666)) < 0)
		return 1;
	while((n = read(k, buf, len > BUFSIZ ? BUFSIZ : (int)len)) > 0) {
		if (write(tmp, buf, n) != n)
			{ rc = 1; break; }
		if ((len -= n) <= 0)
			break;
		}
	close(k);
	close(tmp);
	return n < 0 ? 1 : rc;
	}

#ifndef L_tmpnam
#define L_tmpnam 16
#endif

 int
#ifdef KR_headers
t_runc(a) alist *a;
#else
t_runc(alist *a)
#endif
{
	char nm[L_tmpnam+12];	/* extra space in case L_tmpnam is tiny */
	long loc, len;
	unit *b;
	int rc = 0;

	b = &f__units[a->aunit];
	if(b->url) return(0);	/*don't truncate direct files*/
	loc=ftell(b->ufd);
	(void) fseek(b->ufd,0L,SEEK_END);
	len=ftell(b->ufd);
	if (loc >= len || b->useek == 0 || b->ufnm == NULL)
		return(0);
#ifdef MSDOS
	fclose(b->ufd);
#else
	rewind(b->ufd);	/* empty buffer */
#endif
	if (!loc) {
		if (close(creat(b->ufnm,0666)))
#ifdef MSDOS
			rc = 1;
		goto done;
#else
			{ rc = 1; goto done; }
		if (b->uwrt)
			b->uwrt = 1;
		return 0;
#endif
		}
#ifdef _POSIX_SOURCE
	tmpnam(nm);
#else
	(void) strcpy(nm,"tmp.FXXXXXX");
	(void) mktemp(nm);
#endif
	if (copy(b->ufnm, loc, nm)
	 || copy(nm, loc, b->ufnm))
		rc = 1;
	unlink(nm);
done:
#ifdef MSDOS
	{
	extern char *f__r_mode[];
	b->uwrt = 0;
	if (!(b->ufd = fopen(b->ufnm, f__r_mode[b->ufmt])))
		rc = 1;
	}
	if (loc)
#endif
	fseek(b->ufd, loc, SEEK_SET);
	if (rc)
		err(a->aerr,111,"endfile");
	return 0;
	}

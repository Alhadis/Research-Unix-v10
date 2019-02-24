#include "sys/param.h"
#include "sys/buf.h"
#include "sys/mtpr.h"
#include "sys/msgbuf.h"
#include "sys/user.h"
#include "sys/pte.h"

/*
 * In case console is off,
 * panicstr contains argument to last
 * call to panic.
 */
char	*panicstr;

/*
 * Scaled down version of C Library printf.
 * Used to print diagnostic information directly on console tty.
 * Since it is not interrupt driven, all system activities are
 * suspended.  Printf should not be used for chit-chat.
 */
/*VARARGS1*/
printf(fmt, x1)
	char *fmt;
	unsigned x1;
{

	prf(fmt, &x1, 0);
}

prf(fmt, adx, touser)
	register char *fmt;
	register u_int *adx;
{
	register int b, c, i;
	char *s;
	int any;

loop:
	while ((c = *fmt++) != '%') {
		if(c == '\0')
			return;
		putchar(c, touser);
	}
again:
	c = *fmt++;
	/* THIS CODE IS VAX DEPENDENT IN HANDLING %l? AND %c */
	switch (c) {

	case 'l':
		goto again;
	case 'x': case 'X':
		b = 16;
		goto number;
	case 'd': case 'D':
	case 'u':		/* what a joke */
		b = 10;
		goto number;
	case 'o': case 'O':
		b = 8;
number:
		printn((u_long)*adx, b, touser);
		break;
	case 'c':
		b = *adx;
		for (i = 24; i >= 0; i -= 8)
			if (c = (b >> i) & 0x7f)
				putchar(c, touser);
		break;
	case 'b':
		b = *adx++;
		s = (char *)*adx;
		printn((u_long)b, *s++, touser);
		any = 0;
		if (b) {
			putchar('<', touser);
			while (i = *s++) {
				if (b & (1 << (i-1))) {
					if (any)
						putchar(',', touser);
					any = 1;
					for (; (c = *s) > 32; s++)
						putchar(c, touser);
				} else
					for (; *s > 32; s++)
						;
			}
			if (any)
				putchar('>', touser);
		}
		break;

	case 's':
		s = (char *)*adx;
		while (c = *s++)
			putchar(c, touser);
		break;

	case '%':
		putchar('%', touser);
		break;
	}
	adx++;
	goto loop;
}

/*
 * Printn prints a number n in base b.
 * We don't use recursion to avoid deep kernel stacks.
 */
printn(n, b, touser)
	u_long n;
{
	char prbuf[11];
	register char *cp;

	if (b == 10 && (int)n < 0) {
		putchar('-', touser);
		n = (unsigned)(-(int)n);
	}
	cp = prbuf;
	do {
		*cp++ = "0123456789abcdef"[n%b];
		n /= b;
	} while (n);
	do
		putchar(*--cp, touser);
	while (cp > prbuf);
}

/*
 * Panic is called on unresolvable fatal errors.
 * print `panic: mesg' and boot
 * reboot may call update if it seems safe
 */
panic(s)
	char *s;
{
	static int panicked = 0;

	panicstr = s;
	if (panicked++) {
		printf("panic again: %s\n", s);
		reboot(0);	/* no sync, so probably won't reenter again */
	}
	printf("panic: %s\n", s);
	reboot(1);
}

/*
 * Warn that a system table is full.
 */
tablefull(tab)
	char *tab;
{

	printf("%s: table is full\n", tab);
}

/*
 * Hard error is the preface to plaintive error messages
 * about failing disk transfers.
 * this is hopeless; the driver should do it.
 * the magic constant 07 shows why.
 */
harderr(bp, cp)
	struct buf *bp;
	char *cp;
{

	printf("%s%d%o: hard error sn%d ", cp,
	    dkunit(bp), minor(bp->b_dev)&07, bp->b_blkno);
}

struct pte *msgbufmap;
struct msgbuf *msgbuf;

/*
 * Print a character on console or users terminal.
 * If destination is console then the last MSGBUFS characters
 * are saved in msgbuf for inspection later.
 */
/*ARGSUSED*/
putchar(c, touser)
	register int c;
{

	if (touser) {
		/* put them on user's stdout instead */
		return;
	}
	if (c != '\0' && c != '\r' && c != 0177 && mfpr(MAPEN)) {
		if (msgbuf->msg_magic != MSG_MAGIC) {
			msgbuf->msg_bufx = 0;
			msgbuf->msg_magic = MSG_MAGIC;
		}
		if (msgbuf->msg_bufx < 0 || msgbuf->msg_bufx >= MSG_BSIZE)
			msgbuf->msg_bufx = 0;
		msgbuf->msg_bufc[msgbuf->msg_bufx++] = c;
	}
	if (c == 0)
		return;
	cnputc(c);
}

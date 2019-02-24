
#define	SALIGN(p)	(char *)(((int)p+1) & ~1)
#define	LALIGN(p)	(char *)(((int)p+3) & ~3)
#define	SNEXT(p)	(char *)((int)p + sizeof (short))
#define	LNEXT(p)	(char *)((int)p + sizeof (long))


/*
 * convert from canonical to
 * local representation.
 */
char *
dkfcanon(fmt, from, to)
register char *fmt, *from, *to;
{
short tmp;
long ltmp;
	while (*fmt) {
		switch(*fmt++) {
		case 's':			/* short */
			tmp = 0;
			tmp = (*from++)&0377;
			tmp |= ((*from++)&0377)<<8;
			to = SALIGN(to);
			*(short *)to = tmp;
			to = SNEXT(to);
			continue;
		case 'l':			/* long */
			ltmp = 0;
			ltmp = (*from++)&0377;
			ltmp |= (long)((*from++)&0377)<<8;
			ltmp |= (long)((*from++)&0377)<<16;
			ltmp |= (long)((*from++)&0377)<<24;
			to = LALIGN(to);
			*(long *)to = ltmp;
			to = LNEXT(to);
			continue;
		case 'b':			/* byte */
			*to++ = *from++;
			continue;
		default:
			return((char *)0);
		}
	}
	return(from);
}

/*
 * convert from local to
 * canonical representation
 */
char *
dktcanon(fmt, from, to)
register char *fmt, *from, *to;
{
short tmp;
long ltmp;

	while (*fmt) {
		switch(*fmt++) {
		case 's':
			from = SALIGN(from);
			tmp = *(short *)from;
			from = SNEXT(from);
			*to++ = tmp;
			tmp >>=8;
			*to++ = tmp;
			continue;
		case 'l':
			from = LALIGN(from);
			ltmp = *(long *)from;
			*to++ = ltmp;
			ltmp >>= 8;
			*to++ = ltmp;
			ltmp >>= 8;
			*to++ = ltmp;
			ltmp >>= 8;
			*to++ = ltmp;
			from = LNEXT(from);
			continue;
		case 'b':
			*to++ = *from++;
			continue;
		default:
			return((char *)0);
		}
	}
	return(from);
}


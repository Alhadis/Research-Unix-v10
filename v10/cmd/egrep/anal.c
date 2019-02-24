#include	"hdr.h"
#include	"y.tab.h"

#define	NLIT	256
#define	LSTART	litp = litb
#define	LEND	{*litp = 0; if(strlen(litb) > strlen(blit))strcpy(blit,litb);}

static char *litp, litb[NLIT], *blit;


/*
	islit has to return 1 if we are to use boyer-moore.
	in this case, set buf to the literal string and
	set bmegrep to 1 if there is a regular expression
	beside the literal (literal can be anywhere).
*/
islit(buf)
	char *buf;
{
	bmegrep = 0;
	blit = buf;
	*blit = 0;
	LSTART;

/* pr(line-2, ""); */

	lit(line-2);
	LEND;
	if(strlen(blit) > 1){
		/* bmegrep set by lit */
		return(1);
	}
	return(0);
}

/*
	lit builds literal strings in litb. we ensure these are compulsory
	literal string by only descending the right nodes.
*/
lit(n)
{
	if(name[n] == CAT){
		lit(left[n]);
		lit(right[n]);
	} else if(name[n] == PLUS){
		lit(left[n]);
		LEND;		/* can't go on past a + */
		LSTART;
		bmegrep = 1;
		lit(left[n]);	/* but we can start with one! */
	} else if((name[n] == CCL) && (chars[right[n]] == 1)){
		*litp++ = chars[right[n]+1];
		if(litp == &litb[NLIT])
			litp--;
	} else if(left[n] == 0 && name[n] < 256
			&& name[n] != LEFT && name[n] != RIGHT){
		*litp++ = name[n];
		if(litp == &litb[NLIT])
			litp--;
	} else {
		LEND;
		LSTART;
		bmegrep = 1;
	}
}

/*#ifdef	DEBUG*/

pr(n, s)
	char *s;
{
	char buf[256];
	int i, cnt;

	print("%s", s);
	sprint(buf, "%s   ", s);
	switch(name[n])
	{
	case FINAL:	print("expr=\n"); pr(left[n], buf); break;
	case DOT:	print("%d: .\n",n); break;
	case STAR:	print("%d: *\n",n); pr(left[n], buf); break;
	case PLUS:	print("%d: +\n",n); pr(left[n], buf); break;
	case QUEST:	print("%d: ?\n",n); pr(left[n], buf); break;
	case CAT:	print("%d: cat\n", n); pr(left[n], buf); pr(right[n], buf); break;
	case OR:	print("%d: or\n", n); pr(left[n], buf); pr(right[n], buf); break;
	case NCCL:
	case CCL:	print("%d: [%s", n, name[n]==NCCL? "^":"");
			for(i = right[n], cnt = chars[i++]; cnt > 0; cnt--)
				print("%c", chars[i++]);
			print("]\n");
			break;
	default:
			if(name[n] < 256)
				print("%d: '%c'\n", n, name[n]);
			else
				print("URK %d\n", name[n]);
			break;
	}
}
/*#endif*/

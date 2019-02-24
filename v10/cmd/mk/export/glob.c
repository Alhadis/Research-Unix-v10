#include	<sys/types.h>
#include	<sys/stat.h>
#ifndef SEQUENT
#include	<ndir.h>
#else
#include	<dir.h>
#endif
#include	<string.h>
#include	"mk.h"

#define		MAXPATH		BIGBLOCK

static char result[MAXPATH];
static doit();

/*
 * globals (file name generation)
 *
 * "*" in params matches r.e ".*"
 * "?" in params matches r.e. "."
 * "[...]" in params matches character class
 * "[...a-z...]" in params matches a through z.
 *
 */

static
gglob(arg, dest, fn)
	char *arg, *dest;
	void (*fn)();
{
	int meta;
	char *slash, *s;
	DIR *dirf;

	/*
		check for meta chars
	*/
	for(s = arg, slash = 0, meta = 0; *s && !meta; s++)
		switch(*s)
		{
		case '/':
			slash = s+1;
			break;
		case '\\':
			if(s[1])
				s++;
			break;
		case '[':
		case '*':
		case '?':
			meta = 1;
			break;
		}
	if(meta == 0){
		strcpy(dest, arg);
		(*fn)(result);
		return;
	}
	if(slash){
		memcpy(dest, arg, slash-arg);
		dest += slash-arg;
		*dest = 0;
	} else
		slash = arg;
	s = strchr(slash, '/');
	if(dirf = opendir(result[0] ? result : ".")){
		struct direct *e;
		int found;

		found = 0;
		if(s) *s = 0;
		/* slash is null terminated (s) pattern */
		while(e = readdir(dirf)){
			if(e->d_name[0] == '.' && *slash != '.'){
				if (e->d_name[1] == 0)
					continue;
				if (e->d_name[1] == '.' && e->d_name[2] == 0)
					continue;
			}
			if(gmatch(e->d_name, slash)){
				found = 1;
				strcpy(dest, e->d_name);
				doit(s, dest, fn);
			}
		}
		if(!found){
			strcpy(dest, slash);
			doit(s, dest, fn);
		}
		closedir(dirf);
	} else {
		if(s)
			*s = 0;
		strcpy(dest, slash);
		dest = strchr(dest, 0);
		if(s){
			*dest++ = '/';
			*dest = 0;
			gglob(s+1, dest, fn);
		} else
			(*fn)(result);
	}
	if(s) *s = '/';
}

static
doit(s, dest, fn)
	char *s, *dest;
	void (*fn)();
{
	if(s){
		register char *ss;

		ss = strchr(dest, 0);
		*ss++ = '/';
		*ss = 0;
		gglob(s+1, ss, fn);
	} else
		(*fn)(result);
}

gmatch(s, p)
register char	*s, *p;
{
	register int scc;
	unsigned char c;

	scc = *s++&(0xFF&~EBIT);
	switch (c = *p++)
	{
	case '[':
		{
			int ok;
			int lc;
			int notflag = 0;

			ok = 0;
			lc = 077777;
			if (*p == '^'){
				notflag = 1;
				p++;
			}
			while (c = *p++){
				if (c == ']')
					return(ok ? gmatch(s, p) : 0);
				else if (c == '-'){
					if (notflag){
						if (scc < lc || scc > *(p++))
							ok++;
						else
							return(0);
					} else {
						if (lc <= scc && scc <= (*p++))
							ok++;
					}
				} else {
					lc = c&~EBIT;
					if (notflag){
						if (scc && scc != lc)
							ok++;
						else
							return(0);
					} else {
						if (scc == lc)
							ok++;
					}
				}
			}
			return(0);
		}

	default:
		if ((c&~EBIT) != scc)
			return(0);

	case '?':
		return(scc ? gmatch(s, p) : 0);

	case '*':
		while (*p == '*')
			p++;

		if (*p == 0)
			return(1);
		--s;
		while (*s){
			if (gmatch(s++, p))
				return(1);
		}
		return(0);

	case 0:
		return(scc == 0);
	}
}

glob(s, fn)
	char *s;
	void (*fn)();
{
	result[0] = 0;
	if(strlen(s) >= MAXPATH){
		SYNERR(inline-1);
		Fprint(2, "too much input for glob expansion; max=%d, given %d\n", MAXPATH, strlen(s));
		Exit();
	}
	gglob(s, result, fn);
}

#ifdef	MAIN

void
pr(s)
	char *s;
{
	Fwrite(1, s, strlen(s));
	Fputc(1, '\n');
}

main(argc, argv)
	char **argv;
{
	for(argv++; *argv; argv++){
		Fprint(1, "<%s>:\n", *argv);
		glob(*argv, pr);
		Fprint(1, "*******\n");
	}
	exit(0);
}
#endif

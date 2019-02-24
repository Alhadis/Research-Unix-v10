#include	"mk.h"

bquote(fd, src, dest)
	register char *src, *dest;
{
	int childin[2], childout[2], pid, i;
	int quotedone = 1;
	char *ldest, *t;
	char *newline = "\n";
	int wearedone = 0;
	int initdone = 0;
	extern char **environ;

	while(wearedone == 0)
		switch(*src)
		{
		default:
			*dest++ = *src++;
			break;
		case 0:
			if(quotedone)
				wearedone = 1;
			else {
				if(t = Frdline(fd)){
					inline++;
					*dest++ = '\n'|EBIT;
					strcpy(src, t);
					continue;
				} else {
					SYNERR(-1); Fprint(2, "missing closing `\n");
					break;
				}
			}
			break;
		case '\\':
			if(*src)
				*dest++ = *src++;
			else
				if(t = Frdline(fd)){
					inline++;
					strcpy(src, t);
				}
			break;
		case '`':
			if(quotedone = 1-quotedone){
				if(initdone == 0){
					execinit();
					initdone = 1;
				}
				Fflush(1);
				Fflush(2);
				if(pipe(childin) < 0){
					SYNERR(-1); perror("pipe1");
					Exit();
				}
				if(pipe(childout) < 0){
					SYNERR(-1); perror("pipe2");
					Exit();
				}
				if((pid = fork()) < 0){
					SYNERR(-1); perror("fork");
					Exit();
				}
				if(pid){	/* parent */
					close(childin[0]);
					close(childout[1]);
					if(ldest < dest){
						writeout(childin[1], ldest, dest);
						writeout(childin[1], newline, newline+1);
					}
					close(childin[1]);
					dest = ldest;
					while((i = read(childout[0], dest, 512)) > 0)
						dest += i;
					if((dest > ldest) && (dest[-1] == '\n'))
						dest--;
					close(childout[0]);
				} else {
					dup2(childin[0], 0);
					dup2(childout[1], 1);
					close(childin[0]);
					close(childin[1]);
					close(childout[0]);
					close(childout[1]);
					if(execle(SHELL, "sh", (char *)0, environ) < 0)
						perror("exec");
					_exit(1);
				}
			} else
				ldest = dest;
			src++;
			break;
		}
	*dest = 0;
}

writeout(fd, b, e)
	register char *b, *e;
{
	char buf[BIGBLOCK];
	register char *s;

	for(s = buf; b < e;){
		if(s >= &buf[BIGBLOCK-1]){
			SYNERR(-1); Fprint(2, "too much text (%d chars) for `expr`\n", s-buf);
			Exit();
		}
		if(*b&EBIT){
			*s++ = '\\';
			*s++ = (~EBIT) & *b++;
		} else
			*s++ = *b++;
	}
	write(fd, buf, s-buf);
}

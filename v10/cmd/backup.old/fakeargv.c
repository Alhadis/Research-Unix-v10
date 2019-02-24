char **
fakeargv(src, ac)
	char *src;
	int *ac;
{
	static char *args[256], argsbuf[4096];
	char *argc = argsbuf;
	char **arg = args;

	*arg++ = argc;
	*argc++ = 0;
	while(*src){
		*arg++ = argc;
		while(*argc++ = *src++)
			if(argc == argsbuf+sizeof argsbuf){
				argc[-1] = 0;
				break;
			}
	}
	*arg = 0;
	*ac = arg-args;
	return(args);
}

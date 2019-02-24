char *
mapdev(s)
	char *s;
{
	static char buf[128];

	if((s[1] == 0) && (*s >= '0') && (*s <= '9')){
		sprint(buf, "/dev/worm%c", *s);
		s = buf;
	}
	return(s);
}

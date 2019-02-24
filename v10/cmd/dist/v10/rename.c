int
rename(char *old, char *new)
{
	if (link(old, new) == 0) {
		unlink(old);
		return 0;
	} else
		return -1;
}

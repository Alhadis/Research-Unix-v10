void *
memmove(void *dst, void *src, int n)
{
	char *dp, *sp;

	dp = dst;
	sp = src;

	if (dp < sp)
		while (n--)
			*dp++ = *sp++;
	else {
		dp += n;
		sp += n;
		while (n--)
			*--dp = *--sp;
	}
	return dp;
}

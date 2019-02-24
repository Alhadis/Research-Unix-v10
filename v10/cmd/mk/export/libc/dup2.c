#ifdef	SYSV
/*
	dup2(a, b)

	dup a as value b
*/

dup2(a, b)
{
	int poot[256];		/* greater than NFILES i hope! */
	register i, ret;

	close(b);
	for(i = 0; (poot[i] = dup(a)) >= 0; i++)
		if(poot[i] == b) break;
	ret = i;
	while(--i >= 0)
		close(poot[i]);		/* don't need 'em anymore */
	return(poot[ret]);
}
#endif

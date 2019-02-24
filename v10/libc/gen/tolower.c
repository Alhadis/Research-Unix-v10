int
tolower(c)
register int c;
{
	if(c >= 'A' && c <= 'Z')
		c |= 040;
	return(c);
}

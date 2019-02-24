int
toupper(c)
register int c;
{
	if(c >= 'a' && c <= 'z')
		c &= ~040;
	return(c);
}

/* This UNIX libm function is included to maintain compatibility with
 * Berkeley UNIX
 */
static	long	randx = 1;

Srand(x)
{
	randx = x;
}

Rand()
{
	return(((randx = randx * 1103515245L + 12345)>>16) & 0x7fff);
}

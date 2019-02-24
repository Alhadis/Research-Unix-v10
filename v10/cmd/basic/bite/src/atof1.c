/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	**** ASCII TO FLOAT CONVERSION ****
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Fri Aug 24 14:29:13 1979 *****
 *	***********************************
 *	***********************************

*/
/*   "@(#) atof1.c:  V 1.1  12/21/80" */

/*
this is a more generalized version of atof. It is also written in "c"
so that it can be compiled for the LSX system.
 * CONVERT STRING TO FLOAT
 */
double atof1(s)
char s[];
{
	register char *s1, *s2;	/* string pointer regs */
	register	i;	/* index register */
	double val,power;
	int sign,esin,exp;

	s1 = s;
	s2 = s;
	while(*s2 != '\0')
		switch (*s2) {
			case ' ' :
			case '\015' :
			case '\012' :
			case '\t':
			s2++;
			break;
			case 'E':
			*s1++ = 'e';
			s2++;
			break;
			default:
			*s1++ = *s2++;
		}
	*s1 = '\0';
	i=0;
	sign = 1;
	if(s[i] == '+' || s[i] == '-')
		sign = (s[i++] == '+') ? 1 : -1 ;
	for(val = 0;s[i] >= '0' && s[i] <= '9';i++)
		val = 10 * val + s[i] - '0';
	if(s[i] == '.')
	{
		i++;
		for(power = 1;s[i] >= '0' && s[i] <= '9'; i++)
		{
			val = val * 10 + s[i] - '0';
			power *= 10;
		}
		val /= power;
	}
	if(s[i] == 'e')
	{
		i++;
		esin = 1;
		if(s[i] == '+' || s[i] == '-')
			esin = (s[i++] == '+') ? 1 : -1;
		for(exp = 0;s[i] >= '0' && s[i] <= '9';i++)
			exp = 10 * exp + s[i] - '0';
		power = 1;
		for(i=0;i<exp;i++)
			power *= 10;
		val = (esin == -1) ? val/power : val * power ;
	}
	return(sign * val);
}

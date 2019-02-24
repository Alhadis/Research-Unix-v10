/*
 * Original ("atof1") from J.P. Hawkins at WH: "atof1.c"
 * "atof" in libc.a is in assembly language and uses floating point;
 * (No good when using RH floating point preprocessor)
 * mjm: put this in "flibu" (-lfu) "mmatof.o
 * 	change name: atof1 -> atof
 */

double atof(s) /*convert string s to floating point*//* mjm: was atof1 */
char s[];
{
	char *s1;
	char *s2;
	double val,power;
	int i,sign,esin,exp;

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

/* ftoa from portable c library, modified to use faster multiply 
   up and down. */
/* mjm: removed printf and _dummy call at end;
 *	now in separate file, rhprintf.o
 */

ftoa (x, str, prec, format)
float x;
char *str;
{
/* converts a floating point number to an ascii string */
/* x is stored into str, which should be at least 30 chars long */
int ie, i, k, ndig, fstyle;
double y;
ndig =  ( prec<0) ? 7 : (prec > 22 ? 23 : prec+1);
if  (format == 'f' || format == 'F')
 fstyle = 1;
else
 fstyle = 0;
/* print in e format unless last arg is 'f' */
ie = 0;
/* if x negative, write minus and reverse */
if ( x < 0.0)
  {
  *str++ = '-';
  x = -x;
  }

/* put x in range 1 <= x < 10 */
if(x > 0.0) {
	if(x<1e-32){x=x* 1e33; ie = ie - 33;}
	if(x<1e-16){x=x* 1e17; ie = ie - 17;}
	if(x<1e-8){x=x* 1e9; ie = ie - 9;}
	if(x<1e-4){x=x* 1e5; ie = ie - 5;}
	if(x<1e-2){x=x* 1e3; ie = ie - 3;}
	if(x<1e-1){x=x* 1e2; ie = ie - 2;}
        while(x<1.0){ x=x* 10.; ie = ie - 1;}
	}
if(x>=1e32){x=x/1e32; ie = ie + 32;}
if(x>=1e16){x=x/1e16; ie = ie + 16;}
if(x>=1e8){x=x/1e8; ie = ie + 8;}
if(x>=1e4){x=x/1e4; ie = ie + 4;}
if(x>=1e2){x=x/1e2; ie = ie + 2;}
while(x>=10.){x=x/10.;ie++;}

/* in f format, number of digits is related to size */
if (fstyle) ndig = ndig + ie;

/* round. x is between 1 and 10 and ndig will be printed to
   right of decimal point so rounding is ... */
y = 10.0;
for (i = 0; i < ndig; i++)
  y=y/10.0;
x=x+.5*y;
if (x >= 10.0) /* repair rounding disasters */
	{
	x = 1.0;
	ie++;
	if (fstyle) ndig++;
	}
/* now loop.  put out a digit (obtain by multiplying by
  10, truncating, subtracting) until enough digits out */
/* if fstyle, and leading zeros, they go out special */
if (fstyle && ie < 0)
	{
	*str++ = '0';
	i = (ndig > 0) ? -1 : (ndig-1);
	i = i - ie;
	if (i > 0)
		{
		*str++ = '.';
		while (i--)
			*str++ = '0';
		}
	}
for (i=0; i < ndig; i++)
  {
  if (i == (fstyle ? ie+1 : 1)) /* where is decimal point */
    *str++ = '.';
  k = x;
  *str++ = k + '0';
  x = x - (y=k);
  x=x*10.;
  }

/* now, in estyle,  put out exponent if not zero */
if (!fstyle && ie != 0)
  {
  *str++ = 'E';
  if (ie < 0)
    {
    ie = -ie;
    *str++ = '-';
    }
  for (k=100; k > ie; k = k/10);
  for (; k > 0; k = k/10)
       {
       *str++ = ie/k + '0';
       ie = ie%k;
       }
  }
*str = '\0';
return;
}

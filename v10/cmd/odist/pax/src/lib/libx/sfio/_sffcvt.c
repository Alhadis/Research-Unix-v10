#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
char *sffcvt(reg double dval, int n_digit, int* decpt, int* sign)
#else
char *sffcvt(dval,n_digit,decpt,sign)
reg double	dval;		/* value to convert */
int		n_digit;	/* number of digits wanted */
int		*decpt;		/* to return decimal point */
int		*sign;		/* to return sign */
#endif
{
	return _SFFCVT(dval,n_digit,decpt,sign);
}

#include	"sfhdr.h"
#include	"stdio.h"

/*	Stdio gets()
**
**	Written by Kiem-Phong Vo (12/10/90)
*/

#if __STDC__ || __cplusplus
char *_stdgets(reg char *s, reg int n)
#else
char *_stdgets(s,n)
reg char	*s;
reg int		n;
#endif
{	if(!s || sfgets(sfstdin,s,n) != s)
		return NIL(char*);
	s[_Sfi--] = '\0';
	return s;
}

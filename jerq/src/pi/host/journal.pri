#ifndef HISTORY_H
#define HISTORY_H
#ifndef UNIV_H
#include "univ.h"
#endif

class Journal : PadRcv {
	int	length;
	long	key;
	long	lastreq;
	Bls	*bls;
	int	*ct;	
	char	*proc;
	Pad	*pad;
	void	banner();
	void	linereq(long,Attrib=0);
PUBLIC(Journal,U_JOURNAL)
	void	open();
	void	hostclose();
		Journal(char*);
	void	insert(char* ...);
};
#endif

#ifndef SRCTEXT
#define SRCTEXT
#ifndef UNIV_H
#include "univ.h"
#endif
#include <CC/stdio.h>

class SrcText : public PadRcv { friend Stmt; friend SrcDir;
class	Pad	*pad;
class	Source	*source;
	int	lastline;
	char	**edge;
	char	*body;
	int	current;
	char	*path;
	int	fd;
	long	compiletime;
	char	*prevpat;
	short	warned;
	short	promoted;

	void	promote();
	void	reopen();
class	Core	*core();
	char	*read(class Menu&);
class	Process	*process();
	void	go();
	void	currentstmt();
	void	stmtstep(long);
	void	stepinto();
	void	free();
	void	banner();
PUBLIC(SrcText,U_SRCTEXT)
		SrcText(Source*, long );
	void	open();
	void	linereq(long,Attrib=0);
	void	select(long,long=0);
	void	userclose();
	char	*kbd(char*);
	char	*help();
	char	*contextsearch(int,char*,int);
	char	*srcline(long);
};
#endif

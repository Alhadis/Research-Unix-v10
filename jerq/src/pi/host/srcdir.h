#ifndef SRCDIR_H
#define SRCDIR_H
#ifndef UNIV_H
#include "univ.h"
#endif

class SrcDir : public PadRcv {
	Pad	*pad;
	Process *process;
PUBLIC(SrcDir,U_SRCDIR)
		SrcDir(class Process *p);
	void	open();
	void	hostclose();
	void	banner();
	char	*kbd(char*);
	char	*help();
};

#endif

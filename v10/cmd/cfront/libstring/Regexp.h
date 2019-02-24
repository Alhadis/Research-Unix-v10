#ifndef CAP_REGEXP_H
#define CAP_REGEXP_H 1

#include <regexp.h>
#include "String.h"

class Regexp {
	String	subject;
	int	offset[NSUBEXP];
	int	len[NSUBEXP];
	regexp*	prog;
public:
		Regexp(const char*);
		Regexp(const String&);
		~Regexp();
	int	exec(const String&);
	int	result(unsigned& start, unsigned& length, unsigned = 0);
	String	subst(const String&);
};

#endif

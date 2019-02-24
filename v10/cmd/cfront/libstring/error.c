#include "String.h"


void
String::error(int i, char *p) _const
{
	if ( handler )
		(*handler)(i, p);
	else {
		cerr << (p?p:"error in String function ") << i;
		abort();
	}
}

GPT
String::sethandler(GPT h)
{
	GPT	rslt = handler;
	handler = h;
	return rslt;
}

void
SubString::error(int i, char *p)
{
	if ( handler )
		(*handler)(i, p);
	else {
		cerr << (p?p:"error in SubString function ") << i;
		abort();
	}
}

GPT
SubString::sethandler(GPT h)
{
	GPT	rslt = handler;
	handler = h;
	return rslt;
}

void
Subchar::error(int i, char *p)
{
	if ( handler )
		(*handler)(i, p);
	else {
		cerr << (p?p:"error in Subchar function ") << i;
		abort();
	}
}

GPT
Subchar::sethandler(GPT h)
{
	GPT	rslt = handler;
	handler = h;
	return rslt;
}

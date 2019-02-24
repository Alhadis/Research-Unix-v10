#include "decl.h"

listimplement(String)
listimplement(mel)
listimplement(Path)

// temporary mkdir and rmdir routines

int
mkdir (const String& s, int mode)
{
	charstr cp = s;
	return mkdir(cp, mode);
}

int
rmdir (const String& s)
{
	charstr cp = s;
	return rmdir(cp);
}

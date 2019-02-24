#include "master.pri"
#include "m68kcore.h"

WerkCore.WerkCore(Process *p, WerkMaster *m):(p,m) {}

long WerkCore.savearea()
{
	return 0;
}

char *WerkCore.seekto(int &fd, long &offset)
{
//	offset -= hdr.a_textorg;
//	offset += sizeof(exec);
	return 0;
}

#ifndef MASTER_H
#define MASTER_H
#ifndef UNIV_H
#include "univ.h"
#endif

void NewWd();

class Master : public PadRcv {	friend HostMaster; friend TermMaster; friend KernMaster; friend SnetMaster; friend WerkMaster;
	Process	*child;
virtual	char	*kbd(char*s);
virtual	char	*help();
virtual	Process	*domakeproc(char*, char*, char*)	{ return 0; }
PUBLIC(Master, U_MASTER)
	Core	*core;
	Pad	*pad;
	void	insert(Process*);
	Process	*search(char*);
	Process	*makeproc(char*, char* =0, char* =0);
		Master();
};

class KernMaster : public Master {
	Process	*domakeproc(char*, char*, char*);
	char	*kbd(char*);
	char	*help();
	void	refresh();
	void	findcores(char*);
public:
		KernMaster(SymTab*);
};

class HostMaster : public Master {
	KernMaster
		*kernmaster;
	Process	*domakeproc(char*, char*, char*);
	char	*dopscmd(char*);
	void	kpi();
	char	*kbd(char*);
	char	*help();
	void	refresh(char*);
public:
		HostMaster();
};

class BatchMaster : public Master {
public:
		BatchMaster(char*, char*);
};

class TermMaster : public Master {	friend	TermCore;
	Remote	*remote;
	HostMaster
		*hostmaster;
	void	layer();
	void	jps();
	void	pi();
	Process	*domakeproc(char*, char*, char*);
	TermProcess
		*makeP(long,char* =0);
	char	*kbd(char*);
	char	*help();
public:
		TermMaster(Remote*);
};

class SnetMaster : public Master {
	Process	*domakeproc(char*, char*, char*);
	char	*kbd(char*);
	char	*help();
public:
		SnetMaster();
};

class WerkMaster : public Master {
	Process	*domakeproc(char*, char*, char*);
	char	*kbd(char*);
	char	*help();
public:
		WerkMaster();
};
#endif

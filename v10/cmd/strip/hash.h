#include "malloc.h"

#define HASHSIZE	1013
#define MAXHUSE		((3*HASHSIZE)/4)
#define STRTABSIZE	4096

struct ht {
	struct ht *next;
	int nused;
	char *ptable[HASHSIZE];
};

struct st {
	struct st *next;
	int nused;
	char strtab[STRTABSIZE];
};

char *hash();

struct ht *hroot; struct st *sroot;

int stcount;

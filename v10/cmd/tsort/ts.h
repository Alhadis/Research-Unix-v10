#ifndef TS_Include
#define TS_Include
#define NBUFLEN 132

struct obj {
	struct obj *next;
	char *name;
	struct group *ogroup;
	unsigned iptr, jct;
	int seq;
	};

struct obj1 {
	struct obj1 *next;
	char *name;
	struct group *ogroup;
	unsigned iptr;
	struct ref *nextref;
	};

struct ref {
	struct ref *next;
	char *name;
	struct obj *robj;
	};

struct refc {
	unsigned rcnext;
	struct ref *rcref;
	};

struct group {
	struct group *next;
	char *name;
	unsigned gseq;
	};
#endif

#define CSEGSIZE	512-2*sizeof(int)

typedef struct Code {
	struct Code *prev;
	char *firstFree;
	char segment[CSEGSIZE];
	} Code;

extern Code *CodeStoreChar();
extern Code *CodeGetBlock();
extern Code *CodeAppend();
extern Code *CodeStoreString(), *CodeMarkLine();

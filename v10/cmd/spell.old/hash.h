/*      @(#)hash.h      1.1     */
#define PRIME1 8191
#define PRIME2 16381
#define WIDTH1 13	/*bitsof(PRIME1)*/
#define WIDTH2 14	/*bitsof(PRIME2)*/
#define HASHWIDTH (WIDTH1+WIDTH2)
#ifdef pdp11
#define INDEXWIDTH 9
#else
#define INDEXWIDTH 10
#endif
#define INDEXSIZE (1<<INDEXWIDTH)
#define NI (INDEXSIZE+1)
#define BYTE 8
#define CODEWIDTH 8
#define EXTRA (1<<CODEWIDTH)

extern unsigned *table;
extern unsigned short index[];  /*into dif table based on hi hash bits*/

extern long hash();

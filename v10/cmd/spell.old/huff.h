/*      @(#)huff.h      1.1     */
#define BYTE 8
#ifdef pdp11
#define QW 1	/* width of bas-q digit in bits */
#else
#define QW 4	/* time-space trade in larger memory */	
#endif
extern struct huff {
        long xn;
        int xw;
        long xc;
        long xcq;       /* (c,0) */
        long xcs;       /* c left justified */
        long xqcs;      /* (q-1,c,q) left justified */
        long xv0;
} huffcode;
#define n huffcode.xn
#define w huffcode.xw
#define c huffcode.xc
#define cq huffcode.xcq
#define cs huffcode.xcs
#define qcs huffcode.xqcs
#define v0 huffcode.xv0

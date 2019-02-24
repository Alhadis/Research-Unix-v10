typedef struct _PCBL{
	struct _PCBL *Pnextp;
	struct _PCBL *Pprevp;
	char *Pbase;		/* Pointer to head of character buffer */
	char *Pptr;		/* Pointer to current read position */
	char *Phiwat;		/* Pointer to high water mark for this buffer */
	int Pcnt;		/* Phiwat-Pptr.  for speed. */
}_PCBL;		/* Pad circular buffer list */

typedef struct _PAD{
	_PCBL	*Pcbl;		/* Circular list for this file descriptor */
	_PCBL	*Pcbb;		/* base of list so we can free it */
	int	Pfile;		/* file descriptor */
}PAD;

#define	Pgetc(p)	(--(p)->Pcbl->Pcnt>=0? *(p)->Pcbl->Pptr++&0377:_Pfilbuf(p))
PAD *Pfopen();
PAD *Pfdopen();

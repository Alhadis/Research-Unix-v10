/* voidparm.h - for pre-ANSI systems where word ptrs differ from char ptrs */
#define fread(d, n, m, p) (fread)((char*)d, n, m, p)
#define fwrite(d, n, m, p) (fwrite)((char*)d, n, m, p)
#define free(p) (free)((char*)p)
#define realloc(p, n)((char*)p, n)
#define bsearch(k, b, m, n, f) (bsearch)((char*)k, (char*)b, m, n, f)
#define qsort(a, m, n, f) (qsort)((char*)a, m, n, f)
#define memmove(d, s, n) (memmove)((char*)d, (char*)s, n)
#define memcpy(d, s, n) (memcpy)((char*)d, (char*)s, n)
#define memcmp(d, s, n) (memcmp)((char*)d, (char*)s, n)
#define memchr(p, c, n) (memchr)((char*)p, c, n)
#define memset(p, c, n) (memset)((char*)p, c, n)

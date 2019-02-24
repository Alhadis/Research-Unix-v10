#ifndef _string_
#define _string_

/* libc string routines */
extern char *strcat();
extern char *strncat();
extern char *strcpy();
extern char *strncpy();
extern int strlen();
extern int strcmp();
extern int strncmp();
extern char *strchr();
extern char *strrchr();

/* extensible strings */
typedef struct string {
	char *base;	/* base of string */
	char *end;	/* end of allocated space+1 */
	char *ptr;	/* ptr into string */
} string;

#define s_putc(s,c)\
	if (s->ptr<s->end) *(s->ptr)++ = c; else s_grow(s,c)
#define s_terminate(s)\
	if (s->ptr<s->end) *s->ptr = 0; else{ s_grow(s,0); s->ptr--; }
#define s_restart(s) (s->ptr = s->base , s)
#define s_reset(s) (s ? (*(s->ptr = s->base) = '\0' , s) : s_new())
#define s_clone(s) s_copy(s->ptr)
#define s_to_c(s) s->base

extern string *s_new();
extern void s_free();
extern string *s_append();
extern string *s_nappend();
extern string *s_array();
extern string *s_copy();
extern string *s_seq_read();
extern char *s_read_line();
extern int s_read_to_eof();
extern string *s_parse();
/* controlling the action of s_seq_read */
#define TOKEN 0		/* read the next whitespace delimieted token */
#define LINE 1		/* read the next logical input line */
#define s_getline(a,b) s_seq_read(a,b,LINE)
#define s_gettoken(a,b) s_seq_read(a,b,TOKEN)


#endif

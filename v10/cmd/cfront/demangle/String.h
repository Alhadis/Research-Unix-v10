/*ident	"@(#)ctrans:demangler/String.h	1.1"*/
/*
 * C++ Demangler Source Code
 * @(#)master	1.5
 * 7/27/88 13:54:37
 */
#define STRING_START 32
#define PTR(S) ((S)->data + (S)->sg.start)

typedef struct {
	int start,end,max;
} StringGuts;

typedef struct {
	StringGuts sg;
	char data[1];
} String;

extern String *prep_String();
extern String *nprep_String();
extern String *app_String();
extern String *napp_String();
extern String *mk_String();
extern void free_String();
extern String *set_String();
extern char *findop();
extern int oplen;

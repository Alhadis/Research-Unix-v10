#include <jerq.h>
#include <font.h>
#define PADS_TERM
typedef enum PI_Proto PI_Proto;

long assertf();
#define assert(e) ( assertf( (long) (e) ) )
extern Bitmap physical;

typedef int  (*PFI)();
typedef long (*PFL)();
typedef char (*PFC)();

extern Point Zpoint;
extern Rectangle ZRectangle;

char *strcpy();
char *strcat();
char *dec();
char *hex();

extern Texture Coffee;
extern Texture PiIcon;
extern Texture *Cursor;

#define butts (mouse.buttons&07)
#define BUTT1 4
#define BUTT2 2
#define BUTT3 1

#ifdef JOURNAL
#define journal Journal
#else
#define journal if(0)
#endif JOURNAL

typedef struct Proc Proc;
extern Proc *UserInterface;

char *AlignErr();
char *ReadErr();
char *WriteErr();
char *SendError();

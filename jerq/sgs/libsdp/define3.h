/*
 * static char ID[] = "@(#) define3.h: 1.2 3/16/82";
 */
/* mnemonic to indicate that a file is closed; cannot equal a file descriptor */
#define	CLOSED		(-1)

/* mnemonic to indicate a null manager link; cannot equal an index */
#define	END		(-1)

/* mnemonic to indicate an impossible hash value */
#define HASHNULL	0

/* mnemonic to indicate a failed search for an id */
#define IDNOTFOUND	(-3l)

/* mnemonic to indicate a failed search; distinct from ERROR or SUCCESS */
#define	NOTFOUND	(-2)

/* mnemonic to indicate a nonexistent file; distinct from CLOSED or descriptor */
#define NOTMADE		(-2)

/* mnemonic to indicate a null ref/chng flag; distinct from RNLY or WRTN */
#define RCNULL		(-1)


/* macros to access library functions through the LIB structures */

#define MALLOC(size)				(*Glib_mem->liballoc)(size)
#define MBUILDNAME(name,i)			(*Glib_file->libbuildname)(name,i)
#define MCLOSE(designator)			(*Glib_file->libclose)(designator)
#define MCREATE(name,mode)				(*Glib_file->libcreate)(name,mode)
#define MFREE(ptr)				(*Glib_mem->libfree)(ptr)
#define MOPEN(name,perms)			(*Glib_file->libopen)(name,perms)
#define MREAD(designator,buffer,nbytes)		(*Glib_file->libread)(designator,buffer,nbytes)
#define MSEEK(designator,position)		(*Glib_file->libseek)(designator,position)
#define MUNLINK(name)				(*Glib_file->libunlink)(name)
#define MWRITE(designator,buffer,nbytes)	(*Glib_file->libwrite)(designator,buffer,nbytes)

/* macro to check id for valid range */
#define INVALID(sp,id)	( !( ((id)>=0) && ((id)<(sp)->maxpage) ) )

/* macro to check id for valid alignment */
#define ALIGNID(id)	( ((id)+sizeof(BOUNDARY)-1)/sizeof(BOUNDARY)*sizeof(BOUNDARY) )

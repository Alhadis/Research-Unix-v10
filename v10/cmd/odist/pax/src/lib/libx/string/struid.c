/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * uid name -> number
 */

#include <hash.h>
#define getpwnam	_SYS_getpwnam
#include <pwd.h>
#undef	getpwnam
extern struct passwd*	getpwnam();

typedef struct
{
	HASH_HEADER;
	int	id;
} bucket;

/*
 * return uid number given uid name
 * -1 on first error for a given name
 * -2 on subsequent errors for a given name
 */

int
struid(name)
char*	name;
{
	register struct passwd*	pw;
	register bucket*	b;

	static HASHTABLE*	uidtab;

	if (!uidtab && !(uidtab = hashalloc((HASHTABLE*)0, HASH_set, HASH_ALLOCATE, HASH_name, "uidnam", 0))) return(-1);
	if (b = (bucket*)hashlook(uidtab, name, HASH_LOOKUP|HASH_FIXED, (char*)sizeof(bucket))) return(b->id);
	if (!(b = (bucket*)hashlook(uidtab, (char*)0, HASH_CREATE|HASH_FIXED, (char*)sizeof(bucket)))) return(-1);
	if (pw = getpwnam(name)) return(b->id = pw->pw_uid);
	b->id = -2;
	return(-1);
}

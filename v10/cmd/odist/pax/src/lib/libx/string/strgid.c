/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * gid name -> number
 */

#include <hash.h>

#define getpwnam	_SYS_getpwnam
#define getgrnam	_SYS_getgrnam
#include <pwd.h>
#include <grp.h>
#undef	getpwnam
#undef	getgrnam
extern struct passwd*	getpwnam();
extern struct group*	getgrnam();

typedef struct
{
	HASH_HEADER;
	int	id;
} bucket;

/*
 * return gid number given gid/uid name
 * gid attempted first, then uid->pw_gid
 * -1 on first error for a given name
 * -2 on subsequent errors for a given name
 */

int
strgid(name)
char*	name;
{
	register struct group*	gr;
	register struct passwd*	pw;
	register bucket*	b;

	static HASHTABLE*	gidtab;

	if (!gidtab && !(gidtab = hashalloc((HASHTABLE*)0, HASH_set, HASH_ALLOCATE, HASH_name, "gidnam", 0))) return(-1);
	if (b = (bucket*)hashlook(gidtab, name, HASH_LOOKUP|HASH_FIXED, (char*)sizeof(bucket))) return(b->id);
	if (!(b = (bucket*)hashlook(gidtab, (char*)0, HASH_CREATE|HASH_FIXED, (char*)sizeof(bucket)))) return(-1);
	if (gr = getgrnam(name)) return(b->id = gr->gr_gid);
	if (pw = getpwnam(name)) return(b->id = pw->pw_gid);
	b->id = -2;
	return(-1);
}

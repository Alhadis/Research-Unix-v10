#ifndef STRING_DOT_H
#define STRING_DOT_H

#define _OVERLOAD_STAT 1
#include <sys/types.h>
#include <sys/stat.h>
#undef _OVERLOAD_STAT

#ifdef BSD
extern char*	bcopy(const char* from, char* to, int);  /* vomit */
extern int	bcmp(const char* from, const char* to, int);  /* only for == */
#define memcpy(t,f,n) bcopy(f,t,n)
#else
extern char*	memcpy(char* to, const char* from, int);
extern int	memcmp(const char* to, const char* from, int);
#endif

overload system, access, acct, chdir, chmod, chown, creat, link, mknod, mount,
	open, umount, unlink, read, write;
#include <sysent.h>

overload strchr, strrchr, strpbrk, strspn, strcspn;
overload fopen, fdopen, freopen, popen, gets, fgets, puts, fputs;

#include <string.h>
#include <stream.h>
#ifndef GENERICH
#include <generic.h>
#endif

#ifndef TRUE
#define FALSE	0
#define TRUE	(!FALSE)
#endif

#ifndef BIT_DEFINED
#define BIT_DEFINED
typedef int	bit;
#endif

overload min;
inline int min(int i, int j) { return i<j ? i : j; }
inline int min(int i, int j, int k) { return i<j ? min(i,k) : min(j,k); }

#define MAXSUBSTRINGLENGTH	0x7FFF
const MAXSTRINGLENGTH = 0xFFFF-1;

class Rep;
class String;
class SubString;
class Subchar;	/* the result of [] */
class Regexp;

struct Charfield;

struct MtRep
{
	MtRep	*next;
	int	dummy;
};

const empty_dummy = ~0 << 16 | 0374;

struct Rep
{
	char	*start;	// the characters (NOT null terminated)
	unsigned short	len;
	unsigned char	refCount;
	unsigned char	flags;
	bit	is_immutable() { return (flags & 02) != 0; }
	void	immutable() { flags |= 02; }
	void	mutable() { flags &= ~02; }
	bit	is_constant() { return (flags & 01) != 0; }
	void	now_constant() { flags |= 01; }
	void	not_constant() { flags &= ~01; }
		Rep() {}
		Rep(const Rep&);
		Rep(unsigned);
		Rep(const char *, unsigned);
		Rep(const char *s) { this = new Rep(s, strlen(s)); }
		~Rep();
	void	refDecr() { if (--refCount == 0 && !is_constant())
				delete this;
		}
	void	refIncr() { if (++refCount == 0) now_constant(); }
	unsigned	length () { return len; }
#ifdef BSD
	int	operator==(const Rep& r) { return len == r.len ?
			!bcmp(start, r.start, len) : 0; }
	int	compare(const Rep& r) {  // bug in case of embedded zero
			int i = strncmp(start, r.start, min(len, r.len));
			return i ? i : ((int)len - (int)r.len); }
#else
	int	operator==(const Rep& r) { return len == r.len ?
			!memcmp(start, r.start, len) : 0; }
	int	compare(const Rep& r) {
			int i = memcmp(start, r.start, min(len, r.len));
			return i ? i : ((int)len - (int)r.len); }
#endif
	int	compare (const char*);
	int	match(const Rep&);	// return first differing position
	int	match(const char*);
	int	hashval ();	// for use in hash tables, etc.
	bit	extend(int);	// TRUE if the Rep is extended
	bit	isMt() { return ((MtRep *)this)->dummy == empty_dummy; }
	Charfield	*myField();
	Rep	*canCat(int);	// returns a new Rep if this can be catenated
	Rep	*newSub(int offset, int length);
	/* the following are from String(3) */
	int	strchr(char);	// position of first occurrence of char
	int	strrchr(char);	// ... last ... (-1 if char not there)
	int	strpbrk(const Rep&);
	int	strspn(const Rep&);
	int	strcspn(const Rep&);
};

// Strings are at the bottom working up, and Reps are at the top working down
struct Charfield
{
	Charfield	*next;
	MtRep	*emptyHead;	/* of list of free Reps with no space */
	Rep	*lastRep;
	char	*field;		/* beginning of characters */
	char	*end;		/* ... of data area */
	char	*firstFree;
	int	usedSpace;
		Charfield();
		Charfield(unsigned int);
	int	compactify(unsigned int);	/* return TRUE if no use */
	Rep	*newRep(unsigned int);
	char	*getSpace(int);	/* new String space for old Rep */
	void	putMt(MtRep *);
	Rep	*getMt();
};

extern Rep	*nullRep;	/* the null String */
extern Rep	*oneChar;	/* all one-character Strings */
extern Charfield	*currfield;

extern char	*Memcpy(char *to, const char *from, int);

inline ostream&
operator<<(ostream& oo, Rep& r)
{
	return oo.put(r.start, r.len);
}

class String
{
	static GPT	handler;
	static bit	startedUp;
	void	error(int = 0, char * = 0);
	Rep	*d;
	friend SubString;
	friend Subchar;
public:
		String();
		String(char);
		String(char, char);
		String(char, char, char);
		String(char, char, char, char);
		String(const char *);
		String(const char *, unsigned);
		String(const String& s) { d = s.d; d->refIncr(); }
		String(const SubString&);
		String(Rep& r) { d = &r; r.refIncr(); }
		~String() { d->refDecr(); }
	friend int stat(const String&, struct stat*);
	friend int system(const String&);
	friend int access(const String&, int);
	friend int acct(const String&);
	friend int chdir(const String&);
	friend int chmod(const String&, int);
	friend int chown(const String&, int, int);
	friend int creat(const String&, int);
	friend int link(const String&, const String&);
	friend int mknod(const String&, int, int);
	friend int mount(const String&, const String&, int);
	friend int open(const String&, int);
	friend int umount(const String&);
	friend int unlink(const String&);
	friend int read(int, String&, int);
	friend int write(int, const String&);
	friend FILE* fopen(const String&, const String&);
	friend FILE* fdopen(int, const String&);
	friend FILE* freopen(const String&, const String&, FILE*);
	friend FILE* popen(const String&, const String&);
	friend puts(const String&);
	friend fputs(const String&, FILE*);
	friend inline ostream& operator<<(ostream&, const String&);
	friend istream& operator>>(istream&, String&);
	friend String operator+(const char, const String&);
	friend String operator+(const char*, const String&);
	friend Rep;
	friend Regexp;
	friend void startUp();
	unsigned	length (){ return d->length(); }
		operator void*() { return length() ? this : 0; }
	int	hashval() { return d->hashval(); }
	int	compare(const String& oo) { return d == oo.d ? 0 :
				d->compare(*oo.d); }
	int	compare(const char *p) { return d->compare(p); }
	bit	operator==(const String& oo) { return d == oo.d ? 1 :
						(*d == *oo.d); }
	bit	operator>(const String& oo) { return compare(oo) > 0; }
	bit	operator>=(const String& oo) { return compare(oo) >= 0; }
	bit	operator<=(const String& oo) { return compare(oo) <= 0; }
	bit	operator<(const String& oo) { return compare(oo) < 0; }
	bit	operator!=(const String& oo) { return d == oo.d ? 0 :
						!(*d == *oo.d); }
	bit	operator==(const char* p) { return compare(p) == 0; }
	bit	operator>(const char* p) { return compare(p) > 0; }
	bit	operator>=(const char* p) { return compare(p) >= 0; }
	bit	operator<=(const char* p) { return compare(p) <= 0; }
	bit	operator<(const char* p) { return compare(p) < 0; }
	bit	operator!=(const char* p) { return compare(p) != 0; }
	// match returns the first differing position
	int	match(const String& s) { return d == s.d ? length() :
			d->match(*s.d); }
	int	match (const char* p) { return d->match(p); }
	String	operator+(const String&);	/* catenate */
	String	operator+(const char*);	/* catenate */
	String	operator+(const char);	/* catenate */
	String&	operator=(const char);
	String&	operator=(const char *);
	String&	operator=(const String& oo);
	String&	put(const char);	/* append or put */
	String&	put(const char*);	/* append or put */
	String&	put(const String&);	/* append or put */
	String&	operator+=(const char c) { return put(c); }
	String&	operator+=(const String& oo) { return put(oo); }
	String&	operator+=(const char *s) { return put(s); }
	bit	getX(char&);	/* get or lop */
	bit	get() { char c; return getX(c); }
	String&	unget(const char);		/* prepend */
	String&	unget(const String&);		/* prepend */
	bit	unputX(char&);		/* remove from back */
	bit	unput() { char c; return unputX(c); }
	bit	firstX(char&);
	bit	lastX(char&);
	SubString&	operator() (const unsigned start, const unsigned length);
	Subchar&	operator[] (const unsigned);	/* character selection */
	void	dump(char *);
	GPT	sethandler(GPT);
	/* the following are from String(3) */
	/* position of first occurrence of char */
	int	strchr(const char c) { return d->strchr(c); }
	/* ... last ... (-1 if char not there) */
	int	strrchr(const char c) { return d->strrchr(c); }
	int	strpbrk(const String& oo) { return d->strpbrk(*oo.d); }
	int	strspn(const String& oo) { return d->strspn(*oo.d); }
	int	strcspn(const String& oo) { return d->strcspn(*oo.d); }
};

inline bit
operator==(const char* p, const String& s)
{
	return s.compare(p) == 0;
}
inline bit
operator>(const char* p, const String& s)
{
	return s < p;
}
inline bit
operator>=(const char* p, const String& s)
{
	return s >= p;
}
inline bit
operator<=(const char* p, const String& s)
{
	return s >= p;
}
inline bit
operator<(const char* p, const String& s)
{
	return s > p;
}
inline bit
operator!=(const char* p, const String& s)
{
	return s != p;
}

inline ostream&
operator<<(ostream& oo, const String& ss)
{
	return oo << *ss.d;
}

class SubString
{
friend String;
	static String	*ss;
	static int	oo;
	static int	ll;
	static GPT	handler;
	void	error(int = 0, char * = 0);
		SubString(const SubString&);
		SubString(String &ii, int off, int len) { this = 0;
				ss = &ii; oo = off; ll = len; }
		~SubString() { this = 0; }
public:
	void	operator=(const String&);
	GPT	sethandler(GPT);
	String	*it() { return ss; }
	int	offset() { return oo; }
	int	length() { return ll; }
};

class Subchar
{
friend String;
	static String	*ss;
	static int	oo;	/* position in the String */
	static GPT	handler;
	void	error(int = 0, char * = 0);
		Subchar(const Subchar&);
		Subchar(String &ii, int off) {
				this = 0; ss = &ii; oo = off; }
		~Subchar() { this = 0; }
public:
	char	operator=(const char);
	operator char() { return *(ss->d->start + oo); }
	GPT	sethandler(GPT);
	String	*it() { return ss; }
	int	offset() { return oo; }
};

overload length;
inline int	length(const String &s) { return s.length(); }
overload hashval;
inline int	hashval(const String &s) { return s.hashval(); }
overload compare;
inline int	compare(const String &s,const String &t) { return s.compare(t); }
inline int	strchr(const String& s, const char c) { return s.strchr(c); }
inline int	strrchr(const String& s, const char c) { return s.strrchr(c); }
inline int	strpbrk(const String& s, const String& t) { return s.strpbrk(t); }
inline int	strspn(const String& s, const String& t) { return s.strspn(t); }
inline int	strcspn(const String& s, const String& t) { return s.strcspn(t); }

String	sgets(istream&);
int	fgets(String&, int, FILE*);
int	gets(String&);
#endif

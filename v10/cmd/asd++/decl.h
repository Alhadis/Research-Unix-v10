#include <String.h>
#include <list.h>

extern "C" {
	extern char *getenv(char *);
	extern int mkdir (char*, int);
	extern int rmdir (char*);
	int sleep (int);
	extern int getlogname (char*);
}

int mkdir (const String&, int);
int rmdir (const String&);


// decl.c

listdeclare (String)

struct mel {
	String p;
	String_list q;
	String s;
	bit operator== (const mel& x) const
		{ return p == x.p && q == x.q && s == x.s; }
	ostream& print (ostream&);
};

listdeclare (mel)

struct map {
	mel_list l;
	ostream& print (ostream&);
	istream& read (istream&);
};

inline ostream& operator<< (ostream& o, map& m) { return m.print (o); }
inline istream& operator>> (istream& i, map& m) { return m.read (i); }

String logname();

class charstr {
	char* p;
public:
	charstr (String s) {
		p = new char[s.length() + 1];
		s.dump (p);
	}
	~charstr() { delete p; }
	operator char*() { return p; };
private:
	charstr(const charstr&);
	void operator=(const charstr&);
};

// path.c

String pathnorm (String);

class Path {
	String s;
	friend Path mkpath(String);
public:
	Path() {}
	Path(const Path& p): s (p.s) {}
	Path(const String& st): s (pathnorm (st)) {}
	Path(const char *cp): s (pathnorm (String (cp))) {}
	Path& operator = (const Path& p) { s = p.s; return *this; }
	Path operator + (const Path& p) const { return s + p.s; }
	Path operator + (char *cp) const { return s + pathnorm (cp); }
	friend Path operator + (char *p, const Path& q)
		{ return Path (p) + q; }
	Path& operator += (const Path& p) { s += Path(p.s); return *this; }
	Path operator & (const Path&) const;
	Path operator & (const String& s) const { return *this & Path(s); }
	Path operator & (const char *cp) const { return *this & Path (cp); }
	friend Path operator & (const char *p, Path q)
		{ return Path (p) & q; }
	friend Path operator & (const char *p, String q)
		{ return Path (p) & Path(q); }
	friend Path operator & (const String& p, char* q)
		{ return Path(p) & q; }
	Path& operator &= (const Path& p) { *this = *this & p; return *this; }
	operator String () const { return s; }
	Path first() const;
	Path last() const;
	Path rmfirst() const;
	Path rmlast() const;
	friend ostream& operator <<(ostream&, const Path&);
	int operator < (const Path& t) const { return s < t.s; }
	int operator > (const Path& t) const { return s > t.s; }
	int operator <= (const Path& t) const { return s <= t.s; }
	int operator >= (const Path& t) const { return s >= t.s; }
	int operator == (const Path& t) const { return s == t.s; }
	int operator != (const Path& t) const { return s != t.s; }
	int operator < (char* t) const { return s < t; }
	int operator > (char* t) const { return s > t; }
	int operator <= (char* t) const { return s <= t; }
	int operator >= (char* t) const { return s >= t; }
	int operator == (char* t) const { return s == t; }
	int operator != (char* t) const { return s != t; }
	friend int operator < (char* p, const Path& q) { return p < q.s; }
	friend int operator > (char* p, const Path& q) { return p > q.s; }
	friend int operator <= (char* p, const Path& q) { return p <= q.s; }
	friend int operator >= (char* p, const Path& q) { return p >= q.s; }
	friend int operator == (char* p, const Path& q) { return p == q.s; }
	friend int operator != (char* p, const Path& q) { return p != q.s; }
};

Path homedir();

listdeclare(Path)

// lock.c

void lock (Path&);
void unlock (Path&);

// dir.c

int isdir (Path&);
Path_list dircontents (Path&);

// domach.c

String domach (String, Path, String_list&);

// checksum.c

static const cksize = 37;

class checksum {
	char buf[cksize];
	char *bufptr;
	char *bufend;
public:
	checksum();
	void combine (char*, int);
	int operator== (checksum&);
	int operator!= (checksum& c) { return !(*this == c); }
	friend int read (int fd, checksum& c)
		{ return ::read (fd, c.buf, cksize); }
	friend int write (int fd, checksum& c)
		{ return ::write (fd, c.buf, cksize); }
};

// param.c

extern char* spooldir;
extern char* etcdir;
extern char* inspkg;
extern char* tmpdir;

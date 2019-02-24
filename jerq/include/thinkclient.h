#ifndef	THINKCLIENT_H
#define	THINKCLIENT_H

struct Thinksubs {
	int (*Thinkchar)();
	int (*Thinknchars)();
	int (*Thinkflush)();
	int (*Thinkmap)();
	int (*Thinkabort)();
};

#ifndef	THINKSHARE_C

#define Thfun(f)	(((struct Thinksubs *)(thclient->data))->f)

#define Thtest	((thclient || (thclient=getname("thinksubs"))) && thclient->proc)

#define Thcall0(f)		(Thtest ? Thfun(f)() : -1)
#define Thcall1(f, a)		(Thtest ? Thfun(f)(a) : -1)
#define Thcall2(f, a, b)	(Thtest ? Thfun(f)((a), (b)) : -1)

#define thinkchar(c)		Thcall1(Thinkchar, c)
#define thinknchars(n, p)	Thcall2(Thinknchars, n, p)
#define thinkflush()		Thcall0(Thinkflush)
#define thinkmap(b, r)		Thcall2(Thinkmap, b, r)
#define thinkabort()		Thcall0(Thinkabort)

struct Nqueue *thclient;

#endif

#endif

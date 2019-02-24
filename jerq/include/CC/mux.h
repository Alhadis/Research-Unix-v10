#ifndef	MUX_H
#define	MUX_H	MUX_H
#ifndef	LAYER_H
#include <CC/layer.h>
#endif
#ifndef	JERQPROC_H
#include <CC/jerqproc.h>
#endif

#define	WAIT	0
#define	NOWAIT	1

extern Bitmap *Jdisplayp;
#define	display	(*Jdisplayp)
typedef	int (*ptr_fint)();
#define	Sys	((ptr_fint *)0x0071e700)
class Proc;
#define	P	(*((struct Proc	**)0x0071e700))
class Font;

/*
 * Cast	macros
 */
#define	Cast(t,	x)	((t (*)(...))Sys[x])
#define	TPoint(x)	Cast(Point, x)
#define	TRectangle(x)	Cast(Rectangle,	x)
#define	Tint(x)		Cast(int, x)
#define	TpBitmap(x)	Cast(Bitmap *, x)
#define	TpLayer(x)	Cast(Layer *, x)
#define	TpWord(x)	Cast(Word *, x)
#define	Tpchar(x)	Cast(char *, x)
#define	Tvoid(x)	Cast(void, x)
#define	TpProc(x)	Cast(struct Proc *, x)
#define	IPoint(x)	(*Cast(Point, x))
#define	IRectangle(x)	(*Cast(Rectangle, x))
#define	Iint(x)		(*Cast(int, x))
#define	Ilong(x)	(*Cast(long, x))
#define	IpBitmap(x)	(*Cast(Bitmap *, x))
#define	IpTexture(x)	(*Cast(Texture *, x))
#define	IpLayer(x)	(*Cast(Layer *,	x))
#define	IpWord(x)	(*Cast(Word *, x))
#define	Ipchar(x)	(*Cast(char *, x))
#define	Ivoid(x)	(*Cast(void, x))
#define	IpProc(x)	(*Cast(struct Proc *, x))

/*
 * System calls
 */
#undef BonW
#undef WonB
/* Thank Ken for Regular Expressions */
#define	add(p, q)	IPoint(2)(p, q)	/*'add'*/
#define	addr(b,	p)	IpWord(3)(b, p)	/*'addr'*/
#define	alarm(n)	Ivoid(4)(n)	/*'alarm'*/
#define	alloc(u)	Ipchar(5)(u)	/*'Ualloc'*/
#define	balloc(r)	IpBitmap(6)(r)	/*'Uballoc'*/
#define	bfree(p)	Ivoid(7)(p)	/*'bfree'*/
#define	bitblt(sb, r, db, p, f)	Ivoid(8)(sb, r,	db, p, f)	/*'Ubitblt'*/
#define	cursallow()	Ivoid(9)()	/*'Ucursallow'*/
#define	cursinhibit()	Ivoid(10)()	/*'Ucursinhibit'*/
#define	cursset(p)	Ivoid(11)(p)	/*'Ucursset'*/
#define	cursswitch(c)	IpTexture(12)(c)	/*'Ucursswitch'*/
#define	debug()		IpProc(13)()	/*'debug'*/
#define	dellayer(l)	Iint(14)(l)	/*'dellayer'*/
#define	div(p, n)	IPoint(15)(p, n)	/*'div'*/
#define	eqrect(r, s)	Iint(16)(r, s)	/*'eqrect'*/
#define	exit()		Ivoid(17)()	/*'Uexit'*/
#define	free(p)		Ivoid(18)(p)	/*'free'*/
#define	gcalloc(n, w)	Ipchar(19)(n, w, P)	/*'realgcalloc'*/
#define	gcfree(s)	Ivoid(20)(s)		/*'gcfree'*/
#define	getrect(n)	IRectangle(79)(n)	/*'getrect'*/
#define	inset(r, n)	IRectangle(22)(r, n)	/*'inset'*/
#define	Jcursallow()	Ivoid(23)()	/*'cursallow'*/
#define	Jcursinhibit()	Ivoid(24)()	/*'cursinhibit'*/
#define	kbdchar()		Iint(25)()		/*'Ukbdchar'*/
#define	lpoint(b, p, f)	Ivoid(26)(b, p,	f)		/*'lpoint'*/
#define	lrectf(b, r, f)	Ivoid(27)(b, r,	f)		/*'lrectf'*/
#define	lsegment(b, p, q, f)	Ivoid(28)(b, p,	q, f)		/*'lsegment'*/
#define	ltexture(b, r, t, f)	Ivoid(29)(b, r,	t, f)		/*'ltexture'*/
#define	menuhit(m, n)	Iint(30)(m, n)	/*'menuhit'*/
#define	mul(p, n)	IPoint(31)(p, n)	/*'mul'*/
#define	muxnewwind(p,c)	Ivoid(32)(p,c)	/*'muxnewwind'*/
#define	nap(s)		Ivoid(33)(s)		/*'nap'*/
#define	newlayer(r)	IpLayer(34)(r)	/*'newlayer'*/
#define	newproc(f)	IpProc(35)(f)	/*'Unewproc'*/
#define	newwindow(f)	Ivoid(36)(f)	/*'newwindow'*/
#define	own()		Iint(37)()	/*'Uown'*/
#define	point(l, p, f)	Ivoid(38)(l, p,	f)	/*'Upoint'*/
#define	ptinrect(p, r)	Iint(39)(p, r)	/*'ptinrect'*/
#define	raddp(r, p)	IRectangle(40)(r, p)	/*'raddp'*/
#define	rcvchar()	Iint(41)()	/*'Urcvchar'*/
#define	realtime()	Ilong(42)()	/*'realtime'*/
#define	rectXrect(r, s)	Iint(43)(r, s)	/*'rectXrect'*/
#define	rectclip(pr, r)	Iint(44)(pr, r)	/*'rectclip'*/
#define	rectf(l, r, f)	Ivoid(45)(l, r,	f)	/*'Urectf'*/
#define	request(r)	Iint(46)(r)	/*'Urequest'*/
#define	rsubp(r, p)	IRectangle(47)(r, p)	/*'rsubp'*/
#define	screenswap(b, r, s)	Ivoid(48)(b, r,	s)	/*'Uscreenswap'*/
#define	segment(l, p, q, f)	Ivoid(49)(l, p,	q, f)	/*'Usegment'*/
#define	sendchar(c)	Ivoid(50)(c)	/*'Usendchar'*/
#define	sendnchars(n, p)	Ivoid(51)(n,p)	/*'sendnchars'*/
#define	sleep(s)	Ivoid(52)(s)		/*'sleep'*/
#define	XXXXXXXX(r)	Ivoid(53)(r, 1)	/*'0'*/
#define	string(F, s, b,	p, f)	IPoint(54)(F, s, b, p, f)	/*'string'*/
#define	strwidth(F, s)	Iint(55)(F,s)	/*'strwidth'*/
#define	sub(p, q)	IPoint(56)(p, q)	/*'sub'*/
#define	texture(l, r, m, f)	Ivoid(57)(l, r,	m, f)	/*'Utexture'*/
#define	tolayer(l)	Ivoid(58)(l)	/*'tolayer'*/
#define	upfront(l)	Iint(59)(l)	/*'upfront'*/
#define	wait(n)		Iint(60)(n)	/*'Uwait'*/
#define	kill(i)		Iint(61)(i, P)	/*'muxkill'*/
#define	setmuxbuf(s)	Ivoid(62)(s)	/*'setmuxbuf'*/
#define	getmuxbuf(s)	Ivoid(63)(s)	/*'getmuxbuf'*/
#define	strinsure(s, n)	Ivoid(64)(s, n, P)	/*'insure'*/
#define	GCalloc(n, p)	Ivoid(65)(n, p, P)	/*'GCalloc'*/
#define	movstring(i, s, d)	Ivoid(66)(i, s, d)	/*'movstring'*/
#define	strinsert(p, i, q)	Ivoid(67)(p, i, q)	/*'insstring'*/
#define	strdelete(p, i, j)	Ivoid(68)(p, i, j)	/*'delstring'*/
#define	frselect(f, pt)	Ivoid(69)(f, pt)	/*'frselect'*/
#define	fralloc(r, m)	IpFrame(70)(r, m)	/*'fralloc'*/
#define	frsetrects(f, r)	Ivoid(71)(f, r)	/*'frsetrects'*/
#define	frinit(f)	Ivoid(72)(f)	/*'frinit'*/
#define	frfree(f)	Ivoid(73)(f)	/*'frfree'*/
#define	ptofchar(f, n)	IPoint(74)(f, n)	/*'ptofchar'*/
#define	charofpt(f, p)	Iint(75)(f, p)	/*'charofpt'*/
#define	border(p, r, i, c)	Iint(76)(p, r, i, c)	/*'border'*/
#define	getproctab()	IpProc(77)()	/*'getproctab'*/
#define	lbitblt(sb, r, db, p, f)	Ivoid(78)(sb, r,	db, p, f)	/*'lbitblt'*/
#define	getrect(n)	IRectangle(79)(n)	/*'getrect'*/
#define	getrectb(n)	IRectangle(80)(n)	/*'getrectb'*/
#define	qputc(q, w)	Iint(81)(q, w)		/*'qputc'*/
#define	qgetc(q)	Iint(82)(q)		/*'qgetc'*/
#define	qputstr(q, s)	Iint(83)(q, s)		/*'qputstr'*/
#define	qclear(q)	Ivoid(84)(q)		/*'qclear'*/
#define	jstrwidth(s)	Iint(85)(s)		/*'jstrwidth'*/
#define	getproc()	IpProc(86)()	/*'getproc'*/
#define layerop		Ivoid(87)		/*'layerop'*/
#define	putname(s,w)	Iint(88)(s,w)	/*'putname'*/
#define	getname(s,w)	IpProc(89)(s,w)	/*'getname'*/
#endif	MUX_H

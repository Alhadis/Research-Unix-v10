#ifndef listdeclare
#ifndef GENERICH
#include <generic.h>
#endif
#include <stream.h>

#ifndef TRUE
#define FALSE	0
#define TRUE	(!FALSE)
#endif

#ifndef NULL
#define NULL	0
#endif

#ifndef BIT_DEFINED
#define BIT_DEFINED
typedef int	bit;
#endif

struct lnk {
	lnk*	nxt;
	lnk*	prv;
	void	init(lnk* p, lnk* s) { prv = p; nxt = s; }
		lnk() {}
		~lnk() {}
};
struct LstHead {
	lnk	*t;	// tail
	lnk	*h() { return t ? t->nxt : 0; }
	int	sz;
	lnk	*cache;		// a recently retrieved link
	int	cacheNo;	// its index or garbage if cache == NULL
	int	refCount;
		LstHead() { sz = 0; cache = t = NULL; }
		virtual ~LstHead();
	void	insert(lnk*);	// used by put() and unget()
	void	unget(lnk* nl) { insert(nl); cacheNo++; }
	void	put(lnk* nl) { insert(nl); t = nl; }
	lnk*	get();
	lnk*	unput();
	lnk*	getAt(int);
};
struct lstiter;
struct List {
	friend lstiter;
	lstiter	*fI;
	LstHead	*it;
	virtual void	sepWork();
	void	separate() { if (it->refCount > 1) sepWork(); }
	int	length() const { return it->sz; }
		List() { fI = NULL; }
		List(List& x) { fI = NULL; it = x.it; it->refCount++; }
		~List();
	lnk*	tail() const { return it->t; }
	lnk*	head() const { return it->h(); }
	lstiter*	firstIt() { return fI; }
	void	newIt(lstiter*, int=0);
	void	removeIt(lstiter*);
		operator void*() { return length() ? this : 0; }
	List&	operator=(List&);
	List&	unget(lnk*);
	List&	put(lnk*);
	lnk*	get();
	lnk*	unput();
};
struct lstiter {
	friend	List;
	lnk	*pred;
	List	*aList;
	lstiter	*nextIt;
	int	index;
		lstiter(List&, int =0);
		lstiter(lstiter&);
		~lstiter();
	List	*theList() { return aList; }
	lstiter&	operator=(lstiter&);
	bit	operator==(lstiter& oo) {
				return aList == oo.aList &&
				index == oo.index; }
	bit	atEnd() { return index == aList->length(); }
	bit	r_atEnd() { return index == 0 ; }
	lnk*	next();
	lnk*	r_next();
	lnk*	peek();
	lnk*	r_peek();
	void	insert(lnk*);
	void	r_insert(lnk*);
	lnk*	remove();
	lnk*	r_remove();
	void	reset(int =0);
	void	r_reset(int i=0) { reset(aList->length() - i); }
};

extern "C" {
	extern int abort();
}

#define lnnk(type) name2(type,lnnk)
#define list(type) name2(type,_list)
#define ListHead(type) name2(type,ListHead)
#define listiterator(type) name2(type,_list_iterator)
#define listsubitem(type) name2(type,listsubitem)
#define plist(type) name2(type,P_list)
#define plistiterator(type) name2(type,P_list_iterator)
#define plistsubitem(type) name2(type,Plistsubitem)
#define CMPF(type) name2(type,CMPFN)
#define CMPFP(type) name2(type,CMPFNP)

#define listdeclare(type)							\
extern GPT errorhandler(list,type);						\
extern GPT set_handler(list,type,GPT);						\
extern GPT errorhandler(listiterator,type);					\
extern GPT set_handler(listiterator,type,GPT);					\
class list(type);								\
class lnnk(type);								\
class ListHead(type);								\
class listiterator(type);							\
class listsubitem(type);							\
typedef int	(*CMPF(type))(type&, type&);					\
ostream&	operator<<(ostream&, list(type)&);				\
class lnnk(type) : public lnk {							\
	friend ListHead(type);							\
	friend list(type);							\
	friend listsubitem(type);						\
	friend listiterator(type);						\
	friend ostream& operator<<(ostream&, list(type)&);			\
	friend void voidP_list_sort_internal(voidP_list&, CMPF(voidP));		\
	type	val;								\
		lnnk(type)(type& pp) : val(pp) {}				\
		~lnnk(type)() {}						\
};										\
class ListHead(type) : public LstHead {						\
	friend list(type);							\
		ListHead(type)() {}						\
		~ListHead(type)();						\
	lnnk(type)	*tail() { return (lnnk(type)*)t; }			\
	lnnk(type)	*head() { return (lnnk(type)*)h(); }			\
	bit	firstX(type&);							\
	bit	lastX(type&);							\
};										\
class list(type) : public List {						\
	lnnk(type)	*tail() const { return (lnnk(type)*)List::tail(); }	\
	lnnk(type)	*head() const { return (lnnk(type)*)List::head(); }	\
	void	init() { it = new ListHead(type); it->refCount = 1; }		\
	void	sepWork();							\
public:										\
	friend ostream& operator<<(ostream&, list(type)&);			\
	friend void voidP_list_sort_internal(voidP_list&, CMPF(voidP));		\
		list(type)() { init(); }					\
		list(type)(list(type)& x) : List(x) {}				\
		~list(type)() {}						\
		operator const void*() const { return length() ? this : 0; }	\
	bit	operator==(list(type)&) const;					\
	bit	operator!=(list(type)& x) const { return !(*this == x); }	\
	list(type)&	operator=(list(type)& ll) { return (list(type)&)	\
		(*(List*)this = (List&)ll); }					\
	list(type)&	put(type &x) { return	(list(type)&)			\
				 List::put(new lnnk(type)(x)); }		\
	list(type)&	put(list(type)&);	/* append */			\
	list(type)&	operator+=(list(type)& oo) { return put(oo); }		\
	list(type)&	operator+=(type& t) { return put(t); }			\
	list(type)	operator+(list(type)& ll) { list(type) ans = *this;	\
				ans += ll; return ans; }			\
	list(type)	operator+(type& t) { list(type) ans = *this;		\
				ans += t; return ans; }				\
	bit	unputX(type &t);						\
	bit	unput();							\
	bit	getX(type&);		/* get */				\
	bit	get();		/* get */					\
	list(type)&	unget(type &x) { return	(list(type)&)			\
				 List::unget(new lnnk(type)(x)); }		\
	list(type)&	unget(list(type)&);	/* prepend */			\
	bit	firstX(type& t) { return					\
			((ListHead(type)*)it)->firstX(t); }			\
	bit	lastX(type& t) { return						\
			((ListHead(type)*)it)->lastX(t); }			\
/* getAt(0) returns the head of the list */					\
	type	getAt(int ii) { return ((lnnk(type)*)it->getAt(ii))->val; }	\
	inline listsubitem(type)	operator[](unsigned ii);		\
		list(type)(type& x) { init(); put(x); }				\
		list(type)(type& x, type& y) { init(); put(x); put(y); }	\
		list(type)(type& x, type& y, type& z) { init(); put(x); put(y);	\
			put(z); }						\
		list(type)(type& x, type& y, type& z, type& w) { init();	\
			put(x); put(y); put(z); put(w); }			\
	void	sort(CMPF(type));						\
};										\
class listiterator(type) : public lstiter {					\
	friend listsubitem(type);						\
	friend list(type);							\
public:										\
		listiterator(type)(list(type)& a, int i=0) :			\
			lstiter(*(List*)&a, i) {}				\
		listiterator(type)(listiterator(type)& a) :			\
			lstiter(*(lstiter*)&a) {}				\
		~listiterator(type)() {}					\
	listiterator(type)&	operator=(listiterator(type)& a) { return	\
			(listiterator(type)&)(*(lstiter*)this = (lstiter&)a); }	\
	bit	nextX(type&);							\
	bit	nextX(type*&);							\
	bit	next() { return lstiter::next() != 0; }				\
	bit	r_nextX(type&);							\
	bit	r_nextX(type*&);						\
	bit	r_next() { return lstiter::r_next() != 0; }			\
	bit	peekX(type&);							\
	bit	peekX(type*&);							\
	bit	peek() { return !atEnd(); }					\
	bit	r_peekX(type&);							\
	bit	r_peekX(type*&);						\
	bit	r_peek() { return !r_atEnd(); }					\
/* remove -- deletes current from list */					\
	bit	remove();							\
	bit	r_remove();							\
	bit	removeX(type&);							\
	bit	r_removeX(type&);						\
/* insert -- put it at the left of the pointer */				\
	void	insert(type& x) { lstiter::insert(new lnnk(type)(x)); }		\
	void	r_insert(type& x) { lstiter::r_insert(new lnnk(type)(x)); }	\
	bit	replace(type&);							\
	bit	r_replace(type&);						\
};										\
class listsubitem(type) : public listiterator(type) {				\
public:										\
	listsubitem(type)(list(type)& t, unsigned i) :				\
			 listiterator(type)(t, i) {}				\
	listsubitem(type)(listsubitem(type)& a) :				\
		listiterator(type)((listiterator(type)&)a) {}			\
	~listsubitem(type)() {}							\
	type&	operator=(type& x) { r_replace(x); return x; }			\
	operator type();							\
};										\
inline listsubitem(type)							\
list(type)::operator[](unsigned ii)						\
{										\
	return listsubitem(type)(*this, ii);					\
}

typedef void*	voidP;
listdeclare(voidP)

#define plistdeclare(type)							\
extern GPT errorhandler(Plist,type);						\
extern GPT set_handler(Plist,type,GPT);						\
extern GPT errorhandler(Plistiterator,type);					\
extern GPT set_handler(Plistiterator,type,GPT);					\
class plist(type);								\
typedef int	(*CMPFP(type))(type*&, type*&);					\
ostream&	operator<<(ostream&, plist(type)&);				\
class plistiterator(type);							\
class plistsubitem(type);							\
class plist(type) : public voidP_list {						\
public:										\
	friend ostream& operator<<(ostream&, plist(type)&);			\
		plist(type)() {}						\
		plist(type)(type* x) : voidP_list((voidP) x) {}			\
		plist(type)(type* x, type* y) :					\
			voidP_list((voidP) x, (voidP) y) {}			\
		plist(type)(type* x, type* y, type* z) :			\
				voidP_list((voidP) x, (voidP) y, (voidP) z) {}	\
		plist(type)(type* x, type* y, type* z, type* w) :		\
		    voidP_list ((voidP) x, (voidP) y, (voidP) z, (voidP) w) {}	\
		plist(type)(plist(type)& ll) :					\
			voidP_list((voidP_list&) ll) {}				\
		~plist(type)() {}						\
		operator const void*() const					\
			{ return voidP_list::operator void*(); }		\
	bit	operator==(plist(type)& ll) { return ((voidP_list&)*this ==	\
			(voidP_list&)ll); }					\
	plist(type)&	operator=(plist(type)& ll) { return (plist(type)&)	\
		((voidP_list&)*this = (voidP_list&)ll); }			\
	plist(type)	operator+(plist(type)& ll) { 				\
		return (plist(type)&)((voidP_list&)*this + (voidP_list&)ll); }	\
	plist(type)	operator+(type* t) {					\
		return (plist(type)&) ((voidP_list&)*this + (voidP)t); }	\
	plist(type)&	put(type *t) { return (plist(type)&)			\
		voidP_list::put((voidP)t); }					\
	plist(type)&	put(plist(type)& ll) { return (plist(type)&)		\
		voidP_list::put((voidP_list&)ll); }				\
	plist(type)&	operator+=(plist(type)& oo) { return put(oo); }		\
	plist(type)&	operator+=(type* t) { return put(t); }			\
	bit	unputX(type *&t) { return voidP_list::unputX((voidP&)t); }	\
	bit	unput() { return voidP_list::unput(); }				\
	bit	getX(type *&t) { return voidP_list::getX((voidP&)t); }		\
	bit	get() { return voidP_list::get(); }				\
	plist(type)&	unget(type *x) { return (plist(type)&)			\
		voidP_list::unget((voidP)x); }					\
	plist(type)&	unget(plist(type)& ll) { return (plist(type)&)		\
		voidP_list::unget((voidP_list&)ll); }				\
	bit	firstX(type *&t) { return voidP_list::firstX((voidP&)t); }	\
	bit	lastX(type *&t) { return voidP_list::lastX((voidP&)t); }	\
/* getAt(0) returns the head of the list */					\
	type*	getAt(int ii) { return (type *) voidP_list::getAt(ii); }	\
	inline plistsubitem(type)	operator[](unsigned ii);		\
	void	sort(CMPFP(type) pf) { voidP_list::sort((CMPF(voidP))pf); }	\
};										\
class plistiterator(type) : public voidP_list_iterator {			\
public:										\
		plistiterator(type)(plist(type)& a, int i=0) :			\
					((voidP_list&)a, i) {}			\
		plistiterator(type)(plistiterator(type)& a) :			\
			((voidP_list_iterator&)a) {}				\
		~plistiterator(type)() {}					\
	plistiterator(type)&	operator=(plistiterator(type)& a) { return	\
			(plistiterator(type)&)((voidP_list_iterator&)*this =	\
			(voidP_list_iterator&)a); }				\
	bit	nextX(type *&t) { return					\
			voidP_list_iterator::nextX((voidP&)t); }		\
	bit	nextX(type **&t) { return					\
			voidP_list_iterator::nextX((voidP*&)t); }		\
	bit	next() { return voidP_list_iterator::next(); }			\
	bit	r_nextX(type *&t) { return					\
			voidP_list_iterator::r_nextX((voidP&)t); }		\
	bit	r_nextX(type **&t) { return					\
			voidP_list_iterator::r_nextX((voidP*&)t); }		\
	bit	r_next() { return voidP_list_iterator::r_next(); }		\
	bit	peekX(type *&t) { return					\
			voidP_list_iterator::peekX((voidP&)t); }		\
	bit	peekX(type **&t) { return					\
			voidP_list_iterator::peekX((voidP*&)t); }		\
	bit	peek() { return voidP_list_iterator::peek(); }			\
	bit	r_peekX(type *&t) { return					\
			voidP_list_iterator::r_peekX((voidP&)t); }		\
	bit	r_peekX(type **&t) { return					\
			voidP_list_iterator::r_peekX((voidP*&)t); }		\
	bit	r_peek() { return voidP_list_iterator::r_peek(); }		\
	bit	remove() { return voidP_list_iterator::remove(); }		\
	bit	r_remove() { return voidP_list_iterator::r_remove(); }		\
	bit	removeX(type *&x) { return					\
			voidP_list_iterator::removeX((voidP&)x); }		\
	bit	r_removeX(type *&x) { return					\
			voidP_list_iterator::r_removeX((voidP&)x); }		\
	void	insert(type *&x) { voidP_list_iterator::insert((voidP&)x); }	\
	void	r_insert(type *&x) {						\
			voidP_list_iterator::r_insert((voidP&)x); }		\
	void	replace(type *x) { voidP_list_iterator::replace((voidP)x); }	\
	void	r_replace(type *x) {						\
			voidP_list_iterator::r_replace((voidP)x); }		\
};										\
class plistsubitem(type) : public voidPlistsubitem {				\
public:										\
	plistsubitem(type)(plist(type)& t, int i) :				\
			voidPlistsubitem ((voidP_list&)t, i) {}			\
	plistsubitem(type)(plistsubitem(type)&ll) :				\
			voidPlistsubitem ((voidPlistsubitem&)ll){}		\
	~plistsubitem(type)() {}						\
	type*&	operator=(type *&t) { return					\
			(type*&) ((voidPlistsubitem&)*this =(voidP&)t); }	\
	operator type*() { return (type*) voidPlistsubitem::operator voidP(); }	\
};										\
inline										\
plistsubitem(type)								\
plist(type)::operator[](unsigned ii)						\
{										\
	return plistsubitem(type)(*this, ii);					\
}


#define listimplement(type)							\
GPT errorhandler(list,type) = genericerror;					\
GPT errorhandler(listiterator,type) = genericerror;				\
bit										\
ListHead(type)::firstX(type& v)							\
{										\
	lnnk(type)	*aLink = (lnnk(type) *)LstHead::h();			\
	return aLink ? (v = aLink->val, TRUE) : FALSE;				\
}										\
bit										\
ListHead(type)::lastX(type& v)							\
{										\
	lnnk(type)	*aLink = (lnnk(type) *)LstHead::t;			\
	return aLink ? (v = aLink->val, TRUE) : FALSE;				\
}										\
ListHead(type)::~ListHead(type)()						\
{										\
	lnnk(type)	*temp;							\
	while (temp = (lnnk(type) *)LstHead::get())				\
		delete temp;							\
}										\
bit										\
listiterator(type)::remove()							\
{										\
	lnnk(type)	*aLink = (lnnk(type) *)lstiter::remove();		\
	return aLink ? (delete aLink, TRUE) : FALSE;				\
}										\
bit										\
listiterator(type)::removeX(type &x)						\
{										\
	lnnk(type)	*aLink = (lnnk(type) *)lstiter::remove();		\
	return aLink ? (x = aLink->val, delete aLink, TRUE) : FALSE;		\
}										\
bit										\
listiterator(type)::r_remove()							\
{										\
	lnnk(type)	*aLink = (lnnk(type) *)lstiter::r_remove();		\
	return aLink ? (delete aLink, TRUE) : FALSE;				\
}										\
bit										\
listiterator(type)::r_removeX(type &x)						\
{										\
	lnnk(type)	*aLink = (lnnk(type) *)lstiter::r_remove();		\
	return aLink ? (x = aLink->val, delete aLink, TRUE) : FALSE;		\
}										\
bit										\
listiterator(type)::nextX(type& t)						\
{										\
	return atEnd() ? FALSE :						\
	    (t = ((lnnk(type)*)lstiter::next())->val, TRUE);			\
}										\
bit										\
listiterator(type)::nextX(type*& t)						\
{										\
	return atEnd() ? FALSE :						\
	    (theList()->separate(), t = &((lnnk(type)*)lstiter::next())->val,	\
				TRUE);						\
}										\
bit										\
listiterator(type)::r_nextX(type& t)						\
{										\
	return r_atEnd() ? FALSE :						\
	    (t = ((lnnk(type)*)lstiter::r_next())->val, TRUE);			\
}										\
bit										\
listiterator(type)::r_nextX(type*& t)						\
{										\
	return r_atEnd() ? FALSE :						\
	    (theList()->separate(), t = &((lnnk(type)*)lstiter::r_next())->val,	\
				TRUE);						\
}										\
bit										\
listiterator(type)::r_peekX(type& t)						\
{										\
	return r_atEnd() ? FALSE :						\
	    (t = ((lnnk(type)*)lstiter::r_peek())->val, TRUE);			\
}										\
bit										\
listiterator(type)::r_peekX(type*& t)						\
{										\
	return r_atEnd() ? FALSE :						\
	    (theList()->separate(), t = &((lnnk(type)*)lstiter::r_peek())->val,	\
				TRUE);						\
}										\
bit										\
listiterator(type)::peekX(type& t)						\
{										\
	return atEnd() ? FALSE :						\
	    (t = ((lnnk(type)*)lstiter::peek())->val, TRUE);			\
}										\
bit										\
listiterator(type)::peekX(type*& t)						\
{										\
	return atEnd() ? FALSE :						\
	    (theList()->separate(), t = &((lnnk(type)*)lstiter::peek())->val,	\
				TRUE);						\
}										\
bit										\
listiterator(type)::replace(type& x)						\
{										\
	return r_atEnd() ? FALSE :						\
	    (theList()->separate(), ((lnnk(type)*)lstiter::r_peek())->val = x,	\
				TRUE);						\
}										\
bit										\
listiterator(type)::r_replace(type& x)						\
{										\
	return atEnd() ? FALSE :						\
	    (theList()->separate(), ((lnnk(type)*)lstiter::peek())->val = x,	\
				TRUE);						\
}										\
bit										\
list(type)::operator==(list(type)& x) const					\
{										\
	if ( length() != x.length() )						\
		return FALSE;							\
	if ( length() == 0 )							\
		return TRUE;							\
	lnnk(type)	*mine = (lnnk(type) *)head();				\
	lnnk(type)	*yours = (lnnk(type) *)x.head();			\
	for ( int i = length(); i--; )						\
		if ( mine->val == yours->val ) {				\
			mine = (lnnk(type) *)mine->nxt;				\
			yours = (lnnk(type) *)yours->nxt;			\
		} else								\
			return FALSE;						\
	return TRUE;								\
}										\
bit										\
list(type)::getX(type& t)							\
{										\
	lnnk(type)	*aLink = (lnnk(type) *)List::get();			\
	return aLink ? (t = aLink->val, delete aLink, TRUE) : FALSE;		\
}										\
bit										\
list(type)::get()								\
{										\
	lnnk(type)	*aLink = (lnnk(type) *)List::get();			\
	return aLink ? (delete aLink, TRUE) : FALSE;				\
}										\
bit										\
list(type)::unputX(type& t)							\
{										\
	lnnk(type)	*aLink = (lnnk(type) *)List::unput();			\
	return aLink ? (t = aLink->val, delete aLink, TRUE) : FALSE;		\
}										\
bit										\
list(type)::unput()								\
{										\
	lnnk(type)	*aLink = (lnnk(type) *)List::unput();			\
	return aLink ? (delete aLink, TRUE) : FALSE;				\
}										\
list(type)&									\
list(type)::put(list(type)& a)							\
{										\
	if (a.length()) {							\
		type	t;							\
		listiterator(type)	ita(a);					\
		while (ita.nextX(t))						\
			put(t);							\
	}									\
	return *this;								\
}										\
list(type)&									\
list(type)::unget(list(type)& a)						\
{										\
	if (a.length()) {							\
		type	t;							\
		listiterator(type)	ita(a);					\
		ita.r_reset();							\
		while (ita.r_nextX(t))						\
			unget(t);						\
	}									\
	return *this;								\
}										\
listsubitem(type)::operator type()						\
{										\
	type	t;								\
	peekX(t);								\
	return t;								\
}										\
void										\
list(type)::sepWork()								\
{										\
	it->refCount--;								\
	ListHead(type)	*newLst = new ListHead(type);				\
	if (tail()) {								\
		for (lnnk(type) *oldLnk = (lnnk(type) *)head();;		\
				oldLnk = (lnnk(type) *)oldLnk->nxt) {		\
			lnnk(type)	*newLnk;				\
			newLst->put(newLnk = new lnnk(type)(oldLnk->val));	\
			/* only be one or two listiterators (I hope) */		\
			for (listiterator(type) *anIt =				\
			    (listiterator(type) *)firstIt(); anIt;		\
			    anIt = (listiterator(type) *)anIt->nextIt) {	\
				if (anIt->pred == oldLnk) {			\
					anIt->pred = newLnk;			\
				}						\
			}							\
			if ( oldLnk == tail() )					\
				break;						\
		}								\
	}									\
	it = newLst;								\
	it->refCount = 1;							\
}										\
void										\
list(type)::sort(CMPF(type) cmp)						\
{										\
	if (length() < 2) return;						\
	separate();								\
	voidP_list_sort_internal(*(voidP_list*)this, (CMPF(voidP))cmp);		\
	for (listiterator(type) *anIt =						\
	    (listiterator(type) *)firstIt(); anIt;				\
	    anIt = (listiterator(type) *)anIt->nextIt)				\
		anIt->reset();							\
}

#define listoutimplement(type)							\
ostream&									\
operator<<(ostream& oo, list(type)& ll)						\
{										\
	oo<<"(";								\
	if ( ll.length() ) {							\
		listiterator(type)	it(ll);					\
		type	t;							\
		it.nextX(t);							\
		oo<<t;								\
		while (it.nextX(t))						\
			oo << " " << t;						\
	} else									\
		oo<<"EMPTYLIST";						\
	oo<<")";								\
	return oo;								\
}

#endif

#define plistimplement(type)							\
GPT errorhandler(Plist,type) = genericerror;					\
GPT errorhandler(Plistiterator,type) = genericerror;

#define plistoutimplement(type)							\
ostream&	operator<<(ostream& oo, plist(type)& ll)			\
{										\
	oo<<"(";								\
	if ( ll.length() ) {							\
		plistiterator(type)	it(ll);					\
		type	*t;							\
		it.nextX(t);							\
		oo<<t;								\
		while (it.nextX(t))						\
			oo << " " << t;						\
	} else									\
		oo<<"EMPTYLIST";						\
	oo<<")";								\
	return oo;								\
}


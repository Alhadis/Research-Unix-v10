#ifndef MAP_H
#define MAP_H 1
#ifndef GENERICH
#include <generic.h>
#endif

#ifndef __const
#ifdef __cplusplus
#define __const const
#else
#define __const
#endif
#endif

#define Map(S,T)name4(Map_,S,_,T)
#define Node(S,T)name4(_Node_,S,_,T)
#define Mapiter(S,T)name4(Mapiter_,S,_,T)

#define Mapdeclare(S,T)							\
class Node(S,T) {							\
	const S key;							\
	T value;							\
	Node(S,T)* L;							\
	Node(S,T)* R;							\
	Node(S,T)* U;							\
	char bal;							\
	Node(S,T)(const S& xs, const T& xt): key (xs), value(xt)	\
		{ L = R = 0; bal = '='; }				\
	~Node(S,T)();							\
	void attach (Node(S,T)*& p, Node(S,T)* dest)			\
		{ p = dest; if (dest) dest->U = this; }			\
	friend class Map(S,T);						\
	friend class Mapiter(S,T);					\
};									\
									\
static S name2(Map(S,T),_Sdef);						\
static T name2(Map(S,T),_Tdef);						\
									\
class Mapiter(S,T) {							\
	friend class Map(S,T);						\
	Map(S,T)* const m;						\
	Node(S,T)* p;							\
	Mapiter(S,T) (const Map(S,T)* map, Node(S,T)* n): m(map), p(n) { } \
public:									\
	Mapiter(S,T) (const Map(S,T)& map): m(&map), p(0) { }		\
	operator int() __const { return p != 0; }			\
	S key() __const { return *(p? &p->key: &name2(Map(S,T),_Sdef)); } \
	T value() __const { return *(p? &p->value: &name2(Map(S,T),_Tdef)); } \
	Mapiter(S,T)& operator++();					\
	Mapiter(S,T)& operator--();					\
};									\
									\
class Map(S,T) {							\
	friend class Mapiter(S,T);					\
	Node(S,T)* head;						\
	int n;								\
	T def;								\
	void clear();							\
	void init() { head = 0; n = 0; }				\
	T& newnode (Node(S,T)*&, const S&, Node(S,T)*);			\
	void rotL (Node(S,T)*);						\
	void rotR (Node(S,T)*);						\
public:									\
	Map(S,T)(): def (name2(Map(S,T),_Tdef)) { init(); }		\
	Map(S,T)(const T& d): def (d) { init(); }			\
	Map(S,T)(const Map(S,T)&);					\
	~Map(S,T)();							\
	Map(S,T)& operator= (const Map(S,T)&);				\
	T& operator[] (const S&);					\
	int size() __const { return n; }				\
	Mapiter(S,T) element (const S&) __const;			\
	Mapiter(S,T) first() __const { return ++Mapiter(S,T) (*this); }	\
	Mapiter(S,T) last() __const { return --Mapiter(S,T) (*this); }	\
};									\
									\
									\

#define Mapimplement(S,T)						\
									\
/*									\
 * Node destructor -- should be inline but cfront			\
 * lays it down even if not needed					\
 */									\
									\
Node(S,T)::~Node(S,T)()							\
{									\
	delete L;							\
	delete R;							\
}									\
									\
void									\
Map(S,T)::clear()							\
{									\
	delete head;							\
	init();								\
}									\
									\
Map(S,T)::~Map(S,T)()							\
{									\
	clear();							\
}									\
									\
Map(S,T)&								\
Map(S,T)::operator= (const Map(S,T)& m)					\
{									\
	if (this != &m) {						\
		clear();						\
		for (Mapiter(S,T) p (m); ++p; ) {			\
			S s = p.key();		/* sidestep a cfront bug */	\
			(*this) [s] = p.value();			\
		}							\
	}								\
	return *this;							\
}									\
									\
Map(S,T)::Map(S,T) (const Map(S,T)& m): def (m.def)			\
{									\
	init();								\
	operator= (m);							\
}									\
									\
T&									\
Map(S,T)::newnode (Node(S,T)*& ptr, const S& s, Node(S,T)* parent)	\
{									\
	ptr = new Node(S,T) (s, def);					\
	T& retval = ptr->value;						\
									\
	ptr->U = parent;						\
	n++;								\
									\
	/* rebalance */							\
									\
	/*								\
	 * phase 1 - run up the tree, looking for an unbalanced		\
	 * node and unbalancing all the balanced nodes we traverse	\
	 */								\
	Node(S,T)* p = ptr->U;						\
	Node(S,T)* q = ptr;						\
									\
	while (p && p->bal == '=') {					\
		p->bal = p->L == q? 'L': 'R';				\
		q = p;							\
		p = p->U;						\
	}								\
									\
	/*								\
	 * p is now 0 or points at an unbalanced node.  If 0, we're done. \
	 * Otherwise calculate the new balance factor for p.		\
	 */								\
	if (p) {							\
		if (p->bal == (p->L == q? 'R': 'L'))			\
			p->bal = '=';					\
		else {							\
			/* we have become truly unbalanced */		\
			if (p->bal == 'R') {				\
				if (q->bal == 'R') {			\
					rotL (p);			\
					p->bal = '=';			\
					q->bal = '=';			\
				} else {				\
					Node(S,T)* r = q->L;		\
					rotR (q);			\
					rotL (p);			\
					switch (r->bal) {		\
					case 'L':			\
						p->bal = '=';		\
						q->bal = 'R';		\
						break;			\
					case 'R':			\
						p->bal = 'L';		\
						q->bal = '=';		\
						break;			\
					case '=':			\
						p->bal = '=';		\
						q->bal = '=';		\
						break;			\
					}				\
					r->bal = '=';			\
				}					\
			} else {					\
				if (q->bal == 'L') {			\
					rotR (p);			\
					p->bal = '=';			\
					q->bal = '=';			\
				} else {				\
					Node(S,T)* r = q->R;		\
					rotL (q);			\
					rotR (p);			\
					switch (r->bal) {		\
					case 'R':			\
						p->bal = '=';		\
						q->bal = 'L';		\
						break;			\
					case 'L':			\
						p->bal = 'R';		\
						q->bal = '=';		\
						break;			\
					case '=':			\
						p->bal = '=';		\
						q->bal = '=';		\
						break;			\
					}				\
					r->bal = '=';			\
				}					\
			}						\
		}							\
	}								\
									\
	return retval;							\
}									\
									\
void									\
Map(S,T)::rotL (Node(S,T)* p)						\
{									\
	Node(S,T)* q = p->R;						\
	Node(S,T)* u = p->U;						\
	p->attach (p->R, q->L);						\
	q->attach (q->L, p);						\
	if (!u)								\
		head = q;						\
	else if (u->L == p)						\
		u->L = q;						\
	else								\
		u->R = q;						\
	q->U = u;							\
}									\
									\
void									\
Map(S,T)::rotR (Node(S,T)* p)						\
{									\
	Node(S,T)* q = p->L;						\
	Node(S,T)* u = p->U;						\
	p->attach (p->L, q->R);						\
	q->attach (q->R, p);						\
	if (!u)								\
		head = q;						\
	else if (u->R == p)						\
		u->R = q;						\
	else								\
		u->L = q;						\
	q->U = u;							\
}									\
									\
T&									\
Map(S,T)::operator[] (const S& s)					\
{									\
	if (head) {							\
		Node(S,T)* t = head;					\
		for(;;) {						\
			if (s < t->key) {				\
				if (t->L)				\
					t = t->L;			\
				else					\
					return newnode (t->L, s, t);	\
			} else if (t->key < s) {			\
				if (t->R)				\
					t = t->R;			\
				else					\
					return newnode (t->R, s, t);	\
			} else						\
				break;					\
		}							\
		return t->value;					\
	} else								\
		return newnode (head, s, 0);				\
}									\
									\
Mapiter(S,T)								\
Map(S,T)::element (const S& s) __const					\
{									\
	Node(S,T)* t = head;						\
	while (t) {							\
		if (s < t->key)						\
			t = t->L;					\
		else if (t->key < s)					\
			t = t->R;					\
		else							\
			break;						\
	}								\
	return Mapiter(S,T) (this, t);					\
}									\
									\
Mapiter(S,T)&								\
Mapiter(S,T)::operator++()						\
{									\
	if (p) {							\
		if (p->R)						\
			p = p->R;					\
		else {							\
			while (p) {					\
				Node(S,T)* q = p;			\
				p = p->U;				\
				if (p && p->L == q)			\
					return *this;			\
			}						\
		}							\
	} else								\
		p = m->head;						\
									\
	/* p = low(p) */						\
	if (p)								\
		while (p->L)						\
			p = p->L;					\
									\
	return *this;							\
}									\
									\
Mapiter(S,T)&								\
Mapiter(S,T)::operator--()						\
{									\
	if (p) {							\
		if (p->L)						\
			p = p->L;					\
		else {							\
			while (p) {					\
				Node(S,T)* q = p;			\
				p = p->U;				\
				if (p && p->R == q)			\
					return *this;			\
			}						\
		}							\
	} else								\
		p = m->head;						\
									\
	/* p = low(p) */						\
	if (p)								\
		while (p->R)						\
			p = p->R;					\
									\
	return *this;							\
}									\
									\
									\


#endif

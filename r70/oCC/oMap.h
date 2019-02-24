#ifndef MAP_H
#define MAP_H 1
#ifndef GENERICH
#include <generic.h>
#endif

#define Map(S,T)name4(Map_,S,_,T)
#define Pair(S,T)name4(Pair_,S,_,T)
#define Node(S,T)name4(_Node_,S,_,T)
#define Mapdeclare(S,T)							\
struct Pair(S,T) {							\
	S key;								\
	T data;								\
	Pair(S,T) (S& xs, T& xt): key (xs), data (xt) {}		\
};									\
									\
class Node(S,T): public Pair(S,T) {					\
	Node(S,T)* L;							\
	Node(S,T)* R;							\
	Node(S,T)* U;							\
	char bal;							\
	Node(S,T)(S& xs, T& xt): (xs, xt) { L = R = 0; bal = '='; }	\
	~Node(S,T)() { delete L; delete R; }				\
	void attach (Node(S,T)*& p, Node(S,T)* dest)			\
		{ p = dest; if (dest) dest->U = this; }			\
	friend class Map(S,T);						\
};									\
									\
class Map(S,T) {							\
	Node(S,T)* head;						\
	int n;								\
	T def;								\
	void clear();							\
	T& newnode (Node(S,T)*&, S&, Node(S,T)*);			\
	void rotL (Node(S,T)*);						\
	void rotR (Node(S,T)*);						\
public:									\
	Map(S,T)();							\
	Map(S,T)(T&);							\
	Map(S,T)(Map(S,T)&);						\
	~Map(S,T)();							\
	Map(S,T)& operator= (Map(S,T)&);				\
	T& operator[] (S&);						\
	int size() { return n; }					\
	Pair(S,T)* element (S&);					\
	Pair(S,T)* succ (Pair(S,T)*);					\
	Pair(S,T)* pred (Pair(S,T)*);					\
};									\
									\
									\

#define Mapimplement(S,T)						\
									\
void									\
Map(S,T)::clear()							\
{									\
	delete head;							\
	n = 0;								\
}									\
									\
static T name2(Map(S,T),_def);						\
									\
Map(S,T)::Map(S,T)(): def (name2(Map(S,T),_def))			\
{									\
	head = 0;							\
	n = 0;								\
}									\
									\
Map(S,T)::Map(S,T)(T& d): def (d)					\
{									\
	head = 0;							\
	n = 0;								\
}									\
									\
Map(S,T)::~Map(S,T)()							\
{									\
	clear();							\
}									\
									\
Map(S,T)&								\
Map(S,T)::operator= (Map(S,T)& m)					\
{									\
	clear();							\
	for (Pair(S,T)* p = m.succ(0); p; p = m.succ (p))		\
		operator[] (p->key) = p->data;				\
	return *this;							\
}									\
									\
Map(S,T)::Map(S,T) (Map(S,T)& m): def (m.def)				\
{									\
	head = 0;							\
	n = 0;								\
	operator= (m);							\
}									\
									\
T&									\
Map(S,T)::newnode (Node(S,T)*& ptr, S& s, Node(S,T)* parent)		\
{									\
	ptr = new Node(S,T) (s, def);					\
	T& retval = ptr->data;						\
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
				q = p->R;				\
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
				q = p->L;				\
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
Map(S,T)::operator[] (S& s)						\
{									\
	if (head) {							\
		Node(S,T)* t = head;					\
		while (s != t->key) {					\
			if (s < t->key) {				\
				if (t->L)				\
					t = t->L;			\
				else					\
					return newnode (t->L, s, t);	\
			} else {					\
				if (t->R)				\
					t = t->R;			\
				else					\
					return newnode (t->R, s, t);	\
			}						\
		}							\
		return t->data;						\
	} else								\
		return newnode (head, s, 0);				\
}									\
									\
Pair(S,T)*								\
Map(S,T)::element (S& s)						\
{									\
	Node(S,T)* t = head;						\
	while (t && s != t->key) {					\
		if (s < t->key)						\
			t = t->L;					\
		else							\
			t = t->R;					\
	}								\
	return t;							\
}									\
									\
Pair(S,T)*								\
Map(S,T)::succ (Pair(S,T)* p)						\
{									\
	Node(S,T)* np = (Node(S,T)*) p;					\
									\
	if (np) {							\
		if (np->R)						\
			np = np->R;					\
		else {							\
			while (np) {					\
				Node(S,T)* u = np->U;			\
				if (u && u->L == np)			\
					return u;			\
				np = u;					\
			}						\
		}							\
	} else								\
		np = head;						\
									\
	/* np = low(np) */						\
	if (np)								\
		while (np->L)						\
			np = np->L;					\
									\
	return np;							\
}									\
									\
Pair(S,T)*								\
Map(S,T)::pred (Pair(S,T)* p)						\
{									\
	Node(S,T)* np = (Node(S,T)*) p;					\
									\
	if (np) {							\
		if (np->L)						\
			np = np->L;					\
		else {							\
			while (np) {					\
				Node(S,T)* u = np->U;			\
				if (u && u->R == np)			\
					return u;			\
				np = u;					\
			}						\
		}							\
	} else								\
		np = head;						\
									\
	/* np = low(np) */						\
	if (np)								\
		while (np->R)						\
			np = np->R;					\
									\
	return np;							\
}									\
									\
									\


#endif

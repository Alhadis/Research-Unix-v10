#include "list.h"

LstHead::~LstHead() {}

void	// don't specify head or tail
LstHead::insert(lnk *nl)
{
	if (t) {
		nl->nxt = h();
		nl->prv = t;
		h()->prv = nl;
		t->nxt = nl;
	} else t = nl->nxt = nl->prv = nl;
	sz++;
}

lnk *
LstHead::get()
{
	if (!t)
		return NULL;
	lnk	*oh = h();
	if ( t == h() )
		t = NULL;
	else {
		lnk	*nh = oh->nxt;
		nh->prv = t;
		t->nxt = nh;
	}
	if (cache == oh) cache = NULL;
	else cacheNo--;
	sz--;
	return oh;
}

lnk *
LstHead::unput()
{
	if (!t)
		return NULL;
	lnk	*ot = t;
	lnk	*hh = h();
	if ( h() == ot )
		t = NULL;
	else {
		lnk	*nt = t->prv;
		hh->prv = t = nt;
		t->nxt = hh;
	}
	if (cache == ot) cache = NULL;
	sz--;
	return ot;
}

lnk*
LstHead::getAt(int ii)
{
	register	count;
	int	forward;
	register lnk*	from;
	if (ii >= sz) abort();	// out of bounds
	if (ii >= sz - ii) {
		count = sz - ii - 1;
		forward = FALSE;
		from = t;
	} else {
		count = ii;
		forward = TRUE;
		from = h();
	}
	if (cache) {
		register	altCount = ii - cacheNo;
		register	a1 = altCount > 0 ? altCount : -altCount;
		if (a1 < count) {
			count = a1;
			forward = a1 == altCount;
			from = cache;
		}
	}
	if (forward)
		while (count--)
			from = from->nxt;
	else
		while (count--)
			from = from->prv;
	cache = from;
	cacheNo = ii;
	return from;
}

void
List::sepWork()
{
	abort();	// Only derived functions should be called.
}
void
List::newIt(lstiter *a, int i)
{
	a->nextIt = fI;
	a->aList = this;
	fI = a;
	a->index = i;
	a->pred = i == 0 || i == length() ? tail() : it->getAt(i - 1);
}
void
List::removeIt(lstiter *a)
{
	if (fI == a)
		fI = a->nextIt;
	else {
		for (register lstiter* anIt = fI; anIt->nextIt != a;
				anIt = anIt->nextIt) ;
		anIt->nextIt = a->nextIt;
	}
	a->nextIt = 0;
	a->aList = 0;
	a->pred = 0;
	a->index = 0;
}
List::~List()
{
	if (--it->refCount == 0)
		delete it;
	register lstiter*	s;
	for (register lstiter* anIt = fI; anIt; anIt = s) {
		s = anIt->nextIt;
		anIt->nextIt = 0;
		anIt->aList = 0;
		anIt->pred = 0;
		anIt->index = 0;
	}
}
List&
List::operator=(List& ll)
{
	LstHead	*head = ll.it;
	head->refCount++;
	if (--it->refCount == 0)
		delete it;
	it = head;
	for (register lstiter *anIt = fI; anIt; anIt = anIt->nextIt) {
		anIt->index = 0;
		anIt->pred = tail();
	}
	return *this;
}
List&
List::unget(lnk* aLink)
{
	separate();
	it->unget(aLink);
	for (register lstiter *anIt = fI; anIt; anIt = anIt->nextIt) {
		if (anIt->index == 0)
			anIt->pred = aLink;
		anIt->index++;
	}
	return *this;
}
List&
List::put(lnk* aLink)
{
	separate();
	it->put(aLink);
	for (register lstiter *anIt = fI; anIt; anIt = anIt->nextIt)
		if (anIt->index == 0)
			anIt->pred = aLink;
	return *this;
}

lnk*
List::get()
{
	separate();
	lnk*	ans = it->get();
	for (register lstiter *anIt = fI; anIt; anIt = anIt->nextIt) {
		if (anIt->index <= 1)	// 0 in case the List is empty
			anIt->pred = tail();	// tail() returns NULL
		if (anIt->index > 0)
			anIt->index--;
	}
	return ans;
}

lnk*
List::unput()
{
	separate();
	lnk*	ans = it->unput();
	for (register lstiter *anIt = fI; anIt; anIt = anIt->nextIt) {
		if (anIt->pred == ans)	// head or tail
			anIt->pred = tail();
		if (anIt->index > length())
			anIt->index--;
	}
	return ans;
}

lstiter&
lstiter::operator=(lstiter& a)
{
	if (!a.aList)
		abort();	/* invalid list iterator */
	if (aList) aList->removeIt(this);
	a.aList->newIt(this);
	pred = a.pred;
	index = a.index;
	return *this;
}
lstiter::lstiter(lstiter& a)
{
	if (!a.aList)
		abort();	/* invalid list iterator */
	a.aList->newIt(this);
	pred = a.pred;
	index = a.index;
}
lstiter::lstiter(List& a, int i)
{
	a.newIt(this, i);
}
lstiter::~lstiter()
{
	if (aList) aList->removeIt(this);
}
void
lstiter::reset(int i)
{
	index = i;
	pred = i == 0 || i == aList->length() ? aList->tail() :
		aList->it->getAt(i - 1);
}
void
lstiter::insert(lnk *x)
{
	aList->separate();
	if (pred) {
		x->init(pred, pred->nxt);
		pred->nxt->prv = x;
		pred->nxt = x;
		for (register lstiter *other = aList->firstIt(); other;
				other = other->nextIt)
			if (this != other && index < other->index)
				other->index++;
	} else {
		x->init(x, x);
		for (register lstiter *other = aList->firstIt(); other;
				other = other->nextIt)
			other->pred = x;
	}
	if (atEnd())
		aList->it->t = x;
	pred = x;
	index++;
	aList->it->sz++;
}
void
lstiter::r_insert(lnk *x)
{
	aList->separate();
	for (register lstiter *other = aList->firstIt(); other;
			other = other->nextIt)
		if (other != this) {
			if (other->pred == pred)
				other->pred = x;
			if (other->index >= index)
				other->index++;
		}
	if (pred) {
		x->init(pred, pred->nxt);
		pred->nxt->prv = x;
		pred->nxt = x;
	} else {
		x->init(x, x);
		pred = x;
	}
	if (atEnd())
		aList->it->t = x;
	aList->it->sz++;
}
lnk *
lstiter::remove()
{
	if (r_atEnd())
		return NULL;
	aList->separate();
	lnk	*doomed = pred;
	int	oldIndex = index;
	if (pred == pred->nxt) {	// this is only item
		aList->it->t = 0;
		for (register lstiter *anIter = aList->firstIt(); anIter;
				anIter = anIter->nextIt) {
			anIter->pred = 0;
			anIter->index = 0;
		}
	} else {
		if (doomed == aList->it->t)	// deleting tail
			aList->it->t = doomed->prv;
		for (register lstiter *anIter = aList->firstIt(); anIter;
				anIter = anIter->nextIt) {
			if (anIter->pred == doomed)
				anIter->pred = doomed->prv;
			if (anIter->index >= oldIndex)
				anIter->index--;
		}
		pred->nxt = doomed->nxt;
		doomed->nxt->prv = pred;
	}
	aList->it->sz--;
	return doomed;
}
lnk *
lstiter::r_remove()
{
	if (atEnd())
		return NULL;
	aList->separate();
	lnk	*doomed = pred->nxt;
	int	deletingHead = doomed == aList->it->h();
	if (pred == pred->nxt) {	// this is only item
		aList->it->t = 0;
		for (register lstiter *anIter = aList->firstIt(); anIter;
				anIter = anIter->nextIt) {
			anIter->pred = 0;
			anIter->index = 0;
		}
	} else {
		if (doomed == aList->it->t)	// deleting tail
			aList->it->t = doomed->prv;
		for (register lstiter *anIt = aList->firstIt(); anIt;
				anIt = anIt->nextIt) {
			if (anIt->pred == doomed)
				anIt->pred = doomed->prv;
			if (anIt->index > index) anIt->index--;
		}
		pred->nxt = doomed->nxt;
		doomed->nxt->prv = pred;
	}
	aList->it->sz--;
	return doomed;
}
lnk *
lstiter::next()
{
	if (atEnd()) return NULL;
	pred = pred->nxt;
	index++;
	return pred;
}
lnk *
lstiter::r_next()
{
	if (r_atEnd()) return NULL;
	lnk	*op = pred;
	pred = pred->prv;
	index--;
	return op;
}
lnk *
lstiter::r_peek()
{
	if (r_atEnd())
		return NULL;
	return pred;
}
lnk *
lstiter::peek()
{
	if (atEnd())
		return NULL;
	return pred->nxt;
}

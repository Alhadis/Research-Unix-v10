#include <Pool.h>

static const round = 4;		// most stringent alignment, in chars

// offset in chars of the data part of a block
static const blockoff = (sizeof(Block_header)+round-1) & -round;

Block_pool::Block_pool (unsigned n)
{
	sz = n;
	head = 0;
}

Block_pool::~Block_pool()
{
	while (head) {
		Block_header* b = head;
		head = head->next;
		delete (char*) b;
	}
}

char*
Block_pool::expand()
{
	Block_header* b = (Block_header*) new char[size()+blockoff];
	b->next = head;
	head = b;
	return ((char*) b) + blockoff;
}


// largest multiple of q that is <= p
static unsigned
floor (unsigned p, unsigned q)
{
	return p - p % (q + (q==0));
}

// smallest multiple of q that is >= p
static unsigned
ceil (unsigned p, unsigned q)
{
	return floor (p + q - 1, q);
}

Pool::Pool(unsigned n): (ceil (1000, n))
{
	elsize = (n + (n == 0) + round - 1) & -round;
	count = Block_pool::size()/elsize;
	head = 0;
}

void
Pool::grow()
{
	register char* p = expand();
	register int n = count;

	while (--n >= 0) {
		register Pool_element_header* ph = (Pool_element_header*) p;
		ph->next = head;
		head = ph;
		p += elsize;
	}
}

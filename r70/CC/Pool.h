#ifndef _POOL_H
#define _POOL_H 1

class Block_header {
	Block_header* next;
	friend class Block_pool;
};

class Block_pool {
	unsigned sz;
	Block_header* head;
	Block_pool(Block_pool&);		// copy protection
	void operator= (Block_pool&);		// copy protection
public:
	unsigned size() { return sz; }
	Block_pool (unsigned);
	~Block_pool();
	char* expand();
};

class Pool_element_header {
	Pool_element_header* next;
	friend class Pool;
};

class Pool: private Block_pool {
	unsigned elsize, count;
	Pool_element_header* head;
	Pool(Pool&);			// copy protection
	void operator= (Pool&);		// copy protection
	void grow();
public:
	Pool(unsigned n);
	void* alloc() {
		if (!head)
			grow();
		register Pool_element_header* p = head;
		head = p->next;
		return p;
	}
	void free(void* b) {
		register Pool_element_header* p = (Pool_element_header*) b;
		p->next = head;
		head = p;
	}
};

#endif

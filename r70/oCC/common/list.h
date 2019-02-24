
struct link {
	link* next;
	common* p;
	link(common* pp, link* nn) { p=pp; next=nn; }
};

class list : public common {	/* an element can occur more than once */
	link* head;
	link* tail;
public:
	list() { head = tail = 0; }
	list(common& oo) { head = tail = new link(&oo, 0); }
	~list();

	void	add(common& oo)	{
			head = new link(&oo,head);
			if (tail == 0) tail=head;
		}

	void	addlast(common& oo) {
		if (tail==0)
			head = tail = new link(&oo,0);
		else
			tail = tail->next = new link(&oo,0);
	}

	common*	remove() {
		if (head) {
			common* p = head->p;
			if (head == tail)
				head = tail = 0;
			else
				head=head->next;
			return p;
		}
		else
			return 0;
	}

	common* remove(common& oo);
	common* first() { return head?head->p:0; }
	common* last()	{ return tail?tail->p:0; }
	int	empty()	{ return head!=0; }
};

class set : public list {	/* unique elements */
	common	remove();
public:
	set();
	set(common&);
	~set();

	int	empty();
	int	insert(common&);
	common*	remove(common&);
	common*	find(char*);
	common* find(common&);
	int	no_of_mem();
};

struct dlink {
	dlink* pre, *suc;
	common* p;
	dlink(dlink* pr, dlink* su, common* pp) { pre=pr; suc=su; p=pp; }
}; 

class dlist : public common {
	dlink* head;
public:
	dlist();
	dlist(common&);
	~dlist();

	void	insert();
	common*	remove();
	common* remove(common&);
	common* curr();
	common*	next();
	common* prev();
};


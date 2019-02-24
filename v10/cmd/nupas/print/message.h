typedef struct message message;

struct message {
	message *prev;		/* doubly linked list for messages */
	message *next;
	message *extent;	/* singly linked list for range of messages */
	string	*sender;
	string	*date;
	string	*body;
	int pos;
	int size;
	int status;
};

extern message *m_get();
extern int m_print();
extern int read_mbox();
extern int write_mbox();
extern void V();
extern int P();

/* message status */
#define DELETED 1

extern message *mlist,*mlast,*mzero;	/* list of messages */

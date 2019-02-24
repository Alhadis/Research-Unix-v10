typedef struct message message;

struct message {
	string	*sender;
	string	*replyaddr;
	string	*date;
	string	*body;
	int	size;
	int	fd;		/* if >= 0, the file the message is stored in */
};

extern void default_from();
extern message *m_new();
extern message *m_read();
extern int m_print();
extern int m_bprint();
extern int m_free();
extern char *thedate();

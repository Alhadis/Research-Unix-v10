class Line;

class Crossbar
{
	char *dial;
	int nouts;		/* number of inputs/outputs */
	int nwires;		/* number of wires per input/output */
	int fd;
	Crossbar *nextc;
	int nextl, froml;
	int *imap;		/* output i is connected to input imap[i] */
	virtual void startup();
	virtual void config();
	friend class System10;
	friend class Dynasty;
public:
	Crossbar(char *dkname, int n, int depth);
	char *name() { return(dial); }
	void link(int ll, Crossbar *c, int l);
	int map(int input, Crossbar **cp, int *ip);
	virtual int connect(int input, Crossbar *c, int output, int nw);
};

class Dynasty : public Crossbar
{
	void startup();
	void config();
	void tocontrol();
	int get1(unsigned char);
	int getn(int);
	void put1(unsigned char);
	void putn(int);
public:
	Dynasty(char *dkname, int n, int depth);
	int connect(int input, Crossbar *c, int output, int nw);
};

class System10 : public Crossbar
{
	void startup();
	void config();
public:
	System10(char *dkname, int n, int depth);
	int connect(int input, Crossbar *c, int output, int nw);
};

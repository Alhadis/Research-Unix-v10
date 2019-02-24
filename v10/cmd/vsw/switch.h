#define		MAXLABEL	36

class Line
{
	char *name[2];		/* short/verbose */
	Crossbar *c;
	int line;
	char label;
	friend class Switch;
	friend class Rgb;
public:
	Line(char, char *, char *, Crossbar *, int);
	int pr(int n, char *buf, int len);
};

class Switch
{
	char *name;
	int nwires;		/* number of wires per input/output */
	int maxlines;		/* high water mark */
	Line *lines[MAXLABEL][2];
	void add(int inout, char label, char *n1, char *n2, Crossbar *c, int l);
	void addl(int inout, char *n1, char *n2, Crossbar *c, int l);
	int lineofcl(int output, Crossbar *, int);
public:
	Switch(char *nam, int n);
	void addi(char label, char *n1, char *n2, Crossbar *c, int l) { add(0, label, n1, n2, c, l); }
	void addo(char label, char *n1, char *n2, Crossbar *c, int l) { add(1, label, n1, n2, c, l); }
	void reinit();
	void describe();
	void connect(char input, char output);
	void pr(char);
};
extern int labeltoint(char);
extern char inttolabel(int);
extern void blank(int fd, int n);

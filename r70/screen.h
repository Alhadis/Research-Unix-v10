typedef int	(*pftn)();

#define	NULL	0

class backgr
{
	friend	figure;
	friend	screen;

	void	new(void);
	void	delete(void);
	void	draw(class line *);
	char	**lines;
public:
	void	setc(int, int, char);
	void	sets(int, int, char *);
};

struct desc
{
	char	x;
	char	y;
	char	c;
};

class figure
{
	friend		screen;
	friend		field;

	void		new(class screen *, int, struct desc *);
	void		delete(void);
	void		display(void);
	void		update(void);
	class figure	**last;
	class figure	*next;
	class screen	*scr;
	int		active;
	int		f_x;
	int		f_y;
	int		size;
	struct desc	*map;
public:
	void		cursor(int);
	void		add(int, int);
	void		remove(void);
	void		move(int, int);
	void		change(void);
};

class line
{
	friend		screen;

	void		new(void);
	void		delete(void);
	void		clean(void);
	void		fix_lines(void);
	void		redisplay(int, int, int);
	void		fix_chars(class line *, int);
	char		*text;
	int		dirty;
	int		leading;
	int		trailing;
	long		hash;
public:
	void		update_vdu(void);
	void		paint(char *);
	void		clr(void);
	void		addch(int x, char c)
	{
		text[x] = c;
		dirty++;
	};
	char		*vect(void)
	{
		return text;
	};
};

class screen
{
	friend		figure;
	friend		obj;

	void		new(class backgr *);
	void		delete(void);
	class backgr	*bg;
	class figure	*list;
	class line	*image;
public:
	char		what(int x, int y)
	{
		return image[y].vect()[x];
	};
	void		freeze(void);
	void		thaw(void);
	void		update(void);
	void		clear(void);
	void		display(void);
};

class field
{
	void		new(class screen *, int);
	void		delete(void);
	void		make_buff(int);
	void		yank(int, int);
	int		cursor_motion(char, int, int, int);
	char		*contents;
	class figure	*fig;
	int		width;
	struct desc	*fdesc;
public:
	char		*text(void)
	{
		return contents;
	};
	void		add(int x, int y)
	{
		fig->add(x, y);
	};
	void		move(int x, int y)
	{
		fig->move(x, y);
	};
	void		remove(void)
	{
		fig->remove();
	};
	void		display(char *);
	int		read(pftn);
	int		edit(int *);
	void		patch(int, int);
};

extern void	get_cursor(int *, int *);
extern void	move_cursor(int, int);
extern void	screen_init(int, int);
extern void	beep(void);
extern void	redraw(void);
extern void	quit(int);
extern int	reset_tty(void);
extern int	set_tty(int);
extern char	*my_name;

/*
 *	Field defines.
 */
#define	ERROR	(-2)
#define	DONE	(-1)

#define	NIL	0
#define	CHANGED	1
#define	UP	2
#define	DOWN	3
#define	COMMAND	4
#define	EXIT	5

#define	cntrl(c)	('c' & 0x1F)

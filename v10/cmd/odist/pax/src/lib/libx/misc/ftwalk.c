/*
**	int ftwalk(char *path, int (*userf)(), int flags, int (*comparf)());
**
**	Function to walk a file system graph in a depth-first search.
**	Arguments of ftwalk() are:
**
**	path:	path name(s) of the root(s) of the graph to be search.
**
**	userf:	a function to be called at each node that is visited.
**		Called as: int userf(struct FTW* ftw)
**		ftw:	a structure containing info about current node.
**			See ftwalk.h for info on its fields.
**		If func returns non-zero, ftwalk() terminates and returns this
**		value to its caller.
**
**	flags:	a bit vector indicating types of traversal.
**		See ftwalk.h for info on available bit fields.
**
**	comparf: a function to be called to order elements in the same directory.
**		Called as: int (*comparf)(struct FTW *f1, struct FTW *f2)
**		It should return -1, 0, 1 to indicate f1<f2, f1=f2 or f1>f2.
**
**	Written by K-Phong Vo, 11/30/88.
*/

#include	<sys/types.h>
#include	<sys/stat.h>
#include	<dirent.h>
#include	<limits.h>
#include	<errno.h>
#include	<ftwalk.h>

#ifdef BSD
#define memcopy(to,fr,n)	bcopy(fr,to,n)
#else
#define memcopy(to,fr,n)	memcpy(to,fr,n)
#endif

#ifndef PATH_MAX
#define PATH_MAX	1024
#endif

#define reg	register
extern char	*malloc();
extern int	stat();
extern int	chdir();
extern int	errno;
#ifdef S_IFLNK
extern int	lstat();
extern int	lpstat();
#endif

#if MAXNAMLEN > 16
#define MINNAME		24
#else
#define MINNAME		16
#endif

#define Ftw		struct FTW

/* function to determine type of an object */
#ifdef S_IFLNK
#define TYPE(i,m)	(i = (m)&S_IFMT,i == S_IFDIR ? FTW_D : \
					i == S_IFLNK ? FTW_SL : FTW_F)
#else
#define TYPE(i,m)	(((m)&S_IFMT) == S_IFDIR ? FTW_D : FTW_F)
#endif

/* to set state.status and state.base on calls to user function */
#define STATUS(cdrv)	(cdrv == 0 ? FTW_NAME : FTW_PATH)

/* see if same object */
#define SAME(one,two)	((one).st_ino == (two).st_ino && (one).st_dev == (two).st_dev)

/* path name */
#define PATH(p,l)	((l) > 0 && (p)[0] == '.' && (p)[1] == '/' ? (p)+2 : (p))

/*
	Make/free an object of type FTW.
*/
static Ftw	*Free;
#define freeFtw(f)	((f)->link = Free, Free = (f))

static Ftw *newFtw(name,namelen)
reg char	*name;
reg int		namelen;
{
	reg Ftw	*f;
	reg int	amount;

	if(Free && namelen < MINNAME)
		f = Free, Free = f->link;
	else
	{
		amount = namelen < MINNAME ? MINNAME : namelen+1;
		if(!(f = (Ftw*) malloc(sizeof(Ftw)+amount-sizeof(int))))
			return 0;
	}
	f->link = 0;
	f->local.number = 0;
	f->local.pointer = 0;
	f->status = FTW_NAME;
	f->namelen = namelen;
	memcopy(f->name,name,namelen+1);

	return f;
}

static int freeAll(f,rv)
reg Ftw	*f;
reg int	rv;
{
	reg Ftw	*next;
	reg int	freeing;

	for(freeing = 0; freeing < 2; ++freeing)
	{
		if(freeing == 1)
			f = Free, Free = 0;
		while(f)
		{
			next = f->link;
			free((char*)f);
			f = next;
		}
	}
	return rv;
}

/*
	To compare directories by device/inode.
*/
static int statcmp(f1,f2)
reg Ftw	*f1, *f2;
{
	reg int	d;
	if((d = f1->statb.st_ino - f2->statb.st_ino) != 0)
		return d;
	if((d = f1->statb.st_dev - f2->statb.st_dev) != 0)
		return d;
	/* hack for NFS system where (dev,ino) do not uniquely identify objects */
	return (f1->statb.st_mtime - f2->statb.st_mtime);
}

/*
	Search trees with top-down splaying (a la Tarjan and Sleator).
	When used for insertion sort, this implements a stable sort.
*/
#define RROTATE(r)	(t = r->left, r->left = t->right, t->right = r, r = t)
#define LROTATE(r)	(t = r->right, r->right = t->left, t->left = r, r = t)

static Ftw *search(e,root,comparf,insert)
reg Ftw	*e, *root;
int	(*comparf)();
int	insert;		/* always insert */
{
	reg int		cmp;
	reg Ftw		*t, *left, *right, *lroot, *rroot;

	left = right = lroot = rroot = 0;
	while(root)
	{
		if((cmp = (*comparf)(e,root)) == 0 && !insert)
			break;
		if(cmp < 0)
		{	/* this is the left zig-zig case */
			if(root->left && (cmp = (*comparf)(e,root->left)) <= 0)
			{
				RROTATE(root);
				if(cmp == 0 && !insert)
					break;
			}

			/* stick all things > e to the right tree */
			if(right)
				right->left = root;
			else	rroot = root;
			right = root;
			root = root->left;
			right->left = 0;
		}
		else
		{	/* this is the right zig-zig case */
			if(root->right && (cmp = (*comparf)(e,root->right)) >= 0)
			{
				LROTATE(root);
				if(cmp == 0 && !insert)
					break;
			}

			/* stick all things <= e to the left tree */
			if(left)
				left->right = root;
			else	lroot = root;
			left = root;
			root = root->right;
			left->right = 0;
		}
	}

	if(!root)
		root = e;
	else
	{
		if(right)
			right->left = root->right;
		else	rroot = root->right;
		if(left)
			left->right = root->left;
		else	lroot = root->left;
	}

	root->left = lroot;
	root->right = rroot;
	return root;
}

/*
**	Delete the root element from the tree
*/
static Ftw *delete(root)
reg Ftw	*root;
{
	reg Ftw *t, *left, *right;

	left = root->left;
	right = root->right;
	if(!left)
		root = right;
	else
	{
		while(left->right)
			LROTATE(left);
		left->right = right;
		root = left;
	}
	return root;
}

/*
	Convert a binary search tree into a sorted todo (link) list
*/
static void getlist(top,bot,root)
reg Ftw	**top, **bot, *root;
{
	if(root->left)
		getlist(top,bot,root->left);
	if (*top) (*bot)->link = root, *bot = root;
	else *top = *bot = root;
	if(root->right)
		getlist(top,bot,root->right);
}

/*
	Set directory when curdir is lost in space
*/
static int setdir(home,path)
reg char	*home, *path;
{
	reg int	cdrv;

	if(path[0] == '/')
		cdrv = setcwd(path,(char*)0);
	else
	{	/* Note that path and home are in the same buffer */
		path[-1] = '/';
		cdrv = setcwd(home,(char*)0);
		path[-1] = '\0';
	}
	if(cdrv < 0)
		(void) setcwd(home,(char*)0);
	return cdrv;
}

/*
	Set to parent dir
*/
static int setpdir(home,path,base)
reg char	*home, *path, *base;
{
	reg int	cdrv, c;

	if(base > path)
	{
		c = base[0];
		base[0] = '\0';
		cdrv = setdir(home,path);
		base[0] = c;
	}
	else	cdrv = setcwd(home,(char*)0);
	return cdrv;
}

/*
	Pop a set of directories
*/
static int popdirs(n_dir,ftw)
reg int	n_dir;
reg Ftw	*ftw;
{
	struct stat	sb;
	reg char	*s, *endbuf;
	char		buf[PATH_MAX];

	if(!ftw || ftw->level < 0)
		return -1;

	endbuf = buf + (PATH_MAX-4);
	while(n_dir > 0)
	{
		for(s = buf; s < endbuf && n_dir > 0; --n_dir)
			*s++ = '.', *s++ = '.', *s++ = '/';
		*s = '\0';
		if(chdir(buf) < 0)
			return -1;
	}
	if(stat(".",&sb) != 0 || !SAME(sb,ftw->statb))
		return -1;
	return 0;
}

/*
	Get top list of elt to process
*/
static Ftw *toplist(paths,statf,comparf)
reg char	**paths;
int		(*statf)(), (*comparf)();
{
	reg int		i;
	reg char	*path;
	reg Ftw		*f, *root;
	Ftw		*top, *bot;
	reg struct stat	*sb;

	top = bot = root = 0;
	for(; *paths; ++paths)
	{
		path = *paths;
		if(!path[0])
			path = ".";

		/* make elements */
		if(!(f = newFtw(path,strlen(path))))
			break;
		f->level = 0;
		sb = &(f->statb);
		f->info = (*statf)(path,sb) < 0 ? FTW_NS : TYPE(i,sb->st_mode);

		if(comparf)
			root = search(f,root,comparf,1);
		else if(bot)
			bot->link = f, bot = f;
		else	top = bot = f;
	}
	if(comparf)
		getlist(&top,&bot,root);
	return top;
}

/*
	Resize path buffer.
	Note that realloc() is not used because we may need to chdir(home)
	if there isn't enough space to continue
*/
static int resize(home,endbuf,path,base,n_buf,incre)
reg char	**home, **endbuf, **path, **base;
int		n_buf, incre;
{
	reg char	*old, *new;
	reg int		n_old;

	/* add space for "/." used in testing FTW_DNX */
	n_old = n_buf;
	n_buf = ((n_buf+incre+4)/PATH_MAX + 1)*PATH_MAX;
	if(!(new = malloc(n_buf)))
		return -1;

	old = *home;
	*home = new;
	memcopy(new,old,n_old);
	if(endbuf)
		*endbuf = new + n_buf - 4;
	if(path)
		*path = new + (*path - old);
	if(base)
		*base = new + (*base - old);

	free(old);
	return n_buf;
}

/*
	The real thing.
*/
#if __STDC__ || __cplusplus
ftwalk(const char *cpath, int (*userf)(struct FTW*), int flags, int (*comparf)(struct FTW*, struct FTW*))
#else
#ifndef const
#define const
#endif
ftwalk(path,userf,flags,comparf)
char	*path;
int	(*userf)();
int	flags;
int	(*comparf)();
#endif
{
#if __STDC__ || __cplusplus
	char		*path = (char*)cpath;
#endif
	reg int		cdrv;		/* chdir value */
	int		fnrv;		/* return value from user function */
	Ftw		topf,		/* the parent of top elt */
			*todo, *top, *bot;
	DIR		*dirfp;
	int		(*statf)();	/* stat function */
	int		preorder, children, postorder;
	char		*endbuf; /* space to build paths */
	static char	*Home;
	static int	N_buf;

	/* decode the flags */
	children = (flags&FTW_CHILDREN) ? 1 : 0;
	children = (children && (flags&FTW_DELAY)) ? 2 : children;
	preorder = ((flags&FTW_POST) == 0 && !children);
	postorder = (flags&(FTW_POST|FTW_TWICE));
	cdrv = (flags&FTW_DOT) ? 1 : -1;
#ifdef S_IFLNK
	statf = (flags&FTW_PHYSICAL) ? lstat : lpstat;
#else
	statf = stat;
#endif
	/* space for home directory and paths */
	if(!Home)
		N_buf = 2*PATH_MAX;
	while(1)
	{
		if(!Home && !(Home = malloc(N_buf)))
			return -1;
		Home[0] = 0;
		if(cdrv > 0 || getcwd(Home,N_buf))
			break;
		else if(errno == ERANGE)
		{	/* need larger buffer */
			free(Home);
			N_buf += PATH_MAX;
			Home = 0;
		}
		else	cdrv = 1;
	}
	endbuf = Home + N_buf - 4;

	fnrv = -1;

	/* make the list of top elements */
	todo = top = bot = 0;
	if((flags&FTW_MULTIPLE) && path)
		todo = toplist((char**)path,statf,comparf);
	else
	{
		char	*p[2];
		p[0] = path ? path : ".";
		p[1] = 0;
		todo = toplist(p,statf,comparf);
	}

	path = Home + strlen(Home) + 1;
	dirfp = 0;
	while(todo)
	{
		reg int		i, nd;
		reg Ftw		*ftw, *f;
		reg struct stat	*sb;
		reg char	*name, *endbase;
		Ftw		*link, *root, *curdir, *diroot, *dotdot;
		struct dirent	*dir;
		char		*base;
		reg int		level, n_base, nostat, cpname;

		/* process the top object on the stack */
		ftw = todo;
		link = ftw->link;
		sb = &(ftw->statb);
		name = ftw->name;
		level = ftw->level;
		fnrv = -1;
		top = bot = root = 0;

		/* initialize for level 0 */
		if(level == 0)
		{
			/* initialize parent */
			memcopy(&topf,ftw,sizeof(topf));
			topf.level = -1;
			topf.name[0] = '\0';
			topf.path = 0;
			topf.pathlen = topf.namelen = 0;
			topf.parent = 0;
			ftw->parent = &topf;
			
			diroot = 0;
			if(cdrv == 0)
				(void) setcwd(Home,(char*)0);
			else if(cdrv < 0)
				cdrv = 0;
			curdir = cdrv ? 0 : ftw->parent;
			base = path;
			*base = '\0';
		}

		/* chdir to parent dir if asked for */
		if(cdrv < 0)
		{
			cdrv = setdir(Home,path);
			curdir = cdrv ? 0 : ftw->parent;
		}

		/* add object's name to the path */
		if((n_base = ftw->namelen) >= endbuf-base &&
	   	   (N_buf = resize(&Home,&endbuf,&path,&base,N_buf,n_base)) < 0)
			goto done;
		memcopy(base,name,n_base+1);
		name = cdrv ? path : base;

		/* check for cycle and open dir */
		if(ftw->info == FTW_D)
		{
			if((diroot = search(ftw,diroot,statcmp,0)) != ftw)
			{
				ftw->info = FTW_DC;
				ftw->link = diroot;
			}
			else
			{	/* buffer is known to be large enough here! */
				if(base[n_base-1] != '/')
					memcopy(base+n_base,"/.",3);
				if(!(dirfp = opendir(name)))
					ftw->info = FTW_DNX;
				base[n_base] = '\0';
				if(!dirfp && !(dirfp = opendir(name)))
					ftw->info = FTW_DNR;
			}
		}

		/* call user function in preorder */
		nd = ftw->info & ~FTW_DNX;
		if(nd || preorder)
		{
			ftw->status = STATUS(cdrv);
			ftw->link = 0;
			ftw->path = PATH(path,level);
			ftw->pathlen = (base - ftw->path) + n_base;
			fnrv = (*userf)(ftw);
			ftw->link = link;
			if(fnrv)
				goto done;

			/* follow symlink if asked to */
			if(ftw->info == FTW_SL && ftw->status == FTW_FOLLOW)
			{
				ftw->info = stat(name,sb) ? FTW_NS : TYPE(i,sb->st_mode);
				if(ftw->info != FTW_SL)
					continue;
			}
			/* about to prune this ftw and already at home */
			if(cdrv == 0 && level == 0 && nd)
				cdrv = -1;
		}

		/* pruning the search tree */
		if(!dirfp || nd || ftw->status == FTW_SKIP)
		{
			if(dirfp)
				closedir(dirfp), dirfp = 0;
			goto popstack;
		}

		/* FTW_D or FTW_DNX, about to read children */
		if(cdrv == 0)
		{
			if((cdrv = chdir(name)) < 0)
				(void) setcwd(Home,(char*)0);
			curdir = cdrv < 0 ? 0 : ftw;
		}
		nostat = (children > 1 || ftw->info == FTW_DNX);
		cpname = ((cdrv && !nostat) || (!children && !comparf));
		dotdot = 0;
		endbase = base+n_base;
		if(endbase[-1] != '/')
			*endbase++ = '/';

		while(dir = readdir(dirfp))
		{
			if(dir->d_ino == 0)
				continue;
			name = dir->d_name;
			nd = 0;
			if(name[0] == '.')
			{
				if(name[1] == '\0')
					nd = 1;
				else if(name[1] == '.' && name[2] == '\0')
					nd = 2;
			}
			if(!children && nd > 0)
				continue;

			/* make a new entry */
			fnrv = -1;
			if(!(f = newFtw(name,i = DIRNAMLEN(dir))))
				goto done;
			f->parent = ftw;
			f->level = level+1;
			sb = &(f->statb);

			/* check for space */
			if(i >= endbuf-endbase)
			{
	   	   		N_buf = resize(&Home,&endbuf,&path,&base,N_buf,i);
				if(N_buf < 0)
					goto done;
				endbase = base+n_base;
				if(endbase[-1] != '/')
					++endbase;
			}
			if(cpname)
			{
				memcopy(endbase,name,i+1);
				if(cdrv)
					name = path;
			}

			if(nd == 1)
			{
				f->info = FTW_D;
				memcopy(sb,&(ftw->statb),sizeof(struct stat));
			}
			else if(nostat || (*statf)(name,sb))
			{
				f->info = FTW_NS;
				sb->st_ino = dir->d_ino;
			}
			else	f->info = TYPE(i,sb->st_mode);

			if(nd)
			{	/* don't recurse on . and .. */
				f->status = FTW_SKIP;
				if(nd == 2 && f->info != FTW_NS)
					dotdot = f;
			}

			if(comparf) /* object ordering */
				root = search(f,root,comparf,1);
			else if(children || f->info == FTW_D || f->info == FTW_SL)
				top ? (bot->link = f, bot = f) : (top = bot = f);
			else
			{	/* terminal node */
				f->status = STATUS(cdrv);
				f->path = PATH(path,1);
				f->pathlen = endbase - f->path + f->namelen;
				fnrv = (*userf)(f);
				freeFtw(f);
				if(fnrv)
					goto done;
			}
		}

		/* done with directory reading */
		closedir(dirfp), dirfp = 0;

		if(root)
			getlist(&top,&bot,root);

		/* delay preorder with the children list */
		if(children)
		{	/* try moving back to parent dir */
			base[n_base] = '\0';
			if(cdrv <= 0)
			{
				f = ftw->parent;
				if(cdrv < 0 || curdir != ftw || !dotdot ||
			   	   !SAME(f->statb,dotdot->statb) ||
				   (cdrv = chdir("..")) < 0)
					cdrv = setpdir(Home,path,base);
				curdir = cdrv ? 0 : f;
			}

			ftw->link = top;
			ftw->path = PATH(path,level);
			ftw->pathlen = (base - ftw->path) + ftw->namelen;
			ftw->status = STATUS(cdrv);
			fnrv = (*userf)(ftw);
			ftw->link = link;
			if(fnrv)
				goto done;

			/* chdir down again */
			nd = (ftw->status == FTW_SKIP);
			if(!nd && cdrv == 0)
			{
				if((cdrv = chdir(base)) < 0)
					(void) setcwd(Home,(char*)0);
				curdir = cdrv ? 0 : ftw;
			}

			/* prune */
			if(base[n_base-1] != '/')
				base[n_base] = '/';
			for(bot = 0, f = top; f; )
			{
				if(nd || f->status == FTW_SKIP)
				{
					if(bot)
						bot->link = f->link;
					else	top = f->link;
					freeFtw(f);
					f = bot ? bot->link : top;
					continue;
				}

				if(children > 1 && ftw->info != FTW_DNX)
				{	/* now read stat buffer */
					sb = &(f->statb);
					if(f->status == FTW_STAT)
						f->info = TYPE(i,sb->st_mode);
					else
					{
						name = f->name;
						if(cdrv)
						{
							memcopy(endbase,
								name,f->namelen+1);
							name = path;
						}
						if((*statf)(name,sb) == 0)
							f->info = TYPE(i,sb->st_mode);
					}
				}

				/* normal continue */
				bot = f, f = f->link;
			}
		}

		base[n_base] = '\0';
		if(top)
			bot->link = todo, todo = top, top = 0;

		/* pop objects completely processed */
	popstack:
		nd = 0;	/* count number of ".." */
		while(todo && ftw == todo)
		{
			f = ftw->parent;
			if(ftw->info & FTW_DNX)
			{
				if(curdir == ftw)	/* ASSERT(cdrv == 0) */
				{
					nd += 1;
					curdir = f;
				}

				/* perform post-order processing */
				if(postorder &&
				   ftw->status != FTW_SKIP && ftw->status != FTW_NOPOST)
				{	/* move to parent dir */
					if(nd > 0)
					{
						cdrv = popdirs(nd,curdir);
						nd = 0;
					}
					if(cdrv < 0)
						cdrv = setpdir(Home,path,base);
					curdir = cdrv ? 0 : f;
	
					ftw->info = FTW_DP;
					ftw->path = PATH(path,ftw->level);
					ftw->pathlen = (base - ftw->path) + ftw->namelen;
					ftw->status = STATUS(cdrv);
					link = ftw->link;
					ftw->link = 0;
					fnrv = (*userf)(ftw);
					ftw->link = link;
					if(fnrv)
						goto done;
					if(ftw->status == FTW_AGAIN)
						ftw->info = FTW_D;
				}

				/* delete from dev/ino tree */
				if(diroot != ftw)
					diroot = search(ftw,diroot,statcmp,0);
				diroot = delete(diroot);
			}

			/* reset base */
			if(base > path+f->namelen)
				--base;
			*base = '\0';
			base -= f->namelen;

			/* delete from top of stack */
			if(ftw->status != FTW_AGAIN)
			{
				todo = todo->link;
				freeFtw(ftw);
			}
			ftw = f;
		}

		/* reset current directory */
		if(nd > 0 && popdirs(nd,curdir) < 0)
		{
			(void) setcwd(Home,(char*)0);
			curdir = 0;
			cdrv = -1;
		}

		if(todo)
		{
			if(*base)
				base += ftw->namelen;
			if(*(base-1) != '/')
				*base++ = '/';
			*base = '\0';
		}
	}

	/* normal ending */
	fnrv = 0;

done:
	if(dirfp)
		closedir(dirfp);
	if(cdrv == 0)
		(void) setcwd(Home,(char*)0);
	if(top)
		bot->link = todo, todo = top;
	return freeAll(todo,fnrv);
}

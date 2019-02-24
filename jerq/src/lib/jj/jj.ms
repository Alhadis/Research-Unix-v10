.hw rect\%angle
.if n .po 5
.de Po
.if t \&\f(CW\\$1\f1\\$2
.if n \&\fI\\$1\f1\\$2
..
.de Ja
.ne 6
.sp
.in 0
.ft CW
.ps 12
.br
.if t .BX \\$1     
.if n \\$1     
.ps 10
.ft 1
\h'4m'\\$2
.in +3
.sp
.ft CW
..
.de Jb
.sp
.ft 1
..
.SH
A Library of Goo for the 5620
.PP
In the following summaries,
all coordinates are screen or bitmap coordinates.
The library is
.Po /usr/jerq/lib/libjj.a
which can be accessed by including a
.Po -ljj
in your compilation.
.sp
.PP
.in 0
.ft 1
.Ja 3d "3d: scaled integer three dimensional geometry
.nf
#define	ONE	16384
#define	XAXIS	0
#define	YAXIS	1
#define	ZAXIS	2
typedef short fract;	/* fixed point scaled by ONE */
typedef fract matrix[4][4];
typedef short coord[4];
typedef struct point3
{
	fract x, y, z, w;
} Hcoord;
.fi 
.Jb
These are the data structures for a 3 dimensional geometry package.
.Po Hcoord
are homogeneous coordinates.
Author (for all the 3d stuff): Tom Duff.
.Ja Clip "Clip: clipped jerq primitives
.nf
Clip(r) Rectangle r;
Cbitblt(sb, sr, db, dp, fc) Bitmap *sb, *db; Rectangle sr; Point dp;
Crectf(bp, r, fc) Bitmap *bp; Rectangle r;
Point Cstring(f, s, bp, p, fc) Font *f; char *s; Bitmap *bp; Point p;
Ctexture(bp, r, t, fc) Bitmap *bp; Rectangle r; Texture *t;
.fi 
.Jb
.Po Clip
sets the clipping rectangle for the routines
.Po Cbitblt ,
.Po Crectf ,
.Po Cstring
and
.Po Ctexture .
These routines implicitly clip to the layer as well.
.Po Cstring
returns the same result as
.Po string ,
that is, the result is not clipped.
Author: Andrew Hume.
.Ja NMenu "NMenu: extended menu
.nf
typedef struct NMitem
{
	char	*text;		/* text in menu */
	char	*help;		/* optional help text */
	struct NMenu *next;	/* cascade to */
	void	(*dfn)(), (*bfn)(), (*hfn)();
} NMitem;
typedef struct NMenu
{
	NMitem	*item;			/* string array, ending with text=0 */
	NMitem	*(*generator)();	/* used if item == 0 */
	/* and some other private data */
} NMenu;
.fi
.Jb
A
.Po NMenu
is used by
.Po mhit
described below.
Null or missing values should be zero.
The help text should be less than 40 characters or so.
The generator function is called with an integer argument.
The end of the menu is indicated by a zero \f(CWtext\fP field.
The function
.Po dfn
is called just before the new menu is invoked.
The function
.Po bfn
is called just after this menu is finished.
The function
.Po hfn
is invoked just after the selection is made and before
.Po bfn .
All three functions are called with one argument,
a pointer to the
.Po NMitem
concerned.
.Po Mhit
returns a pointer to the NMitem that was selected or
.Po (NMitem *)0 .
The above definitions are available in
.Po /usr/jerq/include/menu.h .
.Ja confirm "confirm: confirm a decision
confirm(but)
.Jb
.Po confirm
waits for all buttons to be released,
changes the cursor to an icon indicating which button to push for an affirmative
reponse,
waits for a button push and returns the equality between the button push
and the argument
.Po but .
Author: Adrian Freed.
.Ja icos "icos, isin, isqrt: accurate scaled functions
.nf
fract icos(x) angle(x);
fract isin(x) angle x;
long isqrt(x) long x;
.fi
.Jb
.Po icos
and
.Po isin
return a scaled value of their (scaled) argument.
See
.Po 3d
above.
.Ja init3 "init3: initialise the 3d geometry world
init3(v, d, s) Bitmap *v;
.Jb
.Po init3
initialises the matrix stack
(see
.Po push3  below).
.Po v
is the Bitmap for the image.
.Po d/s
is the distance to the screen whose half-width is 1.
.Ja kbdstr "kbdstr: enter a string from the keyboard
kbdstr(s) char *s;
.Jb
.Po kbdstr
displays the given text in a box 300 pixels long and allows the user to
type after the prompt.
The erase and kill characters are
.Po BS
and
.Po @
respectively.
Author: Andrew Hume.
.Ja mhit "mhit: extended analog to menuhit
NMitem *mhit(m, but, 0) NMenu *m;
.Jb
.Po mhit
supports cascading menus with help.
Menus may be a static list of
.Po MNitem
or a generating function.
.Po mhit
may be called with a button argument of 2 or 3 only.
Button 1 causes the help message associated with the currently
highlighted entry to be displayed.
The menu items (and help messages) scroll.
Author: Andrew Hume.
.Ja polyture "polyture: texture a polygon
.nf
polyture(bp, pts, n, t, c) Bitmap *bp; Point *pts; Texture *t; Code c;
.fi
.Jb
.Po Polyture
textures with mode
.Po c
the polygon bounded by and including the
.Po n
vertices specified by
.Po pts .
The list should be closed, i.e.
.Po pts[0]==pts[n-1] .
Author: Andrew Hume.
.Ja push3 "push3, pop3: manipulate the 3d matrix stack
.nf
push3()
pop()
.fi
.Jb
.Po push3
pushes a copy of the top of the matrix stack onto the matrix stack.
.Po pop3
throws away the top of the matrix stack.
.Ja rot3 "rot3, rosc3: rotate a matrix
.nf
rot3(theta, axis) angle theta;
rotsc3(s, c, axis) short s, c;
.fi
.Jb
These routines rotate the top of the matrix stack about the specified axis.
.Po rot3(theta,\ axis)
is identical to
.Po rotsc3(isin(theta),\ icos(theta),\ axis) .
.Ja todo "3d stuff to be done
.nf
Hcoord hcoord(x,y,z,w) fract x, y, z, w;
scale3(p) Hcoord p;
tran3(p) Hcoord p;
ident(m) matrix m;
xform3(m) matrix m;
long dot(a, b) Hcoord a, b;
Hcoord unitize(x) Hcoord(x);
Hcoord cross(a, b) Hcoord a, b;
look3(e, l, u) Hcoord e, l, u;
move3(p) Hcoord p;
line3(p) Hcoord p;
.fi
.Jb
These have yet to be documented.

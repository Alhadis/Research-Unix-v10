.so ../ADM/mac
.XX ideal 79 "Ideal \(em A Picture-specification Language"
...\" Thu May 1 10:28:25 EDT 1986
...\"refer first
...TR 103
...RP 80-11272-13 11173 39199-11
...ND December 17, 1981
.hy 14		\" set hyphenation: 2=not last lines; 4= no -xx; 8=no xx-
.ds id \f2ideal\fP
.ds [. [
.ds .] ]
.nr DV 0		\" for .P1/.P2
.de Ks
.SP
.KS
..
.de Ke
.KE
.SP
..
.de IS			\" do not touch these!
.br
.nf
..
.de IE
..
.de Ts
.nr PS -1
.nr VS -1
.QS
..
.de Te
.nr PS +1
.nr VS +1
.QE
..
.EQ
delim $$
.EN
......graphics
.TL
Ideal \(em A Picture-specification Language
.br
User and Reference Manual\(dg
.AU "MH 2C-518" 5292
Christopher J. Van Wyk
.AI
.MH
.AB
This manual describes how to use the existing implementation of \*(id,
a programming language to be used for describing
pictures.
The main use of \*(id is as a preprocessor to
.I troff ,
so that pictures and text may reside in the same
file and be typeset together.
.I Ideal
can also produce device-independent descriptions
of pictures which, for example, can be displayed through
the
.UX
system plot filters.
.AE
.FS
\(dg This is a revised version of |reference(van wyk ideal cstr).
.FE
.NH 1
Introduction
.PP
.I Ideal
is a language for describing pictures.
It is intended primarily to operate as a
preprocessor to
.I troff |reference(troff latest reference),
much like
.I eqn |reference(latest eqn),
.I tbl |reference(latest tbl)
and
.I pic |reference(latest pic).
.PP
I have explained the principles that motivate
the form of \*(id elsewhere|reference(ideal thesis)|reference(ideal acm).
This document describes how to use
the existing implementation of \*(id
and treats several examples in depth.
.Ts
Paragraphs like this that appear in smaller
type may be skipped on first reading:
they present sidelights that
may be ignored safely by beginners.
.Te
.NH 1
Overview of \*(id
.PP
To take advantage of \*(id's capabilities,
you must believe that
.ce 1
.I "complex numbers are good" .
.IP \(bu
Complex numbers have a natural correspondence
to points in the Cartesian ($x-y$) coordinate
system.
.IP \(bu
Using complex numbers obviates the need for
distinguishing between ``points'' and ``dimensions.''
.IP \(bu
Complex numbers capture readily such common
operations as translation, rotation, and
reflection in the plane.
.LP
.I Ideal
programs define pictures by means of
a system of simultaneous equations in the
significant points of the picture and
a set of drawing instructions to be
carried out with respect to those points.
.I Ideal
solves the system of equations,
then draws the picture using the points
so determined.
.PP
All variables in \*(id programs are complex
numbers, with the usual operations:
.IP \(bu
component-wise addition and subtraction:
$(a,b) ~ +- ~ (c,d) ~ = ~ (a +- c , b +- d)$
.IP \(bu
vector multiplication:
$(a,b)*(c,d) ~ = ~ (ac - bd , ad + bc)$
.IP \(bu
vector division:
$(a,b)/(c,d) ~ = ~ (ac + bd , -ad + bc)/( c sup 2 + d sup 2 )$
.IP \(bu
component manipulation:
$roman "re" ((a,b)) ~ = ~ a$,
$roman "im" ((a,b)) ~ = ~ b$,
and
$roman "conj" ((a,b)) ~ = ~ (a,-b)$
.IP \(bu
vector magnitude calculation:
$roman "abs" ((a,b)) ~ = ~ sqrt { { a sup 2 } + { b sup 2 } }$
.IP \(bu
unit vector function:
$roman "cis" ( theta ) ~ = ~ cos theta + i sin theta$
.LP
A non-standard notation that has proved useful is
$alpha [ x , y ]$, for complex $x$ and $y$,
defined by $x + alpha ( y - x )$, and
meaning ``$alpha$ of the way from $x$ to $y$.''
.PP
Scalars are treated as vectors with null imaginary component.
For example,
.CW 1
is treated as
.CW (1,0) .
.PP
The scale of the coordinate system in which \*(id
programs are written is entirely a matter
of convenience.
The \*(id processor proper
produces output in the same coordinate
system as the input.
Postprocessors (``filters'') scale this coordinate
system to make sense for the device on which the
picture is displayed.
.PP
Some of the pictures below include captions keyed
to the associated programs.
Some of the labels are not produced by the program:
I added them to help explain the picture.
Such labels are parenthesized.
For pictures and programs that have not been
labeled, you may find that labeling them yourself
helps you understand the program.
.NH 1
Elements of the \*(id Language
.PP
This section presents statements that make up the
fundamental units of \*(id programs, so the
displayed program text represents fragments of
complete programs.
Text that appears between
.CW /*
and
.CW */
is a comment.
.NH 2
Boxes
.PP
The building blocks of \*(id programs are called
.I boxes ,
which readers familiar with programming may think
of as procedures or subroutines.
In fact,
the picture drawn by an \*(id program must itself
be a box,
called
.I main ;
we suppress this extra level
of box-nesting for all of Section 3.
.NH 3
Defining Boxes
.PP
Here is a simple box
and one instance of it:
.Ks
.IS
...libfile rect
...width 0.5
...colwid 8.0
main {
	put A:rect {
		ht = 0.5;
		wd = 1;
		sw = 0;
	};
	right '(sw) ' at A.sw;
	right '(nw) ' at A.nw;
	left ' (se)' at A.se;
	left ' (ne)' at A.ne;
}
.IF
.P1
rect {
	var ne, nw, se, sw,
		wd, ht;
	nw = sw + (0,1)*ht;
	ne = nw + wd;
	se = sw + wd;
	conn ne to nw to sw to se to ne;
}
.P2
.Ke
.LP
It is called
.I rect ,
has six local complex variables:
.I ne ,
.I nw ,
.I se ,
.I sw ,
.I wd ,
and
.I ht ,
three equations among these variables,
and an instruction to draw four lines.
.NH 3
Placing Boxes
.PP
To ask for an instance of
.I rect ,
we use a
.I \fIput\fP statement with a
.I "parameter section"
containing enough additional
equations that the local variables of
this instance of
.I rect
are determined uniquely.
For example, we might give the dimensions
.I ht "" (
and
.I wd )
and one of the corners
(say
.I sw ):
.Ks
.IS
...width 0.25
main {
	put rect {
		sw = 0;
		ht = 1;
		wd = 0.5;
	};
}
.IF
.P1
put rect {
	ht = 2;
	wd = 1;
	sw = 0;
};
.P2
.Ke
Any of the following \fIput\fP statements would draw the same rectangle:
(C programmers will recognize the
.CW /*
and
.CW */
comment brackets.)
.P1
/* giving one corner, one dimension
   and a relation on the dimensions */
put rect {
	ht = 2;
	wd = 0.5*ht;
	nw = (0,2);
};
.P2
.P1
/* giving two adjacent corners
   and the perpendicular dimension */
put rect {
	nw = (0,2);
	sw = 0;
	wd = 1;
};
.P2
.P1
/* giving two diagonal corners
   and a relation on the dimensions */
put rect {
	ne = (1,2);
	sw = 0;
	wd = 0.5*ht;
};
.P2
.P1
/* giving three corners */
put rect {
	ne = (1,2);
	nw = (0,2);
	se = 1;
};
.P2
.P1
/* giving the center of a side,
   a corner, and another dimension */
put rect {
	(nw+sw)/2 = (0,1);
	nw = (0,2);
	wd = 1;
};
.P2
.LP
The \fIput\fP statement is to \*(id what a procedure
call is to a conventional programming language.
The difference is that none of the variables
of a box must be specified to be a parameter
whose value is expected in any call:
any of the box's variables may be set by the
\fIput\fP statement, and \*(id will determine the
rest by means of the equations
in the definition of the box.
This means we can give whatever
information we know about this instance of the box,
as long as it is enough to determine everything uniquely.
This mechanism is useful because we often want to
set down a rectangle by giving one of its corners,
but not necessarily the same corner each time;
in a conventional programming language, we would
need to provide a different procedure for each
corner, and the code to solve for the other
corners from it.
(Almost certainly, not all of the \fIput\fP statements
above are equally useful; but it is good to be
able to use any of them when the need arises.)
.Ts
Here is how \*(id solves the system of equations
implicit in a program:
First, all equations are placed on a queue.
Every time a box is called, the solutions
to these equations may\(emand probably will\(embe
different, so all the equations of a box
are enqueued separately for each time that
box is put.
(Of course, different copies of the equation
refer to different copies of the variable,
but \*(id keeps that straight.)
During processing, \*(id maintains two classes
of variables: dependent and independent.
Each dependent variable is represented as a
linear combination of independent variables
plus a constant term.
(Variables whose values are known are a trivial
case of dependent variables.)
All variables start out independent.
As long as there are equations on the queue, \*(id
examines the head equation:
if, after substituting for all dependent variables,
the equation is linear, \*(id determines new
information from it if possible\(emthat is, \*(id
tries to make one variable dependent
on the others, thus reducing the number of
independent variables\(emor decides
whether it is redundant or inconsistent;
if the equation remains non-linear after substitution, \*(id
adds it to the end of the queue and proceeds.
If \*(id ever goes through the whole queue without
discovering any new information, the system cannot
be solved
(by \*(id, anyway)
and \*(id complains bitterly.
If there are any independent variables left after
this processing, \*(id will complain too, because
there is no way they can become known.
.Te
.PP
The variables
.I wd
and
.I ht
above are complex numbers just like the corners,
so we can rotate the rectangle
by giving them complex non-real values:
.Ks
.IS
...width 0.5
main {
	put rect {
		sw = 0;
		wd = (1,1)/abs((1,1));
		ht = 2*wd;
	};
}
.IF
.P1
put rect {
	sw = 0;
	wd = (1,1)/abs((1,1));
	ht = 2*wd;
};
.P2
.Ke
.LP
(The double parentheses are needed in
.CW abs((1,1))
because
.CW abs(1,1)
is parsed as a function with two arguments.)
One point about this example often confuses new users:
the vectors
.I wd
and
.I ht
point in the
.I same
direction.
It is in the definition of
.I rect
that the $ht$ vector is rotated ninety degrees
and added to the southern points to arrive
at the northern points.
Thus, if we give a
.I ht
that is perpendicular to
.I wd ,
we get a very flat rectangle.
.PP
On the other hand, the definition of
.I rect
does not assure that
.I ht
and
.I wd
will point in the same direction.
This \fIput\fP statement
draws a parallelogram:
.Ks
.IS
...width 0.25
main {
	put rect {
		ht = 1;
		wd = (1,1)/abs((1,1));
		sw = 0;
	};
}
.IF
.P1
put rect {
	ht = 1;
	wd = (1,1)/abs((1,1));
	sw = 0;
};
.P2
.Ke
.LP
Some people who feel that
a box called
.I rect
should draw only rectangles
are disturbed by this example.
One remedy is to add another equation
to the definition of
.I rect ,
asserting that two adjacent sides
are perpendicular; \*(id will complain if this equation
is not satisfied
(although it won't stop drawing).
One such equation is
.P1
wd/abs(wd) = ht/abs(ht);
.P2
.PP
Here is a definition for box
.I arrow
that keeps the head of the arrow symmetrical
about its shaft:
.Ks
.IS
arrow {
	var hd, tl, head, wing;
	head = 0.2;
	wing = head*(tl-hd)/abs(tl-hd);
	conn hd to tl;
	conn hd + cis(25)*wing to hd to hd + cis(-25)*wing;
}
main {
	put arrow {
		hd = 0;
		tl = (1,1);
	};
}
.IF
.P1
arrow {
	var hd, tl, head, wing;
	head = 0.1;
	wing = head*(tl-hd)/abs(tl-hd);
	conn hd to tl;
	conn hd + cis(25)*wing to hd to hd + cis(-25)*wing;
}
.P2
.Ke
.LP
Note the definition of
.I wing
in the example above:
the second part of the expression is a unit vector
that points from
.I tl
to
.I hd ;
this is multiplied by
.I head ,
the length of the ``wings'' on the arrowhead.
.Ts
When a \fIput\fP statement is interpreted, the equations in
its parameter section are processed before
the equations in its definition.
Thus, if there are inconsistent equations
between the \fIput\fP statement
and the box definition, the equations in
the \fIput\fP statement take precedence.
This can be useful to provide
default values for variables of a box.
For instance, if the definition of
.I rect
gave such default values to
.I ht
and
.I wd ,
they would take effect unless overridden
by equations in the parameter section
of the \fIput\fP statement.
.Te
.Ts
.I Ideal
ignores inconsistent equations, but it
does generate error messages about them.
To avoid this error message
about a particular equation, use a
tilde
.CW ~
instead of an equals
sign in the equation.
The tilde does
.I not
give the equation any lower ``priority''
than an equation with an equals sign:
all it does is shut off the error message.
So, the two ordered systems
.CW "x = 1" ,
.CW "x ~ 2"
and
.CW "x ~ 2" ,
.CW "x = 1"
are
.I different .
In the former, $x$ receives the value 1, and no error
message appears when the second equation is processed;
in the latter, $x$ receives the value 2, and an error
is generated when the second equation is encountered.
.Te
.NH 2
Special Boxes\(emCircles and Arcs
.PP
Boxes to draw circles and circular arcs are defined
in special library files.
.NH 3
Circles
.PP
The box named
.I circle
has five local variables:
.I center ,
.I radius ,
.I z1 ,
.I z2 ,
and
.I z3 .
The last three may be any points on the circle.
As above, we must give enough information to
determine a circle;
giving two points on it is insufficient, and
giving three collinear points is inconsistent.
Here are three ways to draw
a circle of radius one centered at the origin.
.Ks
.IS
...width 0.5
...libfile circle
dot {
	var center;
	center = ZAP.center;
	put ZAP:circle {
		radius = 0.1;
	};
}
main {
	put A:circle {
		center = 0;
		radius = 1;
	};
	left ' (z1)' at cis(0);
	right '(z2) ' at cis(135);
	right '(z3)  ' at cis(245);
	put dot {center = cis(0);};
	put dot {center = cis(135);};
	put dot {center = cis(245);};
}
.IF
.P1
/* giving center and radius */
put circle {
	center = 0;
	radius = 1;
};
.P2
.Ke
.P1
/* giving center and a point on the circle */
put circle {
	center = 0;
	z1 = 1; /* could also have given z2 or z3 */
};
.P2
.P1
/* giving three points on the circle */
put circle {
	z1 = (1,0);
	z2 = cis(135);
	z3 = cis(235);
};
.P2
.LP
Once the circle has been determined, all five of its
internal variables are known.
So, if we ask for a circle giving three points, the
radius and center will be known afterward.
On the other hand, if we ask for a circle by giving
the center and the radius, the three points $z1$, $z2$,
and $z3$, will be known, and will be on the circle,
but there is no guarantee where they will be.
.NH 3
Arcs
.PP
Box
.I arc
has eight local variables:
.I center ,
.I radius ,
.I start ,
.I midway ,
.I end ,
.I startang ,
.I midang ,
and
.I endang .
It is an arc centered at point
.I center
with radius
.I radius ,
starting at point
.I start
at an angle
.I startang ,
passing through point
.I midway
at an angle
.I midang ,
and ending at point
.I end
at an angle
.I endang .
(All angles are measured with respect to $center$,
in degrees, with the positive $x$-axis taken to be
zero degrees, and the counterclockwise direction to
be positive.)
Note that $midway$ is
.I not
necessarily the midpoint of the arc!
If neither
.I midway
nor
.I midang
is given, the arc is drawn
counterclockwise from
.I start
to
.I end .
Once again, a variety of \fIput\fP statements draw the same arc:
.Ks
.IS
...libfile arc
main {
	put A:arc {
		start = 1;
		end = cis(235);
		center = 0;
	};
	left ' (start)' at A.start;
	right '(midway) ' at cis(180);
	left ' (end)' at A.end;
	put dot {center = A.start;};
	put dot {center = cis(180);};
	put dot {center = A.end;};
}
.IF
.P1
/* giving center, radius, and
   starting and ending angles */
put arc {
	center = 0;
	radius = 1;
	startang = 0;
	endang = 235;
};
.P2
.Ke
.P1
/* giving center, starting point,
   and ending angle */
put arc {
	center = 0;
	start = 1;
	endang = 235;
};
.P2
.Ks
.IS
main {
	put A:arc {
		start = 1;
		end = cis(235);
		center = 0;
	};
	left ' (end)' at A.start;
	right '(midway) ' at cis(180);
	left ' (start)' at A.end;
	put dot {center = A.start;};
	put dot {center = cis(180);};
	put dot {center = A.end;};
}
.IF
.P1
/* giving three points on the arc */
put arc {
	start = cis(235);
	midway = -1;
	end = 0;
};
.P2
.Ke
.NH 2
Other Elements of \*(id Pictures
.PP
.I Ideal
pictures also may contain text and splines.
.NH 3
Text Captions
.PP
There are three commands to place text with respect to a point.
The
.I left
command left-justifies the text with respect to the specified location:
the text will start there.
The
.I right
command right-justifies the text so that it ends at the specified
location.
The default is to center the text at the point.
(The arrows in this picture point to the locations of the named points.)
.Ks
.P1
left "some text" at x;
.P2
.P1
\&"some centered text" at y;
.P2
.P1
right "some other text" at z;
.P2
.IS
...colwid 6.0
...width 4.0
arrow {
	var tl, hd, head, perp, headang;
	conn tl to hd;
	perp = head*(tl-hd)/abs(tl-hd);
	conn hd + cis(headang)*perp to hd to hd + cis(-headang)*perp;
	head ~ 0.1;
        headang ~ 25;
	var start, end;
	start = tl;
	end = hd;
}
main {
	var x, y, z;
	var offset;
	offset = (1,-1);
	x = 0;
	left 'some text' at x;
	put arrow {
		start = x + offset;
		end = x;
		left ' (x)' at start;
	};
	y = 3;
	'some centered text' at y;
	put arrow {
		start = y + offset;
		end = y;
		left ' (y)' at start;
	};
	z = 6;
	right 'some other text' at z;
	put arrow {
		start = z + offset;
		end = z;
		left ' (z)' at start;
	};
}
.IE
.Ke
.LP
To include a double quote mark in a string, escape it with a back-slash.
If you have a line running through a point at which you place some text,
you may want to add a space to one end of the text so that the line
doesn't chop through the text.
The string may include
.I troff
special characters (like
.CW \e(bu )
and commands to other preprocessors (notably
.I eqn ),
but \*(id should be run before any other preprocessors.
.NH 3
Splines
.PP
.I Ideal
provides quadratic splines that are drawn with a
B-spline basis:
the user supplies a sequence of guiding points, and \*(id
draws a smooth curve that is tangent to the polygonal
path they define at the midpoint of each segment;
the spline also starts at the first point and ends at the last.
.Ks
.IS
...colwid 8.0
...width 1.0
...maxx 2.0
...minx -1.0
...maxy 2.0
...miny 0.0
main {
	var x, y, z, w;
	x = 0;
	y = 1;
	z = (-1,1);
	w = (2,2);
	spline x to y to z to w;
	right '(x) ' at x;
	left ' (y)' at y;
	right '(z) ' at z;
	left ' (w)' at w;
	put dot {center = x;};
	put dot {center = y;};
	put dot {center = z;};
	put dot {center = w;};
}
.IF
.P1
spline x to y to z to w;


.P2
.Ke
.NH 1
Putting Boxes Together
.PP
In this section we present several complete \*(id
programs to show how to build up
pictures from boxes.
.NH 2
Naming Instances of Boxes
.PP
Most pictures involve more than placements of simple boxes.
And when pictures involve several boxes, certain geometrical
relationships should exist among them.
We can refer to the local variables of a box that has been put\(emall
we need to do is name the \fIput\fP statement.
Here is a simple example showing a diagram that could be used to illustrate
Pythagoras's Theorem for isoceles triangles:
.Ks
.IS
...libfile rect
...width 1.0
main {
	put first: rect {
		sw = 0;
		ht = wd = 1;
	};
	put next: rect {
		nw = first.se;
		ht = wd = first.ht;
	};
	put last: rect {
		sw = first.ne;
		se = next.ne;
		ht = wd;
	};
}
.IF
.P1
main {
	put first: rect {
		sw = 0;
		ht = wd = 1;
	};
	put next: rect {
		nw = first.se;
		ht = wd = first.ht;
	};
	put last: rect {
		sw = first.ne;
		se = next.ne;
		ht = wd;
	};
}
.P2
.Ke
.LP
First we place an instance of
.I rect
called
.I first .
Then we place another
.I rect
with its upper left corner ($ne$)
at the lower right of
.I first .
Finally, we draw a third square,
two of whose adjacent points are
identified with points on the first
two squares placed.
We could have used any two adjacent points on
.I last :
if we had placed
$last.ne$ at $first.ne$
and $last.nw$ at $next.ne$, \*(id
would have figured all the relationships
out right, although the program might be quite
inscrutable to humans.
.PP
How can we circumscribe a triangle?
If we name the \fIput\fP statement that produces
the instance, we can just give its three
vertices as values for $z1$, $z2$, and $z3$
in an instance of
.I circle .
.Ks
.IS
...libfile circle
triangle {
	var z1, z2, z3;
	conn z1 to z2 to z3 to z1;
}
main {
	put T: triangle {
		z1 = 0;
		z2 = 1;
		z3 = (2,2);
	};
	put circle {
		z1 = T.z1;
		z2 = T.z2;
		z3 = T.z3;
	};
}
.IF
.P1
triangle {
	var z1, z2, z3;
	conn z1 to z2 to z3 to z1;
}

main {
	put T: triangle {
		z1 = 0;
		z2 = 1;
		z3 = (2,2);
	};
	put circle {
		z1 = T.z1;
		z2 = T.z2;
		z3 = T.z3;
	};
}
.P2
.Ke
.NH 2
Parameter Section Commands
.PP
Any \*(id statement may appear in the parameter
section of a \fIput\fP statement.
This section illustrates some uses of this feature.
.PP
Suppose we will need to draw
pictures of a linked list before and after
insertion of a new node.
We might start with this definition
of a
.I listnode :
.Ks
.IS
...width 1.0
...colwid 8.0
listnode {
	put info: rect {
		var hook;
		hook = (nw + sw)/2;
		ht = lht;
		wd = lwd/2;
	};
	put next: rect {
		var c;
		c = (nw + se)/2;
		sw = info.se;
		ht = lht;
		wd = lwd/2;
	};
}
main {
	put listnode {
		info.sw = 0;
		var lht, lwd;
		lht = 1;
		lwd = 2;
	};
}
.IF
.P1
listnode {
	put info: rect {
		var hook;
		hook = (nw + sw)/2;
		ht = lht;
		wd = lwd/2;
	};
	put next: rect {
		var c;
		c = (nw + se)/2;
		sw = info.se;
		ht = lht;
		wd = lwd/2;
	};
}
.P2
.Ke
.LP
This version of
.I listnode
depends on
.I rect .
Notice that it references two variables
($lwd$ and $lht$) that are not local to
itself:
these variables must be defined in any
environment in which $listnode$ is put.
(They are global to it.)
We have added statements that define new
variables ($hook$ in $first$ and $c$ in $next$)
that are local to the particular instance of
.I rect .
.PP
Now we draw the list as it is before insertion:
.Ks
.IS
arrow {
	var hd, tl, head, wing;
	head = 0.2;
	wing = head*(tl-hd)/abs(tl-hd);
	conn hd to tl;
	conn hd + cis(25)*wing to hd to hd + cis(-25)*wing;
}
...width 2.0
main {
	var lht, lwd;
	lht = 1;
	lwd = 2*lht;
	put first: listnode {
		info.sw = 0;
	};
	put last: listnode {
		info.sw = 2[first.info.sw,first.next.se];
		conn next.sw to next.ne;
	};
	put new: listnode {
		info.nw = 2[first.next.ne,first.next.se];
	};
	put arrow {
		hd = last.info.hook;
		tl = first.next.c;
	};
	put arrow {
		hd = new.info.hook;
		tl = hd - 1;
		right 'new ' at tl;
	};
	put arrow {
		hd = first.info.hook;
		tl = hd - 1;
		right 'list ' at tl;
	};
}
.IF
.P1
main {
	var lht, lwd;
	lht = 1;
	lwd = 2*lht;
	put first: listnode {
		info.sw = 0;
	};
	put last: listnode {
		info.sw = 2[first.info.sw,first.next.se];
		conn next.sw to next.ne;
	};
	put new: listnode {
		info.nw = 2[first.next.ne,first.next.se];
	};
	put arrow {
		hd = last.info.hook;
		tl = first.next.c;
	};
	put arrow {
		hd = new.info.hook;
		tl = hd - 1;
		right "new " at tl;
	};
	put arrow {
		hd = first.info.hook;
		tl = hd - 1;
		right "list " at tl;
	};
}
.P2
.Ke
.LP
Here we have added statements directly to the
parameter section of two of the calls to
.I arrow
to avoid either naming each instance of
.I arrow
or naming the tails of these arrows so that
text could be placed there.
We have also added a statement to draw the
null pointer in box
.I last .
.PP
The program to draw the list after insertion of the
new node remains largely unchanged:
the nodes haven't moved; only the arrows hooking them
together have moved.
.Ks
.IS
main {
	var lht, lwd;
	lht = 1;
	lwd = 2*lht;
	put first: listnode {
		info.sw = 0;
	};
	put last: listnode {
		info.sw = 2[first.info.sw,first.next.se];
		conn next.sw to next.ne;
	};
	put new: listnode {
		info.nw = 2[first.next.ne,first.next.se];
	};
	put arrow {
		hd = new.info.nw;
		tl = first.next.c;
	};
	put arrow {
		hd = last.info.sw;
		tl = new.next.c;
	};
	put arrow {
		hd = new.info.hook;
		tl = hd - 1;
		right 'new ' at tl;
	};
	put arrow {
		hd = first.info.hook;
		tl = hd - 1;
		right 'list ' at tl;
	};
}
.IF
.P1
main {
	var lht, lwd;
	lht = 1;
	lwd = 2*lht;
	put first: listnode {
		info.sw = 0;
	};
	put last: listnode {
		info.sw = 2[first.info.sw,first.next.se];
		conn next.sw to next.ne;
	};
	put new: listnode {
		info.nw = 2[first.next.ne,first.next.se];
	};
	/* These two arrows are different: */
	put arrow {
		hd = new.info.nw;
		tl = first.next.c;
	};
	put arrow {
		hd = last.info.sw;
		tl = new.next.c;
	}
	/* These are the same as before: */
	put arrow {
		hd = new.info.hook;
		tl = hd - 1;
		right "new " at tl;
	};
	put arrow {
		hd = first.info.hook;
		tl = hd - 1;
		right "list " at tl;
	};
}
.P2
.Ke
.NH 2
A Shorter Version
.PP
The previous example is somewhat long-winded
because it demonstrates many features of \*(id
that really aren't needed.
For instance, it would probably have made better
sense to define
.I listnode
as a basic element rather than building it out
of
.I rect s.
This would reduce the need for delving deep
into the internals of boxes from outside
(e.g.
.I first.info.hook ).
Here is the same example reworked:
.Ks
.IS
...width 2
listnode {
	var n, s, e, w, ne, nw, se, sw, next;
	n = s + (0,1)*lht;
	ne = n + 0.5*lwd = nw + lwd;
	se = s + 0.5*lwd = sw + lwd;
	e = (ne + se)/2;
	w = (nw + sw)/2;
	next = (ne + s)/2;
	conn nw to ne to se to sw to nw;
	conn n to s;
}

main {
	var lht, lwd;
	lht = 1;
	lwd = 2;
	put first: listnode {
		sw = 0;
	};
	put last: listnode {
		sw = 2[first.sw,first.se];
		conn s to ne;
	};
	put new: listnode {
		nw = 2[first.ne,first.se];
	};
	put arrow {
		hd = new.nw;
		tl = first.next;
	};
	put arrow {
		hd = last.sw;
		tl = new.next;
	};
	put arrow {
		hd = new.w;
		tl = hd - 1;
		right "new " at tl;
	};
	put arrow {
		hd = first.w;
		tl = hd - 1;
		right "list " at tl;
	};
}
.IE
.P1
listnode {
	var n, s, e, w, ne, nw, se, sw, next;
	n = s + (0,1)*lht;
	ne = n + 0.5*lwd = nw + lwd;
	se = s + 0.5*lwd = sw + lwd;
	e = (ne + se)/2;
	w = (nw + sw)/2;
	next = (ne + s)/2;
	conn nw to ne to se to sw to nw;
	conn n to s;
}

main {
	var lht, lwd;
	lht = 1;
	lwd = 2;
	put first: listnode {
		sw = 0;
	};
	put last: listnode {
		sw = 2[first.sw,first.se];
		conn s to ne;
	};
	put new: listnode {
		nw = 2[first.ne,first.se];
	};
	put arrow {
		hd = new.nw;
		tl = first.next;
	};
	put arrow {
		hd = last.sw;
		tl = new.next;
	};
	put arrow {
		hd = new.w;
		tl = hd - 1;
		right "new " at tl;
	};
	put arrow {
		hd = first.w;
		tl = hd - 1;
		right "list " at tl;
	};
}
.P2
.Ke
Notice that equations need not be just a left side and a right side:
they may include several expressions that should be equal.
...........
.NH 1
Iteration of \*(id Constructs
.NH 2
Pens
.PP
In Donald Knuth's \s-2METAFONT\s0|reference(knuth metafont)
system, pens are different
shapes\(emcircles, ellipses, and polygons\(emthat draw along
curves.
.I Ideal
includes pens as a generalization of this idea:
.I any
box may be used to draw along a line.
For instance, users may define ``dashed'' or ``dotted'' pens.
A pen statement looks like this:
.SP
.P1
conn $x$ to $y$
	using $n$ $pen$ {
		...
	} <$a$,$b$>;
.P2
(The only keywords in this statement are $conn$, $"to"$, and $using$.)
.I Pen
may be any box.
.I Ideal
will place $n$ copies of
.I pen
in the space from $x$ to $y$.
$a$ and $b$ are expressions known to
.I pen .
The first instance of
.I pen
will have $a$ at $x$, and
the last instance of
.I pen
will have $b$ at $y$;
every instance in between will have
its $a$ at the preceding one's $b$,
and its $b$ at the succeeding one's $a$,
as shown in this picture:
.Ks
.IS
...colwid 6.0
...width 4.0
...libfile rect
pen {
	var e, w, c;
	var s;
	e = r.e;
	s = r.s;
	w = r.w;
	c = r.c;
	put r: rect {
		ht = wd = 1;
	};
	left ' \f2a\fP' at w;
	right '\f2b\fP ' at e;
}
main {
	put first: pen {
		w = 0;
		right '\f2x\fP ' at w;
		'1' at s+(0,0.1);
	};
	put second: pen {
		w = first.e;
		'2' at s+(0,0.1);
	};
	'...' at (second.e + ipth.w)/2;
	put ipth: pen {
		w = 2[second.w,second.e];
		'\f2i\fP\-1' at s+(0,0.1);
	};
	put ith: pen {
		w = ipth.e;
		'\f2i\fP' at s+(0,0.1);
	};
	put inth: pen {
		w = ith.e;
		'\f2i\fP+1' at s+(0,0.1);
	};
	'...' at (inth.e + last.w)/2;
	put last: pen {
		w = 2[inth.w,inth.e];
		'\f2n\fP' at s+(0,0.1);
		left ' \f2y\fP' at e;
	};
}
.IE
.Ke
.PP
Here is an example box that contains an angular wavy path:
.Ks
.IS
...colwid 9.0
...width 1.0
wavy {
	var start, end, perp, pt1, pt2, ht;
	perp = (0,1)*(start - end)/abs(start - end);
	pt1 = 0.25[start,end] + perp*ht;
	pt2 = 0.75[start,end] - perp*ht;
	conn start to pt1 to pt2 to end;
}
main {
	put wavy {
		start = 0;
		end = 1;
		ht = 0.2;
		right '(start) ' at start;
		left ' (end)' at end;
	};
}
.IE
.P1
wavy {
	var start, end, perp, pt1, pt2, ht;
	perp = (0,1)*(start - end)/abs(start - end);
	pt1 = 0.25[start,end] + perp*ht;
	pt2 = 0.75[start,end] - perp*ht;
	conn start to pt1 to pt2 to end;
}
.P2
.Ke
.LP
Here we use
.I wavy
as a pen to indicate that part of a rectangle is missing.
.Ks
.IS
main {
	var ne, nw, se, sw;
	var n1, s1, n2, s2;
	sw = 0;
	ne = nw + 2;
	se = sw + 2;
	ne = se + (0,1);
	n2 - 0.4 = n1 = 0.6[nw,ne];
	s2 - 0.4 = s1 = 0.4[sw,se];
	conn n1 to nw to sw to s1;
	conn n1 to s1
		using int(5*abs(n1-s1)) wavy {
			ht = -0.1;
		} <start,end>;
	conn n2 to ne to se to s2;
	conn n2 to s2
		using int(5*abs(n2-s2)) wavy {
			ht = -0.1;
		} <start,end>;
}
.IF
.P1
main {
	var ne, nw, se, sw;
	var n1, s1, n2, s2;
	ne = nw + 2;
	se = sw + 2;
	ne = se + (0,1);
	n2 - 0.4 = n1 = 0.6[nw,ne];
	s2 - 0.4 = s1 = 0.4[sw,se];
	conn n1 to nw to sw to s1;
	conn n1 to s1
		using int(5*abs(n1-s1)) wavy {
			ht = -0.1;
		} <start,end>;
	conn n2 to ne to se to s2;
	conn n2 to s2
		using int(5*abs(n2-s2)) wavy {
			ht = -0.1;
		} <start,end>;
}
.P2
.Ke
.LP
We can change
.I wavy
to contain a smooth wave
by changing the word
.I conn
in the fifth line of its definition to
.I spline ,
and use the same instructions above to draw
the picture with the new pen.
.Ks
.IS
wavy {
	var start, end, perp, pt1, pt2, ht;
	perp = (0,1)*(start - end)/abs(start - end);
	pt1 = 0.25[start,end] + perp*ht;
	pt2 = 0.75[start,end] - perp*ht;
	spline start to pt1 to pt2 to end;
}
main {
	var ne, nw, se, sw;
	var n1, s1, n2, s2;
	sw = 0;
	ne = nw + 2;
	se = sw + 2;
	ne = se + (0,1);
	n2 - 0.4 = n1 = 0.6[nw,ne];
	s2 - 0.4 = s1 = 0.4[sw,se];
	conn n1 to nw to sw to s1;
	conn n1 to s1
		using int(5*abs(n1-s1)) wavy {
			ht = -0.1;
		} <start,end>;
	conn n2 to ne to se to s2;
	conn n2 to s2
		using int(5*abs(n2-s2)) wavy {
			ht = -0.1;
		} <start,end>;
}
.IE
.Ke
.NH 2
Pens as For-Statements
.PP
If \*(id had a for-statement, the pen statement
.P1
conn $x$ to $y$
	using $n$ $pen$ {
		...
	} <$a$,$b$>;
.P2
.LP
would be equivalent to
.P1
for $i$ = 1 to $n$ {
	put $pen$ {
		$a$ = (($i$-1)/$n$)[$x$,$y$];
		$b$ = ($i$/$n$)[$x$,$y$];
		...
	};
}
.P2
.Ts
This means a pen statement can be used to synthesize
a for-statement in an \*(id program.
Here is a pen statement to draw a dashed arc:
.Ks
.IS
...libfile arc
main {
	conn 0 to 180
		using 10 arc {
			center = 0;
			radius = 1;
		}<startang, 9+endang>;
}
.IF
.P1
conn 0 to 180
	using 10 arc {
		center = 0;
		radius = 1;
	}<startang, 9+endang>;
.P2
.Ke
.Te
.Ts
To draw a set of concentric circles
we can say:
.Ks
.IS
...libfile circle
main {
	conn 1 to 6
		using 5 circle {
			center = 0;
			} <radius, radius+1>;
}
.IF
.P1
main {
	conn 1 to 6
		using 5 circle {
			center = 0;
			} <radius,radius+1>;
}
.P2
.Ke
.Te
.Ts
In the first example, the second expression in angle
brackets is important: it means that each ``dash''
covers nine degrees.
In the second example, this expression is redundant:
its only purpose is to prevent \*(id from generating
a stream of error messages about inconsistent equations.
This contorted way of simulating for-statements
is necessary only because pens are meant to do
the right thing at each end of and all along a path that is
being drawn with some box, and not as general
iteration constructs.
.Te
.Ts
Why doesn't \*(id have
for-statements?
Notice that every variable in an \*(id program
is assigned a value exactly once.
Obviously the index of a for-statement must be
an exception to that rule.
Generating the index internally prevents the
need to have two kinds of variables\(emchanging
and fixed.
The local variables of boxes placed by pen statements
cannot be referenced outside the pen statement,
because the boxes are not named when they are placed.
A general for-statement could lead to the need to
generate automatically names for different instances of boxes,
a hard problem that I don't understand well enough yet.
.Te
..........
.NH 2
Filling Regions
.PP
Because any box can be used as a pen, we can shade regions.
(We can have not only dashed and dotted ink, but checkered paint!)
Take box
.I wavy ,
for instance.
First we construct a box
.I brush ,
which consists of seven copies of
.I wavy
going horizontally:
.Ks
.IS
wavy {
	var start, end, perp, pt1, pt2, ht;
	perp = (0,1)*(start - end)/abs(start - end);
	pt1 = 0.25[start,end] + perp*ht;
	pt2 = 0.75[start,end] - perp*ht;
	conn start to pt1 to pt2 to end;
}
brush {
	var top, bot;
	var bwd, bht;
	var leftpt, rightpt;
	leftpt = 0.5*(top+bot) - bwd/2;
	rightpt = 0.5*(top+bot) +bwd/2;
	conn leftpt to rightpt
		using 7 wavy {
			ht = bht;
		}<start,end>;
}
main {
	put brush {
		leftpt = 0;
		rightpt = 7;
		bht = 0.3;
	};
}
.IF
.P1
brush {
	var top, bot;
	var bwd, bht;
	var leftpt, rightpt;
	leftpt = 0.5*(top+bot) - bwd/2;
	rightpt = 0.5*(top+bot) + bwd/2;
	conn leftpt to rightpt
		using 7 wavy {
			ht = bht;
		}<start,end>;
}
.P2
.Ke
Then we use ``brush'' to draw vertically
over the region of interest.
.Ks
.IS
background {
	conn (0,1) to (0,-1)
		using 6 brush {
			bwd = 2;
			bht = 0.1;
			}<top,bot>;
}
main {
	put background {
	};
}
.IF
.P1
conn (0,1) to (0,-1)
	using 6 brush {
		bwd = 2;
		bht = 0.1;
		}<top,bot>;




.P2
.Ke
.NH 1
Opaque Boxes
.PP
.I Ideal
includes statements to blot out pieces
of a picture.
In this section we will sometimes place opaque boxes
without explicitly drawing any background.
In such cases, assume
that we have painted over
the area with pens as above.
.NH 2
Opaque Polygons
.PP
.I Ideal
needs to know the vertices of a polygon in order
to opaque the area it covers.
The vertices are specified as a list in a
.I boundary
statement.
For instance, to opaque a rectangular region using the
.I rect
box defined in Section 3, we could use the following statement:
.Ks
.IS
...libfile rect
wavy {
	var start, end, perp, pt1, pt2, ht;
	perp = (0,1)*(start - end)/abs(start - end);
	pt1 = 0.25[start,end] + perp*ht;
	pt2 = 0.75[start,end] - perp*ht;
	conn start to pt1 to pt2 to end;
}
brush {
	var top, bot;
	var bwd, bht;
	var leftpt, rightpt;
	leftpt = 0.5*(top+bot) - bwd/2;
	rightpt = 0.5*(top+bot) +bwd/2;
	conn leftpt to rightpt
		using 7 wavy {
			ht = bht;
		}<start,end>;
}
background {
	conn (0,1) to (0,-1)
		using 7 brush {
			bwd = 2;
			bht = 0.1;
			}<top,bot>;
}
main {
	put background {
	};
	put rect {
		opaque;
		boundary = sw, se, ne, nw;
		sw = (-0.4,-0.4);
		wd = ht = 1;
	};
}
.IF
.P1
put rect {
	opaque;
	boundary= sw, se, ne, nw;
	sw = (-0.4,-0.4);
	wd = ht = 1;
};




.P2
.Ke
.LP
The sides of the rectangle are drawn by
.I rect :
they are not supplied automatically by the opaquing routine.
If we wanted to save only the interior of the rectangle,
we could use almost the same statement:
.Ks
.IS
...minx -1.0
...maxx 1.0
...miny -1.0
...maxy 1.0
main {
	put background {
	};
put rect {
	opaque exterior;
	boundary = sw, se, ne, nw;
	sw = (-0.4,-0.4);
	wd = ht = 1;
};
}
.IF
.P1
put rect {
	opaque exterior;
	boundary = sw, se, ne, nw;
	sw = (-0.4,-0.4);
	wd = ht = 1;
};
.P2
.Ke
.PP
If we plan to opaque a lot of rectangles,
we should include a
.I boundary
in the definition of
.I rect .
Such a default
.I boundary
would be referenced only if the parameter section
of the \fIput\fP statement included an opaque statement
.I and
did not include its own
.I boundary .
.NH 2
Opaque Circular Arc Polygons
.PP
The edges of opaque regions can also be circular arcs.
.Ts
This generalization of the simple boundary statement
is the most recent addition to \*(id.
It avoids treating circles and their sectors and segments
as special cases, and makes opaquing circular
arc polygons much easier.
.Te
.LP
To specify a circular arc edge, one gives its endpoints
and a point through which it passes;
this ``pass-through'' point is marked in the boundary
list by the symbol ``\f8^\fP''.
For example, the boundary list for the sector shown below is
.Ks
.IS
...libfile arc
main {
	put background {
	};
put arc {
	center = (-1,-1);
	radius = 2;
	startang = 30;
	endang = 60;
	boundary = center,
		center+radius*cis(startang),
		^center+radius*cis(0.5*(startang+endang)),
		center+radius*cis(endang);
	opaque;
	conn start to center to end;
};
}
.IF
.P1
boundary = center, cis(30), ^ cis(45), cis(60);





.P2
.Ke
.PP
Another common opaque arc is the segment:
.Ks
.IS
main {
	put background {
	};
put arc {
	center = (-1,-1);
	radius = 2;
	startang = 0;
	endang = 90;
	boundary = start,
		^center + radius*cis(0.5*(startang+endang)),
		end;
	opaque;
	conn start to end;
};
}
.IF
.P1
boundary = cis(0), ^ cis(45), cis(90);










.P2
.Ke
.PP
One can construct an opaque circle out of two semicircular edges:
.Ks
.IS
...libfile circle
main {
	put background {
	};
put circle {
	radius = 0.5;
	center = 0;
	opaque;
};
put circle {
	radius = 1;
	center = 0;
	opaque exterior;
};
}
.IF
.P1
boundary = cis(0), ^ cis(90), cis(180), ^ cis(270);






.P2
.Ke
.LP
Here, the outer circle has an opaque exterior,
while the inner circle has an opaque interior.
.NH 2
Order is Important
.PP
Without the ability to opaque,
the order in which boxes are put does not matter.
But when some boxes are opaque, order obviously
.I does
matter.
Put statements are executed
in the order in which they appear in the box definition.
When an opaque box is drawn, the opaquing is done
.I first ,
then the lines of the box are drawn;
so, for instance, an opaque
.I listnode
does include the line down its middle separating its
.I info
field from its
.I next
field.
.NH 2
Some Hard Facts
.PP
Neither text nor splines can be used to opaque objects,
nor will they be clipped properly if they are in a
picture and an opaque box is placed over them.
.Ts
The problem with text is that \*(id operates as a
.I troff
preprocessor,
so it cannot determine anything about the size of the
text, and it needs to know that if it is to do anything
involving opaquing and text.
.Te
.Ts
The problem with splines is more subtle.
When a line or circular arc is chopped,
it is easy to specify the lines or circular arcs that remain.
But when a spline is chopped, the guiding points of the resulting curve pieces
are hard to determine.
.Te
.NH 1
Paper Commands
.PP
.I Ideal
includes two commands that are analogous
to the way people draw on paper.
.NH 2
Construct
.PP
The
.I construct
statement looks just like a \fIput\fP statement,
with the keyword
.I put
replaced by
.I construct .
It is best to think of it as laying a sheet of
tracing paper over the current drawing.
Anything constructed will be drawn on this sheet.
When you return to the layer underneath,
you may refer to any of the local variables
(for example, with a \fIput\fP statement), but the lines and curves
in the constructed picture don't show.
.Ks
.IS
...maxy 2.0
...minx -1.0
...maxx 1.0
...libfile rect arrow
main {
	construct A: rect {
		sw = 0;
		wd = ht = 1;
	};
	construct B: rect {
		n = A.s - (0,1);
		wd = ht = 1;
	};
	'top' at A.c;
	'bottom' at B.c;
	put arrow {
		hd = B.n;
		tl = A.s;
	};
}
.IF
.P1
main {
	construct A: rect {
		sw = 0;
		wd = ht = 1;
	};
	construct B: rect {
		n = A.s - (0,1);
		wd = ht = 1;
	};
	'top' at A.c;
	'bottom' at B.c;
	put arrow {
		hd = B.n;
		tl = A.s;
	};
}
.P2
.Ke
.LP
Here we have used ``invisible boxes'' to place the
arrow around the text without drawing the boxes.
.NH 2
Draw
.PP
The
.I construct
command may also be used to localize the
effects of opaque boxes.
We saw above how to draw a filled polygon:
paint over the area, then opaque the exterior
of the polygon.
But how can we draw
.I two
filled polygons in the same picture?
.PP
One solution is to
construct them both, then use the
.I draw
command to add them to the main picture.
The
.I draw
command transfers the contents of the
named sheet of tracing paper to the sheet below.
.Ks
.IS
...libfile circle
...width 2.0
...colwid 10.0
null {
}
pentagon {
	var center, radius,
		pt1, pt2, pt3, pt4, pt5;
	pt1 = center + radius;
	pt2 = center + cis(72)*radius;
	pt3 = center + cis(144)*radius;
	pt4 = center + cis(-144)*radius;
	pt5 = center + cis(-72)*radius;
	conn pt1 to pt2 to pt3 to pt4 to pt5 to pt1;
	boundary = pt1, pt2, pt3, pt4, pt5;
}
main {
	construct A: null {
		conn (0,1) to 0
			using 7 brush {
				bwd = 1;
				bht = 0.1;
				}<top,bot>;
		put pentagon {
			center = (0,0.5);
			radius = (0,0.5);
			opaque exterior;
		};
	};
	construct B: null {
		conn (0.5,0.5) to (1.5,0.5)
			using 5 brush {
				bwd = (0,1);
				bht = 0.1;
				}<top,bot>;
		put circle {
			center = (1,0.5);
			radius = 0.5;
			opaque exterior;
		};
	};
	draw A;
	draw B;
}
.IF
.P1
null {
}

pentagon {
	var center, radius,
		pt1, pt2, pt3, pt4, pt5;
	pt1 = center + radius;
	pt2 = center + cis(72)*radius;
	pt3 = center + cis(144)*radius;
	pt4 = center + cis(-144)*radius;
	pt5 = center + cis(-72)*radius;
	conn pt1 to pt2 to pt3 to pt4 to pt5 to pt1;
	boundary = pt1, pt2, pt3, pt4, pt5;
}

main {
	construct A: null {
		conn (0,1) to 0
			using 7 brush {
				bwd = 1;
				bht = 0.1;
				}<top,bot>;
		put pentagon {
			center = (0,0.5);
			radius = (0,0.5);
			opaque exterior;
		};
	};
	construct B: null {
		conn (0.5,0.5) to (1.5,0.5)
			using 5 brush {
				bwd = (0,1);
				bht = 0.1;
				}<top,bot>;
		put circle {
			center = (1,0.5);
			radius = 0.5;
			opaque exterior;
		};
	};
	draw A;
	draw B;
}
.P2
.Ke
.LP
Even \fIput\fP statements can be added to the parameter sections
of
.I construct
(and \fIput\fP) statements!
Believe it or not, box
.I null
defined above is one of the most useful:
it gives us a way to name a set of commands and variables
so that we may reference them later, yet it doesn't
require us to define a box that will be used only once.
.PP
Given
.I construct
and
.I draw ,
why do we need a special box
.I hole
to opaque a circular area without drawing anything?
Why not just construct an opaque circle, then draw it
on the main picture?
The problem is that the effect of opaquing is localized
just as much as the drawing, so an opaque constructed
circle won't opaque anything that lies underneath.
........
.NH 1
Library Files
.PP
Library files are available to draw common figures and for special
figures like circles and arcs.
To include a library file as part of an \*(id program,
include the line
.P1
\&...libfile $name$
.P2
in your \*(id program (between the
.CW .IS
and
.CW .IE
lines that mark its
start and end.)
This section describes available library files that have been alluded
to up to now.
.NH 2
Rectangle
.PP
File
.I rect
contains the definition of box
.I rect ,
which looks like the definition in Section 3, with
five more variables:  $n$, $s$, $e$, $w$, and $c$,
which are the four compass points and the center,
respectively.
.P1
rect {
	var ne, nw, sw, se,
	n, e, w, s, c,
	ht, wd;
	ne = se + (0,1)*ht;
	nw = sw + (0,1)*ht;
	ne = nw + wd;
	n = (ne+nw)/2;
	s = (se+sw)/2;
	e = (ne+se)/2;
	w = (nw+sw)/2;
	c = (ne+sw)/2;
	ht ~ 1;
	wd ~ 1.5;
	boundary = ne, nw, sw, se;
	conn ne to nw to sw to se to ne;
}
.P2
.NH 2
Arrow
.PP
File
.I arrow
contains the definition of box
.I arrow
as given in Section 3.
.P1
arrow {
	var tl, hd, head, perp, headang;
	conn tl to hd;
	perp = head*(tl-hd)/abs(tl-hd);
	conn hd + cis(headang)*perp to hd to hd + cis(-headang)*perp;
	head ~ 0.1;
	headang ~ 25;
}
.P2
.NH 2
Wavy
.PP
Library file
.I wavy
contains the definition for the familiar
.I wavy .
.P1
wavy {
	var start, end, perp, pt1, pt2, ht;
	perp = (0,1)*(start - end)/abs(start - end);
	pt1 = 0.25[start,end] + perp*ht;
	pt2 = 0.75[start,end] - perp*ht;
	conn start to pt1 to pt2 to end;
}
.P2
.NH 2
Dash
.PP
Library file
.I dash
contains the definition of box
.I dash ,
which may be useful for drawing dashed lines:
.P1
dash {
	var start, end;
	conn start to 0.25[start,end];
	conn 0.75[start,end] to end;
}
.P2
..........
.NH 2
Circles
.PP
Box
.I circle
resides in a library file of the same name, and
has the local variables described in Section 3:
$center$, $radius$, $z1$, $z2$, and $z3$.
.Ts
Box
.I circle
actually consists of variable declarations
and a put of box
.I CIRCLE .
So, if you will have many circles of a particular radius,
it might be easier for you to define your own, say, $circle3$:
.P1
...libfile CIRCLE
circle {
	var center, radius, z1, z2, z3;
	put CIRCLE {
		radius = 3;
	}
}
.P2
.Te
.NH 2
Arcs
.PP
Box
.I arc
is contained in a library file of the same name,
with local variables as described in Section 3:
$center$, $radius$, $start$, $midway$, $end$,
$startang$, $midang$, and $endang$.
.Ts
As above,
.I arc
calls on
a box called
.I ARC .
.Te
.NH 1
Examples
.NH 2
B-Trees
.PP
This example depicts a B-tree|reference(btree) whose nodes have many children.
.Ks
.IS
...colwid 6.0
...width 3.0
...libfile rect
arrow {
	var tl, hd, headvec, head;
	headvec = hd + head*(tl - hd)/abs(tl - hd);
	conn tl to hd;
	conn hd + cis(20)*(headvec - hd)
		to hd
		to hd + cis(-20)*(headvec - hd);
}

dot {
	var s, e;
	'\(bu' at 0.5[s, e] - (0,0.1);
}

per {
	var s, e;
	'.' at 0.5[s, e];
}

main {
var rw, rh;
rw = 2;
rh = 1;
var hmv, vmv;
hmv = 0.6;
vmv = 0.3;
var ah;
ah = 0.2;
put root:rect{
	sw = (0,1);
	wd = rw;
	ht = 1;
	};
put next:rect{
	sw = youngest.sw + 2*(hmv, vmv);
	wd = rw;
	ht = rh;
	};
put youngest:rect{
	sw = bro.sw + (hmv, vmv);
	wd = rw;
	ht = rh;
	opaque;
	};
put bro:rect{
	sw = eldest.sw + (hmv, vmv);
	wd = rw;
	ht = rh;
	opaque;
	};
put eldest:rect{
	im(nw) = im(3[root.nw,root.sw]);
	re(0.5[nw, next.nw]) = re(0.5[root.nw, root.ne]);
	wd = rw;
	ht = rh;
	opaque;
	put arrow{
		head = ah;
		tl = 0.0[sw, se] + 0.5 * (nw - sw);
		hd = tl - (0, 1) * cis(-27);
	};
	put arrow{
		head = ah;
		tl = 0.33[sw, se] + 0.5 * (nw -sw);
		hd = tl - (0,1) * cis(-9);
	};
	put arrow{
		head = ah;
		tl = 0.67[sw, se] + 0.5 * (nw - sw);
		hd = tl - (0,1) * cis(9);
	};
	put arrow{
		head = ah;
		tl = 1.0[sw, se] + 0.5 * (nw -sw);
		hd = tl - (0,1) * cis(27);
	};
	};
put arrow{
	head = ah;
	tl=root.sw + 0.5 * (root.nw - root.sw);
	hd=eldest.nw;
};
put arrow{
	head = ah;
	tl=0.1[root.sw,root.se] + 0.5 * (root.nw - root.sw);
	hd=bro.nw;
};
put a:arrow{
	head = ah;
	tl=0.2[root.sw,root.se] + 0.5 * (root.nw - root.sw);
	hd=youngest.nw;
};
put b:arrow{
	head = ah;
	tl = 0.5[root.ne, root.se];
	hd = next.nw;
};
put arrow{
	head = ah;
	hd = root.nw;
	tl = 0.5[root.nw, root.ne] + (0, 1.0);
};
conn next.nw to youngest.nw using 3 dot{}<s, e>;
conn next.se to youngest.se using 3 dot{}<s, e>;
conn b.tl to a.tl using 4 per{}<s, e>;
}
.IE
.Ke
.P1
\&...libfile rect
arrow {
	var tl, hd, headvec, head;
	headvec = hd + head*(tl - hd)/abs(tl - hd);
	conn tl to hd;
	conn hd + cis(20)*(headvec - hd)
		to hd
		to hd + cis(-20)*(headvec - hd);
}

dot {
	var s, e;
	'\e(bu' at 0.5[s, e] - (0,0.1);
}

per {
	var s, e;
	'.' at 0.5[s, e];
}

main {
	var rw, rh;
	rw = 2;
	rh = 1;
	var hmv, vmv;
	hmv = 0.6;
	vmv = 0.3;
	var ah;
	ah = 0.2;
	put root: rect {
		sw = (0,1);
		wd = rw;
		ht = 1;
	};
	put next: rect {
		sw = youngest.sw + 2*(hmv, vmv);
		wd = rw;
		ht = rh;
	};
	put youngest: rect {
		sw = bro.sw + (hmv, vmv);
		wd = rw;
		ht = rh;
		opaque;
	};
	put bro: rect {
		sw = eldest.sw + (hmv, vmv);
		wd = rw;
		ht = rh;
		opaque;
	};
	put eldest: rect {
		im(nw) = im(3[root.nw,root.sw]);
		re(0.5[nw, next.nw]) = re(0.5[root.nw, root.ne]);
		wd = rw;
		ht = rh;
		opaque;
		put arrow {
			head = ah;
			tl = 0.0[sw, se] + 0.5 * (nw - sw);
			hd = tl - (0, 1) * cis(-27);
		};
		put arrow {
			head = ah;
			tl = 0.33[sw, se] + 0.5 * (nw -sw);
			hd = tl - (0,1) * cis(-9);
		};
		put arrow {
			head = ah;
			tl = 0.67[sw, se] + 0.5 * (nw - sw);
			hd = tl - (0,1) * cis(9);
		};
		put arrow {
			head = ah;
			tl = 1.0[sw, se] + 0.5 * (nw -sw);
			hd = tl - (0,1) * cis(27);
		};
	};
	put arrow {
		head = ah;
		tl=root.sw + 0.5 * (root.nw - root.sw);
		hd=eldest.nw;
	};
	put arrow {
		head = ah;
		tl=0.1[root.sw,root.se] + 0.5 * (root.nw - root.sw);
		hd=bro.nw;
	};
	put a: arrow {
		head = ah;
		tl=0.2[root.sw,root.se] + 0.5 * (root.nw - root.sw);
		hd=youngest.nw;
	};
	put b: arrow {
		head = ah;
		tl = 0.5[root.ne, root.se];
		hd = next.nw;
	};
	put arrow {
		head = ah;
		hd = root.nw;
		tl = 0.5[root.nw, root.ne] + (0, 1.0);
	};
	conn next.nw to youngest.nw using 3 dot{}<s, e>;
	conn next.se to youngest.se using 3 dot{}<s, e>;
	conn b.tl to a.tl using 4 per{}<s, e>;
}
.P2
.NH 2
A Sector Grid
.PP
Norm Schryer used this figure to show how numerical integration
is used to find the area of planar regions.
.Ks
.IS
...colwid 8.0
...width 2.0
...libfile hole
gridline {
	var a,b;
	var neg, pos;
	conn a - neg to a + pos;
}

main {
	var n;
	n = 21;
	conn (0,0) to (0,1+1/(n-1)) using n gridline {neg = 0; pos = 1;} <a,b>;
	conn (0,0) to (1+1/(n-1),0) using n gridline {neg = 0; pos = (0,1);} <a,b>;
	put hole {
		radius = 1;
		center = (0,0);
		opaque exterior;
	};
}
.IF
.P1





\&...libfile hole
gridline {
	var a,b;
	var neg, pos;
	conn a - neg to a + pos;
}

main {
	var n;
	n = 21;
	conn (0,0) to (0,1+1/(n-1))
		using n gridline {
			neg = 0;
			pos = 1;
		} <a,b>;
	conn (0,0) to (1+1/(n-1),0)
		using n gridline {
			neg = 0;
			pos = (0,1);
		} <a,b>;
	put hole {
		radius = 1;
		center = (0,0);
		opaque exterior;
	};
}
.P2
.Ke
.NH 2
Polygon Clipping
.PP
This example from |reference(pavlidis image processing)
illustrates all possible positions of a line
segment with respect to a polygon.
.Ks
.ps 8
.EQ
delim $$
gsize 8
define P12 " P sub 1 ( P sub 2 ) "
define P21 " P sub 2 ( P sub 1 )"
.EN
.IS
...width 4.5
...colwid 6.0
...libfile circle
box spot{
	var loc;
	put circle{
		var center, radius;
		center = loc;
		radius = 0.02;
		opaque;
		};
	}
box vert{
	var midd, llength;
	conn midd+llength to midd-llength;
	}
box poly{
	var c1,p1,p2,p3,p4,p5,r1,tp,bp,ta,tb;
	p1 = c1+r1;
	p2 = cis(65)[c1,p1];
	p3 = cis(170)[c1,p1];
	p4 = cis(190)[c1,p1];
	p5 = cis(300)[c1,p1];
	tp = c1+(0,ta)*r1;
	bp = c1+(0,tb)*r1;
	conn p1 to p2;
	conn p2 to p3;
	conn p3 to p4;
	conn p4 to p5;
	conn p5 to p1;
	put vert{ midd=c1; llength=(0,1.6)*r1; };
	put spot{ loc=tp; };
	put spot{ loc=bp; };
	left '$P12$' at tp+0.1;
	left '$P21$' at bp+0.1;
	}
...minx -1
...maxx 6
...miny -1
...maxy 12
box main {
	put poly{
		c1 =(1,9); r1 = (1,0); ta=1.1; tb=1.4;
		left '$++--$' at p2-1.5;
		right '(a)' at c1-(0.7,1.2);
		};
	put poly{
		c1 =(3.5,9); r1 = (1,0); ta=1.1; tb=0.2;
		left '$+---$' at p2+0.5;
		left '$(-+--)$' at p2+(0.5,-0.15);
		right '(b)(b1)' at c1-(0.7,1.2);
		};
	put poly{
		c1 =(1,5); r1 = (1,0); ta=1.1; tb=-1;
		left '$+--+$' at p2-1.5;
		left '$(-++-)$' at p2-(1.5,0.15);
		right '(c)(c1)' at c1-(0.7,1.2);
		};
	put poly{
		c1 =(3.5,5); r1 = (1,0); ta=0.2; tb=-1;
		left '$---+$' at p2+(0.5,-0.3);
		left '$(--+-)$' at p2+(0.5,-0.45);
		right '(d)(d1)' at c1-(0.7,1.2);
		};
	put poly{
		c1 =(1,1); r1 = (1,0); ta=-1; tb=-1.4;
		left '$--++$' at p4-(0,0.5);
		right '(e)' at c1-(0.7,1.2);
		};
	put poly{
		c1 =(3.5,1); r1 = (1,0); ta=0.2; tb=-0.2;
		left '$----$' at p1+0.2;
		right '(f)' at c1-(0.7,1.2);
		};
}
.IE
.Ke
.ps
.P1
\&.ps 8
.EQ
delim off
.EN
\&.EQ
gsize 8
define P12 " P sub 1 ( P sub 2 ) "
define P21 " P sub 2 ( P sub 1 )"
\&.EN
\&...libfile circle
\&...minx -1
\&...maxx 6
\&...miny -1
\&...maxy 12
box spot{
	var loc;
	put circle{
		var center, radius;
		center = loc;
		radius = 0.02;
		opaque;
		};
	}
box vert{
	var midd, llength;
	conn midd+llength to midd-llength;
	}
box poly{
	var c1,p1,p2,p3,p4,p5,r1,tp,bp,ta,tb;
	p1 = c1+r1;
	p2 = cis(65)[c1,p1];
	p3 = cis(170)[c1,p1];
	p4 = cis(190)[c1,p1];
	p5 = cis(300)[c1,p1];
	tp = c1+(0,ta)*r1;
	bp = c1+(0,tb)*r1;
	conn p1 to p2;
	conn p2 to p3;
	conn p3 to p4;
	conn p4 to p5;
	conn p5 to p1;
	put vert{ midd=c1; llength=(0,1.6)*r1; };
	put spot{ loc=tp; };
	put spot{ loc=bp; };
	left '$P12$' at tp+0.1;
	left '$P21$' at bp+0.1;
	}
box main {
	put poly{
		c1 =(1,9); r1 = (1,0); ta=1.1; tb=1.4;
		left '$++--$' at p2-1.5;
		right '(a)' at c1-(0.7,1.2);
		};
	put poly{
		c1 =(3.5,9); r1 = (1,0); ta=1.1; tb=0.2;
		left '$+---$' at p2+0.5;
		left '$(-+--)$' at p2+(0.5,-0.15);
		right '(b)(b1)' at c1-(0.7,1.2);
		};
	put poly{
		c1 =(1,5); r1 = (1,0); ta=1.1; tb=-1;
		left '$+--+$' at p2-1.5;
		left '$(-++-)$' at p2-(1.5,0.15);
		right '(c)(c1)' at c1-(0.7,1.2);
		};
	put poly{
		c1 =(3.5,5); r1 = (1,0); ta=0.2; tb=-1;
		left '$---+$' at p2+(0.5,-0.3);
		left '$(--+-)$' at p2+(0.5,-0.45);
		right '(d)(d1)' at c1-(0.7,1.2);
		};
	put poly{
		c1 =(1,1); r1 = (1,0); ta=-1; tb=-1.4;
		left '$--++$' at p4-(0,0.5);
		right '(e)' at c1-(0.7,1.2);
		};
	put poly{
		c1 =(3.5,1); r1 = (1,0); ta=0.2; tb=-0.2;
		left '$----$' at p1+0.2;
		right '(f)' at c1-(0.7,1.2);
		};
}
.P2
.NH 1
Acknowledgements
.PP
My thanks to all who read drafts of this manual and
criticized constructively:
Al Aho,
Lorinda Cherry,
Eric Grosse,
Steve Johnson,
Brian Kernighan,
John Mashey,
Doug McIlroy,
and
Theo Pavlidis;
and to early users of \*(id, especially
Eric Grosse,
Theo Pavlidis,
Norm Schryer,
and
Peter Weinberger.
.NH 1
References
.LP
|reference_placement
.FC
.BP
.2C
.EQ
delim $$
gsize 10
.EN
.........
.NH 1
Ideal Reference Manual
.PP
.I Ideal
programs are usually interpolated into
.I troff
input files.
.Tm .IF	S
Each program must be preceded by a
.CW .IS
line, and followed by
either a
.CW .IE
line or a
.CW .IF
line.
The
.CW .IE
line directs that further text will
follow the picture, while the
.CW .IF
line leaves the current
position as it was when the
.CW .IS
line was encountered.
.PP
In this description, italicized names refer to nonterminal grammar symbols.
A parenthesized construct followed by a star means zero or
more repetitions of the construct within.
Comments may appear in \*(id programs between
.CW /*
and
.CW */
brackets
(which nest), and between
.CW #
and newline.
.NH 2
Elements of the \*(id Language
.PP
.I Ideal
expects to be presented with a collection of boxes.
.I Ideal
prepares instructions to draw the picture specified by box
.I main .
If no such box exists, no picture is drawn, but the definitions
that did appear are remembered.
A box looks like this:
.P1
$identifier$ {
	$(statement)*$
}
.P2
.LP
The sections below describe allowed statements.
An $identifier$ is a sequence of letters and digits that starts with a letter.
A $string$ is a sequence of characters between double quotes.
.NH 3
Variable Declarations
.P1
var $identifier$ $( font CW "," identifier)*$ ;
.P2
.PP
This declares complex variables for which space will be allocated whenever
the box in which this statement appears is instantiated.
These declarations should appear before any other statements,
though this order is not enforced at the moment.
.NH 3
Equations
.P1
$expr$ = $expr$ $( ~ font CW "=" ~ expr)*$ ;
$expr$ ~ $expr$ $( ~ font CW "~" ~ expr)*$ ;
.P2
.PP
These statements declare relations that should exist among the named variables.
Equations that involve more than two expressions to be equated are processed left to right.
The second form (an equation with a
.CW ~
instead of an equals sign)
requests that no error message be generated if the equation is inconsistent.
Built-in operators include
.CW + ,
.CW -
(both unary and binary),
.CW * ,
and
.CW / ,
with unary minus binding more tightly than multiplication
and division, which in turn bind more tightly than addition or subtraction.
Parentheses may be used to enforce a particular order of evaluation.
The expression
.P1
( $expr sub 1$ , $expr sub 2$ )
.P2
.LP
has as real part the real part of $expr sub 1$ and as imaginary
part the real part of $expr sub 2$.
(The parentheses in this example are not metacharacters.)
The expression
.P1
$expr sub 1$ [ $expr sub 2$ , $expr sub 3$ ]
.P2
.LP
is evaluated as $expr sub 2 + expr sub 1 * ( expr sub 3 - expr sub 2 )$,
that is
``$expr sub 1$ of the way from $expr sub 2$ to $expr sub 3$.''
Built-in functions are described below.
.NH 4
Manipulating Complex Numbers
.P1
re ( $expr$ )
im ( $expr$ )
conj ( $expr$ )
.P2
.PP
These functions return, respectively, the real part, the imaginary part,
and the complex conjugate of $expr$.
.NH 4
Absolute Value
.P1
abs ( $expr$ )
.P2
.PP
This returns the absolute value of $expr$.
.NH 4
Unit Vector Functions
.P1
cis ( $expr$ )
E ( $expr$ )
unit ( $expr$ )
.P2
.PP
Function $font CW "cis" ()$ returns a unit vector in the direction
of its argument,
which is interpreted as an angle.
For $theta$ in radians, $font CW "cis" ~ theta$ is defined as
$e sup { i theta } ~ == ~ cos ~ theta ~ + ~ i sin ~ theta$,
and
$font CW "E" (x)$ is defined as $font CW "cis" ~ 2 pi x$.
If $theta$ is in degrees, which it usually is in
.I ideal
programs (see below),
.I ideal
makes the necessary conversion
so that $font CW "cis" ()$ and $font CW "E" ()$ work right.
.PP
Function $font CW "unit" ()$ returns a unit vector in the direction
of its argument,
which is a vector.
For $z$ a complex number,
$font CW "unit" (z) ~ == ~ z / font CW "abs" (z)$.
.NH 4
Floating Truncation
.P1
int ( $expr$ )
.P2
.PP
This function returns the integer part of the real part of $expr$.
.NH 4
Inverse Trigonometric Function
.P1
angle ( $expr$ )
.P2
.PP
This function returns the arctangent of $font CW "im" (expr)/ font CW "re" (expr)$.
.NH 3
Square Root Function
.P1
sqrt ( $expr$ )
.P2
.PP
This function returns the square root of its complex argument.
.NH 3
Line Drawing
.P1
conn $expr$ to $expr$ $($ to $~ expr)*$ ;
.P2
.PP
This statement directs that lines be drawn between each successive
pair of points.
.NH 3
Box Placement
.P1
$[ident sub 1 font CW ":" ]$ put $ident sub 2$ { $(statement)*$ } ;
put $ident sub 1$ : $ident sub 2$ { $(statement)*$ } ;
.P2
.PP
This statement directs that box $ident sub 2$ be instantiated with the
statements in braces prepended to those already present in its definition.
If $ident sub 1$ is present in the statement (the optional first form,
or the second form), the instantiated box's name is $ident sub 1$,
and a local variable $x$ of the box may be referenced as $ident sub 1$.$x$.
.NH 3
Pen Drawing
.P1
conn $expr sub 1$ to $expr sub 2$ using $expr sub 3$
	$identifier$ { $(statement)*$ }
	< $expr sub 4$ , $expr sub 5$ > ;
.P2
.PP
This statement is shorthand for the following loop:
.P1 0
for i = 1 to $expr sub 3$ by 1
	put $identifier$ {
		$expr sub 4$ = ((i-1)/$expr sub 3$)
			[$expr sub 1$,$expr sub 2$];
		$expr sub 5$ = (i/$expr sub 3$)
			[$expr sub 1$,$expr sub 2$];
		$(statement)*$
	};
.P2
.LP
Note that it is different from mere text expansion because the upper limit
($expr sub 3$) may not be known explicitly when the program is written.
Note too that \*(id does
.I not
include a for-statement!
.NH 3
Drawing Splines
.P1
spline $expr$ to $expr$ $( font CW "to" ~ expr)*$ ;
.P2
.PP
This statement draws a spline guided by the named points in order.
.NH 3
Placing Captions
.P1
left $string$ at $expr$;
$string$ at $expr$;
right $string$ at $expr$;
.P2
.PP
Use these statements to place $string$ at $expr$.
The default is to center the string at $expr$.
The keyword $"left"$ causes the string to start at $expr$,
while the keyword $"right"$ causes the string to end there.
.NH 3
Constructing and Drawing Boxes
.P1
construct $ident sub 1$ :
	$ident sub 2$ { $(statement)*$ } ;
$ident sub 1$ : construct
	$ident sub 2$ { $(statement)*$ } ;
draw $identifier$ ;
.P2
.PP
The first two of these statements are like the $put$ statements defined above,
but they add to a picture named $ident sub 1$, instead of to the current picture.
The third directs that the named picture (created by a $construct$ command)
be added to the current picture.
.NH 3
Statements Related to Opaquing
.P1
boundary = $expr$ , $expr$ $( font CW "," expr)+$ ;
opaque [ interior ] ;
opaque exterior ;
.P2
.PP
The $boundary$ statement specifies the vertices of a polygon
to be opaqued.
The first $opaque$ statement directs that the interior of the polygon
be opaque.
The second directs that the exterior be made opaque.
If no boundary list is given for a circle, it will opaque a
circular region.
To get an opaque circular region without drawing the circle,
use the box $hole$ instead.
Two other boxes are available for opaquing: $sector$ opaques
a sector of an arc and $segment$ opaques a segment of an arc.
The variables in these boxes are the same as in $arc$.
.NH 2
Command-Line Options
.PP
These options may appear on the command line.
.NH 3
Selecting the Postprocessor
.IP
.CW -p
.br
.CW -4
.br
.CW -n
.PP
.I Ideal
usually prepares output for processing by
.I troff ;
all output dimensions are expressed in inches so that
.I troff
can do the appropriate thing for any typesetter.
The
.CW -p
option directs that the output be generic
.I plot
output.
The
.CW -4
option causes the output to appear on the screen of a 4014.
Both options cause a screen erase at each
.CW .IS ,
but only the
.CW -4
option causes a pause for input at each
.CW .IE .
The screen erase at
.CW .IS
lines may be suppressed by including the line
.P1
\&...noerase
.P2
and may be restored by the line
.P1
\&...yeserase
.P2
Option
.CW -n
directs that the raw
.I ideal
output remain uninterpreted,
so it can be run through
.I nroff
without harm.
.........
.NH 3
Quality Option
.IP
.CW -q
.PP
When
.I ideal
is used to prepare figures for typeset text,
one has a choice of how to draw horizontal and vertical lines.
Without the
.CW -q
option,
.I ideal
uses rule characters,
which draw the lines fast, but may not be accurate.
Giving the
.CW -q
option causes
.I ideal
to draw all lines,
including horizontal and vertical lines, using dots;
this takes longer, but is more accurate.
.NH 3
Angle Processing
.IP
.CW -r
.PP
.I Ideal
expects angles in degrees.
The
.CW -r
option causes
.I ideal
to expect them in radians instead.
(See also the section on Angle Processing as a command to
.I ideal ,
below.)
.NH 3
Including Library Files
.IP
.CW -l\fIlibfile\fP
.PP
This causes
.I ideal
to read the named library file before it begins processing
the input files or the standard input.
(See also the section on Including Library Files as a command to
.I ideal ,
below.)
.NH 2
Commands to Control
.I Ideal
Processing
.PP
These commands affect decisions that must be made during the processing of
an
.I ideal
program.
They should appear after the
.CW .IS
delimiter,
each on a separate line beginning with three dots.
.NH 3
Forgetting a Box
.P1
\&...forget $(boxname)*$
.P2
.PP
All box definitions except the definition of $main$ survive across
.CW .IS\fR/\fP.IE
boundaries.
If you won't need a box again, and memory space is tight, you can
use the $forget$ command to reclaim the space its definition requires.
Because of implementation deficiencies, at most five names of boxes may
appear on one $forget$ line.
There is no way to remember the $main$.
.NH 3
Angle Processing
.P1
\&...degrees
.P2
.P1
\&...radians
.P2
.PP
.I Ideal
expects angles (arguments to $roman "cis"$
and the result of $angle$)
to appear in degrees,
unless it was invoked with the
.CW -r
option on the command line,
in which case it expects angles in radians.
These two commands may be used to switch back and forth between
degrees and radians.
Only one convention may be used within a single
.I ideal
program,
that is between
.CW .IS
and
.CW .IE .
.NH 3
Including Files
.P1
\&...libfile $(file)*$
\&...include $(file)*$
.P2
.PP
The first command directs that the named library files be interpolated so
that their box definitions may be used.
The second is a way to include one's own files.
The following library files are available:
.I arc ,
.I arrow ,
.I circle ,
.I dash ,
.I rect ,
and
.I wavy.
.NH 2
Commands to the Postprocessor
.PP
Each of these commands must appear
between the
.CW .IS
and
.CW .IE
delimiters
on a separate line that begins with
three dots.
.NH 3
Defining the Bounding Box
.P1
\&...minx $number$
\&...miny $number$
\&...maxx $number$
\&...maxy $number$
.P2
.PP
The
.I ideal
processor computes the minimum and maximum extent of the
picture in both $x$- and $y$-directions so that the bounding box
(hence, the picture) can be scaled to the desired width.
These commands allow you to override
.I ideal 's
computed bounding box.
They are useful when the computed bounding box is too large or too small.
.I Ideal
cannot compute the bounding box of a string, so it doesn't try,
and this may be too liberal.
.PP
Normally these dimensions are computed anew for each picture produced
by
.I ideal .
Use the line
.P1
\&...obbox
.P2
between subsequent
.CW .IS
and
.CW .IE\fR/\fP.IF
lines to keep the
bounding box of the previous picture.
.NH 3
Setting the Picture Width
.P1
\&...width $number$
.P2
.PP
This command causes the bounding box to be scaled to a width of $number$
inches.
(If not specified, the default is four inches.)
It persists across
.CW .IS\fR/\fP.IE
boundaries.
.NH 3
Setting the Picture Height
.P1
\&...height $number$
.P2
.PP
This command causes the bounding box to be scaled to a height of $number$
inches.
This allows one to impose a different scale on the horizontal and vertical
dimensions of a picture.
The setting persists across
.CW .IS\fR/\fP.IE
boundaries.
........
.NH 3
Setting the Column Width
.P1
\&...colwid $number$
.P2
.PP
This command gives the width of the column in which the picture will appear
so that it can be centered in the column.
(If not specified, the default is six inches.)
The setting persists across
.CW .IS\fR/\fP.IE
boundaries.
.NH 3
Using Postprocessor Commands to Scale Pictures
.PP
The bounding box and width information can be adjusted
to scale the picture or move it on the page.
.I Ideal
does not check that the entire picture is in fact
contained in the bounding box, and it cannot check
that the column width is correct:
it uses the bounding box information to scale the
picture to the width requested,
then uses the column width to center the bounding box.
.PP
For example, to produce figures at the right of the
page,
.I ideal
was told that the column was nine inches wide.
Since the pictures are typically no more than two inches
wide, they lie within the actual six inch column.
.PP
Suppose one's
figures are drawn in the unit square,
though some pieces lie a little outside,
and one wants one inch to correspond to one in \*(id's
coordinate system.
One could set the six parameters as follows:
.P1
\&...minx 0
\&...miny 0
\&...maxx 1
\&...maxy 1
\&...width 1
\&...colwid 6
.P2
.LP
This gives a way for
users to get exact scale
representations of their pictures.
.NH 2
Obsolete Features
.PP
These holdovers from earlier implementations are not guaranteed to survive.
.NH 3
Options on the
.CW .IS
Line
.PP
One could specify the bounding box on the
.CW .IS
line by
.P1
\&.IS minx maxy maxx miny
.P2
.LP
The bounding box commands to the postprocessor are now preferred.
.NH 3
Changes to Keywords
.PP
The keyword $box$ used to be required before a box definition.
.PP
The keyword $boundary$ has replaced $bdlist$.
.PP
The function $angle$ used to be called $atan2$.
.NH 3
String Delimiters
.PP
Strings used to be delimited by single quotes.

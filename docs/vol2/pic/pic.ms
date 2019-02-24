.so ../ADM/mac
.XX pic 53 "Pic \(em A Graphics Language for Typesetting"
...\" Wed Sep 24 15:49:34 EDT 1986
....TR 116
....TR 85
....TM 80-1272-2 39199 39199-11
.....graphics
.TL
Pic \(em A Graphics Language for Typesetting
.br
User Manual\(dg
.AU
Brian W. Kernighan
.AI
.MH
.AB
.PP
.IT Pic
is a language for drawing simple figures
on a typesetter.
The basic objects in
.IT pic
are
boxes,
circles,
ellipses,
lines,
arrows,
arcs,
spline curves,
and text.
These may be placed anywhere,
at positions specified absolutely
or in terms of previous objects.
The example below illustrates the style and basic capabilities
of the language.
.PS
ellipse "document"
arrow
box "PIC"
arrow
box "TBL/EQN" "(optional)" dashed
arrow
box "TROFF"
arrow
ellipse "typesetter"
.PE
This picture was created with the input
.P1
	\&.PS
	ellipse "document"
	arrow
	box "PIC"
	arrow
	box "TBL/EQN" "(optional)" dashed
	arrow
	box "TROFF"
	arrow
	ellipse "typesetter"
	\&.PE
.P2
.PP
.IT Pic 
is a
.IT troff
preprocessor;
it passes most of its input through untouched, but translates
commands between
.CW .PS
and
.CW .PE
into
.IT troff
commands that draw the pictures.
.AE
.FS
\(dg This is a revised version of |reference(pic cstr 116).
.FE
.NH
Introduction
.PP
.IT Pic
is a language for drawing
pictures.
It operates as yet another
.IT troff
|reference(latest reference troff)
preprocessor
(in the same style as
.IT eqn
|reference(latest eqn) and
.IT tbl
|reference(latest tbl)),
with pictures delimited by
.CW .PS
and
.CW .PE .
.PP
.IT Pic
was inspired partly by Chris Van Wyk's
early work on
.IT ideal
|reference(ideal acm);
it has somewhat the same capabilities,
but quite a different flavor.
In particular,
.IT pic
is much more procedural\(ema picture
is drawn by specifying (sometimes in painful detail)
the motions that one goes through to draw it.
Other direct influences include the PICTURE language
|reference(beatty picture),
the V viewgraph language |reference(viewgraph-v),
and, more recently, new features from the
.IT grap
language |reference(bentley kernighan grap)
for typesetting graphs.
.PP
This paper is primarily a user's manual for
.IT pic ;
a discussion of design issues and user experience
may be found in
|reference(kernighan pic spe).
The next section shows how to use
.IT pic
in the most simple way.
Subsequent sections describe how to change
the sizes of objects when the defaults are wrong,
and how to change their positions when the standard positioning rules
are wrong.
An appendix describes the language succinctly and
summarizes changes since the last manual.
.........
.NH
Basics
.PP
.IT Pic
provides boxes, lines, arrows, circles, ellipses, arcs,
and splines (smooth curves),
plus facilities for positioning
and labeling them.
The picture below shows all of the fundamental objects
(except for splines)
in their default sizes:
.PS
box "box"
move
line "line" above
move
arrow "arrow" above
move
circle "circle"
move
ellipse "ellipse"
move down .25 right
arc "arc"
.PE
Each picture begins with
.CW .PS
and ends with
.CW .PE ;
between them are commands to describe the picture.
Each command is typed on a line by itself.
For example
.P1
\&.PS
box "this is" "a box"
\&.PE
.P2
creates a standard box (\(34 inch wide, \(12 inch high)
and centers the two pieces of text in it:
.PS
box "this is" "a box"
.PE
.PP
Each line of text is a separate quoted string.
Quotes are mandatory, even if the text contains no blanks.
(Of course there needn't be any text at all.)
Each line will be
printed in the current size and font,
centered horizontally, and separated
vertically by the current
.IT troff
line spacing.
.IT Pic
does
not
center the complete drawing itself,
but the default definitions of
.CW .PS
and
.CW .PE
in the
.CW -ms
macro package do.
.PP
You can use
.CW circle
or
.CW ellipse
in place of
.CW box :
.PS
circle "this is" "a box"
move; move
ellipse "this is" "a box"
.PE
.PP
Text is centered on lines and arrows; if there is more than one
line of text, the lines are centered above and below:
.P1
line "this is" "a line"
.P2
.PS
line "this is" "a line"
.PE
.P1
arrow "this is" "an arrow"
.P2
.PS
arrow "this is" "an arrow"
.PE
.PP
Boxes and lines may be dashed or dotted;
just add the word
.CW dashed
or
.CW dotted
after
.CW box
or
.CW line :
.P1
line dashed "dashed" "line"
.P2
.PS
line dashed "dashed" "line"
.PE
.PP
Arcs by default turn 90 degrees counterclockwise from the current direction;
you can make them turn clockwise by saying
.CW arc
.CW cw :
.P1
line; arc; arc cw; arrow
.P2
.PS
line; arc; arc cw; arrow
.PE
A spline might well do this job better; we will return to that shortly.
.PP
As you might guess,
.P1
arc; arc; arc; arc
.P2
draws a circle, though not very efficiently.
Notice that several commands can be put on a single line
if they are separated by semicolons.
.PP
Objects are normally drawn one after another,
left to right,
and connected at the obvious places:
.P1
arrow; box "input"; arrow; box "process"; arrow; box "output"; arrow
.P2
.PS
arrow; box "input"; arrow; box "process"; arrow; box "output"; arrow
.PE
One way to leave a space is with
.CW move :
.P1
box; move; box; move; box
.P2
.PS
box; move; box; move; box
.PE
.PP
Although objects are normally connected left to right,
this can be changed.
If you specify a direction (as a separate object), subsequent objects
will be joined in that direction:
.P1
down; box; arrow; ellipse; arrow; circle
.P2
.PS
down; box; arrow; ellipse; arrow; circle
.PE
.P1
left; box; arrow; ellipse; arrow; circle
.P2
.PS
left; box; arrow; ellipse; arrow; circle
.PE
Each new picture begins going to the right.
.PP
Normally, figures are drawn at a fixed scale,
with objects of a standard size.
It is possible, however, to arrange that a figure be expanded or shrunk
to fit a particular width.
If the
.CW .PS
line contains a number, the drawing is forced to be that many inches wide,
with the height scaled proportionately.
Thus
.P1
\&.PS 3.5
.P2
causes the picture to be 3.5 inches wide.
If two dimensions are specified, the second is the height.
.PP
.IT Pic
is pretty dumb about the size of text in relation
to the size of boxes, circles, and so on.
There is no way to say
``make a box that just fits around this text''
or ``make this text fit inside this circle''
or ``draw a line as long as this text.''
Tight fitting of text
can generally only be done by trial and error.
.PP
Speaking of errors,
if you make a grammatical error in the way you describe
a picture,
.IT pic
will complain and try to indicate where.
For example, the invalid input
.P1
box arrow box
.P2
will draw the message
.P1
pic: syntax error near line 5, file -
 context is
        box >>>  arrow <<<  box
.P2
The brackets
point to the place where the error was first noted;
it sometimes follows 
the word in error, although in this example it's right on target.
The filename
.CW - ' `
is the standard input.
.NH
Controlling Sizes
.PP
This section deals with how to
control the sizes of objects
when the default sizes are not what is wanted.
The next section deals with positioning them
when the default positions are not right.
.PP
Each object that
.IT pic
knows about
(boxes, circles, etc.) has
associated dimensions, like height, width, radius, and so on.
By default,
.IT pic
tries to choose sensible default values for these dimensions,
so that simple pictures can be drawn with a minimum
of fuss and bother.
All of the figures and motions shown so far
have been in their default sizes:
.DS
.ta 1i
box	\(34" wide \(mu \(12" high
circle	\(12" diameter
ellipse	\(34" wide \(mu \(12" high
arc	\(12" radius
line or arrow	\(12" long
move	\(12" in the current direction
.DE
.LP
When necessary,
you can make any object any size you want:
.P1
box width 3 height 0.1; circle radius 0.1
.P2
.PS
	box wid 3 ht 0.1; circle radius 0.1
.PE
All positions and dimensions are in inches, so
the box is 3 inches wide and 1/10 inch high and the circle has radius 1/10 inch.
.PP
An attribute like 
.CW width
changes only the one instance of the object.
You can also change the default size for all objects
of a particular type by assigning values to
.IT pic
variables;
this will be discussed in Section 6.
.PP
The attributes of
.CW height
(which you can abbreviate to
.CW ht )
and
.CW width
(or
.CW wid )
apply to boxes, circles,
ellipses,
and to the head on an arrow.
The attributes of
.CW radius
(or
.CW rad )
and
.CW diameter
(or
.CW diam )
can be used for circles and arcs if they seem more natural.
.PP
Lines and arrows are most easily drawn by specifying
the amount of motion from where you are right now,
in terms of directions.
Accordingly the words
.CW up ,
.CW down ,
.CW left
and
.CW right
and an optional distance can be attached to
.CW line ,
.CW arrow ,
and
.CW move :
.P1
line up 1 right 2
arrow left 2
move left 0.1
line <-> down 1 "height " rjust
.P2
.PS
line up 1 right 2
arrow left 2
move left 0.1
line <-> down 1 "height " rjust
.PE
The notation
.CW <->
indicates a two-headed arrow;
use
.CW ->
for a head on the end and
.CW <-
for one on the start.
Lines and arrows are really the same thing;
in fact,
.CW arrow
is a synonym for
.CW line
.CW -> .
.PP
If you don't specify any distance after
.CW up ,
.CW down ,
etc.,
.IT pic
uses the standard distance:
.P1
line up right; line down; line down left; line up
.P2
.PS
line up right; line down; line down left; line up
.PE
If you omit the direction associated with a distance,
the current direction is used.
.PP
Boxes and lines may be dotted or dashed:
.P1
box dotted; line dotted; move; line dashed; box dashed
.P2
.PS
box dotted; line dotted; move; line dashed; box dashed
.PE
If there is a number after
.CW dot ,
the dots will be approximately that far apart.
You can also control the size of the dashes (at least somewhat):
if there is a length after the word
.CW dashed ,
the dashes will be that long,
and the intervening spaces will be as close as possible to that size:
.P1
line right 5 dashed; move left 5 down .25; right
line right 5 dashed 0.25; move left 5 down .25; right
line right 5 dashed 0.5; move left 5 down .25; right
line right 5 dashed 1
.P2
.PS
line right 5 dashed; move left 5 down .25; right
line right 5 dashed 0.25; move left 5 down .25; right
line right 5 dashed 0.5; move left 5 down .25; right
line right 5 dashed 1
.PE
Dotted or dashed attributes apply only to lines and boxes.
.LP
.PP
You can make any object invisible by adding the word
.CW invis
after it.
This is particularly useful for positioning things correctly
near text:
.P1
box invis "input"; arrow; box invis "output"
.P2
.PS
box invis "input"; arrow; box invis "output"
.PE
.PP
Text may be positioned on lines and arrows:
.P1
arrow "on top of"; move
arrow "above" "below"; move
arrow "above" above; move
arrow "below" below; move
arrow "above" "on top of" "below"
.P2
.PS
arrow "on top of"; move
arrow "above" "below"; move
arrow "above" above; move
arrow "below" below; move
arrow "above" "on top of" "below"
.PE
.LP
.PP
The ``width'' of an arrowhead is the distance across
its tail;
the ``height'' is the distance along the shaft.
The arrowheads in this picture are default size and shape.
.PP
As we said earlier, arcs go 90 degrees counterclockwise
from where you are right now, and
.CW arc
.CW cw
changes this to clockwise.
The default radius is the same as for circles,
but you can change it with the
.CW rad
attribute.
It is also easy to draw arcs between specific places;
this will be described in the next section.
.PP
To put an arrowhead on an arc, use one of
.CW <- ,
.CW ->
or
.CW <-> ,
as with lines.
.LP
.PP
In all cases,
unless an explicit dimension for some object is specified,
you will get the default size.
If you want an object to have the same size
as the previous one of that kind,
add the word
.CW same .
Thus
in the set of boxes given by
.P1
down; box ht 0.2 wid 1.5; move down 0.15; box same; move same; box same
.P2
.PS
down; box ht 0.2 wid 1.5; move down 0.15; box same; move same; box same
.PE
the dimensions set by the first
.CW box
are used several times;
similarly, the amount of motion for the second
.CW move
is the same as for the first one.
.PP
You can change the default sizes of objects
by assigning values to the variables that define their values.
Here is the list, with their default values:
.P1
.ta 2i 3.3i
boxwid = 0.75;	boxht = 0.5
linewid = 0.75;	lineht = 0.5
circlerad = 0.25;	arcrad = 0.25
ellipsewid = 0.75;	ellipseht = 0.5
movewid = 0.75;	moveht = 0.5
textwid = 0;	textht = 0
arrowwid = 0.05;	arrowht = 0.1	\f1(These refer to the arrowhead.)\fP
dashwid = 0.05;	arrowhead = 2	\f1(Arrowhead fill style)\fP
maxpsht = 8.5;	maxpswid = 11	\f1(Maximum picture dimensions)\fP
fillval = 0.3;	scale = 1
.P2
So if you want all your boxes to be long and skinny, and relatively close together,
.P1
boxwid = 0.1; boxht = 1
movewid = 0.2
box; move; box; move; box
.P2
.PS
boxwid = 0.1; boxht = 1
movewid = 0.2
box; move; box; move; box
.PE
.nf
.PS
x = boxwid
 y = boxht
boxwid = 0.1
 boxht = 1
movewid = 0.2
boxwid = x
 boxht = y
arrowhead = 7
 arrowhead = 2
.PE
.PP
Setting the variable
.CW arrowhead
to a value like 7 causes arrowheads to be filled by overstriking;
the default is 2:
.P1
arrowhead = 7; arrow; move; arrowhead = 2; arrow
.P2
.PS
arrowhead = 7; arrow; move; arrowhead = 2; arrow
.PE
.PP
.IT Pic
works internally in inches.
Setting the variable
.CW scale
to some value causes all dimensions to be scaled down
by that value.
Thus, for example,
.CW scale=2.54
causes dimensions to be interpreted as centimeters.
.PP
The numbers given in the
.CW .PS
line override the dimensions given in the picture;
these can be used to force a picture to a particular width and height.
Experience indicates that a good way to get a picture
of the right size is to enter its dimensions in inches,
then if necessary add a width and perhaps height to the
.CW .PS
line.
.PP
Once set, variables like
.CW boxht
retain their values from one picture to the next.
You can reset variables to their default values by listing them in a
.CW reset
statement:
.P1
reset boxht, boxwid
.P2
A bare
.CW reset
resets all variables.
.PP
There is a minimal facility for filling or shading objects,
intended for Postscript output devices.
The attribute
.CW fill
.IT expr
sets the gray scale value to
.IT expr ;
the default, determined by the variable
.CW fillval ,
is 0.3.
Following Postscript, smaller values are darker.
Thus:
.P1
box fill
box ht boxht/2 wid boxwid/2 "hello" at last box
.P2
.PS
reset
box fill
box ht boxht/2 wid boxwid/2 "hello" at last box
.PE
.CW fill
currently only applies to boxes, circles, and ellipses.
.NH
Controlling Positions
.PP
You can place things anywhere you want;
.IT pic
provides a variety of ways to talk about positions.
.IT Pic
uses a standard Cartesian coordinate system with
.IT x
increasing rightwards and
.IT y
increasing upwards,
so any point or object has an
.IT x
and
.IT y
position, measured in inches.
The first object is placed with its start at position 0,0 by default.
The
.IT x,y
position of a box, circle or ellipse is its geometric center;
the position of a line or spline or motion is its beginning;
the position of an arc is the center of the corresponding circle.
.PP
Position modifiers like
.CW from ,
.CW to ,
.CW by
and
.CW at
are followed by an
.IT x,y
pair, and
can be attached to boxes, circles, lines, motions, and so on,
to specify or modify a position.
.PP
You can also use
.CW up ,
.CW down ,
.CW right ,
and
.CW left
with
.CW line
and
.CW move :
.P1
box ht 0.2 wid 0.2 at 0,0 "1"
move right 0.5			# or "move to 0.5,0"
box ht 0.2 wid 0.2 "2"
move right 0.5			# or "move 0.5" or "move same"
box ht 0.2 wid 0.2 "3"
.P2
.PS 2
box ht 0.2 wid 0.2 at 0,0 "1"
move right 0.5			# or "move to 0.5,0"
box ht 0.2 wid 0.2 "2"
move right 0.5
box ht 0.2 wid 0.2 "3"
.PE
Comments can be used in pictures;
they begin with a
.CW #
and end at the end of the line.
.PP
Attributes like
.CW ht
and
.CW wid
and positions like
.CW at
can be written out in any order.
So
.P1
box ht 0.2 wid 0.2 at 0,0
box at 0,0 wid 0.2 ht 0.2
box ht 0.2 at 0,0 wid 0.2
.P2
are all equivalent, though the last is harder to read
and thus less desirable.
.PP
The
.CW from
and
.CW to
attributes are particularly useful with arcs,
to specify the endpoints.
By default, arcs are drawn counterclockwise,
.P1
"+" at 0,0
arc -> from 0.5,0 to 0,0.5
arc -> cw from 0,0 to 1,0.5
.P2
.PS
"+" at 0,0
arc -> from 0.5,0 to 0,0.5
arc -> cw from 0,0 to 1,0.5
.PE
The radius can be made large to provide flat arcs:
.P1
arc -> cw from 0,0 to 2,0 rad 15
.P2
.PS
arc -> cw from 0,0 to 2,0 rad 15
.PE
If the circle is under-specified,
.IT pic
guesses a radius and/or center;
you will have to provide them explicitly if the guess is wrong.
.LP
.PP
We said earlier that
objects are normally connected left to right.
This is an over-simplification.
The truth is that objects are connected together
in the direction specified by the most recent
.CW up ,
.CW down ,
.CW left
or
.CW right
(either alone or as part of some object).
Thus, in
.P1
arrow left; box; arrow; circle; arrow
.P2
the
.CW left
implies connection towards the left:
.PS
arrow left; box; arrow; circle; arrow
.PE
This could also be written as
.P1
left; arrow; box; arrow; circle; arrow
.P2
.PP
Objects are joined in the direction determined by the last
.CW up ,
.CW down ,
etc., with the entry point of the second object attached
to the exit point of the first
(which is fixed at the time of entry).
Entry and exit points for boxes, circles and ellipses are
on opposite sides.
This automatic connection and direction selection
works well if the direction doesn't change but it will occasionally surprise you:
.P1
arrow; circle; down; arrow
.P2
.PS
arrow; circle; down; arrow
.PE
The arrow comes out of the right side of the circle, not the bottom,
as might be expected.
.PP
If a set of commands is enclosed in braces
.CW {...} ,
the current position and direction of motion when the group is finished
will be exactly where it was when entered.
Nothing else is restored.
There is also a more general way to group objects,
using
.CW [
and
.CW ] ,
which is discussed in Section 9.
.NH
Labels and Corners
.PP
Objects can be labelled or named so that you can talk about them later.
For example,
.P1
Box1:	box
	# ... other stuff ...
	move to Box1
.P2
Place names 
.IT must
begin with an upper case letter
(to distinguish them from variable names,
which begin with lower case letters).
The name refers to the ``center''
of the object,
which is the geometric center for most things.
.PP
Other combinations also work:
.P1
line from Box1 to Box2
move to Box1 up 0.1 right 0.2
move to Box1 + 0.2,0.1	# same as previous
line to Box1 - 0.5,0
.P2
The reserved name
.CW Here
may be used to refer to the current position.
.PP
Labels can be reset several times in a single picture,
so a statement like
.P1
Box1:  Box1 + 1,1
.P2
is perfectly legal.
.LP
.PP
You can also refer to previously drawn objects of each type,
using the word
.CW last .
For example, given the input
.P1
box "A"; circle "B"; box "C"
.P2
then
.CW last \& `
.CW box '
refers to box
.CW C ,
.CW last \& `
.CW circle '
refers to circle
.CW B ,
and
.CW 2nd \& `
.CW last
.CW box '
refers to box
.CW A .
Numbering of objects can also be done from the beginning,
so boxes
.CW A
and
.CW C
are
.CW 1st \& `
.CW box '
and
.CW 2nd \& `
.CW box '
respectively.
.PP
To cut down the need for explicit coordinates,
objects have ``corners'' named by compass points:
.P1
B: box "B.c" ht 1 wid 1.5
	" B.e" at B.e ljust
	" B.ne" at B.ne ljust
	" B.se" at B.se ljust
	"B.s" at B.s below
	"B.n" at B.n above
	"B.sw " at B.sw rjust
	"B.w " at B.w rjust
	"B.nw " at B.nw rjust
.P2
.PS
B: box "B.c" ht 1 wid 1.5
" B.e" at B.e ljust
" B.ne" at B.ne ljust
" B.se" at B.se ljust
"B.s" at B.s below
"B.n" at B.n above
"B.sw " at B.sw rjust
"B.w " at B.w rjust
"B.nw " at B.nw rjust
.PE
Note the use of
.CW ljust ,
.CW rjust ,
.CW above ,
and
.CW below
to alter the default positioning of text,
and of a blank within some strings to help space them away from a vertical line.
.PP
Lines and arrows have a
.CW start ,
an
.CW end
and a 
.CW center
in addition to corners.
Circles and ellipses have corners too;
an arc has the same corners as the circle of which it is a part.
The words
.CW left ,
.CW right ,
.CW top ,
and
.CW bottom
are synonyms for
.CW west ,
.CW east ,
.CW north
and
.CW south .
.PP
It is often easiest to position objects by positioning
some part of one at some part of another,
for example the southwest corner of one at the southeast corner
of another.
The
.CW with
attribute
permits this kind of positioning:
.P1
box ht 0.75 wid 0.75
box ht 0.5 wid 0.5 with .sw at last box.se
.P2
.PS
box ht 0.75 wid 0.75
box ht 0.5 wid 0.5 with .sw at last box.se
.PE
Notice that the corner after
.CW with
is written
.CW .sw .
.PP
As another example, consider
.P1
ellipse
ellipse ht .2 wid .3 with .se at 1st ellipse.nw
ellipse ht .2 wid .3 with .sw at 1st ellipse.ne
.P2
.PS
ellipse
ellipse ht .2 wid .3 with .se at 1st ellipse.nw
ellipse ht .2 wid .3 with .sw at 1st ellipse.ne
.PE
.LP
.PP
Sometimes it is desirable to have a line intersect a circle
at a point which is not one of the eight compass points
that
.IT pic
knows about.
In such cases, the proper visual effect can be obtained by using
the attribute
.CW chop
to chop off part of the line:
.P1
circlerad = 0.15; arrowhead = 7
circle "a"
circle "b" at 1st circle - (0.4, 0.6)
circle "c" at 1st circle + (0.4, -0.6)
arrow from 1st circle to 2nd circle chop
arrow from 1st circle to 3rd circle chop
.P2
.PS
circlerad = 0.15; arrowhead = 7
circle "a"
circle "b" at 1st circle - (0.4, 0.6)
circle "c" at 1st circle + (0.4, -0.6)
arrow from 1st circle to 2nd circle chop
arrow from 1st circle to 3rd circle chop
reset
.PE
By default the line is chopped by
.CW circlerad
at each end.
This may be changed:
.P1
line ... chop \f2r\fP
.P2
chops both ends by
\f2r\fP, and
.P1
line ... chop \f2r1\fP chop \f2r2\fP
.P2
chops the beginning by
.IT r1
and the end by
.IT r2 .
More complicated intersections can be computed with
the built-in trigonometric functions listed in the next section.
.PP
There is one other form of positioning that is sometimes useful,
to refer to a point some fraction of the way between
two other points.
This can be expressed in 
.IT pic
as
.P1
\f2fraction\fP of the way between \f2position1\fP and \f2position2\fP
.P2
where
.IT fraction
is any expression, and
.IT position1
and
.IT position2
are any positions.
You can abbreviate this rather windy phrase;
``of the way'' is optional, and the whole thing can
be written instead as
.P1
\f2fraction\fP < \f2position1\fP , \f2position2\fP >
.P2
As a pair of examples:
.P1
box
arrow right from 1/3 of the way between last box.ne and last box.se
arrow right from 2/3 <last box.ne, last box.se>
.P2
.PS
box
arrow right from 1/3 of the way between last box.ne and last box.se
arrow right from 2/3 <last box.ne, last box.se>
.PE
.P1
A:  ellipse
    ellipse ht .2 wid .3 with .se at 1st ellipse.nw
    ellipse ht .2 wid .3 with .sw at 1st ellipse.ne
    circle rad .05 at 0.5 <A.nw,A.c>
    circle rad .05 at 0.5 <A.ne,A.c>
    arc from 0.25 <A.w,A.e> to 0.75 <A.w,A.e>
.P2
.PS
A:	ellipse
	ellipse ht .2 wid .3 with .se at 1st ellipse.nw
	ellipse ht .2 wid .3 with .sw at 1st ellipse.ne
	circle rad .05 at 0.5 <A.nw,A.c>
	circle rad .05 at 0.5 <A.ne,A.c>
	arc from 0.25 <A.w,A.e> to 0.75 <A.w,A.e>
.PE
Naturally,
the distance given by
.IT fraction
can be greater than 1 or less than 0.
.PP
Advice:\ 
experience suggests that the easiest way to position objects
is by placing them relative to previous objects
and places, using
.CW with ,
.CW at ,
etc.
This is better than using
.CW move ;
you should generally avoid
.CW move .
.NH
Variables, Expressions and Built-in Functions
.PP
It's generally a bad idea to write everything in 
absolute coordinates if you are likely
to change things.
.IT Pic
variables let you parameterize your picture:
.P1
a = 0.5;  b = 1

box wid a ht b
ellipse wid a/2 ht 1.5*b
Box2:  Box1 - (a/2, b/2)
.P2
Expressions may use the standard operators
.CW + ,
.CW - ,
.CW * ,
.CW / ,
.CW % ,
.CW ^
(exponentiation),
and parentheses for grouping.
.PP
The most important variables are the predefined ones for
controlling the default sizes of objects,
listed in Section 3.
These may be set at any time in any picture,
and retain their values from picture to picture until reset.
.PP
You can use the height, width, radius,
and
.IT x
and
.IT y
coordinates of any object or corner in an expression:
.P1
Box1.x		# the \f2x\fP coordinate of the center of Box1
Box1.ne.y	# the \f2y\fP coordinate of the northeast corner of Box1
Box1.wid	# the width of Box1
Box1.ht		# and its height
2nd last circle.rad	# the radius of the 2nd last circle
.P2
.PP
Any pair of expressions enclosed in parentheses
defines a position;  furthermore such positions
can be added or subtracted to yield new positions:
.EQ
delim $$
define cw X font CW X
.EN
.P1
(\f2x\fP, \f2y\fP)
($x sub 1$, $y sub 1$) + ($x sub 2$, $y sub 2$)
.P2
If $p sub 1$ and $p sub 2$ are positions, then
$( p sub 1 , p sub 2 )$
refers to the point
$( p sub 1 font CW .x ,~ p sub 2 font CW .y )$.
.PP
.IT Pic
provides a small collection of standard functions:
.DS
.ta 3i
$cw "sin" (expr)$, $cw "cos" (expr)$, $cw atan2 (y,x)$	(angle in radians)
$cw "log" (expr)$, $cw "exp" (expr)$	(Beware: both base 10)
$cw "sqrt" (expr)$, $cw "max" (e sub 1 , e sub 2 )$, $cw "min" (e sub 1 , e sub 2 )$
$cw "int" (expr)$	(integer part of $expr$)
$cw rand ()$	(random number between 0 and 1)
.DE
.EQ
delim off
.EN
.NH
More on Text
.PP
Normally, text is centered
at the geometric center of the object it is associated with.
The attribute
.CW ljust
causes the left end to be at the specified point
(which means that the text lies to the right of the specified place!),
and
.CW rjust
puts the right end at the place.
.CW above
and
.CW below
center the text one half line space in the given direction.
.PP
Text attributes can be compounded:
.P1
arrow 1 "ljust above" ljust above
.P2
.PS
arrow 1 "ljust above" ljust above
.PE
.LP
.PP
Text is most often an attribute of some other object,
but you can also have self-standing text:
.P1
"origin" "(0,0)" at 0,0
"this is" "(1,1)" at 1,1
box ht 1 wid 1 dotted with .sw at 0,0
.P2
.PS
"origin" "(0,0)" at 0,0
"this is" "(1,1)" at 1,1
box ht 1 wid 1 dotted with .sw at 0,0
.PE
.LP
In effect, \f2n\fP text strings are contained in an invisible box
of width
.CW textwid
and height \f2n\fP \(mu
.CW textht .
The variables
.CW textwid
and
.CW textht
may be set to any values;
they are normally zero.
.PP
A list of numeric expressions can be converted to a formatted string
with the
.CW sprintf
function and used anywhere a quoted string can be:
.P1
B: box wid log(20)
   sprintf("width = %g, height = %g ", B.wid, B.ht) rjust at B.w
.P2
.PS
B: box wid log(20)
   sprintf("width = %g, height = %g ", B.wid, B.ht) rjust at B.w
.PE
.......
.NH
Lines and Splines
.PP
A ``line'' may actually be a path,
that is, it may consist of connected segments:
.P1
line right 1 then down .5 left 1 then right 1
.P2
.PS
line right 1 then down .5 left 1 then right 1
.PE
The word
.CW then
separates components of the path.
.PP
A spline is a smooth curve guided by a set of straight lines
just like the line above.
It begins at the same place, ends at the same place,
and in between is tangent to the mid-point of each guiding line.
The syntax for a spline is identical to a (path) line
except for using
.CW spline
instead of
.CW line :
.P1
line dashed right 1 then down .5 left 1 then right 1
spline from start of last line \e
  right 1 then down .5 left 1 then right 1
.P2
.PS
line dashed right 1 then down .5 left 1 then right 1
spline from start of last line \
  right 1 then down .5 left 1 then right 1
.PE
(Long input lines can be split by ending each piece
with a backslash.)
.PP
The elements of a path, whether line or spline,
are specified as a series of points,
either in absolute terms or by
.CW up ,
.CW down , 
etc.
.P1
spline right then up then left then down ->
.P2
.PS
spline right then up then left then down ->
.PE
.P1
spline right then up left then down ->
.P2
.PS
spline right then up left then down ->
.PE
.PP
Notice that arrowheads may be put on the ends
of a line or spline.
.NH
Blocks
.PP
Any sequence of
.IT pic
statements may be enclosed in
brackets
.CW [
and
.CW ]
to form
a block,
which can then be treated as a single object,
and manipulated rather like an ordinary box:
.P1
box "1"
[ box "2"; arrow "3" above; box "4" ] with .n at last box.s - (0,0.1)
"Thing 2: " rjust at last [].w
.P2
.PS
box "1"
[ box "2"; arrow "3" above; box "4" ] with .n at last box.s - (0,0.1)
"Thing 2: " rjust at last [].w
.PE
Notice that
``last''-type constructs treat blocks as a unit and don't look
inside for objects:
.CW last "" ``
.CW box.s ''
refers to box 1, not box 2 or 4.
You can use
.CW last\ [] ,
etc.,
just like
.CW last\ box .
.PP
Blocks have the same compass corners as
boxes (determined by the bounding box).
It is also
possible to position a block by placing either an absolute 
coordinate (like
.CW 0,0 )
or an internal label (like
.CW A )
at some
external point, as in
.P1
[ ...; A: ...; ... ] with .A at ...
.P2
By default, blocks join with other things as boxes do, at the
center of the appropriate side.
.PP
Names of variables and places within a block are local
to that block, and thus do not affect variables and places
of the same name outside.
(In particular, that includes the built-in variables like
.CW boxwid ,
etc.;
if they are set within a block,
they revert to their original values when the block is left.)
You can get at the internal
place names with constructs like
.P1
last [].A
.P2
or
.P1
B.A
.P2
where
.CW B
is a name attached to a block:
.P1
B:  [ ... ;  A: ...;  ]
.P2
When combined with
.CW define
statements (next section), blocks provide
a reasonable simulation of a procedure mechanism.
.PP
Although blocks nest,
it is currently possible to look only one level deep
with constructs like
.CW B.A ,
although
.CW A
may be
further qualified by a corner name (i.e.,
.CW B.A.sw
or
.CW top
.CW of
.CW B.A
are legal).
.PP
The following example illustrates most of the points made above
about how blocks work:
.P1
h = .5;  dh = .02;  dw = .1
[
	Ptr: [
		boxht = h; boxwid = dw
		A: box
		B: box
		C: box
		box wid 2*boxwid "..."
		D: box
	]
	Block: [
		boxht = 2*dw; boxwid = 2*dw
		movewid = 2*dh
		A: box; move
		B: box; move
		C: box; move
		box invis "..." wid 2*boxwid; move
		D: box
	] with .t at Ptr.s - (0,h/2)
	arrow from Ptr.A to Block.A.nw + (dh,0)
	arrow from Ptr.B to Block.B.nw + (dh,0)
	arrow from Ptr.C to Block.C.nw + (dh,0)
	arrow from Ptr.D to Block.D.nw + (dh,0)
]
box dashed ht last [].ht+dw wid last [].wid+dw at last []
.P2
This produces
.PS
h = .5
dh = .02
dw = .1
[
	Ptr: [
		boxht = h; boxwid = dw
		A: box
		B: box
		C: box
		box wid 2*boxwid "..."
		D: box
	]
	Block: [
		boxht = 2*dw; boxwid = 2*dw
		movewid = 2*dh
		A: box; move
		B: box; move
		C: box; move
		box invis "..." wid 2*boxwid; move
		D: box
	] with .t at Ptr.s - (0,h/2)
	arrow from Ptr.A to Block.A.nw + (dh,0)
	arrow from Ptr.B to Block.B.nw + (dh,0)
	arrow from Ptr.C to Block.C.nw + (dh,0)
	arrow from Ptr.D to Block.D.nw + (dh,0)
]
box dashed ht last [].ht+dw wid last [].wid+dw at last []
.PE
.NH
Macros
.PP
.IT Pic
provides a basic macro facility.
In the simplest form,
.P1
define  \f2name\fP  { \f2replacement text\fP }
.P2
defines
.IT name
to be the
.IT "replacement text" .
Any subsequent occurrence of
.IT name
will be replaced by
.IT "replacement text" .
.PP
Macros may have arguments.
If the replacement text of a macro definition contains occurrences of
.CW $1 ,
.CW $2 ,
etc.,
these will be replaced by the corresponding actual arguments
when the macro is invoked.
The invocation for a macro with arguments is
.P1
name(arg1, arg2, ...)
.P2
Non-existent arguments are replaced by null strings.
Macro definitions last from picture to picture;
a macro definition can be removed by
.P1
undef \f2macro-name\fP
.P2
.PP
As an example, one might define a
.CW square
by
.P1
define square { box ht $1 wid $1  $2 }
.P2
and use it as
.P1
square(1, "one" "inch")
square(0.5)
square(0.25, "\es-4tiny\es+4" dashed)
.P2
.PS
define square { box ht $1 wid $1  $2 }
square(1, "one" "inch")
square(0.5)
square(0.25, "\s-4tiny\s+4" dashed)
.PE
Notice how the second argument may be used to pass in arbitrary contents.
.PP
Coordinates like
.IT x,y
may be enclosed in parentheses,
as in
.IT x,y ), (
so they can be included in a macro argument.
.NH
File Copy
.PP
The statement
.P1
copy "\f2filename\fP"
.P2
inserts the contents of the named file
at that point in the input.
Any
.CW .PS
or
.CW .PE
lines within the file
are ignored, so previously prepared pictures
can be used as parts of larger ones without editing.
.PP
.IT Pic
also provides the
.CW copy
.CW thru
mechanism found in
.IT grap :
.P1
copy "\f2file\fP" thru \f2macro-name\fP
.P2
copies
.IT file ,
treating each line as an invocation of the named macro
(each field being an argument).
A literal macro may be used instead of a name:
.P1
copy "\f2file\fP" thru { \f2macro replacement text\fP }
.P2
and if no file name is given,
the remainder of the input until the next
.CW .PE
is used.
So to plot a set of circles at points whose coordinates and radii
are included in-line:
.P1
copy thru { circle rad $3 at $1,$2 }
0 0 .05
1 1 .1
.2 .3 .03
.4 .7 .05
\&...
.P2
.PS
copy thru { circle rad $3 at $1,$2 }
0 0 .05
1 1 .1
.2 .3 .03
.4 .7 .05
.1 .8 .1
.4 .9 .02
.7 .5 .15
1 .4 .04
.8 0 .075
.5 .25 .03
.PE
.PP
The
.CW sh
command executes an arbitrary commandline:
.P1
sh { \f2anything\fP }
.P2
Macros within
.IT anything
are expanded first.
.NH
Loops and Conditionals
.PP
.IT Pic
provides an
.CW if
statement and a
.CW for
loop.
.P1
pi = atan2(0,-1)
for i = 0 to 2 * pi by 0.1 do {
	"\-" at i/2, 0
	"s" at i/2, sin(i)/2
	"c" at i/2, cos(i)/2
}
.P2
.PS
.ps -2
pi = atan2(0,-1)
for i = 0 to 2 * pi by 0.1 do {
	"\-" at i/2, 0
	"s" at i/2, sin(i)/2
	"c" at i/2, cos(i)/2
}
.ps +2
.PE
The
.CW by
clause is optional;
if the value is preceded by a
.CW * ,
the steps are multiplicative, not additive.
The body of the loop is delimited by braces,
which are also used for definitions,
.CW copy ,
.CW sh
and
.CW if
statements.
.PP
The
.CW if
statement is
.P1
if \f2expression\fP then { \f2anything\fP } else { \f2anything\fP }
.P2
where the
.CW else
clause is optional.
The
.IT expression
may use the usual relational operators:
.CW == ,
.CW != ,
.CW > ,
.CW >= ,
.CW < ,
.CW <= ,
.CW && ,
and
.CW || .
.P1
pi = atan2(0,-1)
for i = 0 to pi by 0.1 do {
	if (s = sin(i)) > 0.8 then { s = 0.8 }
	"x" at i/2, s/2
}
.P2
.PS
.ps -2
pi = atan2(0,-1)
for i = 0 to pi by 0.1 do {
	if (s = sin(i)) > 0.8 then { s = 0.8 }
	"x" at i/2, s/2
}
.ps +2
.PE
A string comparison using
.CW ==
or
.CW !=
is also permitted, to compare quoted strings:
.P1
if "\f2string1\fP" == "\f2string2\fP" then ...
.P2
.NH
\fITroff\fP Interface
.PP
.IT Pic
is usually run as a
.IT troff
preprocessor:
.P1
pic file | troff -ms
.P2
Run it before
.IT eqn
if it is also present.
.PP
If the
.CW .PS
line looks like
.P1
\&.PS <file
.P2
then the contents of 
.CW file
are inserted in place of the
.CW .PS
line,
whether or not the file contains
.CW .PS
or
.CW .PE .
(This feature is deprecated in favor of the
.CW copy
statement.)
.PP
.IT Pic
copies the
.CW .PS
and
.CW .PE
lines from input to output intact,
except that it adds two things on the same line as the
.CW .PS :
.P1
\&.PS h w
.P2
.CW h
and
.CW w
are the picture height and width in units.
The
.CW -ms
macro package has definitions for
.CW .PS
and
.CW .PE
that cause
pictures to be centered and offset a bit from surrounding text.
(See the appendix.)
.PP
If 
.CW .PF '' ``
is used instead of
.CW .PE ,
the position after printing is restored to where
it was before the picture started, instead of being at the bottom.
.CW F '' (``
is for ``flyback.'')
.PP
Any input line that begins with a
period is assumed to be a
.IT troff
command that makes sense at that point;
it is copied to the output at that point in the document.
It is asking for trouble to add vertical space or in any way fiddle with
the line spacing here,
but point size and font changes are generally harmless:
.P1
\&.ps 24
circle radius .4 at 0,0
\&.ps 12
circle radius .2 at 0,0
\&.ps 8
circle radius .1 at 0,0
\&.ps 6
circle radius .05 at 0,0
\&.ps 10	\e" don't forget to restore size
.P2
.PS
.ps 24
circle radius .4 at 0,0
.ps 12
circle radius .2 at 0,0
.ps 8
circle radius .1 at 0,0
.ps 6
circle radius .05 at 0,0
.ps 10	\" don't forget to restore size
.PE
.IT Pic
does preserve the state of 
.IT troff 's
fill mode across pictures.
.PP
It is also safe to include sizes, fonts and local motions
within quoted strings
.CW \&"..." ) (
in
.IT pic ,
so long as whatever changes are made are unmade before exiting the string.
For example, to print text in Helvetica Bold 2 points larger:
.P1
ellipse "\es+2\ef(HBSmile!\efP\es-2"
.P2
.PS
ellipse "\s+2\f(HBSmile!\fP\s-2"
.PE
This is essentially the same rule as applies in
.IT eqn .
.PP
There is a subtle problem with complicated equations inside
.IT pic
pictures \(em they come out wrong
if
.IT eqn
has to leave extra vertical space for the equation.
If your equation involves more than subscripts and superscripts,
you must add to the beginning of each equation the extra information
.CW "space 0" :
.P1
arrow
box "$space 0 {H( omega )} over {1 - H( omega )}$"
arrow
.P2
.EQ
delim $$
.EN
.PS
arrow
box "$space 0 {H( omega )} over {1 - H( omega )}$"
arrow
.PE
.NH
Some Examples
.PP
Here are a handful of larger examples:
.KS
.PS
define ndblock X
	box wid boxwid/2 ht boxht/2
	down;  box same with .t at bottom of last box;   box same
X
.ps -2
.ft CW
boxht = .2; boxwid = .3; dx = 0.05
down; box; box; box; box ht 3*boxht "." "." "."
L: box; box; box invis wid 2*boxwid "hashtab:" with .e at 1st box .w
right
Start: box wid .5 with .sw at 1st box.ne + (.4,.2) "..."
N1: box wid .2 "n1";  D1: box wid .3 "d1"
N3: box wid .4 "n3";  D3: box wid .3 "d3"
box wid .4 "..."
N2: box wid .5 "n2";  D2: box wid .2 "d2"

arrow right from 2nd box 
ndblock
spline -> right .2 from 3rd last box then to N1.sw + (dx,0)
spline -> right .3 from 2nd last box then to D1.sw + (dx,0)
arrow right from last box
ndblock
spline -> right .2 from 3rd last box to N2.sw-(dx,.2) to N2.sw+(dx,0)
spline -> right .3 from 2nd last box to D2.sw-(dx,.2) to D2.sw+(dx,0)
arrow right 2*linewid from L
ndblock
spline -> right .2 from 3rd last box to N3.sw + (dx,0)
spline -> right .3 from 2nd last box to D3.sw + (dx,0)

circlerad = .3
circle invis "ndblock"  at last box.e + (1.2,.2)
arrow dashed from last circle.w to last box chop 0 chop .3

box invis wid 2*boxwid "ndtable:" with .e at Start.w
.ps
.ft
.PE
.KE
.sp
.P1
.vs -1p
define ndblock {
	box wid boxwid/2 ht boxht/2
	down;  box same with .t at bottom of last box;   box same
}
boxht = .2; boxwid = .3; circlerad = .3; dx = 0.05
down; box; box; box; box ht 3*boxht "." "." "."
L: box; box; box invis wid 2*boxwid "hashtab:" with .e at 1st box .w
right
Start: box wid .5 with .sw at 1st box.ne + (.4,.2) "..."
N1: box wid .2 "n1";  D1: box wid .3 "d1"
N3: box wid .4 "n3";  D3: box wid .3 "d3"
box wid .4 "..."
N2: box wid .5 "n2";  D2: box wid .2 "d2"
arrow right from 2nd box 
ndblock
spline -> right .2 from 3rd last box then to N1.sw + (dx,0)
spline -> right .3 from 2nd last box then to D1.sw + (dx,0)
arrow right from last box
ndblock
spline -> right .2 from 3rd last box to N2.sw-(dx,.2) to N2.sw+(dx,0)
spline -> right .3 from 2nd last box to D2.sw-(dx,.2) to D2.sw+(dx,0)
arrow right 2*linewid from L
ndblock
spline -> right .2 from 3rd last box to N3.sw + (dx,0)
spline -> right .3 from 2nd last box to D3.sw + (dx,0)
circlerad = .3
circle invis "ndblock"  at last box.e + (1.2,.2)
arrow dashed from last circle.w to last box chop 0 chop .3
box invis wid 2*boxwid "ndtable:" with .e at Start.w
.P2
.sp
.KS
.PS 5
.ps 8
boxht = .5; boxwid = .75
circlerad = .25
	arrow "source" "code"
LA:	box "lexical" "analyzer"
	arrow "tokens" above
P:	box "parser"
	arrow "intermediate" "code"
Sem:	box "semantic" "checker"
	arrow

	arrow <-> up from top of LA
LC:	box "lexical" "corrector"
	arrow <-> up from top of P
Syn:	box "syntactic" "corrector"
	arrow up
DMP:	box "diagnostic" "message" "printer"
	arrow <-> right  from right of DMP
ST:	box "symbol" "table"
	arrow from LC.ne to DMP.sw
	arrow from Sem.nw to DMP.se
	arrow <-> from Sem.top to ST.bot
.PE
.KE
.LP
.P1
\&.PS 5
\&.ps 8
\&	arrow "source" "code"
\&LA:	box "lexical" "analyzer"
\&	arrow "tokens" above
\&P:	box "parser"
\&	arrow "intermediate" "code"
\&Sem:	box "semantic" "checker"
\&	arrow
\&
\&	arrow <-> up from top of LA
\&LC:	box "lexical" "corrector"
\&	arrow <-> up from top of P
\&Syn:	box "syntactic" "corrector"
\&	arrow up
\&DMP:	box "diagnostic" "message" "printer"
\&	arrow <-> right  from right of DMP
\&ST:	box "symbol" "table"
\&	arrow from LC.ne to DMP.sw
\&	arrow from Sem.nw to DMP.se
\&	arrow <-> from Sem.top to ST.bot
\&.PE
.P2
.PP
There are eighteen objects (boxes and arrows) in the picture,
and one line of
.IT pic
input for each;
this seems like an acceptable level of verbosity.
.sp
.KS
.PS
.ps -2
	circle "DISK"
	arrow "character" "defns"
CPU:	box "CPU" "(16-bit mini)"
	{ arrow <- from top of CPU up "input " rjust }
	arrow
CRT:	"   CRT" ljust
	line from CRT - 0,0.075 up 0.15 \
		then right 0.5 \
		then right 0.5 up 0.25 \
		then down 0.5+0.15 \
		then left 0.5 up 0.25 \
		then left 0.5
Paper:	CRT + 1.05,0.75
	arrow <- from Paper down 1.5
	" ...  paper" ljust at end of last arrow + 0, 0.25
	circle rad 0.05 at Paper + (-0.055, -0.25)
	circle rad 0.05 at Paper + (0.055, -0.25)
	"   rollers" ljust at Paper + (0.1, -0.25)
.ps +2
.PE
.ce
Basic Digital Typesetter
.sp
.KE
.LP
.P1
.vs -1
\&.PS
\&.ps -2
	circle "DISK"
	arrow "character" "defns"
CPU:	box "CPU" "(16-bit mini)"
	{ arrow <- from top of CPU up "input " rjust }
	arrow
CRT:	"   CRT" ljust
	line from CRT - 0,0.075 up 0.15 \e
		then right 0.5 \e
		then right 0.5 up 0.25 \e
		then down 0.5+0.15 \e
		then left 0.5 up 0.25 \e
		then left 0.5
Paper:	CRT + 1.05,0.75
	arrow <- from Paper down 1.5
	" ...  paper" ljust at end of last arrow + 0, 0.25
	circle rad 0.05 at Paper + (-0.055, -0.25)
	circle rad 0.05 at Paper + (0.055, -0.25)
	"   rollers" ljust at Paper + (0.1, -0.25)
\&.ps +2
\&.PE
\&.ce
\&Basic Digital Typesetter
.P2
.SH
Acknowledgements
.PP
I am indebted to Chris Van Wyk for ideas from several versions of
.IT ideal .
He and Doug McIlroy have also contributed algorithms
for line and circle drawing,
and made useful suggestions on the design of
.IT pic .
Theo Pavlidis contributed the basic spline algorithm;
Eric Grosse provided code to compute the bounding box of an arc.
Charles Wetherell pointed out reference
[2]
to me, and made several valuable criticisms on an early
draft of the language and manual.
The exposition in this manual has been greatly improved
by suggestions from Jim Blinn and Mark Miller.
I am grateful to
my early users \(em
Brenda Baker,
Dottie Luciani,
and Paul Tukey
\(em
for their suggestions and cheerful use of
an often shaky and clumsy system.
.PP
More recent versions of
.IT pic
have benefited greatly from
adventurous use by
Jon Bentley and Ravi Sethi;
their comments on the manual are also much appreciated.
.SH
References
.LP
|reference_placement
.BP
........
.SH
Appendix A:  \fIPic\fP Reference Manual
.SH
Pictures
.PP
The top-level object in
.IT pic
is the ``picture'':
.P1
\f2picture\fP:
	.PS \f2optional-width\fP \f2optional-height\fP
	\f2element-list\fP
	.PE
.P2
If
.IT optional-width
is present, the picture is made that many inches wide,
regardless of any dimensions used internally.
The height is scaled in the same proportion unless
.IT optional-height
is present.
If instead the line is
.P1
\&.PS <f
.P2
the file
.CW f
is inserted in place of the
.CW .PS
line.
If
.CW .PF
is used instead of
.CW .PE ,
the position after printing is restored to what it was
upon entry.
.PP
In no case will the picture be larger than
.CW maxpsht
\(mu
.CW maxpswid .
.SH
Elements
.PP
An
.IT element-list
is a list of elements (what else?);
the elements are
.P1
\f2element\fP:
	\f2primitive  attribute-list\fP
	\f2placename\fP : \f2element\fP
	\f2placename\fP : \f2position\fP
	\f2var\fP = \f2expr\fP
	\f2direction\fP
	{ \f2element-list\fP }
	[ \f2element-list\fP ]
	for \f2var\fP = \f2expr\fP to \f2expr\fP by \f2expr\fP do { \f2anything\fP }
	if \f2expr\fP then { \f2anything\fP } else { \f2anything\fP }
	copy \f2file\fP
	copy thru \f2macro\fP
	copy \f2file\fP thru \fPmacro\fP
	sh { \f2commandline\fP }
	print \f2expr\fP
	reset \f2optional var-list\fP
	\f2troff-command\fP
.P2
.PP
Elements are separated by newlines or semicolons;
a long element may be continued by ending the line with a backslash.
Comments are introduced by a
.CW #
and terminated by a newline.
.PP
Variable names begin with a lower case letter;
place names begin with upper case.
Place and variable names retain their values
from one picture to the next.
.PP
The current position and direction of motion are saved upon entry
to a
.CW {...}
block and restored upon exit.
.PP
Elements within a block enclosed in
.CW [...]
are treated as a unit;
the dimensions are determined by the extreme points
of the contained objects.
Names, variables, and direction of motion within a block are local to that block.
.PP
.IT troff-command
is any line that begins with a period.
Such a line is assumed to make sense in the context where it appears;
accordingly,
if it doesn't work, don't call.
.SH
Primitives
.PP
The primitive objects are
.P1
\f2primitive\fP:
	box
	circle
	ellipse
	arc
	line
	arrow
	spline
	move
	\f2text-list\fP
.P2
.CW arrow
is a synonym for
.CW line
.CW -> .
.SH
Attributes
.PP
An
.IT attribute-list
is a sequence of zero or more attributes;
each attribute consists of a keyword, perhaps followed by a value.
.P1
.ta .5i 2.5i
\f2attribute\fP:
	h(eigh)t \f2expr\fP	wid(th) \f2expr\fP
	rad(ius) \f2expr\fP	diam(eter) \f2expr\fP
	up \f2opt-expr\fP	down \f2opt-expr\fP
	right \f2opt-expr\fP	left \f2opt-expr\fP
	from \f2position\fP	to \f2position\fP
	at \f2position\fP	with \f2corner\fP
	by \f2expr, expr\fP	then
	dotted \f2opt-expr\fP	dashed \f2opt-expr\fP
	chop \f2opt-expr\fP	->  <-  <->
	invis	solid
	fill \f2opt-expr\fP	same
	\f2text-list\fP	\f2expr\fP
.P2
Missing attributes and values are filled in from defaults.
Not all attributes make sense for all primitives;
irrelevant ones are silently ignored.
The attribute
.CW at
causes the geometrical center to be put at the specified place;
.CW with
causes the position on the object to be put at the specified place.
For lines, splines and arcs,
.CW height
and
.CW width
refer to arrowhead size.
A bare
.IT expr
implies motion in the current direction.
.SH
Text
.PP
Text is normally an attribute of some primitive;
by default it is placed at the geometrical center of the object.
Stand-alone text is also permitted.
A
.IT text-list
is a list of text items; a text item is
a quoted string optionally followed by positioning requests:
.P1
\f2text-item\fP:
	"..." \f2positioning ...\fP
	sprintf("\f2format\fP", \f2expr\fP, \f2...\fP) \f2positioning ...\fP
\f2positioning\fP:
	center  ljust  rjust  above  below
.P2
If there are multiple text items for some primitive,
they are centered vertically except as qualified.
Positioning requests apply to each item independently.
.PP
Text items can contain
.IT troff
commands for size and font changes, local motions, etc.,
but make sure that these are balanced
so that the entering state is restored before exiting.
.SH
Positions and places
.PP
A position is ultimately an
.IT x,y
coordinate pair, but it may be specified in other ways.
.P1
\f2position\fP:
	\f2expr, expr\fP
	\f2place\fP \(+- \f2expr, expr\fP
	\f2place\fP \(+- ( \f2expr, expr\fP )
	( \f2position\fP,\f2 position\fP )
	\f2expr\fP \f2[\fPof the way\f2]\fP between \f2position\fP and \f2position\fP
	\f2expr\fP < \f2position\fP , \f2position\fP >
	( \f2position\fP )
.sp .5
\f2place\fP:
	\f2placename\fP \f2optional-corner\fP
	\f2corner\fP of \f2placename\fP
	\f2nth\fP \f2primitive\fP \f2optional-corner\fP
	\f2corner\fP of \f2nth\fP \f2primitive\fP
	Here
.P2
An
.IT optional-corner
is one of the eight compass points
or the center or the start or end of a primitive.
.P1
\f2optional-corner\fP:
	.n  .e  .w  .s  .ne  .se  .nw  .sw  .c  .start  .end
\f2corner\fP:
	top  bot  left  right  start  end
.P2
Each object in a picture has an ordinal number;
.IT nth
refers to this.
.P1
\f2nth\fP:
	\f2n\fPth
	\f2n\fPth last
.P2
Since barbarisms like
.CW 1th
and
.CW 3th
are barbaric,
synonyms like
.CW 1st
and
.CW 3rd
are accepted as well.
.SH
Variables
.PP
The built-in variables and their default values are:
.P1
.ta .5i 2.5i
	boxwid 0.75	boxht 0.5
	circlerad 0.25	arcrad 0.25
	ellipsewid 0.75	ellipseht 0.5
	linewid 0.5	lineht 0.5
	movewid 0.5	moveht 0.5
	textwid 0	textht 0
	arrowwid 0.05	arrowht 0.1
	dashwid 0.1	arrowhead 2
	maxpsht 8.5	maxpswid 11
	scale 1	fillval .3
.P2
These may be changed at any time,
and the new values remain in force from picture to picture until changed again
or reset by a
.CW reset
statement.
Variables changed within
.CW [
and
.CW ]
revert to their previous value upon exit from the block.
Dimensions are divided by
.CW scale
during output.
.SH
Expressions
.PP
Expressions in
.IT pic
are evaluated in floating point.
All numbers representing dimensions are taken to be in inches.
.EQ
delim $$
.EN
.P1
\f2expr\fP:
	\f2expr\fP \f2op\fP \f2expr\fP
	- \f2expr\fP
	! \f2expr\fP
	( \f2expr\fP )
	variable
	number
	\f2place\fP .x
	\f2place\fP .y
	\f2place\fP .ht
	\f2place\fP .wid
	\f2place\fP .rad
	sin($expr$)  cos($expr$)  atan2($expr,expr$)  log($expr$)  exp($expr$)
	sqrt($expr$)  max($expr,expr$)  min($expr,expr$)  int($expr$)  rand()
\f2op\fP:
	+  -   *  /   %   ^
	<  <=  >  >=  ==  !=  &&  ||
.P2
.SH
Definitions
.PP
The
.CW define
and
.CW undef
statements are not part of the grammar.
.P1
\f2define\fP:
	define \f2name\fP { \f2replacement text\fP }
\f2undef\fP:
	undef \f2name\fP
.P2
.EQ
delim off
.EN
Occurrences of
.CW $1 ,
.CW $2 ,
etc.,
in the replacement text
will be replaced by the corresponding arguments if
.IT name
is invoked as
.P1
\f2name\fP(\f2arg1\fP, \f2arg2\fP, ...)
.P2
Non-existent arguments are replaced by null strings.
.IT Replacement
.IT text
may contain newlines.
The
.CW undef
statement removes the definition of a macro.
.SH
The .PS and .PE Macros
.PP
This is the default definition of the
.CW .PS
and
.CW .PE
macros:
.P1
 .de PS	\e" start picture; $1 is height, $2 is width, in inches
 .sp .3
 .in (\e\en(.lu-\e\e$2)/2u
 .ne \e\e$1
 ..
 .de PE	\e" end of picture
 .in
 .sp .6
 ..
.P2
.SH
Summary of Features Added Since Original Version
.PP
The built-in functions
.CW sin ,
.CW cos ,
.CW atan2 ,
.CW log ,
.CW exp ,
.CW sqrt ,
.CW max ,
.CW min ,
.CW int ,
and
.CW rand
are available.
.PP
The
.CW copy
statement includes data from a file or that follows immediately:
.P1
copy "\f2filename\fP"
copy thru \f2macro\fP
copy "\f2filename\fP" thru \f2macro\fP
.P2
The
.IT macro
may be either the name of a defined macro, or the body of a macro
enclosed in braces.
If no filename is given,
.CW copy
copies the input until the next
.CW .PE .
.PP
The
.CW for
and
.CW if
statements provide loops and decision-making:
.P1
for \f2var\fP=\f2expr\fP to \f2expr\fP by \f2expr\fP do { \f2anything\fP }
if \f2expr\fP then { \f2anything\fP } else { \f2anything\fP }
.P2
The
.CW by
and
.CW else
clauses are optional.
The
.IT expr
in an
.CW if
may use the usual relational operators
or the tests
.IT str1
.CW ==
(or
.CW != )
.IT str2
for two quoted strings.
.PP
The
.CW sh
command executes any sequence of commands, after expanding any defined names:
.P1
sh { \f2anything\fP }
.P2
.PP
In all of the above,
any single character that does not occur within the body
may be used to enclose a body,
instead of
braces, as in
.P1
define name X replacement text X
.P2
.PP
Text strings are first-class citizens:
a statement beginning with a sequence of text strings is treated
as an invisible box with the strings positioned at its center.
The variable
.CW textht
and
.CW textwid
may be set to any values to control positioning.
The height of \f2n\fP such strings is \f2n\fP \(mu
.CW textht .
.PP
It is possible to convert expressions to formatted strings:
.P1
sprintf("\f2format\fP", \f2expr\fP, \f2expr\fP, ...)
.P2
is equivalent to a quoted string in any context.
Variants of
.CW %f
and
.CW %g
are the only sensible format conversions.
.PP
Built-in variables may be reset to default values with
.CW reset .
.PP
The
.CW undef
statement removes a macro definition.
.PP
Arrowheads may be filled with the 
.CW arrowhead
variable;
their positioning is improved on arcs.
.PP
The width and height of the generated picture
may be set independently from the
.CW .PS
line;
the maximum height and width are set from
.CW maxpsht 
and
.CW maxpswid .
.PP
The state of
.IT troff 's
fill or no-fill mode is preserved around a picture.
.PP
Input numbers may be expressed in
.CW E
notation.
.PP
Numerous internal changes have been made as well.
Any number of objects, text strings, etc., may be used;
the only limit is total available memory.
Output is now produced in inches instead of units
for a particular typesetter,
so the 
.CW -T
option has gone away.

.so ../ADM/mac
.XX cip 175 "Cip User's Manual"
....TM 82-11276-1 11173 39199-11
....ND March 19, 1982
.EQ
delim $$
.EN
.TL
Cip User's Manual:
.br
One Picture is Worth a Thousand Words
.AU "MH 2C-510" 7448
Sally A. Browning
.AI
.MH
.AB
.I Cip
is an interactive drawing system for the Blit
|reference(locanthi spe blit)
bitmap display terminal designed by Bart Locanthi and Robert Pike.  
It provides a pallette of 
.I pic
|reference(latest pic)
shapes (box, circle, ellipse, line, arc, spline, and text)
for drawing and editing pictures on a screen.  There is a macro
facility for treating a collection of drawn shapes as a single entity.
The principal
input device is a 
.I mouse
that provides positional information.  It has three buttons that invoke
drawing, editing, and command functions.
The keyboard is used only for typing in text 
strings and filenames.  The pictures are saved as 
.I pic
descriptions, and can be included with text,
tables, and equations as input to 
.I troff |reference(latest reference troff).
Existing
.I pic
descriptions of pictures can be displayed and edited as well.  The picture
below was drawn with
.I cip.  
.PS <blit.pic
.AE
.2C
.NH
The Screen
.PP
The screen is divided into three areas.  The pallette of 
.I pic 
shapes is shown at the top of the screen.  The middle and major part of
the screen is the drawing arena. At the bottom of the screen, there is room
for typing text strings and error messages, and a 
box describing the mouse button functions.  There may be a
box showing edit depth:  it appears only when you are editing a macro.
Figure 1 shows the screen immediately after
.I cip 
has been loaded into the Blit.  
(Figure 1 was drawn with 
.I cip,
of course.)
.1C
.KF
.PS <screen.pic
.SP 0.2 exactly
.ce
\f3Figure 1.\fR  The \fIcip\fP screen
.KE
.2C
.PP
The drawing area is about 8 inches square.  In the spirit of "what you
see is what you get", 
.I cip
scales the picture to appear on paper as it does on the screen.
The exception is text:  the
font selection on the Blit
is meager compared to the typesetter, and I make
no attempt to understand
.I troff .*
.FS
*This paper describes early versions of
.I cip .
The latest version (3.0) of
.I cip ,
widely known as
.I xcip ,
does display and edit text in its
correct size and font.
.FE
Thus,
.I eqn
and
.I troff
commands appear
literally instead of as they will appear on paper, and all text appears
in the same boring font and size.  If you want to shrink the picture after
drawing it to fill the screen, you need only change one number in the
.I pic
description.  This, and other advantages of having a textual description of
the picture, are discussed later.
........
.NH
Using the Mouse
.PP
The mouse is the primary input device to
.I cip.  
It is a red hemispheric device that fits comfortably into the
palm of your hand, with three fingers positioned on the function 
buttons.  The cursor displayed on the screen will move as you move the mouse.
The mouse reports 
.I relative
positions based on the movements of the steel ball in the bottom that moves
along the table surface.  If you pick up the mouse and put it down somewhere
else without causing the ball to rotate, the cursor won't move.  This feature
is handy if you don't have much clear space on your desk.
In addition to providing positional information, the mouse has three buttons
that represent command functions.  In general, the first (left) button is 
used for selection, the second (middle) for drawing and editing shapes,
and the third (right) for menu selection.  In each case, both the down and up 
motions of the button are significant.  When I describe button motions, I will
use 
.I push
to describe the press and hold action and
.I release
to mean letting go of the button.  
.I Clicking
a button means pushing and releasing the button in quick succession.
.PP
At the bottom of the screen is a box labeled "Mouse Buttons".  It contains
three smaller boxes that represent the three buttons on the mouse.  The boxes
are labeled to reflect the 
.I current
set of functions associated with the mouse buttons.  For example,  Figure 1
shows the initial set: (select,,menu).  Notice that the second button is
meaningless in this state.  The second button invokes actions on a selected
object, and thus is meaningful only when something (either a template
from the top row of the display or a previously drawn object) has been selected.
There are other occasions when the right and left buttons are meaningless, and 
even occasions when all of the buttons are ignored.  Table 1 shows the
possible button function triples and what states they are associated with.
Figure 2 shows the possible transitions between triples of mouse button
functions.
.PP
The mouse button functions are displayed for at least two reasons. First, they
make it easier to get acclimated to 
.I cip
and to using a pointing device as the primary input device.  Second, they
help the more experienced user determine the current state.
.1C
.KF
.PS <mouse.pic
.RT
.SP 0.2 exactly
.ce
\fBFigure 2.\fR  Transition graph for mouse button functions
.TS
center box;
c | l | l | c | l.
State	Button Functions	Menu	Next State	How to Get to Next State
=
1	(select,,menu)	command	4	select drawn object (not text or macro)
\^	\^	\^	_	_
\^	\^	\^	5	select a template
\^	\^	\^	_	_
\^	\^	\^	6	choose \fBdefine macro\fR
\^	\^	\^	_	_
\^	\^	\^	7	choose \fBget file\fR or \fBput file\fR
_
2	(select,copy,menu)	object	1	select nothing
\^	\^	\^	_	_
\^	\^	\^	3	select a text string or a macro instance
\^	\^	\^	_	_
\^	\^	\^	4	select an object (not text or macro)
\^	\^	\^	_	_
\^	\^	\^	5	select a template
_
3	(select,move,menu)	object	1	select nothing
\^	\^	\^	_	_
\^	\^	\^	2	choose \fBcopy\fR
\^	\^	\^	_	_
\^	\^	\^	4	select an object (not text or macro)
\^	\^	\^	_	_
\^	\^	\^	5	select a template
_
4	(select,edit,menu)	object	1	select nothing
\^	\^	\^	_	_
\^	\^	\^	2	choose \fBcopy\fR
\^	\^	\^	_	_
\^	\^	\^	3	select a text string or a macro instance
\^	\^	\^	_	_
\^	\^	\^	5	select a template
_
5	(select,draw,menu)	command	3	select a text string or a macro instance
\^	\^	\^	_	_
\^	\^	\^	4	select a drawn object (not text or macro)
\^	\^	\^	_	_
\^	\^	\^	6	choose \fBget file\fR or \fBput file\fR
\^	\^	\^	_	_
\^	\^	\^	7	choose \fBget file\fR or \fBput file\fR
\^	\^	\^	_	_
\^	\^	\^	8	start drawing a spline
_
6	(,draw box,)		1	finish drawing box around macro
_
7	(,,,)		1	finish reading or writing file
\^	\^	\^	_	_
\^	\^	\^	5	finish reading or writing file
_
8	(,draw,end spline)		5	finish drawing a spline
_
.TE
.SP 0.2 exactly
.ce
\fBTable 1.\fR  Mouse button functions
.KE
.2C
.NH
Selection
.PP
The leftmost button on the mouse selects either a template from the
row at the top of the screen or an object in the drawing arena.  Templates are
selected by positioning the cursor inside the box surrounding the desired 
shape, and clicking button 1.  The selected shape will be 
displayed in reverse video (light on black background) until something 
else is selected.  Drawn objects are selected by positioning the cursor within
the bounding box of the desired object, and pushing button 1.  The selected 
object will vanish as long as the button is held down, reappearing when the
button is released.  This vanishing act alerts the user to which object has 
been selected.  If it is not the desired one, reposition the mouse and try
again.  It may require several attempts to select an object,
particularly in an area of the screen that is congested. An object stays
selected until either another object or a template is selected.
.PP
To "unselect" an object, either select a template or move to a blank area of the
screen, push button 1 and select nothing.  The unselect was successful if the
middle mouse button function is either "draw" or null and the third button
function is "menu".  A selected object must be unselected
(by selecting either nothing or a template) to invoke the command
menu rather than an object menu.  Menus are described next.
.NH
Menus
.PP
The right button on the mouse displays menus of commands and
refinements.  Before describing the contents of the menus, let me describe 
how they work.
.PP
A menu is displayed at the location of the cursor when button 3 is pushed down.
The display is visible as long as the button is held down.  The menu is a list 
of words, each representing a command or transformation.  When the menu appears
on the screen, one of the entries is highlighted in reverse video.  If button
3 were released, this is the menu item that would be selected.  As the cursor
moves up and down in the menu, the highlighted entry changes.  If the cursor
moves outside the menu, nothing is highlighted.  The entry that is highlighted 
when the third button is released is the chosen one.  (If no item is chosen,
nothing happens.  This allows you to leave a menu without choosing an entry.)
.PP
There are two classes of menus.  The
.I 
command menu
.R
appears whenever no drawn object is selected and button 3 is pushed.  It
contains commands that affect the whole screen or some part of it, but not
a specific single object.  For example, 
.B 
reverse video
.R
flips the sense of
the screen.  If the screen showed black objects on a light background (the
initial state), it will be flipped to show light objects on black.  This
entry is a flip flop: selecting it an even number of times is equivalent
to not selecting it at all.  Another entry in the command menu is 
.B grid.
Selecting this entry causes a grid of dots to be displayed on the screen,
and future drawing is constrained to the grid points.  The command is also
a flip flop: to turn it off, select it again.  Other commands are
.B
clear screen, redraw screen, define macro, get file, put file,
.R 
and
.B
quit. 
.R
The macro and file
commands are described later; the others are self-explanatory.
.PP
If a drawn object is selected when the menu button is pushed, a menu with
entries that in some way transform the selected object is displayed.  The
contents of this menu are tailored to the class of object selected.  For
example, instances of circles, ellipses, and text can be copied or deleted.
Arcs and splines can be copied, deleted, or reflected around either the
x or y axis of their bounding box.  Boxes can be copied, deleted, 
or have their boundary be dotted, dashed, or solid.  Lines can be copied,
deleted, reflected, dotted, dashed, solid, or have an arrowhead attached to
the endpoint closest to the cursor.  The arrow 
option is a flip flop: if
the nearer endpoint already has an arrowhead, selecting 
.B arrow
again will remove it. Splines can also have arrowheads attached to either or
both ends.
.PP
Remember, if the menu button is depressed accidentally, leave it down long enough
to move the cursor outside the menu!
.PP
Figure 3 shows both command and object menus being used. In the first three
frames, the menu associated with splines is displayed and the spline is reflected
around the y-axis.  The command menu is invoked in the remaining five frames to
get a new file.  The menu is initially displayed with 
.B 
define macro
.R
highlighted; the mouse is moved up to select
.B
get file
.R
before the third button is released.
.1C
.KF
.sp 1
.PS <menus.pic
.SP 0.2 exactly
.ce
\fBFigure 3.\fR  Using menus
.KE
.2C
.NH
Drawing
.PP
The
.I pic
language can describe and position 
circles, boxes (solid, dotted, and dashed), ellipses,
lines (solid, dotted, dashed, with or without arrowheads), circular arcs,
splines, and text.  In addition, there is a mechanism for treating a
.I collection
of objects as a single entity, hereafter called a 
.B macro.
.NH 2
Drawing New Shapes
.PP
There are two ways to draw shapes on the screen.  First, select a template with
button 1.
Button 2 can be used
to draw objects of the selected shape on the screen.  The location and size of
the drawn object is determined by the down and up motions of the button.  For
example, to draw a circle, select the circle template, move the cursor to the desired position for
the 
.I center
of the circle, and push down on the middle button.  As long the button is
depressed, the trial radius of the circle will track the mouse movement:
as the mouse moves away from the origin, the circle will expand and
vise versa.  When button 2 is released, the circle radius is fixed as
the distance between the origin and mouse position. 
Table 2 defines the points fixed by button 2 motions for all the
shapes.
.KF
.SP 1
.ps -1
.vs -1p
.TS
box center ;
c | l | l.
Object	Push	Release
_
_
circle	center	radius = distance(push,release)
_
box	a corner	opposite corner
_
ellipse	center	height = $| roman "push" .y - roman "release" .y|$
		width = $| roman "push" .x - roman "release" .x|$
_
line	endpoint	endpoint
_
arc	endpoint	endpoint
		center 
_
spline	endpoint	point
		(subsequent releases define
		 another point, until button 3)
_
text	center of
	baseline
.TE
.ps +1
.vs +1p
.SP 0.2 exactly
.ce
\fBTable 2.\fR  Using button 2 for drawing shapes
.KE
.PP
Arcs are drawn counter-clockwise.  When an arc is drawn, it will be
either one quarter or three quarters of a circle (in imitation of 
.I pic
defaults).  If you want something else, select and edit the arc once it is 
drawn.  The picture below shows the default arcs;  the order that the endpoints
are entered determines whether one fourth or three fourths of a circle is 
drawn.
.KS
.PS <arcs.pic
.KE
.PP
Two of the shapes, splines and text, require a few more words to explain
how to draw them.  Splines are curves that
shape themselves to a framework of line segments and are drawn with repetitive
pushes of the middle button.
Each click adds a line segment to the framework of the spline.  
.I Pic ,
and thus
.I cip ,
splines will contain the first and last points entered and will be tangent to
the midpoint of
each line segment in the framework.  The spline will be 
.I near 
the endpoints of each line segment.
Only the framework is shown on the screen while you are entering the spline.
To terminate and draw the spline, push the third button.  Figure 4 shows the process
of drawing a spline.
.1C
.KF
.PS <spline.pic
.SP 0.2 exactly
.ce
\fBFigure 4.\fR  Drawing a spline
.KE
.2C
.PP
To add some text to the picture, move to the desired position for the
.I center
of the text string
and push and release button 2.  Now type the text.  It will
appear on the lower left portion of the screen, just below the drawing arena.
The backspace key can be used to make minor corrections.  As soon as, and
not until, you push carriage return, the string will be displayed in the
drawing arena, centered at the selected location.  Figure 5 pictures the
stages of drawing text.*
.FS
*At least, in older versions of
.I cip .
In version 3.0, text appears as you type it.
.FE
.1C
.KF
.PS <text.pic
.SP 0.2 exactly
.ce
\fBFigure 5.\fR  Drawing text
.KE
.2C
.NH 2
Copying Existing Shapes
.PP
The second way to draw shapes on the screen is by copying previously drawn
things.  First use button 1 to select a specific drawn object
rather than a template.  
Then push button 3 to get a menu pertinent to the selected object.
Select the menu entry
.B copy.  
Now, button 2 can be used to position exact copies of
the selected object.  The
.I origin
of the object (see Table 3) will be located at the cursor position when
button 2 is pushed.  In this case, the release of button 2 is necessary but
meaningless.
.KF
.SP
.TS
center allbox;
c l.
Object	Origin
_
circle	center	
box	upper left corner
ellipse	center	
line	first drawn point
arc	first drawn point
spline	first drawn point
text	center of baseline
macro	upper left corner
.TE
.SP 0.2 exactly
.ce
\fBTable 3.\fR  The origins of objects 
.KE
.NH 2
Alignment Aids
.PP
.I Cip
has a few features built in to make positioning objects easier.  First, lines
that are drawn 
.I nearly
horizontal or vertical will be made exactly so.  Also, each of the basic shapes
has a number of points designated 
.I sticky.
As you move the cursor near these special points, it will tend to stick to them
until you move sufficiently far away.  Figure 6 shows the sticky points for 
each shape.  The corners of the 
.I
bounding box
.R
of a macro instance are sticky as well.  
The final alignment aid is the grid.  When the grid is turned on, drawn
objects will rachet from grid point to grid point as you move the cursor.
The cursor is prejudiced in favor of sticky points: it will move to the
nearest grid point only when there is no close sticky point.
.1C
.KF
.PS <sticky.pic
.SP 0.2 exactly
.ce
\fBFigure 6.\fR  Sticky points
.KE
.2C
.NH
Editing
.PP
The editing facilities of
.I cip
allow both the position and size of previously drawn objects to be changed.  The
basic mechanism is to 
.I select
the object with button 1, and then use button 2 for the actual editing.  The
position of the cursor when button 2 is depressed and the shape of the selected 
object determine the editing action.  For example, suppose a circle
has been selected.  If the cursor is near the origin of the circle when
the middle button is pushed, the origin of the circle will track the cursor
as long as the button is depressed.  Its new location will be the cursor 
position when the button is released, and the radius will be unchanged.
If, instead, the cursor is near the edge of the circle when the middle button 
is depressed,  the center of the circle will remain fixed, and the radius of
the circle will track the cursor.  The other objects are edited in the same 
manner:  near the center moves the object, near the edge (or corner, or
endpoint) changes the size, with the exception of arcs, splines, text, and macros.
.PP
Arcs have three interesting points: the two endpoints, and the center of
the circle that contains the arc.  The point nearest the cursor when
button 2 is depressed will track the cursor.  (There is no way to 
reposition the arc.)  This includes the center of the circle, which is shown
with a pair of lines when the arc is selected.
.PP
Splines have a variable number of 
.I 
guiding points
.R
that can be changed.  Lines between the
guiding points are drawn to help in locating them when the spline is selected.
The point
nearest the cursor when the middle button is depressed will track the mouse 
motions, becoming fixed at the cursor position when the button is released.
Again, there is no way to move the whole spline.  
.PP
It doesn't make sense in 
.I cip's
world to change the size of text.  Accordingly, it can only be moved.  After
the text is selected and button 2 is pushed, the text string will track
the cursor, coming to rest when the button is released. 
The
.I contents
of a text string are changed by deleting the string and re-entering it 
correctly.*
.FS
*In version 3.0, the size, font, vertical spacing and adjustment
(left justified, right justified or centered) can be changed from the button 3 menu.
.FE
.PP
Macro editing is more complex.  An instance of a macro can be repositioned
by selecting the instance and pushing the second button.  The entire macro
will track the mouse movements until the button is released.  Changing the
.I components
of the macro will be discussed in the next section.
.PP
The editing facilities of 
.I cip
are summarized below.  Figure 7 shows the process of editing
a spline; the technique is similar for all simple shapes.
.KS
.PS
scale=100
box ht 40 wid 60 with .nw at 70,650 dashed
box ht 80 wid 120 with .nw at 40,670 
box ht 40 wid 60 with .nw at 10,610 dotted
box ht 40 wid 60 with .nw at 10,690 dotted
box ht 40 wid 60 with .nw at 130,690 dotted
box ht 40 wid 60 with .nw at 130,610 dotted
.PE
.KE
.de XY
.ti 0
..
.de XX
.br
..
.in 5n
.XY
To edit solid box:
.XX
a) cursor inside a dotted box: move corner
.XX
b) cursor inside dashed box: move box
.KS
.PS
scale=100
circle rad 29 at 96,458
circle rad 65 at 96,458
.PE
.KE
.XY
To edit outer circle:
.XX
a) cursor inside inner circle: move circle
.XX
b) cursor in ring between circles: change radius
.KS
.PS
scale=100
ellipse ht 120 wid 216 at 116,257
box ht 51 wid 65 with .nw at 6,283 dotted
box ht 51 wid 65 with .nw at 161,283 dotted
box ht 36 wid 90 with .nw at 71,232 dashed
box ht 36 wid 90 with .nw at 71,319 dashed
.PE
.KE
.XY
To edit ellipse:
.XX
a) cursor in a dashed box: change height
.XX
b) cursor in a dotted box: change width
.XX
c) cursor anywhere else in ellipse: move ellipse
.KS
.PS
scale=100
line  from 104,0 to 0,60 dotted
line  from 104,119 to 209,60 dotted
line  from 0,0 to 209,119 
box ht 119 wid 209 with .nw at 0,119 dotted
.PE
.KE
.XY
To edit solid line:
.XX
a) cursor in the dotted box and between the dotted lines: move line
.XX
b) cursor in a dotted triangle: move endpoint
.KS
.PS
scale=100
box ht 128 wid 128 with .nw at 20,702 dashed
line  from 84,574 to 84,606 dashed
line  from 84,670 to 84,702 dashed
circle rad 32 at 84,638
line  from 129,683 to 84,638 dotted
line  from 39,683 to 84,638 dotted
arc from 129,683 to 39,683 at 84,638
.PE
.KE
.XY
To edit arc:  (dotted lines appear (solid) when arc is selected)
.XX
a) cursor inside circle: move origin
.XX
b) cursor outside circle, inside left dashed box: move endpoint
.XX
c) cursor outside circle, inside right dashed box: move startpoint
.XX
.KS
.PS
scale=100
box ht 173 wid 179 with .nw at 0,485 dotted
line  from 179,372 to 104,355 dotted
line  from 98,338 to 104,355 dotted
line  from 148,448 to 74,410 dotted
line  from 179,464 to 148,448 dotted
line  from 80,453 to 64,428 dotted
line  from 64,428 to 7,424 dotted
line  from 93,485 to 80,453 dotted
line  from 85,391 to 104,355 dotted
line  from 64,428 to 85,391 dotted
line  from 85,312 to 98,338 dotted
line  from 0,485 to 140,475 dashed
line  from 170,417 to 0,485 dashed
line  from 13,369 to 179,312 dashed
line  from 170,417 to 13,369 dashed
spline from 140,475\
to 0,485\
to 170,417\
to 13,369\
to 179,312
"\(bu" at 0,485
"\(bu" at 179,312
"\(bu" at 13,369
"\(bu" at 170,417
"\(bu" at 140,475
.PE
.KE
.XY
To edit spline:  (dashed lines appear (solid) when spline is selected)
.XX
a) cursor inside a dotted region: move guiding point (large dot)
.KS
.PS
scale=100
box ht 18 wid 92 with .nw at 36,254 dotted
"Text String" at 79,241
.PE
.KE
.XY
To edit text:
.XX
a) cursor anywhere: move text
(To change the contents of the string, delete it
and re-type it correctly.)*
.FS
*In version 3.0, you can change the point size, vertical spacing,
font and adjustment of text from the menu.
.FE
.KS
.PS
scale=100
box ht 130 wid 139 with .nw at 19,130 dotted
line  from 58,71 to 92,96 
line  from 92,96 to 123,73 
line  from 43,32 to 58,51 
line  from 58,51 to 71,32 
line  from 110,33 to 123,53 
line  from 123,53 to 138,34 
circle rad 10 at 92,106
circle rad 10 at 58,61
circle rad 10 at 123,63
circle rad 10 at 43,22
circle rad 10 at 71,22
circle rad 10 at 110,23
circle rad 10 at 138,24
.PE
.KE
.XY
To move an instance of a macro:
.XX
a) cursor anywhere: move macro
.br
(Changing the components of a macro is discussed elsewhere)
.in 0
.1C
.KF
.PS <editSpline.pic
.SP 0.2 exactly
.ce
\fBFigure 7.\fR  Editing a spline
.KE
.2C
.NH
Macros
.PP
.I Cip
has a macro facility that allows objects to be grouped together and treated 
as a single entity.  The aggregate can be moved, reflected, copied, or deleted.
In addition, components of the macro can be modified, deleted, or added.  
Changes made to the components will be reflected in
.I all
instances of the macro, except those that have been reflected.
.PP
A group of objects is collected into a macro by selecting the 
.B
define macro
.R
option in the command menu.  A bounding box is drawn around the desired
objects using button 2.  Any object that is completely contained in the 
bounding box will be included in the aggregate.
.PP
Once the definition is complete, the macro instance can be selected in the 
usual manner.
The object menu for macros has entries for deleting, copying, and reflecting
the macro instance.  It can be repositioned by holding the middle button down.
The center of the bounding box will track the cursor until the button is
released.
.PP
The macro object menu has a fifth entry that allows the components of the
macro to be modified, deleted, or added.  Selection, drawing, editing, and
menus work exactly as described before except that they operate within
the context of the macro.  When editing begins, a small box appears on the 
screen to the left of the Mouse Buttons box.  It shows the current depth of
macro editing.  (Yes, Virginia, macros can be nested.)  To terminate the
editing, move the cursor inside the Edit Depth box and click the
selection button, decrementing the depth count.  When the
count goes to zero, the screen will be redrawn to reflect the changes.  
Modifications to macros affect
.I all
instances of the macro.
.PP
Detail can be added to a single instance of a macro by drawing 
over it.  Components of an individual
instance cannot be deleted or modified without changing all instances.
.PP
Suppose you define a macro that contains a circle in a box.  After
replicating it several times, you decide that one instance should not
have the circle.  The sixth entry in the macro menu,
.B separate,
will separate the aggregate back into individual objects.  That is, for a 
.I 
selected instance
.R
of a macro, the 
.B
define macro
.R 
operation will be undone.  Now you can select the offending circle and 
delete it without changing the other instances of the macro.  However, 
changes made to the macro will no longer affect the separated objects.
.NH
Saving Pictures
.PP
The command menu has two entries that involve file operations.
.B
Put file
.R 
converts the picture on the screen into 
.I pic
and saves it in a file on the host machine. Similarly,
.B
get file
.R
reads a file, presumed to be 
.I pic,
and draws the resulting picture on the screen.  Both commands ask for a
file name: the prompt
.CW FILE?
appears in the lower left part of the screen,
just below the drawing arena (see Figure 3g).  
The filename from the previous get or put
command will be automatically displayed to reduce the amount of typing 
necessary.  Typing <CR> (carriage return) uses the default filename.
.PP
There are four error conditions that may arise following a file operation.
Attempts to put a file are the least error prone:  the only error,
besides the host being down, is that you don't have the right to
write the file in question.  Get file commands are subject to
errors involving access rights to the file, problems opening a pipe to 
parse the pic, and having a picture that is too
complicated to fit within the available memory of the Blit.
Errors are reported in the lower left portion of the screen, just below
the drawing arena.
.NH
Commentary
.PP
.I Cip
grew out of my experience with 
.I pic.  
It is wonderful to be able to describe and draw pictures that can included
right along with text in a document.  However, words are not always the 
best form of expression.  It is easy to spend an afternoon 
turning a five minute sketch into a 
.I pic 
description.  
.I Cip
allows you to do the sketch online and get the 
.I pic 
description as a bonus.
.PP
I have put most of my efforts into the user interface.  There is a minimum of
typing: only file names and text strings are entered from the keyboard.  There
are alignment aids: sticky points, help with vertical and horizontal lines,
and a grid option.  The current meaning of the mouse buttons is displayed.
Only use will tell if these aids are enough.
.PP 
Before going on to a discussion of the limitations of 
.I cip,
let me mention a 
.I good
thing about saving the picture as readable text.  The 
.I pic
produced by 
.I cip
is full of numbers, including the line
.CW scale=100 .
The resolution on the Blit 
is 100 pixels/inch, and this command is necessary for the picture
to be the same size on both the screen and a piece of paper.  
Thus, the size of the picture can be changed 
by changing the scale.  For example, all of the miniature 
.I cip
screens that appear in this document originated from the big version in
Figure 1.  The screens in Figures 3, 4, 5, and 7 were created with
.CW scale=300
and are roughly one ninth the size of the original; the one
on the cover page had
.CW scale=450 .
Ragged edges and small imperfections
in the picture can be removed by twiddling the position and size of
objects.  One common way to use 
.I cip
is to draw a rough version on the screen, and refine it by using a 
combination of 
.I cip
editing and 
.I pic
editing.
.PP
.I Cip
resides almost entirely in the Blit.
(Implementations also exist for the DMD5620 and 630MTG terminals.)
The host machine provides
the file system and parses
.I pic; 
everything else is done in the terminal.  While this approach is
not subject to the delays in response time that plague timesharing users,
it does require a large program (50K bytes) to be downloaded into the
terminal through a serial connection.  It should be interesting to compare this
approach to one in which the major part of the program runs in the host, with 
the terminal providing only the graphic support.
.PP
There are some things that can be expressed in a textual language like 
.I pic
that are difficult to describe graphically.  Examples are relationships between
objects (text centered in a box that is somehow part of the box and should 
move with it), shortening a line between two circles by the radius of the 
circle, naming objects and referring to them.  Ivan Sutherland's thesis,
Sketchpad|reference(sutherland sketchpad)
dealt with the problem of expressing constraints between objects; his techniques
will be incorporated into 
.I cip
at some point in the future.
.PP
Adding functionality to 
.I cip
is constrained by having 
.I pic 
as the output form. 
Potentially interesting extensions like arbitrary rotation of objects,
shapes that are solid or shaded rather than outlined, shapes that
obscure things that are behind them, and so on, are ruled out
because they cannot be described in 
.I pic.
Chris Van Wyk's 
.I ideal |reference(latest ideal)
.I does
allow these extensions.  It might be interesting to do a drawing system
for it.
.PP
Brian Kernighan, in addition to his role as 
.I pic
oracle, provided the program that runs on the host, parses 
.I pic
files, and sends a simplified form of the picture to the Blit.
Bart Locanthi and Rob Pike built the Blit 
and wrote the graphics support and file system access routines.  And finally,
my first and most enthusiastic user was Norm Schryer.  My thanks to them all.
.NH 1
References
.LP
|reference_placement

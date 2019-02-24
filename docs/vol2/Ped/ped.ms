.so ../ADM/mac
.XX 12 195 "PED \(em A Picture Editor"
.de fC
.sp .3v
.B "Figure \\$1."
..
.TL
PED \(em A Picture Editor
.br
A User Manual
.AU
Theo Pavlidis
.AI
.MH
.SP 2
.2C
.NH
Introduction
.PP
\fIPed\fP is a program for creating illustrations interactively
using the DMD5620 (Blit) terminal and the mouse.
(A version also exists for the Sun workstation.)
It runs under
.ft I
mux (mpx, layers)
.ft
so that you can use one window for drawing while keeping in contact
with the world through the other windows.
You can create simple illustration without having to read
more than a small part of this manual
provided that you have already some familiarity with
.US
and the terminal itself.
There is on-line documentation which you can access by pressing
button 2 of the mouse over a menu item. An expanded copy of that documentation
is given as Appendix A and you can consult it anytime you wish
to find out the meaning of a particular menu selection.
Section 3 describes how to produce
hard copies from
.I ped.
Part of
.I ped
runs on the host machine and part on the terminal. Small drawings are
handled only by the terminal part but large drawings are kept on the host
and only parts of them are brought in the terminal for editing.
Figure 1 should give you an idea of what you can draw with
.I ped .
.1C
.KF bottom
.BB
.so dragon.so
.fC 1
An example of an illustration produced using \fIped\fP.
(Drawn by Paul Pavlidis.)
.KE
.2C
.PP
\fIPed\fP is part of Research
.UX
but it is also
available in many installations running System V or BSD
.UX
systems.
While it is was written originally for terminals with 256K
of memory its size has increased since then and there may
be problems in running the current version on such terminals.
.PP
While you draw on the bitmap of the terminal with the mouse
.I ped
maintains the information not as a copy of the bitmap but as
a list of points. This makes scaling easy so that
drawings produced by
.I ped
can be displayed on devices of different resolution.
(Thus while the image includes filled areas,
lines, and curves, \fIped\fP knows only about points.
As a matter of fact its name stands for \f(CWP\fPoint \f(CWED\fPitor.)
\fIPed\fP files are in ASCII and therefore can be edited using
text editors, although this is not a recommended procedure.
Whenever
\fIped\fP
exits (or is asked to edit a new file), it checks whether the file
has been modified since a previous save, and if it has it writes in
.CW ped.save .
Error messages (if any) are saved in a file called
.CW .pederr
in the user's current directory.
.PP
While 
.I ped
is a currently available program, I consider it only as a first step
towards the development of a tool that would be as convenient to
use as writing and drawing on paper.
I would like to call such a facility
.ft I
electronic paper
.ft
and it is a concept that goes in the opposite direction from picture
description languages (such as \fIpic\fP|reference(latest pic)) or constraint systems
(such as \fIideal\fP|reference(latest ideal) or Xerox PARC's \fIjuno\fP).
The hope is that future versions of
.I ped
will provide more options without increasing the size of the menu or
the documentation.
See Section 5 (Beautification) for more on this topic.
.NH
Organization of PED
.NH 2
Points, Objects, and Families
.PP
The basic unit or data structure
.I ped
is the
``object.''
As a rule objects appear as connected polygonal lines,
splines, circles, or blocks of text.
(See Appendix B for a formal definition of objects.)
Internally they consist (usually) of a set of points
and a ``type'' flag which determines how
the plotting routines will
use the points to produced various shapes.
.ft I
An important consequence
is that you must address objects on the screen
by pointing at their points rather than at sides or the interior
of polygons.
.ft
.PP
We could have added code to
.I ped
that would relax the above requirement so that people could
address polygons by pointing to
their interior, etc. We did not do so for two reasons.
One is that such code would increase the size of the program
and the time of interaction in a nontrivial way. Since users
adapted to the current mode quickly we felt that the slow
down caused by the new code would not be justified.
A second reason is that addressing the
interior of polygons would not be a useful action in a cluttered drawing.
.PP
Objects can become
associated in a ``family'' without changing their external
appearance. We describe later the menu selections that can do that.
When you touch a point
.I ped
also knows about the object that the points belongs to and,
if appropriate, an action is applied to the whole object.
If that object is a member of a family, then the action is applied
to the whole family.
.NH 2
The Menus
.PP
.I Ped
displays a menu on the left side of the window. That menu may be changed
by selecting from a ``pop up'' menu with button 3 of the mouse. The following
selections are possible.
.IP "\f(CWbasic\fP: "
This is the active menu in the beginning and includes commands for
file manipulation, changing the type of display, etc.
(See Figure 1.)
.IP "\f(CWch size\fP: "
Commands that change the size of what has been drawn.
Also transformations such as rotation.
.IP "\f(CWmove\fP: "
Commands that delete, copy, do parallel translations
(move) objects, etc. Also commands that break objects in parts or join
objects to form new ones.
.IP "\f(CWdraw\fP: "
Set up a grid, draw lines, text labels, circles, boxes, etc.
Also make a family out of a group of objects.
.IP "\f(CWreshape\fP: "
Convert a polygon into a spline, adjust the position of points, 
dissolve a family of objects, or restore it, etc.
(See Figure 2.)
.IP "\f(CWrefine\fP: "
Edit text, line form (dashed, solid, double, etc.), change point size, etc.
.IP "\f(CWshade\fP: "
Fill polygons or splines with texture or unfill them.
.IP "\f(CWcolor\fP: "
Assign a color to objects. This has no visible effect on the DMD 5620
but the color label may be used by other devices.
.IP "\f(CWremote\fP: "
Commands to perform editing on the host.
.1C
.KF
.BB
.so mvpoint.so
.fC 2
The screen of the DMD 5620 when \fIreshape\fP has been
selected from the pop up menu by button 3 of the mouse
and then \fImv point\fP from the left side menu by button 1.
.KE
.2C
.PP
As a rule, the program remembers the last command given and will
execute it repeatedly, as long as it is meaningful to do so
or until the user makes another selection.
Thus one can fill many polygons with the same texture without
having to select it each time. In this case the mouse cursor
has the form of the texture. In other menus
the effective command is shown in reverse video.
.NH 2
The Mouse Buttons
.PP
Button 1 is the ``creative'' or ``start'' button. It is used to select
actions from the menu displayed on the left hand side, to draw
or edit objects, and, in general,
pretty much as pencil could be used to draw or point
to things.
.PP
Button 3 is the ``terminating'' button. When pressed it stops drawing
or editing, or it may cancel some commands. It is also used to
change the menu on the left.
.PP
Button 2 is the ``panic'' button. When pressed over a menu item,
it causes the display of documentation. When pressed elsewhere,
it cause the
.I basic
menu to be displayed on the left.
.PP
Typing text disables the mouse so
that you cannot exit from that mode by pressing button 3.
However, hitting RETURN twice will take you out.
.NH 2
Files
.PP
When a file is first read it is kept on the host.
It requires user action to bring it to the terminal so that
it can be edited (``bring in'' item on the ``basic'' menu).
``View host'' lets you display the complete picture
that is the file and is now kept in the host; no data are kept in the terminal
part and therefore you cannot edit the file (except by the ``remote''
menu commands.)
Since large figures will not usually fit in the terminal, you
must display them (scaled down) first with the ``view host'' command and
then select a part with a window controlled by button 1.
After selecting ``bring in'' from
a menu the black square cursor appears. Clicking button 1
fixes the top left corner, clicking button 1 a second time fixes the
bottom right corner of a rectangle.
(Note that this is different from the way that windows are controlled
under the multiplexing program
with button 3.)
Any objects found within that rectangle
are brought in and displayed on the terminal,
and at a bigger scale. Then they can be edited.
.PP
A unique feature of
.I ped
is that you may read a file into a particular part of
the screen (``read'' command in the ``remote'' menu), or write a
part of the screen into a file (``write'' command in the ``remote'' menu).
.NH
Hard Copy
.PP
There are two ways to make hard copy of the pictures you draw,
either by copying bitmaps from the 5620 screen or by expanding
the
.I ped
files by the
.I troff
preprocessor
.I tped.
.NH 2
Bitmaps
.PP
You can get bitmaps by selecting ``blitblt'' from the basic menu (see Figure 1)
and then sweeping a rectangular area from the screen.
This action will
create a file called BLITBLT which is a run length encoded
copy of the bitmap. Of course, you can run the
.US
.I blitblt
command from another window if you wish with the same result.
If the rectangle has size zero (or nearly so) or button 3 is pressed,
then a copy of the whole drawing are is made.
The BLITBLT file can be printed on the Gnot
printers with the \fIgbcan\fP command or included in \fItroff\fP
files by using the \fImbits\fP macros.
(This is how all the pictures of the ped screen in this document were made.)
The action of the library macro BM depends on the number of its arguments.
The
.CW .bM
macro defined below can be used either as
.P1
\&.bM fname
.P2
to print the bitmap of file \fIfname\fP with default setting of dot size (6)
or as
.P1
\&.bM number fname
.P2
to print with dot size \fI number\fP.
.P1
\&.de bM
\&.sp
\&.ie \e\en(.$=1 .BM \e\e$1 6 dX dY
\&.el .BM \e\e$2 \e\e$1 dX dY
\&.br
\&.ne \e\en(dYu
\&.in (\e\en(.lu-\e\en(dXu)/2u
\&.ie \e\en(.$=1 .BM \e\e$1 6
\&.el .BM \e\e$2 \e\e$1
\&.in
\&.rs
\&.sp \e\en(dYu
\&..
.P2
.NH 2
Tped
.PP
This program works like other
\fItroff\fP preprocessors with the following specifications.
It is run as
.P1
tped \fIfiles\fP ... | troff \fIoptions\fP
.P2
The various files can be \fIped\fP files without any modifications.
Or they may be \fItroff\fP input files
including \fIped\fP files in one of the following ways.
.IP (a)
The \fIped\fP file is embedded in the \fItroff\fP file
between
.P1
\&.GS
\&.....
ped file contents
\&.....
\&.GE
.P2
.IP (b)
The name of the \fIped\fP file is given as argument as in
.P1
\&.GS file_name
.P2
.PP
.I tped
recognizes the following flags:
.IP \f(CW-b\fP 6n
place a box around each illustration.
.IP \f(CW-c\fP
produce expanded output.
.IP \f(CW-Ta\fP
assume that final output will be done in
the \fIaps\fP phototypesetter.
.IP \f(CW-Td\fP
assume final output on the Mergenthaler
or the Gnot printers (default).
.IP \f(CW-m\fP
if used after a set of file names, it causes \fItped\fR
to read from the standard input after reading the files.
.IP \f(CW-s\fP
scale the letter point size.
.PP
Two of these flags require special discussion.
When
.I tped
expands the
.I ped
files it can do in one of two ways. It may produce a series of dots
that form the lines, textured areas, etc. or it may simply pass on
the object description with slight modifications only. Then the
plotting is done by the
.I troff
postprocessor.
Plots obtained in this way are of better quality than plots when
the expansion is done before
.I troff.
However the postprocessors must know about
.I ped
so that version is less portable. (See Appendix C.)
The
.CW -c
flag allows the user to select either option.
.PP
In addition to the above, it is possible to change parameters
with arguments supplied with the
.CW .GS
line. The following options exist:
.KS
.TS
center;
cFCW l.
h=\fInumber\fP	Make drawing height \fInumber\fP inches.
w=\fInumber\fP	Make drawing width \fInumber\fP inches.
-C	Center drawing on page (default).
-L	Left adjust drawing.
-R	Right adjust drawing.
-s	Toggle scaling of text.
.TE
.KE
The adjustment options can be used to print
.I ped
drawings side by side as the following example illustrates.
.P1
\&.sp
\&.ne 4i
\&.sp
\&.mk
\&.GS \-L left_part
\&.sp
\&.rt
\&.GS \-R right_part
\&.sp
.P2
The above arrangement assumes that both parts have the same dimensions.
If not, similar heigh may be forced with the \f(CWh=number\fP option.
.NH
Special Features
.PP
All the features of
.I ped
are documented systematically in Appendix A. We review here only
those that we felt they needed a more detailed explanation.
.NH 2
Markers
.PP
Sometimes it is not easy to identify the points of an object
and for this reason \fIped\fP provides the ``markers'' selection
in its ``basic'' menu. If you select that option, then various auxiliary lines
and points appear on the screen. In particular, a ``bullet''
is used to identify one point on each object,
dotted lines connect objects belonging to the same family,
and a simplified drawing is used for some
of the fancier objects. 
The use of markers is recommended during preparation
of an illustration and you should turn them off only when
you want to see how your finished product will look.
For example, a text label can be addressed only by placing
the mouse at the particular point associated with the label.
That point is not visible unless the markers are on.
.NH 2
Text
.PP
Text can be included by selecting ``text''
in the ``draw'' menu and then typing it. Text labels
consisting of many lines. To terminate text entry type an
empty line. When text is entered at first it appears as
left adjusted. The ``refine'' menu has a command that lets you
make text centered or right adjusted. In addition you can edit
text by using the command ``edit text'' in the ``refine''
menu. The selected text appears at the top area of the screen.
Click button 1 over the spot you wish to change.
Use back space to erase and type new characters to replace
or insert. Hitting return
completes the correction at the selected spot.
You may make more corrections if you wish
by selecting another spot on the label. Otherwise you hit button 3
to indicate that text editing is over.
.PP
It is not possible to edit in
.I ped
labels that are too long to fit in the top area of the window
(over 80 characters for windows that take the full width of the terminal).
This is perhaps the only instance that a
.I ped
file must be edited with a text editor.
.PP
Text is normally entered in point size 10, but the ``psize''
lets specify another size.
All new text entered from then on will be in the selected point size.
The commands ``expand'' and ``shrink'' of the ``ch size'' menu
result in a change of the point size when applied to text.
.NH 2
Splines
.PP
The splines used in
.I ped
consist of parabolic arcs tangent to a polygon drawn by the user.
They are the same splines that
.I pic
uses and they are discussed in detail in |reference(pavlidis image processing).
.PP
Splines pass through the vertex of the polygon only if that vertex
is made a ``corner'' by the ``reshape'' menu selection.
The first and last points of an open polygon are made by default into
corners when it is converted into a spline.
If you wish interpolating splines you must use the command ``isplin''
in the ``refine'' menu. In order to let the user have access to the
original points the type of the figure is not changed: it is a polygon
but it is drawn as a spline.
.NH 2
Line Forms
.PP
Polygons or splines may be drawn either by solid or dashed lines
in different thickness as specified by the line point size.
In addition polygons may use double lines, heavy lines, and interpolating
splines (see above). The last three forms require
floating point arithmetic so they are not displayed in local
mode but only when you do a ``view host.'' (In local mode the
sides are marked with a symbol.) Such fleeting drawings forms may
be fixed so that the objects appear in tha form even in local mode.
However in this case the original points are lost. See Figure 3
for an example.
.1C
.KF
.GS h=2 fig1
.fC 3
The figure displays the original polygon,
the interpolating spline, and the guiding polygon for the latter.
The ``fix'' command replaces the original polygons by the guiding
polygon.
.KE
.2C
.NH 2
Families and Links
.PP
Objects may be associated in a ``family''
so that they can be moved around or scaled as a group.
The ``draw'' menu has a command ``family/bx.'' When you select it
a sweeping rectangle becomes available and any objects enclosed
by it become members of the same family. If markers are on,
.I ped
places dotted lines connecting objects of the same family.
.PP
Families are groups of objects that have nothing in common
except that
.I ped
thinks of them as a single object. They can have
different types and a family may be
a member of another family.
On the other hand
linked objects must be of the same kind: all open polygons,
or all closed splines, etc.
.PP
When objects are linked
the visual appearance of the sketch does not change.
This is similar to a family but the
link is far stronger and the ``dissolve'' command has no effect.
On the other hand linked objects can be edited the same way
as simple objects.
.ft I
Warning: \fRPed\fP provides no visible connection between linked objects.
Link should be used only between objects that are obviously related.
.ft
Indeed, the command was introduced with the following two
applications in mind. (a) To prepare filled
polygons that have holes in them.
(In this case ``link'' should be followed immediately by a shading command.)
(b) To
create a single object after the ``match'' instruction has been
applied.
In this way a T-shaped figure (or a tree) could become a single object.
The ``break'' operation has special effects on linked objects.
It simply removes the first link it encounters and breaks the polygon
into two. At least one of them has no links at all.
.I Ped
actually refuses to link splines, but you can defeat that restriction by
linking first, and then converting into a spline.
Application of ``join'' to an open polygon with links converts all
its contours into closed polygons.
.PP
Users should keep in mind that the ``remote'' menu allows operations
on groups of objects so there is no need to make the whole drawing
a family in order to apply an operation on all the objects.
.NH 2
Planning and Clutter
.PP
A major difference between drawing on paper and
drawing by electronic means is that you do not have to worry about the
composition until you are ready to do so.
Components of the illustration can and should be drawn separatedly
and pulled together only at the end. Large drawings may be in separate files
and be composed through the ``read'' command of the ``remote''
menu. The ``match'' and ``center'' commands of the ``move'' menu
allow fine positioning adjustments.
.PP
Temporary moves are useful for reducing clutter.
If you select ``mv obj'' and then press button 3 (or 2)
the object is left in the chosen position only temporarily.
Next time you return to the basic menu, all such objects
resume their original positions.
.NH 2
Equations
.PP
Mathematical equations can be included in \fIped\fP drawings by
typing
.I eqn
commands between delimiters (e.g.
.CW $$ )
as text
and then piping the output of
.I tped
through
.I eqn.
Since the form of displayed labels is different than those
of the final output some experimentation is needed to
achieve the desired positioning.
.NH
Beautification
.PP
A good draftsman is able to take a rough sketch and convert it into
a ``neat'' drawing. It is desirable to have similar facilities
in the graphics editor.
The menu selection
``formal'' (in the ``reshape'' and the ``remote'' menus) will
transform an object or group of objects so that lines that are
nearly parallel become parallel, lengths that are
nearly equal become equal, etc.
The algorithm used for the transformation is described in detail
in |reference(pavlidis van wyk beautifier).
.1C
.KF bottom
.BB
.so nice.so
.fC 4
A drawing before and after beautification.
.KE
.2C
.PP
Beautification can reduce the need for long menus. Suppose we want
to draw a line tangent to a circle. We could add a menu option
that would draw a tangent from a point to a circle but we could also
leave the problem to the beautifier. You would draw a circle
and a line approximating the tangent and then pass these to
objects to the beautifier. Nearly tangent can be detected by
finding that the distance of the center of the circle from
the line is nearly equal to the radius. Then a system of
equations and inequalities
can be solved to find new objects that are mutually tangent
and close to the original objects. The same routines could
be generalized to handle the problem of having a line (or lines)
tangent to any number of circles, without having to add any more
menu instructions.
.PP
.I Ped
is still very far from replacing a good draftsman and it is highly
unlikely that it ever will. But it is likely that more
features will be added to the beautification transformation.
.NH
User Operations
.PP
There is a mechanism for executing
.UX
commands and incorporating their output in the drawing.
A command may be typed in advance (while in the command typing mode) or
it may be typed at the time of its invocation.
There are two menus offering a ``user oper'' selection:
``draw'' and ``refine.'' In the former you need select only
a point on the screen before applying the command.\(dg
.FS
\(dg The current implementation also expects typing of the command
at that moment.
.FE
In the latter you must select an object as well.
If there is no previously defined command you are prompted
for it.
The following rules are applicable.
.PP
If the object type is text, then the text is itself is supplied as
input to the
.UX
command. the output is collected as text and replaces
the original.
.PP
If the object type is polygon or spline the two coordinates of each point
are supplied to the
.UX
program, one point per line.
The string
.CW 1e6
is appended as end-of-file indicator.
.PP
If the object type is family, then the whole object is provided
to the command using
.I ped
format.
.PP
Often the command is
\f(CWa.out\fP,
a program written by the user for transforming geometric figures.
Text processing commands such as
.DS
.ft CW
tr "[a-z]" "[A-Z]"
.ft
.DE
are also common.
Finally, the ``user oper'' command may used in order to include
the output of commands such as
.I date
as part of illustrations prepared by
.I ped.
The inclusion of the output of programs that produce complex
illustrations is best done by having them output
their results in
.I ped
format. (See Appendix B.)
.SH
Acknowledgments
.PP
Many people provided considerable input for
.I ped,
especially its early users and those porting it to new machines.
Early users include amongst others Paul Pavlidis, Mary Bittrich,
Doug McIlroy, Bob Hutchison, John Cregar, and Sandy Fraser.
Bob Hutchison and Jon Sauer contributed significantly
to the porting of the program to
.UX
V and other
output devices. Janet Lee wrote the online documentation
and also contributed to the porting on 3B machines.
.PP
The following people either provided parts of the software
or let me use their code as prototype:
Ken Thompson (routines for creating the double lined objects),
Tom Killian (``blitblt'' routines),
Tom Szymanski (high level communications protocol),
and Tom Cargill (menu design).
George Wolberg helped with the implementation of many of the routines
in an early version of the program.
.PP
Lorinda Cherry, Steve Mahaney, and Doug McIlroy made many useful comments on
various versions of the documents describing the use and
organization of
.I ped.
.SH
References
.LP
|reference_placement
.de hD
.ft CW
.sp 0.5
.NE 1i
..
.SH
Appendix A: Command Actions
.PP
The following is a slightly expanded version of the online documentation
which becomes available when you press button 2 over
a menu selection. It is based on documentation written by Janet Lee.
.hD
basic
.LP
Collection of commands that manipulate files and/or groups
of objects.
.RS
.hD
type comm
.LP
Enable the keyboard for file manipulation and
.UX
commands.
Commands are:
.CW "w filename"
(write to a file),
.CW "r filename"
(read
from a file),
.CW "e filename"
(edit a file),
.CW f
(print current file),
.CW "u string"
(execute `string' as a
.UX
command when ``user oper'' is
selected in the ``refine'' menu),
.CW pwd
and
.CW cd
(execute the
.UX
commands),
.CW qq
(exit PED) and
.CW m
(revive the mouse). The mouse is also revived
by a newline, or pressing any button.
.hD
view host
.LP
Display the file stored on the host machine and make it available
for remote editing. The display is made in STORE mode so you
can see how the picture would look when printed by the tped|troff
combination. Also filled splines, double lines, and other fancy
features of PED are shown correctly.
When selecting ``view host'' while there is already a display,
you can specify the portion of the drawing to be viewed by drawing
a rectangle using button one.
Buttons two and three cancel the view host.
.hD
bring in
.LP
Bring the file into the DMD terminal for local editing.
If there is no display the whole file is brought in. Otherwise
a sweep rectangle is made available for selecting the portion
to be brought in. If the sweep rectangle is too small, the
entire file is brought in.
.hD
markers(t)
.LP
Mark with black ``bullets'' the first point of each object.
Also display the control polygons of splines and show dashed
lines
connecting the members of each family. It is a good idea to
have the markers on when working on a file.
The command is a toggle; selecting ``markers'' again erases the
markers.
.hD
HELP/plot
.LP
Display a summary documentation of PED and then redraw the
display of objects.
.hD
zoom
.LP
Display a closeup of the image. The apparent size is
increased by 1/8. The actual size of the picture is unchanged.
.hD
unzoom
.LP
The apparent size of the image is decreased by 1/8.
The actual size of  picture is unchanged.
.hD
scroll
.LP
Scrolls the image on the screen.  A line is drawn from the center
of the screen. Any button selects the terminating point of the
line which becomes the new center of the screen. The entire
drawing is shifted to accommodate the new center.
.hD
size?
.LP
A sweep rectangle becomes available. After sweeping an area of the
screen PED prints the size of that area in inches when the drawing
is printed by tped|troff.
If the rectangle size is too small, it prints the
size of the whole drawing area.
.hD
blitblt
.LP
Creates a bit map of the image in the file BLITBLT in your current
directory. This can be printed on other devices by appropriate
commands. For the Gnot printer the command is
.I gbcan .
.hD
exit
.LP
Exit PED program. If your file has been modified since the
last write it is saved in a file called
.CW ped.save
in the current
directory. PED plays it safe so that if it is not sure
whether a file has been modified it will save it anyway.
Errors, if any are in
.CW .pederr .
.RE
.hD
ch size
.LP
Commands that scale or apply simple geometrical transformations
to the drawing. Mot of them are executed on the host because of
the need for floating point.
.RS
.hD
expand
.LP
Increases the size of an object by a factor of 1.25.
Click button one over the object to be expanded.
.hD
shrink
.LP
Reduces the size of an object by a factor of 0.8.
Click button one over the object to be shrunk.
.hD
rotate
.LP
Select an object with button one. This produces a circle around
the object and a line drawn from the circle.  Selecting a point
on the circle with button one rotates the object to this point.
Selecting a point on the axis that is not on the circle expands
or shrinks the object with the rotation.
If the point is selected on the center of the circle, then a 90
degree rotation is made.
Pressing button three terminates the command.
.hD
turn 45
.LP
Select an object with button one and the object rotates 45 degrees.
.hD
v-flip
.LP
Select an object with button one and the object is flipped
vertically.
.hD
h-flip
.LP
Select an object with button one and the object is flipped
horizontally.
.hD
v-elong
.LP
Select and object with button one and the object is elongated
vertically.
.hD
h-elong
.LP
Select and object with button one and the object is elongated
horizontally.
.hD
undorot
.LP
Select an object with button one to undo a previous rotation.
Undorot must be used immediately after the rotation command
and for the same object.
.RE
.br
.ne 2i
.hD
move
.LP
Commands for manipulating and moving around objects
.RS
.hD
del obj
.LP
Delete the object selected. Clicking button one over any object
removes the object and replaces it with a skull and crossbones
symbol. Pressing button three completes the delete, any other
button cancels it.
.hD
copy obj
.LP
Copies the object selected.  Click button one over the
object to be copied.  An image of the object appears.
Move the image using the mouse to the desired location
and press button one to complete the copy. Pressing button
two or three cancels the copy.
.hD
mv obj
.LP
Move the selected object. Click button one over the object and
an image of the object appears.  Move the image using the mouse
and click button one to complete the move when the object appears
in the desired location. Clicking button 2 or 3 cancels the move
but it leaves the object in the new location until one returns
to the basic menu. This operation is useful for unscrambling
temporarily crowded areas of the drawing.
.hD
brk obj
.LP
Used to remove sides of polygons or destroy links.
Click button one over the first point and then select the
second point.  The line between them is removed.
If the polygon is not connected then the first application
of the command will make two objects out of the commands
rather than remove any side.
.hD
attach
.LP
It merges two open polygons by moving the second so that
the two selected points coincide. The command fails if either
of the selected points is not an endpoint. The two points
are selected with button one. Using any other button
between the two point selections cancels the attachment.
.hD
join
.LP
It merges two open polygons by drawing a line between the
two selected points without moving anything. The command fails
if either of the selected points is not an endpoint.
The two points are selected with button one.
Using any other button between the two point selections
cancels the attachment.
.hD
link obj
.LP
Select a polygon using button one.  Any polygon selected
afterwards with button one is linked to the first polygon
(i.e. the objects become one object).  You can link any number
of polygons together.  Terminate the command by pressing button
two or three. The polygons may be open or closed but all of
them must be the same kind (open or closed). The main use
of the command is to prepare contours for filled objects
with holes.
.hD
match
.LP
Select an object with button one.  Then select any number of 
objects with button one and the touched points will coincide.
Pressing any button other than button one terminates the command.
The objects remain separate.
.hD
center
.LP
Select an object with button one. Then select any number of
objects with button one.  The objects are moved so their
centers coincide (the first object selected dictates which
object to center on). Pressing button two or three terminates 
the command.
.hD
family/pt
.LP
Define a family by selecting points of different objects
with button one.  The family connections can be viewed
using the  markers option (basic menu). When families are
formed in this way, their objects are plotted in the order
by which they were placed in the family. Therefore one can
determine sequence of plotting which may be important for
filled objects.
Using button two or three terminates the command.
.RE
.hD
draw
.LP
Used for drawing objects.  Must be used to begin a drawing.
.RS
.hD
no grid
.LP
Removes the grid from the screen
.hD
grid:FMC
.LP
Create a grid on the screen either Fine, Medium, or
Coarse (FMC) depending on the number of times the
option is selected.
.hD
lines
.LP
Draws lines.  Button one is used to select the starting
and terminating points of a line.
Buttons two and three terminate the command.
.hD
text
.LP
Select a point for the text using button one and type from the
keyboard.  Two returns terminates input.
.hD
circle
.LP
Draws circles.  Click button one over the center of the circle.
Draw the circle outward and click button one again to define the
radius.
.hD
box
.LP
Draws boxes. Click button one over the upper left (right) corner
of the box and move the mouse right (left) and define the lower
right (left) corner of the box by clicking button one again.
.hD
oval
.LP
Draws ovals. Click button one over the upper left (right) corner
of the oval and move the mouse right (left) and define the lower
right (left) corner of the oval by clicking button one again.
.hD
family/bx
.LP
Define a family by surrounding family elements in a box.
The upper and lower corners of the box are defined with
button one.  Any objects whose first point lies within the box
become members of the family and are connected with dashed
lines if the ``markers'' option is on (basic menu). A family
is manipulated as a unit.
.hD
user oper
.LP
Used to incorporate output from a
.UX
command on the screen.
There is no way to provide standard input but arguments
are passed.
Click button one over the area where the output should appear.
Type the command and hit two returns to terminate the input. 
The output from the command is printed on the screen.
Typical use is for commands like ``date'', ``who'', etc.
If PED thinks that incorporation of the command output into
a PED file may cause problems (memory overflow for example)
it will cancel the operation.
.hD
fixsz(t)
.LP
Prevents scaling of characters with the drawing. Scaling may be
restored by executing the command a second time. When used
it saves terminal memory since only one font is stored. It is
also desirable in its own right since many users do not like
scaling of text.
.RE
.hD
reshape
.LP
Reshape allows you to change the shape of objects.
.RS
.hD
formal
.LP
Clicking button one over any object will
straighten all lines of the object that are almost completely
vertical or horizontal as
described in section 5.
.hD
spline(t)
.LP
Convert a polygon into a spline controlled by the polygon or
convert a spline into its controlling polygon.
.hD
arrow(t)
.LP
Clicking button one over the end point of
an object will add an arrow.  Arrows cannot be added to circles
All arrows must be at an end point of a line (i.e you cannot
add an arrow to the middle of the side of a box. However, for
ovals, the arrow is placed along one side of the oval).
.hD
corner(t)
.LP
Make a spline have a corner by pointing to one of the vertices
of its controlling polygon. If there is already a corner there
make the spline smooth.
.hD
mv point
.LP
Clicking button one over any point on a object (except circles)
moves the point to a location specified by a second click of
button one.  This is useful in reshaping splines and creating
distorted boxes and ovals.
.hD
del pnt
.LP
Clicking button one over any point removes the point and reshapes
the object accordingly (i.e a rectangle becomes a triangle).
Circles cannot be used.
.hD
insrt pnt
.LP
This command inserts a point between two points of an object
(except circles). Click button one over any point on a object.
Then select a point where the inserted point will lie. 
Finally, select the terminating point of any line drawn from
the first point.  The line on the object is removed and two new
lines are created joining the three points selected.
.hD
close
.LP
Click button one over any open point of a spline or polygon
and the shortest possible line is drawn to the other open
end of the object. When you close a spline the first and
last points are corners.
.hD
dis fam
.LP
Click button one over any member of a family and the family
will dissolve.
.hD
rest fam
.LP
Click button one over any previous family and it will be
restored.  CAUTION: Some operations on family members after
dissolving may make it impossible to restore it by this
command so you will have to use the make family command
again.
.hD
undo
.LP
Click button one over an object to undo the last command
performed on the object.  You MUST select undo immediately
after executing the command you wish to undo.
.RE
.hD
refine
.LP
Assorted commands for changing the form of objects. (They would
not fit all in the reshape menu.)
.RS
.hD
edit text
.LP
Edit a text object selected by button one. A copy of the text
is displayed on the top of the screen.
Newlines are represented by three horizontal lines or whatever
other symbol your DMD uses for control characters.
If the object has more characters than what can be fitted in the
top line of the screen the command is canceled. (It is
suggested that you use a text editor to edit large blocks of
text. PED files are ASCII and can be edited by any text editor such as
.I ed
or
.I sam .)
Button one is used to select a position within the text. 
A cursor appears in the selected position.
Insert by typing or delete with backspaces.  End an
insert or delete with a return. Press button two or three to
terminate the command.
.hD
adjt(CRL)
.LP
Allows adjustment of text either left, center or right.
Text is initially left adjusted.  Click button one over the
object and it will center.  A second click will right adjust.
Successive clicks continue the pattern Left, Center, Right.
After such an operation all new text is entered in the
selected mode.
.hD
psize
.LP
Set point size for text. Select a text object with button one. A
cursor appears on the top of the screen.  Type a point size and
return and the text will be adjusted to the selected point size.
All successive text uses that point size. Cancel the command
by hitting return.
.hD
what size
.LP
It prints the pointsize for the selected object.
.hD
dashed(t)
.LP
Select an object with button one and the object is
composed of dashed lines.  Circles cannot be made dashed.
.hD
double(t)
.LP
Select an object with button one and the object is composed
of double lines.  Circles, ovals and splines cannot be made
double.  The double lines WILL NOT appear in the local mode
(i.e. ``bring in'') but only on ``view host.''
.hD
heavy(t)
.LP
Select an object with button one and the object is composed
of heavy lines.  Circles, ovals and splines cannot be made
heavy.  The heavy lines WILL NOT appear in the local mode
(i.e. ``bring in'') but only on ``view host.''
.hD
isplin(t)
.LP
Plot the points of a polygon using an interpolating spline.
The interpolant WILL NOT appear in the local mode
(i.e. ``bring in'') but only on ``view host.''
.hD
fix
.LP
Select (with button one) an object with double or heavy lines
or an interpolating spline (isplin). The object display will
become effective not only under the `view host' mode,
but also under the `local' mode.
However the original object description will be lost.
.hD
user oper
.LP
Allows execution of a host
.UX
command defined using the ``type
comm'' option ``u'' or a command defined now. You must select an
object with button 1. If a command has been defined it will
be executed using the object as input. Otherwise you will be
prompted for the name. The object is used as input for the
command if it expects one. See the user's manual for more
details.
.hD
undo
.LP
Undoes the previous command if you point to the object where
that command was applied.
.RE
.hD
remote
.LP
Commands to perform editing on the host.  All commands
request that you enclose the desired area to be edited 
within a rectangle.
.RS
.hD
delete
.LP
Using button one, draw a rectangle around the objects to be
deleted. Confirm by pressing button 3.
.hD
copy
.LP
Using button one, draw a rectangle around the objects to be
copied.  Use button one to select the location of the copied
object.
.hD
move
.LP
Using button one, draw a rectangle around the objects to be
moved.  Use button one to select the location of the moved
object.
.hD
read
.LP
Using button one, draw a rectangle indicating the region where
the read in file will reside.  Once the rectangle is defined,
type in the file name and hit return. The file will be scaled
to fit in the rectangle
.hD
write
.LP
Using button one, draw a rectangle around the objects to be
written to the file.  Once the rectangle is define, type a
file name and hit return.  All objects within the rectangle
are written to the file.
.hD
formal
.LP
Using button one, draw a rectangle around the objects to be
formalized (beautified).  Click button one again to initiate
the procedure. See the user's manual for details about the
formalization procedure.
.hD
xpand
.LP
Using button one, draw a rectangle around the objects to be
expanded.  Click button one to initiate the expansion. Objects
expand by a factor of 1.25.
.hD
shrink
.LP
Using button one, draw a rectangle around the objects to be
shrunk.  Click button one to initiate the shrink command. Objects
shrink by a factor of 0.8.
.hD
v-flip
.LP
Using button one, draw a rectangle around the objects to be
vertically flipped.  Click button one to initiate the vertical
flip.
.hD
h-flip
.LP
Using button one, draw a rectangle around the objects to be
horizontally flipped.  Click button one to initiate the
horizontal flip.
.RE
.hD
shade
.LP
This menu is used to shade objects.  Select a shade with
button one.  Your mouse will become a box that resembles
the selected shade.  Click button one over any object that
you want filled in with that shade.  To remove a shade,
point to `unfill' and then to an object with shading.
.hD
color
.LP
Assign this color to the object selected. There is no visible
effect on the DMD terminal, unless you have set the markers option.
Then the color name appears next to the object (except for the
color black). The assignment of color becomes meaningful when
you display a PED file on a color device.
.EQ
delim !!
.EN
.ds aA \s+2\(->\s-2
.SH
Appendix B: PED File Format
.PP
The following is a specification of the format of
.I ped
files. This information is useful for writing programs to perform
one of the following functions:
(a) display
.I ped
files on new devices;
(b) create illustrations that can be edited by
.I ped
and included in
.I troff
documents by
.I tped.
.PP
It is also a formal definition of
.I ped
objects.
The specification is given as a production system.
Semantics are given in
.I italics .
Nonterminals start with
.CW $ ,
.CW % ,
.CW & ,
or
.CW .
and the following
conventions are observed:
.TS
center;
cfI cfI
lFCW l.
Prefix	Interpretation
%	number (integer or floating point),
&	character string
\&.	single character
.TE
All other symbols are terminals.
.SH
Initial Action
.TS
delim(!!);
lFCW lFCW.
File \*(aA	o 8i  \fIopen version\fP
\^	r %N %N %N %N  !x sub "min"! !y sub "min"! !x sub "max"! !y sub "max"!
\^	$$	\fIobject list\fP
\^	e
.TE
.SH
Creation of Object List
.TS
lFCW lFCW.
$$ \*(aA	$Q $$
\^	$Q
.TE
.SH
Creation of Objects
.TS
lFCW lFCW.
$Q \*(aA	[ $HF $$ ]  \fIfamily\fP
\^	[ $H $P ]  \fIpolygon\fP
\^	[ $H $U ]  \fIfilled polygon\fP
\^	[ $H $S ]  \fIspline\fP
\^	[ $HT $T ]  \fItext\fP
\^	[ $H $C ]  \fIcircle\fP
.TE
.SH
Object Headers
.TS
lFCW lFCW.
$H \*(aA	j %N  \fIcolor\fP
\^	h %N  \fIpoint size\fP
\^	i .N   \fIfilled objects: drawing mode\fP
\^	i T.N  \fIfilled objects: texture\fP
\^	k.A .N  \fIdrawing mode\fP
_
$HF \*(aA	j %N  \fIignored\fP
\^	h %N  \fIignored\fP
\^	f %N %N  \fIundefined point\fP
_
$HT \*(aA	j %N  \fIcolor\fP
\^	h %N  \fIpoint size\fP
\^	i P%N  \fIpoint size (obsolete)\fP
\^	i F&N  \fIfont\fP
_
\&.A \*(aA	a  \fIarrow on first point\fP
\^	b  \fIarrow on last point\fP
\^	c  \fIarrow on both points\fP
\^	NULL  \fIno arrow\fP
.TE
.SH
Start of Outlines
.TS
lFCW lFCW.
$P \*(aA	p %N %N
\^	p %N %N	$R
\^	P %N %N	$R
_
$S \*(aA	s %N %N	$R
\^	S %N %N	$R
_
$U \*(aA	Q %N %N $R
.TE
.SH
Continuation of Outline
.TS
lFCW lFCW.
$R \*(aA	.V %N %N
\^	.V %N %N $R
_
\&.V \*(aA	v  \fIconnected vertex\fP
\^	w
\^	x  \fIunconnected vertex\fP
\^	X  \fIsame as\fP w \fIand\fP x
.TE
.LP
.CW v
stands for a simple vertex connected to the previous one.
.CW w
is the same as
.CW v
but in the case of splines is
counted as multiple and creates a corner in the curve.
\f(CWx\fP is a simple vertex NOT connected to the previous one.
(The result of a
.I link
operation.)
.SH
Circle
.TS
lFCW lFCW.
$C \*(aA	c %xc %yc %rad
.TE
.SH
Text
.TS
lFCW lFCW.
$T \*(aA	m %N %N $TT
$TT \*(aA	u .M&T $TR
_
$TR \*(aA	u &T $TR
\^	t &T
_
\&.M \*(aA	L  \fIleft adjusted text\fP
\^	C  \fIcentered text\fP
\^	R  \fIright adjusted text\fP
.TE
.LP
Valid numbers for color are 0 to 7. Valid characters for texture
filling are E through P. Valid characters for drawing mode are
.TS
center;
lFCW l.
s	solid
d	dashed
t	double
h	heavy
i	interpolating spline
.TE
.PP
The specification is rather awkward at points for historical reason.
Originally, we had hoped that the
.I ped
file format would be the same as that for the PLOT(5) metafile.
This goal was eventually abandoned but by that time there were too many
.I ped
files around to allow a drastic redesign of the format.
.SH
Appendix C: Extensions of TROFF and TROFF postprocessors
needed for expanding the compact output of TPED
.PP
.I Troff
has two primitives for drawing graphics:
.CW \eD
and
.CW \eX .
In both cases, the next letter specifies an action.
If
.I troff
does not understand this letter, it simply passes the whole construct through
to its output so that a post-processor can deal with it.
This Appendix details the constructs
needed to interpret the compact output of
.I tped.
.LP
.CW
\eD'p dx dy ...'
.R
a polygonal line.
.LP
.CW
\eD'q dx dy ...'
.R
a spline
.PP
In either case closure of the curve must be made explicit by repeating the
first point for a polygon and the first two for a spline. Multiplicity
of a spline point is achieved by adding a pair 0 0 after the point. Thus
the current
.P1
\eD'~ dx dy ...'
.P2
is equivalent to
.P1
\eD'q 0 0 dx dy ... 0 0'
.P2
.LP
.CW \eX\'P\fIoption\fP'
is used to supply information about the way a curve
will be plotted. Currently the following options are recognized
.de XX
.IP \f(CW\\$1\f1 10
..
.XX \eX'Pd'
dashed line (or spline)
.XX \eX'Pd'
dashed line
.XX \eX'Ps'
solid line
.XX \eX'Pf'
do not plot the contours but collect them to create a region
with interior filled by texture.
.XX \eX'Pg'
same as previous but the contour of the regions will also be plotted.
.XX \eX'Pe\fIS\fP'
do the actual scan conversion of collected contours using a texture
specified by the character S. (The textures of \fIped\fP
are described in order by the characters E to P.)

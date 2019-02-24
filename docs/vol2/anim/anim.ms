.so ../ADM/mac
.XX anim 451 "A System for Algorithm Animation"
.EQ
delim @@
.EN
.nr dP 1
.nr dV 1
.nr dT 8	\" tab stops this far apart in .P1/.P2
.nr NH .5v	\" adds extra space before NH or SH heading
.nr ds .5i	\" default indent for programs
.hy 14		\" set hyphenation: 2=not last lines; 4= no -xx; 8=no xx-
.\"
.de IN	\" assumes called as .INCLUDE filename
.sy compile \\$2.t
.so \\$2.o
..
.de ge	\" assumes .ge called as .get
.sy trget \\n(.$ \\$2 '\\$3' '\\$4' '\\$5' >junk.\\n($$
.so junk.\\n($$
.sy rm junk.\\n($$
..
.de ru	\" assumes .ru called as .run
.sy \\$2 \\$3 \\$4 \\$5 \\$6 \\$7 \\$8 \\$9 >junk1.\\n($$
.get junk1.\\n($$
.sy rm junk1.\\n($$
..
.TL
A System for Algorithm Animation
.br
Tutorial and User Manual
.AU
Jon L. Bentley
Brian W. Kernighan
.AI
.MH
.AB
A program or an algorithm can be animated by a movie
that graphically represents its dynamic execution.
For instance, a memory allocator might be animated by lines that
appear when memory is allocated and disappear when it is freed;
a sort might be animated by a randomly scrambled
sequence of lines being permuted into order.
Such animations are useful for debugging programs, for developing
new programs, and for communicating information about how programs work.
This paper describes a basic system for algorithm animation:
the output is crude, but the system is easy to use;
novice users can animate a program in a couple of hours.
The system currently produces movies on Teletype 5620 terminals
and workstations that support the X window system, and also renders movies into
``stills'' that can be included in
.I troff
documents.
This paper is a user manual and a tutorial introduction
to algorithm animation using the system.
.AE
.NH
Introduction
.PP
Dynamic displays are better than static displays for
giving insight into the behavior of dynamic systems.
The pictures in Figure 1, for instance, illustrate four
equal-mass bodies moving in the plane
under Newtonian attraction.
.KF
.INCLUDE stars
.sp
.ce
\f3Figure 1:\fP  Bodies moving under Newtonian attraction
.KE
Time marches across the pictures left to right,
at roughly equal intervals.
Each column, called a
.I snapshot ,
depicts three
.I views
of the bodies.
The top view gives the current position as a dot; the tail formed
by the last few positions hints at the velocity and the acceleration.
The middle view renders the path of each body as a line.
The bottom view depicts the history of velocities by dots
recorded at equal time intervals:
low velocities give close dots and
high velocities leave the dots far apart.
.PP
The first snapshot shows the bodies starting slowly, far apart.
In the second snapshot they have recently experienced a
high-velocity encounter near the center.
In the third snapshot the bodies are again far apart,
moving slowly toward another encounter in the fourth snapshot.
In the fifth snapshot the large time-step of our simulation program
violates conservation of energy and sends the bodies racing away
from each other.
.PP
This paper describes the animation system that produced those pictures.
A sixty-line C simulation program was augmented with eight
.I printf
statements to generate a
.I script
file describing the paths of the four bodies in the three views.
That file was processed by a program named
.I stills
to produce the pictures above, using
.I pic
and
.I troff ;
we were able to control what frames were displayed, in what size and form.
A program named
.I movie
displays the same data on a Teletype 5620 terminal or an X workstation;
the viewer can control the speed of display,
proceed forward or backward through time,
and change the screen layout to emphasize certain views.
Those components can be depicted as:
.PS
boxht = .25; boxwid = .6
ellipseht = .25; ellipsewid = .6
lineht = .1; linewid = .4
down
box "generator"
line down
A: ellipse "script"
line from A.se right down
box "stills"
line down
box invis "pic | troff | ..."
line from A.sw left down
box "movie"
line down
box invis "5620, X, ..."
.PE
.PP
Several systems have been developed for algorithm animation
|reference(sedgewick brown ieee).
Most of those systems produce animations of very high quality;
unfortunately, they are expensive in both programmer time and CPU time.
Our system is at the opposite end of the spectrum:
its output is primitive, but the system is easy to use;
a new user can animate a simple program
in an hour or two by adding a few lines of code.
Although our system was designed primarily with program animation
in mind, the gravitational example shows that it can be useful
in other domains as well.
.PP
Section 2 introduces the system by animating a sorting algorithm.
Sections 3 through 5 describe the three primary components of
the system:
script
files,
.I movie ,
and
.I stills .
Section 6 describes the animation of several larger programs,
and Section 7 discusses a few everyday matters about using the system.
.NH
A Simple Example \(em Sorting
.PP
Insertion sort is the method most card players use.
As each card is dealt, it is inserted into
its proper place among the existing cards.
To sort the array @X[1..N]@, insertion sort maintains the
sorted subset in @X[1..I]@ and increases @I@ from 2 to @N@.
The subarray @X[1..1]@ is sorted by definition.
The first phase of the algorithm sifts @X[2]@ down
so that @X[1..2]@ is sorted, the second phase sifts
@X[3]@ down so that @X[1..3]@ is sorted, and so on.
.PP
Here is an implementation of insertion sort in
.I awk |reference(awk book):
.P1
.get is.awk
.P2
The first
.CW for
statement sprinkles
.CW x[1..n]
with random integers in the range @1..25@, and the second and third
.CW for
statements perform the insertion sort.
The function
.CW swap
exchanges array elements;
.CW show
prints the current state of the array.
.PP
If we run this program with
.CW n
set to 7, we get this static display
of sorting a 7-element array of random integers:
.P1
.run sh is.awk 7
.P2
.PP
The animation system provides an alternative:
by adding a few more
.CW print
statements to the program, we can produce input for the animation system,
thus providing a dynamic display
of the sorting process.
Furthermore, we can use graphics as well as text,
to give a more visual presentation of the algorithm.
.PP
For this example, we decided to present two
views.
The top view simply shows the numbers in the array as they are sorted;
it is essentially the same as the textual output above,
except that we have added a vertical bar:
elements to the left of the bar are in order.
The bottom view is graphical: the value of each element is represented
by the length of the corresponding vertical line.
.PP
These six snapshots
show the state after each phase of the sorting algorithm:
.sy make.sorts
.INCLUDE is3
.PP
Although the two views represent the same information,
they are useful for different tasks.
The textual nature of the top view is easier for novices to follow,
and the complete information is handy for debugging small examples.
The more visual bottom view is better for displaying large sorts.
.sp 1
.LP
.I "The Program."
.PP
Here is the complete
.I awk
program
.CW is.gen
that generated the animation depicted above.
.P1
.get is.gen
.P2
It is very similar to the first version,
but we have added the functions
.CW less
to make comparisons and
.CW draw
to display elements,
as well as several
.CW print
statements.
In addition to performing their sorting functions,
.CW less
and
.CW swap
record their actions by printing output
into a script file that will serve as input to
.I movie
and
.I stills .
This animation uses four commands:
.CW line ,
.CW text ,
.CW view
and
.CW click .
.PP
A line from @( x sub 1 , y sub 1 )@ to @( x sub 2 , y sub 2 )@ is
drawn by a command of the form
.P1
@optional_label:@ line @x sub 1@ @y sub 1@ @x sub 2@ @y sub 2@
.P2
(Throughout this paper, literals are shown in
.CW typewriter
font and categories are in @italics@.)
The coordinates of the line can lie in any range;
later programs will scale them appropriately.
The label on a line is optional.
When a labeled object is drawn
the object that previously had that label is erased.
Thus for all lines that have the same label,
the act of drawing one line erases its predecessor.
(There is also an explicit
.CW erase
command.)
.PP
Text is produced at @( x , y )@ by a command of the form
.P1
@optional_label:@ text @x@ @y@ @anything@ @at@ @all@
.P2
As with lines, labels are permitted;
re-use of a label erases whatever object previously had that label.
For example, the vertical bar is always printed with the label
.CW bar ,
so each bar erases the previous one.
.PP
The
.CW view
command is used to place output in a particular view.
There are two views here,
.CW text
and
.CW geom :
.P1
view text
view geom
.P2
The
.CW draw
function draws text in the
.CW text
view
and lines in the
.CW geom
view, relying on implicit erasure to remove the previous object
before creating a new one.
Different views are independent, so a label like
.CW a1
can be used in several views without interference.
.PP
Interesting events are marked by the
.CW click
command:
.P1
click swap
click comp
click phase
.P2
.I Stills
and
.I movie
can refer to each click with this mechanism,
as we will see in more detail shortly.
.PP
Labels, view names and click names are arbitrary
and unrelated to one another.
.sp 1
.LP
.I "The Script File."
.PP
Executing the command
.P1
is.gen 3 >is1.s
.P2
produces the script file
.CW is1.s
containing a sort of three elements.
The basename
.CW is1
is for ``insertion sort 1''; the suffix
.CW .s
identifies it as a script file and is required by the animation system.
Here is
.CW is1.s ,
printed in two columns to save space:
.P1
.ps 8
.vs 8
.ta 3i
.run 2col is1.s
.P2
.sp
.PP
Here is a picture of all clicks in the script file
.CW is1.s ,
produced by
.I stills .
.INCLUDE is1
As before, time goes from left to right.
Both views of the current state are depicted in a snapshot at each
.CW click ;
the label below each frame tells the name and number of the click.
This detailed picture illustrates the animation system;
here is a sparser picture that illustrates insertion sort
on a 14-element array:
.INCLUDE is2
.sp 1
.LP
.I "Making Stills."
.PP
The last picture was included in this document by this
.I stills
description:
.P1
.get is2.t
.P2
The description is delimited by the lines
.CW "\&.begin stills"
and
.CW \&.end
(which correspond, for instance, to
.CW \&.EQ
and
.CW \&.EN
in
.CW eqn
or
.CW \&.TS
and
.CW \&.TE
in
.CW tbl ).
Text following the sharp character
.CW #
is a comment to be ignored.
The
.CW view
statements specify the empty string as the title for both
views of the data.
The
.CW print
statement displays snapshots at the requested
.CW click s
of
.CW phase .
.PP
The last four statements are parameter assignments of the form
.P1
@parameter_name@   @value@
.P2
Assignment to the
.CW file
parameter names the script file to be displayed.
The next two assignments set the height and width of frames,
and the final statement causes
.CW medium
text (the default text size) to be set
five points smaller than the current
.I troff
point size.
.sp 1
.LP
.I "Viewing A Movie."
.PP
To watch a movie on the 5620,
make a window of suitable size and shape
and in it type the command
.P1
is.gen 20 | movie
.P2
If you want to access the script later, type instead
.P1
is.gen 20 >is.s
movie is.s
.P2
After a pause to run
.CW is.gen
(about 15 seconds on a VAX\(tm-750 in this case), the 5620 down-loading procedure
will begin;
after that (another 30 seconds), the data for the movie itself will
begin to appear.
When this is finished (also about 30 seconds),
a message about the number of bytes
sent (about 9500) appears in the upper left corner.
.PP
At this point, the mouse buttons can be used to
redisplay the movie.
Button 3 is the main control.
It has 9 menu items:
.P1
again
faster 1
slower 1
1 step
backward
fatter 1
thinner 1
or mode
new file
Quit?
.P2
To play the movie again,
select
.CW again .
(This movie takes about 6 seconds at full speed.)
You can stop it at any point by pushing any button;
a further push of button 1 continues it.
To slow the display, select 
.CW slower ;
each selection halves the speed by increasing
a wait interval by a factor of two.
After three selections the menu reads
.P1
again
faster 8
slower 8
\&...
.P2
Try selecting
.CW again
to see the sort once at this speed, then select
.CW faster
three times to get back to full speed.
.PP
The items labeled
.CW fatter
and
.CW thinner
control the thickness of lines in an analogous manner.
Selecting
.CW fatter
several times results in obese lines;
you may return things to normal with
.CW thinner .
.PP
Three menu items control binary mode settings:
.CW 1
.CW step
or
.CW run ;
.CW backward
or
.CW forward ;
and
.CW or
.CW mode
or
.CW xor
.CW mode .
For each, the label indicates the next state,
not the current state.
.PP
Normally the movie is played from beginning to end without pause.
The menu item labeled
.CW 1
.CW step
puts it into a mode where it displays only one ``step''
each time button 1 is pushed.
This allows you to inspect the sort a frame at a time.
This item changes to
.CW run
when
.CW 1
.CW step
has been selected
so you can revert to continuous action.
.PP
The
.CW backward
item causes the steps to be taken in reverse order (time runs backwards);
the menu item changes to
.CW forward
when selected.
After the sort ends, select
.CW backward
and
.CW again
and watch the array scramble itself before your very eyes.
Judicious use of 
.CW 1
.CW step
and
.CW backward
and
.CW forward
make it easy to examine a few snapshots in detail.
.PP
Normally items are displayed in ``exclusive OR'' mode,
which means that a bit drawn over a previous one erases it.
If you are drawing numerous objects in a crowded area,
this can lead to unintended erasures.
The
.CW or
.CW mode
item switches to ``inclusive OR'' for drawing objects,
and erases objects by clearing.
Some movies are far better in one mode than the other.
.PP
The
.CW new
.CW file
item allows one to view a new movie without having to reload the
.I movie
program into the 5620.
We'll see how it works in Section 4.
.PP
The
.CW Quit?
menu item is the way to exit.
If selected, it displays a skull and crossbones
to warn that its effect is irreversible.
Pushing button 3 again exits;
a different button avoids quitting.
.PP
Button 2 contains menu items to control the size and shape
of the views on the screen and to control the meaning of a
``step'' in 1-step mode.
For this sorting movie, button 2 looks like:
.P1
view text
view geom
click comp
click swap
click phase
.P2
When a view is selected, you can sweep a rectangle
in which that view is to be displayed;
the use is exactly like the
.CW New
menu item in
.I mux .
You can arrange the views any way you like;
try deleting the textual view by sweeping its rectangle out of the window.
.PP
When a
.CW click
is selected, 1-stepping proceeds to the next
occurrence of that click.
So, for example, to see swaps one at a time,
select
.CW click
.CW swap
on button 2 then select
.CW 1
.CW step
on button 3.
Each hit of button 1 will pause at the next
.CW click
.CW swap
statement in the script file, in either
.CW forward
and
.CW backward
mode.
.PP
Multiple clicks may be selected.
Selecting both
.CW click
.CW comp
and
.CW click
.CW swap
will cause a pause after every comparison and every swap.
Selected clicks are marked with an asterisk in the menu;
selecting a click that already has an asterisk
removes the asterisk and turns off the click.
.PP
In
.CW run
mode, the movie runs at full speed until it encounters a selected click,
then pauses for a time proportional to the selected speed.
Selecting no clicks is equivalent to having selected an implicit
click that occurs after each geometric object
(text, line, circle, rectangle)
is drawn or erased,
so animations run faster when some clicks are turned on (with
.CW click
.CW phase
selected, for instance, the sorting movie runs nearly twice as fast).
.sp 1
.LP
.I "Summary."
.PP
This exercise illustrates the capabilities
and limitations of our animation system.
The output of
.I movie
is a crude but useful animation.
The output of
.I stills
is handy for more detailed study and for presentation in documents
(we would like to include a movie in this document, for instance,
but paper is easier to distribute than videotape).
A sophisticated animation system might require 500 lines of
code to produce beautiful animations of insertion sort.
Our output is unpolished by comparison, but it is adequate
for many purposes and requires just a few dozen lines.
.PP
If our system is so crude, why bother using it?
Why not animate an algorithm simply by drawing geometric objects
on the output device you happen to be using?
Some of the answer lies in extra services like these:
.IP
.I "Device Independence."
A script file can be rendered as a movie on a 5620 or an X11 workstation;
the system is designed to make it easy to port to additional
output devices.
The same script file can be incorporated into a document by
.I stills .
.IP
.I "Names."
Labels allow geometric objects to be erased;
implicit erasure by re-using a label avoids
much of the tedium of bookkeeping.
Click names mark key events;
they can be used to group related events.
.IP
.I "Independent Views."
Different simultaneous views of a process
are crucial for animating algorithms.
In our system, a single statement moves from one view to another.
Within a view, the user need not be
concerned about the range of coordinates;
the system scales automatically.
Labels in different views are independent.
.IP
.I "Viewer Control."
Both
.I movie
and
.I stills
allow the viewer to select which views will be displayed
and which clicks will be recognized.
Additionally,
.I movie
allows the viewer to go forward or backward, in single
steps or running at a selected speed.
.IP
.I "An Interface To The World."
Although writing to files takes more computer time than using the
geometric primitives provided by a specific output device,
we will soon see how those files allow complicated tasks
to be easily composed out of simple software tools.
.LP
Our system does not support interactive animations, however:
once the script has been generated,
there's no way to change it
except to generate it again.
.NH
The Script Language
.PP
This section is a more complete description of the
script language in which animations are described.
A script file is processed by the heretofore unmentioned program
.I develop ;
errors in script files are reported by that program.
The output of
.I develop
feeds
.I stills
and
.I movie :
.PS
boxht = .25; boxwid = .6
ellipseht = .25; ellipsewid = .6
lineht = .1; linewid = .4
down
box "generator"
line down
ellipse "fname.s"
line down
box "develop"
line down
A: ellipse "fname.i"
B: box "stills" with .w at A.e + (.2, -.4)
line down
box invis "pic | troff | ..."
C: box "movie" with .e at A.w - (.2, .4)
line down
box invis "5620, X, ..."
line from A.se to B.nw
line from A.sw to C.ne
up
line up from B.n
ellipse "docfile"
.PE
The command
.CW develop
.CW fname.s
produces the
.I intermediate
file
.CW fname.i
from the script file
.CW fname.s ,
unless
.CW fname.i
already exists and is newer than
the corresponding script file.
Fortunately, most users need not be concerned with intermediate files and the
.I develop
program; both
.I movie
and
.I stills
call
.I develop
implicitly.
Appendix I defines the format of an intermediate file.
.PP
The script language provides
commands to draw geometric objects
and commands that control the pictures.
A line whose first non-blank character is
.CW #
is a comment;
comments may not appear on the line after other commands.
Blank lines are ignored.
.sp 1
.LP
.I "Geometric Commands" :
.CW text ,
.CW line ,
.CW box ,
.CW circle .
.PP
Geometric commands describe text, lines, rectangles, or circles.
They share the common form
.P1
@optional_label:@ @command@  @options@  @x@ @y@  @additional@ @parts@
.P2
If a label is present, it names the object and will implicitly
erase any existing object with the same name in the same view.
The options are a (possibly null) list of names,
terminated by the next numeric field.
.PP
Text is placed at a position by the command
.P1
@optional_label:@ text  @options@  @x@ @y@  @string@
.P2
The available options are
.P1
[center]  ljust  rjust  above  below
small  [medium]  big  bigbig
.P2
At most one option may be selected from each line;
if none is selected, the option in brackets is used.
The first line describes text position, and
the second line describes text size.
The text string may be quoted.
If there is no leading quote, then the string starts at the first
non-blank character and continues until the end of the line.
If there is a leading quote, subsequent leading white space is kept
and any trailing quote at the end of the line is removed;
intermediate quotes are kept.
Some strings, including
(but not necessarily limited to)
.CW bullet ,
.CW dot ,
.CW circle ,
and
.CW times ,
are recognized by later processors.
.PP
A line is drawn by
.P1
@optional_label:@ line  @options@  @x sub 1@ @y sub 1@ @x sub 2@ @y sub 2@
.P2
The available options are
.P1
[-]  ->  <-  <->
[solid]  fat  fatfat  dotted  dashed
.P2
The first line of options describe whether the line should
be drawn with arrowheads; the default is without arrowheads.
The option
.CW <-
puts an arrowhead at the @( x sub 1 , y sub 1 )@ end of the line,
.CW ->
puts an arrowhead at the other end,
and
.CW <->
puts them at both ends.
The second line of options describes the body of the line.
.PP
A rectangle is drawn by
.P1
@optional_label:@ box  @options@  @xmin@ @ymin@ @xmax@ @ymax@
.P2
The only options are
.P1
[nofill]  fill
.P2
Under the default
.CW nofill
only the border of the box is drawn;
a
.CW fill ed
box has a solid interior as well.
.PP
A circle is drawn by
.P1
@optional_label:@ circle  @options@  @x@ @y@  @radius@
.P2
The radius is measured in the @x@ dimension.
Circles will look right only if @x@ and @y@ are in about the same range.
As with rectangles, the options are
.P1
[nofill]  fill
.P2
.sp 1
.LP
.I "Control Commands" :
.CW view ,
.CW click ,
.CW erase ,
.CW clear .
.PP
The current view is set by the statement
.P1
view  @name@
.P2
If there are no view statements in the script file,
.I develop
generates a single implicit view named
.CW def.view .
If geometric objects appear before the first view statement,
they go in that view and a warning message is generated.
.PP
A click is named by
.P1
click  @optional_name@
.P2
If no name is present, then the name
.CW def.click
is implicitly supplied.
.PP
A labeled geometric object can be explicitly erased by the command
.P1
erase  @label@
.P2
.I develop
prints a warning
if the object was never defined or has already been erased.
The various views have distinct name spaces;
the same label may be applied to two unrelated objects in two different views.
All objects in the current view can be erased by the statement
.P1
clear
.P2
.PP
None of these commands may have labels.
.sp 1
.LP
.I "Summary."
.PP
The script language contains the following commands;
options are indented on a subsequent line, with defaults in brackets:
.P1
# comment
@optional_label:@ line  @options@  @x sub 1@ @y sub 1@ @x sub 2@ @y sub 2@
	[-]  ->  <-  <->
	[solid]  fat  fatfat  dotted  dashed
@optional_label:@ text  @options@  @x@ @y@  @string@
	[center]  ljust  rjust  above  below
	small  [medium]  big  bigbig
@optional_label:@ box  @options@  @xmin@ @ymin@ @xmax@ @ymax@
	[nofill]  fill
@optional_label:@ circle  @options@  @x@ @y@  @radius@
	[nofill]  fill
view  @name@
click  @optional_name@
erase  @label@
clear
.P2
.PP
The shell command
.CW develop
.CW fname.s
makes the intermediate file
.CW fname.i
from the script file
.CW fname.s ,
if
.CW fname.i
is out of date.
The purpose of the intermediate file is to trade
increased space (for storing the intermediate file) for
reduced run time (a script file is developed just once, not
each time it is used).
The
.I movie
and
.I stills
shell scripts could be rewritten to pipe their inputs through
.I fdevelop ,
a filter form of
.I develop .
The
.I fdevelop
program can handle script files with at most 20,000 lines;
the argument 
.CW -l\f2n\fP
changes the upper bound to
.I n
instead.
It can similarly handle at most 10,000 pieces of geometry
active at any time;
the argument 
.CW -s\f2n\fP
(for ``slots'') changes that upper bound.
Error messages tell when these bounds need to be increased.
After any
.CW -l
and
.CW -s
arguments,
.I fdevelop
can have an optional file name.
If there is a name, that is the input file; otherwise,
the standard input is used.
The output is written on
the standard output.
.NH
The Movie Program
.PP
Movie production, as with most 5620 programs,
uses a host process and a terminal process.
The host sends the intermediate file produced by
.I develop
in a compact form to the terminal,
which stores it in a form suited
for forward or backward display.
As the file is shipped, the line number in
the intermediate file is displayed in the
upper-left corner of the window every 100 lines.
Afterwards, the total number of bytes
stored is displayed in that location.
The terminal process allocates 80,000 bytes
(typically 5-10,000 objects)
for the picture;
the argument 
.CW -m\f2n\fP
sets the allocation to
.I n
instead.
.PP
The button 3 menu was sketched in Section 2.
In general, drawing can be interrupted at any point by
pushing any button, then resumed by pushing button 1.
.PP
Four menu items control two variables:
.P1
faster [speed]
slower [speed]
.P2
decrease (halve) and increase (double) the pause at selected clicks, and
.P1
thinner [line width]
fatter [line width]
.P2
alter the width of lines.
(If the line thickness is @n@, then
.CW solid
lines are @2n - 1@ bits wide;
.CW fat
and
.CW fatfat
lines are larger.)
Three menu items control binary attributes:
.P1
backward    forward
or mode     xor mode
1 step      run
.P2
The mode displayed on the menu is the next state, not the current one.
If the program is currently in
.CW or
.CW mode ,
for instance, then
.CW xor
.CW mode
is displayed.
.PP
The
.CW new
.CW file
item allows one to view a new movie without downloading the
.I movie
program again.
After selecting that item,
text in the upper left of the window asks for the name of the intermediate
file to be processed.
The
.I movie
program does not call
.I develop
to make the intermediate
.CW .i ) (
file from the script
.CW .s ) (
file;
that is the responsibility of the user,
typically in a separate window.
.PP
Button 2 lists views and clicks.
Selecting a view results in an icon for sweeping a rectangle,
as in
.I mux .
Views may be positioned anywhere;
portions positioned outside the window will not be shown.
Initially, views have a 5 percent margin at each edge;
this margin is zero for views that have been reshaped.
If the window itself is reshaped, all views revert to
the default position and margin.
.PP
Normally, in 1-step mode, the display pauses after each
primitive object (line, text, etc.) has been drawn or erased.
If any clicks are defined and turned on by button 2, however,
then the display pauses only at those points.
Any number of clicks may be turned on.
Clicks that are turned on are marked with an asterisk;
they may be turned off by selecting them again.
.PP
As it is for the 5620, so it is for X workstations,
although the exigencies of the X window system
have forced us to curtail some features.
To keep the code relatively portable,
there are again two processes, so
the window in which one starts the animation clones another
window of uncontrolled size, shape and position
where the animation itself occurs.
.PP
The current terminal programs support
many, but not all, text size and line mode options.
.PP
If
.I movie
has a single argument, it must be either a
.CW .s
or
.CW .i
file;
.I movie
.I develop s
a
.CW .s
file.
If it has no arguments, then it will pipe
the standard input
through
.I fdevelop ;
no intermediate file is created.
For more exotic situations, use
.I fmovie :
with no arguments, it projects the intermediate file
from the standard input;
with a single argument, it projects that intermediate file.
.NH
The Stills Language
.PP
The
.I stills
program is a typical
.I troff
preprocessor.
Portions of its input bracketed by
.CW .begin
.CW stills
and
.CW .end
are translated into
.I pic
commands, and the rest of the input is passed through untouched.
A paper containing
.I stills
input is typically compiled by a command like
.P1
stills paper | pic | troff >paper.out
.P2
.PP
There are three classes of statements in a
.I stills
description:
.CW print ,
.CW view ,
and parameter assignments.
Only two statements are mandatory in a particular description:
an assignment to the
.CW file
parameter and a
.CW print
statement.
Text following the sharp symbol
.CW #
is discarded as a comment; blank lines are ignored.
.PP
There may be any number of
.CW print
statements of any combination of the following forms:
.P1
print all
print final
print @clickname@ all
print @clickname@ @number@ @number@ @number@ ...
.P2
The first statement causes a snapshot to be drawn at each
.CW click
statement in the script file; the second draws one at the end of the file.
The third form prints all
.CW click s
of the designated name, and the fourth prints only the clicks enumerated
in the list of numbers.
.PP
View statements select which views are to be printed in snapshots
and assign titles to views.
.P1
view  @name@  @optional@ @title@
.P2
The views appear in the order they are named,
either top-to-bottom if time goes
.CW across
the page or left-to-right if time goes
.CW down
the page.
If there are no
.CW view
statements, each snapshot depicts all views in the script.
If the title is enclosed in quote marks, they are stripped and
leading space is kept.
If no title is given for a particular view, the view name
itself is used as a title;
thus an empty title is needed to turn off printing.
.PP
Parameters can be set by assignment statements of the form
.P1
@parameter_name@  @value@
.P2
All parameters are reset to their default values at each
.CW .begin
.I stills
statement.
Numeric values may be of the form
.CW n ,
.CW +n ,
.CW -n ,
or absent (zero default).
.IP
.I Filename 
parameter:
.CW file .
The script file is named by assigning to the
.CW file
parameter with a statement of the form
.CW file
.CW basename.s .
The
.I stills
program
.I develop s
that script file and then reads
.CW basename.i .
.IP
.I "Text sizes" :
.CW "small medium big bigbig" .
The assignment
.CW small
.CW -2
causes text with the
.CW small
option to be printed two point sizes smaller than the current
.I troff
point size.
Assigning
.CW "+5"
or
.CW "5"
to
.CW "bigbig"
increases the point size by 5 for
.CW bigbig
text.
All changes are relative; there is no way to set
absolute point size.
.IP
.I "Line widths" :
.CW "solid fat fatfat" .
Relative size changes for line widths, exactly as for text sizes.
.IP
.I "Direction" :
.CW "across down" .
By default, snapshots proceed across the page in time.
The assignment
.CW down
.CW 5
causes time to proceed down the page for 5 snapshots before
starting a new column;
.CW down
.CW 0
or
.CW "down"
yields as many snapshots as will fit on an 8-inch page.
The assignment
.CW across
.CW 7
gives 7 snapshots in a row before starting a new row;
.CW across
.CW 0
or
.CW across
adapts to a 6-inch width.
.IP
.I "Frame parameters" :
.CW "frameht framewid margin" .
The height and width of frames are given in inches; defaults are 1.5.
The margin of a frame is the white space surrounding the data;
the default is 0.05, or a five percent border.
.IP
.I "Optional parts" :
.CW "frames times" .
Frames are the solid borders around pictures;
times are the click name and number that triggered a snapshot.
Both have the default value
.CW vis
and are shown; they may be suppressed by assigning them the value
.CW invis .
.PP
In summary,
.I stills
input consists of these commands:
.P1
print all
print final
print @clickname@ all
print @clickname@ @number@ @number@ @number@ ...
view  @name@  @optional@ @title@
@parameter_name@  @value@
.P2
.LP
At least one
.CW print
statement and a
.CW file
assignment are mandatory;
other statements are optional.
The parameter names in the right column may appear on the left side of a
name/value assignment:
.P1
.ta 1.1i
\f2Filename\fP	file
\f2Text sizes\fP	small medium big bigbig
\f2Line widths\fP	solid fat fatfat
\f2Direction\fP	across down
\f2Frame parameters\fP	frameht framewid margin
\f2Optional parts\fP	frames times
.P2
.........
.NH
Larger Animations
.PP
The examples in this section illustrate algorithms on several
classes of data structures, including arrays, trees and graphs.
The graphical style is simple; this is easy for the animator and
effective for the viewer.
The programs in this section are not presented as paradigms of
good programming style; rather, they show how succinct programs
can yield useful animations.
.PP
In the paper cited earlier, Brown and Sedgewick employ
several conceptual levels in animating an algorithm.
.IP
.I Execution.
At one extreme is the program to be animated,
executing on the data of interest.
.IP
.I "History of ``Interesting Events.''
What events in a computation should be depicted in the animation?
The simple animation of insertion sort in Section 2 used augmented
.CW less
and
.CW swap
routines to capture two primitives of most sorting programs:
comparisons and data movements.
It also explicitly recorded information associated with the
interesting event of finishing a phase.
.IP
.I "Geometric interpretation of events.
One must next decide how to represent
the interesting events pictorially.
An array of integers, for instance, might be represented by
a sequence of numbers, a scatterplot of bullets, or a sequence
of vertical bars.
.IP
.I "Rendering on a device.
In our system, this is the job of
.I movie
and
.I stills .
.LP
The sample program
.CW is.gen
identified interesting events and gave them
a geometric interpretation in separate procedures
within a single program.
Alternatively, it may be more convenient
to implement the various tasks as a pipeline of two programs:
a generator program writes interesting events that
are given a geometric interpretation by a program that writes
a script file.
.PP
One usually prints a
.CW click
statement immediately after the event it marks.
Sometimes, though, one draws additional objects to
highlight the event, which are erased immediately after the
.CW click .
In that case, one uses a sequence like
.P1
@draw@ @event@ @E@
@highlight@ @E@
click E
@remove@ @highlights@
.P2
.sp 1
.LP
.I "Sorting, Again."
.PP
We will return to the subject of
sorting with a more interesting animation:
a race of insertion sort versus quicksort.
We will use the same insertion sort we saw earlier,
and a quicksort described in Section 10.2 of |reference(programming pearls).
The two sorting routines and their supporting functions
are contained in this
.I awk
program:
.P1
.get race.gen
.P2
Quicksort is called as
.CW "qsort(l,u)"
to sort the subarray
.CW x[l..u] .
The function
.CW draw
represents the numbers to be sorted as vertical lines.
The
.CW BEGIN
block sets
.CW n ,
initializes the array,
draws the initial representation,
and then calls a sort routine.
As it stands, the code depicts a 50-element quicksort.
.PP
Some algorithm animation systems present races of programs
by implementing a simple form of time slicing.
To create a race in our system, we first ran the program into the file
.CW qs.s .
We then changed the final line in the
.CW BEGIN
section to call
.CW isort ,
and ran that into the file
.CW is.s .
Finally, we merged the two scripts into one with this
.I awk
program:
.P1
.get race.splice
.P2
The loop copies the script of insertion sort until it encounters a
.CW click
.CW comp
statement; it then copies quicksort until it encounters the corresponding
.CW click ,
at which points it prints the
.CW click
statement.
The variables
.CW s1
and
.CW s2
store the status of the most recent
.CW getline s
of the two files; the status is one if a record was found and
zero if an end-of-file was encountered.
When one file is exhausted, the remainder of the other is copied.
(A production splicer should
have a more graceful interface for naming the files and views.)
.PP
The resulting script file is an execution of the
sorts in two parallel views, synchronized by comparisons:
.INCLUDE race
As before, insertion sort sifts each element into place in turn.
This picture only hints at the operation of quicksort;
insight into that algorithm requires the identification
of interesting events beyond comparisons and swaps.
Quicksort finishes after 240 comparisons, while insertion
sort takes almost three times as long.
For completeness, here is the
.I stills
input that produced the picture:
.P1
.get race.t
.P2
.sp 1
.LP
.I "Trees."
.PP
Here are pictures of a (nonbalanced) binary search tree after inserting
5, 10, 15 and 20 random integers in the range 0..999:
.INCLUDE bst
In the last two frames, the labels for node 3
and 13 are squeezed too close together.
The script was generated by this
.I awk
program:
.P1
.get bst.gen
.P2
The animation code consists of just three lines in the
.CW insert
procedure.
.PP
The pictures give insight into random binary search trees
in spite of being rather ugly.
The trees have two distinct failings: the depiction of
individual nodes, and the layout of the entire tree.
.PP
A node is represented by its numeric value;
each node is connected to its parent.
If the shape of the tree is more important than
the values it contains, one can delete the values entirely.
We will shortly see a tree with more graceful edges.
.PP
The other aesthetic issue is the layout of the tree.
The @y@-value is the depth of the node in the tree,
which is a very robust choice
(one could also use the time at which the node was inserted).
The @x@-value in this example is simply the randomly generated value itself;
there are many alternative choices.
One could instead use the number of the node in an inorder traversal,
which involves a multiple-pass algorithm:
the tree is first built, then traversed and numbered, and the
insertions are then reported with knowledge of the numbers.
For this representation, it is crucial to separate the
interesting events from their geometric representation;
it is convenient to calculate these in two filters in a pipeline.
.PP
This animation of heapsort uses an alternative representation of trees.
It was generated by a 50-line
.I awk
program.
.INCLUDE heap
Each snapshot shows the result of a
.CW sift
operation.
The first four
.CW sift s
build the heap; subsequent
.CW sift s
maintain the unsorted elements as a heap.
Arrows point from lesser elements to greater elements.
.PP
A node in the heap is represented by its value;
lines between nodes have a single arrowhead and
are chopped by twenty percent at each end.
The root of the heap is at @(1/2,~-1)@,
its two children are at @(1/4,~-2)@ and @(3/4, ~-2)@, their four
children are at @(1/8,~-3)@, @(3/8,~-3)@, @(5/8,~-3)@, @(7/8,~-3)@, etc.
.PP
Other tree layouts proved useful in animating
two algorithms dealing with parse trees.
In both cases, a node's @x@-value was the minimum of the
@x@-values among the node's descendants (equivalently,
the @x@-value of its leftmost child); all edges were therefore
either vertical or slanting down to the right.
A random sentence generator built the tree left-to-right and top-down;
just as in the binary search tree,
the height of a node was one less than its parent.
A parser built the tree in postorder and bottom-up:
the height of a node was one greater than the
maximum of the heights of its children.
.sp 1
.LP
.I "A Graph Algorithm."
.PP
Prim's algorithm for computing the minimum spanning tree (MST)
of a graph starts with a fragment consisting of a single vertex.
It increases the fragment by adding the nearest vertex until all
vertices are in the fragment, at which point it is the MST
of the entire graph.
This picture shows Prim's algorithm on the complete
graph induced by a set of 50 planar points;
the weight of an edge between two points is
defined to be their Euclidean distance.
.INCLUDE mst2
The snapshots are taken every ten stages.
.PP
The obvious implementation of Prim's algorithm on an @N@-point
set requires time proportional to @N sup 3@.
Dijkstra discovered an elegant implementation of the algorithm
with running time proportional to @N sup 2@:
every point not in the fragment keeps a pointer
to its nearest neighbor in the fragment.
Here is Dijkstra's implementation on a ten-node planar graph:
.INCLUDE mst1
In this animation, nodes in the fragment are bullets, nodes
not in the fragment are crosses, edges in the MST are
.CW fat
lines, and the nearest neighbors are pointed to by arrows.
.PP
These two sequences illustrate two
styles of drawing graphs with our system.
Dots and lines are sufficient for simple algorithms,
while various symbols and line options can depict subtle processes.
.PP
The geometric nature of the above graphs made them easy to lay out.
Laying out a general graph is very hard.
If the graphs in your applications are specialized (such as trees),
you might exploit that structure to compute an effective layout.
A graph of @N@ vertices can be easily represented
by an @N times N@ matrix in which the @i,j@-th element
represents the edge from vertex @i@ to vertex @j@;
that is useful for insight into some graph algorithms.
If you have access to a program that produces good layouts of general graphs,
you might use that program to compute positions of vertices,
and then feed those into our system.
.sp 1
.LP
.I "A Memory Allocator."
.PP
The
.I develop
program uses the
.I malloc
memory allocator for several of its data structures.
We augmented
.I develop 's
calls to the allocator with data gathering
routines to produce an animation;
here is the final snapshot.
.KF
.INCLUDE malloc
.KE
The left frame is the arena of storage from which memory is allocated.
Memory blocks are represented by lines,
low addresses are at the bottom of the picture, and
the picture is 1024 bytes wide.
The right frame is a histogram of the sizes of memory currently
allocated; there is a dot for each element, a vertical bar every ten
positions to help counting, and the maximum value is in the lower right corner.
.PP
This snapshot was taken at the end of execution of
.I fdevelop .
The histogram shows two large blocks
of 20,000 and 40,000 bytes, 96 blocks of size 16, and 96
slightly larger blocks (the blocks of size 16 are symbol table records;
each points to an allocated string).
The arena shows the two huge pieces (the larger is higher)
and a gap above the smaller (memory allocated by
procedures that didn't call our augmented
.I malloc ).
The remainder of the arena is allocated efficiently.
.PP
A movie like this helped us find a bug in
.I fdevelop :
an early version allocated the symbol table nodes but did not
.I free
them.
This problem manifested itself in an overloaded arena and a huge
spike in the histogram at size 16.
.PP
The
.I fdevelop
program interacts with the storage allocator only through
the two routines
.I emalloc
and
.I efree .
We animated the storage allocator by modifying those routines:
.P1
.get malloc.c
.P2
They write on the named file output lines of two types:
.P1
m @address@ @length@
f @address@
.P2
The first line denotes that a
.I malloc
of the given length returned the given address;
the second marks a
.I free .
.PP
The resulting history file contains the interesting events;
they are given a geometric interpretation by a subsequent program.
Here is a simple program that generates only the arena view
from the script file:
.P1
.get malloc.awk
.P2
The
.CW BEGIN
block initializes variables,
the actions for
.CW m
lines draw memory, and those for
.CW f
lines erase memory.
The variable
.CW s
is the starting byte of a block;
.CW e
is the ending block.
The variables
.CW sx
and
.CW sy
are the @x@ and @y@ positions of the starting byte, and
similarly for
.CW ex
and
.CW ey .
If the block fits on one line, then only a single line fragment
need be drawn; otherwise, fragments of three types are needed.
.PP
The complete program for generating scripts
from history files is 56 lines of
.I awk .
To ease tracing the action in the arena, it marks a
.I malloc
with an
.CW x
and a
.I free
with an
.CW o .
The histogram view is embellished with bars and the maximum value.
There is room for further elaborations;
one might, for instance, want to put either or both of
the axes of the histogram on logarithmic scales.
.sp 1
.LP
.I "Dynamic Statistical Displays."
.PP
Rick Becker constructed this display of air pollution
in the Northeast United States:
.INCLUDE ozone
The data was gathered hourly on
a single summer day in the early 1970's.
The radius of each circle is proportional to the ozone
reading (a common measurement of air pollution)
at one of 32 stations in New Jersey,
New York, Connecticut, and Massachusetts.
The radius of the clock denotes the maximum ozone level prescribed
by the Environmental Protection Agency;
its hour hand goes from 7:00 AM to midnight.
.PP
This display shows how New York City's
air pollution is blown to the northeast.
Becker and his colleagues prepared a similar movie in the
early 1970's using the technology of the day;
it required several weeks of programming time, then
a weekend with a movie camera to make the final film.
He built this display from the same data in a couple of hours,
then prepared a video tape of the
resulting movie in under thirty minutes.
The original movie was a bit nicer (it drew the background
map and the circles in two different colors),
but the new movie is just as useful, and provides stills for free.
......
.NH
Living With The System
.PP
The system that we have described is the
bare bones of an animation environment.
We have found that the most fruitful way of enhancing the
environment is not by modifying the primary programs,
but rather by using small filters that interact with the
various files in the system.
.PP
We showed earlier, for instance, a race of two sorting algorithms.
While other animation systems implement races with
a general mechanism for time sharing,
we did the job with a small
.I awk
program that merges two files.
Our system does not have a facility for counting clicks;
rather, we use filters such as
.P1
grep 'click comps' | wc
.P2
to see how many comparisons were made.
We will even admit to using text editors to make minor changes
to both script and intermediate files.
.PP
We have built several useful filters in addition to
.I merge .
The program
.I view.clicks
prints a summary of the views and clicks used in a script file;
it is helpful as one is preparing a
.I stills
file.
Its implementation uses the intermediate file described
in Appendix I:
.P1
.get view.clicks
.P2
The first step of the shell script is to develop the script file;
the subsequent
.I awk
program then reads the resulting intermediate file.
The first pattern/action pair prints for each view its
name and the @x@ and @y@ ranges in the script file.
The second pair builds a string of all clicks from the
define click statements, and the third pair prints that
string and exits at the first non-define statement.
.PP
The program
.I show.clicks
takes a script file as input;
its output is a new script file containing all information in
the input and, in addition, a new view named
.CW click.count
in which the various clicks are counted.
This is useful for preparing
.I stills
files and for debugging.
Its input is a script file,
either named explicitly or as the standard input.
Here is the code:
.P1
.get show.clicks
.P2
The second pattern/action pair stores the name of the current view,
and the fourth action copies each line onto the output file.
The first pattern/action pair supplies a default view name, if needed.
.PP
The work is done when the third pattern recognizes a
.CW click
statement.
The
.CW if
statement puts the click name in
.CW cname ,
and the
.CW print
statement switches to the new view.
The second
.CW if
statement is executed when a new
.CW click
statement is seen; it assigns it a number and prints out the
click name, once and for all.
The two following statements rewrite the appropriate count
and place a bullet next to it.
The final statement returns to the current view.
.PP
One can use the ideas in
.I show.clicks
to process lines in the script file of the form
.P1
@name@ = @value@
.P2
The output script file has a new view named
.CW variables ;
it contains the name of each variable mentioned and its current value.
.PP
Larger filters have also proven useful.
For instance, we built a set of tools to render animations
of three-dimensional lines and text
(circles and rectangles were not supported).
The primary program translated a three-dimensional script into a
standard script that contained two two-dimensional views for
each three-dimensional view; the resulting
.I movie
and
.I stills
were suitable for viewing with standard stereo viewers.
Support programs included a filter for rotating a view
around a given line.
.PP
The
.I movie
and
.I develop
programs are in fact simple shell scripts that call filter
versions named
.I fmovie
and
.I fdevelop .
You may find it convenient to rewrite those shell scripts for
your environment.
.SH
Acknowledgements
.PP
We are deeply indebted to Howard Trickey;
he gave us invaluable advice for getting a minimal
animation facility working in the Sun environment,
then finished the job properly.
He subsequently made it all work under the X window system.
Andrew Hume and Jane Elliott made possible
our first experiments with animation.
Our early users, Rick Becker and Chris Van Wyk,
gave us bug reports and suggestions for improvements.
Eric Grosse, John Linderman, Doug McIlroy,
Steve Mahaney, Howard Trickey, and Chris Van Wyk
made helpful comments on this paper.
.SH
References
.LP
|reference_placement
.sp 100
.BP
.SH
Appendix I \(em Intermediate Files
.PP
This appendix defines the intermediate files produced by
.I develop .
The files are easier to process than the corresponding script files.
For instance,
names are converted to small integers,
floating point numbers are scaled to integers in 0..9999,
and commands are abbreviated to single letters.
.PP
A line whose first non-blank character is
.CW #
is a comment.
.PP
An intermediate file begins with define statements
that give the names of the views and the clicks:
.P1
d v @vnum@ @viewname@ @minx@ @miny@ @maxx@ @maxy@
d c @cnum@ @clickname@
d p @any@ @text@
d p e
.P2
Fields are separated by tab characters.
Both views and clicks are numbered 0, 1, 2, ....
The four final numbers on a view line tell the
range of the coordinates in the original script file.
A line that begins with
.CW "d p"
is a ``pragma''; both
.I movie
and
.I stills
currently ignore all such lines.
The defines appear at the front of the file in
the order views, clicks, pragmas, then
the ``end of defines'' pragma
.CW "d p e" .
.PP
The geometric commands for lines, boxes, circles and text
are mapped to the following:
.P1
g @slotnum@ l @vnum@ @opts@ @x sub 1@ @y sub 1@ @x sub 2@ @y sub 2@
g @slotnum@ b @vnum@ @opts@ @x sub 1@ @y sub 1@ @x sub 2@ @y sub 2@
g @slotnum@ c @vnum@ @opts@ @x@ @y@ @rad@
g @slotnum@ t @vnum@ @opts@ @x@ @y@ @text@ @string@
.P2
Objects that are never erased have slot number 0;
other objects are placed in ``slots'' that can hold at most one object.
The third field is the type of geometric object;
the fourth field is the view number.
All @x@ and @y@ values are normalized to integers in the range 0..9999.
There is a single separating tab before the (unquoted) text string.
Options are given as a string of characters,
whose length and interpretation are summarized as:
.P1
.ps -2
.vs -3
OBJECT	POS	NAME	ABBREV
text	1	center	c
		ljust	l
		rjust	r
		above	a
		below	b
	2	medium	m
		small	s
		big	b
		bigbig	B
line	1	solid	s
		fat	f
		fatfat	F
		dotted	o
		dashed	a
	2	-	-
		->	>
		<-	<
		<->	x
box	1	nofill	n
		fill	f
circle	1	nofill	n
		fill	f
.vs +3
.ps +2
.P2
For instance, the options for a text string are described
by two characters giving its position and its size;
.CW small
.CW center
text has the option string
.CW cs .
Further options may be added at the right end of the string;
subsequent programs should ignore letters they don't expect.
.PP
A click statement is represented as
.P1
c @cnum@
.P2
.PP
An erase statement is translated into
.P1
e @line@ @repeated@ @here@, @except@ e @italic "for"@ @leading@ g
.P2
A processor may choose to implement this statement using
either the geometric description or the slot number.
.PP
A
.CW clear
statement (which erases all objects in a view)
is translated into a pair of starting and ending ``blank''
commands that bracket a sequence of erase statements:
.P1
b s @vnum@
b e @vnum@
.P2
The erase statements together clear the view.
A processor may choose to implement the
.CW clear
either by ignoring the
.CW b
commands and letting the erase statements
take their course or by explicitly processing the start
command and then ignoring
.CW e
commands until encountering the
.CW "b e"
line.
.PP
As an example, running
.CW develop
on this trivial script file
.P1
line 1 2 3 4
text small above 5 6 "Hello, world."
click stage
clear
.P2
produces this intermediate file:
.P1
d	v	0	def.view	1	2	5	6
d	c	0	stage
d	p	e
g	1	l	0	s-	0	0	4999	4999
g	2	t	0	as	9999	9999	Hello, world.
c	0
b	s	0
e	1	l	0	s-	0	0	4999	4999
e	2	t	0	as	9999	9999	Hello, world.
b	e	0
.P2
.PP
Here is a summary of the commands in the intermediate language:
.P1
# @comment@
b s @vnum@
b e @vnum@
c @cnum@
d v @vnum@ @viewname@ @minx@ @miny@ @maxx@ @maxy@
d c @cnum@ @clickname@
d p @any@ @text@
d p e
e @line@ @repeated@ @here@, @except@ e @italic "for"@ @leading@ g
g @slotnum@ l @vnum@ @opts@ @x sub 1@ @y sub 1@ @x sub 2@ @y sub 2@
g @slotnum@ b @vnum@ @opts@ @x sub 1@ @y sub 1@ @x sub 2@ @y sub 2@
g @slotnum@ c @vnum@ @opts@ @x@ @y@ @rad@
g @slotnum@ t @vnum@ @opts@ @x@ @y@ @text@ @string@
.P2

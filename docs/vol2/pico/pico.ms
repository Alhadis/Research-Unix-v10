.so ../ADM/mac
.XX pico 423 "Pico \(em A Language For Composing Digital Images"
.nr dP 2
.nr dV 3p
.EQ
delim @#
.EN
.ND July, 1989
.TL
Pico \(em A Language For Composing Digital Images
.AU "MH 2C-521" 6335
Gerard J. Holzmann
.AI
.MH
.AB
\fIPico\fP is a small expression language for picture compositions.
It can be used either interactively with a display or stand alone as a
picture file editor.
The following \fIpico\fP script, for instance, turns an arbitrary digitized
image stored in a file 
.CW in 
upside down, rotates it by 90 degrees,
and writes its negative into a file 
.CW out :
.P1 20n
$ pico
1: new = Z-$in[X-y,x]
2: w out
3: q
$
.P2
.PP
Numerous examples of
.I pico
transformations of pictures are included throughout this volume.
.AE
.2C
.NH
Black&White Images
.PP
The pictures that can be manipulated by \fIpico\fP are stored as
regular files in the picture file format described in
.I picfile (5).
The picture editor is most conveniently used interactively
with a Metheus frame buffer display.
The result of picture transformations is then directly visible
and can be used to correct or enhance the mistakes that one is
bound to make.
.SH
New and Old
.PP
Assuming that you want to work interactively and have access to one
of the Metheus frame buffers, e.g. on 
.CW pipe ,
a session with \fIpico\fP can be started by typing
.P1 0
$ pico -m\fIN\fP
b&w, 512x512 pel, Metheus display
1: 
.P2
where
.I N
is the device number of the frame buffer to be used.
By default,
.I N
is zero and opens
.CW /dev/om0 .
Also by default the size of the workarea is 512x512 pixels.
If you need to work with larger images, you
can override the default by explicitly setting a different
window width and height, for instance:
.P1 0
$ pico -m5 -w1024 -h1024
b&w, 1024x1024 pel, Metheus display
1: 
.P2
The maximum size image that the Metheus frame buffer
can display is 1280x1024 pixels.
.PP
The number followed by a colon in the example above
is \fIpico\fP's prompt for commands.
.PP
The result of the last edit operation (initially an all black image)
is accessible under the predefined name 
.CW old ,
and
the destination of the image transformations is known as 
.CW new .
To quickly get a picture into the workbuffer, you can use the command 
.CW get
followed by the name of the file with the image.
.P1 0
1: get "pjw"
.P2
The most frequently used command in \fIpico\fP is 
.CW x ,
short for 
.CW execute .
To make a black&white negative from the current picture
the command would be:
.P1 0
2: x new=Z-old
.P2
where Z is a predefined constant with the value of maximum white (255).
By default the transformation is applied to every pixel on the screen.
.PP
Assume we have two image files with portraits.
We can open these files by using 
.CW get ,
or we can specify them
on the \fIpico\fP command line, as follows, using
.CW /dev/om5 :
.P1 0
$ pico -m5 ./face/rob ./face/pjw
b&w, 512x512 pel, Metheus display
1: 
.P2
We can create a new image, for instance, by averaging the two faces:
.P1 0
$ pico -m5 ./face/rob ./face/pjw
b&w, 512x512 pel, Metheus display
1: x new=($rob+$pjw)/2
2: 
.P2
The transformation is written as an assignment of an
expression to the destination 
.CW new .
Names preceded by a dollar sign refer to picture files,
for instance as specified on the command line.
A long name such as 
.CW ./face/rob 
can be abbreviated
to its base name 
.CW rob 
(the part following the last slash).
Not all file names have to be provided on the command line though.
We can also 
.CW append 
a new file 
.CW doug ,
without reading it, by typing:
.P1 0
2: a "doug"
.P2
The double quotes are necessary.
They avoid confusion when, for instance, 
.CW / 
symbols
are part of the filename.
We can check which files are currently open by typing 
.CW f :
.P1 0
3: f
$0: old	color	resident
$1: rob  b&w resident
$2: pjw  b&w resident
$3: doug b&w absent
.P2
The numbers in the first column serve as a shorthand for the file names.
Typing 
.CW $1 
therefore is equivalent to typing 
.CW $rob .
We will use both notations 
.CW $1 
and 
.CW $rob 
below.
The first line 
.CW $0 
is a shorthand for 
.CW old 
and refers to
the workarea (the screen in interactive usage).
.PP
We have a black and white image on the screen that is an average
of the two files 
.CW rob 
and 
.CW pjw .
To see 
.CW rob 
separately we could type:
.P1 0
4: x new=$1
.P2
but that is hardly an inspiring procedure.
Let's just take the left half
or rob's face combined with the right half of peter's:
.P1 0
5: x new=(x<256)?$rob:$pjw
.P2
or to make a mirror image:
.P1 0
5: x new=(x<256)?$rob[x,y]:$rob[X-x,y]
.P2
The variable 
.CW x 
used in the expression is predefined.
Don't confuse it with the first 
.CW x 
on the command line
which identifies the type of command to
be executed.
The variable 
.CW x 
gives the current x-coordinate of a pixel
during transformations.
Since in this case 512 pixels fit on one scan line, a pixel in
the middle of the screen has an 
.CW x -coordinate
of 256.
The maximum value of 
.CW x 
is given by a predefined variable 
.CW X .
.CW X/2 ,
therefore, is a safer way to specify the middle of a scan line.
.PP
The expression above is a conditional of the form:
.P1 0
\fIcondition\fP ? \fIiftrue\fP : \fIiffalse\fP
.P2
For every screen position where the condition holds the 
.I iftrue 
part
of the expression applies and everywhere else the 
.I iffalse 
part applies.
Another predefined variable of this type is 
.CW y
(the y-coordinate of the destination).
The maximum 
.CW y 
value is called 
.CW Y .
Since
.CW $0
refers to the screen we can turn the picture
on the screen upside down by typing:
.P1 0
6: x new = $0[x, Y-y]
.P2
All pixels in the black&white picture are internally represented by a
value in the range 0..255, where 0 means black and 255 means white.
To reverse an image, therefore it would suffice to subtract the current
value of each pixel from its maximum value 255, which is stored in constant 
.CW Z .
Getting very bold we can turn the picture on its side,
and make it negative by saying:
.P1 0
8: x new = 255 - old[y,511-x]
.P2
or, slightly more abstract
.P1 0
9: x new = Z - old[y,Y-x]
.P2
Note that we swapped x and y to turn the picture on its side.
Nothing can stop us now:
.P1 0
10: x new=(x<X/3)?$1:(x>X*2/3)?$2: \*(cr
	3*((x-X/3)*$2+(X*2/3-x)*$1)/X
.P2
fades 
.CW rob 
slowly into 
.CW pjw .
(We have used \*(cr to break the line into
two pieces for lay-out purposes.
When using \fIpico\fP, it should be typed as one complete line.)
Actually this last transformation is easier to read as a little \fIpico\fP
program.
To see how this works, and what the defaults in the above expression
are, the above expression could be typed as:
.P1 0
10: x {
	int L, R

	L = X/3; R = X*2/3

	for (y = 0; y < Y; y++)
	for (x = 0; x < X; x++)
	{	if (x < L)
			new[x,y] = $1
		else if (x > R)
			new[x,y] = $2
		else
			new[x,y] = 3*((x-L)* \*(cr
				$2+(R-x)*$1)/X
	}
}
.P2
There fewer defaults here, though an assignment to 
.CW new 
is still
interpreted as a parallel assignment to all three color channels in the
picture.
You can override also these defaults by making the program still more explicit,
for instance by using the suffixes 
.CW red , 
.CW grn ,
and 
.CW blu 
to access
color channels separately: 
.CW new[x,y].red , 
.CW new[x,y].grn ,
and 
.CW new[x,y].blu .
To see the effect you need to set the workbuffer to color mode first
with the command 
.CW color .
(You go back to the default black&white mode with the command 
.CW nocolor ).
.PP
All normal arithmetic operators from C are available.
The 
.CW ^
operator, for instance, makes an 
.I "exclusive or"
of its operands.
Thus,
.P1 0
11: x new=x^y^$rob
.P2
is a particularly striking effect, and
.P1 0
13: x new = $rob +(Z - $rob[x+2, y+2])
.P2
is an attempt to make a relief.
.PP
There is no range checking on explicit or implicit array indexing.
The use of 
.CW x+2 
in the last expression is therefore risky
and is best protected with a conditional:
.P1 0
13: x new=$rob+(Z-(x<509 && y<509)? \*(cr
		$rob[x+2,y+2]:Z)
.P2
or more conveniently with the builtins 
.CW xclamp
and 
.CW yclamp :
.P1 0
14: x new=$rob+(Z-$rob[xclamp(x+2), \*(cr
		yclamp(y+2)])
.P2
Another promising attempt to make a core dump would be to type
something like
.P1 0
14: x new=$rob[x*y, x/y]
.P2
.NH
Color Images
.PP
A complete picture specifies pixel values for each of three
separate color channels: red, green, and blue.
When the editor is used in black&white mode only the
red channel is used.
When a black&white picture is converted to color mode,
all three channels are made equal.
The omission of a channel suffix to 
.CW old , 
.CW new 
or a file name
is similarly interpreted to mean that a transformation expression will
apply equally to all three color channels.
By specifying an explicit suffix
.CW red , 
.CW grn ,
or 
.CW blu ,
however, we can write each channel separately.
So:
.P1 0
14: color
15: x new.red=$rob
16: x new.grn=$rob
17: x new.blu=255-$rob
.P2
will write 
.CW rob 
on the red and green channels, and its negative on the
blue channel.
If 
.CW rob 
is a black&white picture then typing
.CW $rob
is, of course,
equivalent to typing
.CW $rob.red .
We could also have combined the first two lines in a chain assignment:
.P1 0
18: x new.red=new.grn=$rob
.P2
We can also write a separate value to each channel
by using 
.CW composites .
A color composite is written as a comma separated list
of three values, enclosed in square brackets:
.P1 0
19: x new.rgb=[$rob,$rob,Z-$rob ]
.P2
The channels are addressed by the three fields of the composite in
the order: [red, green, blue].
Omitting to specify a composite when an 
.CW rgb 
destination is
used typically results in only the red channel being written.
As expected,
.P1 0
20: x new.rgb=[old.grn,old.blu,old.red ]
.P2
rotates the colors of the picture.
And, of course, you can freely combine
the color suffixes with array indexing:
.CW $rob.blu
is just a shorthand
for 
.CW "$rob[x, y].blu "
where the variables 
.CW x ,
and 
.CW y 
can be replaced by just any monstrous C-style expression.
.NH
The Color Maps
.PP
When working interactively,
the color map in the Metheus frame buffer display
can be set with one of the commands
.CW cmap
(all channels),
.CW cmap.red ,
.CW cmap.grn , or
.CW cmap.blu .
The color map is a mapping table that can arbitrarily map
pixel brightness values in the range 0..255 to other
brightness values, within the same range 0..255.
The update, however, only happens on the screen and is not stored
when the image file is written.
The variable 
.CW i 
is used to index the color map.
For instance:
.P1 0
21: x cmap = Z-i
.P2
will very quickly make a negative, and
.P1 0
22: x cmap = i
.P2
turns the picture back to normal.
To fake color in a black and white image you can try:
.P1 0
23: x cmap.red = (i<=85)?i:0
24: x cmap.grn = (i>85 && i<170)?i:0
25: x cmap.blu = (i>=170)?i:0
.P2
Remember that changing the color map only changes the appearance
of the picture on the screen, not its definition in memory.
.NH
Read, Write, and Windows
.PP
The 
.CW append
command, to add files to the list of dollar arguments
was discussed before.
Using the command 
.CW get 
instead of 
.CW a 
will also put the file into $0,
that is on the screen.
To save the current state of the display in a file, use:
.P1 0
26: w filename
.P2
A raw black&white picture file, without the picture file header,
can be written by using 
.CW "w -" 
instead of 
.CW w
(for instance when dumping a file to be processed
by software uneducated in picture file headers).
The size of the file written conforms to the
current window size of the editor (see also below).
To close a no longer used file and free up some memory for others, say:
.P1 0
27: d doug
.P2
or
.P1 0
27: d $1
.P2
giving the file's base name or its dollar number.
To restrict the updates to a window on the screen you can set:
.P1 0
30: window 10 100 200 300
.P2
which makes a window with origin at (x,y) = (10,100), 200 pixels wide
and 300 pixels deep.
And, if you really want to exit \fIpico\fP you can type a control-D
or resort to the 
.CW quit
command:
.P1 0
32: q
.P2
.NH
Programs
.PP
As shown in one of the examples above, it is possible to write
small \fIpico\fP programs for the more difficult transformations
that cannot be handled by the defaults.
The control structure for the \fIpico\fP programs is again stolen from C.
.PP
A \fIpico\fP program starts with a left curly brace 
.CW { 
followed by
zero or more declarations of (long) integers or arrays.
For instance,
.P1 0
33: x {
	int a, b; array ken[100]

	...
.P2
Note carefully that the left curly brace turns off the default control
flow over all the pixels in a picture; the control flow in a program
must be specified explicitly.
The above program fragment declares two local integers 
.CW a 
and 
.CW b 
which
by default will be initialized to zero,
and an array of 100 (long) integers named 
.CW ken ,
also initialized to zeros.
To initialize it to another value, use constants:
.P1 0
int a = 9
.P2
Statements can either be separated by newlines or by explicit semicolons.
Here then is a list of valid types of statements:
.P1 0
\fIlvalue\fP = \fIexpr\fP
if (\fIexpr\fP) \fIstmnt\fP
if (\fIexpr\fP) \fIstmnt\fP else \fIstmnt\fP
for (\fIexpr\fP; \fIexpr\fP; \fIexpr\fP) \fIstmnt\fP
while (\fIexpr\fP) \fIstmnt\fP
do \fIstmnt\fP while (\fIexpr\fP)
\fIlabel\fP: \fIstmnt\fP
goto label
{ \fIstmnt\fP }
.P2
An \fIlvalue\fP is an explicitly declared local variable or array element,
one of the predefined variables 
.CW x ,
or 
.CW y ,
or a picture
element.
A picture element can again be a file name such as
.CW $doug
with a
default selection of the pixel inside it,
.CW $doug[x,y] ,
or it can be
more elaborate as in
.CW "$doug[x/2, y<<1].red" .
The destination of the transformation, is again referred to by
the keyword 
.CW new , 
but this time it needs an explicit array indexing.
The equivalent of
.P1 0
34: x new=old[y,x]
.P2
to turn the picture on its side, can be written as a \fIpico\fP program:
.P1 0
35: x {
	for (y = 0; y < 512; y++)
	for (x = 0; x < 512; x++)
		new[x,y] = old[y,x]
}
.P2
Note that also the control flow must be explicit.
Typing only
.P1 0
36: x { new[x,y] = old[y,x] }
.P2
would use the initial (zero) values of 
.CW x 
and 
.CW y 
and
merely assign
.CW "new[0,0] = old[0,0]" .
.NH
Array Indexing and Control Flow Defaults
.PP
One more word about defaults.
\fIPico\fP tries to be smart about assigning types to values.
When a single rvalue is needed and a color composite is available
and average of the color channels is the default, for instance:
.P1 0
.CW old becomes
.CW (old[x,y].red+old[x,y].grn+old[x,y].blu)/3 .
.P2
If on the other hand a value is available and a composite is
needed the value will be replicated into a fake composite.
To override the defaults assignments can of course always
be made more explicit.
Normal cases should work as expected, for instance, by default:
.P1 0
new = old
.P2
truly means
.P1 0
new.red=old.red;
new.grn=old.grn;
new.blu=old.blu
.P2
.NH
Procedures
.PP
There is a facility in \fIpico\fP to declare named segments of code and
use these as functions or procedures.
As an example, the following command declares a procedure 
.CW doit
that makes a histogram of a window of pixels on the screen.
It is equivalent to the sequence:
.P1 0
37: window a b w d
38: x { global array histog[256]; }
39: x histo[old]++
40: window 0 0 512 512
.P2
In a procedure this is written as:
.P1 0
37: def doit(a, b, w, d) {
	global array histog[256]

	for (y = a; y < a+w; y++)
	for (x = b; x < b+d; x++)
		histog[old[x,y]]++
}
.P2
The declaration prefix 
.CW global 
extends the scope of an array so that it can be
referred to in subsequent procedures, programs or expressions.
We can now call the procedure and use the histogram to arbitrarily
change the color map:
.P1 0
38: x { doit(0,0,512,512); }
39: x cmap = histog[i]%256
.P2
Or more usefully, to calculate and apply a simple
histogram equalization:
.P1 0
40: x {
	int ave, i, j, R, L, Hint;
	global array eqlz[256]

.P3
	for (i = ave = 0; i < 256; i++)
		ave += histog[i];
	ave /= 256

.P3
	for (i = R = Hint = 0; i < 256; i++)
	{	L = R
		Hint += histog[i]
		while (Hint > ave)
		{	Hint -= ave
			R++
		}
		j = (L+R)/2
		eqlz[i] = (j>255)?255:j
	}
}
41: x new=eqlz[old]
.P2
When \fIpico\fP starts up it reads a small set of library procedures
by simulating an 
.CW r 
command (see Table 1).
These definitions are stored in the file
.CW  /usr/lib/pico/defines .
The builtins listed in Table 2 can be used in \fIpico\fP programs and
\fIpico\fP procedures.
.NH
Non-Interactive Use of \fIPico\fP
.PP
When \fIpico\fP is used without having access to a frame buffer
all commands will still work.
To check the result of executing commands
it can be convenient to write what would have been the current screen image
into a file with the 
.CW w 
command
and view it in another \fImux\fP-window on the 5620 (or 630) terminal with
a command like 
.I flicks .
For instance, in one window the session can be:
.P1 0
$ pico rob pjw
1: nocolor
2: x new=($rob*$pjw)/255
3: w - junk
4: 
.P2
While in another window the file is displayed
with 
.I flicks , 
as follows:
.P1 0
$ flicks -em junk
.P2
.NH
See Also
.PP
More examples on how to use \fIpico\fP, and on its internal structure,
can be found in:
|reference(pico %no_cite)|reference(digital darkroom %no_cite)
.PP
|reference_placement
.FC
.1C
.TS
center;
c c
lFCW lw(4i).
_
COMMAND			DESCRIPTION
=
a [x y w d] file	T{
append file with optional offset/dimensions
T}
d basename/$n	delete file
h file	T{
read header information from file(s)
T}
r file	read (library) command file
w [-] file	T{
write file or window with or without a header
format: default = \fIpico\fP header, \- means no header
T}
_
nocolor	T{
update & display only 1 channel
T}
color	T{
update & display all 3 channels
T}
_
window x y w d	T{
restrict workarea to this window
T}
_
get [x y w d] file	T{
read file contents into 
.CW old
T}
get $n	T{
refresh 
.CW old 
with an already opened file
T}
_
f	show mounted files
show [name]	T{
show symbol information (values)
T}
functions	show functions
_
def name { pprog }	define a function
x expr	execute expr in default loop
x { pprog }	execute \fIpico\fP program
_
q	quit
_
.TE
.ce
File names containing nonalphanumeric characters (period, slash) must be enclosed in double quotes.
.sp .5
.ce
\fBTable 1.\fR Command Summary
.SP 3
.TS
center;
lFCW lw(4i).
_
printf(string, args)	recognizes only: \f(CW%d\fP, \f(CW%s\fP, \f(CW\en\fP, \f(CW\et\fP
x_cart(radius, angle)	T{
convert radius and angle (degrees: 0..360) into x_coordinate
T}
y_cart(radius, angle)	T{
convert radius and angle (degrees: 0..360) into y_coordinate
T}
X_cart(r,a),Y_cart(r,a)	T{
same as [\fIxy\fP]_\fIcart\fP, but expects \fIangle\fP in centidegrees
T}
r_polar(x,y)	convert \fIx,y\fP coordinate into radius
a_polar(x,y)	T{
angle returned is in degrees: 0..360, acuuracy=\(+- 2 degrees
T}
A_polar(x,y)	angle returned is in centidegrees: 0..36000
putframe(nr)	dump window into the file \f(CW"frame.%6d"\fP, \fInr\fP
getframe(nr)	T{
read from the file \f(CW"frame.%6d"\fP, \fInr\fP
T}
setcmap(i, r,g,b)	write \fIi\fPth value in colormap
getcmap(i, r,g,b)	read \fIi\fPth value in colormap
redcmap(i, z)
grncmap(), blucmap()
_
sin(angle), cos(angle)	T{
returns 0..10,000, \fIangle\fP in degrees : 0..360
T}
Sin(angle), Cos(angle)	T{
same but expects \fIangle\fP in centidegrees: 0..36000
T}
atan(x, y)	T{
arc-tangent of \fIy/x\fP, returns angle in degrees: 0..360
T}
exp(a)	as advertised
log(a),log10(a)	returns 1024*result
sqrt(a)	integer square root of \fIa\fP
pow(a,b)	\fIa\fP to the power \fIb\fP
rand()	returns a random integer \fIr\fP: 0\(<=\fIr\fP<32768.
_
.TE
.ce
\fBTable 2.\fR Builtin Procedures

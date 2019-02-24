.so ../ADM/mac
.XX 36 571 "A Research Picture File Format and I/O Library"
.fp 5 T CW	\" T for Typewriter
.TL
A Research Picture File Format and I/O Library
.AU
Tom Duff
Don Mitchell
.AI
.MH
.SP
.2C
.NH
Introduction
.PP
Researchers in centers 1122, 1125 and 1127 have for several years used
a general file format for storing digital images,
depth maps, mattes, and other two-dimensional digital signals.
We have a convenient, portable library of C-language functions for
manipulating picture files and a rapidly growing collection of graphics
and image-processing software that uses the picfile format and library.
.1C
.KF bottom
.P1 0
TYPE=dump
WINDOW=0 0 512 512
NCHAN=1
CHAN=m
COMMAND= antiquantize 'halftone CLASSIC' 512.halftone LIBERTY.anticlassic 
COMMAND=  halftone CLASSIC 512.liberty 512.halftone 1.75 512.halftone 
COMMAND=   transpose IN OUT 
COMMAND=    resample 512 IN OUT 
COMMAND=     transpose IN OUT 
COMMAND=      resample 512 IN OUT 
COMMAND=       clip 400 400 LIBERTY OUT
.P2
.ce
Figure 1
.KE
.2C
.NH
The File Format
.PP
A picture file (often called a `picfile') contains a variable-length ascii header
followed by a optional color-look-up table
and binary data representing a two-dimensional array of multi-byte pixels.
.PP
The header is a list of attribute/value pairs
separated by newlines, terminated by an
empty line.  A pure ascii header is
machine independent.
Its open-ended form allows applications to annotate pictures
any way that they wish.  Figure 1 shows a typical header.
Each header line is of the form
.I `attribute=value'
where
.I attribute
is any string not containing an equal sign `=', an ascii nul (zero byte), or a
newline, and
.I value
is any string not containing a newline or nul.  The newline-terminated
entries are separated from the binary data by an empty line (i.e. a pair of newlines).
Thus,
.P1 0
sed '/^$/q' \fIfilename\fP
.P2
will print a picfile's header.
.PP
Several attributes are mandatory.
The
.CW TYPE
attribute
.I must
come first,
acting as a ``magic number'' and specifying the binary encoding of the
image data.  Currently, five binary formats are supported:
.IP \fTTYPE=dump\fP 5n
.br
The binary data follows in traditional scan-line order.
The first pixel in the file is at the upper left corner of
the image.
.I X
coordinates increase to the
.I right
on a
display, and
.I y
coordinates increase
.I down.
Pixels are stored in row-major order (i.e.,
.I x
increases faster than
.I y ).
.IP \fTTYPE=runcode\fP
.br
Each pixel is preceded by a one-byte count that indicates how many adjacent
pixels have the given value.  Count
values of 0 to 255 represent a replication factor of 1 to 256.  No
run may extend from one scan-line to the next.
This format is useful in reducing the size of mattes or pictures containing
large areas of constant color, but it may be a poor choice for digitized
natural images or pictures containing noise.
.IP \fTTYPE=bitmap\fP
.br
Pixels are one bit each, packed
8 bits to a byte, with the leftmost pixel in the high-order bit of
the first byte.  Each row must be an even number of bytes,
so there can be up to 15 zero bits after the right-most pixel.
A bit value of 1 represents black; 0 represents white.
This is exactly the encoding used by SUN Microsystems and compatible
manufacturers.
.IP \fTTYPE=ccitt-g4\fP
.br
A black-and-white image encoded according to the CCITT FAX Group 4 standard.
This is highly compressive on printed text and sparse line-graphics.
.IP \fTTYPE=ccir601\fP
.br
The image is encoded according to the IEEE digital component video standard.
(Y,U,V) values are stored as 8-bit values with chrominance compressed to half
as much bandwidth as luminance.  The layout of each row is
Y U Y V Y U Y V ...
.LP
The other mandatory attribute is:
.IP "\fTWINDOW=\fIx0 y0 x1 y1\fR"
.br
Specifies the size of the image by the
coordinates
.I "(x0, y0)
of the upper left
corner of the picture and
.I "(x1, y1)
of the pixel one beyond the lower right.
The C interface uses
.I "x0, y0, width, height
to avoid confusion about whether
the corner is in or out of the picture.
.LP
For
.CW TYPE=dump
and
.CW TYPE=runcode ,
the following attribute is mandatory:
.IP \fTNCHAN=\fInbytes\fR
.br
Specifies the number of bytes in each pixel.
.CW TYPE=bitmap
and
.CW TYPE=ccitt-g4
may optionally take
.CW NCHAN=1 ,
and
.CW TYPE=ccir601
may optionally take
.CW NCHAN=3 .
.LP
It is conventional but not mandatory to include:
.IP "\fTCHAN=\fIformat\fR"
.br
Specifies the internal structure
of a pixel.
.I Format
is a string of length equal to the value of
.CW NCHAN .
For example:
.RS
.IP \fTCHAN=rgb\fP\ \ 
24-bit red-green-blue pixel.
.IP \fTCHAN=rgbz...\fP\ \ 
24-bit red, green and blue
entries and a 32-bit depth
value (floating point).
.IP \fTCHAN=m\fP\ \ 
1-byte monochrome or
color mapped values.
.RE
The picfile routines specified here do
not care how the user names channels.
However, there are some conventions:
.RS
.IP \fTrgb\fP
red, green, and blue channels
.IP \fTyiq\fP
NTSC luminance/chrominance channels
.IP \fTyuv\fP
IEEE or PAL luminance/chrominance channels
.IP \fTa\fP
alpha channel (matte)
.IP \fTm\fP
monochrome or mapped value
.IP \fTf...\fP
dots indicate a
multiple-byte channel (e.g. a floating-point number)
.RE
.IP "\fTCOMMAND=\fIcommand line\fR"
.br
Store picfile history information.
Multiple
.CW COMMAND
entries may be present in the header.
The first is the command line used to make
this picture, and following it are the
.CW COMMAND
entries (indented by one space) from
all picfiles opened before this header
was written.
The header in Figure 1 shows
a picture clipped to 400\(mu400, digitally
resampled to 512\(mu512, converted to bitmap
by a halftone screen, and then converted
back to gray-level by Wiener-filter
reconstruction.
.IP "\fTRES=\fIxres yres\fR"
.br
Specifies the horizontal and vertical resolution in pixels per inch.
This attribute is often used for images of documents.
.IP \fTCMAP=\fP
.br
The
.CW CMAP
attribute has no value associated with it.  It flags
the presence of a color-look-up table after the header.
This table is 768 bytes long and consists of 256 values of red, green,
and blue.
The bytes of the
colormap are stored as rgbrgbrgb...
.LP
The colormap should be loaded into the frame buffer's
colormap when the image is displayed.
The mapping from pixel channels to colormap values
in a full-color (e.g.
.CW CHAN=rgb )
picture is usually:
.P1 0
displayed red   = colormap[3*r + 0]
displayed green = colormap[3*g + 1]
displayed blue  = colormap[3*b + 2]
.P2
In a monochrome picture, channel
.CW m
is normally mapped:
.P1 0
displayed red   = colormap[3*m + 0]
displayed green = colormap[3*m + 1]
displayed blue  = colormap[3*m + 2]
.P2
Applications may add other header lines.  It is perfectly
permissible to include
.CW SHOESIZE=10
in the header, which all
standard software will pass uninterpreted.
.PP
Following the ASCII header and the optional colormap is the
binary data for the image.
The routines described below assume nothing about this data
except what is specified by
.CW TYPE ,
.CW WINDOW
and
.CW NCHAN .
.PP
Pixels
.I N
bytes long, where
.I N
is the value of the
.CW NCHAN
attribute.
.I Picread
and
.I picwrite ,
described below, do not interpret pixel values, except for conversion to
a uniform format.
Higher level software must assign format and meaning to
pixels and specify byte ordering
of short or long integers and the format of floating point data.
.NH
The Picfile Library
.PP
Picture files are referred to by a pointer to a
.I PICFILE
data structure,
analogous to the standard I/O library's
.I FILE
structure.  There
are just a few routines for opening, reading and writing
picture files one scan line at a time.  To use them, a program
must include the header file
.CW picfile.h .
On 10th edition
.UX
systems this file is in
.CW /usr/include .
On center 1125's SUNs it may for
administrative reasons be found elsewhere.
.P1 0
PICFILE * picopen_r(char *filename);
.P2
.P1 0
PICFILE * picopen_w(char *filename,
	char *type, int x0, int y0,
	int width, int height, char *
	chan, char *argv[], char *cmap);
.P2
.I Picopen_r
opens a picture file for reading;
.I picopen_w
opens it for writing.
For both routines,
.I filename
is usually a name of a
.UX
file.
Some implementations may treat certain names specially,
although this not part of the standard for now.
The current implementation reserves the names
.CW OUT
and
.CW IN
to mean standard output and input.
.PP
For
.I picopen_w ,
.I type
is the value of the
.CW TYPE
attribute (e.g., 
.CW dump ,
.CW runcode ,
etc.)
.PP
.I X0
and
.I y0
are the coordinates of the upper left corner of the image.
.I Width
and
.I height
are the size of the image.
.I Chan
is a string (e.g.
.CW rgb ,
or
.CW m ,
or
.CW rgbaz... )
giving the
value of the
.CW CHAN
attribute and, by implication,
.CW NCHAN ,
which is always the string length of
.I chan.
.PP
If nonzero,
.I argv
is the second
argument of
.I main .
This passes information to the picture file routines
that allow them to construct the
.CW COMMAND
attributes that maintain history
information.  It is strongly advised that users provide this pointer so
proper history can be kept in picture files.
.PP
If non-zero,
.I cmap
points at a color-look-up table.
.PP
Once a picfile has been opened, I/O can be performed by the routines
.P1 0
int picread(PICFILE *pid, char *buffer);

int picwrite(PICFILE *pid, char *buffer);
.P2
.I Buffer
must be large enough to hold one row of the
image (i.e., picture width times number of bytes per pixel).  These
routines read or write one row of an image,
returning 1 if successful and 0 if error or end-of-file.
.PP
If an image has
.CW TYPE=runcode ,
.I picread
and
.I picwrite
will do the run-length
decoding and encoding.  If
.CW TYPE=bitmap
or
.CW TYPE=ccitt-g4 ,
.I picread
will unpack each row of the image into one byte per pixel (with values of 0 or 255), and
.I picwrite
will pack the pixels back into one bit each by a threshold
test (1 if pixel < 128, otherwise 0).
.CW TYPE=ccir601
files are unpacked and converted into
.CW CHAN=rgb
format,
so that they can be processed by programs designed for conventional
color images.
.I Picread
and
.I picwrite
do these conversions so that programs
need not be greatly concerned with which image
type is being used.
The idea is to make all encodings look like
the simplest,
.CW TYPE=dump .
.PP
A picfile can be closed by
.P1 0
void picclose(PICFILE *pid);
.P2
.I Picclose
closes the file descriptors and frees allocated memory associated with the picture file.
.PP
After calling
.I picopen_w ,
but before the first call to
.I picwrite
(at which time the header is written out),
users may add or change attribute values by calling
.P1 0
PICFILE *picputprop(PICFILE *pid,
	char *name, char *value);
.P2
.I Picputprop
adds the attribute
.I name=value
to the picture file.  The modified picfile descriptor is returned.
.P1 0
char *picgetprop(PICFILE *pid, char *name);
.P2
.I Picgetprop
returns the value of the named attribute of a picfile descriptor,
or a zero (\fINULL\fP) pointer if absent.
.PP
Attributes such as
.CW COMMAND
sometimes appear more than once in a header,
as in the example at the start of section 2.
.I Picgetprop
and
.I picputprop
treat these as a single attribute-value pair, with newline characters
delimiting the sequence of values.
.PP
There are macros defined in
.CW picfile.h
to extract commonly-used
numeric attributes from picfiles.  Each of the following returns an integer value:
.P1 0
PIC_NCHAN(pid)    /* value of NCHAN */
PIC_WIDTH(pid)    /* picture width */
PIC_HEIGHT(pid)   /* picture height */
.P2
Other attributes should be accessed using
.I picgetprop
and not by inspecting the
.I PICFILE
data structure, which may change in the future.
.PP
The call
.P1 0
pid2=picopen_w(name, PIC_SAMEARGS(pid1));
.P2
will create a picture with the same arguments as a previously opened picture.
The
.I PIC_SAMEARGS
macro includes argv in the argument list, so that variable
must be defined at the point of call.
.PP
While not directly related to picture file I/O, the following two routines
can assist some application programs which deal with complex pixel definitions.
.P1 0
void picunpack(PICFILE *pid, char *pixels,
   char *format[, void *chan_ptr] ...);

void picpack(PICFILE *pid, char *pixels,
   char *format[, void *chan_ptr] ...);
.P2
.I Picunpack
takes
an array of pixels (as produced by
.I picread )
and extracts channels into separate arrays of types specified by
.I format .
Similarly,
.I picpack
copies data from various channel arrays and packs
them into a single array of pixels (as expected by
.I picwrite ).
.LP
Characters in
.I format
can be:
.IP \fTc\fP
read or write a one-byte channel.  The corresponding
.I channel_pointer
must be of type
.I char
(or
.I "unsigned char" ).
.IP \fTs\fP
read or write a two-byte channel.  The corresponding
.I channel_pointer
must be of type
.I short .
.IP \fTl\fP
read or write a four-byte channel.  The corresponding
.I channel_pointer
must be of type
.I long .
.IP \fTf\fP
read or write a four-byte channel.  The corresponding
.I channel_pointer
must be of type
.I float .
.IP \fTd\fP
read or write an eight-byte channel.  The corresponding
.I channel_pointer
must be of type
.I double .
.IP \fT_\fP
underscore indicates a byte in the pixel should be skipped.
.PP
For example, in a picture with
.CW CHAN=rgbz... ,
an unpacking of the
green channel and the z channel would be done as follows:
.P1
PICFILE *pid;
char *pixels, *green;
float *zdepth;
picread(pid, pixels);
picunpack(pid, pixels, "_c_f",
	green, zdepth);
.P2
By using
.I picpack
and
.I picunpack ,
the user also benefits from the standard
machine-independent byte ordering that these routines will specify
for
.I short
and
.I long
type date.  Data of type
.I float
should also be standardized to use IEEE floating point format, but that
is not being done now, so as not to inconvenience users of
non-IEEE hardware.
.PP
Random-access routines such as
.I picseek
and
.I pictell
have not been
included in order to encourage the development of
.UX -style
tool programs which can be combined with pipes.  For example,
using currently existing tools, a picture file can be resampled
to 320\(mu256 as follows:
.P1
resample 320 pfile1 OUT |
	transpose IN OUT |
	resample 256 IN OUT |
	transpose IN pfile2
.P2
.NH
Example Program
.PP
Figure 2 shows a complete sample program.
.1C
.KF
.P1 0
		/* reflect a picture about horizontal median */
		#include <picfile.h>
		
		main(int argc, char *argv[])
		{
			PICFILE *in, *out;
			register char *left, *right, channel;
			char *buffer, *malloc();
			int i;
		
			if (argc != 3)
				usage("reflect infile outfile");
			in = picopen_r(argv[1]);
			if (in == 0) {
				perror(argv[1]);
				exit(1);
			}
			out = picopen_w(argv[2], PIC_SAMEARGS(in));
			if (out == 0) {
				perror(argv[2]);
				exit(2);
			}
			buffer = malloc(w*n);
			while (picread(in, buffer)) {
				left = buffer;
				right = buffer + n*(w - 1);
				while (left < right) {
					for (i = 0; i < n; i++) {
						channel = *left;
						*left++ = *right;
						*right++ = channel;
					}
					right -= n + n;
				}
				picwrite(out, buffer);
			}
			exit(0);
		}
.P2
.sp
.ce
Figure 2
.KE
.2C
.NH
Acknowledgements
.PP
We would like to thank John Amanatides, Jon Helfman and Bob Safranek for
there comments and experiences with this picture format.
.PP
The highly complex CCITT FAX format was implemented by Henry Baird.

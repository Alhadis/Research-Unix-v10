.so ../ADM/mac
.XX tbl 35 "Tbl \(em A Program to Format Tables"
.nr zz \n(%#u-1u
....if \nM=0 .RP
....TM 76-1274-5 39199 39199-11
.if \n(tm>0 .ND June 11, 1976*
.TL
Tbl \(em A Program to Format Tables\(dg
.AU "MH 2C-569" 6377
L. L. Cherry
M. E. Lesk
.AI
.MH
.AB
.I Tbl
is a document formatting preprocessor for
.I troff
or
.I nroff
which makes
even
fairly complex tables easy to specify and enter.
Tables are made up of columns which may be independently centered, right-adjusted,
left-adjusted, or aligned by decimal points.
Headings may be placed over single columns or groups of columns.
A table entry may contain equations, or may consist of several rows of text.
Horizontal or vertical lines may be drawn as desired
in the table,
and any table or element may be enclosed in a box.
For example:
.TS
center, box;
c  s  s  s
c  s  s  s
c |c |c |c
c |cp-2 |c |c
l |n |n |n.
U. S. Energy Production/Consumption
\s-2(in quadrillion Btu)\s0
=
Year	Production	Consumption	Net
\^	all sources		\^
_
1953	36.77	36.27	+0.51
1958	38.81	40.35	\-1.54
1963	45.85	48.32	\-2.47
1968	56.81	61.00	\-4.19
1973	62.06	74.28	\-12.22
1978	61.01	78.09	\-17.08
1983	61.19	70.05	\-8.86
1987	64.55	76.01	\-11.46
.TE
.AE
.tr ~ 
.de e1
.nr \\$1 \\$2
..
.de e2
.\".if !\\n(\\$1=\\n% .tm Error in example \\$1: referred to page \\n(\\$1, is on page \\n%
.Tm Example \\$1 \\n(\\$1
.rr \\$1
..
.di qq
.EQ
delim $$
.EN
.di
.rm qq
.2C
.FS
\(dgThis is a version of |reference(v7tbl) revised by Lorinda Cherry.
.FE
.NH
Introduction.
.PP
.I Tbl
turns a simple description
of a table into a
.I troff
or
.I nroff |reference(latest troff reference)
program (list of commands) that prints the table.
It
attempts to isolate a portion of a job that it can
successfully
handle and leave the remainder for other programs.
Thus
.I tbl
may be used with the equation formatting
program
.I eqn |reference(latest eqn)
or various
layout macro packages |reference(latest msmacros)|reference(lesk kernighan typesetting)|reference(mashey smith documentation),
but does not duplicate their functions.
.PP
This memorandum is divided into two parts.
First we give the rules for preparing
.I tbl
input; then some examples are shown.
The description of rules is precise but technical, and
the beginning user may prefer to read the examples first,
as they show some common table arrangements.
A section explaining how to invoke
.I tbl
precedes the examples.
To avoid repetition, henceforth read
.I troff
as
.I
``troff
.R
or
.I
nroff.''
.R
.ds . \^\s14.\s0
.PP
The input to
.I
tbl
.R
is text for a document, with tables preceded by a ``\f(CW.TS\fP''
(table start)
command and followed by a ``\f(CW.TE\fP''
(table end) command.
.I Tbl
processes the tables, generating
.I troff
formatting commands,
and leaves the remainder of the text unchanged.
The
.CW .TS
and
.CW .TE
lines are copied, too, so that
.I troff
page layout macros
(such as the memo formatting macros|reference(latest msmacros)) can use these lines
to delimit and place tables as they see fit.
In particular, any arguments on the
.CW .TS
or
.CW .TE
lines
are copied but otherwise ignored, and may be used by document layout
macro commands.
.PP
The format of the input is as follows:
.DS
\&text
.CW .TS
\&\fItable\fR
.CW .TE
\&text
.CW .TS
\&\fItable\fR
.CW .TE
\&text
\&\&\*. \*. \*.
.DE
where the format of each table is as follows:
.DS
.CW .TS
\fIoptions \f(CW;\fR
\fIformat \f(CW.\fP
data
.CW .TE
.DE
.R
Each table is independent, and must contain formatting
information
followed by the data to be entered in the table.
The formatting information, which describes the
individual columns and rows of the table, may be preceded
by a few options that affect the entire table.
A detailed description of tables is given in the next section.
.NH
Input commands.
.PP
As indicated above, a table contains, first, global options,
then a format section describing the layout of the table
entries, and then the data to be printed.  The format and data
are always required, but not the options.
The various parts of the table are entered as follows:
.NH 2
Options
.PP
There may be a single line of options
affecting the whole table.
If present, this line must follow the
.CW .TS
line immediately
and must contain a list of option names
separated by spaces, tabs, or commas, and must be terminated by a semicolon.
The allowable options are:
.IP \f(CW~~center\fR 15
center the table (default is left-adjust);
.IP \f(CW~~expand\fR
make the table as wide as the current line length;
.IP \f(CW~~box\fR
enclose the table in a box;
.IP \f(CW~~allbox\fR
enclose each item in the table in a box;
.IP \f(CW~~doublebox\fR
enclose the table in two boxes;
.Tm tab	S
.IP \f(CW~~tab~\fR(\fIx\fR\^)
use \fIx\fR instead of tab to separate data items.
.IP \f(CW~~linesize(\fIn\fP)\fR
set lines or rules (e.g. from \f(CWbox\fR\^) in \fIn\fR point type;
.Tm delim	S
.IP \f(CW~~delim(\fIxy\fP)\fR
recognize \fIx\fR and \fIy\fR as the \fIeqn\fR delimiters.
.PP
.I Tbl
tries to keep boxed tables on one page by issuing
appropriate
.I troff
\&``need''
.CW .ne ) (
commands.
These requests are calculated from the number of lines in the tables,
and if there are spacing commands embedded in the input, these requests may be inaccurate;
use normal
.I troff
procedures, such as keep-release macros, in that case.
The user who must have a multi-page boxed table
should use macros designed for this purpose,
as explained below under `Usage.'
.NH 2
Format
.PP
The format section of the table specifies the layout
of the columns.
Each line in this section corresponds to one line
of the table (except that the last line corresponds to all following
lines up to the next
.CW .T& ,
if any \(em see below),
and each line contains a key-letter for each column
of the table.
It is good practice to separate the key letters for each
column by spaces or tabs.
Each key-letter is one of the following:
.cs B 25
.IP "~~\f(CWL\fR~or~\f(CWl\fR" 10
to indicate a left-adjusted column entry;
.IP "~~\f(CWR\fR~or~\f(CWr\fR
to indicate a right-adjusted column entry;
.IP "~~\f(CWC\fR~or~\f(CWc\fR
to indicate a centered column entry;
.IP "~~\f(CWN\fR~or~\f(CWn\fR
to indicate a numerical column entry, to be aligned with other
numerical entries so that the units digits of numbers line up;
.IP "~~\f(CWA\fR~or~\f(CWa\fR
to indicate an alphabetic subcolumn;
all corresponding entries are aligned on the left, and positioned
so that the widest is centered within the column (see
example on page 12);
.IP "~~\f(CWS\fR~or~\f(CWs\fR
to indicate a spanned heading, i.e. to indicate that the
entry from the previous column continues across this
column
(not allowed for the first column, obviously); or
.IP ~~\f(CW\s+4\v'6p'^\v'-6p'\s0\fR
to indicate a vertically spanned heading, i.e. to indicate that
the entry from the previous row continues down through this row.
(Not allowed for the first row of the table, obviously).
.cs B
.PP
When numerical alignment is specified, a location for the decimal
point is sought.  The rightmost dot
.CW . ) (
adjacent to a digit is used as a decimal point; if there is no
dot adjoining a digit, the rightmost digit is used as a units digit;
if no alignment is indicated, the item is centered in the column.
However, the special non-printing character string
.CW \e&
may be used
to override unconditionally dots
and digits, or to align alphabetic data;
this string lines up where a dot normally would,
and then disappears from the final output.
In the example below, the items shown at the left will be
aligned (in a numerical column) as shown on the right:
.KS
.TS
center;
l6 n.
13	13
4\*.2	4\&\*.2
26\*.4\*.12	26\*.4\&\*.12
abc	abc
abc\e&	abc\&
43\e&3\*.22	43\&3\*.22
749\*.12	749\&\*.12
.TE
.KE
.PP
\fBNote:\fR
If numerical data are used in the same column with wider
.CW L
or
.CW r
type table entries, the widest \fInumber\fR is centered relative
to the wider
.CW L
or
.CW r
items (\f(CWL\fR is used instead of \f(CWl\fR for readability;
they have the same meaning as key-letters).
Alignment within the numerical items is preserved.
This is similar to the behavior of
.CW a
type data, as explained above.
However,
alphabetic subcolumns (requested by the
.CW a
key-letter)
are always slightly indented relative to
.CW L
items;
if necessary, the column width is increased to force this.
This is not true for \f(CWn\fR type entries.
.PP
.ft BI
Warning:
.ft 1
the \f(CWn\fR and \f(CWa\fR items should not be used in
the same column.
.PP
For readability, the key-letters describing each column should
be separated by spaces.
The end of the format section is indicated by a period.
The layout of the key-letters in the format section resembles
the layout of the actual data in the table.
Thus a simple format might appear as:
.P1
c  s  s
l  n  n .
.P2
which specifies a table of three columns. The first line
of the table contains a heading centered across all three
columns; each remaining line contains a left-adjusted item
in the first column followed by two columns of numerical
data.
A sample table in this format might be:
.br
.ne 6v
.br
.in +4
.TS
c s s
l n n.
Overall title
Item-a	34.22	9.1
Item-b	12.65	.02
Items: c,d,e	23	5.8
Total	69.87	14.92
.TE
.in -4
There are some additional features of the key-letter system:
.IP "\fI~~Horizontal lines~\fR"
\(em A key-letter may be replaced by
.CW _
(underscore) to indicate
a horizontal line in place of the corresponding column entry,
or by
.CW =
to indicate a double horizontal line.
If an adjacent column contains a horizontal line, or if
there are vertical lines adjoining this column,
this horizontal line is extended to meet the nearby lines.
If any data entry is provided for this column, it is ignored and
a warning message is printed.
.QQ do the D(x) and M(x) to draw a line of 'x' characters.
.QQ D will draw divided lines, M merged lines. thus - is simplh M(\(ru)
.IP "\fI~~Vertical lines~\fR"
\(em A vertical bar may be placed between column key-letters.  This will
cause a vertical line between the corresponding columns of the table.
A vertical bar to the left of the first key-letter
or to the right of the last one produces a line at the
edge of the table.
If two vertical bars appear between key-letters, a double vertical
line is drawn.
.IP "\fI~~Space between columns~\fR"
\(em A number may follow the key-letter.  This indicates the amount of separation
between this column and the next column.
The number normally specifies the separation in
.I ens *.
.FS
* One en is about the width of the letter `n', or more precisely,
half the current type size measured in points (1 point = 1/72 inch).
.FE
If the ``expand'' option is used, then these numbers are multiplied
by a constant such that the table is as wide as the current line length.
The default column separation number is 3.
If the separation is changed the worst case (largest space requested) governs.
.IP "\fI~~Vertical spanning~\fR"
\(em Normally, vertically spanned items extending over several
rows of the table are centered in their vertical range.
If a key-letter is followed by
.CW t
or
.CW T ,
any corresponding vertically spanned item
will begin at the top line of its range.
.IP "\fI~~Font changes~\fR"
\(em A key-letter may be followed by a string containing a font
name or number
preceded by the letter
.CW f
or
.CW F .
This indicates that the corresponding column should be in a different
font from the default font (usually Roman).
All font names are one or two letters; a one-letter font
name should be separated from whatever follows by a space or tab.
The single letters
.CW B ,
.CW b ,
.CW I ,
and
.CW i
are shorter synonyms for
.CW fB
and
.CW fI
and refer to the bold and italic fonts in the current font family.
Font change commands given with the table entries
override these specifications.
.IP "\fI~~Point size changes~\fR"
\(em A key-letter may be followed by the letter
.CW p
or
.CW P
and a number to indicate the point size of the corresponding table entries.
The number may be a signed digit, in which case it is taken as
an increment or decrement
from the current point size.
If both a point size and a column separation value are given,
one or more blanks must separate them.
.IP "\fI~~Vertical spacing changes~\fR"
\(em A key-letter may be followed by the letter
.CW v
or
.CW V
and a number to indicate the vertical line spacing to be used
within a multi-line corresponding table entry.
The number may be a signed digit, in which case it is taken as an increment
or decrement from the current vertical spacing.
A column separation value must be separated by blanks or some other
specification from a vertical spacing request.
This request has no effect unless the corresponding table entry
is a text block (see below).
.IP "\fI~~Column width indication~\fR"
\(em A key-letter may be followed by the letter
.CW w
or
.CW W
and a width value in parentheses.
This width is used as a minimum column width.
If the largest element in the column is not as wide as the width value
given after the \f(CWw\fR, the largest element is assumed to be that wide.
If the largest element in the column is wider than the specified value,
its width is used.
The width is also used as a default line
length for included text blocks.
Normal
.I troff
units can be used to scale the width value; if none are used,
the default is
ens.
If the width specification is a unitless integer
the parentheses may be omitted.
If the width value is changed in a column, the \fIlast\fR one given controls.
.IP "\fI~~Equal width columns~\fR"
\(em A key-letter may be followed by the letter
.CW e
or
.CW E
to indicate equal width columns.
All columns whose
key-letters are followed by \f(CWe\fR or \f(CWE\fR are made the same width.
This permits the user to get a group of regularly spaced
columns.
.IP "\fI~~Staggered columns~\fR"
\(em A key-letter may be followed by the letter
.CW u
or
.CW U
to indicate that the corresponding entry is to be moved up one half line.
This makes it easy, for example, to have a column of differences
between numbers in an adjoining column.
The
.CW allbox
option does not work with staggered columns.
.IP "\fI~~Zero-width item~\fR"
\(em A key-letter may be followed by the letter
.CW z
or
.CW Z
to indicate that the corresponding
data item is to be ignored in calculating column widths.
This may be useful, for example, in allowing headings
to run across adjacent columns where spanned headings
would be inappropriate.
.IP "~~\fBNote:\fR~"
The order of the above features is immaterial; they need not be separated
by spaces, except as indicated above to avoid ambiguities involving
point size and font changes.  Thus
a numerical column entry in italic font and 12 point type with a minimum
width of 2.5 inches and separated by 6 ens from the next column could
be specified as
.P1
np12w(2.5i)fI  6
.P2
.IP "\fI~~Alternative notation~\fR"
\(em Instead of listing the format of successive lines of a table
on consecutive lines of the format section,
successive line formats may be given on the same line, separated
by commas, so that the format for the example
above might have been written:
.P1
c s s, l n n .
.P2
.IP "\fI~~Default~\fR"
\(em Column descriptors missing from the end
of a format line are assumed to be \f(CWL\fR.
The longest line in the format section, however,
defines the number of columns
in the table;
extra columns in the data are ignored silently.
.QQ put in the diagnostics.
.NH 2
Data
.PP
The data for the table are typed after the format.
Normally, each table line is typed as one line of data.
Very long input lines can be broken: any line whose last character is
.CW \e
is combined with the following line (and the
.CW \e
vanishes).
The data for different columns (the table entries) are separated by tabs,
or by whatever character has been specified with the option
.CW tabs .
There are a few special cases:
.IP "\fI~~Troff commands within tables\fR~"
\(em An input line
beginning with a
.CW .
followed by anything
but a number is assumed to be a command to
.I troff
and is passed through unchanged, retaining its position in the table.
So, for example, vertical space within a table may be produced by
.CW .sp
commands
in the data.
......
.IP "\fI~~Full width horizontal lines\fR~"
\(em An input line
containing only
.CW _
(underscore)
or
.CW =
(equal sign) is taken to be a single or double
line, respectively, extending the
full width
of the table.
.IP "\fI~~Single column horizontal lines\fR~"
\(em An input table entry
containing only the character $fat "\(ul" $ or \f(CW=\fR
is taken to be a single or double line extending
the full width of the column.
Such lines are extended to meet horizontal or vertical
lines adjoining this column.
To obtain these characters explicitly in a column, either
precede them by
.CW \e&
or
follow them by a space before the usual tab or newline.
.IP "\fI~~Short horizontal lines\fR~"
\(em An input table entry
containing only the string
.CW \e_
is taken to be a single line as wide as the contents of
the column.  It is not extended to meet
adjoining lines.
.IP "\fI~~Repeated characters\fR~"
\(em An input table entry
containing only a string of the form
.CW \eR \fIx\fR
where
.I x
is any character is replaced by repetitions of the character
.I x
as wide as the data in the column.
The sequence of
.I x 's
is not extended to meet adjoining columns.
.IP "\fI~~Vertically spanned items\fR~"
\(em An input table entry containing only the
character string
.CW \e^
indicates that the table entry immediately
above spans downward over this row.  It is equivalent
to a table format key-letter of
.CW ^ .
.IP "\fI~~Text blocks\fR~"
\(em In order to include a block of text as a table entry,
precede it by
.CW T{
and follow
it by
.CW T} .
Thus the sequence
.P1
\&\fR. . .\fP T{
.I
block of
text
.CW
T}\fR . . .
.P2
is the way to enter, as a single entry in the table, something
that cannot conveniently be typed as a simple string between
tabs.
Note that the
.CW T}
end delimiter must begin a line;
additional columns of data may follow after a tab on the same line.
.nr zy \n(zz+10
See the example on page \n(zy for an illustration of included text blocks
.e1 aa \n(zy
in a table.
If more than fifty or so text blocks are used in a table,
various limits in the
.I troff
program are likely to be exceeded,
producing diagnostics such as `too many string/macro names' or `too many
number registers.'
.IP
Text blocks are pulled out from the table, processed separately by
.I troff ,
and replaced in the table as a solid block.  If no line length
is specified in the block of text
tself, or in the table format,
the default is to use
$ L times C / (N+1) $
where $L$ is the current line length,
$C$ is the number of table columns spanned by the text,
and $N$ is the total number of columns in the table.
The other parameters (point size, font, etc.) used in setting the
block of text are those in effect at the beginning of the table (including
the effect of the
.CW .TS
macro)
and any table format specifications of size, spacing and font,
using the \f(CWp\fR, \f(CWv\fR and \f(CWf\fR modifiers to the column key-letters.
Commands within the text block itself are also recognized, of course.
However,
.I troff
commands within the table data but not within the text block
do not affect that block.
.br
.di RR
.     this is going down a rathole
.EQ
delim off
.EN
.di
.rm RR
.IP "~~\fBWarnings:\fR~"
Although any number of lines may be present in a table,
only the first 200 lines are used in calculating
the widths of the various columns.
A multi-page table,
of course, may be arranged as several single-page tables
if this proves to be a problem.
Other difficulties with formatting may arise because,
in the calculation of column widths all table entries
are assumed to be in the font and size being used when
the
.CW .TS
command was encountered, except for font and size changes
indicated (a) in the table format section and (b)
within the table data (as in the entry
.CW \es+3\efIdata\efP\es0 ).
Therefore, although arbitrary
.I troff
requests may be sprinkled in a table, care must be taken
to avoid confusing the width calculations;
use requests such as
.CW .ps
with care.
.tr ~~
.sp .5v
.NH 2
Additional Command Lines
.PP
If the format of a table must be changed after
many similar lines, as with sub-headings or summarizations, the
.CW .T&
(table continue)
command can be used
to change column parameters.
The outline of such a table input is:
.DS
.ft CW
\&.TS
.ft I
\&options \f(CW;\fP
\&format \*.
\&data
\&\*. \*. \*.
.ft CW
\&.T&
.ft I
\&format \*.
\&data
.ft CW
\&.T&
.ft I
\&format \*.
\&data
.ft CW
\&.TE
.DE
.R
.nr zy \n(zz+9
.nr zx \n(zz+12
as in the examples on pages \n(zy and \n(zx.
.e1 ab \n(zy
.e1 ac \n(zx
Using this procedure, each table line can be close to its corresponding format line.
.sp 3p
.ft BI
Warning:
.ft R
It is not possible to change the number of columns, the space
between columns, the global options such as \fIbox,\fR
or the selection of columns to be made equal width.
Nor is
.CW .T&
recognized after the first 200 lines of a table.
.NH
Usage.
.PP
On
.UX
systems,
.I tbl
can be run on a simple table with the command
.P1
tbl input-file | troff
.P2
but
for more complicated use, where there are several input files,
and they contain equations and
.CW -ms
layout commands as well
as tables, the normal command would be
.P1
tbl \fIfiles ...\fP | eqn | troff -ms
.P2
and, of course, the usual options may be used on the
.I troff
and
.I eqn
commands. If any of the file names are
.CW -
the standard input is read at that point.
.PP
For the convenience of users employing line printers without
adequate driving tables or post-filters, there is a special
.CW -TX
command line option to
.I tbl
which produces output that does not have fractional line
motions in it.
.PP
Note that when
.I eqn
and
.I tbl
are used together on the same file,
.I tbl
should be used first.
If there are no equations within tables,
either order works, but it is usually faster
to run
.I tbl
first, since
.I eqn
normally produces a larger expansion of the input
than
.I tbl .
However, if there are equations within tables
(using the
.I delim
mechanism in
.I eqn ),
.I tbl
must be first or the output will be scrambled.
Users must also beware of using equations in
\f(CWn\fR-style columns; this is nearly always wrong,
since
.I tbl
attempts to split numerical format items into two parts and this
is not possible with equations.
The user can defend against this by giving the
.I delim(xx)
table option;
this prevents splitting of numerical columns within the delimiters.
For example, if the
.I eqn
delimiters
are
.I $$ ,
giving
.I delim($$)
a numerical column such as
.CW "1245$+- 16$"
will be divided after 1245, not after 16.
.PP
.I Tbl
accepts up to about 35 columns, the exact number
depending on the availability
of number registers.
The user must avoid number register names used
by
.I tbl ,
which include two-digit numbers from 31 to 97,
and strings of the form
.CW 4\fIx\fR,
.CW 5\fIx\fR,
.CW #\fIx\fR,
\fIx\f(CW+\fR, \fIx\f(CW|\fR, \f(CW^\fIx\fR, and \fIx\f(CW-\fR,
where
\fIx\fR is any lower case letter.
The names
.CW ## ,
.CW #- ,
and
.CW #^
are also used in certain circumstances.
To conserve number register names, the
.CW n
and
.CW a
formats share a register;
hence the restriction above that they may not be used in the same column.
.PP
For aid in writing layout macros,
.I tbl
defines a number register
.CW TW
which is
the table width; it is defined by the time that the
.CW .TE
macro
is invoked and may be used in the expansion of that macro.
More importantly, to assist in laying out multi-page boxed tables
the macro T# is defined to produce the bottom lines and side lines of a boxed
table, and then invoked at its end.  By use of this macro
in the page footer a multi-page table can be boxed.
In particular, the
.CW -ms
macros can be used to print a multi-page boxed table with a repeated heading
by giving the
argument
.CW H
to the
.CW .TS
macro.
If the table start macro is written
.P1
	\&\*.TS H
.P2
a line of the form
.P1
	\&\*.TH
.P2
must be given in the table after any table heading (or at the start if none).
Material up to the
.CW .TH
is placed at the top of each page of table;
the remaining lines in the table are placed on several pages as required.
Note that this is
.I not
a feature of
.I tbl ,
but of the
.CW -ms
layout macros.
.FC
.1C
.NH
Examples.
.PP
Here are some examples illustrating features of
.I
tbl.
.R
.ds T \|\h'.4n'\v'-.2n'\s6\zT\s0\s10\v'.2n'\h'-.4n'\(ci\|\s0
The symbol \*T in the input represents a tab character.
.de IN
.po \\n(POu
.sp |\\n(.hu
.sp
.NE \\$1
.mk
.B
Input:
.R
.sp .5
.nf
.in +3n
..
.de OU
.br
.in -3n
.rt
.po +3i
.B
Output:
.R
.sp .5
..
.rm TS
.rm TE
.nf
.KS
.IN 2.5i
\&\*.TS
\&box;
\&c c c
\&l l r\*.
\&Fact\*TLocation\*TStatistic
\&
\&Largest state\*TAlaska\*T591,004 sq. mi.
\&Smallest state\*TRhode Island\*T1,212 sq. mi.
\&Longest river\*TMississippi-Missouri\*T3,710 mi.
\&Highest mountain\*TMount McKinley, AK\*T20,320 ft.
\&Lowest point\*TDeath Valley, CA\*T\-282 ft.
\&\*.TE
.OU
.TS
box;
c c c
l l r.
Fact	Location	Statistic

Largest state	Alaska	591,004 sq. mi.
Smallest state	Rhode Island	1,212 sq. mi.
Longest river	Mississippi-Missouri	3,710 mi.
Highest mountain	Mount McKinley, AK	20,320 ft.
Lowest point	Death Valley, CA	\-282 ft.
.TE
.IN 2.8i
\&\*.TS
\&allbox;
\&c s s
\&c c c
\&n n n\*.
\&AT&T Common Stock
\&Year\*TPrice\*TDividend
\&1984\*T15-20\*T$1\*.20
\&5\*T19-25\*T1\*.20
\&6\*T21-28\*T1\*.20
\&7\*T20-36\*T1\*.20
\&8\*T24-30\*T1\*.20
\&9\*T29-37\*T\*.30*
\&\*.TE
\&* (first quarter only)
.OU
.TS
allbox;
c s s
c c c
n n n.
AT&T Common Stock
Year	Price	Dividend
1984	15-20	$1.20
5	19-25	1.20
6	21-28	1.20
7	20-36	1.20
8	24-30	1.20
9	29-37	.30*
.TE
* (first quarter only)
.KE
.BP
.KS
.IN 4i
\&\*.TS
\&box;
\&c s s
\&c | c | c
\&l | l | n\*.
\&Major New York Bridges
\&=
\&Bridge\*TDesigner\*TLength
\&\(ul
\&Brooklyn\*TJ\*. A\*. Roebling\*T1595
\&Manhattan\*TG\*. Lindenthal\*T1470
\&Williamsburg\*TL\*. L\*. Buck\*T1600
\&\(ul
\&Queensborough\*TPalmer &\*T1182
\&\*T  Hornbostel
\&\(ul
\&\*T\*T1380
\&Triborough\*TO\*. H\*. Ammann\*T\(ul
\&\*T\*T383
\&\(ul
\&Bronx Whitestone\*TO\*. H\*. Ammann\*T2300
\&Throgs Neck\*TO\*. H\*. Ammann\*T1800
\&\(ul
\&George Washington\*TO\*. H\*. Ammann\*T3500
\&\*.TE
.OU
.TS
box;
c s s
c | c | c
l | l | n.
Major New York Bridges
=
Bridge	Designer	Length
_
Brooklyn	J. A. Roebling	1595
Manhattan	G. Lindenthal	1470
Williamsburg	L. L. Buck	1600
_
Queensborough	Palmer &	1182
	  Hornbostel
_
		1380
Triborough	O. H. Ammann	_
		383
_
Bronx Whitestone	O. H. Ammann	2300
Throgs Neck	O. H. Ammann	1800
_
George Washington	O. H. Ammann	3500
.TE
.IN 3.0i
\&\*.TS
\&c c
\&np-2 | n | \*.
\&\*TStack
\&\*T\(ul
\&1\*T46
\&\*T\(ul
\&2\*T23
\&\*T\(ul
\&3\*T15
\&\*T\(ul
\&4\*T6\*.5
\&\*T\(ul
\&5\*T2\*.1
\&\*T\(ul
\&\*.TE
.OU
.TS
c c
np-2 | n |.
	Stack
	_
1	46
	_
2	23
	_
3	15
	_
4	6.5
	_
5	2.1
	_
.TE
.KE
.BP
.KS
.IN 2.5i
\&\*.TS
\&box;
\&L L L
\&L L \(ul
\&L L | LB
\&L L \(ul
\&L L L\*.
\&january\*Tfebruary\*Tmarch
\&april\*Tmay
\&june\*Tjuly\*TMonths
\&august\*Tseptember
\&october\*Tnovember\*Tdecember
\&\*.TE
.OU
.TS
box;
L L L
L L _
L L | LB
L L _
L L L.
january	february	march
april	may
june	july	Months
august	september
october	november	december
.TE
.IN 5.0i
.e2 ab
\&\*.FP helvetica
\&\*.TS
\&box;
\&cf(CW s s s\*.
\&Composition of Foods
\&\(ul
\&\*.T&
\&c  | c s s
\&c  | c s s
\&c   | c  | c  | c\*.
\&Food\*TPercent by Weight
\&\e^\*T\(ul
\&\e^\*TProtein\*TFat\*TCarbo-
\&\e^\*T\e^\*T\e^\*Thydrate
\&\(ul
\&\*.T&
\&l  | n  | n  | n\*.
\&Apples\*T\*.4\*T\*.5\*T13\*.0
\&Halibut\*T18\*.4\*T5\*.2\*T\*. \*. \*.
\&Lima beans\*T7\*.5\*T\*.8\*T22\*.0
\&Milk\*T3\*.3\*T4\*.0\*T5\*.0
\&Mushrooms\*T3\*.5\*T\*.4\*T6\*.0
\&Rye bread\*T9\*.0\*T\*.6\*T52\*.7
\&\*.TE
\&\*.FP times
.OU
.FP helvetica
.TS
box;
cBCW s s s.
Composition of Foods
_
.T&
c  |c s s
c  |c s s
c   |c  |c  |c.
Food	Percent by Weight
\^	_
\^	Protein	Fat	Carbo-
\^	\^	\^	hydrate
_
.T&
l  |n  |n  |n.
Apples	.4	.5	13.0
Halibut	18.4	5.2	...
Lima beans	7.5	.8	22.0
Milk	3.3	4.0	5.0
Mushrooms	3.5	.4	6.0
Rye bread	9.0	.6	52.7
.TE
.FP times
.KE
.BP
.KS
.IN 3.7i
.e2 aa
\&\*.TS
\&allbox;
\&cfI  s   s
\&c    cw(1i)   cw(1i)
\&lp9 lp9 lp9\*.
\&New York Area Rocks
\&Era\*TFormation\*TAge (years)
\&Precambrian\*TReading Prong\*T>1 billion
\&Paleozoic\*TManhattan Prong\*T400 million
\&Mesozoic\*TT{
\&\*.na
\&Newark Basin, incl\*.
\&Stockton, Lockatong, and Brunswick
\&formations; also Watchungs
\&and Palisades\*.
\&T}\*T200 million
\&Cenozoic\*TCoastal Plain\*TT{
\&On Long Island 30,000 years;
\&Cretaceous sediments redeposited
\&by recent glaciation\*.
\&\*.ad
\&T}
\&\*.TE
.OU
.fi
.TS
allbox;
cfI s   s
c   cw(1i)   cw(1i)
lp9 lp9 lp9.
New York Area Rocks
Era	Formation	Age (years)
Precambrian	Reading Prong	>1 billion
Paleozoic	Manhattan Prong	400 million
Mesozoic	T{
.na
Newark Basin, incl.
Stockton, Lockatong, and Brunswick
formations; also Watchungs
and Palisades.
T}	200 million
Cenozoic	Coastal Plain	T{
On Long Island 30,000 years;
Cretaceous sediments redeposited
by recent glaciation.
.ad
T}
.TE
.IN 2i
\&\*.EQ
\&delim $$
\&\*.EN
.sp
\&\*. \*. \*.
.sp
\&\*.TS
\&doublebox;
\&c c
\&l l\*.
\&Name\*TDefinition
\&\*.sp
\&\*.vs +2p
\&Gamma\*T$GAMMA (z) = int sub 0 sup inf  t sup {z-1} e sup -t dt$
\&Sine\*T$sin (x) = 1 over 2i ( e sup ix - e sup -ix )$
\&Error\*T$ roman erf (z) = 2 over sqrt pi int sub 0 sup z e sup {-t sup 2} dt$
\&Bessel\*T$ J sub 0 (z) = 1 over pi int sub 0 sup pi cos ( z sin theta ) d theta $
\&Zeta\*T$ zeta (s) = sum from k=1 to inf k sup -s ~~( Re~s > 1)$
\&\*.vs -2p
\&\*.TE
.di qq
.EQ
delim $$
.EN
.di
.rm qq
.rs
.OU
.TS
doublebox;
c c
l l.
Name	Definition
.sp
.vs +2p
Gamma	$GAMMA (z) = int sub 0 sup inf  t sup {z-1} e sup -t dt$
Sine	$sin (x) = 1 over 2i ( e sup ix - e sup -ix )$
Error	$ roman erf (z) = 2 over sqrt pi int sub 0 sup z e sup {-t sup 2} dt$
Bessel	$ J sub 0 (z) = 1 over pi int sub 0 sup pi cos ( z sin theta ) d theta $
Zeta	$ zeta (s) = sum from k=1 to inf k sup -s ~~( Re~s > 1)$
.vs -2p
.TE
.KE
.KS
.ds : \|:\|
.IN 2i
\&\*.TS
\&box, tab(\*:);
\&cb s s s s
\&cp-2 s s s s
\&c |\|| c | c | c | c
\&c |\|| c | c | c | c
\&r2 |\|| n2 | n2 | n2 | n\*.
\&Readability of Text
\&Line Width and Leading for 10-Point Type
\&=
\&Line\*:Set\*:1-Point\*:2-Point\*:4-Point
\&Width\*:Solid\*:Leading\*:Leading\*:Leading
\&_
\&9 Pica\*:\e-9\*.3\*:\e-6\*.0\*:\e-5\*.3\*:\e-7\*.1
\&14 Pica\*:\e-4\*.5\*:\e-0\*.6\*:\e-0\*.3\*:\e-1\*.7
\&19 Pica\*:\e-5\*.0\*:\e-5\*.1\*: 0\*.0\*:\e-2\*.0
\&31 Pica\*:\e-3\*.7\*:\e-3\*.8\*:\e-2\*.4\*:\e-3\*.6
\&43 Pica\*:\e-9\*.1\*:\e-9\*.0\*:\e-5\*.9\*:\e-8\*.8
\&\*.TE
.OU
.TS
box, tab(:);
cb s s s s
cp-2 s s s s
c ||c |c |c |c
c ||c |c |c |c
r2 ||n2 |n2 |n2 |n.
Readability of Text
Line Width and Leading for 10-Point Type
=
Line:Set:1-Point:2-Point:4-Point
Width:Solid:Leading:Leading:Leading
_
9 Pica:\-9.3:\-6.0:\-5.3:\-7.1
14 Pica:\-4.5:\-0.6:\-0.3:\-1.7
19 Pica:\-5.0:\-5.1: 0.0:\-2.0
31 Pica:\-3.7:\-3.8:\-2.4:\-3.6
43 Pica:\-9.1:\-9.0:\-5.9:\-8.8
.TE
.KE
.KS
.IN 7i
.e2 ac
\&\*.TS
\&c s
\&cip-2 s
\&l n
\&a n\*.
\&Some London Transport Statistics
\&(Year 1964)
\&Railway route miles\*T244
\&Tube\*T66
\&Sub-surface\*T22
\&Surface\*T156
\&\*.sp \*.5
\&\*.T&
\&l r
\&a r\*.
\&Passenger traffic \e- railway
\&Journeys\*T674 million
\&Average length\*T4\*.55 miles
\&Passenger miles\*T3,066 million
\&\*.T&
\&l r
\&a r\*.
\&Passenger traffic \e- road
\&Journeys\*T2,252 million
\&Average length\*T2\*.26 miles
\&Passenger miles\*T5,094 million
\&\*.T&
\&l n
\&a n\*.
\&\*.sp \*.5
\&Vehicles\*T12,521
\&Railway motor cars\*T2,905
\&Railway trailer cars\*T1,269
\&Total railway\*T4,174
\&Omnibuses\*T8,347
\&\*.T&
\&l n
\&a n\*.
\&\*.sp \*.5
\&Staff\*T73,739
\&Administrative, etc\*.\*T5,582
\&Civil engineering\*T5,134
\&Electrical eng\*.\*T1,714
\&Mech\*. eng\*. \e- railway\*T4,310
\&Mech\*. eng\*. \e- road\*T9,152
\&Railway operations\*T8,930
\&Road operations\*T35,946
\&Other\*T2,971
\&\*.TE
.OU
.TS
c s
cip-2 s
l n
a n.
Some London Transport Statistics
(Year 1964)
Railway route miles	244
Tube	66
Sub-surface	22
Surface	156
.sp .5
.T&
l r
a r.
Passenger traffic \(mi railway
Journeys	674 million
Average length	4.55 miles
Passenger miles	3,066 million
.T&
l r
a r.
Passenger traffic \(mi road
Journeys	2,252 million
Average length	2.26 miles
Passenger miles	5,094 million
.T&
l n
a n.
.sp .5
Vehicles	12,521
Railway motor cars	2,905
Railway trailer cars	1,269
Total railway	4,174
Omnibuses	8,347
.T&
l n
a n.
.sp .5
Staff	73,739
Administrative, etc.	5,582
Civil engineering	5,134
Electrical eng.	1,714
Mech. eng. \(mi railway	4,310
Mech. eng. \(mi road	9,152
Railway operations	8,930
Road operations	35,946
Other	2,971
.TE
.KE
.KS
.po \n(POu
.sp |\n(.hu
.de IN
.sp
.ne 1i
.B
Input:
.R
.sp .5
.in +3n
.nf
..
.de OU
.sp
.in -3n
.ne 1i
.B
Output:
.R
.sp .5
..
.ns
.EQ
delim off
.EN
.IN
.ps 8
.vs 10
\&\*.TS
center box;
\&cBp+2 s s
\&cB s s
\&li l lb\*.
\&Crisis and Emergency Numbers
\&\*.sp
\&24-Hour Hotlines
\&Abuse\*TChild Abuse Reports\*T1-800-792-8610
\&\*.sp \*.5
\&Alcohol\*TAla-Call\*T1-800-322-5525
\&Alcohol\*TOverdose\*T1-800-962-1253
\&\*.sp \*.5
\&Drug\*TDrug Hotline\*T1-800-225-0196
\&Drug\*TOverdose\*T1-800-962-1253
\&Drug\*TNarcotics Anonymous\*T1-800-992-0401
\&\*.sp \*.5
\&Poison\*TPoison Hotline\*T1-800-962-1253
\&\*.sp \*.5
\&Runaways\*THotline\*T1-800-231-6946
\&Runaways\*TCovenant House\*T1-800-999-9999
\&\*.sp \*.5
\&\*.T&
\&cB s s
\&li l lb\*.
\&Other Hotlines
\&AIDS\*TNational AIDS Hotline\*T1-800-342-AIDS
\&AIDS\*TNJ AIDS Hotline\*T1-800-624-2377
\&Drug\*TCocaine\*T1-800-COCAINE
\&Eating\*TEating Disorders Hotline\*T1-800-624-2268
\&Support\*TSupport Group Information\*T1-800-367-6274
\&VD\*TVD Information\*T1-800-227-8922
\&\*.TE
.ne 3.2i
.ps 10
.vs 12
.OU
.ps 8
.vs 10p
.TS
center box;
cBp+2 s s
cB s s
li l lb.
Crisis and Emergency Numbers
.sp .5
24-Hour Hotlines
Abuse	Child Abuse Reports	1-800-792-8610
.sp .5
Alcohol	Ala-Call	1-800-322-5525
Alcohol	Overdose	1-800-962-1253
.sp .5
Drug	Drug Hotline	1-800-225-0196
Drug	Overdose	1-800-962-1253
Drug	Narcotics Anonymous	1-800-992-0401
.sp .5
Poison	Poison Hotline	1-800-962-1253
.sp .5
Runaways	Hotline	1-800-231-6946
Runaways	Covenant House	1-800-999-9999
.sp .5
.T&
cB s s
li l lb.
Other Hotlines
AIDS	National AIDS Hotline	1-800-342-AIDS
AIDS	NJ AIDS Hotline	1-800-624-2377
Drug	Cocaine	1-800-COCAINE
Eating	Eating Disorders Hotline	1-800-624-2268
Support	Support Group Information	1-800-367-6274
VD	VD Information	1-800-227-8922
.TE
.ps 10
.vs 12p
.sp
.fi
This is a paragraph of normal text placed here only to indicate where
the left and right margins are.  In this way the reader can judge
the appearance of centered tables or expanded tables, and observe
how such tables are formatted.
.KE
.KS
.IN
.ps 8
.vs 10
\&\*.TS
\&expand;
\&c s s s
\&c c c c
\&l l n n\*.
\&New Jersey Bell Labs Locations
\&Name\*TAddress\*TArea Code\*TPhone
\&Chester\*TChester, NJ 07930\*T201\*T879-3400
\&Corporate Park III\*TSomerset, NJ 08873\*T201\*T271-2300
\&Crawford Hill\*THolmdel, NJ 07733\*T201\*T888-7000
\&ERC\*THopewell Township, NJ 08525\*T609\*T639-1234
\&Freehold\*TFreehold, NJ 07728\*T201\*T577-5000
\&Headquarters Plaza\*TMorristown, NJ 07960\*T201\*T898-6000
\&Holmdel\*THolmdel, NJ 07733\*T201\*T834-1000
\&Knightsbridge\*TPiscataway, NJ 08854\*T201\*T457-2000
\&Liberty Corner\*TWarren, NJ 07060\*T201\*T580-4000
\&Lincroft\*TLincroft, NJ 07738\*T201\*T576-4000
\&MDC\*THopewell Township, NJ 08525\*T609\*T639-6100
\&Middletown\*TMiddletown, NJ 07748\*T201\*T957-2000
\&Morris\*TMorristown, NJ 07960\*T201\*T898-1000
\&Murray Hill\*TMurray Hill, NJ 07974\*T201\*T582-3000
\&Parsippany\*TParsippany, NJ 07054\*T201\*T299-3000
\&Patriots Plaza\*TMorristown, NJ 07960\*T201\*T829-7200
\&Red Hill\*TMiddletown, NJ 07748\*T201\*T949-3000
\&Short Hills\*TShort Hills, NJ 07078\*T201\*T564-2000
\&Somerset\*TSomerset, NJ 08873\*T201\*T560-1300
\&Summit\*TSummit, NJ 07901\*T201\*T522-6555
\&Warren Service Center\*TWarren, NJ 07060\*T201\*T756-1527
\&West Long Branch\*TWest Long Branch, NJ 07764\*T201\*T870-7000
\&Whippany\*TWhippany, NJ 07981\*T201\*T386-3000
\&Woodhollow\*TParsippany, NJ 07054\*T201\*T581-3000
\&\*.TE
.ne 1.3i
.OU
.TS
expand;
c s s s
c c c c
l l n n.
New Jersey Bell Labs Locations
Name	Address	Area Code	Phone
Chester	Chester, NJ 07930	201	879-3400
Corporate Park III	Somerset, NJ 08873	201	271-2300
Crawford Hill	Holmdel, NJ 07733	201	888-7000
ERC	Hopewell Township, NJ 08525	609	639-1234
Freehold	Freehold, NJ 07728	201	577-5000
Headquarters Plaza	Morristown, NJ 07960	201	898-6000
Holmdel	Holmdel, NJ 07733	201	834-1000
Knightsbridge	Piscataway, NJ 08854	201	457-2000
Liberty Corner	Warren, NJ 07060	201	580-4000
Lincroft	Lincroft, NJ 07738	201	576-4000
MDC	Hopewell Township, NJ 08525	609	639-6100
Middletown	Middletown, NJ 07748	201	957-2000
Morris	Morristown, NJ 07960	201	898-1000
Murray Hill	Murray Hill, NJ 07974	201	582-3000
Parsippany	Parsippany, NJ 07054	201	299-3000
Patriots Plaza	Morristown, NJ 07960	201	829-7200
Red Hill	Middletown, NJ 07748	201	949-3000
Short Hills	Short Hills, NJ 07078	201	564-2000
Somerset	Somerset, NJ 08873	201	560-1300
Summit	Summit, NJ 07901	201	522-6555
Warren Service Center	Warren, NJ 07060	201	756-1527
West Long Branch	West Long Branch, NJ 07764	201	870-7000
Whippany	Whippany, NJ 07981	201	386-3000
Woodhollow	Parsippany, NJ 07054	201	581-3000
.TE
.KE
.br
.ps 8
.vs 9p
.ne 5i
.IN
\&\*.TS
\&box;
\&cb   s   s   s
\&c | c | c   s
\&ltiw(1i) | ltw(2i) | lp8 | lw(1\*.6i)p8\*.
\&Some Interesting Places
\&_
\&Name\*TDescription\*TPractical Information
\&_
\&T{
\&American Museum of Natural History
\&T}\*TT{
\&The collections fill 11\*.5 acres (Michelin) or 25 acres (MTA)
\&of exhibition halls on four floors\*.  There is a full-sized replica
\&of a blue whale and the world's largest star sapphire (stolen in 1964)\*.
\&T}\*THours\*T10-5:45 S M Tu Th, 10-9 W Sat\*. Sun\*.
\&\e^\*T\e^\*TLocation\*TT{
\&Central Park West & 79th St\*.
\&T}
\&\e^\*T\e^\*TAdmission\*TDonation: $1\*.00 asked
\&\e^\*T\e^\*TSubway\*TAA to 81st St\*.
\&\e^\*T\e^\*TTelephone\*T212-769-5100
\&_
\&Bronx Zoo\*TT{
\&About a mile long and \*.6 mile wide, this is the largest zoo in America\*.
\&A lion eats 18 pounds
\&of meat a day while a sea lion eats 15 pounds of fish\*.
\&T}\*THours\*TT{
\&10-5 M-Sat, to 5:30 Sun
\&T}
\&\e^\*T\e^\*TLocation\*TT{
\&185th St\*. & Southern Blvd, the Bronx\*.
\&T}
\&\e^\*T\e^\*TAdmission\*T$3.75/1.50 adult/children; Tu,We,Th donation
\&\e^\*T\e^\*TSubway\*T2, 5 to East Tremont Ave\*.
\&\e^\*T\e^\*TTelephone\*T212-367-1010
\&_
\&Brooklyn Museum\*TT{
\&Five floors of galleries contain American and ancient art\*.
\&There are American period rooms and architectural ornaments saved
\&from wreckers, such as a classical figure from Pennsylvania Station\*.
\&T}\*THours\*T10-5, closed Tues
\&\e^\*T\e^\*TLocation\*TT{
\&Eastern Parkway & Washington Ave\*., Brooklyn\*.
\&T}
\&\e^\*T\e^\*TAdmission\*TFree
\&\e^\*T\e^\*TSubway\*T2,3 to Eastern Parkway\*.
\&\e^\*T\e^\*TTelephone\*T212-367-1010
\&_
\&T{
\&New-York Historical Society
\&T}\*TT{
\&All the original paintings for Audubon's
\&\*.I
\&Birds of America
\&\*.R
\&are here, as are exhibits of American decorative arts, New York history,
\&Hudson River school paintings, carriages, and glass paperweights\*.
\&T}\*THours\*TT{
\&Tues-Sun, 10-5
\&T}
\&\e^\*T\e^\*TLocation\*TT{
\&Central Park West & 77th St\*.
\&T}
\&\e^\*T\e^\*TAdmission\*TFree
\&\e^\*T\e^\*TSubway\*TAA to 81st St\*.
\&\e^\*T\e^\*TTelephone\*T212-873-3400
\&\*.TE
.BP
.ps \n(PS
.vs \n(VSp
.OU
.fi
.rr 10 11 12 13 14 15 16 17 18 19 20 21 22 98 99
.rr 60 61 62 63 64 65 66 67 68 69 70
.rr 43 44 45 46 47 48 49 50 51 52 53
.rr 83 84 85 86 87 88 89 90 91 92 93
.rr #a
.rr #b
.rr #c
.rr #d
.rr #e
.rr YY
.rr OJ
.rr P
.rr AV CW GW DW FL KN SJ A1 A2 A3 I1 I2 I3
.in 0
.hy 1
.1C
.TS
box;
cb   s   s   s
c | c | c   s
ltiw(1i) | ltw(2i) | lp8| lw(1.6i)p8.
Some Interesting Places
_
Name	Description	Practical Information
_
T{
American Museum of Natural History
T}	T{
The collections fill 11.5 acres (Michelin) or 25 acres (MTA)
of exhibition halls on four floors.  There is a full-sized replica
of a blue whale and the world's largest star sapphire (stolen in 1964).
T}	Hours	10-5:45 S M Tu Th, 10-9 W Sat. Sun.
\^	\^	Location	T{
Central Park West & 79th St.
T}
\^	\^	Admission	Donation: $1.00 asked
\^	\^	Subway	AA to 81st St.
\^	\^	Telephone	212-769-5100
_
Bronx Zoo	T{
About a mile long and .6 mile wide, this is the largest zoo in America.
A lion eats 18 pounds
of meat a day while a sea lion eats 15 pounds of fish.
T}	Hours	T{
10-5 M-Sat, to 5:30 Sun
T}
\^	\^	Location	T{
185th St. & Southern Blvd, the Bronx.
T}
\^	\^	Admission	$3.75/1.50 adult/children; Tu,We,Th donation
\^	\^	Subway	2, 5 to East Tremont Ave.
\^	\^	Telephone	212-367-1010
_
Brooklyn Museum	T{
Five floors of galleries contain American and ancient art.
There are American period rooms and architectural ornaments saved
from wreckers, such as a classical figure from Pennsylvania Station.
T}	Hours	10-5, closed Tues
\^	\^	Location	T{
Eastern Parkway & Washington Ave., Brooklyn.
T}
\^	\^	Admission	Free
\^	\^	Subway	2,3 to Eastern Parkway.
\^	\^	Telephone	718-638-5000
_
T{
New-York Historical Society
T}	T{
All the original paintings for Audubon's
.I
Birds of America
.R
are here, as are exhibits of American decorative arts, New York history,
Hudson River school paintings, carriages, and glass paperweights.
T}	Hours	T{
Tues-Sun, 10-5
T}
\^	\^	Location	T{
Central Park West & 77th St.
T}
\^	\^	Admission	Free
\^	\^	Subway	AA to 81st St.
\^	\^	Telephone	212-873-3400
.TE
.SP
.2C
.rr 40
.rr 41
.rr 42
.rr 43
.rr 80
.rr 81
.rr 82
.rr 83
.rr 60
.rr 61
.rr 62
.rr 63
.rr #a
.rr #b
.rr #c
.rr #d
.rr #e
.rr ##
.fi
.FC
.SP
.2C
.NH
Acknowledgments.
.PP
Many thanks are due to J. C. Blinn, who has done a large amount
of testing and assisted with the design of the program.
He has also written many of the more intelligible sentences
in this document and helped edit all of it.
All phototypesetting programs on
.UX
are dependent on the work
of the late J. F. Ossanna, whose assistance with this program in particular
had been most helpful.
This program is patterned on a table formatter originally
written by J. F. Gimpel.
The assistance of
T. A. Dolotta, B. W. Kernighan, and J. N. Sturman
is gratefully acknowledged.
.NH
References.
.LP
|reference_placement
.FC
.1C
.BP
.NH
List of Tbl Command Characters and Words
.LP
.EQ
delim $$
gfont roman
.EN
.TS
center;
cI cI cI
aFCW3 l n.
Command	Meaning	Section
a A	Alphabetic subcolumn	2.2
allbox	Draw box around all items	2.1
b B	Boldface item	2.2
box	Draw box around table	2.1
c C	Centered column	2.2
center	Center table in page	2.1
delim(\fIxy\|\fP)	Set \fIeqn\fP delimiters	2.1
doublebox	Doubled box around table	2.1
e E	Equal width columns	2.2
expand	Make table full line width	2.1
f F	Font change	2.2
i I	Italic item	2.2
l L	Left adjusted column	2.2
linesize(\fIn\|\fP)\ \ 	Set point size for drawing lines	2.1
n N	Numerical column	2.2
\fInnn\fR	Column separation	2.2
p P	Point size change	2.2
r R	Right adjusted column	2.2
s S	Spanned item	2.2
t T	Vertical spanning at top	2.2
tab(\fIx\|)	Change data separator character	2.1
T{ T}	Text block	2.3
u U	Staggered columns	2.2
v V	Vertical spacing change	2.2
w W	Minimum width value	2.2
z Z	Ignore width of data in this column	2.2
\&.\fIxx\fR	Included \fItroff\fR command	2.3
|	Vertical line	2.2
||	Double vertical line	2.2
^	Vertical span	2.2
\e^	Vertical span	2.3
\&=	Double horizontal line	2.3
\&_	Horizontal line	2.3
\e_	Short horizontal line	2.3
\&\eR\fIx\fR	Repeat character	2.3
.TE

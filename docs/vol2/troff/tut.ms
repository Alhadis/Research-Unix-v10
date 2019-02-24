.so ../ADM/mac
.XX trofftut 223 "A Troff Tutorial"
.nr PI .2i
.nr PD 2u
.de UC
\\$3\s-1\\$1\s+1\\$2
..
.de BD
\&\\$3\f1\\$1\h'-\w'\\$1'u+3u'\\$1\fP\\$2
..
.....TM 76-1273-7 39199 39199-11
.TL
A Troff Tutorial\(dg
.AU
Brian W. Kernighan
.AI
.MH
.AB
.PP
.IT Troff
is a text-formatting program for typesetters and laser printers.
Such devices normally provide multiple fonts,
and a substantial number of mathematical symbols
and other special characters.
Characters can be printed in a range of sizes,
and placed anywhere on the page.
.PP
.IT Troff
allows the user full control over fonts,
sizes, and character positions,
as well as the usual features of a formatter \(em
right-margin justification, automatic hyphenation,
page titling and numbering, and so on.
It also provides macros, arithmetic variables and operations,
and conditional testing, for complicated formatting tasks.
.PP
This document is an introduction to the most basic use of
.IT troff .
It presents just enough information to enable the user
to do simple formatting
tasks like making viewgraphs,
and to make incremental changes to existing packages
of
.IT troff
commands.
.AE
.2C
.NH
Introduction
.tr ^.
.PP
.FS
\(dg This is a version of |reference(troff tutorial v7man) revised by B. W. Kernighan.
.FE
.IT Troff
is a text-formatting program,
written by J. F. Ossanna,
for producing
high-quality printed output from phototypesetters and laser printers.
This document is an example of
.IT troff
output.
.PP
The single most important rule
of using
.IT troff
is
not to use it directly, but through some intermediary.
In many ways,
.IT troff
resembles an assembly language \(em
a remarkably powerful and flexible one \(em
but nonetheless such that many operations must be specified
at a level of detail and in a form that is too hard 
for most people to use effectively.
.PP
For some special applications, there are programs that provide
an interface to
.IT troff
for the majority of users.
For example,
.IT eqn 
|reference(latest eqn)
provides an easy to learn language for typesetting mathematics;
the 
.IT eqn
user
need know no 
.IT troff
whatsoever
to typeset mathematics.
.IT Tbl
|reference(latest tbl)
provides the same convenience for producing tables.
.PP
For producing straight text (which may well contain mathematics or tables), there are a number of ``macro packages''
that define formatting rules and operations for specific styles
of documents,
and reduce the amount of
direct contact with 
.IT troff .
In particular, the
.CW -ms ,
.CW -mpm
and
.CW -mm
packages
for Bell Labs internal memoranda and external papers
provide most of the facilities needed
for a wide range of document preparation.
(This paper was prepared with
.CW -mpm .)
There are also packages for viewgraphs,
various journals,
and other special applications.
Typically you will find these packages easier to use
than
.IT troff
once you get beyond the most trivial operations;
you should always consider them first.
.PP
In the few cases where existing packages don't do the whole job,
the solution is
.ul
not
to write an entirely new set of
.IT troff
instructions from scratch, but to make small changes
to adapt packages that already exist.
.PP
In accordance with this philosophy of letting someone else
do the work,
the part of
.IT troff
described here is only a small part of the whole,
although it tries to concentrate on the more useful parts.
In any case, there is no attempt to be complete.
Rather, the emphasis is on showing how to do simple things,
and how to make incremental changes to what already exists.
The contents of the remaining sections are:
.SP .5
.nf
.in .1i
.ta .3i
\02.	Point sizes; line spacing
\03.	Fonts and special characters
\04.	Indents and line lengths
\05.	Tabs
\06.	Local motions: drawing lines and characters
\07.	Strings
\08.	Introduction to macros
\09.	Titles, pages and numbering
10.	Number registers and arithmetic
11.	Macros with arguments
12.	Conditionals
13.	Environments
14.	Diversions
	Appendix A:  Character set
.SP .5
.in 0
.fi
.PP
To use
.IT troff
you have to prepare not only the actual text you want printed,
but some information that tells
.ul
how
you want it printed.
For
.IT troff
the text
and
the formatting information are often intertwined quite intimately.
Most commands to
.IT troff
are placed on a line separate from the text itself,
beginning with a period (one command per line).
For example,
.P1
Some text.
^ps 14
Some more text.
.P2
will change the `point size',
that is,
the size of the letters being printed,
to `14 point' (one point is 1/72 inch) like this:
.P1
.ft 1
.fi
Some text.
.ps 14
Some more text.
.ps 10
.P2
.PP
Occasionally, though,
something special occurs in the middle of a line \(em
to produce
.P1
.ft 1
Area = \(*p\fIr\fR\|\s8\u2\d\s0
.P2
you have to type
.P1
Area = \e(*p\efIr\efR\e\^|\^\es8\eu2\ed\es0
.P2
(which we will explain shortly).
The backslash character
.CW  \e 
is used 
to introduce
.IT troff
commands and special characters within a line of text.
.NH
Point Sizes; Line Spacing
.PP
As mentioned above,
the command
.CW .ps
sets the point size.
One point is 1/72 inch,
so 6-point characters are at most 1/12 inch high,
and 36-point characters are \(12 inch.
Different devices provide different sets of sizes;
a representative set is shown below.
.P1 1
.ft 1
.ps 6
6 point: Pack my box with five dozen liquor jugs.
.ps 7
.vs 8p
7 point: Pack my box with five dozen liquor jugs.
.vs 9p
.ps 8
8 point: Pack my box with five dozen liquor jugs.
.vs 10p
.ps 9
9 point: Pack my box with five dozen liquor jugs.
.vs 11p
.ps 10
10 point: Pack my box with five dozen liquor
.vs 12p
.ps 11
11 point: Pack my box with five dozen 
.vs 14p
.ps 12
12 point: Pack my box with five dozen
.vs 16p
.ps 14
14 point: Pack my box with five
.vs 24p
\s1616 point\s18 18 point\s20 20 point
.vs 40p
\s2222\s24 24\s28 28\s36 36
.ps 10
.vs 12p
.P2
.PP
If the number after
.CW .ps 
is not a
legal size,
it is rounded up to the next valid value.
If no number follows
.CW .ps ,
.IT troff
reverts to the previous size, whatever it was.
.IT troff
begins with point size 10,
which is usually fine.
This document is printed in \n(.s point.
.PP
The point size can also be changed in the middle of a line
or even a word
with the in-line command
.CW \es .
To produce
.P1
.ft 1
\s8UNIX\s10 ran on a \s8PDP-\s1011/45
.P2
type
.P1 0
\es8UNIX\es10 ran on a \es8PDP-\es1011/45
.P2
As above,
.CW \es 
should be followed by a legal point size,
except that
.CW \es0 
causes the size to revert to
its previous value.
Notice that
.CW \es1011
can be understood correctly as `size 10, followed by an 11', if the size is legal,
but not otherwise.
Be cautious with similar constructions.
.PP
Relative size changes are also legal and useful:
.P1
\es-2UNIX\es+2
.P2
temporarily decreases the size, whatever it is, by two points, then
restores it, producing \s-2UNIX\s+2.
Relative size changes have the advantage that the size difference
is independent of the starting size of the document.
The amount of the relative change is restricted
to a single digit.
.PP
The other parameter that determines what the type looks like
is the spacing between lines,
which is set independently of the point size.
Vertical spacing is measured from the bottom of one line to
the bottom of the next.
The command to control vertical spacing is
.CW .vs .
For running text, it is usually best to set the vertical spacing
about 20% bigger than the character size.
For example, so far in this document, we have used
``10 on 12'', that is,
.P1
^ps 10
^vs 12p
.P2
If we changed to
.P1
^ps 10
^vs 10p
.P2
.vs 10p
.ne 3
the running text would look like this.
After a few lines, you will agree it looks a little cramped.
The right vertical spacing is partly a matter of taste, depending on how
much text you want to squeeze into a given space,
and partly a matter of traditional printing style.
By default,
.IT troff
uses 10 on 12.
.PP
.vs 14p
.ps 12
Point size and vertical spacing make a substantial difference in the amount of text
per square inch.
This is 12 on 14.
.ne 2
.PP
.ne 2
.ps 6
.vs 7p
Point size and vertical spacing make a substantial difference in the amount of text
per square inch.
For example,
10 on 12 uses about twice as much space as 7 on 8.
This is 6 on 7, which is even smaller.
It packs a lot more words per line,
but you can go blind trying to read it.
.PP
When used without arguments,
.CW .ps
and
.CW .vs
revert to the previous size and vertical spacing
respectively.
.PP
The command
.CW .sp
is used to get extra vertical space.
Unadorned, 
it gives you one extra blank line (one
.CW .vs ,
whatever that has been set to).
Typically, that's more or less than you want,
so
.CW .sp
can be followed by
information about how much space you want \(em
.P1
^sp 2i
.P2
means `two inches of vertical space'.
.P1
^sp 2p
.P2
means `two points of vertical space';
and
.P1
^sp 2
.P2
means `two vertical spaces' \(em two of whatever
.CW .vs
is set to
(this can also be made explicit with
.CW .sp\ 2v );
.IT troff
also understands decimal fractions in most places,
so
.P1
^sp 1.5i
.P2
is a space of 1.5 inches.
These same scale factors can be used after
.CW .vs
to define line spacing, and in fact after most commands
that deal with physical dimensions.
.PP
It should be noted that all size numbers are converted internally
to `machine units', which are much smaller, reflecting the finer
resolution of particular output devices.
Many laser printers run at 300 units/inch;
typesetters are often more than 1000 units/inch.
.NH
Fonts and Special Characters
.PP
.IT Troff
allows many different fonts at any one time.
Normally a family of serif fonts (Times roman, italic and bold),
a family of san-serif fonts (Helvetica), and a collection of special characters
are available.
.P1 .2i
.ft R
abcdefghijklmnopqrstuvwxyz 0123456789
ABCDEFGHIJKLMNOPQRSTUVWXYZ
.ft I
abcdefghijklmnopqrstuvwxyz 0123456789
ABCDEFGHIJKLMNOPQRSTUVWXYZ
.ft B
abcdefghijklmnopqrstuvwxyz 0123456789
ABCDEFGHIJKLMNOPQRSTUVWXYZ
.ft BI
abcdefghijklmnopqrstuvwxyz 0123456789
ABCDEFGHIJKLMNOPQRSTUVWXYZ
.sp 3p
.ft H
abcdefghijklmnopqrstuvwxyz 0123456789
ABCDEFGHIJKLMNOPQRSTUVWXYZ
.ft HI
abcdefghijklmnopqrstuvwxyz 0123456789
ABCDEFGHIJKLMNOPQRSTUVWXYZ
.ft HB
abcdefghijklmnopqrstuvwxyz 0123456789
ABCDEFGHIJKLMNOPQRSTUVWXYZ
.P2
A representative collection of
greek, mathematical symbols and miscellaneous other characters are
listed in Appendix A.
Appendix B shows the font families commonly available on Postscript printers;
your mileage may vary.
.PP
.IT Troff
prints in roman unless told otherwise.
To switch into bold, use
the
.CW .ft
command
.P1
^ft B
.P2
and for italics,
.P1
^ft I
.P2
To return to roman, use
.CW .ft\ R ;
to return to the previous font,
whatever it was,
use either
.CW .ft\ P
or just
.CW .ft  .
The `underline' command
.P1
^ul
.P2
causes the next input line to print in italics.
.CW .ul
can be followed by a count to
indicate that more than one line is to be italicized.
.PP
Fonts can also be changed within a line or word
with the in-line command
.CW \ef :
.P1
\fBbold\fIface\fR text
.P2
is produced by
.P1
\efBbold\efIface\efR text
.P2
If you want to do this so the previous font, whatever it was,
is left undisturbed, insert extra
.CW \efP
commands, like this:
.P1
\efBbold\efP\efIface\efP\efR text\efP
.P2
Because only the immediately previous font is remembered,
you have to restore the previous font after each change
or you can lose it.
The same is true of 
.CW .ps
and
.CW .vs
when used without an argument.
.PP
There are many other fonts available besides the standard set.
Normally you can just use a font by naming it,
but if an entire document is to be printed
in a non-standard set of fonts,
it is best to mount the fonts explicitly.
This tells 
.IT troff
which fonts you will make the most use of.
The
.CW .fp
command
.P1
^fp 3 PB
.P2
says that the Palatino Bold font
.ft PB
(which looks like this)
.ft
is mounted on position 3.
.PP
It is possible to make a document relatively independent
of the actual fonts used to print it
by using font numbers instead of names;
for example,
.CW \ef3
and
.CW ".ft 3
mean `whatever font is mounted at position 3',
and thus work for any setting.
The standard macro packages assume that fonts 1 through 4 contain
roman, italic, bold, and bold italic members of a family.
.PP
There is also a way to get synthetic bold fonts
by overstriking letters with a slight offset,
with the
.CW .bd
command;
it is illustrated in Section 11.
.PP
Special characters have four-character names beginning with
.CW \e( ;
they may be inserted anywhere.
For example,
.P1
.ft 1
\(14 + \(12 = \(34
.P2
is produced by
.P1
\e(14 + \e(12 = \e(34
.P2
In particular,
greek letters are all of the form
.CW  \e(*\- ,
where
.CW -
is an upper or lower case roman letter
reminiscent of the greek.
Thus
to get
.P1
.ft 1
\(*S(\(*a\(mu\(*b) \(-> \(if
.P2
in bare
.IT troff
we have to type
.P1
\e(*S(\e(*a\e(mu\e(*b) \e(\(mi> \e(if
.P2
That line is unscrambled as follows:
.P1
.ta 1i 2i 3i
\e(*S	\(*S
(	(
\e(*a	\(*a
\e(mu	\(mu
\e(*b	\(*b
)	)
\e(\(mi>	\(->
\e(if	\(if
.P2
Appendix A contains a sample of special names.
.PP
In
.IT eqn 
the same effect can be achieved with the input
.P1
SIGMA ( alpha times beta ) \-> inf
.P2
which is less concise, but clearer to the uninitiated.
.PP
Notice that
each
four-character name is a single character
as far as
.IT troff
is concerned \(em
the
`translate' command
.P1
^tr \e(mi\e(em
.P2
is perfectly clear, meaning
.P1
^tr \f1\(mi\h'.1m'\(em
.P2
that is, to translate \(mi into \(em.
.PP
Some characters are automatically translated into others:
grave  \(ga  and acute  \(aa  accents (apostrophes) become open and close single quotes
`\0';
the combination of ``...'' is generally preferable to the double quotes "...".
Similarly a typed minus sign becomes a hyphen -.
To print an explicit \- sign, use
.CW \e- .
To print a backslash, use
.CW \ee .
.NH
Indents and Line Lengths
.PP
.IT Troff
starts with a line length of 6.5 inches,
too wide for 8\(12\(mu11 paper.
To reset the line length,
use
the
.CW .ll
command:
.P1
^ll 6i
.P2
As with
.CW .sp ,
the actual length can be specified in several ways;
inches are probably the most intuitive.
.PP
Output is normally positioned an inch from the left edge of the paper.
To reset the default physical left margin (``page offset''),
use the
.CW .po
command.
.P1
^po 0
.P2
sets the offset as far to the left as it will go.
.PP
The indent command
.CW .in
causes the left margin to be indented
by some specified amount from the page offset.
If we use
.CW .in
to move the left margin in,
and
.CW .ll
to move the right margin to the left,
we can
make offset blocks of text:
.P1
^in 0.3i
^ll \(mi0.3i
text to be set into a block
^ll +0.3i
^in \(mi0.3i
.P2
will create a block that looks like this:
.P1
.fi
.ll -0.3i
.ft 1
Pater noster qui est in caelis sanctificetur nomen tuum;
adveniat regnum tuum; fiat voluntas tua, sicut in caelo,
et in terra. ...
Amen.
.ll +0.3i
.P2
Notice the use of `+' and `\(mi'
to specify the amount of change.
These change the previous setting by the specified amount,
rather than just overriding it.
The distinction is quite important:
.CW .ll\ +1i
makes lines one inch longer;
.CW .ll\ 1i
makes them one inch
.ul
long.
.PP
With 
.CW .in ,
.CW .ll
and
.CW .po ,
the previous value is used if no argument is specified.
.PP
To indent a single line, use the `temporary indent'
command
.CW .ti .
For example, all paragraphs in this memo
effectively begin with the command
.P1
^ti 3
.P2
Three of what?
The default unit for
.CW .ti ,
as for most horizontally oriented commands
.CW .ll , (
.CW .in ,
.CW .po ),
is ems;
an em is roughly the width of the letter `m'
in the current point size.
(Precisely, a em in size
.ul
p
is
.ul
p
points.)
Although inches are usually clearer than ems to people who don't set type
for a living,
ems have a place:
they are a measure of size that is proportional to the current point size.
If you want to make text that keeps its proportions
regardless of point size,
you should use ems for all dimensions.
Ems can be specified as scale factors directly,
as in
.CW .ti\ 2.5m .
.PP
Lines can also be indented negatively
if the indent is already positive:
.P1
^ti \(mi0.3i
.P2
causes the next line to be moved back three tenths of an inch.
Thus to make a decorative initial capital,
we indent the whole paragraph, then move the letter `P' back with
a
.CW .ti
command:
.KS
.SP .5
.ll -0.3i
.fi
.in +.6i
.ti -0.3i
.ft 1
\s36\v'2'P\v'-2'\s0ater noster qui est in caelis sanctificetur
nomen tuum;
adveniat regnum tuum;
'in -.3i
fiat voluntas tua,
sicut in caelo, et in terra. ...
Amen.
.ll +0.3i
.in 0
.br
.SP .5
.KE
.LP
Of course, there is also some trickery to make the `P'
bigger (just a
.CW \es36P\es0 ),
and to move it
down from its normal position
(see the section on local motions).
.NH
Tabs
.PP
Tabs
(the \s8ASCII\s0 `horizontal tab' character)
can be used to produce output in columns,
or to set the horizontal position of output.
Typically
tabs are used only in unfilled text.
Tab stops are set by default every half inch from the
current indent,
but
can be changed by the
.CW .ta
command.
To set stops every inch, for example,
.P1
^ta 1i 2i 3i 4i 5i 6i
.P2
.PP
By default the stops are left-justified
(as on a typewriter),
but there are also centering and right-adjusting tabs.
.PP
For a handful of numeric columns, use right-adjusting tabs:
.P1
^nf
^ta .5iR 1iR 1.5iR
\fItab\f(CW 1\fI tab \f(CW2\fI tab \f(CW3
\fItab \f(CW40\fI tab \f(CW50\fI tab \f(CW60
\fItab \f(CW700\fI tab \f(CW800\fI tab \f(CW900\f(CW
^fi
.P2
The 
.CW R
means ``right adjust.''
When printed, this will produce
.P1 0
.ft 1
.ta .5iR 1iR 1.5iR
	1	2	3
	40	50	60
	700	800	900
.P2
If you have many numbers,
or if you need more complicated table layout,
don't
use
.IT troff 
directly;
use
.IT tbl .
.PP
It is also possible to fill up tabbed-over space with
some character other than blanks by setting the `tab replacement character'
with the
.CW .tc
command:
.P1
^ta 1.5i 2.5i
^tc \e(ru	(\e(ru is "\(ru")
Name \fItab\fR Age \fItab\fR 
.P2
produces
.P1 3
.ft 1
.ta 1.5i 2.5i
.tc \(ru
Name	 Age 	
.tc
.P2
To reset the tab replacement character to a blank, use
.CW .tc
with no argument.
(Lines can also be drawn with the
.CW \el
command, described in Section 6.)
.PP
.IT Troff
also provides a very general mechanism called `fields'
for setting up complicated columns.
(This is used by
.IT tbl .)
We will not go into it in this paper.
.NH
Local Motions: Drawing lines and characters
.PP
.sp 10p
Remember `Area = \(*pr\u2\d' and the big `P'
in the Paternoster.
How are they done?
.IT Troff
provides a host of commands for placing characters of any size
at any place.
You can use them to draw special characters 
or to tune your output for a particular appearance.
Most of these commands are straightforward, but messy to read
and tough to type correctly.
.PP
If you won't use 
.IT eqn ,
subscripts and superscripts are most easily done with
the half-line local motions
.CW \eu
and
.CW \ed .
To go back up the page half a point-size, insert a
.CW \eu
at the desired place;
to go down, insert a
.CW \ed .
.CW \eu \& (
and
.CW \ed
should always
be used in pairs, as explained below.)
Thus
.P1
Area = \e(*pr\eu2\ed
.P2
produces
.P1
.ft 1
Area = \(*pr\u2\d
.P2
To make the `2' smaller, bracket it with
.CW \es\-2
and
.CW \es0 .
Since
.CW \eu
and
.CW \ed
refer to the current point size,
be sure to put them either both inside or both outside
the size changes,
or you will get an unbalanced vertical motion.
.PP
Sometimes the space given by
.CW \eu
and
.CW \ed
isn't the right amount.
The 
.CW \ev
command can be used to request an arbitrary amount of vertical motion.
The in-line command
.P1
\ev'\f2amount\fP'
.P2
causes motion up or down the page by the amount specified in
`\f2amount\fP'.
For example, to move the `P' down, we used
.P1 2
.ta 1i
^in +0.6i	(move paragraph in)
^ll \-0.3i	(shorten lines)
^ti \-0.3i	(move P back)
\ev'2'\es36P\es0\ev'\-2'ater noster qui est
in caelis ...
.P2
A minus sign causes upward motion, while
no sign or a plus sign means down the page.
Thus
.CW \ev'\-2'
causes an upward vertical motion 
of two line spaces.
.PP
There are many other ways to specify the amount of motion \(em
.P1
\ev'0.1i'
\ev'3p'
\ev'\-0.5m'
.P2
and so on are all legal.
Notice that the scale specifier
.CW i
or
.CW p
or
.CW m
goes inside the quotes.
Any character can be used in place of the quotes;
this is also true of all other
.IT troff
commands described in this section.
.PP
Since
.IT troff
does not take within-the-line vertical motions into account
when figuring out where it is on the page,
output lines can have unexpected positions
if the left and right ends aren't at the same
vertical position.
Thus
.CW \ev ,
like
.CW \eu
and
.CW \ed ,
should always balance upward vertical motion in a line with
the same amount in the downward direction.
.PP
Arbitrary horizontal motions are also available \(em
.CW \eh
is like
.CW \ev ,
except that the default scale factor is ems instead of line spaces.
As an example,
.P1
\eh'\-0.1i'
.P2
causes a backwards motion of a tenth of an inch.
Consider printing the mathematical symbol
`>>'.
The default spacing is a bit too wide, so
.IT eqn
replaces this by
.P1
>\eh'\-0.1m'>
.P2
to produce >\h'-.1m'>.
.PP
Frequently
.CW \eh
is used with the `width function'
.CW \ew
to generate motions equal to the width
of some character string.
The construction
.P1
\ew'thing'
.P2
is a number equal to the width of `thing' in machine units.
All
.IT troff
computations are ultimately done in these units.
To move horizontally the width of an `x',
we can say
.P1
\eh'\ew'x'u'
.P2
As we mentioned above,
the default scale factor for
all horizontal dimensions is
.CW m ,
ems, so here we must have the
.CW u
for machine units,
or the motion produced will be far too large.
.IT Troff
is quite happy with the nested quotes, by the way,
so long as you don't leave any out.
.SP
.PP
There are also several special-purpose
.IT troff
commands for local motion.
.CW \e0
is an unpaddable white space
of the same width as a digit.
`Unpaddable' means that it will never be widened
or split across a line by line justification and filling.
There is also
.CW \e (blank),
.tr ^^
which is an unpaddable character the width of a space,
.CW \e| ,
which is half that width,
.CW \e^ ,
which is one quarter of the width of a space,
and
.CW \e& ,
which has zero width.
.tr ^.
(This last one is useful, for example, in entering
a text line which would otherwise begin with a `.'.)
.PP
The command
.CW \eo ,
used like
.P1
\eo'set of characters'
.P2
causes (up to 9)
characters to be overstruck,
centered on the widest.
This is nice for accents, as in
.P1
m\eo"e\e(aa"long\eo"e\e(ga"ne
.P2
which makes
.P1
.ft 1
m\o"e\(aa"long\o"e\(ga"ne
.P2
The accents are
.CW \e(ga
and
.CW \e(aa ,
or
.CW \e\` 
and
.CW \e\' ;
remember that each is just one character to
.IT troff .
.SP
.PP
You can make your own overstrikes with another special convention,
.CW \ez ,
the zero-motion command.
.CW \ezx
suppresses the normal horizontal motion
after printing the single character
.CW x ,
so another character can be laid on top of it.
Although sizes can be changed within
.CW \eo ,
it centers the characters on the widest,
and
there can be no horizontal or vertical motions,
so
.CW \ez
may be the only way to get what you want:
.P1
.sp 2
\s8\z\(sq\s14\z\(sq\s22\z\(sq\s36\(sq
.P2
is produced by
.P1 0
^sp 2
\es8\ez\e(sq\es14\ez\e(sq\es22\ez\e(sq\es36\e(sq
.P2
The
.CW .sp
is needed to leave room for the result.
.PP
As another example, an extra-heavy semicolon
that looks like
.P1
.ft 1
\s+6\z,\v'-0.25m'.\v'0.25m'\s0  instead of  ;  or  \s+6;\s0
.P2
can be constructed with a big comma and a big period above it:
.P1
\es+6\ez,\ev'\(mi0.25m'.\ev'0.25m'\es0 
.P2
`0.25m' is an empirical constant.
.SP
.PP
A more ornate overstrike is given by the bracketing function
.CW \eb ,
which piles up characters vertically,
centered on the current baseline.
Thus we can get big brackets,
constructing them with piled-up smaller pieces:
.P1
.sp
.ne 3
\b'\(lt\(lk\(lb' \b'\(lc\(lf' x \b'\(rc\(rf' \b'\(rt\(rk\(rb'
.sp
.P2
by typing in only this:
.P1 0
.ps -2
\&^sp
\eb'\e(lt\e(lk\e(lb' \eb'\e(lc\e(lf' x \e
\eb'\e(rc\e(rf' \eb'\e(rt\e(rk\e(rb'
.P2
.PP
.IT Troff
also provides a convenient facility for drawing horizontal and vertical
lines of arbitrary length with arbitrary characters.
.CW \el'1i'
draws a line one inch long, like this:
\l'1i'\|.
The length can be followed by
the character to use if the \(ru isn't appropriate;
.CW \el'0.5i.'\&
draws a half-inch line of dots: \l'.5i.'.
The construction
.CW \eL
is analogous to
.CW \el ,
except that it draws a vertical line instead of horizontal.
.NH
Strings
.PP
Obviously if a paper contains a large number of occurrences
of an acute accent over a letter `e',
typing
.CW \eo"e\e\'"
for each \*e
would be a great nuisance.
.PP
Fortunately,
.IT troff
provides a way in which you can store an arbitrary
collection of text in a `string',
and thereafter use the string name as a shorthand
for its contents.
Strings are one of several
.IT troff
mechanisms whose judicious use
lets you type a document
with less effort and organize
it
so that extensive format changes
can be made with few editing changes.
.PP
Strings are defined with the command
.CW .ds .
The line
.P1
\&^ds e \eo"e\e'"
.P2
defines the string
.CW e
to have the value
.CW \eo"e\e\'" .
.PP
A reference to a string is replaced by whatever
text
the string was defined as.
String names may be either one or two characters long,
and are referred to by
.CW \e*x
for one character names or
.CW \e*(xy
for two character names.
Thus to get
.ds e \o"e\*'"
t\*el\*ephone,
given the definition of the string
.CW e
as above,
we can type
.CW t\e*el\e*ephone ,
to produce t\*el\*ephone.
.PP
If a string must begin with blanks, define it as
.P1
\&.ds xx "      text
.P2
The double quote signals the beginning of the definition.
There is no trailing quote;
the end of the line terminates the string.
.PP
A string may actually be several lines long;
if
.IT troff
encounters a 
.CW \e
at the end of
.ul
any
line, it is thrown away and the next line
added to the current one.
So you can make a long string simply by ending each line
but the last with a backslash:
.P1
\&^ds xx this \e
is a very \e
long string
.P2
.PP
Strings may be defined in terms of other strings, or even in terms of themselves;
we will discuss some of these possibilities later.
.NH
Introduction to Macros
.PP
Before we can go much further in
.IT troff ,
we need to learn a bit about the
macro
facility.
In its simplest form, a macro is just a shorthand notation
quite similar to a string.
Suppose we want every paragraph to start
in exactly the same way \(em
with a space and a temporary indent of two ems:
.P1
^sp
^ti +2m
.P2
Then to save typing, we would like to collapse these into
one shorthand line,
a
.IT troff
`command' like
.P1
^PP
.P2
that would be treated by
.IT troff
exactly as
.P1
^sp
^ti +2m
.P2
.CW .PP
is called a
.ul
macro.
The way we tell
.IT troff
what
.CW .PP
means is to
.ul
define
it with the
.CW .de
command:
.P1
^de PP
^sp
^ti +2m
^^
.P2
The first line names the macro
(we used
.CW .PP ' `
for `paragraph',
and upper case so it wouldn't conflict with
any name that
.IT troff
might
already know about).
The last line
.CW ..\&
marks the end of the definition.
In between is the text,
which is simply inserted whenever
.IT troff
sees the `command'
or macro call
.P1
^PP
.P2
A macro
can contain any mixture of text and formatting commands.
.PP
The definition of
.CW .PP
has to precede its first use;
undefined macros are simply ignored.
Names are restricted to one or two characters.
.PP
Using macros for commonly occurring sequences of commands
is critically important.
Not only does it save typing,
but it makes later changes much easier.
Suppose we decide that the paragraph indent is too small,
the vertical space is much too big,
and roman font should be forced.
Instead of changing the whole document,
we need only change the definition of
.CW .PP
to
something like
.P1
^de PP	\e" paragraph macro
^sp 2p
^ti +3m
^ft R
^^
.P2
and the change takes
effect everywhere we used
.CW .PP .
.PP
.CW \e"
is a
.IT troff
command that causes the rest of the line to be ignored.
We use it here to add comments to the macro
definition
(a wise idea once definitions get complicated).
.PP
As another example of macros,
consider these two which start and end a block of offset,
unfilled text, like most of the examples in this paper:
.P1
^de BS	\e" start indented block
^sp
^nf
^in +0.3i
^ft CW  \e" constant width font
^^
^de BE	\e" end indented block
^sp
^ft 1
^fi
^in \(mi0.3i
^^
.P2
Now we can surround text like
.P1
Copy to
John Doe
Richard Roberts
Stanley Smith
.P2
by the commands
.CW .BS
and
.CW .BE ,
and it will come out as it did above.
Notice that we indented by
.CW .in
.CW +0.3i
instead of
.CW .in
.CW 0.3i .
This way we can nest our uses of
.CW .BS
and
.CW .BE
to get blocks within blocks.
.PP
If later on we decide that the indent
should be 0.5i, then it is only necessary to
change the definitions of
.CW .BS
and
.CW .BE ,
not the whole paper.
.NH
Titles, Pages and Numbering
.PP
This is an area where things get tougher,
because nothing is done for you automatically.
Of necessity, some of this section is a cookbook,
to be copied literally until you get some experience.
.PP
Suppose you want a title at the top of each page,
saying just
.sp 3p
.lt 2.8i
.tl '\ \ \ \ left top'center top'right top\ \ \ \ '
.lt
.sp 3p
Unfortunately, this requires work.
.PP
You have to say what the actual title is (easy);
when to print it (easy enough);
and what to do at and around the title line (harder).
Taking these in reverse order,
first we define a macro
.CW .NP
(for `new page') to process
titles and the like at the end of one page
and the beginning of the next:
.P1 .2i
^de NP
\&'bp
\&'sp 0.5i
\&\&.tl 'left top'center top'right top'
\&'sp 0.3i
\&^^
.P2
To make sure we're at the top of a page,
we issue a `begin page' command
.CW 'bp ,
which causes a skip to top-of-page
(we'll explain the
.CW '
shortly).
Then we space down half an inch,
print the title
(the use of
.CW .tl
should be self explanatory; later we will discuss parameterizing the titles),
space another 0.3 inches,
and we're done.
.PP
To ask for
.CW .NP
at the bottom of each page,
we have to say something like
`when the text is within an inch
of the bottom of the page,
start the processing
for a new page.'
This is done with a `when' command
.CW .wh :
.P1
^wh \-1i NP
.P2
(No `.' is used before
.CW NP ;
this is simply the name of a macro, not a macro call.)
The minus sign means
`measure up from the bottom of the page',
so
.CW -1i
means `one inch from the bottom'.
.PP
The
.CW .wh
command appears in the input outside the definition of
.CW .NP ;
typically the input would be
.P1
^de NP
^^^
^^
^wh \-1i NP
.P2
.PP
Now what happens?
As text is actually being output,
.IT troff 
keeps track of its vertical position on the page,
and after a line is printed within one inch from the bottom,
the
.CW .NP
macro is activated.
(In the jargon, the
.CW .wh
command sets a
.ul
trap
at the specified place,
which is `sprung' when that point is passed.)
.CW .NP
causes a skip to the top of the next page
(that's what the
.CW 'bp
was for),
then prints the title with the appropriate margins.
.PP
Why
.CW 'bp
and
.CW 'sp 
instead of
.CW .bp
and
.CW .sp ?
The answer is that
.CW .sp
and
.CW .bp ,
like several other commands,
cause a
.ul
break
to take place.
That is, all the input text collected but not yet printed
is flushed out as soon as possible,
and the next input line is guaranteed to start
a new line of output.
If we had used
.CW .sp
or
.CW .bp
in the
.CW .NP
macro,
this would cause a break in the middle
of the current output line when a new page is started.
The effect would be to print the left-over part of that line
at the top of the page, followed by the next input line on a new output line.
This is
.ul
not
what we want.
Using
.CW '
instead of
.CW . \&
for a command
tells
.IT troff 
that
no break is to take place \(em
the output line
currently being filled
should
.ul
not
be forced out before the space or new page.
.PP
The list of commands that cause a break 
is short and natural:
.P1 .1i
^bp  ^br  ^ce  ^fi  ^nf  ^sp  ^in  ^ti
.P2
All others cause
.ul
no
break,
regardless of whether you use a
.CW . \&
or a 
.CW ' .
If you really need a break, add a
.CW .br 
command at the appropriate place.
.PP
One other thing to beware of \(em
if you're changing fonts or point sizes a lot,
you may find that
if you cross a page boundary
in an unexpected font or size,
your titles come out in that size and font
instead of what you intended.
Furthermore, the length of a title is independent of the current line length,
so titles will come out at the default length of 6.5 inches
unless you change it,
which is done with the
.CW .lt
command.
.PP
There are several ways to fix the problems of point sizes
and fonts in titles.
For the simplest applications, we can change
.CW .NP 
to set the proper size and font for the title,
then restore the previous values as shown below.
.KF
.P1 .2i
.ta .8i
^de NP
\&'bp
\&'sp 0.5i
^ft R	\e" set title font to roman
^ps 10	\e" and size to 10 point
^lt 6i	\e" and length to 6 inches
^tl 'left'center'right'
^ps	\e" revert to previous size
^ft P	\e" and to previous font
\&'sp 0.3i
^^
.P2
.KE
.PP
This version of
.CW .NP
does
.ul
not
work if the fields in the
.CW .tl
command contain size or font changes.
To cope with that
requires
.IT troff 's
`environment' mechanism,
which we will discuss in Section 13.
.PP
To get a footer at the bottom of a page,
you can modify
.CW .NP
so it does
some processing before
the
.CW 'bp
command,
or split the job into a footer macro invoked
at the bottom margin and a header macro invoked
at the top of the page.
These variations are left as exercises.
.SP
.PP
Output page numbers are computed automatically
as each page is produced (starting at 1),
but no numbers are printed unless you ask for them explicitly.
To get page numbers printed,
include the character
.CW %
in the
.CW .tl
line at
the position where you want the number to appear.
For example
.P1
^tl ''- % -''
.P2
centers the page number inside hyphens, as on this page.
You can set the page number at any time
with either
.CW .bp\ n ,
which immediately starts a new page numbered
.CW n ,
or with
.CW .pn\ n ,
which sets the page number for the next page
but doesn't cause a skip to the new page.
Again,
.CW .bp\ +n
sets the page number to
.CW n
more than its current value;
.CW .bp
means
.CW .bp\ +1 .
.NH
Number Registers and Arithmetic
.PP
.IT Troff
has a facility for doing arithmetic,
and for defining and using variables with numeric values,
called
.ul
number registers.
Number registers, like strings and macros, can be useful in setting up a document
so it is easy to change later.
And of course they serve for any sort of arithmetic computation.
.PP
Like strings, number registers have one or two character names.
They are set by the
.CW .nr
command,
and are referenced anywhere by
.CW \enx
(one character name) or
.CW \en(xy
(two character name).
.PP
There are quite a few pre-defined number registers maintained by
.IT troff ,
among them
.CW %
for the current page number;
.CW nl
for the current vertical position on the page;
.CW dy ,
.CW mo
and
.CW yr
for the current day, month and year; and
.CW .s
and
.CW .f
for the current size and font.
(The font is a number from 1 to typically around 10.)
Any of these can be used in computations like any other register,
but some, like
.CW .s
and
.CW .f ,
cannot be changed with
.CW .nr .
.PP
As an example of the use of number registers,
in the
.CW -ms
macro package,
most significant parameters are defined in terms of the values
of a handful of number registers.
These include the point size for text, the vertical spacing,
and the line and title lengths.
To set the point size and vertical spacing for the following paragraphs, for example, a user may say
.P1
^nr PS 9
^nr VS 11
.P2
The paragraph macro
.CW .PP
is defined (roughly) as follows:
.P1
.ta  1i
^de PP
^ps \e\en(PS	\e" reset size
^vs \e\en(VSp	\e" spacing
^ft 1	\e" font
^sp 0.5v	\e" half a line
^ti +3m
^^
.P2
This sets the font to font 1 and the point size and line spacing
to whatever values are stored in the number registers
.CW PS
and
.CW VS .
.PP
Why are there two backslashes?
This is the eternal problem of how to quote a quote.
When
.IT troff
originally reads the macro definition,
it peels off one backslash
to see what's coming next.
To ensure that another is left in the definition when the 
macro is
.ul
used,
we have to put in two backslashes in the definition.
If only one backslash is used, 
point size and vertical spacing will be frozen at the time the macro
is defined, not when it is used.
.PP
Protecting by an extra layer of backslashes
is only needed for
.CW \en ,
.CW \e* ,
.CW \e$
(which we haven't come to yet),
and
.CW \e
itself.
Things like
.CW \es ,
.CW \ef ,
.CW \eh ,
.CW \ev ,
and so on do not need an extra backslash,
since they are converted by
.IT troff
to an internal code immediately upon being seen.
.SP
.PP
Arithmetic expressions can appear anywhere that
a number is expected.
As a trivial example,
.P1
^nr PS \e\en(PS\-2
.P2
decrements
.CW PS
by 2.
Expressions can use the arithmetic operators +, \-, *, /, % (mod),
the relational operators >, >=, <, <=, =, and != (not equal),
and parentheses.
.PP
Although the arithmetic we have done so far
has been straightforward,
more complicated things are somewhat tricky.
First,
number registers hold only integers.
.IT troff
arithmetic uses truncating integer division, just like Fortran.
Second, in the absence of parentheses,
evaluation is done left-to-right
without any operator precedence
(including relational operators).
Thus
.P1
7*\-4+3/13
.P2
becomes `\-1'.
Number registers can occur anywhere in an expression,
and so can scale indicators like
.CW p ,
.CW i ,
.CW m ,
and so on (but no spaces).
Although integer division causes truncation,
each number and its scale indicator is converted
to machine units before any arithmetic is done,
so
.CW 1i/2u
evaluates to
0.5i
correctly.
.PP
The scale indicator
.CW u
often has to appear
when you wouldn't expect it \(em
in particular, when arithmetic is being done
in a context that implies horizontal or vertical dimensions.
For example,
.P1
^ll 7/2i
.P2
would seem obvious enough \(em
3\(12 inches.
Sorry.
Remember that the default units for horizontal parameters like
.CW .ll
are ems.
That's really `7 ems / 2 inches',
and when translated into machine units, it becomes zero.
How about
.P1
^ll 7i/2
.P2
Sorry, still no good \(em
the `2' is `2 ems', so `7i/2' is small,
although not zero.
You
.ul
must
use
.P1
^ll 7i/2u
.P2
So again, a safe rule is to
attach a scale indicator to every number,
even constants.
.PP
For arithmetic done within a
.CW .nr
command,
there is no implication of horizontal or vertical dimension,
so the default units are `units',
and
.CW 7i/2
and
.CW 7i/2u
mean the same thing.
Thus
.P1
^nr ll 7i/2
^ll \e\en(llu
.P2
does just what you want,
so long as you
don't forget the
.CW u
on the
.CW .ll
command.
.NH
Macros with arguments
.PP
The next step is to define macros that can change from one
use to the next
according to parameters supplied as arguments.
To make this work, we need two things.
First, when we define the macro, we have to indicate that some
parts of it will be provided as arguments when the macro is called.
Then when the macro is 
called
we have to provide actual arguments
to be plugged into the definition.
.PP
Let us illustrate by defining a macro
.CW .SM
that will print its argument two points
smaller than the surrounding text.
That is, the macro call
.P1
^SM UNIX
.P2
will produce
\s-2UNIX\s0
instead of UNIX.
.PP
The definition of
.CW .SM
is
.P1
^de SM
\es\-2\e\e$1\es+2
^^
.P2
Within a macro definition,
the symbol
.CW \e\e$n
refers to the
.CW n th
argument
that the macro was called with.
Thus
.CW \e\e$1
is the string to be placed in a smaller point
size when
.CW .SM
is called.
.PP
As a slightly more complicated version, the following definition of
.CW .SM
permits optional second and third arguments
that will be printed in the normal size:
.P1
^de SM
\e\e$3\es\-2\e\e$1\es+2\e\e$2
^^
.P2
Arguments not provided when the macro is called are treated
as empty,
so
.P1
^SM UNIX ),
.P2
produces
.UC UNIX ),
while
.P1
^SM UNIX ). (
.P2
produces
.UC UNIX ). (
It is convenient to reverse 
the order of arguments because trailing punctuation
is much more common than leading.
.PP
By the way, the number of arguments that a macro was called with
is available in number register
.CW .$ .
.PP
The following macro
.CW ^BD
is used to make
`bold roman',
.BD like
.BD this
.BD phrase.
It combines horizontal motions, width computations,
and argument rearrangement.
.P1 .05i
\&.de BD
\e&\e\e$3\ef1\e\e$1\eh'-\ew'\e\e$1'u+3u'\e\e$1\efP\e\e$2
\&..
.P2
The
.CW \eh
and
.CW \ew
commands need no extra backslash, as we discussed above.
The
.CW \e&
is there in case the argument begins with a period.
.SP
.PP
Two backslashes are needed with the
.CW \e\e$n
commands, though,
to protect one of them when the macro is
being defined.
Perhaps a second example will make this clearer.
Consider a macro called
.CW .NH
which
produces numbered headings rather like those in this paper,
with the sections numbered automatically,
and the title in bold in a smaller size.
The use is
.P1
^NH "Section title ..."
.P2
(If the argument to a macro is to contain blanks,
then it must be
.ul
surrounded
by double quotes,
unlike a string, where only one leading quote is permitted.)
.PP
Here is the definition of the
.CW .NH
macro:
.P1 .1i
.ta .75i 1.15i
^nr NH 0  \e" initialize section number
^de NH
^sp 0.3i
^ft B
^nr NH \e\en(NH+1	\e" increment number
^ps \e\en(PS\-1	\e" decrease PS
\e\en(NH.  \e\e$1	\e" number. title
^ps \e\en(PS		\e" restore PS
^sp 0.3i
^ft R
^^
.P2
The section number is kept in number register
.CW NH ,
which is incremented each
time just before it is used.
(A number register, but not a string, may have the same name as a macro
without conflict.)
.PP
We used
.CW \e\en(NH
instead of
.CW \en(NH
and
.CW \e\en(PS
instead of
.CW \en(PS .
If we had used
.CW \en(NH ,
we would get the value of the register at the time the macro was
.ul
defined,
not at the time it was
.ul
used.
If that's what you want, fine,
but not here.
Similarly,
by using
.CW \e\en(PS ,
we get the point size at the time the macro is called.
.SP
.PP
As an example that does not involve numbers,
recall our
.CW .NP
macro which had a
.P1
^tl 'left'center'right'
.P2
We could make these into parameters by using instead
.P1
^tl '\e\e*(LT'\e\e*(CT'\e\e*(RT'
.P2
so the title comes from three strings called
.CW LT ,
.CW CT
and 
.CW RT .
If these are empty, then the title will be a blank line.
Normally
.CW CT
would be set with something like
.P1
\&^ds CT - % -
.P2
to give just the page number between hyphens,
but a user could supply private definitions for
any of the strings.
.NH
Conditionals
.PP
Suppose we want the
.CW .NH
macro to leave two extra inches of space just before section 1,
but nowhere else.
The cleanest way to do that is to test inside the
.CW .NH
macro
whether
the section number is 1,
and add some space if it is.
The
.CW .if
command provides the conditional test
that we can add
just before the heading line is output:
.P1 0
 ^if \e\en(NH=1 ^sp 2i \e" 1st section only
.P2
.PP
The condition after the
.CW .if
can be any arithmetic or logical expression.
If the condition is logically true, or arithmetically greater than zero,
the rest of the line is treated as if
it were text \(em
here a command.
If the condition is false, or zero or negative,
the rest of the line is skipped.
.PP
It is possible to do more than one command if a condition is true.
Suppose several operations are to be done before section 1.
One possibility is to define a macro 
.CW .S1
and invoke it
if we are about to do section 1
(as determined by an
.CW .if ).
.P1
^de S1
---  processing for section 1 ---
^^
^de NH
^^^
^if \e\en(NH=1 ^S1
^^^
^^
.P2
.PP
An alternate way is to use the
extended form of the
.CW .if ,
like this:
.P1
^if \e\en(NH=1 \e{--- processing
for section 1 ----\e}
.P2
The braces
.CW \e{
and
.CW \e}
must occur in the positions shown
or you will get unexpected extra lines in your output.
.IT Troff
also provides
an `if-else' construction,
which we will not go into here.
.PP
A condition can be negated by preceding it with
.CW ! ;
we get the same effect as above (but less clearly) by using
.P1
^if !\e\en(NH>1 ^S1
.P2
.PP
There are a handful of 
other conditions that can be tested with
.CW .if .
For example, is the current page even or odd?
.P1
^if e ^tl ''even page title''
^if o ^tl ''odd page title''
.P2
gives facing pages different titles
when used inside an appropriate new page macro.
.PP
Two other conditions
are
.CW t
and
.CW n ,
which tell you whether the formatter is
.IT troff
or
.IT nroff . 
.P1
^if t troff stuff ...
^if n nroff stuff ...
.P2
.PP
Finally, string comparisons may be made in an
.CW .if :
.P1
^if 'string1'string2' stuff
.P2
does `stuff' if
.CW string1
is the same as
.CW string2 .
The character separating the strings can be anything
reasonable that is
not contained in either string.
The strings themselves can reference strings with
.CW \e* ,
arguments with 
.CW \e$ ,
and so on.
.NH
Environments
.PP
As we mentioned, there is a potential problem
when going across a page boundary:
parameters like size and font
for a page title may well be different from those
in effect in the text when the page boundary occurs.
.IT Troff
provides a very general way to deal with this and
similar situations.
There are three `environments',
each of which has independently settable versions of
many of the parameters associated with processing,
including size, font, line and title lengths,
fill/nofill mode, tab stops, and even partially collected lines.
Thus the titling problem may be readily solved by processing the main text
in one environment and titles in a separate one
with its own suitable parameters.
.PP
The command
.CW .ev\ n
shifts to environment
.CW n ;
.CW n
must be 0, 1 or 2.
The command
.CW .ev
with no argument returns to the
previous environment.
Environment names are maintained in a stack, so calls
for different environments may be nested and unwound consistently.
.PP
Suppose we say that the main text is processed in environment 0,
which is where 
.IT troff
begins by default.
Then we can modify the new page macro
.CW .NP
to process titles in environment 1 like this:
.P1 2
^de NP
^ev 1	\e" shift to new environment
^lt 6i	\e" set parameters here
^ft 1
^ps 10
\&... any other processing ...
^ev	\e" return to prev environment
^^
.P2
It is also possible to initialize the parameters for an environment
outside the
.CW .NP
macro,
but the version shown keeps all the processing in one place
and is thus easier to understand and change.
.NH
Diversions
.PP
There are numerous occasions in page layout when it is necessary to store some text
for a period of time without actually printing it.
Footnotes are one obvious example:
the text of the footnote usually appears in the input well before the place
on the page where it is to be printed is reached.
In fact,
the place where it is output normally depends on how big it is,
which implies that there must be a way
to process the footnote at least
enough to decide its size
without printing it.
.PP
.IT Troff
provides a mechanism called a diversion
for doing this processing.
Any part of the output may be diverted into a macro instead
of being printed,
and then at some convenient time the macro may be put back into
the input.
.PP
The command
.CW .di\ xy
begins a diversion \(em all subsequent output is collected into the macro
.CW xy
until the command
.CW .di
with no arguments is encountered.
This terminates the diversion.
The processed text is available at any time thereafter, simply
by giving the command
.P1
^xy
.P2
The vertical size of the last finished diversion is contained in
the built-in number register
.CW dn .
.PP
As a simple example,
suppose we want to implement a `keep-release'
operation,
so that text between the commands
.CW .KS 
and
.CW .KE
will not be split across a page boundary
(as for a figure or table).
Clearly, when a
.CW .KS
is encountered, we have to begin diverting
the output so we can find out how big it is.
Then when a
.CW .KE
is seen, we decide
whether the diverted text will fit on the current page,
and print it either there if it fits, or at the top of the next page if it doesn't.
So:
.P1 2
.ta .6i
^de KS	\e" start keep
^br	\e" start fresh line
^ev 1	\e" collect in new environment
^fi	\e" make it filled text
^di XX	\e" collect in XX
^^
.P2
.P1 2
.ta .6i
^de KE	\e" end keep
^br	\e" get last partial line
^di	\e" end diversion
^if \e\en(dn>=\e\en(.t .bp \e" bp if too big
^nf	\e" bring it back in no-fill
^XX	\e" text
^ev	\e" return to normal environment
^^
.P2
Recall that number register
.CW nl
is the current position
on the output page.
Since output was being diverted, this remains
at its value when the diversion started.
.CW dn
is the amount of text in the diversion;
.CW .t
(another built-in register)
is the distance to the next trap,
which we assume is at the bottom margin of the page.
If the diversion is large enough to go past the trap,
the
.CW .if
is satisfied, and
a
.CW .bp
is issued.
In either case, the diverted output is then brought back with
.CW .XX .
It is essential to bring it back in no-fill mode so
.IT troff
will do no further processing on it.
.PP
This is not the most general keep-release,
nor is it robust in the face of all conceivable inputs,
but it would require more space than we have here to write it
in full generality.
This section is not intended
to teach everything about diversions,
but to sketch out enough that you can read
existing macro packages with some comprehension.
.NH
Acknowledgements
.PP
I am deeply indebted to the late J. F. Ossanna,
the author of
.IT troff ,
for his repeated patient explanations
of
fine points,
and for his continuing willingness to
adapt
.IT troff
to make other uses easier.
.PP
I am also grateful to Jim Blinn, Ted Dolotta,
Doug McIlroy, Mike Lesk and Joel Sturman
for helpful comments on this paper.
.NH
References
.LP
|reference_placement
.sp 100
.BP
.1C
.........
.SH
Appendix A: Character Set
.SP .5
.PP
Different output devices support different character sets,
although there is considerable commonality.
To get the one on the left, type the
four-character name on the right.
.TS
center;
l l6fCW l l6fCW l l6fCW l l6fCW.
\(ff	\e(ff	\(fi	\e(fi	\(fl	\e(fl	\(Fi	\e(Fi	\(Fl	\e(Fl
\(ru	\e(ru	\(em	\e(em	\(14	\e(14	\(12	\e(12	\(34	\e(34
\(co	\e(co	\(de	\e(de	\(dg	\e(dg	\(fm	\e(fm	\(ct	\e(ct	
\(rg	\e(rg	\(bu	\e(bu	\(sq	\e(sq	\(hy	\e(hy
.TE
.PP
The following are special-font characters:
.TS
center;
l l6fCW l l6fCW l l6fCW l l6fCW.
\(pl	\e(pl	\(mi	\e(mi	\(mu	\e(mu	\(di	\e(di	
\(eq	\e(eq	\(==	\e(==	\(>=	\e(>=	\(<=	\e(<=	
\(!=	\e(!=	\(+-	\e(+-	\(no	\e(no	\(sl	\e(sl	
\(ap	\e(ap	\(~=	\e(~=	\(pt	\e(pt	\(gr	\e(gr	
\(->	\e(->	\(<-	\e(<-	\(ua	\e(ua	\(da	\e(da	
\(is	\e(is	\(pd	\e(pd	\(if	\e(if	\(sr	\e(sr	
\(sb	\e(sb	\(sp	\e(sp	\(cu	\e(cu	\(ca	\e(ca	
\(ib	\e(ib	\(ip	\e(ip	\(mo	\e(mo	\(es	\e(es	
\(aa	\e(aa	\(ga	\e(ga	\(ci	\e(ci	\(L1	\e(L1	
\(sc	\e(sc	\(dd	\e(dd	\(lh	\e(lh	\(rh	\e(rh	
\(lt	\e(lt	\(rt	\e(rt	\(lc	\e(lc	\(rc	\e(rc	
\(lb	\e(lb	\(rb	\e(rb	\(lf	\e(lf	\(rf	\e(rf	
\(lk	\e(lk	\(rk	\e(rk	\(bv	\e(bv	\(ts	\e(ts	
\(br	\e(br	\(or	\e(or	\(ul	\e(ul	\(rn	\e(rn	
\(**	\e(**
.TE
.PP
These
four
characters also have two-character names. 
The \' is the apostrophe on terminals;
the \` is the other quote mark.
.TS
center;
l l6fCW l l6fCW l l6fCW l l6fCW.
\'	\e\(aa	\`	\e\(ga	\(mi	\e\(mi	\(em	\e(em
.TE
.PP
For greek, precede the roman letter by
.CW \e(*
to get the corresponding greek;
for example, 
.CW \e(*a
is
\(*a.
.sp
.in 0.5i
.nf
.cs R 36
abgdezyhiklmncoprstufxqw
\(*a\(*b\(*g\(*d\(*e\(*z\(*y\(*h\(*i\(*k\(*l\(*m\(*n\(*c\(*o\(*p\(*r\(*s\(*t\(*u\(*f\(*x\(*q\(*w
.sp
ABGDEZYHIKLMNCOPRSTUFXQW
\(*A\(*B\(*G\(*D\(*E\(*Z\(*Y\(*H\(*I\(*K\(*L\(*M\(*N\(*C\(*O\(*P\(*R\(*S\(*T\(*U\(*F\(*X\(*Q\(*W
.cs R
.in 0
.fi
.BP
.2C
.SH
Appendix B: Some Common Fonts
.LP
The following fonts are currently available on our Laserwriter II printers.
The magic
.I troff
short name is given as well as the name in the font catalog.
.de XX
.KS
.SP .5
\f1\\$1 (\f(CW\\$2\f1)
.br
.nf
.ft \\$2
abcdefghijklmnopqrstuvwxyz 0123456789
ABCDEFGHIJKLMNOPQRSTUVWXYZ
.ft
.KE
..
.XX AvantGarde-BookOblique AI
.XX AvantGarde-Book AR
.XX AvantGarde-DemiOblique AX
.XX AvantGarde-Demi AB
.XX Bookman-DemiItalic KX
.XX Bookman-Demi KB
.XX Bookman-LightItalic KI
.XX Bookman-Light KR
.XX Courier-BoldOblique CX
.XX Courier-Bold CB
.XX Courier-Oblique CI
.XX Courier CW
.XX Helvetica-BoldOblique HX
.XX Helvetica-Bold HB
.XX Helvetica-Narrow-BoldOblique Hx
.XX Helvetica-Narrow-Bold Hb
.XX Helvetica-Narrow-Oblique Hi
.XX Helvetica-Narrow Hr
.XX Helvetica-Oblique HI
.XX Helvetica H
.XX NewCenturySchlbk-BoldItalic NX
.XX NewCenturySchlbk-Bold NB
.XX NewCenturySchlbk-Italic NI
.XX NewCenturySchlbk-Roman NR
.XX Palatino-BoldItalic PX
.XX Palatino-Bold PB
.XX Palatino-Italic PI
.XX Palatino-Roman PA
.XX Times-BoldItalic BI
.XX Times-Bold B
.XX Times-Italic I
.XX Times-Roman R
.XX ZapfChancery-MediumItalic ZI
.ig
show dingbats here
..

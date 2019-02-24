.so ../ADM/mac
.XX pm 21 "The \f(CW\-mpm\fP Macro Package"
.Tm typesetting	g
.TL
The \&\f(CB\-mpm\fP Macro Package
.AU
Brian W. Kernighan
Christopher J. Van Wyk
.AI
.MH
.AB
The
.CW -mpm
macro package is an alternative to the standard
.CW -ms
macros|reference(latest msmacros) that is designed to produce better page makeup than
.CW -ms 
does.
It includes most of the
.CW -ms
macros, while doing vertical justification and automatic figure placement,
and avoiding widows.
.AE
.2C
.NH
Executive Summary
.EQ
delim $$
.EN
.PP
Any document in generic
.CW -ms
format (not a
.CW .TM
or a
.CW .TR ,
for example) is formatted by
.P1
 ... | troff -ms
.P2
Just by changing this command (and not the document) to
.P1
 ... | troff -mpm
.P2
the
.CW -mpm
macro package, which defines most of the same macros as
.CW -ms ,
automatically invokes a
.I troff
postprocessor
.CW /usr/lib/tmac/pm ) (
that produces vertically justified pages
according to makeup rules that control figure placement
and prevent the creation of widow lines in text.
The general approach and algorithms of
.I pm
are described in |reference(page makeup).
.NH
Macros
.PP
The following macros are different from or not part of
.CW -ms .
Vertical distances denoted by
.I n
have default value
.CW 1v ,
i.e., one line.
.IP " \f(CW.SP\fP \f2n\fP"
.br
Add paddable space of height $n$.
The key word here is ``paddable'':
this space may expand or be removed to aid vertical justification.
Spaces produced by
.CW .sp
(instead of
.CW .SP )
may or may not appear in the output.
.IP " \f(CW.SP\fP \f2n\fP \f(CWexactly\fP"
.br
Add unpaddable space of height $n$.
This space cannot be removed.
.IP " \f(CW.KF\fP \f2m\fP"
.br
Text through a subsequent
.CW .KE
line defines a ``floating keep,''
whose center will be placed as close as possible to
vertical position $m$ on the page.
If $m$ is omitted,
the keep will appear at the top or bottom of a page.
The macro recognizes
.CW top
and
.CW bottom
as values for $m$.
.IP " \f(CW.FL\fP"
.br
Flush out all queued keeps before producing any more running text
(useful when a section or chapter ends).
.IP " \f(CW.BP\fP"
.br
Force a page break.
The
.I troff
command
.CW .bp
will not cause a page break.
.IP " \f(CW.NE\fP \f2n\fP"
.br
Start a new page if a chunk of output $n$ high would not fit here on the page.
.CW .NE
does not cause a break.
.IP " \f(CW.P1\fP, \f(CW.P2\fP, \f(CW.P3\fP"
.br
.CW .P1
begins a program display (Courier font);
.CW .P2
ends it;
.CW .P3
inserts an optional break point in a program display.
.IP " \f(CW.Tm\fP \f2any text\fP"
.br
On the standard error output, print
the current page number, a tab, and
.I "any text" .
The spelling is unusual because 
.CW -ms
pre-empted
.CW .TM .
.IP " \f(CW.X\fP \f2any text\fP"
.br
Present
.I "any text
to the page-makeup program
as part of the sequence
.CW x
.CW X
.I "any text
(an output-device-dependent line in
.I troff
output).
This is equivalent to the
.I troff
command
.CW \eX'\f2any\ text\fP' .
.NH
Two-Column Operation
.PP
The page-makeup program maintains two ``regions''
on each page of output\(emone for single-column text
that is the full width of the page,
and one for two-column text that is about half the width of the page.
This convention makes it easy to insert single-column
figures into two-column text.
If one wishes to alternate single- and two-column output on a page
(as some journals alternate two-column text with single-column equations),
it is possible to ``freeze'' the contents of the current two-column region,
add the result to the single-column region,
and start with a fresh two-column region.
.PP
The following commands are related to two-column operation:
.IP " \f(CW.1C\fP
.br
Subsequent text up to the next
.CW .2C
command should be added to the single-column region.
.IP " \f(CW.2C\fP
.br
Subsequent text up to the next
.CW .1C
command should be added to the two-column region.
.LP
Note that it is the user's responsibility to label text with
.CW .1C
or
.CW .2C ;
the page-makeup program does not check whether the text widths make sense.
.IP "  \f(CW.FC\fP"
.br
Freeze the current two-column region and start a new (empty) one.
.NH
Some Useful Number Registers and Strings
.PP
Table 1 shows some number registers that users may wish to set.
The value of register
.CW \en(PD
should be at least 1u to make vertical justification possible.
The value of register
.CW \en(TS
should be zero for side-by-side tables.
.1C
.KF
.nr PS 9
.nr VS 11
.ps 9
.vs 11
.TS
center, box;
cfI lfI lfI lfI
afCW a lfCWw(2i) a.
Register	Default	Used By	Purpose
=
\f(CWPS\f1	10p		point size
\f(CWVS\f1	12p		vertical spacing
\f(CWLL\f1	6i		line length
\f(CWLT\f1	\f(CW\en(LL\f1		title length
\f(CWFL\f1	\f(CW\en(LL\f1		footnote length
\f(CWCW\f1	7\(mu\f(CW\en(LL\f1/15		column width for two-column output
\f(CWGW\f1	\f(CW\en(LL\f1/15		gutter width for two-column output
_
HM	1i	.PT	header margin
FM	1i	.BT	footer margin
FO	10i		vertical position of bottom margin
_
PO	1i		page offset from left margin
PI	5n	.PP	paragraph indent
QI	5n	.QP,.QS/.QE	quotation indent
_
PD	.3v	.PP,.LP,.IP,.QP,.QS/.QE	inter-paragraph vertical space
TS	.5v	.TS/.TE	vertical space around tables
Kf	.5v	.KF/.KE	vertical space around fixed keeps
Ks	.5v	.DS/.DE,.IS/.IE,.KS/.KE	vertical space around floating keeps
_
dP	1p	.P1/.P2	point-size decrease in programs
dT	8	.P1/.P2	spaces per tab stop in programs
dV	2p	.P1/.P2	vertical-spacing decrease in programs
DV	.5v	.P1/.P2	vertical space around program displays
.TE
.ce 1
.B "Table 1
.nr PS 10
.nr VS 12
.ps 10
.vs 12
.KE
.2C
.PP
The vertical spacings shown in Table 1 are all paddable.
The macros shown in Table 2 produce vertical spacing that
cannot be adjusted by the user.
.KF bottom
.nr PS 9
.nr VS 11
.ps 9
.vs 11
.TS
center, box;
lfI lfI
afCW a.
Macro	Vertical Spacing
=
\&.EQ	.5v
\&.EN	.5v
\&.NH	1v
\&.PS	.35v
\&.PE	.65v
\&.SH	1v
.TE
.ce 1
.B "Table 2
.nr PS 10
.nr VS 12
.ps 10
.vs 12
.KE
.PP
Table 3 shows strings used to produce page headers.
The value of
.CW \en(%#
at most points in the input is unrelated to the number of the page
on which that point will appear in the output.
.1C
.KF
.nr PS 9
.nr VS 11
.ps 9
.vs 11
.TS
center, box;
cfI cfI lfI
afCW lfCW a.
String	Used by	Purpose
=
%#	.PT\fR,\fP .BT	page number
%e	.PT	header for even-numbered pages
%o	.PT	header for odd-numbered pages
%E	.BT	footer for even-numbered pages
%O	.BT	footer for odd-numbered pages
.TE
.ce 1
.B "Table 3
.nr PS 10
.nr VS 12
.ps 10
.vs 12
.KE
.2C
.NH
Features
.PP
Warnings appear as comments in the standard output,
preceded by
.CW "MESSAGE TO USER:"  '' ``
Common warnings concern pages that were not justified
because they were too empty and
keeps that had to be split because they were too large.
Users can set the
.CW WARN
parameter to see warnings on the standard error output;
see Section 6.
.PP
The
.CW -mpm
macro package does not support completely the rich variety of document types
(TM, CSTR, etc.,) that
.CW -ms
does, and does not provide obsolete styles like Engineer's Notes at all.
.PP
The interaction between
.I troff
and the page-makeup program produces strange results when
.I troff
input spaces up the page past paddable space.
(See the comment about
.CW \en(TS
in Section 4.)
.CW -mpm
issues a warning about ``an embedded SP.''
.PP
More generally, the ``flyback'' commands in
.I pic
and
.I ideal
.CW .PF "" (
and
.CW .IF ,
respectively) do not work with
.CW -mpm .
One can usually place narrow figures beside narrow text or other narrow figures
by setting the pieces in two columns and using the
.CW .FC
command.
.PP
The
.CW .FS
macro in
.CW -mpm
does not provide a line to separate footnotes from running text.
There are two reasons:
(1) if multiple footnotes appeared on a page,
each would have a separating line;
(2) if a footnote split across two pages,
no separator line would be provided
for the second part of the footnote.
Users can add footnote separating lines explicitly by calling the
.CW .FA
macro.
.PP 
The implementation of multiple columns is incomplete:
.CW -mpm
provides only
.CW .2C .
.PP
Users who find that the page-makeup program stretches
their pages too much can experiment with adjusting the
minimum fullness and column tolerance parameters
described in Section 6.
.PP
Headings are a bit tricky if you want things like
running titles that include the current section title.
Normally you will need a two-pass procedure using 
.CW .Tm
output.
On the bright side,
this procedure will produce correct headings,
where the standard one-pass procedure of resetting headers
in the
.CW .SH
macro can produce incorrect headers on odd-numbered pages.
.NH
Rolling Your Own
.PP
If you want to create your own macros that control
.I pm ,
Table 4 shows the components of
.I pm 's
``command language.''  They are inserted into
the
.I troff
output in the form of
.CW "x X ..." '' ``
commands, created either by
.CW \eX'...'
or by the
.CW .X
macro in
.CW -mpm .
.CW /usr/lib/tmac/tmac.pm
contains examples of use.
.1C
.KF
.nr PS 9
.nr VS 11
.ps 9
.vs 11
.TS
center, box;
lfI lfI lfI lfI
lfCW l lw(2.75i) lfCWw(.55i).
Command	Name	Description	Used by
=
SP $n$	space	T{
Paddable space of $n$
T}	.SP
BS $n$	breakable stream	T{
$n ~=$ minimum number of lines from the stream that must appear on page
T}	T{
\&.PP,.LP,
\&.IP,.QP
T}
US	unbreakable stream	T{
An \f(CWEND\fP-terminated item that cannot float or be broken
T}	T{
\&.DS,\&.EQ,\&.IS,
\&.KS,\&.TS,\&.PS
T}
BF $v$	breakable float	T{
An \f(CWEND\fP-terminated item that can float or be broken;
$v$ = preferred vertical location of box center;
two successive commands give two preferences
T}	.FS
UF $v$	unbreakable float	T{
An \f(CWEND\fP-terminated item that can float but cannot be broken;
$v$ = preferred vertical location of box center;
two successive commands give two preferences
T}	.KF
PT	page title	T{
User has absolute control between PT and END;
no SP's or other
.I pm
commands inside are processed.
T}	.PT
BT	bottom title	T{
User has absolute control between BT and END, as for PT.
T}	.BT
END	end	T{
End a US, BF, UF, PT, or BT.
All constructs nest, but a float within another float
or US will not float within or outside the block.
T}	T{
\&.DE,\&.EN,\&.FE,
\&.IE,\&.KE,\&.TE,
\&.PE
T}
TM $str$	message	T{
Print the page number, a tab, and $str$ on standard error output.
T}	.Tm
COORD $str$	output coordinates	T{
Print $str$ with current page number and output $x$ and $y$ coordinates.
T}
NE $n$	need	T{
Break page if a VBOX of height $n$ would not fit on page
T}	.NE,.SH,.NH
MC $n$ $o$	multiple column	T{
$n$ columns, $i$th offset from first by $o(i-1)$.
Only 1 and 2 columns work.
T}	.1C,.2C
CMD FC	freeze columns	T{
Force out current two-column contents;
start a fresh one.
T}	.FC
CMD FL	flush	T{
Force all queued figures out before any more
stream material is output.
T}	.FL
CMD BP	break page	T{
Force page break.
T}	.BP
PARM NP $v$	new page	T{
Top of
.I pm
text at vertical position $v$.
T}	
PARM FO $v$	footer	T{
Bottom of
.I pm
text at vertical position $v$;
length of text on page = FO\ \-\ NP.
T}
PARM PL $v$	page length	T{
Physical page ends at $v$;
default = FO + NP.
T}
PARM MF $x$ 	minimum fullness	T{
Tolerance to prevent padding;
default = 90.
A page that is less than $x$% full will not be padded.
T}
PARM CT $x$	column tolerance	T{
Tolerance for two-column operation;
default = 50.
Values larger than 50% will generally cause more text
to be included in the first column.
T}
PARM WARN $x$	warning flag	T{
Print warnings on standard error output if $x != 0$.
T}
.TE
.ce 1
.B "Table 4
.nr PS 10
.nr VS 12
.ps 10
.vs 12
.KE
.2C
.NH
References
.LP
|reference_placement
.ig
``Page Makeup by Postprocessing Text Formatter Output,''
Brian W. Kernighan and Christopher J. Van Wyk,
.I "Computing Systems,
$bold "2"$, 2, Spring 1989, pp 103-132.
..

.so ../ADM/mac
.XX sed 389 "Sed \(em A Non-interactive Text Editor"
.hw de-limit
.hw de-limit-ing
.de P3
.ft CW
.ps 8
..
.de P4
.ft
.ps 10
..
.ND August 15, 1978
.TL
Sed \(em A Non-interactive Text Editor\(dg
.AU "MH 2C-555" 3302
Lee E. McMahon
.AI
.MH
.AB
.PP
.I Sed
is a non-interactive context editor
that runs on the
.UX
operating system.
.I Sed
is
designed to be especially useful for
editing files too large for comfortable
interactive editing,
editing any size file when the sequence
of editing commands is too complicated to be comfortably
typed in interactive mode,
performing multiple `global' editing functions
efficiently in one pass through the input, and
providing editing `glue' between programs in a pipeline.
.PP
This document describes
.I sed 's
commands in detail and illustrates their use.
.AE
.2C
.FS
\(dg This is a version of |reference(sed v7man) heavily revised by L. L. Cherry.
.FE
.SH
Introduction
.PP
.I Sed
is a non-interactive context editor designed to be especially useful in
four cases:
.IP 1) 3
To edit files too large for comfortable
interactive editing;
.IP 2)
To edit any size file when the sequence
of editing commands is too complicated to be comfortably
typed in interactive mode;
.IP 3)
To perform multiple `global' editing functions
efficiently in one pass through the input.
.IP 4)
To provide editing `glue' between programs in a pipeline.
.PP
Since only a few lines of the input reside in core
at one time, and no temporary files are used,
the effective size of file that can be edited is limited only
by the requirement that the input and output fit simultaneously
into available secondary storage.
.PP
Complicated editing scripts can be created separately and given
to 
.I sed
as a command file.
For complex edits, this saves considerable typing, and its
attendant errors.
.I Sed
running from a command file is much more efficient than any interactive
editor known to the author, even if that editor
can be driven by a pre-written script.
.PP
The principal defects compared to an interactive editor
are lack of relative addressing (because of the line-at-a-time
operation), and lack of immediate verification that a command has
done what was intended.
.PP
.I Sed
is a lineal descendant of the
.UX
editor,
.I ed .
Because of the differences between interactive and non-interactive
operation, considerable changes have been made between
.I ed
and
.I sed ;
even confirmed users of
.I ed
will frequently be surprised (and probably chagrined),
if they rashly use 
.I sed
without reading Sections 2 and 3 of this document.
The most striking family resemblance between the two
editors is in the class of patterns (`regular expressions') they
recognize;
the code for matching patterns is copied almost
verbatim from the code for
.I ed ,
and the description of regular expressions in Section 2
is copied almost verbatim from the
.UX
Programmer's
Manual|reference(v7manual).
(Both code and description were written by Dennis M. Ritchie.)
.SH
1. Overall Operation
.PP
.I Sed
by default copies the standard input to the standard output,
perhaps performing one or more editing commands on each
line before writing it to the output.
This behavior may be modified by flags on the command line;
see Section 1.1 below.
.PP
The general format of an editing command is:
.ce
[\fIaddress1\fR, \fIaddress2\fR][\fIfunction\fR][\fIarguments\fR]
.PP
One or both addresses may be omitted; the format of addresses is
given in Section 2.
Any number of spaces or tabs may separate the addresses
from the function.
The function must be present; the available commands are discussed
in Section 3.
The arguments may be required or optional, according to which function
is given; again, they are discussed in Section 3 under each individual
function.
.PP
Tab characters and spaces at the beginning of lines are ignored.
.SH
1.1. Command-line Flags
.PP
Three flags are recognized on the command line:
.IP \f(CW-n\fR 5
tells
.I sed
not to copy all lines, but only those specified by
.CW p
functions or
.CW p
flags after 
.CW s
functions (see Section 3.3);
.IP \f(CW-e\fR
tells
.I sed
to take the next argument as an editing command;
.IP \f(CW-f\fR
tells
.I sed
to take the next argument as a file name;
the file should contain editing commands, one to a line.
.SH
1.2. Order of Application of Editing Commands
.PP
Before any editing is done (in fact, before any input file is
even opened), all the editing commands are compiled into
a form which will be moderately efficient during
the execution phase (when the commands are actually applied to
lines of the input file).
The commands are compiled in the order in which they are
encountered; this is generally the order in which they will
be attempted at execution time.
The commands are applied one at a time; the input to each command
is the output of all preceding commands.
.PP
The default linear order of application of editing commands can
be changed by the flow-of-control commands,
.CW t
and
.CW b
(see Section 3).
Even when the order of application is changed
by these commands, it is still true that the input line to any
command is the output of any previously applied command.
.SH
1.3.  Pattern-space
.PP
The range of pattern matches is called the pattern space.
Ordinarily, the pattern space is one line of the input text,
but more than one line can be read into the pattern space
by using the
.CW N
command (Section 3.6.).
.SH
1.4. Some Useful Examples
.LP
.CW "sed '10q' file"
.in 5
prints first 10 lines
.in 0
.CW "sed '/^$/d' file"
.ti 5
deletes empty lines
.br
.CW "sed -n '/,/p' file"
.in 5
prints all lines containing \f(CW,\fR
.in 0
.CW "sed 's/UNIX/& system/g'"
.in 5
substitutes `UNIX system' for all occurrences of UNIX
.in 0
.CW "sed 's/ /\e"
.br
.CW "/g' file"
.in 5
prints file 1 word per line
.in 0
.CW "sed '/./s/^/    /' file"
.in 5
indents nonempty lines 1 tab
.in 0
.SH
2. Addresses: Selecting lines for editing
.PP
Lines in the input file(s) to which editing commands are
to be applied can be selected by addresses.
Addresses may be either line numbers or context addresses.
.PP
The application of a group of commands can be controlled by
one address (or address-pair) by grouping
the commands with curly braces
.CW {}
(Sec. 3.6.).
.SH
2.1. Line-number Addresses
.PP
A line number is a decimal integer.
As each line is read from the input, a line-number counter
is incremented;
a line-number address matches (selects) the input
line which causes the internal counter to equal the
address line-number.
The counter runs cumulatively through multiple input files;
it is not reset when a new input file is opened.
.PP
As a special case, the character
.CW $
matches the last line of the last input file.
.SH
2.2. Context Addresses
.PP
A context address is a pattern (`regular expression') enclosed in slashes
.CW / ). (
The regular expressions recognized by
.I sed
are constructed as follows:
.IP 1) 3
An ordinary character (not one of those discussed below)
is a regular expression, and matches that character.
.IP 2)
A circumflex
.CW ^
at the beginning of a regular expression
matches the null character at the beginning of a line.
.IP 3)
A dollar-sign
.CW $
at the end of a regular expression
matches the null character at the end of a line.
.IP 4)
The characters
.CW \en
match an embedded newline character,
but not the newline at the end of the pattern space.
.IP 5)
A period
.CW .
matches any character except the terminal newline
of the pattern space.
.IP 6)
A regular expression followed by an asterisk
.CW *
matches any
number (including 0) of adjacent occurrences of the regular
expression it follows.
.IP 7)
A string of characters in square brackets
.CW []
matches any character
in the string, and no others.
If, however, the first character of the string is circumflex
.CW ^ ,
the regular expression matches any character
.I except
the characters in the string and the terminal newline of the pattern space.
.IP 8)
A concatenation of regular expressions is a regular expression
which matches the concatenation of strings matched by the
components of the regular expression.
.IP 9)
A regular expression between the sequences
.CW \e(
and
.CW \e)
is
identical in effect to the unadorned regular expression, but has
side-effects which are described under the
.CW s
command below and specification 10) immediately below.
.IP 10)
The expression
.CW \e\fId
means the same string of characters matched
by an expression enclosed in
.CW \e(
and
.CW \e)
earlier in the same pattern.
Here
.I d
is a single digit;
the string specified is that beginning with the
\fId\|\fRth
occurrence of
.CW \e(
counting from the left.
For example, the expression
.P1
^\e(..*\e)\e1
.P2
matches a line beginning with
two repeated occurrences of the same string.
.IP 11)
The null regular expression standing alone (e.g.,
.CW // )
is
equivalent to the  last regular expression compiled.
.PP
To use one of the special characters
.CW "^$.*[]\e/"
as a literal
(to match an occurrence of itself in the input), precede the
special character by a backslash
.CW \e .
.PP
For a context address to `match' the input requires that
the whole pattern within the address match some
portion of the pattern space.
.SH
2.3. Number of Addresses
.PP
The commands in the next section can have 0, 1, or 2 addresses.
Under each command the maximum number of allowed addresses is
given.
For a command to have more addresses than the maximum allowed
is considered an error.
.PP
If a command has no addresses, it is applied to every line
in the input.
.PP
If a command has one address, it is applied to all
lines which match that address.
.PP
If a command has two addresses, it is applied to the first
line which matches the first address, and to all subsequent lines
until (and including) the first subsequent line which matches
the second address.
Then an attempt is made on subsequent lines to again match the first
address, and the process is repeated.
.PP
Two addresses are separated by a comma.
.SH
Input for Examples:
.PP
Many examples follow in the text.
Except where otherwise noted,
the examples all assume the following input stored in a
file named
.CW countries :
.P3
.TS
l1l1l1l1l1.
USSR	8649	287	$3,000	Asia
Canada	3852	25.3	$14,000	North America
China	3705	1069.6	$258	Asia
USA	3615	247.5	$13,451	North America
India	1267	833.4	$150	Asia
France	211	55.8	$13,046	Europe
.TE
.P4
The entries are separated by a single tab character.
.SH
Example Addresses:
.LP
.TS
center;
cc
lFCW l.
Pattern	Matches
/US/	lines 1 and 4
/C.*A/	line 2 and 3
/^F/	the last line
/a$/	all but the last line
/./	all lines
/\e./	all but line 1
/a*na/	lines 2 (C\f(CWana\fRda) and 3 (Chi\f(CWna\fR)
/^[A-G]/	lines 2, 3, and 6
/^[^A-G]/	lines 1, 4, and 5
/\e(ia\e).*\e1/	line 5
.TE
.SH
3. Functions
.PP
All functions are named by a single character.
In the following summary, the maximum number of allowable addresses
is given enclosed in parentheses, then the single character
function name, possible arguments enclosed in angle brackets
.CW <> ,
an expanded English translation of the single-character name,
and finally a description of what each function does.
The angle brackets around the arguments are
.I not
part of the argument, and should not be typed
in actual editing commands.
.de XX
.sp .5
..
.SH
3.1. Whole-line Oriented Functions
.IP "(2)\f(CWd\fR \(em delete lines
.XX
The
.CW d
function deletes from the file (does not write to the output)
all those lines matched by its address(es).
.XX
It also has the side effect that no further commands are attempted
on the corpse of a deleted line;
as soon as the
.CW d
function is executed, a new line is read from the input, and
the list of editing commands is re-started from the beginning
on the new line.
.IP "(2)\f(CWn\fR \(em next line
.XX
The
.CW n
function reads the next line from the input, replacing
the current line.
The current line is first written to the output if it should
be.
The list of editing commands is continued 
following the 
.CW n
command.
.IP "(1)\f(CWa\e
.IP "\fI<text>\fR \(em append lines
.XX
The
.CW a
function causes the argument
.I <text>
to be written to the
output after the line matched by its address.
The
.CW a
command is inherently multi-line;
.CW a
must appear at the end of a line, and
.I <text>
may contain
any number of lines.
To preserve the one-command-to-a-line fiction,
the interior newlines must be hidden by a
backslash character
.CW \e
immediately preceding the
newline.
The
.I <text>
argument is terminated by the first unhidden
newline (the first one not immediately preceded
by backslash).
.XX
Once an
.CW a
function is successfully executed,
.I <text>
will be
written to the output regardless of what later commands do to
the line which triggered it.
The triggering line may be 
deleted entirely;
.I <text>
will still be written to the output.
.XX
The
.I <text>
is not scanned for address matches, and no editing
commands are attempted on it.
It does not cause any change in the line-number counter.
.IP "(1)\f(CWi\e
.IP "\fI<text>\fR \(em insert lines
.XX
The
.CW i
function  behaves identically to the
.CW a
function, except that
.I <text>
is written to the output
.I before
the matched line.
All other comments about the
.CW a
function apply to the
.CW i
function as well.
.IP "(2)\f(CWc\e
.IP "\fI<text>\fR \(em change lines
.XX
The
.CW c
function deletes the lines selected by its address(es),
and replaces them with the lines in
.I <text> .
Like
.CW a
and
.CW i ,
.CW c
must be followed by a newline hidden by a backslash;
and interior new lines in
.I <text>
must be hidden by
backslashes.
.XX
The
.CW c
command may have two addresses, and therefore select a range
of lines.
If it does, all the lines in the range are deleted, but only
one copy of <text> is written to the output,
.I not
one copy per line deleted.
As with
.CW a
and
.CW i ,
.I <text>
is not scanned for address matches, and no
editing commands are attempted on it.
It does not change the  line-number counter.
.XX
After a line has been deleted by a
.CW c
function, no further commands are attempted on the corpse.
.XX
If text is appended after a line by
.CW a
or
.CW r
functions, and the line is subsequently changed, the text
inserted by the
.CW c
function will be placed
.I before
the text of the
.CW a
or
.CW r
functions.
(The
.CW r
function is described in Section 3.4.)
.LP
.B Note :
Within the text put in the output by these functions,
leading blanks and tabs will disappear, as always in 
.I sed
commands.
To get leading blanks and tabs into the output, precede the first
desired blank or tab by a backslash; the backslash will not
appear in the output.
.SH
Examples:
.PP
The list of editing commands:
.P1
1i\e
\&.TS\e
lnnnl.
$a\e
\&.TE
.P2
applied to our standard input, produces:
.P3
.TS
l1l1l1l1l1.
\&.TS
lnnnl.
USSR	8649	287	$3,000	Asia
Canada	3852	25.3	$14,000	North America
China	3705	1069.6	$258	Asia
USA	3615	247.5	$13,451	North America
India	1267	833.4	$150	Asia
France	211	55.8	$13,046	Europe
\&.TE
.TE
.P4
Any of the following command lists:
.TS
center;
lFCWw(.5i) lFCWw(.5i) lFCW.
n	n	n
a\e	i\e	c\e
XX	XX	XX
d	d
.TE
produce:
.P3
.TS
l1l1l1l1l1.
USSR	8649	287	$3,000	Asia
XX
China	3705	1069.6	$258	Asia
XX
India	1267	833.4	$150	Asia
XX
.TE
.P4
The following commands:
.P1
1,2c\e
2 gone
5,6c\e
2 gone
.P2
produce:
.P3
.TS
l1l1l1l1l1.
2 gone
China	3705	1069.6	$258	Asia
USA	3615	247.5	$13,451	North America
2 gone
.TE
.P4
.SH
3.2. Substitute Function
.PP
One very important function changes parts of lines selected by
a context search within the line.
.IP "(2)\f(CWs\fI<pattern><replacement><flags>\fR \(em
.IP "     substitute
.XX
The
.CW s
function replaces
.I part
of a line (selected by
.I <pattern> )
with
.I <replacement> .
It can best be read as:
.XX
Substitute for
.I <pattern>,
.I <replacement>
.XX
The
.I <pattern>
argument contains a pattern,
exactly like the patterns in addresses (see 2.2 above).
The only difference between
.I <pattern>
and a context address is
that the context address must be delimited by
.CW / s;
.I <pattern>
may be delimited by any character other than space or
newline.
.XX
By default, only the first string matched by
.I <pattern>
is replaced,
but see the
.CW g
flag below.
.XX
The
.I <replacement>
argument begins immediately after the
second delimiting character of
.I <pattern> ,
and must be followed
immediately by another instance of the delimiting character.
(Thus there are exactly 
.I three
instances of the delimiting character.)
.XX
The
.I <replacement>
is not a pattern,
and the characters which are special in patterns
do not have special meaning in
.I <replacement> .
Instead, other characters are special:
.RS
.IP \f(CW&\fP
is replaced by the string matched by
.I <pattern>
.IP \f(CW\e\fId\fR
(where
.I d
is a single digit) is replaced by the \fId\fRth substring
matched by parts of
.I <pattern>
enclosed in
.CW \e(
and
.CW \e) .
If nested substrings occur in
.I <pattern> ,
the \fId\fRth
is determined by counting the opening delimiters
.CW \e( .
.XX
As in patterns, special characters may be made
literal by preceding them with backslash
.CW \e .
.RE
The
.I <flags>
argument may contain the following flags:
.RS
.IP \f(CWg\fP
\(em substitute
.I <replacement>
for all (non-overlapping)
instances of
.I <pattern>
in the line.
After a successful substitution, the scan for the next
instance of
.I <pattern>
begins just after the end of the
inserted characters; characters put into the line from
.I <replacement>
are not rescanned.
.IP \f(CWp\fP
\(em print the line if a successful replacement was done.
The
.CW p
flag causes the line to be written to the output if and only
if a substitution was actually made by the
.CW s
function.
Notice that if several
.CW s
functions, each followed by a
.CW p
flag, successfully substitute in the same input line,
multiple copies of the line will be written to the
output: one for each successful substitution.
.IP "\f(CWw\fI <filename>\fR\ 
\(em write the line to a file if a successful
replacement was done.
The
.CW w
flag causes lines which are actually substituted by the
.CW s
function to be written to a file named by
.I <filename> .
If
.I <filename>
exists before
.I sed
is run, it is overwritten;
if not, it is created.
.XX
A single space must separate
.CW w
and
.I <filename> .
.XX
The possibilities of multiple, somewhat different copies of
one input line being written are the same as for 
.CW p .
.XX
The maximum number of different file names that may be mentioned after
.CW w
flags and
.CW w
functions depends on the number of open files allowed by the system.
.RE
.SH
Examples:
.PP
The following command, applied to our standard input,
.P1
s/,[0-9]*/K/w wealthy
.P2
produces, on the standard output:
.P3
.TS
l1l1l1l1l1.
USSR	8649	287	$3K	Asia
Canada	3852	25.3	$14K	North America
China	3705	1069.6	$258	Asia
USA	3615	247.5	$13K	North America
India	1267	833.4	$150	Asia
France	211	55.8	$13K	Europe
.TE
.P4
and, on the file
.CW wealthy :
.P3
.TS
l1l1l1l1l1.
USSR	8649	287	$3K	Asia
Canada	3852	25.3	$14K	North America
USA	3615	247.5	$13K	North America
France	211	55.8	$13K	Europe
.TE
.P4
The command:
.P1 0
sed -n '1s/[A-Z]*/\e\efB&\e\efP/p' countries
.P2
produces:
.P3
.TS
l1l1l1l1l1.
\efBUSSR\efP	8649	287	$3,000	Asia
.TE
.P4
While
.P1 0
sed -n '2s/\e([A-Za-z]*\e)  \e(.*\e)/\e2, \e1/p'\e
  countries
.P2
produces:
.P3
.TS
l1l1l1l1l1.
3852	25.3	$14,000	North America, Canada
.TE
.P4
Finally, to illustrate the effect of the
.CW g
flag,
the command:
.P1
sed -n '1s.	./.p' countries
.P2
produces:
.P3
.TS
l1l1l1l1l1.
USSR/8649	287	$3,000	Asia
.TE
.P4
but the command:
.P1
sed -n '1s.	./.gp' countries
.P2
produces:
.P1
USSR/8649/287/$3,000/Asia
.P2
The pattern in both of the above is the tab character.
.SH
3.3. Input-output Functions
.IP "(2)\f(CWp \(em\fR print
.XX
The print function writes the addressed lines to the standard output file.
They are written at the time the
.CW p
function is encountered, regardless of what succeeding
editing commands may do to the lines.
.IP "(2)\f(CWw\fI <filename>\fR \(em write to a file
.XX
The write function writes the addressed lines to the file named
by
.I <filename> .
If the file previously existed, it is overwritten; if not, it is created.
The lines are written exactly as they exist when the write function
is encountered for each line, regardless of what subsequent
editing commands may do to them.
.XX
Exactly one space must separate the
.CW w
and
.I <filename> .
.XX
The maximum number of different files that may be mentioned in write
functions and
.I w
flags after
.CW s
functions combined depends on the system.
.XX
.IP "(1)\f(CWr\fI <filename>\fR \(em read the contents of a file
.XX
The read function reads the contents of
.I <filename> ,
and appends
them after the line matched by the address.
The file is read and appended regardless of what subsequent
editing commands do to the line which matched its address.
If
.CW r
and
.CW a
functions are executed on the same line,
the text from the 
.CW a
functions and the
.CW r
functions is written to the output in the order that
the functions are executed.
.XX
Exactly one space must separate the
.CW r
and
.I <filename> .
If a file mentioned by a
.CW r
function cannot be opened, it is considered a null file,
not an error, and no diagnostic is given.
.XX
.PP
.I Note :
Since there is a limit to the number of files that can be opened
simultaneously, care should be taken not to exceed that system-dependent number
in
.CW w
functions or flags; that number is reduced by one if any
.CW r
functions are present.
(Only one read file is open at one time.)
.SH
Example
.PP
Assume that the file
.CW more
contains:
.P3
.TS
l1l1l1l1l.
Brazil	3286	153.9	$1,523	South America
Australia	2966	16.0	$10,282	Australia
.TE
.P4
Then the following command:
.P1
/India/r more
.P2
produces:
.P3
.TS
l1l1l1l1l1.
USSR	8649	287	$3,000	Asia
Canada	3852	25.3	$14,000	North America
China	3705	1069.6	$258	Asia
USA	3615	247.5	$13,451	North America
India	1267	833.4	$150	Asia
Brazil	3286	153.9	$1,523	South America
Australia	2966	16.0	$10,282	Australia
France	211	55.8	$13,046	Europe
.TE
.P4
The command:
.P1
/USSR/,/USA/w big
.P2
writes the first 4 lines to file
.CW big .
.SH
3.4.
Multiple Input-line Functions
.PP
Three functions, all spelled with capital letters, deal
specially with pattern spaces containing embedded newlines;
they are intended principally to provide pattern matches across
lines in the input.
.IP "(2)\f(CWN\fR \(em Next line
.XX
The next input line is appended to the current line in the
pattern space; the two input lines are separated by an embedded
newline.
Pattern matches may extend across the embedded newline(s).
.XX
.IP "(2)\f(CWD\fR \(em Delete first part of the pattern space
.XX
Delete up to and including the first newline character
in the current pattern space.
If the pattern space becomes empty (the only newline
was the terminal newline),
read another line from the input.
In any case, begin the list of editing commands again
from its beginning.
.XX
.IP "(2)\f(CWP\fR \(em Print first part of the pattern space
.XX
Print up to and including the first newline in the pattern space.
.XX
The 
.CW P
and
.CW D
functions are equivalent to their lower-case counterparts
if there are no embedded newlines in the pattern space.
.SH
Example
.PP
If the input file, called
.CW countries2 ,
contains:
.P1
USSR	8649
USSR	287
USSR	$3,000
USSR	Asia
Canada	3852
Canada	25.3
Canada	$14,000
Canada	North America
China	3705
China	1069.6
China	$258
China	Asia
.P2
then the commands
.P1 0
N
/\e([A-Za-z]*\e)\e(.*\e)\en\e1\e(.*\e)/s//\e1\e2\e3/
.P2
produce
.P1
USSR	8649	287
USSR	$3,000	Asia
Canada	3852	25.3
Canada	$14,000	North America
China	3705	1069.6
China	$258	Asia
.P2
.SH
3.5.  Hold and Get Functions
.PP
Four functions save and retrieve part of the input for possible later
use.
.XX
.IP "(2)\f(CWh\fR \(em hold pattern space
.XX
The
.CW h
functions copies the contents of the pattern space
into a hold area (destroying the previous contents of the
hold area).
.XX
.IP "(2)\f(CWH\fR \(em Hold pattern space
.XX
The
.CW H
function appends the contents of the pattern space
to the contents of the hold area; the former and new contents
are separated by a newline.
.XX
.IP "(2)\f(CWg\fR \(em get contents of hold area
.XX
The
.CW g
function copies the contents of the hold area into
the pattern space (destroying the previous contents of the
pattern space).
.XX
.IP "(2)\f(CWG\fR \(em Get contents of hold area
.XX
The
.CW G
function appends the contents of the hold area to the
contents of the pattern space; the former and new contents are separated by
a newline.
.XX
.IP "(2)\f(CWx\fR \(em exchange
.XX
The exchange command interchanges the contents
of the pattern space and the hold area.
.SH
3.6.  Flow-of-Control Functions
.PP
These functions do no editing on the input
lines, but control the application of functions
to the lines selected by the address part.
.XX
.IP "(2)\f(CW!\fR \(em Don't
.XX
The
.I Don't
command causes the next command
(written on the same line), to be applied to all and only those input lines
.I not
selected by the address part.
.XX
.IP "(2)\f(CW{\fR \(em Grouping
.XX
The grouping command
.CW {
causes the
next set of commands to be applied
(or not applied) as a block to the
input lines selected by the addresses
of the grouping command.
The first of the commands under control of the grouping
may appear on the same line as the
.CW {
or on the next line.
.PP
The group of commands is terminated by a
matching
.CW }
standing on a line by itself.
.PP
Groups can be nested.
.XX
.IP "(0)\f(CW:\fI<label>\fR \(em place a label
.XX
The label function marks a place in the list
of editing commands which may be referred to by
.CW b
and
.CW t
functions.
The
.I <label>
may be any sequence of seven or fewer characters;
if two different colon functions have identical labels,
a compile time diagnostic will be generated, and
no execution attempted.
.XX
.IP "(2)\f(CWb\fI<label>\fR \(em branch to label
.XX
The branch function causes  the sequence of editing commands being
applied to the current input line to be restarted immediately
after the place where a colon function with the same
.I <label>
was encountered.
If no colon function with the same label can be found after
all the editing commands have been compiled, a compile time diagnostic
is produced, and no execution is attempted.
.XX
A
.CW b
function with no
.I <label>
is taken to be a branch to the end of the
list of editing commands;
whatever should be done with the current input line is done, and
another input line is read; the list of editing commands is restarted from the
beginning on the new line.
.XX
.IP "(2)\f(CWt\fI<label>\fR \(em test substitutions
.XX
The
.CW t
function tests whether 
.I any
successful substitutions have been made on the current input
line;
if so, it branches to
.I <label> ;
if not, it does nothing.
The flag which indicates that a successful substitution has
been executed is reset by:
.XX
.RS
.IP "1) reading a new input line, or
.br
.IP "2) executing a \f(CWt\fR function.
.RE
.SH
3.7. Miscellaneous Functions
.IP "(1)\f(CW=\fR \(em equals
.XX
The
.CW =
function writes to the standard output the line number of the
line matched by its address.
.XX
.IP "(1)\f(CWq\fR \(em quit
.XX
The
.CW q
function causes the current line to be written to the
output (if it should be), any appended or read text to be written, and
execution to be terminated.
.SH
Example
.PP
If the file
.CW prog.sed
contains:
.P1 0
.ps -1
:next
N
/^\e([A-Z][a-zA-Z][a-zA-Z]*\e)\e(.*\e)\en\e1\e(.*\e)/{
	s//\e1\e2\e3/
	b next
}
h
s/\en.*//p
g
s/.*\en//
.ps +1
.P2
The following command:
.P1
sed -n -f prog.sed countries2
.P2
produces:
.P3
.TS
l1l1l1l1l1.
USSR	8649	287	$3,000	Asia
Canada	3852	25.3	$14,000	North America
China	3705	1069.6	$258	Asia
.TE
.P4
.PP
This needs some explanation, which goes as follows:
The first line is a label that we'll need later.
.CW N
appends a newline and the next input line onto the pattern space.
So the pattern space now contains:
.P1
USSR   8649\enUSSR    287
.P2
The next line 
matches if the input line read by
.CW N
begins with the same string as the previous input line.
In this case it does and we execute the code inside the {},
where \e1 is
.CW USSR ,
\e2 is
.CW "   8649" ,
and \e3 is
.CW "   287" .
So the pattern space now contains:
.P1
USSR   8649   287
.P2
and we branch back to the beginning label
.CW next
and read another line into the pattern space with the
.CW N
command.
This loop is repeated on our input until we read the line
.P1
Canada   3852
.P2
At that point the pattern space contains:
.P3
.TS
l1l1l1l1l1l1l1.
USSR	8649	287	$3,000	Asia\enCanada	3852
.TE
.P4
which fails to match the line after the
.CW N .
The
.CW h
copies the pattern space to the hold area.
The
next line
deletes the embedded newline and the line just read from the pattern
space and prints the following:
.P1 0
USSR   8649   287   $3,000   Asia
.P2
The
.CW g
retrieves the saved pattern space from the hold area and the next line
deletes what was just printed, leaving
.CW "Canada   3852" .
Having reached the end on the program,
.CW sed
automatically branches back to the beginning.
.PP
But there's a bug.
This program fails to print the
.CW China
line.
When
.CW sed
reads an end-of-file with the
.CW N
command, it quits.
The fix for this is to add
.CW $p
immediately after the label.
Then when we branch back on the last line, the pattern space is printed.
You may have wondered why the pattern to match the first string was so complicated.
Why not just say
.CW /^[A-Z][A-Za-z]*/
or even
.CW /^[A-Za-z]*/ ?
The problem is that both of these patterns cause the
.CW C
in
.CW Canada
to match the
.CW C
in
.CW China .
When writing complicated
.CW sed
programs, it's wise to do lots of testing.
.SH
References
.LP
|reference_placement


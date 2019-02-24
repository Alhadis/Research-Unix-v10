.so ../ADM/mac
.XX f77 317 "A Portable Fortran 77 Compiler"
.de XX
.IP "\f(CW\\$1\f1" 5
..
.EQ
delim $$
.EN
. \".ND "1 August 1978"
.ND "30 June 1989"
.TL
A Portable Fortran 77 Compiler\(dg
.AU
S. I. Feldman
P. J. Weinberger
.AI
.MH
.AB
.PP
We report here on the first complete implementation of a compiler and run-time system for
the current Fortran language, known as Fortran 77.
The compiler is designed to be portable,
to be correct and complete,
and to generate code compatible with calling sequences produced by C compilers.
In particular, this Fortran is quite usable on
.UX
systems.
In this paper, we describe the language compiled,
interfaces between procedures,
and file formats assumed by the I/O system.
An appendix describes the Fortran 77 language.
.AE
.2C
.FS
\(dg This is a version of |reference(f77 v7man) revised by D. M. Gay.
.FE
.NH 1
Introduction
.PP
The current Fortran language, known as Fortran 77,
became an official American National Standard|reference(standard f77)
on April 3, 1978.
Fortran 77 supplants 1966 Standard Fortran|reference(standard f66).
We report here on a compiler and run-time system for Fortran 77.
The compiler and computation library were written by SIF, the I/O system by PJW.
We believe ours to be the first complete Fortran 77 system
to have been implemented.
This compiler is designed to be portable to a number of different machines,
to be correct and complete,
and to generate code compatible with calling sequences produced
by compilers for the C language|reference(cbook).
In particular,
it is in use on
.UX
systems.
The C compilers in use at Bell Labs include two families,
one based on D. M. Ritchie's PDP-11 compiler|reference(%type pamphlet
%author D. M. Ritchie
%title private communication), another
based on S. C. Johnson's portable C compiler|reference(pcc).
This Fortran compiler can drive the second passes of either family.
In this paper, we describe the language compiled,
interfaces between procedures,
and file formats assumed by the I/O system.
.NH 2
Usage
.PP
.I F77
is a general-purpose command for compiling and loading Fortran and Fortran-related files.
The command to run the compiler is
.P1
f77  \fIflags  file . . .\fR
.P2
EFL|reference(efl) and Ratfor|reference(ratfor)
source files will be preprocessed before
being presented to the Fortran compiler.
C and assembler source files will be compiled by the appropriate programs.
Object files will be loaded.
(The
.I f77
and
.I cc
commands cause slightly different loading sequences to be generated,
since Fortran programs need a few extra libraries and a different startup routine
than do C programs.)
The following file name suffixes are understood:
.TS
center;
aFCW a.
\&.f	Fortran source file
\&.e	EFL source file
\&.r	Ratfor source file
\&.c	C source file
\&.s	Assembler source file
\&.o	Object file
.TE
The following flags are understood:
.XX -S
Generate assembler output for each source file, but do not assemble it.
Assembler output for a source file
.CW x.f ,
.CW x.e ,
.CW x.r ,
or
.CW x.c
is put on file
.CW x.s .
.XX -c
Compile but do not load.
Output for
.CW x.f ,
.CW x.e ,
.CW x.r ,
.CW x.c ,
or
.CW x.s
is put on file
.CW x.o .
.XX -m
Apply the M4 macro preprocessor to each EFL or Ratfor source file before using the appropriate compiler.
.XX -p
Generate code to produce usage profiles.
.XX "-o \fIf\fR"
Put executable module on file
.I f.
(Default is
.CW a.out ).
.XX -w
Suppress all warning messages.
.XX -w66\ 
Suppress warnings about Fortran 66 features used.
.XX -O
Invoke the C object code optimizer.
.XX -C
Compile code that checks that subscripts are within array bounds.
.XX -onetrip\ \ 
Compile code that performs every
.CW do
loop at least once.
(See Section 2.11).
.XX -U
Do not convert upper case letters to lower case.
The default is to convert Fortran programs to lower case.
.XX -u
Make the default type of a variable
.CW undefined.
(See Section 2.2).
.XX -I2
On machines which support short integers,
make the default integer constants and variables short.
(\f(CW-I4\fR is the standard value of this option).  (See Section 2.13).
All logical quantities will be short.
.XX -E
The remaining characters in the argument are used as an EFL flag argument.
.XX -R
The remaining characters in the argument are used as a Ratfor flag argument.
.XX -F
Ratfor and EFL source programs are pre-processed into Fortran files,
but those files are not compiled or removed.
.in -1i
.LP
Other flags,
all library names (arguments beginning \f(CW-l\fR),
and any names not ending with one of the understood suffixes are passed to the loader.
.NH 2
Documentation Conventions
.PP
We write Fortran keywords and other literal strings in
.CW typewriter-font
lower case.
.NH 2
Implementation Strategy
.PP
The compiler and library are written entirely in C.
The compiler  generates C compiler intermediate code.
Since there are C compilers running on a variety of machines,
relatively small changes will make this Fortran compiler generate code for any of them.
Furthermore, this approach guarantees that the resulting programs are compatible with C usage.
The runtime computational library is complete.
The mathematical functions rely on the functions in C's \fIlibm\fR.
The runtime I/O library makes use of D. M. Ritchie's Standard C I/O package
|reference(stdio)
for transferring data.
With the exception described below, only documented calls are used,
so it should be relatively easy to modify the I/O library
to run on other operating
systems.
.NH 1
Language Extensions
.PP
Fortran 77 includes almost all of Fortran 66 as a subset.
We describe the differences briefly in the Appendix.
The most important additions are a character string data type,
file-oriented input/output statements, and random access I/O.
Also, the language has been cleaned up considerably.
.PP
In addition to implementing the language specified in the Fortran 77 Standard,
our compiler implements a few extensions described in this section.
Most are useful additions to the language.
The remainder are extensions
to make it easier to communicate with C procedures
or to permit compilation of
old (1966 Standard) programs.
.NH 2
Double Complex Data Type
.PP
The new type
.CW "double complex"
is defined.
Each datum is represented by a pair of double precision real variables.
A double complex version of every
.CW complex
built-in function is provided.
The specific function names begin with \f(CWz\fR instead of \f(CWc\fR.
.NH 2
Implicit Undefined statement
.PP
Fortran 66 has a fixed rule that the type of a variable that does not appear in a type statement
is
.CW integer
if its first letter is one of
.CW ijklmn
and
.CW real
otherwise.
Fortran 77 has an
.CW implicit
statement for overriding this rule.
As an aid to good programming practice, we permit an additional type,
.CW undefined .
The statement
.P1
implicit undefined(a-z)
.P2
turns off the automatic data typing mechanism,
and the compiler will issue a diagnostic for each variable that is used but does
not appear in a type statement.
Specifying the
.CW -u
compiler flag is equivalent to beginning each procedure with this statement.
.NH 2
Recursion
.PP
Procedures may call themselves, directly or through a chain of other procedures.
.NH 2
Automatic Storage
.PP
Two new keywords are recognized,
.CW static
and
.CW automatic .
These keywords may appear as ``types'' in type statements and in
.CW implicit
statements.
Local variables are static by default;
there is exactly one copy of the datum, and its value is retained between calls.
There is one copy of each variable declared
.CW automatic
for each invocation of the procedure.
Automatic variables may not appear in
.CW equivalence ,
.CW data ,
or
.CW save
statements.
.NH 2
Source Input Format
.PP
The Standard expects input to the compiler to be in 72 column format:
except in comment lines,
the first five characters are the statement label, the next is the continuation character,
and the next sixty-six are the body of the line.
(If there are fewer than seventy-two characters on a line, the compiler pads it with blanks;
characters after the seventy-second are ignored).
.PP
To make typing Fortran programs easier,
our compiler also accepts input in variable length lines.
An ampersand
.CW &
in the first position of a line indicates a continuation
line; the remaining characters form the body of the line.
A tab character in one of the first six positions of a line signals the
end of the statement label and continuation part of the line;
the remaining characters form the body of the line.
A tab elsewhere on the line is treated as another kind of blank by the
compiler.
.PP
In the Standard, there are only 26 letters \(em Fortran is a one-case language.
Consistent with ordinary
.UX
system usage, our compiler expects lower case input.
By default, the compiler converts all upper case characters to lower case except those inside character constants.
However, if the
.CW -U
compiler flag is specified, upper case letters are not transformed.
In this mode, it is possible to specify external names with upper case letters in them,
and to have distinct variables differing only in case,
but then all Fortran keywords must be in lower case.
.NH 2
\f(CWinclude\fR Statement
.PP
The statement
.P1
include 'stuff'
.P2
is replaced by the contents of the file
.CW stuff .
.CW include s
may be nested to a reasonable depth, currently ten.
.NH 2
Binary Initialization Constants
.PP
A
.CW logical ,
.CW real ,
or
.CW integer
variable may be initialized in a
.CW data
statement
by a binary constant, denoted by a letter followed by a quoted string.
If the letter is
.CW b ,
the string is binary, and only zeros and ones are permitted.
If the letter is
.CW o ,
the string is octal, with digits
.CW 0-7 .
If the letter is
.CW z
or
.CW x ,
the string is hexadecimal, with digits
.CW 0-9a-f .
Thus, the statements
.P1
integer a(3)
data a / b'1010', o'12', z'a' /
.P2
initialize all three elements of
.I a
to ten.
.NH 2
Character Strings
.PP
For compatibility with C usage, the following backslash escapes are recognized:
.TS
center;
aFCW a.
\en	newline
\et	tab
\eb	backspace
\ef	form feed
\e0	null
\e'	apostrophe (does not terminate a string)
\e"	quotation mark (does not terminate a string)
\e\e	\e
\e\fIx\fP	\fIx\fR, where \fIx\fR is any other character
.TE
Fortran 77 only has one quoting character, the apostrophe.
Our compiler and I/O system recognize
both the apostrophe
.CW '
and the double-quote \f(CW"\fR.
If a string begins with one variety of quote mark, the other may be embedded within it
without using the repeated quote or backslash escapes.
.PP
Every unequivalenced scalar local character variable and every character string constant is aligned
on an
.CW integer
word boundary.
Each character string constant appearing outside a
.CW data
statement is followed by a
null character to ease communication with C routines.
.NH 2
Hollerith
.PP
Fortran 77 does not have the old Hollerith (\fIn\|\f(CWh\fR)
notation,
though the new Standard recommends implementing the old Hollerith feature
in order to improve compatibility with old programs.
In our compiler, Hollerith data may be used in place of character string constants,
and may also be used to initialize non-character variables in
.CW data
statements.
.NH 2
Equivalence Statements
.PP
As a very special and peculiar case,
Fortran 66 permits an element of a multiply-dimensioned array to be represented by
a singly-subscripted reference in
.CW equivalence
statements.
Fortran 77 does not permit this usage, since
subscript lower bounds may now be different from 1.
Our compiler permits single subscripts in
.CW equivalence
statements,
under the interpretation that all missing subscripts are equal to 1.
A warning message is printed for each such incomplete subscript.
.NH 2
One-Trip \f(CWdo\fP Loops
.PP
The Fortran 77 Standard requires that the range of a
.CW do
loop not be performed
if the initial value is already past the limit value,
as in
.P1
do 10 i = 2, 1
.P2
The 1966 Standard stated that the effect of such a statement was undefined,
but it was common practice that the range of a
.CW do
loop would be performed
at least once.
In order to accommodate old programs, though they were in violation of the 1966 Standard,
the
.CW -onetrip
compiler flag causes non-standard loops to be generated.
.NH 2
Commas in Formatted Input
.PP
The I/O system attempts to be more lenient than the
Standard when it seems worthwhile.
In a formatted read of non-character variables,
commas may be used as value separators in the input record,
overriding the field lengths given in the format statement.
Thus,
the format
.P1
(i10, f20.10, i4)
.P2
will read the record
.P1
-345,.05e-3,12
.P2
correctly.
.NH 2
Short Integers
.PP
On machines that support halfword integers,
the compiler accepts declarations of type
.CW integer\(**2 .
(Ordinary integers follow the Fortran rules about occupying the same
space as a
.CW real
variable; they are assumed to be of C type
.CW "long int" ;
halfword integers are of C type
.CW "short int" .)
An expression involving only objects of type
.CW integer\(**2
is of that type.
Generic functions return short or long integers depending on the actual types of their arguments.
If a procedure is compiled using the
.CW -I2
flag, all small integer constants will be
of type
.CW integer\(**2 .
If the precision of an integer-valued intrinsic function is not determined by the generic function rules,
one will be chosen that returns the prevailing length
(\f(CWinteger\(**2\fR when the \f(CW-I2\fR command flag is in effect).
When the
.CW -I2
option is in effect, all quantities of type
.CW logical
will be short.
Note that these short integer and logical quantities do not obey the standard rules for storage association.
.NH 2
Additional Intrinsic Functions
.PP
This compiler supports all of the intrinsic functions specified in the Fortran 77 Standard.
In addition, there are functions for performing bitwise Boolean operations
(\fIor\fR, \fIand\fR, \fIxor\fR, and \fInot\fR)
and for accessing the
.UX
command arguments
(\fIgetarg\fR and \fIiargc\fR).
.NH 1
Violations of the Standard
.PP
We know only two ways in which our Fortran system violates
the Fortran 77 standard:
.NH 2
Double Precision Alignment
.PP
The Fortran standards (both 1966 and 1977)
permit
.CW common
or
.CW equivalence
statements to force a double precision quantity onto an odd word boundary,
as in the following example:
.P1 I
real a(4)
double precision b,c
.sp .5
equivalence (a(1),b), (a(4),c)
.P2
Some machines (e.g., Honeywell 6000, IBM 360) require that double precision quantities be on double word boundaries;
other machines (e.g., IBM 370), run inefficiently if this alignment rule is not observed.
It is possible to tell which equivalenced and common variables suffer from a forced odd
alignment, but every double precision argument would have to be assumed on a bad boundary.
To load such a quantity on some machines,
it would be necessary to use separate operations to move the upper and lower halves
into the halves of an aligned temporary, then to load that double precision temporary; the reverse would be
needed to store a result.
We have chosen to require that all double precision real and complex quantities
fall on even word boundaries on machines with corresponding hardware requirements,
and to issue a diagnostic if the source code demands a violation of the rule.
.NH 2
\f(CWt\fR and \f(CWtl\fR Formats
.PP
The implementation of the
.CW t
(absolute tab)
and
.CW tl
(leftward tab)
format codes
is defective.
These codes allow rereading or rewriting part of the
record which has already been processed.
(See Section 6.3.2 in the Appendix.)
The implementation uses seeks,
so if the unit is not one which allows seeks,
such as a terminal,
the program is in error.
(People who can make a case for using
.CW tl
should let us know.)
A benefit of the implementation chosen is
that there is no upper limit on the length of
a record,
nor is it necessary to predeclare any record
lengths except where specifically required
by Fortran or the operating system.
.NH 1
Inter-Procedure Interface
.PP
To be able to write C procedures that call or are called by Fortran procedures,
it is necessary to know the conventions for procedure names,
data representation,
return values,
and argument lists that the compiled code obeys.
.NH 2
Procedure Names
.PP
On
.UX
systems,
the name of a common block or a Fortran procedure
has an underscore appended to it by the compiler
to distinguish it from a C procedure or external variable
with the same user-assigned name.
Fortran library procedure names have embedded underscores to avoid clashes
with user-assigned subroutine names.
.KF
.TS
center;
c c
l l.
Fortran	C     
.sp .5
integer\(**2 x	short int x;
integer x	long int x;
logical x	long int x;
real x	float x;
double precision x	double x;
complex x	struct { float r, i; } x;
double complex x	struct { double dr, di; } x;
character\(**6 x	char x[6];
.TE
.ce
\fBTable 1. Fortran and C declarations
.KE
.NH 2
Data Representations
.PP
Table 1 shows corresponding Fortran and C declarations:
(By the rules of Fortran,
.CW integer,
.CW logical,
and
.CW real
data occupy the same amount of memory).
.NH 2
Return Values
.PP
A function of type
.CW integer ,
.CW logical ,
.CW real ,
or
.CW "double precision"
must be declared as a C function that returns the corresponding type.
A
.CW complex
or
.CW "double complex"
function is equivalent to a C routine
with an additional
initial argument that points to the place where the return value is to be stored.
Thus,
.P1
complex function f( . . . )
.P2
is equivalent to
.P1
f_(temp, . . .)
struct { float r, i; } \(**temp;
 . . .
.P2
A character-valued function is equivalent to a C routine with
two extra initial arguments:
a data address and a length.
Thus,
.P1
character\(**15 function g( . . . )
.P2
is equivalent to
.P1
g_(result, length, . . .)
char result[ ];
long int length;
 . . .
.P2
and could be invoked in C by
.P1
char chars[15];
 . . .
g_(chars, 15L, . . . );
.P2
Subroutines are invoked as if they were
.CW integer -valued
functions
whose value specifies which alternate return to use.
Alternate return arguments (statement labels) are not passed to the function,
but are used to do an indexed branch in the calling procedure.
(If the subroutine has no entry points with alternate return arguments,
the returned value is undefined.)
The statement
.P1
call nret(\(**1, \(**2, \(**3)
.P2
is treated exactly as if it were the computed
.CW goto
.P1
goto (1, 2, 3),  nret( )
.P2
.NH 2
Argument Lists
.PP
All Fortran arguments are passed by address.
In addition,
for every non-function argument that is of type character,
an argument giving the length of the value is passed.
(The string lengths are
.CW "long int"
quantities passed by value).
The order of arguments is then:
extra arguments for complex and character functions,
address for each datum or function,
a \f(CWlong int\fR for each character argument (other than character-valued functions).
Thus, the call in
.P1
external f
character\(**7 s
integer b(3)
 . . .
call sam(f, b(2), s)
.P2
is equivalent to that in
.P1
int f();
char s[7];
long int b[3];
 . . .
sam_(f, &b[1], s, 7L);
.P2
Note that the first element of a C array always has subscript zero,
but Fortran arrays begin at 1 by default.
Fortran arrays are stored in column-major order, C arrays are stored in row-major order.
.NH 1
File Formats
.NH 2
Structure of Fortran Files
.PP
Fortran requires four kinds of external files:
sequential formatted and unformatted,
and direct formatted and unformatted.
On
.UX
systems,
these are all implemented as ordinary files
which are assumed to have the proper
internal structure.
.PP
Fortran I/O is based on ``records''.
When a direct file is opened in a Fortran program,
the record length of the records must be given,
and this is used by the Fortran I/O system to
make the file look as if it is made up of records
of the given length.
In the special case that the record length is given
as 1,
the files are not considered to be divided into records,
but are treated as byte-addressable byte strings;
that is,
as ordinary
.UX
file system files.
(A read or write request on such a file keeps consuming bytes until
satisfied, rather than being restricted to a single record.)
.PP
The peculiar requirements on sequential unformatted files
make it unlikely that they will ever be read or written by any means except Fortran I/O statements.
Each record is preceded and followed by
an integer containing the record's length in bytes.
.PP
The Fortran I/O system breaks sequential formatted files
into records while reading by using each newline
as a record separator.
The result of reading off the end of a record is undefined according to the Standard.
The I/O system is permissive and
treats the record as being extended by blanks.
On output,
the I/O system will write a newline at the end of each
record.
It is also possible for programs to write newlines
for themselves.
This is an error,
but the only effect will be that the single record
the user thought he wrote will be treated as
more than one record when being read or
backspaced over.
.NH 2
Portability Considerations
.PP
The Fortran I/O system uses only the facilities of the
standard C I/O library,
a widely available and fairly portable package,
with the following exceptions.
The I/O system needs to know whether a file
can be used for direct I/O,
and whether or not it is possible to backspace.
Both of these facilities are implemented
using the
.I fseek
routine,
so there is a routine
.I canseek
which determines if
.I fseek
will have the desired effect.
Combinations of the
\f(CWbackspace\fR, \f(CWwrite\fR, \f(CWendfile\fR, and
.CW close
statements may require that a file be shortened.
For efficiency, the \fIcopy\fR routine
that does this job uses the
.UX
system calls \fIcreat\fR, \fIread\fR, \fIwrite\fR, and \fIclose\fR.
Finally, to open write-only files, the I/O system must resort to the
\fIcreat\fR and \fIopen\fR system calls.
.NH 2
Pre-Connected Files and File Positions
.PP
Units 5, 6, and 0 are preconnected when the program starts.
Unit 5 is connected to the standard input,
unit 6 is connected to the standard output,
and unit 0 is connected to the standard error unit.
All are connected for sequential formatted I/O.
.PP
All the other units are also preconnected when execution
begins.
Unit
.I n
is connected to a file named \f(CWfort.\fIn\fR.
These files need not exist,
nor will they be created unless their units are used
without first executing an
.CW open .
The default connection is for sequential formatted I/O.
.PP
The Standard does not specify where a file which has been explicitly \f(CWopen\fRed
for sequential I/O is initially positioned.
Following common practice, the I/O system positions the file
at its beginning (and has done so for several years).
Originally it attempted to position the file
at the end, but this was a frequent source of confusion, and it
limited portability.
. \"In fact,
. \"the I/O system attempts to position the file at the end,
. \"so a
. \".B write
. \"will append to the file and a
. \".B read
. \"will result in an end-of-file indication.
. \"To position a file to its beginning,
. \"use a
. \".B rewind
. \"statement.
The preconnected units
0, 5, and 6 are positioned as they come
from the program's parent process.
.NH
References
.LP
|reference_placement
.NH
Appendix.  Differences Between Fortran 66 and Fortran 77
.PP
The following is a very brief description of the differences
between the 1966 and the 1977 Standard languages.
We assume that the reader is familiar with Fortran 66.
We do not pretend to be complete, precise,
or unbiased,
but plan to describe what we feel are the most important aspects of the new language.
.NH 0
Features Deleted from Fortran 66
.NH 2
Hollerith
.PP
All notions of ``Hollerith''
(\fIn\|\f(CWh\fR)
as data
have been officially removed, although our compiler, like almost all in the foreseeable future,
will continue to support this archaism.
.NH 2
Extended Range
.PP
In Fortran 66, under a set of very restrictive and rarely understood conditions, it is permissible
to jump out of the range of a
.CW do
loop, then jump back into it.
Extended range has been removed in the Fortran 77 language.
The restrictions are so special, and the implementation of extended range is so unreliable in many compilers,
that this change really counts as no loss.
.NH 1
Program Form
.NH 2
Blank Lines
.PP
Completely blank lines are now legal comment lines.
.NH 2
Program and Block Data Statements
.PP
A main program may now begin with a statement that gives that program an external name:
.P1
program work
.P2
Block data procedures may also have names.
.P1
block data stuff
.P2
There is now a rule that only
.I one
unnamed
block data procedure may appear in a program.
(This rule is not enforced by our system.)
The Standard does not specify the effect of the program and block data names,
but they are clearly intended to aid conventional loaders.
.NH 2
\f(CWentry\fP Statement
.PP
Multiple entry points are now legal.
Subroutine and function subprograms may have additional entry points,
declared by an
.CW entry
statement with an optional argument list.
.P1
entry extra(a, b, c)
.P2
Execution begins at the first statement following the
.CW entry
line.
All variable declarations must precede all executable statements in the procedure.
If the procedure begins with a
.CW subroutine
statement,
all entry points are subroutine names.
If it begins with a
.CW function
statement, each entry is a function entry point,
with type determined by the type declared for the entry name.
If any entry is a character-valued function,
then all entries must be.
In a function, an entry name of the same type as that where control entered
must be assigned a value.
Arguments do not retain their values between calls.
(The ancient trick of calling one entry point with a large number of arguments
to cause the procedure to ``remember'' the locations of those arguments,
then invoking an entry with just a few arguments for later calculation,
is still illegal.
Furthermore, the trick doesn't work in our implementation,
since arguments are not kept in static storage.)
.NH 2
\f(CWdo\fP Loops
.PP
.CW do
variables and range parameters may now be of integer, real, or double precision types.
(The use of floating point
.CW do
variables is very dangerous
because of the possibility of unexpected roundoff,
and we strongly recommend against their use).
The action of the
.CW do
statement is now defined for all values of the
.CW do
parameters.
The statement
.P1
do 10 i = l, u, d
.P2
performs one iteration if $l ~=~ u$ and
$ max (0^,^ left floor ( u - l ) / d^ right floor )$
iterations otherwise.
The
.CW do
variable has a predictable value when exiting a loop:
the value at the time a
.CW goto
or
.CW return
terminates the loop;
otherwise
the value that failed the limit test.
.NH 2
Alternate Returns
.PP
In a
.CW subroutine
or subroutine
.CW entry
statement,
some of the arguments may be noted by an asterisk, as in
.P1
subroutine s(a, \(**, b, \(**)
.P2
The meaning of the ``alternate returns'' is described in section 5.2 of the Appendix.
.NH 1
Declarations
.NH 2
\f(CWcharacter\fP Data Type
.PP
One of the biggest improvements to the language is the addition of a character-string data type.
Local and
common character variables must have a length denoted by a constant expression:
.P1
character\(**17 a, b(3,4)
character\(**(6+3) c
.P2
If the length is omitted entirely, it is assumed equal to 1.
A character string argument may have a constant length,
or the length may be declared to be the same as that of the corresponding actual argument at run time
by a statement like
.P1
character\(**(\(**) a
.P2
(There is an intrinsic function
.CW len
that returns the actual length of a character string).
Character arrays and common blocks containing character variables must be packed:
in an array of character variables, the first character of one element must follow the last character of
the preceding element, without holes.
.NH 2
\f(CWimplicit\fP Statement
.PP
The traditional implied declaration rules still hold:
a variable whose name begins with one of
.CW ijklmn
is of type
.CW integer .
Other variables are of type
.CW real
unless otherwise declared.
This general rule may be overridden with an
.CW implicit
statement:
.P1 0
implicit real(a-c,g), complex(w-z)
implicit character*(17) (s)
.P2
declares that variables whose name begins with one of
.CW abcg
are
.CW real ;
those beginning with
.CW wxyz
are assumed
.CW complex ,
and so on.
It is still poor practice to depend on implicit typing,
but this statement is an industry standard.
.NH 2
\f(CWparameter\fP Statement
.PP
It is now possible to give a constant a symbolic name, as in
.P1 0
parameter (pi=3.1415d0,y=pi/3,s='hello')
.P2
The type of each parameter name is governed by the same implicit and explicit rules as for a variable.
The right side of each equal sign must be a constant expression
(an expression made up of constants, operators, and already defined parameters).
.NH 2
Array Declarations
.PP
Arrays may now have as many as seven dimensions.
(Only three were permitted in 1966).
The lower bound of each dimension may be declared
to be other than 1 by
using a colon.
Furthermore, an adjustable array bound may be an integer expression involving constants,
arguments, and variables in
.CW common .
.P1 0
real a(-5:3, 7, m:n), b(n+1:2\(**n)
.P2
The upper bound on the last dimension of an array argument may be denoted by an asterisk
to indicate that the upper bound is not specified:
.P1 0
integer a(5, \(**),  b(\(**), c(0:1, -2:\(**)
.P2
.NH 2
\f(CWsave\fP Statement
.PP
A poorly known rule of Fortran 66 is that local variables in a procedure do not necessarily retain their values between
invocations of that procedure.
At any instant in the execution of a program,
if a common block is declared neither in the currently executing procedure
nor in any of the procedures in the chain of callers,
all of the variables in that common block also become undefined.
(The only exceptions are variables that have been defined in a
.CW data
statement and never changed).
These rules permit overlay and stack implementations for the affected variables.
Fortran 77 permits one to specify that certain variables and common blocks are to retain their
values between invocations.
The declaration
.P1
save a, /b/, c
.P2
leaves the values of the variables
.CW a
and
.CW c
and all of the contents of common block
.CW b
unaffected by a return.
The simple declaration
.P1
save
.P2
has this effect on all variables and common blocks in the procedure.
A common block must be
.CW save d
in every procedure in which it is declared if the desired effect is to occur.
.NH 2
\f(CWintrinsic\fP Statement
.PP
All of the functions specified in the Standard
are in the single category ``intrinsic functions'',
rather than being divided into ``intrinsic'' and ``basic external'' functions.
If an intrinsic function is to be passed to another procedure, it must be declared
.CW intrinsic .
Declaring it
.CW external
(as in Fortran 66) causes a function other than the built-in one to be passed.
.NH 1
Expressions
.NH 2
Character Constants
.PP
Character string constants are marked by strings surrounded by apostrophes.
If an apostrophe is to be included in a constant, it is repeated:
.P1
 'abc'
 'ain''t'
.P2
There are no null (zero-length) character strings in Fortran 77.
Our compiler has two different quotation marks,
.CW '
and \f(CW"\fR.
(See Section 2.8 in the main text.)
.NH 2
Concatenation
.PP
One new operator has been added, character string concatenation, marked by a double slash
.CW // .
The result of a concatenation is the string containing the characters of the left operand followed by the characters of
the right operand.
The strings
.P1
 'ab' // 'cd'
 'abcd'
.P2
are equal.
The strings being concatenated must be of constant length in all concatenations
that are not the right sides of assignments.
(The only concatenation expressions in which a
character string declared adjustable with a
.CW *(*)
modifier
or a substring denotation with nonconstant position values may appear
are the right sides of assignments).
.NH 2
Character String Assignment
.PP
The left and right sides of a character assignment may not share storage.
(The assumed implementation of character assignment is to copy characters from the right to the left side.)
If the left side is longer than the right, it is padded with blanks.
If the left side is shorter than the right, trailing characters are discarded.
.NH 2
Substrings
.PP
It is possible to extract a substring of a character variable or character array element, using the colon notation:
.P1
a(i,\|j) (m:n)
.P2
is the string of $(n-m+1)$ characters beginning at the
$m sup th$ character of the character array element $a sub ij$.
Results are undefined unless $m<=n$.
Substrings may be used on the left sides of assignments and as procedure actual arguments.
.NH 2
Exponentiation
.PP
It is now permissible to raise real quantities to complex powers,
or complex quantities to real or complex powers.
(The principal part of the logarithm is used).
Also, multiple exponentiation is now defined:
.P1
a\(**\(**b\(**\(**c = a \(**\(** (b\(**\(**c)
.P2
.NH 2
Relaxation of Restrictions
.PP
Mixed mode expressions are now permitted.
(For instance,
it is permissible to combine integer and complex quantities in an expression.)
.PP
Constant expressions are permitted where a constant is allowed,
except in
.CW data
statements.
(A constant expression is made up of explicit constants and
.CW parameter s
and the Fortran operators,
except for exponentiation to a floating-point power).
An adjustable dimension may now be an integer expression involving constants,
arguments, and variables in
.CW common .
.PP
Subscripts may now be general integer expressions;
the old
$c v +- c'$
rules have been removed.
.CW do
loop bounds may be general integer, real, or double precision expressions.
Computed
.CW goto
expressions and I/O unit numbers may be general integer expressions.
.NH 1
Executable Statements
.NH 2
Block If
.PP
At last, an
if-then-else
branching structure (``Block If'') has been added to Fortran.
A Block If begins with a statement of the form
.P1
if ( . . . ) then
.P2
and ends with an
.P1
end if
.P2
statement.
Two other new statements may appear in a Block If.
There may be several
.P1
else if(. . .) then
.P2
statements,
followed by at most one
.P1
else
.P2
statement.
If the logical expression in the Block If statement is true,
the statements following it up to the
next
.CW elseif ,
.CW else ,
or
.CW endif
are executed.
Otherwise, the next
.CW elseif
statement in the group is executed.
If none of the
.CW elseif
conditions are true, control passes to the statements following the
.CW else
statement, if any.
(The
.CW else
must follow all
.CW elseif s
in a Block If.
Of course, there may be Block Ifs embedded inside of other Block If structures).
A case construct may be rendered
.P1
if (s .eq. 'ab') then
 . . .
else if (s .eq. 'cd') then
 . . .
else
 . . .
end if
.P2
.NH 2
Alternate Returns
.PP
Some of the arguments of a subroutine call may be statement labels preceded by an asterisk, as in
.P1
call joe(j, \(**10, m, \(**2)
.P2
A
.CW return
statement may have an integer expression, such as
.P1
return k
.P2
If the entry point has
$n$
alternate return (asterisk) arguments
and if $1<=k<=n$, the return is followed by a branch to the corresponding statement label;
otherwise the usual return to the statement following the
.CW call
is executed.
.NH 1
Input/Output
.NH 2
Format Variables
.PP
A format may be the value of a character expression (constant or otherwise),
or be stored in a character array, as in
.P1
write(6, '(i5)') x
.P2
.NH 2
\f(CWend=\fP, \f(CWerr=\fP, and \f(CWiostat=\fP Clauses
.PP
A
.CW read
or
.CW write
statement may contain
.CW end= ,
.CW err= ,
and
.CW iostat=
clauses, as in
.P1 0
write(6, 101, err=20, iostat=a(4))
read(5, 101, err=20, end=30, iostat=x)
.P2
Here 5 and 6 are the
.I unit s
on which the I/O is done,
101 is the statement label of the associated format,
20 and 30 are statement labels,
and
.CW a
and
.CW x
are integers.
If an error occurs during I/O,
control returns to the program at statement 20.
If the end of the file is reached,
control returns to the program at statement 30.
In any case, the variable referred to in
the
.CW iostat=
clause is given a value when
the I/O statement finishes.
(Yes, the value is assigned to the name on the right side of the equal sign.)
This value is zero if all went well,
negative for end of file,
and some positive value for errors.
.NH 2
Formatted I/O
.NH 3
Character Constants
.PP
Character constants in formats are copied literally to the output.
Character constants cannot be read into.
.P1 0
write(6,'(i2,'' isn''''t '',i1)') 7, 4
.P2
produces
.P1
 7 isn't 4
.P2
Here the format is the character constant
.P1
(i2,' isn''t ',i1)
.P2
and the character constant
.P1
 isn't
.P2
is copied into the output.
.NH 3
Positional Editing Codes
.PP
.CW t ,
.CW tl ,
.CW tr ,
and
.CW x
codes
control where the
next character is in the record.
.CW tr\fIn
or \fIn\f(CWx\fR
specifies that the next character is
$n$ to the right of the current position.
.CW tl\fIn
specifies that the next character is
$n$ to the left of the current position,
allowing parts of the record to be reconsidered.
.CW t\fIn
says that the next character is to be character
number $n$ in the record.
(See section 3.2 in the main text.)
.NH 3
Colon
.PP
A colon in the format terminates the I/O operation
if there are no more data items in the I/O list;
otherwise it has no effect.
In the fragment
.P1
x='("hello", :, " there", i4)'
write(6, x) 12
write(6, x)
.P2
the first
.CW write
statement prints
.CW "hello there 12" ,
while the second only prints
.CW hello .
.NH 3
Optional Plus Signs
.PP
According to the Standard,
each implementation has the option of putting
plus signs in front of non-negative
numeric output.
The
.CW sp
format code may be used to make the optional plus
signs actually appear for all subsequent items
while the format is active.
The
.CW ss
format code guarantees that the I/O system will not
insert the optional plus signs,
and the
.CW s
format code restores the default behavior of
the I/O system.
(Since we never put out optional plus signs,
.CW ss
and
.CW s
codes have the same effect in our implementation.)
.NH 3
Blanks on Input
.PP
Blanks in numeric input fields,
other than leading blanks
will be ignored following a
.CW bn
code in a format
statement,
and will be treated as zeros following a
.CW bz
code in a format statement.
The default for a unit may be changed by using
the
.CW open
statement.
(Blanks are ignored by default.)
.NH 3
Unrepresentable Values
.PP
The Standard requires that if a numeric item
cannot be represented in the form required by a format code,
the output field must be filled with asterisks.
(We think this should have been an option.)
.NH 3
\f(CWI\fIw.m\fR Format
.PP
There is a new integer output code
.CW i\fIw.m
which is the same as
.CW i\fIw ,
except that there will be at least $m$
digits in the output field,
including,
if necessary,
leading zeros.
The case
.CW i\fIw\fP.0
is special,
in that if the value being printed is 0,
the output field is
entirely blank.
.CW i\fIw\fP.1
is the same as
.CW i\fIw .
.NH 3
Floating Point
.PP
On input, exponents may start with one of
.CW EeDd .
All have the same meaning.
On output we always use
.CW E .
The
.CW e
and
.CW d
format codes also have identical meanings.
A leading zero before the decimal point in
.CW e
output
without a scale factor is optional with the
implementation.
(We do not print it.)
There is a
.CW g\fIw.d
format code which is the same as
.CW e\fIw.d
and
.CW f\fIw.d
on input,
but which chooses
.CW f
or
.CW e
formats for output depending
on the size of the number and of $d$.
.NH 3
\f(CWa\fP Format
.PP
\f(CWa\fP codes are used for character values.
.CW a\fIw
uses a field width of $w$,
while a plain
.CW a
uses the length of the character item.
.NH 2
Standard Units
.PP
There are default formatted input and output units.
The statement
.P1
read 10, a, b
.P2
reads from the standard unit using format statement 10.
The default unit may be explicitly specified by an asterisk, as in
.P1
read(*, 10) a,b
.P2
Similarly, the standard output units is specified by a
.CW print
statement or an asterisk unit:
.P1
print 10
write(*, 10)
.P2
.NH 2
List-Directed Formatting
.PP
List-directed I/O is a
kind of free format for sequential I/O.
It is invoked by using an asterisk as the
format identifier, as in
.P1
read(6, *) a,b,c
.P2
.PP
On input,
values are separated by strings of blanks
and possibly a comma.
Values,
except for character strings,
cannot contain blanks.
End of record counts as a blank,
except in character strings,
where it is ignored.
Complex constants are given as two real constants
separated by a comma and enclosed in parentheses.
A null input field,
such as between two consecutive commas,
means the corresponding variable in the
I/O list is not changed.
Values may be preceded by repetition counts,
as in
.P1
4*(3.,2.)  2*, 4*'hello'
.P2
which stands for 4 complex constants, 2 null values,
and 4 string constants.
.PP
For output, suitable formats are chosen for
each item.
The values of character strings are printed;
they are not enclosed in quotes, so they cannot be read back
using list-directed input.
.NH 2
Direct I/O
.PP
A file connected for direct access consists of
a set of equal-sized records each of which is
uniquely identified by a positive integer.
The records may be written or read in any order,
using direct access I/O statements.
.PP
Direct access
.CW read
and
.CW write
statements
have an extra argument,
.CW rec=,
which gives the record number to be read or written.
.P1 0
read(2, rec=13, err=20) (a(i), i=1, 203)
.P2
reads the thirteenth record into the array
.CW a.
.PP
The size of the records must be given by an
.CW open
statement
(see below).
Direct access files may be connected for either formatted
or unformatted I/O.
.NH 2
Internal Files
.PP
Internal files are character string objects,
such as variables or substrings,
or arrays of type character.
In the former cases there is only a single record
in the file;
in the latter case each array element is a record.
The Standard includes only sequential
formatted I/O on internal files.
(I/O is not a very precise term to use here,
but internal files are dealt with using
.CW read
and
.CW write ).
There is no list-directed I/O on internal files.
Internal files are used by giving the name of the
character object in place of the unit number, as in
.P1
character\(**80 x
read(5,"(a)") x
read(x,"(i3,i4)") n1,n2
.P2
which reads a card image into
.CW x
and then reads
two integers from the front of it.
A sequential
.CW read
or
.CW write
always starts at the beginning
of an internal file.
.NH 2
File Control Statements
.PP
These statements are used to connect and disconnect
units and files,
and to gather information about units and files.
.NH 3
\f(CWopen\fP Statement
.PP
The
.CW open
statement is used to connect a file with a
unit,
or to alter some properties of the connection.
The following is a minimal example.
.P1
open(1, file='fort.junk')
.P2
.CW open
takes a variety of arguments with meanings described below.
.RS
.	\" macros here
.EQ
delim off
.EN
.de HP
.RT
.if !\\(IP .nr IP +1
.sp \\n(PDu
.ne 3v
.in +\\n(PIu
.ti -\\n(PIu
\f(CW\\$1\fR\ \c
..
.de TH
.RT
.sp \\n(PDu
.ne 3v
\f(CW\\$1\\$2\\$3\\$4\\$5\\$6\fR\ \c
..
.	\" end of macros
.EQ
delim $$
.EN
.HP unit=
a small non-negative integer which is the unit to
which the file is to be connected.
We allow,
at the time of this writing,
0 through 99.
If this parameter is the first one in the
.CW open
statement,
the
.CW unit=
can be omitted.
.HP iostat=
is the same as in
.CW read
or
.CW write.
.HP err=
is the same as in
.CW read
or
.CW write.
.HP file=
a character expression,
which when stripped of trailing blanks,
is the name of the file to be connected to the unit.
The filename should not be given if the
.CW status='scratch' .
.HP status=
one of
.CW 'old' ,
.CW 'new' ,
.CW 'scratch' ,
or
.CW 'unknown' .
If this parameter is not given,
.CW 'unknown'
is assumed.
If
.CW 'scratch'
is given,
a temporary file will be created.
Temporary files are destroyed at the end of execution.
If
.CW 'new'
is given,
the file will be created if it doesn't exist,
or truncated if it does.
The meaning of
.CW 'unknown'
is processor dependent;
our system complains if an
.CW 'old'
file does not exist,
and creates a nonexistent file
if it is
.CW 'unknown' .
.HP access=
.CW 'sequential'
or
.CW 'direct' ,
depending on whether the file is
to be opened for sequential or direct I/O.
.HP form=
.CW 'formatted'
or
.CW 'unformatted' .
.HP recl=
a positive integer specifying the record length of
the direct access file being opened.
We measure all record lengths in bytes.
On
.UX
systems a record length of 1 has the special meaning explained
in section 5.1 of the text.
.HP blank=
.CW 'null'
or
.CW 'zero' .
This parameter has meaning only for formatted I/O.
The default value is
.CW 'null' .
.CW 'zero'
means that blanks,
other than leading blanks,
in numeric input fields are to be treated as zeros.
.RE
.PP
Opening a new file on a unit which is already connected
has the effect of first closing the old file.
.NH 3
\f(CWclose\fP Statement
.PP
.CW close
severs the connection between a unit and a file.
The unit number must be given.
The optional parameters are
.CW iostat=
and
.CW err=
with
their usual meanings,
and
.CW status=
either
.CW 'keep'
or
.CW 'delete' .
Scratch files cannot be kept,
otherwise
.CW 'keep'
is the default.
.CW 'delete'
means the file will be removed.
A simple example is
.P1
close(3, err=17)
.P2
.NH 3
\f(CWinquire\fP Statement
.PP
The
.CW inquire
statement gives information about
a unit
(``inquire by unit'')
or a file (``inquire by file'').
Simple examples are:
.P1 0
inquire(unit=3, name=x)
inquire(file='junk', number=n, exist=l)
.P2
.RS
.HP file=
a character variable specifies the file the
.CW inquire
is about.
Trailing blanks in the file name are ignored.
.HP unit=
an integer variable specifies the unit the
.CW inquire
is about.
Exactly one of
.CW file=
or
.CW unit=
must be used.
.HP "iostat=, err="
are as before.
.HP exist=
a logical variable.
The logical variable is set to
\f(CW.true.\fP if the file or unit
exists and is set to
\f(CW.false.\fP otherwise.
.HP opened=
a logical variable.
The logical variable is set to
\f(CW.true.\fP if the file
is connected to a unit or if the unit is connected
to a file,
and it is set to
\f(CW.false.\fP otherwise.
.HP number=
an integer variable to which is assigned the
number of the unit connected to the file,
if any.
.HP named=
a logical variable to which is assigned
\f(CW.true.\fP if the file has a name, or
\f(CW.false.\fP otherwise.
.HP name=
a character variable to which is assigned the name
of the file (inquire by file) or, if known, the name of the
file connected to the unit (inquire by unit).
.HP access=
a character variable to which will be assigned
the value
.CW 'sequential'
if the connection is for
sequential I/O,
.CW 'direct'
if the connection is for direct I/O.
The value becomes undefined if there is no connection.
.HP sequential=
a character variable to which is assigned the
value
.CW 'yes'
if the file could be connected for
sequential I/O,
.CW 'no'
if the file could not be connected for sequential I/O,
and
.CW 'unknown'
if we can't tell.
.HP direct=
a character variable to which is assigned the value
.CW 'yes'
if the file could be connected for direct I/O,
.CW'no'
if the file could not be connected for direct
I/O,
and
.CW 'unknown'
if we can't tell.
.HP form=
a character variable to which is assigned the value
.CW 'formatted'
if the file is connected for formatted I/O,
or
.CW 'unformatted'
if the file is connected for unformatted
I/O.
.HP formatted=
a character variable to which is assigned the value
.CW 'yes'
if the file could be connected for formatted I/O,
.CW 'no'
if the file could not be connected for formatted I/O,
and
.CW 'unknown'
if we can't tell.
.HP unformatted=
a character variable to which is assigned the value
.CW 'yes'
if
the file could be connected for unformatted I/O,
.CW 'no'
if the file could not be connected for unformatted I/O,
and
.CW 'unknown'
if we can't tell.
.HP recl=
an integer variable to which is assigned the record length
of the records in the file if the file is connected
for direct access.
.HP nextrec=
an integer variable to which is assigned one more
than the number of the last record read from a file connected
for direct access.
.HP blank=
a character variable to which is assigned the value
.CW 'null'
if null blank control is in effect for the file
connected for formatted I/O,
.CW 'zero'
if blanks are being converted to zeros and
the file is connected for formatted I/O.
.RE
.PP
The gentle reader
will remember that the people who wrote the standard
probably weren't thinking of his needs.
Here is an example.
The declarations are omitted.
.P1
open(1, file="/dev/console")
.P2
On a
.UX
system this statement opens the console for formatted sequential
I/O.
An
.CW inquire
statement for either unit 1 or file
.CW "/dev/console"
would reveal that the file exists, is connected to unit 1,
has a name, namely 
.CW "/dev/console" ,
is opened for sequential I/O,
could be connected for sequential I/O,
could not be connected for direct I/O (can't seek),
is connected for formatted I/O,
could be connected for formatted I/O,
could not be connected for unformatted I/O
(can't seek),
has neither a record length nor a next record number,
and is ignoring blanks in numeric fields.
.PP
In the
.UX
system environment,
the only way to discover what permissions you have
for a file is to open it and try to read and write it.
The
.CW iostat=
parameter will return system error numbers.
The
.CW inquire
statement does not give a way of determining permissions.

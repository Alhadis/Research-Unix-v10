.nr Cl 7
.ND "October 29, 1979"
.TL "20239-7048" "40295-2"
Guide To The Internals Of
.I Bite
.AU "R. B. Drake" RBD WH 2425 4163 8C-005
.TM 79-2425-5
.AS 1
.I Bite,
.B B asic
.B I nterpreter
for
.B T esting
and
.B E ngineering,
with very few exceptions, follows the syntax of the original Dartmouth
.I BASIC
It is unique because it is written in C,
it's source code is available, new commands to fit any desired purpose
can be written in C and easily added, 
or for that matter, any existing routine such as those in section 3 of
the
.I "UNIX Users Manual"
can be linked in with
.I Bite .
The authors purpose was to
provide an interpretive language to control automated test sets where
the basic control flow operations,
.I Print
statements,
.I If
statements
etc. would be standard 
.I BASIC
but where additional commands to control
test instruments would be added as needed.
With this approach, the language can be tailored to fit virtually any test
set configuration, or molded to perform any
kind of task where an interpretive language is suitable.
.P
Presented here is a tutorial which describes in detail how to add/remove
commands and functions to/from the language and how to use the vital internal
functions which store and retrieve variables, alter program flow, parse 
expressions,
evaluate expressions, print diagnostic messages etc.
.AE
.OK "Basic" "Interpreters" "Bite" "Automated Testing" "Product Evaluation Project"
.MT 1
.SA 0
.H 1 INTRODUCTION
.H 2 Purpose
This memorandum is for the C language programmer who wishes to modify or
extend
.I bite
in order to tailor it to some special purpose. It is presumed that the
reader is already fluent in C and
.I UNIX*
.FS *
UNIX is a Trademark of Bell Laboratories.
.FE
and familiar with
.I bite .\*F
.FS
Hawkins J. P.,"BITE Users Guide",BTL,TM-79-2425-4
.FE
We will follow the method of tutoring by example so that when the reader
is finished he should be able to:
.BL 
.LI
Add or remove commands from the language.
.LI
Add or remove function subroutines.
.LI
Understand how to use certain important internal function subroutines.
.LI
Have a general understanding of how
.I bite
works internally.
.LI
Have a general understanding of why the authors made certain decisions
regarding the way
.I bite
is implemented.
.H 1 "BITE RELEASE TAPE"
.I Bite
is available on magnetic tape from the author or J. P. Hawkins of department
2425.
The 
.I Bite
release tape is divided into four  major  directories,
doc,include,lib  and  src.  Doc  of course contains all currently
available documentation. Include contains all of the header  (.h)
files necessary to recompile.
Lib
contains archive files  containing  all  of  the  object  modules
which may be used to create a new load module with perhaps some
new or changed routines without recompiling everything.
Src contains five directories,
each of which contains source code for all of  the  files
contained  in  the  Lib archives. In addition to source code each
directory under "src" contains one or more shell scripts intended
to  build  a  new  archive or recompile individual modules within
that particular source directory and link the new module with the
existing  ones  in  the archives. In general the scripts to build
new archives are called "build.sh" and the ones to recompile  individual
modules  are  called  "compile.sh".  In  the "src/bite"
directory there are separate "build" and  "compile"  scripts  for
.I bite ,
.I bitex
and
.I bitem
Where
.I bite
is intended to run under PWB/UNIX version 2.0.
.I Bitex
runs stand alone on a PDP-11/03 micro computer and contains drivers for the
IBV11-A (IEEE 488 instrumentt bus) and certain test instruments utilized by
Dept. 2425 for automated testing. Due to space considerations, this version
does not have a complete math package (i.e. sin(),cos(),tan() etc.).
.I Bitem
also runs stand alone on a PDP 11/03 but contains no instrument bus drivers
and does contain a complete math package.
Both PDP 11/03 versions are compiled using a special floating point precompiler,
most of which was obtained from Ron Hardin of Columbus Laboratories and
are linked together with a set of subroutines referred to as the
file system interface (FSI). FSI interfaces
.I bite
with the file system and contains all necessary system utilities
such that, on the PDP-11/03,
.I bite
runs with no operating system in memory. Versions of FSI are available for
Digital Equipment Corp. RX-01 floppy disks and RL-01 hard disks. The "src"
directory contains the source code for both the special compiler and FSI.
The tape supplied is created by the  "cpio" command.
To load the tape, create a directory where it is desired to
place the software, mount the tape and issue:
.DF 1 0
	cpio -id < /dev/mt0
	where 0 is mag tape unit zero.
.DE
.H 1 "ADDING COMMANDS"
.H 2 "General Format Of bite Instructions"
In
.I bite
the users
source code is operated on by a special purpose editor 
.I bed.c
and stored as pseudo object code in large character array hereafter
referred to as the
.I "Program Buffer" .
Each instruction consists of four fields, which are arranged as follows:
.VL 8 5
.LI 1)
The first two bytes of each instruction are interpreted as an integer
which is the line number of the instruction.
.LI 2)
The third byte is an excess 200 tab counter. One or more tabs may be inserted 
between the line number and the command. Rather than physically storing
these tabs in the program buffer, all that is kept is a count of how many
tabs. A tab count of zero indicates that one blank is to be used. If the user
enters more than one blank, the additional ones will be stripped away.
Excess 200 means that 200 (base 8) is added to the actual tab count. This is
done so that byte three will either be zero or a number greater than 200 and
as such will never look like a valid
.I ascii
character. Since it is not a valid character, it can be and is used as a
beginning of line delimiter. Such a delimiter is needed whenever the buffer
pointer must be moved backwards to the beginning of the previous line as is
done by the "-" command.
.LI 3)
The fourth byte is interpreted as an integer hereafter referred to as the
.I opcode
which is an index into a table of function addresses hereafter called the
.I "Command Dispatch Table"
which is used to call the appropriate function via an indirect function call.
.LI 4)
The fourth field is variable in length beginning with the fifth byte
and continuing through a
.I NULL
byte. This
.I expression 
field is pure ascii text, except that all blanks are
stripped out of it and certain
.I keywords
(See 3.6.2)
are encoded into a single ascii character in the
range of 0 to 37 base 8 i.e. the control characters.
The
.I expression
field
is used to pass variable information to the function called.
.LE
.SP
Thus the name of a command (if, print, for etc.) is never stored
in the program buffer as 
.I ascii
text but rather is encoded into a single integer number. 
Internally then the
.I bite
command is executed by the C language statement
.DF 2 0
bascall(opcode)
.DE
where
.I bascall
is a macro defined within the
.I bite
header file
.I bas.h
and has the following form:
.DF 1 0
#define bascall(opcode) ((*cmdtbl[opcode].function)())
.DE
and
.I cmdtbl
is defined as follows:
.DF 1 0
struct {
	char *cmdtxt /*pointer to the command name */
	int (*function)(); /*pointer to the command function*/
	} cmdtbl[];
.DE
The expression field of the instruction is decoded by the function called.
Conversion of the first four bytes of each instruction is handled by
copying them into a "union" defined in
.I bas.h
as follows:
.DF 1 0
/* line number and command code structure aligned with 4 bytes for storage*/
union bascmd
	{
		struct
			{
				int linno;
				struct
					{
					char hibyte;
					char lobyte;
					} opcode;
			};
		char byte[4];
	};
.DE
Each time an instruction is fetched from the program buffer (See 3.3.1)
the first four bytes are copied into a golbally accessable
instance "inst" of this "union" defined
in "bed.c". This eliminates the need to be concerned with byte allignment
in the program buffer. Without this "union", each instruction would have to
begin on an even byte boundary so that the first two bytes could be referred
to as an integer. We pay a small execution time price, by requireing the
copy but save space in the program buffer by not requireing the byte allignment.
.H 2 Encode.c
To add an instruction to
.I bite ,
first edit the
.I encode.c
module to define the name of the function and add its name and address
into the command dispatch table.
The order in which commands are defined or appear in the dispatch table
is arbitrary and totally unimportant to
.I bite .
The list of definitions begins with the comment:
.DF 1 0
BASIC INTERPRETER COMMAND TABLE
.DE
.SP
simply follow the syntax of the other definitions and add in the new one.
The list entries in the dispatch table begins with the line:
.DF 1 0
struct COMMAND cmdtbl[]
.DE
Each entry in the dispatch table consists of two items:
.VL 8 5
.LI 1)
The name of the command as it will be written in the
.I bite
source statement.
.LI 2)
The name of the function to be called. These two names may or may not
be the same. Syntactically it's easier for human beings to read the program
if the names are the same but quite often other considerations such as
conflicts with
.I Unix
system names etc. force them to be different.
.LE
.SP
The next step is to write the function in C, then compile and link it
with
.I bite
using the appropriate shell script (See section 2).
.H 3 Example
We will take a look at the most simple command in
.I bite
"rem".
It's definition appears in the command table as:
.DF 2 0
int	no_op();	
.DE
It's entry in the dispatch table is:
.DF 2 0
{"rem",	no_op},
.DE
and the C language function is as follows:
.DF 1 0
no_op()
{
	return(0);
}
.DE
Obviously, "rem" is a
.I bite
no-op, i.e. it does nothing.
The "return(0)" is absolutely necessary however. An unpublished and therefore
often unknown to the new C programmer (veterans have learned it, usually
the hard way) fact is that although C does not require a "return" statement
at the end of a function, the value returned by a function without a
"return" statement is undefined i.e. it could be anything. Since the run time
monitor of
.I bite
tests the value returned by all functions, all must have a specific
returned value. A value of -1 indicates that a fatal error was detected
during the execution of the function and the run is terminated. A value
of 0 indicates that no fatal errors were detected.
Every time a source statement such as:
.DF 2 0
10 rem this is a comment
.DE
is encountered,
the "no_op" function is called.
It is up to "no_op" to evaluate the expression "this is a comment", however,
comments don't require any action so "no_op" just returns without doing
anything.
.H 2 fetch.c
The "run" function of
.I bite
uses an internal function called
.I fetch
to retrieve instructions from the program buffer. The call to
fetch is as follows:
.DF 1 0
fetch(n,&ptr);
.DE
where n is an integer with the following meaning:
.VL 15 5
.LI "n= -1"
Get the first instruction in the program buffer and set "ptr" ("ptr" is
defined as "char *ptr") to point to the next sequential line, which in
this case, will be the second line in the program buffer.
.LI "n= 0"
Get the line pointed to by "ptr" and set "ptr" to point to the next line.
.LI "n= line #"
Get line number "n" and set "ptr" to point to the next line.
If line number "n" does not exist, get the line with the next
higher line number or the last line of the program if there
are no higher line numbers.
.LE
.SP
.I Fetch
takes the following action:
.VL 8 5
.LI 1)
Copies the first four bytes into the instruction "union" (See 3.1).
.LI 2)
Sets a global character pointer "expr" to point to the buffer position where
the expression field of the fetched line begins.
.LI 3)
As noted above, updates the value of the argument "ptr".
.LI 4)
Sets a global character pointer "curptr" to point to the beginning
of the fetched line.
.LI 5)
Returns an integer with the following meaning: 0 if the operation was
100% successful, -1 if the operation was a total failure, -2 if a line
was found but not the one asked for.
.LE
.SP
Thus, the line number, opcode, tab count,
and expression field are globally available
after a call to
.I fetch .
It should be noted here that if the function being called via
.I opcode
must for some reason modify the character string pointed to by "expr"
it
.B must
copy the expression to a local place otherwise it will be permanently
modifying the program buffer which can have disastrous side affects.
In most cases, however, it is not necessary for the function to modify
the expression field, rather it need only evaluate what's there.
.H 2 "Run Time Monitor"
Before continuing the discussion of
.I "Expression Evaluation"
it will be helpful for the reader to understand a little of how
program execution is controlled.
The "run" function in
.I bite
which is sometimes referred to as
.I "The Run Time Monitor"
is first called by typing "run" instead of a line number followed
by an operation and expression. The
.I bite
editor
.I bed
which is in control when
.I bite
is first invoked recognizes it as a command for immediate execution
because it is not preceded by a line number and calls it. 
"run" first carries out a few housekeeping chores then fetches
the first instruction in the program buffer and executes it via
.I bascall.
Assuming that the first instruction does not return a -1 (fatal error)
"run" then begins sequentially fetching and calling each instruction in order.
This continues until
.I fetch
returns a non zero value (meaning end of file) or until a fatal error
is returned by one of the instruction functions.
In either case when "run" executes it's return instruction, 
control is returned to
.I bed.
.H 3 Example
Another rather short
.I bite
function
.I goto
will be used to demonstrate the features discussed so far. The source
of
.I goto
is as follows:
.DF 1 0
/* ///// goto routine ////// 
/* part of Drake Hawkins Basic
/*      R. B. Drake    /////
/* copyright Bell Telephone Laboratories ///
/*      Whippany N. J. ////////////
*/
#include <bas.h>
__goto()
{
	int savno,start;
	extern char *ptr,*curptr;
	savno = inst.linno;
	start=atoi(expr);
	fetch(start,&ptr);
	if(inst.linno != start)
	{
		error(savno,0);
		inst.linno = savno;
		return(-1);
	}
	ptr = curptr;
	return(0);
}
.DE
"savno" is used to save the current line number for possible use in
a call to the error printing routine (See 3.5). "start" is
used to hold the line number gleaned from the expression field. "*ptr"
is the global instruction pointer used by the run time monitor and
"*curptr" is set by
.I fetch
(see 3.3).
The statement
.DF 1 0
start=atoi(expr);
.DE
.SP
Converts the line number which must be in the expression field i.e.
.DF 1 0
10 rem the 40 in line 30 is the
20 rem line number in the expression field
30 goto 40
40 end
.DE
The statement
.DF 2 0
fetch(start,&ptr);
.DE
fetches the line asked for and the code which follows that checks to
see that the line fetched is indeed the one requested. If the line
fetched is not the one requested, then the "goto" refers to a
non-existent line in which case an error message is printed and a
-1 (function detected a fatal error) is returned to the run time
monitor. Otherwise, *ptr is set to *curptr and a zero is returned
to the run time monitor. Setting *ptr to *curptr primes
.I fetch
so that the next time it is called with a
.DF 1 0
fetch(0,&ptr);
.DE
which is what the run time monitor will do when it gets control again,
the line indicated in the "goto" is the one that will be fetched.
.H 2 Error.c
All printing of error messages in
.I bite
is done by calling a special error printing routine as follows:
.DF 2 0
error(linno,num);
.DE
.B Notes:
.VL 8 5
.LI 1)
If
.I linno
is greater than zero, it prints:
.DF 1 0
ERROR LINE
.I linno
MESSAGE INDICATED BY
.I num
.DE
.SP
where 
.I num
is an index number into an array of pointers to character
strings.
The array of pointers to character strings is defined within
.I error.c
as follows:
.DF
static char *mesg[]
{
	"REFERS TO A NON EXISTING LINE NUMBER", /* 0 */
	"UNRECOGNIZABLE OPERATION", /* 1 */
		.
		.
		.
		.
	"EXPRESSION YIELDS AN IMPOSSIBLE VALUE", /* 38 */
};
.DE
So that if the call to
.I error
was
.DF 2 0
error(10,0);
.DE
The message printed would be:
.DF 1 0
ERROR LINE 10 REFERS TO A NON EXISTING LINE NUMBER
.DE
The numbers inside the comment delimiters i.e. "/* 0 */" correspond
to the appropriate array index number to access that message. This is
indispensable to the programmer and of utmost importance that anyone
modifying
.I error.c
maintain that convention.
It should also be noted that since the messages are referred to by
their index number, the position of the current messages can not
be changed. for example if one message is deleted, all the messages
above it would be in the wrong position, and any calls to
.I error
with intent to print those messages would print the wrong one. 
Therefore any new messages must be added to the end of the list.
.LI 2)
If
.I num
is zero, the printing of the line number is suppressed. This feature
is used in commands being executed in immediate mode where there is
no line number to reference.
.LI 3)
Error numbers less than 100 are reserved for basic
.I bite .
That is, that portion of
.I bite
which is common to all versions. While error numbers 100 and greater
are used for user added instructions. This is convenient for the use
of conditional compile instructions so that several versions may be
kept in a single source file and the appropriate version can be compiled
by simply changing one
.I define
statement
in a header file.
For example, the current source tape of
.I bite
contains a conditional variable "LSX". If "LSX" is defined in the header
file
.I bas.h ,
then a version for the PDP-11/03 computer containing many test set
instructions is compiled. If "LSX" is not defined then a version
for the PDP-11/70 computer containing no test set instructions or
the error messages that go with them is compiled.
.LE
.SP
Printing error messages in this fashion as opposed to just printing them
via
.I printf
statements where they occur has a number of advantages.
.VL 8 5
.LI 1)
It gathers all of the error messages in one place where they are easily
seen and therefore there is no temptation to have the same or similar
messages appearing in several different places within
.I bite
which would be an unnecessary waste of core space.
.LI 2)
It standardizes the format in which errors are printed.
.LI 3)
If core space should become tight as it is in the LSI version of
.I bite ,
all error messages can be reduced to the printing of an error number
which the user would have to look up, thereby saving the space required
to store the messages. While this would be less convenient, it would be
less of a sacrifice than having to eliminate needed functions. Since
all the messages are in one place, only one routine
.I error.c
would have to be modified to accomplish this.
.LE
.SP
.H 2 "Parsing Expressions"
.H 3 prncpy
Many commands in 
.I bite
are followed by an expression field consisting of a list of items
separated by commas such as:
.DF 1 0
10 input a,b,c
20 on a goto 30,40,50
30 print a,b,c
40 stop
50 goto 10
.DE
The comma is always the delimiter separating items except when it is
within parenthesis as in:
.DF 1 0
10 read a(1,1),a(1,2)
.DE
In the above case, the only comma which is an item delimiter is the
one before "a(1,2)".
.I Prncpy
is a function designed to parse this type of construct. The call to
.I prncpy
is as follows:
.DF 1 0
char *prncpy();
char *pt
extern char *expr;
char to[80];
pt = prncpy(to,expr);
.DE
Assuming that "expr" is a pointer to the beginning of the expression
field (see 3.3),
.I prncpy
takes the following action:
.VL 8 5
.LI 1)
Copies the characters beginning at "expr" to the array "to" up to
but not including the first comma
which is
.B not
enclosed in parentheses.
If the end of string (NULL byte) occurs before such a comma is found,
the copy stops. In either case the string copied into "to" is NULL
terminated.
.LI 2)
Returns a pointer to the comma or NULL byte. In this way the calling routine
can use the returned pointer to determine what the delimiter was and if
there are more items to be parsed, it has a pointer to the beginning of the
next item by bumping the returned pointer by one.
.LE
.SP
.H 3 evalx
The inner workings of
.I evalx
will be described in a separate memorandum by J. P. Hawkins who is
the author of that particular function. However, it is of utmost
importance to this discussion and therefore will be described here
in enough detail to allow readers of this document to use it.
.P
The call to
.I evalx
is
.DF 1 0
char *pointer;
double evalx();
evalx(pointer);
.DE
Here, "pointer" must point to the beginning of a NULL or
.I keyword
terminated expression (See 3.1 note 3).
Where in this case,
expression is one of the following:
.VL 8 5
.LI 1)
A variable. Such as "a" or "b1" etc.
.LI 2)
A Subscripted variable. Such as "a(1,1)".
.LI 3)
Any numeric literal. Such as "10" or "551.325" etc.
.LI 4)
A function reference. Such as "sin(3.14159)" or "log(10)" etc.
.LI 5)
A mathematical expression consisting of any or all of the above
connected by mathematical operators. Such as
.DF 1 0
a+a(1,1)*551.325/sin(3.14159)
.DE
The keywords and their associated ascii codes are:
.DF 2 0
.B "KEYWORD		ASCII CODE(octal)"
goto		01
go to		02
then		03
to		04
step		05
<>		06
<=		07
=<		10
<		11
>=		12
=>		13
>		14
=		15
gosub		16
.DE
.SP
.I Evalx
takes the following action:
.VL 8 5
.LI 1)
Evaluates the expression and returns its numeric value in double precision
floating point form.
.LI 2)
Sets a global character pointer "eoexpr" (mnemonic for "end of expression
pointer") to point to the character at the end of the expression.
.LI 3)
Unlike other
.I bite
functions,
"Evalx" can not return a "-1" if an error occurs during the expression
evaluation because "-1" is a perfectly valid value. So, if an error occurs
during expression evaluation,"evalx" prints an error message,  sets the
stop flag "stpflg" and returns a value of zero.
This will cause program termination when control
is returned to the run time moniter.
.H 2 "Calling command functions as though they were internal functions".
Very often, one of the
.I bite
commands already does exactly whats needed in the way of expression
parsing. For example, consider the instruction
.DF 2 0
10 for i=1 to 100
.DE
The first thing that the
.I for
function must be concerned with is setting the variable "i" equal to
1. The
.I let
command already deals with expressions of this nature and the pointer
"expr" already points to the "i" at the beginning of the
expression field (See 3.3). So one of the first statements
in
.I for
is
.DF 2 0
let();
.DE
Since
.I let
uses
.I evalx
to evaluate that part of the expression to the right of the "=", and
since
.I evalx
terminates at
.I keywords
(in this case the "to"), on return from the call to
.I let
the variable "i" will have been set to 1 and the pointer
.I eoexpr
will be pointing to the ascii character "004" which is the
encoded "to". The
.I for
function then proceeds to deal with that part of the expression beyond
the "to" and of course uses
.I evalx
again to accomplish that.
.H 3 Example.
The
.I on
function is fairly short and provides a good sample of the features
discussed thus far. The source code is as follows:
.DF 1 0
/* routine to handle on goto */
/* R.B. Drake 4/26/79 */
#include <bas.h>
extern int stpflg;
double evalx();
char *prncpy();
extern char *expr,*eoexpr;
on()
{
	int i,j;
	char buffer[80];
	char c;
	if((j = evalx(expr)) <= 0)
	{
		error(inst.linno,38);
		return(-1);
	}
	/* "stpflg" is set by the interrupt routine "stopl"
	 * when the user hits the "del" or "rubout" key.
	 * When "stpflg" becomes set, the "run" is to be
	 * terminated. It is necessary to test it here because
	 * this routine can remain active for an unlimited number
	 * of instructions and may be called recursively. 
	 * i.e. "gosub" does not return until it encounters a
	 * "return" instruction in the program buffer and there
	 * is nothing which prevents the occurrence of other
	 * "gosub's" or other "on" statements within the range
	 * of "gosub".
	*/
	if(stpflg == 1)
		return(-1);
	if((c= *eoexpr++) > '\\003' && c != '\\016')
	{
		error(inst.linno,8); /* expression syntax */
		return(-1);
	}
	for(i=0;i<j;i++)
	{
		eoexpr = prncpy(buffer,eoexpr);
		if(*eoexpr++ == '\\0' && i != (j-1))
		{
			error(inst.linno,38); /* not enough references*/
			return(-1);
		}
	}
	expr = buffer;
	if(c != '\\016')
	{
	if(__goto() < 0)
		return(-1);
	return(0);
	}
	if(gosub() < 0)
		return(-1);
	return(0);
}
.DE
Recall that the format of the
.I on
statement in
.I bite
is
.DF 2 0
line# on expr goto line#,line#......,line#
or
line# on expr gosub line#,line#......,line#
.DE
Where "expr" will be evaluated and truncated to an integer
indicating which of the "line#'s" to go to.
.P
Referring now to the above source code, note that the very first
executable instructions is
.DF 2 0
if(j = evalx(expr) <= 0)
.DE
Whatever the form of "expr", (See 3.6.2)
.I evalx
will return its value as a double precision
floating point number. However, since "j" was declared integer,
C will automatically truncate it. Of course if it's value is negative,
there is an error because negative numbers have no place in this context.
.I Evalx
will return at the first
.I keyword
which in this case must be either "goto" or "gosub". This syntax
is tested by the statement
.DF 2 0
if((c= *eoexpr++) > '\\003' && c != '\\016')
.DE
Where '\\003' is the
.I ascii
character for "goto" and '\\016' the "gosub" (See 3.6.2).
Note that the "++" in the above statement pushes the pointer
beyond the
.I keyword
and makes it point to the beginning of the list of
line numbers. Remember that the
.I bite
editor
.I bed
strips all blanks and tabs out of the expression field.
Therefore, there is no need for any of the command functions
to worry about white space.
The "for" loop beginning at
.DF 2 0
for(i=0;i<j;i++)
.DE
takes care of copying the j'th line# in the list
into the array "buffer".
This is accomplished
by calling
.I prncpy
(See 3.6.1) "j" times, while making sure that the end of the
expression string is not reached before the j'th line number is
found. Now, "buffer" contains a character string representing
a line number which is to be the object of the "goto" or "gosub".
Since
.I bite
already has functions to handle "goto" and "gosub" all that is
necessary is to set the pointer "expr" to point to "buffer"
and call the appropriate "go" routine. 
.I Bite
functions don't
care whether
"expr" points into the program buffer or somewhere else as
long as the string it points to has the appropriate syntax.
.H 2 "Dealing With Variables" .
In the discussion of the "for" command in section 3.7, the issue
of variables was avoided by allowing the
.I let
function to handle them. It is recommended that, where ever possible,
that procedure be followed. However, there may well be situations
where that is not practical. There are several internal functions
which are used for this purpose as follows:
.DF 1 0
class(from,to)
getvar(varnam,value)
agetvar(varnam,value)
putvar(varnam,value)
aputvar(varnam,value)
.DE
A brief description of the calling protocol and the action of each
of these will be given here. The details on how they work will be
covered in another memorandum.
.H 3 class(from,to)
.SP
Calling protocol:
.SP
.DF 1 0
char *from;	 /* pointer to an expression string */
char *to;	/* pointer to a character array large enough
		 * to hold one element of the expression */
class(&from,to);
.DE
.I Action:
.VL 8 5
.LI 1)
Copies one element of the expression into the array "to" and
NULL terminates the string. Where an expression element is
a mathematical operator,
a numeric literal, a variable name, an array name or a function name.
(See 3.6.2).
.LI 2)
Bumps the pointer "from", leaving it pointing to the first character
of the next expression element.
.LI 3)
Returns an integer, which is a code telling the calling routine
what kind of element was copied. These integers are defined as
manifest constants in the header file
.I bas.h
as follows:
.DF 1 0
#define	OPCLASS	1	/* operator field ^ * / + - ( */
#define	NMCLASS	2	/* numeric field */
#define	VRCLASS	3	/* variable name */
#define	VACLASS	5	/* variable array name */
#define	FNCLASS	6	/* function name */
.DE
.H 3 "Getvar and Agetvar"
.SP
Calling protocol for getvar:
.SP
.nf
char *string;	/*pointer to a string containing a variable name*/
double value;	/*place to store the variable's value*/
getvar(string,&value);	/* call */
.SP
Calling protocol for agetvar:
.SP
Same as for getvar except "string" must point to the name of an
array variable name.
.SP
.fi
These routines find the address of the data associated with the variable
or array name, retrieve the data from the data buffer and store it in
the location specified by their second argument.
.H 3 "Putvar and aputvar"
.SP
Calling protocol for putvar:
.SP
.nf
char *string; /* pointer to the name of a variable*/
double value; /*data to be stored*/
putvar(string,value);
.SP
Calling protocol for aputvar:
.SP
Same as for putvar except "string" must point to the name of an
array variable.
.SP
.fi
These routines find the address of
the storage area in the data buffer where the data associated with the
variable name is to go. In the case of
.I putvar
if no such storage area exists, one will be allocated. In the case
of
.I aputvar ,
a storage area must have been reserved by a previous "dim" statement
or a fatal error will occur with an appropriate error diagnostic 
message.
.H 4 "Example."
Perhaps the most complete example using all of these functions is in
"evalx.c". However, "evalx" is long and complex therefore we will
use the simpler "let" routine which hopefully will demonstrate the
point without clouding it by complexity.
.DF 1 0

#include	<bas.h>
/*
#define	skip00() {while(*lexptr == ' ' || *lexptr == '\t') *lexptr++;}
*/
#define	skip00()	{}	/* does nothing */

let()
{
	int	type;	/* field type */
	char	varnam[40];
	float	evalx();
	float	value;
	char	field[40];
	char	*lexptr;	/* string pointer for let */

	lexptr = expr;		/* set text pointer */

	type = class(&lexptr,field);	/* get field type */
	if((type != VRCLASS) && (type != VACLASS)) /* if not var */
	{
		error(inst.linno, 3); /* ill var name */
		return(-1);
	}
	strcpy(varnam, field);	/* copy variable name */
	skip00();
	if(*lexptr++ == '\15')	/* if code for '=' */
	{
		value = evalx(lexptr);	/* evaluate right side */
		switch(type)
		{
			case VRCLASS: /* regular variable name */
				putvar(varnam, value);
				return(0);
				break;
			case VACLASS: /* subscripted var field */
				aputvar(varnam, value);
				return(0);
				break;
			default:
				break;
		}
	}
	else
	{
		error(inst.linno, 8); /* missing = */
		return(-1);
	}
}
.DE
"Let" of course expects to see an expression field of the form
"variable = mathematical expression" (See 3.6.2 for the definition
of these terms). Therefore the first thing it does is make sure that
the expression begins with a variable or array name.
This is done by:
.DF 1 0
	type = class(&lexptr,field);	/* get field type */
	if((type != VRCLASS) && (type != VACLASS)) /* if not var */
	{
		error(inst.linno, 3); /* ill var name */
		return(-1);
	}
.DE
After the call to "class", "type" should be equal to either
"VRCLASS" or "VACLASS", "lexptr" should be pointing to an "="
and "field" will contain the null terminated name of the variable
or array. (in the case of an array, the name includes the parenthises
and what's inside them i.e. "a(x,y)" ). The variable or array name
will be needed later so is copied to a new temporary location "varnam"
by:
.DF 1 0
strcpy(varnam, field);	/* copy variable name */
.DE
Then a test is made to be sure that "lexptr" is pointing to an "="
and at the same time "lexptr' is bumped so that it will point to the
first character of the mathematical expression by the statement:
.DF 1 0
if(*lexptr++ == '\15')	/* if code for '=' */
.DE
If "lexptr" was not pointing to an "=" an error message is issued
and a fatal error indicated by:
.DF 1 0
else
{
	error(inst.linno, 8); /* missing = */
	return(-1);
}
.DE
But assuming that everything is allright so far, the mathematical expression
is evaluated by:
.DF 1 0
value = evalx(lexptr);	/* evaluate right side */
.DE
Now all that remains is to store "value" in the appropriate variable or
array element. This is handled by:
.DF 1 0
switch(type)
{
	case VRCLASS: /* regular variable name */
		putvar(varnam, value);
		return(0);
		break;
	case VACLASS: /* subscripted var field */
		aputvar(varnam, value);
		return(0);
		break;
}
.DE
Admitedly this example has not used either "getvar" or "agetvar",
but they are so similar to their "put" sisters that such an example
does not seem necessary.
.H 1 "ADDING FUNCTION SUBROUTINES"
Function subroutines differ from commands in that they appear in the
expression field and are replaced during expression evaluation by the
value they return. The syntax is:
.DF 1 0
anyname(arg)
.DE
There is no hard limit as to the number of characters that can be
in the name but make sure it can't be mistaken for an array name 
(i.e. it must be at least two characters long and 
the second character must
not be
a digit).
"Arg" may be nothing or any expression acceptable to "evalx" (see 3.6.2).
Only one argument is permitted. Adding a function subroutine to
.I bite
is very similar to adding a command except that the function declaration
and dispatch table are in "evalx.c", just preceeding the routine called
"mathcall". "Mathcall" is the routine which actually handles the calling
of function subroutines. To add a function subroutine, first write your
function in "c" being sure to declare it as a "float" function and if
there is to be an argument, the argument must also be declared as float.
Then declare its name along with the others (i.e. float exp()) and add
it's name into the dispatch table "mathtbl" (i.e. {"exp", exp},).
.H 2 "Example".
Perhaps the simplest example of a function subroutine, so simple in fact
that it was included right in the "evllx.c" file instead of as a seperate
file, is "int". "Int" simply truncates a floating point number to an integer
value. Since there are no actual integers in
.I bite
the value is still in floating point form but the fractional part of the
number will be zero. "Int" appears as follows:
 "Int" appears as follows:
.DF 1 0
float _int(num)
float num;
{
	long	trunc;	
	trunc=num;
	num=trunc;
	return(num);
}
.DE
This routine simply makes use of the automatic type conversions and truncation
provided by the "c" compiler. That is, "num" is truncated and
converted to a long integer
by the statement "trunc=num" and then from that integer back to a floating
value by the statement "num=trunc". It's use in bite is as follows:
.DF 1 0
20 b = (1+int(1.1234))/2
.DE
After executing the above instruction the value of "b" will be 1.
i.e. (1+1)/2.
.H 1 "Conclusion"
One of the reasons that UNIX is becoming very popular throughout the
computing industry is that it's source code is aviailable and it is
written predominantly in a semi high level language, "C". Users are
no longer stuck with the features that the system designers decided
that they should have. Rather the user is free to add or subtract
whatever seems appropriate for his task.
.I Bite
is no exception, we feel that we have provided a strong foundation for
a good interprative language but make no pretence that we have thought
of everyting.
We invite users to dig into it, improve it and add to it.
The author hopes that this memorandum provides enough information to make
that task easy so that no one will again feel the need to start from
scratch as we did.
.H 1 ACKNOWLEDGEMENTS
.I Bite
is entirely the work of the author and J. P. Hawkins. However, two people
helped us immeasurably by being our "friendly users". They began using
.I bite
when it was only half finished and far from debugged. Many thanks to
Nick Episcopo and Don Jackowski.
Their patience is much appreciated and the evidence of their
suggestions is everywhere.
.CS
.TC
.SG UNIX

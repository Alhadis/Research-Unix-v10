.de aH
.nr a +1
.nr P 0
.PH "'''APPENDIX \\na - \\\\\\\\nP'"
.SK
.HU "\\$1"
..
.ND "September 6, 1979"
.TL "20239-7048" "40295-2"
BITE users guide.
.AU "J. P. Hawkins" JPH WH 2425 4610 8C-001
.TM 79-2425-
.AS 1
.I BITE
(BASIC Interpreter for Testing and Engineering)
is a BASIC language interpreter
designed for use in automated testing
controlled by PDP-11 microcomputers.
The interpreter implements an extended instruction set designed for
instrument control using the IEEE 488 Instrument Bus.
.P
This memo describes the syntax of the language and
describes useful programming techniques.
.AE
.OK "BASIC" "Interpreters" "BITE" "Automatic Testing"
.MT 1
.SA 0
.H 1 INTRODUCTION
.H 2 "General Description"
.I BITE
is a
.I BASIC
language interpreter designed for use with
automated test equipment. The interpreter runs on
PDP-11 mini- and micro-computers using the
UNIX*
.FS *
UNIX is a Trademark of Bell Laboratories.
.FE
operating system.
.I BITE
is distinguished from other
.I BASIC
interpreters in the following ways:
.BL 4
.LI
.I BITE
is written in the 'C' language.
It is, therefore, portable and can be installed and used
on any system with a standard 'C' compiler.
.LI
The interpreter provides an extended set of commands and functions
for controlling and reading electronic instruments using the IEEE 488
Buss. The extended instruction set can be expanded
by the user who is knowledgeable in 'C' programming by implementation
of "custom commands" \[ref Drake\].
.LI
.I BITE
can be executed from a script
running on the Bourne Shell of UNIX.
Thus the string manipulation of the Shell
and the control and computational capabilities
of
.I BITE
are combined to provide a uniquely powerful system.
.LI
.I BITE
accepts
.I BASIC
language programs using the original Dartmouth
syntax with little or no modification required.
Those features described above which differ from standard
.I BASIC
are extensions of
.I BASIC
rather than exceptions to the syntax rules.
.H 2 "Micro and Mini Versions"
There are two versions of
.I BITE.
One version is designed for the PDP-11/70 UNIX environment and
the other version is a "standalone" program which runs in the PDP-11/03
and is invoked from the LSX version of UNIX.
.BL 4
.LI
The PDP-11/70 version has complete interface capabilities with the
Bourne Shell as described below and all math functions are available.
.LI
The other version is designed for the PDP-11/03 micro-computer.
This version contains the extended instruction set for instrument control.
Differences between this and the host version include omission of verbose
error messages, Shell and system call facilities, omission of some math
functions and reduced user working storage. The omissions of features
are due to the drastically reduced memory availability on the PDP-11/03
as compared to that of the PDP-11/70 with memory management.
Since most core space is used up, the PDP-11/03 version is "standalone"
(operating system not resident) with a skeleton version of UNIX for
file I/O. This version must be loaded with a special loader program
.I BITEX
which is included with the software release package.
.H 2 "Scope of This memo"
In the following, the syntax of
.I BITE
is described. When a command differs in the two versions, this fact is noted
explicitly. Programming techniques, including control of
.I BITE
by the Shell, are described.
.H 1 Conventions
.VL 20 5
.LI "This Memo"
All things enclosed in [] are optional.
.LI expr
Any algebraic expression which could be a constant, variable, math
function or a combination of same, separated by arithmetic operators
as in: a+b*3.14*(4.4+c2*sin(b+s))+a(2,2)
See "variables" and "math functions" below.
.LI Operators
+,-,*,/ or ^ for addition, subtraction, multiplication, division or
exponentiation in order of lowest to highest precedence. + and - have
the same precedence and * and / have the same precedence.
Parenthesis () around expressions forces the contents to be higher precedence
than all parts of the expression outside those parenthesis.
Note also that when the - is used as a unary it maintains its low precedence,
hence the expression -2^2 yields -4 instead of 4. In all cases a good rule
of thumb to insure precedence is to enclose the part of high precedence in
parenthesis, thereby (-2)^2 yields 4.
.LI Relationals
<, >, =, <=, >=, <> for less than, greater than, equality, less or equal,
greater than or equal and not equal.
.LI "Source Path"
When reference is made to a
.I BITE
source file (i.e. the
.I old
and
.I load
command), two directories are searched, the first being the current
directory and then /usr/lib/bites which is a "pool" where shared programs
should be stored. The /usr/lib/bites directory is analogous to the
/usr/bin directory in
UNIX.
.LI "Source Program Name"
.br
The source program name is twelve or less characters suffixed by a .b .
.LI Statement
A basic statement consists of a line number (integer value between 1 and
32767) followed by a command, space and operand which follows the
syntax governed by the command as in:
.br
100 print "Hello World"
.br
A statement can be typed without a line number in which case it will
execute immediately. This is true for all commands, but doesn't make
sense for some commands such as
.I for.
Immediate execution is handy
for diagnostic purposes such as: print a, to find out what the value
of 'a'.
.LI Strings
Sequences of ASCII characters delimited by double quote characters
at the beginning and the end.
.LI Variables
All variable names are either a lower case alpha character (a-z)
or a lower-case alpha character followed by an integer (0-9).
Arrays have the same name convention as regular variables and take
the form varname(expr1,expr2,expr3....expr10) where expr1-expr10 are the
dimension
attributes of the array and can take the form of any legal expression
(including another array) as in a(b(2,2),x).
.SK
.H 1 Commands
.H 2 "Standard Commands"
.VL 20 5
.LI "bye or q"
Exit
the interpreter.
Typing the control/D key will also exit the interpreter.
.LI "call name,line#"
.br
.I "Call Overlay Subroutine."
Name is the name of a file (name.b) containing a subroutine.
The subroutine must be sequenced such that line# is the
first line# in the file.
The first line must be "line# rem name" where "name"
must match the one in the
.I call
statement.
.I Call
checks the line "line#" to see if the subroutine
has already been loaded. If it has, a "gosub line#" is
executed.
If the subroutine is not already loaded, it loads it
and then does a "gosub line#".
.LI com[mon]
Preserve
variables for subsequent
.I run.
Issue of the
.I run
command
otherwise de-allocates all variables.
.LI "con [line#]"
Continue normal execution from single step mode.
See
.I sing
command.
.LI "data (expr),(expr),(expr),........."
.br
The
.I data
statement is a string of defined constants or expressions referred
to by the "read" statement. Unlike most BASIC interpreters, the data is
stored only in the form of text strings which allows the read statement to
evaluate expressions as well as constants.
.LI "del[ete] lownum [, highnum]"
.br
Delete line-number specified if only lownum given. Delete all lines
between lownum and highnum if both are specified.
See the
.I undo
command.
.LI "dim variable(expr1,expr2,.......,expr10)"
.br
Allocate space and define the dimensional characteristics of
subscripted variable.
.LI end
Define logical end of program. Causes termination of current
.I run.
.LI expunge
Force all variable space, including subscripted variables to be freed.
Or de-allocate used variable space.
.LI f
Typing 'f' causes the currently referenced
file (if any) to be displayed.
.LI "for - next"
Cause code enclosed by this combination to be executed under the
conditions specified in the
.I for
statement as in: for variable = expr1 to expr2 [step expr].
.LI "gosub line#"
Goto subroutine, resume from following statement after
.I return
encountered.
.LI "goto line#"
Force execution to continue starting at the line# specified.
.LI "if (expr1) relational (expr2) then line#"
.br
.br
Redirect program flow to line# if expr1 is related to expr2 by the
specified relational.
The
.I then
in the
.I if
statement can be optionally replaced with
.I goto
.I "go to"
or
.I gosub.
The
.I if
statement can also take the form:
.br
if (expr1) relational (expr2) then var = (expr)
.LI "input [_fildes]var1[,var2,var3,....]"
.br
Prompt for input and assign inputed value to variable.
If 's' is typed program is halted.
.LI "[let] variable = expr"
.br
Assign the value of expr to variable.
The let is optional.
.LI "l[ist] [lownum [, highnum]]"
.br
List the text in working storage. If lownum is given then only that number
is listed. if lownum and highnum are specified, then a listing is
displayed between the given statement numbers.
.LI "load [program name]"
.br
Same as the
.I old
command, except working storage is not cleared.
.LI "mov startnum, endnum, newnum [,increm]"
.br
The mov command causes the lines beginning with
.I startnum
and ending with
.I endnum
to be moved (ie. resequenced) to the line beginning with
.I newnum
and incremented by
.I increm.
The default value for
.I increm
is 10.
All references to the moved lines are updated. The user is responsible to
see that line numbers associated with moved lines do not conflict with
existing lines which will cause loss of program text.
.I mov
is similar to
.I reseq
(see below) except that only the specified lines are resequenced.
.LI n
List the next 23 lines. Useful for paging through a listing on a CRT.
.LI new
Clear program working storage for new program to be typed.
.LI "old [program name]"
.br
Clear user space and load program. If old is typed with no argument
it will prompt the user for a program name if not defined or load
the last defined program name.
.LI "on (expr) goto line#, line#,....."
.br
Is a selective goto with multiple line number targets. The target
branched to depends on the value of expr which is truncated.
Control is passed to the first line# specified after goto if the
value of the expression is 1. Control passes to the second line#
if the value is 2, the third if 3 and so on.
.LI "on (expr) gosub line#, line#,......"
.br
Same action as
.I on-goto,
except action taken is that of
.I gosub.
.LI pause
Causes execution to be suspended until a "newline" or "return" is typed.
This is useful for programs which need to be continuously in
.I run,
but
need to allow a time for user action i.e. unit insertion.
.LI "pr[int] [_fildes](expr's, quoted strings or tab operators)"
.br
The print statement is a limited format display statement in which
expressions are evaluated and displayed along with quoted literals.
The tab(expr) operator causes the print head to move to the absolute
column position computed by expr provided the current head position
is smaller. The specifiers must be separated by one or more commas
or semicolons.
.LI "printf (format string)[,expr1,expr2,.....,expr10]"
.br
This is an interpretive implementation of the UNIX 'C' library
routine, printf. It is, however restricted to only the floating
point format control specifiers 'f' and 'g'. Use of any of the
other specifiers such as 'o', 'd' or 's' will give erroneous results.
Print controls such as \\b (backspace), \\n (newline), \\r (return)
or \\t can also be used. The printf format was chosen in lieu
of the usual
.I "print using"
command because it was felt that
printf is not only a 'C' language standard but easier to use
than
.I "print using".
.br
Usage Example:
.br
100 printf "Var a=%2.2f\\tVar b=%g.\\n",a,b
.LI randomize
Causes
.I rnd
statement to start at an "unpredictable" value.
.LI "read var1,var2,var3,............"
.br
The
.I read
statement causes data to be assigned to each variable in the
list from the constants or expressions contained in
.I data
statements. The
reading starts at the location of the data pointer.
The data pointer
points to the last data field accessed if a read was done or to the first
data field in the first data statement if the
.I restore
statement is issued
or the program is re-run.
.SK
.LI rem
The remark statement causes no operation in 
.I BITE
but may be followed by any string of characters for the purpose of commenting
a program. Unlike compiler languages, remarks do take up program buffer space;
however, they are of paramount importance in making a program readable by
human beings and are therefore strongly recommended.
.LI "reseq [startnum [, increm]]"
.br
The resequence command causes the statement numbers and all references
to them (such as if's goto's, gosub's, etc) to be resequenced starting
at
.I startnum
and incremented by
.I increm.
If startnum and/or increm are omitted, the default values are 10 and
10 respectively.
.LI restore
Restores the data pointer to the first field of the first
.I data
statement.
.LI return
Return from subroutine called by
.I gosub
statement.
.LI "run [program name]"
.br
Run basic program specified. If no argument is given,
.I run
attempts to
execute whatever is currently in working storage.
.LI "s line#/old-string/new-string[/]"
.br
Substitute in line line# the new-string for the old-string. The last delimiter
is optional, unless new-string is null in which case it is desired that
old-string merely be removed.
See the
.I undo
command.
.LI "sing [line#]"
Enter the single step mode starting at the line# specified or at
the first line of the program if no line# is specified.
In single step mode an instruction is executed and then the prompt '^'
is displayed.
At this time the user may enter
any command (i.e. print) or hit the "return" key to execute the next
instruction.
See the
.I con
command.
.LI size
Causes amount of storage used and remaining or free space in 
decimal number of bytes.
.LI stop
Stop execution of program.
.LI "save [program name]"
.br
Save the contents of working storage in file-name specified by program name.
If no program name is given, last referenced file-name is used. If no file
name was referenced, the user is prompted for a name.
.LI undo
Undo last
.I s
command or
.I "single line deletion"
.LI ! (any shell command string)
UNIX shell command invocation allows system commands to be
executed from the interpreter.
This command is
.I "not included"
in the LSI-11/03 versions of BITE.
.SK
.H 2 "File Commands"
The file commands: append, openi, and openo are followed by one or more
file-names separated by commas, each file-name being no more that
14 characters long. Files are assigned to designators (integer values
between 1 and 4 inclusive) in the order that they are open.
All commands
such as
.I print
and
.I input
which refer to a file use the designator
number preceded by a '_' character to refer to that file
as in: 100 print _1"hello world" or 100 input _3a(x,y) .
.VL 20 5
.LI "append file1[,file2,.....,file4]"
.br
If file exists open for output cause new data to be appended. If file
does not exist, the named file is created.
.LI "openi file1[,file2,.....,file4]"
.br
Open file for input. File must exist.
.LI "openo file1[,file2,.....,file4]"
.br
Create named file(s) and open for output. If named files exist, the old
data is destroyed.
.LI "close _fildes"
.br
Close file associated with file designator.
.LI closeall
Close all files input and output.
.H 2 "ATS Instrument Commands (Extended Instruction Set)"
These instructions are those which were implemented for
the
.I "Production Test Set."
.VL 20 5
.LI "buspr 'busadr(text and expressions)"
.br
Buspr is merely an extension of the print statement which allows
the print string which would otherwise be displayed on the tty
to be sent via the IBV-11 bus to the bus address specified by "busadr".
The ' preceding busadr distinguishes the following character from anything
other than a single character to be interpreted as an address.
The address character can be
.I "any ASCII character"
except ones which are possibly interpreted by the system as control
characters such as back-space.
.br
Usage Example:	100 buspr '6"F2R";r
.LI "cmd string"
Send character string over IBV-11 command lines.
.LI delay num
Causes a delay of num 60ths of a second where num is an integer.
.br
Usage Example:	100 delay 120		(delay 2 minutes or 120/60ths sec)
.LI dvminit
Initialize Digital Voltmeter.
.LI "dvms function, range"
.br
(for HP 3455A digital voltmeter)
Digital voltmeter set command, where: function is 'ac', 'dc' or 'ohms'
and range is .1, 1, 10, 100, 1k, 10k or 'aut'. i.e.
.br
Usage Example:	100 dvms dc,1k
.LI "hprintf (format string)"[,expr1,expr2,...,expr10]"
.br
(for HP 5150A Thermal Printer)
Formated print to strip printer. Syntax rules are the same as
.I printf.
Strip printer is 20 columns wide, anything past the 20th column is
truncated.
.LI "lodset lodnum,mode,value"
.br
(for POWER DESIGNS  X-510 & TRANSISTOR DEVICES  DLP 50-60-1000
electronic loads)
Set load. Where lodnum an integer describing which load referred to,
mode is the manner in which the load is set and value is an expression
describing the current or resistance the load was set to depending on the
mode. Mode is a single character 'r', 'R', 'i' or 'I' where 'r' is resistance
mode (value in ohms) and 'i' is current mode (value in amperes). Small
letter causes a hunt for the value and capital causes set on first try.
.LI "relay function, relnum1[, relnum2, relnum3,...]"
.br
(for HP 6940B MULTIPROGRAMMER)
Set multiprogrammer relays. Function is m (make), b (break) or c (clear).
Function is followed by all relay numbers to be acted upon which may
be expressions or variables. The clear function when not followed by
anything, simply means open all relays. When followed by relay numbers,
clear means all relays are open EXCEPT the ones specified.
.br
Usage Example:	100 relay m,10,20,21,a,b,rnd(10)
.LI "ps psno,voltage,current limit,overvoltage"
.br
(for KEPCO ATE 75-15M, ATE 150-7M, ATE 55-1M & JQE Power supplies)
Set power supply parameters. Psno is an integer representing the
power supply number describing which power supply is to be used,
voltage, current limit and overvoltage are self explanatory. Each
one of the parameters may be a legal algebraic expression so that
they may be controlled by the program.
.br
Usage Example:	100 dvms 1,10,1,11 or 100 dvms n,v1,i1,v1+1
.LI "scan scanner-channel"
.br
(for HP 3495A SCANNER)
Set 3495A Scanner channel to number specified.
.H 1 Functions
.H 2 "Standard Functions"
.VL 20 5
.LI abs(expr)
Absolute value.
.LI atn(expr)
Arc-tangent.
.LI cos(expr)
Cosine.
.LI exp(expr)
Natural exponential.
.LI int(expr)
Integerize or truncate fractional part of result of expr.
.LI log(expr)
Natural log.
.LI rnd(expr)
Return random number between 0 and evaluated expr.
.LI sin(expr)
Sine.
.LI sqr(expr)
Square root.
.H 2 "Instrument Functions (Extended Set)"
.VL 20 5
.LI btn(expr)
Button function returns non-zero if control button number (expr) is
depressed.
.LI dvmr()
Return digital voltmeter reading.
.LI error()
Return 1 if last instrument command caused instrument error, otherwise
return 0.
.SK
.H 1 "Modes of Operation"
.H 2 "Editor or Idle Mode"
When the
.I BITE
interpreter is invoked with no argument, a prompt '*' appears meaning
that the interpreter is waiting for the user to enter something from
the keyboard.
.I BITE
is then said to be in the
.I "Editor or Idle mode."
.P
Editing is accomplished as it is in any BASIC language interpreter
in that lines are entered by typing a line-number followed
by the statement and removed or deleted by merely typing the line-number.
Listing is accomplished with the
.I list
command (explained under "Standard Commands").
In addition to the above, it is possible to list single lines by typing
the return key in which case the program is listed one line-at-a-time,
starting at the first. When the last one is reached, the sequence starts
at the first line again.
At any time it is also possible to type the '-' symbol to "backup" a
line-at-a-time.
Other editing facilities are
.I "s, delete,"
and
.I reseq
also explained
under "Standard Commands".
.P
.H 2 "Run Mode"
If the
.I run
command is typed and a program
is currently in user storage, the program begins execution,
starting with the first line of the program, then executing each
line in order of line numbered sequence. The sequence of execution
is altered by program flow control statements like
.I "if, for-next"
or any statement containing a
.I goto.
.H 2 "Immediate Execution Mode"
Immediate execution is accomplished by typing a command without
preceding it with a line number.
Although this is possible with all commands, it doesn't always
make sense.
For example, using commands that control program flow in
immediate mode is unlikely and often disastrous.
.P
Immediate mode is designed so that the user may get immediate action
as in the command
.I run
or
.I "print a".
Some commands are almost always used in immediate mode such as
.I "q, delete, expunge, load, list, old, reseq, save,"
etc.
.H 2 "Single Step Mode"
Single step mode is entered with the
.I sing
command and
exited with the
.I con
command.
During this mode, one may find "BUGS" in the program
by observing the program flow or sequence or examining the values
of variables at given points in the program
to see if they have the expected values. See
.I sing
or
.I con
under the "Standard Commands" section of this paper.
.H 1 "Interruption of program"
At times it becomes necessary to escape from an endless loop or
abort an
action such as
.I list
before it completes. To cause such an interruption,
the (DEL) or (RUB) key is typed.
.H 1 "Programming Techniques and Tools"
.H 2 "Program Segmentation"
In situations of limited memory space as in the case of the LSI-11/03
it becomes impossible to fit large programs in storage at any one time
time. It then becomes necessary to write the program in pieces or segments
each of which must be loaded separately as needed.
Segmented are handled by "chaining" or "overlaying".
.H 3 "Chaining"
.P
Chaining is accomplished by insertion of the
.I run
command in the
program text. If a program is to be split into say, program1 and
program2 then by simply inserting the line "run program2" as the
last executed statement of program1, program2 is now chained to
program1. Upon completion of program2, if it is desired to reload
program1, the last executed statement should be "old program1" which
will clear user space and reload program1.

.SK
It must be noted that execution of a
.I run
causes variables to be
wiped out. To preserve variables from one program to the next, the
statement
.I common
must be executed prior to the chaining
.I run
statement.

The chaining process may go on indefinitely, the only expense being
some time delay for each program load.

The chaining point must sometimes be strategically chosen so that
it will not occur during an instrument action or some time critical
part of the program.
.H 3 "Overlaying"
.P
Overlaying is best accomplished with the
.I call
command. This technique is particularly useful where core space is minimal
and lends itself to keeping programs modular. The
.I call
command has a built-in feature which prevents loading a module
which is already resident.
In a line numbered language, overlay segments are delimited by
line number boundries rather than address boundries as is true in
machine level programming. The following is an example of a simple
implementation of an overlay:
.nf

100 rem THIS IS THE ROOT SEGMENT OF THE PROGRAM
110 rem The "root" segment remains resident and usually contains
120 rem all the commonly called subroutines
130 call pscheck,2000		\fIcall in power supply check overlay\fR
140 call loadck,2000		\fIcall in load check overlay\fR
150 call loadck,2000		\fIcall load check again\fR
150 rem THE REST OF THE "ROOT" SEGMENT
		|
		|
		|
900 stop
2000 rem oldstuff
2010 rem THIS PART OF THE PROG IS DESTROYED WHEN OVERLAYS ARE CALLED
2020 return

2000 rem pscheck
2010 rem This is the power supply check routine which is called in
2020 rem the root segment
2030 rem the first line (2000) must appear in the program as shown
		|
		|
3000 return

2000 rem loadck
2010 rem This is the load check overlay. This occupies the same
2020 rem line number space as pscheck and will therefor replace it
		|
		|
3000 return
.fi

In the above example, two overlays (pscheck and loadck) are called.
If those routines are found on disk they will be loaded and replace
the old program text starting at line 2000.
Note that loadck is called twice in succession. The second call will
not cause a load since loadck is already resident. The interpreter
believes this to be true by virtue of the
.I rem
statement with the name of the overlay at line 2000.
.SK
.H 2 "System Shell Control"
Invoked by the Bourne shell,
.I BITE
can be a powerful tool which can add mathematical capabilities to the
shell. The following shell-script invokes
.I BITE
without running it, loading the program add.b, entering a data
statement with two numbers to be added, issuing the
.I run
command
and putting the result in file "result".
.sp
.nf
:
: shell script to add two numbers
:
bite - add <<! > result
115 data $1,$2
run
!

.fi
The following is the add.b program invoked by the above shell script.
.nf

90 rem BASIC PROGRAM TO BE RUN BY SHELL SCRIPT
91 rem add numbers in data statement and output to standard output
100 read a,b			\fIget the values of the 2 numbers in the data\fR
110 print a+b			\fIoutput the result\fR
115 rem This line is replaced by a shell script line data statement
120 bye				\fIexit\fR
.fi

.I BITE
gives the Bourne shell the complete ability to EXECUTE BASIC INSTRUCTIONS!
This is accomplished by putting all the command lines in the shell-script
and using the <<! device pass the program to BITE.
.P
Below is a shell script which adds two numbers passed to it as arguments.
Note that the statements need not be in numerical order, since
.I BITE
will order them as they are entered.
Type this program in, give it a name, say add and make it executable
via chmod. Then try it by typing "add 3.14 2.22".
.nf

bite <<!
100 read a,b
110 print a+b
120 bye 
115 data $1,$2
run
!
.fi

An example of a shell-script using the immediate mode of
.I BITE
to get the sine of an angle in degrees is:
.nf

bite <<!
print sin(((2*3.1415926)/360)*$1)
bye
!
.fi

By naming the above shell-script "sin", one can type "sin 45" from
the system level to get an immediate answer.
.SK
Strings can be manipulated by the Bourne shell and system
utilities then passed to a
.I "BITE program."
The following is a simple example:
.nf

a="Now is the time"
b="for all good men"
bite <<!
print "$a $b to come to the aid of their country."
q
!
.fi
.H 1 "Error Messages"
Diagnostic error messages are issued by the interpreter
which indicate syntax errors , system failure, illegal commands or expressions,etc.
The
.I LSI-11/03
Version of
.I BITE
does not issue explicit error messages, but displays an error number
in which the meanings are listed below. This is done to regain
approximatly
2 kilobytes memory in an already tight
.I LSI-11/03
memory.
.H 2 "Standard Error Messages"
.nf
	NUMBER	MESSAGE TEXT
	--------------------
	0	REFERS TO A NON-EXISTING LINE NUMBER
	1	UNRECOGNIZABLE OPERATION
	2	CANNOT OPEN FILE
	3	ILLEGAL VARIABLE NAME
	4	BAD FILENAME
	5	WORKING STORAGE AREA EMPTY
	6	RUNS NESTED TOO DEEPLY
	7	UNASSIGNED VARIABLE
	8	EXPRESSION SYNTAX
	9	BAD KEYWORD IN STATEMENT
	10	IMPROPER OR NO RELATIONAL OPERATOR
	11	UNBALANCED QUOTES
	12	FILE EDITING NOT PERMITTED IN SINGLE STEP MODE
	13	MISSING OR ILLEGAL DELIMITER
	14	GOSUB WITH NO RETURN
	15	IS FATAL
	16	UNBALANCED PARENTHESIS
	17	UNKNOWN MATH FUNCTION
	18	NEXT WITH NO OR WRONG FOR IN PROGRESS
	19	CANNOT PROCESS IMAGINARY NUMBER
	20	WHAT ?
	21	BAD DIMENSION SYNTAX
	22	TOO MANY DIMENSIONS
	23	REDUNDANT DIM STATEMENT
	24	NOT ENOUGH WORKING STORAGE SPACE
	25	VARIABLE NOT DIMENSIONED
	26	WRONG NUM OF DIMS
	27	ONE OR MORE DIMS LARGER THAN ASSIGNED
	28	NEG. OR ZERO DIMENSION ILLEGAL
	29	DIVIDE BY ZERO
	30	BAD TAB SPEC. IN PRINT
	31	SYS CALL FAILED
	32	BAD FILE DECLARE SYNTAX
	33	OUT OF DATA
	34	FILE-NAME TOO LONG
	35	FILE DES. USED UP
	36	FILE NOT OPEN FOR OUTPUT
	37	FILE NOT OPEN FOR INPUT
	38	EXPRESSION YIELDS AN IMPOSSIBLE VALUE
	39	PRINTF: ARG COUNT MISMATCH
	40	PRINTF: MORE THAN 10 ARGS
	41	LINE TOO LONG FOR STRIP PRINTER
	42	MOV REQUIRES 3 LINE #'s, SPACING IS OPTIONAL
	43	BAD NAME OR LINE NUMBER AT BEGINNING OF SUBROUTINE
.H 2 "Test Set and Instrument Error Messages"
.nf
	100	MISSING ' DELIMITER BEFORE BUS ADDR
	101	PS: VOLTAGE OUT OF RANGE
	102	PS: CURRENT OUT OF RANGE
	103	PS: OVERVOLTAGE OUT OF RANGE
	104	RELAY ERR
	105	RELAY: INVALID FUNC.
	106	RELAY: INVALID NUMBER
	107	DVM: INVALID MODE
	108	DVM: INVALID RANGE
	109	LODSET: IMPROPER NUMBER OF ARGUMENTS
	110	LODSET: IMPROPER MODE
	111	LODSET: UNABLE TO SET LOAD
.H 1 Acknowledgement
.P
.I BITE
was written in the 'C' programming language by R. B. Drake
and myself. This memorandum was reviewed for accuracy and clarity by
R. B. Drake and J. D. McElroy.
The device drivers used for the extended instruction set are
a result of the efforts of R. B. Drake, R. E. Ellenbogen, L. W. Schaper
and myself.
Testing of the initial versions of
.I BITE
were made by N. P. Episcopo and D. J. Jackowski.
D. J. Jackowski performed extensive testing of
.I BITE,
made many helpful suggestions
and showed us how
.I BITE
could be used with the
.I "Bourne Shell".
N. P. Episcopo made some benchmark numerical comparisons between
.I BITE
and
.I BASIC
on
.I TSS.
Many helpful suggestions were contributed by R. Scuderi.
.fi
.SG UNIX
.SK
.PH """"
.sp 3
.ce
.ul
REFERENCES
.VL 8 4
.LI \[1\]
R. B. Drake "Customization of BITE", Bell Laboratories TM 78-2425-
.LI \[2\]
DEC, "introduction to BASIC",
Digital Equipment Corporation
.LI \[3\]
C. Joseph Sass, "BASIC Programming and Applications",
Allyn and Bacon, Inc.
.LI \[4\]
S. R. Bourne, "An Introduction to the UNIX Shell"
Bell Laboratories TM 78-1274-4
.LI \[5\]
R. E. Ellenbogen & John Tardy, "Automated Testing of Power Supplies"
Bell Laboratories TM 79-2425-1
.nr Hu 1
.nr a 0
.aH "SAMPLE FILE I/O PROGRAM"
The following is a sample of a program which opens a file for output,
writes values out to it, closes the file, re-opens the file for input,
reads the values from the file into an array and tabulates the
values.

The
.I comments
to the right of the statements are there
as an aid in this document. It is improper syntax to insert comments
or remarks in any other manner than by using the
.I rem
statement.


.nf
100 dim d(100)			\fIdimension a 100 variable array\fR
110 openo junk			\fIopen file junk for output\fR
120 for a = 1 to 100		\fIfor values of 'a' from 1 to 100\fR
130 pr _1a*10			\fIoutput 10*a to file junk\fR
140 next a			\fIloop\fR
150 closeall			\fIclose file junk\fR
160 openi junk			\fIopen file junk for input\fR
170 for x = 1 to 100		\fIfor 100 points points in array d(x)\fR
180 input _1d(x)		\fIinput next value to d(x)\fR
190 next x			\fIloop\fR
200 closeall			\fIclose file junk\fR
210 for x = 1 to 100		\fItabulate values of elements in d(100)\fR
220 printf "d(%4.1f)=%4.2f\\n",x,d(x)
230 next x



The output of the above program looks as follows:

d( 1.0)=10.00
d( 2.0)=20.00
d( 3.0)=30.00
d( 4.0)=40.00
d( 5.0)=50.00
d( 6.0)=60.00
d( 7.0)=70.00
d( 8.0)=80.00
d( 9.0)=90.00
d(10.0)=100.00
	|
	|
d(100.0)=1000.00
.fi
.SK
.HU "SAMPLE PROGRAM TO STEP VOLTAGE ON POWER SUPPLY"


.nf
100 for v = 1 to 30 step .5	\fIfor voltages of 1 - 30 in steps of .5 volts\fR
110 ps 1,v,1,30			\fIset power supply number 1 to 'v' volts\fR
120 next v			\fIloop again\fR
130 ps 0,0,0,0			\fIshut down power supply\fR
.fi

.HU "WAIT FOR BUTTON PRESS"


.nf
100 if btn(1) = 0 then 100	\fIwait for button 1 to be pressed\fR
110 n = 2			\fIset a button number\fR
120 if btn(n) = 0 then 120	\fIwait for button 2\fR
130 n = n + 1			\fInext button\fR
140 if btn(n) = 0 then 140	\fIwait for button 3\fR
.fi


.HU "EXERCISE SCANNER AND RELAYS"

.nf
100 for s = 1 to 30		\fIfor 30 scanner channels\fR
120 scan s			\fIset scanner channel s\fR
130 delay 30			\fIwait 1/2 second\fR
140 next s			\fIloop\fR
150 rem
160 rem exercise
170 rem
180 relay c			\fIopen all relays\fR
190 relay m,10,20,30		\fIclose relay 10,20 and 30\fR
200 relay b,10,20		\fIopen only relays 10 and 20\fR
210 relay c			\fIopen all relays\fR
220 for r = 1 to 60		\fIfor relays 1 through 60\fR
230 relay m,r			\fIclose cumulatively\fR
240 next r			\fIloop\fR
250 rem now 60 relays are closed
260 relay c,1			\fIopen all but relay number 1\fR
.fi
.CS 12 2 14 0 0 4
.TC

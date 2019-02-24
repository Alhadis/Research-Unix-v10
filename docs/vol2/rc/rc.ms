.so ../ADM/mac
.XX rc 283 "Rc \(em A Shell for Plan 9 and UNIX"
.fp 5 T CW	\" T for Typewriter
.Tm shell programming language	g
.de TP	\" An indented paragraph describing some command, tagged with the command name
.IP "\\fT\\$1\\fR" 5
.if \\w'\\fT\\$1\\fR'-4n .br
..
.de CI
.nr Sf \\n(.f
\%\&\\$3\f(CW\\$1\fI\&\\$2\f\\n(Sf
..
.TL
Rc \(em A Shell for Plan 9 and UNIX
.AU
Tom Duff
.AI
.MH
.AB
.I Rc
is a command interpreter for Plan 9.
It also runs on a variety of UNIX systems,
including SunOS and the Tenth Edition.
It provides similar facilities to Bourne's
.I /bin/sh ,
with some small additions and mostly less idiosyncratic syntax.
This paper introduces
.I rc
with numerous examples, and discusses its design and
why it varies from Bourne's.
.AE
.2C
.NH
Introduction
.PP
Plan 9 needs a command-programming language.  As porting the
Bourne shell to a non-UNIX environment seemed a daunting task,
I chose to write a new command interpreter, called
.I rc
because it runs commands.
Although tinkering with perfection is a dangerous business,
I could hardly resist trying to `improve' on Bourne's design.
Thus,
.I rc
is similar in spirit, but different in detail from Bourne's
shell.
.NH
Simple commands
.PP
For the simplest uses,
.I rc
has syntax familiar to Bourne shell users.
A simple command is a blank-separated sequence of arguments.
.P1
date
.P2
prints the current date and time, and
.P1
con alice
.P2
connects the user's terminal to the machine
.CW alice .
.NH
Quotation
.PP
An argument that contains a space or one of
.I rc 's
other syntax characters should be enclosed in apostrophes
.CW ' ). (
For example:
.P1
rm 'odd file name'
.P2
To include an apostrophe in a quoted name, it must be doubled:
.P1
echo 'How''s your father?'
.P2
.NH
Comments and continuation
.PP
A number-sign
.CW # ) (
and all characters following it up to, but not including, the next newline
are ignored, except in quotation marks.
.PP
A long command line may be continued on subsequent lines by typing a
backslash
.CW \e ) (
followed immediately by a newline, anywhere that white space is required.
Commands also continue onto the next line when the last symbol on
a line could not reasonably be the end of the command.
.NH
I/O Redirection
.PP
Many commands write output on the standard output file, normally the terminal.
A command's standard output may be made to overwrite a file by writing
.P1
who >user.names
.P2
This command produces a list of the currently logged in users in a file named
.CW user.names .
Standard output may be appended to the end of a file by
.P1
who >>user.names
.P2
In either case, mentioning a non-existent file causes it to be created.
.PP
Likewise, a command's standard input, also normally the terminal, may be connected to
a file by
.P1
wc <file
.P2
This command will report the number of characters, lines and words in
.CW file .
........
.NH
Pipelines
.PP
The standard output of one command may be connected to the standard input of another
command like this:
.P1
who | wc
.P2
This prints the number of users currently logged in, and is equivalent to
.P1
who >tmp
wc <tmp
.P2
except that no temporary file is produced.  Instead a pipe \(em an
interprocess channel that looks to one process like an output file and to
another like an input file \(em is created joining the two commands, which
run simultaneously.  The pipe contains a data buffer so that the two
commands need not produce and consume data in perfect lock-step.
.NH
Filename patterns
.PP
.I Rc
has several ways to generate argument lists automatically.
These include filename pattern matching, variable substitution, string concatenation,
command substitution and pipeline branching.
.PP
Any literal word containing one of the meta-characters
.CW * ,
.CW ?
or
.CW [
is a pattern used to match filenames.
A
.CW *
matches any sequence of characters
a
.CW ?
matches any single character,
and a list of characters between
.CW [
and
.CW ]
matches a single character from the listed set.
The list may include ranges like
.CW a-z ,
specifying all ASCII characters between the two.
The entire set is complemented if the first character
after
.CW [
is
.CW ~
(a tilde.)
No meta-character can match
the first character of the filenames
.CW .
and
.CW ..
or the character
.CW / .
.PP
A pattern is replaced by a list of all the names that it matches.
If it matches no name, it remains unreplaced.
.PP
Meta-characters are active only when they appear literally
and unquoted.  For example, if a variable (see below) has a
.CW *
in it, and the value is substituted into a line, it is
not used for filename matching.
.NH
Variables
.PP
.I Rc
provides variables whose values are lists of arguments.
Variables may be given values by typing, for example:
.P1
path=(. /bin /usr/bin)
user=td
tty=/dev/tty8
.P2
The parentheses indicate that the value assigned to
.CW path
is a list of three strings. The variables
.CW user
and
.CW tty
are assigned lists containing a single string.
.PP
The value of a variable can be substituted into a command by
preceding its name with a
.CW $ ,
like this:
.P1
echo $path
.P2
If
.CW path
had been set as above, this would be equivalent to
.P1
echo . /bin /usr/bin
.P2
Variables may be subscripted by numbers or lists of numbers,
like this:
.P1
echo $path(2)
echo $path(3 2 1)
.P2
These are equivalent to
.P1
echo /bin
echo /usr/bin /bin .
.P2
There can be no space separating the variable's name from the
left parenthesis.  Otherwise, the subscript would be considered
a separate parenthesized list.
.PP
The number of strings in a variable can be determined by the
.CW $#
operator.  For example,
.P1
echo $#path
.P2
would print the number of entries in
.CW $path .
.PP
The following two assignments are subtly different:
.P1
empty=()
null=''
.P2
The first sets
.CW empty
to a list containing no strings.
The second sets
.CW null
to a list containing a single string,
but the string contains no characters.
.PP
Although these may seem like more or less
the same thing (in Bourne's shell, they are
indistinguishable), they behave differently
in almost all circumstances.
Among other things
.P1
echo $#empty
.P2
prints 0, whereas
.P1
echo $#null
.P2
prints 1.
.PP
All variables that have never been set have the value
.CW () .
......
.NH
Arguments
.PP
When
.I rc
is reading its input from a file, the file has access
to the arguments supplied on
.I rc 's
command line.  The variable
.CW $*
initially has the list of arguments assigned to it.
The names
.CW $1 ,
.CW $2 ,
etc. are synonyms for
.CW $*(1) ,
.CW $*(2) ,
etc.
In addition,
.CW $0
is the name of the file from which
.I rc 's
input is being read.
.PP
The built-in
.CW shift
command deletes the first member of
.CW $* .
If
.CW shift
is given an argument, it deletes the given
number of members from
.CW $*.
Thus,
.P1
*=(Never, ever type rm -fr /)
shift 2
echo Please $*
.P2
would print
.P1
Please type rm -fr /
.P2
.NH
Concatenation
.PP
.I Rc
has a string concatenation operator, the caret 
.CW ^ ,
to build a arguments out of pieces.
.P1
echo hully^gully
.P2
is exactly equivalent to
.P1
echo hullygully
.P2
Suppose variable
.CW i
contains the name of a command.
Then
.P1
cc -o $i $i^.c
.P2
might compile the command's source code, leaving the
result in the appropriate file.
.PP
Concatenation distributes over lists. The following
.P1
echo (a b c)^(1 2 3)
src=(main subr io)
cc $src^.c
.P2
are equivalent to
.P1
echo a1 b2 c3
cc main.c subr.c io.c
.P2
In detail, the rule is: if both operands of
.CW ^
are lists of the same non-zero number of strings, they are concatenated
pairwise.  Otherwise, if one of the operands is a single string,
it is concatenated with each member of the other operand in turn.
Any other combination of operands is an error.
.NH
Free carets
.PP
User demand has dictated that
.I rc
insert carets in certain places, to make the syntax
look more like the Bourne shell.  For example, this:
.P1
cc -$flags $stems.c
.P2
is equivalent to
.P1
cc -^$flags $stems^.c
.P2
In general,
.I rc
will insert
.CW ^
between two arguments that are not separated by white space.
Specifically, whenever one of
.CW "$'`
follows a quoted or unquoted word, or an unquoted word follows
a quoted word with no intervening blanks or tabs, a
.CW ^
is inserted between the two.  If an unquoted word immediately following a
.CW $
contains a character other than an alphanumeric, underscore or
.CW * ,
a
.CW ^
is inserted before the first such character.
.NH
Command substitution
.PP
It is often useful to build an argument list from the output of a command.
.I Rc
allows a command, enclosed in braces and preceded by a left quote,
.CW "`{...}" ,
anywhere that an argument is required.  The command is executed and its
standard output captured.
The characters stored in the variable
.CW ifs
are used to split the output into arguments.
For example,
.P1
cat `{ls -tr|sed 10q}
.P2
will catenate the ten oldest files in the current directory in temporal order.
.NH
Pipeline branching
.PP
The pipeline notation described above is general enough for almost all cases.
Very occasionally it is useful to have pipelines that are not linear.
Pipeline topologies more general than trees can require arbitrarily large pipe buffers,
or worse, can cause deadlock.
.I Rc
has syntax for some kinds of non-linear but treelike pipelines.
For example,
.P1
	cmp <{old} <{new}
.P2
will regression test a new version of a command.
A
.CW <
(or
.CW > )
followed by a command in braces causes the command to be run with
its standard output (or input) attached to a pipe.  The parent command
.CW cmp "" (
in the example)
is started with the other end of the pipe attached to some file descriptor
or other, and with an argument that will connect to the pipe when opened
(e.g.
.CW /dev/fd/6 .)
On systems without
.CW /dev/fd ,
or something similar (SunOS for example) this feature does not work.
.....
.NH
Exit status
.PP
When a command exits, it returns status to the program that executed it.
On UNIX, status is a small positive number, with zero indicating normal termination
and non-zero values encoding error conditions (in a non-standard way).  If a
UNIX command terminates abnormally rather than by executing
.CW exit (2),
.I rc
takes the status to be 1000 plus the termination status provided to
.CW wait (2).
On Plan 9 status is a character string describing an error condition.
On normal termination it is empty.
.PP
.I Rc
captures commands' exit statuses in the variable
.CW $status .
For a simple command, the value of
.CW $status
is just as described above.  For a pipeline,
.CW $status
is set to the concatenation of the statuses of the pipeline components,
with
.CW |
characters for separators.
.PP
.I Rc
has a number of command forms that various kinds of control flow,
many of them conditioned by the status returned from previously
executed commands.  In all cases, any
.CW $status
containing only
.CW 0 's
and
.CW | 's
has boolean value
.I true .
Any other status is
.I false .
.NH
Flow of control
.PP
.I Rc
will accept multiple commands on a line, separated by
.CW &
or
.CW ; .
A command followed by
.CW &
is executed asynchronously \(em
.I rc
does not wait for it to exit, and it
disconnects its standard input from the terminal,
redirecting it to
.CW /dev/null
so that the command and
.I rc
do not compete for input lines.
The process id of an asynchronous command is stored in the variable
.CW $apid ,
and if
.I rc
is running interactively (that is, its input is coming from a terminal),
the process id is printed when the command starts.
.PP
Commands separated by
.CW ;
are executed in sequence, but
.I rc
waits for the first command to terminate before starting the second.
Thus
.P1
fortune;fortune&
.P2
prints out two fortunes (and a process id), not waiting for the
second to appear before prompting for more commands.
.NH
Command grouping
.PP
A sequence of commands enclosed in
.CW {}
may be used anywhere a command is required.
For example:
.P1
{sleep 3600;echo 'Time''s up!'}&
.P2
will wait an hour in the background, then print a message.
Without the braces:
.P1
sleep 3600;echo 'Time''s up!'&
.P2
this would lock up the terminal for an hour,
then print the message in the background!
.NH
Conditional execution \(em \f(CW&&\fP and \f(CW||\fP
.PP
A pair of commands may be separated by
.CW &&
or
.CW || .
In either case, the first command is executed and its
status examined.
If the operator is
.CW &&
and the status is
.I true ,
the second command is executed.
If the operator is
.CW ||
the status must be
.I false
to execute the second command.
Thus
.P1
cyntax *.c && cc -g -o cmd *.c
.P2
compiles
.CW cmd
only if
.CW cyntax
passes on it, and
.P1
{sleep 86400;rm -r /junk} ||
    echo Failure!|mail td &
.P2
waits 24 hours, then tries to remove all files in
.CW /junk .
On failure it sends me mail.  It does all of this in
the background, so I can use my terminal for something
else in the interim.
.NH
Subshells
.PP
The
.CW @
operator executes the following command in a new process.
This is useful primarily to insulate the parent process
from the effects of built-in commands, like this
.P1
@ {cd /elsewhere;tar cf - .} |
    tar -xf -
.P2
This dubious but venerated idiom copies
the contents of
.CW /elsewhere
to the current directory by building a
.CW tar
file on standard output and immediately
cracking it at the other end of a pipe.
The first
.CW tar
runs in
.CW /elsewhere ,
but, because of the
.CW @ ,
the second operates in the original directory.
.NH
Control flow \(em \f(CWfor\fP
.PP
A command may be executed once for each member of a list
by typing, for example:
.P1
for(i in printf scanf putchar)
    look $i /usr/td/lib/dw.dat
.P2
This looks for each of the words
.CW printf ,
.CW scanf
and
.CW putchar
in the given file.
The general form is
.P1
for(\fIname\fP in \fIlist\fP)
    \fIcommand\fP
.P2
or
.P1
for(\fIname\fP)
    \fIcommand\fP
.P2
In the first case,
.I command
is executed once for each member of
.I list ,
with that member assigned to variable
.I name .
If
.CI in " list
is not given,
.I $*
is used.
.NH
Conditional execution \(em \f(CWif\fP
.PP
.I Rc
also provides a general if-statement.  For example:
.P1
if(cyntax *.c)
    cc -g -o cmd *.c
.P2
is equivalent to the similar example above.
An `if not' statement provides a two-tailed conditional.
For example:
.P1
for(i){
    if(test -f /tmp/$i)
        echo $i already in /tmp
    if not
        cp $i /tmp
}
.P2
This loops over each file in
.CW $* ,
copying to
.CW /tmp
those that do not already appear there, and
printing a message for those that do.
.NH
Control flow \(em \f(CWwhile\fP
.PP
.I Rc 's
while statement looks like this:
.P1
while(newer subr.c subr.o) sleep 5
.P2
This waits until
.CW subr.o
is newer than
.CW subr.c
(presumably because the C compiler finished with it.)
.NH
Control flow \(em \f(CWswitch\fP
.PP
.I Rc
provides a switch statement to do pattern-matching on
arbitrary strings.  Its general form is
.P1
switch(\fIword\fP){
case \fIpattern ...\fP
    \fIcommands\fP
case \fIpattern ...\fP
    \fIcommands\fP
...
}
.P2
.I Rc
attempts to match the word against the patterns in each case statement in turn.
Patterns are the same as for filename matching, except that
.CW /
and the first characters of
.CW .
and
.CW ..
need not be matched explicitly.
.PP
If any pattern matches, the
commands following that case up to
the next case (or the end of the switch)
are executed, and execution of the switch
is complete.  For example,
.P1
switch($#*){
case 1
    cat >>$1
case 2
    cat >>$2 <$1
case *
    echo 'Usage: append [from] to'
}
.P2
is an append command.  Called with one file argument,
it tacks standard input to its end.  With two, the
first is appended to the second.  Any other number
elicits a usage message.
.PP
The built-in
.CW ~
command also matches patterns, and is often more concise than a switch.
Its arguments are a string and a list of patterns.  It sets
.CW $status
to zero if and only if any of the patterns matches the string.
The following example processes option arguments for the
.I man (1)
command:
.P1
opt=()
while(~ $1 -* [1-9] 10){
    switch($1){
    case [1-9] 10
        sec=$1 secn=$1
    case -f
        c=f s=f
    case -[qwnt]
        cmd=$1
    case -T*
        T=$1
    case -*
        opt=($opt $1)
    }
    shift
}
.P2
.NH
Functions
.PP
Functions may be defined by typing
.P1
fn \fIname\fP { \fIcommands\fP }
.P2
Subsequently, whenever a command named
.I name
is encountered, the remainder of the command's
argument list will assigned to
.CW $*
and
.I rc
will execute the
.I commands .
The value of
.CW $*
will be restored on completion.
For example:
.P1
fn g {
    gre -e $1 *.[hcyl]
}
.P2
defines
.CI g " pattern
to look for occurrences of
.I pattern
in all program source files in the current directory.
.PP
Function definitions are deleted by writing
.P1
fn \fIname\fP
.P2
with no function body.
.....
.NH
Command execution
.PP
Up to now we've said very little about what
.I rc
does to execute a simple command.
If the command name is the name of a function defined using
.CW fn ,
the function is executed.
Otherwise, if it is the name of a built-in command, the
built-in is executed directly by
.I rc .
Otherwise, if the name contains a
.CW / ,
it is taken to be the name of a binary program
and is executed using
.I exec (2).
If the name contains no
.CW / ,
then directories mentioned in the variable
.CW $path
are searched until an executable file is found.
.NH
Built-in commands
.PP
Several commands are executed internally by
.I rc ,
usually because their execution changes or depends on
.I rc 's
internal state.
.TP ". [-i] \fIfile ...\fT
Execute commands from
.I file .
.CW $*
is set for the duration to the reminder of the argument list following
.I file .
.CW $path
is used to search for
.I file .
Option
.CW -i
indicates interactive input \(mi a prompt
(found in
.CW $prompt )
is printed before each command is read.
.TP "builtin \fIcommand ...\fT
Execute
.I command
as usual except that any function named
.I command
is ignored.
For example,
.P1
fn cd{
    builtin cd $* && pwd
}
.P2
defines a replacement for the
.CW cd
built-in (see below) that announces the full name of the new directory.
.TP "cd [\fIdir\fT]
Change the current directory to
.I dir .
The default argument is
.CW $home .
.CW $cdpath
is a list of places in which to search for
.I dir .
.TP "eval [\fIarg ...\fT]
The arguments are concatenated separated by spaces into a string, read as input to
.I rc ,
and executed.  For example,
.P1
x='$y'
y=Doody
eval echo Howdy, $x
.P2
would echo
.P1
Howdy, Doody
.P2
since arguments of
.CW eval
would be
.P1
Howdy, $y
.P2
.TP "exec \fIcommand ...\fT
.I Rc
replaces itself with the given non-built-in
.I command .
.TP "exit [\fIstatus\fP]
Exit with the given status.  If none is given, the current value of
.CW $status
is used.
.TP finit
Initialize functions imported from the environment.
Intended for use only by
.I rc 's
initialization procedure.
Currently this is non-functional on Plan 9, as no
means of environmental function-storage yet exists.
.TP "flag \fIletter\fP"
Sets
.CW $status
to zero if and only if
.I rc
was invoked with option
.CI - letter
set.  Intended for use by
.I rc 's
initialization procedure.
.TP "mount \fInew old\fT
(Plan 9 only.)
Mount the name
.I new
on
.I old .
.TP "shift [\fIn\fT]
Delete the first
.I n
(default 1) elements of
.CW $* .
.TP "umask [\fIoctal\fP]
(UNIX only.)  Set
.I rc 's
file creation mask to the given octal value.
If no value is given, the current mask value is printed.
.TP "unmount [\fInew\fT] \fIold
(Plan 9 only.)
Unmount the name
.I old .
If
.I new
is given, unmount
.I old
from
.I new .
.TP "wait [\fIpid\fP]
Wait for the process with the given
.I pid
to exit.  If no
.I pid
is given, all outstanding processes are waited for.
.TP "whatis \fIname ...\fT
Print the value of each
.I name
in a form suitable for input to
.I rc .
The output is an assignment to any variable, the definition of any function,
a call to
.CW builtin
for any built-in command, or the full path name of any binary program.
For example,
.P1
whatis path g cd who
.P2
might print
.P1
path=(. /bin /usr/bin)
fn g {gre -e $1 *.[hycl]}
builtin cd
/bin/who
.P2
.TP "~ \fIsubject pattern ...
The
.I subject
is matched against each
.I pattern
in turn.  On a match,
.CW $status
is set to zero.
Otherwise, it is set to one.
Patterns are the same as for filename matching,
except that
.CW /
and the first character of
.CW .
and
.CW ..
need not be matched explicitly.
The
.I patterns
are not subjected to filename replacement before the
.CW ~
command is executed, so they need not be enclosed in
quotation marks, unless of course, a literal match for
.CW *
.CW [
or
.CW ?
is required.
For example
.P1
~ $1 ?
.P2
matches any single character, whereas
.P1
~ $1 '?'
.P2
only matches a literal question mark.
.NH
Special variables
.PP
Several variables are set internally by
.I rc
or used to guide its execution.
.TP $*
Set to
.I rc 's
argument list during initialization.
A
.CW .
command or function execution saves
.CW $*
on a stack and sets it to the new argument list.  The saved
value is restored on completion.
.TP $apid
Whenever a process is started asynchronously with
.CW & ,
.CW $apid
receives its process id.
.TP $cflag
Set to the string provided on
.I rc 's
command line with the
.CW -c
option, if any.  This is provided for the
use of
.I rc's initialization procedure.
.TP $home
The default directory for
.CW cd .
On UNIX, if
.CW $home
is not initially set, and
.CW $HOME
is, then
.CW $home
is set to
.CW $HOME .
.TP $ifs
The input field separators used in command substitution.
If not set in
.I rc 's
environment, it is initialized to blank, tab and newline.
.TP $path
The search path used to find commands and input files for the
.CW .
command.
If not set in the environment, it is initialized to
.CW "(. /bin /usr/bin)
on UNIX,
.CW "(. /bin)
on Plan 9.
.TP $pid
Set during initialization to
.I rc 's
process id.
.TP $prompt
When
.I rc
is run interactively, the first member of
.CW $prompt
is printed before reading each command.
The second member is printed if
.I rc
expects a command to be continued on another line.
If not set in the environment,
.I rc
initializes it to
.CW "('% ' '	')" .
.TP $status
On UNIX, this is set to the low 8 bits of the
.I exit (2)
argument of a normally terminating binary (unless started with
.CW & ),
or to 1000 plus the termination status on abnormal termination.
On Plan 9,
.CW $status
is set to the process's termination message.
On either system, the statuses of all processes in a pipeline
are captured and concatenated, separated by
.CW |
characters.
.CW !
and
.CW ~
also set
.CW $status .
Its value is used to control execution in
.CW && ,
.CW || ,
.CW if
and
.CW while
commands.
When
.I rc
exits at end-of-file of its input or on executing an
.CW exit
command with no argument,
.CW $status
is its exit status.
(This is only strictly true on Plan 9.  On UNIX, the exit status
must be numeric.  Therefore, if the value of
.CW $status
contains
.CW |
characters, they are deleted before converting it to a number, and if
.CW $status
contains other non-numeric characters,
.I rc
exits with status 1.)
.NH
Advanced I/O Redirection
.PP
.I Rc
allows redirection of file descriptors other than 0 and 1
(standard input and output) by specifying the file descriptor
in square brackets
.CW "[ ]
after the
.CW <
or
.CW > .
For example,
.P1
cc junk.c >[2]junk.diag
.P2
saves the compiler's diagnostics in
.CW junk.diag .
.PP
File descriptors may be replaced by a copy, in the sense of
.I dup (2),
of an already-open file by typing, for example
.P1
cc junk.c >[2=1]
.P2
This replaces file descriptor 2 with a copy of file descriptor 1.
It is more useful in conjunction with other redirections, like this
.P1
cc junk.c >junk.out >[2=1]
.P2
Redirections are evaluated from left to right, so this redirects
file descriptor 1 to
.CW junk.out ,
then points file descriptor 2 at the same file.
By contrast,
.P1
cc junk.c >[2=1] >junk.out
.P2
Redirects file descriptor 2 to a copy of file descriptor 1
(presumably the terminal), and then directs file descriptor 1
at a file.  In the first case, standard and diagnostic output
will be intermixed in
.CW junk.out .
In the second, diagnostic output will appear on the terminal,
and standard output will be sent to the file.
.PP
File descriptors may be closed by using the duplication notation
with an empty right-hand side.
For example,
.P1
cc junk.c >[2=]
.P2
will discard diagnostics from the compilation.
Often closing a file descriptor will not have the
expected effect, because many UNIX programs do not
behave politely when started with file descriptors
0, 1 or 2 closed.  Consider, for example,
.P1
who | tee a b c >[1=]
.P2
One might expect this to produce copies of the output of
.CW who
on files
.CW a ,
.CW b
and
.CW c ,
discarding the copy that
.CW tee
normally writes on its standard output.
However,
.CW tee
does not check for its standard output being closed.
When it opens
.CW a ,
the file descriptor returned is 1, and unsurprisingly,
.CW tee
writes two interleaved copies of its input onto
.CW a .
It is better explicitly to direct the output at
.CW /dev/null .
.PP
Arbitrary file descriptors may be sent through
a pipe by typing, for example
.P1
cc junk.c |[2] grep -v '^$'
.P2
This deletes those ever-so-annoying blank lines
from the C compiler's output.  Note that the output
of
.CW grep
still appears on file descriptor 1.
.PP
Very occasionally you may wish to connect the input side of
a pipe to some file descriptor other than zero.
The notation
.P1
cmd1 |[5=19] cmd2
.P2
creates a pipeline with
.CW cmd1 's
file descriptor 5 connected through a pipe to
.CW cmd2 's
file descriptor 19.
.NH
Here documents
.PP
.I Rc
procedures may include data, called ``here documents'',
to be provided as input to commands, as in this version of the
.I tel
command
.P1
for(i) grep $i <<!
...
nls 2T-402 2912
norman 2C-514 2842
pjw 2T-502 7214
...
!
.P2
A here document is introduced by the redirection symbol
.CW << ,
followed by an arbitrary eof marker
.CW ! "" (
in the example).  Lines following the command,
up to a line containing only the eof marker are saved
in a temporary file that it connected to the command's
standard input when it is run.
.PP
.I Rc
does variable substitution in here documents.  The following
.I subst
command:
.P1
ed $3 <<EOF
g/$1/s//$2/g
w
EOF
.P2
changes all occurrences of
.CW $1
to
.CW $2
in file
.CW $3 .
To include a literal
.CW $
in a here document, type
.CW $$ .
If the name of a variable is followed immediately by
.CW ^ ,
the caret is deleted.
.PP
Variable substitution can be entirely suppressed by enclosing
the eof marker following
.CW <<
in quotation marks.
.PP
Here documents may be provided on file descriptors other than 0 by typing, for example
.P1
cmd <<[4]End
...
End
.P2
.NH
Signals
.PP
.I Rc
scripts normally terminate when an interrupt is received from the terminal.
A function with the name of a signal, in lower case, is defined in the usual way,
but called when
.I rc
receives the signal.  Signals of interest are:
.TP sighup
Hangup.  The controlling teletype has disconnected from
.I rc .
.TP sigint
The interrupt character (usually ASCII del) was typed on the controlling terminal.
.TP sigquit
The quit character (usually ASCII fs, ctrl-\e) was typed on the controlling terminal.
.TP sigterm
This signal is normally sent by
.I kill (1).
.TP sigexit
An artificial signal sent when
.I rc
is about to exit.
.LP
As an example,
.P1
fn sigint{
    rm /tmp/junk
    exit
}
.P2
sets a trap for the keyboard interrupt that
removes a temporary file before exiting.
.PP
Signals will be ignored if the signal routine is set to
.CW {} .
Signals revert to their default behavior when their handlers'
definitions are deleted.
.NH
Environment
.PP
The environment is a list of name-value pairs made available to
executing binaries.  On UNIX the environment is passed
to commands as a second argument list containing
an entry for each variable whose value is non-empty, and
an entry for each function.  UNIX does not directly support
environment entries containing multiple strings, so if a variable
has more than one component, these are separated by ctrl-a
.CW \e001 ) (
characters.
.PP
On Plan 9, the environment is stored in a file system named
.CW #e ,
normally mounted on
.CW /env .
The value of each variable is stored in a separate file, with components
terminated by ASCII nuls.
(This is not quite as horrendous as it sounds, the file system is
maintained entirely in core, so no disk or network access is involved.)
The contents of
.CW /env
are shared on a per-process group basis \(mi when a new process group is
created it effectively attaches
.CW /env
to a new file system initialized with a copy of the old one.
A consequence of this organization is that commands can change environment
entries and see the changes reflected in
.I rc .
This is not possible on UNIX, as the environment is passed to children by
.I exec (2),
but is not passed back by
.I fork (2)
and
.I wait (2).
.PP
There is not currently a way on Plan 9 to place functions in the environment,
although this could easily done by mounting another instance of
.CW #e
on another directory.  The problem is that currently there can be only one instance of
.CW #e
per process group.
.NH
Local Variables
.PP
It is often useful to set a variable for the duration
of a single command.  An assignment followed by a command
has this effect.  For example
.P1
a=global
a=local echo $a
echo $a
.P2
will print
.P1
local
global
.P2
This works even for compound commands, like
.P1
f=/fairly/long/file/name {
    {   wc $f
        spell $f
        diff $f.old $f
    } | pr -h 'Facts about '$f |
        lp -ddp
}
.P2
.NH
Invocation
.PP
When
.I rc
starts, it digests its option arguments, initializes variables
and functions from the environment, sets
.CW $* ,
.CW $pid
and, if the
.CW -c
option is given,
.CW $cflag ,
and starts reading commands.
.PP
Most of
.I rc 's
initialization is done in a command file that is run at startup.
On UNIX the file is called
.CW /usr/lib/rcmain .
On Plan 9 it is
.CW /sys/lib/rcmain .
.LP
Legal options are:
.TP "-c \fIstring
Commands are read from
.I string .
.TP -e
Exit if
.CW $status
is non-zero after executing any simple command.
.TP -i
Interactive mode.
Also set if
.I rc
is given no arguments and its standard input is a terminal,
as determined by
.I isatty (3)
on UNIX, or by presumption on Plan 9.
Commands are prompted for using
.CW $prompt .
.CW SIGINT
and
.CW SIGQUIT
are caught and sloughed off.
.TP -I
Non-interactive mode.
Suppresses interactive mode even if
.I rc
is given no arguments and its standard input is a terminal.
.TP -l
Login mode.
Also set if the first character of argument zero is
.CW - 
(this is an obscure UNIX convention).
.I Rc
reads commands from
.CW $home/.rcrc ,
if it exists, before reading its normal input.
.TP -p
Protected mode.
.CW $path
is set to
.CW "(/bin /usr/bin)
on UNIX, or
.CW "/bin
on Plan 9, regardless of the setting in the environment.
Furthermore, functions are not initialized from the environment.
This provides a ``protected environment'' in which it is
difficult for malicious users to subvert an
.I rc
script by placing Trojan-horse versions of commands in
the environment or an unprotected directory.  On Plan 9 this is
not much use, since the meaning of all names, including those in
.CW /bin ,
is determined by the user.
.TP -v
Echo input on file descriptor 2 (diagnostic output) as it is read.
Useful for clarifying syntax problems in
.I rc
scripts.
.TP -x
Print each simple command on diagnostic output before executing it.
Useful for debugging
.I rc
scripts.
.PP
The following flags are useful primarily for debugging
.I rc .
.TP -d
Causes
.I rc
only to catch
.CW SIGINT ,
so that
.CW SIGQUIT
will cause it to dump core.
.TP -r
.I Rc 's
interpreter will print a running commentary on its state.
This is useful only for debugging
.I rc .
.TP -V
Echo input on diagnostic output as it is read, including
.CW rcmain ,
which is normally suppressed when using
.CW -v .
.NH
Examples \(em \fIcd, pwd\fP
.PP
Here is a pair of functions that provide
enhanced versions of the standard
.CW cd
and
.CW pwd
commands.  (Thanks to Rob Pike for these.)
.P1
ps1='% '	# default prompt
tab='	'	# a tab character
fn pbd{
  /bin/pwd|sed 's;.*/;;'
}
fn cd{
  builtin cd $1 &&
  switch($#*){
  case 0
    dir=$home
    prompt=($ps1 $tab)
  case *
    switch($1)
    case /*
      dir=$1
      prompt=(`{pbd}^$ps1 $tab)
    case */* ..*
      dir=()
      prompt=(`{pbd}^$ps1 $tab)
    case *
      dir=()
      prompt=($1^$ps1 $tab)
    }
  }
}
fn pwd{
  if(~ $#dir 0)
    dir=`{/bin/pwd}
  echo $dir
}
.P2
Function
.CW pwd
is a version of the standard
.CW pwd
that caches its value in variable
.CW $dir ,
because the genuine
.CW pwd
can be quite slow to execute.
.PP
Function
.CW pbd
is a helper that prints the last component of a directory name.
Function
.CW cd
calls the
.CW cd
built-in, and checks that it was successful.
If so, it sets
.CW $dir
and
.CW $prompt .
The prompt will include the last component of the
current directory (except in the home directory,
where it will be null), and
.CW $dir
will be reset either to the correct value or to
.CW () ,
so that the
.CW pwd
function will work correctly.
.NH
Examples \(em \fIman\fP
.PP
The
.I man
command prints pages from of the UNIX Programmer's Manual.
It is called, for example, as
.P1
man 3 isatty
man rc
man -t cat
.P2
In the first case, the page for
.I isatty
in section 3 is printed.
In the second case, the manual page for
.I rc
is printed.  Since no manual section is specified,
all sections are searched for the page, and it is found
in section 1.
In the third case, the page for
.I cat
is typeset (the
.CW -t
option).
.P1
cd /n/bowell/usr/man || {
  echo $0: Manual not on line! >[1=2]
  exit 1
}
NT=n  # default nroff
s='*' # section, default try all
for(i) switch($i){
case -t
  NT=t
case -n
  NT=n
case -*
  echo Usage: $0\e
    '[-nt] [section] page ...' >[1=2]
  exit 1
case [1-9] 10
  s=$i
case *
  eval 'pages=man'$s/$i'.*'
  for(page in $pages){
    if(test -f $page)
      $NT^roff -man $page
    if not
      echo $0: $i not found >[1=2]
  }
}
.P2
Note the use of
.CW eval
to make a list of candidate manual pages.
Without
.CW eval ,
the
.CW *
stored in
.CW $s
would not trigger filename matching
\(em it's enclosed in quotation marks,
and even if it weren't, it would be expanded
when assigned to
.CW $s .
Eval causes its arguments
to be re-processed by
.I rc 's
parser and interpreter, effectively delaying
evaluation of the
.CW *
until the assignment to
.CW $pages .
.NH
Examples \(em \fIholmdel\fP
.PP
The following
.I rc
script plays the deceptively simple game
.I holmdel ,
in which the players alternately name Bell Labs locations,
the winner being the first to mention Holmdel.
.KF
.P1
t=/tmp/holmdel$pid
fn read{
	$1=`{awk '{print;exit}'}
}
ifs='
\&'	# just a newline
fn sigexit sigint sigquit sighup{
	rm -f $t
	exit
}
cat <<'!' >$t
Allentown 
Atlanta
Cedar Crest
Chester
Columbus
Elmhurst
Fullerton
Holmdel
Indian Hill
Merrimack Valley
Morristown
Piscataway
Reading
Short Hills
South Plainfield
Summit
Whippany
West Long Branch
!
while(true){
   lab=`{/usr/games/fortune $t}
   echo $lab
   if(~ $lab Holmdel){
      echo You lose.
      exit
   }
   while(read lab
          ! grep -i -s $lab $t)
      echo No such location.
   if(~ $lab [hH]olmdel){
      echo You win.
      exit
   }
}
.P2
.KE
.LP
This script is worth describing in detail
(rather, it would be if it weren't so silly.)
.PP
Variable
.CW $t
is an abbreviation for the name of a temporary file.
Including
.CW $pid
in the names of temporary files insures that their
names won't collide, in case more than one instance
of the script is running at a time.
.PP
Function
.CW read 's
argument is the name of a variable into which a
line gathered from standard input is read.
.CW $ifs
is set to just a newline.  Thus
.CW read 's
input is not split apart at spaces, but the terminating
newline is deleted.
.PP
A handler is set to catch
.CW sigint ,
.CW sigquit ,
and
.CW sighup,
and the artificial
.CW sigexit
signal.  It just removes the temporary file and exits.
.PP
The temporary file is initialized from a here
document containing a list of Bell Labs locations, and
the main loop starts.
.PP
First, the program guesses a location (in
.CW $lab )
using the
.CW fortune
program to pick a random line from the location list.
It prints the location, and if it guessed Holmdel, prints
a message and exits.
.PP
Then it uses the
.CW read
function to get lines from standard input and validity-check
them until it gets a legal name.
Note that the condition part of a
.CW while
can be a compound command.  Only the exit status of the
last command in the sequence is checked.
.PP
Again, if the result
is Holmdel, it prints a message and exits.
Otherwise it goes back to the top of the loop.
.NH
Discussion
.PP
Steve Bourne's
.CW /bin/sh
is extremely well-designed; any successor is bound to
suffer in comparison.  I have tried to fix its
best-acknowledged shortcomings and to simplify things
wherever possible, usually by omitting unessential features.
Only when irresistibly tempted have I introduced novel ideas.
Obviously I have tinkered extensively with Bourne's syntax,
that being where his work was most open to criticism.
.PP
The most important principle in
.I rc 's
design is that it's not a macro processor.  Input is never
scanned more than once by the lexical and syntactic analysis
code (except, of course, by the
.CW eval
command, whose
.I "raison d'etre
is to break the rule).
.PP
Bourne shell scripts can often be made
to run wild by passing them arguments containing spaces.
These will be split into multiple arguments using
.CW IFS ,
often as inopportune times.
In
.I rc ,
values of variables, including command line arguments, are not re-read
when substituted into a command.
Arguments have presumably been scanned in the parent process, and ought
not to be re-read.
.PP
Why does Bourne re-scan commands after variable substitution?
He needs to be able to store lists of arguments in variables whose values are
character strings.
If we eliminate re-scanning, we must change the type of variables, so that
they can explicitly carry lists of strings.
.PP
This introduces some
conceptual complications.  We need a notation for lists of words.
There are two different kinds of concatenation, for strings \(em
.CW $a^$b ,
and lists \(em
.CW "($a $b)" .
The difference between
.CW ()
and
.CW ''
is confusing to novices,
although the distinction is arguably sensible \(em
a null argument is not the same as no argument.
.PP
Bourne also rescans input when doing command substitution.
This is because the text enclosed in back-quotes is not
properly a string, but a command.  Properly, it ought to
be parsed when the enclosing command is, but this makes
it difficult to
handle nested command substitutions, like this:
.P1				
size=`wc -l \e`ls -t|sed 1q\e``
.P2
The inner back-quotes must be escaped
to avoid terminating the outer command.
This can get much worse than the above example;
the number of
.CW \e 's
required is exponential in the nesting depth.
.I Rc
fixes this by making the backquote a unary operator
whose argument is a command, like this:
.P1
size=`{wc -l `{ls -t|sed 1q}}
.P2
No escapes are ever required, and the whole thing
is parsed in one pass.
.PP
A similar rationale explains why
.I rc
defines signal handlers as though they were functions,
instead of associating a string with each signal, as Bourne does,
with the attendant possibility of getting a syntax error message
in response to typing the interrupt character.  Since
.I rc
parses input when typed, it reports errors when you make them.
.PP
For all this trouble, we gain substantial semantic simplifications.
There is no need for the distinction between
.CW $*
and
.CW $@ .
There is no need for four types of quotation, nor the
extremely complicated rules that govern them.  In
.I rc
you use quotation marks exactly when you want a syntax character
to appear in an argument.
.CW IFS
is no longer used, except in the one case where it was indispensable:
converting command output into argument lists during command substitution.
.PP
This also avoids an important security hole|reference(reeds loophole).
.I System (3)
and
.I popen (3)
call
.CW /bin/sh
to execute a command.  It is impossible to use either
of these routines with any assurance that the specified command will
be executed, even if the caller of
.I system
or
.I popen
specifies a full path name for the command.  This can be devastating
if it occurs in a set-userid program.
The problem is that
.CW IFS
is used to split the command into words, so an attacker can just
set
.CW IFS=/
in his environment and leave a Trojan horse
named
.CW usr
or
.CW bin
in the current working directory before running the privileged program.
.I Rc
fixes this by not ever rescanning input for any reason.
.PP
Most of the other differences between
.I rc
and the Bourne shell are not so serious.  I eliminated Bourne's
peculiar forms of variable substitution, like
.P1
echo ${a=b} ${c-d} ${e?error}
.P2
because they are little used, redundant and easily
expressed in less abstruse terms.
I deleted the builtins
.CW export ,
.CW readonly ,
.CW break ,
.CW continue ,
.CW read ,
.CW return ,
.CW set ,
.CW times
and
.CW unset
because they seem redundant or
only marginally useful.
.PP
Where Bourne's syntax draws from Algol 68,
.I rc 's
is based on C or Awk.  This is harder to defend.
I believe that, for example
.P1
if(test -f junk) rm junk
.P2
is better syntax than
.P1
if test -f junk; then rm junk; fi
.P2
because it is less cluttered with keywords,
it avoids the semicolons that Bourne requires
in odd places,
and the syntax characters better set off the
active parts of the command.
.PP
The one bit of large-scale syntax that Bourne
unquestionably does better than
.I rc
is the
.CW if
statement with
.CW "else
clause.
.I Rc 's
.CW if
has no terminating
.CW fi -like
bracket.  As a result, the parser cannot
tell whether or not to expect an
.CW "else
clause without looking ahead in its input.
The problem is that after reading, for example
.P1
if(test -f junk) echo junk found
.P2
in interactive mode,
.I rc
cannot decide whether to execute it immediately and print
.CW $prompt(1) ,
or to print
.CW $prompt(2)
and wait for the
.CW "else
to be typed.
In the Bourne shell, this is not a problem, because the
.CW if
command must end with
.CW fi ,
regardless of whether it contains an
.CW else
or not.
.PP
.I Rc 's
admittedly feeble solution is to declare that the
.CW else
clause is a separate statement, with the semantic
proviso that it must immediately follow an
.CW if ,
and to call it
.CW "if not
rather than
.CW else ,
as a reminder that something odd is going on.
The only noticeable consequences of this is that
the braces are required in the construction
.P1
for(i){
    if(test -f $i) echo $i found
    if not echo $i not found
}
.P2
and that
.I rc
resolves the ``dangling else'' ambiguity in opposition
to most people's expectations.
.PP
It is remarkable that in the four most recent editions of the UNIX
programmer's manual the Bourne shell grammar described in the manual page
does not admit the command
.CW who|wc .
This is surely an oversight, but it suggests something darker:
nobody really knows what the Bourne shell's grammar is.  Even examination
of the source code is little help.  The parser is implemented by recursive
descent, but the routines corresponding to the syntactic categories all
have a flag argument that subtly changes their operation depending on the
context.
.I Rc 's
parser is implemented using
.I yacc ,
so I can say precisely what the grammar is \(em it's reproduced in the appendix.
.PP
Its lexical structure is harder to describe.
I would simplify it considerably
except for two things.
There is a lexical kludge to distinguish between parentheses that immediately
follow a word with no intervening spaces and those that don't that I would
eliminate if there were a reasonable pair of characters to use
for subscript brackets.  I could also eliminate the insertion of
free carets if users were not adamant about it.
.NH
Acknowledgements
.PP
Rob Pike, Howard Trickey and other Plan 9 users have been insistent, incessant
sources of good ideas and criticism.  Some examples in this document are plagiarized
from |reference(bstj shell), as are most of
.I rc 's
good features.
.NH
References
.LP
|reference_placement
.SH
Appendix \(em an
.I rc
grammar
.PP
Below is reproduced
.I rc 's
.I yacc
grammar, extracted directly from the source code, edited only
to remove semantic actions.  The grammar describes a single
command.  An
.I rc
script is a sequence of zero or more of these.
.PP
In the grammar, the token
.CW WORD
represents either a quoted string or
a non-empty string of characters other
than blank, tab, newline or any of the following:
.P1
# ; & | ^ $ = ` ' { } ( ) < >
.P2
with the proviso that a word that immediately follows a
.CW $
or
.CW $#
ends at the first character that is not alphanumeric or
.CW _
or
.CW * .
.PP
The token
.CW SUB
represents a left parenthesis that follows a
.CW WORD
with no intervening spaces and therefore should be interpreted
as the start of a subscript.
.CW REDIR
represents any of the redirection symbols,
.P1
> >[\fIn\fT]
< <[\fIn\fT]
>> >>[\fIn\fT]
<< <<[\fIn\fT]
.P2
.CW PIPE
is any of the pipe symbols
.P1
| |[\fIn\fT] |[\fIn\fT=\fIm\fT]
.P2
.CW DUP
is any of the file-descriptor duplication or closing symbols
.P1
>[\fIn\fT=\fIm\fT] <[\fIn\fT=\fIm\fT] >>[\fIn\fT=\fIm\fT]
>[\fIn\fT=] <[\fIn\fT=] >>[\fIn\fT=]
.P2
.CW ANDAND
stands for
.CW && ,
.CW OROR
stands for
.CW || ,
and
.CW COUNT
stands for
.CW $# .
The other token names stand for or key words, which are lexically
indistinguishable from
.CW WORD s:
.P1
BANG	!
FN	fn
FOR	for
IF	if
IN	in
NOT	not
SUBSHELL	@
SWITCH	switch
TWIDDLE	~
WHILE	while
.P2
.PP
Comments start with
.CW #
and continue to the end of the line, and are deleted by lexical
analysis, which also converts a backslash
.CW \e ) (
followed by a newline into a space, and
inserts free carets whenever a
.CW WORD
is followed by another
.CW WORD
or a
.CW `
or
.CW $ ,
with no intervening spaces.
.PP
Here then is
.I rc 's
grammar.  Semantic actions that call function
.CW skipnl
have been included at places where a command
may be continued on a new line.
.br
\X'PARM CT 80'
.KS
.P1
%term FOR IN WHILE IF NOT
%term TWIDDLE BANG SUBSHELL
%term SWITCH FN WORD REDIR DUP
%term PIPE SUB
%left IF WHILE FOR SWITCH ')' NOT
%left ANDAND OROR
%left BANG SUBSHELL
%left PIPE
%left '^'
%right '$' COUNT
%left SUB
%%
rc:
	|  line '\en'
line: cmd
	|  cmdsa line
body: cmd
	|  cmdsan body
cmdsa: cmd ';'
	|  cmd '&'
cmdsan: cmdsa
	|  cmd '\en'
brace: '{' body '}'
paren: '(' body ')'
assign: first '=' word
epilog:
	|  redir epilog
redir: REDIR word
	|  DUP
cmd:
	|  brace epilog
	|  IF paren {skipnl();} cmd
	|  IF NOT {skipnl();} cmd
	|  FOR '(' word IN words ')'
        {skipnl();} cmd
	|  FOR '(' word ')'
        {skipnl();} cmd
	|  WHILE paren {skipnl();} cmd
	|  SWITCH word {skipnl();} brace
	|  simple
	|  TWIDDLE word words
	|  cmd ANDAND cmd
	|  cmd OROR cmd
	|  cmd PIPE cmd
	|  redir cmd  %prec BANG
	|  assign cmd %prec BANG
	|  BANG cmd
	|  SUBSHELL cmd
	|  FN words brace
	|  FN words
.P2
.KE
.KS
.P1
simple: first
	|  simple word
	|  simple redir
first: comword  
	|  first '^' word
word: keyword
	|  comword
	|  word '^' word
comword: '$' word
	|  '$' word SUB words ')'
	|  COUNT word
	|  WORD
	|  '`' brace
	|  '(' words ')'
	|  REDIR brace
keyword: FOR|IN|WHILE|IF|NOT
	|  TWIDDLE|BANG|SUBSHELL|SWITCH
	|  FN
words:
	|  words word
.P2
.KE

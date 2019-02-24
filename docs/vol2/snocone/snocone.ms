.so ../ADM/mac
.XX snocone 297 "The Snocone Programming Language"
.nr dP 2
.nr dV 3p
.EQ
delim $$
.EN
.ds S4 \s-2SNOBOL4\s0
.de Sx
.P1 0
..
.de Sy
.P2
..
.de Tx
.IP "" 5
..
.de Ty
.PP
..
.TL
The Snocone Programming Language
.AU
Andrew Koenig
.AI
.MH
.AB
.PP
\*(S4 is a well-known programming language with convenient
semantics and clumsy syntax.
Following the pattern of Ratfor and \s-2EFL\s0,
we have added ``syntactic sugar'' to
\*(S4, with an eye toward making it easier to use.
We call the result
.I Snocone.
.PP
This paper describes the Snocone language in enough
detail that people not familiar with \*(S4 can
learn to use it, although previous familiarity
with \*(S4 will help.
.AE
.2C
.NH 1
Introduction
.PP
The semantics of the \*(S4 programming language |reference(poage griswold)
include such unusual and useful
features as
dynamic typing,
a general-purpose garbage collector,
excellent character string facilities,
associative arrays,
and strong run-time diagnostics.
Griswold|reference(snobol4 applications),
Gimpel|reference(gimpel snobol4),
and others have documented many ways of
doing things in \*(S4 that can only be accomplished
with much more difficulty in other languages.
.PP
Unfortunately, the control structures of
\*(S4 are archaic, and the fact that
blank is an operator tends to encourage certain types
of errors that are hard to detect.
Other aspects of the language make it a nuisance
to construct large programs out of small parts.
.PP
To ameliorate some of these problems,
we have designed and implemented a new language
that provides some syntactic sugar for \*(S4.
The obvious name for such a language is
.I Snocone .
.PP
The design of the Snocone language was inspired by
Ratfor|reference(kernighan ratfor)
and \s-2EFL\s0|reference(efl cstr).
Like \s-2EFL\s0, and unlike Ratfor, Snocone is a self-contained
programming language, rather than a proper superset of
\*(S4.
Like Ratfor, and unlike \s-2EFL\s0, the Snocone translator
makes no attempt to produce \*(S4 output that is
easy for humans to understand.
.PP
Hanson|reference(ratsno)
has written a similar, but simpler preprocessor
for \*(S4.
His preprocessor is like Ratfor
in that it does not change the statements in
the basic language but rather adds new syntax.
.PP
Griswold|reference(rebus)
has written a preprocessor for a language
similar to Snocone,
the syntax of which is based on Icon|reference(icon language)
rather than on C or \s-2EFL\s0.
His preprocessor is written in C,
and uses Yacc for its parsing.
.PP
In contrast, Snocone has a syntax roughly
based on C and is written in Snocone.
.SH
What's nice about \*(S4
.PP
Variables in \*(S4 are dynamically typed:
the type of any variable is the type of the
value most recently assigned to it.
Thus, declarations are unnecessary, and, in fact,
\*(S4 has no declarations as such
(except that procedures can have local variables).
.PP
All operators and built-in functions check
their argument types;
each argument is converted automatically
to an appropriate type.
A run-time diagnostic message results
if a conversion is impossible.
For instance, if an addend
is a string, \*(S4 will try
to convert it to an integer or real number,
depending on the form of its value.
If the value is inappropriate to convert to
a numeric type, the program will halt.
.PP
Like Lisp, and unlike most other languages,
\*(S4 does not have any explicit mechanism for
returning memory to the system.
Rather, the implementation must detect
when memory is no
longer needed and make that memory available
for re-use.
This makes the language harder to implement,
but easier to use.
One nice by-product of this sort of memory
allocator is that \*(S4 programs tend
to be free of arbitrary size limitations.
.PP
One \*(S4 data type is the
.I pattern ,
which describes an (arbitrarily complex)
set of character strings.
Briefly, the language incorporates a general
top-down backtracking parser.
This parser is so easy to use that it
is the usual way of dealing with strings
in \*(S4.
.PP
Another useful data type is the
.I table .
A table is like a one-dimensional array, except that
its subscripts are not limited to being integers.
For instance, a compiler symbol table might be maintained
as a \*(S4 table, with the subscript being the identifier
and the value being some structure which holds the desired
information about that identifier.
.PP
An interesting and unusual semantic idea in \*(S4 is
that of
.I "statement failure" .
Many operations can easily encounter conditions that
preclude their execution.
For example, an attempt may be made to access a non-existent
array element, read beyond the last record of a file,
search a string for a pattern that it does not contain,
or even make a comparison that gives an unexpected result.
When this happens, the operation
.I fails .
Usually, the failure of an operation implies the failure
of the statement of which it is a part.
While statement failure is not an error condition,
it can be tested.
In fact, conditional transfer on the success or
failure of a statement is virtually the only
kind of control structure in \*(S4.
.PP
A good implementation of \*(S4 is
Macrospitbol, by Dewar and McCann|reference(spitbol).
This is a portable threaded-code interpreter, which
is fast and robust enough to be used for ``production''
purposes.
For example, on a VAX-11/780 it translates about 150
statements per second and executes about 5,000.
.PP
\*(S4 implementations in general have excellent
run-time diagnostic facilities.
The language definition includes ways of tracing
programs for debugging, and of trapping almost any
kind of run-time error.
.SH
What's not nice about \*(S4
.PP
Here is a simple \*(S4 program to add the
integers between 1 and 1000:
.P1 0
	sum = 0
	term = 1
loop	sum = sum + term
	term = term + 1
	LE(term,1000)		:s(loop)
	OUTPUT = "The sum is " sum
END
.P2
.PP
The first two lines of this program assign values to
variables.
The name
.CW loop
in the third line is a label; it is recognized as such
because it begins the line without any white space
ahead of it.
.PP
The fifth line calls a built-in function to compare the
value of the variable
.CW term
to 1000.
\*(S4 has no comparison operators: all comparisons are
done by
.I predicate
functions that either return the null string if the condition
being tested is true or fail if the condition is false.
The \f(CW:s(loop)\fP at the end of the line
is a conditional
.B "go to" :
it causes control to transfer to the label
.CW loop
if the statement succeeds, and to the following
statement if it fails.
.PP
In contrast, the program looks like this in Snocone:
.P1 0
sum = 0
term = 1
do {
	sum = sum + term
	term = term + 1
} while (term <= 1000)
OUTPUT = "The sum is " && sum
.P2
.PP
In the past fifteen years, the trend in programming
language design has been overwhelmingly toward
programming languages with control structures that
make it reasonable to write programs with at most
a very few
.B "go to"
statements.
\*(S4 exhibits almost the exact opposite of this trend:
essentially the only control structure in \*(S4 is
a form of conditional
.B "go to"
statement.
There is no block structure, and all labels are global.
Thus, writing a \*(S4 program requires a constant effort
to invent new label names, and to choose names that have
at least some chance of telling the reader whether their
use is local or global.
.PP
The programmer's job is not made any easier by the peculiar way
\*(S4 handles subroutines.
\*(S4 subroutines are defined at run time by a built-in
function called \f(CWDEFINE\fP.
Its argument is a
.I prototype
of the subroutine to be defined \-
a character string that describes how the
subroutine is to be used.
The subroutine's entry point is the label with the same name.
Because the statement bearing this label is not special
in any other way, it must be placed where it will not
be executed in the ordinary course of events.
The call to \f(CWDEFINE\fP, on the other hand, must
be executed before the subroutine it defines can be used.
.PP
This leads \*(S4 programmers into contortions.
To keep the \f(CWDEFINE\fP call near the subroutine body,
one must invent yet another label:
.P1 0
        DEFINE("square(x)")	:(square.end)
square  square = x * x		:(RETURN)
square.end
.P2
Alternatively, one can put all the \f(CWDEFINE\fP calls
in one place, at the cost of moving the body of each
subroutine arbitrarily far from where its prototype appears.
.PP
The Snocone programmer has an easier job:
.P1 0
procedure square (x) {
	return x * x
}
.P2
.SH
Motivation
.PP
Snocone's purpose, then, is to make it easy for a
programmer used to a block-structured language like C
to write programs that have the freedom and semantic flexibility
of \*(S4.
To this end, we have changed the \*(S4 language in several ways.
.PP
First, we introduced an explicit concatenation operator.
\*(S4 uses blank for concatenation, so
.P1 0
y = f(x)
.P2
is a function call, but
.P1 0
y = f (x)
.P2
assigns to \f(CWy\fP the concatenation of the 
values of the variables
\f(CWf\fP and \f(CWx\fP.
We chose \f(CW&&\fP to represent concatenation because
\*(S4 programs often use concatenation for the same purpose
that C programs use \f(CW&&\fP.
.PP
Second, we allow much the same freedom with spaces that
C does, with one exception:  newline ends a statement.
Statements may be continued onto multiple lines in much the
same way as in \s-2EFL\s0 programs: a line is continued
if it ends with an
operator or some kind of open bracket.
.PP
Third, we have added procedure and structure declarations.
These things are accomplished in \*(S4 by calling built-in
subroutines, and the context of the calls is often obscure.
.PP
Finally, we have introduced control structures similar to
those in C and other block-structured languages: the
\f(CWif\fP, \f(CWwhile\fP, \f(CWfor\fP, and \f(CWdo\fP statements.
.NH 1
Language Description
.NH 2
Lexical Conventions
.PP
Blanks (spaces and tabs, but not newlines) may not appear within a
token, but may freely separate tokens.
Comments begin with a \f(CW#\fP
character and end at the end of the line.
.PP
Constants may be integers, reals, or strings.
There are no signed
numeric constants.
Integers range from 0 to $2 sup 31 - 1$.
Real constants
are short-precision only, and must contain either a decimal point or an
\f(CWE\fP (or \f(CWe\fP).
String constants may be delimited by single or double quotes
with the same meaning.
Characters in quotes receive no special
interpretation.
A single quote may appear in strings delimited by
double quotes, and vice versa.
.EQ
delim off
.EN
.PP
Identifiers may be of any length.
All characters in an identifier are
significant, but their case is presently not significant.
Case may
become significant in the future.
An identifier is a non-empty
sequence of letters and digits, beginning with a letter.
Underscore
(_) is a letter.
The same identifier may be used independently to
represent a procedure, label, or variable.
.NH 2
Statement Separation
.PP
Snocone delimits statements much like the Shell|reference(bstj shell)
(the command interpreter in the
.UX
operating system)|reference(unix cacm ritchie thompson)
or \s-2EFL\s0.
A statement is
ended either by a semicolon or newline, except that if the last token
on a line is an operator or open parenthesis or bracket, the next line
is automatically considered as part of the current statement.
Newlines
may also separate clauses of compound statements, so
.P1 0
if (a < 0)
	a = 0
.P2
is acceptable and means the same as
.P1 0
if (a < 0) a = 0
.P2
or, spreading things as much as possible:
.P1 0
if (
a <
0)
a =
0
.P2
.NH 2
File Inclusion
.PP
The contents of an arbitrary file can be incorporated into
the program by writing an
.I "include line"
in one of the following four forms:
.P1 0
#include "\fIfile\fP"
#include '\fIfile\fP'
#include <\fIfile\fP>
#include {\fIfile\fP}
.P2
Spaces may appear between \f(CW#\fP and \f(CWinclude\fP.
The contents of the named file are substituted for
the \f(CWinclude\fP line.
.PP
The exact behavior of an \f(CWinclude\fP line depends
on the delimiters surrounding the file name.
If quotes are used (single or double), the file is
sought in the current directory.
If brackets are used (angle brackets or curly braces),
the file is sought in an installation-dependent
system library
(\f(CW/usr/lib/snocone\fP on our systems).
If double quotes or angle brackets are used, the
file is included unconditionally, even if the same
file is included several times.
If single quotes or curly braces are used,
the file will only be included once, even if it
is named in several \f(CWinclude\fP lines.
.PP
This latter behavior is useful in the following sort of situation.
Suppose that procedure
.I proc1
is defined in
file
.I proc1.h ,
and procedure
.I proc2
is defined
in file
.I proc2.h .
A program that calls both
.I proc1
and
.I proc2
might then contain:
.P1 0
#include "proc1.h"
#include "proc2.h"
.P2
Now, suppose that
.I proc1
is changed to call
.I proc2 .
If
.I proc1.h
is made to contain
.P1 0
#include "proc2.h"
.P2
then our hypothetical sample program will wind
up with two copies of
.I proc2.h
and will therefore run into trouble with duplicate
procedure definitions.
If, however,
.I proc1.h
contains:
.P1 0
#include 'proc2.h'
.P2
and the main program contains:
.P1 0
#include 'proc1.h'
#include 'proc2.h'
.P2
then
.I proc2
will be defined only once.
.NH 2
Expression Evaluation, Success, and Failure
.PP
Evaluating an expression has one of three
outcomes: a value, failure, or error.
.PP
Errors normally result in a diagnostic message
and termination of the program, and arise
from the usual sorts of things:
overflow, underflow,
division by zero, impossible conversions,
running out of memory, and so on.
.PP
Failure, on the other hand, is not an error
condition.
An expression that fails is merely one that
does not yield a value.
Examples of expressions that fail include
attempts to refer to non-existent array
elements,
attempts to read beyond end of file,
and even comparisons that yield unexpected results.
For instance, the expression
.P1 0
a < b
.P2
yields a null string if \f(CWa\fP is indeed less than \f(CWb\fP,
and fails otherwise.
.PP
Most operators fail if any of their operands fails.
The few exceptions will be noted below.
.PP
An expression that always either yields a null string or fails
is sometimes called a
.I predicate .
Testing such expressions for success or failure in \f(CWif\fP,
\f(CWwhile\fP, and \f(CWdo\fP statements is the primary way
of affecting the flow of control
in Snocone.
.NH 2
Data Types, Declarations, and Scope
.PP
Variables in Snocone are dynamically typed: a variable has the type of
the value most recently assigned to it.
Except for a few predefined variables
(described under
.I "pattern matching" ),
all variables have the null string as their initial values.
All variables are global, but procedures
can nominate variables whose
values will automatically be saved at
entry and restored at exit.
The only declarations define
structures:
.P1 0
struct cons {car, cdr}
.P2
In effect, this declaration defines three procedures named
\f(CWcons\fP, 
\f(CWcar\fP,
and \f(CWcdr\fP.
The value of \f(CWcons(a,b)\fP is a newly-created
\f(CWcons\fP object with \f(CWa\fP and \f(CWb\fP
as the values of its \f(CWcar\fP and \f(CWcdr\fP fields,
respectively.
The fields, in turn, are accessed by similarly-named
functions.
For instance, the \f(CWcdr\fP field of \f(CWcons\fP structure \f(CWx\fP
is accessed as \f(CWcdr(x)\fP.
Thus this program:
.P1 0
struct cons {car, cdr}
a = cons (3, cons (4, 5))
OUTPUT = car (cdr (a))
.P2
prints \f(CW4\fP.
The procedures corresponding to field names may
be used as the target of an assignment:
.P1 0
car (a) = "Hello"
.P2
Snocone offers other data types than those described above.
While
numeric constants cannot be negative, variables and expressions
suffer from no such restriction.
Strings may be of any length up
to an implementation-defined upper bound, usually many thousands
of characters.
All variables have the null string as their
initial value.
.PP
Aggregate values come in two types: arrays and tables.
Each type
is created by a built-in procedure with the same name.
Thus:
.P1 0
a = ARRAY (20)
.P2
creates a 20-element array, initializes each element to the null
string, and assigns it to a.
The second argument to the array
procedure is an initializing value:
.P1 0
a = ARRAY (20, -1)
.P2
makes a an array of 20 elements, each with value \-1.
To get
multi-dimensional arrays, express the dimensions as a string:
.P1 0
a = ARRAY ('4,5', -1)
.P2
One can also give explicit lower bounds:
.P1 0
a = ARRAY ('-10:10')
.P2
The default lower bound is 1.
.PP
Array elements are referenced by Algol-like subscripts:
.P1 0
a[i,j] = a[i,j] + b[i,k] * c[k,j]
.P2
Each array element behaves as a variable, and can therefore have
a value of any data type, independently of any other element.
.PP
A table is like a one-dimensional array whose subscripts are not
restricted to integers:
.P1 0
t = TABLE (20)
.P2
The argument to the \f(CWTABLE\fP procedure is an estimate of the maximum
number of elements that will actually be stored in the table.
If substantially more elements than
this are stored, access will
begin to slow down.
On the other hand, giving too large an
initial value wastes space.
Once a table has been created, any
value can be used for a subscript.
\f(CWt[a]\fP and \f(CWt[b]\fP will refer to
the same element if and only if a and b are identical.
The
precise meaning of ``identical'' is given with the description of
the \f(CW::\fP and \f(CW:!:\fP operators;
suffice it to say that \f(CW3\fP and \f(CW"3"\fP are
not identical, and that after executing
.P1 0
a = b
.P2
\f(CWa\fP and \f(CWb\fP are
identical regardless of what values they had before.
.NH 2
Binary Operators
.PP
The following operators are grouped in order of decreasing priority.
Unless otherwise stated, they are left-associative.
.IP "\f(CW. $\fP"
Pattern value assignment (see Patterns)
.IP "\f(CW^\fP"
Exponentiation (right-associative).
The right argument
must be an integer.
If both arguments are integers,
the right argument must be non-negative
.IP "\f(CW* / %\fP"
.br
Multiplication, division, and remainder.
As in C,
and as not in \*(S4, multiplication and
division have the same precedence.
.IP "\f(CW+ -\fP"
Addition and subtraction.
.IP "\f(CW== != < > <= >= :==: :!=:\fP"
.IP "\f(CW:<: :>: :<=: :>=: :: :!:\fP"
.br
Comparison predicates.
Each of these operators returns
a null string if the indicated relation holds,
and fails if not.
The first six do
numeric comparisons: an error results if either
operand cannot be converted to a number.
The next six do string comparisons: an error results
if either operand cannot be converted to a string.
The last two test if the two operands are identical.
Values of different data types are never identical.
Strings and numbers are identical if their data types and values match.
Other values are identical if they refer to the same object.
.IP "\f(CW&&\fP"
Concatenation.
The left operand is evaluated
first; if its evaluation fails, the \f(CW&&\fP operator fails.
Otherwise, the right operand is
evaluated, and \f(CW&&\fP fails if the right operand fails.
If either operand is the null string, the
result is the other operand, even if that operand is not a string.
Otherwise, both operands are
converted to strings and the result is their concatenation.
An error results if either
operand cannot be converted to a string.
Note that if the operands of \f(CW&&\fP are predicates,
\f(CW&&\fP can be used as a kind of logical conjunction.
.IP "\f(CW||\fP"
Logical disjunction.
The left operand is evaluated first; if it
succeeds, its value is the value of \f(CW||\fP.
Otherwise, the value is that of the right operand.
If both operands fail, \f(CW||\fP fails.
.IP "\f(CW|\fP"
Pattern alternation.
See
.I "pattern matching"
for details.
.IP "\f(CW=\fP"
Assignment.
This operator is right-associative.
.IP "\f(CW?\fP"
Pattern match operator.
The left operand is
converted to a string and searched for the first
substring that matches the pattern given by the right operand.
If no such substring is found, the operator fails.
If the left operand is a variable, \f(CW?\fP may be used on the left
side of an assignment.
.NH 2
Unary Operators
.PP
Unary operators bind more tightly than all binary
operators.
.IP "\f(CW+ -\fP"
Unary plus and minus.
The result is always
numeric, so unary plus is sometimes used for type conversion.
Because unary operators bind so tightly, expressions that look like negative
constants behave that way for all practical purposes.
.IP "\f(CW.\fP"
Name operator.
The operand must be a variable;
the result is essentially a pointer to the variable, similarly
to the unary & operator in C.
The result of applying the \f(CWDATATYPE\fP function
to the result of the \f(CW.\fP operator is the string \f(CW"NAME"\fP.
.IP "\f(CW$\fP"
Indirection.
The operand is converted to a name;
the result is the object thus named.
\f(CW$\fP always yields an lvalue.
.IP "\f(CW?\fP"
Query.
If its operand fails, \f(CW?\fP fails.
If its operand succeeds, \f(CW?\fP yields a null string.
Useful for evaluating an expression solely for
its side effects.
.IP "\f(CW~\fP"
Logical negation.
If its operand fails, \f(CW~\fP yields a null string.
If the operand succeeds, \f(CW~\fP fails.
.IP "\f(CW&\fP"
Keyword value.
The operand must be the name of one of a restricted set of variables.
The lvalue result is a system variable, whose value affects the
execution of the program in some way.
System variables are discussed separately.
.IP "\f(CW@\fP"
Pattern cursor assignment.
See
.I "pattern matching"
for details.
.IP "\f(CW*\fP"
Deferred evaluation.
Returns a value of type \f(CWEXPRESSION\fP that
contains all the information necessary to evaluate the operand.
The operand is not actually evaluated at this time, but can be
evaluated later,
either by the \f(CWEVAL\fP procedure or implicitly during pattern matching.
.NH 2
Statements
.PP
Elements in brackets are optional.
If the description of a statement
is split over more than one line, the statement itself may be split
analogously.
Snocone does not have a null statement.
.Sx
\fIexpression\fP
.Sy
.Tx
The expression is evaluated for its side effects.
The result, if any, is discarded.
.Tm if	S
.Ty
.Sx
if (\fIexpression\fP)
	\fIstatement1\fP
[ else
	\fIstatement2\fP ]
.Sy
.Tx
The parenthesized
.I expression
is evaluated.
If it
succeeds,
.I statement1
is executed, otherwise
.I statement2
is executed.
.Ty
.Sx
while (\fIexpression\fP)
	\fIstatement\fP
.Sy
.Tx
Behaves similarly to C: the
.I expression
is evaluated,
and if it succeeds, the
.I statement
is executed and
control passes back to the beginning of the \f(CWwhile\fP
statement.
Unlike C, Snocone has no
.I break
or
.I continue
statements.
.Ty
.Sx
do
	\fIstatement\fP
while (\fIexpression\fP)
.Sy
.Tx
Behaves similarly to C: the
.I "statement"
is executed, then the
.I expression
is evaluated, and if the
.I expression
succeeds, control passes back to the beginning of the \f(CWdo\fP
statement.
.Tm for	S
.Ty
.Sx
for (\fIexpression1\fP, \fIexpression2\fP, \fIexpression3\fP)
	\fIstatement\fP
.Sy
.Tx
Equivalent to:
.P1 0
\fIexpression1\fP
while (\fIexpression3\fP) {
	\fIstatement\fP
	\fIexpression2\fP
}
.P2
.Ty
.Sx
{
	\fIstatement list\fP
}
.Sy
.Tx
The statements in the list, which may contain zero or
more statements, are executed in sequence.
.Ty
.Sx
\fIlabel\fP: \fIstatement\fP
.Sy
.Tx
All labels are global (because all \*(S4
labels are global), even across procedure boundaries,
so they must be chosen with care.
A useful convention is
to begin a label inside a procedure body with the name
of the procedure and an underscore.
.Ty
.Sx
go to \fIlabel\fP
.Sy
.Tx
The space between \f(CWgo\fP and \f(CWto\fP is optional.
It is wise
not to jump from a point inside one procedure into another.
Program
execution may be terminated by jumping to the reserved
label \f(CWEND\fP.
Labels \f(CWRETURN\fP, \f(CWFRETURN\fP, \f(CWNRETURN\fP, \f(CWABORT\fP, and
\f(CWCONTINUE\fP are also reserved.
.Ty
.Sx
return [ \fIexpression\fP ]
.Sy
.Tx
The current procedure returns to its caller.
If the
.I expression
is given, the procedure yields that value.
If no
.I expression
is given, the value returned is that of
the variable with the same name as the procedure; if
that variable was not assigned in the procedure, the
null string is returned.
.Ty
.Sx
freturn
.Sy
.Tx
The current procedure returns and fails.
.Ty
.Sx
nreturn [ \fIexpression\fP ]
.Sy
.Tx
The current procedure returns \f(CW$(\fP\fIexpression\fP\f(CW)\fP as an
lvalue.
If the
.I expression
is not given, the indirection
is applied to the variable with the same name as the
procedure.
An error results if this variable was not
given a value inside the procedure.
.NH 2
Procedures
.PP
Here is an example of a procedure declaration:
.P1 0
procedure gcd (m, n) {
	while (m != n) {
		if (m > n)
			m = m % n
		else
			n = n % m
	}
	return m
}
.P2
.PP
Arguments are passed by value, but note that the value passed for
an aggregate argument (array, table, or structure) is really a
pointer to the aggregate itself.
.PP
If a procedure is called with too few arguments, extra null strings
are supplied as necessary.
If called with too many arguments, the extras
are quietly ignored.
.PP
Local variables can be nominated for a procedure:
.P1 0
procedure f(x) y, z {...
.P2
All variables are global, but name scoping is dynamic.
One way to look
at it is to imagine that when a procedure
is entered, all the variables defined in that procedure are saved and
set to null.
Those variables are then restored when the procedure returns.
.PP
Thus, the following example prints 5 and then 1:
.P1 0
a = 1
f()
g()

procedure f() a {
	a = 5
	g()
}

procedure g() {
	OUTPUT = a
}
.P2
The following example also prints 5 and then 1:
.P1 0
a = 1
b = .a
f()
g()
.P3
procedure f() a {
	a = 5
	g()
}
.P3
procedure g() {
	OUTPUT = $b
}
.P2
Local variables can only be associated with procedures.
Procedures are
recursive.
.NH 2
Input-Output
.PP
All I/O is done through ``associated variables''.
A variable may be
input-associated or output-associated (or both).
Whenever a value is
assigned to an output-associated variable, that value is automatically
written in the file associated with that variable.
Whenever a value
is requested for an input-associated variable, a line is read from the
file associated with that variable, and the contents of the line are
used for the value.
When the end of an input file is reached,
any attempts to access variables associated
with that file will fail.
.PP
Initially, the variable \f(CWOUTPUT\fP is output-associated with the standard
output file, the variable \f(CWINPUT\fP is input-associated with the standard
input file, and the variable \f(CWTERMINAL\fP is both input-
and output-associated
with the user's terminal.
.PP
Thus, the following program copies its standard
input to its standard output, a line at a time:
.P1 0
while (OUTPUT = INPUT) {}
.P2
.PP
New associations are formed by the \f(CWINPUT\fP
and \f(CWOUTPUT\fP procedures:
.P1 0
INPUT  (\fIname\fP, \fIchannel\fP, \fIfile\fP)
OUTPUT (\fIname\fP, \fIchannel\fP, \fIfile\fP)
.P2
In both cases, 
.I name
is the name of the variable to be associated (the
name of the variable
.I x
is \f(CW.x\fP), and
.I file
is the file to be used.
.I Channel
is a string that you will use to identify subsequent operations
on that file.
Internally, there is a one-to-one correspondence between
channel names and file descriptors.
.PP
The file argument must not be given for other than the first call to
\f(CWINPUT\fP or \f(CWOUTPUT\fP on a given channel, as a channel can only be
connected to a single file.
.PP
Other procedures dealing with input-output are:
.Tm SET	S
.Sx
SET (\fIchannel\fP, \fIoffset\fP, \fIwhence\fP)
.Sy
.Tx
This function repositions the file specified by its
first argument in a system-dependent manner.
In implementations running under the
.UX
system, the behavior is similar to the
.I lseek
system call.
.Ty
.Sx
REWIND (\fIchannel\fP)
.Sy
.Tx
Repositions the named channel to the beginning of the file.
.Ty
.Sx
ENDFILE (\fIchannel\fP)
.Sy
.Tx
Indicates that you are done using the given channel.
All variables associated through that channel are
disassociated, output buffers are flushed (if any),
and the file is closed.
.Ty
.Sx
DETACH (\fIname\fP)
.Sy
.Tx
Disassociates the named variable.
Does not close
the file: a later call to \f(CWINPUT\fP or \f(CWOUTPUT\fP can reassociate it.
.NH 2
Pattern Matching
.PP
A
.I pattern
is a data structure that describes a class of strings.
Patterns
are used by the \f(CW?\fP operator,
which determines if the string given as its
left operand contains a substring described by the pattern given as its
right operand.
The part of the Snocone system that
does this is called the
.I scanner .
The input to the scanner is the string to be searched,
called the
.I subject ,
and the pattern sought.
.PP
The scanner tries to find a substring of the
subject that is matched by the pattern.
It first looks at substrings starting at
the first character of the subject.
If it doesn't find one, it tries the ones
starting at the second subject character,
and so on.
If the scanner cannot find an appropriate
substring, the pattern match fails.
.PP
If the \f(CW&ANCHOR\fP system variable is
nonzero, the scanner only looks at substrings
that start at the first character of the
subject.
This is said to be an
.I anchored
pattern match.
The \f(CW&ANCHOR\fP variable is zero at the
start of program execution.
.PP
The important part of pattern matching is therefore
determining whether the subject contains a
substring that starts at a given character
and matches a given pattern.
To understand how this is done, we must
take a closer look at patterns.
.PP
Every pattern is a concatenation of one or more
.I elements ,
each of which has zero or more
.I alternatives .
Each alternative may itself be an arbitrarily complicated pattern.
Some patterns have alternatives that are determined
dynamically as they are needed during pattern matching.
.PP
If a pattern has only one element, the scanner determines
if it matches at a given point by trying to match each
of the pattern's alternatives at that point.
If no alternative matches at that point, the element
cannot be matched.
.PP
If the pattern has more than one element,
matching that pattern at a given point means:
(a) finding an alternative for the first element
of the pattern that matches a subject substring
starting at the given point, and that also (b) allows
the remaining elements of the pattern to match
a substring that starts immediately after the
substring matched in (a).
.PP
During pattern matching, the scanner keeps its place
by means of an internal value called the
.I cursor ,
which represents the number of characters in the
subject that precede the current location.
These characters are counted from the beginning
of the subject even when the scanner is trying
to match a substring that starts at some other place
in the subject.
.PP
The concatenation of two patterns \f(CWP1\fP and \f(CWP2\fP
is a pattern whose elements
are \f(CWP1\fP and \f(CWP2\fP.
The alternation operator \f(CW|\fP similarly constructs alternatives.
When a string is used as a pattern, it matches only itself.
Thus,
.P1 0
"a" | "e" | "i" | "o" | "u"
.P2
is a pattern that matches any lower-case vowel.
.PP
Consider the following
statement:
.P1 0
P1 = ("ab" | "a") && ("b" | "c")
.P2
This assigns to \f(CWP\fP a pattern with two elements.
The first one matches
either \f(CWab\fP or \f(CWa\fP, and the second matches either
\f(CWb\fP or \f(CWc\fP.
Look at:
.P1 0
"ab" ? P1
.P2
The scanner first tries the first alternative of the first element of
P1 by matching \f(CWab\fP in the subject with \f(CWab\fP in the pattern.
This alternative matches, so the element (\f(CW"ab"|"a"\fP) matches, and
the scanner goes on to the second element (\f(CW"b"|"c"\fP).
Here, it will be unsuccessful
in matching both \f(CWb\fP and \f(CWc\fP,
because it has already exhausted all the
characters of the subject.
Thus the scanner fails to match the second element of
\f(CWP1\fP, and must back up and rematch the first element.
Fortunately, the
first element has an alternative in \f(CWa\fP;
this matches, and now the scanner
can try to match the second element (\f(CW"b"|"c"\fP) again.
The first alternative (\f(CWb\fP) succeeds, so
the \f(CW?\fP operator therefore succeeds.
If we wanted to examine just how the pattern elements matched,
we could have written:
.P1 0
P1a = ("ab"|"a").OUTPUT && ("b"|"c").OUTPUT
"ab" ? P1a
.P2
This would print \f(CWa\fP and \f(CWb\fP on separate lines,
showing that \f(CW"ab"|"a"\fP matched \f(CWa\fP and
\f(CW"b"|"c"\fP matched \f(CWb\fP.
.PP
When any pattern is first encountered
during pattern matching, it will match some string, and when the scanner
backs into it, it may match some other string.
Because many patterns behave
differently on their initial match than when rematched, it is useful to
describe the two cases separately.
.PP
For instance, when a string is used as a pattern, it initially matches
itself.
If the scanner later backs into it, it fails.
.PP
As another example, consider \f(CWP1|P2\fP.
This pattern matches every
possibility for \f(CWP1\fP, and when it has exhausted \f(CWP1\fP,
then matches every
possibility for \f(CWP2\fP before finally failing.
.PP
With this in mind, we can describe the various pattern-matching operators,
procedures, and pre-defined variables:
.IP "\f(CWP1 && P2\fP" 10
Tries to match \f(CWP1\fP, fails if it can't.
Then tries to match \f(CWP2\fP.
If \f(CWP2\fP fails, tries the next alternative for
\f(CWP1\fP, and then tries \f(CWP2\fP again.
Eventually, it either
runs out of alternatives for \f(CWP1\fP, in which case \f(CWP1&&P2\fP
fails, or it finds alternatives for both \f(CWP1\fP and \f(CWP2\fP which
allow them to match.
.IP "\f(CWP1 | P2\fP"
Tries to match \f(CWP1\fP.
If successful, \f(CWP1|P2\fP matches the
string that was matched by \f(CWP1\fP.
Otherwise, matches whatever
\f(CWP2\fP matches, and fails if \f(CWP2\fP fails.
.IP "\f(CWP $ V\fP"
Tries to match \f(CWP\fP.
If \f(CWP\fP matches, a copy of the substring
matched by \f(CWP\fP is immediately assigned to the variable \f(CWV\fP.
.IP "\f(CWP . V\fP"
Tries to match \f(CWP\fP.
If the entire pattern match of which
this element is a part is ultimately successful, a copy
of the substring matched by \f(CWP\fP is assigned to \f(CWV\fP after
the entire match completes.
.IP "\f(CW@N\fP"
Matches a null string, and immediately assigns
cursor position to the variable \f(CWN\fP.
The cursor position is
the number of characters that precede this null string
in the subject of the entire pattern match.
Thus:
.P1
"abcde" ? @OUTPUT && "c"
.P2
prints \f(CW0\fP, \f(CW1\fP, and \f(CW2\fP on separate lines.
.IP "\f(CWABORT\fP"
The entire pattern match is aborted.
.IP "\f(CWARB\fP"
A pre-defined variable that matches anything at all.
More specifically, it initially matches the null string.
When backed into, it matches a string one character longer
than the one it matched last time.
.IP "\f(CWBAL\fP"
A pre-defined variable that matches a non-empty parenthesis-
balanced string.
This string is balanced only with respect to parentheses,
and not any other kinds of brackets.
.IP "\f(CWFAIL\fP"
A pre-defined variable that always fails to match.
It
is sometimes useful to force the scanner to try all
alternatives for a pattern.
For instance:
.P1
s ARB $ OUTPUT && FAIL
.P2
prints all substrings of \f(CWs\fP.
.IP "\f(CWFENCE\fP"
Initially matches the null string, but if the scanner
backs into it, the entire pattern match is aborted.
Identical to to \f(CW""|ABORT\fP.
.IP "\f(CWREM\fP"
Matches from the current position to the end of the
subject.
Identical to \f(CWRTAB(0)\fP.
.IP "\f(CWSUCCEED\fP"
Identical to \f(CWARBNO("")\fP
.IP "\f(CWANY(s)\fP"
Matches any single character in \f(CWs\fP.
.IP "\f(CWARBNO(p)\fP"
A pre-defined procedure that yields a pattern that matches
zero or more copies of the pattern \f(CWp\fP.
Initially matches
the null string.
Each time the scanner backs into it,
it tries to extend the substring already matched by
matching one more instance of \f(CWp\fP.
.IP "\f(CWBREAK(s)\fP"
Matches a string starting at the current position up to
but not including the next character in the subject
that is also found somewhere in \f(CWs\fP.
Failure
if no such character is found.
No alternatives.
.IP "\f(CWBREAKX(s)\fP"
Like \f(CWBREAK\fP, but if backed into, it matches up to the
next subject character also found in \f(CWs\fP, and so on.
.IP "\f(CWLEN(n)\fP"
Matches exactly \f(CWn\fP characters.
.IP "\f(CWNOTANY(s)\fP"
Matches any single character not in \f(CWs\fP.
.IP "\f(CWPOS(n)\fP"
If exactly \f(CWn\fP subject characters precede the current
position, \f(CWPOS\fP matches the null string.
Otherwise it
fails.
.IP "\f(CWRPOS(n)\fP"
If exactly \f(CWn\fP subject characters remain to be matched,
\f(CWRPOS\fP matches the null string.
Otherwise it fails.
.IP "\f(CWRTAB(n)\fP"
If at least \f(CWn\fP subject characters remain in the subject,
\f(CWRTAB\fP matches characters until exactly \f(CWn\fP remain.
Otherwise
it fails.
.IP "\f(CWSPAN(s)\fP"
Starting at the current position, matches as many characters
as possible taken from \f(CWs\fP.
.IP "\f(CWTAB(n)\fP"
\f(CWTAB\fP matches from the current position forward up to
and including the \f(CWn\fPth character of the subject.
If more than \f(CWn\fP characters have already been matched
in the subject string, \f(CWTAB\fP fails.
.PP
All pattern-valued procedures can take an unevaluated expression as argument.
The expression will be evaluated when the pattern element is matched.
.SH
System Variables
.PP
The \f(CW&\fP operator looks at the name of its operand,
not its value.
For each of a small set of names, \f(CW&\fP yields a
.I "system variable" ,
whose value affects the operation of the system in some way.
For instance, we have already seen \f(CW&ANCHOR\fP, which
controls whether or not the scanner is restricted to examining
initial substrings of the subject.
The complete list of system variables is:
.IP "\f(CW&ABORT\fP" 13
The same value as the pre-defined variable \f(CWABORT\fP.
.IP "\f(CW&ALPHABET\fP"
A string that contains all the characters of the machine's
collating sequence, in order.
.IP "\f(CW&ANCHOR\fP"
If zero, all pattern matches are unanchored,
otherwise they are anchored.
.IP "\f(CW&ARB\fP"
The same value as the pre-defined variable \f(CWARB\fP.
.IP "\f(CW&BAL\fP"
The same value as the pre-defined variable \f(CWBAL\fP.
.IP "\f(CW&CODE\fP"
This variable is initially zero.
Its value is returned to the operating system when the
program finishes executing.
.IP "\f(CW&DUMP\fP"
This variable is initially zero.
If it is 1 at the end of execution, the values of
all variables are printed.
If it is 2, the values of all array, table, and structure
elements are also printed.
.IP "\f(CW&FAIL\fP"
The same value as the pre-defined variable \f(CWFAIL\fP.
.IP "\f(CW&FENCE\fP"
The same value as the pre-defined variable \f(CWFENCE\fP.
.IP "\f(CW&FNCLEVEL\fP"
The current level of procedure nesting.
.IP "\f(CW&INPUT\fP"
If this variable is set to 0, all
input association is suspended.
.IP "\f(CW&MAXLNGTH\fP"
The maximum length of a string.
This value cannot be increased beyond its
initial value.
.IP "\f(CW&OUTPUT\fP"
If this variable is set to 0, all output is suppressed
until it is again set nonzero.
.IP "\f(CW&REM\fP"
The same value as the pre-defined variable \f(CWREM\fP.
.IP "\f(CW&STCOUNT\fP"
A count of how many statements have been executed so far.
Because this counts \*(S4 statements, not
Snocone statements, it should be considered only approximate.
.IP "\f(CW&STLIMIT\fP"
When \f(CW&STCOUNT\fP reaches this value,
execution terminates with an error message.
It is initially 50000, and may be set freely.
If it is set to a negative value, statement
counting is disabled.
.IP "\f(CW&SUCCEED\fP"
The same value as the pre-defined variable \f(CWSUCCEED\fP.
.NH 1
Examples
.NH 2
Hello World
.PP
This is the canonical sample program:
.P1 0
OUTPUT = "Hello world!"
.P2
The present implementation of Snocone is case-insensitive
in identifiers, but future versions may well become
case-sensitive.
If so, it is likely that pre-defined names, such as
\f(CWINPUT\fP and \f(CWOUTPUT\fP, will have to be written
in upper case.
Keywords, such as \f(CWif\fP and \f(CWwhile\fP, will be
written in lower case.
.NH 2
Topological Sorting
.PP
We now develop
a topological sorting program based on the algorithm
described on page 262 of
.I "Fundamental Algorithms" |reference(knuth volume1).
The reader may wish to compare this program with the
\*(S4 program based on the same algorithm
that appears on pages 221-222 of
.I "The \*(S4 Programming Language" .
.PP
The input is
a set of pairs of objects,
where the first object in each pair
is considered to precede the second.
The output is a list of objects
in a sequence that meets all the constraints
implied by the input.
In other words, the program generates
a total ordering that includes a given
partial ordering.
.PP
If the input contains a loop, the program
will detect this fact and complain.
.PP
For example, given the following input:
.P1
letters alphanum
numbers alphanum
blanks optblanks
numbers real
numbers integer
letters variable
alphanum variable
binary binaryop
blanks binaryop
unqalphabet dliteral
unqalphabet sliteral
sliteral literal
dliteral literal
integer literal
real literal
.P2
the program will produce the following output:
.P1
letters
numbers
blanks
binary
unqalphabet
alphanum
real
integer
optblanks
binaryop
dliteral
sliteral
variable
literal
.P2
.PP
The basic strategy of the program is simple:
for each object, we remember how many immediate predecessors
it has, and store a list of all its immediate successors.
When we have finished reading the input,
we can immediately output the objects that have
no predecessors.
Each time we output an object, we remove it from
the data structure and decrement the predecessor
count of each of its immediate successors.
.PP
We will eventually reach a state in which we run
out of objects without predecessors.
When that happens, we are done.
If any objects remain, they form a loop.
.PP
To reduce the time spent searching for objects
without predecessors, we keep a queue of such
objects.
We must also keep a queue of the successors
of each object.
In both cases, we could use a stack instead of
a queue, but using a queue tends to favor the
order in which objects appeared in the input,
which makes the output more intuitively useful.
.PP
The following structure declarations and subroutines
manipulate queues.
A queue consists of a header (of type \f(CWqueue\fP)
which points to the first and last elements of
a singly-linked list of queue elements (of type \f(CWqel\fP).
.P1 0
struct queue {head, tail}
struct qel {obj, link}

.P3
procedure enqueue (q, x) y {
	if (head(q) :: "")
		head(q) = tail(q) = qel(x)
	else {
		y = qel(x)
		link(tail(q)) = y
		tail(q) = y
	}
}
.P3

procedure dequeue (q) x {
	if (head(q) :: "")
		freturn
	x = head(q)
	if ((head(q) = link(x)) :: "")
		tail(q) = ""
	return obj(x)
}
.P2
.PP
By convention, we use the null string to indicate
the end of a list.
This is convenient because uninitialized variables
and structure fields and missing arguments are automatically set to
the null string.
Thus, the test
.P1
head(q) :: ""
.P2
is a convenient way of testing whether \f(CWhead(q)\fP
has been set or not.
.PP
We represent each object as a structure containing
the object's name (so we can print it), the count of
immediate predecessors, and the queue of successors:
.P1
struct object {name, count, suc}
.P2
.PP
Since we will be reading the names of objects
rather than the objects directly, we will need
to map names to objects.
This can easily be done with a table and a mapping subroutine
that creates elements in the table as needed:
.P1 0
namemap = TABLE()
objects = queue()

procedure getobj (name) {
    if ((getobj = namemap[name]) :: "") {
        getobj = namemap[name] =
            object (name, 0, queue())
        enqueue (objects, getobj)
        nobj = nobj + 1
    }
}
.P2
.PP
This procedure uses the feature that if no return value
is explicitly given, the value of the variable with the
same name as the procedure is used.
If an appropriate table element already
exists, the \f(CW::\fP operator will fail
and the value of \f(CWgetobj\fP will be the
value retrieved from the table.
As a side effect, we maintain a global queue of
all known objects in \f(CWobjects\fP and
count them in \f(CWnobj\fP.
.PP
Now that we can map from names to objects, it is
an easy matter to enter a new relation into
our data structure.
Procedure \f(CWenter\fP takes the
names of two objects:
.P1 0
procedure enter (p, q) {
	p = getobj (p)
	q = getobj (q)
	count(q) = count(q) + 1
	enqueue (suc(p), q)
}
.P2
.PP
We first locate the objects to which \f(CWp\fP
and \f(CWq\fP refer, creating them if necessary.
Since \f(CWp\fP precedes \f(CWq\fP, we increment
the predecessor count of \f(CWq\fP and append
\f(CWq\fP to the successor list of \f(CWp\fP.
.PP
Building the data structure is now just a matter of
scanning the input file:
.P1 0
while (line = INPUT) {
   if (line ? FENCE && BREAK(' ').p &&
		SPAN(' ') && rem.q)
        enter (p, q)
    else
        TERMINAL = "bad input line: " && line
}
.P2
.PP
The pattern match assumes that everything up to the first
blank in the input line is the first object in a
relation, and everything after the first blank is
the second object.
If the match fails, the program complains.
.PP
Once all the input has been read, we must initialize
the queue of minimal objects (objects without predecessors).
This was the reason for keeping a queue of all objects,
which is now destroyed to build the queue of minimal objects.
It is not necessary to destroy the queue, but it is more
convenient because it is then possible to use \f(CWdequeue\fP:
.P1 0
zeroes = queue()

while (x = dequeue (objects))
	if (count (x) == 0)
		enqueue (zeroes, x)
.P2
.PP
As long as there is a minimal object,
we can print its name, delete it, and
decrement the predecessor count of each
of its successors.
If we decrement a predecessor count to
zero, that object is now minimal.
.P1 0
while (x = dequeue (zeroes)) {
    nobj = nobj - 1
    OUTPUT = name(x)
    while (y = dequeue (suc (x))) {
        if ((count(y) = count(y) - 1) == 0)
            enqueue (zeroes, y)
    }
}
.P2
.PP
This loop runs until there are no more minimal objects.
If there are still elements remaining (\f(CWnobj\fP is nonzero),
then those elements form a loop.
.P1 0
if (nobj != 0)
    TERMINAL = "The ordering contains a loop."
.P2
.NH 1
References
.LP
|reference_placement

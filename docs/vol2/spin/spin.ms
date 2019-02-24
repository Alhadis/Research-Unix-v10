.so ../ADM/mac
.XX spin 429 "Spin \(em A Protocol Analyzer"
.nr dP 2
.nr dV 3p
.EQ
delim @@
.EN
.de IH	\" makes a bold italic sub heading
.NH 2
\\$1
..
.ds P \\s-2PROMELA\\s0
.ds S \fISpin\fP
.ds s \fIspin\fP
.TL
Spin \(em A Protocol Analyzer
.AU "MH 2C-521" 6335
Gerard J. Holzmann
.AI
.MH
.AB
\*S is a tool for analyzing the logical consistency of
concurrent systems, specifically of data communication protocols.
The system is described in a modeling language called \*P.
The language allows for the dynamic creation of concurrent processes.
Communication via message channels can be defined to be synchronous
(i.e., rendez-vous), or asynchronous (i.e., buffered).
.PP
Given a model system specified in \*P, \*s
can either perform random simulations of the system's execution
or it can generate a C program that performs a fast exhaustive
validation of the system state space.
During simulations and validations \*s checks for the absence of deadlocks,
unspecified receptions, and unexecutable code.
The validator can also be used to verify the correctness of
system invariants, and it can find non-progress execution cycles.
.PP
The validator is setup to be extremely fast and to use
only a minimal amount of memory.
The exhaustive validations performed by \*s are conclusive,
They establish with certainty whether or not a given behavior
is error-free.
Very large validation runs, that can ordinarily not be
performed with automated techniques, can be
done in \*s with a novel ``bit state space'' technique.
With this method the state space is collapsed to a single
bit per system state stored, with minimal side-effects.
.PP
The first part of this memo gives an introduction to \*P,
the second part discusses the usage of \*s, and
the third part contains a brief reference manual for \*P.
In the appendix an example is used to illustrate the construction
of a basic \*P model for \*s validations.
.AE
.2C
.NH
Introduction to \*P
.PP
\*P is a validation modeling language.
It provides a vehicle for making abstractions of protocols
(or distributed systems in general) that suppress details
that are unrelated to process interaction.
The intended use of \*s is to validate fractions of process
behavior, that for one reason or another is considered suspect.
The relevant behavior is modeled in \*P and validated.
A complete validation is therefore typically performed in a series of steps,
with the construction of increasingly detailed \*P models.
Each model can be validated with \*s under different types of
assumptions about the environment (e.g., message loss, message
duplications etc).
Once the correctness of a model has been established with \*s, that
fact can be used in the construction and validation of all
subsequent models.
.PP
\*P programs consist of \f2processes\f1,
message \f2channels\f1, and \f2variables\f1.
Processes are global objects.
Message channels and variables can be declared either globally
or locally within a process.
Processes specify behavior; channels and global variables
define the environment in which the processes run.
.IH Executability
.PP
In \*P there is no difference between conditions and
statements, even isolated boolean conditions can be used as statements.
The execution of every statement is conditional on its
.I executability .
Statements are either executable or blocked.
The executability is the basic means of synchronization.
A process can wait for an event to happen by waiting
for a statement to become executable.
For instance, instead of writing a busy wait loop:
.P1 0
while (a != b)
	skip	/* wait for a==b */
.P2
one can achieve the same effect in \*P with the statement
.P1 0
(a == b)
.P2
A condition can only be executed (passed) when it holds.
If the condition does not hold, execution blocks until it does.
.PP
Variables are used to store either global information about the
system as a whole, or information local to one specific process,
depending on where the declaration for the variable is placed.
The declarations
.P1 0
bool flag;
int state;
byte msg;
.P2
define variables that can store integer values in
three different ranges.
The scope of a variable is global if it is declared outside all
process declarations, and local if it is declared within a process
declaration.
.IH "Data Types"
.PP
Table 1 summarizes the basic data types, sizes,
and the corresponding value ranges (on a DEC VAX computer).
.KS
.SP .5
.ps -1
.vs -2
.TS
center;
l l l l
lFCW n r c.
=
Name	Size (bits)	Usage	Range
_
bit	1	unsigned	0..1
bool	1	unsigned	0..1
byte	8	unsigned	0..255
short	16	signed	@-2 sup 15@..@{2 sup 15} - 1@
int	32	signed	@-2 sup 31@..@{2 sup 31} - 1@
_
.TE
.ps +1
.vs +2
.SP .5
.ce
\fBTable 1.\fP Data Types
.SP
.KE
The names
.CW bit
and
.CW bool
are synonyms for a single bit of information.
A
.CW byte
is an unsigned quantity that can store a value between
.CW 0
and
.CW 255 .
.CW short s
and
.CW int s
are signed quantities that
differ only in the range of values they can hold.
.IH "Array Variables"
.PP
Variables can be declared as arrays.
For instance,
.P1 0
byte state[N]
.P2
declares an array of
.CW N
bytes that can be accessed in statements such as
.P1 0
state[0] = state[3] + 5 * state[3*2/n]
.P2
where
.CW n
is a constant or a variable declared elsewhere.
The index to an array can be any expression that
determines a unique integer value.
The effect of an index value outside the range
.CW "0 .. N-1"
is undefined; most likely it will cause a runtime error.
.PP
So far we have seen examples of variable declarations
and of two types of statements: boolean conditions and
assignments.
Declarations and assignments are always \f2executable\f1.
Conditions are only executable when they hold.
.IH "Process Types"
.PP
The state of a variable or of a message channel
can only be changed or inspected by processes.
The behavior of a process is defined in a
.CW proctype
declaration.
The following, for instance, declares a process with one local variable
.CW state .
.P1 0
proctype A()
{	byte state;

	state = 3
}
.P2
The process type is named
.CW A .
The body of the declaration is enclosed in curly braces.
The declaration body consists of a list of zero or more
declarations of local variables and/or statements.
The declaration above contains one local variable declaration
and a single statement: an assignment of the
value 3 to variable
.CW state .
.PP
The semicolon is a statement \f2separator\f1 (not a statement terminator,
hence there is no semicolon after the last statement).
\*P accepts two different statement separators:
.CW `->'
(arrow) and
.CW `;' .
The two statement separators are equivalent.
The arrow is sometimes used as an informal way to indicate a causal
relation between two statements.
Consider the following example.
.P1 0
byte state = 2;

proctype A()
{	(state == 1) -> state = 3
}
.P3
proctype B()
{	state = state \- 1
}
.P2
In this example we declared two types of processes,
.CW A
and
.CW B .
Variable
.CW state
is now a global, initialized to the value two.
Process type
.CW A
contains two statements, separated by an arrow.
In the example, process declaration
.CW B
contains a single statement that decrements
the value of the state variable by one.
Since the assignment is always executable,
processes of type
.CW B
can always complete without delay.
Processes of type
.CW A ,
however, are delayed at the condition until
the variable
.CW state
contains the proper value.
......
.IH "Process Instantiation"
.PP
A
.CW proctype
definition only declares process behavior, it
does not execute it.
Initially, in the \*P model, just one process will be executed:
a process of type
.CW init ,
that must be declared explicitly in every \*P specification.
The smallest possible \*P specification, therefore, is:
.P1 0
init { skip }
.P2
where
.CW skip
is a dummy, null statement.
More interestingly, however,
the initial process can initialize global variables,
and instantiate processes.
An
.CW init
declaration for the above system, for instance, could look as follows.
.P1 0
init
{	run A(); run B()
}
.P2
.PP
.CW run
is used as a unary operator that takes the name of a process type (e.g.
.CW A ).
It is executable only if a process of
the type specified can be instantiated.
It is unexecutable if this cannot be done,
for instance if too many processes are already running.
.PP
The
.CW run
statement can pass parameter values of all basic
data types to the new process.
The declarations are then written, for instance, as follows:
.P1 0
proctype A(byte state; short foo)
{
	(state == 1) -> state = foo
}
.P3
init
{
	run A(1, 3)
}
.P2
Data arrays or process types can not be passed as parameters.
As we will see below, there is just one other data type that
can be used as a parameter: a message channel.
.PP
.CW Run
statements can be used in any process to spawn new processes,
not just in the initial process.
Processes are created with the
.CW run
statements.
An executing process disappears again when it terminates
(i.e., reaches the end of the body of its
process type declaration), but not before all processes
that it started have terminated.
.PP
With the
.CW run
statement we can create any number of copies of the process types
.CW A
and
.CW B .
If, however, more than one concurrent process is allowed to both read and
write the value of a global variable a well-known set of problems
can result; for example see |reference(dijkstra concurrent).
Consider, for instance, the following system of
two processes, sharing access to the global variable
.CW state .
.P1 0
byte state = 1;

proctype A()
{	(state==1) -> state = state+1
}
.P3
proctype B()
{	(state==1) -> state = state\-1
}
.P3
init
{	run A(); run B()
}
.P2
If one of the two processes completes before its competitor has
started, the other process will block forever on the initial condition.
If both pass the condition simultaneously, both will complete, but
the resulting value of
.CW state
is unpredictable.
It can be any of the values
.CW 0 ,
.CW 1 ,
or
.CW 2 .
.PP
Many solutions to this problem have been considered, ranging from
an abolishment of global variables to the provision of special
machine instructions that can guarantee an indivisible test and
set sequence on a shared variable.
The example below was one of the first solutions published.
It is due to the Dutch mathematician Dekker.
It grants two processes mutually exclusion access to an arbitrary
.I
critical section
.R
in their code, by manipulation three additional global variables.
The first four lines in the \*P specification
below are C-style macro definitions.
The first two macros define
.CW true
to be a constant value equal to
.CW 1
and
.CW false
to be a constant
.CW 0 .
Similarly,
.CW Aturn
and
.CW Bturn
are defined as constants.
.P1 0
#define true	1
#define false	0
#define Aturn	false
#define Bturn	true

bool x, y, t;
.P3

proctype A()
{	x = true;
	t = Bturn;
	(y == false || t == Aturn);
	/* critical section */
	x = false
}
.P3
proctype B()
{	y = true;
	t = Aturn;
	(x == false || t == Bturn);
	/* critical section */
	y = false
}
.P3
init
{	run A(); run B()
}
.P2
The algorithm can be executed repeatedly and is independent of
the relative speeds of the two processes.
.IH "Atomic Sequences"
.PP
In \*P there is also another way to avoid the \f2test and set\f1
problem:
.CW atomic
sequences.
By prefixing a sequence of statements enclosed in curly
braces with the keyword
.CW atomic
the user can indicate that the sequence is to be executed
as one indivisible unit, non-interleaved with any other
processes.
It causes a run-time error if any statement, other than
the first statement, blocks in an atomic sequence.
This is how we can use atomic sequences to protect the
concurrent access to the global variable
.CW state
in the earlier example.
.P1 0
byte state = 1;

proctype A()
{	atomic {
	  (state==1) -> state = state+1
	}
}
.P3
proctype B()
{	atomic {
	  (state==1) -> state = state\-1
	}
}
.P3
init
{	run A(); run B()
}
.P2
In this case the final value of
.CW state
is guaranteed to be zero, though during the execution of
.CW A
and
.CW B
the intermediate value can be either
.CW 0
or
.CW 2 .
.PP
Atomic sequences can be an important tool in reducing
the complexity of validation models.
Note that atomic sequence restricts the amount of
interleaving that is allowed in a distributed system.
Otherwise untractable models can be made tractable
by, for instance, labeling all manipulations of local variables
with atomic sequences.
The reduction in complexity can be dramatic.
.IH "Message Passing"
.PP
Message channels are used to model the transfer of data
from one process to another.
They are declared either locally or globally,
for instance as follows:
.P1 0
chan qname[16] of { short }
.P2
This declares a channel that can store up
to 16 messages of type
.CW short .
Channel names can be passed from one process to another via
channels or as parameters in process instantiations.
If the messages to be passed by the channel have more than
one field, the declaration may look as follows:
.P1 0
chan qname[16] of { byte, int, chan, byte }
.P2
This time each message in the channel stores up to
sixteen messages, each consisting of two 8-bit values,
one 32-bit value, and a channel name.
.PP
.Tm !	S
.Tm ?	S
The statement
.P1 0
qname!expr
.P2
sends the value of expression
.CW expr
to the channel that we just created, that is:
it appends the value to the tail of the channel.
.P1 0
qname?msg
.P2
receives the message, it retrieves it from the head of the channel,
and stores it in a variable
.CW msg .
The channels pass messages in first-in-first-out order.
In the above cases only a single value
is passed through the channel.
If more than one value is to be transferred per message,
they are specified in a comma separated list
.P1 0
qname!expr1,expr2,expr3
qname?var1,var2,var3
.P2
If more parameters are sent per message then the message channel
can store, the redundant parameters are lost without warning.
If fewer parameters are sent then the message channel can store,
the value of the remaining parameters is undefined.
Similarly, if the receive operation tries to retrieve more
parameters than available, the value of the extra parameters is
undefined; if it receives fewer than the number of parameters
that was sent, the extra information is lost.
.PP
By convention, the first message field is often
used to specify the message type (i.e. a constant).
An alternative, and equivalent, notation for the
send and receive operations is therefore to specify the
message type, followed by a list of message fields
enclosed in braces.
In general:
.P1 0
qname!expr1(expr2,expr3)
qname?var1(var2,var3)
.P2
.PP
The send operation is executable only when the channel addressed is not full.
The receive operation, similarly, is only executable
when the channel is non empty.
Optionally, some of the arguments of the receive operation
can be constants:
.P1 0
qname?cons1,var2,cons2
.P2
in this case, a further condition on the executability of the
receive operation is that the value of all message fields that are
specified as constants match the value of the corresponding fields
in the message that is at the head of the channel.
Again, nothing bad will happen if a statement happens to be non-executable.
The process trying to execute it will be delayed until the
statement, or, more likely, an alternative statement, becomes executable.
.PP
Here is an example that uses some of the mechanisms introduced
so far.
.P1 0
proctype A(chan q1)
{	chan q2;
	q1?q2;
	q2!123
}
.P3
proctype B(chan qforb)
{	int x;
	qforb?x;
	printf("x = %d\n", x)
}
.P3
init {
	chan qname[1] of { chan };
	chan qforb[1] of { int };
	run A(qname);
	run B(qforb);
	qname!qforb
}
.P2
The value printed will be
.CW 123 .
.PP
A predefined function
.CW len(qname)
returns the number of messages currently
stored in channel
.CW qname .
Note that if
.CW len
is used as a statement, rather than on
the right hand side of an assignment, it will be unexecutable if
the channel is empty: it returns a zero result, which by definition
means that the statement is temporarily unexecutable.
Composite conditions such as
.P1 0
(qname?var == 0)
.P2
or
.P1 0
(a > b && qname!123)
.P2
are invalid in \*P (note that these conditions can not be evaluated
without side-effects).
For a receive statement there is an alternative, using square
brackets around the clause behind the question mark.
.P1 0
qname?[ack,var]
.P2
is evaluated as a condition.
It returns
.CW 1
if the corresponding receive statement
.P1 0
qname?ack,var
.P2
is executable, i.e., if there is indeed a message
.CW ack
at the head of the channel.
It returns
.CW 0
otherwise.
In neither case has the evaluation of a statement such as
.P1 0
qname?[ack,var]
.P2
any side-effects: the receive is evaluated, not executed.
.PP
Note carefully that in non-atomic sequences of two statements such as
.P1 0
(len(qname) < MAX) -> qname!msgtype
.P2
or
.P1 0
qname?[msgtype] -> qname?msgtype
.P2
the second statement is not \f2necessarily\f1 executable
after the first one has been executed.
There may be race conditions if access to the channels
is shared between several processes.
In the first case
another process can send a message to channel
.CW qname
just after this process determined that the channel was not full.
In the second case, the other process can steal away the
message just after our process determined its presence.
.IH "Rendez-Vous Communication"
.PP
So far we have talked about asynchronous communication between processes
via message channels, declared in statements such as
.P1 0
chan qname [N] of { byte }
.P2
where
.CW N
is a positive constant that defines the buffer size.
A logical extension is to allow for the declaration
.P1 0
chan port [0] of { byte }
.P2
to define a rendez-vous port that can pass single byte messages.
The channel size is zero, that is, the channel
.CW port
can pass, but can not store messages.
Message interactions via such rendez-vous ports are
by definition synchronous.
Consider the following example.
.P1 0
#define msgtype 33

chan name [0] of { byte, byte };

proctype A()
{	name!msgtype(124);
	name!msgtype(121)
}
.P3
proctype B()
{	byte state;
	name?msgtype(state)
}
.P3
init
{	atomic { run A(); run B() }
}
.P2
Channel
.CW name
is a global rendez-vous port.
The two processes will synchronously execute their first statement:
a handshake on message
.CW msgtype
and a transfer of the value 124 to local variable
.CW state .
The second statement in process
.CW A
will be unexecutable,
because there is no matching receive operation in process
.CW B .
.PP
If the channel
.CW name
is defined  with a non-zero buffer capacity,
the behavior is different.
If the buffer size is at least 2, the process of type
.CW A
can complete its execution, before its peer even starts.
If the buffer size is 1, the sequence of events is as follows.
The process of type
.CW A
can complete its first send action, but it blocks on the
second, because the channel is now filled to capacity.
The process of type
.CW B
can then retrieve the first message and complete.
At this point
.CW A
becomes executable again and completes,
leaving its last message as a residual in the channel.
.PP
Rendez-vous communication is binary: only two processes,
a sender and a receiver, can be synchronized in a
rendez-vous handshake.
We will see an example of a way to exploit this to
build a semaphore below.
But first, let us introduce a few more control flow structures
that may be useful.
.NH 2
Control Flow
.PP
Between the lines, we have already introduced three ways of
defining control flow: concatenation of statements
within a process, parallel execution of processes, and
atomic sequences.
There are three other control flow constructs in \*P to be discussed.
They are case selection,
repetition, and
unconditional jumps.
.NH 3
Case Selection
.PP
The simplest construct is the selection structure.
Using the relative values of two variables
.CW a
and
.CW b
to choose between two options, for instance, we can write:
.P1 0
if
:: (a != b) -> option1
:: (a == b) -> option2
fi
.P2
The selection structure contains two execution sequences,
each preceded by a double colon.
Only one sequence from the list will be executed.
A sequence can be selected only if its first statement is executable.
The first statement is therefore called a \f2guard\f1.
.PP
In the above example the guards are mutually exclusive, but they
need not be.
If more than one guard is executable, one of the corresponding sequences
is selected nondeterministically.
If all guards are unexecutable the process will block until at least
one of them can be selected.
There is no restriction on the type of statements that can be used
as a guard.
The following example, for instance, uses input statements.
.P1 0
#define a 1
#define b 2

chan ch[1] of { byte };

proctype A()
{	ch!a
}
.P3
proctype B()
{	ch!b
}
.P3
proctype C()
{	if
	:: ch?a
	:: ch?b
	fi
}
.P3
init
{	atomic { run A(); run B(); run C() }
}
.P2
The example defines three processes and one channel.
The first option in the selection structure of the process
of type
.CW C
is executable if the channel contains
a message
.CW a ,
where
.CW a
is a constant with value
.CW 1 ,
defined in a macro definition at the start of the program.
The second option is executable if it contains a message
.CW b ,
where, similarly,
.CW b
is a constant.
Which message will be available depends on the unknown
relative speeds of the processes.
.PP
A process of the following type will either increment
or decrement the value of variable
.CW count
once.
.P1 0
byte count;

proctype counter()
{
	if
	:: count = count + 1
	:: count = count \- 1
	fi
}
.P2
.NH 3
Repetition
.PP
A logical extension of the selection structure is
the repetition structure.
We can modify the above program as follows, to obtain
a cyclic program that randomly changes the value of
the variable up or down.
.P1 0
byte count;

proctype counter()
{
	do
	:: count = count + 1
	:: count = count \- 1
	:: (count == 0) -> break
	od
}
.P2
.PP
Only one option can be selected for execution at a time.
After the option completes, the execution of the structure
is repeated.
The normal way to terminate the repetition structure is
with a
.CW break
statement.
In the example, the loop can be
broken when the count reaches zero.
Note, however, that it need
not terminate since the other two options always remain executable.
To force termination we could modify the program as follows.
.P1 0
proctype counter()
{
	do
	:: (count != 0) ->
		if
		:: count = count + 1
		:: count = count \- 1
		fi
	:: (count == 0) -> break
	od
}
.P2
.NH 3
Unconditional Jumps
.PP
Another way to break the loop is with an unconditional jump:
the infamous
.CW goto
statement.
This is illustrated in the following implementation of Euclid's algorithm for
finding the greatest common divisor of two non-zero, positive numbers:
.P1 0
proctype Euclid(int x, y)
{
	do
	:: (x >  y) -> x = x \- y
	:: (x <  y) -> y = y \- x
	:: (x == y) -> goto done
	od;
done:
	skip
}
.P2
The
.CW goto
in this example jumps to a label named
.CW done .
A label can only appear before a statement.
Above we want to jump to the end of the program.
In this case a dummy statement
.CW skip
is useful: it is a place holder that
is always executable and has no effect.
The
.CW goto
is also always executable.
.PP
The following example specifies a filter that receives
messages from a channel
.CW in
and divides them over two channels
.CW large
and
.CW small
depending on the values attached.
The constant
.CW N
is defined to be
.CW 128
and
.CW size
is defined to be
.CW 16
in the two macro definitions.
.P1 0
#define N    128
#define size  16
.P3

chan in    [size] of { short };
chan large [size] of { short };
chan small [size] of { short };
.P3

proctype split()
{	short cargo;

	do
	:: in?cargo ->
		if
		:: (cargo >= N) ->
			large!cargo
		:: (cargo <  N) ->
			small!cargo
		fi
	od
}
.P3
init
{	run split()
}
.P2
A process type that merges the two streams back into one, most
likely in a different order, and writes it back
into the channel
.CW in
could be specified as follows.
.P1 0
proctype merge()
{	short cargo;

	do
	::	if
		:: large?cargo
		:: small?cargo
		fi;
		in!cargo
	od
}
.P2
If we now modify the
.CW init
process as follows, the
split and merge processes could busily perform their
duties forever on.
.P1 0
init
{	in!345; in!12; in!6777;
	in!32;  in!0;
	run split();
	run merge()
}
.P2
.PP
As a final example, consider the following implementation of
a Dijkstra semaphore, using binary rendez-vous communication.
.P1 0
#define p	0
#define v	1

chan sema[0] of { bit };
.P3
proctype dijkstra()
{	byte count = 1;

	do
	:: (count == 1) \->
		sema!p; count = 0
	:: (count == 0) \->
		sema?v; count = 1
	od	
}
.P3
proctype user()
{	do
	:: sema?p;
	   /*     critical section */
	   sema!v;
	   /* non-critical section */
	od
}
.P3
init
{	run dijkstra();
	run user();
	run user();
	run user()
}
.P2
The semaphore guarantees that only one of the user processes
can enter its critical section at a time.
It does not necessarily prevent the monopolization of
the access to the critical section by one of the processes.
.IH "Modeling Procedures and Recursion"
.PP
Procedures can be modeled as processes, even recursive ones.
The return value can be passed back to the calling process
via a global variable, or via a message.
The following program illustrates this.
.P1 0
proctype fact(int n; chan p)
{	chan child[1] of { int };
	int result;

	if
	:: (n <= 1) -> p!1
	:: (n >= 2) ->
		run fact(n-1, child);
		child?result;
		p!n*result
	fi
}
init
{	chan child [1] of { int };
	int result;

	run fact(7, child);
	child?result;
	printf("result: %d\n", result)
}
.P2
The process
.I "fact(n, p)"
recursively calculates the factorial of
.I n ,
communicating the result via a message to its parent process
.I p .
.IH "Timeouts"
.PP
We have already discussed two types of statement
with a predefined meaning in \*P:
.CW skip ,
and
.CW break .
Another predefined statement is
.CW timeout .
The
.CW timeout
models a special condition that allows a process to
abort the waiting for a condition that may never become true, e.g.
an input from an empty channel.
The timeout keyword is a modeling feature in \*P that provides an
escape from a hang state.
The timeout condition becomes true only when no other
statements within the distributed system is executable.
Note that we deliberately abstract from absolute timing
considerations, which is crucial in validation work,
and we do not specify how the timeout should be implemented.
A simple example is the following process that will send
a reset message to a channel named \f2guard\f1 whenever the
system comes to a standstill.
.P1 0
proctype watchdog()
{
	do
	:: timeout -> guard!reset
	od
}
.P2
.IH "Assertions"
.PP
Another important language construct in \*P that
needs little explanation is the
.CW assert
statement.
Statements of the form
.P1 0
assert(any_boolean_condition)
.P2
are always executable.
If the boolean condition specified holds, the statement has no effect.
If, however, the condition does not necessarily hold,
the statement will produce an error report during validations with \*s.
.NH 2
More Advanced Usage
.PP
The modeling language has a few features that specifically address
the validation aspects.
It shows up in the way labels are used, in the
semantics of the \*P
.CW timeout
statement, and in the usage of statements such as
\f(CWassert\f1 that we discuss next.
.NH 3
End-State Labels
.PP
When \*P is used as a validation language the user must
be able to make very specific assertions about the behavior
that is being modeled.
In particular, if a \*P is checked for the presence of
deadlocks, the validator must be able to distinguish a normal \f2end state\f1
from an abnormal one.
.PP
A normal end state could be a state in which every \*P process
that was instantiated has properly reached the end of the
defining program body, and all message channels are empty.
But, not all \*P process are, of course, meant to reach the
end of their program body.
Some may very well linger in an \f(CWIDLE\f1
state, or they may sit patiently in a loop
ready to spring into action when new input arrives.
.PP
To make it clear to the validator that these alternate end states
are legal, and do not constitute a deadlock, a \*P model can use
end state labels.
For instance, if by adding a label to the process type
\f(CWdijkstra()\f1, from section 1.9:
.P1
proctype dijkstra()
{	byte count = 1;

end:	do
	:: (count == 1) \->
		sema!p; count = 0
	:: (count == 0) \->
		sema?v; count = 1
	od	
}
.P2
we indicate that it is not an error if, at the end of an
execution sequence, a process of type \f(CWdijkstra()\f1
has not reached its closing curly brace, but waits in the loop.
Of course, such a state could still be part of a deadlock state, but
if so, it is not caused by this particular process.
(It will still be reported if any one of the other processes
in not in a valid end-state).
.PP
There may be more than one end state label per validation model.
If so, all labels that occur within the same process body must
be unique.
The rule is that every label name that \f2starts\f1 with the three
character sequence \f(CW"end"\f1
is an endstate label.
So it is perfectly valid to use variations such as
\f(CWenddne\f1, \f(CWend0\f1, \f(CWend_appel\f1, etc.
.NH 3
Progress-State Labels
.PP
In the same spirit as the end state labels, the user can also
define \f2progress state\f1 labels.
In this case, a progress state labels will mark a state that
\f2must\f1 be executed for the protocol to make progress.
Any infinite cycle in the protocol execution that does not
pass through at least one of these progress states, is a
potential starvation loop.
In the
.CW dijkstra
example, for instance, we can label the
successful passing of a semaphore test as ``progress'' and
ask a validator to make sure that there is no cycle in the
protocol execution where at least one process succeeds in
passing the semaphore guard.
If more than one state carries a progress label,
variations with a common prefix are again valid:
\f(CWprogress0\f1, \f(CWprogress_foo\f1, etc.
.KF
.P1
proctype dijkstra()
{	byte count = 1;

end:	do
	:: (count == 1) ->
progress:	sema!p; count = 0
	:: (count == 0) ->
		sema?v; count = 1
	od	
}
.P2
.KE
.PP
.CW "spin -a"
generates analyzers that support (after compilation) a
.CW -l
option, which makes the analyzer use a fast search for non-progress loops,
instead of the default search for deadlocks.
The
.CW -l
search completely avoids the expense of a
full construction of all strongly
connected components in the reachability graph
(the conventional method for doing loop analysis).
The expense is therefore never more than about
twice the time and memory requirements of
a default search for deadlocks.
.IH "Message Type Definitions"
.PP
We have seen how variables are declared and how constants
can be defined using C-style macros.
As a mild form of syntactic sugar, \*P also allows for
message type definitions that look as follows:
.P1 0
mtype = {
	ack, nak, err,
	next, accept
}
.P2
This is a preferred way of specifying the message types since
it abstracts from the specific values to be used, and it makes
the names of the constants available to an implementation,
which can improve error reporting.
.IH "Pseudo Statements"
.PP
We have now discussed all the basic types of statements defined in \*P:
assignments, conditions, send and receive,
.CW assert ,
.CW timeout ,
.CW goto ,
.CW break
and
.CW skip .
Note that
.CW chan ,
.CW len 
and
.CW run
are not really statements but unary operators that can be used in
conditions and assignments.
.PP
The
.CW skip
statement was mentioned in passing as a statement that can be
a useful filler to satisfy syntax requirements, but that really
has no effect.
It is formally not part of the language but a \f2pseudo-statement\f1,
merely a synonym of another statement with the same effect: a
simple condition of a constant value
.CW (1) .
In the same spirit other pseudo-statements could be
defined, such as
\f(CWblock\f1 or \f(CWhang\f1, equivalents of \f(CW(0)\f1,
and
\f(CWhalt\f1, as an equivalent of \f(CWassert(0)\f1..
.IH Example
.PP
Here is a simple example of a (flawed) protocol, modeled in \*P.
.P1 0
mtype = {
	ack, nak, err, next, accept
}

.P3
proctype transfer(chan in,out,chin,chout)
{	byte o, i;

	in?next(o);
.P3
	do
	:: chin?nak(i) ->
			out!accept(i);
			chout!ack(o)
.P3
	:: chin?ack(i) ->
			out!accept(i);
			in?next(o);
			chout!ack(o)
.P3
	:: chin?err(i) ->
			chout!nak(o)
	od
}

.P3
init
{	chan AtoB[1] of { byte, byte };
	chan BtoA[1] of { byte, byte };
.P3
	chan Ain [2] of { byte };
	chan Bin [2] of { byte };
.P3
	chan Aout[2] of { byte };
	chan Bout[2] of { byte };
.P3
	atomic {
	  run transfer(Ain,Aout, AtoB,BtoA);
	  run transfer(Bin,Bout, BtoA,AtoB)
	};
.P3
	AtoB!err(0)
}
.P2
The channels
.CW Ain
and
.CW Bin
are to be filled with
token messages of type
.CW next
and arbitrary values (e.g.
ASCII character values) by unspecified background processes:
the users of the transfer service.
Similarly, these user processes
can read received data from the channels
.CW Aout
and
.CW Bout .
The channels and processes are initialized in a single
atomic statement, and started with the dummy
.CW err
message.
.NH
Introduction to Spin
.PP
Given a model system specified in \*P, \*s
can either perform random simulations of the system's execution
or it can generate a C program that performs a fast exhaustive
validation of the system state space.
The validator can check, for instance, if user specified system
invariants may be violated during a protocol's execution.
.PP
If \*s is invoked without any options it performs a random simulation.
With option
.CW -n\fIN
the seed for the simulation is set explicitly to the integer value
.I N .
.PP
The options
.CW pglrs
controls the amount of information output from the simulation run.
Every line of output normally contains a reference to the source
line in the specification that caused it.
.IP \f(CW-p\f1
Shows the state changes of the \*P
processes at every time step.
.IP \f(CW-g\f1
Shows the current value of global variables at every time step.
.IP \f(CW-l\f1
Shows the current value of local variables, after the
process that owns them has changed state.
It is best used in combination with option
.CW -p .
.IP \f(CW-r\f1
Shows all message receive events.
It shows the process performing the receive, its name and number,
the source line number, the message parameter number (there is
one line for each parameter), the message type and the message
channel number and name.
.IP \f(CW-s\f1
Shows all message send events.
.LP
\*S understands four other options:
.IP \f(CW-a\f1
Generates a protocol specific analyzer.
The output is written into a set of C files, named
.CW pan.[cbhmt] ,
that can be compiled to produce the analyzer
(which is then executed to perform the analysis).
To guarantee an exhaustive exploration of the state space, the
program can be compiled simply as
.RS
.IP
.P1
$ cc -o run pan.c
.P2
.RE
.IP
For larger systems this may, however, exhaust the available memory
on the machine used.
Large to very large systems can still be analyzed by using a
memory efficient bit state space method by
.RS
.IP
.P1
$ cc -DBITSTATE -o run pan.c
.P2
.RE
.IP
An indication of the coverage of such a search can be derived from the
.I "hash factor"
(see below).
The generated executable analyzer, named
.CW run
above, has its own set of options that can be seen by typing
.CW "run -?"
(see also below in ``The Analyzer'').
.IP \f(CW-m\f1
can be used to change the default semantics of send actions.
Normally, a send operation is only executable if the target channel
is non-full.
This imposes an implicit synchronization that can not always
be justified.
Option \f(CW-m\f1 causes send actions to be always executable.
Messages sent to a channel that is full are then dropped.
If this option is combined with \f(CW-a\f1 the semantics of send
in the analyzers generated is similarly altered, and the validations
will take the effects of this type of message loss into consideration.
.IP \f(CW-q\f1
causes \*s to peruse the model for obviously atomicable
sequences, and to label them appropriately, in an effort to
reduce the complexity of large validation runs.
Typically, the user can do better by hand (by being more
daring than \*s can be in this case).
.IP \f(CW-t\f1
is a trail-hunting option.
If the analyzer finds a violation of an assertion, a deadlock or
an unspecified reception, it writes an error trail into a file
named
.CW pan.trail .
The trail can be inspected in detail by invoking \*s with the
.CW -t
option.
In combination with the options
.CW pglrs
different views of the error sequence are then easily obtained.
.PP
For brevity, other features of \*s are not discussed here.
For details see|reference(holzmann spinbook), for a hint of
what else is available, see ``Digging Deeper'' at the end of this manual.
.IH "The Simulator"
.PP
Consider the following example protocol, that we will store in a
file named
.CW lynch .
.P1 0
   1  #define MIN	9
   2  #define MAX	12
   3  #define FILL	99
   4  
   5  mtype = { ack, nak, err }
   6  
.P3
   7  proctype transfer(chan chin, chout)
   8  {	byte o, i, last_i=MIN;
   9  
  10  	o = MIN+1;
.P3
  11  	do
  12  	:: chin?nak(i) ->
  13  		assert(i == last_i+1);
  14  		chout!ack(o)
.P3
  15  	:: chin?ack(i) ->
  16  		if
  17  		:: (o <  MAX) -> o = o+1
  18  		:: (o >= MAX) -> o = FILL
  19  		fi;
  20  		chout!ack(o)
.P3
  21  	:: chin?err(i) ->
  22  		chout!nak(o)
  23  	od
  24  }
  25  
.P3
  26  proctype channel(chan in, out)
  27  {	byte md, mt;
  28  	do
  29  	:: in?mt,md ->
  30  		if
  31  		:: out!mt,md
  32  		:: out!err,0
  33  		fi
  34  	od
  35  }
  36  
.P3
  37  init
  38  {	chan AtoB[1] of { byte, byte };
  39  	chan BtoC[1] of { byte, byte };
  40  	chan CtoA[1] of { byte, byte };
  41  	atomic {
  42  		run transfer(AtoB, BtoC);
  43  		run channel(BtoC, CtoA);
  44  		run transfer(CtoA, AtoB)
  45  	};
  46  	AtoB!err,0;	/* start */
  47  	0		/* hang */
  48  }
.P2
The protocol uses three message types: \f2ack\f1, \f2nak\f1, and
a special type \f2err\f1 that is used to model message distortions
on the communication channel between the two transfer processes.
The behavior of the channel is modeled explicitly with a channel
process.
There is also an
.CW assert
statement that claims a (faulty) invariant
relation between two local variables in the transfer processes.
.PP
Running \*s without options gives us a random simulation that
will only provide output when execution terminates, or if
a \f2printf\f1 statement is encountered.
In this case:
.P1 0
$ spin lynch
spin: "lynch" line 13: assertion violated
#processes: 4
proc  3 (transfer)	line 11 (state 15)
proc  2 (channel)	line 28 (state 6)
proc  1 (transfer)	line 13 (state 3)
proc  0 (:init:)	line 48 (state 6)
4 processes created
$ 
.P2
There are no \f2printf\f1's in the specification, but execution
halts on an assertion violation.
Curious to find out more, we can repeat the run with more verbose
output, e.g. printing all receive events.
The result of that run is shown in Figure 1.
Most output will be self-explanatory.
.PP
The above simulation run ends in the same assertion violation.
Since the simulation resolves nondeterministic choices in a
random manner, this need not always be the case.
To force a reproducible run, the option
.CW -n\fIN
can be used.
For instance:
.P1 0
$ spin -r -n100 lynch
.P2
will seed the random number generator with the integer value 100
and is guaranteed to produce the same output each time it is executed.
.PP
The other options can add still more output to the simulation run,
but the amount of text can quickly become overwhelming.
An easy solution is to filter the output through \f2grep\f1.
For instance, if we are only interested in the behavior of the
channel process in the above example, we say:
.P1 0
$ spin -n100 -r lynch | grep "proc  2"
.P2
The results are shown in Figure 1.
.1C
.KF
.nf
.ps -2
.vs -3p
.ft CW
.TS
box expand;
l
l.
      $ spin -r lynch
      proc  1 (transfer) line  21, Recv err,0  <- queue 1 (chin)
      proc  2 (channel)  line  29, Recv nak,10 <- queue 2 (in)
      proc  3 (transfer) line  12, Recv nak,10 <- queue 3 (chin)
      proc  1 (transfer) line  15, Recv ack,10 <- queue 1 (chin)
      \&...
      proc  1 (transfer) line  15, Recv ack,12 <- queue 1 (chin)
      proc  2 (channel)  line  29, Recv ack,99 <- queue 2 (in)
      proc  3 (transfer) line  15, Recv ack,99 <- queue 3 (chin)
      proc  1 (transfer) line  15, Recv ack,99 <- queue 1 (chin)
      proc  2 (channel)  line  29, Recv ack,99 <- queue 2 (in)
      proc  3 (transfer) line  21, Recv err,0  <- queue 3 (chin)
      proc  1 (transfer) line  12, Recv nak,99 <- queue 1 (chin)
      spin: "lynch" line 13: assertion violated
      #processes: 4
      proc  3 (transfer) line 11 (state 15)
      proc  2 (channel)  line 28 (state 6)
      proc  1 (transfer) line 13 (state 3)
      proc  0 (:init:)   line 48 (state 6)
      4 processes created
      $ spin -n100 -r lynch | grep "proc  2"
      proc  2 (channel) line 29, Recv nak,10 <- queue 2 (in)
      proc  2 (channel) line 29, Recv ack,11 <- queue 2 (in)
      proc  2 (channel) line 29, Recv ack,12 <- queue 2 (in)
      proc  2 (channel) line 28 (state 6)
.TE
.fi
.ps +2
.vs +3p
.SP .5
.ce
\fBFigure 1.\fR  Simulation Run Output
.SP .5
.KE
.2C
......
.IH "The Analyzer"
.PP
The simulation runs can be useful in quick debugging of
new designs, but by simulation alone we can not prove
that the system is really error free.
A validation of even very large models can be performed with the
.CW -a
and
.CW -t
options of \*s.
.PP
An exhaustive state space searching program for a protocol
model is generated as follows, producing five files, named \f2pan.[bchmt]\f1.
.P1 0
$ spin -a lynch
.P3
$ wc pan.[bchmt]
.P3
     92     326    2041 pan.b
.P3
    854    2502   17524 pan.c
.P3
    147     576    3475 pan.h
.P3
    307    1230    7493 pan.m
.P3
    177     548    3997 pan.t
.P3
   1577    5182   34530 total
.P2
The details are none too interesting: \f2pan.c\f1 contains
most of the C code for the analysis of the protocol.
File \f2pan.t\f1 contains a transition matrix that encodes
the protocol control flow; \f2pan.b\f1 and \f2pan.m\f1 contain
C code for forward and backward transitions and
\f2pan.h\f1 is a general header file.
The program can be compiled in two different ways: with a full
state space or with a bit state space.
.IH "Exhaustive Search"
.PP
The best method, that works up to system state spaces of
roughly 100,000 states, is to use the
default compilation of the program:
.P1 0
$ cc -o run pan.c
.P2
The executable program \f2run\f1 can now be executed to perform
the validation.
The validation is truly exhaustive: it tests all possible
event sequences in all possible orders.
It should, of course, find the same assertion violation.
.P1 0
$ run
assertion violated (i == last_i + 1))
pan: aborted
pan: wrote pan.trail
search interrupted
vector 64 byte, depth reached 56
      61 states, stored
       5 states, linked
       1 states, matched
hash conflicts: 0 (resolved)
(size 2^18 states, stack frames: 0/5)
.P2
The first line of the output announces the assertion violation
and attempts to give a first indication of the invariant that
was violated.
The violation was found after 61 states had been generated.
Hash "conflicts" gives the number
of hash collisions that happened during access to the state space.
As indicated,
all collisions are resolved in full search mode, since all states are
placed in a linked list.
The most relevant piece of output in this case, however, is on the
third line which tells us that a trail file was created that can
be used in combination with the simulator to recreate the error
sequence.
We can now say, for instance
.P1 0
$ spin -t -r lynch | grep "proc  2"
.P2
to determine the cause of the error.
Note carefully that the validator is guaranteed to find the
assertion violation if it is feasible.
If an exhaustive search does not report such a violation, it is
certain that \f2no\f1 execution execution sequence exists that can
violate the assertion.
.IH Options
.PP
The executable analyzer that is generated comes with a modest
number of options that can be checked as follows
.P1 0
$ run -?
-cN stop at Nth error (default=1)
-l  find non-progress loops
-mN max depth N (default=10k)
-wN hash table of 2^N entries (default=18)
.P2
Using a zero as an argument to the first option
forces the state space search to continue,
even if errors are found.
An overview of unexecutable (unreachable) code is given with every
complete run: either the default run if it did not find any
errors, or the run with option
.CW -c0 .
In this case the output is:
.P1 0
$ run -c0
assertion violated (i == (last_i + 1))
assertion violated (i == (last_i + 1))
assertion violated (i == (last_i + 1))
assertion violated (i == (last_i + 1))
assertion violated (i == (last_i + 1))
.P3
vector 64 byte, depth reached 60, errors: 5
     165 states, stored
       5 states, linked
      26 states, matched
hash conflicts: 1 (resolved)
(size 2^18 states, stack frames: 0/6)

unreached code :init: (proc 0):
	reached all 9 states
unreached code channel (proc 1):
	 line 35 (state 9),
	reached: 8 of 9 states
unreached code transfer (proc 2):
	 line 24 (state 18),
	reached: 17 of 18 states
.P2
There were five assertion violations, and some 165 unique
system states were generated.
Each state description (the \f2vector size\f1) took up 64 bytes
of memory; the longest non-cyclic execution sequence was 60.
There is one unreachable state both in the channel process and in
the transfer process.
In both cases the unreachable state is the control flow point
just after the do-loop in each process.
Note that both loops are indeed meant to be non-terminating.
.PP
The \f(CW-l\f1 option will cause the analyzer to search for
non-progress loops rather than deadlocks or assertion violations.
The option is explained in the section on ``More Advanced Usage.''
.PP
The executable analyzer has two other options.
By default the search depth is restricted to a rather
arbitrary 10,000 steps.
If the depth limit is reached, the search is truncated, making
the validation less than exhaustive.
To make certain that the search is exhaustive, make sure that the
"depth reached" notice is within the maximum search depth, and
if not, repeat the analysis with an explicit
.CW -m
argument.
.PP
The
.CW -m
option can of course also be used to truncate
the search explicitly, in an effort to find the shortest possible
execution sequence that violates a given assertion.
Such a truncated search, however, is not guaranteed to find every
possible violation, even within the search depth.
.PP
The last option
.CW -w\fIN
can only affect the run time, not
the scope, of an analysis with a full state space.
This "hash table width" should normally be set equal to,
or preferably higher than,
the logarithm of the expected number of unique system states generated
by the analyzer.
(If it is set too low, the number of hash collisions will increase
and slow down the search.)
The default
.I N
of 18 handles up to 262,144 system states, which should
suffice for almost all applications of a full state space analysis.
.IH "Bit State Space Analysis"
.PP
It can easily be calculated what the memory requirements of an analysis
with a full state space are|reference(holzmann atttj).
If, as in the example we have used, the protocol requires 64 bytes
of memory to encode one system state, and we have a total of 2MB
of memory available for the search, we can store up to 32,768 states.
The analysis fails if there are more reachable states in the
system state space.
So far, \*s is the \f2only\f1 validation system that can avoid this trap.
All other existing automated validation system (irrespective on
which formalism they are based) simply run out of memory and
abort their analysis without returning a useful answer to the user.
.PP
The coverage of a conventional analysis goes down rapidly when
the memory limit is hit, i.e. if there are
twice as many states in the full state space than we can store,
the effective coverage of the search is only 50% and so on.
\*S does substantially better in those cases by using the bit state
space storage method|reference(holzmann atttj).
The bit state space can be included by compiling the analyzer as follows:
.P1 0
$ cc -DBITSTATE -o run pan.c
.P2
The analyzer compiled in this way
should of course find the same assertion violation again:
.P1 0
$ run
assertion violated (i == ((last_i + 1))
pan: aborted
pan: wrote pan.trail
search interrupted
vector 64 byte, depth reached 56
      61 states, stored
       5 states, linked
       1 states, matched
hash factor: 67650.064516
(size 2^22 states, stack frames: 0/5)
$ 
.P2
In fact, for small to medium size problems there is very little
difference between the full state space method and the bit state
space method (with the exception that the latter is somewhat
faster and uses substantially less memory).
The big difference comes for larger problems.
The last two lines in the output are useful in estimating
the \f2coverage\f1 of a large run.
The maximum number of states that the bit state space can
accommodate is written on the last line (here @2 sup 22@ bytes or about 32 million
bits = states).
The line above it gives the \f2hash factor\f1: roughly
equal to the maximum number of states divided by the actual
number of states.
A large hash factor (larger than 100) means, with high reliability,
a coverage of 99% or 100%.
As the hash factor approaches 1 the coverage approaches 0%.
.PP
Note carefully that the analyzer realizes a partial coverage \f2only\f1
in cases where traditional validators are either unable to perform a
search, or realize a far smaller coverage.
In \f2no\f1 case will \*s produce an answer that is less reliable than
that produced by other automated validation systems (quite on the contrary).
.PP
The object of a bit state validation is to achieve a hash factor
larger than 100 by allocating the maximum amount of memory
for the bit state space.
For the best result obtainable: use the
.CW -w\fIN
option to size the state space to precisely the amount
of real (not virtual) memory available on your machine.
By default,
.I N
is 22, corresponding to a state space of 4MB.
For example, if your machine has 128MB of real memory, you can use
.CW -w27
to analyze systems with up to a billion reachable states.
.SP 2
.NH
\*P Reference Manual
.PP
This section describes the language \*P proper.
As much as possible, the presentation follows the example
from the
.CW C
reference manuals|reference(cbook).
It does not cover possible restrictions or extensions of
specific implementations.
The current implementation of \*s, for instance,
has an extra keyword
.CW printf ,
to access the corresponding
.UX
library function.
.IH "Lexical Conventions"
.PP
There are five classes of tokens: identifiers, keywords, constants,
operators and statement separators.
Blanks, tabs, newlines, and comments serve only to separate tokens.
If more than one interpretation is possible, a token is
taken to be the longest string of characters that can
constitute a token.
.ix lexical conventions
.ix tokens
.IH Comments
.PP
Any string started with
.CW /*
and terminated with
.CW */
is a comment.
Comments may not be nested.
.ix comments
.IH Identifiers
.PP
An identifier is a single letter, period, or underscore
followed by zero or more letters, digits, periods, or underscores.
.ix identifiers
.IH Keywords
.PP
The following identifiers are reserved for use as keywords:
.ix keywords
.KS
.ft CW
.ps -1
.vs -1
.TS
center;
l l l.
assert	atomic	bit
bool	break	byte
chan	do	fi
goto	if	init
int	len	mtype
never	printf	od
of	proctype	run
short	skip	timeout
.ps +1
.vs +1
.TE
.KE
.IH Constants
.PP
A constant is a sequence of digits representing a decimal integer.
There are no floating point numbers in \*P.
.ix constants
Symbolic names for constants can be defined in two ways.
The first method is to use a C-style macro definition
.P1 0
#define	NAME	value
.P2
The second method is to use the keyword
.CW mtype
(see ``declarations'' below).
.IH Expressions
.PP
The following operators can be used to build expressions.
.ix expressions
.ix operators
.KS
.TS
center;
lFCW.
+ \- * \/ %
> >= < <= == != !
&& ||
& | ~ >> <<
.TE
.KE
.PP
Most operators are binary.
The logical negation \f2!\f1 and the minus \f2\-\f1
operator can be both unary and binary, depending on context.
Expressions are used, for instance, in assignments of the type
.CW "a = expression" ,
with
.CW a
a variable.
.ix assignment
There is also one unary operator that applies to message channels:
.P1 0
len
.P2
It measures the number of messages an existing channel holds.
There is one unary operator that is used for process instantiations:
.P1 0
run
.P2
And, finally, there are two binary operators
.P1 0
! ?
.P2
which are used for sending and receiving messages (see below).
.IH Declarations
.PP
.ix declarations
Processes, channels, and variables must be declared before they can be used.
Variables and channels can be declared either locally,
within a process, or globally.
A process can only be declared globally in a
.CW proctype
declaration.
Local declarations may appear anywhere in a process body.
.IH Variables
.PP
.ix variables
.ix local variables
.ix global variables
.ix initializers
A variable declaration is started by a keyword indicating the
basic data type of the variable,
.CW bit ,
.CW bool ,
.CW byte ,
.CW short ,
or
.CW int ,
followed
by one or more identifiers, optionally followed by
an initializer.
.P1 0
byte name1, name2 = 4, name3
.P2
By default all variables are initialized to zero.
An initializer, if specified, must be a constant.
The table below summarizes the width and attributes of each
basic data type.
.KS
.ps -1
.vs -2
.TS
center;
l l l
lFCW n r.
=
Name	Size (bits)	Usage
_
bit	1	unsigned
bool	1	unsigned
byte	8	unsigned
short	16	signed
int	32	signed
_
.TE
.ps +1
.vs +2
.KE
The names \f2bit\f1 and \f2bool\f1
are synonyms for a single bit of
information.
A \f2byte\f1 is an unsigned quantity that can store a value between
0 and 255.
\f2Short\f1s and \f2int\f1s are signed quantities that
differ only in the range of values they can hold.
.PP
An array of variables is declared as follows:
.P1 0
int name1[N]
.P2
where
.CW N
is a constant.
An array can have a just a single constant as an initializer.
If specified it is used to initialize all elements of the array.
.PP
Symbolic names for constants, e.g. message types,
can, optionally, be defined in a declaration
of the type
.P1 0
mtype = { namelist }
.P2
where
.CW namelist
is a comma separated list of symbolic names.
.IH "Message Channels"
.PP
A message channel can be declared, for instance, as follows:
.ix channels
.P1 0
chan name[N] of { short, short }
.P2
where
.CW N
is a constant that specifies the maximum number of messages
that can be stored in the channel.
A list of one or more data types (or the channel type
.CW chan )
enclosed in curly braces defines the type of the messages that can
be passed through the channel.
All channels are initialized to be empty.
.IH Processes
.PP
.ix process
A process declaration starts with the keyword
.CW proctype
followed by a name, a list of formal parameters
enclosed in round braces, and
a sequence of statements and local
variable declarations.
The body of process declaration is enclosed in curly braces.
.P1 0
proctype name( /* parameter decls */ )
{
	/* statements */
}
.P2
.IH Statements
.PP
.ix statements
.ix gotos
.ix labels
.ix skip
There are twelve types of statements:
.KS
.TS
center;
a a a.
.ft CW
.ps -1
.vs -1
assertion	assignment	atomic
break	declaration	expression
goto	receive	selection
repetition	send	timeout
.ft
.ps +1
.vs +1
.TE
.KE
Each statement may be preceded by a label: a name followed by a colon.
A statement can only be passed if it is executable.
To determine its executability the statement can be evaluated:
if evaluation returns a zero value the statement is blocked.
In all other cases the statement is executable and can be passed.
The act of passing the statement after a successful evaluation is
called the ``execution'' of the statement.
There is one so-called \fIpseudo\fR-statements
.CW skip ,
a null statement equivalent to \(CW(1)\f1; it is always executable.
It has no effect when executed, but may be needed
to satisfy syntax requirements.
The evaluation of an assertion statement
.CW assert(condition)
has no effect if the condition holds, but aborts the
running process if evaluation of the condition returns a
zero result (the boolean value ``false'').
.PP
.CW goto
statements can be used to transfer control to any labeled statement
within the same process or procedure.
They also are always executable.
Assignments have been discussed above, they are
always executable.
A declaration is also always executable.
Expressions are only executable if they return a non-zero value.
That is, the expression \(CW0\fR (zero) is never executable, and
similarly \(CW1\fR always is executable.
Below we consider the remaining statements: selection, repetition,
send, receive, break, timeout, and atomic statements.
.IH Selection
.PP
.ix if statement
.ix case selection
.ix nondeterminism
A selection statement is started with the keyword
.CW if ,
followed by
a list of one or more `options' and terminated with the keyword
.CW fi .
Every `option' is started with the flag \f(CW::\fR followed by any sequence
of statements.
One and only one option from a selection statement will
be selected for execution.
The first statement of an option determines
whether the option can be selected or not.
If more than one option is executable, one will be selected at random.
Note that this randomness makes the language a nondeterministic one.
.IH "Repetition and Break"
.PP
.ix do statement
.ix repetition
A repetition or
.CW do
statement is similar to a selection statement, but is executed
repeatedly until either a
.CW break
statement is executed or a
.CW goto
jump will transfer control outside the cycle.
The keywords of the repetition statement are
.CW do
and
.CW od
instead of the
.CW if
and
.CW fi
of selection.
The
.CW break
statement will terminate the innermost repetition
statement in which it is executed.
The use of a \(CWbreak\fR statement outside a
repetition statement is illegal.
......
.IH "Atomic Sequences"
.PP
The keyword
.CW atomic
introduces an atomic sequence of statements, that is
to be executed as one indivisible step.
The syntax is as follows
.P1 0
atomic { sequence }
.P2
Logically the sequence of statements is now equivalent
to one single statement.
It is a run-time error if any statement that is part of an
atomic sequence is found to be unexecutable.
The safest is therefore to include only assignments and
local conditions in atomic sequences, but no sends or receives.
Labeling local computations as atomic can bring an important
reduction of the complexity of a validation model.
For the lazy, \*s has an option (\f(CW-q\f1) that tries to find
the most obvious ``atomicable'' sequences in the code,
but the user can often do better by hand.
.IH Send
.PP
.ix i/o statements
.ix send
The syntax of a send statement is:
.P1 0
expr1!expr2
.P2
where
.CW expr1
returns the identity of a channel, e.g. obtained from a
.CW chan
operation, and
.CW expr2
returns a value to be appended to the channel.
The send statement is not executable (blocks) if the addressed channel is full
or does not exist.
.ix value transfer
If more than one value is to be passed from sender to receiver, the expressions
are written in a comma separated list:
.P1 0
expr1!expr2,expr3,expr4
.P2
Equivalently, this may be written
.P1 0
expr1!expr2(expr3,expr4) .
.P2
.IH Receive
.PP
.ix i/o statements
.ix receive
.ix value transfer
The syntax of the receive statement is:
.P1 0
expr1?name
.P2
where
.CW expr1
returns the name of a channel and
.CW name
is a variable or a constant.
If a constant is specified the receive statement is only executable
if the channel exists and
the oldest message stored in the channel contains the same value.
If a variable is specified, the receive statement is executable
if the channel exists and contains any message at all.
The variable in that case will receive the value of the message
that is retrieved.
If more than one value is sent per message, the receive statement
also take a comma separated list of variables and constants
.P1 0
expr1?name1,name2,...
.P2
which again is syntactically equivalent to
.P1 0
expr1?name1(name2,...)
.P2
Each constant in this list puts an extra condition on the
executability of the receive: it must be matched by the
value of the corresponding message field of the
message to be retrieved.
The variable fields retrieve the values of the corresponding
message fields on a receive.
.PP
Placing square brackets around the clause after the `?'
in the receiver operation converts it into a condition,
that is true only if the corresponding receive operation
is executable.
It can be used freely in any type of composite boolean condition,
and it has no side-effects when evaluated.
.PP
A last type of operation allowed on channels is
.P1 0
len(expr)
.P2
where
.CW expr
returns the identity of an instantiated channel.
The operation returns the number of messages in
the channel specified, or zero if the channel does not exist.
.IH Timeout
.PP
The timeout condition is a modeling feature that by definition becomes true
only if no statement in any of the running processes is executable.
It has no effect when executed.
.IH "Macros and Include Files"
.PP
.ix macros
.ix include files
.ix preprocessor
The source text of a specification is processed by the C|reference(cbook)
preprocessor for macro-expansion and file inclusions.
.NH
Summary
.PP
In the first part of this memo
we have introduced a notation for modeling concurrent
systems, including but not limited to asynchronous
data communication protocols, in a language named \*P.
The language has several unusual features.
All communication between processes takes
place via either messages or shared variables.
Both synchronous and asynchronous communication
are modeled as two special cases of a general message
passing mechanism.
Every statement in \*P can potentially model delay: it is
either executable or not, in most cases depending on the state
of the environment of the running process.
Process interaction and process coordination is thus at
the very basis of the language.
More about the design of \*P, of the validator \*s, and
its application to protocol design, can be found in |reference(holzmann spinbook).
.PP
\*P is deliberately a validation modeling language, not a programming language.
There are, for instance, no elaborate abstract data types,
or more than a few basic types of variable.
A validation model is an abstraction of a protocol implementation.
The abstraction maintains the essentials of the process interactions,
so that it can be studied in isolation.
It suppresses implementation and programming detail.
.PP
The syntax of \*P expressions, declarations, and assignments
is loosely based on the language
.CW C |reference(cbook).
The language was influenced significantly by the ``guarded command languages''
of E.W. Dijkstra |reference(dijkstra guarded) and C.A.R. Hoare
|reference(hoare csp).
There are, however, important differences.
Dijkstra's language had no primitives for process interaction.
Hoare's language was based exclusively on synchronous
communication.
Also in Hoare's language, the type
of statements that could appear in the guards of an option was
restricted.
The semantics of the selection and cycling statements
in \*P is also rather different from other guarded
command languages: the statements are not aborted when all guards
are false but they block: thus providing the required synchronization.
.PP
With minimal effort \*s allows the user to generate sophisticated
analyzers from \*P validation models.
Both the \*s software itself, and the analyzers it can generate,
are written in ANSII C and are portable across
.UX
systems.
They can be scaled to fully exploit the physical limitations
of the host computer, and deliver within those
limits the best possible analyses that can be realized
with the current state of the art in protocol analysis.
.NH
References
.LP
|reference_placement
.af H1 A
.nr H1 1
.nr H2 0
.SH
Appendix: Building A Validation Suite
.PP
The first order of business in using \*s for
a validation is the construction of a
faithful model in \*P of the problem at hand.
The language is deliberately kept small.
The purpose of the modeling is to extract those
aspects of the system that are relevant to the
coordination problem being studied.
All other details are suppressed.
Formally: the model is a reduction of the
system that needs to be equivalent to the full system
only with respect to the properties that are being validated.
Once a model has been constructed, it becomes
the basis for the construction of a series of,
what we may call, ``validation suites'' that
are used to verify its properties.
To build a validation suite we can prime the
model with assertions.
The assertions can formalize invariant relations
about the values of variables or about allowable
sequences of events in the model.
.NH 2
An Example
.PP
As a first example we take the following solution
to the mutual exclusion problem, discussed earlier,
published in 1966 by H. Hyman in the Communications of the ACM.
It was listed, in pseudo Algol, as follows.
.P1 0
  1 \f3Boolean array\f2 b(0;1) \f3integer\f2 k, i,\f(CW
  2 \f3comment\f2 process i, with i either 0 or 1;\f(CW
  3 \f2C0:	b(i) := \f3false\f2;\f(CW
  4 \f2C1:	\f3if\f2 k != i \f3then begin\f2\f(CW
  5 \f2C2:	\f3if\f2 not (b(1-i) \f3then go to\f2 C2;\f(CW
  6 	\f3else\f2 k := i; \f3go to\f2 C1 \f3end\f2;\f(CW
  7 	\f3else\f2 critical section;\f(CW
  8 	\f2b(i) := \f3true\f2;\f(CW
  9 	\f2remainder of program;\f(CW
 10 	\f3go to\f2 C0;\f(CW
 11 	\f3end\f(CW
.P2
The solution, as Dekker's earlier solution, is for two processes,
numbered 0 and 1.
Suppose we wanted to prove that Hyman's solution truly
guaranteed mutually exclusive access to the critical section.
Our first task is to build a model of the solution in \*P.
While we're at it, we can pick some more useful names for
the variables that are used.
.P1 0
   1  bool want[2];	/* Bool array b */
   2  bool turn;	/* integer    k */
   3  
.P3
   4  proctype P(bool i)
   5  {
   6  	want[i] = 1;
.P3
   7  	do
   8  	:: (turn != i) ->
   9  		(!want[1-i]);
  10  		turn = i
.P3
  11  	:: (turn == i) ->
  12  		break
  13  	od;
.P3
  14  	skip; /* critical section */
  15  	want[i] = 0
  16  }
.P3
  17  
.P3
  18  init { run P(0); run P(1) }
.P2
We can generate, compile, and run a validator for this
model, to see if there are any major problems, such as
a global system deadlock.
.P1 0
$ spin -a hyman0
$ cc pan.c
$ a.out
full statespace search for:
assertion violations and invalid endstates
vector 20 byte, depth reached 19, errors: 0
      79 states, stored
       0 states, linked
      38 states, matched	total: 117
hash conflicts: 4 (resolved)
(size 2^18 states, stack frames: 3/0)

unreached code _init (proc 0):
	reached all 3 states
unreached code P (proc 1):
	reached all 12 states
.P2
The model passes this first test.
What we are really interested in, however, is if
the algorithm guarantees mutual exclusion.
There are several ways to proceed.
The simplest is to just add enough information
to the model that we can express the correctness
requirement in a \*P assertion.
.P1 0
   1  bool want[2];
   2  bool turn;
   3  byte cnt;
   4  
.P3
   5  proctype P(bool i)
   6  {
.P3
   7  	want[i] = 1;
.P3
   8  	do
   9  	:: (turn != i) ->
  10  		(!want[1-i]);
  11  		turn = i
.P3
  12  	:: (turn == i) ->
  13  		break
  14  	od;
  15  	skip; /* critical section */
.P3
  16  	cnt = cnt+1;
  17  	assert(cnt == 1);
  18  	cnt = cnt-1;
  19  	want[i] = 0
  20  }
.P3
  21  
.P3
  22  init { run P(0); run P(1) }
.P2
We have added a global variable
.CW cnt
that is incremented upon each access to the
critical section, and decremented upon each exit
from it.
The maximum value that this variable should ever
have is 1, and it can only have this value when
a process is inside the critical section.
.P1 0
$ spin -a hyman1
$ cc pan.c
$ a.out
assertion violated (cnt==1)
pan: aborted (at depth 15)
pan: wrote pan.trail
full statespace search for:
assertion violations and invalid endstates
search was not completed
vector 20 byte, depth reached 25, errors: 1
     123 states, stored
       0 states, linked
      55 states, matched	total: 178
hash conflicts: 42 (resolved)
(size 2^18 states, stack frames: 3/0)
.P2
The validator claims that the assertion can be violated.
We can use the error trail to check it with \*s's \f(CW-t\f1 option:
.P1 0
$ spin -t -p hyman1
proc  0 (_init)	line 24 (state 2)
proc  0 (_init)	line 24 (state 3)
.P3
proc  2 (P)	line 8 (state 7)
proc  2 (P)	line 9 (state 2)
.P3
proc  2 (P)	line 10 (state 3)
proc  2 (P)	line 11 (state 4)
.P3
proc  1 (P)	line 8 (state 7)
proc  1 (P)	line 12 (state 5)
.P3
proc  1 (P)	line 15 (state 10)
proc  2 (P)	line 8 (state 7)
.P3
proc  2 (P)	line 12 (state 5)
proc  2 (P)	line 15 (state 10)
.P3
proc  2 (P)	line 16 (state 11)
proc  2 (P)	line 17 (state 12)
.P3
proc  2 (P)	line 18 (state 13)
proc  1 (P)	line 16 (state 11)
.P3
proc  1 (P)	line 17 (state 12)
spin: "hyman1" line 17: assertion violated
.P3
step 17, #processes: 3
		want[0] = 1
		_p[0] = 12
		turn[0] = 1
		cnt[0] = 2
.P3
proc  2 (P)	line 18 (state 13)
proc  1 (P)	line 17 (state 12)
proc  0 (_init)	line 24 (state 3)
3 processes created
.P2
Here is another way to catch the error.
We again lace the model with the information that
will allow us to count the number of processes
in the critical section.
.P1 0
   1  bool want[2];
   2  bool turn;
   3  byte cnt;
   4  
.P3
   5  proctype P(bool i)
   6  {
   7  	want[i] = 1;
.P3
   8  	do
   9  	:: (turn != i) ->
  10  		(!want[1-i]);
  11  		turn = i
.P3
  12  	:: (turn == i) ->
  13  		break
  14  	od;
.P3
  15  	cnt = cnt+1;
  16  	skip;	/* critical section */
  17  	cnt = cnt-1;
  18  	want[i] = 0
  19  }
.P3
  20  
.P3
  21  proctype monitor()
  22  {
  23  	assert(cnt == 0 || cnt == 1)
  24  }
.P3
  25  
.P3
  26  init {
  27  	run P(0); run P(1); run monitor()
  28  }
.P2
The invariant condition on the value of counter
.CW cnt
is now place in a separate process
.CW monitor()
(the name is immaterial).
The extra process runs along with the two others.
It will always terminate in one step, but it
could execute that step at \f2any\f1 time.
The systems modeled by \*P and validated by \*s
are completely asynchronous.
That means that the validation of \*s take into
account \f2all\f1 possible relative timings of
the three processes.
In a full validation, the assertion therefore
can be evaluated at any time during the lifetime
of the other two processes.
If the validator reports that it is not violated
we can indeed conclude that there is no execution
sequence at all (no way to select relative speeds for
the three processes) in which the assertion can be
violated.
The setup with the monitor process is therefore an
elegant way to check the validity of a system invariant.
The validation produces:
.P1 0
$ spin -a hyman2
$ cc pan.c
$ a.out
assertion violated ((cnt==0)||(cnt==1))
pan: aborted (at depth 15)
pan: wrote pan.trail
full statespace search for:
assertion violations and invalid endstates
search was not completed
vector 24 byte, depth reached 26, errors: 1
     368 states, stored
       0 states, linked
     379 states, matched	total: 747
hash conflicts: 180 (resolved)
(size 2^18 states, stack frames: 4/0)
.P2
Because of the extra interleaving of the two processes
with a third monitor, the number of system states that
had to be searched has increased, but the error is again
correctly reported.
.br
.NE 8v
.NH 2
Another Example
.PP
Not always can a correctness requirement be cast in
terms of a global system invariant.
Here is an example that illustrates this.
It is a simple alternating bit protocol, modeling
the possibility of message loss, and distortion,
and extended with negative acknowledgements.
.P1 0
   1  #define MAX	5
   2  
   3  mtype = { mesg, ack, nak, err };
   4  
.P3
   5  proctype sender(chan in, out)
   6  {	byte o, s, r;
   7  
   8  	o=MAX-1;
   9  	do
  10  	:: o = (o+1)%MAX; /* next msg */
  11  again: if
  12         :: out!mesg(o,s) /* send */
  13         :: out!err(0,0)  /* distort */
  14         :: skip       /* or lose */
  15         fi;
.P3
  16         if
  17         :: timeout   -> goto again
  18         :: in?err(0,0) -> goto again
  19         :: in?nak(r,0) -> goto again
  20         :: in?ack(r,0) ->
  21  		if
  22  		:: (r == s) -> goto progress
  23  		:: (r != s) -> goto again
  24  		fi
  25         fi;
  26  progress:	s = 1-s	/* toggle seqno */
  27  	od
  28  }
  29  
.P3
  30  proctype receiver(chan in, out)
  31  {	byte i;		/* actual input   */
  32  	byte s;		/* actual seqno   */
  33  	byte es;	/* expected seqno */
  34  	byte ei;	/* expected input */
  35  
  36  	do
  37  	:: in?mesg(i, s) ->
  38  		if
  39  		:: (s == es) ->
  40  			assert(i == ei);
  41  progress:		es = 1 - es;
  42  			ei = (ei + 1)%MAX;
  43  			if
  44  	/* send,   */	:: out!ack(s,0)
  45  	/* distort */	:: out!err(0,0)
  46  	/* or lose */	:: skip
  47  			fi
.P3
  48  		:: (s != es) ->
.P3
  49  			if
.P3
  50  	/* send,   */	:: out!nak(s,0)
  51  	/* distort */	:: out!err
  52  	/* or lose */	:: skip
.P3
  53  			fi
.P3
  54  		fi
  55  	:: in?err(0,0) ->
  56  		out!nak(s,0)
.P3
  57  	od
  58  }
  59  
.P3
  60  init {
.P3
  61  	chan s_r [1] of { byte,byte,byte };
  62  	chan r_s [1] of { byte,byte,byte };
.P3
  63  	atomic {
.P3
  64  		run sender(r_s, s_r);
  65  		run receiver(s_r, r_s)
.P3
  66  	}
.P3
  67  }
.P2
To test the proposition that this protocol will
correctly transfer data, the model has already
been primed for the first validation runs.
First, the sender is setup to transfer an infinite
series of integers as messages, where the value
of the integers are incremented modulo
.CW MAX .
The value of
.CW MAX
is not really too interesting, as long as it is
larger than the range of the sequence numbers in
the protocol: in this case 2.
We want to verify that data that is sent can only be
delivered to the receiver without any deletions or reorderings,
despite the possibility of arbitrary message loss.
The assertion on line 40 verifies precisely that.
Note that if it were ever possible for the protocol to
fail to meet the above requirement, the assertion can be violated.
.PP
A first validation run reassures us that this is not possible.
.P1 0
$ spin -a ABP0
$ cc pan.c
$ a.out
full statespace search for:
assertion violations and invalid endstates
vector 40 byte, depth reached 131, errors: 0
     346 states, stored
       1 states, linked
     125 states, matched	 total: 472
hash conflicts: 17 (resolved)
(size 2^18 states, stack frames: 0/25)

unreached code _init (proc 0):
	reached all 4 states
unreached code receiver (proc 1):
	line 58 (state 24)
	reached: 23 of 24 states
unreached code sender (proc 2):
	line 28 (state 27)
	reached: 26 of 27 states
.P2
But, be careful.
The result means that all data that is delivered, is
delivered in the correct order without deletions etc.
We did not check that the data \f2will\f1 necessarily be delivered.
It may be possible for sender and receiver to cycle
through a series of states, exchanges erroneous messages,
without ever making effective progress.
To check this, the state in the sender and in the receiver
process that unmistakingly signify progress, were labeled
as a ``progress states.''
(In fact, either one by itself would suffice.)
.PP
We should now be able to demonstrate the absence of
infinite execution cycles that do not pass through any
of these progress states.
We can use the same executable from the last run, but
this time we perform a loop-check.
.P1 0 
$  a.out -l
pan: non-progress cycle (at depth 6)
pan: wrote pan.trail
full statespace search for:
assertion violations and non-progress loops
search was not completed
vector 44 byte, depth reached 8, loops: 1
      12 states, stored
       1 states, linked
       0 states, matched	total: 13
hash conflicts: 0 (resolved)
(size 2^18 states, stack frames: 0/1)
.P2
There are non-progress cycles.
The first one encountered is dumped into the error trail
by the validator, and we can inspect it.
The results are shown in the first half of Figure 2.
The channel can distort or lose the message infinitely often;
true, but not too exciting as an error scenario.
To see how many non-progress cycles there are, we can use the \f(CW-c\f1 flag.
If we set its numeric argument to zero, only
a total count of all errors will be printed.
.P1 0
$ a.out -l -c0
full statespace search for:
assertion violations and non-progress loops
vector 44 byte, depth reached 137, loops: 92
     671 states, stored
       2 states, linked
     521 states, matched	 total: 1194
hash conflicts: 39 (resolved)
(size 2^18 states, stack frames: 0/26)
.P2
There are 92 cases to consider, and we could look at each
one, using the \f(CW-c\f1 option (\f(CW-c1\f1, \f(CW-c2\f1, \f(CW-c3\f1, ...etc.)
But, we can make the job a little easier by at least
filtering out the errors caused by infinite message loss.
We label all loss events (lines 13, 43, and 48) as
progress states, using label names with the common 8-character
prefix ``progress,'' and look at the cycles that remain.
(Labels go behind the ``::'' flags.)
.P1 0
$ spin -a ABP1
$ cc pan.c
.P3
$ a.out -l
pan: non-progress cycle (at depth 133)
pan: wrote pan.trail
.P3
full statespace search for:
assertion violations and non-progress loops
search was not completed
.P3
vector 44 byte, depth reached 136, loops: 1
.P3
     148 states, stored
       2 states, linked
       2 states, matched	 total: 152
.P3
hash conflicts: 0 (resolved)
(size 2^18 states, stack frames: 0/26)
.P2
This time, the trace reveals an honest and a serious bug in the protocol.
The second half of Figure 2 shows the trace-back.
.1C
.KF
.nf
.ps -2
.vs -3p
.ft CW
.TS
box expand;
l
l.
      $ spin -t -r -s ABP0
      <<<<<START OF CYCLE>>>>>
      proc  1 (sender)   line  13, Send err,0,0 -> queue 2 (out)
      proc  2 (receiver) line  55, Recv err,0,0 <- queue 2 (in)
      proc  2 (receiver) line  56, Send nak,0,0 -> queue 1 (out)
      proc  1 (sender)   line  19, Recv nak,0,0 <- queue 1 (in)
      spin: trail ends after 12 steps
      step 12, #processes: 3
		_p[0] = 6
      proc  2 (receiver)	line 36 (state 21)
      proc  1 (sender)	line 11 (state 6)
      proc  0 (_init)	line 67 (state 4)
      3 processes created
      $ 
      $ spin -t -r -s ABP1
      \&...
      proc  2 (receiver) line  39, Recv mesg,0,0 <- queue 2 (in)
      proc  2 (receiver) line  47, Send err,0,0  -> queue 1 (out)
      proc  1 (sender)   line  20, Recv err,1,0  <- queue 1 (in)
      proc  1 (sender)   line  12, Send mesg,0,0 -> queue 2 (out)
      proc  2 (receiver) line  39, Recv mesg,0,0 <- queue 2 (in)
      proc  2 (receiver) line  52, Send nak,0,0  -> queue 1 (out)
      proc  1 (sender)   line  21, Recv nak,0,0  <- queue 1 (in)
      proc  1 (sender)   line  12, Send mesg,0,0 -> queue 2 (out)
      proc  2 (receiver) line  39, Recv mesg,0,0 <- queue 2 (in)
      proc  2 (receiver) line  52, Send nak,0,0  -> queue 1 (out)
      <<<<<START OF CYCLE>>>>>
      proc  1 (sender)   line  21, Recv nak,0,0  <- queue 1 (in)
      proc  1 (sender)   line  12, Send mesg,0,0 -> queue 2 (out)
      proc  2 (receiver) line  39, Recv mesg,0,0 <- queue 2 (in)
      proc  2 (receiver) line  52, Send nak,0,0  -> queue 1 (out)
      spin: trail ends after 226 steps
      \&...
.TE
.fi
.ps +2
.vs +3p
.SP .5
.ce
\fBFigure 2.\fR  Error Trails - Extended Alternating Bit Protocol
.SP .5
.KE
.2C
.PP
After a single positive acknowledgement is distorted
and transformed into an
.CW err
message, sender and receiver get caught in an infinite
cycle, where the sender will stubbornly repeat the last
message for which it did not receive an acknowledgement,
and the receiver, just as stubbornly, will reject that
message with a negative acknowledgment.
.NH 2
Digging Deeper
.PP
This manual can only give an outline of the
main features of \*s, and the more common
ways in which it can be used for validations.
There is a small number of \*s features that
have not been discussed here, but that may be useful
for tackling non-standard validation problems.
\*S, for instance, can give \*P processes access
to extra system information, such as the current
values of normally invisible local variables,
or the current execution states of remote processes.
With this extra information it may be easier in
some cases to build accurate assertions about
required system behavior.
.PP
\*S also allows for a straightforward validation of ``tasks.''
That is, if the user formalizes a task that is claimed to be
performed by the system, \*s can quickly either prove or
disprove that claim.
The tasks can be used directly to verify any
propositional temporal logic formula
on the behavior of a system.
.PP
\*S also allows the user to formalize ``reductions'' of
the system state space, which can be used
to restrict a search it to a user defined subset.
With this method it becomes trivial to verify quickly
whether or not a given error pattern is within the range of
behaviors of a system, even when a complete validation is
considered to be infeasible.
.PP
For details about these alternative uses of \*P and
the \*s software, refer to [5].

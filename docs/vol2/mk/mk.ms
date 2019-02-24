.so ../ADM/mac
.XX mk 269 "Mk \(em A Successor to Make"
.nr dP 2
.nr dV 3p
.TL
Mk: a successor to make
.AU
Andrew G. Hume
.AI
.MH
.AB
.PP
.I Mk
is an efficient general tool
for describing and maintaining dependencies between
files or programs.
.I Mk
is styled on the
.UX
tool
.I make .
The major advantages of
.I mk
over
.I make
are executing recipes in parallel,
using pattern-matching metarules rather than suffix transformation rules,
and deriving dependencies by transitive closure on all rules.
Despite doing a harder job,
.I mk
runs anywhere from 2 to 30 times faster than
.I make .
.PP
This report describes release 3 (February 1990) of
.I mk
mainly by means of an evolving example.
It also summarizes the differences between
.I mk
and
.I make
and discusses the principles underlying
.I mk 's
design.
.AE
.de SS
.PP
\\f4\\$1\\f1
..
.PS
	# ln --		line to $1, $2 chop elliptically
	#		chopw and choph are the chop ellipse width and height
	define ln %
	 {	[
			x = $1*hu; y = $2*vu
			xsq = x*x
			rsq = xsq + y*y; r = sqrt(rsq);
			asq = chopw*chopw/4;
			bsq = choph*choph/4
			esq = (asq - bsq)/asq
			p = sqrt(bsq/(1-esq*(xsq/rsq)))/r;

		O:	""
		B:	O + p*x, p*y
		E:	O + (1-p)*x, (1-p)*y
		line $3 from B to E
		] with .O at Here
	}
	move to Here + $1*hu, $2*vu
	%

	hu = .5; vu = 1/3; chopw = .8; choph = 1/6

	#"if-then-else"
	# {
	#	ln(-1,-1, -> dashed); "a < b"
	#}{
	#	ln( 0,-1, ->); "y"
	#}{
	#	ln( 1,-1, ->); "z"
	#}
.PE
.2C
.NH 1
Introduction
.PP
A large fraction of computer activity consists of repeated application of
tools (special or general purpose programs) to input files to produce
output files.
The most obvious example is programming,
but other no less important examples
range from simple document-processing pipelines to the
generation of a circuit board or integrated circuit involving hundreds of files.
Common to all these activities are file dependencies,
where changing a file requires that other files be remade.
.I Mk
reads a dependency description
(called a
.I mkfile )
and does the minimal work necessary to
bring a target file up to date.
.PP
.I Mk
owes much to
.I make ,
written by Stu Feldman|reference(feldman make),
which has been doing a similar job on
.UX
systems since 1976.
The version of
.I make
referred to throughout this report is Feldman's research version
distributed with Eighth Edition Research
.UX ,
and is significantly different from the versions
found in System V or Berkeley
.UX
systems.
.PP
The next section is rather long.
It follows the gradual development of a somewhat complicated mkfile
describing how to build a C program.
It is followed by a section on more advanced features and uses of
.I mk .
The fourth section summarizes the differences between
.I mk
and
.I make
including performance.
The fifth section highlights the principles underlying
.I mk .
The first appendix documents the predefined or builtin variables and rules for
.I mk .
The second appendix describes the changes made since the first release.
.NH 1
An Extended Example
.PP
This section describes
.I mk
in the context of building C programs.
This is for the reader's comfort;
.I mk
knows nothing special about C programs
(other than the rules in Appendix 1).
The example starts off small and simple and is extended throughout the section.
Sometimes,
.I mk 's
behavior is best demonstrated by excerpts from a terminal session.
These will be shown as
.P1 0
$ date
Fri Feb 20 20:06:03 EST 1987
.P2
where
.CW $
is the prompt for the next command.
Comments will be shown in
.I italics .
.PP
Initially, our program is called
.CW prog
and is made from
.CW a.o
and
.CW b.o ,
which are made by compiling
.CW a.c
and
.CW b.c
respectively.
In addition,
.CW b.c
includes a header file
.CW prog.h .
We represent these relationships pictorially below
.PS
"prog"
{ ln(-2,-1,->); "a.o"; { ln(0,-1,->); "a.c";} }
{ ln(1,-1,->); "b.o"; { ln(-1,-1,->); "prog.h"; } { ln(1,-1,->); "b.c"; }}
.PE
The arrow means ``depends on.''
Thus,
.CW prog
depends on
.CW a.o
and
.CW b.o
and if
.CW a.o
or
.CW b.o
is modified, then
.CW prog
needs to be rebuilt.
Similarly,
.CW a.o
depends on
.CW a.c
and
.CW b.o
depends on
.CW b.c
and
.CW prog.h .
.PP
The textual description of how
.CW prog
is built is called a mkfile and looks like
.P1 0
prog:	a.o b.o
	cc -o prog a.o b.o
a.o:	a.c
	cc -c a.c
b.o:	b.c prog.h
	cc -c b.c
.P2
The mkfile is a sequence of
.I rules .
Each rule defines a target
(say
.CW prog )
that depends on some prerequisites
.CW \fR(\fPa.o
and
.CW b.o )
and the commands
(a shell script called the
.I recipe )
to bring the target up to date.
.I Mk
takes this description from a file named
.CW mkfile
and builds the given targets.
If no targets are given on the command line,
the first target in the mkfile is built.
For example, if we start with just the source files in our directory,
.I mk
creates
.CW prog
by compiling
.CW a.c
and
.CW b.c .
.P1 0
$ mk
cc -c a.c
cc -c b.c
cc -o prog a.o b.o
.P2
Executing
.I mk
again does nothing, as
.CW prog
is now up to date.
.P1 0
$ mk
mk: `prog' is up to date
.P2
If we change a source file,
.I mk
rebuilds only the files that are out of date:
.P1 0
\f2modify a.c\fP
$ mk
cc -c a.c
cc -o prog a.o b.o
.P2
.I Mk
will explain why it is rebuilding a file if we use the
.CW -e
option.
For example,
.P1 0
\f2modify prog.h\fP
$ mk -e
b.o(540869437) < prog.h(540869535)
cc -c b.c
prog(540869493) < b.o(540869546)
cc -o prog a.o b.o
.P2
Thus,
.CW b.o
was out of date with respect to
.CW prog.h .
After
.CW b.o
was remade,
.CW prog
was found to be out of date with respect to
.CW b.o
and was then rebuilt.
The numbers are the actual time stamps of the files:
the values are not as important as the difference between them.
A time stamp of zero indicates a non-existent file.
.NH 2
Variables
.PP
Suppose we now need to compile the source files with the
.CW -g
flag so that we can use the debugger.
We can of course simply edit each rule
to change
.CW cc
into
.CW "cc -g" :
.P1 0
prog:	a.o b.o
	cc -g -o prog a.o b.o
a.o:	a.c
	cc -g -c a.c
b.o:	b.c prog.h
	cc -g -c b.c
.P2
A better solution is to use a
.I variable .
A
.I mk
variable has a similar form and use to a shell variable.
A suitable (mnemonic) name is
.CW CFLAGS .
The new mkfile looks like this:
.P1 0
CFLAGS=-g
prog:	a.o b.o
	cc $CFLAGS -o prog a.o b.o
a.o:	a.c
	cc $CFLAGS -c a.c
b.o:	b.c prog.h
	cc $CFLAGS -c b.c
.P2
Now, if we want to profile
.CW prog
(which means compiling everything with the
.CW -p
option),
we need only change the first line to
.P1 0
CFLAGS=-g -p
.P2
and recompile all the object files.
The easiest way to recompile everything is with
.CW "mk -a"
which says to always make every target regardless of time stamps.
.PP
Some variables are supplied by
.I mk
for use by the recipe.
One is
.CW prereq
whose value is all the prerequisites for this rule.
We can rewrite the first rule like this:
.P1 0
prog:	a.o b.o
	cc $CFLAGS -o prog $prereq
.P2
This guarantees that the two lists of object files
(the prerequisite line and the
.I cc
line) are the same.
It is now easy to incorporate a new object file
.CW c.o
by adding the new name just once:
.P1 0
CFLAGS=-g -p
prog:	a.o b.o c.o
	cc $CFLAGS -o prog $prereq
a.o:	a.c
	cc $CFLAGS -c a.c
b.o:	b.c prog.h
	cc $CFLAGS -c b.c
c.o:	c.c prog.h
	cc $CFLAGS -c c.c
.P2
Variables get their initial value by the following rules
(in decreasing order of precedence):
.IP [1] 5
command line assignments (arguments with an embedded
.CW = )
.IP [2]
assignments within the mkfile
.IP [3]
values inherited from the environment
.IP [4]
default values as defined in Appendix 1.
.LP
After a variable gets its initial value,
subsequent values can only be assigned in the mkfile.
.P1 0
$ cat mkfile
SYSTEM=-DV9
CFLAGS=-g
CFLAGS="$CFLAGS $SYSTEM"
printcflags:Q:
	echo $CFLAGS
$ mk
-g -DV9
$ mk SYSTEM=-DSYSTEMV
-g -DSYSTEMV
$ mk CFLAGS=-O
-O -DV9
.P2
The
.CW Q (uiet)
attribute is described later;
briefly, it suppresses printing of the recipe.
.NH 2
Metarules
.PP
The rules for all the
.CW .o
files are very similar.
.I Mk
supports
.I metarules ,
that is, rules that apply to a class of targets, rather than just one
specific target.
The class of targets is defined by pattern matching,
with the symbol
.CW %
(called the stem)
equivalent to the regular expression
.CW .* .
For example, the normal rule for compiling C source files is
.P1 0
%.o:	%.c
	$CC $CFLAGS -c $stem.c
.P2
The variable
.CW stem
in the recipe is the string matched by the
.CW % .
The
.CW %
can appear anywhere in the target or prerequisite,
not just at the beginning.
The
.CW CC
variable is good planning;
a different compiler can be used very easily.
Using this metarule, our mkfile becomes shorter: 
.P1 0
CC=cc
CFLAGS=-g -p
prog:	a.o b.o c.o
	$CC $CFLAGS -o prog $prereq
b.o:	prog.h
c.o:	prog.h
%.o:	%.c
	$CC $CFLAGS -c $stem.c
.P2
Notice that the prerequisites for a target can be spread across many rules.
Two rules apply to
.CW b.o ,
the specific rule with
.CW prog.h
and the metarule for
.CW .o 's.
Metarules with recipes do not match targets of non-metarules with recipes.
Thus, metarules for generating
.CW .o 's
(say) do not conflict with any rule for generating a specific
.CW .o .
Only one of the rules for a target should have a recipe.
If there is more than one recipe,
.I mk
complains that the way to make the target is ambiguous.
.PP
.I Mk
has some predefined variables and rules listed in Appendix 1.
Because our rule for
.CW %.o
and the value for
.CW CC
are the same as the predefined rules and variables,
we can omit them for a shorter mkfile:
.P1 0
CFLAGS=-g -p
prog:	a.o b.o c.o
	$CC $CFLAGS -o prog $prereq
b.o:	prog.h
c.o:	prog.h
.P2
.NH 2
Rules with no prerequisites
.PP
Rules need not actually build their targets.
Some rules are simply shell scripts embedded in the mkfile for
convenience.
For example, most mkfiles have the target
.CW clean :
.P1 0
clean:
	rm -f *.o prog core
.P2
Note that
.CW clean
is intended as a label, not a file.
Unfortunately, if a file named
.CW clean
exists, the recipe will not be executed, since
.CW clean
is up to date (because no prerequisite has caused it to be out of date).
We want to avoid any such inadvertent interactions with the file system.
.I Mk
allows a label to have an attribute of
.I virtual ,
which means that it is distinct from a file of the same name.
Targets can be marked as virtual by appending a
.CW V:
to the colon separator between targets and prerequisites:
.P1 0
clean:V:
	rm -f *.o prog core
.P2
Attributes (like
.CW V )
that apply to labels can be given by more than one rule.
Where feasible, it is good style to use an attribute on all rules
applying to a particular label, not just one.
Other attributes are described below.
.NH 2
Rules with multiple targets
.PP
The rules relating
.CW b.o
and
.CW c.o
to
.CW prog.h
can be combined into one rule with two targets.
.P1 0
CFLAGS=-g -p
prog:	a.o b.o c.o
	$CC $CFLAGS -o prog $prereq
b.o c.o:	prog.h
clean:V:
	rm -f *.o prog core
.P2
If a rule with multiple targets has no recipe, it is simply a shorthand notation
for all the simple rules with one target.
A rule with multiple targets and a recipe has subtle implications described below.
To motivate the subtleties, we digress to describe the
.I yacc
parser generator.
.PP
.I Yacc
takes a file describing a grammar and produces the source for a C routine
that will parse input according to the given grammar.
The source is put in the file
.CW y.tab.c .
.I Yacc
also produces a header file called
.CW y.tab.h
that links the parser to a lexical analyzer.
The grammar file also contains semantic action code.
Typically, changes to the grammar file do not change the header
.CW y.tab.h ,
but only the semantic routines embedded in
.CW y.tab.c .
.PP
Let us add a grammar and a lexical analyzer to
.CW prog
(omitting some unimportant detail):
.P1 0
prog:	a.o b.o c.o y.tab.o lex.o
	$CC $CFLAGS -o prog $prereq
b.o c.o:	prog.h
lex.o:	y.tab.h
y.tab.c y.tab.h:	gram.y
	yacc -d gram.y
.P2
The grammar is kept in
.CW gram.y
(the conventional suffix for
.I yacc
input is
.CW .y ).
The
.CW -d
option to
.I yacc
produces
.CW y.tab.h .
Unfortunately, this mkfile does too much work in the normal case.
Every time the grammar file is changed, a new
.CW y.tab.h
is made and thus
.CW lex.o
will always be out of date even though the contents of
.CW y.tab.h
may not have been changed.
The best solution maintains another header file
(say
.CW x.tab.h )
that only changes when necessary,
that is, when the contents of
.CW y.tab.h
actually change.
The new mkfile is
.P1 0
prog:	a.o b.o c.o y.tab.o lex.o
	$CC $CFLAGS -o prog $prereq
b.o c.o:	prog.h
lex.o:	x.tab.h
x.tab.h:	y.tab.h
	cmp -s x.tab.h y.tab.h ||
	cp y.tab.h x.tab.h
y.tab.c y.tab.h:	gram.y
	yacc -d gram.y
.P2
The
recipe for
.CW x.tab.h
is a conditional shell construct;
if the command
.CW "cmp -s x.tab.h y.tab.h"
returns with an error (the files are different),
then execute the command
.CW "cp y.tab.h x.tab.h"
to update
.CW x.tab.h .
In the case where
.CW y.tab.h
doesn't change, the action is straightforward:
.P1 0
$ mk -e
y.tab.c(541051073) < gram.y(541051092)
y.tab.h(541051072) < gram.y(541051092)
yacc -d gram.y
y.tab.o(541051082) < y.tab.c(541051100)
cc  -c y.tab.c
.P3
x.tab.h(541042236) < y.tab.h(541051099)
cmp -s x.tab.h y.tab.h || cp y.tab.h x.tab.h
\f2cp not done\fP
prog(541051087) < y.tab.o(541051109)
cc  -o prog a.o b.o c.o y.tab.o lex.o
.P2
If we now change the grammar so that the header file does change:
.P1 0
$ mk -e
y.tab.c(541051100) < gram.y(541051148)
y.tab.h(541051099) < gram.y(541051148)
yacc -d gram.y
y.tab.o(541051109) < y.tab.c(541051155)
cc  -c y.tab.c
x.tab.h(541042236) < y.tab.h(541051154)
cmp -s x.tab.h y.tab.h || cp y.tab.h x.tab.h
\f2cp done, x.tab.h updated\fP
lex.o(541042267) < x.tab.h(541051165)
cc  -c lex.c
prog(541051114) < y.tab.o(541051163)
prog(541051114) < lex.o(541051169)
cc  -o prog a.o b.o c.o y.tab.o lex.o
.P2
The subtleties are twofold.
The first is that the time stamps for files are only examined
when the file is initially referenced or when it is the target of a rule.
If
.CW y.tab.h
had not been a target for the
.I yacc
rule,
then
.I mk
would assume that
.CW y.tab.h
had not been updated.
The second subtlety
is that the rule for
.CW x.tab.h
need not change
.CW x.tab.h .
If it does not, then
.CW lex.o
need not be recompiled.
.NH 2
Aggregates
.PP
Some of the things we would like to maintain with
.I mk
are actually collections or
.I aggregates
of entities, such as
.UX
object libraries (archives maintained by
.I ar ).
Other (unsupported as yet) examples are
.I cpio
and SCCS files.
The type of aggregate is determined by the file's ``magic number'',
often the first 16 bits of the file.
Each type has support code within
.I mk
to get the time stamp of a member and to ``touch'' (see below) a member.
The notation
.CW a(m)
refers to member
.CW m
of aggregate
.CW a .
For example, consider an archive
.CW lib.a
made up of
.CW a.o ,
.CW b.o ,
and
.CW c.o .
The mkfile looks like
.P1 0
lib.a:N:     lib.a(a.o) lib.a(b.o) lib.a(c.o)
lib.a(%.o):  %.o
	ar r lib.a $stem.o
.P2
(The
.CW N
attribute is described below.)
As each new
.CW \&.o
file is generated, it is put into
.CW lib.a .
This is straightforward and correct but inefficient:
an
.CW ar
command is executed for every out of date object file.
A better way is to generate all the
.CW .o
files and then do the
.I ar .
We use the variable
.CW newprereq
(supplied by
.I mk )
because we only need to replace the object files that have changed.
The new mkfile relies on a shell script called
.I membername :
.P1 0
lib.a:	lib.a(a.o) lib.a(b.o) lib.a(c.o)
	ar r lib.a `membername $newprereq`
lib.a(%.o):N:	%.o
.P2
.I Membername
takes aggregate notation and extracts the member names.
For example,
.P1 0
$ membername 'lib.a(a.o)' 'lib.a(b.o)'
a.o b.o
.P2
The quotes are to stop the shell from interpreting the
.CW () .
.NH 2
Targets without recipes
.PP
Sometimes a target may be out of date with respect to some prerequisites
and yet there is no recipe that
.I mk
can use to remake the target.
For example,
.P1 0
all:V:	prog1 prog2 prog3
.P2
When we say
.CW "mk all" ,
we simply want to make
.CW prog1 ,
.CW prog2 ,
and
.CW prog3 .
Because
.CW all
is virtual, it doesn't matter that it isn't actually remade;
all
.I mk
does with virtual targets is propagate the date stamp.
A more serious example is
.P1 0
prog:	a.o b.o c.o
	$CC $CFLAGS -o $target $prereq
%.o:	hdr.h
.P2
If there is no
.CW c.c ,
then
.I mk
does not know how to make
.CW c.o
and yet needs it in order to make
.CW prog .
The only rule for
.CW c.o
is the dependency on
.CW hdr.h
which has no recipe.
Therefore, in these situations,
where a nonvirtual target is out of date but none of the rules
applying to that target has a recipe,
.I mk
will stop and complain that it doesn't know how to make that target.
.PP
There is one type of rule where this is not the right thing to do.
Take for example the above archive rule:
.P1 0
lib.a:	lib.a(a.o) lib.a(b.o) lib.a(c.o)
	ar r lib.a `membername $newprereq`
lib.a(%.o):N:	%.o
.P2
There is no recipe to build (say)
.CW lib.a(a.o)
but this is okay as it will be built later by
.I ar .
The
.CW N
attribute stops
.I mk
from complaining that there is no recipe.
.NH 2
Recipe Execution
.PP
Unlike
.I make ,
.I mk
executes recipes without performing any textual substitutions.
The recipe is collected, deleting the initial tab character and the result
is given as standard input to
.CW "/bin/sh -e"
(the
.CW -e
means stop on error).
Variables such as
.CW $target
are simply shell variables passed through the environment to the shell
running the recipe.
Thus, the recipe is exactly a shell script with a tab prepended to each line.
This allows many applications to avoid many small shell and
.I awk
scripts by storing these in the recipes.
Such an example comes from Bart Locanthi; the domain is circuit board design:
.P1 0
parts:D: gnot.wx
	grep "^\.c" $prereq | sort | uniq | awk '
		{ count[$3]++ }
	END	{ for (c in count){
			print c "\et" count[c]
		  }
		}' | sort > $target
.P2
.PP
Be warned that these scripts cannot read from standard input
as they themselves are standard input to the shell.
Thus, in order to read or write to the terminal requires using
.CW /dev/tty
or its equivalent.
.NH 2
Parallel processing
.PP
.I Mk
executes recipes by continually traversing the dependency graph
looking for targets that can be made.
For example, in our mkfile:
.P1 0
prog:	a.o b.o c.o y.tab.o lex.o
	$CC $CFLAGS -o prog $prereq
b.o c.o:	prog.h
lex.o:	x.tab.h
x.tab.h:	y.tab.h
	cmp -s x.tab.h y.tab.h ||
	cp y.tab.h x.tab.h
y.tab.c y.tab.h:	gram.y
	yacc -d gram.y
.P2
the target
.CW a.o
can be made immediately,
while the target
.CW y.tab.o
has to wait for
.CW y.tab.c
to be made.
When
.I mk
finds a recipe it can execute, it puts the recipe on a queue.
When the recipe terminates,
.I mk
updates the dependency graph.
The number of recipes executing simultaneously
is bounded by the value of the variable
.CW NPROC ,
which is initially one.
On multi-processor machines,
.I mk
goes faster with higher values;
most mkfiles on our 12 processor machine have
.CW NPROC
between 6 and 10.
In most situations, increasing
.CW NPROC
beyond a certain limit gains little
as we run into other system bottlenecks such as disk I/O.
The other way to speed up parallel builds is to ensure that as many recipes
as possible are executing; that is, order the sub-targets
such that the slowest are done first.
While
.I mk
gives no guarantees about the order of builds, generally
prerequisites are built in left-to-right order as in the mkfile.
.PP
The
.CW -u
(utilization)
option measures how many seconds (real time)
.mk
are spent with so many recipes executing.
For example, building
.CW prog
with three simultaneous recipes yields
.P1 0
0: 1
1: 4
2: 7
3: 10
.P2
This means that the entire run took 22 seconds real time;
10 seconds with three recipes running, 7 with two and 4 with one.
The time with zero recipes executing corresponds to
.I mk
reading the mkfile and building the dependency graph.
.PP
Parallel execution implies that recipes should not interact unnecessarily.
For example, the first version of the library mkfile should not be run in parallel
as simultaneous
.I ar 's
on the same archive interfere.
The second version can be run in parallel because
only one
.I ar
is done,
after all the object files are made.
Arguably,
.I mk
should support some primitives for handling recipes that might collide
(other parallel makes such as
.I cmake |reference(cmake)
support mechanisms such as mutually exclusive recipes).
.I Mk
doesn't; the need has never arisen.
.NH 2
Missing intermediates
.PP
In all the examples we have seen so far,
.I mk
has made all the targets ``between'' the file that changed and the main target.
This is not always done.
Any non-existent intermediate target
(a target other than the root target with prerequisites)
is treated specially.
If pretending it existed with the time stamp of its most recent prerequisite
would make all targets that depended on it be up to date,
then it is not made.
For example, in our mkfile:
.P1 0
$ mk -e
mk: `prog' is up to date
\fIremove a.o\fP
$ mk -e
pretending a.o has time 540869454
mk: `prog' is up to date
.P2
The intuition is that if we use the mkfile to build the targets,
then removing the intermediates causes no harm.
Of course, if we actually need the missing intermediates,
.I mk
builds them.
.P1 0
\fIchange b.c\fP
$ mk -e
pretending a.o has time 540869454
b.o(540869546) < b.c(541350226)
cc  -c b.c
unpretending a.o because of prog because of b.o
a.o(0) < a.c(540869454)
cc  -c a.c
prog(541104056) < a.o(541350255)
prog(541104056) < b.o(541350244)
cc  -o prog a.o b.o c.o y.tab.o lex.o
.P2
The action is not too hard to follow:
first
.I mk
sees that
.CW a.o
is missing and pretends it is there.
Then
.I mk
notices
.CW b.o
is out of date and needs to be rebuilt.
When
.CW b.o
is finally built,
it causes
.CW prog
to become out of date and therefore
.I mk
no longer can pretend that
.CW a.o
is up to date.
It then builds
.CW a.o
and then
.CW prog .
.PP
The major advantage of missing intermediates is avoiding multiple copies of files.
For example, in our mkfile to maintain a library,
we keep two copies of every object file.
By using the notion of missing intermediates, we can keep one copy \(em the copy
we need in the archive.
To do so, simply remove the object files after they have been archived:
.P1 0
lib.a:	lib.a(a.o) lib.a(b.o) lib.a(c.o)
	names=`membername $newprereq`
	ar r lib.a $names && rm $names
lib.a(%.o):N:	%.o
.P2
We store the object files' names in the variable
.CW names
to avoid executing
.I membername
twice.
The
.CW &&
is another conditional shell construct;
we remove the files only if the archive command succeeds.
.PP
The special treatment of missing intermediates is suppressed by the
.CW -i
option of
.I mk .
.NH 2
Administrative
.PP
.I Mk
provides an easy way to bring a target up to date without actually doing any work.
For example, if we change
.CW prog.h
in such a way that
.CW b.o
or
.CW c.o
won't change
(such as adding a comment),
we don't want to recompile the files.
Instead, we can ask
.I mk
to (chattily) modify the files' time stamps.
.P1 0
\fIadd something to prog.h\fP
$ mk -t
touch(b.o)
touch(c.o)
touch(prog)
.P2
This is a dangerous feature; use it carefully and sparingly.
Virtual targets are not affected because
.I touch ing
only changes files.
.PP
.I Mk
can also tell us what it would do without actually doing it.
The option
.CW -n
causes recipes to be printed rather than executed.
There are two caveats.
.I Mk
assumes that every recipe will update all its targets.
Normally this is true, but for our mkfile,
.CW "mk -n"
would erroneously indicate that
.CW lex.o
will always be remade.
Thus, unnecessary work may be indicated.
The second problem is that when
.I mk
prints the recipe it is about to execute
(or would execute under
.CW -n ),
it expands recognizable references to shell variables.
It does this without parsing the shell script and thus can make mistakes
by ignoring variable assignments within a recipe and with constructs like
.I for
loops.
For example, with the mkfile
(the
.CW Q
attribute suppresses the normal recipe echo)
.P1 0
i=a b c
all:Q:
	for i in x y z
	do
		echo $i
	done
.P2
the difference between
.CW mk
and
.CW "mk -n"
is:
.P1 0
$ mk -n
for i in x y z
do
	echo a b c
done
$ mk
x
y
z
.P2
This latter problem applies to the normal recipe echo as well.
.PP
Sometimes we would like to know what
.I mk
would do if some files were changed.
The
.CW -w\fIfiles,...
option supports this ``what if'' query by setting the time stamps internally
for the named files to the current time.
With our mkfile
for
.CW prog ,
we can ask what would happen if we changed
.CW prog.h :
.P1 0
$ mk -n -wprog.h
cc  -c b.c
cc  -c c.c
cc  -o prog a.o b.o c.o y.tab.o lex.o
.P2
The advantage of
.CW -w
is that neither the files nor their time stamps are changed.
Of course,
.CW -w
can be used without using
.CW -n .
For example, to force
.I mk
to remake
.CW b.o
we can say
.P1 0
$ mk -wb.c b.o
cc  -c b.c
.P2
.NH 2
Quoting
.PP
The quoting rules for assignment lines and rule header lines
are intended to be the same as for
.I sh (1)
(the Bourne shell).
As these rules are not described clearly in
.I sh (1),
we describe
.I mk 's
quoting rules below.
The term
.I "quoting a character"
means making that character stand for itself,
rather than any special, or meta, meaning.
For example,
.CW $a
stands for the value of the variable
.CW a ,
whereas
.CW \e$a
(the
.CW $
is now quoted)
stands for the two characters
.CW $
and
.CW a .
.PP
Input is collected until a newline without a preceding
.CW \e
is seen.
During this collection,
.CW \e
quotes every character except newline (which is deleted),
and text between unquoted single quotes is quoted.
Inside single quotes, every character is quoted.
The resulting text is rescanned for unquoted backquotes.
When an unquoted backquote
.CW \&`
is seen, input is collected until the next unquoted backquote.
The collected input is given as standard input to the shell
and the standard output replaces the collected input and the two backquotes
as quoted text.
The resulting text is rescanned (again!) for
double quotes and variable expansions.
.tr #"
Text between double quotes is quoted after variable expansion is done and
.CW \e
only quotes the characters
.CW #'$\e .
.PP
.tr ##
The text is then broken into parts separated by unquoted white space
and any part containing
.CW [*?
as unquoted characters is then expanded as filenames as per
.I sh (1).
.NH 2
More on metarules
.PP
There are actually two kinds of metarules;
we have looked at the first kind that uses
.CW %
to match arbitrary strings.
The second kind uses full regular expressions as supported in
.I regexp (3).
The expression may include sub-expressions enclosed in
.CW () ;
the values of the sub-expressions can be used in the prerequisites and the recipe.
For example, consider the problem of making object files in sub-directories.
That is, we wish to make
.CW dir/a.o
from
.CW dir/a.c .
Because the C compiler only generates object files in the current directory,
the normal builtin rule does the wrong thing.
To fix this, we need to break the target into two parts:
.P1 0
\&'(.*)/([^/]*)\e.o$':R:	'\e1/\e2.c'
	cd $stem1; $CC $CFLAGS -c $stem2.c
.P2
The
.CW R
attribute for the rule means interpret the target(s) as regular expression(s).
The different ways of referring to the sub-expressions
(\f(CW$stem1\fP inside the recipe and
.CW \e1
on the rule header line) are regrettably a consequence
of not processing the recipe.
Note also that this rule will probably conflict with the normal builtin rule for
.CW .o 's.
A warning:
regular expression metarules are significantly slower than
.CW %
metarules.
.PP
Another way to use this is to specify groups of targets that are handled
in similar ways.
For example, some programs may need different libraries or loader flags.
.P1 0
\&'^(foo|bar)$':R:	'\e1.o'
    $CC $CFLAGS -o $target $prereq -lipc
\&'^(gee|whiz)$':R:	'\e1.o' mylib.a
    $CC $CFLAGS -o $target $LDFLAGS $prereq
.P2
.PP
Regardless of which kind of metarule you use,
certain metarules can lead to infinite dependency graphs.
For example, the metarule
.P1 0
%:	%.z
	unpack $stem.z
.P2
gives this dependency graph
.PS 2.5i
hu=1
"x"; ln(1,0,->); "x.z"; ln(1,0,->); "x.z.z"; ln(1,0,->); "..."
.PE
.sp
The problem arises any time a metarule has a prerequisite that can be a target
of the same rule.
.I Mk
handles this problem by restricting the number of times a metarule is used
in generating prerequisites to the value of the variable
.CW NREP .
This value is normally one; if set to 2,
the dependency graph for
.CW x
in our example is
.PS 2.5i
hu=1
"x"; ln(1,0,->); "x.z"; ln(1,0,->); "x.z.z"
.PE
.sp
Thus, setting
.CW NREP
to greater than one is necessary if we have files that have been packed repeatedly.
.NH 1
Getting Fancy
.PP
This section is a collection of problems
.I mk
users
(and
.I make
users in general) run into and how I have seen
.I mk
used to deal with those problems.
In general,
other make programs have special features to deal with these problems;
.I mk
solves them, sometimes clumsily, by its general mechanisms.
.NH 2
Namelists
.PP
One of the first problems you run into when the mkfiles get big
is having several versions of file names.
For example:
.P1 0
SRC=main.c lex.l gram.y subr.c
OBJ=main.o lex.o gram.o subr.o
pr:V:	$SRC
	pr $prereq | lp
prog:	$OBJ
	$CC $CFLAGS -o $target $prereq
.P2
We can derive the
.CW OBJ
list from the
.CW SRC
list by:
.P1 0
SRC=main.c lex.l gram.y subr.c
OBJ=`echo "$SRC" | sed 's/\e../.o/g'`
.P2
A less general but more efficient mechanism works on variable values.
A variable value of the form
.CW ${var:\fIa\fP%\fIb\fP=\fIc\fP%\fId\fP}
is evaluated by expanding
.CW $var
into a list of white space separated words, and for every word that is of the form
.I a\f(CW%\fPb
(where
.I a
and
.I b
are (possibly null) literals and
.CW %
matches any string of characters, substitute
.I c\f(CW%\fPd
(where
.I c
and
.I d
are (possibly null) literals and if
.CW %
is present, the characters that matched the
.CW %
on the left side.
For example, if you are using two different compilers that produce
object files with different suffices:
.P1 0
SRC=a.c b.c c.c
LOBJ=${SRC:%.c=%.o}
KOBJ=${SRC:%.c=%.2}
.P2
Prefixes can be exercised by SCCS:
.P1 0
SRC=s.a.c s.b.c
PROGS=${SRC:s.%=%}
OBJ=${SRC:s.%.c=%.o}
.P2
.NH 2
Dependencies on variables
.PP
Often, an object file generated from a C source depends on compiler flags embedded
in the makefile.
The ``correct'' thing to do would be to make the object file depend on the makefile
but this would imply remaking all the objects whenever you changed the makefile
even though most of the changes would not be concerned with the compiler flags.
Some makes, for example
.I nmake |reference(nmake),
store the flags used to make the object in a database and
thus can tell when it changes.
The
.I mk
solution is to follow the paradigm of files depending on files;
put the C compiler flags in a file!
.P1 0
$ cat CFLAGS
-g -DSYSV
$ cat mkfile
CFLAGS=`cat CFLAGS`
%.o:	CFLAGS
.P2
Everything falls out; objects get remade just when they need to.
This trick can often be used when files depend on parts of the mkfile.
Other examples are adding a new object file to a program or
deleting an element of an archive library.
Both of these cases can be handled the same way;
make them depend on a file which contains the element names.
.NH 2
Deleting targets on errors
.PP
Sometimes a recipe fails
.I after
modifying the target.
Often in this case, the fix doesn't cause the target to become out of date.
(This can happen when
.I mk
is interrupted;
a target is probably not complete but it is up to date!)
For these cases, use the
.CW D
attribute;
.I mk
will remove the target on errors.
.I Mk
reports exactly what files were removed.
This is normally only necessary for recipes of the form
.CW "\&.... > $target" .
For example, (folding the long output line at \*(cr)
.P1 0
$ mk
pic mk.ms | tbl | troff -mpm > mk.out
\fIsend an interrupt\fP
mk: pic mk.ms | ...  : exit status=1,\*(cr
	deleting 'mk.out'
mk: interrupted!
.P2
.NH 2
Nontemporal dependencies
.PP
.I Mk
supports a general mechanism for determining if a file is out of date.
The
.CW :Pcmd:
attribute
means that to find out if
.I a
is out of date with respect to
.I b ,
.I mk
runs the command
.P1 0
cmd 'a' 'b'
.P2
and uses the exit status as the out of date indicator
(a non-zero status means out of date).
For example, the yacc rule can be rewritten as
.P1 0
x.tab.h:Pcmp -s:	y.tab.h
	cp $prereq $target
.P2
The meaning here is that
.CW x.tab.h
is out of date only if it is different from
.CW y.tab.h .
.NH 2
Updating a target every time
.PP
The following mkfile only executes the recipe if the target
.CW a
doesn't exist.
.P1
a:
	# some recipe ...
.P2
This is because if
..CW a
exists,
there are no prerequisites that can cause it to be out of date.
If you seek the
.I make
behavior of always executing the recipe, use the
.CW P
attribute to always make the target out of date:
.P1
a:Pexit 1:	/
	# some recipe ...
.P2
We have to give it at least one prerequisite;
.CW /
is a convenient file that always exists on
.UX
systems.
.NH 2
Quick hacks
.PP
Often mkfiles do not include all the header file dependencies.
How do you safely remake things after changing the definition of a variable
.CW var ?
.P1 0
$ mk -w"`grep -l var *.[cyl]`"
.P2
The double quotes keep the file names together as one argument.
.PP
.I Mk
does not support the conditional variable reference in
.I sh (1).
You can easily fake it with backquotes:
.P1 0
GOAL=`echo "${GOAL:-all}"`
.P2
This sets
.CW $GOAL
to the value in the environment or if that was null then to
.CW all .
.NH 2
Segmented mkfiles
.PP
.I Mk
supports three ways to partition a mkfile.
The first is to set the environment variable
.CW BUILTINS .
.P1 0
BUILTINS='%.z:	%
	pack $stem'
.P2
The second is to invoke
.I mk
with multiple mkfile arguments.
.P1 0
$ mk -f /usr/cad/rules -f board.mk amp.pcb
.P2
The third is to include another file from within a mkfile using the
.CW <
operator.
.P1 0
$ cat mkfile
< $SRC/CONF
< DEPEND
prog:	a.o b.o c.o
	$CC $CFLAGS -o $target $prereq
.P2
.NH 2
Dynamic mkfiles
.PP
The
.CW <
attribute can be used to change
.I mk 's
behavior at runtime.
For example, you might want to generate two versions of a program from the same mkfile.
.P1 0
$ cat mkfile
CFLAGS=-g
CC=68cc
prog:	prog.c
	$CC $CFLAGS -o $target $prereq
vax:<:
	echo "CFLAGS='$CFLAGS -Dvax'"
	echo "CC=vaxcc"
$ mk vax prog
vaxcc -g -Dvax -o prog prog.c
.P2
The only useful changes are to variable definitions;
adding rules is useless because the dependency graph has already been formed.
In this case, you must call
.I mk
recursively.
Following the example above,
.P1 0
$ cat mkfile
default:VQ:
	echo "there is no default target!"
vax.%:VQ:
	mk $MKFLAGS -f vax.mk -f gen.mk $stem
.P2
.NH 1
Differences between \f4make\fP and \f4mk\fP
.PP
The qualitative differences between
.I mk
and
.I make
can be summarized as
(pertinent differences to the older
.I make s
are noted in parentheses):
.IP \(bu 3n
.I Make
builds targets when it needs them, allowing systematic use of side effects.
.I Mk
constructs the entire dependency graph before building any target.
.IP \(bu
.I Make
supports suffix rules and
.CW %
metarules.
.I Mk
supports
.CW %
and regular expression metarules.
(Older
.I make s
only have suffix rules.)
.IP \(bu
.I Mk
performs transitive closure on metarules,
.I make
does not.
.IP \(bu
.I Make
supports cyclic dependencies,
.I mk
does not.
.IP \(bu
.I Make 's
recipes are collections of one-line shell commands,
executed a line at a time.
Variable values are passed by editing the recipe text before passing it
through to the shell.
.I Mk 's
are simply shell scripts executed as one unit.
Variable values are passed through environment variables.
.IP \(bu
.I Make
supports parallel execution of single-line recipes when building
the prerequisites for specified targets.
.I Mk
supports parallel execution of all recipes.
(Older
.I make s
did not support any parallel execution of recipes.)
.IP \(bu
.I Make
uses special targets (beginning with a
.CW . )
to indicate special processing.
.I Mk
uses attributes indicated by qualifiers after the
.CW :
separator in a rule definition.
.IP \(bu
.I Mk
allows the standard output of a recipe to be read as an additional
mkfile while
.I mk
is running.
This allows a mkfile to configure itself at run time.
.IP \(bu
.I Mk
supports
.I virtual
targets which exist only within an execution of
.I mk
and are independent of the underlying file system.
.IP \(bu
.I Mk
supports a general mechanism for deciding whether a file is out of date
as well as the normal method of comparing file modification times.
.PP
In most situations,
mkfiles and makefiles (the input for
.I make )
will have only minor syntactic differences.
In practice, mkfiles are often bigger
because of embedded shell scripts or to make the most of underlying
parallel hardware.
(Parallelism works best when it has a lot to do;
one way to do this is to merge mkfiles together
and have one mkfile to control multiple subdirectories.)
.PP
The most striking difference between
.I mk
and
.I make
is in speed of execution.
There are three main factors involved.
.I Make
uses a linear list to access variables and rules;
.I mk
uses a hash table.
.I Mk
and
.I make
use time stamps in slightly different ways;
.I make
often has to measure a file's time stamp unnecessarily.
If there are metarules,
.I mk
will typically create a much larger dependency graph than
.I make .
The graph gets pruned but at the cost of testing (for existence)
a large number of files.
In the examples given below,
execution times are given (in seconds) as a sum of user time
(a measure of how efficiently the dependency graph is built and executed)
and system time (a measure of how many time stamps are measured).
The times do not include times for recipe executions.
.PP
For mkfiles with no metarules,
.I mk
is always faster than
.I make
because of better accessing algorithms.
For example,
the mkfile to compile the operating system describes 83 object files.
.I Make
takes 19.8u+3.6s
(19.8 seconds of user time, 3.6 seconds of system time),
.I mk
takes 6.6u+3.6s.
.I Mk
is faster by a factor of 3 (user time) and 2.3 (user+sys).
.PP
For more normal mkfiles (that use the builtin metarules),
.I make
is somewhat faster than
.I mk
until about a dozen prerequisites are involved.
.I Mk
is much better for larger mkfiles.
In most cases,
.I mk 's
performance can be improved by only using necessary metarules.
For example,
for a program made from 61 object files all compiled from
.CW \&.c
files, Table 1 gives the times for a normal mkfile
and a mkfile that has the only necessary metarule (generating
.CW %.o
from
.CW %.c ).
.1C
.KF
.TS
center;
|c| c| c| c|
|^| ^| c| c|
|l| c| n| n|.
_
Command	Run Time	Relative Speed	Relative Speed
		(user)	(user+sys)
=
make	12.0u+9.7s	1	1
mk (all metarules)	5.1u+4.0s	2.3	2.4
mk (one metarule)	3.9u+2.9s	3	3.2
_
.TE
.sp .5
.ce
\fBTable 1.\fP Execution times
.KE
.2C
.PP
.I Mk
handles aggregates efficiently.
The main C library has 242 members.
.I Make
takes 47.7u+10.9s,
.I mk
takes 6.3u+12.5s.
.I Mk
is faster by a factor of 7.6 (user time) and 3.1 (user+sys).
.PP
The final example comes from Ted Kowalski at AT&T Bell Laboratories.
The mkfile is about 20,000 characters and describes an experimental
workstation environment
built from 238
.CW \&.c
files, 59
.CW \&.h
files, 7
.CW \&.y
files and 7
.CW \&.l
files.
The mkfile makes heavy use of variables.
.I Make
takes 278.8u+16.2s,
.I mk
takes 8.4u+10.5s.
.I Mk
is faster by a factor of 33 (user time) and 15.6 (user+sys).
.PP
Despite the marked speed advantage of
.I mk
over
.I make ,
the main reason users in our computing community use
.I mk
is its functionality,
in particular, transitive closure on metarules,
parallel execution of recipes,
and the regular expression metarules.
.NH 2
Conversion between \f4make\fP and \f4mk\fP
.PP
Conversion of makefiles into mkfiles comes in two parts.
The first is a mechanical process of syntax conversion
(such as changing variable references) handled by the
.I sed (1)
script
.I mkconv .
It produces a mkfile on its standard output and warns about known nasties
such as recursive calls to
.I make
and use of the
.I cd
command.
For routine makefiles, this is all that needs to be done.
.PP
If needed, the other changes have to be done by hand.
They involve the use of side-effects by
.I make ,
such as the normal way
.I yacc
grammars are handled.
The proper way to handle these grammars is described above;
in other cases, the general rule is to tell the truth about dependencies
and let the dynamic time measuring prevent unnecessary work.
.I Mk
has much support for the debugging for these cases,
particularly where the makefile is complex or subtle.
The most useful options are
.CW -dg
(to find out the exact dependency graph),
.CW -e
(to explain why
.I mk
thinks something is out of date),
and
.CW -n
and
.CW -w
(to conduct what if? experiments).
.PP
Conversion of mkfiles into makefiles is an analogous process.
The command
.P1
mk -m
.P2
generates a makefile on its standard output.
It forms the dependency graph and then emits it in makefile format;
thus, any needed transitive closure has already been done and will be
output in expanded form.
Beware, this is a new and relatively untested feature.
.NH 2
Availability of \f4mk\fP
.PP
There are three sources for
.I mk
depending on who wants it.
AT&T Bell Laboratories employees can get it from USTOP.
Commercial
.UX
licensees can obtain
.I mk
from the AT&T Toolchest
(1-800-828-UNIX to talk to a person;
1-201-522-6900, login
.CW guest ,
to browse and talk to a computer).
Educational (and Administrative)
.UX
licensees can get an electronic
or magnetic tape distribution from
.DS
.nf
Judith L. Macor
Computing Information Service
AT&T Bell Laboratories
600 Mountain Avenue
Murray Hill, NJ 07974
.DE
.LP
.I Mk
runs on a variety of hardware:
VAX, 3B, Sun (68000), Sequent (32032), MIPS and Cray X-MP/24.
It is not tuned to any particular variant of the
.UX
operating system;
it runs on Research V9, BSD4.[23], Dynix, UNICOS and various forms of System V.
The distribution is self-contained; it includes the
.I mk
source, library support for systems without the V9 libraries and documentation
including a manual entry and this tutorial.
The
.I mk
source is 63KB (3500 lines) in 33 files.
The text size of the executable (on a VAX) is about 34KB.
.LP
Bugs, questions or other feedback should be directed to the author.
.NH 1
The Principles
.PP
.I Mk 's
semantics and syntax were designed according to a few general principles
or guidelines.
.SS "Use existing syntax and notions."
The syntax of mkfiles is almost exactly the same as a makefile
(used by
.I make ).
(The only syntactic change for rules is the attribute marking.)
.I Mk 's
variables are exactly the same as shell variables.
Recipes are written in
.I sh (1),
not a special purpose language.
The regular expression syntax and semantics were adopted
from existing tools (such as
.I egrep
and
.I ed ),
trading some awkwardness for familiarity.
.SS "Generalize features."
.I Make 's
metarules (already a generalization of the early
.I make
suffix rules)
were extended to full regular expressions.
.I Mk
performs the transitive closure on the target-prerequisite relations defined
by all rules, including metarules.
The primitive form of parallel processing supported by
.I make
has been generalized to allow parallel execution of any recipe.
By constructing the entire dependency graph before executing any recipes,
.I mk
maximizes the benefits from parallel processing.
.SS "Removing special cases."
.I Make 's
variables and recipes were so close to being shell variables and scripts
that the differences were removed in
.I mk .
Making recipes shell scripts had the further advantage that
.I mk
does not have to parse or process the recipes.
The use of special target and prerequisite names
(beginning with a dot) to indicate special actions has been dropped
in favor of a more explicit notion of target
.I attributes .
.SS "Mk is a simple, general purpose tool."
Recent versions of
.I make
(such as
.I nmake )
focus on the issues connected with building software and generally
contain much builtin knowledge about C programming.
.I Mk ,
on the other hand,
is a tool for maintaining file dependencies,
whether they be programs or circuit board descriptions.
It offers general purpose and powerful mechanism for all users,
not just help for programmers.
And it does so by having a simple, comprehensible model of behavior
that a user can predict without having to be an expert.
.NH
References
.LP
|reference_placement
.bp
.NH 2
Appendix 1
.PP
The default variable definitions are:
.P1 0
AS=as
CC=cc
CFLAGS=
FC=f77
FFLAGS=
LDFLAGS=
.P3
LEX=lex
LFLAGS=
NPROC=1
NREP=1
YACC=yacc
YFLAGS=
.P2
The builtin rules are
.P1 0
%.o:	%.c
	$CC $CFLAGS -c $stem.c
%.o:	%.s
	$AS -o $stem.o $stem.s
%.o:	%.f
	$FC $FFLAGS -c $stem.c
.P3
%.o:	%.y
	$YACC $YFLAGS $stem.y &&
	$CC $CFLAGS -c y.tab.c &&
	mv y.tab.o $stem.o; rm y.tab.c
%.o:	%.l
	$LEX $LFLAGS -t $stem.l > $stem.c &&
	$CC $CFLAGS -c $stem.c && rm $stem.c
.P2
The environment for the recipe's shell is augmented by
these variables:
.IP \f(CWalltarget\f1 14
all the targets for this rule.
.IP \f(CWnewprereq\f1
the prerequisites that are more recent than the target.
.IP \f(CWnproc\f1
this is the process slot for this recipe.
It is a number between zero and
.CW $NPROC-1
inclusive.
It is useful for parallel execution on a single CPU machine
on a network.
.IP \f(CWpid\f1
the process id for the
.I mk
invoking this script.
This is useful for communicating with other rules.
.IP \f(CWprereq\f1
all the prerequisites for this target.
This may include prerequisites from several rules.
.IP \f(CWstem,...\f1
the value of
.CW %
in a metarule.
It is null for a non-metarule.
The value of the
.I n th
subexpression in a regular expression metarule is put in the variable
.CW stem\f2n ,
for
.I n <10.
It is null otherwise.
.IP \f(CWtarget\f1
the targets being built for this rule.
.bp
.NH 2
Appendix 2
.LP
The changes from Release 1 to Release 2 were:
.IP [1]
The order of processing the environment variables was changed.
The original hope was that if the environment overrode the values
inside the mkfile, there would be no need for command line variable
assignments.
This didn't work out.
In fact, the chief offender was
.I mk
calling itself recursively (all the variables were now in the environment!).
The processing order is now the same as
.I make .
.IP [2]
The builtin rule for
.I yacc
grammars was tweaked slightly so as to not rely on the Eight Edition
.I yacc
which supports an arbitrary prefix for all the generated files.
.IP [3]
The filenames for the
.CW -w
option were allowed to be separated by blanks and newlines as well as commas.
This allowed the names to be generated conveniently by a backquote expression.
.IP [4]
The
.CW D
attribute was introduced for the real problem of recipes which
update their targets even though they fail.
I resisted this as long as I could;
.I mk
is supposed to create files, not remove them.
.IP [5]
The
.CW N
attribute is a sign of
.I mk 's
increasing dogma.
The problem is rules which have out of date prerequisites but no recipe
to update the target.
If the target is virtual, this is benign and
.I mk
does not complain.
Otherwise, the target is being made as a side-effect or is not being made.
Sometimes, as in the library rules, the former is true.
Other times, the latter is true.
For example,
.P1 \n(I\n(IUu
prog:	a.o b.o c.o
	$CC $CFLAGS -o $target $prereq
%.o:	hdr.h
.P2
If we add a new object to the list (say
.CW d.o )
without creating a corresponding
.CW d.c ,
then
.I mk
used to happily say everything was fine
and the loader would complain about a missing
.CW d.o !
(The reason
.I mk
didn't complain about not knowing how to make
.CW d.o
was because of the
.CW "%.o:hdr.h"
rule.)
I regard allowing a recipe to run without all of its prerequisites made as
a serious error so
.I mk
doesn't any more.
.IP [6]
The
.CW P
attribute was supposed to be in the first release but I couldn't
figure out how to describe the semantics.
The key is that out-of-date-ness and date stamp propagation
are separate issues.
So that even if files are related by a general nontemporal relation,
the date stamps can be propagated in the normal way and allow
proper interaction with targets made with the normal mechanism.
The resulting rule for updating
.CW x.tab.h
in the
.I yacc
example is unexpectedly nice.
.IP [7]
A few small bugs were fixed, the main one being that backquote
expressions were evaluated before rather than after processing
single quotes.
.SP 1
.LP
The changes from Release 2 to Release 3 were:
.IP [1]
Files could be included by
.CW "< filename" .
The syntax is ugly but convenient.
.IP [2]
The implementation of recipe executions was changed when
people I care about started using
.I mk
on System V machines that commonly have ludicrously small
.I exec
limits of 5120 bytes.
This is easily strained by
.I mk ;
the limit has to exceed the size of both recipe and variables.
(The filenames alone for the main X library add up to about 3800 characters!)
Recipes are now given as standard input to
.I sh ,
rather than as an argument.
.IP [3]
.I Mk
originally supported a form of string manipulation for
variable expansions but I discarded it as unnecessary when backquote
expressions were working (more functionality, less code).
When users asked for a more efficient mechanism, I countered with ``It's
just a couple more processes.''
This worked until Rob Pike countered back with the Plan 9 kernel mkfile;
it had eight(!) lines doing simple string manipulation with
.I sed (1).
I borrowed, and then generalized, the mechanism from the System V
.I make .
.IP [4]
Many
.I mk
users wanted to ship their stuff to places without
.I mk .
The problem of converting mkfiles into makefiles can not be done in general.
However, provided the mkfile is fairly straightforward,
.I mk
can generate an explicit makefile which is pretty much right.
.IP [5]
The delimiter for pieces of the environment (from the variable
.CW ENVIRON )
has been changed from
bytes with the 0200 bit set to the specific byte 001.
This brings it into line with other programs and can easily be entered
from a keyboard.

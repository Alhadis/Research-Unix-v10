|make(rp)

|insert(../monk/monk.mac)

|comment(VOL2HEADER)|set_counter(page 325-1)|set_string(page_headers "'''")|set_string(even_headers "'The C Interpreter: A Tutorial for Cin Version 0.18''\f(NIcin\fP'")|set_string(odd_headers "'\f(NIcin\fP''The C Interpreter: A Tutorial for Cin Version 0.18'")|set_string(even_footers "_\\nP__UNIX Papers_")|set_string(odd_footers "'Research Tenth Edition''\\nP'")

|environment(computeroutput;
	verbatim on, file.information, line.spacing 1,
	fill off, size -1, space -1, font cw, blank.lines -1,
	tab.stops "\w'        'u 2u*\w'        'u 3u*\w'        'u 4u*\w'        'u 5u*\w'        'u 6u*\w'        'u 7u*\w'        'u 8u*\w'        'u 9u*\w'        'u 10u*\w'        'u 11u*\w'        'u";

	tab.stops, blank.lines -1, SPACE)

|author(name "T. J. Kowalski", initials TJK, location MH, department 11229,
	extension x2771, room 2C-568)
|document(number 11229-880606-07TMS, file_case 25952,
	work_program 311401-2199)

|author(name "H. H. Goguen", initials HHG, location MH, department 11229,
	extension x2771, room 2C-568)
|document(number 11229-880606-07TMS, file_case 25952,
	work_program 311401-2199)

|author(name "J. J. Puttress", initials JJP, location MH, department 11229,
	extension x7290, room 2C-577)
|document(number 11229-880606-07TMS, file_case 25952,
	work_program 311401-2199)

|date(June 6, 1988)

|title(The C Interpreter:
A Tutorial for Cin Version 0.18)

|begin(abstract)
The C interpreter, |i(cin), is the core of an interactive programming environment for
the C programming language.
We believe that by providing integrated facilities to create,
edit,
browse,
execute,
and debug programs,
this environment should enhance the productivity
of C programmers at |s(AT&T) and improve the quality of their code.
Furthermore,
we feel that |i(cin) provides an easy-to-use instructional environment.
|p
This memorandum is a tutorial guide to help users get started.
Although it does not cover everything,
it provides enough information for most users' daily needs.
Topics discussed include
creating simple programs,
testing individual functions and expressions,
inspecting and modifying variables,
using program views, and setting breakpoints.
|end(abstract)

|titlebox

|style(two_column)

|unnumbered_section(Introduction)

Interactive programming environments are common for |s(LISP),
|reference(Teitelman Masinter Interlisp Programming Environment)
|reference(Sandewall Programming in an Interactive Environment 1978)
|s(PASCAL),
|reference(Delisle Menicosy Schwartz Viewing a Programming Environment)
|reference(Reiss Graphical Program Development with PECAN)
|reference(Brown Sedgewick A System for Algorithm Animation)
|s(PL/C),
|reference(Archer Conway COPE: A Cooperative Programming Environment)
|reference(Teitelbaum Reps The Cornell Program Synthesizer 1981)
and Smalltalk.
|reference(Goldberg Robson Smalltalk-80: The Language and Its Implementation)
|reference(Tesler The Smalltalk Environment)
Some work has been done on various pieces of a C program environment, including
syntax directed editors,
|reference(Horgan Moore Techniques for Improving Language-Based Editors)
interpreters,
|reference(Saber-C 1988)
|reference(Feuer si - An Interpreter for the C Language)
|reference(Ostby A C interpreter)
debuggers,|reference(Adams Muchnick Dbxtool A Window-Based Symbolic Debugger)
|reference(Cargill The Feel of Pi)
and browsers.
|reference(Steffen Interactive Examination of a C Program with Cscope)
|p
|i(Cin) is a C interpreter: an interactive program
for creating, testing, modifying, and debugging a C program,
using directions provided by a user at a terminal.
The program can be a single function in a single file,
or a large collection of files and functions as in System 75.
|reference(System 75: Project Development Environment 1985)
|i(Cin) is part of the Integrated C Programming Environment,
|cw(cens),
currently being developed by Department 11229.
It implements correct full C,
enables rapid prototyping,
permits extensive error checking,
facilitates incremental update,
manages multiple software views,
and
provides a programmable command language.
|p
This tutorial is meant to simplify learning |i(cin).
We suggest that you read this document,
simultaneously using |i(cin) to follow the examples,
then read the manual page in the appendix
while continuing to experiment with |i(cin).
Furthermore,
we strongly recommend completing the exercises.
They both reinforce material covered in the text
and explore material not thoroughly discussed.
|p
This is an introduction and a tutorial.
For that reason,
no attempt is made to cover
more than a part of the facilities that |i(cin) offers
(although this fraction includes the most useful and frequently needed parts).
Explanations of the C language and the |s(UNIX)|sp(registered) system
are beyond the scope of this paper.
We assume that you know how to log on to a |s(UNIX) system
and how to express algorithms in C.
Information on both the |s(UNIX) system
and C is readily available from a variety of sources.
|reference(Kernighan Pike Prentice-Hall)
|reference(Kernighan Ritchie Prentice-Hall)

|section(Getting Started)

We'll assume you have logged in to a |s(UNIX) system
and have just received the shell prompt, e.g. ``$''.
The easiest way to start |i(cin) is to type

|begin(computeroutput)
|i($ )cin -i
|i(cin>)
|end(computeroutput)

You are now ready to go
|sp(Em dash) |i(cin) is waiting for your instructions.
|i(Cin) shows this by echoing a

|begin(computeroutput)
|i(cin>)
|end(computeroutput)

to your terminal.
Throughout this tutorial, responses from |i(cin) are in |i(italics).
|p
This section shows how to use the interactive mode of |i(cin),
that is, a mode where we can enter and execute programs interactively.
Later we'll talk about changing and debugging programs.
As our first problem,
let's leap the hurdle of creating, compiling, loading, and running
a program that prints ``hello, world'' on our terminal.
Though this is a painful task when we use the normal editor, compiler,
loader, and shell to run the program,
it's easy in |i(cin).
|p
When |i(cin) is started with the |cw(-i) option,
it is like working with a blank piece
of paper |sp(Em dash) there is no program or data present.
They must be supplied by the person using |i(cin).
The program,
with its data,
can be entered interactively
or read into |i(cin) from a file.
We will start by typing in a program
and return shortly to the reading of files.
|p
First a bit of terminology.
In |i(cin) jargon,
the program currently worked on is said to be
``kept in the current |cw(view)''.
Think of the |cw(view) as a work space,
if you like,
or simply as the information that you are going to be
using.
The |cw(view) is like a piece of paper,
on which we will write things,
then change some of them,
and finally file the whole thing away for another day.
|p
The user tells |i(cin) what to do to the |cw(view)
by typing C expressions and declarations.
C expressions are executed immediately,
while declarations are stored in the |cw(view).
When an expression is executed,
|i(cin) uses both local and global variables.
|p
Now we're ready to solve our problem.
The easiest way is to write a C expression that calls the |cw(printf)
library routine.

|begin(computeroutput)
|i(cin>) printf("hello, world|sp(\)n");
|i(hello, world
cin>)
|end(computeroutput)

|p
Another way to solve our problem is to write a declaration for a |cw(main) function
and call it using a C expression.

|begin(computeroutput)
|i(cin>) void main()
{
  printf("hello, world|sp(\)n");
}
|i(cin>) main();
|i(hello, world
cin>)
|end(computeroutput)

|p
We draw your attention to the |cw(void main) declaration.
When you're in |i(cin)'s interactive mode,
you need to declare a return type for each function.
This allows |i(cin)'s interactive mode
to tell the difference between an expression calling |cw(main)
and a declaration of |cw(main).
Don't panic!
You don't have to change all your C programs.
This is only true when typing directly to interactive mode.
The normal default declaration, |cw(int), still applies when files are read in.
We could have designed |i(cin) to work like the
C interpreters referenced in the introduction, which require you to
type a special character to enter and leave a command mode,
but we felt that declaring a return type was more consistent with C syntax.
|p
You also saw that |i(cin) prompts only for a C expression
or a declaration.
Thus, there are no prompts for the function body of |cw(main).
This silence is preferred by experienced users,
but sometimes disturbs beginners.
|p
Finally finished with our problem, we'd like to return to our shell.
Exit |i(cin) by typing

|begin(computeroutput)
|i(cin>) cin_quit();
|i($)
|end(computeroutput)

or by typing the end-of-file character (hold the |s(CTRL) key down
and press |cw(d)).

|section(I Seemed To Have Confused It)

If you didn't get a ``hello, world'',
then |i(cin) is confused by the directions it got.
Let's look at some possible problems that may have occurred
and how to correct them.
|p
One possibility is

|begin(computeroutput)
|i($) cin -i
|i(cin: not found)
|i($)
|end(computeroutput)

The C interpreter, |i(cin), is not available on your system in the
directories specified by your |s(PATH) shell variable.
Please contact your system administrator.
If your system has |s(EXPTOOLS),
the administrator can get |i(cin) from Alan Hastings.
Otherwise,
your administrator can ask for |i(cin) by sending mail to |cw(research!frodo),
including his/her name,
department number,
telephone number,
location,
and room number.
|p
Another possibility is

|begin(computeroutput)
|i($) cin -i
|i(cin>) printff("hello, world|sp(\)n");
|i(interactive mode: 1: warning: 'printff' undefined
breakpoint in function 'cin_system'
 at line 1 of file 'interactive mode'
cin>)
|end(computeroutput)

|cw(printf) was misspelled.
Whenever |i(cin) encounters an undefined function or variable,
it recursively calls a breakpoint function, |cw(cin_system),
which places you in |i(cin)'s interactive mode waiting for input.
The simplest corrective action is to return and ignore the problem.
We can do this by typing

|begin(computeroutput)
|i(cin>) cin_return();
|i[interactive mode: 2: warning:
 call of undefined function (returning 0L)
cin>]
|end(computeroutput)

or by typing the end-of-file character.
This acts as if you decided to define a function, |cw(printff),
that returns a |cw(long) zero.
A future release of |i(cin) will allow you to modify the prompt to show
the number of recursive calls to |cw(cin_system).
Later we'll talk about additional corrective actions and
other predefined |cw(cin_) functions and variables.
|p
Yet another possibility is

|begin(computeroutput)
|i($) cin -i
|i(cin>) printf("hello, world|sp(\)n")
|end(computeroutput)

Neither ``hello, world'', nor the |cw(|i(cin>)) prompt appears.
You probably forgot to type the last semicolon (|cw(;)).
Even experienced users forget the terminating semicolon (|cw(;))
sometimes.
If |i(cin) seems to be ignoring you,

|begin(computeroutput)
|i($) cin -i
|i(cin>) printf("hello, world|sp(\)n);
|i(interactive mode: 1: newline in string)
|end(computeroutput)

type an extra line with just a semicolon (|cw(;)) on it,
or in some rare cases

|begin(computeroutput)
|i($) cin -i
|i(cin>) void main()
{
	printf("hello, world|sp(\)n);
}
|i(interactive mode: 3: newline in string
interactive mode: 4: syntax error
 in parameter list of function call)
|end(computeroutput)

an extra line with just a semicolon (|cw(;))
followed by an extra line with just a closing curly brace (|cw(})) on it.
Isn't error recovery wonderful!
The next release of |i(cin) will be more friendlier.
|p
Throughout the rest of the tutorial,
we will assume that our typing is perfect.
We use the |cw(samuel)|reference(Puttress 1986 browser)
editor to remember C statements and declarations in a scratch pad,
sending them to |i(cin) whenever we are happy with the syntax.
However, you can also remember and send
C statements and declarations with |cw(sam), |cw(jim), |cw(emacs), or |cw(inedit).

|subsection(Exercise 1)

Enter |i(cin) and use the formula |e<degree~ F ~=~ ( 9 / 5 ) ~*~ degree~ C ~+~ 32>
to create a conversion table from centigrade or Celsius temperatures to Fahrenheit
for 20|sp(degree) C to 30|sp(degree) C.
Do this exercise in two different ways:
|begin(number_list)
|item Write a C expression that calls the |cw(printf) library routine.
|item Declare a |cw(main) function and run it in |i(cin).
|end(number_list)

|section(Writing A Small Program)

Our next problem is to write a few routines to
print algebraic expression trees.
Let's start by declaring the template |cw(node) for our tree.

|begin(computeroutput)
|i(cin>) struct node {
  struct node *left;
  struct node *right;
  char *value;
  int type;
};
|i(cin>)
|end(computeroutput)

|p
The |cw(left) and |cw(right) fields are pointers to other |cw(node)s.
The |cw(value) field holds a character string representing
the operator, variable, or constant associated with the node.
The |cw(type) field shows the kind of operator,
variable, or constant held in |cw(value) field.
The allowable types are:

|begin(computeroutput)
|i(cin>) #define	ADD	1
#define	SUB	2
#define	MUL	3
#define	DIV	4
#define	EXPT	5
#define	UMINUS	6
#define	CONST	7
#define	VAR	8
|end(computeroutput)

|p
We'd like to pause and talk about prompts once again.
You're probably puzzled why there was only one |i(cin>) for the above set
of pre-processor statements.
Well,
in the 0.18 release |i(cin) prompts only for a C expression
or a declaration, and C pre-processor statements are a little different.
We think this is a botch and will change it in the next release,
but until then it's a feature.|dagnote(Features are
bugs that are not yet fixed.)
|p
Now that we have a data structure,
let's write a |cw(prt) routine to display the tree
in the format normally used for an equation.
This requires an in-order walk; in other words |sp(Em dash)
go left as far as you can,
print the node,
then go right as far as you can.

|begin(computeroutput)
|i(cin>) void
prt(n)
struct node *n;
{
  switch(n->type) {

  default:
    prt(n->left);
    printf("%s ", n->value);
    prt(n->right);
    break;

  case UMINUS:
    printf("%s ", n->value);
    prt(n->right);
    break;

  case CONST:
  case VAR:
    printf("%s ", n->value);
    break;

  }
}
|i(cin>)
|end(computeroutput)

|p
A powerful feature of |i(cin) enables a routine
to be tested as soon it is written.
This way you can make sure it does what you want
before incorporating it into a large program.
|i(Cin) also takes care of allocating and deallocating
data structures for testing.
|p
Let's test the |cw(prt) function.
First we'll include the standard I/O header file
so we can turn off output buffering.

|begin(computeroutput)
|i(cin>) #include <stdio.h>
|i(cin>) setbuf(stdout, NULL);
|i(cin>) setbuf(stderr, NULL);
|i(cin>)
|end(computeroutput)

Next we let |i(cin) allocate storage space for a sample node,
|cw(a1),
and set it to hold a constant, |cw(CONST), with a value of |cw(17).
Finally,
we call |cw(prt) and see it print |cw(17).

|begin(computeroutput)
|i(cin>) struct node a1;
|i(cin>) a1.type = CONST;
|i(cin>) a1.value = "17";
|i(cin>) prt(&a1);
|i(17 cin>)
|end(computeroutput)

For constants,
|cw(left) and |cw(right) are unused.
|p
It's likely that we'll want to save our work for later use.
A simple and easy way to do this
is to save an image of our program and data definitions as an executable file.
Let's do this by calling the predefined function |cw(cin_dump)
with the name of a file, |cw(./saved).

|begin(computeroutput)
|i(cin>) cin_dump("./saved");
|i(cin>) cin_quit();
|i($)
|end(computeroutput)

Any time afterwards we can invoke our saved executable and use the program
and data definitions we have just entered.

|begin(computeroutput)
|i($) ./saved
|i(cin>) prt(&a1);
|i(17 cin>)
|end(computeroutput)

This feature is useful for customizing our |i(cin) environment,
but in the 0.18 release of |i(cin) there is no way to print out
the definition of a given function.
We plan to enhance |i(cin) to do this in the future,
but for now we recommend using an editor to keep your functions
in files.
|p
Thus, we need to learn how to
load source or object files into |i(cin).
We'll assume you've returned to your shell,
placed the definition for |cw(node)
and its |cw(#define)s in |cw(node.h),
and placed |cw(#include "node.h")
along with the definition for |cw(prt) in |cw(prt.c).
|p
Now let's go back to |i(cin) and load in the |cw(prt) function.

|begin(computeroutput)
|i($) cin -i
|i(cin>) cin_load("prt.c");
|i(cin>)
|end(computeroutput)

|p
When |i(cin) loaded |cw(prt.c), it placed
the structure definition for |cw(node),
the |cw(#define)s,
and the definition for |cw(prt)
in a |cw(view) called |cw(prt.c).
A |cw(view) is the internal representation of a C source file |sp(Em dash)
|cw(#define)s, static variables and functions, and global declarations.
Outside |i(cin), the functions and data of a C program are packaged in a file;
inside |i(cin) they are contained in a |cw(view).
Earlier, when we typed declarations directly into the interpreter,
they were placed into a default |cw(view) called ``interactive mode''.
To use the declarations,
we need to be in the |cw(view) where they are stored.
So if we are sure we defined something, but |i(cin) can't find it,
we have probably gotten into the wrong |cw(view).
We can see what |cw(view)s are available by using the predefined function
|cw(cin_views), and we can change our |cw(view) by using |cw(cin_view).

|begin(computeroutput)
|i(cin>) cin_views();
|i(   prt.c
 |sp(*) interactive mode
cin>) cin_view("prt.c");
|i(cin>) cin_views();
|i( |sp(*) prt.c
   interactive mode
cin>)
|end(computeroutput)

|p
We have just seen that |cw(cin_views) marks our current |cw(view)
with an asterisk (|cw(*)).
Do not confuse the ``interactive mode'' session,
which is entered by typing |cw(cin -i),
with the default interactive mode |cw(view).
In interactive mode,
you may access any |cw(view)
and enter C expressions or declarations in the context of that |cw(view).
The interactive mode |cw(view) is simply the default |cw(view),
used for storing C declarations not associated with an external file.
|p
Let's use the declaration of |cw(struct node)
and the |cw(#define) for |cw(CONST) in the current or |cw(prt.c) |cw(view)
to initialize a |cw(struct node a1).

|begin(computeroutput)
|i(cin>) struct node a1;
|i(cin>) a1.type = CONST;
|i(cin>) a1.value = "17";
|i(cin>)
|end(computeroutput)

|p
Although we said our typing would be prefect,|dagnote(as you can see it's not)
let's see what happens if we make the common mistake of passing
a structure instead of a pointer to the structure.

|begin(computeroutput)
|i(cin>) prt(a1);
|i(prt.c: 7: null pointer access
breakpoint in function 'prt' at line 7 of file 'prt.c'
cin>)
|end(computeroutput)

|p
At this point we can escape to our shell and print a numbered listing
of |cw(prt.c).

|begin(computeroutput)
|i(cin>) system("pr -n -t prt.c");
|i[ 1	#include "node.h"
 2
 3	void
 4	prt(n)
 5	struct node |sp(*)n;
 6	{
 7	  switch(n|cw(->)type) {
 8
 9	  default:
10	    prt(n|cw(->)left);
11	    printf("%s ", n|cw(->)value);
12	    prt(n|cw(->)right);
13	    break;
14
15	  case UMINUS:
16	    printf("%s ", n|cw(->)value);
17	    prt(n|cw(->)right);
18	    break;
19
20	  case CONST:
21	  case VAR:
22	    printf("%s ", n|cw(->)value);
23	    break;
24
25	  }
26	}
cin>]
|end(computeroutput)

|p
By a quick process of elimination,
we realize that
|cw(n) must be our problem,
because line 7 only has one pointer variable.
When you are in a |i(cin) breakpoint function,
|i(cin) computes the value of expressions in the current execution context
|sp(Em dash) an expression is evaluated just as though
it were placed in the program source at the current point of execution.
Thus, we can check to see if |cw(n) is zero by printing its value.

|begin(computeroutput)
|i(cin>) printf("%d|sp(\)n", n);
|i(0)
|i(cin>)
|end(computeroutput)

|p
If we failed to notice our ``passing
a structure instead of a pointer to the structure'' typo,
we might think that |cw(prt) got into trouble because it called itself.
We can check this by using the predefined function |cw(cin_where).

|begin(computeroutput)
|i(cin>) cin_where();
|i{cin_system()   [prt.c:1]
prt(n = 0x0)   [prt.c:7]
cin_system()   [interactive mode:32]
cin>}
|end(computeroutput)

This tells us that we are currently in |i(cin)'s breakpoint function,
|cw(cin_system),
looking at a call of |cw(prt) that was made from
interactive mode.
Because there are not two calls to |cw(prt),
we are assured it didn't recursively call itself.
|p
As you can see from the reference to line number 32,
the authors need to struggle further with what line numbers should mean in
interactive mode.
In particular, |cw(cin_where) and |cw(cin_break),
discussed later,
don't seem to use sensible line numbers for functions defined in interactive mode.
|p
Let's fix the problem with |cw(n) by modifying its value to be
|cw(&a1) and allowing the program to continue.

|begin(computeroutput)
|i(cin>) n = &a1;
|i(cin>) cin_return();
|i(17 cin>)
|end(computeroutput)

Most debuggers don't permit you to continue after a serious error.
Whenever possible,
|i(cin) allows you to recover from an error
once you've taken corrective action.
If we don't want to take corrective action,
we can remove all calls of |i(cin)'s breakpoint function by calling |cw(exit).

|begin(computeroutput)
|i(cin>) exit(0);
|i(cin>)
|end(computeroutput)

Sometimes there is no way to fix the problem
and all we can do is |cw(cin_quit).
|p
Now let's define a function that creates a node,
placing the function in |cw(create.c).
We can enter our favorite editor by

|begin(computeroutput)
|i(cin>) system("${EDITOR-/bin/ed} foo.c");
|i(?foo.c)
|end(computeroutput)

and printing out our file

|begin(computeroutput)
|i(cin>) system("pr -n -t create.c");
|i[ 1	#include <stdio.h>
 2	#include "node.h"
 3
 4	struct node |sp(*)
 5	create(left, right, type, value)
 6	struct node |sp(*)left, |sp(*)right;
 7	char |sp(*)value;
 8	{
 9	  struct node |sp(*)n;
10	  struct node |sp(*)memory();
11
12	  if (n = memory(
13	   sizeof(struct node))) {
14	    n|cw(->)left = left;
15	    n|cw(->)right = right;
16	    n|cw(->)type = type;
17	    n|cw(->)value = value;
18	    return(n);
19	  }
20
21	  fprintf(stderr,
22	   "No memory to create a node|sp(\)n");
23	  exit(1);
24	}
cin>] cin_load("create.c");
|i[create.c: 8: warning: function 'create'
has implicit return and 'return e;' (line 18)
cin>]
|end(computeroutput)

|p
The error message warns us that the function |cw(create),
which is declared starting on line 8,
ends without returning a value.
This is fine, because the function |cw(exit)
would never let us get to the end of the routine.
|p
Now let's change our |cw(view) to the |cw(create.c) declarations,
make a few nodes, and print the expression tree
for |cw[2 * (17 - x)].
We can represent the expression tree graphically as

|begin(here)
|begin(picture)
scale=100
box invis ht 96 wid 144 with .sw at 0,0
"\fR\s10\&|sp(-) (t1)\f1\s0" at 122,42
"\fR\s10\&x (t3)\f1\s0" at 160,-10
"\fR\s10\&17 (t2)\f1\s0" at 80,-10
line from 104,40 to 144,0
line from 104,40 to 64,0
"\fR\s10\&2 (t5)\f1\s0" at 24,26
"\fR\s10\&|sp(*) (t4)\f1\s0" at 80,86
line from 64,80 to 104,40
line from 64,80 to 24,40
|end(picture)
|end(here)

where the labels in parentheses are the variable names for the |cw(struct node)
pointers
and the other symbols are the |cw(value)s for the |cw(node)s.

|begin(computeroutput)
|i(cin>) cin_view("create.c");
|i(cin>) struct node *t1, *t2, *t3;
|i(cin>) t3 = create((struct node *)0,
(struct node *)0, VAR, "x");
|i(create.c: 12: warning: 'memory' undefined
breakpoint in function 'create'
at line 12 of file 'create.c'
cin>)
|end(computeroutput)

|p
A common programming practice is to leave ``stubs'',
or functions to be defined at a later date.
|i(Cin) will stop when it encounters functions or variables that are not defined,
permitting you to define them.
Here |i(cin) has stopped because it can't find a definition for |cw(memory).

|begin(computeroutput)
|i(cin>) struct node *
memory(n)
unsigned n;
{
  struct node *malloc();
  return malloc(n);
}
|i(cin>) cin_return();
|i(cin>) t2 = create((struct node *)0,
(struct node *)0, CONST, "17");
|i(cin>) t1 = create(t2, t3, ADD, "+");
|i(cin>) struct node *t4, *t5;
|i(cin>) t5 = create((struct node *)0,
(struct node *)0, CONST, "2");
|i(cin>) t4 = create(t5, t1, MUL, "*");
|i(cin>) prt(t4);
|i(2 |sp(*) 17 + x cin>)
|end(computeroutput)

We'd like to emphasize that declarations can be mixed
with C expressions in interactive mode.

|subsection(Exercise 2)

Use |i(cin) and your favorite editor to enhance the |cw(prt) function
by adding parentheses to the output that show the nesting levels;
e.g., |cw<prt(t4)> should output |cw<( 2 * ( 17 + x ) )>.

|section(Debugging With Breakpoints)

We've already seen that any legal C expression can be used
when |i(cin) enters the breakpoint function,
but we haven't seen how to force |i(cin)
to enter a breakpoint function.
The easiest way is to use your editor and insert calls to the
predefined function |cw(cin_system) into your program.

|begin(computeroutput)
1	hello()
2	{
3	  int i;
4	  i = 17;
5	  printf("hello, %d", i);
6	  cin_system();
7	  printf("world %d times|sp(\)n", i);
8	}
|end(computeroutput)

|p
So if we were to load in |cw(hello.c) and run it:

|begin(computeroutput)
cin> |i($) cin -i
|i(cin>) cin_load("hello.c");
|i(cin>) hello();
|i(breakpoint in function 'hello' at line 6 of file 'hello.c'
hello, 17cin>) i = 3;
|i(cin>) cin_return();
|i(world 3 times
cin>)
|end(computeroutput)

We'd be able to change the value of |cw(i),
or anything else, for that matter,
to whatever we want.
This is like placing |cw(printf)'s in our code that allow us
to print or modify anything.
|p
We can obtain a less permanent breakpoint using the predefined function
|cw(cin_break), and remove it using |cw(cin_unbreak).
Let's return to the original |cw(prt) routine contained in |cw(prt.c),
assert a breakpoint on line 7,
and run it with |cw(t4).
Because we've already debugged |cw(create) and |cw(memory),
we can use the C compiler to create object files for them,
continuing to debug |cw(prt) in source form.
By mixing object files with source files, we get the speed of compiled code
for functions that are debugged,
combined with the error checking of interpreted code
for functions we are testing.

|begin(computeroutput)
|i($) cc -O -c create.c memory.c
|i(create.c:
memory.c:
$) cin -i prt.c create.o memory.o
|i(cin>) cin_view("prt.c");
|i(cin>) struct node *t1, *t2, *t3;
|i(cin>) struct node *create();
|i(cin>) #include <stdio.h>
|i(cin>) t3 = create((struct node *)0,
(struct node *)0, VAR, "x");
|i(cin>) t2 = create((struct node *)0,
(struct node *)0, CONST, "17");
|i(cin>) t1 = create(t2, t3, ADD, "+");
|i(cin>) struct node *t4, *t5;
|i(cin>) t5 = create((struct node *)0,
(struct node *)0, CONST, "2");
|i(cin>) t4 = create(t5, t1, MUL, "*");
|i(cin>)
|end(computeroutput)

|p
|cw(Cin_break) and |cw(cin_unbreak) both require us to be in the
|cw(view) where the function is defined and restrict us to functions
that are being interpreted.
We plan to enhance |i(cin) to remove these restrictions in the future.

|begin(computeroutput)
|i(cin>) cin_break("prt", 7);
|i(breakpoint in function prt at line 7
cin>) prt(t4);
|i(breakpoint in function 'prt' at line 7 of file 'prt.c'
cin>) cin_return();
|i(breakpoint in function 'prt' at line 7 of file 'prt.c'
cin>) cin_return();
|i(breakpoint in function 'prt' at line 7 of file 'prt.c'
2 |sp(*) cin>) cin_where();
|i{cin_system()   [prt.c:1]
prt(n = 0x9382C)   [prt.c:7]
prt(n = 0x93854)   [prt.c:12]
cin_system()   [stdio.h:50]
cin>} cin_unbreak("prt", 7);
|i(breakpoint removed from line 7 in prt
cin>) cin_return();
|i(17 + x cin>) cin_quit();
|i($)
|end(computeroutput)

We plan to enhance |i(cin) by adding another argument to |cw(cin_break).
This argument will be a string that is executed when the breakpoint function
is entered.
This will let us set breakpoints that remove themselves,
print interesting things,
or even set other breakpoints.

|section(Program Equals Data)

A common trick of interpreters,
especially in the |s(LISP) world,
is to use the same representation for data handled by a program
and the program text itself.
|p
As our last programming task in this tutorial,
let's write a simple desk calculator.
To accomplish this,
we need to read a string from the user,
wrap it in a |cw(printf) C expression,
and pass it to the predefined function |cw(cin_eval).

|begin(computeroutput)
|i($) cin -i -lm
|i(cin>) #include <stdio.h>
|i(cin>) #include <math.h>
|i(cin>) char buf[1024], buf2[1024];
|i(cin>) while(gets(buf) != NULL) {
  sprintf(buf2,
   "printf(|sp(\)"%%g|sp(\)|sp(\)n|sp(\)",(double)(%s));",
   buf);
  cin_eval(buf2);
}
sqrt(2.0) * sqrt(2.0)
|i(2)
|end(computeroutput)

In writing the |cw(printf) expression shown above,
the trickiest part was ensuring that the argument would be appropriate
to the |cw(g) format.
That's what the |cw<(double)> accomplished.
This example is also plagued with getting the right number of
|sp(\)s and |cw(%)s.

|subsection(Exercise 3)

Enhance the desk calculator C expression to remember the
last value as |cw(t)
and make it into a shell file that can be easily executed.

|section(Bugs)

|center(You find 'em.
We squash 'em!)
|p
The interactive mode is new with the 0.18 release of |i(cin).
It's hard to anticipate all the errors you may encounter,
and because of this we need your help to improve our tool.
Thus, we welcome bug reports sent to research!frodo.
Please include a short program along with the terminal input and
output that creates the |i(cin) problem.

|section(Summary Of Predefined Functions)

Several pre-defined functions are provided for the user,
where |i(name), |i(string), |i(file), |i(line), |i(func), |i(func_mod),
and |i(func_ref) are declared as:

|begin(computeroutput)
char *|i(name), *|i(string), *|i(file);
int |i(line);
long (*|i(func))(), (*|i(func_mod))(), (*|i(func_ref))();
|end(computeroutput)

|blank_space(-1)
|begin(computeroutput)
void cin_bind(|i(name), |i(func))
|end(computeroutput)
|blank_space(-1)
Set an alias of |i(name) for the function |i(func).

|begin(computeroutput)
int cin_break(|i(name), |i(line))
|end(computeroutput)
|blank_space(-1)
Set a breakpoint in function |i(name) at line number |i(line) in the current
|cw(view).
Returns 0 if breakpoint cannot be set.

|begin(computeroutput)
int cin_dump(|i(name))
|end(computeroutput)
|blank_space(-1)
Create an |cw(a.out) and place it in the file |i(name).
Returns 0 if |i(name) cannot be created.

|begin(computeroutput)
int cin_eval(|i(string))
|end(computeroutput)
|blank_space(-1)
Execute the C statement |i(string)
as if it were present in the program where the |cw(cin_eval) is located.
Returns 0 if |i(string) could not be parsed.

|begin(computeroutput)
int cin_load(|i(file))
|end(computeroutput)
|blank_space(-1)
Load |i(file) into a new |cw(view),
using the standard file-naming conventions for |i(cc)(1).
Returns 0 if |i(file) can not be loaded.

|begin(computeroutput)
void cin_return()
|end(computeroutput)
|blank_space(-1)
Return from a breakpoint.

|begin(computeroutput)
int cin_spy(|i(name), |i(func_mod), |i(func_ref))
|end(computeroutput)
|blank_space(-1)
Call the function |i(func_mod) whenever the variable |i(name) is modified.
Call the function |i(func_ref) whenever the variable |i(name) is referenced.
Either |i(func_mod) or |i(func_ref) can be |cw<(long (*)())0>.
Returns 0 if there are no spies active.

|begin(computeroutput)
void cin_system()
|end(computeroutput)
|blank_space(-1)
Enter ``interactive mode''.

|begin(computeroutput)
int cin_unbreak(|i(name), |i(line))
|end(computeroutput)
|blank_space(-1)
Remove a breakpoint in function |i(name) at line number |i(line) in the current
|cw(view).
Returns 0 if the breakpoint was not set.

|begin(computeroutput)
int cin_view(|i(name))
|end(computeroutput)
|blank_space(-1)
Change the current |cw(view) to |i(name).
Returns 0 if the |cw(view) was not found.

|begin(computeroutput)
void cin_views()
|end(computeroutput)
|blank_space(-1)
Print the available |cw(view)s.
The current |cw(view) is marked with an asterisk (|cw(*)).

|begin(computeroutput)
void cin_whatis(|i(name))
|end(computeroutput)
|blank_space(-1)
Print the type of the variable |i(name).

|begin(computeroutput)
void cin_where()
|end(computeroutput)
|blank_space(-1)
Print the trace of subroutine calls.

|begin(computeroutput)
void cin_quit()
|end(computeroutput)
|blank_space(-1)
Exit |i(cin).

|section(Summary Of Predefined Variables)

Several predefined variables are provided for the user.

|begin(computeroutput)
extern char *cin_libpath
|end(computeroutput)
|blank_space(-1)
A colon (|cw(:)) -separated list of libraries to search for undefined routines
(defaults to libraries specified on the command line and
``|cw(-lc)'').

|begin(computeroutput)
extern char *cin_prompt
|end(computeroutput)
|blank_space(-1)
The ``interactive mode'' prompt (default ``|cw(cin>) '').

|section(Answers)

|subsection(Answer 1)

|blank_space(-1)
|begin(computeroutput)
|i($) cin -i
|i(cin>) int i;
|i(cin>) for (i = 20; i <= 30; ++i)
 printf("%d %g|sp(\)n",
  i, 9. / 5. * i + 32.);
|i(20 68
21 69.8
22 71.6
23 73.4
24 75.2
25 77
26 78.8
27 80.6
28 82.4
29 84.2
30 86
cin>) void main()
{
  int i;

  for (i = 20; i <= 30; ++i)
   printf("%d %g|sp(\)n",
     i, 9. / 5. * i + 32.);
}
|i(cin>) main();
|i(20 68
21 69.8
22 71.6
23 73.4
24 75.2
25 77
26 78.8
27 80.6
28 82.4
29 84.2
30 86
cin>) cin_quit();
|i($)
|end(computeroutput)
|blank_space(-1)

|subsection(Answer 2)

|blank_space(-1)
|begin(computeroutput)
void
prt(n)
struct node *n;
{
  switch(n->type) {

  default:
    printf("( ");
    prt(n->left);
    printf("%s ", n->value);
    prt(n->right);
    printf(") ");
    break;

  case UMINUS:
    printf("%s ", n->value);
    prt(n->right);
    break;

  case CONST:
  case VAR:
    printf("%s ", n->value);
    break;

  }
}
|end(computeroutput)
|blank_space(-1)

|subsection(Answer 3)

|blank_space(-1)
|begin(computeroutput)
(echo 'extern char *cin_prompt;
cin_prompt="";
#include <stdio.h>
#include <math.h>
char buf[1024], buf2[1024];
double t;
while(gets(buf) != NULL) {
  sprintf(buf2,
   "printf(|sp(\)"%%g|sp(\)|sp(\)n|sp(\)",t=(double)(%s));",
   buf);
    cin_eval(buf2);
}'; cat ) | cin -i
|end(computeroutput)
|blank_space(-1)

|section(Acknowledgements)

We thank Brian Kernighan
for permission to draw inspiration and borrow phrases from his editor tutorial.
We also thank
Sumit Bandyopadhyay,
Bill Carpenter,
Paul De Bra,
Alan Hastings,
Jonathan Helfman,
Andrew Hume,
Leonard Kasday,
Brian Kernighan,
Tom Kirk,
David Korn,
Lisa Kowalski,
Richard Maus,
Lawrence Mayka,
Doug McIlroy,
Marcel Meth,
Eric Muehrcke,
Jishnu Mukerji,
Rob Murray,
Sharon Peeters,
Mike Riley,
Larry Satz,
Judy Schmidt,
Timothy Schroeder,
Jonathan Shapiro,
Joe Steffen,
Andy Tripp,
Tom Walsh,
William Wetzel,
and
Myron Wish
for useful comments on previous versions of this paper.

|comment<
|style(one_column)

|signature(MH-11229-TJK/HHG/JJP-tjk/hhg/jjp)
>

|comment<
|notation(Att.
References)

|other
>

|reference_placement

|keywords(Rapid Prototyping,
Incremental Update,
C Programming)

|comment<
|mercury(cmp, elc)

|att(is_release yes)

|distribute_complete_memo(Executive Directors 112
Directors 112
Department Heads 1122
Sumit Bandyopadhyay
Bill Carpenter
Paul De Bra
Alan Hastings
Jonathan Helfman
Yean-Ming Huang
Andrew Hume
Leonard Kasday
Brian Kernighan
Tom Kirk
David Korn
Lisa Kowalski
Richard Maus
Lawrence Mayka
Doug McIlroy
Marcel Meth
Eric Muerhcke
Jishnu Mukerji
Rob Murray
Sharon Peeters
Mike Riley
Larry Satz
Judy Schmidt
Timothy Schroeder
Jonathan Shapiro
Joe Steffen
Andy Tripp
Tom Walsh
William Wetzel)

|distribute_cover_sheet(A. A. Penzias
1122 MTS
Miguel Abdo
Tom Beattie
Jim Burnash
Tom Cargill
John Carter
Craig Cleaveland
Don deCourcelle
Helen Diamantidis
Laura Eaves
Faiq Fazal
Tom Foregger
Carol Franklin
Narain Gehani
Richard Greer
John Gregg
Jim Grenier
Inars Gruntals
Roy Harkness
Kurt Haserodt
Alan Hewett
Viet Hoang
James Holmes
Mike Hudson
Douglas Johnston
Jon Kaplowitz
Kevin Kinnear
Peter Kirslis
Balachander Krishnamurthy
Ajoy Kumar
David Lubinsky
Robert Lyons
John Malleo-Roach
Pat McGowan
Al McPherson
Douglas Miller
Datta Miruke
John Mocenigo
Dave Neal
Eric Olson
Joseph Patterson
Virginia Piccininni
Thomas Pisciotta
David Potter
Demetri Prountzos
Benjamin Reytblat
Ernie Rice
James Rowland
Ed Schan
William Schell
Carl Seaquist
Steve Shepherd
John Sherman
Tyrone Shiu
Les Shupe
David Smull
G. Mark Stewart
Art Storm
Rich Struse
Mark Swartz
Rick Thomas
Tim Thompson
Peter Ting
Gregg Vesonder
Yu-Lien Yen
John Young
Avi Zahavi)

|add_totals(other 4)

|cover_sheet
>

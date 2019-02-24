.EQ
delim ##
.EN
.DS C
\f3 Twig: A Brief Description\f1
.DE
.sp 2 ex
.PP
Twig is a language for processing trees based on the
algorithms described above.  A twig program consists of a set
of pattern-action rules together with associated declarations.  Patterns
describe trees to be matched.  Actions calculate costs, perform tree
manipulations and other functions such as emitting code.
A twig program is translated by the twig
preprocessor into subroutines and tables in the host language which is C.
.PP
There are many ways to represent trees and costs.  To give the user more
choice in representation, twig treats them as abstract data
types (ADT).  Its manipulation of trees and costs are strictly performed
through calls to subroutines provided by the user.
.sp 1ex
.SH
Rules
.PP
The fundamental unit of a twig program is a rule.
.sp 1ex
.ti 1.5in
\f2label \f3: \f2tree_pattern \f3[ \f2cost \f3] [ = \f2action \f3]\f1
.sp 1ex
.PP
Intuitively, the pattern is used to match a subtree.  The \f2cost\f1 code
fragment is then evaluated.  The resulting cost is recorded by
the matcher for use in dynamic programming.  The \f2action\f1 is executed
if the rule is part of the least cost cover of the tree.
.PP
If the \f2cost\f1 part is missing, Twig will insert default code
that returns the special value, DEFAULT_COST.
A missing \f2action\f1 part indicates that no action will be taken when a
match is found.
.SH
Tree Patterns
.PP
Tree patterns are specified
in prefix parenthesized form and can be described by the
following BNF:
.DS
.na
.nf
\f2tree_pattern \(-> node_id | label_id
tree_pattern \(-> node_id (tree_list)
tree_list \(-> tree,tree_list | tree
.DE
.ad
.fi
For example, the tree,
.DS
.sp 1in
.DE
will be written in twig as
.sp 1ex
.DS
.na
.nf
\f3oper(leftOp,rightOp)\f1
.DE
.ad
.fi
.PP
There are two types of symbols: \f2node_id\f1s and \f2label_id\f1s.
\f2Node_id\f1s are used to denote internal nodes and leaves.
\f2Label_id\f1s label tree patterns and are analogous to
non-terminals in context free grammars.
For example, the following twig rules without their action parts describe
simple expression trees with the \f3plus\f1 operator.
.DS
\f3
.na
.nf
expr: plus(expr, expr)
expr: identifier
expr: constant
.DE
.ad
.fi
\f3Identifier\f1 and \f3constant\f1 are \f2node_id\f1s
representing leaves, and
\f2plus\f1 is a \f2node_id\f1s representing an internal node whereas
\f3expr\f1 is a
\f2label_id\f1.  Leaves of patterns that are \f2label_id\f1s are called
\f2labelled leaves\f1.
In the first rule, both leaves of the pattern are labelled.
.PP
Twig assigns an integer to each node symbol and label symbol.  These
integers are used by the twig pattern matcher as encodings for the
symbols.
As the matcher traverses the
tree, a user supplied subroutine is called to provide an integer
corresponding to each node.
.SH
Costs
.PP
To increase the flexibility of representing costs, the tree matcher
views costs as an ADT.
For example, costs may be represented as an integer or
as a vector of integers with each element representing the cost of a specific
resource.
A cost ADT suitable for Twig must have the following four definitions:
.IP
\(bu \f3COST\f1 is a C type.
Although the proper functioning of the tree matcher
does not depend on the details of the \f3COST\f1 type, it must have
the type information for storage allocation purposes.
.IP
\(bu
\f3INFINITY\f1 is the maximum attainable cost value.  The matcher uses
\f3INFINITY\f1 to initialize its data structures.
.IP
\(bu
\f3DEFAULT_COST\f1 is the cost
value returned by rules without a cost part.
.IP
\(bu
\f3COSTLESS\f1 is a function of
two cost values.  It must return true if and only if the first argument is
less than the second.
.sp 2ex
.SH
Trees
.PP
As with costs, Twig treats trees as an ADT.  Here, using an ADT is
even more important because trees come in a variety of shapes and
representations.
Twig would be overly complicated if it had to
know any more than the fundamental properties of trees.  Thus,
twig treats trees as an acyclic directed
graph of almost featureless nodes with one
distinguished node, the root.  Each node has only one feature and that
is an integer corresponding to the \f2node_id\f1s discussed above.
To provide this
view to Twig, the user must provide the following definitions and
subroutines.
.IP
\(bu \f3NODE\f1 is the type of a node.  The actual
details of
the NODE are irrelevant and Twig uses this definition only for storage
allocation and declaration purposes.
.IP
\(bu \f3NONODE\f1 is a constant of type \f3NODE\f1.  It is a distinguished
value returned by the routines defined below to represent a null value.
.IP
\(bu
\f3mtGetNodes#(r,n)# returns the #n#th child of \f2r\f1 where \f2r\f1
is a \f3NODE\f1.  The
leftmost child is \f3mtGetNodes\f2(r,1)\f1.  If #n>degree(r)#
then the function should return \f3NONODE\f1.  Twig expects the
expression \f3mtGetNodes\f2(\f3NONODE\f2, 0)\f1
to denote the root node of
the subject tree.
.IP
\(bu
\f3mtValue\f2(r)\f1 returns the integer associated with \f2r\f1.
.IP
\(bu
\f3mtSetNodes\f2(r,n,c)\f1 replaces the \f2n\f1th child of \f2r\f1 with
\f2c\f1.  This routine may be used to replace whole subtrees with others.
.sp 2ex
.SH
Pattern Matcher Operation
.PP
The pattern matcher operates in two phases: the costing phase and the
execution phase.
.PP
In the costing phase, the matcher performs a preorder traversal of
a subject tree and discovers matches from the bottom up.  At the same time, it
builds a skeleton tree that is structurally isomorphic to the subject tree.
When a match is discovered the cost clause of the pattern is invoked to
calculate the cost.  Many patterns with different labels could match at any
given node but only the lowest cost pattern for each label is recorded in
the skeleton.
.PP
When a pattern is matched, its label is then used as input to the pattern
matcher so that matching of patterns with labelled leaves can begin.
This
process
is analogous to a reduce action in
bottom up parsers.  In twig, multiple reductions are
tracked.
.PP
The cost part of a rule can also assign a \f2mode\f1 to a match.  The
\f2mode\f1 controls how action parts are to be executed.
Actions parts of \f2defer\f1 mode matches are not executed until the
execution phase.  \f2Rewrite\f1 mode matches cause the action part to
be executed immediately during the costing phase to transform the
matched subtree.  Once the rewrite mode action is executed, the costing
phase continues by purging all information in the skeleton for the old
subtree and then scanning the newly transformed subtree.
\f2Rewrite\f1 actions are similar
to actions that are invoked on a reduction in
LR parsing.
.PP
In the case where multiple rewrite actions are possible the one with
the lowest cost will be executed.  Furthermore, a rewrite action is
considered only if its cost is lower than all the defer mode actions.
Rewrite mode actions that are not executed are treated in the same
manner as defer mode actions in the execution phase.
.PP
When the root of the subject tree is reached in the preorder
traversal, the execution phase begins.  The lowest cost match is then
chosen at the root and executed.  In this context, execution involves
executing the actions of the labelled leaves before invoking the code
in the current action.
.SH
Lexical Issues and Conventions
.PP
Currently the
following are keywords of twig in no particular order:
.DS
.TS
center;
l l l.
label	node	cost
action	prologue	insert
.TE
.DE
They have special meanings and may not be used as
identifiers.
;:(),= are special characters.  All blanks, tabs, formfeeds and
newlines are ignored by twig but they may not appear inside
identifiers and numbers.  Identifiers are nonempty strings
made of letters, digits or underscores and starting with a letter.
Numbers are nonempty string of digits.  Code fragments or action parts
are enclosed in braces.  Inside code fragments, C lexical rules must
be obeyed except that strings of the form $...$ that are not inside C
strings are interpreted by twig.  In the following sections, $id$
denotes an identifier; \f2int1\f1, \f2int2\f1 denotes numbers; \f2ccode\f1
denotes C code fragments; ... indicates repetition of the previous
item; [...] indicates that ... is optional.
.PP
The input to the Twig program will be referred to as the subject tree.
.sp 2ex
.SH
Prologue and Inserts
\f1
.DS
\f3prologue \f2ccode\f1;
.DE
signals to twig that \f2ccode\f1 should be inserted at the
beginning of the output source file.  \f2Ccode\f1 should contain
definitions relevant to the C code in
rules that are defined later in the twig source file.
.DS
\f3insert \f2ccode\f1;
.DE
should cause \f2ccode\f1 to be placed into the source file.
There can be multiple inserts and they will be placed into the source
file
in the order that they appear.
.SH
Declarations
.PP
All twig identifiers are declared before they are used.
.DS
\f3node\f1 \f2id\f1[(\f2int1\f1)] [= \f2int2\f1] ...;
.DE
A node declaration declares one or more identifier to be
associated with nodes of the subject tree.  The identifiers are
assigned numbers by twig but the user can overide the assigned number
by specifying \f2int2\f1.  The degree of the node identifer, i.e. the
number of children, is assumed to be fixed.  Normally, twig will
deduce the degree when \f2id\f1 is used in a rule.  However, the user may
explicitly give the degree by specifying \f2int1\f1.
.DS
\f3label\f2 id\f1 ...;
.DE
A label declaration indicates that the following $id$'s are to be used
as labels of rules.
.sp 2ex
.SH
Costs and Action code
.PP
Code fragments such as the \f2cost\f1 and \f2action\f1 clauses of a rule are
specfied by enclosing C code in braces.  All legal C constructs are
permitted in code fragments.  In addition, the following are provided
for convenient access to the subject tree and internal data
structures of the matcher.
.sp 1ex
.IP
\(bu $%\f2n\f1$ denotes a pointer value to the matcher data
structure for the $n$th labelled leaf.  Thus to access the cost value
associated with that leaf, the notation $%\f2n\f1$\(->cost may be used.
.IP
\(bu
$$ denotes a pointer value to the root of the subject
tree.
.IP
\(bu
$#{n sub 1}.{n sub 2}.{n sub 3}...{n sub {k-1}}.{n sub k}#$ denotes
a pointer value to the ${n sub k}$th child of the $#{n sub {k-1}}#th$
child of the
$#{n sub {k-2}}#$th child of ... of the $#{n sub 1 }#$th child of the root of
the subject tree.  Each $#{n sub i}#$ is a positive non-zero integer.
.PP
Some special constructs are available to code fragments in the cost
part of the specification.  The
statement "\f3ABORT;\f1" when encountered during the execution of
the cost code, signals to the matcher that this pattern is to be
rejected.  When a "\f3REWRITE;\f1" statement is encountered, control
is returned to the matcher and the rule will become a \f2rewrite\f1
mode match.  When the end of the cost code fragment is reached,
control is returned to the matcher and the rule becomes a \f2defer\f1
mode match.  Cost values are returned to the matcher by assigning to
a variable named
"\f3cost\f1" in the cost clause.  If no assignment is
made to \f3cost\f1 then the returned cost will be
\f3DEFAULT_COST\f1.
.PP
Action clauses are expected to return a new tree which will replace
the subject tree.  This is done by returning using the standard C
"\f3return\f2(new_tree);\f1" statement
where \f2new_tree\f1 is of type
\f3NODEPTR\f1.  When the end of the action clause is encountered, the
matcher resumes execution and the subject tree is not replaced.
.bp
.SH
An Example
.PP
The following are examples of twig rules that generate VAX code for
the subtract instruction:
.DS
.na
.nf
prologue {
	NODE gettermp();
};
node	long constant sub;
label	operand temp;

operand: long;			/* rule 1 */
operand: constant;		/* rule 2 */
operand: temp;			/* rule 3 */
temp:	 operand;		/* rule 4 */
	{ cost = TEMP_COST+$%1$\(->cost; }
	= {
		NODE t = gettemp();
		emit("mov", $$, t, 0);
		return(t);
	};

operand: sub(operand,opernad)	/* rule 5 */
	{ cost = $%1$\(->cost-$%2$\(->cost+SUB_COST; }
	= {
		NODE t = gettemp();
		emit("sub", $1$, $2$, t, 0);
		return(t);
	};

temp:	sub(temp,constant)	/* rule 6 $*/
	{
		if(value($2$)==1)
			cost = $%1$\(->cost+DEC_COST;
		else ABORT;
	} = {
		emit("dec", $1$, 0);
		return($1$);
	};
.DE
.IP
\(bu
Rules 3 and 4 form a loop.
The potential loop: temp\(->operand\(->temp\(->operand... is
broken
by the matcher recognizing that the cost of the second match of temp
does not cost less than the first match of temp.
.IP
\(bu In the cost clause of rule 5, the cost is the sum of the
leaves plus the cost of the subtract instruction.
The action clause emits code to add the two operands
and leave the result in a temporary location.  The temporary is
returned as a substitution for the subject tree.
.IP
\(bu Rule 6 handles a special case where the left operand is
already in a temporary and the constant is one.  In this case, the
temporary is directly decremented and returned as the new tree.
.bp
.DS C
\f3Experimental Results\f1
.DE
.sp 2ex
.PP
A code generator for the VAX has been build using Twig and incorporated
into the pcc2 compiler.  The following table summarizes the results
for three benchmark programs.
The compiler using the Twig code generator is abbreviated \f2tcc\f1.
The times were obtained on a VAX-11/780
running 4.2bsd UNIX.
The first number is the time spent in the compiler.
The parenthesized number is the time spent in the operating system.
.TS
center;
c c c c c
l n n n n.
	lines	tcc	pcc2	improvement
_
grep.c	458	9.9(1.4)	11.4(0.9)	13%
pmach.c	610	13.2(1.5)	13.5(0.6)	2%
reader.c	1005	23.3(1.6)	31.2(0.9)	25%
.TE
.PP
The running time for tcc is slightly
faster than that of pcc2.
However, The higher system times of tcc indicates that tcc place
higher demands on the operating system.
In the case of pmach.c, the total CPU time is actually more than that
of pcc2's.
.PP
Twig requires more dynamic storage than pcc2.  For a tree, the dynamic
storage requirement on the average is  number of nodes * 200
bytes
while the worst case is about number of nodes * 1000 bytes.
However this storage is reclaimed and reused for every new tree.
The larger system time
is caused by calls to the runtime system for dynamic storage.
.PP
Writing and modifying tcc was quick.
The basic
VAX without indexed addressing mode can be described in about two weeks.
This was done concurrently with debugging much of the table
walker.  Adding indexed modes took
only several hours once we were confident that the rest of the
description was correct.
.PP
One reason for the ease of modifcation is that rules can be added
independent of the other rules.  Looping can be completely ignored since
dynamic programming eliminates that possiblity.
Table generation was extremely fast or about 4.2 seconds for the
VAX description.  Thus testing the description could be done quickly.
.PP
The tables were very small.  For the VAX, there were only 109 rules.
This required 7.5K bytes of table space.
The whole code generator is another 40K
bytes.
.SH
Code Quality
.PP
The code generated by the Twig for the test programs were of the
same quality as code generated by pcc2.
However, Twig does better targeting.
.DS
tcc:
.ti +4em
addl3	-4(fp),-8(fp),-12(fp)
.sp 1ex
pcc2:
.ti +4em
addl3	-4(fp),-8(fp),r0
.ti +4em
movl	r0,-12(fp)
.DE
On the other hand, pcc2 handled temporary registers better
and for an equivalent sequence of code, Twig may use
more temporary registers.
.DS
tcc:
.ti +4em
	cvtbl	(r0),r1
pcc2:
.ti +4em
	cvtbl	(r0),r0
.DE
This is actually a symptom of how temporary registers are allocated in tcc.
Registers are allocated before code is emitted and thus any registers freed
during code emissions will not be reused until the next code sequence.
.SH
Room for improvement
.PP
Currently Twig is slow compared to other table-driven systems such as
[Henry] and [Ganapathi].  However, we have had less than one year of
experience with Twig and we believe that its performance can be
improved because
.IP
\(bu Unit productions such as
.DS
operand: temp
.DE
in the above example increases the running time of Twig
since the closure operation for unit production is done at run-time.
.IP
\(bu Machine representations.  The current compact
representation of the machine require linear searching except at the
start state where the large branching factor compelled us to use an
array scheme.  Using another representation would speed up the
matcher.  For example, implementing the VAX description as an array is
estimated to use about 50K bytes but would provide a significant
performance improvement.
.IP
\(bu Twig performs many more procedure calls than pcc2.
Every machine step requires at least one procedure call.  Contrast
this to YACC where a machine step is performed in line.  Procedure
calls on the VAX are expensive.

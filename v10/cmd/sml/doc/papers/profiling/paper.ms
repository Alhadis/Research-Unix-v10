.LP
-
.ft 3
.ce 99
.sp 2i
.LG
Profiling in the Presence of Optimization and Garbage Collection
.sp
.ft 2
.ce 99
.NL
Andrew W. Appel*
Bruce F. Duba\(dg
David B. MacQueen\(dg
.sp 0.6i
.ce 99
.NL
November 1988
.sp 1i
.ce
.ft  2
ABSTRACT
.ft 1
.IP
Profiling the execution of programs can be a great help in tuning their
performance, and programs written in functional languages are no exception.
The standard techniques of call-counting and statistical (interrupt-driven)
execution time measurement work well, but with some modification.  In
particular, the program counter is not the best indicator of ``current
function.''
Our profiler inserts explicit increment and assignment statements into
the intermediate representation, and is therefore very simple to implement
and completely independent of the code-generator.
.LP
.sp 1i
.nr PS  8
.nr VS 10
.LP
* Supported in part by NSF Grant CCR-8806121 and by a Digital Equipment Corp.
Faculty Incentive Grant.
.LP
\(dg AT&T Bell Laboratories, Murray Hill, NJ.
.nr PS 10
.nr VS 16
.LP
.bp
.NH
Execution profiling
.LP
A large program usually consists of many small functions.  
When such a program is to be tuned for efficiency, it is necessary to
identify which of those functions are taking the bulk of the execution time.
Then the commonly-used functions can be made more efficient, or called less
often, or both.  By using a theoretical analysis of the algorithms used
in a program, such functions can be identified; but a complete
theoretical analysis is complex and
impractical for large programs.

An execution profiler provides an empirical measurement of the time spent
in each function.  A widely-used Unix tool, \fBprof\fP [Unix],
provides a count of how many times each function is called, and how many
seconds were spent in each function.   This information is very useful
in identifying which functions are in need of improvement, after which
a theoretical analysis of just those functions might be carried out,
a much less forbidding endeavor than analyzing the whole program.

\fBProf\fP gathers
call-count information 
by having the compiler insert at the beginning of each function
an instruction that increments a call-count variable associated with
the function.  An approximation to the amount
of total time spent in the function is gathered by the use of a timer
interrupt:  every 1/60th of a second, the operating system notes
in a ``histogram'' array the value of the program counter.
(This is an ancient technique [Johnston70].)
Then, at the end of program execution, \fBprof\fP estimates
the amount of time spent in each
function by summing the values in the histogram array
corresponding to program counter samples between the beginning and end of the
machine code for that function.
The interrupt-driven sampling method has a much lower overhead than
querying a clock on each entry to and exit from a function.

A more elaborate profiling tool, \fBgprof\fP [Graham82],
provides even more information.  
When one primitive function (e.g. a
table-lookup routine) is used in many places, it is useful to know,
not only the total time for the execution of the primitive, but also, how
much time to ``charge'' the calling functions. With \fBgprof\fP this is
approximated by keeping a count of the number of
times each call-site is used, and on that basis apportioning the
average execution time
of the called function to the functions that call it.

These approaches to execution-time measurement and apportionment pose certain
problems for optimizing compilers and for functional languages:
.IP 1.
The machine-code for a function is not necessarily all contiguous.
A function may be turned into several pieces of code, with portions of
the code for other functions interspersed.  This problem could certainly
be solved by elaborate bookkeeping in the optimizer and code generator,
but we wanted to avoid that complexity.
.IP 2.
An optimizer can expand functions in-line in other functions.
The program-counter method will charge the calling function
instead of the called function,
even though it might be desirable for in-line expansion to be made
semantically invisible.
.IP 3.
The histogram array in \fBprof\fP
must be proportional in size to the address range spanned by pieces of
code for executable functions.  Our runtime
system intersperses code and data
throughout memory; even worse, it periodically garbage collects, moving
code and data from place to place.  This problem could have been solved
by elaborate bookkeeping in the runtime system, which we also wanted to avoid.
.LP
We had to deal with these problems in the course of implementing a
profiler for an optimizing compiler for the functional language
Standard ML [Appel87].  The approach we used is described in the next
section.
.NH
Intermediate Representation of call-counting and current-function
.LP
For execution-time estimation we use a timer interrupt, as does
\fBprof\fP, to increment a histogram entry.  However, we don't use the
program counter to calculate which histogram entry to increment.
Instead, we maintain a ``pointer-to-current-function-entry'' in a
global variable called \fBcurrent\fP that is accessible to the
timer-interrupt handler.  Each function has associated with it two
auxiliary variables: a call-count and an interrupt-count.  On entry to
a function, it increments the call-count and assigns the address of
the interrupt-count variable into the global \fBcurrent\fP variable.
Then, when a timer interrupt occurs, the interrupt handler just increments the
variable that \fBcurrent\fP points to.

When a function returns \(em either normally or via an exception \(em
\fBcurrent\fP must be set back to the interrupt-count variable of the
function that it is returning to.  This resetting could be done either
by the calling function (after the called function has returned), or by
the called function before exit.  For several reasons, it is better done by
the calling function.  If the called function does the reset,  a stack
of current-function pointers is required; this is expensive to maintain.
A stack of current-pointers would also greatly complicate the treatment
of exception-handlers; with the caller-reset method, the exception-handler
justs sets the \fBcurrent\fP to point at the appropriate counter variable
on entry.
On recursive calls, \fBcurrent\fP need not be reset (as the calling
and called function are the same), but only the calling function knows
which calls are recursive.  And finally, tail-calls can be optimized
if the caller resets \fBcurrent\fP.

A tail-call is one that is not followed by any
executable code before the function returns.  After a tail-call, the
function will immediately return and therefore \fBcurrent\fP will
immediately be reset.  Therefore, it is not necessary for the calling
function to reset \fBcurrent\fP after a tail-call.
This is a useful optimization, and it is particularly important
when used with a compiler that optimizes tail-calls into jumps; if the
current pointer had to be reset after the tail-call, it would no
longer be a tail-call and performance would suffer dramatically.
Fortunately, it is easy to identify tail-calls statically as the
profiling instructions are being inserted.

We insert the profiling instructions as ordinary assignment statements
in the intermediate representation.  In almost any compiler's
intermediate representation it is easy to represent the operations
of fetching, adding one, and storing, for the call-count increment operation;
and storing, for the assignment to the \fBcurrent\fP variable.

Functional programming languages introduce another problem for the
design of profilers: what to do with anonymous, first-class functions.
The simplest choice is to do nothing; collect no call-counts and let
the time be charged to the caller of the unnamed function. The main
disadvantage of this solution, besides not having the call-counts, is
that there is no convenient way to find the code that contributes to
the cost of a profiled function that calls anonymous functions.

Probably the most general solution is to make up names for the unnamed
functions (for example, an unnamed function statically enclosed in function
\fIf\fP might be called \fIf.anon\fP).
If anonymous functions are given names they can be treated
just as any other function; call counts and execution time will be
reported.  Of course, the user will need to associate the new names
with the correct function, but in practice this is rarely a problem.
.NH
An example
.LP
To illustrate the technique, we present a simple example (figure 1).  The ML function
\f(CWsubset\fP takes a predicate function as an argument, and returns
a function that maps lists to lists; the output list will be that sublist
of the input list containing just those elements that satisfy the predicate.  
The user's program is displayed in typewriter font; the compiler puts some
scaffolding around it (indicated in italics) to make a record
containing all the functions declared by the user.
.KF
.DS
.ft CW
\fIlet\fP fun subset pred =
       let fun f nil = nil
             | f (a::r) = if pred a then a::f(r) else f(r)
        in f
       end

   fun isPrime x = 
       let fun test i = i>=x orelse (x mod i <> 0 andalso test(i+1))
        in test 2
       end

   val primes = subset isPrime
\fI in (subset, isPrime, primes)
end\fP
.ft R
.DE
.DS C
Figure 1.
.DE
.KE
If this code is compiled with profiling enabled, the compiler inserts
the call-counting and current-function instructions into the intermediate
representation.  Here, we display the effects as if written in the source language
(figure 2).
.KF
.DS
.ft CW
\fIlet val subset.CC = ref 0 and subset.IC = ref 0
    and subset.f.CC = ref 0 and subset.f.IC = ref 0
    and isPrime.CC = ref 0 and isPrime.IC = ref 0
    and isPrime.test.CC = ref 0 and isPrime.test.IC = ref 0
\fP
   fun subset pred =
       \fI(subset.CC := !subset.CC + 1;\fP
       \fIcurrent := subset.IC;\fP
       let fun f x =
           \fI(subset.f.CC := !subset.f.CC + 1;\fP
           \fIcurrent := subset.f.IC;\fP
           case x of
             nil => nil
           | a::r => let val pa = pred a
                      in \fIcurrent := subset.f.IC;\fP
                         if pa then a :: f(r) else f(r)
                     end
        in f
       end

   fun isPrime x = \fI(isPrime.CC := !isPrime.CC + 1;\fP
                   \fIcurrent := isPrime.IC;\fP
                   . . . )

   val primes = subset isPrime
\fI
 in ((subset, isPrime, primes),
     ((subset.CC, subset.IC, "subset"),
      (subset.f.CC, subset.f.IC, "subset.f"),
      (isPrime.test.CC, isPrime.test.IC, "isPrime.test"),
      (isPrime.CC, isPrime.IC, "isPrime")))
end\fP
.ft R
.DE
.DS C
Figure 2.
.DE
.KE

For each function, two variables are introduced:  a call-count and an interrupt-count.
On entry to a function, the call-count is incremented, and the global variable
\fBcurrent\fP is set to point to the interrupt-count.  On re-entry to a function
after a subroutine call, \fBcurrent\fP is reset to the function's
interrupt-count variable.  However, this is not necessary after recursive calls
and tail calls, e.g. the calls to \f(CWf\fP.

The initial \fIlet\fP-bindings create all the count variables, and the
last four lines produce, instead of just a record containing the user's
declared objects, a pair of records:  the user's declared objects, and a
list of records containing profiling variables, each with an identifying
string constant.  These string constants will be embedded in the executable
code for this module, and will enable the call-count variables to be self-identifying.
Our runtime system maintains a global list of these
3-element records; when it is time to print an execution profile, they are
sorted in decreasing order of interrupt-count.

.KS
Our output looks like the output of \fBprof\fP:
.DS
.ft CW
%time  cumsecs   #call ms/call  name
 90.4     3.52   78189    .045  isPrime.test
  8.4     3.85    1000    .330  isPrime
   .7     3.88       0          (unprofiled)
   .2     3.89    1001    .009  subset.f
   .0     3.89    1001    .000  natlist
   .0     3.89       1    .000  subset
.ft R
.DE
.KE

Now, armed with this information, a programmer
might decide that it is worthwhile
re-writing the \f(CWisPrime\fP function
to make it as efficient as possible.  But at a certain point
the programmer will want to know what functions are calling \f(CWisPrime\fP
so he can make them call it less often.  By re-compiling with
\f(CWisPrime\fP unprofiled, any time spent in \f(CWisPrime\fP will
now be charged to the function that called it.  This is because
\f(CWisPrime\fP will not change the \fBcurrent\fP variable, so that the
timer-interrupt will increment the  count for the function that last
set \fBcurrent\fP \(em and this will be the one that called \fPisPrime\fP.
The profiling system won't do this automatically, but by comparing two
different execution profiles, one with \f(CWisPrime\fP compiled with
profile instructions and one with \f(CWisPrime\fP unprofiled, an accurate estimate
can be made of who is calling it.
.NH 
Advantages of our current-function method
.LP
Since we use ordinary intermediate-representation operators
for profiling,
the optimizer and code-generator ``believe'' that 
profiling operations are part of the program.
Since an optimizer must not modify the semantics of the program,
the semantics of profiling will not be modified either.
Therefore, if one function is copied and inserted in-line into another,
the call-count and current-function instructions will be copied and
inserted at the right place.  Other optimizations that break functions
into several disjoint pieces of code will leave the profiling
instructions in the appropriate places.

Furthermore, the result is that the implementation of the profiler
is completely independent of the code generator.  We have four different
code generators for our compiler (two different algorithms each for the
Vax and the Motorola 68020), and not a line of any of them was modified
for the installation of the profiler.

By compiling some functions unprofiled, as described in the previous section,
we can find out what callers are responsible for most of their execution time.
This kind of trick serves much the same purpose that the more elaborate
program \fBgprof\fP does; and it's a trick that wouldn't work with
a program-counter histogram.
Furthermore, our method is more accurate than \fBgprof\fP.  Suppose
functions \fIf\fP and \fIg\fP both call a function \fIisPrime\fP, but
\fIf\fP consistently makes expensive calls (that take a long time) while
\fIg\fP makes cheap ones.  \fBGprof\fP allocates the total time spent in
\fIisPrime\fP on the basis of call counts from \fIf\fP and \fIg\fP;
this will miss the fact that \fIf\fP is responsible for most of the cost.
In this example, when profiling for \fIisPrime\fP is turned off, \fIf\fP
and \fIg\fP will be charged for the actual time spent in \fIisPrime\fP
on their behalf.  (On the other hand, \fBGprof\fP will give an accurate
breakdown of call-site counts that our method does not provide.)

If a profiled function calls an unprofiled function, then during the
execution of the called function, all timer interrupts will be charged
to the caller (since \fBcurrent\fP still points to the caller's
variable).  This is often desirable, as described above.  But if an
unprofiled function calls a profiled function, then upon return to the
unprofiled function the \fBcurrent\fP pointer won't be reset, and
interrupts will continue to be charged to the called function after it
has returned.  This is undesirable, and should be prevented by the
compiler.  In a language with first-class functions, it is difficult
to prevent profiled functions from being passed as arguments to
unprofiled functions that might then call them.  In practice, this has
not proved to be a problem, probably because unprofiled functionals
are typically simple primitives like \fIapp\fP and \fImap\fP, which do
little intrinsic computation.
.NH
Overhead measurements
.LP
We ran the same program several times with various of our profiling
features enabled; this gives a reasonably accurate measurement of profiling
overhead:
.KF
.TS
tab(|) box center;
l c c
l n n.
|Time|%Overhead|LenL|LenR|Comp
_
User code|2801 sec|
_
Call counts|568|20.3%
Setting current function|286|10.2
Interrupts|47|1.7
_
Total Overhead|901|32.2%
.TE
.KE
The total overhead of 32% is not prohibitively expensive.  Our code generator
takes three instructions to increment a call-count (fetch, add, store);
a better instruction-selector could probably reduce this overhead to 8%, and the
total overhead to 20%.

.KF
There is also an implementation overhead; it turned out to be fairly simple
to get this profiler running.
.TS
tab(|) center;
l n.
Insertion of profiling instructions|49 lines
Interrupt handling|32
Global database|16
Report generation|72
_
Total|169 lines
.TE
In contrast, this paper is about 500 lines long.
.KE
.NH
Conclusion
.LP
Traditional approaches to profiling run into problems when we attempt to apply
them to functional languages where code may be moved around by garbage collection,
and the task is further complicated when an optimizing compiler freely rearranges
the code.  The basic difficulty is that the mapping between the current pc and the
currently executing function is difficult to maintain.  

We have found a simple way around this difficulty, which consists of
maintaining a global variable that always points to the interrupt
count for the current function, and which is to be charged whenever
there is a timer interrupt.  Because we manipulate this variable in the
intermediate representation of the compiler, our method is very easy
to implement and has no nasty interactions with code generation or
garbage collection algorithms (which already preserve semantics of
intermediate-representation operations).

This method has acceptable overhead and accuracy.  Furthermore, by
judiciously mixing profiled and unprofiled functions, one can extract
information on inherited costs as well as the direct costs of calling
particular functions.  This information is similar to that provided by
sophisticated profilers like gprof, but is more accurate.
.SH
References
.LP
.IP [Appel87] 1i
Appel, Andrew W. and MacQueen, David B.  ``A Standard ML compiler,''
in \fIFunctional Programming Languages and Computer Architecture\fP,
LNCS 274, G. Kahn, ed., pp 301-324, 1987
.IP [Graham82]
Graham, Susan L. Graham, Peter B. Kessler, and Marshall K. McKusick.
``gprof: a call graph execution profiler''" in 
\fIProc. SIGPLAN '82 Symp. on Compiler Construction, SIGPLAN Notices\fP
17(4), pp. 120-126, 1982.
.IP [Johnston70]
Johnston, T. Y., and Johnson, R. H., \fIProgram Performance
Measurement\fP, SLAC User Note 33, Rev. 1, Stanford University, California, 1970.
.IP [Unix]
Unix Programmer's Manual, ``prof command,'' section 1, Bell Laboratories, Murray Hill,
NJ, 1979.

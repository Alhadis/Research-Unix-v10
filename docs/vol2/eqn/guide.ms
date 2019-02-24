.so ../ADM/mac
.XX eqn 25 "Typesetting Mathematics \(em User's Guide"
.nr PI .2i
.nr PD .1v
.EQ
delim $$
.EN
.TL
Typesetting Mathematics \(em User's Guide\(dg
.AU 2C-518 6021
Brian W. Kernighan
Lorinda L. Cherry
.AI
.MH
.AB
.PP
This is the user's guide for
.IT eqn ,
a system for typesetting
mathematics.
Mathematical expressions are described in a language
designed to be easy to use
by people who know neither mathematics nor typesetting.
Enough of the language to set in-line expressions like
$lim from {x-> pi /2} ( tan~x) sup{sin~2x}~=~1$
or display equations like
.EQ I
G(z)~mark =~ e sup { ln ~ G(z) }
~=~ exp left (
sum from k>=1 {S sub k z sup k} over k right )
~=~  prod from k>=1 e sup {S sub k z sup k /k}
.EN
.EQ I
lineup = left ( 1 + S sub 1 z +
{ S sub 1 sup 2 z sup 2 } over 2! + ".\^.\^." right )
left ( 1+ { S sub 2 z sup 2 } over 2
+ { S sub 2 sup 2 z sup 4 } over { 2 sup 2 cdot 2! }
+ ".\^.\^." right ) ".\^.\^."
.EN
.EQ I
lineup =  sum from m>=0 left (
sum from
pile { k sub 1 ,k sub 2 ",\^.\^.\^.\^," k sub m  >=0
above
k sub 1 +2k sub 2 + ".\^.\^." +mk sub m =m}
{ S sub 1 sup {k sub 1} } over {1 sup k sub 1 k sub 1 ! } ~
{ S sub 2 sup {k sub 2} } over {2 sup k sub 2 k sub 2 ! } ~
".\^.\^."
{ S sub m sup {k sub m} } over {m sup k sub m k sub m ! }
right ) z sup m
.EN
can be learned in an hour or so.
.PP
The language interfaces directly with
the formatting language
.IT troff ,
so mathematical expressions can be embedded in the running
text
of a manuscript,
and the entire document produced in one process.
This user's guide is an example of its output.
.AE
.2C
.FS
\(dg This is a revised version of |reference(eqn cstr).
.FE
.NH
Introduction
.PP
.IT Eqn
is a
program and a language for typesetting mathematics.
The
.IT eqn
language was designed to be easy to use
by people who know neither mathematics
nor typesetting.
Thus
.IT eqn
knows relatively little about mathematics.
In particular, mathematical symbols like
+, \(mi, \(mu, parentheses, and so on have no special meanings.
.IT Eqn
is quite happy to set garbage (but it will look good).
.PP
.IT Eqn
is a preprocessor for the typesetter formatter
.IT troff ,
so the normal mode of operation is to prepare
a document with both mathematics and ordinary text
interspersed,
and let
.IT eqn
set the mathematics while
.IT troff
does the body of the text.
.PP
To use
.IT eqn
on
.UX ,
.P1
eqn files | troff
.P2
.PP
The design and implementation of 
.I eqn
is discussed in |reference(kernighan cherry cacm).
.NH
Display Equations
.PP
To tell
.IT eqn
where a mathematical expression begins and ends,
one marks it with lines beginning
.CW .EQ
and
.CW .EN .
Thus
if you type the lines
.P1
\&.EQ
x=y+z
\&.EN
.P2
your output will look like
.EQ
x=y+z
.EN
The
.CW .EQ
and
.CW .EN
are copied through untouched;
they
are not otherwise processed
by
.IT eqn .
This means that you have to take care
of things like centering, numbering, and so on
yourself.
The most common way is to use a
.IT troff
macro package like
.CW -ms
or
.CW -mm ,
which allow you to center, indent, left-justify, and number equations.
.PP
With the
.CW -ms
package,
equations are centered by default.
To left-justify an equation, use
.CW \&.EQ\ L
instead of
.CW .EQ .
To indent it, use
.CW .EQ\ I .
Any of these can be followed by an arbitrary `equation number'
which will be placed at the right margin.
For example, the input
.P1
\&.EQ I (3.1a)
x = f(y/2) + y/2
\&.EN
.P2
produces the output
.EQ I (3.1a)
x = f(y/2) + y/2
.EN
.PP
For stupid historical reasons, the
.CW -mm
macros package requires you to surround
.CW .EQ
and
.CW .EN
with a pair of totally unnecessary commands,
.CW .DS
and
.CW .DE :
.P1
\&.DS	\f2for \fP-mm\f2 only\fP
\&.EQ
\f2your equation\fP
\&.EN
\&.DE
.P2
.PP
There is also a shorthand notation so
in-line expressions
like
$pi sub i sup 2$
can be entered without
.CW .EQ
and
.CW .EN .
We will talk about it in section 9.
.NH
Input spaces
.PP
Spaces, tabs, and newlines within an expression are thrown away by
.IT eqn .
(Normal text is left absolutely alone.)
Thus
between
.CW .EQ
and
.CW .EN ,
.P1
x=y+z
.P2
and
.P1
x = y + z
.P2
and
.P1
x   =   y
   + z
.P2
and so on
all produce the same
output:
.EQ
x=y+z
.EN
You should use spaces and newlines freely to make your input equations
readable and easy to edit.
In particular, very long lines are a bad idea,
since they are often hard to fix if you make a mistake.
.NH
Output spaces
.PP
To force extra spaces into the
.IT output,
use a tilde
.CW ~ '' ``
for each space you want:
.P1
x~=~y~+~z
.P2
gives
.EQ
x~=~y~+~z
.EN
You can also use a circumflex
.CW ^ '', ``
which gives a space half the width of a tilde.
It is mainly useful for fine-tuning.
.NH
Symbols, Special Names, Greek
.PP
.IT Eqn
knows some mathematical symbols,
some mathematical names, and the Greek alphabet.
For example,
.P1
x=2 pi int sin ( omega t)dt
.P2
produces
.EQ
x = 2 pi int sin ( omega t)dt
.EN
Here the spaces in the input are
necessary
to tell
.IT eqn
that
.CW int ,
.CW pi ,
.CW sin ,
and
.CW omega
are separate entities that should get special treatment.
The
.CW sin ,
digit 2, and parentheses are set in roman type instead of italic;
.CW pi
and
.CW omega
are made Greek;
and
.CW int
becomes the integral sign.
.PP
When in doubt, leave spaces around separate parts of the input.
A
common error is to type
.CW f(pi)
without leaving spaces on both sides of the
.CW pi .
As a result,
.IT eqn
does not recognize
.CW pi
as a special word, and you get
$f(pi)$
instead of
$f( pi )$.
.PP
A list of
.IT eqn
names appears in section 24.
Knowledgeable users can also use
.IT troff
four-character names
for anything
.IT eqn
doesn't know about,
like
.CW \e(L1
for the AT&T death-star \(L1.
.NH
Spaces, Again
.PP
The only way
.IT eqn
can deduce that some sequence
of letters might be special
is if that sequence is separated from the letters
on either side of it.
This can be done by surrounding a special word by spaces,
tabs, or newlines,
as we did in the previous section.
.PP
You can also make special words stand out by surrounding them
with tildes or circumflexes:
.P1
x~=~2~pi~int~sin~(~omega~t~)~dt
.P2
is much the same as the last example,
except that the tildes
not only
separate the magic words
like
.CW sin ,
.CW omega ,
and so on,
but also add extra spaces,
one space per tilde:
.EQ
x~=~2~pi~int~sin~(~omega~t~)~dt
.EN
.PP
Special words can also be separated by braces
.CW {}
and double quotes
.CW \&"" ,
which have special meanings that we will
see soon.
.......
.NH
Subscripts and Superscripts
.PP
Subscripts and superscripts are
obtained with the words
.CW sub
and
.CW sup .
.P1
x sup 2 + y sub k
.P2
gives
.EQ
x sup 2 + y sub k
.EN
.IT Eqn
takes care of all the size changes and vertical motions
needed to make the output look right.
The words
.CW sub
and
.CW sup
must be surrounded by spaces;
.CW "x sub2"
will give you
$x sub2$ instead of $x sub 2$.
Furthermore, don't forget to leave a space
(or a tilde, etc.)\&
to mark the end of a subscript or superscript.
A common error is to say
something like
.P1
y = (x sup 2)+1
.P2
which causes
.EQ
y = (x sup 2)+1
.EN
instead of
the intended
.EQ
y = (x sup 2 )+1
.EN
.PP
Subscripted subscripts and superscripted superscripts
also work:
.P1
x sub i sub 1
.P2
is
.EQ
x sub i sub 1
.EN
A subscript and superscript on the same thing
are printed one above the other
if the subscript comes
.IT first :
.P1
x sub i sup 2
.P2
is
.EQ
x sub i sup 2
.EN
.PP
Other than this special case,
.CW sub
and
.CW sup
group to the right, so
.IT "x\ sup\ y\ sub\ z
means
$x sup {y sub z}$, not ${x sup y} sub z$.
.NH
Braces for Grouping
.PP
Normally, the end of a subscript or superscript is marked
simply by a blank (or tab or tilde, etc.)
What if the subscript or superscript is something that has to be typed
with blanks in it?
In that case, you can use the braces
{ and } to mark the
beginning and end of the subscript or superscript:
.P1
e sup {i omega t}
.P2
is
.EQ
e sup {i omega t}
.EN
.sp
Rule:  Braces can
always
be used to force
.IT eqn
to treat something as a unit,
or just to make your intent perfectly clear.
Thus:
.P1
x sub {i sub 1} sup 2
.P2
is
.EQ
x sub {i sub 1} sup 2
.EN
with braces, but
.P1
x sub i sub 1 sup 2
.P2
is
.EQ
x sub i sub 1 sup 2
.EN
which is rather different.
.PP
Braces can occur within braces:
.P1
e sup {i pi sup {rho +1}}
.P2
is
.EQ
e sup {i pi sup {rho +1}}
.EN
The general rule is that anywhere you could use some single
thing like
.CW x ,
you can use an arbitrarily complicated thing if you enclose
it in braces.
.IT Eqn
will look after all the details of positioning it and making
it the right size.
.PP
In all cases, make sure you have the
right number of braces.
Leaving one out or adding an extra will cause
.IT eqn
to complain bitterly.
.PP
Occasionally you will have to
print braces.
To do this,
enclose them in double quotes,
like
.CW \&"{" .
Quoting is discussed in more detail in section 15.
.NH
Shorthand for In-line Equations
.PP
With the
.CW -ms
and
.CW -mm
macro packages,
.CW .EQ
and
.CW .EN
imply a displayed equation.
But in most documents,
it is necessary to follow mathematical conventions
not just in display equations,
but also in the body of the text,
for example by making variable names like $x$ italic.
.PP
.IT Eqn
provides a shorthand for short in-line expressions.
You can define two characters to mark the left and right ends
of an in-line equation, and then type expressions right in the middle of text
lines.
To set both the left and right characters to dollar signs, for example,
add to the beginning of your document the three lines
.EQ
delim off
.EN
.P1
\&.EQ
delim $$
\&.EN
.P2
Having done this, you can then say things like
.P1
.fi
Let $alpha sub i$ be positive,
and let $beta$ be zero.
Then we can show that $alpha sub 1$ is $>=0$.
.P2
.EQ
delim $$
.EN
This works as
you might expect\(emspaces, newlines, and so on are significant
in the text, but not in the equation part itself.
Multiple equations can occur in a single input line.
.PP
Enough room is left before and after a line that contains
in-line expressions
that something tall like
$sum from i=1 to n x sub i$
does not interfere with the lines surrounding it.
.PP
To turn off the delimiters,
.P1
\&.EQ
delim off
\&.EN
.P2
Warning: don't use braces, tildes, circumflexes,
or double quotes as delimiters\(emchaos will result.
.NH
Fractions
.PP
To make a fraction,
use the word
.CW over :
.P1
a+b over 2c = 1
.P2
gives
.EQ
a+b over 2c = 1
.EN
The line is made the right length and positioned automatically.
Braces can be used to make clear what goes over what:
.P1
{alpha + beta} over {sin (x)}
.P2
is
.EQ
{alpha + beta} over {sin (x)}
.EN
What happens when there is both an
.CW over
and a
.CW sup
in the same expression?
In such an apparently ambiguous case,
.IT eqn
does the
.CW sup
before the
.CW over ,
so
.P1
-b sup 2 over pi
.P2
is
$-b sup 2 over pi$
instead of
$-b sup {2 over pi}$.
The rules
that decide which operation is done first in cases like this
are summarized in section 24.
When in doubt, however,
use braces
to make clear what goes with what.
.NH
Square Roots
.PP
To draw a square root, use
.CW sqrt :
.P1 2
sqrt a+b + 1 over sqrt {ax sup 2 +bx+c}
.P2
is
.EQ
sqrt a+b + 1 over sqrt {ax sup 2 +bx+c}
.EN
Warning\(emsquare roots of tall quantities look bad,
because a root-sign
big enough to cover the quantity is
too dark and heavy:
.P1
sqrt {a sup 2 over b sub 2}
.P2
is
.EQ
sqrt {a sup 2 over b sub 2}
.EN
Big square roots are generally better written as something
to the power \(12:
.EQ
(a sup 2 /b sub 2 ) sup half
.EN
which is
.P1
(a sup 2 /b sub 2 ) sup half
.P2
.NH
Summation, Integral, Etc.
.PP
Summations, integrals, and similar constructions
are easy:
.P1
sum from i=0 to {i= inf} x sup i
.P2
produces
.EQ
sum from i=0 to {i= inf} x sup i
.EN
Notice that we used
braces to indicate where the upper
part
$i= inf$
begins and ends.
No braces were necessary for the lower part $i=0$,
because it contained no blanks.
The braces will never hurt,
and if the
.CW from
and
.CW to
parts contain any blanks, you must use braces around them.
.PP
The
.CW from
and
.CW to
parts are both optional,
but if both are used,
they have to occur in that order.
.PP
Other useful characters can replace the
.CW sum
in our example:
.P1
int   prod   union   inter
.P2
become, respectively,
.EQ
int ~~~~~~ prod ~~~~~~ union ~~~~~~ inter
.EN
Since the thing before the
.CW from
can be anything,
even something in braces,
.CW from\f2-\fPto
can often be used in unexpected ways:
.P1
lim from {n \(mi> inf} x sub n =0
.P2
is
.EQ
lim from {n-> inf} x sub n =0
.EN
.PP
Notice the difference between
.CW from\f2-\fPto
and
.CW sub\f2-\fPsup :
.P1
int from a to b ~~~ int sub a sup b
.P2
is
.EQ
int from a to b ~~~ int sub a sup b
.EN
.NH
Size and Font Changes
.PP
By default, equations are set in 
the current point size (this text is 10-point type),
with standard mathematical conventions
to determine what characters are in roman and what in italic.
Although
.IT eqn
makes a valiant attempt to use
aesthetically pleasing sizes and fonts,
it is not perfect.
To change sizes and fonts, use
.CW size 
.IT n
and
.CW roman ,
.CW italic ,
.CW bold
and
.CW fat .
Like
.CW sub
and
.CW sup ,
size
and font changes affect only the thing that follows
them, and revert to the normal situation
at the end of it. Thus
.P1
bold x y
.P2
is
.EQ
bold x y
.EN
and
.P1
size 12 bold x = y +
   size 12 {alpha + beta}
.P2
gives
.EQ
size 12 bold x = y +
   size 12 {alpha + beta}
.EN
As always, you can use braces if you want to affect something
more complicated than a single letter.
For example, you can change the size of an entire equation by
.P1
size 8 { ... }
.P2
You can also change the size
by
a given amount;
for example, you can say
.CW size\ +2
to make the size two points bigger,
or
.CW size\ -3
to make it three points smaller.
This has the advantage that you don't have
to know what the current size is.
.PP
If you are using fonts other than roman, italic and bold,
you can say
.CW font
.IT X
where
.IT X
is a
.IT troff
name or number for the font,
one or two characters long.
Since
.IT eqn
is tuned for roman, italic and bold,
other fonts may not give quite as good an appearance.
.PP
The
.CW fat
operation takes the current font and widens it by overstriking:
.CW fat\ grad
is
$fat grad$ and
.CW fat
.CW "{x sub i}
is
$fat {x sub i}$.
.PP
If an entire document is to be in a non-standard size
or font, it is a severe nuisance
to have to write out a size and font change for each
equation.
Accordingly, you can set a ``global'' size or font
which thereafter affects all equations.
At the beginning of any equation, you might say, for instance,
.P1
\&.EQ
gsize 16
gfont R
\&...
\&.EN
.P2
to set the size to 16 and the font to roman thereafter.
In place of R, you can use any of the
.IT troff
font names.
The size after
.CW gsize
can be a relative change with
.CW +
or
.CW - .
.PP
Generally,
.CW gsize
and
.CW gfont
will appear at the beginning of a document
but they can also appear
throughout a document: the global font and size
can be changed as often as needed.\(dd
.FS
\l'1i'
.br
\(ddIn a footnote like this one, in which we have a
few random
expressions like $x sub i$ and $pi sup 2$,
.IT eqn
handles the size change automatically.
.FE
.PP
Since
.IT eqn
manages most size changes automatically,
.CW gsize
is rarely necessary.
.NH
Diacritical Marks
.PP
To get diacritical marks on top of letters,
there are several words:
.P1
.ta 1i
.vs +3
x dot	$x dot$
x dotdot	$x dotdot$
x hat	$x hat$
x tilde	$x tilde$
x vec	$x vec$
x dyad	$x dyad$
x bar	$x bar$
x under	$x under$
x utilde	$x utilde$
.vs -3
.P2
The diacritical mark is placed at the right height.
The
.CW bar
and
.CW under
are made the right length for the entire construct,
as in $x+y+z bar$;
other marks are centered.
.PP
Sometimes
.IT eqn
guesses wrong on the height for bars.
As an interim fix, the words
.CW highbar
and
.CW lowbar
are synonymous with
.CW bar,
but force the bar to be either high
(as with $X bar$)
or low (as with $x bar$) regardless of the apparent height of the object.
This is useful for constructions like
${x sub 1} lowbar$, which is
.CW "{x sub 1} lowbar" .
.......
.NH
Quoted Text
.PP
Any input entirely within quotes
.CW \&"\f2...\fP"
is not subject to any of the normal font changes and spacing
adjustments.
This provides a way to do your own spacing and adjusting if needed:
.P1
italic "sin(x)" + sin (x)
.P2
is
.EQ
italic "sin(x)" + sin (x)
.EN
.PP
Quotes are also used to get braces and other
.IT eqn
keywords printed:
.P1
"{ size alpha }"
.P2
is
.EQ
"{ size alpha }"
.EN
and
.P1
roman "{ size alpha }"
.P2
is
.EQ
roman "{ size alpha }"
.EN
.PP
The construction
.CW \&""
is often used as a place-holder
when grammatically
.IT eqn
needs something, but you don't actually want anything in your output.
For example, to make
$"" sup 2 roman He$,
you can't just type
.CW "sup 2 roman He
because a
.CW sup
has to be a superscript
.IT on
something.
Thus you must say
.P1
"" sup 2 roman He
.P2
.PP
To get a literal quote
use
.CW \&"\e"" .
If delimiters are set with
.CW delim ,
you can include them in a quoted string too.
.IT Troff
special-character names like
.CW \e(bs
can appear unquoted, but all other
.IT troff
constructions
should
always
be quoted.
.NH
Lining up Equations
.PP
Sometimes it's necessary to line up a series of equations
at some horizontal position, often at an equals sign.
This is done with two operations called
.CW mark
and
.CW lineup .
.PP
The word
.CW mark
may appear once at any place in an equation.
It remembers the horizontal position where it appeared.
Successive equations can contain one occurrence of the word
.CW lineup .
The place where
.CW lineup
appears is made to line up
with the place marked by the previous
.CW mark
if possible.
Thus, for example,
you can say
.P1
\&.EQ I
x+y mark = z
\&.EN
\&.EQ I
x lineup = 1
\&.EN
.P2
to produce
.EQ I
x+y mark = z
.EN
.EQ I
x lineup = 1
.EN
For reasons too complicated to talk about,
when you use
.IT eqn
and
.CW -ms ,
use either
.CW .EQ\ I
or
.CW .EQ\ L ;
.CW mark
and
.CW lineup
don't work with centered equations.
Also bear in mind that
.CW mark
doesn't look ahead;
.P1
x mark =1
\&...
x+y lineup =z
.P2
will not work if there isn't room
for the
.CW x+y
part after the
.CW mark
remembers where the
.CW x
is.
If you use
.CW .EQ
.CW I ,
that may suffice;
if not, use tildes to move the first equation to the right.
.NH
Big Brackets, Etc.
.PP
To get big brackets,
braces, parentheses, and bars
around things, use the
.CW left
and
.CW right
commands:
.P1
left { a over b + 1 right }
 ~=~ left ( c over d right )
 + left [ e right ]
.P2
is
.EQ
left { a over b + 1 right } ~=~ left ( c over d right ) + left [ e right ]
.EN
The resulting brackets are made big enough to cover whatever they enclose.
Other characters can be used besides these,
but are not likely to look very good.
The
.CW floor
and
.CW ceiling
characters are exceptions:
.P1 .2i
left floor x over y right floor <= 
left ceiling a over b right ceiling
.P2
produces
.EQ
left floor x over y right floor
<= left ceiling a over b right ceiling
.EN
.PP
Several warnings about brackets are in order.
First, braces are typically bigger than brackets and parentheses,
because they are made up of three, five, seven, etc., pieces,
while brackets can be made up of two, three, etc.
Second, big left and right parentheses often look poor,
because the character set is poorly designed.
.PP
The
.CW right
part may be omitted:
a ``left something'' need not have a
corresponding
``right
something''.
If the
.CW right
part is omitted,
put braces around the thing you want the left bracket
to encompass.
Otherwise, the resulting brackets may be too large.
.PP
If you want to omit the
.CW left
part, things are more complicated,
because technically you can't have a
.CW right
without a corresponding
.CW left .
Instead you have to say
.P1
left "" ... right )
.P2
for example.
The
.CW
left ""
.R
means a ``left nothing''.
This satisfies the rules without hurting your output.
.NH
Piles
.PP
There is a general facility for making vertical piles
of things; it comes in several flavors.
For example:
.P1
A ~=~ left [
  pile { a above b above c }
  ~~ pile { x above y above z }
right ]
.P2
will make
.EQ
A ~=~ left [
pile { a above b above c } ~~ pile { x above y above z }
right ]
.EN
The elements of the pile (there can be as many as you want)
are centered one above another, at the right height for
most purposes.
The keyword
.CW above
is used to separate the pieces;
braces are used around the entire list.
The elements of a pile can be as complicated as needed, even containing more piles.
.PP
Three other forms of pile exist:
.CW lpile
makes a pile with the elements left-justified;
.CW rpile
makes a right-justified pile;
and
.CW cpile
makes a centered pile, just like
.CW pile.
The vertical spacing between the pieces
is somewhat larger for
.CW l ,
.CW r ,
and
.CW cpiles
than it is for ordinary piles.
.P1 .1i
roman sign (x)~=~
left {
   lpile {1 above 0 above -1}
   ~~ lpile
    {if~x>0 above if~x=0 above if~x<0}
.P2
makes
.EQ
roman sign (x)~=~
left {
   lpile {1 above 0 above -1}
   ~~ lpile
    {if~x>0 above if~x=0 above if~x<0}
.EN
Notice the left brace
without a matching right one.
.PP
It is possible to change the default vertical separation
between elements of a pile,
by adding a number immediately after the word
.CW lpile ,
etc.  The number is in units of 1/100's of an em
(an em is about the width of the letter `m');
.CW "lpile 33
makes the spacing 1/3 of an em.
.NH
Matrices
.PP
It is also possible to make matrices.
For example, to make
a neat array like
.EQ
matrix {
  ccol { x sub i above y sub i }
  ccol { x sup 2 above y sup 2 }
}
.EN
you have to type
.P1
matrix {
  ccol { x sub i above y sub i }
  ccol { x sup 2 above y sup 2 }
}
.P2
This produces a matrix with
two centered columns.
The elements of the columns are then listed just as for a pile,
each element separated by the word
.CW above.
You can also use
.CW lcol
or
.CW rcol
to left or right adjust columns.
Each column can be separately adjusted,
and there can be as many columns as you like.
.PP
The words
.CW pile
and
.CW col ,
.CW lpile
and
.CW lcol ,
etc., are synonyms.
.PP
The reason for using a matrix instead of two adjacent piles, by the way,
is that if the elements of the piles don't all have the same height,
they won't line up properly.
A matrix forces them to line up,
because it looks at the entire structure before deciding what
spacing to use.
.PP
A word of warning about matrices\(emeach column must have the same number of elements in it.
The world will end if you get this wrong.
.NH
Local Motions
.EQ
delim $$
.EN
.PP
Although
.IT eqn
tries to print things at the right place,
it isn't perfect, and occasionally you will need to tune
the output to make it just right.
Small extra horizontal spaces can be obtained with
tilde and circumflex.
You can also say
.CW back
.IT n
and
.CW fwd
.IT n
to move small amounts horizontally.
.IT n
is how far to move in 1/100's of an em.
Thus
.CW back
.CW 50
moves back about half the width of an m.
For example,
.CW <<
produces
$<<$, while
.CW "< back 50 <
produces $< back 50 <$.
.PP
Similarly you can move things up or down with
.CW up
.IT n
and
.CW down
.IT n .
.P1
x sub down 20 i sup up 20 j
.P2
produces $x sub down 20 i sup up 20 j$ instead of the usual $x sub i sup j$.
.PP
As with
.CW sub
or
.CW sup ,
the local motions affect the next thing in the input,
and this can be something arbitrarily complicated if it is enclosed
in braces.
.NH
Definitions
.PP
.IT Eqn
provides a facility so you can give
a frequently-used string of characters a name,
and thereafter just type the name instead of the
whole string.
For example, if the sequence
.P1
x sub i + y sub i
.P2
appears repeatedly throughout a paper,
you can save re-typing it each time by defining it like this:
.P1 2
define xy 'x sub i + y sub i'
.P2
This makes
.CW xy
a shorthand for whatever characters occur between the single quotes
in the definition.
You can use any character instead of quote to mark the ends of the definition,
so long as it doesn't appear inside the definition.
.PP
Now you can use
.CW xy
like this:
.P1
\&.EQ
f(x) = xy ...
\&.EN
.P2
and so on.
Each occurrence of
.CW xy
will expand into what it was defined as.
Be careful to leave spaces or their equivalent
around the name
when you actually use it, so
.IT eqn
will be able to identify it as special.
.PP
There are several things to watch out for.
First, although definitions can use previous definitions,
as in
.P1
\&.EQ
define xi ' x sub i '
define xi1 ' xi sub 1 '
\&.EN
.P2
.ul
Don't define something in terms of itself!
A favorite error is to say
.P1
define X ' roman X '
.P2
This is a guaranteed disaster,
since
.CW X
is
now defined in terms of itself.
If you say
.P1
define X ' roman "X" '
.P2
however, the quotes
protect the second 
.CW X ,
and everything works fine.
.PP
.IT Eqn
keywords can be redefined.
You can make
.CW /
mean
.CW over
by saying
.P1
define / ' over '
.P2
or redefine
.CW over
as
.CW /
with
.P1
define over ' / '
.P2
.PP
.EQ
delim off
.EN
It is possible to create a parameterized definition, which will
expand into something different each time it is used.
If a definition contains occurrences of
.CW $1 ,
.CW $2 ,
etc., as in
.P1
define f 'font $1 {"$2"}'
.P2
then subsequent uses of that name that are followed by a parenthesized
list of values will have the values substituted for the
.CW $\f2n\fP\f1's:\fP
.P1
f(CW, a phrase)
.P2
will print
.CW "a phrase
in the constant-width font
.CW CW .
.PP
You can test whether something is defined with
.CW ifdef :
.P1
ifdef f / \f2anything at all\fP /
.P2
is replaced by
.IT "anything at all
if
.CW f
is defined.
.NH
File Inclusion
.PP
You can include a file of
.IT eqn
input as part of an equation by the construction
.P1
include "\f2filename\fP"
.P2
For historical reasons, 
.CW copy
is a synonym for
.CW include .
.NH
A Large Example
.PP
Here is the complete source for the three display equations
in the abstract of this guide.
.br
.ps -3
.vs -3
.P1 0
\&.EQ I
G(z)~mark =~ e sup { ln ~ G(z) }
~=~ exp left (
sum from k>=1 {S sub k z sup k} over k right )
~=~  prod from k>=1 e sup {S sub k z sup k /k}
\&.EN
\&.EQ I
lineup = left ( 1 + S sub 1 z +
{ S sub 1 sup 2 z sup 2 } over 2! + .\^.\^. right )
left ( 1+ { S sub 2 z sup 2 } over 2
+ { S sub 2 sup 2 z sup 4 } over { 2 sup 2 cdot 2! }
+ .\^.\^. right ) .\^.\^.
\&.EN
\&.EQ I
lineup =  sum from m>=0 left (
sum from
pile { k sub 1 ,k sub 2 ,\^.\^.\^.\^, k sub m  >=0
above
k sub 1 +2k sub 2 + .\^.\^. +mk sub m =m}
{ S sub 1 sup {k sub 1} } over {1 sup k sub 1 k sub 1 ! } ~
{ S sub 2 sup {k sub 2} } over {2 sup k sub 2 k sub 2 ! } ~
\&.\^.\^.
{ S sub m sup {k sub m} } over {m sup k sub m k sub m ! }
right ) z sup m
\&.EN
.P2
.ps +3
.vs +3
......
.NH
Keywords, Precedences, Etc.
.PP
Here are the keywords of
.IT eqn
in order of decreasing precedence:
.P1 .1i
dyad vec under utilde bar tilde
         hat dot dotdot
left  right
fwd   back   down  up
fat   roman  italic  bold  size
sub   sup
sqrt  over
from  to
.P2
These operations group to the left:
.P1 .1i
over  sqrt  left  right
.P2
All others group to the right.
.PP
Digits, parentheses, brackets, punctuation marks, and these mathematical words
are converted
to Roman font when encountered:
.P1 .1i
sin  cos  tan  sinh  cosh  tanh  arc
max  min  lim  log   ln    exp
Re   Im   and  if    for   det
.P2
These character sequences are recognized and translated as shown.
.EQ
delim $$
.EN
.P1
>=	$>=$	nothing	$nothing$
<=	$<=$	cdot	$cdot$
==	$==$	times	$times$
!=	$!=$	del	$del$
+-	$+-$	grad	$grad$
->	$->$	\&...	$"..."$
<-	$<-$	,...,	$",...,"$
<<	$<<$	sum	$sum$
.sp 3p
>>	$>>$	int	$int$
inf	$inf$	prod	$prod$
partial	$partial$	union	$union$
half	$half$	inter	$inter$
prime	$prime$	dollar	$dollar$
approx	$approx$
.P2
.PP
To obtain Greek letters,
simply spell them out in whatever case you want:
.P1
.ta .7i 1.4i 2.1i
DELTA	$DELTA$	iota	$iota$
GAMMA	$GAMMA$	kappa	$kappa$
LAMBDA	$LAMBDA$	lambda	$lambda$
OMEGA	$OMEGA$	mu	$mu$
PHI	$PHI$	nu	$nu$
PI	$PI$	omega	$omega$
PSI	$PSI$	omicron	$omicron$
SIGMA	$SIGMA$	phi	$phi$
THETA	$THETA$	pi	$pi$
UPSILON	$UPSILON$	psi	$psi$
XI	$XI$	rho	$rho$
alpha	$alpha$	sigma	$sigma$
beta	$beta$	tau	$tau$
chi	$chi$	theta	$theta$
delta	$delta$	upsilon	$upsilon$
epsilon	$epsilon$	xi	$xi$
eta	$eta$	zeta	$zeta$
gamma	$gamma$
.P2
.IT Eqn
really ought to know the whole Greek alphabet,
but the missing upper-case characters are identical to Roman ones.
.PP
These are all the words known to
.IT eqn ,
except for characters with names.
.P1
above	fat	lcol	size
back	font	left	space
bar	from	lineup	sqrt
bold	fwd	lowbar	sub
ccol	gfont	lpile	sum
col	gsize	mark	sup
copy	hat	matrix	tilde
cpile	highbar	over	to
define	ifdef	pile	under
delim	include	prod	union
dot	int	rcol	up
dotdot	integral right	utilde
down	inter	roman	vec
dyad	italic	rpile
.P2
.NH
Troubleshooting
.PP
If you make a mistake in an equation,
like leaving out a brace (very common)
or having one too many (very common)
or having a
.CW sup
with nothing before it (common),
.IT eqn
will tell you with the message
.P1 2
syntax error near line \f2n\fP, file \f2f\fP,
  context is >>> \f2something\fP <<<
.P2
where
.IT n
is approximately the line
where the trouble occurred,
.IT f
is the name
of the input file, and
.IT something
is a guess about what input was in error.
The line number and context is approximate\(emlook nearby as well.
There are also self-explanatory messages that arise if you leave out a quote
or try to run
.IT eqn
on a non-existent file.
.PP
If you want to check a document before actually printing it,
.P1
eqn files >/dev/null
.P2
will
throw away the output but print the messages.
.PP
If you use something like dollar signs as delimiters,
it is easy to leave one out.
This causes very strange troubles.
The program
.CW checkeq
checks for misplaced or missing delimiters
and similar troubles.
.PP
In-line equations can only be so big
because of an internal limit in
.IT troff .
If you get the message
``word overflow,''
you have exceeded this limit.
If you print the equation as a displayed equation
this message will usually go away.
The message
``line overflow''
indicates you have exceeded a different, bigger internal limit.
The only cure for this is to break the equation into two separate ones.
.PP
On a related topic,
.IT eqn
does not break equations by itself\(emyou must split long equations up across multiple lines
by yourself,
marking each by a separate
.CW .EQ
\&...\&
.CW .EN
sequence.
.NH
Use on UNIX
.PP
To print a document that contains mathematics,
.P1
eqn files | troff
.P2
If
there are any
.IT troff
options, they go after the
.IT troff
part of the command. For example,
.P1
eqn files | troff -ms
.P2
.IT Eqn
assumes the output is aimed at a Postscript device.
If you are using some other typesetter or printer,
you have to tell 
.IT eqn
with the same
.CW -T
argument that you use with
.IT troff :
.P1
eqn -Taps files | troff -Taps ...
.P2
.IT Eqn
also uses the environment variable
.CW TYPESETTER ;
the default is
.CW post ,
as it is for
.IT troff .
.PP
.IT Eqn
can be used with the
.IT tbl
program
for setting tables that contain mathematics.
Use
.IT tbl
before
.IT eqn ,
like this:
.P1
tbl files | eqn | troff
.P2
.PP
When
.IT eqn
is used with
.IT pic
or
.IT grap ,
the additional vertical space
that
.IT eqn
adds to a line to keep it from colliding
with adjacent lines will conflict with
.IT pic
and
.IT grap
spacing.
The phrase
.CW "space 0
at the beginning of an equation will turn off the extra space.
.SH
Acknowledgements
.PP
We are deeply indebted to the late J. F. Ossanna,
the author of
.IT troff ,
for his willingness to extend
.IT troff
to make our task easier,
and for his
continuous assistance during the development
and evolution
of
.IT eqn .
We are also grateful to A. V. Aho
for advice on language design,
to S. C. Johnson for assistance with
the
.IT yacc
compiler-compiler,
and to all the
.IT eqn
users
who have made helpful suggestions and criticisms.
.SH
References
.LP
|reference_placement
.SH
Appendix 1. Tuning \fIeqn\fP Output
.PP
There are numerous values that can be adjusted to tune the output
of 
.IT eqn
for a particular output device.
In general, it is necessary to have access to the source code
to do this tuning.
.PP
Some values can be set without access to the source.
The names shown below (together with their default values) control the positioning
of diacritical marks and special characters.
They can be defined with any string value,
as in
.P1 0
define vec_def " \ev'-.5m'\es-3\e(->\es0\ev'.5m'
.P2
.1C
.KF bottom
.nr dP 1
.nr dV 2p
.SP
.P1 20n
vec_def		\ev'-.5m'\es-3\e(->\es0\ev'.5m'
dyad_def	\ev'-.5m'\es-3\ez\e(<-\e|\e(->\es0\ev'.5m'
hat_def		\ev'-.05m'\es+1^\es0\ev'.05m'
tilde_def	\ev'-.05m'\es+1~\es0\ev'.05m'
dot_def		\ev'-.67m'.\ev'.67m'
dotdot_def	\ev'-.67m'..\ev'.67m'
utilde_def	\ev'1.0m'\es+2~\es-2\ev'-1.0m'
sum_def		\e|\ev'.3m'\es+5\e(*S\es-5\ev'-.3m'\e|
union_def	\e|\ev'.3m'\es+5\e(cu\es-5\ev'-.3m'\e|
inter_def	\e|\ev'.3m'\es+5\e(ca\es-5\ev'-.3m'\e|
prod_def	\e|\ev'.3m'\es+5\e(*P\es-5\ev'-.3m'\e|
int_def		\ev'.1m'\es+4\e(is\es-4\ev'-.1m'
Subbase		0.2
Supbase		0.4
.P2
.KE
.2C
The amount by which a subscript is shifted down
and a superscript shifted up are controlled by
.CW Subbase
and
.CW Supbase .

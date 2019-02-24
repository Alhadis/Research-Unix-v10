.so ../ADM/mac
.XX index 609 "Index"
.TL
Index
.AU
L. L. Cherry
.AI
.MH
.AB
.AE
.2C
.NH
Introduction
.PP
Because the index that follows was generated mechanically,
a description of its layout and contents will help
you use it more effectively.
I'll describe how the index is organized,
how the terms were generated, how the index was generated from
the terms, and finally, the difficulties.
.NH
The Organization
.PP
Each of the papers is assigned a nickname that
is printed in italic to the left of the title in the
Table of Contents and in the header of the paper.
So, for example, the paper 
"A System for
Algorithm Animation" is called \fIanim\fR and
the paper ``A Troff Tutorial'' is called \fItrofftut\fR.
In the index the paper nicknames always appear in italics.
Literals, which may be file names, command names or keywords
in a language, appear in constant width, i.e. \f(CW/bin\fR,
\f(CWawk\fR, \f(CWcircle\fR.
The terms in the index are not fully cross-referenced.
Each paper appears alphabetically under its nickname, followed by an
indented mini-index of terms specific to that paper.
Generally, these terms are not cross-referenced in the top or global level.
So, for example, the terms \f(CWagain\fR, \f(CWbackward\fR,
and ``current view'' in \fIanim\fR only appear indented under \fIanim\fR.
Terms you might look up to determine whether a topic is
discussed anywhere in this volume, however, are fully cross-referenced,
like ``Movie Program'' or ``binary search tree.''
File names and program names are also indexed globally.
Terms that would be redundant or useless under the specific
paper only appear at the global
level.
So, ``Algorithm Animation'' is not in the mini-index under \fIanim\fR.
.NH
Getting the terms
.PP
The terms for the index were generated mechanically
from the text of the papers.
For each paper, a file containing the title and headings, repeated noun phrases,
and distinguished words was generated.
By distinguished words, I mean those that appear in the paper in
a distinguishing font like italics, bold or constant width.
This file was than manually edited to a reasonable list of
indexing terms.
Finally, the terms were coded with indexing level (only global, only specific or
fully cross-referenced) and font information.
.NH
Index Generation
.PP
Production of the index was completely automatic once the terms were generated
and edited.
A form of the \fIdiction\fP program that does pattern matching on English
text, mapping upper case letters to lower case and punctuation to blanks,
located the terms in the text of the paper.
Using this location information, \f(CW.Tm\fR commands
were inserted in the paper to cause \fItroff\fR to print the page number
for each term.
An \fIawk\fR program combined the page numbers for each term and
generated the global and local versions of the indexing term for each paper.
For example, in \fIbackup\fR, references to \f(CW/n\fR, the network
file system, appear in the file generated
by \fItroff\fR as,
.P1
596	/n	c
597	/n	c
602	/n	c
.P2
and become two indexing entries
.P1
/n, backup, 596-597, 602	c
backup, /n, 596-597, 602	c
.P2
The \f(CWc\fR is a code for constant width font.
.PP
Finally the indexes for all of the papers are sorted into one index
and another \fIawk\fR program merges like terms and adds font changes.
So for the term \f(CW/n\fR, which is referenced in \fIbackup\fR, \fInetb\fR, and \fIsetup\fR,
we get
.DS
\f(CW/n\fP, \fIbackup\fR, 596-597, 602
    \fInetb\fR, 519
    \fIsetup\fR, 499-500
.DE
at the global level and \f(CW/n\fR also appears in the mini-indexes
for \fIbackup\fR, \fInetb\fR, and \fIsetup\fR.
.NH
Difficulties
.PP
Because the terms are generated directly from the text
and the text was written by 40 authors,
language choices differ among the papers, and hence
among the terms in the index.
To locate a paper of interest, you may have to look
in the vicinity of the term.
For example, although there are 14 papers that deal with
``typesetting'', not all of them explicitly include the term.
So we have indexing terms ``typeset,'' ``typesetter'' and
``typesetter graphics.''
.PP
Terms as they appear in the papers may not be in the same
font or case as they appear in the index.
There are several reasons for this.
First the matching is done solely in lower case
in order to match terms that begin sentences
or appear in headings.
Terms that were derived from headings
have their upper case letters restored but may appear in the text
of the paper in lower case as well.
For some indexing terms case is significant, like the \f(CWd\fR and \f(CWD\fR
commands in \fIsam\fR, but unfortunately lost.
Both commands are mapped to lower case.
Because font changes in the index carry information
(paper nickname, literal) the font in the index may differ
from that used in the paper.
Although most of language papers use constant width font for
literals, UNIX commands appear in italic.
In the index UNIX commands are in constant width.
.PP
Where possible, indexing terms are matched in programming
examples as well as in the text of the papers.
For some terms, like the \f(CWif\fR statement in \fIspin\fR,
this is not possible without matching every occurrence
of the word ``if'' in the text.
.PP
Finally, the page number of the term in the index may be off by one,
referring to the page before the actual occurrence of the term.
When the terms are inserted in the text of the papers
for \fItroff\fR to compute their page numbers, they are
added in front of the sentence in which they occur.
If this sentence begins on the preceding page, then
the term will be attributed to that page rather than
the one on which it physically appears.
.PP
If you remember to look in the vicinity of
the term you're looking for in the index and not
expect the font or case to be identical in the paper,
you should find the information you want in this volume.
.NH
Acknowledgements
.PP
I am indebted to Doug McIlroy, N. Peter Nelson
and Lloyd Nakatani for looking at early
versions of the index and for their helpful suggestions
on its form.
....
.... augmented term in index - 5620 ->5620 terminal
..... sometimes lc used to enhence folding of terms

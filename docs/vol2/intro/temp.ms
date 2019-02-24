.so ../ADM/mac
.XX intro 1 "Introduction"
.TL
Introduction
.AU
Andrew G. Hume
.AI
.MH
.SP
.2C
.NH
Introduction
.PP
This volume contains reports, papers and manuals that supplement
the manual pages in Volume 1 of this manual.
The selection criteria are not well defined but include utility,
general interest, and unavailability in the published literature.
Some authors were cajoled into writing papers specially for this volume;
otherwise, the papers were chosen from the existing literature,
both external and internal to Bell Laboratories.
(Most existing papers and reports were extensively revised for this volume.)
Accordingly, several papers (such as the
.I "Uucp Administration"
paper) do not match exactly the software in the current system.
The online manual pages should be consulted before depending
on any specific details contained in any of the papers in this volume.
.PP
The documents here are grouped roughly into
the areas of
programming,
language tools,
document preparation,
and
system maintenance.
Many documents from the last Volume 2 (Seventh Edition, 1979) have been dropped.
Some are simply obsolete; others, particularly the introductory papers,
have been superseded by better and more up to date books on the
.UX
system.
The volume has not shrunk;
for every paper that has been dropped,
a new program or paper has appeared.
.NH
Production
.PP
The papers in this volume have not simply been reprinted.
Most have been converted to use the
.I mpm (7)
macros (see the
.I mpm
paper).
The rest of the papers use
.I monk (1)
or
.I tex (1).
The papers use a two column format except when liberal use
of full-width displays make this inappropriate.
This not only makes the volume look better; it also makes it about
100 pages thinner.
.PP
The papers also try to follow Volume 1 in the typographical representation of names.
That is, programs referred to by name are in italics and are capitalized
when starting a sentence (like any other noun).
Literal filenames,
program fragments and things the user would type are in typewriter font.
As far as possible, typographic flourishes are eschewed, so that
\s-1L\h'-.12m'\v'-.17m'A\v'.17m'\h'-.12m'T\v'.17m'\h'-.12m'E\h'-.12m'\v'-.17m'X\s+1
is rendered as Latex.
Long lines broken for readability use \*(cr to indicate the breaks:
.P1
this is one line which has\*(cr
been broken because of the column\*(cr
width
.P2
Most papers needed significant editing to conform to this standard;
some of the larger older papers were not changed.
.PP
The layout chosen for this volume implies that all papers must have
an even number of pages.
Where necessary, papers have been extended by digitized photographs
transformed by Gerard Holzmann using
.I pico .
A few photographs were chosen for sentimental and historical reasons;
the rest mostly for aesthetic appeal although we tried to include
most of the authors of the papers in this volume.
.PP
The index was produced mechanically by Lorinda Cherry using a small amount
of hand editing and a considerable amount of software;
details are given as a preamble to the index.
Our Center's venerable Linotron 202 typesetter is being retired;
the camera-ready copy for this manual was prepared on a
Linotronic 200P PostScript printer running at 1270 dpi.
.de X1
.IP "" 3n
..
.de X2
..
.NH
A Selected Bibliography
.PP
During the fourteen years since a volume of ``Documents for use with UNIX''
was first gathered for the sixth edition of the
.I "UNIX Programmer's Manual, 
the system has grown, and so has its literature.
In selecting the contents for this volume,
we have omitted topics such as software design and the use of
programming languages;
good books on these are available elsewhere.
A list of the more important books follow.
For references on other special languages see
.I langs (1).
.NH 2
General
.LP
|reference(kernighan pike %no_cite) |reference_placement
.X1
How to exploit the facilities
of the system: utilities, shell, and system calls.
Written for programmers; intended for novices but
yet also contains nuggets for experts.
.X2
.LP
|reference(lomuto %no_cite) |reference_placement
.X1
Written for nonprogrammers; readable and authoritative.
Stresses text processing.
.X2
.LP
|reference(unix readings %no_cite) |reference_placement
.X1
Papers reprinted from special issues of the
.I "Bell System Technical Journal,
August, 1978, and October, 1984.
Includes classics on the design and history of the
system in its many incarnations at Bell Laboratories.
.X2
.LP
|reference(nemeth administration %no_cite) |reference_placement
.X1
A comprehensive guide to administering your
.UX
system.
.NH 2
C Programming
.LP
|reference(
%type book
%author B. W. Kernighan
%author D. M. Ritchie
%title The C Programming Language
%publisher Prentice Hall
%address Englewood Cliffs, NJ
%date 1978
%other 1st Edition
%no_cite) |reference_placement
.X1
The bible for the classical language and how to use it.
.X2
.LP
|reference(cbook %no_cite) |reference_placement
.X1
The new testament covering the new American National
Standard dialect of C.
.X2
.LP
|reference(harbison steele %no_cite) |reference_placement
.X1
The most exact description of what C really was before the
American National Standard.
.X2
.LP
|reference(cplusplus %no_cite) |reference_placement
.X1
A major, largely compatible, ``object-oriented'' extension to C
that incorporates user-defined types and operations.
.X2
.LP
|reference(ansic %no_cite) |reference_placement
.X1
The future Talmud.
.NH 2
System Manuals
.LP
|reference(v7hrw %no_cite) |reference_placement
.X1
The seventh edition of this manual.
Volume 2 contains more discursive
papers on the use and implementation of
.I adb, 
.I bc,
.I cc,
.I dc,
.I lint,
.I m4,
.I make,
.I ratfor,
.I sed,
and other facilities.
.X2
.LP
|reference(bsdmanual %no_cite) |reference_placement
.X1
The Berkeley descendant of the seventh edition.
Volume 2C contains descriptions of 
.I vi
and
.I "Franz Lisp.
.X2
.LP
|reference(svid %no_cite) |reference_placement
.X1
The official description of what makes a commercial offering
qualify as a 
.UX
system.
.X2
.LP
|reference(ieee1003.1 %no_cite) |reference_placement
.X1
A standardized vendor-neutral description of
.UX
kernel functionality.
.NH 2
Implementation
.LP
|reference(bach unix %no_cite) |reference_placement
.X1
How System V works.
.LP
|reference(lefler karels implementation %no_cite) |reference_placement
.X1
How the other half lives.
.X2
.LP
|reference(stevens network programming %no_cite) |reference_placement
.X1
A guide to how
.UX
programs access IPC and networks.
.NH 2
Special Languages and Applications
.X2
.LP
|reference(awkbook %no_cite) |reference_placement
.X1
Suggestive examples of how to exploit an unusual language.
.X2
.LP
|reference(mllanguage %no_cite) |reference_placement
.X1
An elegant interactive language based on the latest theory
of higher level types.
.X2
.LP
|reference(slanguage %no_cite) |reference_placement
.X1
Just what the title says it is.
.X2
.LP
|reference(port3 %no_cite) |reference_placement
.X1
A comprehensive library of Fortran subroutines for approximation,
ordinary and partial differential equations, linear algebra,
optimization, quadrature, special functions and FFTs.
.X2
.LP
|reference(texbook knuth %no_cite) |reference_placement
.X1
A modern alternative to
.I troff
and
.I eqn ,
by the master of computer science.
.X2
.LP
|reference(latex %no_cite) |reference_placement
.X1
.I LaTeX
is to 
.I TeX
as 
.CW -ms
and 
.I refer
are to 
.I troff.
.X2
.LP
|reference(bolsky vi %no_cite) |reference_placement
.X1
At 66 pages, one of the shorter descriptions of this big,
yet popular, program.
.X2
.LP
|reference(wwb %no_cite) |reference_placement
.X1
Complete detail about
.I wwb (1).
.BP
photo page
.BP
divider with title
.sp
.ce
Document Preparation

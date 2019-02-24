.so ../ADM/mac
.XX \& 3 "Contents"
.af PN i
.nr n 0 +1
.nr Z1 1.7c	\" left margin for titles
.nr Z2 .5c+\w'000'	\" indent from right margin of right margin for titles
.nr Z3 2.5c	\" left margin for description
.nr Z4 3c	\" indent from right margin of right margin for description
.nr Z5 4.5c	\" left margin of continuation of titles
.\" title, start of authors
.de XX
.nr n +1
.LP
.nr XY \\$2	\" page number
.ad l
.mk x		\" mark current vertical spot in x
.ll \\n(LLu-\\n(Z2u
.in \\n(Z5u
.ti 0
\\f(NI\\$1\\fP\\h'|\\n(Z1u'\\$4\\$5.
.\" was =\\nn=0=\\$3=\\$4
.tm =\\$1=0=\\$3=\\$4
.ft I
..
.\" end of authors, start of description
.de X1
.mk y
.\" now set up z to be length of first line
.ie \\nyu-\\nxu .nr z \\n(.nu
.el .nr z \\n(.ku
.\".nr z -\\n(.iu
.ft R
.nf
.nr XZ \\n(LLu-\w'\\n(XY'u-\\nzu-\w'\^\^'
.sp |\\nxu
\\h'\\nzu-\\n(.iu'\^\\l'\\n(XZu'\^\\n(XY
.nr y +1v
.sp |\\nyu
.fi
.nr z +\\n(.iu
.in \\n(Z3u
.ll \\n(LLu-\\n(Z4u
.\"z=\\nzu ll=\\n(LL XY=\\n(XY in=\\n(Z6
..
.de X2
.in 0n
.ll \\n(LLu
..
.de X3
.SP .25i exactly
..
.TL
Table of Contents
.SP 2
.nh
.LP
.XX intro 1 intro/intro.ms "Introduction"
A. G. Hume.
.X1
Supplementary material for the Research
.UX
system.
.X2
.X3
.SH
Document Preparation
.XX ms 7 ms/ms "Typing Documents on the UNIX System" ": Using the \f(CW-ms\fP and \f(CW-mcs\fP Macros with Troff"
L.\ L.\ Cherry and
M. E. Lesk.
.X1
The basic
.I troff
macro packages: the 
.CW -ms
document style is useful for most purposes,
and
.CW -mcs
handles Bell Labs cover sheets.
.X2
.XX pm 21 pm/pm.ms "The \f(CW\-mpm\fP Macro Package"
B. W. Kernighan and
C. J. Van Wyk.
.X1
Describes the
.CW -mpm
macros,
a version of
.CW -ms
that does automatic page balancing.
.X2
.XX eqn 25 eqn/guide.ms "Typesetting Mathematics \(em User's Guide"
B. W. Kernighan and L. L. Cherry.
.X1
Describes
.I eqn ,
an easy-to-learn language for doing high-quality mathematical typesetting.
.X2
.XX tbl 35 tbl/tbl.ms "Tbl \(em A Program to Format Tables"
L. L. Cherry and
M. E. Lesk.
.X1
A program to permit easy specification of tabular material
for typesetting.
Again, easy to learn and use.
.X2
.XX pic 53 pic/pic.ms "Pic \(em A Graphics Language for Typesetting"
B. W. Kernighan
.X1
A language for describing line drawings.
.I Pic
descriptions included
in manuscripts make high-quality drawings
when typeset.
.X2
.XX ideal 79 ideal/user.ms "Ideal \(em A Picture-specification Language"
C. J. Van Wyk
.X1
This relative of
.I pic
specifies pictures with constraints and equations.
Although harder to use than
.I pic ,
it supports generalized pens, region filling and opaqueing.
.X2
.XX grap 109 grap/paper.ms "Grap \(em A Language for Typesetting Graphs"
J. L. Bentley and
B. W. Kernighan.
.X1
Converts numerical data into beautifully typeset
graphs and charts.
Instructions to
.I grap
may be included in any manuscript.
.X2
.XX dag 147 dag/dag.ms "Dag \(em A Program for Drawing Directed Graphs"
E. R. Gansner,
S. C. North, and
K.\ P.\ Vo.
.X1
Given the connection list (and labels) for a graph,
.I dag
generates an uncluttered layout for typesetting.
.X2
.XX prefer 163 prefer/prefer.m "Formatting References with Prefer"
M. A. Derr.
.X1
.I Prefer ,
the modern replacement for
.I refer ,
replaces symbolic citations in
a text with properly sequenced bibliographic references
and prepares a bibliography.
.X2
.XX cip 175 cip/cip.ms "Cip User's Manual" ": One Picture is Worth a Thousand Words"
S. A. Browning.
.X1
How to interactively draw
.I pic
pictures for your documents on a screen instead
of programming them.
.X2
.XX troffref 187 troff/man.ms "Troff User's Manual"
J. F. Ossanna and
B. W. Kernighan.
.X1
The basic text-formatting program.
.X2
.XX trofftut 223 troff/tut.ms "A Troff Tutorial"
B. W. Kernighan.
.X1
An introduction to
.I troff
for those who
really want to know such things.
.X2
.XX monk 239 monk/monk.m "Typing Documents on the UNIX System: Using Monk 0.6"
S. L. Murrel and
T.\ J.\ Kowalski.
.X1
How to format documents with
.I troff
without ever writing
.I troff
code.
.X2
.XX latex 259 latex/latex.tex "Latex User Guide"
H. Trickey.
.X1
Local variations to the Latex book.
.X2
.X3
.SH
Programming
.XX mk 269 mk/mk.ms "Mk \(em A Successor to Make"
A. G. Hume.
.X1
The best tool for making sure that large programs
are properly compiled with minimal effort.
.X2
.XX rc 283 rc/rc.ms "Rc \(em A Shell for Plan 9 and UNIX"
T. Duff.
.X1
The next stage in symbolic shells.
.X2
.XX snocone 297 snocone/snocone.ms "The Snocone Programming Language"
A. Koenig.
.X1
A
.SM SNOBOL
with syntactic sugar.
.X2
.XX f77 311 f77/f77.ms "A Portable Fortran 77 Compiler"
S. I. Feldman and P. J. Weinberger.
.X1
Details about the local Fortran.
.X2
.XX cin 325 cin/cin.m "The C Interpreter: A Tutorial for Cin Version 0.18"
T. J. Kowalski,
H. H. Goguen, and
J.\ J.\ Puttress.
.X1
A surprisingly effective approach to C.
Interpreted code can be combined with compiled code for both flexibility
and speed.
.X2
.XX pi 335 pi/feel.ms "The Feel of Pi"
T. A. Cargill.
.X1
How to use an amazing multi-window debugger for C programs.
.X2
.X3
.SH
Supporting Tools and Languages
.XX yacc 347 yacc/yacc.ms "Yacc: A Parser Generator"
S. C. Johnson and
R. Sethi.
.X1
Converts a BNF specification of a language
and semantic actions written in C into a compiler for the language.
.X2
.XX lex 375 lex/lex.ms "Lex \(em A Lexical Analyzer Generator"
M. E. Lesk and E. Schmidt.
.X1
Creates a recognizer for a set of regular expressions;
each regular expression can be followed by arbitrary C code
which will be executed when the regular expression is found.
.X2
.XX sed 389 sed/sed.ms "Sed \(em A Non-interactive Text Editor"
L. E. McMahon.
.X1
A variant of the editor for hands-off processing
in a single pass.
.X2
.XX sam 399 sam/sam.ms "The Text Editor Sam"
R. Pike.
.X1
The design and use of a remarkable programmable editor with a slick mouse interface.
.X2
.XX pico 423 pico/pico.ms "Pico \(em A Language For Composing Digital Images"
G. J. Holzmann.
.X1
A sophisticated, programmable transformer of digitized pictures.
.X2
.XX spin 429 spin/spin.ms "Spin \(em A Protocol Analyzer"
G. J. Holzmann.
.X1
A small and powerful tool for the analysis of distributed systems.
.X2
.XX anim 451 anim/anim.ms "A System for Algorithm Animation"
J. L. Bentley and
B. W. Kernighan.
.X1
A system for displaying data that varies over time.
Outputs include an interactive viewer and snapshots
for inclusion in a
.I troff
document.
.X2
.XX music 477 music/music.ms "Computer Music Under the 10th Edition UNIX System"
T. J. Killian.
.X1
Programs that convert
.SM ASCII
to audio.
.X2
.XX raster 483 graphics/cmd.ms "The 10th Edition Raster Graphics System"
T. Duff.
.X1
How to make, process, and display high quality color images.
.X2
.X3
.SH
Implementation and Maintenance
.XX setup 497 setup/setup.ms "Setting Up a Research UNIX System"
N. Wilson.
.X1
How to configure your system and get it running.
.X2
.XX streams 503 streams/streams.ms "A Stream Input-Output System"
D. M. Ritchie.
.X1
The way data is passed among processes and communication devices.
.X2
.XX netb 513 netb/netb.ms "A Look at the Ninth Edition Network File System"
S. A. Rago.
.X1
Details of the current network file system protocol for Research
.UX .
.X2
.XX ipc 523 ipc/ipc.ms "Interprocess Communication" " in the Ninth Edition UNIX System"
D. L. Presotto and
D.\ M.\ Ritchie.
.X1
A description of the current IPC system;
both the underlying system primitives
and the user-level code.
.X2
.XX authmgr 531 auth/auth.ms "Authmgr \(em An Authentication Service for Datakit"
D. Cohrs.
.X1
A Datakit service for authenticating users.
.X2
.XX security 543 security/security.ms "UNIX System Security"
F. T. Grampp and
R. H. Morris.
.X1
How the bad guys can get you if you don't watch out.
.X2
.XX upas 557 upas/upas.ms "Upas \(em A Simpler Approach to Network Mail"
D. L. Presotto and
W. R. Cheswick.
.X1
How the mail system is organized and how it deals
with networks.
.X2
.XX uucp 563 uucp/admin.ms "Uucp Administration"
D. A. Nowitz.
.X1
How
.I uucp
works, and how to administer it.
.X2
.XX fsck 581 fsck/fsck.m "Fsck \(em The UNIX File System Check Program"
T. J. Kowalski.
.X1
Semi-automatic diagnosis and repair of
.UX
file systems.
.X2
.XX backup 593 fm/fm.ms "The File Motel: An Owner's Manual"
A. G. Hume.
.X1
The design and maintenance of the 10th Edition incremental file backup system.
.X2
.XX lp 603 lp/lp.ms "A Guide to the Lp Printer Spooler"
P. Glick.
.X1
The intricacies of running your printers.
.X2
.SP
.XX index 609 index/index.ms "Index"
L. L. Cherry.
.X1
An almost mechanically generated index.
.X2
.BP
photo page with lee macmahons picture

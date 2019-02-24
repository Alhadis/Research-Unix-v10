.so ../ADM/mac
.XX 45 649 "UUCP \(em The Program That Wouldn't Go Away"
.nr dP 2
.nr dV 3p
.EQ
delim @@
.EN
.TL
UUCP \(em The Program That Wouldn't Go Away
.AU
Peter Honeyman
.AI
Princeton University
Princeton, New Jersey 08544
.AU
Dave Nowitz
.AI
AT&T Bell Laboratories
Murray Hill, New Jersey 07974
.AU
Brian E. Redman
.AI
Bell Communications Research
Whippany, New Jersey 07981
.AB
.PP
The
.UX
to
.UX
Copy Program (\fIUUCP\fP) embodies many good ideas for
an inexpensive file transfer and remote execution network;
however, the current implementation is over five years
old and is troubled by many problems.
.PP
This paper describes a new implementation.
The main goals were to
make UUCP more robust, secure, powerful, and
maintainable.
The major activities were:
.RS
.RS
.IP \(bu
A massive code reduction.
Four core programs comprise the system:
.I
uucp, uucico, uux,
.R
and
.I
uuxqt.
.R
Code that did not directly implement the
purpose of these programs was excised.
.IP \(bu
The connection algorithm was rewritten to persevere in the face of
adversity and to provide a mechanism that enables the administrator
to incorporate new calling devices easily.
.IP \(bu
The spooling mechanism was replaced with one that hashes the queued
files by remote system name.
.IP \(bu
The USERFILE syntax was discarded and replaced by an extremely flexible
and intelligible mechanism with practical (secure) defaults.
.IP \(bu
The code was reviewed and modified
to meet the authors' standards for programming style, robustness, and
maintainability.
.RE
.RE
.AE
.2C
.NH
History
.PP
UUCP was invented by Mike Lesk
in 1976.
He required a transport mechanism to build an automated software distribution
package|reference(lesk proposal 1977)|reference(lesk cohen 1977)|reference(asd koenig)
upon.  However it became
clear that UUCP required (and deserved) considerable follow-up
activity and it, the tool, became the focus of attention rather than
the original job for which it was designed.
.PP
The first version of UUCP was approximately 4000 lines of code.
It was limited to transferring files, although in fact it included
a remote execution facility to invoke \fIuucp\fR on remote systems in
some syntactically special cases.
Its major failing (even for its time) was that its access mechanism was too
simplistic.  It would connect to a remote system and login
with a standard interactive shell.  It would then issue commands
on the remote system such as \fIcopyin\fR and \fIcopyout\fR to effect
transfers.
Although an attempt to hide the remote system's phone number, login and
password was
provided in the code, anyone
who installed UUCP was privy to such information, and a careless
administrator could easily make it public.
.PP
The second version of
UUCP|reference(v7man nowitz lesk)|reference(nowitz v7man implementation)|reference(nowitz lesk unix network 1982)
readdressed the security
concerns. It also provided new features including a general
remote execution facility, a packet driver, and administrative aids.
This version (@approx@ 6800 lines) was distributed with Seventh Edition
.nr FL 4i
.UX
and instantly
became invaluable to the
.nr FL 5.5i
.UX
user community.  The second version
of UUCP is the version in use today with minor differences
among various flavors of
.UX .
.PP
Lots of
.UX
systems means lots of UUCP nodes sending lots of
data  (mail, netnews|reference(usenet byte), and other information)
to lots of other
.UX
systems.
The sheer volume of work that
UUCP was required to handle taxed its engineered limits
beyond its capabilities.  Stu Feldman has compared it to a bicycle bridge
over which tanks must now cross.
.NH
Introduction
.PP
Here we present a brief description of how UUCP works from a
functional point of view.  Those familiar with UUCP's internals
might skip this section. 
.PP
UUCP is a system composed of four principal programs and
several administrative support programs.  The core programs
are \fIuucp\fR, \fIuucico\fR, \fIuux\fR, and \fIuuxqt\fR*.
.FS
*The names are pronounced by sounding the name of each letter.
.FE
Administrative programs
include \fIuuclean\fR, \fIuulog\fR, \fIuustat\fR and possibly others depending
upon which strain of the second version you look at.
The system provides a mechanism to
move data among computers and optionally to invoke commands remotely
upon the transferred data.
.PP
The \fIuucp\fR command behaves much like the standard
.UX
copy command, \fIcp\fR.
While \fIcp\fR will only copy files on the same machine,
\fIuucp\fR will copy files
among machines.  \fIUucp\fR is a batch mode command that
queues requests for copying.
The \fIuucp\fR command
creates files in the \fIuucp spool directory\fR that specify the
user's request.
\fIUucp\fR generates two types of files:
\fIcommand file\fRs (whose names are prefixed with \fIC.\fR) and
the \fIdata file\fRs
(prefixed with \fID\fR.).
Each \fIuucp\fR command will generate as few as one
command file and a data file for each file that
the user has requested to be transferred.
The command file
specifies whether this is a request to send files to a remote system
or to receive them and contains information regarding the
name of the target files, the user and various options.
The data file contains the information to be transferred.
.PP
The \fIuux\fR command also
queues requests in the spool directory.
It
generates an additional type of file, the \fIexecution
file\fR.
This file contains information required for
remote command execution,
and includes the name of the command,
input and output specifications, and other information relevant to
the treatment of the remotely executed command's status.
On the local system, it is treated as a data file.
\fIUux\fR also creates
data files containing information required by the commands
to be executed remotely
and a command file to specify the transfer of the data files
described.
.PP
The \fIuucico\fR program is the workhorse of UUCP.
While \fIuux\fR and \fIuucp\fR
merely queue work into the spool directory, \fIuucico\fR
examines the spool directory and enacts the transfer of data
as dictated by the command files.  A simplistic view of \fIuucico\fR is:
.IP
Connect to remote system.
.IP
Cycle through command files and send or receive data as prescribed.
.IP
Allow remote system to send or receive files if it has command files
for the local system.
.IP
Hang up.
.IP
Invoke \fIuuxqt\fR to process execution files that have been received.
.PP
The \fIuuxqt\fR program performs the execution of remotely specified
commands.
We noted that \fIuux\fR creates data files to be transferred
to the remote.
The special data file that specifies the command to be executed
is renamed when it is copied to the remote to indicate that it is an
execution file. 
\fIUuxqt\fR searches
the spool directory for execution files
(whose names begin with
.CW X. )
and operates upon them.
These files contain the command to be executed, indicate which
other files are to be used in conjunction with the command, and
determine the fate of the output of said command.
.NH
Problems
.PP
The second version of UUCP served mostly as a base upon which
many many variations were built.
The multiplicity
was in response to the problems associated with the second version.
Its extraordinary use caused many bugs and design flaws to surface.
We will discuss these problems and some earlier attempts
to rectify them.  The section after will describe the solutions
implemented in our (the third) version of UUCP.
.NH 2
Performance
.PP
The major issue here revolves around a problem of positive feedback.
UUCP depends heavily upon the
.UX
file system.  The location of data within
the file system and the names of the files containing data impart
specific knowledge to UUCP.  The second version of UUCP uses a
single directory to store its command files, data files, execution
files, lock files, temporary files, etc.  On a smoothly running
heavily trafficked system, the numbers of these files will typically
range in the hundreds.  The searching of large directories is performed
inefficiently in
.UX *.
.FS
*This is representative of some of
.UX `s
controversial
design decisions.  It does the job in a straightforward manner which
is usually quite suitable.
Although there are admittedly pathological cases
which are problematic, the operating system was not contorted
to handle them specially.
Usually reevaluation of their design will indicate a more
appropriate solution.
.FE
Accessing data files or information relevant to such files involved
locating the name of the file (a directory search) and then checking
the status of the file (another directory search).
The time to perform these operations is then
quadratic in the number of directory entries.
The performance degradation due to manipulating files in very large
directories results in the following common scenario.
.IP
UUCP connects to a remote system.
.IP
It then begins to search the spool directory for work files relevant
to this session.  This takes an inordinate amount of time.
.IP
The remote system times out waiting for UUCP to indicate its intentions.
It disconnects.
.IP
UUCP finally completes its task, finds that the remote is no longer
connected and hangs up.
.IP
Some time later the cycle is repeated.
.PP
This positive feedback loop consumes large amounts of resources
yet produces no work.
The problem becomes worse as more and more
requests are placed in the spool directory.
This then fatally cripples
UUCP.  In order to get the system working again, the administrator
chooses between time-consuming, tedious, error-prone manipulations (create a new
spool directory and slowly feed data and work files into it at
a rate no greater than they are processed) and merely
deleting all the queued files (with the obvious effects of lost data).
.PP
There are several reasons why the spool directory may become very large.
A remote system may be unavailable for an extended period of time while
work continues to be spooled for it.
A site may act as a central distribution point for large volumes of information
to many other systems.
The addition of more nodes may provide the straw that breaks UUCP.
A system may find itself short on cycles on a given day and the extended
time required to process work may provide the stimulus to begin a positive
feedback cycle.
Once the syndrome is displayed, the situation deteriorates.
As the number of UUCP sites grows and the amount of information transferred
increases this effect becomes more probable.
.PP
There have been various attacks on this problem.  Some address only the
symptoms, others merely postpone its onset.
They include:
.IP
The initial approach:  To increase the amount of time a system will
wait until it decides to give up.  This does just that.
.IP
The automatic recovery approach:
A set of programs to move the spool directory, recreate it, and
add the files back in the correct order for processing.  It typically takes
several days to set things right again (until the next time).
.IP
The approach in 4.2bsd (credited to Tom Truscott):  Separate the spool
directory into subdirectories, one each for command files, data files,
and execution files.  This reduces the probability of a blockage or
postpones its occurrence because each directory is somewhat smaller.
However potential for problems remains as great.
.IP
A method we used on a major information distribution hub:
We recompiled the
UUCP programs with different spool directories for each of the
sites to which we distribute.
The distribution software used the appropriate version
of \fIuucico\fR.
The \fIlogin shell\fR for each of the affected systems was
determined likewise.  The major drawback was the number of copies of
UUCP programs required.
Also, some work was spooled in special directories
and other jobs were spooled in the main spool directory because
of the desire not to change the programs that use UUCP.
Although this might seem advantageous (netnews in one directory, mail
in another) when the remote system called us, we only processed the work in
the special spool directory.  Both spool directories were used when we called out.
.NH 2
Security
.PP
The second version of UUCP presented mechanisms that enabled
the system to be secure, but they were poorly understood
and were usually ignored.
These mechanisms  were also not flexible enough
to enforce security where it was needed and omit it from where it
wasn't.
.PP
The second version dealt with security on three fronts.
.PP
1)
The login shell for UUCP was not a general purpose shell, and
only a complex program (i.e., UUCP) could make use of this access.
.PP
2)
By previous arrangement any
communicating systems could share a sequence number which was incremented for
each session.
This required that an imposter not only know the login
and password, but the sequence number as well.
A penetration would be discovered as the legitimate system would now be
out of synch.
.PP
3)
The USERFILE was not administrator
friendly.
It provided four types of constraint.
.IP [1]
Files accessible by a normal user of the local system.
.IP [2]
Which files can be accessed from a remote system.
.IP [3]
Login name used by a remote system.
.IP [4]
Whether a remote system should be called back in order to confirm
its identity.
.PP
The special login shell was the most important development.
That allowed
access restricted to and by the UUCP programs.
The sequence file facility, to the best of our
knowledge, has never been used other than to test its capabilities.
Its lack of use must be blamed on inadequate documentation.
We considered removing it from our rewrite but
felt that it may still prove valuable.
The USERFILE concept provided a good base but was not flexible enough
for the various environments in which it would be used.
It could not grant
different access to files depending
upon whether they were to be read or written (many systems are not willing
to allow data to escape, but eagerly accept contributions).  It lacked
an ``all but'' syntax.
It vainly attempted to restrict local users' access.
Normal
.UX
file access
mechanisms were satisfactory in this regard.
We provide new semantics
and a significantly improved syntax which is discussed later on.
.PP
Recently another version of UUCP* was developed at AT&T Bell Labs|reference(morris try uucp)|reference(morris maintaining uucp)
whose main thrust was to address the security aspects.
.FS
* It is not our purpose to completely describe it but to point to
some examples of alternate approaches.
.FE
Systems may not request files nor can files be received at destinations
other than a public directory and an additional directory provided as a
sort of secured receiving dock.
Although it does well in its attempt to tighten up security, it takes a
rather severe approach that can get in the way of users in a casual
environment.
.NH 2
Obsolescence
.PP
The second version of UUCP was designed for two types
of communications media,
the Bell 801C/212A type Automatic Calling Unit (ACU) and modem hardware,
and direct hardwired lines.
The code was simply implemented as:
.P1 0
if (ACU)
   call(flds); /* use the 801/212 */
else
   direct(flds); /* just open the device */
.P2
.PP
Code was later added for some special network apparatus
(DATAKIT).
This was patched in with conditional compilation.
Incorporation of new calling hardware was messy
and error-prone.
.PP
The mechanism that UUCP provided
for negotiating a connection with a remote system (expect this, send that, ...)
was not general enough.
For example, each token sent out was terminated by a newline character.
As port selectors of various
flavors became more prevalent, their idiosyncrasies became more
troublesome.
It was not uncommon to be required to present them
with arbitrary characters, delays of varying duration,
or tokens that did not end in newline.
One accepted only an even parity carriage return.
These capabilities were
difficult or impossible to provide without changing the code.
Therefore new syntaxes and special tokens were invented as the
need arose that led to a variety of incompatible connection descriptions.
Even if the hardware worked properly, there was a significant
probability that the connection would fail to be completed.
.PP
Most recent fixes to UUCP addressed the token problem, though
few used the same conventions and many were lacking
some key capability or another.  The addition of new hardware was generally
handled with conditional compilations and new keywords were invented as
needed.
Several systems implemented connection routines
as a general library function, isolating, but not resolving, the problems.
.NH 2
Limitations
.PP
When UUCP was designed several decisions had to be made based on
anticipated usage.
Many of the practical limits imposed have been exceeded.
The greatest problems occur in the area of array sizes.
UUCP handles all sorts of arrays that represent
commands, users, files and systems.
At a time when most sites could be easily connected with all others
the limit of 64 characters easily accommodated an electronic mail
address.
But the network has grown quite wide and such addresses often
exceed two hundred characters.
Some versions of
.UX
permit login names that exceed the standard restrictions on the number
of characters.
These developments require changes to the limits imposed by UUCP.
.PP
Some limits were imposed due to system design constraints.
The name of a site was restricted to seven characters because
it was encoded into file names along with a two character type
prefix, a grade consisting of one character and four characters making
up a sequence number.
It is becoming increasingly difficult, with thousands of sites,
to select mnemonic names with this limitation.
The problem is becoming more severe as
.UX ,
and therefore UUCP, usage grows explosively.
.NH 2
Error Handling
.PP
UUCP often becomes totally frustrated when erroneous data is
presented to it.
It is common that a command file will be created with invalid
fields and \fIuucico\fR will unexpectedly abort in an attempt
to process it.
All subsequent invocations of \fIuucico\fR exhibit the same
behavior until the bad file is manually removed.
Such files are created in the first place because UUCP fails to
check the return status of critical \fIwrite\fR system calls.
They are allowed to impede the system because there is seldom
any attention paid to the values returned from \fIread\fR.
When a system runs out of disk space
(which systems very often do),
failing \fIwrite\fRs go unnoticed yet files appear to be transferred
successfully.
Data is lost with no error indication whatsoever.
.NH 2
Inconsistency
.PP
Because of the problems described above UUCP has been modified
by many systems maintainers.
Modifications include necessary bug fixes required for normal use,
enhancements required by users with special needs, and changes
to make the system more robust.
Significant work has been 
done at Rand, AT&T Bell Labs,
ITT, UC Berkeley, Duke and many others.
Unfortunately the various
versions represent different solutions to the same problems and
they are sometimes incompatible.
For example the syntaxes used to specify
scripts for connections have been vastly different.
Since few versions are alike, enhancements
and fixes
won't apply to another version and
will be of limited value to the overall community.
This is especially frustrating when good solutions are presented
but not widely accepted because of the cost of providing a different
implementation.
.NH 2
Administration
.PP
The second version of UUCP is difficult to administer.
It introduced two administrative tools, \fIuuclean\fR and \fIuulog\fR,
and others have come about since.
But these programs are not sufficient and there are problems with them.
.PP
\fIUuclean\fR was introduced to aid the administrator by
cleaning up after UUCP.
It would find old files (temporaries, spooled
requests, etc.), remove them and optionally notify a user.
However, deleting old files is not always the best approach.
Often such files represented precious data that could not be regenerated.
The algorithm for notification would often inform an inappropriate
user of the deletion.
Even if the appropriate
person was notified, the information contained in the notification was
cryptic and unuseful.  For example:
.P1 0
file D.fooAD1234 removed after two weeks.
Could not contact remote.
.P2
.PP
UUCP kept log information in a single file.
If that file was busy, a process would use a temporary log file instead.
The access modes of the temporaries indicated whether or not the
process that owned them was still active.
If they were not active, then \fIuulog\fR consolidated them into the main
log file.
However its purpose was often defeated because UUCP processes that terminated
abnormally did not mark the temporaries for consolidation.
\fIUuclean\fR would then come along and eradicate information
that would have been useful for debugging.
\fIUulog\fR also had options to find log information about a system or a user.
This function is more easily performed by \fIgrep\fR and \fIsed\fR.
With the enhanced second version of UUCP introduced with
.UX
3.0
(System III) came the \fIuustat\fR command.  This was a good attempt at a program to
aid administration and enhance the user's capabilities for control and
access.  Unfortunately its implementation was weak, requiring many
hooks into UUCP and acting as another fragile link in the
increasingly unstable system.
.NH 2
Code Maintenance
.PP
One of the problems associated with any large system is maintaining
the source code.  This problem is aggravated in UUCP because it has
been an extremely dynamic system (as measured by the number of Modification
Requests entered for it).
The coding responsibility is no longer with
the designers, but has been assigned to programmers (over the past
several years) with differing experience levels.  The source code is scarred
by the many hands that have operated on it.  As early as 1980,
individuals responsible
for UUCP 
were hesitant to make any changes because
it had become ``too big a mess and was too difficult to maintain''.  Despite
this attitude, UUCP continued to be fixed, enhanced,
and otherwise modified, and grew ever more unmaintainable.
.NH
The Solutions
.PP
.NH 2
Site Name Hashing
.PP
The rationale for reorganizing UUCP's data into separate directories
has been described.
However it later became apparent that this
technique was not necessary to deal with the intended problem.
Some technical understanding of
.UX
is required to fully appreciate the following.
The culprit that caused the quadratic behavior in the second version
of UUCP was a gratuitous \fIstat\fR system call placed in a procedure
that searched for command files.
This function read the name of each file from the spool directory.
It checked that the file had the proper prefix and
then used \fIstat\fR to determine if it was publicly readable.
UUCP created command files write-only (mode 0200) by the uucp login and
changed them to be publicly readable when they were complete.
This prevented the system from using partially written files.
Since the file was \fIwrite-only\fR, an open for reading by the
search function would fail.
The use of \fIstat\fR was presumably an additional check to back up
the normal system access mechanism.
As the code was revised, the \fIstat\fR became an \fIaccess\fR, and that
eventually disappeared without fanfare.
Directory searching within UUCP became virtually linear with respect
to the size of the directory.
(Multiple processes encountered directory locking phenomena.)
In retrospect it
appears obvious that that was the
solution to the critical performance problem.
But before that was realized separate spool directories had been
implemented.  The other benefits of such a design justify its existence.
.PP
Data files representing a particular remote system are now
placed in a directory with the same name as the remote system.
This name is stored in a global variable.
The UUCP programs that access these files change
their working directory into the directory associated with
the remote system and perform their operations there.
This is in effect a simple hashing algorithm.
It improves access time, especially for heavily loaded systems.
It also provides a convenient structure for data storage that
is taken advantage of by programs and by people.  For example if you want
to know how many files are destined for a system named \fIsfwoo\fR,
you could:
.P1 0
wc -c /usr/spool/uucp/sfwoo/C*
.P2
It is also advantageous to know that
files in a system's directory were put there either by the given system or by
the local UUCP.  This bit of knowledge will be exploited later
in the discussion of security mechanisms.
.PP
Another outgrowth of separate spool directories is the notion
of parallel remote execution processing.
The process that executes commands on behalf of remote systems (\fIuuxqt\fR)
finds work in separate directories.
Multiple \fIuuxqt\fRs
can execute simultaneously, one per remote.
This is a reasonable compromise
between the limit of one invocation of \fIuuxqt\fR per local system and
the possibility of one per X. file.
We provide
optional limiting so that a machine can support useful work in addition to
handling mail and netnews.
.NH 2
Site Name Limitations
.PP
The name of a site is no longer encoded in the file name.
The location of the file in the directory structure implies the site.
All C. files in
.CW /usr/spool/uucp/fugit
are work files for the system named \fIfugit\fR,
all X. files in
.CW /usr/spool/uucp/gummo
are remote
execution requests from the system named \fIgummo\fR, etc.
This means that site names need no
longer be limited by seven (or six in some versions) characters.
Since the directory name is the limiting factor, site names of
up to fourteen characters are permitted.
We have tried to isolate this limitation to a single manifest constant.
Thus systems which support longer file names may allow site names of greater
length.
Perhaps future versions will use the directory name as a pointer
to the real site name.
Even conservative use of the character set would allow over
@ 64 sup 14 @ different names.
This added flexibility (increasing
the name space) has some controversial ramifications which are discussed
further on.
.NH 2
Security
.PP
Our goal with respect to UUCP security was to make the mechanisms robust,
flexible and usable.
Robustness was achieved by fixing bugs, plugging holes,
being careful in designing and coding our revisions,
and thorough testing under a variety of operating conditions.
Flexibility and usability came together in what is known as the
\fIPermissions\fR file.
.PP
The \fIPermissions\fR file replaces the \fIUSERFILE\fR of the second version of
UUCP.
Its purpose is to support a mechanism for clearly and flexibly
stating what restrictions apply to or what permissions are granted to
the sites with which the local UUCP system communicates.
It is located in the UUCP library directory and
contains statements which customize the behavior of UUCP for any or all
systems by identifying them with the login name they use or their site name.
The statements described are
used in conjunction with two basic types of entries which
are
.P1
LOGNAME=
.P2
for remotely initiated connections and
.P1
MACHINE=
.P2
for locally initiated connections.
There must be a LOGNAME= statement for each login that might be used
by a remote system.
The minimal contents of the
\fIPermissions\fR file is:
.P1 0
LOGNAME=uucp
.P2
This specifies that a remote system may login as \fIuucp\fR and will be
restricted by the defaults.
.PP
The permissions fall into three basic categories:
file system access,
command execution,
and identity verification.
.PP
File system access permissions dictate which directories a remote system
may access for reading, and, separately, which may be accessed for writing.
This is done by specifying any combination of the keywords
.CW READ ,
.CW WRITE ,
.CW NOREAD ,
and
.CW NOWRITE
followed by a
.CW :
separated list of directories.
In the example:
.P1 0
READ=/usr/ber:/usr/honey:/usr/dan \e
   WRITE=/tmp NOREAD=/usr/ber/secret
.P2
a system can read any files
that the UUCP login can access
in the directories
.CW /usr/ber ,
.CW /usr/honey ,
and
.CW /usr/dan
except those files in
.CW /usr/ber/secret .
It can also write into any file in
.CW /tmp .
.PP
The command execution permissions specify commands that can be invoked.
.P1 0
COMMANDS=rnews:lpr:opr:rmail
.P2
shows that the site associated with this line can execute any of the
listed commands and no others.
If a path name is specified then it
must match the command requested by the remote system exactly.
.PP
Identity verification is accomplished in a variety of ways.
.CW CALLBACK=yes
is most restrictive.
It requires that when the remote system calls, it
must hang up and wait for the local system to return the call.
This provides 
the greatest assurance that we are communicating with the correct party.
.PP
.CW SENDFILES=no
prevents the \fImaster/slave\fR protocol from being
reversed when the remote system calls.
The result is that although the local site may
have data queued for the remote site, it will not transmit it if the
remote system originates the call.
This is slightly less restrictive
than
.CW CALLBACK=yes .
.PP
.CW REQUEST=no
indicates that the remote site may not request files from us under
any circumstance.
In order to receive files the remote user must
have an agent on the local site to initiate the transfer.
This restriction applies no matter who calls whom.
.PP
Finally, when
.CW VALIDATE=
appears, the specified systems must have
logged in using one of the associated logins indicated.
For example:
.P1 0
LOGNAME=Umarx VALIDATE=chico:harpo:zeppo
.P2
will cause UUCP to terminate the conversation if \fIchico\fR, \fIharpo\fR or
\fIzeppo\fR log in with a name other than \fIUmarx\fR.
.PP
The default permissions are:
.P1 0
LOGNAME=uucp \e
  WRITE=/usr/spool/uucppublic \e
  REQUEST=no SENDFILES=no \e
  COMMANDS=rmail:rnews
.P2
Note that the statement must appear as a single record.  Lengthy statements
may be continued with
.CW \e .
Appendix I is a sample \fIPermissions\fR file,
Appendix II is the description emitted from a program that checks the
\fIPermissions\fR file (described later).
.PP
These mechanisms allow a site to implement secure features where
they are needed while still allowing considerable flexibility in the
treatment of sites based on knowledge attributed to the login method
and remote name.
This concept allows UUCP to be useful
both with tightly coupled well-trusted machines and the loosely
coupled less secure dialup network environment.
.NH 2
A Rational Connection Function
.PP
The third general area of revision is that of the connection function.
The \fIconn\fR of UUCP is a beautiful abstraction that subsumes all the
knowledge of setting up a connection to a remote system.
It takes as its
argument the name of the remote system and returns a file descriptor open for
reading and writing to communicate with it.
.I conn
accounts for
over ten percent of the UUCP code.
Its function is applicable to
any utility that connects to a remote system or resource.
Several
developers have extracted
.I conn
from UUCP and provided it as
a stand alone facility (\fIdial\fR(3) in System V and \fIdialout\fR(3)
in Eighth Edition
.UX ).
Our implementation is suited to that purpose as well, though
.I conn
remains bundled with UUCP.
.PP
The goals in revising
.I conn
were to bolster its robustness,
handle more types of connecting hardware and to provide a
mechanism for easily utilizing new hardware.
This was done by splitting the hardware-dependent operations from
.I conn
into two routines.
One understands the needs of
\fIcallers\fR (e.g., how to interpret the fields in the \fISystems\fR file to them)
and deals with locking and bookkeeping.
The other understands how to manipulate the hardware to connect to a remote
resource (parochially known as dialing).
.I Conn
uses a table containing the name of each
caller (ACU, Micom, TCP, Sytek, etc.) and a pointer to the function
that manipulates it.
The caller function uses a table of \fIdialers\fR which contains their
names (212, Penril, Vadic, etc.), and pointers to the
functions that perform dialing.
Not all callers have associated dialers.
Another way to describe it is that the caller specifies the nature of the
network and the dialer dictates the mechanism to access such a network.
Thus we find currently that there are many dialers for the ACU caller
because there is a variety of hardware to access the Direct Distance Dialing
network.
But we have only one interface to the DATAKIT network so the calling
routine for it may as well (and indeed does) include the dialing function.
If alternate interfaces become available then the dialing function will
be split out and the caller will be able to choose among them.
The enhancement procedure for hardware unknown to us at the time of
this writing then entails supplying a new caller and
dialer function, updating the tables, and recompiling.
.PP
Given these tables which contain identifiers, we provide a new
syntax for the \fIDevices\fR file (formerly called \fIL-devices\fR) in which the
caller hardware and dialer function are specified.  Appendix III, is a sample
\fIDevices\fR file, Appendix IV is a sample
\fISystems\fR file (formerly \fIL.sys\fR).
(\fIL-dialcodes\fR has been renamed \fIDialcodes\fR but is unchanged from previous
versions.)
Note the existence of a \fIchat\fR script in the \fIDevices\fR file.
This is a syntactic convenience used to isolate the differences in various
switches so their similarities can be exploited in a
generic caller routine.  This helps to clean up the \fISystems\fR file.
.PP
.I Conn
is called with the name of the remote system and
behaves as follows:
.P1 0
.ps -1
.vs -1p
for each entry in \fISystems\fP that matches the argument
    for each caller in \fIDevices\fP that matches this entry
        if this caller doesn't need a dialer
            attempt to connect
            if successful
                return file descriptor
        else
            for each dialer that can be used with this caller
                attempt a connection using the dialer function
                if successful
                    return file descriptor
try again later
.ps
.vs
.P2
.PP
This mechanism allows us to exploit all the alternative
hardware at our disposal in an attempt to make a connection.
The priorities of the various alternatives are expressed in the
ordering of the entries in the \fISystems\fR and \fIDevices\fR files.
A subtlety of interest is the use of the \f(CWANY\fR keyword in the \fIclass\fR
fields of the \fIDevices\fR file.  This affords more flexibility
in determining which connection hardware to use.  For example one may
wish to connect to some systems at lower speeds than the hardware is
capable of to counteract deficiencies in their terminal drivers.
.PP
UUCP selects a protocol used to communicate with a remote system.
Typically this is the `g' protocol which is a variant of X.25.
It uses 64 byte packets and is quite resilient in error-rich environs
such as the DDD network.
There were also the `x' and `d' protocols for use over
X.25 and DATAKIT lines.
We have added the
`e' protocol (ostensibly for Ethernet),
which is suitable for error
free transmission media.
The packet size is in fact the file size,
so the overhead becomes almost negligible.
The \fISystems\fR file has been enhanced to provide the ability
to specify a preferred protocol depending on the hardware used to initiate
the connection.  (We attribute this idea to R. T. Morris|reference(morris maintaining uucp).)
.PP
A problem with the coding of
.I conn
is that it nests very deeply in order to
establish a connection.
The handling of error-returns through intermediate functions
was clumsy and not generalized.
The use of a single error-indicating variable (\fIUerror\fR)
has allowed us to implement the function more cleanly.
.NH 2
Coping With Bad Data and Inadequate Environments
.PP
Although the new implementation of UUCP is laudably robust in the creation
and transmission of data, it will still have to deal with improperly formatted
files that are the result of lesser versions or system errors.
When UUCP evaluates a command or execution file it checks that the contents are
plausible.
When corrupt files are identified, they are moved to a special directory.
The administrative daemon checks this directory (\fI.Corrupt\fR)
periodically and informs the administrator of its contents.
.PP
Previous behavior with respect to handling conditions where a system
did not have adequate disk space was unacceptable and quite frustrating to
the anonymous users who had their data silently lost.
The new UUCP checks the level of the file system using the \fIustat\fR system call
(an equivalent user level routine is provided for systems lacking \fIustat\fR).
A special error code is transmitted to the remote system if there is
insufficient space.
The event is logged and the conversation is terminated.
If UUCP receives this error indication from a system, it will disengage
from the conversation assuming that the remote is in trouble.
Scans of the log files will reveal these situations and the local administrator
may wish to inform the remote counterpart.
There has been considerable discussion among the authors about
ways to improve the above scenarios.  It is generally agreed that
when a system runs out of space it ought to reverse its role and attempt
to send data.  If it had run out of space as master it ought to continue
scanning work files performing sends and deferring receives.
If the system notes that the remote has run out of space, then it ought
to scan the command files looking for receives and reverse roles when
they are completed.
Unfortunately, implementing these actions would be quite
complex.  They have been deferred to the wish list.
.NH
Getting It Up and Keeping It Running
.NH 2
Configuration
.PP
This system has been described as a tribute to the C preprocessor.
Because our goal was to produce a sound system to which
changes would inevitably be applied,
compile-time parameterization is ubiquitous.
It had to run on a variety of
.UX
versions and processors.
We also found the need for extensive options to express varied
preferences.
.PP
For instance, to compile a system for machines constrained by memory limitations
we have provided a parameter that allows only the most critical debugging
facilities to be included.  This idea of conditional compilation may be
extended to the general case of time/speed tradeoffs.   Many algorithms
are chosen in consideration of the environment in which they will be applied.
But UUCP is used on many different processors by users with varying needs.
Dynamic memory allocation, although costly in time and complexity, may
be appropriate in order to drastically reduce the memory requirements.
Systems running on machines with lots of memory ought to be able to have
larger caches and faster hashing schemes.
.PP
It was also necessary to be able to configure it
to interface with various user programs.
For example,
even though lock files don't belong in UUCP's spool directory, 
in reality some people won't change the other programs that
are affected.
We provided the option for compatibility.
.PP
Another set of problems relegated to the preprocessor concerned opinion and taste.
As the system was developed, there arose
many situations where a new idea was not immediately embraced by all.
Often it was not clear what the correct behavior
should be, as `correctness' was sometimes subjective.
Rather than throw out good (albeit minority opinion) ideas,
or force them on others,
they were compiled in optionally.
Many of these eventually found enough favor to be included unconditionally.
Others were blessed as the default, while some remain debatable.
Parameters likely to be changed out of necessity or preference
have been gathered in one place
(\fIparms.h\fR) for easy identification and manipulation.
.NH 2
Installation
.PP
UUCP has become less straightforward in its use of the file system and
more dependent upon various files to enhance its flexibility.
It is more important than ever to aid the user in the installation
process.
Although the programs have been designed to fall back
on sensible defaults if some files are not present (such as \fIMaxuuqts\fR
which potentially limits the number of simultaneous \fIuuxqt\fRs),
other files
and directories are too critical to do without (e.g., \fISystems\fR).
And for many the modes are quite important (imagine a spool directory only writable
by root).
Since this version is significantly different,
current working data must be converted to conform to the required
formats.  A comprehensive \fImakefile\fR provides the basis
for installation (we thank those who were then 6.0 developers).
A conversion shell script is provided as well as a program
(\fIuucheck\fR) that verifies the installation and the integrity of the various
files used by UUCP as well as optionally interpreting their contents
to the user in very descriptive terms (i.e., system so-and-so can do
such-and-such and is restricted from this-and-that).
See Appendix II.
Ideally a recipient of this package will be able to edit the
parameter file, and type
``make install'' to generate a working system.
.NH 2
Maintenance
.PP
Although we have provided tools to get the system
up and running, it's at least as important to help keep
the system running smoothly.
Due to the anarchic and volatile nature of the network that UUCP supports,
not all requests that are generated are completed.  For one reason
or another data files will become orphaned, command and execution files
widowed, temporary files will strive for immortality, \fIcore\fR files
will be born (not by UUCP of course) and \fIdead.letters\fR will litter
directories.
Only a program can expend the time and effort required to manage
a busy system routinely.
.PP
One of the first programs we threw away was \fIuuclean\fR.
This left us without an automated mechanism to sweep all of UUCP's
litter under the rug.
Over a period of several weeks we noted our actions and our
thoughts as we manually handled the situations described above
in the most considerate manner.
From these observations a set of heuristics
was compiled that effectively deal with the deficiencies of
an imperfect system.
They are represented by a program (\fIuucleanup\fR) that
deals handily with the
typical cases.
Unanticipated problems are dealt with inelegantly
in the style of \fIuuclean\fR, but are recorded to facilitate the
incorporation of additional knowledge as it becomes necessary.
Appendix V shows examples of the various situations encountered
and how they are handled.
This ``expert'' program along with expanded \fIdaemon shells\fR that
monitor activity and generate reports provides enormous assistance
in maintaining UUCP.
.NH 2
Accounting
.PP
All the core programs
(\fIuucp\fR, \fIuucico\fR, \fIuuxqt\fR and \fIuux\fR) append accounting data
to log files, but they do not operate on the data.
Separate programs such as \fIuustat\fR are provided
that examine the files and operate on their contents.
Maintenance functions are also delegated to separate programs as
discussed.  
These separations
have significantly reduced the complexity of the core programs.
\fIUucp\fR spools requests to copy files.  \fIUux\fR spools remote
command executions.  \fIUucico\fR performs the actual transfer of files and
\fIuuxqt\fR executes remotely specified commands.
.NH 1
Things The Users Will Notice
.NH 2
Reporting Remote Execution Status
.PP
Specific knowledge of the commands invoked by \fIuuxqt\fR has been removed.
All commands are handled in a uniform manner.
.PP
In previous versions of \fIuuxqt\fR, the \fImail\fR command was handled specially.
Except for \fImail\fR, the return status of all commands was
unconditionally reported to the invoker.
For \fImail\fR however, no status was returned if the
command succeeded.
But, if the \fImail\fR command failed, \fIuuxqt\fR would
return the standard input as well as the status.
This was justified by the fact that \fIuuxqt\fR was principally used for
remote mailing.
When netnews became widely used, it too was a logical
candidate for exceptional handling within \fIuuxqt\fR.
Two approaches had been
taken.  The first was to treat the \fIrnews\fR command specially and ignore the
return status.
The second, more elegant solution appeared in the version
distributed with
.UX
4.0.  That was to enhance \fIuux\fR
(and \fIuuxqt\fR) with an option
.CW -n ) (
to specify that command return status was not
to be sent back to the invoker.  This was a step in the right direction
but was not sufficient.
Another option was added locally to return status if and only if it was
not zero
.CW -z ). (
This was still not good enough.  More flexibility
was needed if we were to remove the builtin knowledge of what to do
with \fImail\fR.
Our solution is to provide three options to \fIuux\fR that modify
\fIuuxqt\fR's behavior.  They are:
.TS
cFCW l.
-n	do not request error notification
-z	request success notification
-b	return standard input on failure
.TE
Each option overrides the default behavior which is the opposite action.
In all cases of a failure, the standard error output is mailed to the originator.
Appendix VI is a sample message from UUCP resulting from a failed \fIrnews\fR
command.
Reporting error output is a significant enhancement.
Now the user is provided with all available information regarding remotely
executed commands with a general mechanism that handles all commands
equally well.
.NH 2
Forwarding
.PP
Users have always been confused by the lack of a forwarding mechanism
in UUCP.  Typically they assume that the syntax used by \fImail\fR
is correct for \fIuucp\fR.
For example,
.P1
mail harpo!allegra!princeton!honey
uucp file harpo!princeton!/usr/honey/uucp/file
.P2
Until very recently \fIuucp\fR could
not handle
such a request.  In response to this Mark Horton wrote a command (\fIuusend\fR)
which was distributed with 4.1bsd.  The \fIuusend\fR command accepted the
mail-like syntax and used the identical mechanism.  The way it worked was that
a \fIuux\fR command was issued to execute \fIuusend\fR on the next site in the
path.
There the \fIuusend\fR command issued another \fIuux\fR for the next site until
the
destination was reached.  The catch was that the \fIuusend\fR command must be
permitted to be remotely executed on each intermediate site.
In the version
of UUCP that we used as a base a forwarding mechanism was incorporated into
the code.  Unfortunately it appeared as a special case requiring additional
semantics for \fIuuxqt\fR on the remote system.  This code was immediately excised
(over 500 lines).
.PP
The \fIuusend\fR model was correct.
But it was argued that it was desirable for
\fIuucp\fR
to accept
the forwarding syntax rather than a separate command.  The other side of the
argument was that a user could generate the \fIuux\fR command and \fIuucp\fR
need know
nothing of it.  It was further argued that \fIuucp\fR itself is merely a special
case of the \fIuux\fR command (where the remotely executed command is \fIcp\fR)
and ought
to done away with (perhaps a shell procedure could take its place for
convenience's sake).  The makers of the first argument prevailed and forwarding
was put back into \fIuucp\fR.  However, rather than treat it as a special case
on the remote end, \fIuucp\fR generates the appropriate \fIuux\fR command.
This has the
advantage that it is simpler and it will work with any version.
The catch remains that the remote systems must allow the \fIuucp\fR command to
be executed by \fIuuxqt\fR.  It's interesting to note that the very first
version of
UUCP used this same mechanism to implement ``uucp tilt!file whuxlb!file''.
The
local system effectively sent a \fIuux\fR request to execute \fIuucp\fR on
\fItilt\fR to \fIwhuxlb\fR.
This was the only remote execution done by the
original (1976) version.
Thus at that time one could accomplish forwarding by a succession of \fIuucp\fR
commands.  I.e.,
.P1
uucp file ima!ist!file
.P2
could have been
accomplished by
.P1
uucp file ima!file;
sleep \fIn\fP
uucp ima!file ist!file
.P2
The only problem was knowing the correct value for \fIn\fR.
.NH 1
Priorities
.PP
Versions of UUCP since the second have had the capability to
grade file transfers by use of the
.CW -g
option.
However it's not clear
that there was an effect other than modifying the name of the command file.
In our version, the grade option does something useful.
When command files
are gathered up, they are sorted, so the grade serves to order the
processing of command files.
When used with \fIuux\fR, the grade option is
bound to the
execution file as well.
Thus upon receiving execution files, they are similarly
gathered and sorted and the grade again dictates their execution order.
The grade option could be used, for example, to provide mail with a higher
priority than netnews.
.NH 2
Sending Files That Uucp Cannot Read
.PP
For reasons of security and convenience UUCP must run under a user id
of its own.  The system would probably be more straightforward, more secure
and functionally superior if UUCP were to run as \fIroot\fR.
But that change seems drastic and would undoubtedly be unpopular.
So UUCP stands on its head
while bending over backwards to provide services and security
without a privileged user id.
There are two problems that users commonly complain about.
The first is that files received via UUCP are owned by the uucp
login and not their own.
This is more of an administrative problem than a technical one.
Although some versions of
.UX
allow users to arbitrarily change the ownership of their
files, it is clearly less desirable to allow users to do this remotely.
The problem is easy to get around on the receiving end by copying the files
and removing the originals.
The second problem is more serious.
Users sending files often find that
the request fails because \fIuucp\fR could not read their data due to stringent
protections.
This however can be dealt with since \fIuucp\fR is invoked by the
user who does have permission to access said files.
By default \fIuucp\fR does
not
copy the source files to its spool directory.
Rather, it transfers the data
directly from the specified file.
However if the file is not readable by
\fIuucp\fR,
then it makes a copy using the invoker's access privileges but owned by the uucp
login.
The file remains secure with respect to the user population because its mode
is changed to be readable only by the uucp login, after the data is copied in.
This obviates the undesirable requirement that the user make a public copy or
change the modes on the original file.
.NH 2
Documentation
.PP
Real programmers don't need auxiliary documentation, they read the
source code.  In deference to those programmers we have put
considerable effort into making the source code readable.
But the UUCP user population will include many more who don't want
to read the code (their loss).  For them we have revised the manual
pages to reflect the system more accurately.  Many new manual pages
are included covering the utilities mentioned.  Even manual pages
for the embedded programs \fIuucico\fR and \fIuuxqt\fR have been written.
Other documents have been published
by the authors|reference(uucp security)|reference(honey danber nijmegen).
A comprehensive administration guide will be included with
the ``Basic Networking Utilities, UNIX System V''.
.NH 1
Things The Administrator Will Appreciate
.NH 2
New Semantics to Support Gateways
.PP
Two options used with the \fIPermissions\fR file
allow users of UUCP to deal with some unresolved networking problems.
They are the \fIMYNAME\fR and \fIPUBDIR\fR options.
.PP
As described previously, the \fIPermissions\fR file allows the system to behave
differently depending upon the remote site it is communicating with.
The \f(CWMYNAME=\fInewname\fR option instructs UUCP to behave as if the local system
were
named \fInewname\fR.  The \f(CWPUBDIR=\fI/newdir\fR causes the local system to use
\fInewdir\fR
rather than the standard spool directory for the remote system.
Initially this was seen as a convenient mechanism for debugging.
UUCP transactions
could be made to take place within a single machine.
However some more interesting possibilities arise.
.PP
One can set up a gateway machine for a group of others.  By instructing remote
systems
to connect to the same physical machine using different logins, data destined
to the satellites will be transferred unwittingly to the gateway.  The data
accumulated in the special spool directories for the satellites can be transferred
to them and their UUCP systems can act upon it as if it had come directly from
the originating system.  Similarly data destined for a remote
system from one of the
satellites
can be redirected, using the \fIPUBDIR\fR option, to a special directory which is
then
transferred to the gateway for actual transmission to the remote system.
The beauty
of this scheme is that it is well confined and invisible to the users both
locally and remotely.
.PP
Another application might be to provide increased bandwidth among machines.
Let's say machines \fItempus\fR and \fIfugit\fR have a great deal of traffic
between them and
their communications link is not sufficient to dispose of all the data.
Then \fItempus\fR can think of \fIfugit\fR as two machines, \fIfugit1\fR and
\fIfugit2\fR.
Likewise
\fIfugit\fR will know of \fItempus1\fR and \fItempus2\fR.
Then it is a simple matter for traffic
between these two machines to utilize two separate routes.
UUCP will have
two simultaneous non-interfering connections between \fItempus\fR and
\fIfugit\fR, thereby
doubling the traffic capacity.  A similar use would be to have high priority
traffic sent to a machine using one name and other data sent using another.
Thus for example mail could be sent upon demand using a more
dear resource (like prime time Direct Distance Dialing), while netnews was
sent only over less expensive (but perhaps not as timely) facilities.
.PP
The possibilities for customization and cost reduction are vast.
These features will undoubtedly be used for applications that haven't
been considered, which is why they are so interesting.
.NH 2
Sequence Numbers
.PP
These numbers are used to provide unique file names for spooled
data.  Their purpose was often defeated on heavily loaded systems
and there has been a fair probability of conflict under certain pathological
conditions.  The implementation of sequence numbers in the second
version of UUCP was simple.  When a command or
data file was required, a sequence lock was created, the sequence number
file was read, the number incremented and written and the lock file
was removed.  The sequence number was four decimal digits.
The C. or D. file was formed from the appropriate one-character prefix, a `.', a
system name  up to seven characters (the remote's for command files,
either the remote or the local name for data files), and the sequence number.
The length of the sequence number was constrained to four characters
because the composite file name was limited to fourteen.
.PP
There are some specific problems identified with this technique.
The file system access required for locking, reading, writing, and
unlocking was expensive, especially when many data files were required.
.PP
For a given system there were @10 sup 4@ unique names
for command files and data files.
Ten thousand names were quickly used up on a very active system.
Typically, they were consumed at the
rate of three for each \fIuux\fR command and two for each \fIuucp\fR command.
Therefore if a system were to spool data for four thousand such commands before it
had transferred initial ones, the sequence number would wrap around and
there would be a high probability (1/3) of a name clash.
This
is not farfetched considering that a site distributing netnews to four others
(at 250 articles per day) will go through 1000 sequence numbers daily.
.PP
Because \fIuux\fR had named its data input files as D.remote\fInnnn\fR, a system
may receive files named identically from different remotes.
The likelihood
of this event was also proportional to the number of systems for which
jobs are remotely executed.  If a system receives netnews from
four others, but for some reason can't process the requests because of
the positive
feedback discussed earlier, the probability of a name clash is at least
one in ten after a day.  In fact it may be much greater because the sites
sending data are not likely to have mutually exclusive sequence numbers.
.PP
A solution to these problems was first introduced by Alan Watt of ITT.
He generated sequence numbers from the set of alphanumeric characters.
This extended the name space from @10 sup 4@ to @62 sup 4@.
(He also cached sequence numbers on each access to the sequence file.)
Although this deals with
locally generated names it provides no defense from receiving name
clashes.  There is also the aesthetically unpleasant aspect of creating
garbage names which may randomly turn out to be offensive.
.PP
Our version of UUCP deals handily with these problems.
The sequence number is seven hex digits.
Four of these digits are derived from a sequence number file.
The remaining three digits represent a cache of sub-job numbers
that modify the base number.
Thus a command will require only one access to the
file for up to @16 sup 3@ sequence numbers.
Since we use separate spool directories for each remote site, name
clashes will not occur when different systems send us the same file
names.
The initial base number is selected randomly, reducing the
possibility that we would generate names that clash 
on a remote site running an old version of UUCP.
The use of seven hex digits and the fact that a different number sequence
is maintained for each remote site allows us to avoid the use of alphabetic
sequences.
.NH 2
A New Locking Mechanism
.PP
Previous versions of UUCP relied on the modification time of a lock file.
It was assumed a file
older than some threshold was invalid.  This led to problems
when UUCP (or other programs sharing resources with UUCP) executed
for extended periods of time.  One awkward solution was to have programs
periodically touch the lock file.  Our solution relies on a property of
System V that enables a process to determine if it can kill another without
actually disturbing it.  The identifier of the process using a resource
is recorded in the lock file.  When another process examines the lock file
it issues a \fIkill\fR(0) to the identifier contained within.
If the \fIkill\fR succeeds
this indicates that the specified process is still running.  If it fails
\fIerrno\fR indicates if it was due to lack of permission (cu processes
will have different user ids).
If the process no longer exists,
the lock file is assumed to be invalid.  The code to add this function
to other versions of
.UX
is slight and straightforward and is included with the source distribution.
Alternatively a routine could be written that uses
\fIps\fR to glean
the same information, though this would be considerably less efficient.
For systems that don't implement this property of \fIkill\fR, UUCP may be
configured to use the old mechanism.
.NH 2
Giving Up
.PP
``RETRY TIME NOT REACHED'' or ``CONNECT FAILED MAXRECALLS'' messages
indicate
that UUCP has given up trying to connect to a remote system.
If a connection fails, when do you try again?
Historically UUCP tried again every 55 minutes. After nine hours
and ten minutes it gave up for good.
This algorithm is clearly inadequate.  If a system was down for
more than ten hours, manual intervention was required.
On the other hand, perhaps 55 minutes was too long to wait between
attempts, but to lower that would mean it would give up after an even
shorter period.
We have replaced the \fIRETRYTIME/MAXRECALLS\fR concept with an exponential
backoff algorithm.
The early attempts are frequent and continue to
be spaced out in time until an attempt is made once every 23 hours.
UUCP will continue trying daily forever.
Optionally the \fISystems\fR
file may indicate that a given system should be treated differently
and attempts will be made continually at the specified interval.
One can of course easily compile in a custom algorithm.
.NH 2
Sharing Modules
.PP
As noted previously UUCP incorporates several functions that
are of value to other applications.  The locking routines can be used
by any facility, which is why lock files were placed in a more generic
location.
The connection function is being used by the \fIcu\fR and
\fIct\fR programs as
well as the various spoolers for line printers, typesetters, etc.
Any utility that needs to establish a bidirectional connection to a shared
resource can make use of these facilities.
The packet driver which was once part of the
.UX
kernel could be
more useful to other communications programs if it were returned there.
.NH 1
Roadmap
.NH 2
Utilities
.PP
The new version of UUCP comes with programs
to assist in setup, administration and debugging.  They are:
.IP "\fICvt\fR\ \ " 8
converts existing data, command and execution files from the old format.
.IP "\fIInstall\fR\ \ "
puts programs in the right place with the right modes.
.IP "\fIUutry\fR\ \ "
starts a \fIuucico\fR to a remote system.  It places debugging output
in a file and tails it.  It optionally causes the status file to be ignored.
.IP "\fISetUp\fR\ \ "
creates all the necessary UUCP system files and renames as necessary
(e.g., \fIL.sys\fR @->@ \fISystems\fR).
.IP "\fIremote.unknown\fR\ \ "
optionally invoked by \fIuucico\fR if an unknown system calls.
This procedure logs the caller and time.
.IP "\fIuudemon.admin\fR\ \ "
sends status information to the administrator.
.IP "\fIuudemon.clean\fR\ \ "
does routine maintenance of log files and invokes the
\fIuucleanup\fR program (previously described).
.IP "\fIuudemon.hour\fR\ \ "
invokes \fIuusched\fR and \fIuuxqt\fR.
.IP "\fIuudemon.poll\fR\ \ "
implements polling utilizing a file that describes which systems are to
be polled at what times.
.IP "\fIuukick\fR\ \ "
starts a \fIuucico\fR for a system, no debugging.
.IP "\fIuulog\fR\ \ "
examines or monitors log files
.IP "\fIuuto\fR\ \ "
sends files and/or directories to a remote user.
.IP "\fIuupick\fR\ \ "
retrieves data sent via \fIuuto\fR and interactively relocates it.
.IP "\fIuucheck\fR\ \ "
verifies that required files and directories are present.
.IP "\fIuucleanup\fR\ \ "
disposes of defunct files in an \fIintelligent\fR manner.
.IP "\fIuustat\fR\ \ "
displays the status of or cancels previously specified UUCP commands
or provides general status on connections to other systems.
It's worth noting that this version of \fIuustat\fR does its job by examining
the log files.  It requires no hooks in the core programs.
.IP "\fIuusched\fR\ \ "
deals with all the scheduling aspects of UUCP.
In a move to reduce the complexity of the core programs \fIuusched\fR was
invented.  
It determines which systems have work and if it's the right time to call (by
examining the \fISystems\fR file).  It optionally limits the number of
\fIuucico\fR
processes
that can be executing at once.
This program relieves \fIuucico\fR of the need to
search through all the system subdirectories for work.
\fIUucico\fR then is always
invoked with the name of the system to which it will connect.
.IP "\fIuugetty\fR\ \ "
may be used instead of the standard \fIgetty\fR to
allow lines to act both as inbound (normal login) and outbound ports for
\fIuucico\fR, \fIcu\fR, and \fIct\fR.
\fIUugetty\fR only works with System V versions of
.UX .
.KF
.PS 2.5i
box invis ht 352 wid 328 with .sw at 0,0
"\fR\s10\&Figure 2.  \f(CW/usr/spool/uucp\f1\s0" at 184,-10
"\f(CW\s10\&harpo\f1\s0" at 336,306
"\f(CW\s10\&chico\f1\s0" at 332,274
"\f(CW\s10\&zeppo\f1\s0" at 292,250
"\f(CW\s10\&.Corrupt\f1\s0" at 268,202
"\f(CW\s10\&.Admin\f1\s0" at 228,170
line  from 256,80 to 256,56 
line  from 256,80 to 240,64 
line  from 256,80 to 272,64 
line  from 208,64 to 208,40 
line  from 208,64 to 192,48 
line  from 208,64 to 224,48 
line  from 160,64 to 160,40 
line  from 160,64 to 144,48 
line  from 160,64 to 176,48 
line  from 120,80 to 104,64 
line  from 120,80 to 120,56 
line  from 120,80 to 136,64 
"\fI\s10\&uucico\f1\s0" at 256,86
"\fI\s10\&uuxqt\f1\s0" at 208,70
"\fI\s10\&uux\f1\s0" at 160,70
"\fI\s10\&uucp\f1\s0" at 120,86
line  from 184,352 to 144,184 
"\f(CW\s10\&.Log\f1\s0" at 184,138
line  from 184,352 to 184,152 
"\f(CW\s10\&.Status\f1\s0" at 144,170
"\f(CW\s10\&.Workspace\f1\s0" at 104,202
line  from 184,128 to 248,96 
line  from 184,128 to 208,80 
line  from 184,128 to 160,80 
line  from 184,128 to 120,96 
line  from 184,352 to 328,320 
line  from 184,352 to 328,288 
line  from 184,352 to 288,264 
line  from 184,352 to 264,216 
line  from 184,352 to 224,184 
"\f(CW\s10\&.Xqtdir\f1\s0" at 76,242
line  from 184,352 to 104,216 
line  from 184,352 to 80,256 
line  from 40,264 to 40,240 
line  from 40,264 to 24,240 
line  from 40,264 to 0,248 
"\f(CW\s10\&.Sequence\f1\s0" at 44,274
line  from 184,352 to 48,288 
"\f(CW\s10\&exodus\f1\s0" at 40,306
line  from 184,352 to 48,320 
.PE
.KE
.NH 2
Subdirectory Mania
.PP
In rearranging the data associated with UUCP, there is some concern
that we may have gone overboard.  See Figure 2.  On the other hand
our structure may not be complete in that perhaps lock files should
be stored hierarchically under \fIsystems\fR, \fIdevices\fR, and
\fIprocesses\fR directories.
(As it now stands \fIuuxqt\fR cannot make lock files incorporating fourteen
character system names.)
The directories are:
.IP "\f(CW.Status\fR\ \ " 8
a status file for each system.
.IP "\f(CW.Log\fR\ \ "
contains subdirectories for each core program which in turn contain
log files for each system.
.IP "\f(CW.Admin\fR\ \ "
This directory contains a miscellaneous collection of files including:
.RS
.PP
an audit file that is appended to when
a remote system invokes \fIuucico\fR with a debugging option,
.PP
a file in which assert errors are logged,
.PP
the file transfer statistics which
include the system, role, time of day, process id, device, direction of transfer,
number of bytes, and time in milliseconds for each transfer,
.PP
and log files created by the \fIuucleanup\fR program telling about
deletions and such.
.RE
.IP "\f(CW.Corrupt\fR\ \ "
where corrupt command and execution files are placed.
.IP "\f(CW.Old\fR -
for old log files being rotated out.
.IP "\f(CW.Sequence\fR\ \ "
files for each system containing the current sequence number.
.IP "\f(CW.Workspace\fR\ \ "
where command and data files are formed before being committed
to their final directory.
.IP "\f(CW.Xqtdir\fR\ \ "
where remotely generated executions take place.
.NH 1
Esoterica
.NH 2
Software Engineering or Hacking by Committee?
.PP
In late April of 1983 a diverse group of people from Bell Labs
met to discuss the state of UUCP and how to improve it.  These
people came from many different geographical and technical areas.
It was an informal gathering of interested parties who for the most
part were working on various projects not related to UUCP. However they
all had intimate dealings with UUCP and wanted to see a system that
was common among them so their efforts could
be combined towards a single improved version.  The notes from that meeting
are reproduced in Appendix VII.
.PP
The meeting provided the incentive and the mandate to do something
about UUCP.  Although several activities were in progress, none
seemed to address all the problems comprehensively.  The effort began
in individual stages.  The most current version of UUCP from the
.UX
Development Lab was delivered to Redman in late April.  The first step
was to add the appropriate code using \fIifdefs\fR so that it could be
compiled under 4.1bsd (Redman's base system).
This was done within a week.
Next, Redman modified the system to use separate spool directories.
The first version was complete towards the end of May.  In the beginning
of June
Honeyman became active and his \fIrational connection functions\fR were incorporated.
Midway through July Nowitz contributed the \fIpermissions\fR code.  At this point
the effort became totally collaborative among the authors.  By the end of July
UUCP was being distributed among those at the April meeting for comments and
enhancements.  Through this process many valuable contributions were received
from the community and the system was soaked in many environments including
all modern versions of
.UX
(4.0, System V, 4.1 bsd, 4.1c bsd, 4.2 bsd and Eighth
Edition) representing systems heavily
loaded with mail forwarding, netnews and software distributions.
.PP
In late September we began reviewing all modules and refining them to
meet standards of style and function.
This phase of our work
was quite useful: as the code was thoughtfully reworked,
design problems became apparent.  Following stylistic conventions
consistently improved the readability.  Its complexities were ironed out
resulting in more comprehensible code which tended to be more efficient.
.PP
From the start SCCS was used to document changes.  Figure 3 shows the
activity over time in lines of code changed.
The version we started with as a base comprised about 13,000 lines of code.
The current system is 14,000 lines.  These numbers represent the removal
of approximately 3,000 lines of code from the delivered system and the
subsequent addition of 4,000 lines.  Although it would have been
gratifying to show a net reduction in the size of the system, the additional
strengths and capabilities justify the growth.
The inclusion of 5,000 lines of documentation
and installation and conversion scripts is also compensation.
(While we're throwing numbers around, it's interesting to note that
the communication among the authors during this development was primarily
via electronic mail.  Approximately 2,000 messages were sent, totaling
on the order of 50,000 lines.  It was truly an Information Age effort.)
This was software engineering rather than hacking because a lot of
people spent a lot of time on it.
.KF
.ps -2
.G1 2.5i
frame invis ht 2 wid 3.5 left solid bot solid
label left "lines" "of code" left .2
label bot "Figure 3.  SCCS Activity"
coord x 3,10 y 0,4096
ticks left in at 0, 256, 512, 768, 1024 "1K", 1280, 1536, 1792, 2048 "2K", 2304, 2560, 2816, 3072 "3K", 3328, 3584, 3840, 4096 "4K"
ticks bot out at 4 "Apr", 5 "May", 6 "Jun", 7 "Jul", 8 "Aug", 9 "Sep"
draw solid
4 170
5 1010
6 1435
7 4000
8 3275
9 1530
new dotted
4 85
5 550
6 2560
7 3480
8 2760
9 1125
"Insertions" size -3 at 9.5, 2100
"Deletions" size -3 at 8, 1600
.G2
.KE
.PP
One of the problems we described earlier was the entropic forces on the
UUCP software.  We know of no solution to the problem that won't result
in stagnancy.  We have invested considerable effort
to produce a system that is better understood and more modular with
the prospect of easing the implementation of changes.  Our best effort then
to deal with change is to recognize that it is inevitable, make allowances
so it is less destructive, and to provide a fresh system for fodder.
.NH 2
Controversy
.PP
No program of this complexity and with such widespread use can be redesigned
without having to make many controversial decisions.
Sometimes there
is no ``right'' way to do a thing.
Sometimes the right way is unpopular
because conventions embrace the ``wrong'' way.
Like most implementors we have been
somewhat stubborn about providing functionality that is orthogonal to our
purpose of correctness.
.PP
A good example is the issue surrounding the length of site names.
We support system names up to a practical limit of fourteen characters
(although a greater number is possible).  The original version of UUCP
only allowed seven characters due to limitations that have been described.
Recently a version of UUCP has been introduced that only permits six
character site names.  Here are the problems.
.PP
It is common for a system to call itself by a name that is greater than
seven characters (e.g., \fIresearch\fR).
In previous versions of UUCP such
a system would have an effective seven character name (i.e., \fIresearc\fR).
There was no problem because the site name was truncated immediately before
any table lookups.  Now we have extended the limit.
We have a system called
\fIresearch\fR in our \fISystems\fR file.
When a system calls us and purports to
be \fIresearc\fR we have a problem.
Is \fIresearc\fR equivalent to \fIresearch\fR?
If we treat it as such, then we have effectively limited names to seven
characters.  Our purpose was to extend the name space.  Therefore we
treat \fIresearc\fR and \fIresearch\fR as two different systems.  In order that
this enhancement not be incompatible with other systems, they are required
to identify themselves with their full name.  To change any version
of UUCP to do this is quite trivial, (both in source and binary versions),
yet it does require a change.  The dilemma is whether to make use of the
flexibility provided or to restrict it on behalf of those that for some
reason or another cannot cope with the changed behavior.
.PP
Lauren Weinstein has proposed an enhancement that addresses the problem.
He has suggested that UUCP be given the knowledge of which systems are
compatible and which are not and change its behavior accordingly.
Given that this can be implemented, a new question arises.  Does the additional
complexity and inconsistent behavior serve our purposes or is it a vain attempt
to please all people all of the time?
This issue is still open.
.PP
A different issue was resolved more quickly because of the mass hysteria
which ensued.  \fIUux\fR keeps a record of the system and user for which
requests are generated and passes it to the \fIuuxqt\fR program on the remote
system via the execution file.
This is the \fIU line\fR.
Unfortunately a fresh
U line is generated by each invocation of \fIuux\fR.
Therefore when mail is
sent from \fIrob\fR on \fIdagaboh\fR to \fIdecvax!mcvax!bellcore!psl\fR, the U line on
\fIdecvax\fR is indicated as
\fIdagaboh!rob\fR.
When \fIdecvax\fR processes the request,
it creates a new U line
containing \fIdecvax!uucp\fR.
When the
request arrives on \fImcvax\fR and for one reason or another cannot be
executed, an indication is sent to \fIdecvax!uucp\fR rather than
\fIdecvax!dagaboh!rob\fR.
The solution was clear!
Build the U line from the previous one.  A new
flag was added to \fIuux\fR to facilitate this.  This solution quickly became
a problem.  Most versions of \fIuuxqt\fR read the U line into a buffer limited
by sixteen characters in length.  The result of processing our more informative
U line caused versions of \fIuuxqt\fR to dump core on most sites throughout
the network.  This caused their systems to cease processing remote
executions because they could never get past our execution file.  The
network was backed up for days.
When confronted with these implications
our response was one of cool headed reason.
We suggested that everyone
increase the buffer allocation.
After all, sixteen was too small anyway because,
as had been pointed out in previous bug reports, some systems had increased
the maximum number of characters for a login name from eight to sixteen.
Our reason did not prevail.
We knuckled under, left the U line alone, and
now add this information via a new record (the R line) which although
less elegant, is less hazardous.
.PP
Finally, there is a problem that is of no great philosophical importance.
\fIUucp\fR (and now \fIuux\fR) either copies its source files to the spool
directory
or it transfers them from their original locations.  The former method
ensures that the user encounters no surprises.  The latter is vastly more
efficient.  Either is an acceptable default provided it can be
overridden.  The problem is that all versions of UUCP that descended from
the V7 line (notably 4.xbsd) use the former mechanism.  All versions that
came from PWB (notably System V) use the latter.  Although the default
can be readily changed at compilation time, it would be useful for a standard
behavior to evolve.
We have chosen the PWB default.
Some people are already unhappy.
.NH 2
Global Problems Requiring Further Investigation
.PP
The issue of backward compatibility is a thorn in the
side of advanced developments.  Technology often provides us with wonderful
solutions that cannot be effectively deployed because they are incompatible
with other systems.  An extreme position is to ignore the problem, cite that
the new way is the right way and other systems must change or die off.  This
spurs evolution but spurns practicality.  It is a position often taken in
research (and rightly so).
The other extreme is to use technology to provide the same old
capabilities more cheaply.
This does not support the ability to use resources in
new ways for different purposes.
Unfortunately the compromise solution has two
drawbacks: it is expensive to provide multiple functionality supporting
both the past and the future, and by maintaining outmoded
technology, you are encouraging it.
.PP
Another issue concerning development and maintenance is perhaps
more amenable to compromise.
This is the consideration of the type of working environment imposed.
On the one hand there is the advantage of a tight control.
Some problems are avoided because designs are proposed, reviewed,
tested and scrutinized before they are unleashed.
This is in contrast to an environment where ideas
are quickly implemented and distributed without an overburdening bureaucracy.
The appropriate environment depends as much on the individuals involved as
the requirements.  Too often one approach or the other is taken as a matter
of policy.
.NH 2
Future Work
.PP
We had planned for this to be the perfection of software art.
It has been approaching that asymptotically.
However, in order that it be made
available for its intended use, there are several things left undone
(although development continues for future releases).
It should be run with controlled conditions in
various environments and systematically tuned.  Different algorithms should
be available for use in different circumstances (e.g., memory rich vs. low budget
computers).  A non-spooling system is desired for use with very fast networks
that provide immediate response.
Also, encrypting of data files may be a useful enhancement.
.NH 1
Conclusion
.PP
This work may serve as a good example of a small team development
of a modestly large program.  We have retained the data which
describes our efforts in the form of electronic mail and SCCS versions.
From these it can be seen how the work was partitioned, how decisions were
made, where the difficulties arose, where we got in each other's way and
where we stood on each other's shoulders.  All in all, what we set out to do,
how we approached it, what we did and why is fairly accurately chronicled.
.PP
Uucp has been rewritten for the third time.  The new system is healthier
and more versatile than its predecessors.  We have attempted to provide a system
that is useful for all sorts of networking applications.  We also hope that
this version may be used to bring UUCP maintainers back to a common system so
that their future developments may be shared more vigorously. 
.SH
Acknowledgements
.PP
We thank the following for their substantive work and comments:
adiron!bob, axiom!smk, cbosgd!mark, masscomp!trb, 
ihnp4!gjm, ittvax!swatt, mouton!karn, ncsu!mcm, rti!trt, 
sun!shannon, watmath!arwhite, icarus!alb,
watmath!dmmartindale, whuxlb!eric.
For moral support, thanks to
decvax!larry, exodus!dvw, gummo!mmp, parsec!kolstad, rabbit!ark,
research!dmr, research!doug, utzoo!henry, vax135!martin, watmath!bstempleton.
.PP
Thanks to  allegra!jpl who reviewed the code with us and contributed
some of his own.  Our appreciation goes to down!pep who can spot a bug on a post
at 100 yards.  To research!rtm for some fine ideas taken from his rewrite.
To teklabs!stevenm for compiling the buglist.  To vortex!lauren for always
being able to see the other side of an issue.  To ulysses!smb for doing most
of the work involving 4.1c/4.2bsd.  A special note of appreciation to bellcore!mel
for dreaming up such a system that would keep so many people busy for so long
improving it.  And an extra acknowledgement to sfwoo!dan for hacking UUCP
with such enthusiasm after these many years and for being able to tell the
rest of us what the variable names meant.
.SH
References
.PP
|reference_placement
.FC
.1C
.BP
.SH
Appendix I - Sample Permissions File
.P1 0
# This entry for public login
# Use default permissions
LOGNAME=nuucp

# This for some friendly outside sites when they call us
# They each have a separate login.
# When they call, we will send queued files
LOGNAME=harpo:gummo:allegra:mhtsa:mhuxt \e
  SENDFILES=yes \e
  WRITE=/usr/spool/uucppublic:/usr/RNEWS

# This entry for when we call these people.
# They also can execute a couple of additional commands.
# The commands are safe, so VALIDATE is not necessary on the LOGNAME entry
MACHINE=mh3bs:harpo:gummo:allegra:mhtsa:mhuxt:pwbqq \e
  WRITE=/usr/spool/uucppublic:/usr/RNEWS \e
  COMMANDS=rnews:rmail:xp:lp

# This entry for machines in our room (when they call us)
# The sites that login with these login-ids have extra command
# privileges, so VALIDATE name vs login-id
# (See next entry--the MACHINE values are related to these VALIDATE values)
LOGNAME=uucp:uucpl \e
  VALIDATE=raven:owl:hawk:dove \e
  REQUEST=yes SENDFILES=yes \e
  READ=/ WRITE=/

# This entry for machines in our room -- when we call them
# It also specifies the commands they can execute locally.
# (The uucp command in COMMANDS option permits forwarding.)
MACHINE=owl:raven:hawk:dove \e
  REQUEST=yes \e
  COMMANDS=rnews:rmail:xp:lp:uucp \e
  READ=/  WRITE=/

# This entry to call back on our faster link
LOGNAME=uucpm MACHINE=mhwpf \e
  COMMANDS=rnews:rmail:xp:lp \e
  CALLBACK=yes
.P2
.SH
Appendix II - Output From \f(CWuucheck -v\fP
.P1 0
*** uucheck:  Check Required Files and Directories
*** uucheck:  Directories Check Complete

*** uucheck:  Check /usr/lib/uucp/Permissions file
** LOGNAME PHASE (when they call us) 

When a system logs in as: (nuucp) 
	We DO NOT allow them to request files.
	We WILL NOT send files queued for them on this call.
	They can send files to
	    /usr/spool/uucppublic (DEFAULT)
	Myname for the conversation will be yquem.
	PUBDIR for the conversation will be /usr/spool/uucppublic.

.P3
When a system logs in as: (harpo) (gummo) (allegra) (mhtsa) (mhuxt) 
	We DO NOT allow them to request files.
	We WILL send files queued for them on this call.
	They can send files to
	    /usr/spool/uucppublic
	    /usr/RNEWS
	Myname for the conversation will be yquem.
	PUBDIR for the conversation will be /usr/spool/uucppublic.
.P3
When a system logs in as: (uucp) (uucpl) 
	We DO allow them to request files.
	We WILL send files queued for them on this call.
	They can send files to
	    /
	They can request files from
	    /
	Myname for the conversation will be yquem.
	PUBDIR for the conversation will be /usr/spool/uucppublic.

When a system logs in as: (uucpm) 
	We will call them back.


** MACHINE PHASE (when we call or execute their uux requests) 

When we call system(s): (mh3bs) (harpo) (gummo) (allegra) (mhtsa) (mhuxt) (pwbqq) 
	We DO NOT allow them to request files.
	They can send files to
	    /usr/spool/uucppublic
	    /usr/RNEWS
	Myname for the conversation will be yquem.
	PUBDIR for the conversation will be /usr/spool/uucppublic.

Machine(s): (mh3bs) (harpo) (gummo) (allegra) (mhtsa) (mhuxt) (pwbqq) 
CAN execute the following commands:
command (rnews), fullname (rnews)
command (rmail), fullname (rmail)
command (xp), fullname (xp)
command (lp), fullname (lp)

When we call system(s): (owl) (raven) (hawk) (dove) 
	We DO allow them to request files.
	They can send files to
	    /
	They can request files from
	    /
	Myname for the conversation will be yquem.
	PUBDIR for the conversation will be /usr/spool/uucppublic.

Machine(s): (owl) (raven) (hawk) (dove) 
CAN execute the following commands:
command (rnews), fullname (rnews)
command (rmail), fullname (rmail)
command (xp), fullname (xp)
command (lp), fullname (lp)
command (uucp), fullname (uucp)

When we call system(s): (mhwpf) 
	We DO NOT allow them to request files.
	They can send files to
	    /usr/spool/uucppublic (DEFAULT)
	Myname for the conversation will be yquem.
	PUBDIR for the conversation will be /usr/spool/uucppublic.

Machine(s): (mhwpf) 
CAN execute the following commands:
command (rnews), fullname (rnews)
command (rmail), fullname (rmail)
command (xp), fullname (xp)
command (lp), fullname (lp)


*** uucheck:  /usr/lib/uucp/Permissions Check Complete
.P2
.BP
.SH
Appendix III - Sample Devices File
.LP
.KS
.TS
l l l l l l.
CALLER	LINE	USEFUL	CLASS	DIALER	CHAT SCRIPT
=
.T&
l s s s s s.
# the ACU's
#
# 212/801 dialers
.T&
lFCW lFCW lFCW lFCW lFCW lFCW.
ACU	cul0	cua0	1200	212	unused
ACU	cul1	cua1	1200	212	unused
.T&
l s s s s s.
# VenTel dialer
.T&
lFCW lFCW lFCW lFCW lFCW lFCW.
ACU	vn0	unused	1200	ventel	unused (for now)
ACU	vn0	unused	300	ventel	unused (for now)
.T&
l s s s s s.
# Vadic dialer
.T&
lFCW lFCW lFCW lFCW lFCW lFCW.
ACU	vd0	unused	1200	vadic	unused (for now)
.T&
l s s s s s.
# special entry for Vadic only systems
.T&
lFCW lFCW lFCW lFCW lFCW lFCW.
ACU	vd0	unused	V1200	vadic	unused (for now)
.T&
l s s s s s.
#
# the Micom also has some VenTels
.T&
lFCW lFCW lFCW lFCW lFCW lFCW.
ACU	Micom	secret	1200	micomventel	unused (for now)
ACU	Micom	secret	300	micomventel	unused (for now)
.T&
l s s s s s.
#
#
# the switches
#
# Micom pbx
# 4800 baud is funny ...
.T&
lFCW lFCW lFCW lFCW lFCW lFCW.
Micom	mc0	unused	4800	unused	\`\`\'\' \es\ec NAME? %s GO \ec
Micom	mc0	unused	Any	unused	\`\`\'\' \`\`\'\' NAME? %s GO \ec
Micom	mc1	unused	4800	unused	\`\`\'\' \es\ec NAME? %s GO \ec
Micom	mc1	unused	Any	unused	\`\`\'\' \`\`\'\' NAME? %s GO \ec
.T&
l s s s s s.
# Develcon pbx
.T&
lFCW lFCW lFCW lFCW lFCW lFCW.
Develcon	dv0	unused	Any	unused	\`\`\'\' \`\`\'\' Request: %s \e007 \ec
Develcon	dv1	unused	Any	unused	\`\`\'\' \`\`\'\' Request: %s \e007 \ec
.T&
l s s s s s.
# DATAKIT PS
.T&
lFCW lFCW lFCW lFCW lFCW lFCW.
Datakit	dk0	unused	Any	unused	\`\`\'\' \ed%s
Datakit	dk1	unused	Any	unused	\`\`\'\' \ed%s
.T&
l s s s s s.
# gandalf
.T&
lFCW lFCW lFCW lFCW lFCW lFCW.
Gandalf	gd0	unused	Any	unused	\`\`\'\' \`\`\'\' class %s start \ec
Gandalf	gd1	unused	Any	unused	\`\`\'\' \`\`\'\' class %s start \ec
.TE
.KE
.SH
Appendix IV - Sample Systems File
.LP
.KS
.TS
l l l l l l.
SITE	WHEN	CALLER	CLASS	CALLCODE	LOGIN
=
.T&
lFCW lFCW lFCW lFCW lFCW lFCW.
fonzie	Any	ACU	D1200	MHd1234	...
fonzie	Any0631-0444	ACU	C1200	MH5678	...
fonzie	Any0631-0444;5	Micom	Any	fonz	...
fonzie	Wk1800-0600,Sa	Datakit,dg	unused	fonzie	...
fonzie	MoWeFr1300-1445	Ethernet,eg	unused	09	...
fonzie	Any	Direct	9600	tty42	...
.TE
.KE
.BP
.SH
Appendix V - Heuristics Used by Uucleanup
.PP
At present, this is what is done:
.PP
For WARNING messages:
.IP
C. files of the age specified are read, looking for either user
files to be sent or received, or mail to be sent.  (Other
remote execution that does not involve sending user files
is not checked for now.)  In either of the cases, the user
is informed by mail that the request is not being processed
due to lack of communications with the remote system, and
the request will be deleted in the future if it the condition
remains for several more days.
.LP
For DELETIONS:
.IP "C. files -" 10
If they reference only D. files, the C. is merely deleted,
because the D. files are usually mail or news, and the later
D. processing will take care of them.
If they reference files from the file system,
a message is constructed that will contain lines like:
.P1 10n
We can't contact the remote.

local!file -> remote!otherfile

can't be executed.
.P2
.IP "X. files -"
These are merely deleted at present - D.s will be taken care of later.
Besides, some of the D.s are missing or else the X. wouldn't be
left around.
.IP "D. files -"
mail type data is sent to a local person if that is where it
was destined.  If not, it is returned to the sender -- assumed
to be from the first From line.  If a sender can't be determed,
the file is merely deleted.
.IP ""
netnews: if locally generated, just delete.  If remote, the X.
got lost, so execute rnews.
.IP "Other files -"
Just delete them.
.PP
Deletions and executions are logged.
.SH
Appendix VI - Example of a Uuxqt Error Report
.P1 0
remote execution	[uucp job allegraA60f1 (6/6-2:50:32)]
	rnews 
exited with status 1


	===== stderr was =====
rnews: Cannot open /usr/spool/news/.sys (r) (From: harpo!decvax!pur-ee!iuvax!dcm).
	perror: No such file or directory


	===== stdin was =====
From: harpo!decvax!pur-ee!iuvax!dcm
Newsgroups: net.unix-wizards
Title: uucp trap 9 ever fixed?
Article-I.D.: iuvax.119
Posted: Fri Jul  2 11:48:26 1982
Received: Sat Jul  3 00:54:31 1982

We (a VAX 4.1) are getting alot of trap 9s, probably from 
the bug mentioned in rv(4); has anyone ever fixed this?
				pur-ee!iuvax!dcm
.P2
.BP
.SH
Appendix VII - Notes From Uucp Lovers Meeting
.FC
.2C
.PP
The first (and last) meeting of the uucp-lovers interest group
took place on 20 April 1983.
Invited were
allegra!honey, vax135!martin, eagle!karn, rabbit!ark, mhb5b!smb,
harpo!ber, cbosgd!mark, floyd!trb, research!rtm, and eagle!dan.
Others who showed up were representatives from USG,
mhtsa!lsc and mhtsa!brad,
whuxlb!pep and gummo!mmp.
The unexpurgated minutes follow.
.PP
.nf
From honey Wed Apr 20 00:46:26 1983
To: uucplovers
Subject: minutes
Cc: dmr doug
.PP
.ad l
pardon my editorial asides in what follows, the minutes of a meeting
called by levy to discuss uucp concerns.  attendants at today's
gathering of the clan were
.PP
.in +5n
eagle!dan, vax135!martin, ihnp4!gjm, gummo!ber, mhtsa!lsc,
mhb5b!smb, mhtsa!brad, eagle!karn, whuxlb!pep, and
allegra!honey
.PP
.in -5n
the first topic of discussion was the corporate electronic mail project
and its sidekick, network action central.  action appears to be jointly
administered through 452 (staff) and 774 (equipment and organizational
support).  murakami lent the impression that this was the beginning of
a permanent fixture w/in the labs and that any startup problems are
temporary.  the L.sys database is gradually falling together as new
data are collected and standards are promulgated.  discussion about the
means for handling the monthly L.sys produced a consensus that sites
should continue to maintain a local L.sys and that the local file should be
searched first, a la koenig, honeyman, and apparently everyone.
.PP
a lively discussion on uucp hacking consumed the major part of the
meeting.  there are as many versions of uucp as there are sites, with
the major contenders being usg 6.0, the new code by morris, and tom
truscott's hacks.  cohen grimly cautioned on the difficulty of getting
good stuff into 6.0, nonetheless, the company flag was raised, all
saluted, and we agreed to use the organizational heavy as a starting
point for producing a version that would satisfy all (and ourselves, in
particular).
.PP
the major enhancement proposed was a hashing scheme for the spool
directory.  while truscott uses separate directories for the C and D
files (as well as other files, i imagine), redman and others use the
names of the remote sites as the subdirectories.  lively discussion
ensued, during which a consensus was reached that the latter scheme was
more robust.  a proposal by levy that the hash function be based on a
short prefix of the machine name was met with interest.  while there
appears to be a problem with mkdir by setuid processes in usg, bellovin
patiently explained techniques to subvert this feature; after the third
or fourth go 'round, the meeting got back on track.  the feeling is
that cico or some daemon should do occasional rmdir's in the spooling
directory; ultimately there was not total agreement on all of the
mechanics of the hashing scheme, and i imagine the issue will be
discussed further in a manner fit for the information age.
.PP
a separate directory for the lock files was also proposed, a point on
which all were agreed, as this admits some gestures toward privacy made
impossible at present due to cu's use of lock files.  in addition,
nowitz and karn muttered about lock file aging.
.PP
while alan watt's code produces wild and amusing sequence numbers,
several people feel uncomfortable with his hacks.  a proposal for a
sequence file per system was propounded, but the necessity to shelter
several hundred seq files was judged problematic.  nowitz finally
suggested maintaining a single file with a separate sequence number for
each machine.  there were murmurs of assent over that, as well as the
suggestion that logging info be maintained per system, which everyone
wants to see reduced.  there was some agreement that morris's command
file per system is also a good idea, but out of its time.
.PP
there were several pleas for some sort of indexing scheme on L.sys.
karn has built weinberger's btrees on L.sys (but i predict problems
putting pjw's apparently top secret, company private hacks into system
VI), honeyman suggested dbm, while levy suggested a version of look.
it's too early to tell which way this one will go.
.PP
cohen reports that 6.0 incorporates all of the "known" bug fixes
(mcgeady's list, other netnoise reports), although honeyman, nowitz,
and redman immediately popped up with some "unknown" bugs.  (i remind
you all to #define NPLINES to 20 or so).  horton's recent note on
delays in the packet driver was discussed and duly deprecated.
.PP
a proposal to upgrade the DEBUG information was thrown out for
consideration whereupon it was thrown out for lack of interest.
.PP
finally, we agreed to take the most recent version of 6.0 uucp and hack
it into an unrecognizable state (i.e. remove the bugs, and make it work
well).  honeyman commandeered conn.c, redman got the spooling hacks,
levy was volunteered to write an L.sys editor and help murakami
standardize the action L.sys, and bellovin accepted the responsibility
for putting the spool directory mods into uuxqt.  cohen pointed out
that usg is unlikely to appreciate anything that falls under the ucb
umbrella, while nowitz argued against reliance on ifdefs.  the feeling
was that we can hide the idiosyncrasies in a single file and mask our
efforts as a gesture toward version 7 compatibility, a transparent
but innocent falsehood.  in any case, this issue is peripheral.
.PP
the final topic on the agenda was the corporate attempt at security
consciousness raising; a shouting match ensued, in the course of which
several and various reputations were sullied, certain paranoid
reactions were taken less than seriously, and no great meeting of the
minds was met.  honeyman argued in favor of mcilroy's position against
allowing remote machines to pull files, falling back on a position that
denied this capacity to all but a local cluster, falling further back
on a suggestion that permits files to be pulled from a directory other
than uucppublic (so that one remote site can't pull a file that another
remote site has sent).  redman pointed out that the only information
worth protecting in L.sys is the phone number (certainly the least
secure piece of data).  redman solicits opinions on the forthcoming GEI
regulations.
.PP
upon notable growlings of empty stomachs, a decision was made to
adjourn to a local chinese restaurant, in the time honored tradition of
uucp hackers everywhere.  this decision was subsequently rescinded in
favor of the oak room (constituting an unpropitious omen).
.in -5n
.ad b

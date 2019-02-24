.so ../ADM/mac
.XX lp 603 "A Guide to the Lp Printer Spooler"
.nr dP 1
.nr dV 1.5p
.ND
.TL
A Guide to the Lp
Printer Spooler
.AU
P. Glick
.AI
AT&T Bell Laboratories
Murray Hill, New Jersey 07974
.AB
.PP
.I Lp
is a collection of programs used to provide an easy-to-use
interface for printing a variety of document types on a variety
of printers.
.I Lp
is meant to be the glue that connects various document language
translators and printer communication programs together so that
the users may have a consistent view of printers.
Most of the glue
is shell script, which can be easily modified.
The user need not
specify options to get sensible output in most cases.
.I Lp
is described here
so that others may make additions and changes.
Only the shell scripts, or glue, are described here.
.AE
.2C
.NH
Introduction
.PP
.I Lp
is used to format and print data on a variety of output devices.
The need for
.I lp
was rooted in the inability of other printer spoolers to do simple
tasks without a great deal of user specification of options.
At the time
.I lp
was written, there were several printer
languages, such as ImPress and P\s-2OST\s+2S\s-2CRIPT\s+2 and
an internally developed printer that would accept
.I troff
output.
A great deal of what underlies
.I lp
is taken from other systems.
The important features of this system are that most of the programs
are easily modified shell scripts and the user need not
fuss with the large amount of underlying software developed by others.
.I Lp
runs under several flavors of the
.UX
Operating System
and uses both Datakit and Ethernet to transport data between machines.
.PP
Text, graphics, and formatted text files are appropriately processed and
placed into a spool directory from which they are taken to be printed.
Additional functions include checking the status of a printer queue
and removing jobs from the printer queue.
.PP
All files associated with
.I lp
reside in the spool directory
(usually \f(CW/usr/spool/lp\fP),
except for the
.I lp
command itself which usually resides in \f(CW/usr/bin\fP.
The spool directory is defined within the
.I lp
command by the shell variable \f(CWLPSPOOL\fP.
In the remainder of this document, file names may be specified
with this shell variable as their root.
.NH
Usage
.PP
.I Lp
requires that an output device be specified
before it will process input.
This can be done in any of three ways described here in increasing order
of precedence.
.SP .5
.TA 5
\(em	The file
.CW $LPSPOOL/defdevice
may contain the name of a default output device.
This may not be practical for environments where
there are many printers
and the default device need not be specified.
.SP .5
\(em	The user's environment variable
.CW LPDEST
may be set to the name of the device to be used.
This is often a more practical solution when there are several printers
available.
This overrides a
.CW defdevice
specification.
.SP .5
\(em	The
.CW "-d printer"
option to the
.I lp
command specifies
.I printer
as the device to which output should be directed, overriding the
previous two specifications.
.PP
If
.I printer
is
.CW ? ,
a list of printers and other information in the
.CW devices
file is printed, as shown in Figure 1.
.1C
.KF
.P1 20n
$ lp -d"?"
device   location  host             class
dp       2C-504    nj/astro/coma    post300+reverse
imagen1  st8_fl1   nj/phone/sid     imagen/2.2+reverse
po       2C-579    nj/astro/pipe    dumb
ps2c364  2C-364    nj/phone/sid     post300+reverse
ps71     2D-154    nj/phone/gauss   post300+reverse
ps83     st8_fl3   nj/phone/gauss   post300+reverse
psu      2C-501    nj/astro/coma    post300+reverse
     .
     .
     .
$
.P2
.ce
\fBFigure 1.\fR  Sample listing of installed printers.
.KE
.SP
.2C
.PP
Normal
.I troff
output can be printed with
.P1
$ troff -ms lp.ms | lp -ddp
.P2
.NE 3
LaTex (and analogously Tex) documents are printed in two steps:
.P1
$ latex lp.tex
     .
     .
$ lp -dpsu lp.dvi
     .
     .
$
.P2
LaTeX
produces a `.dvi' file and
does not permit the use of a pipe
connection to the standard input of
.I lp .
In both these cases I have explicitly specified a destination
because my
.CW LPDEST
variable is set to a printer near my office, which I use infrequently.
Although this doesn't make a great deal of sense, it is useful for
the purpose of these examples.
.PP
To look at the status and queue of a device:
.P1 0
$ lp -dpsu -q
status:
%%[ status: idle ]%%
sending file psu/29436.1
%%[ status: busy; source: serial 25 ]%%

queue on nj/astro/coma:
job		user	try	size
gauss29436.1	pg	0	17454
coma17565.1	ches	1	49995
$
.P2
This command prints the status and queue on the local
and remote hosts.
The format of the status and queue printout is up to the administrator.
.PP
The job started above can be killed as follows
(where the output is too long for the column, it has been folded at the \*(cr):
.P1 0
$ lp -dpsu -k gauss29436.1
gauss29436.1 removed from psu queue\*(cr
on nj/astro/coma
.P2
If you wish to kill a job on the queue but the job has begun to print,
you then have to turn the printer off and on to kill the currently
printing job.
This may be fixed someday.
.NH
Options
.PP
There are options available to modify the way in which a job is handled.
It is the job of the
.I lp
programs to convert the option settings so they can be used by each of the
different translation and interface programs.
Not all options are applicable to all printer environments.
Table 1 lists the standard
.I lp
options, the shell variable settings and description of the options.
.1C
.KF
.SP
.TS
center;
c | c s s | c
c | c c c | c
lfCWw(.7i)| lfCWw(.6i) cfCWw(.4i) cfCWw(.6i)| lw(2.8i).
=
option	shell variable	action
\^	name	default	set	\^
_
-D	DEBUG	N	-D	turn on debugging mode.
-H	NOHEADER	N	1	suppress header page.
-L	LAND	N	1	make long page dimension horizontal.
-M \fImach\fP	LPMACHID	N	\fImach\fP	set the source machine name.
-Q	QONLY	N	1	do not execute daemon; for debugging purposes.
-c \fIn\fP	COPIES	N	\fIn\fP	number of copies to be printed.
-d \fIprinter\fP	LPDEST	U	\fIprinter\fP	set job destination; override other settings.
-f \fIfont.pt\fP	FONT	N	\fIfont\fP	set font style and point size for printing.
	POINT	N	\fIpt\fP
-i \fIn\fP	IBIN	N	\fIn\fP	T{
select alternate paper tray.
The argument given is dependent on the printer type,
although some effort is made in the process files to make `2'
an acceptable argument.
T}
-k	KILLFLAG	0	1	T{
take non-option arguments as job numbers to be removed from queue.
T}
-l \fIn\fP	LINES	N	\fIn\fP	T{
for printed data, the number of lines per logical page.
T}
-m \fIf\fP	MAG	N	\fIf\fP	T{
magnify the image by a factor \fIf\fP.
The factor should be a positive real number.
T}
-n \fIn\fP	NPAG	N	\fIn\fP	T{
put \fIn\fP logical pages on a single physical page.
A simple algorithm is used to pack the pages.
T}
-o \fIlist\fP	OLIST	N	\fIlist\fP	T{
print only those pages specified in the list.
The list may be a sequence of numbers or ranges separated by commas.
A range is a pair of numbers separated by a hyphen.
T}
-p \fIproc\fP	LPPROC	L	\fIproc\fP	T{
use the preprocessor \fIproc\fP instead of the preprocessor given
in the
.CW devices
file for this printer.
T}
-q	LPQ	N	1	T{
print the status and queue.
T}
-r	REVERSE	L	-	T{
this toggles the
.CW REVERSE
flag changing whether or not page reversal should occur in preprocessing.
Page reversal is needed if a printer delivers pages face up.
The keyword
.CW reverse
can be placed in the
.I lpclass
field of the
.CW devices
file.
If a document has already been processed this flag has no effect.
T}
-u \fIuser\fP	LPUSERID	U	\fIuser\fP	T{
change the user id that appears on the cover page to \fIuser\fP.
T}
-x \fIoffset\fP	XOFF	N	\fIoffset\fP	T{
move the image \fIoffset\fP inches to the right.
A negative \fIoffset\fP will move the image to the left.
The \fIoffset\fP may be any reasonable real number.
T}
-y \fIoffset\fP	YOFF	N	\fIoffset\fP	T{
same as for \f(CW-x\fP except a positive offset will move the image down.
T}
_
.T&
l l cp-2 lp-2 s
l l cfCWp-2 lp-2 s.
.vs -2p
		default	setting definition
		N	set to the null string (``'') initially in \fIlp\fP.
		L	set from printer entry in \f(CWdevices\fP file.
		U	set from the users environment.
.vs +2p
.TE
.SP
.ce
\fBTable 1. \fILp\fR Option List
.SP 4
.KE
.2C	
.NH
.CW devices
file
.PP
The
.CW devices
file is found in the spool directory.
Each line in the file is composed of 12 fields which describe the attributes
of the printer and how it should be serviced.
The following list gives the shell variable set, and a description of
each of the attributes.
.IP "\f(CWLPDEST\fP "
is the name of the device as given to
.I lp
with the
.CW -d
option
or as specified by the users shell environment variable
.CW LPDEST
or as specified by
the file
.CW $LPSPOOL/defdevice .
This name is used in creating directories and log files that are associated with
the printers operation.
.IP "\f(CWLOC\fP "
is only used to describe where the printer is located.
.IP "\f(CWDEST_HOST\fP "
is the host from which the files are printed.
Files may be spooled on other machines before being transferred to the
destination host.
.IP "\f(CWOUT_DEV\fP "
is the physical device name or network address needed by the printer daemon
to connect to the printer.*
.FS
* This field depends on the requirements of the daemon and may contain a `\(en'
if not required.
.FE
.IP "\f(CWSPEED\fP "
is the baud rate setting for the port.*
.IP "\f(CWLPCLASS\fP "
is used to distinguish minor printer differences.
For example, the keyword `reversal' is used by some of the preprocessors
to reverse the order the pages are printed to accommodate different output
trays (either face up or face down).
.IP "\f(CWLPPROC\fP "
specifies the default preprocessor to be used to convert input to a format
which will be accepted by the device.
The preprocessor is invoked by the spooler.
.IP "\f(CWSPOOLER\fP "
is the process used to invoke the preprocessor and its output in the local
spool directory.
.IP "\f(CWSTAT\fP "
prints the status of the device (whatever that means!) and the list of jobs
waiting on the queue for this device.
The status information depends on what is available from the printer
and interface software.
The queue information has been changed several times to show information
useful in tracking down problems.
The scheduler is used to show the jobs in the order
in which they will be printed.
.IP "\f(CWKILL\fP "
removes jobs from the queue.
The jobs to be removed are given as arguments to the
.I lp
command.
When possible, it should also abort the currently running job
if it has been killed.
.IP "\f(CWDAEMON\fP "
is a process that is meant to run asynchronously to remove
jobs from the queue.
Jobs may either be passed on to another host or sent to the
printing device.
.I Lp
always tries to start a daemon process when one is specified.
.IP "\f(CWSCHED\fP "
is used to present the job names to the daemon and stat programs
in some order, e.g. first-in-first-out, smallest first.
.NH
Support programs
.PP
The following sections describe the basic function of the programs
that are found in the subdirectories of
.CW LPSPOOL .
The programs in a specific directory vary the
type of output device or networks that have to be dealt with or some
special set of user requirements.
.NH 2
.CW process
Files
.PP
The
.CW generic
preprocessor
is the default preprocessor for most printers.
It uses the
.I file (1)
command to determine the format of the input file.
The appropriate preprocessor is then selected to transform the
file to a format suitable for the printer.
.PP
Table 2 shows the names of programs that convert input formats (first column),
to output formats (top row).
.1C
.KF
.TS
center box;
c || c | c | c
c || cI | cI | cI.
	P\s-2OST\s+2S\s-2CRIPT\s+2	Impress	T{
Canon
.br
(obsolete)
T}
=
text, listings	ppost	daisimag	can
_
troff output	dpost	dimp	dcan
_
Latex output (.dvi files)	dvipost	dviimp	\(em
_
bitmaps [see \fIbitfile\fR(9.5)]	bpost	bimp	bcan
_
Tektronix graphics	tpost	tekimag	tcan
.TE
.ce
\fBTable 2.\fP
.SP 2
.KE
.2C
.PP
There are also several interface routines here used to send
files to, e.g. the color P\s-2OST\s+2S\s-2CRIPT\s+2 printer
and the
.SM LINOTRONICS
P\s-2OST\s+2S\s-2CRIPT\s+2 phototypesetter.
.NH 2
.CW spool
Files
.PP
The generic spooler is responsible for executing the preprocessor
and directing its output to a file in the printers queue.
An additional file is created containing information such as the system name,
user id, job number, and number of times this job was attempted.
The format of the id file has been changed from time to time
to assist with debugging.
.PP
Certain printer handling programs do not require separate preprocessing
and spooling.
For such circumstances a
.CW nospool
spooler is available which simply executes the preprocessing program.
The processing and spooling functions are assumed by this program.
.NH 2
.CW stat
Files
.PP
The function of these files is to present status information about the
printer and its queue.
Because
.I lp
often works over a network connection, the stat function is designed
to return information about the local queue as well as the remote queue.
The scheduler is used to print the queue in the order in which the jobs
will be executed.
The various commands in the
.CW stat
directory differ only in the means in which they access different networks.
.NH 2
.CW kill
Files
.PP
These programs receive command line arguments passed to them by
.I lp
and remove the job and id files which match the arguments
from the particular queue.
.NH 2
.CW daemon
Files
.PP
The
daemon is the last program invoked by
.I lp
if the \f(CW-Q\fP option has not been given.
The daemon process is executed asynchronously
with its standard output and standard error appended to
the printer log file.
The log file is described in a subsequent section.
Because the daemon runs asynchronously, it must
catch signals that could cause it to terminate abnormally.
The daemon first checks to see that it is the only one running
by using the
.CW LOCK
program found in the daemon directory.
The daemon then executes the scheduler to obtain the name of the
next job on the queue.
.PP
The processing of jobs may entail transfer to another host
or transmission to a printer.
The details of this are specific to the individual daemons.
If a job is processed without error, it is removed from the queue.
If a job does not succeed, the associated files may be
moved to a printer specific directory in
\f(CW$LPSPOOL/prob\fP.
In either case, the daemon can make an entry in the printer's
log file.
Before exiting, the daemon should clean up lock files by calling
.SM \fIUNLOCK\fP .
.PP
The
.SM \fILOCK\fP
command creates a
.CW LOCK
file in the printer's queue and makes a link to a file called
.CW LINK .
The locking mechanism could be improved but there have been very
few failures.
.PP
Several non-standard daemon programs have been designed
to suite various requirements and whims.
One such program announces job completion and empty paper trays
by causing icons to appear in peoples'
.I vismon (9.1)
window.
Another, using a voice synthesizer, makes verbal announcements.
Other daemons may be designed to taste.
.NH 2
.CW sched
Files
.PP
The scheduler must decide which job files should be executed and
in what order.
The most commonly used scheduler algorithm is FIFO, which looks like this:
.P1 0
ls -tr $* | sed -n -e 's/.*  *//' \e
  -e '/^[0-9][0-9]*\.[1-9][0-9]*$/p'
.P2
This lists all the job files in this printer's queue in modification
time order.
Jobs entering the queue have a dot (.) prepended to their name
to keep the scheduler from selecting them before they are complete.
.NH
When Thing Go Wrong
.NH 2
.CW log
Files
.PP
The log files for a particular
.I printer
appear in a subdirectory of the spool directory
.CW log/printer .
There are currently two types of log files that appear
here.
One is for the daemon to log errors and successful completions
of jobs.
These are named
.I printer.day
where day is the three letter abbreviation for the day of the week.
These are overwritten once a week to avoid the need for regular
cleanup.
The other type of log file contains the status of the printer and
is written by the program that communicates with the printer itself.
These are named
.I printer.st .
These are overwritten with each new job and are saved in the
.I prob
directory along with the job under circumstances described below.
When a printer does not appear to be functioning these files are the
place to look first.
.NH 2
.I prob
Files
.PP
When a job fails to produce output,
the log files should be checked for any obvious problems.
If none can be found, a directory with full read and write permissions
should be created with the name of the printer in the
.CW $LPSPOOL/prob
directory.
Subsequent failure of that job will cause the daemon to leave a
copy of the job and the printer communication log in the
printers
.I prob
directory.
It is not uncommon for a printer to enter states from which
it cannot be rescued except by manually resetting it.
.NH 2
Repairing Stuck Daemons
.PP
There are two failure conditions that occur which are not handled
by the daemons.
.PP
One problem can only be described as the printer entering a
comatose state.
The printer does not respond to any messages sent to it.
The daemon should recover from the reset and an error message
will appear in the log files.
.PP
The second problem is more insidious.
When communicating with a printer directly connected to Datakit,
or when transferring files between a remote and local host over
Datakit,
the Datakit channel can become
frozen and the I/O process hangs.
The I/O process must be killed and another must be allowed to start
up.
The ultimate cure for this problem lies in another domain.
.......
.NH
Interprocessor Communication
.PP
When a Tenth Edition
.UX
System is first being set up as a printer host,
two files need to be edited.
(Sample lines are shown in Figure 2.)
.CW /usr/ipc/lib/serv.local
needs to define the
.I lp
service.
Authorization lines need to be added to
.CW /usr/ipc/lib/auth.local ,
permitting certain machines
(in this case any machine on the
.CW alpha
or
.CW beta
Datakit nodes) to use the
.I lp
service as the user
.CW daemon
on this machine.
The function of all these lines is explained in more detail in
|reference(latest ipc presotto).
.1C
.KF
.P1 10n
/usr/ipc/lib/serv.local:
.sp .3
#service        actions
lp              v9auth+args+cmd(/usr/bin/lp)

/usr/ipc/lib/auth.local:
.sp .3
#service 	remote-hosts          	rem-uid  local-uid
lp       	dk!nj/(alpha|beta)/.* 	.*       daemon
.P2
.SP
.ce
\fBFigure 2.\fR  IPC configuration lines
.KE
.2C
.PP
The use of
.I lp
over Ethernet
is either more complicated or more restricted in the way
.I lp
can be used.
Because
.I lp
does not run with special setuid permissions, users must have
permission to execute the command remotely.
Otherwise, a special program
would have to be written to handle the I/O setup for the
.I lp
program.
.NH
Acknowledgements
.PP
Special thanks to Rich Drechsler for supplying and maintaining all
the P\s-2OST\s+2S\s-2CRIPT\s+2 translation and interface programs,
without which
.I lp
would be an empty shell.
Howard Trickey provided the
T\s-2E\s+2X
to P\s-2OST\s+2S\s-2CRIPT\s+2
translation program.
.NH
References
.LP
|reference_placement

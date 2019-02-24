.so ../ADM/mac
.XX backup 593 "The File Motel: An Owner's Manual"
.nr dP 2
.nr dV 3p
.TL
The File Motel:
.br
An Owner's Manual
.AU
Andrew G. Hume
.AI
.MH
.AB
.PP
The File Motel is an incremental user-level file backup system for
.UX
systems.
The first version of the File Motel has been in successful operation
for over two years with three sites supporting about 50 systems.
The first version supported only Ninth Edition
.UX
systems, although with only modest inconvenience
files could be saved from Sun 3 clients.
The second version of the File Motel is a complete reworking
of the original system, emphasizing easy portability to most
.UX
systems.
The files are stored in a machine-independent form;
as an example, I have recovered a directory onto a Sun 3
from a server on a MIPS 120/5 that had been originally
saved from a Cray X/MP\-24.
The user and administrative interfaces have been streamlined,
based on experience in the field.
.PP
The system has been restructured to look like a kit.
Most of the modules, such as the database, networking and media code,
have been isolated via simple interface routines.
As with a kit, you may not find exactly what you want,
but it should be easy to roll your own.
.AE
.2C
.NH
An Overview
.PP
This is a manual for the File Motel |reference(file motel usenix),
a backup system for
.UX
systems.
The File Motel consists of a central server system
servicing many client systems.
The server system is almost always also a client system.
The File Motel saves only the files that change on any given client system,
using a database to record what versions
have been saved for any particular file.
Under normal usage patterns, this is on the order of 1\-5% of the user files
on the client.
This makes backup practical over slow networks to slow backup media.
.PP
The daily routine in the File Motel starts around midnight when
the clients send copies of any new or recently modified files to the server machine.
After receiving the files from all the clients,
a separate processing step transforms the received files into
backup copies, which are then written to the backup media
of your choice (typically, WORM disks).
Backup and recovery can be performed by anyone with the appropriate permissions;
in general, there is no administrative overhead other than
fussing with backup media.
.PP
This description may be a little clearer with some details.
The following description includes
some sample numbers from our Center's File Motel;
other sites will differ.
The first step is a client sending files to the server.
A shell script configured for the client generates a list of (say 5000)
candidates for backup (say, all the files changed in the last week).
This list is sent to the server which returns a list of the files (say 900)
that really need to be backed up.
Each of these files is then transmitted to the server together with a header
(which includes a checksum).
(On average, about 15MB is sent taking about 20 minutes.)
There is an acknowledgement from the server after every file;
this allows graceful termination when the server has problems,
such as running out of space.
The redundancy in the candidate list allows non-critical clients
to cope with transient faults (such as a broken network) without
administrative intervention by ignoring the fault
and getting the files the following night.
The client process is normally initiated either by the server machine
or by
.I cron (8).
Exactly the same mechanism is used for user-initiated backups;
the only difference is that the system backup is executed by the super-user
(and thus has access to all the files on the client system).
.PP
The files sent by the clients are kept in receiving areas each of
32 subdirectories, each processed in turn.
First, the program
.I sweep
deletes any unnecessary files, assigns each backup copy a name
(which is stored in the file's header) and recalculates the file's checksum.
The remaining files are fed to
.I dbupdate
which deletes any unnecessary files and stores the version information
in the database.
Finally, the surviving files are moved to a staging area for writing to the
backup media.
.PP
The last step is writing the backup copies to the backup media.
The only medium currently supported is a WORM disk.
In our environment they are preferred because of their large capacity
and because you can get reliable jukeboxes (automatic disk changers).
Optical jukeboxes come in all sorts of sizes; our center has a
SONY WDA 3000-10 with a total capacity of 164GB (328GB after October 1989).
.PP
There are many other programs in the File Motel,
some intended for the user (for example, recovering files),
and others for the administrator (usage statistics, backing up the database).
.PP
The rest of this manual is intended for the caretaker of the File Motel.
Section 2 details some of the
peculiar aspects of the File Motel that have caused problems in the past;
if you can survive these,
then installing and running the File Motel
ought to be easy.
If there are incompatibilities, then installing the File Motel
will require (perhaps substantial) work.
The File Motel uses many small single-purpose tools;
if you need to figure out what is going on (or wrong, as the case may be),
these tools are described in Section 3.
Sections 4 and 5 are step by step instructions for installing
a client and a server respectively.
Finally, Section 6 elaborates on media management.
.NH
Some Things You Should Know
.PP
.de BL
.IP \ \ \ \ \s+3\(bu\s-3
..
This section describes some of the assumptions underlying the
construction of the File Motel software.
Most of these assumptions have caused problems in porting to
systems less hospitable than 10th Edition
.UX
(or V10 for short).
.BL
Each server has one global name space for all the files saved from all the clients.
The file
.I z
from machine
.I mach
is stored under the name
.I /n/mach/z .
It so happens that this is how V10 networked file systems are normally mounted
and in fact, all file references actually go through this network name.
For other systems, you should define the
.CW -DNO_NETNAME
switch as described in section 5.
.BL
All client\-service communication uses a uniform networking interface.
That is, a system invokes a service on the remote machine and gets a
pair of file descriptors attached to the input and output of that service.
Both Berkeley-style sockets and V10-style IPC are supported.
For the case of a single system that is both a server and client
and has no networking, you will have to write an execution service that
constructs pipes to the desired services.
Note that it is possible to provide this interface even if all
the networking  you have is a user program (such as
.I rx
or
.I rsh )
that executes a program on another machine.
.BL
It must be possible to nominate the user that the remote service runs as.
Most run as a regular user, say
.I fmdaemon ,
but some must run as superuser and on V10 systems, one must run as
.I bin .
.BL
The code is reasonably portable; with the canned configuration files
it runs on a Cray X-MP/24 (UNICOS),
VAX 11/750, Microvax II, 8550, 8600 and 11/780 (V10, Ultrix, 4.3BSD),
Sun 3 (SunOs 4.0),
MIPS M120/5, M2000 (UMIPS 3.0, 3.10, RISC/os 4.0).
Some of the code, notably Ken Thompson's new version of
.CW doprint ,
makes assumptions about variable argument lists.
So far, the code has continued to work on all the systems we have tried
(although we can't optimise on the MIPS)
but in this world of perverse hardware and compilers, you may not be so lucky.
.BL
The code assumes no particular byte-ordering but does assume that there
is an integer type of at least 32 bits.
By and large, the programs allocate all data areas dynamically;
whenever there is choice, programs trade space for smaller runtime,
so there must be at least 24 bits of data space.
If you have a 32 bit machine but have 16 bit ints,
you will have trouble (perhaps
.I lint
will help).
.BL
It is assumed that the backup medium can hold at least one backup copy
and practically, it should hold at least one volume.
This is about 20MB by default; if you have smaller backup media
and cannot arrange better, change the volume size \(em it is
a constant defined in
.CW fm/sweep.c .
.BL
The File Motel depends on each file having a unique name.
This continue to cause problems, particularly in the presence of symbolic links.
For example, on a system I use
.CW /usr/andrew
is a symbolic link
.CW /usr2/guest/andrew .
The right thing to do is to save files under
.CW /usr/andrew
(so that you can move them from file system to file system and keep their name).
Yet, the user may not be aware of this name; if they do a
.I pwd
to find out, they will get the wrong answer.
.NH
A Detailed Description
.PP
The action in the File Motel can be functionally divided into four areas:
client selecting and sending files to the server,
the server processing the client files onto backup media,
client recovering files back from the server,
and an assortment of administrative functions.
.PP
Programs and scripts used by the File Motel live in three places:
.CW /usr/bin/backup
is the user interface,
.CW /usr/lib/filemotel
holds all the programs and scripts used by clients,
and
.CW /usr/filemotel/bin
holds the server-specific programs.
These are the conventional names \(em they can be reconfigured to taste.
Because of this and their length,
these abbreviations will be used in the following text:
.TS
center;
lFCW lFCW.
$FM	/usr/filemotel
$FB	/usr/filemotel/bin
$FL	/usr/lib/filemotel
.TE
.NH 2
Client Sends Files to the Server
.PP
The controlling script here is
.CW $FL/doclient :
.P1
#!/bin/sh
$FL/sel | $FL/act
.P2
The selection script
.I sel
has to generate a list of absolute filenames.
You can use any tools available to you; the File Motel supplies
the program
.I fcheck
which is rather more efficient than
.I find (1)
and follows symbolic links that are arguments.
This is to help clients save files as
.CW /usr/andrew/...
rather than the less than informative
.CW /usr2/guest/andrew/... .
A small
.I sel
file is shown below.
.KF
.P1 0
/usr/lib/filemotel/fcheck 512 7 /etc /usr/* |
sed -e '/\e.o$/d
/\e/a\e.out$/d
/\e/core$/d
/\e/foo$/d
/^\e/usr\e/tmp\e//d
/^\e/usr\e/spool\e//d'
cat <<EOF
/unix
EOF
.P2
.KE
.PP
The script
.I act
works in a straightforward way.
First, the filenames are transformed into the input format for
.I missing
by the program
.I iprint .
This prepends
.CW /n/\fImachine
to the filename (unless this is already there) and appends the
inode change time and size.
There is a convention that an input filename starting with a
.CW //
is a symbolic link to be followed (that is, use
.I stat (2)
rather than
.I lstat (2)
to get the time and size).
The size is carried around so that if you choose a file because it is small
and it grows dramatically while you are asking about it, you can reject it
later on (although this is not done now because no one cares yet).
.I Missing
takes these names and ships them to the corresponding server
.I missing_
on the server machine.
(Servers for a service
.CW abc
are called
.CW abc_ ).
.I Missing_
checks the name,time tuples against the database and sends back
the lines that are newer than the entry in the database.
Transmissions in both directions are checksummed; any errors
are reported to standard error and are also logged in the log file
on the server machine.
.PP
The results from
.I missing
are stored in
.CW $FL/files.\fIday\fP .
They are given to
.I fmpush
which actually pushes them to the backup system.
.I Fmpush
also takes a system name argument for logging purposes.
If there are any errors,
.I fmpush
reports the error and the number of files transmitted.
This allows the push to be restarted efficiently:
.P1
$ pwd
$FL
$ fmpush wild < files.Tue
EOF after 2713 files sent.
$ sed 1,2713d files.Tue | fmpush wild
.P2
Any diagnostics are mailed to the user
.I backup
and also kept in
.CW $FL/files.\fIday\fP.sho .
It is not necessary to keep these files around after they have been used
but they are relatively small and often useful;
for example, a client who normally saves 100 or so files suddenly sends
you 10,000 files \(em you can quickly go to that client and check
what the files were.
When possible, diagnostics are also logged on the backup system.
.PP
Only regular files, symbolic links and directories have their contents saved;
all other files (such as devices) just have their
.I stat (2)
buffers saved.
To preserve machine independence, the content of a directory is saved
as a list of null-terminated element names.
This removes the need for the server to be able to guess a
client's directory structure, although it does lose a small
amount of subtle information contained in the freed slots of the
directory.
.........
.NH 2
Server Processes Client's Files
.PP
Received files are processed by the script
.CW $FL/munge .
This processing is decoupled from either receiving or restoring
client files; for example,
it is safe to process files while receiving them.
Munging is typically started by
.I cron ,
but you can also cause
.I rcv
to invoke
.I munge
automatically,
and you can invoke
.I munge
manually by executing
.CW $FL/callmunge .
.PP
Regardless of how it is called,
.I munge
scans the 32 receiving subdirectories in each of the receiving areas in
.CW $FM/adm/rcvdirs
looking for files to process.
If it finds any, it calls a program
whose name is supplied as
.CW $PROCPERM
to copy the final copies
to the media of your choice.
It repeats this scan until it found nothing to do during the last scan.
.PP
The action within a subdirectory is simple.
.CW $FB/sweep
looks for files with mode
.CW 0 ,
.CW 0400 ,
or
.CW 0600 .
Mode
.CW 0
files are files that are being received
(\fIrcv\fP
marks a file as done by changing its mode to
.CW 0600 )
and are ignored unless it is hasn't been modified within the
last 12 hours.
In this latter case, it is regarded as stale (almost always a network
connection was dropped) and unlinked.
Mode
.CW 0400
files have been already processed by
.I sweep
but for some reason (most often, running out of space)
weren't copied to the backup area.
Mode
.CW 0600
files are assigned a backup copy name and after recalculating the
checksum are changed to mode
.CW 0400 .
.I Sweep
emits the names of all the files ready to be copied to the backup area
and this is saved in a file.
.I Munge
then makes any needed directories in the backup area that don't exist.
.CW $FB/fmmv
then moves all the files to be copied to the backup area.
We then update the database with information from the files we just copied.
.PP
Updating the database is a two part process.
Run
.CW $FB/updatef
on the files (use the program
.I updatew
for files on the WORM)
and then feed the output to
.CW $FB/dbupdate .
In this way, we guarantee that the database is purely a function of
the backed up files
(assuming none get lost between the backup area and the backup media).
The input to
.I dbupdate
is (roughly) a sequence of backup file headers and contents of
backed up directories.
.I Dbupdate
updates the various databases (described below) and sometimes tries to unlink
the backup copies.
(This happens when two copies of the same file are
in the same receiving subdirectory.
.I Sweep
happily copies both to the backup area but when
.I dbupdate
goes to update the main database for the second copy, it discovers
it already has this copy and so unlinks the second copy.
It doesn't care if the unlink fails because this is just an attempt
to be space efficient and in any case, the unlink can fail only if the file
has already been committed to the backup media.)
.I Dbupdate
also appends accounting statistics records for each file, containing the time
the file was saved, the size, the owner and the system name, to the file
.CW $FM/stat.log .
.PP
After
.I munge
is finished scanning the receive areas,
it processes the statistics records generated by
.I dbupdate
by
calling
.CW $FB/procstats .
This reads (and then truncates)
.CW $FM/stat.log
and adds new records to the files
.CW $FM/stat/\fIsystem\f(CW .
These records are in machine independent format and have been collapsed to
refer to all the files per user/day combination.
Even in this compressed format,
the statistics records would grow without bound.
Accordingly,
.I munge
calls
.CW "procstats -c"
to further collapse together all the records older than 30 days for each user.
(The number 30 comes from the only program that looks at these statistics,
.CW "backup stats" .)
.PP
Throughout its work,
.I munge
checks to see if it should exit by checking the existence of a guard file;
this is created by
.CW $FB/stopmunge .
.NH 2
The Databases on the Server
.PP
There are three databases kept on the server, all conventionally kept in
.CW $FM/db .
The first,
.CW filemap ,
is the main and only required database;
it contains the mappings from filename to last modify date and
from (filename, modify date) tuple to backup copy name.
The second,
.CW dir ,
is optional and maps (directory, modify date) tuples to their contents.
It is used to make recovery of file trees go (much) faster.
The third,
.CW fs ,
is optional and maps (filename, modify date) tuples to their
.I stat
buffers.
It is used to implement the backup file system.
.PP
The default implementation of these databases is Peter Weinberger's
compressed B-trees (see
.I cbt (1)).
(The compression refers to eliding common prefixes
of successive keys; it does very well on the pathnames used by the File Motel.)
The
.I cbt
database
.I db
consists of two files,
.I db\f(CW.T\fR
(the tree part) and
.I db\f(CW.F\fR
(the data part).
As the
.I cbt
routines do not reclaim space, the
.CW .T
file can start growing at a very fast rate when the tree is large
(say four levels).
This has proved to be a real nuisance so there is considerable support
for periodic squashing of the database
(which reclaims space by rebuilding the database) and
for supporting the
.I filemap
database as a collection of separate databases.
.PP
The latter is intended to be used in the following way.
The file
.CW $FM/db/filemap
is always the current
.I filemap
database.
If the file
.CW $FM/db/filemaplist
exists, it is taken as a list of database names,
one per line in oldest to newest order, to be used in addition to
.CW $FM/db/filemap .
These are searched only, never updated.
At our site, we produce one of these databases for about every 15\-16GB
of backup files.
.NH 2
Server Sends Files to the Client
.PP
All requests for files go through a central server
.CW $FB/fetch_ .
This program simply farms out work to other programs.
.I Fetchf
attempts to find files that are still under
.CW $FM/v .
Systems with plenty of mass storage can leave the backup copies
online and things will go quite fast.
For the files that
.I fetchf
can't find,
.I fetch_
looks in the configuration file
(\f(CW$FL/conf\fP)
to determine the backup media (say
.CW j
for jukebox).
It then calls
.CW $FB/fetchj
with the appropriate filenames.
If you just have a WORM drive, you should use
.CW $FB/fetchw
instead.
These two programs purport to be generic drivers for jukeboxes
and single drives; if you have different media (say an Exabyte tape),
you should be able simply to load the drivers with your media library
to generate the appropriate fetch program.
More details are given below in the section on media management.
.PP
Users can generally access any files they have read permission for,
regardless of what system they are on or the system from which the files were
stored from.
In addition, we trust our users (or more importantly, our network)
and so we do no checking of a user's right to retrieve files.
Such checking, such as a password, can easily be added to the startup
protocol between the program the user calls (\f(CW$FL/fetch\fP)
and the server (\f(CW$FB/fetch_\fP).
.NH 2
Administrivia
.PP
This section is a bunch of administrative odds and ends for the way we organise
the File Motel in our Center.
Your details may be different, and indeed ours change over time,
but the examples are probably helpful.
.NH 3
File Layout
.PP
We store the File Motel under the directory
.CW /usr/backup ,
which is a file system large enough to hold comfortably the current
databases and a squashed version (more on this later).
The receiving area is another smallish file system (about 120MB) mounted on
.CW /usr/backup/rcv
and the holding area
.CW /usr/backup/v
is another file system of the same size.
This is done to isolate the effects of client excesses;
the sending processes all know how to deal with running out of space
(we practice often).
I regard running out of space once a month as tolerable;
once a week is too much.
To aid searches for files, we keep a file
.CW /usr/backup/filenames ), (
which is a sorted list of all filenames.
This is maintained by the database squasher.
.PP
The main drawback to Weinberger's B-tree software is that it does
not reclaim space in the tree.
Thus, over time the tree file gets huge
(the rate grows as the depth of the tree).
The fix is to periodically squash the tree.
We combine this with dumping the database to WORM disk in the script
.CW $FB/backupdb .
.NH 3
Talking to the Clients
.PP
We have found it best to call the clients rather than have them call us.
The load seems more balanced and things get done sooner.
We use
.I mk
as it handles parallel processing; a typical mkfile is
.P1 0
CLIENTS=Cwild C3k Ctcp!tempel
NPROC=3

clients:VQ:	$CLIENTS
	PROCPERM=$FB/toworm $FB/munge

C%:VQ:
	set +e; $FB/callclient $stem; exit 0
.P2
Understanding this completely requires familiarity with
.I mk
but the intent is clear.
We first get the files from the clients by the
.CW C%
rule and then process them by
.CW munge
and then put them out on WORM disk by
.CW toworm .
As we use Datakit, most clients are called using Datakit but some
(like
.CW tempel
in the mkfile) are called using TCP/IP.
This convenient piece of magic works on V9 because of Dave Presotto's
clever design of the IPC system; you may have to work harder.
The
.CW set
stuff in the
.CW C%
rule means to keep on processing even if a client gets an error.
The entry in
.CW /etc/crontab
is more or less
(this is one physical line folded at the \*(cr because of the column width)
.P1 0
eval "cd /usr/backup/adm; mk clients 2>&1" |\*(cr
mail backup
.P2
We use the
.CW backup
mailbox to redirect mail to someone appropriate.
.NH 2
Disasters
.PP
Currently, the only disaster we have had that was not the result of a kernel bug
is running out of space;
this is either inconvenient or quite bad.
Running out of space in the receiving or safe areas is just inconvenient.
By default, the client's
.I fmpush
stops when the receiving area runs out of space after saying how many files got
transmitted.
This is enough information to resend the rest when convenient.
Alternatively, you can change
.CW $FL/act
to give
.I fmpush
the
.CW -r
flag; this means that it will retry sending files every hour or so
until it succeeds.
Running out of space in the holding area is also not too bad;
eventually
.I munge
will put the holding area onto the backup media and then cycle through
the receiving area again.
.PP
The worst effect of running out of space is ruining your database.
(This happens rarely for us as
we keep our databases on a file system apart from the receiving/holding areas.)
Rebuilding the database is not too hard.
First, find out the next backup name to be assigned
(by a
.CW "sweep -n"
or by examining the backup media and holding areas).
Then, get the most recent backup copy of your database and install it.
Set the next backup name you found in the first step with
.CW "sweep -s" .
You then need to extract the database information for each file added
to the database since the backup copy of the database was made.
The starting file name is stored in the
.CW .N
file by
.I backupdb .
The program
.I updatew
will extract this from files on a WORM, and
.I updatef
from regular disk files.
The result is fed to
.I dbupdate
as done in
.I munge .
.NH
Installing the File Motel on a Client System
.PP
The following instructions assume you have the source
.CW fm.cpio
somewhere, say
.CW /tmp/fm.cpio .
Note also that these instructions will change over time;
you must follow the online copy of this document included with the source.
.IP [1]
You will need version 3 of
.I mk
(or any version
dated later than Mar 11, 1989).
.IP [2]
Select the root directory for the source, set the
environment variable
.CW FMSRC
to its name
and export
.CW FMSRC .
For example,
.P1
FMSRC=/usr/filemotel/src
export FMSRC
.P2
.IP [3]
Install the source tree by
.P1
cd $FMSRC/..; cpio -iudc < /tmp/fm.cpio
.P2
.IP [4]
Create a
.CW CONF
file.
This describes your installation environment
and is included in lower-level mkfiles.
The various switches are described in detail below;
however, the easiest way is to start with one of the
sample configuration files in the directory
.CW $FMSRC/conf .
.IP [5]
If necessary, create the repository for client files:
.P1
mkdir /usr/lib/filemotel
.P2
(this is configurable, see
.CW FMLIB
below)
and if you have not defined
.CW NO_NETNAME
in
.CW CONF ,
ensure that
.CW /n/clientname
is a link to
.CW / .
.IP [6]
Initialise the source tree for compiling by
.P1
mk depend
.P2
This only needs to be done once.
If you have to repeat, you can undo this by
.P1
mk undepend
.P2
.IP [7]
Compile and install the client software
by
.P1
mk client
.P2
This can be repeated as often as you like.
Only files in
.CW $FMLIB
and the file
.CW $FMBIN/backup
(these are configurable, see
.CW FMBIN
below)
are affected.
.IP [8]
Setup up the dialstring of the server system by
.P1
echo server-machine-name > $FMLIB/conf
.P2
The name should match the type of IPC you selected in
.CW CONF .
For example, 
.TS
center;
c c
l lFCW.
IPC	Example
Datakit	nj/astro/wild
Datakit	wild
IP	wild.astro.nj.att.com
.TE
.IP [9]
In theory, you are now operational.
A couple of small tests are described in the file
.CW SANITY .
Some common problems and their cures are described below.
.IP [10]
You need to construct the script
.CW $FMLIB/sel
which prints the names of files that you want backed up.
There is a sample script (\f(CWsample.sel\fP) in that directory.
Be careful not to backup networked file systems by mistake.
.IP [11]
If you are initiating backup via
.I cron (8),
add the following command to
.CW crontab :
.P1
eval "/usr/lib/filemotel/sel | \*(cr
/usr/lib/filemotel/act 2>&1" | \*(cr
mail backup
.P2
The exact format varies from system to system;
the File Motel administrator should tell you what time to set it off.
.IP
If your client's backup is initiated from the server system,
you will have to add the line for
.CW fmclient
to your flavour of IPC services file.
If you communicate to the server by TCP/IP
(that is, your
.CW CONF
file has
.CW IPC=socket ),
get the
.I fmclient
line from the file
.CW tcp.inetd
and add it to
.CW /etc/inetd.conf
(some systems use
.CW /usr/etc/inetd.conf )
and add the
.I fmclient
line from the file
.CW tcp.services
and add it to
.CW /etc/services .
You then need to prod
.I inetd
to look at the new files (commonly by sending it a hangup signal).
On some systems, like SunOS, you may need to prod name servers
such as the Yellow Pages as well.
.IP
If you use V10 IPC,
add the corresponding line for
.CW fmclient
from
.CW ipc.V10
to
.CW /usr/ipc/lib/serv.local .
The files
.CW tcp.inetd
and
.CW ipc.V10
are made by
.P1
cd $FMSRC; mk ipc.list
.P2
.NH 2
Some Common Installation Problems
.PP
As a general rule, keep an eye on the log file (on the server)
when setting up the File Motel.
The most convenient way is a window with a
.P1
tail -f $FM/log
.P2
(\f(CW$FM\fP is the root directory of the File Motel.)
.PP
The most common problem is that the basic IPC software doesn't work.
This affects most programs because they involve calling a service on the
backup machine.
That is why the first thing you try to get working is
.I logger
which sends messages to the logger process on the backup machine.
The kinds of bugs I have seen here are typically bugs in the networking code,
particularly TCP/IP.
For example, the
.I logprint
function expects an acknowledgement from the logger server
to indicate that everything went okay.
On at least two of the systems I use, this sometimes doesn't happen because
the closing of the socket by the logger server after sending the ack
seems to speed pass the ack and get to
.I logprint
first.
Naturally,
.I logprint
complains, as might we all.
The best solution is to fix the TCP/IP implementation; failing that,
you might try a judicious sleep between the ack and the close in
the logger server.
This is only one example of a general class of timing problems.
.PP
Another fertile field of failed implementations have to do
with user and system names.
The File Motel tries to check the validity of system and user names
and denies service if there appears to be something sleazy going on.
Regrettably, some otherwise sound TCP/IP implementations resemble sleaze.
For example, a user
.CW mary
on a client may appear on the server as the user
.CW bill .
Or a system may have a system name that is unrelated to the name
the networking code uses.
An attempt is made to cope with these cases, but may fail with
unexpectedly bizarre implementations.
If worst comes to worst, simply turn off all the checking
and hope no one does anything naughty.
(Even if you do this, think hard about allowing remote users
to claim they are
.CW root ;
they will be able to look at all sorts of things.)
Unlike X,
I implement function and policy.
However, all the checking is done in one place
.CW serv_$IPC.c ); (
feel free to do whatever you like,
it's your Motel now.
.de XX
.IP \\f(CW\\$1\\fP
.br
..
.NH 2
Configuration and Compiling Options
.PP
The File Motel software is designed for easy installation in heterogeneous
environments.
The configuration details described below are stored in the file
.CW $FMSRC/CONF .
.CW $FMSRC
contains a number of files
containing settings for various systems; you may want to use
one of these as a starting point.
(Remember that the following information has a small half-life;
the truth should be in the online copy of this manual.)
The most obvious aspect of configuring the File Motel means choosing on
the three directories where files live.
The source directory,
.CW FMSRC ,
has been described above. The other two are
.XX FMLIB=/usr/lib/filemotel
Change this to wherever you want to put the subprograms.
.XX FMBIN=/usr/bin
The directory for the (only) user-called command,
.CW backup .
.LP
Configuring the source to your environment is mostly done with
.I mk
variables and an interface library in
.CW src/sys/\f2system .
The
.I mk
variables are
.XX RANLIB=ranlib
Some systems, mostly BSD-based, whine incessantly unless archive libraries
are processed with some program typically called
.I ranlib .
In this case, set
.CW RANLIB=ranlib ;
otherwise, say if you are on a System V machine, use a harmless program
such as
.CW RANLIB=: .
.XX IPC=socket
Select your favorite type of IPC.
Different clients can use different types and the client's type
need not match the backup system.
(For example, in our Center,
the Cray talks to us via TCP/IP but we talk to it via Datakit.)
The only choices are
.CW socket
and
.CW v10 .
.XX IPCLIB=
Set this if you need a special library in order to use your flavor of IPC.
For example, on V10 systems set
.CW IPCLIB=-lipc .
.XX LIBTYPE=a
This should be set to
.CW a
unless you are on the Cray (which doesn't have archives yet!)
when it should be
.CW o .
.XX COMPAT=
Set
.CW COMPAT=.compat
if you want to be able to process older File Motel files.
(You may have to work hard to get this to work on some systems;
I gave up on the Cray.)
.XX SECTYPE=
Set
.CW SECTYPE=v9
if you are running a McIlroy-Reeds compatible security kernel.
.XX WORMFACE=uda
If you are running the WORM software, you need to say what kind of interface
the WORM is attached to.
The other option
(and the best if you just want to compile without thinking too hard) is
.CW scsi .
The latter may need customizing at your site.
.LP
Currently the system dependent interface library includes the following routines:
.TS
center;
lFCW l.
dirtoents	convert directory to element names
ftw	traverse file tree
nofile	number of fd's available
sysname	system name
username	user's login name
rx_$IPC	call a remote service
serv_$IPC	receive calls
service	service/socket mapping details
dateadjust	do daylight savings/timezone
.TE
.LP
There are a small number of
.CW #define 's
inside
.CW .c
files. 
.XX -DSTRINGH="'<string.h>'"
Define the value to be the string function header file.
.XX -DNO_NETNAME
Define this to disable saving and restoring files through
.CW /n/machine-name
although they will still be stored with that prefix.
.NH
Installing the File Motel on a Server System
.PP
The source comes in both
.I cpio
and
.I tar
formats.
As with the client source installation, note that the following description
is dated and the online copy may be significantly different in detail.
.IP [1]
Follow the client installation process steps 1\-8.
You also need to set the place where the administrative binaries are kept.
I do it this way:
.P1
FMAB=$FM/bin
export FMAB
.P2
.IP [2]
Complete
.CW $FMLIB/conf .
You have to specify the default media type and the root of the administrative
file tree (denoted by
.CW $FM
below).
Details are in
.I backup (5);
my File Motel has this configuration:
.P1
wild
j
/usr/backup
.P2
.IP [3]
Everything that doesn't need to run as
.CW root
should run as an otherwise unused id.
By default, this is
.CW fmdaemon ;
if you don't like this, change the define in
.CW libfm/server.c .
Whatever you choose, set up an account for them;
the File Motel requires nothing but their name/uid
(not even a login directory).
By default, all the shell scripts send mail to the mailbox
.CW backup .
This should be set to an alias for the File Motel caretaker.
.IP [4]
Inform your IPC system of the many services the File Motel offers.
See the notes under step 11 in the client installation above but
install everything, not just
.CW fmclient .
(See step 10 below as well.)
You also need to set up the periodic (normally nightly)
calling of clients and/or
the processing of their files by
.CW $FB/munge .
.I Munge
needs the name of a program to copy the files to your backup media;
set the variable
.CW PROCPERM
to that program's name.
As described previously, you also need to periodically backup the databases
with
.CW backupdb ;
it also needs the name of the program to copy files to your media.
.IP [5]
Initialise the log file:
.P1
> $FM/log; chown bin $FM/log
chmod 644 $FM/log
.P2
.IP [6]
Install the server programs:
.P1
mk server
.P2
.IP [7]
Setup the receiving areas.
List their names in
.CW $FM/adm/rcvdirs
and initialise each are by running
.P1
$FB/rcvdirs
.P2
We use one 120MB file system mounted on
.CW $FM/rcv .
.IP [8]
Allocate the safe area for backup copies.
It must have the name
.CW $FM/v
but may be a symbolic link if there is not enough space in
.CW $FM .
We use an identically sized file system to the receive area mounted on
.CW $FM/v .
.IP [9]
After deciding which databases you want maintained,
initialise the databases with
.P1
src/dbinit.sh
.P2
You may want to start off with all three and remove any you don't want later on
(like when they get to be too big).
.IP [10]
Choose how the receiving process
.I rcv
works.
By default, it simply accepts files.
If it is invoked by the name
.CW mrcv ,
it initiates processing of the received by
.I munge
(or more accurately,
.CW $FL/callmunge )
after the first and last files have been received
(you need both in case any one file took longer to receive than
.I munge 's
cycle time).
The advantage is that you will almost never run out of space, as you will be
processing files at the same time as receiving them.
The disadvantage is that everything will run slower.
I use the default behavior; we rarely run out of space and I like to
investigate why some client is sending much more than normal
before accepting it all.
.IP [11]
Finish the client installation starting with step 10, making sure you
do not backup the receiving areas or
.CW $FM/v .
.IP [12]
Add the command
.P1
$FB/rmlocks
.P2
to
.CW /etc/rc
(or whatever passes for system startup on your system).
This simply removes any lockfiles in
.CW $FM/locks .
.NH
Media Management
.PP
An attempt has been made to provide generic media management programs.
For example, the recovery servers
.I fetchw_
and
.I fetchj_
are instances of a single device server and a jukebox server respectively.
To make this work, a media library is used.
To use a new media, such as Exabyte tapes,
implement the routines in the library, and link the library with
.CW fm/media_.o
or
.CW fm/mmedia_.o .
An informal description of the routines follow.
.LP
.CW "mediainit(char *device, char *vol_id)"
.ti +5n
Initialise the media on the device specified by
.I device .
The latter may a full name or any recognizable abbreviation.
If
.I vol_id
is given, it is checked against the media present.
.LP
.CW "char *mediamount(char *vol_id)"
.ti +5n
Mount the media named
.I vol_id
and return the appropriate device
(suitable for use by
.I mediainit ).
Currently, the values given as
.I vol_id
are those returned by
.I medianame
(below).
.LP
.CW "medianame(char *volume)"
.ti +5n
Return the media name containing
.I volume .
.LP
.CW "mediaopen(char *name, Media *m)"
.ti +5n
Set up
.I m
to point at the backup copy
.I name .
The fields in a
.CW Media
include a file descriptor, preferred read block size, and copy size.
.LP
.CW "void mediafiles(int32 v, int32 n, Media *m, Tb **bp)"
.ti +5n
Return a Media and a list (in
.CW *bp )
of backup copy pointers for all backup copies more recent than
file
.I n
in volume
.I v .
A
.CW Tb
has the creation time and initial (1K) block number for a backup copy.
(It is used by
.I dbupdate ).
The size returned in
.I m
is not actually a size but the number of records in
.CW *bp .
.PP
This is not a complete description;
if you have to write new versions of these routines,
look at the existing implementations (in
.CW $FMSRC/media )
and the programs that use them
(all in
.CW $FMSRC/fm ).
.NH
References
.LP
|reference_placement

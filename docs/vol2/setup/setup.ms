.so ../ADM/mac
.XX setup 497 "Setting Up a Research UNIX System"
.nr dP 2
.nr dV 3p
.TL
Setting Up a Research
.UX
System
.AU
Norman Wilson
.AI
.MH
.SP 2
.2C
.NH
Introduction
.PP
`Research
.UX '
is the
.UX
system used and fiddled with
in the
Bell Laboratories Computing Science Research Center.
It is often named after the most recent version of the manual.
This is the Tenth Edition.
.PP
The system runs only on VAXes,
and only on some CPUs and some peripheral devices.
At the moment these include the VAX-11/750, 11/780, and 8550,
the MicroVAX II and III,
MSCP disks connected to UDA50, KDA50, KDB50, and RQDX3 controllers,
and TU78, TE16, and TU81 tapes.
Various Ethernet and Datakit controllers are supported.
The list is likely to change as time passes.
There will probably never be support for
DEC HSC50 and HSC70 disk servers.
.PP
There is no official distribution tape for the system,
nor are there any plans to produce one.
This reflects our desire to stay out of the software support business;
we have enough of that keeping our own machines running.
We are, however, willing to send snapshots of our current system
to deserving people with the right expectations.
We are interested in sharing the system with colleagues;
we aren't set up to support customers.
.PP
Our system
is not packaged for novices.
It is assumed that anyone who gets a copy
is self-sufficient and can
read our manuals, the hardware manuals, and the source code
to resolve confusions and problems.
Each copy of the distribution
is hand-crafted at a different time;
different kinds of digging around may be necessary
to get any given copy running.
.PP
If you haven't been scared off yet,
here are some suggestions about how to get started.
.NH
Documentation
.PP
The system changes daily, so the printed manual is always out of date.
Check the online manual,
and any supplementary printed pages
that came with the tape,
before taking the printed book as gospel.
.PP
Neither the online manual nor the printed one
is pure;
various local programs and local conventions
have crept in,
especially
in the crucial system administration parts.
Examples should be taken as examples,
not as recipes.
.NH
Cracking the tape and booting
.PP
The tape consists of
at least a root file system image
and a
.I tar
image to be put in
.CW /usr .
There may be other files as well.
Tapes are hand-crafted,
so details do vary;
see the notes that came with the tape.
.PP
The root image is a block-by-block copy
of a root file system,
containing a
kernel binary in
.CW /unix
specific to your hardware.
It is probably exactly five megabytes long.
It should be copied
(you figure out how)
to the beginning of the disk you want to boot from.
One way is to
plug the disk into some already-running machine
with a tape drive,
and
.P1 0
dd bs=blocksize </dev/rawtape >/dev/rawdisk
.P2
.PP
The root image includes a boot block
that behaves as described in
.I reboot (8).
On the VAX-11/750
and machines with similar boot ROMs,
such as the 8200 (if we ever support it),
nothing more is needed.
On other VAXes,
in particular the 11/780
and the 8500-8700 machines,
some boot programs must be installed on the console disk.
A floppy and instructions
should have come with the tape.
.PP
Once the root is installed,
boot the system to single-user state,
.I reboot (8).
The kernel image is in file
.CW /unix .
It is prudent to run
.I fsck
or
.I chuck (8)
on the root file system to be sure it's OK.
It is mandatory to examine
the special files in
.CW /dev
to make sure they're correct for your disk and tape drives;
......
see
.I mknod (8)
and the driver writeups
referred to there.
The special files on the tape bear no necessary correspondence
to the ones you need,
though we try to supply correct ones for the root file system.
.PP
To read in
.CW /usr ,
do something like this:
.P1 0
# /etc/mkbitfs /dev/ra02 31231 4 40
	\fR(make a new 4KB-block file system;
	see below for details)\fP
# /etc/mount /dev/ra02 /usr
# cd /usr
# mt fsf 1  \fR(skip first file on the tape, the root image)\fP
# umask 2
# tar xbf 20 /dev/rmt0  \fR(extract \f(CW/usr\fR files)\fP
.P2
.PP
The
.CW /usr
files occupy about 50 megabytes.
The
.CW umask
is needed because
.I tar
carefully records permissions in the archive,
but at the moment neglects to restore them.
Most of the directories have group (but not general)
write permissions on our system.
.I Tar
does at least set ownership correctly.
.PP
There may be a snapshot of our sources as well,
in one or more additional tape files or on additional tapes.
They won't exactly correspond to the binaries.
The tape won't have absolute pathnames on it,
and for the most part you can rearrange things as you please
(though dependencies do creep in),
but the arrangement on the tape
reflects our conventions.
The source to the operating system belongs in
.CW /usr/src/sys ;
that to the 5620 software in
.CW /usr/jerq/src ;
that to everything else in
.CW /usr/src .
Sources to the network programs
live in
.CW /usr/src/ipc
(even some programs that are installed in
.CW /usr/bin ),
except the network file system,
which is in
.CW /usr/src/netb .
Most programs are built with
.I mk (1),
but a few still use
.I make
instead.
Programs with their own directories have their own
.I mkfile s;
there is one
.I mkfile
in
.CW /usr/src/cmd
for all the small programs there.
.PP
The sources occupy some indeterminate but large amount of space;
expect to need at least 135 megabytes.
.NH
Disks and file systems
.PP
There are two kinds of disk file system,
referred to herein as
`classical'
and
`bitmapped.'
A classical file system
uses 1024-byte blocks
and a V7-style free block list.
A bitmapped file system
uses 4096-byte blocks
and keeps the free block list in a bitmap.
Bitmapped file systems
come in two flavors:
small ones, which keep the free block bitmap in the super-block
(which means they can't be much bigger than 120 megabytes),
and big ones, which keep the bitmap in the highest-numbered blocks
in the file system.
We use bitmapped file systems almost exclusively.
The root image on the tape will be a bitmapped file system.
.PP
The system distinguishes
classical file systems from bitmapped ones
by the minor device number:
bitmapped file systems are on devices
with numbers greater than 64.
A block device with minor number 10
could contain a classical file system;
if it contains a bitmapped file system,
it should have minor number 74 instead.
All the disk drivers ignore the 64 bit.
.PP
Various tools work with the different kinds of file systems:
.IP
.I Fsck
understands classical and small bitmapped file systems.
It doesn't know about big ones yet.
.IP
.I Chuck
understands both kinds of bitmapped file system,
but doesn't know how to rebuild the free list in a classical file system.
.IP
.I Icheck ,
.I dcheck ,
and
.I ncheck
work with all kinds of file system.
.PP
If you check the block special file,
you don't have to reboot the system after fixing the root.
We borrowed this simple convenience from
4.1BSD;
it seems to have vanished from subsequent Berkeley systems.
.PP
.I Mkfs
makes a classical file system;
.I mkbitfs
a bitmapped file system,
which will be the small kind if possible.
To get reasonable speed,
it is important to set file system spacing parameters correctly;
see
.I mkfs (8).
In our experience,
the `space' number is very important,
but the `cylinder size' makes little difference.
.PP
Read
.I mount
and
.I umount (8)
carefully;
notice in particular that the arguments to
.I umount
are mount points like
.CW /usr ,
not special files like
.CW /dev/ra02 .
.PP
An example to get you started:
many of our machines have system files on a single RA81.
The driver allows the disk to be split into certain sections
(\c
.I ra (4)).
We use these sections:
.IP
Section 0 (5 megabytes) is the root.
.IP
Section 1 (10 megabytes) is the swap area.
.IP
Section 2 (120 megabytes)
is
.CW /usr .
.IP
Section 5 (50 megabytes at the high end of the disk)
is a rather generous
.CW /tmp .
.PP
Users' files might go in
.CW /usr ,
or in other file systems in the other two
120 megabyte sections;
if the latter,
we use symbolic links
to arrange that
.CW /usr/andrew
always means
.I andrew 's
home directory.
.PP
Backups are somewhat ad hoc.
The old V7
.I dump
and
.I restor
programs have long since rotted,
and their newer Berkeley-derived kin
seem too complicated to be worth adapting.
We back up whole file systems
simply by copying the special file to tape with
.I dd ;
120 megabytes in 16Kbyte blocks
fits nicely on a 6250 bpi magtape.
Such backups are mostly meant for use when disk drives break;
they aren't very convenient when files are accidentally deleted.
.PP
A more sophisticated and more complicated system
for incremental backups
is described in
.I backup (1)
and
.I backup (8).
.I Cpio
and
.I tar
are simpler possibilities.
.NH
Files to watch for
.PP
Most of our machines are run in a fairly open manner;
special files for disks and memory
often have general read permissions.
Most system files and directories
may be written by group
.I bin ;
if the special files aren't generally readable,
they may certainly be read by group
.I sys .
You may want to change some of this,
and to browse around generally for holes.
.PP
Here are some files that have system-dependent contents;
the versions on the tape are probably wrong,
and must be edited for your system.
.de Fp
.IP
.ne 2
.ti -5n
.CW "\\$1"
.br
.if !'\\$2''\{.
.ti -5n
.CW "\\$2"
.br\}
..
.Fp /etc/whoami
A single line of text containing this machine's name,
printed by
.I getty (8),
used by
.I uucp
and miscellaneous network programs.
.Fp /n
Mount points for network file systems,
.I netfs (8).
.I
.CW /n/ remsys
.R
is the (empty) directory
on which the remote file system for system
.I remsys
is mounted.
.I
.CW /n/ myname\c
.R
,
where
.I myname
is the name in
.CW /etc/whoami ,
is conventionally a symbolic link
to
.CW / .
.Fp /etc/fstab
Filesystems to be mounted in normal operation,
.I fstab (5).
.Fp /etc/ttys
Terminal lines to be maintained by
.I init
and
.I getty (8),
.I ttys (5).
.Fp /etc/crontab
List of actions for
.I cron (8).
.Fp /etc/passwd /etc/group
List of users and groups,
.I passwd (5).
.Fp /etc/rc
Script to be executed when the system starts,
.I rc (8).
.Fp /lib/dst
Daylight saving time definitions,
.I ctime (3).
The timezone is compiled into the kernel,
as is a flag that declares whether daylight time ever happens;
these are probably both mistakes.
.Fp /lib/ttynames
List of directories searched by
.I ttyname (3).
.Fp /etc/aculist
List of telephone dialling devices,
.I dialout (3).
.I Cu
and
.I uucp
depend on this list.
.PP
If you use networking,
.I uucp ,
or electronic mail,
there are other files to fix;
details below.
.PP
Here are some accounting files
that should be truncated from time to time:
.Fp /usr/adm/wtmp
Session accounting,
.I login (8).
.Fp /usr/adm/messages
Record of console messages,
.I dmesg (8).
.Fp /usr/adm/smdr
Autodialler use;
written if possible by
.I dialout (3).
.Fp /usr/spool/mail/mail.log
Record of mail sent or delivered,
.I upas (8).
.Fp /usr/netb/setupl /usr/netb/zarf.log
Network file system logs,
.I netfs (8).
.Fp /usr/ipc/log
Directory where various daemons in
.CW /usr/ipc/mgrs
write log files;
.I svcmgr ,
.I dkmgr ,
.I tcpmgr (8)
among others.
.Fp /usr/spool/uucp/.Log
Log files from various
.I uucp
daemons.
.PP
We run
.CW /usr/ipc/servers/daily
just before midnight every day;
it maintains a week's worth of daily issues
of the
.CW /usr/ipc/log
files and of
.CW mail.log .
If present,
.CW /usr/adm/weekclean
and
.CW /usr/adm/monthclean
show what we do
about some of the other files.
.NH
.....
Setting up mail
.PP
Everyone has a complicated mail system these days;
ours is perhaps simpler than many,
but still requires some setting up.
There are various files to fix in
directory
.CW /usr/lib/upas ,
especially
the list of addressing rules in
.CW rewrite
and the list of local name alias files in
.CW namefiles .
See
.I mail (1)
and
.I upas (8),
and the
.I upas
paper in Volume 2.
.PP
The
.CW rewrite
on the tape knows about local mail
and
.I uucp ,
which is probably enough to get you started;
the supplied
.CW namefiles
points at two empty files
.CW names.local
and
.CW names.global .
Our usual
.CW rewrite
file is in
.CW rewrite.fancy ;
you don't want to use it unchanged,
but it is there as an advanced example.
Shell scripts like
.CW route
go with it.
Before trying to use them,
please read them carefully,
and remove things like
use of our gateway machine
.I research .
.NH
Setting up uucp
.PP
Our
.I uucp
programs belong to the
honey danber
family,
and behave in a mostly standard way.
The principle difference is that
the programs don't read a single
.CW Systems
file to look up a system.
Instead,
they run
.CW /usr/lib/uucp/uugrep
with an argument naming the system of interest.
.I Uugrep
is expected to print one or more
.CW Systems
lines,
which may or may not be relevant to that system;
the system name in each line must still be checked.
.PP
Normally
.I uugrep
just prints the contents of all the files named in
.CW /usr/lib/uucp/Sysfiles .
However, if the files
.CW /usr/lib/uucp/Bsystems.[FT]
exist,
they are assumed to contain a
.I cbt (3)
database of
.CW Systems
entries;
.I uugrep
searches the database
and prints only the lines for the interesting system.
If that system isn't found,
or one of the original
.CW Systems
files is newer than the database,
the database is ignored
and all the files are printed.
.PP
.I Uugrep
without arguments
spits out all the entries,
and rebuilds the database if necessary.
.PP
To use the database,
create empty
.CW Bsystems.T
and
.CW Bsystems.F
files (with the same permissions used for Systems files),
and run
.P1 0
/usr/lib/uucp/uugrep >/dev/null 2>&1
.P2
Make arrangements to run this whenever one of the
.CW Systems
files changes;
we just do it every hour.
.PP
.CW /usr/lib/uucp/uudemon.hr
is run hourly,
.CW uudemon.day
daily.
Take them as examples.
.PP
None of the
.I uucp
files are used by
.I cu
in our system.
.NH
Setting up networking
.PP
The programs in
.I con (1)
are used to remotely log into another machine
and to execute remote commands.
.I Push
and
.I rcp (1)
copy files.
Our
.I push
speaks a different
(and markedly simpler)
language from the DKHOST program of the same name;
.I npush
and
.I npull
attempt compatibility.
.PP
For each kind of network,
there is at least one network-specific daemon
responsible for listening and dialling.
This daemon announces a local name
through which outbound calls may be requested.
It also listens for inbound calls from the network,
maps the network's idea of a service name
through some local convention,
and calls the appropriate local name for the service.
Most local services,
login and command execution in particular,
are handled by
.I svcmgr (8).
.PP
Local service names,
including network dialling,
are announced with
.I ipccreat
and called with
.I ipcopen ;
see
.I ipc (3).
The services are usually announced in directory
.CW /cs .
For example,
the Datakit dialler announces itself as
.CW /cs/dk ;
.I con
opens that name with
.I ipcopen
to place a call.
.PP
To turn on any network,
you must run
.I svcmgr .
Edit
.CW /usr/ipc/lib/serv
and
.CW auth
to contain the desired services
and authorization mappings.
The ones on the tape
are certainly wrong;
use them as examples.
.PP
To turn on Datakit,
you must have the right special files in
.CW /dev/dk ,
and must run
.I dkhup
and
.I dkmgr ;
see
.I dk (4)
and
.I dkmgr (8).
The device files on the tape are probably wrong.
Watch out for
.CW /dev/dk/dkctl ,
which should be linked to the common signalling channel;
this is channel 1 on production Datakits,
but not on most of our machines.
.PP
If you're using a KMC,
run
.CW /etc/kdiload
to load microcode into it,
after editing
.CW /etc/kmctab
to point at the appropriate KMC microcode:
.CW /etc/dkk.kdi
for the KMS11-K,
.CW /etc/dkk.dubhi
for the DUBHI.
.I Kmc (8)
explains some of this.
.PP
To turn on TCP/IP,
you must run an instance of
.I ipconfig (8)
for each IP network interface,
and one instance of
.I tcpmgr (8).
If you want to use UDP,
run
.I udpconfig
too.
.PP
Besides the hardware devices
(e.g. an Ethernet controller),
these special files are needed
for TCP/IP.
Channel numbers like that in
.CW /dev/tcp01
must always be two digits.
.KS
.TS
c c c
lFCW l l.
_
file	maj,min
=
/dev/ip0	42,0	for general IP control
/dev/iptcp	42,6	for TCP
/dev/ip16	42,16	for \fIrouted\fP
/dev/ipudp	42,17	for UDP
/dev/tcp\fInn\fP	43,\fInn\fP	TCP channels
/dev/udp\fInn\fP	50,\fInn\fP	UDP channels
_
.TE
.KE
.PP
Most TCP/IP programs depend on the name service mechanisms
described in
.I qns (7)
and
.I ns (8).
You must run
.I ns ,
after adjusting
.CW /usr/ipc/lib/ns.db .
.PP
To turn on our kind of network file system,
you need to edit files in
.CW /usr/netb ,
and to run certain programs kept there;
see
.I netfs (8).
Our network file system protocol
is different from everyone else's.
Since our server is just a user program,
it is possible with some effort to make it run
under other operating systems,
making their files accessible
to the machine running our system
(but not vice versa).
For example, some of our machines
can see all the files on
the local Cray.
Look in the sources if you want to do this.
.NH
Bitmapped terminals
.PP
There is a fairly extensive set of programs
for the Teletype 5620 terminal;
see section 9 of the manual,
in particular
.I intro (9).
The programs live in
.CW /usr/jerq .
Some of the programs have been moved to the AT&T 630 terminal;
the 630 versions live in
.CW /usr/630 .
There is no documentation for the 630 versions,
but most are the same as their 5620 cousins.
.I 630mux
is the window multiplexer.
The name is deceptive;
.I 630mux
terminal windows
act like the ones in the 630 ROM,
not like those in
.I mux .
.NH
Miscellaneous hardware support
.PP
If you have a VAX-11/750
with a patchable control store
(most do),
.I ldpcs (8)
will load DEC's microcode patches.
.PP
The system doesn't replace bad blocks automatically
on MSCP disks,
but
.I rarepl (8)
will replace a particular block on request,
and
.I smash (8)
will often make it unnecessary.
In our experience,
bad block replacement is almost never appropriate on RA81s.
.PP
On a VAX-11/780,
.CW /dev/floppy
is the console disk;
it should be a character device
with major number 8,
minor number 0.
.I Arff (8)
attempts to understand the RT-11 file system format used by the console.

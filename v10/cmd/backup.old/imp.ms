.FP palatino
.TL
Backup Recovery Implementation
.AU
Andrew Hume
.AI
.MH
.SH
Overview
.PP
This note describes how the backup recovery process takes place,
at least with respect to the WORM software.
.PP
The typical scenario is a user invokes
.I wfetch
with a list of backup filenames, a destination system name and a WORM drive number.
The destination (or WORM) system will have either a small number of WORM drives
or a jukebox.
The programs on the WORM system are
.I wormy
which takes file names and pulls them off a WORM drive and
.I wormserver
(or
.I jukeserver )
which acts as an interface between multiple
.I wfetch 's
and
.I wormy .
.PP
All the chitchat between the above programs does not depend on any I/O
blocking, thus TCP can be used as an underlying transport.
In general, handshaking is done on a line by line basis
switching to big block I/O for files.
.SH
\f4wfetch\fP
.PP
.I Wfetch
establishes a connection with the far
.I wormserver .
It does this by opening
.I mach!/cs/dk/bkpwormn
where
.I n
is the drive number.
If this fails (which it will 90% of the time),
it calls a startup service (found in
.I /usr/ipc/lib/serv.local )
called
.I startwormn .
This is a call to either
.I "wormserver n"
or
.I jukeserver .
After
.I wormserver
has done the
.I ipccreat
to mount
.I mach!/cs/dk/bkpwormn ,
it sends an acknowledge back to
.I wfetch
which then attempts to access
.I mach!/cs/dk/bkpwormn
once more.
If this fails again,
.I wfetch
gives up.
.PP
After the connection is established,
.I wfetch
thinks it has been connected to an instantiation of
.I wormy .
.I Wfetch
sends all the file names and then waits
for files or diagnostics to be sent back.
Files are processed and put into the filesystem
and diagnostics are simply echoed.
.SH
\f4wormy\fP
.PP
.I Wormy
is a simple process.
It reads the WORM on a given drive (specified as an argument)
and after reading a list of filenames from file descriptor 0,
transmits the files on file descriptor 1.
Missing files are divided into two classes.
Ones that should be on this disk get long diagnostics.
Otherwise, the name of the disk the files are on are reported.
.SH
\f4wormserver\fP
.PP
.I Wormserver
is a simple server.
As soon as it starts it spawns a
.I wormy
and then sits waiting for calls.
When it gets a call, it passes the call's fd to its
.I wormy
and waits for an ack back to show that
.I wormy
is done.
During this time, calls are pseudo-rejected to indicate busy.
(Pseudo-rejected because Datakit is buggered; we actually accept but
pass a magic cookie.)
The drive number for the spawned
.I wormy
was an argument to
.I wormserver .
.PP
If more than some timelimit (2.5mins) occurs between calls then
.I wormserver
simply exits (and thus killing the spawned
.I wormy ).
.SH
\f4jukeserver\fP
.PP
.I Jukeserver
is much the same as
.I wormserver .
The only difference is in processing an accepted call.
We first suck up all the filenames.
We then divide the filenames into groups per disk.
Then for each disk, we mount the disk and fire up a
.I wormy .
All but the last invocation of
.I wormy
are killed when they are done.

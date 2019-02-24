.so ../ADM/mac
.XX netb 513 "A Look at the Ninth Edition Network File System"
.nr dP 2
.nr dV 3p
.nr tP 1
.nr tV 1p
.nr Hs 5
.TL M10200 40125-100
A Look at the Ninth Edition Network File System
.AU sar SF XT9112200 x6084 5-121 attunix!sar
Stephen Rago
.AI
.MH
.AB
The protocol used for the network file system in Research
.UX
has evolved since its original design and implementation.  This paper describes the
current version of the protocol (NETB), including the semantics for both the client
and the server.
.AE
.2C
.NH 1 
Introduction
.PP
In Version 8 of the
.UX
operating system, Peter Weinberger*
.FS
.nf
*
.ps 6
.ft H
.tr x.
.tr -
.cs H 5
.vs 8u
-------------------xxxxxx-x---------------------
-----------------xxxxxxxxxxxxx------------------
----------------xxxxxxx-xxxxxxxx----------------
---------------xx-xxxxxxx-xxxxxxxx--------------
---------------xxxx-xxxxxxxxx-x-xxx-------------
--------------x---------xxxxxxxxxxxxx-----------
--------------x----------xxxxxxxxxxxxxxxx-------
---------------------------xxxxx-xxxxxx---------
------------xx-------------xxxxxxxxxxxxxxx------
---------------------------x-x-xxxxxxxxxxx------
----------xx---------------xxxxxxxxxxxxxxxxx----
---------xxx----------------xxxxxxxxxxx-x-xx----
--------xx-------------------xxxxxxxxxxxxxxxx---
--------xxx------------------xxxxxxxxxxxxxx-x---
-------xxxx-------------------xxxxxxxxxxxxxxx---
------xxxx---------------------xxxxxxxxxxxxxx---
------xxxxx--------------------xxxxxxxxxxxxxx---
-----xxxxx----x-x---------------xxxxxxxxxxxx----
----xxxxxxxxx-x-xxxxx-----xxxxxxx-xxxxxxxxxxx---
----xxxxxxx------xxxxx---xx--xxxxxxxxxxxxxxxx---
---xxxxxxxxx---xxxx-xxxxxxxxx--x-xxxxxxxxxxxx---
---xxxxxxxxx-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx---
---xxxxxxxxx-x-xx-x--x--xxxxxxxxxxxxxxxxxxxxx---
---xxxxxxx-x------x--x---xxxxxxxxxxxxxxxxxxx----
----xxxxxxx-----x--------x---x-xxxxxxxxxxxxx----
---xxxxxx-x---------x----xxx------xxxxxxxxxx----
------xx--x--------------xx-----xx-xxxxxxxx-----
----x-xxx----------x------xx-------xxxxxx-------
-------xx------x-xx--------xxxxxxxxx-xxxx-------
--------x----x-x-x-------x-xx-x--x-xxxxx--------
--------------------x-xxxxxxx-xxxxxxxxxx--------
-------xx----------xxxxxxxx-x---xxx-xxx---------
------xxx------------xxxxxxx--x-x-xxxxx---------
-------xx-----x-------xxxx-x-x-xxxx-xxxxxx------
--------x----------------xxx-x-x-xxxxxxxx-------
-------xxx-----xxx-x-x-xxx-xxx-x-x--xxxxx-------
--------xx-----x-x-x-xxxxxxxxxxxxxxxxxxxx-------
---------x----------------xxx--x-xxxxxxx--------
---------xx--------------xx-x-xxxx--xxx---------
-------------x-----xxxxxxxx-x-xx-xx-------------
---------------------xxxxxxxxxxxxxxxx-----------
--------------x------------x-x-xx-x-------------
-------------------------x-x-xxxxx--------------
--------------x-----------xxxxxx-x--------------
----------------x--------x--x-xxxx--------------
--------------x-x-xxx-xxxxxxxxx-x---------------
----------------x---xxxxxxxxx-xxxx--------------
---------------x-xxxx-x-x-xxxxxxxx--------------
.tr --
.tr xx
.fi
.vs
.ps
.ft
.FE
wrote a network file system that enabled files to be shared between machines
connected by a network.  When I asked someone if there was a paper describing
it, they said, "yeah, but it's only an abstract."  I thought they were speaking
figuratively.  They were speaking literally |reference(weinberger neta).
Hence, as I began studying the
Version 9 network file system in preparation for porting it to UNIX System V Release 4.0,
it was suggested that I also document what it does.
This paper describes the inner workings of Weinberger's network file system.
.PP
The UNIX Version 8 network file system, known as NETA, provided remote file access
for the Computing Science Research Center.  It has evolved over the years and
in its current form, runs on UNIX Version 9 and is now called NETB.
.NH 1 
Architecture
.PP
The client side is implemented in the kernel as a file system switch entry.  The server
side is implemented as a user-level process.  There is one server process
per client machine.
.PP
The server's name is
.I zarf .
(A zarf is an ornamental metal holder for a hot coffee cup.)
.PP
By convention, each machine
mounts other machines' file systems on mount points under
.CW /n .
.NH 1 
Client Startup
.PP
The client side of the network file system includes a command/daemon (\fIsetup\fP)
to set up
connections and a file system (NETB) to translate file system operations into
streams messages.  \fISetup\fP makes a connection to the remote machine's server and
sends it information about the protocol version, client user ids, and client group ids.
Then it mounts the connection in the file system name space using NETB.
.PP
In the daemon mode, the command is started without any arguments.  It reads the
file
.CW /usr/netb/friends
to decide which systems to mount.  
Every 60 seconds the daemon checks the remotely mounted file systems to see if they are
still available.  It also checks to see if the friends file has changed.  If any friendly file
system is unavailable, then an attempt is made to mount it.
The format of the friends file is a list of entries containing a \fInetname\fP,
a \fIcall_arg\fP, a \fImount_point\fP,
a \fIprotocol\fP, a \fIunique_id\fP, a \fIdebug_flag\fP, and a \fIusername\fP,
separated by white space.
.PP
The \fInetname\fP is the network name (see
.I ipc (3))
of the server machine.
The \fImount_point\fP is the location in the local file system where the remote file
system is to be mounted.
The \fIprotocol\fP indicates how the connection is to be placed.  Protocol
.CW d
uses the
default network
.CW dk
(Datakit\(rg), expecting the service to be named
.CW fsb
and ignoring the
\fIcall_arg\fP.  Protocol
.CW t
uses TCP, starting the service given by the
\fIcall_arg\fP using the
.I rsh
protocol.
The \fIunique_id\fP is an integer between 0 and 255 that distinguishes the connections
and must be unique among all active remote file systems.
The \fIdebug_flag\fP determines how much information gets written to the log files.
The \fIusername\fP is an optional field that provides the name of the calling process
making the mount.  The default user name is
.CW daemon .
.PP
In command mode, the remote file system is mounted, but no daemon is created.
The arguments to the command are the same as the parameters in the friends file.
.NH 1 
Server Startup
.PP
The server expects to be started by the remote execution facility.
As such, file descriptor zero is attached to the network connection.
First, the server tries to identify the client.  The method it uses depends
on the environment in which it is running.
On 4BSD systems, it does this by calling
.I getpeername (2).
On Version 9 (and System V)
systems, it does this
by checking the environment variables
.CW CSOURCE
and
.CW DKSOURCE .*
.FS
* CSOURCE is set by the remote execution facility when a service is invoked.
DKSOURCE is set by an older version of the remote execution facility.
.FE
The server then changes directory to
.CW /usr/netb .
If this fails, the server
changes directory to
.CW /tmp .
A log file,
.CW zarf.log ,
is created.
.PP
The first network message read by the server is 16 bytes long.
The format is:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
a | a .
byte	use
=	=
0	maximum client message size
_	_
1	client device number
_	_
2	protocol type
_	_
3	debug flag
_	_
4-15	unused
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
The maximum message size is currently set to 5K bytes.  In the message,
the size is expressed in units of 1KB.  The client can change the maximum
message size via an \fIioctl\fP (see \(sc5.1.2).
The device number is the major device number
of the client.
It is the same as the unique id obtained from the friends file.
The server creates device numbers using this major number.  The minor number
is created on the fly and is unique among all device numbers the server has seen since
it started.
.PP
The protocol type is expressed as an ASCII character.
The currently supported protocols are
.CW t
(TCP byte streams) and
.CW d
(Datakit messages).
The debug flag determines the detail of the messages that get written to the log file.
.PP
The server responds to the first message with a one-byte response
containing the value 1.
.PP
The second network message read is \fImax_message_size\fP bytes long.
It contains the user ids from the client machine,
obtained from the client's password file.  The entries are in ASCII, with the last entry
followed by a NULL.
The format of each entry in the message is:
.P1
user_name user_id\n
.P2
The server responds to this message with a one-byte response containing the value 2.
.PP
The user id message is parsed and a table is built mapping client user ids to server
user ids.  Client and server hash lists map user ids to indices into the mapping table
to speed lookup operations.
.PP
The third network message read is also \fImax_message_size\fP bytes long.
It contains the group ids from the client machine,
obtained from the client's group file.  The entries are in ASCII, with the last entry
followed by a NULL.
The format of each entry in the message is:
.P1
group_name group_id\n
.P2
The server responds to this message with a one-byte response containing the value 3.
.PP
The group id message is parsed and a table is built mapping client group ids to server
group ids.  Client and server hash lists map group ids to indices into the mapping table
to speed lookup operations.
.PP
If there are too many user or group ids to fit in the message, the client does not
attempt to send the message to the server.  Rather, the client closes the connection
and the server exits.
.PP
If everything is successful, the
server opens its root and creates an internal file entry mapping
its root to the client's mount point.  The rest of the time is spent in a loop
servicing client requests.
.NH 1 
File System Operations
.PP
The file system operations in Version 9 are:
.nr xx \\n(PDu
.nr PD 0.1v
.IP \fBmount\fP 8
mount and unmount the file system
.IP \fBput\fP
release an inode
.IP \fBget\fP
get an inode
.IP \fBfree\fP
free the disk space associated with the inode
.IP \fBupdat\fP
update the times on the inode
.IP \fBread\fP
read from the inode's file
.IP \fBwrite\fP
write to the inode's file
.IP \fBstat\fP
return the statistics on the inode's file
.IP \fBtrunc\fP
truncate the inode's file
.IP \fBnami\fP
parse a pathname
.IP \fBioctl\fP
file system-specific ioctls and device ioctls
.IP \fBopen\fP
open a device
.IP \fBdirread\fP
read a directory
.nr PD \\n(xxu
.PP
In the network file system, file system operations
consist of request/response messages.  The client sends a message
to the server and blocks until the response comes back.
For historical reasons, the messages follow VAX-style byte ordering and
all network message structures are multiples of 8 bytes.
Messages sent from the client to the server are of the following format:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
a .
message header
(struct sendb)
_
optional
operation-specific
header
_
optional
user data
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
.PP
Throughout the rest of this paper, C structure definitions are used to describe
network messages.  Elements of type \f(CWchar\fP are one byte long,  elements of type
\f(CWshort\fP are two bytes long, and elements of type \f(CWlong\fP are four bytes long.
Fields are included to remove any ambiguity regarding structure padding.
.PP
The request message header is defined by a \f(CWstruct sendb\fP:
.P1
struct sendb {
	char	version;
	char	cmd;
	char	flags;
	char	rsva;
	long	trannum;
	long	len;
	long	tag;
	short	uid;
	short	gid;
	long	rsvb;
}
.P2
.PP
\f(CWversion\fP indicates which version of the protocol is in use.
\f(CWcmd\fP indicates the operation requested.
\f(CWrsva\fP and \f(CWrsvb\fP are pad fields.
\f(CWtrannum\fP is the transaction number for the message.
\f(CWlen\fP is the length of the message in bytes, including the headers and data.
\f(CWtag\fP is used to identify the file to which the operation pertains.
The tags are composed of the device number where the file lives shifted left 16 bits,
ORed with the inode number.
\f(CWuid\fP and \f(CWgid\fP are the user and group ids of the client generating the operation,
respectively.
.PP
Messages sent from the server to the client are of the following format:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
a .
message header
(struct recvb)
_
optional
operation-specific
header
_
optional
user data
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
.PP
The response message header is defined by a \f(CWstruct recvb\fP:
.P1
struct recvb {
	long	trannum;
	short	errno;
	char	flags;
	char	rsvd;
	long	len;
	long	fsize;
}
.P2
The server copies \f(CWtrannum\fP from the request message so the client can
match up responses with requests.
\f(CWerrno\fP is the error number if the operation failed, or 0 on success.
The \f(CWflags\fP field is used to indicate special conditions for certain operations.
\f(CWrsvd\fP is a pad field.  \f(CWlen\fP is the
length of the message, including the headers and data.  \f(CWfsize\fP is the file
size and is set only by the read, write, and dirread file system operations.
.PP
The following sections describe some details of the file system operations
for the network file system, including the content of messages exchanged between the client
and the server.  There is no
.I nbopen ()
entry point.
Instead, a stub (\fInullopen\fP) is used, which is provided by the system.
This is because \fIzarf\fP opens the file as part of parsing the pathname, so no extra
protocol is necessary.
\fIZarf\fP does not fully support access to devices.
.NH 2 
nbmount
.PP
When mounting the file system, the \fIflag\fP argument to
.I fmount (2)
contains the
device number for the remote file system.
The file descriptor passed to \fIfmount\fP is the client's end of the network
connection to the server.
The client side creates an inode for the root of the file system with this information.
.PP
When unmounting the file system, all active inodes on that file system
are converted to inodes in the error file system (errfs).  This will cause all further
operations on them to fail, until the inodes are freed.
.NH 2 
nbget
.PP
The information
in the inode is filled in by \fInbget\fP, called from \fInami\fP.
For the root inode, it can be called from \fInbmount\fP, so it
constructs the information by hand.  However, for all other inodes, it fills
in the information with the cached data obtained from the previous nami message.
The nami cache is one entry long.
.X "PARM CT 70
.NH 2 
nbput
.PP
If the inode is marked ICHG, \fInbupdat\fP is called.
Then a NBPUT message is sent
to the server.  The format of the message is:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBPUT request
=
sendb	value
_	_	
version	2
cmd	NBPUT
flags	0
rsva	unused
trannum	transaction number
len	sizeof(struct sendb)
	(24 bytes)
tag	tag of inode
uid	uid of client
gid	gid of client
rsvb	unused
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
.PP
For this operation, the server will search through its list of open files, matching on
\f(CWtag\fP.
If the file is found and it is not the root, then the server closes the file and invalidates
its entry in the list.  The response sent back to the client is:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBPUT response
=
recvb	value
_	_	
trannum	transaction number
errno	error number or 0
flags	0
rsvd	unused
len	sizeof(struct recvb)
	(16 bytes)
fsize	0
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
.X "PARM CT 50
.NH 2 
nbfree
.PP
\fINbfree\fP is just a stub that doesn't do anything.  Since the server removes
the disk space associated with a file during the nami NI_DEL and NI_RMDIR
operations, there is no need to do anything in \fInbfree\fP.
.NH 2 
nbupdat
.PP
This operation invalidates the nami cache.
The message sent to the server contains an operation-specific header in the form of
a \f(CW struct snbup\fP:
.P1
struct snbup {
	unsigned short	mode;
	short		rdvdd;
	long		rsvd;
	long		ta;
	long		tm;
}
.P2
The \f(CWmode\fP is the mode of the file from the inode.  \f(CWrdvdd\fP is currently unused.
\f(CWrsvd\fP is a pad field.  \f(CWta\fP
is the access time on the client machine and \f(CWtm\fP is
the modify time on the client machine.
If the inode is not marked IACC, \f(CWta\fP is set to 0.
If the inode is not marked IUPD, \f(CWtm\fP is set to 0.
The message sent to the server is as follows:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBUPD request
=
sendb	value
_	_	
version	2
cmd	NBUPD
flags	0
rsva	unused
trannum	transaction number
len	sizeof(struct sendb)+
	sizeof(struct snbup)
	(40 bytes)
tag	tag of inode
uid	uid of client
gid	gid of client
rsvb	unused
_	_
snbup	value
_	_
mode	mode of inode
rdvdd	unused
rsvd	unused
ta	access time or 0
tm	modify time or 0
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
.PP
If the times are zero in the client request message, the server obtains the times
from the cached
.I stat (2)
information for the file.  Otherwise the times from the message
are adjusted with a time drift correction (see \(sc5.8).
The access and modify times are updated via
.I utime (2).
If the modes
are to be changed, and the client process owns the file, they are updated with either
.I fchmod (2)
or
.I chmod (2).
The server responds to the client with the following message:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBUPD response
=
recvb	value
_	_	
trannum	transaction number
errno	error number or 0
flags	0
rsvd	unused
len	sizeof(struct recvb)
	(16 bytes)
fsize	0
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
........
.NH 2 
nbread
.PP
This operation invalidates the nami cache.
The client sends multiple messages to the server, reading at most \fImax_message_size\fP-16
bytes at a time (currently 5104 bytes).
.PP
The message sent to the server includes an operation-specific \f(CWsnbread\fP structure:
.P1
struct snbread {
	long len;
	long offset;
}
.P2
\f(CWlen\fP is the number of bytes to be read and \f(CWoffset\fP is the offset in
the file at which the read should start.  The message sent to the server is as follows:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBREAD request
=
sendb	value
_	_	
version	2
cmd	NBREAD
flags	0
rsva	unused
trannum	transaction number
len	sizeof(struct sendb)+
	sizeof(struct snbread)
	(32 bytes)
tag	tag of inode
uid	uid of client
gid	gid of client
rsvb	unused
_	_
snbread	value
_	_
len	number of bytes to read
offset	byte offset where to start
	reading
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
.PP
The server reads the data and sends it back to the client.  When all of
the data is read, or
.I read (2)
returns less than the amount asked for, the \f(CWflags\fP
field is set to NBEND.
The message sent from the server to the client is:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBREAD response
=
recvb	value
_	_	
trannum	transaction number
errno	error number or 0
flags	0 or NBEND
rsvd	unused
len	sizeof(struct recvb)+
	number of bytes read
fsize	size of attempted read
_	_
data	read from file
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
.NH 2 
nbwrite
.PP
This operation invalidates the nami cache.
The client sends multiple messages to the server, writing at most \fImax_message_size\fP-32
bytes at a time (currently 5088 bytes).
The message sent to the server includes an operation-specific \f(CWsnbwrite\fP structure:
.P1
struct snbwrite {
	long len;
	long offset;
}
.P2
\f(CWlen\fP is the number of bytes to be written and \f(CWoffset\fP is the offset in
the file at which the write should start.  The message sent to the server is as follows:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBWRT request
=
sendb	value
_	_	
version	2
cmd	NBWRT
flags	0
rsva	unused
trannum	transaction number
len	sizeof(struct sendb)+
	sizeof(struct snbwrite)+
	number of bytes to write
tag	tag of inode
uid	uid of client
gid	gid of client
rsvb	unused
_	_
snbwrite	value
_	_
len	number of bytes to write
offset	byte offset where to
	start writing
_	_
data	to be written
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
The client marks the inode (IUPD|ICHG).  After the write, the server seeks to
the end of the file and updates the size of the file.  The message sent back to the client is:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBWRT response
=
recvb	value
_	_	
trannum	transaction number
errno	error number or 0
flags	0
rsvd	unused
len	sizeof(struct recvb)
	(16 bytes)
fsize	file size
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
The client updates the size of the file in the inode with \f(CWfsize\fP.
.NH 2 
nbstat
.PP
The client maintains a one-entry cache from nami processing to avoid
sending a stat message to the server immediately after a nami operation.
If the client cache entry is valid, the
.I stat (2)
is satisfied from the
cached data.  Otherwise a message is sent to the server.  The operation-specific
part of the message contains a \f(CWsnbstat\fP structure:
.P1
struct snbstat {
	long ta;
	long rsvd;
}
.P2
\f(CWta\fP is the current time on the client and is used for synchronization.
\f(CWrsvd\fP is a pad field.  The message sent from the client to the server is:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBSTAT request
=
sendb	value
_	_	
version	2
cmd	NBSTAT
flags	0
rsva	unused
trannum	transaction number
len	sizeof(struct sendb)+
	sizeof(struct snbstat)
	(32 bytes)
tag	tag of inode
uid	uid of client
gid	gid of client
rsvb	unused
_	_
snbstat	value
_	_
ta	current time on client
rsvd	unused
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
.PP
After each of the first three stat messages, and
after every third stat message thereafter, the server recalculates the time drift between the
client machine and the server machine.  The server uses
.I fstat (2) to obtain the
information about the given file.
The response message sent to the client has an operation-specific header
that contains a \f(CWrnbstat\fP structure:
.P1
struct rnbstat {
	long	ino;
	short	dev;
	short	mode;
	short	nlink;
	short	uid, gid;
	short	rdev;
	long	size;
	long	ta;
	long	tm;
	long	tc;
};
.P2
The response message is as follows:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBSTAT response
=
recvb	value
_	_	
trannum	transaction number
errno	error number or 0
flags	0
rsvd	unused
len	sizeof(struct recvb)+
	sizeof(struct rnbstat)
	(48 bytes)
fsize	0
_	_
rnbstat	value
_	_
ino	inode number
dev	device number
mode	mode of file
nlink	number of links to file
uid	owner of file
gid	group of file
rdev	unused
size	size of file
ta	access time
tm	modify time
tc	change time
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
.NH 2 
nbtrunc
.PP
\fINbtrunc\fP invalidates the nami cache.
The message sent to the server is as follows:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBTRNC request
=
sendb	value
_	_	
version	2
cmd	NBTRNC
flags	0
rsva	unused
trannum	transaction number
len	sizeof(struct sendb)
	(24 bytes)
tag	tag of inode
uid	uid of client
gid	gid of client
rsvb	unused
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
.PP
The client must own the file to be allowed to truncate it.  The server uses
.I creat (2)
to truncate the file.  Since
.I creat
also opens the file
for writing, the server immediately closes it.  The message sent back to the
client is:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBTRNC response
=
recvb	value
_	_	
trannum	transaction number
errno	error number or 0
flags	nami flags
rsvd	unused
len	sizeof(struct recvb)
	(16 bytes)
fsize	0
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
.NH 2 
nbnami
.PP
The file system-specific nami performs a number of operations.
It can parse a pathname, link a file, unlink a file,
make a directory, remove a directory, and create a file. 
The message sent to the server contains an operation-specific header in the form
of a \f(CWsnbnami\fP structure:
.P1
struct snbnami {
	short mode;
	short dev;
	long ino;
}
.P2
The message sent to the server is as follows:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBNAMI request
=
sendb	value
_	_	
version	2
cmd	NBNAMI
flags	nami operation
rsva	unused
trannum	transaction number
len	sizeof(struct sendb)+
	sizeof(struct snbnami)+
	number of bytes in the
	pathname
tag	tag of inode
uid	uid of client
gid	gid of client
rsvb	unused
_	_
snbnami	value
_	_
mode	mode of inode for
	NI_CREAT, NI_NXCREAT,
	and NI_MKDIR
dev	unused
ino	inode number (tag) for
	NI_LINK
_	_
data	full pathname
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
The \f(CWflags\fP field determines what the server should do.
While parsing the pathname, if it goes out of the remote file system, the server
sets the \f(CWflags\fP field in the response message to NBROOT and sends the message
back to the client so the
client can continue with pathname evaluation.  Each nami operation is described in the
following sections.
.X "PARM CT 60
.PP
The response message sent back to the client contains a \f(CWrnbnami\fP structure:
.P1
struct rnbnami {
	long	tag;
	long	ino;
	short	dev;
	short	mode;
	long	used;
	short	nlink;
	short	uid, gid;
	short	rdev;
	long	size;
	long	ta;
	long	tm;
	long	tc;
};
.P2
The message is as follows:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBNAMI response
=
recvb	value
_	_	
trannum	transaction number
errno	error number or 0
flags	0 or NBROOT
rsvd	unused
len	sizeof(struct recvb)+
	sizeof(struct rnbnami)
	(56 bytes)
fsize	0
_	_
rnbnami	value
_	_
tag	tag
ino	inode number
dev	device number
mode	mode of file
used	if NBROOT, number of bytes
	parsed in pathname
nlink	number of links to file
uid	owner of file
gid	group of file
rdev	unused
size	size of file
ta	access time
tm	modify time
tc	change time
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
.PP
If the pathname evaluates to a file on the server's file system, then the server
will perform different tasks based on the value of \f(CWflags\fP in the request message.
.NH 3 
NI_SEARCH
.PP
The server opens the file and sends the response message to the client.
.NH 3 
NI_DEL
.PP
If the client process has write permission in the directory containing the file, and if
the file is a regular file, then the server attempts to
.I unlink (2)
it.
If the file is a directory, the server tries to remove it with
.I rmdir (2)
instead.  Other
file types cannot be deleted.
.NH 3 
NI_CREAT
.PP
The server creates (and truncates) the file with
.I creat (2).
For
new file creation, the server does not allow special files or symbolic links
to be created.
.NH 3 
NI_NXCREAT
.PP
This does the same thing as NI_CREAT, but if the file already exists, it
fails with EEXIST.
.NH 3 
NI_LINK
.PP
If the file already exists, the server fails the operation with EEXIST.
Otherwise, the server uses
.I link (2)
to link the filename to the file
represented by the client inode given by \f(CWino\fP.
.NH 3 
NI_MKDIR
.PP
If the file (directory) already exists, the server fails the operation with EEXIST.
If the server has write permissions in the parent directory, then a new directory
is created with
.I mkdir (2).
.NH 3 
NI_RMDIR
.PP
This does the same thing as NI_DEL, but deals only with removing directories.
.NH 2 
nbdirread
.PP
This operation is used to read a directory and present the contents in a canonical form.
The form is a NULL-terminated character string containing the i-number in decimal, a
tab, and the filename.
.PP
The client sends one message to the server, reading at most \fImax_message_size\fP-24
bytes at a time (5096 bytes).
The message sent to the server contains an operation-specific header in the form
of a \f(CWsnbread\fP structure (see \(sc5.6).
The message is as follows:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBDIR request
=
sendb	value
_	_	
version	2
cmd	NBDIR
flags	0
rsva	unused
trannum	transaction number
len	sizeof(struct sendb)+
	sizeof(struct snbread)
	(32 bytes)
tag	tag of inode
uid	uid of client
gid	gid of client
rsvb	unused
_	_
snbread	value
_	_
len	number of bytes to read
offset	byte offset where to
	start reading
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
.PP
The server reads the directory and responds back with a message containing the directory
contents and an operation-specific header in the form of a \f(CWrnbdir\fP structure:
.P1
struct rnbdir {
	long	used;
	long	rsvd;
};
.P2
The response message is as follows:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBDIR response
=
recvb	value
_	_	
trannum	transaction number
errno	error number or 0
flags	0
rsvd	unused
len	sizeof(struct recvb)+
	sizeof(struct rnbdir)
	(24 bytes)
fsize	size of attempted read
_	_
rnbdir	value
_	_
used	bytes read
rsvd	unused
_	_
data	directory entries
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
.PP
On success, \f(CWused\fP contains the number of bytes read.  It is added to \f(CWu.u_offset\fP
to get the new offset in the directory.
.NH 2 
nbioctl
.PP
The client side supports three
.I ioctl (2)
commands private to
the file system type.  NBIOCON turns debugging on, NBIOCOFF turns
debugging off, and NBIOCBSZ changes the network message size.
All other commands are sent to the server.  It is assumed that all
.I ioctl s
contain a 64-byte buffer of data needed for the operation.
Data are copied in from the buffer to send to the server and any resulting data
are copied out to the buffer if the operation succeeds.
The message sent to the server contains an operation-specific header in the form
of a \f(CWsnbioc\fP structure:
.P1
struct snbioc {
	long	cmd;
	short	flag;
	short	rsvd;
}
.P2
The message sent to the server is:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBIOCTL request
=
sendb	value
_	_	
version	2
cmd	NBIOCTL
flags	0
rsva	unused
trannum	transaction number
len	sizeof(struct sendb)+
	sizeof(struct snbioc)+
	64 bytes of user data
	(96 bytes)
tag	tag of inode
uid	uid of client
gid	gid of client
rsvb	unused
_	_
snbioc	value
_	_
cmd	ioctl command
flag	file table flags
rsvd	unused
_	_
data	from user ioctl buffer
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
The response message sent from the server to the client is:
.KS
.ps -\n(tP
.vs -\n(tVu
.TS
center,box;
c s
a | a .
NBIOCTL response
=
recvb	value
_	_	
trannum	transaction number
errno	error number or 0
flags	0
rsvd	unused
len	sizeof(struct recvb)+
	64 bytes of user data
	(80 bytes)
fsize	0
_	_
data	for user ioctl buffer
.TE
.ps +\n(tP
.vs +\n(tVu
.KE
.PP
Currently, \fIzarf\fP does not support \fIioctl\fP and returns ENOTTY for
this operation.
.X PARM CT 50
.NH 1 
Permissions
.PP
The server determines permissions by matching passwd and group files.
Exceptions to the default permissions can be found in
.CW /usr/netb/except.local
and
.CW /usr/netb/except .
The format of these files are comment lines begin with
.CW # ,
blank lines are ignored, and lines may contain one of four valid directives.
.PP
The
.CW client
directive specifies the client to which the following lines apply:
.P1
client \fICLIENT_NAME
.P2
where \fICLIENT_NAME\fP is the name of the client machine, or
.CW *
for all clients that
do not have an explicit entry.  The client name is separated from the keyword
by spaces or tabs.
.PP
The
.CW uid
directive specifies how to map a given user:
.P1
uid \fICLIENT_USR_NAME\fP=\fISERVER_USR_NAME
.P2
where \fICLIENT_USR_NAME\fP is the name of a user on the client machine and
\fISERVER_USR_NAME\fP is the name of a user on the server machine to which the client
is mapped.  \fISERVER_USR_NAME\fP
may be left out signifying the client is not to be mapped (i.e. disallowed access.)
.PP
The
.CW gid
directive specifies how to map a given group:
.P1
gid \fICLIENT_GRP_NAME\fP=\fISERVER_GRP_NAME
.P2
where \fICLIENT_GRP_NAME\fP is the name of a group on the client machine and
\fISERVER_GRP_NAME\fP
is the name of a group on the server machine to which the client is mapped.
\fISERVER_GRP_NAME\fP
may be left out signifying the client is not to be mapped (i.e. disallowed access.)
.PP
The
.CW param
directive conveys specific parameters to the server:
.P1
param \fINAME\fP=\fIVALUE
.P2
The following parameters
.I NAME ) (
are supported:
.IP \f(CWotherok\fP 10
this may take on a value of either 0 or 1.  A value of 1 gives read
permissions to client ids that are not mapped and a value of 0 denies access.  In either
case, client ids that are not mapped are denied write access.
.IP \f(CWroot\fP
the value for this parameter is the pathname of the root from
which the server will execute.
.NH 1 
Acknowledgements
.PP
Peter Weinberger originally wrote the network file system.
More recently, Norman
Wilson has fixed bugs and rewritten \fIzarf\fP and \fIsetup\fP.
He greatly improved the
portability of \fIzarf\fP.  Thanks to Peter Honeyman, Dennis Ritchie, and Norman
Wilson for reviewing this paper.
.NH 1 
References
.LP
|reference_placement

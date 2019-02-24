.so ../ADM/mac
.XX uucp 563 "Uucp Administration"
.ds ut \(<-\h'-3p'\v'-3p'\(sp\v'+3p'
." constant width bold, revert to previous font for second arg, if any
."
.de cB
\&\f(CB\\$1\fP\\$2
..
."
." constant width, revert to previous font for second arg, if any
."
.de cW
\&\f(CW\\$1\fP\\$2
..
."
." constant width in quotes, revert to previous font for second arg, if any
."
.de qC
\&``\f(CW\\$1\fP''\\$2
..
."
." how do you spell UUCP?
."
.ds uU \&\fIuucp\fP
.ds uU \&\fIUucp\fP
."
." how do you spell ASCII?
."
.ds aS \&\fR\s-1ASCII\s0\fP
."
."
." Command name in text, revert to previous font for second arg, if any
."
.de cN
\&\f2\\$1\fP\\$2
..
."
." Option letter in text, revert to previous font for second arg, if any
."
.de oP
\&\f(CW\\$1\fP\\$2
..
."
." File name in text, revert to previous font for second arg, if any
."
.de fN
\&\f(CW\s-1\\$1\s0\fP\\$2
..
.de FG
.ce
\fBFigure\ \\$1.\fP \\$2
..
.de TB
.ce
\fBTable\ \\$1.\fP \\$2
..
.nr dP 2
.nr dV 3p
.TL
Uucp Administration
.AU
David A. Nowitz
.AI
.MH
.AB
This describes the setup and administration of the \*(uU system
shipped with System V
(also known as Honey Danber).
.AE
.2C
.NH 1 
Introduction
.PP
This document describes the administration of the \*(uU system.
Administrators should be familiar with the
manual pages for each of the \*(uU related commands.
.NH 2 
Extent of the Network
.PP
Some basic decisions about
access
to processors in the network must be made
before attempting to set up the configuration files.
If an administrator has control over only one processor
and an existing
network is being joined,
then the administrator must decide what level of access should
be granted to other systems.
The other members of the network must make similar decisions for the new system.
The
.UX
system's
.I password
mechanism is
used to grant access to other systems.
The file
.fN /usr/lib/uucp/Permissions
provides various permissions and restrictions for file system access
and command execution from remote machines,
and the file
.fN /usr/lib/uucp/Systems
on the local processor determines the
systems on the network that can be reached directly.
.PP
When setting up more than one processor,
the administrator has control of a larger portion
of the network and can make more decisions about the
setup.
For example, the network can be set up as a
private
network
where only those machines under the direct control of the administrator
can access each other.
Granting
no
access to machines outside the network can be done if security
is paramount;
however, this is usually impractical.
Limited access can be granted to outside machines by each
of the systems
on the
private
network.
Alternatively, access to/from the outside
world can be confined to only one processor.
This is frequently done to minimize the effort in keeping
access information (passwords, phone numbers, etc.)
updated and to localize the security holes
for the private network.
.NH 2 
Hardware
.PP
There are several networks that are supported
with internal interface routines:
.IP 1. 3
Direct connection using a null modem.
.IP 2.
Connection over the Direct Distance Dialing (DDD) network
using various dialers (e.g. AT&T, Hayes, Ventel).
.IP 3.
Connection using a DATAKIT\*(tm VCS.
.IP 4.
UNET\(em3COM Ethernet\*(tm network.
.IP 5.
TCP
.IP 6.
Sytek
.IP 7.
AT&T Transport Layer Interface networks
.PP
Networks that require only a single line, asynchronous connection can
be set up using the 
.fN Dialers
file.
.1C
.KF
.TS
center ;
|c| c|
|lfCW|  l| .
_
program	description
=
uucp	File transfer command
uux	Remote execution command
uucico	The \*(uU file transfer program
uustat	Network status command
uusched	File transfer daemon scheduler
uucheck	\*(uU system files, directories, and Permissions file checker
uucleanup	Spool directories cleanup program
uuxqt	Remote execution program
uugetty	T{
.fi
.ll 4i
\f(CWgetty\fP that can be used for lines/modems that are to
work as dialin and dialout ports
T}
uulog	Shell program for looking at the log files
Uutry	T{
.fi
.ll 4i
Shell program used to try a remote
system with debugging options
T}
uudemon.cleanup	T{
.fi
.ll 4i
Shell program started by
.cN cron
to clean up \*(uU directories.
T}
uudemon.hour	T{
.fi
.ll 4i
Shell program, started by
.cN cron
used to start the file transfer scheduler and
the command execution program
T}
uudemon.admin	T{
.fi
.ll 4i
Shell program, started by
.cN cron
that sends data to the uucp administrator
giving information about the status of the \*(uU system
T}
uudemon.poll	T{
.fi
.ll 4i
Shell program, started by
.cN cron
is used for periodic polling of remote systems
T}
_
.TE
.SP 1
.......
.ce
.TB 1 "Uucp System Programs"
.SP 1
.KE
.KF bottom
.PS
scale=100
define m0 |
[ box invis ht 24 wid 80 with .sw at 0,0
arc from 0,18 to 80,18 at 40,58
] |

define m1 |
[ box invis ht 106 wid 80 with .sw at 0,0
ellipse ht 48 wid 80 at 40,82
m0 with .nw at 0,24
line  from 0,82 to 0,18 
line  from 80,82 to 80,18 
] |

define m2 |
[ box invis ht 48 wid 88 with .sw at 0,0
box ht 48 wid 88 with .nw at 0,48 
"\fR\s18\&uucico\f1\s0" at 48,19
] |

define m3 |
[ box invis ht 48 wid 88 with .sw at 0,0
box ht 48 wid 88 with .nw at 0,48 
"\fR\s18\&uuxqt\f1\s0" at 44,19
] |

define m4 |
[ box invis ht 48 wid 88 with .sw at 0,0
box ht 48 wid 88 with .nw at 0,48 
"\fR\s18\&uux\f1\s0" at 40,23
] |

define m5 |
[ box invis ht 48 wid 88 with .sw at 0,0
"\fR\s18\&uucp\f1\s0" at 44,23
box ht 48 wid 88 with .nw at 0,48 
] |

box invis ht 240 wid 504 with .sw at 0,0
box ht 240 wid 504 with .nw at 0,240 
line -> from 256,128 to 328,72 
line <-> from 256,144 to 296,184 
"\fR\s12\&file transfer\f1\s0" at 444,133
line -> from 112,88 to 176,120 
line -> from 112,176 to 176,136 
line -> from 360,136 to 448,112 
line  from 392,136 to 360,136 
line  from 336,160 to 392,136 
"\fR\s18\&spool\f1\s0" at 216,115
m5 with .nw at 24,200
m4 with .nw at 24,112
m1 with .nw at 176,192
m3 with .nw at 312,72
m2 with .nw at 296,208
.PE
.FG 1 "\*(UU transfer and remote execution programs"
.KE
.2C
.NH 1 
Uucp Software
.PP
Table 1 is a summary of the \*(uU System Program.
Figure 1
illustrates the programs that are
used to process user requests.
The
.cN uucp
and
.cN uux
commands queue users' requests:
.cN uucp
for file transfers,
and
.cN uux
for requests for remote execution.
(\c
.cN Uux
is most often invoked from within the
.cN mail
command.)
.cN Uucico
takes care of the file transfers between machines.
Remote execution jobs are transferred by
.cN uucico
and executed by the
.cN uuxqt
program.
.1C
.KF
.PS
scale=100
define t169 |
[ box invis ht 38 wid 66 with .sw at 0,0
"\s12\&Scan for\f1\s0" at 33,29
"\fR\s12\&Work\f1\s0" at 33,10
] |

define t166 |
[ box invis ht 38 wid 130 with .sw at 0,0
"\fR\s12\&Connect to\f1\s0" at 65,29
"\fR\s12\&Remote Machine\f1\s0" at 65,10
] |

define t170 |
[ box invis ht 38 wid 62 with .sw at 0,0
"\fR\s12\&Start\f1\s0" at 31,29
"\fR\s12\&Protocol\f1\s0" at 31,10
] |

define t171 |
[ box invis ht 38 wid 66 with .sw at 0,0
"\fR\s12\&File\f1\s0" at 33,29
"\fR\s12\&Transfer\f1\s0" at 33,10
] |

define t184 |
[ box invis ht 56 wid 112 with .sw at 0,0
"\fR\s12\&establish\f1\s0" at 56,47
"\fR\s12\&communication\f1\s0" at 56,28
"\fR\s12\&channel\f1\s0" at 56,9
] |

define t170 |
[ box invis ht 38 wid 62 with .sw at 0,0
"\fR\s12\&Start\f1\s0" at 31,29
"\fR\s12\&Protocol\f1\s0" at 31,10
] |

define t171 |
[ box invis ht 38 wid 66 with .sw at 0,0
"\fR\s12\&File\f1\s0" at 33,29
"\fR\s12\&Transfer\f1\s0" at 33,10
] |

define t169 |
[ box invis ht 38 wid 66 with .sw at 0,0
"\fR\s12\&Scan for\f1\s0" at 33,29
"\fR\s12\&Work\f1\s0" at 33,10
] |

define m0 |
[ box invis ht 48 wid 160 with .sw at 0,0
box ht 48 wid 160 with .nw at 0,48 
t169 with .nw at 47,40
] |

define m1 |
[ box invis ht 48 wid 160 with .sw at 0,0
box ht 48 wid 160 with .nw at 0,48 
t166 with .nw at 15,40
] |

define m2 |
[ box invis ht 48 wid 160 with .sw at 0,0
box ht 48 wid 160 with .nw at 0,48 
t170 with .nw at 45,40
] |

define m3 |
[ box invis ht 48 wid 160 with .sw at 0,0
t171 with .nw at 47,40
box ht 48 wid 160 with .nw at 0,48 
] |

define m4 |
[ box invis ht 240 wid 160 with .sw at 0,0
m0 with .nw at 0,240
m1 with .nw at 0,176
m2 with .nw at 0,112
m3 with .nw at 0,48
] |

define m5 |
[ box invis ht 240 wid 160 with .sw at 0,0
box ht 48 wid 160 with .nw at 0,176 
m2 with .nw at 0,112
m3 with .nw at 0,48
m0 with .nw at 0,240
] |

box invis ht 368 wid 576 with .sw at 0,0
box ht 368 wid 576 with .nw at 0,368 
box ht 272 wid 192 with .nw at 32,336 dashed
m4 with .nw at 48,320
t184 with .nw at 232,289
line  from 240,224 to 352,208 
line  from 256,232 to 240,224 
line  from 208,240 to 256,232 
line <-> from 208,104 to 368,104 
"\fR\s14\&UUCICO\f1\s0" at 132,31
"\fR\s14\&UUCICO\f1\s0" at 448,31
box ht 272 wid 192 with .nw at 352,336 dashed
m5 with .nw at 368,320
.PE
.FG 2 "\*(uU file transfer functions"
.KE
.2C
.PP
Figure 2
illustrates the structure of
.cN uucico ,
the program that performs the communication with remote systems.
.NH 1 
\f(CB/etc/passwd\fP
.PP
To allow remote systems to call
the local system, password entries must be made
for any \*(uU
logins.
An example follows.
(In this and other examples to come,
the text must be on one physical line but is too long
for our column width.
A line break marked by \*(cr is only for readability.)
.P1 0
nuucp:zaaAAaaa:6:1:Admin:   \*(cr
	/usr/spool/uucppublic:   \*(cr
	/usr/lib/uucp/uucico
.P2
Note that the
.cN uucico
program is used for the
shell,
and the \*(uU public directory
is used as the working directory.
.PP
There must also be an entry in the
.fN /etc/passwd
file for a \*(uU
administrative
login.
This login is the owner of all the \*(uU
object and spooled data files and is usually ``uucp''.
For example, the following is a entry in
.fN /etc/passwd
for this administrative login:
.P1 0
uucp:zAvLCKp:5:1:UUCP.Admin:/usr/lib/uucp:
.P2
........
.NH 1 
\f(CB/etc/inittab\fP
.PP
After the ports are set up, entries should be put into
.fN /etc/inittab
corresponding to the devices.
Ports can
configured for outgoing traffic only, in which case, the action field of
the
.fN /etc/inittab
entry should be ``off''.
.P1 0
10:off:/etc/getty tty10 1200
	# outgoing ACU entry
.P2
.PP
Bi-directional traffic can be set up using the
.CW /usr/lib/uucp/uugetty
command.
.P1 0
10:respawn:/usr/lib/uucp/uugetty -r -t60 \*(cr
	tty10 1200H # bidirectional
.P2
Note here that in the speed field there is an H after the speed.
This
corresponds to the 1200H
.fN /etc/gettydefs
entry that does not
set HUPCL in the initial flags for the line.
If the HUPCL flag was set, some modems would drop DTR
causing the
.cN uugetty
to exit.
Here are two typical
.fN /etc/gettydefs
entries for the 1200H and 1200:
.P1 0
1200H# B1200 # B1200 SANE IXANY TAB3 \*(cr
	HUPCL #login: #300H
1200# B1200 HUPCL # B1200 SANE IXANY \*(cr
	TAB3 #login: #300
.P2
In addition, the
.oP \-r
option is needed on the
.cN uugetty
command to inhibit the echo of the
.cW login
prompt until a character is read;
this is because many modems bring up
DTR as soon as the
.cN open
is executed and the
.cN uugetty
would not be able to determine
whether the calling incoming or outgoing.
Also, if there is a direct line with
.cN uugetty 's
running
on both sides, the
.cW login
prompt would be read by both sides as
a user attempting to login and would get into a loop.
.NH 1 
Setting Up ACU Devices
.PP
Create a device in the
.fN /dev
directory corresponding to the
port of the ACU.
.P1 0
mknod /dev/tty10 c 1 6
.P2
will create the ACU device on
.fN tty10
that has major and minor
device numbers 1 and 6.
This will correspond to the following entry
in
.fN /usr/lib/uucp/Devices
.P1 0
ACU tty10 - 1200 att4024
.P2
for an AT&T 4024 modem on tty10.
.PP
Next, the access modes and ownership of the device should be changed.
.P1 0
chown uucp /dev/tty10
chmod 644 /dev/tty10
.P2
.PP
For old Western Electric 212/801 type modems,
an additional device is required\(emthe 801
device.
Create a device for the 212 modem line.
.P1 0
mknod /dev/cul1 c 1 6
chown uucp /dev/cul1
chmod 644 /dev/cul1
.P2
Next, create a separate device for the 801 dialer.
.P1 0
mknod /dev/cua1 c 7 1
chown uucp /dev/cul1
chmod 644 /dev/cua1
.P2
The corresponding
.fN /usr/lib/uucp/Devices
entry for this ACU would be
.P1 0
ACU cul1 cua1 1200 801
.P2
Note that for 801/212 type modems, only one speed is allowed;
it can not be configured to work at both speeds when calling out.
.NH 1 
\f(CBDevices\fP File
.PP
The file
.fN /usr/lib/uucp/Devices
contains the device information needed
for calling other systems.
The format of the file is
.ce
\fICaller Line Line2 Class Token-Pairs\fP
.PP
The
.I Token-Pairs
field contains dialer names and
arguments that are passed to these dialer
routines.
Dialers can either be built-in or defined in the
.fN /usr/lib/uucp/Dialers
file.
.PP
.I Caller :
The keywords available in this field are shown below.
.KF
.TS
center;
|c| c|
|lFCW| lw(1.7i)|.
_
keyword	meaning
=
Direct	T{
hard-wired line used by cu for direct connections
T}
ACU	T{
make the connection through an autodialer
T}
\fINETWORK\fP	T{
.fi
\fRmake the connection through a switch or network
such as
Sytek, TCP, DK
T}
\fISYSTEM-NAME\fP	T{
.fi
for hard-wired connections to a particular system where
\fISYSTEM-NAME\fP is replaced by the name of the system
T}
\fIOTHER\fP	T{
.fi
other names that you define
(e.g.
.cW DKACU
can be set up as an
.cW ACU
on a Datakit network).
T}
_
.TE
.KE
.PP
.I Line :
This data transmission device
(.e.g.
.fN tty10
for device
.fN /dev/tty10 ).
.PP
.I Line2 :
If the ACU keyword is specified and the device type is 801,
this field contains the
device name of the 801 dialing device ACU.
Otherwise, the field is ignored; however, a place-holder must be
used in this field (use
.qC -
character for the place-holder).
.PP
.I Class :
For ACU, this can be just the speed,
or it can contain a letter and speed (e.g. D1200, C1200
to differentiate between classes of dialers: Centrex, Dimension.
These names can be used in the
.fN Systems
file to select specific types of
.cW ACU s
for connections to some systems).
Some devices can be used at any speed, so the keyword
.qC Any
is used\(emthis line will match any speed requested in Systems.
Note:
If this field is
.qC Any
and the Systems class field is
.qC Any
then the speed is taken from the default set in
.fN parms.h
by the
.CW DEFAULT_BAUDRATE
constant.
.PP
.I Token-Pairs :
The rest of the line contains pairs of
.I dialers
and
.I tokens .
Each pair represents a
.I dialer
function and an argument to
pass to that function.
.KF
.TS
center;
|c| c|
|l| lw(2i)|.
_
dialer	function
=
801	T{
Western Electric 801/212 or 801/103 combinations
T}
212	T{
Western Electric 801/212 or 801/103 combinations
T}
Dialout	T{
Dialing using the dialout(3) routine
T}
TCP	TCP Network
Unetserver	3COM Ethernet\*(tm network
DK	T{
Datakit with Multiplex Interface
T}
Sytek	Sytek Network
TLI	T{
AT&T Transport Layer Network without Streams
T}
TLIS	T{
AT&T Transport Layer Network with Streams
T}
_
.TE
.TB 2 "Built-in Dialers"
.KE
.PP
Table 2 provides the names of the built-in
.I dialer
routines.
All other
.I dialer
functions are defined in the
.fN /usr/lib/uucp/Dialers
file.
.PP
The second field in the
.cW Token-pair
is the argument to be passed to the
.I dialer
function\(emthis is typically the destination phone number.
Two special tokens are used to represent the phone number
field from the
.fN Systems
file:
.qC \eD
is the phone number field as it appears in the
.fN Systems
file while
.qC \eT
is the phone number after being processed using the
.fN Dialcodes
file to expand the phone number prefix.
If the last token on the
.fN Devices
file line is the phone number,
it can be omitted;
.qC \eD
is assumed, since routines defined in the
.fN Dialers
file
can use
.qC \eT
to do the expansion
and all internal routines will
do the expansion.
.PP
For cases where dialout modems are on a networks,
the
.cW Caller
will be
.cW ACU
and the
.cW Token-Pairs
will be the token given to the
network routine to attach to the dialout modem.
These two fields will be followed by another
.cW Token-Pair ,
the dialer routine name (e.g.
.cW ventel )
and the phone number
(e.g. 
.qC \eD
or 
.qC \eT ).
If the last token on the line is
.qC \eD
it may be omitted.
.PP
The following examples
illustrate various types of connections:
.P1 0
ACU     cul0  cua0  1200  801
ACU     cul1  cua1   300  801
ACU     vn0   -     1200  ventel
ACU     vn0   -      300  ventel
ACU     vd0   -     1200  vadic
ACU     vd0   -    V1200  vadic
ACU     at1   -     2400  att4024
ACU     at1   -     1200  att4024
Direct  at1   -     1200  att4024
.P2
.PP
.fN /dev/cul0
and
.fN /dev/cul1
are 212 modems (\c
.fN /dev/cul1
may be a 103 type),
with 801s at
.fN /dev/cua0
and
.fN /dev/cua1
respectively;
.fN /dev/vn0
is hooked to a ventel and can be used
at 1200 or 300 baud, and
.fN /dev/vd0
is hooked to a vadic.
There is also a att4024 dialer on
.fN /dev/at1 ;
it can be used at either 2400 or 1200 baud.
The
.I Direct
line is present for
.cN cu
to have direct access to the modem on
.fN /dev/at1 .
.PP
.P1 0
ACU  culd0  -  Any  datakit dial att4024
ACU  culd1  -  Any  datakit dial att4024
ACU  -      0  Any  DK      dial.\eT
.P2
.PP
There are two RS-232 Datakit ports available and at least
two
AT&T 4024 modems attached to the network.
The
.I datakit
.fN Dialers
line will be accessed with the argument
.cW dial .
The
.I att4024
.fN Dialers
line will be used with the
telephone number
from the
.fN Systems
file after it is processed using the
.fN Dialcodes
file prefix processing.
The last line in the above example uses the
internal, Datakit multiplexed interface routine.
Here, the
telephone number must be translated,
using the
.qW \eT
token, before
being passed to the
.cW DK
routine.
.PP
.P1 0
raven   ttyab  -  9600  direct
Direct  ttyab  -  9600  direct
.P2
.PP
There is a direct line\(em\c
.fN ttyab
has a null-modem connection to
system
.I raven .
The
.I Direct
line is for
.cN cu
access to the line.
.NH 1 
\f(CBDialers\fP File
.PP
Each line in the
.fN Dialers
file
is used to specify
the handshaking that should occur before it
is made available for user data.
Each line contains the following fields:
.ce
\fIDialer Translate Handshake\fP
.PP
.I Dialer :
Identifies the dialer, and is used
to match the first token of the
.cW Token-pairs
field in the
.fN Devices
file
for those dialers that are not built-in functions
specified in Table 2.
.PP
.I Translate :
Specifies the ``wait-for-dialtone'' and ``pause'' characters
that are used for the particular dialer.
(For the phone number in the
.fN Systems
file, the
.CW =
is used for
``wait-for-dialtone'' and
.CW -
is used for
``pause''.)
The string contains four characters, two pairs.
The first pair starts with
.CW =
and specifies the ``wait-for-dialtone'' character;
the second pair starts with
.CW -
and specifies the 
``pause'' character.
The second character of each pair is the corresponding
character for the dialer.
If no translation is required, a pair of double quotes
is used for a place-holder.
.PP
.I Handshaking :
The sequence of \*(aS strings
that are transmitted and expected,
and is used to dial a phone number using an \*(aS dialer
(such as an AT&T 4024) or connect via a dataswitch to
another system on the dataswitch.
(See the ``Login-script'' part of the
``Systems'' section below for an explanation of expect-send fields.)
Figure 3 shows some sample lines.
.1C
.KF
.P1 20n
att4024 =+-,   "" atzod,o12=y,o4=n\er\ec \e006 atT\eT\er\ec ed
ventel  =&-%   "" \er\ep\er\ec $ K call: \eT%% Online!
direct
datakit ""     "" \er TION: \eD
hayes   =,-,   "" \edAT\er\ec OK\er \eEATDT\eT\er\ec CONNECT
.P2
.FG 3 "Sample Dialer Lines"
.KE
.2C
.PP
The escape characters, those beginning with
.qC \e ,
have the same
meaning as specified in the ``Login-script'' part of the
``Systems'' file section below.
In addition to those mentioned in that section,
the
.qC \eT
and
.qC \eD
are used to substitute the phone number
string passed to the dialing function.
.NH 1 
\f(CBSystems\fP File
.PP
Lines in the
.fN /usr/lib/uucp/Systems
file represent systems
that can be called by the local \*(uU programs.
More than one line may be present for a particular system;
the additional lines represent alternative
communication paths that will be tried in sequential order.
In addition, remote systems that don't appear in the
.fN Systems
file can be prevented from communicating.
(This is the default configuration;
it can be modified by changing
.fN parms.h
before
compilation.)
Each line contains the following fields:
.ce
\fISystem Time Caller Class Phone Login-script\fP
.PP
.I System :
Name of the remote system.
.PP
.I Time :
This is a string that indicates the days-of-week
and times-of-day when the system should
be called
(e.g., 
.cW MoTuTh0800\-1730 ).
.PP
The day portion may be a list containing
\f(CWSu\fP,
\f(CWMo\fP,
\f(CWTu\fP,
\f(CWWe\fP,
\f(CWTh\fP,
\f(CWFr\fP,
\f(CWSa\fP;
or it may be
\f(CWWk\fP
for any week-day or
\f(CWAny\fP
for any day.
The time should be a range of times (e.g., 
.cW 0800\-1230 ).
If no time portion is specified, any time
of day is assumed to be allowed for the call.
Note that a time range that spans 0000 is permitted;
\f(CW0800-0600\fP means all times are allowed \fIexcept\fP
times between 6 and 8 am.
Multiple time fields may be include using a
.qC ,
separator
(e.g.
.cW "Wk1800-0600,Sa,Su" ).
An optional subfield is available to specify the minimum time (minutes)
before a retry following a failed attempt.
(Note that if this subfield is used, it will override the normal
exponential backoff algorithm for retry upon failure.)
This subfield is separated by a
.qC ;
character.
.PP
.I Caller :
These are names that appear in the
first field of the
.fN Devices
file.
(e.g.
.cW ACU ,
.cWDK ,
.cW Sytek ,
.cWTCP ).
.PP
.I Class :
This is usually the line speed for the call (e.g., 
.cW 300 ,
.cW 1200 ,
.cW Any ).
If the field is not used for a particular entry, a
.qC -
can be used
as the place-holder.
When the value is
.qC Any ,
it means match any speed found for the particular caller.
If both the
.fN Systems
and
.fN Devices
files value is
.qC Any ,
then the value is
taken from the
.cW DEFAULT_BAUDRATE
constant defined in
.fN parms.h .
.PP
.I Phone :
For autodialers, the phone number is made up of an optional
alphabetic abbreviation (dialing prefix) and a numeric part.
The abbreviation should be one that appears in the
.fN Dialcodes
file (e.g., 
.cW mh1212 , 
.cW boston555\-1212 ).
For direct connections, the phone field is ignored.
(A
.qC -
should be used as a place-holder).
.PP
For
.I NETWORK
access,
the phone field is the token the switch
or network routine needs to get to the
particular system\(emit is used by the caller functions specified
in the
.fN Devices
file.
.PP
.I Login-script :
The login information is given as a series of
fields and subfields in the format
.P1 0
[ expect  send ] .\|.\|.
.P2
where
.cW expect
is the string expected to be read and
.cW send
is the string to be sent when the
.cW expect
string is received.
Each
.cW expect
field may be made up of subfields
of the form
.P1 0
expect[\-send\-expect] .\|.\|.
.P2
where the
.cW send
is sent if the prior
.cW expect
is
.I not
successfully read
and the
.cW expect
following the
.cW send
is the next expected string.
(For example,
.cW "login--login"
will expect
.cW login ;
if it gets it, the program will go on to the next field;
if it does not get
.cW login ,
it will send
.I null
followed by a new line,
then expect
.cW login
again.)
If no characters are initially expected from the remote
machine, the string
\&``\f(CW""\fP''
(a
.I null
string) should be used in the
first expect field.
Note that all
.cW send
fields will be sent followed by a new-line unless
the
.cW send
string is terminated with a
.qC \ec .
.1C
.KF
.TS
center;
|c| c|
|lfCW| l|.
_
character	meaning
=
\eb	send a backspace character.
\ec	T{
.ll 4i
.fi
\fRif at the end of a string, suppress the new-line that
is normally sent,
Ignored otherwise
T}
\ed	delay two seconds before sending or reading more characters
\eE	turn on echo checking (for slow devices)
\ee	turn off echo checking
\eK	insert a BREAK
\eN	send a null character.
\en	send a new-line character.
\ep	insert a pause (approximately \(14\-\(12 second).
\er	send a carriage-return.
\es	send a space character.
\et	send a tab character.
\e\e	send a \e character.
EOT	T{
.fi
\fRsend an EOT character (EOT new-line is sent twice)
T}
BREAK	send a break character
\e\fIddd\fP	T{
.fi
collapse the octal digits (\fIddd\fP) into
a single character and send that character.
T}
_
.TE
.TB 3 "Special Strings"
.KE
.2C
.PP
Table 3 gives the special characters that are used in the
.cW Login-script
field.
.PP
A typical entry in the
.fN Systems
file for a system that is reached by dialing out on
a modem is be
.P1 0
sys Any ACU 1200 mh7654 login--login uucp \*(cr
	ssword: word
.P2
A
.fN Systems
file entry for a direct connection would be
.P1 0
hawk Any hawk 9600 - login--login uucp \*(cr
	ssword: word
.P2
The corresponding
.fN Device
file entry would be
.P1 0
hawk ttyhh - 9600 direct
.P2
Note that the
.I expect
algorithm matches all or part of the input
string as illustrated in the password field above.
.NH 1 
\f(CBDialcodes\fP File
.PP
The
.fN Dialcodes
file contains the dial-code abbreviations used
in the
.fN Systems
file (e.g., py, mh, boston).
The entry format is
.P1 0
abb   dial-sequence
.P2
where
.cW abb
is the abbreviation and
.cW "dial-sequence"
is the dial sequence to call that location.
.PP
The line
.P1 0
mh  132-
.P2
would be set up so that entry
.cW mh7777
would
send
.cW 132-7777
to the dial unit.
......
.NH 1 
\f(CBSysfiles\fB \(em alternate \f(CBSystems, Devices, Dialers\fR files
.PP
It is sometimes useful to have more than one
.fN Systems ,
.fN Devices ,
and
.fN Dialers
files.
The
.fN Systems
file can be split into smaller, more manageable files\(emone
containing local and one global data.
In addition,
.cN uucico
and
.cN cu
may need different
.fN Systems
files.
The
.fN Sysfiles
file provides a mechanism for specifying different
resource files.
The general form of the file is name-value pairs similar to the
.fN Permissions
file.
.cW "File-lists"
are colon separated lists of file names
that are in the
.fN /usr/lib/uucp
directory.
.P1 0
service=<service name> \e
	systems=<systems file-list> \e
	devices=<devices file-list> \e
	dialers=<dialers file-list>
.P2
Each entry is a single logical line where the trailing ``\e''
character is used to indicate continuation.
.P1 0
service=uucico \e
	systems=Systems.cico:Systems \e
	dialers=Dialers.cico:Dialers

service=cu \e
	systems=Systems.cu:Systems \e
	dialers=Dialers.cu:Dialers
.P2
.PP
In the example,
.cN uucico
will use the
.fN Systems.cico
file
followed by the
.fN Systems
file for remote systems information.
It also specifies two
.fN Dialers
files.
The
.cN cu
program will use
.fN Systems.cu
and the
.fN Systems
file.
It also has two
.fN Dialers
files.
Any options not specified in
.fN Sysfiles
will have the default of
.fN /usr/lib/uucp/Systems ,
.fN /usr/lib/uucp/Devices ", and"
.fN /usr/lib/uucp/Dialers .
.NH 1 
\f(CBPermissions\fP File
.PP
The
.fN /usr/lib/uucp/Permissions
file specifies the permission that remote sites
have with respect to login, file access, and command
execution.
Options provide for restricting the ability to request files and
the ability to receive files queued by the local site.
In addition, an option is available to specify the commands that a
remote site can execute on the local system.
.PP
The next sub-section gives three
.fN Permissions
file entries.
Taken together, they provide all the entries needed by most
sites running the \*(uU system.
The remainder of the section gives a detailed explanation of the
options.
.NH 2 
Starting Examples
.PP
The first example is the model of an entry for the public login on
your system;
it represents the most restrictive access to your system.
.P1 0
LOGNAME=nuucp
.P2
states that login
.cW nuucp
has all the default permissions/restrictions:
.IP \(bu
The remote site can send files exclusively to the
.I "uucp public"
directory.
(usually
.fN /usr/spool/uucppublic )
.IP \(bu
The remote site can \fInot\fP request to receive any files.
.IP \(bu
\fINo\fP files that are queued for the remote site will be transferred
during the present session.
.IP \(bu
The only commands that can be executed are the defaults\(emusually
.cN rmail .
.PP
This entry alone is sufficient to start communications with remote sites,
permitting files to be transferred to the
.I "uucp public"
directory by request of
the remote site.
.PP
The next example is for remote sites that log in, but have fewer restrictions.
The login and password corresponding to this entry should \fBnot\fP be distributed
to the general public;
it is usually reserved for closely coupled systems where the
.fN Systems
file
information can be tightly controlled.
.P1 0
LOGNAME=uucpz REQUEST=yes SENDFILES=yes \e
	READ=/  WRITE=/
.P2
This entry provides the following permissions
when a remote site logs in as
.cW uucpz :
.IP \(bu
Files can be requested from the local site (\c
.cW REQUEST
option).
.IP \(bu
Files can be transferred to any directory or any file
that is writable by user
.I other \(em\c
that is
a file/directory that is writable by a local user with
neither owner nor group permissions.
(Option
.cW WRITE
controls this permission.)
.IP \(bu
Any files readable by user
.I other
can be requested.
(Option
.cW READ
controls this permission.)
.IP \(bu
Any requests queued by the local site will be executed during
the conversation;
these are requests by local users that are destined for the
site that is calling in.
(\c
.cW SENDFILES
option).
.IP \(bu
The commands sent for execution on the local system by the remote
must be in the default set (usually
.cN rmail ).
.PP
Thus far, the examples showed entries that referred to remote sites
when they log in to the local system.
This example is an entry used when calling a remote site.
.P1 0
MACHINE=mhtsa:mhtsb:mhtsc:pwbcc \e
	REQUEST=yes READ=/ WRITE=/
.P2
When calling any of the systems given in the
.cW MACHINE
list,
the following permissions prevail:
.IP \(bu
The remote site can both request and send files (\c
.cW REQUEST
option).
.IP \(bu
The source or destination of the files on the local system can
be anywhere in the file system.
.IP \(bu
The only commands that will be executed for the remote site
are those in the default list.
.PP
Any site that is called that does not have its name in a
.cW MACHINE
entry will have the default permissions
with the exception that files queued for that site will be sent
(the
.cW SENDFILES
option only has meaning in a
.cW LOGNAME
entry).
.PP
The
three examples in this section form a model
.fN Permissions
file that
can be used by a system with a public login for remote sites
and several closely coupled machines.
.NH 2 
Basics
.PP
Each
.I entry
is a logical line;
physical lines are terminated with a
.qC \e
to indicate continuation.
Entries are made up of
.I "white space"
delimited
.I options .
Each option is a
name/value pair;
these are constructed by an option name followed by an
.qC =
followed by
the value.
Note that
white space is \fBnot\fP allowed within a name/value pair.
.PP
Comment lines begin with
.qC # ;
they occupy the entire line up to a newline character.
Blank lines are ignored (even within multi line entries).
.PP
There are two types of entries:
.CW LOGNAME
entries specify permissions for remote sites
when they log in to the local machine, and
.CW MACHINE
entries
specify permissions for sites that the local machine calls.
.PP
LOGNAME entries will contain a
.cW LOGNAME
option.
MACHINE entries will contain a
.cW MACHINE
option somewhere in the entry.
.NH 2 
Some Rules
.PP
All login ids used by remote sites to login for uucp
\fImust\fP appear in one and only one
LOGNAME entry.
.PP
Any site that is called that
\fIdoes not\fP appear in a
MACHINE entry
will have the following
default permissions/restrictions:
.IP \(em 3n
Local send and receive requests will be executed.
.IP \(em
The remote can send files to the system's public uucp directory.
.IP \(em
The commands sent by the remote for execution on the local machine
must be in the default set\(emusually
.cN rmail
and
.cN rnews .
.1C
.KF
.TS
center;
|c| c| c| c|
|lFCW| lfR | lFCW| lFCW| .
_
option	meaning	values	default
=
MACHINE	remote machine name identification	\fImachine list\fP
LOGNAME	login used by a remote	\fIlogin ids.\fP
REQUEST	remote machine can request files	yes/no	no
READ	directories remote can request from	\fIdirectory list\fP
WRITE	directories remote can write into	\fIdirectory list\fP
SENDFILES	T{
.nf
send queued files when called by
remote
T}	yes/no	no
NOREAD	directories remote can \fBnot\fP request from	\fIdirectory list\fP
NOWRITE	directories remote can \fBnot\fP write into	\fIdirectory list\fP
CALLBACK	call back remote	yes/no	no
COMMANDS	T{
.nf
list of allowed commands for execution
by \f(CWuuxqt\fP
T}	T{
ALL or
.nf
\fIcommand list\fP
T}
PUBDIR	\*(uU public directory	\fIdirectory\fP	T{
.nf
\fIlogin
directory\fP
T}
MYNAME	local machine name	\fImachine name\fP	T{
.nf
\fIlocal
name\fP
T}
VALIDATE	verify remote system name vs. login id.	\fImachine list\fP 
_
.TE
.TB 4 "Permissions File Options"
.KE
.2C
.NH 2 
Options
.PP
This section give the details of each option, specifying how they are
used and their default values.
Table 4 is a summary of the options available for the
.fN Permissions
file.
.NH 2 
MACHINE and LOGNAME
.PP
The MACHINE entry specifies the permissions that take effect when
a remote site is
.I called .
.P1 0
MACHINE=mhtsa
.P2
is the start of an entry that will specify the permissions associated
with machine
.cW mhtsa .
The MACHINE option can contain a list of different system names,
each separated by a
.qC :
character.
For example,
.P1 0
MACHINE=mhtsa:mhtsb:mhtsc
.P2
.ne5
.PP
The
.cW LOGNAME
entry specifies a list of login ids of remote sites
that are able to log into the local system.
The option contains one or more names separated by a
.qC :
character.
For example,
.P1 0
LOGNAME=nuucp
LOGNAME=uucpz:uucyz
.P2
Names that appear in
.cW LOGNAME
options can appear in only one such entry.
.NH 2 
REQUEST
.PP
The
.cW REQUEST
option can appear in either a
LOGNAME
entry or a
MACHINE
entry
and specifies whether the remote can make requests to receive local
files.
.P1 0
REQUEST=yes
.P2
specifies that the remote \fBcan\fP request files.
.P1 0
REQUEST=no
.P2
specifies that the remote \fBcan not\fP request files.
The latter is the default\(emit will be used if the
.cW REQUEST
option is not specified.
.NH 2 
SENDFILES
.PP
.cW  SENDFILES
specifies whether the \fIcalled\fP site will execute locally
queued requests during the conversation.
The default is that locally queued
requests will not be executed during the call;
they will be done only when the remote is \fIcalled\fP by the local system.
(I don't care who you say you are, I'll send you queued files when
I call you.)
.PP
Clearly, this option is only significant in LOGNAME entries, since
MACHINE entries apply when calls are made out to remote sites.
The option is ignored when a MACHINE entry is being used.
.P1 0
SENDFILES=yes
.P2
specifies that the locally queued requests will be executed when
the remote site logs in as one of the names in this entry's
.cW LOGNAME
option.
.PP
The default setting for the
.cW SENDFILE
option is
.P1 0
SENDFILES=call
.P2
meaning that queued files will be sent only when the local machine
makes the connection.
This
.cW call
value can be specified for documentation purposes.
.NH 2 
READ and WRITE
.PP
The default for both the
.cW READ
and
.cW WRITE
options
is the
\*(uU
public directory.
The options
.P1 0
READ=/usr/spool/uucppublic \e
	WRITE=/usr/spool/uucppublic
.P2
are the defaults and may be specified for documentation purposes.
The options
.P1 0
READ=/ WRITE=/
.P2
specify permission to access any file that can be accessed by a local
user with
.I "other"
permissions.
.PP
The value of these entries is a colon separated list of path
names.
The
.cW READ
option is for requesting files and the
.cW WRITE
option for
depositing files.
Any file coming in or going out must
match a prefix in
.cW READ
or
.cW WRITE
option.
.PP
To grant permission to deposit files in
.fN /usr/news ,
as well as the public directory, specify
.P1 0
WRITE=/usr/spool/uucppublic:/usr/news
.P2
in the entry.
.PP
\fBIf the
.cW READ
or
.cW WRITE
option is specified, all the
path names must be specified;
they do not add to the default
list.\fP
.NH 2 
NOREAD and NOWRITE
.PP
There are two other options in the file access class,
.cW NOREAD
and
.cW NOWRITE.
These will rarely be used;
they specify exceptions to the
.cW READ
and
.cW WRITE
options or defaults.
.P1 0
READ=/ NOREAD=/etc \e
	WRITE=/usr/spool/uucppublic
.P2
This example would permit reading any file except those in the
.fN /etc
directory (and its sub directories\(emremember these are prefixes)
and writing only to the default
.I public
directory.
.cW NOWRITE
works the same way for sending files to the local system.
.NH 2 
CALLBACK
.PP
The
.cW CALLBACK
option is used in LOGNAME entries to specify that
no transaction will take place, but the calling system,
as established during handshake, will be called back.
.P1 0
CALLBACK=yes
.P2
specifies this action.
The default is
.P1 0
CALLBACK=no
.P2
The
.cW CALLBACK
option will rarely be used.
(Note that if two sites have this option set for each other, a conversation
will never get started.)
.NH 2 
COMMANDS
.QS
.ft B
WARNING!!
The
.cW COMMANDS
option can be hazardous to the security
of your system.
Use it with extreme care.
.ft R
.QE
.PP
The
.cW VALIDATE
option should be used in conjunction with the
.cW COMMANDS
option whenever potentially dangerous commands
like
.cN cat
and
.cN uucp
are specified.
Any command that reads or writes files is potentially
dangerous to local security when executed by the \*(uU
remote execution program (\c
.cN uuxqt ).
.PP
The
.cN uux
program will generate remote execution requests and queue
them to be transferred to the remote site.
Files and a command are sent to the target site.
The
.cW COMMANDS
option can be used
in MACHINE entries to specify the commands that a remote
machine can execute.
.ft R
.P1 0
COMMANDS=rmail:rnews
.P2
This line specifies the commands that can be executed by the
remote machine are either
.cN rmail
or
.cN rnews
exclusively.
(The default list is specified in the
.fN parms.h
header file
during compilation of \*(uU.
The defaults settings will be discussed later.)
The entry
.P1 0
MACHINE=owl:raven:hawk:dove \e
	COMMANDS=rmail:rnews:lp
.P2
overrides the
.cW COMMAND
default such that the command list
for machines owl, raven, hawk, and dove now consists of
.cN rmail ,
.cN rnews ,
and
.cN lp .
.PP
.ft B
If you don't trust a caller's identity, don't let that system
execute dangerous commands.
.ft
(If you can't trust a site,
make sure that the login and password it uses is restricted.)
.PP
.ft B
Giving a site an unrestricted login, with file access and remote execution
capability, is like giving anyone on that system a local login.
.ft
.NH 2 
VALIDATE
.PP
Use the
.cW VALIDATE
option in connection with the
.cW COMMANDS
option
when specifying dangerous commands.
It is used in LOGNAME entries to provide \fIsome\fP verification
of the caller's identity.
However, an important aspect of this validation is that the
login/password associated with this entry be protected.
If an outsider gets that information, the validation is not valid!
.P1 0
LOGNAME=uucpfriend VALIDATE=eagle:owl:hawk
.P2
This entry specifies that if a remote logs in and says that it is
any of the specified birds, it must have logged in as
.cW uucpfriend .
If an outsider gets the
.cW uucpfriend
login/password,
masquerading is trivial.
.PP
But what does this have to do with the
.cW COMMANDS
option,
which only appears in MACHINE entries?
A short answer is that it connects the MACHINE entry that has the
.cW COMMANDS
option with a protected login entry that appears in
a
.cW LOGNAME
option.
This connection is needed because the execution demon is not
running while the remote is logged in;
it is an asynchronous process with no knowledge of
what system sent the execution requests.
.PP
Therefore, the real question is, how does the local site know who originated the
execution files (\c
.qC X.
files sent by the
.cN uux
command on the remote site)?
.PP
Each remote site has its own
.I spool
directory, with write permission
only given to the \*(uU programs.
The execution files from the remote site are put in its
.I spool
directory.
Therefore, when the
.cN uuxqt
program runs, it can use the
.I spool
directory name to find the MACHINE entry in the
.fN Permissions
file and get the
.cW COMMANDS
list, or if the machine name does not
appear in the
.fN Permissions
file, the default list will be used.
.P1 0
MACHINE=mhtsa:mhtsb:mhtsc \e
	REQUEST=yes \e
	COMMANDS=ALL \e
	READ=/  WRITE=/

LOGNAME=uucpz \e
	VALIDATE=mhtsa:mhtsb:mhtsc \e
	REQUEST=yes   SENDFILES=yes \e
	READ=/  WRITE=/
.P2
The example above
specifies unrestricted read, write, and command execution.
The
.qC ALL
value in the commands option means that any command
can be executed!
\fBWARNING\fP:
Using the
.qC ALL
value gives the remote site unlimited access to your
system.
Files that are only readable or writable by user
.I "uucp"
(like
.fN Systems )
can be accessed using commands like
.cN "ed" .
.PP
The assumption you make by the first entry above is that when you
call
.I mhtsa ,
.I mhtsb
or
.I mhtsc ,
you really know who you are talking to.
Therefore, any files put into one of the
.I mhtsa ,
.I mhtsb
or
.I mhtsc ,
.I spool
directories is put there by one of those sites.
If a remote site logs in declaring it is one of these three systems,
the execution files will also be put in the privileged
.I spool
directory.
You therefore have to validate that the site has the privileged
login
.I "uucpz"
to prevent masquerading.
.NH 2 
COMMANDS revisited
.PP
The
.cW COMMANDS
option specifies a list of commands that can be
executed by remote machines.
In addition to the names as specified above, they can be full path
names of commands, for example
.P1 0
COMMANDS=rmail:/usr/lbin/rnews:/usr/local/lp
.P2
specifies that command
.cN "rmail"
uses the default path,
which is set up at \*(uU compilation time\(emspecified in the
.fN parms.h
file.
When the remote site specifies
.cN rnews
or
.fN /usr/lbin/rnews
for the
command to be executed,
.fN /usr/lbin/rnews
will be executed
regardless of the default path.
Likewise,
.fN /usr/local/lp
is the
.cN lp
command that will be executed.
.PP
Including the
.qC "ALL"
value in the list means that any command from the
remote machine(s) specified in the entry will be executed.
.P1 0
COMMANDS=/usr/lbin/rnews:ALL:/usr/local/lp
.P2
This example illustrates two points.
The
.qC ALL
value can appear anywhere in the string.
And, the path names specified for
.cN rnew
and
.cN lp
will be used
if the requested command does not contain the full-path names for
.cN rnews
or
.cN lp .
.NH 2 
MYNAME
.PP
When a remote calls, the called system responds with the local system
name;
this communicated in the
.cW Shere
message.
There are some situations when a system may want to say it is someone else.
For testing, this permits a system to call itself.
Also, a series of systems can be made to look like one to the outside world,
while retaining unique identities within a local network.
.P1 0
LOGNAME=uucptest   MYNAME=testing
.P2
The local system will report its name as
.cW testing
whenever a remote logs in as uucptest.
.PP
This facility can also be used when calling out:
.P1 0
MACHINE=testmach   MYNAME=atest
.P2
Tells the machine,
.cW testmach ,
that machine
.cW atest
is calling.
.......
.NH 2 
PUBDIR
.PP
.fN /usr/spool/uucppublic ,
the
.I public
directory,
provides directories
for public access.
One may want to have different
.I public
directories based on
login ids.
.P1 0
LOGNAME=loginA\e
	PUBDIR=/usr/spool/uucppublic/loginA
LOGNAME=loginB\e
	PUBDIR=/usr/spool/uucppublic/loginB
.P2
This can also be specified when remote machines are called:
.P1 0
MACHINE=machineA\e
	PUBDIR=/usr/spool/uucppublic/machineA
MACHINE=machineB\e
	PUBDIR=/usr/spool/uucppublic/machineB
.P2
.NH 2 
Default Settings
.PP
The
.fN parms.h
header file contains some default settings that affect
the
.fN Permissions
file processing.
The
.cW PATH
manifest defines the
.cW PATH
environment variable that will be
set when remote commands are executed.
A typical line is
.P1 0
#define PATH "PATH=/bin:/usr/bin:/usr/lbin"
.P2
The default list of commands is defined by
.P1 0
#define DEFAULTCMDS     "rmail"
.P2
Another example is
(note this is one physical line broken here for readability)
.P1 0
#define DEFAULTCMDS
	"rmail:rnews:xp:lp"
.P2
These take effect if no
.cW COMMANDS
option is specified for the
machine that sent the remote execution.
.NH 2 
MACHINE Entry For Other Systems
.PP
An administrator may want to specify different option values for
the machines it calls that are not mentioned in specific MACHINE
entries.
This may occur when there are many machines calling in, and the command
set changes from time to time.
For these cases, it is not convenient to change the
.cW DEFAULTCMDS
as it would require a recompile.
The name
.cW OTHER
for the machine name is used for this entry.
.P1 0
MACHINE=OTHER  \e
COMMANDS=rmail:rnews:/usr/lbin/Photo
.P2
All other options available for the MACHINE entry may also be set
for the machines that are not mentioned in other MACHINE entries.
.NH 2 
Combining MACHINE and LOGNAME Entries
.PP
It is possible to combine MACHINE and LOGNAME entries into a single entry
where the common options are the same.
For example, these two entries
.P1 0
MACHINE=mhtsa:mhtsb:mhtsc REQUEST=yes \e
	READ=/  WRITE=/

LOGNAME=uucpz REQUEST=yes SENDFILES=yes \e
	READ=/  WRITE=/
.P2
share the REQUEST, READ, and WRITE options.
They can be merged into one entry
.P1 0
MACHINE=mhtsa:mhtsb:mhtsc REQUEST=yes \e
	LOGNAME=uucpz SENDFILES=yes \e
	READ=/  WRITE=/
.P2
that will take the place of the two entries.
.NH 1 
\f(CBMaxuuxqts\fP File
.PP
The
.fN /usr/lib/uucp/Maxuuxqts
file limits the number of simultaneous
.cN uuxqt
programs running;
it contains an \*(aS number.
The installation procedure sets the number to two;
the administrator may want to change this number to meet local needs.
If you get a lot of traffic from
.cN mail
or
.cN netnews ,
you may want to increase
the number to decrease wait time.
But remember, the more you have running, the higher the load on the system.
.NH 1 
\f(CBMaxuuscheds\fP File
.PP
The
.fN /usr/lib/uucp/Maxuuscheds
file limits the number of simultaneous
.cN uusched
programs running;
it contains an \*(aS number.
Each 
.cN uusched
running will have one
.cN uucico
associated with it;
limiting the number will throttle the load on the system.
The limit should be less than the number of outgoing lines used
by \*(uU ;
a smaller number is often desirable.
The installation procedure sets the number to two;
the administrator may want to change this number to meet local needs.
.NH 1 
\f(CBremote.unknown\fP Program
.PP
This program is called when a remote site that is not in the
.fN Systems
file calls in to start a conversation.
The program logs attempts by unknown remote systems in
.fN /usr/spool/uucp/.Admin/Foreign
and sends mail to the
.cW uucp
login.
Execution of this program can be turned off by an option in
.fN parms.h .
The program
.cN unknown.c
is installed in
.cN remote.unknown ,
so the source can be modified to provide different functionality
by modifying the program and re-installing it.
.NH 1 
Administration
.PP
The work required by the \*(uU
administrator depends heavily
on the amount of traffic that enters or leaves a system and
the quality of the connections that can be made to and from that system.
For the average system, only a modest amount of traffic (100 to 200 files
per day) pass through the system and little if any
intervention with the
\*(uU
automatic cleanup functions is necessary.
Systems that pass large numbers of files
may require more attention.
The following rest of this section
describes the routine administrative
tasks that must be performed by the administrator
or are automatically performed by
the
\*(uU
package.
.NH 2 
Cleanup of Undeliverable Jobs
.PP
A big problem
in a dialup network like \*(uU
is dealing with the backlog of jobs that cannot
be transmitted to other systems.
The
.cN uustat
program should be invoked regularly to give information
about the status of connection to various machines, and the size and
age of the queued requests.
The
.cN uudemon.admin
shell should be started by
.cN cron
at least once per day\(emthis will send the administrator the
current status.
Of particular interest are the age (in days)
of the oldest request in each queue,
the number of times failure to reach that system has occurred, and
the reason for failure.
In addition, the age of the oldest execution request (\c
.fN X.
file) is
also given.
.PP
Each
.I spool
directory will contain some
.fN X.
files,
.fN  C.
files, and
.fN  D.
files.
When work can not be done, these files should be
removed.
The
.cN uucleanup
program,
which is run from
.cN uudemon.cleanup
will provide this function.
Options to
.cN uucleanup
specify the age for sending a
warning message to the requester and age for deleting the
various file.
The
.cN uucleanup
program knows
about the different type files that could get left in the
.I spool
directories.
It uses heuristics to try to give the users relevant
information about failures, for example, it tries to return
undeliverable mail messages to the sender.
In addition, for send/receive requests, it tells the requester
what was attempted by giving specific file names.
.NH 2 
Cleanup of the Public Directory
.PP
In order to keep the local file system from overflowing
when files are sent to the
.I public
directory, the
.cN uudemon.cleanup
procedure is set up with a
.cN find
command to remove any files that are older than 7 days
and directories that are empty.
This interval may need to be shortened if
there is not enough space to devote to the
.I public
directory.
.PP
Since the
.I spool
directories (those in
.fN /usr/spool/uucp )
may be very dynamic; they may grow
large before transfers take place, it is a good idea to
reorganize and compact the structure.
One way to do this is to put some code in
.fN /etc/rc
to be executed upon booting the system.
Use
.fN cpio
to move all the files and directories out,
remove the directories under
.fN /usr/spool/uucp ,
and then move back the files and directories.
.NH 2 
Compaction of Log Files
.PP
\*(uU has individual log files for each system
and each program\(emthere is a separate
logfile for
.cN uucico
requests and one for
.cN uuxqt
execution requests.
The
.cN uulog
shell gives the user access to the information in these
files by system name.
These files are combined and stored in directory
.fN /usr/lib/uucp/.Old
whenever
.cN uudemon.cleanup
is executed.
The daemon saves two days' files;
this can be easily changed by the administrator.
If space is a problem, the administrator might consider reducing the
number of days the files are kept, or modify the shell to compact
the files using the
.cN pack
command.
.NH 2 
Polling Other Systems
.PP
Systems that are passive members of the network
must be polled by other systems
in order for their files to be sent.
This can be arranged by using the
.cN uudemon.poll
shell.
The
.cN uudemon.poll
read the
.fN /usr/lib/uucp/Poll
file, which contains
the systems and times to poll them.
The lines contain the name of the remote to call followed by a TAB
character and then a space separated list of times to poll.
For example,
.P1 0
eagle        0 4 8 12 16 20
.P2
will provide polling of system
.I eagle
every four hours.
Note that
.cN uudemon.poll
does not do the polling, it merely sets up a polling
.fN C.
file in the
.I spool
directory that will be
seen by the scheduler started by
.cN uudemon.hour .
.......
.NH 2 
Out of Space
.PP
The file system used to spool
incoming or outgoing jobs
can run out of space and prevent jobs from being spawned
or received from remote systems.
The inability to receive jobs is the worse of the two conditions.
When file space does become available, the system will be
flooded
with the backlog of traffic.
.NH 2 
Bad ACU and Modems
.PP
The ACU and incoming modems
occasionally cause problems that make it difficult to contact
other systems or to receive files.
These problems are usually readily identifiable since
the status files accessed by
.cN uustat
give counts and
reasons for contact failure.
If a bad line is suspected, it is useful to use
the
.cN cu
command to
try calling another system using the suspected line.
This method could also be used to check the login/password
information and phone number.
.NH 2 
Debugging
.PP
To verify that a system on the network can be contacted,
the
.cN uucico
program can be invoked by the administrator.
The
.cN Uutry
shell program is distributed for this purpose.
For example, to verify that
.I mhtsd
can be contacted, try
.P1 0
Uutry mhtsd
.P2
This will start the transfer program (\c
.cN uucico )
with a moderate
amount of debugging output, putting the output into a temporary
file (\c
.fN /tmp/mhtsa )
and executing a
.cW "tail -f"
command so the
administrator can hit a BREAK to get back to the shell, while
being able to come back at a later time to look at the output.
.PP
If that works, the administrator can attempt to transfer a file
while watching the debugging output.
Proceed as follows
.P1 0
uucp -r some\-file mhtsd!~/some\-name
.P2
This will queue a job but not start the transfer program.
Now proceed as before using
.cN Uutry .
The output can be analyzed to locate problems.
.PP
.fN /usr/spool/uucp/.Admin/errors
contains errors\(em\c
conditions that causes one of the \*(uU programs to abort
(\fIASSERT\fP errors).
An explanation of these is given
in Appendix II along with an explanation of the messages one
can see as output from
.cN "uustat -q" .
Most of these will never occur\(emthey indicate that something is wrong
with your system or the \*(uU software.
However, the
.I PKXSTART
will occur and generally means that the
other side aborted during a conversation in a non-recoverable way;
these can be generally ignored.
.NH
Appendix I: Local Configuration Options
.PP
The
.fN parms.h
header file is used to set up local site
options before a
.cN make
command is attempted.
The file is set up with default settings for a standard
.UX
distribution, however, there are some options that
the local administrator might want to use.
The file has comments to briefly describe the options;
this section contains more details.
.de XX
.IP "\f(CW\\$1\fP\ " 5n
..
.XX "ATTSV, V7, V8, V9, V10, BSD4_2"
.br
One of these should be defined:
.CW ATTSV
for standard
.UX
systems;
.CW V7
for Version 7 based systems like 32V, Berkeley 4.1 systems;
.CW BSD4_2
for Berkeley 4.2 systems;
.CW V8 ,
.CW V9 ,
.CW V10
for that Edition of Research
.UX
systems.
.XX UUCPUID
There are several places in the code where the uid of ``uucp'',
the owner of the uucp programs, files, and directories,
must be used.
In most cases, the uid can be obtained, but on some systems, when
running as root, the info will not be forthcoming, so this
manifest is used;
it is the uid of the uucp login (again, the owner) from the /etc/passwd
file.
.XX ATTSVKILL
The new lock-file mechanism uses the system call
.cN "kill(0, pid)"
to determine if a process-id in a LCK file is still active.
Standard
.UX
systems provide this facility, but some do not
support it.
Define ATTSVKILL if you system supports the
.cN "kill(0, pid)"
call.
(Note that this is automatically defined if ATTSV is defined.)
.XX NONAP
Define NONAP if you have no high-resolution sleep call.
The standard
.UX
system
does not have this high-resolution sleep, so this
must be defined.
.XX FASTTIMER
This is the device that goes along with the high-resolution timer.
Do not define if for
.UX
systems,
since it is not available.
.XX V7USTAT 
\*(uU uses
.cN ustat
to decide whether there's enough space to receive a
file.
If you're not ATTSV, you can use a setgid program to read the
number of free blocks and free inodes directly off the disk.
If you
choose this course, do not define NOUSTAT;
rather, define V7USTAT to
be the name of that program.
Be sure it accepts 2 args, major and minor
device numbers, and returns two numbers, blocks and inodes,
in "%d %d" format, or you'll never receive another file.
.XX NOUSTAT
Define this if your system does not have a
.cN ustat()
system call.
The standard
.UX
system
has the call;
don't define it for those systems.
.XX "GRPCHK, GRPMIN, GRPMAX"
.br
Define GRPCHK if you want to restrict the ability to read
.fN Systems
information by way of the DEBUG flags.
If you define GRPCHK, then the group-ids GRPMIN and GRPMAX limit
the group-ids for which the
.cN Systems
file password information will be
displayed when the DEBUG option is used.
.XX UNET
Use this to include code for 3com ethernet media.
Appropriate changes must be made in the
.fN makefile
to include the needed routines.
See comments in the
.fN makefile .
.XX DATAKIT
Define DATAKIT if your system is connected to a DATAKIT VCS.
If you use this option, you must also make the appropriate
changes in the
.fN makefile
to access the dk library and loading
of the dio.o routine\(emsee the comments in the makefile.
.XX TCP
Define TCP for BSD systems that have TCP or UNET.
.XX SYTEK
Define SYTEK for systems that access a Sytek network.
.XX TLI
Define this for access to the AT&T Transport Layer Interface
.I without
Streams.
.XX TLIS
Define this for access to the AT&T Transport Layer Interface
.I with
Streams.
.XX DIAL801
This is defined for the standard 801/212-103 dialer interface.
It will be defined by default.
.XX X25
Use this to include code for the X25 media.
Appropriate changes must be made in the
.fN makefile
to include the needed routines.
See comments in the makefile.
.XX DUMB_DN
Define DUMB_DN if your dn driver (801 acu) can't handle '=' character
to wait for dialtone.
.XX DEFAULT_BAUDRATE
This is the baud rate you want to use when both
.fN Systems
file and 
.fN Devices
file specify
.qC Any
for the speed.
.XX M_DEVICEMODE
This is the mode that the device will be set to by the caller
during execution.
.XX S_DEVICEMODE
This is the mode that the device will be set to by the callee
during execution.
.XX R_DEVICEMODE
This is the mode that the device will be left in upon exit if
the current mode can not be obtained using
.cW fstst() .
.XX UUSTAT_TBL
There is a table in
.fN uustat.c
that can hold all machine names that currently
have work or execute files (C. or X.) or have a status file.
If necessary, the table size can be changed.
For machines
with much memory, a large number like 1000 will not hurt much since the
program is not executed often.
For small machines, 256K memory, the number should be much smaller
like 100.
.XX UNAME
Define UNAME if
.cW uname()
should be used to get uucpname;
this will be defined automatically if ATTSV is defined.
.XX RETRYTIME
This is the initial retry after failure time.
Each successive failure will double the current retry time.
The time is given in minutes.
.XX MAXRETRYTIME
This is the high limit to the retry backoff.
.XX ASSERT_MAXRETRYTIME
This is the high limit to the retry backoff when an
.I ASSERT
error occurs.
.XX PATH
This is the path that will be used for
.cW uuxqt
command executions.
.XX DEFAULTCMDS
This is the set of default commands that can be executed 
if none is given for the system name in
.fN Permissions
file.
It is a colon separated list as in
.fN Permissions
file.
.XX HZ
Define HZ to be the number of clock ticks per second;
not needed for standard
.UX
system.
.XX MYNAME
Put in local uucp name of this machine if there is no
.cW /etc/whoami
and no
.cW uname()
call.
This is not needed for standard
.UX
systems.
.XX NOSTRANGERS
Define NOSTRANGERS if you want to reject calls from systems that
are not in your
.fN Systems
file.
If defined, NOSTRANGERS should be the name
of the program to execute when such a system dials in.
The argument
to the program will be the name of the calling system.
The
.fN unknown.c
program is supplied and installed in
.fN /usr/lib/uucp
as
.cN remote.unknown .
.XX LMTUUXQT
Define LMTUUXQT to be the name of a file that contains the number
(in \*(aS) of simultaneous
.cW uuxqt 's
that you will permit.
If it is
not defined, there may be
.I many
.cW uuxqt 's
running.
Two is reasonable number.
The system will create the default file
and set the limit to 2.
.XX LMTUUSCHED
Define LMTUUSCHED to be the name of a file that contains the number
(in \*(aS) of simultaneous
.cW uusched 's
that you will permit.
If it is
not defined, there may be
.I many
.cW uusched 's
running.
Two is reasonable number.
The system will create the default file
and set the limit to 2.
The more you permit the higher the load on the system;
each
.cW uusched
has one
.cW uucico
associated with it.
.XX USRSPOOLLOCKS
Define USRSPOOLLOCKS if you like your lock files in
.fN /usr/spool/locks .
Be sure other programs such as
.cW cu
and
.cW ct
know about this.
.XX PKSPEEDUP
Define PKSPEEDUP if you want to try the recommended speedup
in
.cW pkcget .
This entails sleeping between reads at low baud rates.
.NH 1
Appendix II: Error Messages
.NH 2
Fatal System Errors
.PP
These are the ASSERT error messages that can occur.
The will appear in the error log (\c
.fN /usr/spool/uucp/.Admin/errors ).
When the errors occur, the program will abort\(emthe file name,
sccsid, line number will appear in the error message along with the
following text.
In most cases, these result from file system problems;
use the
.cW  errno ,
when present as indicated below, to check out the problem.
.de XX
.ie \\n(.$-1 .IP "\f(CW\\$1\fR(\fIerrno\fR)" 5n
.el .IP "\f(CW\\$1\fP" 5n
.br
..
.XX "CAN'T OPEN" x
An
.I open
or
.I fopen
failed.
.XX "CAN'T WRITE" x
A
.I write ,
.I fwrite ,
or
.I fprintf
etc failed.
.XX "CAN'T READ" x
A
.I read ,
.I fgets ,
etc failed.
.XX "CAN'T CREATE" x
A
.I creat
failed.
.XX "CAN'T ALLOCATE"
A dynamic allocation failed.
.XX "CAN'T LOCK"
An attempt to make a LCK (lock) file failed.
In some
cases, this is a fatal error.
.XX "CAN'T STAT" x
A
.I stat
failed.
.XX "CAN'T CHMOD" x
A
.I chmod()
failed.
.XX "CAN'T CHOWN" x
A
.I chown
failed.
.XX "CAN'T LINK" x
A
.I link
failed.
.XX "CAN'T CHDIR". x
A
.I chdir
failed.
.XX "CAN'T UNLINK" x
A
.I unlink
failed.
.XX "WRONG ROLE"
This is an internal logic problem.
.XX "CAN'T MOVE TO CORRUPTDIR"
An attempt to move some bad
.fN C.
or
.fN X.
files
to the
.fN .Corrupt
directory failed\(emthe directory is
probably missing or has wrong modes or owner
(\c
.fN /usr/spool/uucp/.Corrupt ).
.XX "CAN'T CLOSE" x
A
.I close
or
.I fclose
failed.
.XX "FILE EXISTS"
The creation of a
.fN C.
or
.fN D.
file is attempted, but the file exists.
This
occurs when there is a problem with the sequence file
access\(emusually a software error.
.XX "No uucp server"
A TCP call is attempted, but there is no server for \*(uU.
.XX "BAD UID"
The uid can not be found in the
.fN /etc/passwd
file.
The file system is in trouble, or the
.fN /etc/passwd
file is inconsistent.
.XX "BAD LOGIN_UID"
same as previous.
.XX "ULIMIT TOO SMALL"
The ulimit for the current user/process is too small;
file transfers may fail, so transfer is not
attempted.
.XX "BAD LINE"
There is a bad line in the
.fN Devices
file;
there are not enough arguments on one or more lines.
.XX "FSTAT FAILED IN EWRDATA"
.br
There is something wrong with the ethernet media.
.XX "SYSLST OVERFLOW"
An internal table in
.cW gename.c
overflowed.
A big/strange request was attempted\(emsend it with MR to the
appropriate place.
.XX "TOO MANY SAVED C FILES"
.br
same as previous
.XX "RETURN FROM fixline ioctl"
.br
An
.cW ioctl
call,
which should never fail, failed.
There is a system or driver problem.
.XX "BAD SPEED"
A bad line speed appears in the
.fN Devices /
.fN Systems
files.
.XX "PERMISSIONS file: BAD OPTION--"
.br
There is a bad line or option in the
.fN Permissions
file.
Fix it immediately!
.XX "PKCGET READ"
The other side probably hung up;
don't worry about it.
.XX "SYSTAT OPEN FAIL"
There is a problem with the modes of
.fN /usr/lib/uucp/.Status ,
or there
is a file with bad modes in the directory.
.XX "TOO MANY LOCKS"
There is some internal problem!
Send in an MR.
.XX "CAN NOT ALLOCATE FOR"
There is some kernel problem;
a call to
.cW calloc()
failed.
.XX "XMV ERROR"
There is a problem with some file or directory;
It is likely the spool directory, since the modes of
the destinations were suppose to be checked before
the program attempts this.
.XX "CAN'T FORK"
An attempt to
.cW fork
or
.cW exec
failed.
The current job should not be lost, but will be attempted later
(\c
.cW uuxqt ).
No action need be taken.
.NH 2
System Status Messages
.PP
These are the messages that will appear in the system status file:
.XX "OK"
Things are OK.
.XX "NO DEVICES AVAILABLE"
There is no currently available device for the call.
Check to see that there is a valid device in
.fN Devices
file
for this system.
.XX "WRONG TIME TO CALL"
self explanatory
.XX "TALKING"
self explanatory
.XX "SOME FAILURE"
to be determined
.XX "BAD SEQUENCE CHECK"
If sequence checking is used between systems, the
sequence numbers do not agree.
(This is almost never used.)
.XX "CALLER SCRIPT FAILED"
The negotiations with the modem/network specified in the
.fN Dialers
file did not complete successfully.
This is similar to DIAL FAILED.
It may occassionally fail, but if it never succeeds, there may
be a problem with the entry in the
.fN Dialers
file.
.XX "LOGIN FAILED"
The login for the given machine failed.
It could
be a wrong login/passwd, wrong number, a very slow
machine, or failure in getting through the
.cW login-script .
.XX "CONVERSATION FAILED"
The conversation failed after successful startup.
This usually means that one side went down, the program
aborted, or the line just hung up.
.XX "DIAL FAILED"
The remote never answered.
It could be a bad dialer or
the wrong phone number.
.XX "BAD LOGIN/MACHINE COMBINATION"
.br
The machine called us with login/machine name
that does not agree with our
.fN Permissions
file.
They could be trying to masquerade!
.XX "DEVICE LOCKED"
The calling device is currently locked and in use
by some process.
.XX "ASSERT ERROR"
An
.I ASSERT
error occurred\(emsee
.fN /usr/spool/uucp/.Admin/errors .
.XX "SYSTEM NOT IN Systems"
The system name is not in
.fN Systems .
.XX "CAN'T ACCESS DEVICE"
The device tried does not exist,
or the modes are wrong.
.XX "DEVICE FAILED"
The open of the device failed.
.XX "WRONG MACHINE NAME"
The called machine is reporting a different
name in the
.cW Shere=
message.
.XX "CALLBACK REQUIRED"
The called machine requires that it call us.
.XX "REMOTE HAS A LCK FILE FOR ME"
.br
The remote site has a
.fN LCK
file for this
system.
They could be trying to call us.
If they have an older version of \(*uU, the process
that was talking to us may have failed, leaving the
.fN LCK
file.
If they have the new \*(uU, and they are
not trying us, then the process that was talking to
us is hung!
.XX "REMOTE DOES NOT KNOW ME"
.br
The remote site does not have our name in their
.fN Systems
file.
.XX "REMOTE REJECT AFTER LOGIN"
.br
The login we used does not correspond to what
the remote site is expecting.
.XX "REMOTE REJECT, UNKNOWN MESSAGE"
.br
The remote site rejected us for unknown
reason\(emthey are probably not running a standard
version of \*(uU.

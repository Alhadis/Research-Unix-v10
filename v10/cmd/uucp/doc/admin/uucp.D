....mm -t -rC3 this document
.ND "June 8, 1984"
.EH "'CHAPTER 10'UUCP ADMINISTRATION''"
.OH "''UUCP ADMINISTRATION'CHAPTER 10'"
.H 1 "UUCP ADMINISTRATION"
.H 2 "INTRODUCTION"
.P
This chapter describes how a \fBuucp\fR network
is set up, the format of control files, and administrative procedures.
Administrators should be familiar with the
manual pages for each of the
\fBuucp\fR
related commands.
.H 2 "PLANNING"
.P
In setting up a network of UNIX systems, there
are several considerations that should be taken into account
\fIbefore\fR
configuring each system on the network.
The following parts attempt to
outline the most important considerations.
.H 3 "Extent of the Network"
.P
Some basic decisions about
access
to processors in the network must be made
before attempting to set up the configuration files.
If an administrator has control over only one processor
and an existing
network is being joined,
then the administrator must decide what level of access should
be granted to other systems.
The other members of the network must make a similar decision for the new system.
The
UNIX
system
\fIpassword\fR
mechanism is
used to grant access to other systems.
The file
\fI/usr/lib/uucp/Permissions\fR
provides various permissions/restrictions for file system tree access
and command execution from remote machines,
and the file
\fI/usr/lib/uucp/Systems\fR
(old name was \fI/usr/lib/uucp/L.sys\fR)
on the local processor determines the
systems on the network that can be directly reached.
.P
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
is paramount; however, this is usually impractical.
Very limited access can be granted to outside machines by each
of the systems
on the
private
network.
Alternatively, access to/from the outside
world can be confined to only one processor.
This is frequently done to minimize the effort in keeping
access information (passwords, phone numbers, login sequences, etc.)
updated and to localize the security holes
for the private network.
.H 3 "Hardware and Line Speeds"
.P
There are several supported means of interconnection
by
\fBuucp\fR(1),
.AL
.LI
Direct connection using a null modem.
.LI
Connection over the Direct Distance Dialing (DDD) network
using various dialers (e.g. WECo 801, Ventel, Vadic, Penril).
.LI
Connection using a DATAKIT\*(Tm VCS.
.FS ""
DATAKIT is a trademark of AT&T.
.FE
.LI
A UNET\*(EMEthernet\*(Tm network.
.FS ""
Ethernet is a trademark of Xerox Corporation.
.FE
.LE
.P
In choosing hardware, the
equipment
used by other processors on the network must be considered.
For example, if some systems on the network
have only 103-type (300-baud) data sets, then
communication with them is not possible
unless the local system has a 300-baud data set connected to a calling unit.
(Most data sets available on systems
are 1200-baud.)
If hard-wired connections are to be used between
systems, then the
distance
between systems must be considered since a null modem cannot be used
when the systems are separated by more than several hundred feet.
The limit for
communication at 9600-baud and 19200-baud is about 800 to 1000 feet.
However, the
RS232 specification and Western Electric Support Groups
allow for up to 50 feet.
Limited distance modems must be used beyond 50 feet as
noise on the lines becomes a problem.
.H 3 "Maintenance and Administration"
.P
There is a minimum amount of maintenance that must be provided
on each system to keep the access files updated, to ensure that
the network is running properly, and to track down line problems.
When more than one system is involved, the job becomes
more difficult because there are more
files to update and because
users are much less patient when failures occur between machines that
are under local control.
A tool, \fIuustat\fR, is available to aid the administrator by providing
information about the last attempts to contact various systems and the
age and number of jobs in the queue for remote systems.
.P
There is also a new cleanup program, \fBuucleanup\fR, that knows
about the different type files that could get left in the spool
directories.
It uses some heuristics in an attempt to give the users relevant
information about failures, for example, it tries to return
undeliverable mail messages to the sender.
In addition, for send/receive requests, it tells the requestor
what was attempted by giving specific file names.
The program is started by \fBuudemon.cleanup\fR,
which provides some addition cleanup functions.
See the \fIuucleanup.1m\fR manual page for details.
.H 2 "UUCP SOFTWARE"
.P
Figure 10-1 (at the end of this chapter)
illustrates the daemons used by the
\fBuucp\fR
network
to communicate with another
system.
The
\fBuucp\fR(1)
or
\fBuux\fR(1)
commands queue users' requests
and spawn
the
\fBuucico\fR
daemon
to call another system.
Figure 10-2 (at the end of this chapter)
illustrates the structure of
\fBuucico\fR
and the tasks that it performs in communicating with another system.
\fBUucico\fR initiates the call to another system and
performs the file transfer.
On the receiving side,
\fBuucico\fR
is invoked to receive the transfer.
Remote execution jobs are actually
done by transferring a command file to the remote
system and invoking a daemon
(\fBuuxqt\fR)
to execute that command file and return the results.
.H 2 "INSTALLATION"
.P
The
\fBuucp\fR(1)
package is delivered
as part of the
standard UNIX system distribution.
It resides in its own subdirectory (called
\fIuucp\fR)
in the commands area and has its own make file
(\fIuucp.mk\fR).
The
\fBuucp\fR
package is installed as part of the normal distribution;
however, if it must be reinstalled for
any reason, then the sequence
.DS I
make \-f uucp.mk install
.DE
should be executed.
In addition, the distribution has default setting for uucp system
parameters, such as the available communication media.
If there are local differences, the \fIparms.h\fR
file should be modified before the ``make'' command is executed.
(See appendix I for details of the options for the parms.h file.)
.H 3 "Object Modules"
.P
The following object modules are installed as part
of the
\fBuucp\fR
make procedure.
.AL
.LI
\fBUucp\fR\*(EMThe file transfer command.
.LI
\fBUux\fR\*(EMThe remote execution command.
.LI
\fBUucico\fR\*(EMThe
\fBuucp\fR
network daemon.
.LI
\fBUustat\fR\*(EMNetwork status command.
.LI
\fBUusched\fR\*(EMFile tranfer daemon scheduler.
.LI
\fBUucheck\fR\*(EMThe \fIuucp\fR system files, directories, and Permissions
file checker.
.LI
\fBUuxqt\fR\*(EMThe remote execution daemon.
.LI
\fBUugetty\fR\*(EMA getty that can be used for lines/modems that are to
work as dialin and dialout ports.
.LI
\fBUulog\fR\*(EMA shell for looking at the various log files.
.LI
\fBUutry\fR\*(EMA shell for administrators that is used to try a remote
system with debugging options.
Note that the name has the first letter in upper case (Uutry).
.LI
Several shell procedures are also distributed;
these are started periodically by \fIcron\fR.
\fBUudemon.cleanup\fR is usually executed daily to clean up
various directories.
\fBUudemon.hour\fR is used to start the file transfer scheduler and
the command execution daemon.
\fBUudemon.admin\fR is available to send data to the uucp administrator
giving information about the status of the system.
And \fBuudemon.poll\fR is used for periodic polling of remote systems.
.LE
.H 3 "Password File"
.P
To allow remote systems to call
the local system, password entries must be made
for any
\fBuucp\fR
logins.
For example,
.DS
\s-1nuucp:zaaAA:6:1:UUCP.Admin:/usr/spool/uucppublic:/usr/lib/uucp/uucico\s+1
.DE
Note that the
\fBuucico\fR
daemon is used for the
shell,
and the spool directory
is used as the working directory.
.P
There must also be an entry in the
\fIpasswd\fR file for a \fBuucp\fR administrative
login.
This login is the owner of all the \fBuucp\fR
object and spooled data files and is usually ``uucp''.
For example, the following is a entry in
\fI/etc/passwd\fR for this administrative login:
.DS I
uucp:zAvLCKp:5:1:UUCP.Admin:/usr/lib/uucp:
.DE
Note that the standard shell is used instead of
\fBuucico\fR.
If an owner other than ``uucp''
is chosen, the \fImake\fR file for \fBuucp\fR
(\fI/usr/src/cmd/uucp/uucp.mk\fR) must be edited.
The line "OWNER=uucp" must be changed to reflect the
new owner login.
.H 3 "Devices File"
.P
The file
\fI/usr/lib/uucp/Devices\fR
(old name was \fI/usr/lib/uucp/L-devices\fR)
contains the device information needed
for calling other systems.
The format of the file is
.ml |
.DS I
Caller  Line  Line2  Class  Dialer-Token-Pairs
.DE
where each field is
.VL 18 4
.LI \fICaller\fR
The keywords available in this field are
.VL 9 0
.LI Direct
for a hard-wired line used by cu for direct connections.
.LI ACU
to make the connection through an autodialer.
.LI \fINETWORK\fR
to make the connection through a switch where \fINETWORK\fR
is replaces by one of
Sytek, Unetserver, DK, Sytek, TCP.
.LI \fISYSTEM-NAME\fR
for hard-wired connections to a particular system where
\fISYSTEM-NAME\fR is replaced by the name of the system.
.LE
.LI \fILine\fR
This is the device name for the line (e.g.,
\fIttyab\fR
for a direct line or hard-wired line to a system,
\fIcul0\fR
for a line connected to an ACU).
.LI \fILine2\fR
If the ACU keyword is specified and the device type is 801,
this field contains the
device name of the 801 dialing device ACU.
Otherwise, the field is ignored; however, a placeholder must be
used in this field (use '-' character for the placeholder).
.ml
.LI \fIClass\fR
For ACU, this can be just the speed,
or it can contain a letter and speed (e.g. D1200, C1200)
to differentiate between classes of dialers (Centrex, Dimension).
Some devices can be used at any speed, so the keyword ``Any''
is used\*(EMthis line will match any speed requested in Systems.
Note:
If this field is ``Any'' and the Systems class field is ``Any''
then the speed is taken from the default set in \fIparms.h\fR
by the \fIDEFAULT_BAUDRATE\fR constant.
(The Class field is currently ignored if an X.25 link is used.)
.ml |
.LI \fIDialer-Token-Pairs\fR
The rest of the line contains pairs of dialers and tokens.
Each pair represents a caller function and an argument to
pass to that function.
(The last token may be left blank if it is to represent a
phone number.
See examples below.)
Dialers that are supported are
.VL 18 0
.LI 801
standard 801 autodialer with 212 or 103 modem.
.LI penril
this is a penril autodialer.
.LI hayes
this is a hayes autodialer.
.LI ventel
this is a ventel autodialer.
.LI vadic
this is a vadic dialer.
.LI \fINETWORK\fR
one of the names listed for \fINETWORK\fR above.
.LI \fIOTHER-DIALERS\fR
other dialers can be constructed by putting information in
the \fIDialers\fR file.
See \fIDialers\fR section below.
.LE
.P
The token following the Dialer can usually be left blank;
this represents the phone number.
The string ``\\D'' can be used in place of the blank;
it means use the phone number given in the \fISystems\fR file
as the argument to the dialer routing.
.P
It is possible to have dialout modems on networks.
For these cases, the \fICaller\fR will be ACU and the
\fIDialer-Token-Pair\fR will be the token given to the
network routine to attach to the dialout modem.
These two fields will be followed by another \fIDialer-Token-Pair\fR,
the dialer routine name (e.g. ventel) and the phone number
(e.g. ``\\D'' or ``\\T'' or blank.)
.LE
.P
The following examples
illustrate various types of connections:
.TS
l l l l l .
ACU	cul0	cua0	1200	801
ACU	cul1	cua1	300	801
ACU	vn0	-	1200	ventel
ACU	vn0	-	300	ventel
ACU	vd0	-	1200	vadic
ACU	vd0	-	V1200	vadic
ACU	pn2	-	1200	penril
ACU	pn2	-	300	penril
.TE
.ml
.P
/dev/cul0 and /dev/cul1 are 212 modems (cul1 may be a 103 type),
with 801s at /dev/cua0 and
/dev/cua1 respectively; /dev/vn0 is hooked to a ventel and can be used
at 1200 or 300 baud, and /dev/vd0 is hooked to a vadic.
There are
two entries for vadic so that sites that speak vadic tones only can be
flagged V1200 in \fISystems\fR.
(Actually, this assumes the vadic is a triple,
because sites that talk 212 tones will get the vadic as well as these two entries.)
There is also a penril dialer on /dev/pn2;
it can be used at either 300 or 1200 baud.
.P
.ml |
.tr ~\\
.TS
l l l l l .
ACU	culd0	-	Any	develcon vent ventel ~D
ACU	culd1	-	Any	develcon vent ventel ~D
.TE
.tr ~
.P
There are two
ventel attached to the develcon; the line is culd0.
The ``vent'' is the token given to the develcon switch to get to the
ventel modem.
The ``\\D'' specifies the telephone number
from the \fISystems\fR file.
.P
Note:
In addition to the ``\\D'', there will sometimes be a need for
passing a phone number to a built-in routine
(e.g. DK) after it is expanded using the \fIDialcodes\fR file.
In this case, a ``\\T'' symbol is used instead of the ``\\D''.
For example
.TS
l l l l l .
ACU	-	0	Any	DK	dial.\T
.TE
.P
is an entry for using a dialer on a Datakit network.
.P
.TS
l l l l l .
raven	ttyab	-	9600	direct
Direct	ttyab	-	9600	direct
cbgbd	x25.s0	-	9600	direct
.TE
.P
There is a direct lines, one (ttyab) is a null-modem type connection
with the other end on system raven
while the second (x25.s0) is an X.25 permanent virtual circuit connection
to machine cbgbd.
Note also the ``Direct'' line;
this is used if a user would like to use the command
.DS
	cu -lttyab
.DE
to connect to machine ``raven''.
.H 4 "Naming Conventions"
.P
It is often useful when naming lines that are directly
connected between systems or that are dedicated to
calling other systems to choose a
naming scheme that conveys the use of the line.
In the earlier examples, the name
\fIttyab\fR
is used for the line that directly connects
two systems named
\fIa\fR
and
\fIb\fR.
Similarly, lines associated with calling
units are best given names that relate them to the
calling unit autodialer (note
the names
\fIcul0\fR
and
\fIcua0\fR
to specify the line and calling unit, respectively).
.H 3 "Dialers"
.P
Each line in the \fIDialers\fR file
is used to specify
the initial handshaking that should occur on a line before it
is made available for user data.
This initial handshaking is usually a sequence of ascii strings
that are transmitted and expected,
and is often used to dial a phone number using an ascii dialer
(such as a PENRIL 300/1200AD) or connect via a dataswitch to
another system on the dataswitch.
If the fifth field in the Devices file line is not
a built-in function name,
the field is used as an
index into the \fIDialers\fR file,
attempting to match the first field on each line.
In addition, each odd numbered field starting with the 7th position
is used as an index into the ``Dialers'' file.
.P
If the match succeeds,
the ``Dialers'' line is interpreted to perform
the dialer negotiations.
Field 1 match the 5th and additional odd numbered
fields in the ``Devices'' file.
The second field is used as a translate
string:
the first of each pair of characters in the string is
mapped to the second character in the pair in the phone number to
be dialed.
This is usually used to translate = and \- into whatever
the dialer requires for ``wait for dialtone'' and ``pause''.
.P
The remaining fields are ``expect''/``send'' strings
(seethe ``login'' section for an explaination of expect-send fields.)
Here are some sample line;
these will be distributed with the system and installed
as part of the installation procedures.
The administrator can modify them as required.
The escape characters, those beginning with ``\\'', have the same
meaning as specified in the ``Login'' section of the
``Systems'' file below.
In addition to those mentioned in that section,
the `\\T'' and ``\\D'' are used to substitute the phone number
string passed to the dialing function:
``\\T'' is expanded using the ``Dialcodes'' file and
``\\D'' is the unexpanded string.
.P
.nf
.tr ~\\
penril   =W-P   "" ~d > s~p9~c )-W~p~r~ds~p9~c-) y~c : ~E~TP > 9~c OK
ventel   =&-%   "" ~E~r~e~p $-~E~r~e~p-$ <K~T%%~r>~c ONLINE!
vadic    =K-K   "" ~005~p *-~005~p-*~005~p-* D~p BER? ~E~T~e ~r~c LINE
develcon ""     "" ~pr~ps~c est:~007 ~D ~007
direct
.fi
.P
The \fIpenril\fR entry executes as follows:
First the phone number argument is translated,
replacing any ``='' with a ``W'' (wait for dialtone)
and replacing any ``\-'' with a ``P'' (pause).
.P
The handshake given by the remainder of the line works as follows:
.VL 18 0
.tr ~"
.LI ~~
Wait for nothing.
.LI ~d
.tr ~\\
Delay for 2 seconds.
.LI >
Wait for a >.
.LI  s~p9~c
send an ``s'', pause for 1 seconds, send a 9,
send no terminating new-line.
.LI  )\-W~p~r~ds~p9~c\-)
wait for a ``)''.
If it does not arrive, process the string between the ``\-'' characters
as follows.
Send a ``W'', pause, send a carriage-return, delay, send an ``s'',
pause, send ``9'', without a new-line,
and then wait for the ``)''.
.LI  y~c
send a ``y''.
.LI  :
wait for a ``:''.
.LI ~E~TP
enable echo checking. (From this point on, whenever a character is transmitted,
will wait for the character to be received before doing anything else.)
Then send the phone number followed by a penril \fIpause\fR character.
(The ~Tmeans take the phone number passed as argument and apply
the ``Dialcodes'' translation and the modem function translation
specified by field number 2 of this entry.)
.LI >
wait for a ``>''.
.LI 9~c
send a ``9'' without a new-line.
.LI OK
Wait for the string ``OK''.
.LE
.P
As another example, consider  the \fIdevelcon\fR entry:
The is phone number is the token used by the switch, for example
the machine name to be connected.)
.VL 18 0
.LI ""
wait for nothing.
.LI ~pr~ps
pause, send an ``r'', pause, send an ``s''.
.LI est:~007
wait for the string ``est:'' followed by a bell character
(~007).
.LI ~T
send the token passed from the \fIDevices\fR file;
often the token in the phone number field from the
.ml
\fISystems\fR file.
.LI ~007
wait for a bell.
.LE
.P
The ``direct'' line above indicates that for direct connections,
ne negotiation is required.
.H 3 "System File"
.P
Each entry in the \fI/usr/lib/uucp/Systems\fR file represents a system
that can be called by the local \fBuucp\fR programs.
In addition, the system can be configured to prevent
any system that does not appear in this file from
logging in.
(This is the default configuration;
it can be modified by changing ``parms.h'' before
compilation.)
More than one line may be present for a particular system;
the additional lines represent alternative
communication paths that will be tried in sequential order.
The fields are described below.
.VL 18 4
.LI "\fISystem name\fR"
Name of the remote system.
.LI \fITime\fR
This is a string that indicates the days-of-week
and times-of-day when the system should
be called
(e.g., MoTuTh0800\-1730).
.P
The day portion may be a list containing
\fISu\fR,
\fIMo\fR,
\fITu\fR,
\fIWe\fR,
\fITh\fR,
\fIFr\fR,
\fISa\fR;
or it may be
.I Wk
for any week-day or
.I Any
for any day.
The time should be a range of times (e.g., 0800\-1230).
If no time portion is specified, any time
of day is assumed to be allowed for the call.
Note that a time range that spans 0000 is permitted;
0800-0600 means all times are allowed \fIexcept\fR
times between 6 and 8 am.
Multiple time fields may be include using a ``,'' separator
(e.g. Wk1800-0600,Sa,Su).
An optional subfield is available to specify the minimum time (minutes)
before a retry following a failed attempt.
(Note that if this subfield is used, it will override the normal
exponential backoff algorithm for retry upon failure.)
This subfield is separated by a ``;''.
.LI \fICaller\fR
The available callers are ACU, Micom, Develcon, Sytek.
.LI \fIClass\fR
This is usually the line speed for the call (e.g., 300, 1200, Any).
If the field is not used for a particular entry, a ``\-'' can be used
as the place holder.
When the value is ``Any'',
it means match any speed found for the particular caller.
If both the Systems and Devices value is ``Any'', then the value is
taken from the \fIDEFAULT_BAUDRATE\fR constant defined in \fIparms.h\fR.
.LI \fIPhone\fR
For autodialers, the phone number is made up of an optional
alphabetic abbreviation (dialing prefix) and a numeric part.
The abbreviation should be one that appears in the
.I "Dialcodes (old name was L-dialcodes)"
file (e.g., mh1212, boston555\-1212).
For direct connections, the phone field is ignored.
(A ``\-'' should be used as a place holder.)
.P
For \fINETWORK\fR access as mentioned above
such as \fISwitch\fR, \fIDevelcon\fR, and \fIMicom\fR,
the phone field is the token the switch needs to get to the
particular system\*(EMit is used by the caller functions specified
in the ``Devices'' file.
.LI \fILogin\fR
The login information is given as a series of
fields and subfields in the format
.DS I
[ expect\ \ send ] .\|.\|.
.DE
where
\fIexpect\fR
is the string expected to be read and
.I send
is the string to be sent when the
.I expect
string is received.
This is simimilar processing to the negotiation specified in the
``Dialers'' file.
The expect field may be made up of subfields
of the form
.DS I
expect[\-send\-expect] .\|.\|.
.DE
where the
.I send
is sent if the prior
.I expect
is
.I not
successfully read
and the
.I expect
following the
.I send
is the next expected string.
(For example, login--login will expect
.I login ;
if it gets it, the program will go on to the next field;
if it does not get
.I login ,
it will send
.I null
followed by a new line,
then expect
.I login
again.)
If no characters are initially expected from the remote
machine, the string ``""'' (a null string) should be used in the
first expect field.
Note that all send fields will be sent followed by a new-line unless
the send string is terminated with a \\c.
.sp
There are two special names available to be sent
during the login sequence.
The string
.I EOT
will send an EOT character, and the string
\fIBREAK\fR
will try to send
a
\fIBREAK\fR
character.
.LE
.P
There are several character strings that cause specific actions
when they are a part of a string sent during the login sequence.
.VL 10 2
.tr ~\\
.LI ~K
insert a BREAK.
.LI ~N
Send a null character.
.LI ~b
Send a backspace character.
.LI ~c
If at the end of a string, suppress the new-line that
is normally sent.
Ignored otherwise.
.LI ~d
Delay two seconds before sending or reading more characters.
.LI ~n
Send a new-line character.
.LI ~p
insert a pause (1 second).
.LI ~r
Send a carrage-return.
.LI ~s
Send a space character.
.LI ~t
Send a tab character.
.LI ~~
Send a ~ character.
.LI EOT
Send \fIeot\fR character (actually \fIeot\fR new-line is sent twice).
.LI BREAK
Send a break character.
.LI ~ddd
Collapse the octal digits (ddd) into
a single character and send that character.
.br
.tr ~~
.LE
.P
These character strings are useful for making \fBuucp\fR
communicate via direct lines to data switches.
.P
A typical entry in the \fISystems\fR file would be
.DS I
sys\ \ Any\ \ ACU\ \ 300\ \ mh7654\ \ login\-\-login\ \ uucp\ \ ssword:\ \ word
.DE
A \fISystems\fR file entry for a direct connection would be
.DS I
hawk\ \ Any\ \ hawk\ \ 9600\ \ \-\ \ login\-\-login\ \ uucp\ \ ssword:\ \ word
.DE
The corresponding \fIDevice\fR file entry would be
.DS I
hawk\ \ ttyhh\ \ \-\ \ 9600\ \ direct
.DE
The ``expect'' algorithm matches all or part of the input
string as illustrated in the password field above.
.H 3 "Dialing Prefixes"
.P
This file contains the dial-code abbreviations used
in the
.I Systems
file (e.g., py, mh, boston).
The entry format is
.DS I
abb\ \ dial-seq
.DE
where
.I abb
is the abbreviation and
.I "dial-seq"
is the dial sequence to call that location.
.P
The line
.DS I
py\ \ 165\-
.DE
would be set up so that entry py7777 would
send 165\-7777 to the dial unit.
.H 3 "Permissions File"
The Permissions file replaces the USERFILE for uucp.
(The USERFILE was used in previous version of uucp to specify
file access permissions).
Its purpose is to specify the permission that remote sites
have with respect to login, file access, and command
execution.
Options provide for restricting the ability to request files and
the ability to receive files queued by the local site.
In addition, an option is available to specify the commands that a
remote site can execute on the local system.
.P
The installation procedure will construct a default file if one does
not already exist in \fI/usr/lib/uucp/Permissions\fR.
This default file will provide a high degree of restriction on remote sites.
A line will be constructed for each login in the \fI/etc/passwd\fR
file that has \fI/usr/lib/uucp/uucico\fR as the shell field.
Each line will look like
.DS CB
		LOGNAME=nuucp
.DE
.P
It states that login ``nuucp'' has all the default permissions/restrictions:
.BL
.LI
The remote site can send files exclusively to the
\fI/usr/spool/uucppublic\fR directory tree.
.LI
The remote site can \fINOT\fR request to receive any files.
.LI
\fINO\fR files that are queued for the remote site will be transferred
during the present session.
.LI
The only commands that can be executed are the defaults.
(The default commands are set in parms.h.
The distribution has them set to \fIrmail\fR and \fIrnews\fR.)
(See appendix IV for full details of the \fIPermissions\fR file setup.)
.LE
.H 3 "Maxuuxqts File"
The \fI/usr/lib/uucp/Maxuuxqts\fR file limits the number of simultaneous \fIuuxqt\fR
programs running;
it contains an ASCII number.
The installation procedure sets the number to two;
the administrator may want to change this number to meet local needs.
If you get a lot of traffic from mail or netnews, you may want to increase
the number to decrease wait time.
But remember, the more you have running, the higher the load on the system.
.H 3 "Maxuuscheds File"
The \fI/usr/lib/uucp/Maxuuscheds\fR file limits the number of simultaneous \fIuusched\fR
programs running;
it contains an ASCII number.
Each \fIuusched\fR running will have one
\fIuucico\fR associated with it;
limiting the number will throttle the load on the system.
The limit should be less than the number of outgoing lines used
by uucp;
a smaller number is often desirable.
The installation procedure sets the number to two;
the administrator may want to change this number to meet local needs.
But remember, the more you have running, the higher the load on the system.
.H 3 "remote.unknown Shell"
This shell program is called when a remote site that is not in the
\fISystems\fR file calls in to start a conversation.
The shell distributed with the system appends the name and time
information to a file, \fI/usr/spool/uucp/.Admin/Foreign\fR.
The calling of the shell can be turned off by an option in parms.h.
In addition, the shell can be modified to meet local needs.
As distributed, it contains the following:
.DS I
FOREIGN=/usr/spool/uucp/.Admin/Foreign
echo "`date`: call from system $1" >> $FOREIGN
.P
Since it is a shell, it can be easily modified.
For example, it can send mail to the administrator.
.DE
.H 3 "uudemon Shells"
.P
.H 2 "ADMINISTRATION"
.P
The role of the
\fIuucp\fR
administrator depends heavily
on the amount of traffic that enters or leaves a system and
the quality of the connections that can be made to and from that system.
For the average system, only a modest amount of traffic (100 to 200 files
per day) pass through the system and little if any
intervention with the
\fIuucp\fR
automatic cleanup functions is necessary.
Systems that pass large numbers of files
(200 to 10,000)
may require more attention when problems occur.
The following parts describe the routine administrative
tasks that must be performed by the administrator
or are automatically performed by
the
\fIuucp\fR
package.
The part on problems describes what
are the most frequent problems and how to effectively
deal with them.
.H 3 "Cleanup"
.P
The biggest problem
in a dialup network like
\fIuucp\fR
is dealing with the backlog of jobs that cannot
be transmitted to other systems.
The following cleanup activities should be routinely performed.
.H 4 "Cleanup of Undeliverable Jobs"
.P
The \fIuustat\fR program should be invoked regularly to give information
about the status of connection to various machines, and the size and
age of the queued requests.
The \fBuudemon.admin\fR shell should be started by \fBcron\fR(1)
at least once per day\*(EMthis will send the administrator the
current status.
Of particular interest are the age (in days)
of the oldest request in each queue,
the number of times failure to reach that system has occurred, and
the reason for failure.
In addition, the age of the oldest execution request (X. file) is
also given.
.P
Each spool directory will contain some
X. files, C. files, and D. files.
When work can not be accomplished, these files should be
removed.
The \fBuucleanup\fR program, which is run from
\fBuudemon.cleanup\fR will provide this function.
Options to \fBuucleanup\fR specify the age for sending a
warning message to the requestor and age for deleting the
various file.
Note that before deleting, the program tries to figure out
what the job was (e.g. a mail job)
and if possible, tries to send it to the receiver, rather than
the sender.
If this is not possible, it gets returned to the sender.
For plain file transfers, the requestor is informed of the
file name(s), date, and destination of the request.
.H 4 "Cleanup of the Public Area"
.P
In order to keep the local file system from overflowing
when files are sent to the public area,
the \fBuudemon.cleanup\fR procedure is set up with a
\fBfind\fR command to remove any files that are older than 7 days
and directories that are empty.
This interval may need to be shortened if
there is not sufficient space to devote to the public area.
.P
Since the spool directories are very dynamic, they may grow
large before transfers take place, it is a good idea to
reorganize the structure.
The best way to do this is to put some code in \fI/etc/rc\fR to 
be executed once per week.
For example
.DS I
#
#	clean up /usr/spool/uucp
#	Most cleanup is now done by uudemon.cleanup
#	so just do copy out and back
#
DY=`date +'%w'`
HR=`date +'%H'`
echo "cleanup uucp spool directories"
echo "FULL CLEANUP ONLY ON MONDAY before 9am "
if [ $DY -eq 1 -a $HR -lt 9 ]
then
    echo "MONDAY--DO FULL CLEANUP!"
    cd /usr/spool/uucp
    mkdir ../nuucp
    chown uucp ../nuucp
    chgrp uucp ../nuucp
    find . -print|cpio -pdml ../nuucp
    cd ..
    mv uucp ouucp
    mv nuucp uucp
    rm -rf ouucp
fi
chown uucp /dev/cu[al]*
chgrp uucp /dev/cu[al]*
chmod 0644 /dev/cul*
chmod 0222 /dev/cua*

#	cleanup uucp LCK files so that a new
#	process doesn't accidently get a pid
#	corresponding to a left around LCK file--
#	thus preventing it from being removed.
echo "cleanup /usr/spool/locks"
rm -f /usr/spool/locks/*

echo "done uucp directory cleanup"


.DE
.H 4 "Compaction of Log Files"
.P
This version of \fBuucp\fR has individual log files for each system
and each program (e.g. system \fIeagle\fR has a logfile for
\fIuucico\fR requests and a logfile for \fIuuxqt\fR execution requests).
The \fIuulog\fR shell gives the user access to the information in these
files by system name.
These files are combined and stored in directory
\fI/usr/lib/uucp/.Old\fR whenever \fBuudemon.cleanup\fR
is executed.
The daemon saves two days files; this can be easily changed by the
administrator.
If space is a problem, the administrator might consider reducing the
number of days the files are kept, or modify the shell to compact
the files using the \fBpack\fR command.
.H 3 "Polling Other Systems"
.P
Systems that are passive members of the network
must be polled by other systems
in order for their files to be sent.
This can be arranged by using the
\fBuudemon.poll\fR shell.
\fBUudemon.poll\fR read the \fI/usr/lib/uucp/Poll\fR file which contains
the systems and times to poll them.
The lines contain the name of the remote to call followed by a TAB
character and then a space separated list of times to poll.
For example,
.sp
	eagle        0 4 8 12 16 20
.sp
will provide polling of system eagle every four hours.
Note that \fBuudemon.poll\fR does not actually do the poll, it merely
sets up a polling C. file in the spool directory that will be
seen by the scheduler started by \fBuudemon.hour\fR.

.H 3 Problems
.P
The following sections list the most frequent problems
that appear on systems that make heavy use of
\fBuucp\fR(1).
.H 4 "Out of Space"
.P
The file system used to spool
incoming or outgoing jobs
can run out of space and prevent jobs from being spawned
or received from remote systems.
The inability to receive jobs is the worse of the two conditions.
When file space does become available, the system will be
flooded
with the backlog of traffic.
.H 4 "Bad ACU and Modems"
.P
The ACU and incoming modems
occasionally cause problems that make it difficult to contact
other systems or to receive files.
These problems are usually readily identifiable since
the status files accessed by \fBuustat\fR give counts and
reasons for contact failure.
If a bad line is suspected, it is useful to use
the
\fBcu\fR(1)
command to
try calling another system using the suspected line.
This method could also be used to check the login/password
information and phone number.
.H 4 "Administrative Problems"
.P
Some
\fBuucp\fR
networks have so many members that it is difficult
to keep track of changing passwords,
changing phone numbers,
or changing logins on remote systems.
This can be a very costly problem
since ACU's will be tied up calling a system that cannot be reached.
.H 2 "DEBUGGING"
.P
In order to verify that a system on the network can be contacted,
the \fBuucico\fR
daemon can be invoked from a user's terminal directly.
A shell procedure \fBUutry\fR is distributed for this purpose;
the administrator will have to install it in an appropriate place.
For example, to verify that
\fImhtsd\fR
can be contacted, try
.DS I
Uutry mhtsd
.DE
This will start the transfer daemon (\fBuucico\fR) with a moderate
amount of debugging output, putting the output into a temporary
file (/tmp/mhtsa) and executing a \fItail -f\fR command so the
administrator can hit a BREAK to get back to the shell, while
being able to come back at a later time to look at the output.
.P
If that works, the administrator can attempt to transfer a file
while watching the debugging output.
Procede as follows
.DS I
uucp -r some\-file mhtsd!~/some\-name
.DE
This will queue a job but not start the transfer daemon.
Now proceed as before using \fBUutry\fR.
If any of these steps fail, a support person may be needed to
diagnose the problem.
The output of the above steps will make it easier.
.P
The file \fI/usr/spool/uucp/.Admin/errors\fR
contains error indications,
conditions that causes one of the \fBuucp\fR programs to abort
(\fIASSERT\fR errors).
An explanation of these is given in
in Appendix II along with an explanation of the messages one
can see as output from \fBuustat -q\fR.
Most of these will never occur and indicate that something is wrong
with your system or the \fBuucp\fR software.
However, the \fIPKXSTART\fR will occur and generally means that the
other side aborted during a conversation in a non recoverable way;
these can be generally ignored.

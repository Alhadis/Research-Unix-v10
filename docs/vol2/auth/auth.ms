.so ../ADM/mac
.XX authmgr 531 "Authmgr \(em An Authentication Service for Datakit"
.TL
Authmgr \(em An Authentication Service for Datakit
.AU
David Cohrs
.AI
Computer Sciences Dept.
.br
University of Wisconsin \(em Madison*
.AB
.PP
\fIAuthmgr\fP is a service available on Ninth Edition
.UX
systems for authenticating users.
It can be used to authenticate users inside an organization, to
authenticate a terminal or workstation, or to help protect the
organization from network intrusions, while allowing legitimate
outside users to access the organization's computers.
\fIAuthmgr\fP receives calls from users, challenges them for a password
or for some other response, such as the outcome of encrypting
some challenge message.
When the user is authenticated, \fIauthmgr\fP makes use of the redial
mechanism provided by Datakit to redial the newly authenticated
call to a new service, such as a remote login service.
This paper describes the design and implementation of \fIauthmgr\fP,
as well as the re-implementation of call redial in the research
Datakit control computer, and the administrative options used
to implement \fIauthmgr\fP's policy decisions.
.AE
.2C
.FS
* This work was performed while the author was a summer intern at
.MH
.FE
.NH 1
Introduction
.PP
\fIAuthmgr\fP allows users to authenticate themselves and to make authenticated
calls within a trusted hardware environment, even if their call
originated outside that trusted environment.
To make authenticated calls, \fIauthmgr\fP makes use of a redial mechanism
supported by the network interface.
It was written to work in a Datakit environment, however,
any virtual circuit network
that supports the necessary redial mechanism could use \fIauthmgr\fP.
\fIAuthmgr\fP itself runs on a
.I "security computer" ,
currently a 10th Edition
.UX
system.
It interfaces with the Datakit network via the Datakit
interface manager (also running on the
security computer) and the
control channel from the security computer to
the Datakit node to which it is attached.
.PP
\fIAuthmgr\fP serves three basic purposes.
First, it allows users from outside the trusted part of the network
(the
.I "untrusted domain" )
to access the trusted part (the
.I "trusted domain" ),
without the use of ``in the clear'' passwords.
This helps to protect legitimate users in the untrusted domain
from other, less honorable, users also in the untrusted domain.
.PP
Second, all calls from outside the trusted domain can be shunted
through to the authentication manager, rather than allowing them
to be placed directly, as shown is Figure 1.
This allows a security check on the untrusted user before allowing
them to even attempt to access any service provided in the
trusted domain.
If the user fails the check, they have no way of accessing any of
the services inside the trusted domain, even those unprotected
services that are available to all users inside the trusted domain.
This is useful in the environment at AT&T Bell Laboratories, where the
trusted domain is made up of the various labs inside the company, and
the untrusted domain is the experimental XUNET network, connecting
the Murray Hill location to a number of universities around the
country.
.1C
.KF
.so fig1.x
.DS C
Figure 1.  Rerouted call from an untrusted trunk
.DE
.KE
.2C
.PP
The third purpose of \fIauthmgr\fP is to allow users of
Gnot terminals to authenticate themselves to the network (equivalent
to logging into a host computer), and have
that terminal remain authenticated throughout the duration of their
session, as shown in Figure 2.
The Gnot terminal|reference(locanthi gnot)
is a small computer with a video display and a network
interface, but no disk.
When powered on or reset, the Gnot makes a network call to a file server
and retrieves an operating system kernel that
allows it to run window systems, editors, and make network connections
to other computers.
The session terminates when the user resets or powers down the Gnot.
The file server needs to have an authenticated username associated
with a session, in order that it may do privilege checking.
That is
.I authmgr 's
job.
.1C
.KF
.so fig2.x
.DS C
Figure 2.  Authenticating a Gnot
.DE
.KE
.2C
.........
.NH 2
The Software Architecture
.PP
Providing the three services described above required
implementing the authentication manager
itself, and also making changes to the software on the Datakit
control computer.
The control computer changes allow it to redial authenticated calls,
to redirect calls coming in over untrusted trunks and
to store the authentication information for the Gnots.
The authentication manager handles all interactions with the
user, obtaining a login name and a unique authenticator from the user.
The controller software must be able to pass back the dialstring
to the originating point of the call and either redial the call from
that point, or
store the authentication information if
the caller was a Gnot.
In addition, the trunk module software must be able to
automatically redirect calls that come from an untrusted domain
to the authentication manager.
Implementing these functions required changes to both the trunk
module processes and the host computer module processes.
.PP
An implementation of the redial mechanism had previously been
done by Wetzel|reference(wetzel radian)
for the generic Radian controller, which is slightly different from the
Datakit controller used inside Murray Hill, so redial was
reimplemented, with some improvements, for the research controller.
Because of the direct application of redial we had in mind,
the redial mechanism is currently supported only for hosts and
on trunks (a Gnot looks like a host to the Datakit controller).
If a user at a normal terminal attempts to call \fIauthmgr\fP
and then redial their call, the redial will fail, because the necessary
synchronization and redial code has not been added to
the terminal module software.
.PP
The paper proceeds as follows.
The second section describes the functions of
the authentication manager and its implementation.
The third section describes the redial mechanism implemented
for the Datakit, and how it is
used in conjunction with the authentication manager.
The administrative hooks that were added to the system are also described
in this section.
Some final comments and conclusions constitute the fourth section.
.NH 1
The Authentication Manager
.PP
The purpose of the authentication manager is to determine
whether a user is really who they claim to be.
One common way of determining the authenticity of a user is
by asking them to enter a password (once we know the username
they claim to be).
The problem with using this approach over a network is that
the password
is usually sent ``in the clear'', although it isn't echoed
at the user's terminal.
If someone manages to tap into the user's line or the network
itself, the password will be compromised.
Obviously, a simple password is not a good way to authenticate
users over an untrusted network.
.PP
A simple solution is to ask the user for the appropriate response
to some challenge, something that can be sent as normal,
character data, but won't be easily compromised.
This is the approach taken by \fIauthmgr\fP.
Currently, the challenge that \fIauthmgr\fP transmits is a decimal
number, up to 8 digits long.
The user must obtain an encryption calculator from the system
administrators to encrypt this number.
\fIAuthmgr\fP was designed so that it could be quickly extended
to support any manufacturer's encryption
calculator, but at the current time, only one is supported.
This box, an Atalla
.I Confidante
terminal (despite the designation ``terminal'', the calculator is a small,
2" \(mu 3" \(mu \(14" box), is programmed to encrypt
data with a key, the key having been
entered into the terminal by the system administrator at
the time the terminal is assigned.
The 
.I Confidante
box uses standard DES encryption to encrypt the
numeric data with this key.
The terminal also requires a password, which the user must enter
before encrypting the challenge.
When the user enters the challenge, the terminal displays an 8 hex
digit number, which the user enters at their keyboard.
Assuming this was the correct response, \fIauthmgr\fP can proceed.
.PP
\fIAuthmgr\fP also allows some users to enter the normal login and
password sequence they are used to.
This is useful inside the trusted organization, where the problem
of compromising a password are much less, and
carrying around a challenge box becomes a bother.
This option can be specified on the basis of the source of the call,
or on a per-user basis.
.PP
Another important choice in designing \fIauthmgr\fP was whether calls coming
from outside the trusted domain should go first to the service the
untrusted user requested (for example, the user could request to
log into a trusted machine, and the call would go directly to the
remote login service on that machine), and have that service call
\fIauthmgr\fP for authentication purposes, or whether calls should automatically
be rerouted to \fIauthmgr\fP.
The former case would have required no changes to the network software,
but is much too trusting.
Many services in the trusted domain (AT&T Bell Labs, in this case)
were written assuming that only the trusted domain existed.
Allowing untrusted users to access these services could breach the
security of the trusted domain, or give away information that the
trusted domain is not allowed to give out (due to licensing or other
similar restrictions).
.PP
In the interest of the security of all of the services in the trusted
domain, \fIauthmgr\fP was designed assuming that calls from outside its
domain would be rerouted to \fIauthmgr\fP first, before they could reach any
unprotected services.
This requires that the networking software reroute calls, and also
requires that \fIauthmgr\fP be able to redial a call to a new destination
once authentication is completed.
The benefits of reducing the chances of information leaking out
of the trusted domain far outweigh the added complexity to the
networking software.
In addition, none of the existing services need to change to benefit
from the added authentication step.
.NH 2
Authmgr Operation
.PP
\fIAuthmgr\fP operates in three different modes, depending on the command
line arguments and its environment at the time it is executed.
In the first mode, \fIauthmgr\fP assumes it is interacting with a human
user at a terminal.
This mode is used when a user connects directly to \fIauthmgr\fP.
The user is prompted for a login name (unless \fIauthmgr\fP can determine
the user's login name from its environment), and then is challenged
to encrypt a number or enter a password.
The user encrypts the number using their encryption calculator and
in any case, enters the response.
\fIAuthmgr\fP tests this against the correct response, and, if it is correct,
prompts the user for a new destination.
The user enters a new destination, and \fIauthmgr\fP attempts to redial
the call.
Figure 3 shows an example of such a session.
.1C
.KF
.sp .5v
.DS B
.CW
coma(cohrs): con -l security.security
Security Authentication check

login: cohrs
Enter response code for 31746735: 41a46474

Number please: seki.whoami
source=dk!nj/astro/security user=cohrs line=astro2.57.27.F
Eof
coma(cohrs):
.R
.DE
.DS C
Figure 3.  A simple, interactive Authentication Session
.DE
.KE
.2C
.PP
The second form is also interactive, but is used when the user is
using a Gnot terminal.
In this mode, the Gnot, when it is powered up,
automatically connects to \fIauthmgr\fP using
a special service name.
\fIAuthmgr\fP again prompts the user for a login name and a challenge.
However, when the user correctly enters the response, \fIauthmgr\fP
does not ask for another destination.
Instead, it redials the call to a special destination that the
network software understands as being an ``authentication only''
message.
When the source receives the redial message, it saves the authenticated
name and location of the user in a safe place, and closes the connection
to \fIauthmgr\fP.
Until the Gnot is reset,
all subsequent calls go out using this authenticated login name and
security ID.
.PP
The third form is meant to be used in conjunction with the various
remote login services available on the Datakit networks, especially
the
.CW dcon
service (used by the
.I con
program),
and the call rerouting option available at
the trunk from an untrusted domain of the network to the trusted domain.
Here, an untrusted user requests to connect to the remote login service of
a computer in the trusted domain.
When the call reaches the trunk to the trusted domain, the control
process on the trunk reroutes the call to \fIauthmgr\fP (the details of
this will be explained later).
When \fIauthmgr\fP is executed to handle the call, one of the parameters in
its environment is the original destination of the call, namely the
login service of some host in the trusted domain.
.PP
\fIAuthmgr\fP sends out a special message understood by
.I con .
.I Con
prompts the user for a login name, and sends this back to \fIauthmgr\fP.
\fIAuthmgr\fP uses the login name to find the key (or password) for this
user, and sends back the appropriate challenge.
.I Con
requests that the
user encrypt the challenge or enter a password, if this is a connection
inside the trusted domain.
Once it receives the information,
.I con
sends a message back to \fIauthmgr\fP
containing the response from the user.
If this is the correct response, the remote login protocol continues
as normal, otherwise, \fIauthmgr\fP again sends a challenge, and the cycle
repeats.
An example of this usage is shown in Figure 4.
While the actual prompts are similar, it should be noted that con
is prompting the user for the data in this example, not
\fIauthmgr\fP; \fIauthmgr\fP communicates with con over the network connection.
Pseudocode for \fIauthmgr\fP's side of the protocol with
.I con
is shown
in Figure 5.
.1C
.KF
.DS
.CW
fishonaplatter(cohrs): con seki
login: cohrs
Enter response for 55374202: 57406251
seki(cohrs):
.R
.DE
.DS C
Figure 4.  A Rerouted Call to a Remote Login Service
.DE
.KE
.2C
.PP
\fIAuthmgr\fP is executed once for each session that requires authentication.
Upon startup, it reads some initial configuration information from
its configuration file.
This includes such things as the maximum number of tries a user gets
before \fIauthmgr\fP closes the connection, and mappings between original
source identifiers and new security identifiers.
This configuration file is explained in detail in the accompanying
manual page for \fIauthmgr\fP, found at the end of this paper.
.PP
The keys are also stored in a file on the security computer's disk.
This file contains extremely sensitive information, because the
keys are necessarily stored unencrypted, unlike the passwords in the
.UX
password file.
For this reason, access to the security computer must be severely
limited, not even allowing a remote login service.
At the present time, the key file is stored in a protected place,
but the file itself is not encrypted.
In some future version on \fIauthmgr\fP, this file could be encrypted, but
that would require human intervention to enter the encryption key
when \fIauthmgr\fP starts up.
For the purpose of simplicity, and to remove the need for human
intervention, the
current version relies on the physical security of the keys file.
.PP
The final aspect of \fIauthmgr\fP's operation is its use of the redial
feature.
\fIAuthmgr\fP redials the call through use of a new
.I ipcredial
library call in the Tenth Edition
.UX
IPC library.
\fIAuthmgr\fP generates a message containing the authenticated name of
the user, the new authenticated security identifier, and the
new destination to redial.
For security reasons, the ipcredial only works for the superuser.
Once the call has been redialed, either successfully or
unsuccessfully, the connection from \fIauthmgr\fP to the remote user
is closed, and \fIauthmgr\fP exits.
All in all, implementing \fIauthmgr\fP itself was straightforward,
and the system support was easily adapted to the redial application.
.1C
.KF bottom
.DS
.CW
int d;
char user[];
char response[];
char challenge[];

con!"CH";\h'8n'/* the magic challenge protocol string */
*[
\h'8n':: con?user ->
\h'16n'challenge = makechallenge(getkeyinfo(user));
\h'16n'con!challenge;
\h'16n'con?response;
\h'16n'encrypt(getkeyinfo(user),challenge) == response ->
\h'24n'redial();\h'8n'/* \fIauthmgr\fP exits */
\h'16n'con!"CH";
\h'8n':: con?EOF ->
\h'16n'exit();
]
.R
.DE
.DS C
Figure 5.  The \fIAuthmgr\fP Remote Login Protocol
.DE
.KE
.2C
.NH 1
The Datakit Redial Mechanism
.PP
The redial mechanism itself is supported in the network control
software.
In the case of the Datakit, where this work was done, this
control software exists inside the network itself, in the
Datakit nodes.
Each Datakit node has an associated control computer; the
research Datakit nodes use Digital PDP-11/23 style computers for
the control computer, which directly controls the switch memory
of the Datakit node.
The redial mechanism is implemented as part of the software
running on this control computer.
.PP
Each device that connects to the Datakit network connects to
the Datakit node via a module, an interface card that lives
on the Datakit backplane.
A process inside the Datakit controller is in charge of
controlling this module.
In addition, if the module is multiplexed, there is a line
process in the Datakit controller to control each line of
the multiplexer (for our purposes, we can consider almost any
module to be multiplexed, especially the host and trunk
modules, in which a line is a single, logical channel on the module).
.PP
Datakit includes a standard signaling protocol, called VLP,
which is used
for setting up and taking down calls.
VLP defines the interactions between
the control processes for the source and destination modules
in the Datakit controller.
There are additional control messages between the line and
control processes within the same module, but these are not
defined as part of VLP.
.PP
Implementing the redial mechanism in the Datakit controller
required changing VLP, and adding two new messages,
CALLMOD and SIREQ.
These messages are defined to occur only when a connection
has been established; in most control processes, this is called
the TALKING state.
CALLMOD is a completely new message and is used for passing
the new dialstring back to the point at which the call will
be redialed.
SIREQ already exists in the generic Datakit controller,
and is used by the splice mechanism, but did not exist
in the research controller.
This message informs the originating point of the call (the
end of the original connection that still exists in the
new, redialed connection) that it needs to resynchronize its
higher level protocols with its new peer.
.PP
Basically, a redial consists of two phases.
The first phase uses CALLMOD messages to pass the new dialstring
from the module that wants to redial the call to the module
that will actually redial the call.
At the same time, this portion of the original connection is
closed down.
When the CALLMOD message reaches the
.I "redial point" ,
the module
that should actually redial the call, the line process for
this connection at that module generates a CALL message to
open up a new connection to the new dialstring.
This is the second phase of the redial.
The redial point eventually receives an ANSWER or a NAK message back from
the new destination of the call.
This part of the protocol works just as if this were a normal
call.
.PP
When the ANSWER reaches the redial point, instead of
propagating it back to the source, we instead send the source
an SIREQ message, telling it to resynchronize its protocols,
and the new connection is established.
The sending of the SIREQ message here differs from Wetzel's
redial implementation, which assumes that the higher level
protocols are using URP, and puts an URP initialization request
into the datastream.
We considered this to be incorrect, and possibly harmful to
correct communication, and opted for fully integrating redial
into the VLP protocol instead, even to the point of reinitializing
the source of the original call.
.1C
.KF
.so fig6.x
.DS C
Figure 6.  Redialing a call on Datakit using VLP
.DE
.KE
.2C
.PP
Figure 6 shows an example of these two phases of call redial.
In this example, a user on the host
.CW coma
has connected to the host
.CW security ,
presumably to use \fIauthmgr\fP (this step is labeled 1 in the figure).
After authentication, \fIauthmgr\fP redials the call from itself to
the host named
.CW west .
This causes the connection from the node1 Datakit to the security
host to close (this step is labeled 2, and is phase 1 of the redial).
When the redial request reaches node1, the control process starts
a new call to the host
.CW west .
This call completes in step 3, and now the user on
.CW coma
is connected to
.CW west .
.PP
In the example, the redial took place on the originating Datakit
node.
This makes for the most optimal path from
.CW coma
to
.CW west .
If we had performed the redial at node2 instead of node1, all messages
between
.CW coma
and
.CW west
would have had to go through 3 Datakit
nodes, rather than 2, increasing the delay and increasing the
load on node2 unnecessarily.
However, in cases where the original call comes from an untrusted
domain, we would want the redial message (phase 1) to be intercepted
by the node at the edge of the trusted domain, and have the new
call be placed from there.
.PP
This scenario is shown in Figure 7.
Here,
.CW pokey
is a computer in the untrusted domain, and is connected
to the Datakit node, nodex.
Once again, the user on pokey calls the security host, and has their
call redialed to
.CW west
(steps 1 and 2).
However, node1 intercepts the redial message and performs phase 2
of the redial itself, rather than forwarding it on to nodex, which
it doesn't trust.
After the call is redialed from node1, the connection is resynchronized,
and
.CW pokey
is now connected to
.CW west .
.1C
.KF top
.so fig7.x
.DS C
Figure 7.  Intercepting a redialed call
.DE
.KE
.2C
.NH 2
Message Flow to Implement Call Redial
.PP
The message flow to implement call redial is more involved than
forwarding on the CALLMOD, CALL and SIREQ message, due to the
software structure of the Datakit controller.
All VLP messages travel out-of-band of the data, and messages
between module pairs, such as the two sides of a trunk, must
travel via the control channel between these two sides.
This amounts to a 4 way communication to pass VLP information
around in the network.
I will first detail the messages necessary to implement
phase 1 of the redial, both from the host to the Datakit, and
between trunks, and terminating in the initiation of a new call.
Next, I will explain the messages necessary to resynchronize
the connection.
You may want to compare this implementation with Wetzel's.
.......
.NH 3
Phase 1 Host Control Module Messages
.PP
Initiating a redial from a host requires the use of two Datakit
channels, the host's control channel, and the data channel
that will be redialed.
All changes on the host were done in user level servers, no
kernel modifications were necessary, not even to the stream
handlers.
Because the host modifications were straightforward and were
not made by me, I will not detail them here.
.PP
The message sequence is shown in Figure 8.
When \fIauthmgr\fP redials its connection, it uses the
.I ipcredial
library call to pass the call back to the Datakit call
manager
.I dkmgr
on the security host.
The call manager passes a control message of type T_CHG over the
Datakit control
connection containing a D_REDIAL request and the line identifier
for the data channel (step 1).
The control process, unixcscp, notifies the line process, unixp, identified
in the D_REDIAL message of the redial, with an EXTFUNC (for extended
function) message, with subtype D_REDIAL (step 2).
When it receives this message, the line process changes the switch
setting in the Datakit so when the host sends data on the data channel,
the line process will receive this data, rather than having the data
sent to the originating end of the call.
The line process notifies
.I dkmgr
that it is ready for the dialstring
by sending an ``O'' character as a dialtone (step 3).
The host responds with the dialstring, and closes its end of the
data channel (step 4).
The dialstring sent by the host is exactly the same as the dialstring
used when placing new calls; no changes were made to the format.
.1C
.KF bottom
.so fig8.x
.DS C
Figure 8.  Phase 1 Host Control Messages
.DE
.KE
.2C
.PP
Unixp is not the process that should actually perform the redial,
because it was the destination of the old call, so it must forward
the new dialstring back along the path of the call to the process
that originated the call, or a trunk process that will intercept
the redial.
It does this in step 5, by sending a CALLMOD message, which contains
the new dialstring, to the module that originated the call.
Unixp then hangs up its end of the old connection by sending a
HANGUP message to unixcscp in step 6.
This ends the host's involvement in phase 1 of the redial.
.NH 3
Phase 1 Trunk Control Module Messages
.PP
The redial in the trunk is even more complicated than that of the
host, because the trunk must be able to both pass a redial through,
if it is not the redial point, or intercept the redial message,
if this trunk goes to an untrusted domain.
Once again, there are four processes involved, the control processes
on each side of the trunk, tdkp, and the line processes, tdktrkp,
which control the data channel of this connection.
.PP
Figure 9 shows the control flow for a trunk that passes CALLMOD
requests through, unchanged.
First, the tdktrkp on the redialing side receives a CALLMOD from
a unixp or another tdktrkp.
It wants to pass this on through the trunk, but must use the
control process, tdkp to do so, and sends tdkp an EXTFUNC/D_REDIAL
message (step 2).
Tdkp responds to this by sending a T_CHG/D_REDIAL message to
its peer on the other side of the trunk, along with the line
identifier for the data channel (step 3).
The remote tdkp sends an EXTFUNC/D_REDIAL message down to
the correct line process (step 4), which causes that tdktrkp
to give a dialtone for the new dialstring (step 5).
The redialing tdktrkp responds by sending the dialstring (step 6).
It is now completed with this call, but waits for the remote
tdktrkp to complete sending the dialstring on.
.1C
.KF
.so fig9.x
.DS C
Figure 9.  Phase 1 Trunk Control Messages
.DE
.KE
.2C
.PP
The remote tdktrkp once again packages up the dialstring in a
CALLMOD message, which it sends on to its peer (presumably the
line process for another trunk or a host) in step 7.
The remote tdktrkp is also finished with this call
and sends a CLOSE to its trkp (step 8).
This causes the normal tear down to take place between the trunk
processes, which eventually causes an ONHOOK message to be received
by the other tdktrkp (step 9),
which then finishes cleaning up this connection.
In this way, the CALLMOD gets forwarded through the network,
and the old portion of the call gets torn down at the same time.
.NH 3
Phase 2 Trunk Control Module Messages
.PP
Eventually, the CALLMOD reaches the originating host control
process or a trunk to an untrusted domain, and phase 2 begins.
Because phase 2 is more interesting in the intercept case,
the trunk intercept case is described here
The host redial case is similar, and somewhat simpler.
.PP
Figure 10 shows the messages involved in phase 2 of a trunk
that intercepts the redial message.
The trunk first receives the CALLMOD message from its peer, just
like it would if it were forwarding the redial.
However, when it checks its administrative database, it determines
that it should intercept the redial.
It sends out a new CALL message to the new destination (step 2)
and waits for a response.
In this example, the call succeeds, and the tdktrkp receives
an ANSWER message from its new peer (step 3).
.1C
.KF
.so fig10.x
.DS C
Figure 10.  Phase 2 Trunk Control Messages
.DE
.KE
.2C
.PP
In Wetzel's redial implementation, phase two ends here.
However, we really need to send back to the originating host,
via the control channels, a resynchronization message.
Wetzel's method of inserting control bytes into the data stream
is unacceptable \- a trunk process cannot know the high level
protocol the endpoints were using, and assuming that they use URP,
while correct in the current state of the Datakit, may not work
in the long run.
In addition, this choice violates the abstraction presented by VLP.
.PP
Phase 2 continues by telling the originating host to resynchronize
its high level protocols with the new peer.
Tdktrkp, after it receives the ANSWER, notifies its tdkp about
this with an SIREQ message (step 4).
The tdkp then sends a T_CHG/D_XINIT message across the control channel
to its peer on the other side of the trunk (step 5), along with
the line identifier for the data channel that needs resynchronization.
The remote tdkp notifies the correct tdktrkp line process with an
SIREQ message as well (step 6).
The receiving tdktrkp forwards the synchronization message along
by sending an SIREQ message to its peer module (step 7).
If step 1 were the receipt of an SIREQ rather than a CALLMOD, the
sequence would be the same, without the CALL or ANSWER.
.NH 3
Phase 2 Host Control Module Messages at the Originating Host
.PP
When a line process on a host interface module receives an SIREQ
message from its peer (either a trunk or another host module),
it must notify the host itself of the synchronization request.
This message sequence is shown in Figure 11.
The unixp process receives the SIREQ (step 1), and sends an
SIREQ to its controlling unixcscp (step 2).
In the generic Datakit, the splice operation resynchronizes a host
by sending it a T_SRV/D_XINIT message.
Because this message does not depend on the splice operation at
all, we use it here.
Unixcscp sends the host a T_SRV/D_XINIT message over the control
channel.
A modification was necessary in the streams modules in the
kernel to process this message.
This message is intercepted by the streams modules,
and causes the data channel to resynchronize (it resets sequence
numbers and any other necessary end-to-end negotiations).
At this point, the redial is completed.
.1C
.KF bottom
.so fig11.x
.DS C
Figure 11.  Phase 2 Host Control Messages
.DE
.KE
.2C
.NH 2
Administrative Changes to Implement Call Redial
.PP
The redial mechanism, by itself, is not enough to support \fIauthmgr\fP.
Additional changes were necessary in the administrative and
configuration portions of the Datakit control software.
One of these changes, the redial intercept, has already been mentioned.
This and other changes will be detailed in this section.
.PP
In order to restrict the use of the redial operation, a new option
was added to the host module configuration description.
This option is a simple boolean, which says whether the host is
allowed to issue redial operations or not.
There are two reasons for this.
First, to support \fIauthmgr\fP, the redial dialstring from the host includes
not only the name of the new destination, but an authenticated
username and the authenticated security ID for \fIauthmgr\fP.
Only the security host should be allowed to send out such messages.
In general, only the Datakit is allowed to put the security ID into
the dialstring.
Furthermore, allowing anyone to perform a redial confuses the
authentication system already in place.
A redialed call from anyone but \fIauthmgr\fP would contain the username
of the
.I redialer ,
not the username of the original caller (the other end of the connection),
and the security ID of the redialed call would be that of the
redialer as well.
This would foil the authentication checks made by the recipient of
the redialed call, making the redialed call appear as if it came from
the host that redialed the call, rather than from the original caller.
For this reason, redial is completely restricted to specific, secure
hosts, and only the superuser (\fIauthmgr\fP runs as the superuser) can
take full advantage of the redial feature.
.PP
Another boolean option was added to the host module configuration
description to support the authentication of Gnots.
In this case, we wish the entire module to retain the knowledge that
this Gnot is currently authenticated.
Therefore, on a module that is connected to a Gnot rather than a
regular,
.UX
host, the Datakit administrator can set this option to retain the
authentication information.
When a CALLMOD request comes into a unixp line process on such a
module, it stores the authenticated username and security ID in
the control database.
All further calls from the Gnot use these values for the username
and security ID instead of anything the Gnot or the normal database
entries contain.
When the Gnot is reset, or its power is cycled, this information is
erased, and the next user will have to be re-authenticated.
.PP
The previously mentioned redial intercept option was added to the
trunk module configuration description.
This is also a boolean, and, if set, marks the trunk as
untrusted.
If a CALLMOD arrives from another module on this side of the
trunk, it will be intercepted, and the redial will take place
at the trunk, rather than being forwarded.
.PP
Two more configuration options are available for the trunk.
The first is necessary to guarantee the authenticity of \fIauthmgr\fP's
security ID.
The Datakit administrator can enter \fIauthmgr\fP's security ID into
one of the fields in
the configuration for an untrusted trunk.
When a call comes in over the untrusted trunk, the security ID
on the call is compared with the security ID of \fIauthmgr\fP.
If it is the same, the security ID on the call is changed to
be that of someone who is completely unknown and untrusted.
This keeps unscrupulous people in the untrusted domain from
making fake, ``authenticated'' calls through the trunk.
.PP
The other option protects the integrity of the trusted domain.
Here, the Datakit administrator can define a destination through
which all calls from the untrusted trunk should be
.I rerouted .
When this field is set (this is known as the Predefined Destination
field) on a trunk, any call arriving from the trunk will be
rerouted to this predefined destination.
The name of the old destination, as specified in the dialstring,
is also passed along.
This is used, for example, to reroute all calls from an untrusted
domain to \fIauthmgr\fP.
In this way, no service in the trusted domain can be accessed from
outside without authenticating the untrusted user
first.
.NH 2
Experience
.PP
In implementing call redial, a good portion of the time was spent reading
the existing code to implement other parts of VLP in the host and
trunk control modules and also studying Wetzel's call redial implementation.
Implementing the basic call redial itself was straightforward,
especially given the references.
Making the administrative modifications work and integrating
them with the
control processes themselves was the most challenging.
.PP
The worst part of the implementation
was in saving the authentication information for the Gnots.
The database inside the Datakit control computer is highly structured,
and is not extendible.
That is, it is impossible, without making major changes to the database
code and structure, to add new data types and objects to the database.
Therefore, the authenticated username and security ID were stored
in two database objects that were otherwise unused for the database
information on host interface modules.
The actual purpose of these objects was meant to be somewhat
different than the
way they have been applied in this case.
A better solution would have been to modify the database structure,
but there was no time for such a major job this summer.
.NH 1
Conclusions
.PP
\fIAuthmgr\fP is, as far as I know, the first major application of call
redial in Datakit.
It allows a trusted domain to interact with a number of untrusted
domains, while not compromising the trust implicit inside
the trusted domain.
The additional use of \fIauthmgr\fP in authenticating Gnots shows its
versatility, and that it can centralize the authentication tasks
inside the trusted domain as well.
The re-implemented redial mechanism points out a problem with
resynchronization in the generic datakit
redial mechanism that should be addressed at some point.
The ease with which \fIauthmgr\fP and call redial fit into the large
body of existing networking and datakit control code was
extremely encouraging, and the structure difficulties within
the control database should be fixable in future versions of
the datakit control software.
.NH 1
Acknowledgements
.PP
This work would have been impossible to complete in one summer
without the direction of my mentor, David Presotto, and the
help of Bill Marshall and Caryl Carr.
I extend my thanks to them and to the various other members
of 1127 that helped make this happen.
.NH
References
.LP
|reference_placement

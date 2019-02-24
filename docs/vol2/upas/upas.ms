.so ../ADM/mac
.XX upas 557 "Upas \(em A Simpler Approach to Network Mail"
.TL
Upas \(em a Simpler Approach to Network Mail
.AU
David L. Presotto
William R. Cheswick
.AI
.MH
.AB
.I Upas*
is a mail interface that routes messages between existing
network-specific mailers, users, and user mailboxes.
It uses a language based on regular expressions describe
how to convert mail
addresses into the commands needed to route the mail to the intended
destination.
Upas is the mail interface for the Tenth Edition
.UX
system.
.AE
.2C
.FS
*
.B upas ,
.I "u\(aapas, n" .
(in full
.B u\(aapas-tree\(aa ),
a fabulous Javanese tree that poisoned everything for miles
around; Javanese tree (\c
.I "Antiaris toxicara" ,
of the mulberry family): the poison of its latex.
[Malay, poison.]
|reference(dictionary chambers)
.FE
.NH 1
Introduction
.PP
Our entry in the `mail race' sprang from events similar to those
motivating the development of many mail systems.
For many years a short and simple mailer was used to deliver local mail 
and to route mail via our home-grown networks.
Although its user interface left a little to be desired, its reliability
was so high that great trust was put into it.
However, as we gained access to more and more networks, particularly ones
over which we had no control, the situation quickly deteriorated.
Each of these networks had their own mail `standards' and addressing conventions.
With some trepidation, we absorbed these standards into our mailer.
Its simplicity was quickly lost along with its fabled reliability.
Realizing our danger, we decided to step back and see if there was a
way to get back to a simple, well-understood, and thereby reliable mail system.
.PP
The job to be performed by a network mail system is illustrated by Figure 1.
A mail system is essentially a large switch for handling the routing and
delivery of messages.
As a router it must be conversant in the various network protocols,
be able to decipher destination addresses, and pass messages along
to the next network.
Sometimes it actually gets to deliver a piece of mail to a mailbox.
Also, since there is no common mail format, the mail system must
convert messages from one format to another as it routes them from
network to network.
Because of the number of networks and mail formats, this can easily lead to
thousands of lines of code.
Our task was to decide how to partition the task in order to create a
manageable yet efficient mail system.
.1C
.KF
.PS
define net |
[ellipse "network" "$1";
	arrow -> from last ellipse.s down boxht/3;
	box invis ht boxht/3 "protocol";
	arrow -> from last box.s down boxht/3;
	box invis ht boxht/3 "convert";
	arrow -> from last box.s down boxht/3;
	box invis;
	arrow -> down boxht/3;
	box invis ht boxht/3 "queue";
	arrow -> from last box.s down boxht/3;
	box invis ht boxht/3 "convert";
	arrow -> from last box.s down boxht/3;
	box invis ht boxht/3 "protocol";
	arrow -> from last box.s down boxht/3;
	ellipse "network" "$1";
] |

# network mail
NetA: net(A)
move right boxwid/4
NetB: net(B)
move right boxwid/4
NetC: net(C)

# local mail
move to NetA.w + 0,boxht/3;
arrow <- left;
ellipse "user";
move to NetC.e + 0,boxht/3;
arrow -> right;
ellipse "mail" "box"

# the router
box dashed wid 3*boxwid at NetB + 0,boxht/3 "routing"
.PE
.sp .5
.ce
\fBFigure 1.\fP  The functions to be performed to route network mail.
.sp
.KE
.2C
.NH 1
Some Observations
.PP
The task of interfacing to a particular network is often
messy and arbitrary.
Fortunately,  most entities (corporations, governments, committees)
that design network protocols also provide code (i.e. mail programs)
that understand these protocols.
In our experience, it has
always been easier to interface one of these mailers to our
mail system than to incorporate the new protocols
into our existing mailer.
Also, code provided by someone else is supported by someone else.
As network protocols change it is easier to pick up the new version of the
network mailer than to rewrite our mailer.
.PP
Although there are many networks, there are far fewer message formats.
It is clear that a message needs a destination address and possibly even
a reply address.
However, the imposition of further structure on the message is at best
distasteful, at worst obstructive.
Imagine what postal delivery would be like if the Postal Service opened
each piece of mail to ensure that it is correctly dated and signed,
that the form of address is correct, and that the company letterhead
obeys some preconceived format,
refusing delivery if any of these conditions are not met.
Unfortunately, some networks impose such requirements.
For a message to obey one standard is difficult enough.
To expect it to survive a number of conversions between
restrictive standards constitutes wishful thinking.
Because of this, most networks adopt standards established by
older or larger networks.
Therefore, although there are many networks, there are relatively few
message formats.
.PP
A network address describes a path through a number of machines
and networks.
This path may be rather simple, consisting of a single machine
and user name.
Often, however, the path crosses a number of administrative domains.
Each such domain imposes some rules for structuring paths within the
domain.
Unfortunately, there is no adhered-to standard
for binding the path segments from each domain into a single
address.
The networks differ on direction of binding (person@machine vs. machine!person),
delimiters (`.' vs. `@' vs. `%'), quotation marks, and even case sensitivity.
Therefore, there is no fixed way to correctly parse and understand a 
network address.
Instead, there are conventions which tend to be very short-lived,
usually until someone issues a new RFC or a new network appears.
As a relatively simple example, consider a message sent from one \fIuucp\fP
|reference(uucp v7man network)
network, through ARPAnet, to another \fIuucp\fP network.
The address format might be something like:
.P1
A!B!person%E%D@C
.P2
The rules for parsing such an address are easily defined.
Unfortunately, the conventions underlying the rules change from day to day.
Once you've managed to write your code, the administrator
at B may decide that he won't accept percent signs in an address
and would really like the address to look like:
.P1
A!B!@d,@e:person@c
.P2
A new set of parsing rules now have to be defined.
In our experience these changes happen with maddening frequency.
They are the direct result of there being no single comprehensive
standard or administrative authority.
Therefore, we have to treat address parsing rules as
ephemeral.
Any network mailer should be able to change its address parsing rules
frequently and with little difficulty.
Tying them to one particular standard such as this week's Internet rules is
equivalent to planned obsolescence.
.PP
Finally, we should make a point about reversibility that many other
mail designers seem to have missed.
In addition to parsing destination addresses, mailers are expected to
maintain some form of return address attached to the message.
This often involves changing the current return address to one
that the mailer will accept as a reply destination.
A mailer should parse and modify return addresses using the same rules
as it does for destination addresses.
Otherwise, as is too often the case, the mailer will reject the very addresses that
it has provided for replies.
.NH 1
A Solution
.PP
The best solution would have been to throw out all the so-called standards and
create a single coherent scheme for formatting and addressing mail.|reference(hideous pike weinberger)
However, since we have no power to impose such a scheme,
we have tried to use the above-stated requirements and observations
to build a mail system that makes the best of a bad situation.
.PP
The structure of our mail system is depicted in figure 2.
Each network has its own interface program for message reception
and transmission.
In general these are the network-specific mailers provided
with the networks.
When a message enters from a network, the network
specific mailer gives it to Upas.
Upas then either deposits the mail in a local mail box or routes the
mail to the next network.
A format-specific filter may be called to convert the message
from network format to one Upas understands or vice-versa;
The
.UX
format is built in.
.1C
.KF
.PS 5i
copy "over.cip"
.PE
.sp .5
.ce 2
\fBFigure 2.\fP  The structure of Upas.
.sp
.KE
.2C
.NH 1
Message Routing
.PP
The routing of messages is determined by a destination address
and by a set of rewriting rules kept in the file
.CW /usr/lib/upas/rewrite .
Each line of the file is a rule.
Blank lines and lines beginning with
.CW #
are ignored.
.nr ss \w'conversion  '
.PP
Each rewriting rule consists of four fields:
.IP \fIpattern\fR \n(ssu
An
.I ed (1)-like
regular expression, with simple parentheses playing the role
of 
.CW \e(
and
.CW \e)
and with the 
.CW +
and
.CW ?
operators of
.I egrep (1).  
This regular expression must match the entire destination
address.  Case is ignored.
.IP \fIcommand\fR \n(ssu
One of the following rewrite commands:
.I alias ,
.I auth ,
.I translate .
.I | ,
or
.I >> .
.IP \fIparameter\fR \n(ssu
An
.I ed (1)-style
replacement string to generate a
parameter to the
.I command .
.IP \fIaddress-list\fR \n(ssu
A list of addresses that might be shipped with a single command.
.PP
The
.I pattern ,
.I parameter ,
and
.I address-list
fields may use the following:
.KS
.IP \f(CW\es\fP  \n(ssu
The address of the sender.
.IP \f(CW\el\fP  \n(ssu
The name of the local machine.
.IP \f(CW&\fP \n(ssu
The entire destination address.
.KE
.PP
The 
.I parameters
and
.I address-list
fields may use
.CW \e0
through
.CW \e9
to match the first ten parenthesized groups matched in the
.I pattern
field.
.PP
When rewriting a destination address,
Upas starts with the first rule and continues
down the list until a pattern
matches the destination address.
The command on that line is executed.
If no match is found, the
mail is returned to sender with an error.
If the command does not result in mail delivery
(i.e is not 
.CW |
or
.CW >> ),
Upas scans the rules again with the latest version of
the destination address, starting from the first rule.
.1C
.KF
.P1
# local mail
[^!@%]+			translate	"exec translate '&'"
local!([^!@%]+)		>>		/usr/spool/mail/\e1
\el!(.+)			alias		\e1

# convert %@ format to ! format
(_822_)!((.+)!)?([^!]+)[%@]([^!%@]+)	alias	\e1!\e2\e5!\e4
([^!]+)[%@]([^!@%]+)			alias	_822_!\e2!\e1
_822_!(.+)				alias	\e1

# special domain names
([^!.]+)\e.(att\e.com|uucp)!(.+)	alias	\e1!\e3
	
([^!]+)!(.+)		|	"/usr/lib/upas/route '\es' '\e1'" "'\e2'"
.P2
.sp .5
.ce 2
\fBFigure 3.\fP  Sample rewrite file for a machine using \fIuucp\fP only.
.sp
.KE
.2C
.PP
There are five rewrite commands:
.IP \fIalias\fR \n(ssu
Rewrites the address with the pattern
in the
.I parameter
field.
.IP \fIauth\fR \n(ssu
Calls
.I parameter
to authorize the mail.
A zero exit status approves the mail, non-zero rejects it.
The 
.I auth
command is called only once per message.
If it is never called, the mail is approved.
.IP \fItranslate\fR \n(ssu
Calls
.I parameter
to rewrite the address.  The program must write the new
address(es) to standard output.  This command is used to implement
mailing lists.
.IP \f(CW|\fP \n(ssu
Pipe the message to the mail delivery agent
.I parameter .
The 
.I address-list
parameter is a list of recipients with the same destination
machine.  If the delivery agent fails, the message is
returned to the sender with the error message from the 
delivery program's standard error file.
.IP \f(CW>>\fP \n(ssu
Deliver the message to a local mailbox.  The file given in
.I parameter
must either exist and appear to be a valid mailbox,
or the last name in the path must be a user name found in
.CW /etc/passwd .
.PP
Rules for most networks can be specified in one or two lines.
In addition, the rules are in a language familiar to most
experienced
.UX
programmers: the regular expressions
seen in many editors, languages, and utilities.
By using such a mini-language, it becomes an easy task to build or
modify Upas configuration files.
The result is that configuration files rarely contain gross
mistakes and take very little time to create
and to edit when addressing conventions change.
Further, the rewrite file is reread for each new mail delivery, so a 
change to the rewrite file will take effect immediately.
.NH 1
SMTP Message Format Conversion
.PP
Upas uses only the 
.I uucp -style 
addressing internally.
The mail delivery program must convert between this
form and its own, if different.  For example, the 
.I smtpd
daemon must convert incoming RFC822 addresses to
.I uucp
form when calling
Upas, and the
.I smtp
program generates header lines on outgoing mail.
.PP
The outbound conversion to SMTP format is required by RFC822. Specifically,
three header lines are required:  
.CW Date: ,
.CW To: ,
and one of several variants of
.CW From: .
If the message appears to have these header lines, and the lines are
formatted properly,  the message is sent unaltered.
For example, if there is an original 
.CW From:
line with an address in the requested domain, it is left alone.  Otherwise, we
generate a 
.CW From:
line and turn any existing one into
.CW Original-From: .
Missing information is filled in from the Unix-style 
.CW From
line.
.PP
We do not add other header lines to mail.
These provide extra bulk (over ten percent
in one of our surveys) with little added utility.
In particular,
.CW Received:
lines are only rarely useful, and the information they provide appears
in our log files.
.PP
Incoming SMTP destination addresses are derived from the
envelope addresses and header information.
The senders address is extracted from the first of the following header lines found:
.UX
.CW From ,
.CW Reply-to: ,
.CW Sender: ,
.CW From: ,
and the sender given in the SMTP
.CW "MAIL FROM:"
command.
.PP
The early versions
handled uucp and SMTP addressing internally.  Later, SMTP was broken
out into two pairs of filters:
.I smtpd
and
.I fromsmtp ,
and
.I tosmtp
and
.I smtp .
.I Fromsmtp
and
.I tosmtp
were filters that extracted and created RFC822 addressing and headers,
respectively.  Recently, these filters were folded into 
.I smtpd
and
.I smtp
for efficiency reasons.
.NH 1
User Control
.PP
Users often wish to specify alternate ways to dispose of their mail.
Upas offers two choices.
The first line of a user's mail
file is interpreted as a command to the mail system.
If the line is of the format
.P1
Forward to \fIlist-of-addresses
.P2
the mail is forwarded to each recipient in 
.I list-of-addresses.
While this can be used to forward a single user's mail, it
can be also be used to create mailing lists.
To do this, one creates a file in the mail directory whose name is
that of the mailing list and which consists of
.CW "Forward to"
followed by the list of recipients.
.PP
If the first line is of the format is
.P1
Pipe to \fIshell-command
.P2
.I shell-command
is executed when mail is delivered, with the message as standard input.
.NH 1
Concealing Machine Names
.PP
It is often useful to hide several machines behind a single mail machine.
For example, our center has over 50 machines, but all mail is directed
through the machine named
.CW research .
The files
.CW /usr/lib/upas/names.*
contain routing information for each user.  A sample entry
might be:
.P1
andrew	pipe!andrew
.P2
Mail sent to 
.CW research!andrew
will be directed to
.CW pipe ,
.CW andrew 's
home machine.  But mail from
.CW andrew
should appear to come from
.CW research ,
not
.CW pipe .
.PP
To hide names, Upas attempts to translate the last field of the
sender's address.  If the translation exactly matches the entire
sending address, the sending address is truncated to the last field.
.NH 1
Loop Detection
.PP
Detecting forward loops, like those provoked by
.CW "Forward to"
is difficult.
It involves combining the forwarding lists of all involved machines
into a single directed graph and then performing a search or
partitioning to detect cycles.
However, if we allow a detection algorithm to reject some legal although
highly-unlikely cases along with real loops, we greatly simplify the problem.
.PP
In the case of a single machine,
an infinite forwarding loop corresponds to
infinite recursion of the mailer.
If a mailer rejects any message that results in recursion past a
certain depth, it will reject all loops and some small number of legal
but very long mail redirections.
In our case a depth is 32 and to date, no legal forwarding loop has been
more than 3 steps long.
.PP
In the case of a multi-machine loop, the recursion technique is not valid.
However, we can still use a similar method.
Instead of counting recursion, we scan the
.CW From
line to see the number
of times the local machine name occurs in the path.
If this exceeds a limit (in our case 8), the mail is returned to the sender.
.NH 1
Installation
.PP
Upas has been ported to most major versions of the
.UX
system.
The source contains a
.CW config
directory where the working Upas directories are specified.
Each variant of Upas is made from a separate directory with
.I make (1).
The
.CW makefile
may require some editing to select the needed programs.
The
.CW config
directory contains a number of sample
rewrite and routing files.
.NH 1
A Comparison With Sendmail
.PP
Upas is an attempt to solve the same problem previously attacked by Sendmail
|reference(sendmail).
Upas owes much of its design and success to Sendmail.
The idea of designing Upas as a central switcher
communicating with network-specific mailers comes directly from Sendmail.
The reasons we wrote Upas and didn't just adopt Sendmail are:
.IP \(bu
We strongly favor messages whose only formatted portion are the
destination and reply addresses.
Sendmail has an unfortunate predilection for verbose and rigidly-structured
messages that we would like to avoid.
.IP \(bu
Sendmail configuration files are famous for their inscrutability.
We wanted a system that had simpler and therefore more easily
verifiable rewriting rules.
.IP \(bu
Sendmail combines the functions of routing, queuing, aliasing,
transmission, header
processing, delivery, translation, etc., into a single large program.
This extra design makes Sendmail more complicated and harder
to understand and support.  Upas's modular design simplifies these
tasks.
.IP \(bu
The size of sendmail has left it prone to several security
problems, some intentional.  It is easier to understand and
check a smaller, more modular program.
.NH 1
Lessons
.PP
The philosophy behind Upas has not changed much since its original
description in |reference(upas presotto),
but there have been many implementation changes.
The rewrite file now has five commands compared to the original
generalized command execution.  Removing case sensitivity, and
anchoring the pattern matches by default has made them more versatile
and easier to read.
.PP
The early Upas understood
.I uucp
and SMTP addresses and formatting.
The SMTP portions are now broken out in separate programs,
simplifying the processing.  The
.I uucp -style
address has 
proven quite easy to teach and to use.  For example,
.P1 2n
bitnet!templevm!rdk
.P2
is much easier to teach and use than
.P1 2n
rdk%templevm.bitnet@cunyvm.cuny.edu
.P2
.PP
Authorization was implemented with a file lookup of trusted machines.
Now, a command can implement arbitrary policies.
.NH 1
Summary
.PP
We have presented a simple yet flexible network mail system.
It gains its simplicity from a number of assumptions which are 
valid in most networked computers.
By using existing network-specific mailers as expert systems
that deal with network details, Upas itself remains relatively
simple and understandable.
Finally, by using a mini-language already
familiar to most
.UX
programmers, Upas is easily modified
to respond to changes in the name space and topology of the
network.
.PP
Upas has run at Research and on the AT&T Internet gateway for
nearly two years now.  It has performed well in these demanding
environments, adjusting nicely to the changes.  Its flexibility
comes at the cost of efficiency.  Even so, we have handled nearly
four thousand messages per day on a VAX 750 with reasonable, if
not spectacular, throughput.
.NH 1
Acknowledgements
.PP
Many people have contributed to the success of Upas.  MIT supplied the
original SMTP code, which was improved by many people. 
Bill Cheswick, Geoff Collyer, Ian Darwin, Peter Honeyman, Dave Presotto,
and Dennis Ritchie have all had a hand in the code.  We have received
helpful feedback from 
Steven Bellovin,
Jonathan Clark,
and
Marcel Frank-Simon.
.NH 1
References
.LP
|reference_placement

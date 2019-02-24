.so ../ADM/mac
.Tm index
.XX 48 667 "Index"
.Tm index
.TL
Index
.AU
L. L. Cherry
.2C
.NH
Introduction
.PP
The part of the
.UX
operating system that deals with terminals
and other character devices
has always been complicated.
In recent versions of the system it has become even more so, for
two reasons.
.IP 1)
Network connections require protocols more ornate than are
easily accommodated in the existing structure.
A notion of ``line disciplines'' was only partially successful,
mostly because in the traditional system only one line discipline
can be active at a time.
.IP 2)
The fundamental data structure of the traditional character I/O system,
a queue of individual characters (the ``clist''),
is costly because it accepts and dispenses characters one at a time.
Attempts to avoid overhead by bypassing the mechanism entirely
or by introducing
.I "ad hoc
routines succeeded in speeding up the
code at the expense of regularity.
.LP
Patchwork solutions to specific problems were
destroying the modularity of this part of the system.
The time was ripe to redo the whole thing.
This paper describes the new organization.
.PP
The system described here runs on about 20 machines
in the Information Sciences Research Division of Bell Laboratories.
Although it is being investigated by other parts of Bell Labs,
it is not generally available.
.Tm overview
.NH
Overview
.PP
This section summarizes the nomenclature, components, and mechanisms
of the new I/O system.
.NH 2
.Tm streams
.I "Streams
.PP
A
.I "stream
is a full-duplex connection between a user's process and a device
or pseudo-device.
It consists of several linearly connected processing modules,
and is analogous to a Shell pipeline, except that
data flows in both directions.
The modules in a stream communicate almost exclusively
by passing messages to their neighbors.
Except for some conventional variables used for flow control, modules do not
require access to the storage of their neighbors.
Moreover, a module provides only one entry point to each neighbor, namely
a routine that accepts messages.
.PP
At the end of the stream closest to the process
is a set of routines that provide the interface to the rest of the system.
A user's
.I "write
and
I/O control requests are turned into messages sent to the stream,
and
.I "read
requests take data from the stream and pass it to the user.
At the other end of the stream is a 
device driver module.
Here, data arriving from the stream is sent to the device;
characters and state transitions detected by the device are
composed into messages and sent into the stream towards the user program.
Intermediate modules process the messages in various ways.
.PP
The two end modules in a stream become connected automatically when
the device is opened;
intermediate modules are attached dynamically by request of the user's program.
Stream processing modules are
symmetrical; their read and write interfaces are identical.
.NH 2
.Tm queues
.I "Queues
.PP
Each stream processing module consists of a pair of
.Tm queues
.I "queues,
one for each direction.
A queue comprises not only a data queue proper, but also two routines
and some status information.
One routine is the
.I "put procedure,
which is called by its neighbor
to place messages on the data queue.
The other, the
.I "service procedure,
is scheduled to execute whenever there is work for it to do.
The status information includes a pointer to the next queue downstream,
various flags, and a pointer to additional state information required
by the instantiation of the queue.
.Tm queues
Queues are allocated in such a way that the routines associated with
one half of a stream module may find the queue associated with the other half.
(This is used, for example, in generating echos for terminal input.)
.NH 2
.Tm message blocks
.I "Message blocks
.Tm queues
.PP
The objects passed between queues are blocks obtained from an allocator.
Each contains a
.I "read pointer,
a
.I "write pointer,
and a
.I "limit pointer,
which specify respectively the beginning of information being passed, its end,
and a bound on the extent to which the write pointer may be increased.
.PP
The header of a block specifies its type; the most common blocks contain
data.
There are also control blocks of various kinds,
all with the same form as data blocks and obtained from the same
allocator.
For example, there are control blocks to introduce delimiters
into the data stream, to pass user I/O control requests, and to announce
special conditions such as line break and carrier loss on terminal
devices.
.PP
Although data blocks arrive in discrete units
at the processing modules,
boundaries between them are semantically insignificant;
standard subroutines may try to coalesce adjacent
data blocks in the same queue.
Control blocks, however, are never coalesced.
.NH 2
.Tm scheduling
.I "Scheduling
.PP
Although each queue module behaves in some ways like a separate process,
it is not a real process; the system saves no state information
for a queue module that is not running.
In particular queue processing routines do not block when they cannot proceed,
but must explicitly return control.
A queue may be
.I "enabled
by mechanisms described below.
When a queue becomes enabled, the system will, as soon as convenient,
call its service procedure entry,
which removes successive blocks
from the associated data queue, processes them, and places them on the
next queue by calling its put procedure.
When there are no more blocks to process, or when the next queue becomes
full, the service procedure returns to the system.
Any special state information must be saved explicitly.
.PP
Standard routines make enabling of queue modules largely automatic.
For example, the routine that puts a block on a queue
enables the queue service routine if the queue was empty.

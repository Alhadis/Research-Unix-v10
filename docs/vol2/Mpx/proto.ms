.so ../ADM/mac
.XX 39 593 "Blit down-load and communications protocols"
.de UL
.lg 0
.if n .ul
\%\&\\$3\f(HI\\$1\f1\&\\$2
.lg
..
.EG
.TL
Blit down-load and communications protocols
.AU
Rob Pike
.AI
.MH
.AB
A description of the protocols used in the Blit software to communicate
between host and terminal.
Four protocols are presented:
the two down-load protocols,
the
.I mpx
communication protocol,
and the layer control protocol between
.I mpx
and
.I mpxterm .
User-level protocols such as in
.I jim ,
.I jx
and
.I joff
are not described.
.AE
.2C
.NH
Byte order
.PP
All data representations in the Blit protocols are 68000 style,
that is, high byte first in a word, high word first in a long-word.
This convention was chosen because Blits may be run on a variety
of host computers, but it was thought there would be only one
processor in the Blit.
Fortunately, both processors used in Blits have the same byte order.
Regarding program binaries, which are processor-dependent,
the following
.I 68ld
protocol descriptions are for the 68000 and may not
apply to the W32000 (and
.I 32ld ?).
.NH
\f468ld\fP protocol for stand-alone down-loading
.PP
.I 68ld
sends a control-P (octal
.CW 020 )
character to initiate down-loading.
The protocol is an error-corrected packet protocol,
running in one of three modes, according to
.I 68ld
options.
The default runs without CRC's or acknowledgements, and is therefore
not error-corrected, but runs significantly faster than the other
modes, due to scheduling turn-around, on some Unix systems.
.CW "68ld -x"
generates and checks CRC's, but the terminal
replies with an ACK packet only after the last data packet.
.CW "68ld -e"
is fully error-corrected, with an ACK for each packet.
.PP
Each packet is formatted like this:
.DS
.UL "<typ^seq> <size> <address>"
.UL "[<data bytes>]"
.UL "[<CRC1> <CRC2>]"
.DE
where the notation
.CW <>
signifies one byte and
.CW []
a optional sequence of bytes.
.PP
The maximum length of the entire packet, including CRC's, is 128 bytes.
.CW typ
specifies the ac\%knowledg\%ment handling for each packet,
although in practise all packets have the same type.
.CW typ=0x80
implies full error correction,
.CW typ=0x40
implies no error correction or CRC's,
and
.CW typ=0xC0
implies only error detection, with a final ACK packet.
.UL seq
is a 6-bit sequence number.
.UL size
is the number of address and data bytes in the packet, excluding header and CRC.
All packets contain the 4-byte address to which the data should be copied.
If
.UL size
is 4,
which is only true on the last packet of the down-load,
.UL address
is the boot address of the program;
receipt of this packet is the signal to begin execution.
If
.UL size
is greater than 4,
the data bytes are part of the program.
If the packet is correctly received, the terminal returns
an ACK, consisting of
the first
.UL typ^seq
byte from the packet.
The CRC is sent low byte first,
and applies to the entire packet except the two CRC bytes.
The polynomial used is (16, 15, 2, 0).
Timeouts or
out-of-sequence ACKs cause the retransmission of
all un-acknowledged packets.
.NH
\f468ld\fP protocol for down-loading \f4mpxterm\fP binaries
.PP
.I 68ld
issues a
.CW JMPX
ioctl to determine that the standard output is an
.I mpx
channel.
It then issues a
.CW JBOOT
ioctl to initiate the down-load protocol.
If
.I 68ld
is invoked with the
.CW -z
option,
.CW JZOMBOOT
is instead sent, signifying that after down-loading the process is to
be suspended until
.I joff
starts it running.  The protocol is the same in either case.
.PP
.I 68ld
next sends the arguments to the program.
It first sends the argument count,
including the program name itself.
It next sends the number of characters in the complete list of arguments,
including a null character terminating each argument string.
It then sends 3 numbers defining the program size:
the text size, data size and bss size, in that order.
All these integers are 32 bits long.
.I mpx
answers with the 32-bit address at which the program
will be loaded.  Text, data and bss are contiguous inside the Blit.
If the returned address is zero,
there was insufficient memory in the terminal and
.I 68ld
must send a
.CW JTERM
ioctl to restore the terminal state.
.PP
.I 68ld
next sends the argument strings, each terminated by a null.
Then, it sends the relocated binary in two sections, transmitted identically:
text and data.  Bss is zeroed out by the Blit, and is not transmitted.
Text and data are sent as unformatted byte streams,
whose combined length must agree with the length of the sections
as reported earlier in the protocol.
When the Blit has received all the characters, execution begins.
.PP
The protocol is not error-corrected, because it runs over
an error-corrected communications channel provided by
.I mpx .
.NH
\f4mpx\fP communications protocol
.PP
Communication between
.I mpx
and
.I mpxterm
is on an eight-bit byte, eight-channel, error-corrected packet-multiplexed protocol.
The protocol is symmetrical with respect to the host and terminal;
it is implemented by the same source code at both ends.
Each channel is double-buffered, so a process writing on a channel does
not block unless both packet slots on that channel are awaiting acknowledgements.
When the ACK is received, the packet slot is made available.
.PP
The layout of a packet is:
.DS L
.UL "<1:1|cntl:1|seq:3|cbits:3>"
.UL "<dsize:8> <dsize bytes of data>"
.UL "<CRC1> <CRC2>"
.DE
where the notation
.CW <>
signifies a byte and
.CW x:\f2n\fP
signifies
.I n
bits of
.CW x .
Bits are numbered from the high bit of the byte.
The CRC is sent low byte first,
and applies to the entire packet except the two CRC bytes.
The polynomial used is (16, 15, 2, 0).
.PP
The first bit (i.e. high bit of first byte) of a packet is always on.
The next bit
.UL cntl ) (
states whether the packet is a control packet.
The next three bits
.UL seq ) (
are the sequence number, modulo 7,
and the bottom three are the channel number.
.UL dsize
is the number of bytes of data, excluding the entire header and CRC.
The maximum value of
.UL dsize
is 64, although this has been 32 and 128 from time to time.
In retrospect, 64 was a bad choice, because the maximum packet size is not
a power of two.
The maximum number of channels could be increased by moving the
sequence number into the
.UL dsize
byte and giving the extra bits to the channel number.
This was not done originally to simplify playing with the
maximum packet sizes.
.PP
Most packets are data packets, that is, packets sent by a user process
and containing user data.
These all have
.UL cntl
set to zero.
.PP
Control packets, with
.UL cntl
set to one, are always ACKs or NAKs (negative acknowledgements).
If
.UL dsize
is zero, the control packet is an acknowledgement, and signifies that
it and all lesser-sequenced packets have been received correctly.
If
.UL dsize
is non-zero, the first data byte is
.CW 0206
for an ACK, or
.CW 0225
for a NAK.
A NAK is a request for retransmission of the packet and all lesser-sequenced
unacknowledged packets.
A control packet can contain an extra data byte, set by the user.
Originally, the byte was used to hold characters for echoing to the terminal,
so the ACK packet for a typed character could include the echo,
halving the packet traffic.
Now, however, only unblock messages (see next section) are transmitted this way.
.NH
Blit-\f4mpx\fP layer protocol
.PP
On top of the error-corrected communications protocol,
.I mpx
and
.I mpxterm
communicate by an asymmetric protocol to establish new processes,
delete processes, initiate down-loading, etc.
.PP
All messages from the Blit to
.I mpx
begin with a byte stating the nature of the message.
Most commonly, the message contains data to be sent to the associated
Unix process
(since this communication runs above the multiplexed protocol,
the channel number is implicit),
but sometimes the message contains control information for
.I mpx .
The message tags are shown in Figure 1.
.1C
.KF
.TS
center;
c c c
lFCW cFCW l.
=
Name	Value	Meaning
_
C_SENDCHAR	1	Send character to Unix process
C_NEW	2	Create new Unix process group
C_UNBLK	3	Unblock layer process
C_DELETE	4	Delete layer process group
C_EXIT	5	Exit
C_BRAINDEATH	6	Send hangup signal to Unix process group
C_SENDNCHARS	7	Send several characters to Unix process
C_RESHAPE	8	Layer has been reshaped
_
.TE
.ce
\fBFigure 1.\fR Protocol messages
.SP
.KE
.2C
.PP
The following descriptions ignore the control byte, which is
always the zeroth byte of the message.
.CW C_SENDCHAR
sends a single character (the first byte of the message) to the Unix process.
.CW C_SENDNCHARS
sends the number of characters specified by the first byte to the
Unix process; the data starts at the second byte.
.CW C_NEW
creates a new process group (shell) on Unix.
The implicit channel number of the message is used thereafter
for full-duplex communication between the terminal and Unix processes.
The data in the packet
specifies the size of the layer for the terminal process.
The format is:
.DS
.UL "<charx> <chary> <bitsx> <bitsy>"
.DE
where
.I char
is the number of standard-size characters across the window,
and
.I bits
is the number of screen pixels,
within the border.
.CW C_RESHAPE
has the same format as
.CW C_NEW
and specifies the dimensions of a layer after reshaping.
.CW C_DELETE
states that the user deleted the layer,
and that a hangup signal should be sent to the associated process group.
.CW C_BRAINDEATH
says that the terminal program faulted (e.g. generated a bus error)
and that the associated Unix process should be sent a hangup signal.
.CW C_EXIT
states that the user selected the
.CW Exit
button on the
.I mpxterm
menu.
.PP
Although the multiplexed protocol is error-corrected, it may back up,
because the terminal process may not read the packet data out of the
protocol's buffers.
The message
.CW C_UNBLK
is therefore sent when a terminal process has emptied out the packet
buffer so there is room for more data at user level in the terminal.
These messages are only generated when the protocol backs up;
if packets are emptied quickly by the terminal process
(which is usually true), unblocks are unnecessary and are not generated.
.PP
Packets traveling from
.I mpx
to the Blit
are always data to be copied into the terminal process's RCV queue.
Process 0 is the demultiplexor process,
and packets sent to it are
ioctl
messages that must be handled by the terminal:
.CW JBOOT ,
.CW JTERM ,
etc.
A packet sent during initialization, either
.CW JTIMO
or
.CW JMTIMO ,
sets the protocol timeout parameters, because the terminal cannot
sense the baud rate.
.CW JTIMO
(followed by the timeout period in seconds)
is used by Berkeley and Eighth Edition systems;
.CW JMTIMO
(followed by the timeout period in milliseconds)
is used by System V and derivative systems.
I have no idea why there is a difference.

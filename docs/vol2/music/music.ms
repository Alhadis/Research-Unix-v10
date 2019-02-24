.so ../ADM/mac
.XX music 477 "Computer Music Under the 10th Edition UNIX System"
.nr dP 1
.nr dV 1.5p
.de FG
.ce
Figure \\$1. \\$2
.SP .5
..
.TL
Computer Music under the 10th Edition
.UX
System
.AU
T. J. Killian
.AI
.MH
.AB
We describe an evolving computer music system that draws upon many of
the novel facilities in the Research
.UX
system, as well as the standard repertoire of
familiar tools.
The Teletype 5620 bitmap display serves both as the user's
terminal and real-time controller.  The
.I mux
window system is used to download a
.SM MIDI
interface driver that
services other windows (by direct code sharing) and
host processes (which write on the driver's control stream).
We presently support several
.SM MIDI
devices: the Yamaha
.SM DX7 ,
.SM TX816 ,
.SM FB01 ,
and
.SM SPX90 .
.PP
Window programs include a piano-roll-style score facility and a
virtual keyboard.  Host programs include a music compiler,
.I m ,
that converts an
.SM ASCII
score notation into
.SM MIDI
events;
it is based on
.I lex
and
.I yacc,
making it very easy to modify in response to user needs.
There are also several filters that perform simple transformations
(e.g., time and pitch translation) on
.SM MIDI
files.  The latter are
.SM ASCII ,
so that text filters (especially
.I awk ,
.I sed ,
and
.I sort)
can be used for sound manipulation.
.AE
.2C
.NH
Introduction \- the
.SM MIDI
standard.
.PP
Computer-music applications have taken off in recent years, largely
due to the introduction of the
.SM MIDI
(Musical Instrument Digital Interface)
standard |reference(midi standard).
.SM MIDI
has made it possible, with very modest hardware,
to interface a computer to synthesizers and other equipment, with broad
compatibility among manufacturers.
A full discussion of the
.SM MIDI
standard is out of place here; we will simply
give some of its essential characteristics.
.PP
.SM MIDI
uses an asynchronous serial protocol at 31.25 Kbaud to transmit \%8-bit
data bytes over a \%5-mA current loop (thus, to the programmer, it looks like an
.SM RS232
line).  Data flow on any given cable is unidirectional.
.I Status
and
.I data
bytes are distinguished by the high-order bit being set or clear, respectively.
A status byte encodes a
.I function,
and (usually) a
.SM MIDI
.I "channel number"
in the range 1 to 16.
Bytes are grouped logically into
.I messages
that consist of a status byte and (except as noted below) 1 or 2 data bytes.
For example, the \%3-byte message (\c
.CW 0x90,\ 0x3c,\ 0x40 )
says, ``On channel 1,
turn on note number 60 (middle C) with a volume of 64 (mf).''
In most cases the size of a message is determined by its status byte, so
the latter may be omitted if it is the same as the status byte most
recently transmitted.
.PP
The messages most commonly used in performance, such as those that
select a synthesizer's pre-programmed voices, or
turn notes on and off, are fixed by the
.SM MIDI
standard.  An escape mechanism
is provided to allow control sequences
peculiar to the equipment of a given manufacturer.  This
.I "system exclusive"
message has the form (\c
.CW 0xf0,
.I "ident, data,"
\&...,
.CW 0xf7 ");"
.I ident
is a manufacturer's code assigned by the standards committee; the
.I data
is arbitrary (and can be of any length).
.PP
A device typically has three
.SM MIDI
sockets, labeled
.SM IN ,
.SM OUT ,
and
.SM THRU .
Messages received at the
.SM IN
socket are acted upon in real time if the device's
.SM MIDI
channel number matches that of the message.
In any case, the data are buffered directly to the
.SM THRU
socket to allow daisy-chaining.
An instrument such as a
keyboard may send an independent stream of messages to its
.SM OUT
socket.  A typical
.SM MIDI
performance consists of (mostly) note-on and note-off messages
emitted by the ``performer'' with proper timing.
.PP
There are several problems with
.SM MIDI ,
the most serious of which
is probably its limited bandwidth.  Chords consisting of, say, ten or twelve
notes become noticeably arpeggiated, and changes in timbre that might
require a long system-exclusive message cannot be fitted into fast passages.
Lack of flow control makes this problem even worse.
.SM MIDI
is also tightly bound to the twelve-tone Western scale; it is sometimes possible
to work around this, again at the expense of bandwidth.
.NH
The
.SM MIDI
device driver.
.PP
Mark Kahrs and I built a
.SM MIDI
interface board that plugs into the parallel
.SM I/O
port of the Teletype 5620 bitmap terminal |reference(kahrs 5620).
The 5620 has a \%32-bit
microprocessor with 1 Mb of memory, and runs the
.I mux
window system |reference(blit bstj).
Using the 5620 as the real-time controller, we are able
to place a synthesizer under the control of a host computer
(in this case, a
.SM VAX
750), without a large amount of systems programming.
.PP
The
.SM MIDI
device driver (\c
.I midiblt )
is down-loaded into a
.I mux
window, where it takes over interrupt handling on the parallel
.SM I/O
port.  Three types of interrupts are handled:
.SM UART
transmitter and receiver ready,
and a \%5-msec clock.
.SM MIDI
data are organized into three queues.  Incoming messages are time-stamped
and placed on the
.I receiver
queue, where they are available to other software.  At clock interrupt, the
.I scheduler
queue is examined for messages with time less than or equal to the current
time; such messages are moved from the scheduler queue to the
.I transmitter
queue, from which they are sent to the hardware as quickly as possible.
.PP
There is a sharp division between routines that control the
.SM UART
directly
(and handle single characters)
and those that manipulate
.SM MIDI
messages.  For example, the
.SM MIDI
transmitter is
a finite-state machine that understands things like elided status bytes; it
is called by a lower-level routine and produces the next byte to be sent by
the
.SM UART .
.PP
Routines that empty the receiver queue and fill the scheduler queue are
available from outside the driver.
.I Midiblt
places their entry points in a name table maintained by
.I mux ;
since there is no memory mapping in the 5620, they are immediately available
to other programs down-loaded in the terminal.
This code-sharing mechanism is used to implement a virtual-keyboard program,
.I jx7 ,
which provides the functionality of a one-fingered mouse-pianist, with slide
controls for volume, vibrato, pitch bend, etc.
.NH
Communication with the host.
.PP
Host communication takes place via the
.I streams
mechanism |reference(latest streams).  Briefly, each
window is associated with a host control stream managed by
.I mux ;
when a program down-loaded in the terminal does a read or write,
.I mux
performs the appropriate operation on the stream associated with the window.
The program at the other end of the stream can be the shell
(this is how multiple virtual terminals are implemented), or a special-purpose
program (as in the case of a text editor).
.I Midiblt
falls into the second category.  It mounts
its stream under the name \&\c
.CW .MIDI
in the
user's home directory, in effect
creating a character-special file for the
.SM MIDI
driver.  The terminal side of
.I midiblt
reads characters as they appear from the host, assembles them into
.SM MIDI
messages, and places the messages on the scheduler queue.  (The host side of
.I midiblt
does not look at this data; error correction and flow control are performed by
.I mux ).
.PP
We have followed a time-honored
.UX
tradition by formatting the
.SM MIDI
file in
.SM ASCII .
Such a file consists of a series of lines, one per
.SM MIDI
message.
Each line is a sequence of blank-separated decimal numbers, viz.: event time
(msec), status byte, and data.  (Backslash-newline can be used
to break long system-exclusive messages.)
In this form, the entire panoply of
.UX
text-processing tools can be
brought to bear in rough-and-ready fashion.
.PP
On the other hand, this format is not well
suited for direct transmission to the 5620, since bandwidth is at a premium.
The program
.I midi ,
which compresses the data, is used; it replaces (and has similar semantics to)
.CW "cat >.MIDI" .
In addition,
.I midi
attaches itself to the
.I "process group"
associated with the \&\c
.CW .MIDI
stream.  This allows the
.I midiblt
window to send signals to the
.I midi
process so that, e.g., if the user does a reset from the
.I midiblt
menu, there is proper coordination between the host and the driver.
.PP
The
.CW "midi | midiblt"
pattern is repeated in the programs
.I score
and
.I scoreblt ,
which produce a pitch vs. time graph.
.I Scoreblt
manages a terminal window and draws the display.  Through the code-sharing
mechanism mentioned earlier, it has access to
.I thinkblt
which drives a HP ThinkJet dot-matrix printer.
.NH
Synthesizer control.
.PP
The system described was first used to run a Yamaha
.SM DX7
and, later, a
.SM TX816 .
Both produce sound via
.SM FM
synthesis |reference(chowning), a voice being described
by around 100 (digital) parameters, all settable by system-exclusive
.SM MIDI
messages.  The
.SM DX7
has internal memory for 32 pre-loaded voices that can be
selected by number (via the
.SM MIDI
.I "program change"
message).  It is a keyboard instrument with additional controls for
editing voice parameters (the current voice, whether internal or downloaded,
is always copied into an editing buffer).
Although the
.SM DX7
is limited to playing in one voice at a time,
up to 16 simultaneous notes can be produced.  The
.SM TX816
is a rack-mounted
unit consisting of eight
.SM TF1 "'s."
Each
.SM TF1
is essentially a
.SM DX7
without keyboard, and
can be set to a different
.SM MIDI
channel, so that
orchestral and multi-track effects are possible.
.PP
A number of C programs are used for synthesizer configuration.
.I Txchan
assigns channel numbers to the
.SM TF1 "'s."
.I Mecho
is used to send ``constant'' data (such as to select an internal voice, or
alter single parameters of a voice).
.I Dxvoice
downloads voice data from a library file on the host.  Figure 1 is an example
of a shell script that sets up the
.SM TX816
with five instruments, one of which
(the harpsichord) uses two
.SM TF1 "'s"
in parallel.  The violin voice needs to be
downloaded; the rest are already stored in the proper
.SM TF1 "'s"
(they came from the factory this way).
Percent signs delimit comments to
.I mecho .
.1C
.KF top
.P1
txchan 1 1 3 4 5 6 7 8
mecho	init \e
	prog	-c1 28		% harpsichord chan 1	% \e
		-c4 3		% reeds chan 4		% \e
		-c6 24		% flute chan 6		% \e
		-c8 3		% bass pipes chan 8	% \e
	dx	-c8 p144 24	% move up an octave	% \e
	parm	-c4 p4 127	% foot control reeds	% \e
		-c6 p2 127	% breath control flute	% \e
		-c8 p4 127	% foot control pipes	%
dxvoice	-c3 -v2 tx816.8		# solo violin chan 3
.P2
.FG 1 "Setting up the TX816"
.KE
.2C
.PP
This scheme is complete in that it allows access to any
.SM MIDI
or
.SM DX
parameter,
but it is not altogether satisfactory.  The user must know, for example, that
voice 3 is ``reeds'' in the fourth unit, but ``bass pipes'' in the eighth.
Simply assigning names to numbered parameters does not reduce the complexity,
however.  Ideally, one would like an interactive ``orchestration editor''
supported by a large database.
.1C
.KF bottom
.P1
main()
{
	int c, x, y, z, magic = 13, size = 64;
	int mask = 2*size-1;
	for (c = z = 0; c <= mask; c++, z += magic)
		for (newfile(), y=0; y < size; y++)
			if ((x = (y ^ z) & mask) < size)
				play((x+y), (x-y));
}
.P2
.FG 2 "Munching squares"
.KE
.2C
.NH
Musical examples in C and the shell.
.PP
The first piece to be played on our system was written by Cynthia P. Killian
using a combination of C and the Bourne shell.  The composer took
raw material produced by the ``munching squares'' algorithm and manipulated
it by splicing and dubbing techniques.  Figure 2 shows the heart of the algorithm.
.I X
and
.I y
trace out short diagonal segments, which are rotated by 45 degrees and passed to
.I play .
The resulting vertical coordinate is mapped onto a tone row, and the length of
the segment (as determined by a sequence of points at the same height) determines
the length of time the tone is held.
.I Newfile
breaks the output into separate small files (\c
.CW smunch.\fI?? )
for convenience.  The
latter are then processed by shell scripts like that shown in Figure 3.
The unfamiliar commands in the script are either shell scripts or trivial C
programs.
.I Sed
and
.I "sort\ \-n"
form the basis for cutting and pasting.  Operators that do a lot of arithmetic
(e.g,
.I retro ,
which time-reverses its input) are written in C.  The shell syntax for
operator precedence and the semantics of the
.UX
filter are extremely
well matched to this application.
.1C
.KF
.P1
(divider <smunch.06 3 4; divider <smunch.07 1 2
 divider <smunch.07 1 2 |
  invert 0 1120 0 | retro) | ttrans 0 280 >tmp$$
(cat tmp$$
 divider <smunch.08 7 8) | ttrans `endtime <tmp$$` -1260
rm tmp$$
.P2
.FG 3 "Shell script for processing munching squares"
.KE
.KF bottom
.P1
/* Inventio 4 (BWV 775) */
8 = 120		/* tempo: 120 eighth notes/min	*/
3 / 8		/* time signature		*/
{
treble : 1	<      6(16)         | 6(16)         >
		| F60: d3 e f g a b@ | c# b@ a g f e |
bass : 1	<      4.            | 4.            >
		| F60: r             | r             |

treble	< 3(8)          | 3(8)          | 6(16)        >
	| f    a   d4   | g3    c4# e   | d e f g a b@ |
bass	< 6(16)         | 6(16)         | 3(8)         >
	| d2 e f g a b@ | c# b@ a g f e | f   a   d3   |
\&...
}
.P2
.FG 4 "The beginning of a Bach invention"
.KE
.2C
.NH
The M language.
.PP
The need for a closer tie with standard musical notation led to the development
of the M language.  We will try to give a feel for the language with a short
example shown in Figure 4.
.PP
An M file consists of ``front matter'' followed by text enclosed
in matching curly braces.  The comment convention is similar to C's,
except that comments nest.
The music is divided into
.I lines
formed by a
.I "voice name" ,
an optional colon and
.SM MIDI
channel number, a
.I "rhythm list" ,
and corresponding
.I "note list" .
Voice names are arbitrary alphanumeric strings.
Within rhythm and note lists,
measures are delimited by barlines.
A rhythm list consists of
.I "time values"
(\c
.CW \%1 "\&\ =\ whole note,"
.CW \%2 "\&\ =\ half note,"
.CW \%4 "\&\ =\ quarter note,"
.CW \%4. "\&\ =\ dotted quarter note,"
etc.) and
.I rests,
possibly with repeated groups (\c
.CW "3(16 16r)"
means
.\".fp 9 MU Musicpi
.\"\f(MU\N'140'\fP\ \f(MU\N'24'\fP
.\"\f(MU\N'140'\fP\ \f(MU\N'24'\fP
.\"\f(MU\N'140'\fP\ \f(MU\N'24'\fP).
.fp 8 MU Sonata
\f(MU\N'120'\fP\ \f(MU\N'197'\fP
\f(MU\N'120'\fP\ \f(MU\N'197'\fP
\f(MU\N'120'\fP\ \f(MU\N'197'\fP ).
A note list consists of
.I notes ,
.I rests ,
and
.I modifiers .
Notes specify
.I "pitch class"
and, optionally,
.I "octave number" .
Pitch class is indicated in standard letter notation, with accidentals
.CW # ,
.CW @
(flat), and
.CW =
(natural).  The octave is given by a number appearing
after the letter name, e.g.,
.CW c3
is middle c, and
.CW c3# ,
.CW c#3
are a half-tone above.
The octave number changes between b and c, so a half-tone below middle c is
.CW b2 .
A missing octave number defaults to that of the previous note in the same voice.
Rests have time value only and are indicated by
.CW r .
.PP
Modifiers are used to set ``environmental'' parameters; in the example,
the modifier
.CW F60
specifies a ``key force'' (volume) of 60 units.
.PP
Not shown in the example are notations for ties and more complicated rhythms.
Ties map particularly easily onto
.SM MIDI
events.  Observe that a note usually
generates two
.SM MIDI
events, note-on and note-off.  A note with a tie going out
(e.g.,
.CW \%c- "),"
simply loses its note-off event, and one with a tie coming in
(e.g.,
.CW _c ")"
loses its note-on event.
Time values may contain multiplicative expressions, e.g.,
.CW 3(3*4)
is a triplet to a quarter note.
Finally, values that are inconvenient to
generate otherwise can be specified with numerator and denominator, e.g.,
.CW 9/17 .
.PP
Also not shown in the example are possibilities for grouping in the
note list.  Parentheses group a
.I sequence ,
and square brackets group a
.I chord .
E.g., all of the notes in
.CW "[c\ e\ g]"
are sounded simultaneously, and
groupings may be nested, so that
.CW "[(c\ d\ c) (e\ g\ e) (g\ b\ g)]"
has three
sequences running in parallel to make a \%I-V-I progression.
.PP
To facilitate machine generation of M code, an alternative rhythm
notation is allowed: the rhythm list can be omitted, and time values prefixed
to the corresponding notes.  This is readable to humans, but less convenient
for keyboard input (see below).
.PP
M is based on
.I lex
and
.I yacc ,
and its continuing development depends heavily on them.
M is intended to be used by musicians who are not computer scientists, and
who can't always pass on the merits of a feature without testing it.  Hence
the ability to experiment is most important.
.I Lex
in particular has been justly criticized on performance grounds, given that
lexical analyzers are fairly easy to write by hand.  But this is the case
only for a static language, and M is still changing rapidly, particularly in
the area of dynamics control.
.PP
M produces a
.SM MIDI
file on its standard output, so that
.CW "m bach\ |\ midi"
is an example of a common invocation.
It also has options to restrict the output
to certain voices or a range of measures, as a debugging aid.
.NH
The M keyboard interface.
.PP
It is possible to generate the notes of an M program by playing at the
.SM DX7
keyboard.  First the note-on and note-off events are collected by
.I midiblt
and written into a file on the host.  This file is then converted into a
list of note names by
.I unmidi .
We have used the conventions that the
.SM DX7 "'s"
.SM YES
button generates a newline, and the
.SM NO
button produces the comment
.CW "/*?*/" .
This gives the user some control over the format and the ability to flag errors.
We also use the ``portamento'' pedal to generate a barline.
.I Unmidi
has an ``output key'' option to direct it towards desired enharmonic spellings.
The output from
.I unmidi
is fed to an
.I awk
script which adds voice names.  Now only the rhythm
is missing.  Since it is separate from the notes, it
can be typed in quickly by making a second pass over the score.
.NH
Conclusions.
.PP
Twelve-tone pieces such as the one in section 5, and serialized pieces
particularly, would be easier and faster to develop with the aid of specialized
tools.  These could range anywhere from a library of C routines to a
complete language implementation.  We expect to begin on a modest scale
with the former and enlist the efforts of interested composers.
.PP
It is clear that we have barely scratched the surface of
an immensely challenging class
of problems.  The
.UX
system, originally crafted as a home for programmers,
has proven remarkably robust, flexible, and downright hospitable as a base
for a very different application.
.NH
References.
.LP
|reference_placement

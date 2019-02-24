|make(rp)

|insert(../monk/monk.mac)

|comment(VOL2HEADER)|set_counter(page 163-1)|set_string(page_headers "'''")|set_string(even_headers "'Formatting References with Prefer''\f(NIprefer\fP'")|set_string(odd_headers "'\f(NIprefer\fP''Formatting References with Prefer'")|set_string(even_footers "_\\nP__UNIX Papers_")|set_string(odd_footers "'Research Tenth Edition''\\nP'")
|author(name "M. A. Derr", initials MAD, location MH, department 11225,
	extension x4721, room 2D-455)

|date(July 1, 1989)
|title(Formatting References with Prefer)

|begin(abstract)
|i(Prefer) is a |small(UNIX) text processing
tool
that formats bibliographic references.
Using a bibliography database, it scans a document to
convert symbolic references
to citation marks and full references ready for typesetting.
It currently supports a variety of reference formatting
styles including those of the |s(ACM), |s(IEEE), and |s(APA).
|i(Prefer)
runs as a preprocessor to |i(troff)
and can also be invoked by
the |i(monk) text processing package.
This paper describes |i(prefer) and its supporting
tools for creating, indexing,
and retrieving from bibliography databases and for
converting |i(refer)-style databases and documents
for use with |i(prefer).
|end(abstract)


|titlebox

|insert(prefer.cite)

|style(two_column)

|section(Introduction)

|i(Prefer) is a |small(UNIX) text processing
tool|dagnote[in addition to
Tenth Edition |small(UNIX),
|i(prefer) runs under
|small(AT&T UNIX) System V
and 4.3|small(BSD UNIX).]
that formats bibliographic references.
Using a bibliography database, it scans a document to
convert symbolic references
to citation marks and full references ready for typesetting.
Like |i(eqn)|reference(latest eqn) and
|i(tbl)|reference(latest tbl), |i(prefer)
runs as a preprocessor to |i(troff)
|reference(latest troff tutorial).
It can also be invoked by
the |i(monk)|reference(latest monk) text processing package.

|p
|i(Prefer) was designed to replace the |i{refer}
|reference{Lesk indexes} bibliography program.
|i(Refer) is not only lacking in flexibility, with much of
its functionality hardwired into C code,
but the code is difficult to modify and maintain.
|i(Prefer) improves on
|i{refer} and
on |i{bib} |reference{bib}, an earlier
|i{refer} replacement.
It also follows style conventions found in
other bibliography formatters such as |i{Scribe} |reference{scribe},
and |i{BibTeX} |reference{latex} |reference{bibtex}.
It attains flexibility by encoding all style-dependent formatting
information in scripts written in |i{awk} |reference{awk 1988},
a pattern scanning and processing language.
|i{Prefer} currently supports seven reference formatting styles:

|begin(bullet_list)
|item |cw(att) \- used at |small(AT&T) Bell Laboratories
|item |cw(acm) \- Association for Computing Machinery
|item |cw(apa) \- American Psychological Association
|item |cw(ieee) \- Institute of Electrical and Electronics Engineers, Inc.
|item |cw(lsa) \- Linguistic Society of America
|item |cw(pami) \- Pattern Analysis and Machine Intelligence
|item  |cw(spectrum) -\ |small(IEEE) Spectrum
|end(bullet_list)
In addition, users may define their own styles
by supplying an appropriate |i{awk} style script.
|i{Prefer} uses the |cw{att} style by default.
Unless noted, all examples in this paper will be
in |cw{att} style.

|p
To give a flavor of what |i(prefer) can do, here is
a simple example of a document containing citations to
references in a bibliography database.
At the end of the document, the full references are printed.
Here's the document before it is formatted:

|begin(here)
|begin(figure)
|begin(small)
|insert(example/simple.in)
|end(small)
|end(figure)
|end(here)

The command |cw<|sp(|s)reference(awk)> tells |i(prefer)
to search a bibliography database for a reference containing
the word ``awk,'' somewhere
in a database entry, e.g, in the author or title field.
|i(Prefer) will replace
the |cw<|sp(|s)reference>
command with a citation mark that corresponds
to the matching reference.
References from each |cw<|sp(|s)reference>
command are remembered by |i(prefer).
When |i(prefer) reaches the |cw<|sp(|s)reference_placement>
command, references seen so far are formatted
according to the current style.
When the above document (in a file called
|cw(document)) is run through |i(prefer)
and |i(troff):

|figure<|small{|cw(prefer document | troff) |i(-options) |cw(...)}>

the following output is produced:

|p
|begin(here)
|begin(small)
|source(example/simple.t)
|end(small)
|end(here)

|p
In addition to the |i{prefer} program,
several other support tools are available.
|i(Penter)
is an interactive tool for creating
or appending to bibliography databases.
|i(Pinvert) creates an index to the bibliography database for fast retrieval.
|i(Plook) retrieves entries that match search keywords
from a bibliography database.
|i(Pconvert) converts |i(refer)-style documents and databases to
|i(prefer)-style.
The remainder of this
paper describes |i(prefer) and its supporting tools
and provides
examples of their use.

|section(Bibliographic Database)
A bibliography database is a file consisting of
one or more entries separated by newlines.
Each entry is composed of several %attribute/value pairs
providing such information as type, author, title, and date.
For example, here is an entry describing the |i(awk) book
referenced in the previous example.

|begin(here)
|begin(figure)
|begin(small)
%type book
%author Alfred V. Aho
%author Brian W. Kernighan
%author Peter J. Weinberger
%title The |small(AWK) Programming Language
%publisher Addison-Wesley
%address Reading, Massachusetts
%date 1988
|end(small)
|end(figure)
|end(here)

Appendix |remember{types}
provides a complete list of types and
attributes.

|subsection{Building a Bibliography}
You may create or modify a bibliography database using your
favorite text editor or by using the tool,
|i{penter}.|starnote<|i(Penter) is based on a program
written by T.J. Kowalski and H. H. Goguen.>
After invoking |i{penter} with the name of the bibliography file
as the argument (|cw[prefer.out] by default),
|i{penter} prompts for a reference type.
Typing |cw<?> to this prompt
gets you a list of legal types.
Typing |cw<help> to the same prompt will get
you a subprompt for a reference type who's attributes
will be listed.  Answering |cw<?> to this subprompt
will list attributes for each type.
|p
Each reference type has some required attributes and some optional
attributes.
After you respond to the initial reference type prompt
with a legal type, |i(penter) prompts for each
attribute associated with that type.
Some attributes have
default values, shown in  brackets, |cw([) |i(default) |cw(]).
Defaults may be accepted
by typing a return, skipped by typing spaces before
the return, or overridden by typing a new value.
For attributes without defaults,
typing a value followed by return
enters that value, while typing return, skips the attribute.
Required attributes may not be skipped.
The character |cw<&> appended to an attribute value
causes the prompt for the current attribute to be repeated.
This allows multiple authors or editors to be entered
or allows titles to span multiple lines, for example.
|p
When |i{penter} is finished with an entry, it
begins again, prompting for another reference type.
To terminate a |i{penter} session, type |cw<quit> and
the new entries will be written to the bibliography file.
This example shows how to enter the previous reference
using |i(penter); the result is put in the file |cw(prefer.out).
|i(Penter)'s prompts are in |i(italics).

|begin(here)
|begin(figure)
|begin(small)
|cw(penter)
|i(Reference type [article]) |cw(book)
|i(author:) |cw(Alfred V. Aho&)
|i(author:) |cw(Brian W. Kernighan&)
|i(author:) |cw(Peter J. Weinberger)
|i(title:) |cw(The AWK Programming Language)
|i(publisher:) |cw(Addison-Wesley)
|i(date: 1988)
|i(volume:)
|i(other:)
|i(also:)
|i(keywords:)
|i(location status [don't have]:)
|i(read status [haven't read]:)
|i(comments:)
|i(Reference type [article]) |cw(quit)
|end(small)
|end(figure)
|end(here)
.............
|p
If you want to modify
the current entry,  respond to the current prompt with |cw<~v>
to invoke a visual editor (|cw($VISUAL)
if set, or |i(vi) if not)
or |cw<~e> to invoke
a line editor (|cw($EDITOR) if set, or |i(ed) if not).
After making some changes and
leaving the editor, you will be back in |i{penter} at the same
point where you left off.
Alternatively, you can modify the bibliography
file with a text editor after you have exited |i(penter).

|subsubsection(Author and editor names.)
Proper names may be entered in a variety of ways;
|i(prefer) knows how to figure out first names, initials, last names,
titles, etc.
It also knows about some two part last names such as
De Paul and Van Winkle.
For example, any of the following are acceptable ways to enter
a name.

|begin(here)
|begin(figure)
|begin(small)
%author John Thomas De Paul
%author Dr. John T. De Paul, Jr.
%editor J. T. De Paul Jr.
%editor De Paul, John T., Jr
|end(small)
|end(figure)
|end(here)
|p
If you use the name of an organization or
corporation as an author or editor and do
not want |i(prefer) to parse it as the name
of a person, enclose the name in the |cw(%verbatim) command.
This command is also useful in keeping
double last names together.
Here are two examples:
|begin(here)
|begin{figure}
|begin(small)
%author %verbatim(|small[AT&T] Bell Laboratories)
%author John %verbatim(Smith Jones)
|end(small)
|end{figure}
|end(here)

|subsubsection(The \f(CW%also\fP attributes.)

Another class of attributes deserves special mention.
The |cw(%also) attributes allow one bibliographic reference
to refer by keyword to another reference contained in the
same database as this example shows:

|comment(replace this reference with something more relevant and shorter; maybe put formatted version in examples section)
|begin(here)
|begin(figure)
|begin(small)
|insert(example/hirst)
|end(small)
|end(figure)
|end(here)

When a reference to the first entry is formatted,
both references will appear in a single entry.
|begin(here)
|begin(small)
|source(example/hirst.t)
|end(small)
|end(here)

|p
Two other attributes,
|cw(%also_begin) and |cw(%also_end), can enclose
another |cw(%also) attribute, surrounding it with optional text
as this example demonstrates:
|begin(here)
|begin(figure)
|begin(small)
%also_begin (See also
%also Grosz Sparck Jones Webber
%also_end )
|end(small)
|end(figure)
|end(here)
Alternatively, a partial or complete reference may be included between
|cw(%also_begin) and |cw(%also_end):

|begin(here)
|begin(figure)
|begin(small)
%also_begin Reprinted in
%type editedbook
%title Programming Languages: A Grand Tour
%editor Ellis Horowitz
%date 1983
%publisher Computer Science Press
%address Rockville, MD
%also_end
|end(small)
|end(figure)
|end(here)
While |cw(%also) is supported by |i{penter},
|cw(%also_begin) and |cw(%also_end) are not and must
be entered using a text editor.
.......

|subsection(Indexing a Bibliography)

|i(Pinvert)|dagnote<Based on the |i(bib)|reference(bib) version
by T. A. Budd>
creates a search key
index to one or more bibliography database files.
Indexing the database lets you refer to any
entry using a unique combination of search keys.
To create the index, |i(pinvert) uses words of at least
three characters found the bibliography.
Some common words (those found in |cw</usr/lib/eign>)
are ignored.
In addition, values of some of the attributes
are ignored (|cw(%volume) and |cw(%pages), for example).
|i(Pinvert) converts all keys to lower case, removes
non-alphanumeric characters, and truncates
each key to six characters.
|p
|i(Pinvert) with no arguments, assumes that the bibliography
file is |cw(prefer.out) and stores the index and header information in
|cw(prefer.out.i) and |cw(prefer.out.h).
If more than one file is being indexed,
the index is named after the first file
in the list.
For example,

|figure<|small(|cw{pinvert ourbib theirbib mybib})>

creates an index to all three bibliography files,
storing the index and header information
in |cw(ourbib.i) and |cw(ourbib.h), respectively.
Alternatively, index and header files can also be given a
unique name using
the |cw(-p) option:

|figure(|small<|cw{pinvert \-p index yours mine ours}>)

stores the index and header files
for the three bibliography files in |cw(index.i) and |cw(index.h).

|p
|i(Pinvert) provides several other options.

|begin(here)
|figure[|cw<\-c> |i(cfile)]
|blank_space(-1)
specifies a file
of common words, one per line,
to exclude as indexing keys
(default is |cw(/usr/lib/eign)).
|end(here)
|begin(here)
|figure[|cw(\-i) |i(ifile)]
|blank_space(-1)
specifies a file
of attributes, one per line,
to ignore while collecting
keys from database entries.
The default file,  |cw(/usr/lib/prefer/ignore),
contains the attributes |cw(%volume), |cw(%number),
|cw(%part), |cw(%pages), |cw(%also), |cw(%X), |cw(%Y), and |cw(%Z).
|end(here)
|begin(here)
|figure[|cw(\-k) |i(maxkey)]
|blank_space(-1)
specifies the maximum number
of search keys
collected per reference (default is 100 keys).
|end(here)
|begin(here)
|figure[|cw(\-l) |i(keylen)]
|blank_space(-1)
specifies the maximum length of a search
key(default is 6 characters).
|end(here)
|begin(here)
|figure[|cw(\-v)]
|blank_space(-1)
tells |i(pinvert) to print statistics.
|end(here)

|blank_space(1)
|subsection<Retrieval by Keyword>

|i(Plook) is a command that uses an index
to search a bibliography database for references
that match search keys.
As in |i(pinvert),
search keys are a sequence of non-white space characters
with non-alphanumeric characters removed. 
Keys must be at least three characters and are
converted to lower-case and truncated to six characters.
Some common words are ignored (those in the file |cw</usr/lib/eign>
by default).
If the search keys appear on the command line, |i(plook) retrieves
all references that contain all the search keys
and prints the references on
the standard output.
For example,

|figure<|small(|cw{plook awk book})>

would output:

|begin(here)
|begin(figure)
|begin(small)
|begin(i)
%type book
%author Alfred V. Aho
%author Brian W. Kernighan
%author Peter J. Weinberger
%title The AWK Programming Language
%publisher Addison-Wesley
%address Reading, Massachusetts
%date 1988
|end(i)
|end(small)
|end(figure)
|end(here)

If the command line contains no search keys,
then |i(plook) reads a line of standard input for search keys.
After returning the matching reference(s) it waits for
the next line of keys, terminating when it receives
an end-of-file character, <|b(control-d)>.
Here's an example of this mode of use:

|begin(here)
|begin(figure)
|begin(small)
|cw(plook)
|cw(lesk indexes)
|begin(i)
%type incollection
%author M. E. Lesk
%title Some Applications of Inverted Indexes
 on the UNIX System
%booktitle UNIX Programmer's Manual,
 Seventh Edition
%publisher Bell Laboratories
%address Murray Hill, NJ
%date January, 1979
%volume 2A
|end(i)

|cw(lesk tables)
|begin(i)
%type incollection
%author M. E. Lesk
%title TBL \- A program to format tables
%booktitle UNIX Text Editing & Phototypesetting
 Starter Package
%editor Claudine Tartanella
%publisher Computing Information Services,
 Bell Laboratories
%address Murray Hill, NJ
%date 1980
|end(i)

|cw(<control-d>)
|end(small)
|end(figure)
|end(here)

|p
|i(Plook)
recognizes the options |cw<-c>, |cw<-l>, and |cw<-p>
with the same meanings as |i(pinvert).
If the index file is out of date or does not exist,
|i(plook) will try to create it by invoking |i(pinvert).

|section(Formatting References In Documents)

This section explores in detail the |i(prefer) program
for formatting documents with bibliographic references.
|i(Prefer) scans a text file looking for one
of the following commands to process:

|begin(here)
|begin(figure)
|cw<|sp(|s)reference( |i[keywords... attributes... flags...] )>
|cw<|sp(|s)reference_placement>
|cw<|sp(|s)reference_include( |i[dbfile...] )> 
|cw<|sp(|s)reference_database( |i[dbfile] )> 
|cw<|sp(|s)reference_list( |i[dbfile...] )>
|cw<|sp(|s)reference_style( |i(style [arg...] ))>
|end(figure)
|end(here)

Each will be described in turn.
The parentheses enclosing the command arguments
may be replaced by the |cw({}), |cw([]), or |cw(<>).
|i(Prefer) also accepts several command line options
that are explained at the end of this section.

|subsection(Citations)

The |cw<|sp(|s)reference>
command is for citing reference within a text.
As you saw in the introduction and in the
following example, you can cite a reference using
keywords which |i(prefer) will use
to search a bibliography database for a matching entry.

|figure<|small[|cw{... sort methods|reference(Knuth Art 1973) ...}]>

Keywords should be chosen so as to uniquely identify
a particular reference.
You can use words from any attribute
not ignored by |i(pinvert), including
|cw(%author), |cw(%title), |cw(%keyword) and |cw(%date).
If multiple references match the keys,
|i(prefer) will print a warning,
but use only the first match.
Similarly, if no references match, a warning message is printed.
If two or more references in the database share all possible search keys,
you can add extra words to the |cw(%keyword) attribute of each
entry in the database to
distinguish among them.

|p
In addition to keyword search, you can cite
a reference by including
the entire reference in the command as shown
below.
Note that subsequent references to this reference
will have to respecify all the attributes.
|begin(here)
|begin(figure)
|begin(small)
|sp(|s)reference(
%type book
%author Donald E. Knuth
%title The Art of Computer Programming
%volume Volume 3/Sorting and Searching
%publisher Addison-Wesley
%keywords sorting searching
%address Reading, MA
%date 1973
)
|end(small)
|end(figure)
|end(here)
|p
A third way is to use keywords to select a database reference,
followed by attributes
that supplement or override attributes
in the database reference.
For example, a keyword selected reference could
be supplemented with a |cw(%also) entry as shown below.
|begin(here)
|begin(figure)
|begin(small)
|sp(|s)reference(
McKeown phdthesis
%also_begin Also in
%type techreport
%publisher University of Pennsylvania
%date 1982
%report Technical Report MS-CIS-82-5
%also_end
)
|end(small)
|end(figure)
|end(here)
In all cases, |i(prefer)
replaces the citation command with
a citation mark that corresponds
to the full reference as it appears in the list
formatted by the |cw<|sp(|s)reference_placement>
command.
The |cw(att), |cw(ieee), and |cw(spectrum) styles
use superscripted numbers for citation marks;
|cw(acm) and |cw(pami) styles use numbers in square brackets;
|cw(apa) and |cw(lsa) styles use (author, year).

|p
Following keywords and attributes, you can supply
flags that
modify the citation mark.
The citation flags, |cw(%no_author) and |cw(%no_date),
relevant to |cw(att) and |cw(lsa) styles,
cause the author field and date field, respectively, to be excluded
from the citation mark.
The |cw(%no_cite) flag, relevant to all styles, causes the entire citation to be
omitted, but the full reference is still printed in the list of
references.
Some styles
let you annotate a citation with text
using |cw(%pre_text) and |cw(%post_text).
Annotations could include introductory phrases
and page numbers, for example.
If multiple citation flags are used (e.g, both |cw(%no_author)
and |cw(%post_text)),
each must appear on it's own line.
Formatting the following document fragment in |cw(apa) style
|begin(here)
|begin(figure)
|begin(small)
|insert(example/lamport.in)
|end(small)
|end(figure)
|end(here)
produces this output:
|begin(here)
|begin(figure)
|begin(small)
|source(example/lamport.t)
|end(small)
|end(figure)
|end(here)

More uses of |cw<|sp(|s)reference> are found in
Appendix |remember(examples).

|subsection(Reference Placement)


The |cw<|sp(|s)reference_placement> command causes all
references seen since the beginning of the document,
the last |cw<|sp(|s)reference_placement>,
or the last |cw<|sp(|s)reference_style> command,
to be formatted and inserted in the document at that point.
The order of references depends on the current style.
References in |cw(att), |cw(ieee), and |cw(spectrum) styles 
appear in the order first cited in the preceding text.
References in |cw(acm), |cw(apa), |cw(lsa), and |cw(pami) are sorted by
author, date, and title.
All except |cw(apa) and |cw(lsa)
have labels that match the citation
marks appearing in the text.
|p
See Appendix |remember(examples) for uses
and results of |cw<|sp(|s)reference_placement>.


|subsection(Including Additional References)
Sometimes you may want the reference list
to include more references than you actually
cite in the text.
The |cw<|sp(|s)reference_include( |i[dbfile...] )> command 
includes the contents of one or more database files
in the list of references produced by
|cw<|sp(|s)reference_placement>, treating
them as |cw(%no_cite) entries.
For example, the |cw<|sp(|s)reference_placement>
command here:

|begin(here)
|begin(figure)
|begin(small)
|i(text with citations)
...
|sp(|s)reference_include(morebibs)
|sp(|s)reference_placement
|end(small)
|end(figure)
|end(here)

will include in its list
of references all the entries in the file |cw(morebibs)
as well as 
any citations found in the text.

|subsection(Switching Databases)

The command to search a new bibliography database is
|cw<|sp(|s)reference_database( |i[dbfile] )>.
|i(Prefer) then closes the current
database file, and opens the new one, |i(dbfile).
But it still remembers citations from the previous
database.
For example, in

|begin(here)
|begin(figure)
|begin(small)
|sp(|s)reference_database(mybib1)
|i(text with citations)
...
|sp(|s)reference_database(mybib2)
|i(text with more citations)
...
|sp(|s)reference_placement
|end(small)
|end(figure)
|end(here)

the first database, |cw(mybib1),
is consulted for the first part of the text,
while the database |cw(mybib2) is consulted
for the second part.
The |cw<|sp(|s)reference_placement> command
will list citations from both databases.
|p
All databases should be indexed using |i(pinvert).
However, if a database has not been indexed since its last update,
|i(prefer) will automatically
invoke |i(pinvert) before using the database.

|subsection(Complete Bibliographies)

Beside formatting references appearing in documents,
|i(prefer) also formats entire bibliography databases.
The command |cw<|sp(|s)reference_list( |i[dbfile...] )>,
behaves like |cw<|sp(|s)reference_placement>
except that instead of formatting citations appearing
in the previous text, it formats
all the references appearing in one or more bibliography
databases.
For example,

|figure<|small[|sp(|s)reference_list(mybib88 mybib89)]>

produces a list of all references in the file |cw(mybib88)
followed by all references in |cw(mybib89).
|p
Like |cw<|sp(|s)reference_database>,
this command changes the search database.
The current style dictates how each entry will be formatted.
By default, the entries will be printed
in the order they appear in the database(s).
Changing this order requires
setting some style arguments or
supplying some command line options.
Both ways are described below.


|subsection(Styles Options)
|save(stylesec)
The |cw<|sp(|s)reference_style( |i(style [arg...] ))> command provides one way
to change options regarding formatting style.
The first argument to this command selects
the style script.
It gives the name of one of
the styles provided by prefer
(|cw(att), |cw(acm), |cw(apa), |cw(ieee), |cw(spectrum), |cw(lsa)
or |cw(pami)), or the pathname of
a user-defined style script.
Note that any file in the current directory with the same
name as one of the |i(prefer) styles (e.g. |cw(acm)),
will override the |i(prefer) style. 
The easiest way to define a new style is
to modify an existing |i{awk} style script that is similar
to the desired style.
Describing how to write a style script in further detail is
beyond the scope of this paper.
|p
Your document may use multiple formatting styles
by resetting the style using |cw<|sp(|s)reference_style>.
If you switch styles before issuing a |cw<|sp(|s)reference_placement>
command, the citations appearing before the style command
will be forgotten and thus,
will not be listed by any subsequent |cw<|sp(|s)reference_placement>.
|p
Some of the optional arguments to the style command control
how references
are formatted by the |cw<|sp(|s)reference_list> command.
|begin(here)
|begin(table)
center;
l l.
|cw(sequence)	list in database order
|cw(sort)	list according to current style
|cw(a)	sort by author
|cw(d)	sort by date
|cw(t)	sort by title
|end(table)
|end(here)
The last three options may be used in combination.
If optional arguments are given, but the
style is to remain the same, the first
argument may be set to |cw(same).
|p
The style command recognizes several other
arguments that affect the formatting of references.
These are:
|begin(here)
|begin(table)
center;
l l.
|cw(tm)	format as a technical memorandum
|cw(rp)	format as a released paper
|cw(troff)	format for |i(troff)
|cw(nroff)	format for |i(nroff)
|end(table)
|end(here)
The defaults are for a paper to be formatted
as a technical memorandum using |i(troff)
Here are some examples of using the style command:
|begin(here)
|begin(figure)
|begin(small)
|begin(cw)
|sp(|s)reference_style(/usr/mad/mystyle ad)
|sp(|s)reference_list(bib)
|sp(|s)reference_style(acm sort rp)
|sp(|s)reference_list(bib)
|sp(|s)reference_style(same sequence nroff)
|sp(|s)reference_list(bib)
|end(cw)
|end(small)
|end(figure)
|end(here)

The first command selects the user-defined style
in |cw(/usr/mad/mystyle) and says to sort according
to author, then date.
The bibliography file |cw(bib) is formatted accordingly.
The third command selects the |cw(acm) style,
and says to sort |cw(bib) according to that style.
It also say says to format it as a released paper.
The fifth command says to use the same style (|cw[acm]),
but this time print the references as they
appear in |cw(bib) and
format the paper using |i(nroff).


|subsection(Command Line arguments)

Command line arguments provide another way
to select bibliography databases and set style options.

|begin(here)
|figure[|cw(\-p) |i(dbfile)]
|blank_space(-1)
specifies a bibliography
database, |i(dbfile) (default is |cw(prefer.out))
|end(here)
|begin(here)
|figure[|cw(\-s) |i(sname)]
|blank_space(-1)
selects formatting style |i(sname)
(default is |cw(att)). The argument |i(sname)
names of one of the provided styles or
a user-defined style.
|end(here)
|begin(here)
|figure[|cw(\-o) |i(okey)]
|blank_space(-1)
specifies how
the |cw<|sp(|s)reference_list>
command should order references.
The argument |i(okey) may be one of
|cw(sequence) (the default), |cw(sort), or a combination of
|cw(a), |cw(d), or |cw(t), as described above in Section
|remember(stylesec).
|end(here)
|begin(here)
|figure[|cw(\-n)]
|blank_space(-1)
format using |i(nroff) rather than for |i(troff),
the default.
|end(here)
|begin(here)
|figure[|cw(\-r)]
|blank_space(-1)
format as
a released paper, rather than an
internal technical memorandum, the default.
|end(here)
For example, the command

|figure<|small[|cw{prefer -r -sacm -pmybib  mydoc}]>

processes |cw(mydoc) as a released paper, using |cw(acm) style
and the bibliography database |cw(mybib).

|section(Using |i<prefer> |b(With) |i<monk>)

As mentioned earlier, the |i(monk) text formatting package
invokes |i(prefer) to format bibliographic references.
To do this, it must convert its own reference commands to ones
understood by |i(prefer).
For the
|cw<|sp(|s)reference>,
|cw<|sp(|s)reference_list>,
and |cw<|sp(|s)reference_database>
commands, this is easy;
|i(monk's) versions are identical to |i(prefer's).
Note that |i(monk) uses angle brackets |cw(<>)
to enclose the |i(prefer)
command arguments.
Thus if angle brackets appear in any of the command
arguments, confusion may arise.

|p
The |cw<|sp(|s)reference_placement> command in |i(monk)
differs slightly from the |i(prefer) version, depending on
the |i(monk) document style.
For example, for technical memoranda
it causes a new page
to be printed and
the reference list to be headed by
a centered |small(|cw<REFERENCE>) title.

|p
|i(Monk) does not have a 
|cw<|sp(|s)reference_style> command.
Instead, it uses it's own |cw<|sp(|s)style> command
along with a |cw(reference) attribute/value pair.
For example, to change the reference style to |cw(ieee), use the
monk command

|figure<|small[|sp(|s)style(reference ieee)]>

To set any of the other options provided by the |i(prefer)
|cw<|sp(|s)reference_style> command,
use the |i(monk) |cw<|sp(|s)style> command
with a |cw(reference_option) attribute/value pair.
For example,

|figure<|small[|sp(|s)style(reference_option rp)]>

means format references for a released paper.
Other possible values for |cw(reference_option) are:
|cw(tm), |cw(troff), |cw(nroff), |cw(sort), |cw(sequence), and any
combination of the letters |cw(a), |cw(d), |cw(t).
These options have the same meanings as for
the |cw<|sp(|s)reference_style> command.
The command line options |cw<\-s>, |cw<\-o>, |cw<\-r>, and |cw<\-n>
of |i(prefer) are not set by |i(monk).
|i(Monk) uses the style command to set these options.

|p
|i(Monk) uses the file |cw</usr/lib/monk/prefs> as its default bibliography
database.
You can specify an alternative database
with the |cw(\-R) option, which |i(monk) translates to the |cw<\-p>
option for |i(prefer).
For example,

|figure<|small(|cw{monk \-Rmybib mydoc})>

says that monk should use the file |cw(mybib)
as the bibliography database when formatting |i(mydoc).

|section(Converting From |i<refer> |b(To) |i<prefer> )

The |i(pconvert) command converts a |i(refer) style bibliography
database
to |i(prefer) format.
For example, the command

|figure<|small(|cw{pconvert refer.out > prefer.out})>

scans the file |cw(refer.out) and writes the transformed version
into |cw(prefer.out).
|i(Pconvert) uses the attributes of each
entry to infer the |cw(%type) of the entry.

|p
With the |cw(\-d) option |i(pconvert)
changes |i(refer) style commands in a document
to |i(prefer) style.
For example

|figure<|small(|cw{pconvert \-d oldpaper > newpaper})>

scans |cw(oldpaper) for |i(refer) citations
and converts them to |i(prefer) commands,
writing the output to |cw(newpaper).


|section(Acknowledgements)

Sharon Peeters and Ted Kowalski
encouraged me to write |i(prefer) and then integrated
it into |i(monk).
The idea of using the |i(awk) language to format references
came from Ravi Sethi.
Julia Hirschberg, Joe Kruskal, and Ted Kowalski
contributed style scripts.
Thanks to those who offered feature suggestions
bug reports, and
comments on versions of
this paper
including
Judy Friedman,
Julia Hirschberg,
Andrew Hume,
Ted Kowalski,
Joe Kruskal,
Doug McIlroy,
Mel Melchner,
Marcel Meth,
Sharon Murrel,
John Puttress,
Sande Pruzansky,
Ravi Sethi,
and
Wayne Wolf.


|environment(reference_placement;
|ifvalue sawreference on [
spacing on, blank.lines 3,
set string Ts "References",
inline off, size +1,
font bold, string Ts, font roman,
center off, size -1, inline on,
minimum.lines,
clear string Rs,
contents section 1 Rs Ts,
reference.placement
];
sawreference off)
|comment<summary: reference_placement (produce references here)>

|environment(appendix;
spacing on, blank.lines 3,
number format AC A, incr AC,
clear S0 S1 S2 S3,
set string Rs "Appendix ",
add string.from.number Rs AC,
divert.string Ts;
sectioning byappendix,
center on, size +1, font bold,
string Rs, blank.lines, string Ts,
font roman, size -1, center off,
minimum.lines 2, paragraph,
store string.from.number Rs AC,
contents section 1 Rs Ts)
|comment<summary: appendix (numbered appendix; IT)>
|reference_placement

|comment<|new_page>
|appendix(Bibliography Types And Attributes)
|save(types)

|insert(example/ptemplate.m)

.ne 3i
|appendix(Examples)
|save(examples)

This sample document contains many examples of
|cw<|sp(|s)reference> commands followed by
a |cw<|sp(|s)reference_placement>.

|begin(here)
|begin(figure)
|begin(small)
|insert(example/type.in)
|end(small)
|end(figure)
|end(here)

|p
When |i(prefer) formats this document according to |cw(acm) style
it looks like this:

|p
|begin(small)
|source(example/type.acm)
|end(small)

|p
When formatted according to |cw(apa) style
it looks like this:
|p
|begin(small)
|source(example/type.apa)
|end(small)

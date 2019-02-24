|make(rp)

|insert(monk.mac)

|comment(VOL2HEADER)|set_counter(page 239-1)|set_string(page_headers "'''")|set_string(even_headers "'Typing Documents on the UNIX System: Using Monk 0.6''\f(NImonk\fP'")|set_string(odd_headers "'\f(NImonk\fP''Typing Documents on the UNIX System: Using Monk 0.6'")|set_string(even_footers "_\\nP__UNIX Papers_")|set_string(odd_footers "'Research Tenth Edition''\\nP'")

|environment(cwfigure;
	new.line, verbatim on, file.information, line.spacing 1,
	fill off, inline off, size -2, font cw, inline on, space -2,
	blank.lines 1;
	SPACE)
	|comment<summary: cwfigure (a constant-width figure)>

|environment(computeroutput;
	new.line, verbatim on, file.information, line.spacing 1,
	fill off, inline off, size -1, font cw, inline on, space -1,
	blank.lines 1,
	tab.stops "\w'        'u 2u*\w'        'u 3u*\w'        'u 4u*\w'        'u 5u*\w'        'u 6u*\w'        'u 7u*\w'        'u 8u*\w'        'u 9u*\w'        'u 10u*\w'        'u 11u*\w'        'u";

	tab.stops, blank.lines 1, SPACE)

|author(name "S. L. Murrel", initials SLM, location MH, department 11229,
	extension x5772, room 2C-578)

.Tm Typing Documents on the |small(UNIX) System: Using Monk	g
|author(name "T. J. Kowalski", initials TJK, location MH, department 11229,
	extension x2771, room 2C-568)

|document(number 11229-851210-11TMS, file_case 25952, work_program 311401-2299)

|document(number 11229-851210-11TMS, file_case 25952, work_program 311401-2199)

|date(December 10, 1985)

|title(Typing Documents on the |small(UNIX) System:
Using Monk 0.6)

.CD "comment_section" "5.1"
.CD "special_section" "5.6"
.CD "table_example" "5.14"
.CD "picture_example" "5.16"
.CD "save_section" "5.18"
.CD "this_section" "5.18"
.CD "foobar" "251"
.CD "style_section" "5.4"
.CD "special_appendix" "B"
.CD "table_appendix" "C.1"
.Tm text-formatting program	g
.CD "The alphabet in common type families" "1"

|begin(abstract)
|i<Monk> is a text-formatting program that produces high-quality
typeset output.
.Tm Letters	s
.Tm Release Papers	s
.Tm Technical Memoranda	s
This paper, printed using |i<monk>,
describes how to produce letters, memos, release papers,
and technical memoranda complete with cover sheets.
We provide extensive details on the three easy steps
needed to prepare a document with |i<monk>:

|begin(number_list)

|item
Use the |i<monksample> command to get a copy of a sample document.

|item
Edit the copy, replacing the sample title, author, and the text body.
.Tm Figures
.Tm tables
.Tm equations
Add sections, appendices, captioned figures, tables, and equations.
.Tm Spacing	s
Select the document style:
character size and type, page width and length, line spacing,
and number of columns.

|item
Format the document using the |i<monk> command,
sending the output to a 5620 or 630MTG
terminal or a printer.
|i<Monk> will automatically invoke
the table, picture, equation, graph,
|comment{index,}
and reference pre-processors in the correct order.
.Tm Figures

|end(number_list)

|end(abstract)

|titlebox

|style(two_column)

|section(Introduction)

Preparing any document involves positioning words and figures on each page.
.Tm Spacing	s
Using a typewriter,
the margins, line spacing, and tab positions are selected by moving levers.
Text is positioned by moving the sheet of paper left, right, up or down.
Using a computer, the text is interspersed with commands that tell a
formatting program what to do.
.Tm Letters	s
.Tm Technical Memoranda	s
|p
|i<Monk> is a new package of commands for producing
|s(AT&T) letters, memos, technical memoranda, and their cover sheets.
This paper will enable you to prepare documents using
|i<monk>.|reference<monk high-level text compiler>|reference<monk database>
|reference<using monk 1985>
You need to know how to log onto a computer and how to use an editor.
.Tm typesetting	g
If you don't, see the |small(UNIX) Text Editing and Phototypesetting
.Tm typesetting	g
.Tm Mixing Text and Commands	s
Starter Package.|reference(Editing Phototypesetting Starter Package)
|blank_space(-1p)|comment{ZZZ: spacing kludge}
|section(Mixing Text and Commands)
|blank_space(-6p)
|p
Using any text editor,
type normally.
However,
instead of skipping a line before paragraphs
type |cw(|sp(|)paragraph) or |cw(|sp(|)p).
|index(paragraphs)
This command will produce a blank line.
To indent,
use |cw<|sp(|)p(i)> or |cw<|sp(|)paragraph(indented)>;
to number,
use |cw<|sp(|)p(n)> or |cw<|sp(|)paragraph(num|sp(hyphenation)bered)>.
|p
There are other commands that let you select document style and character type
.Tm Spacing	s
(e.g., double spacing and italics)
.Tm tables
and include various sections and displays (e.g., chapters and tables).
.Tm Command Scope	s
.Tm delimiters	s

|section(Command Scope)

|comment{|index(command delimiters)|index(italics)}

Using |i<monk>,
the sentence ``Italicize the words in |sp(|s)italics(parentheses)'' produces
``Italicize the words in |i(parentheses).''
All |i<monk> commands begin with the character `|sp(|s)'.
The command |cw(|sp(|)italics) selects the operation and
the parentheses delimit the words to be italicized.
.Tm delimiters	s
There are seven pairs of delimiters:

|begin(center)
.Tm delimiters	s
|cw<(>...|cw<)>, |cw([)...|cw(]), |cw(<)...|cw(>), |cw({)...|cw(}), |cw(")...|cw("), |cw(')...|cw('), |cw(`)...|cw(')
|end(center)

|p
Always choose delimiters so that the closing delimiter does not appear in the
text.
``Italicize the equation |sp(|s)i(a = (b + c) / number)'' produces
``Italicize the equation |i(a = (b + c) / number).''
The closing delimiter ')' is found after the letter `c',
so that the rest of the equation is not italicized.
The input ``Italicize the equation |sp(|s)i<a = (b + c) / number>'' will
correctly italicize the entire equation, |i<a = (b + c) / number>.
In contrast, when |i<monk> commands are inside other |i<monk> commands,
the delimiter pairs are not ambiguous.
``Italicize and shrink this |sp(|s)small(|sp(|s)i(word) but only shrink the rest)''
produces
``Italicize and shrink this |small(|i(word) but only shrink the rest).''
|p
There is a second form of delimiter |sp(Em dash)
when a command operates on a long text segment,
the commands |cw(|sp(|)begin) and |cw(|sp(|)end)
are used instead of brackets or quotes.
The text fragment

|begin(cwfigure)
|sp(|s)begin(italics)
Four score and seven years ago our fathers
brought forth to this continent a new
nation, conceived in liberty and ...
|sp(|s)end(italics)
|end(cwfigure)

appears in the final document as

|begin(italics)
Four score and seven years ago our fathers brought forth to this continent
a new nation, conceived in liberty and ...
|end(italics)

The commands |cw(|sp(|)begin) and |cw(|sp(|)end) delimit
the text operated on by the
command |cw(|sp(|)italics).
.Tm Document Commands	s
.Tm Letters	s
.Tm Memoranda	s
.Tm Technical Memoranda	s
.Tm Release Papers	s

|section(Document Commands)

This section describes the commands that are specific to formatting
four types of documents:
letters, |small(AT&T) Bell Laboratories memoranda, technical memoranda,
and release papers.
.Tm Typesetting a Letter	s

|environment(ret_address;
	verbatim on, indent \n(U0u+\n(O0u-6n, fill off, here on;
	minimum.lines)

|subsection(Typesetting a Letter)

For a letter, the input should begin as follows:

|begin(cwfigure)
|sp(|)make(letter)
|sp(|)begin(ret_address)
|sp(|)s(AT&T) Bell Labs 2C-501
Murray Hill, N. J. 07974
|sp(|)end(ret_address)
|end(cwfigure)

The first line specifies that the standard format for a letter should be used.
Then the return address, labeled |cw(ret_address), is delimited
by |cw(|sp(|)begin) and |cw(|sp(|)end) commands.
The formatter skips a couple of lines, indents, single spaces the address
itself, skips more lines, and then returns to the left hand margin
producing:

|begin(figure)
|begin(ret_address)
|s(AT&T) Bell Labs 2C-501
Murray Hill, N. J. 07974
|end(ret_address)
|end(figure)

The return address itself is not modified.
If the city, state, and zip code are input as separate lines,
they will appear that way in the final output:

|begin(figure)
|begin(ret_address)
|s(AT&T) Bell Labs 2C-501
Murray Hill
N. J.
07974
|end(ret_address)
|end(figure)

|p
A letter consists of a sequence of standard groups:
a return address, date, recipient's address, greeting, body,
closing, and signature.
Accordingly,
the following prototype letter is available.

|begin(cwfigure)
|comment<inserting samples/letter.nx starting here>
|sp(|)make(letter)
|blank_space(.5)
|sp(|)begin(ret_address)
|sp(|)s(AT&T) Bell Laboratories 2C-501
Murray Hill, New Jersey 07974
|sp(|)end(ret_address)
|blank_space(.5)
|sp(|)date(April 1, 1985)
|blank_space(.5)
|sp(|)begin(address)
G. H. Reiber
|sp(|)s(AT&T) Communications
Room 3C-110
Bedminster, |sp(|)s(NJ) 07921
|sp(|)end(address)
|blank_space(.5)
|sp(|)greeting(George Reiber:)
|blank_space(.5)
|sp(|)begin(body)
Yes, I would like to hire a painter for the
night of Monday, September 16, 1985.
Thank you very much.
|sp(|)end(body)
|blank_space(.5)
|sp(|)closing(Sincerely,)
|blank_space(.5)
|sp(|)signature(G. R. Emlin)
|comment<inserting samples/letter.nx ending here>
|end(cwfigure)

|p
To copy this letter into a file named |cw<myfile> in your current directory,
type the |small(UNIX) command

|begin(cwfigure)
monksample letter > myfile
|end(cwfigure)

Using this prototype, it is not necessary for you to remember that
the label for return address is |cw(ret_address).
You need only recognize the English-like descriptions and
replace the sample text as appropriate.
Although any text editor will suffice, the large display of a visual editor
allows you to glance at the file and spot sample text.
|p
It is convenient to tailor the samples by entering your
address, closing, and signature and then saving the modified file.
.Tm Letters	s
This new file can then be used for all your letters simply
by adding an appropriate greeting, the recipient's address, and the body.
.Tm Letters	s
If you prepare letters for more than one person, you might want to maintain
a directory of tailored prototypes rather than begin with the
standard |i<monk> sample.

|begin(cwfigure)
|comment<inserting samples/tailor.nx starting here>
|sp(|)make(letter)
|blank_space(.5)
|sp(|)begin(ret_address)
|sp(|)s(AT&T) Bell Laboratories 2C-524
Murray Hill, New Jersey 07974
|sp(|)end(ret_address)
|blank_space(.5)
|sp(|)date(April 1, 1985)
|blank_space(.5)
|sp(|)begin(address)
G. H. Reiber
|sp(|)s(AT&T) Communications
Room 3C-110
Bedminster, |sp(|)s(NJ) 07921
|sp(|)end(address)
|blank_space(.5)
|sp(|)greeting(To Whom It May Concern:)
|blank_space(.5)
|sp(|)begin(body)
Thank you very much.
|sp(|)end(body)
|blank_space(.5)
|sp(|)closing(Sincerely,)
|sp(|)signature(Sam)
|comment<inserting samples/tailor.nx ending here>
|end(cwfigure)

|p
When installed,
|i<monk> is configured to use local printers as standard output
devices.|dagnote{Specify the printer that you have permission to use
by defining |s<LPDEST> in your |i<.profile>,
e.g., |cw<LPDEST=crete; export LPDEST>.}
You will need to talk to someone using or maintaining your computer system
to find out what and where your local standard printer is.
|p
To run off this memo on the standard printing device,
type |cw(monk myfile).
To send your output to a printing device other than the standard,
use the |cw<|sp(minus)S>|i<dest> and |cw<|sp(minus)T>|i<font> options.
To preview postscript output on a |s(AT&T) 5620 or 630MTG terminal, type
|cw(monk |sp(minus)Tpost |sp(minus)Sproof myfile).
.Tm Fonts	s
Devices and fonts will vary with your location,
but commonly available types include:

|begin(figure)
|begin(table)
center;
c1FCW lfB
l1FCWp-2 l.
.sp .5
|sp(minus)S|i(dest)	Sends Output To Device |i(dest)
.sp .25
_
.sp .25
|sp(minus)Sapsend	Linotronic 200P, see |i(apsend)(1)
|sp(minus)Sd202	Mergenthaler Linotronic 202, see |i(d202)(1)
|sp(minus)Si300	Spooling for |s(EXPTOOLS), see |i(i300)(1)
|sp(minus)Slp	Spooling mechanism, see |i(lp)(1) (default)
|sp(minus)Slpr	Spooling at Allentown, see |i(lpr)(1)
|sp(minus)Sproof	5620 or 630MTG terminal, see |i(proof)(9.1)
|sp(minus)Sprt	Spooling at computer centers, see |i(prt)(1)
|sp(minus)Sthink	Thinkjet, see |i(thinkblt)(9.1)
|sp(minus)S|sp(minus)	Standard output
.sp
.T&
c1FCW lfB
l1FCWp-2 l.
.sp .5
|sp(minus)T|i(fonts)	Selects Typesetter Or Terminal |i(fonts)
.sp .25
_
.sp .25
|sp(minus)T202	Mergenthaler Linotronic 202
|sp(minus)Taps	Autologic |s(APS-5)
|sp(minus)Ti10	Imagen 10 printer
|sp(minus)Ti300	Imagen 300 printer
|sp(minus)Tim300	Imagen 300 printer at Liberty Corner
|sp(minus)Tpost	PostScript|sp(registered) printer (default)
|sp(minus)Tthink	Think jet printer
|end(table)
|end(figure)
.Tm Fonts	s
.Tm Fonts	s
.Tm Fonts	s

|p
If |s(TYPESETTER) is set and exported, then it determines the typesetter
or terminal fonts used in preparing the output.
Be careful to type |cw(monk |sp(minus)Tpost |sp(minus)Sapsend myfile)
or |cw(monk |sp(minus)Taps |sp(minus)Sprt myfile)
|b(only) when you intend to send
final copy to the high-quality, high-priced phototypesetter.

|p
When |i<monk> is run, the individualized prototype letter will appear as
a full size version of the page below.
This is an excerpt of the sample's output,
scaled and included using PostScript.
.br
|comment< to make the postscript output the column width: x 3.1i, y 7i >
.ne 14v
|begin(ruled.display)
|postscript(file "samples/tailor.ps",
			x 4.2i, y 7i, xoffset -.5i, yoffset -1i)
|blank_space(13v)
|end(ruled.display)
|blank_space(1v)
..........
.Tm Typesetting a Memorandum	s

|subsection(Typesetting a Memorandum)

Preparing a memorandum is similar to preparing a letter.
.Tm Memoranda	s
Memoranda require a title, more information about the author,
and the document itself.
The information you provide is automatically arranged at the top of the page.
|p
Type

|begin(cwfigure)
monksample memo > myfile
|end(cwfigure)

to get the following prototype:

|begin(cwfigure)
|comment<inserting samples/memo.nx starting here>
|sp(|)make(memo)
|blank_space(.5)
|sp(|)author(name "W. H. MacWilliams",
   initials WHM, location WH, room 2E-221,
   extension x511, department 1344)
|sp(|)document(number 1344-561210-09,
   file_case 25952, work_program 311401-0199)
|blank_space(.5)
|sp(|)date(December 10, 1956)
|blank_space(.5)
|sp(|)title(|sp(|)s(MTS) Employment: Dr. J. L. Flanagan)
|blank_space(.5)
|sp(|)titlebox
|blank_space(.5)
F. D. Leamer:
|sp(|)p
This is to propose an offer of regular
|sp(|)s(MTS) employment to Dr. J. L. Flanagan
commencing on February 18, 1957.
|blank_space(.5)
|sp(|)signature(WH-1344-WHM-hhs)
|blank_space(.5)
|sp(|)notation[Copy (with att.) to:
E. E. David Jr.
A. G. Jensen]
|blank_space(.5)
|sp(|)approved(J. R. Pierce)
|comment<inserting samples/memo.nx ending here>
|end(cwfigure)
|blank_space(-1)|blank_space(-4p)|comment<ZZZ kludge - blankline at top of
column - trailing size change after cwfigure >
|p
The |cw<|sp(|)make> command asks |i<monk> to format the document as a memo.
The information about each author is given in a
separate |cw<|sp(|)author> command:
name, initials, location, department, extension, and room number.
Each instance of |cw<|sp(|)document> gives the document information for
the corresponding author.
Author, document, title, and date may appear in any order,
but must precede the command |cw<|sp(|)titlebox>,
which organizes, formats, and outputs all this information.
.Tm Figures
|p
After the titlebox comes the body of the memo,
which consists of paragraphs
separated by |cw<|sp(|)p>, simple font changes, and figures.
After the body comes the signature, any necessary notations,
and approvals.
This sample produces the following memo:
.br
.ne 20v
|begin(ruled.display)
|postscript(file "samples/memo.ps",
			x 4.2i, y 7i, xoffset -.5i, yoffset -.9i)
|blank_space(19v)
|end(ruled.display)
|blank_space(1v)
...........
|p
To omit information, simply delete the appropriate |i<monk> command.
Delete the entire line to omit notations or approvals.
To omit an author's location, omit the relevant labels and associated numbers,
e.g., |cw{|sp(|)author(name "W. H. MacWilliams", initials WHM, department 1344)}.
.Tm Typesetting a Technical Memorandum	s

|subsection(Typesetting a Technical Memorandum)

This section describes how to prepare a technical memorandum.
The example below is the skeleton of the file that produced the previous |i(tm) version
of this document, excluding cover sheet information.
.Tm References	s
This prototype consists of the declaration of the document type,
document numbers,
date,
authors, title, abstract, document body, appendices, and references.
It can be copied with the command |cw(monksample tm > myfile).

|comment{entire tm here or simply body, ending before cover sheet info}

|begin(cwfigure)
|comment<inserting samples/tmbody.nx starting here>
|sp(|)make(tm)
|blank_space(.5)
|sp(|)author(name "S. L. Murrel", initials SLM,
	location MH, department 11229,
	extension x5772, room 2C-578)
|sp(|)document(number 11229-851210-11TMS,
	file_case 25952,
	work_program 311401-2299)
|sp(|)author(name "T. J. Kowalski", initials TJK,
	location MH, department 11229,
	extension x2771, room 2C-568)
|sp(|)document(number 11229-851210-11TMS,
	file_case 25952,
	work_program 311401-2199)
|blank_space(.5)
|sp(|)date(December 10, 1985)
|blank_space(.5)
|sp(|)title(Typing Documents on the UNIX System:
Using Monk 0.3)
.Tm text-formatting program	g
|blank_space(.5)
|sp(|)begin(abstract)
|sp(|)i(Monk) is a text-formatting program that
produces high-quality printed text.
|sp(|)end(abstract)
|blank_space(.5)
|sp(|)titlebox
|blank_space(.5)
|sp(|)unnumbered_section(INTRODUCTION)
|blank_space(.5)
A reference example.|sp(|)|sp(zero width space)reference(
Murrel Kowalski Using Monk 0.3 1985)
.Tm Mixing Text and Commands	s
|blank_space(.5)
|sp(|)section(MIXING TEXT AND COMMANDS)
.Tm Typesetting a Technical Memorandum	s
|blank_space(.5)
|sp(|)subsection(Typesetting a Technical Memorandum)
.Tm Unnumbered headings	s
|blank_space(.5)
|sp(|)subsubsection(Unnumbered headings.)
|blank_space(.5)
|sp(|)signature(MH-11229-SLM/TJK-slm/tjk)
|blank_space(.5)
|sp(|)notation(Att.
.Tm References	s
References)
|blank_space(.5)
|sp(|)other
|blank_space(.5)
|sp(|)|sp(zero width space)reference_placement
|comment<inserting samples/tmbody.nx ending here>
|end(cwfigure)
|p
The first line should specify the document type.
After that the order is flexible,
with the one restriction that all the
component information be available when larger units, such as the
titlebox, are printed.
.Tm Technical Memoranda	s
Thus the author, date, and document information must be specified
before the command
|cw(|sp(|)titlebox), because it uses that information to produce
the standard header at the top of technical memoranda.
Note that the same |cw<|sp(|)titlebox> command produces different output
depending on the document type specified.
|p
The title is not filled, so that newlines entered by the author are preserved:
here |cw<Using Monk 0.3> appears as
the second line of the title.|dagnote<In unfilled text,
input lines are reproduced unchanged in the output;
in filled text, words from successive input lines may be combined to ensure
maximum length lines in the output.>
This is the first page produced by the |i<tm> input sample, scaled
to fit into the column.
The second page of the output provides the formatted reference list.
.br
.ne 25v
|begin(ruled.display)
|postscript(file "samples/tmbody.ps",
			x 4.2i, y 7i, xoffset -.5i, yoffset -.9i)
|blank_space(24v)
|end(ruled.display)
|blank_space(1v)
...........
|p
The commands |cw(|sp(|)author) and |cw(|sp(|)document)
belong to a type we have not previously discussed.
They allow you to specify a collection of information about a given item.
The first instance of |cw(|sp(|)author) specifies the name, initials,
location, department, extension, and room number of the first author.
The first instance of |cw(|sp(|)document) gives the document information
for the first author.
Authors and documents are linked as they appear, so that

|begin(cwfigure)
|sp(|)author(name "S. L. Murrel", initials SLM,
   location MH, department 11229)
|sp(|)author(name "T. J. Kowalski", location MH,
   initials TJK, department 11229)
|sp(|)document(number 11229-851210-11TMS,
   file_case 25952, work_program 311401-2299)
|sp(|)document(number 11229-851210-11TMS,
   file_case 25952, work_program 311401-2199)
|end(cwfigure)

is equivalent to the ordering given in the |i(tm) above.
Note that in these examples only part of the author information is included:
the room and extension numbers were omitted.
Any information can be omitted simply by deleting the corresponding |i<monk>
labels, e.g., |cw<room> and |cw<extension>.
Monk will try to format the information given gracefully.
|p
Additional information is needed to produce a standard |s<AT&T> cover sheet:

|begin(cwfigure)
|comment<inserting samples/tmcs.nx starting here>
|sp(|)keywords(Word Processing,
Text Processing,
Formatting)
|blank_space(.5)
|sp(|)mercury(cmp, elc)
|blank_space(.5)
|sp(|)att(is_release yes)
|blank_space(.5)
|sp(|)distribute_complete_memo(Exec Directors 112
Directors 112
Department Heads 1122)
|blank_space(.5)
|sp(|)distribute_cover_sheet(A. A. Penzias
1122 MTS)
|blank_space(.5)
|sp(|)cover_sheet
|comment<inserting samples/tmcs.nx ending here>
|end(cwfigure)

Again, this information can appear anywhere in the document as long as it
precedes the command |cw(|sp(|)cover_sheet).
.Tm Lists	s
Following local convention, keywords are filled, while the distribution lists
are not.
Although the title was not filled in the titlebox,
it is filled on the cover sheet because of space limitations.
.Tm tables
.Tm Figures
.Tm References	s
|p
In producing the cover sheet, |i<monk> automatically calculates the numbers of
tables, figures, references,
as well as text pages, other pages, and total pages.
Use the command |cw<|sp(|)other> to mark the end of the text body and the
beginning of attachments.
|i<Monk> needs it to correctly distinguish text from other pages.
If there are any pages that are not included in this input file, you
can add to the counts using the command |cw<|sp(|)add_totals>.
For example,
if there is an additional attachment page that is not a table or figure,
then the command |cw<|sp(|)add_totals(other 1)> suffices.
.Tm tables
.Tm Figures
.Tm References	s
You should never have to use the complete form of the command:
|cw<|sp(|)add_totals(text 0, tables 0, figures 1, references 11,
other 2)>.
.Tm References	s
This command means that you are adding 2 additional pages by hand,
which include one figure and eleven references.
.Tm tables
The counts of text pages and tables are untouched.
|p
There are six commands specific to the |s(AT&T B)ell |s(L)abs cover sheet
that provide distribution information.
Use |cw<|sp(|)mercury> to specify the company-wide announcements
that should include
the abstract of this document:
|cw<chm>, |cw<cmm>, |cw<cmp>, |cw<elc>, |cw<lfs>, |cw<man>, |cw<mas>, |cw<mkt>,
and |cw<phy>.
Use the |i<monk> command |cw<|sp(|)distribute_complete_memo> to
list individu\%als who should receive the complete memo and
|cw<|sp(|)distribute_cover_sheet>
to list those who should receive only the cover sheet.
The command |cw<|sp(|)att> gives relevant |s(AT&T) release information:
|cw<is_release> marks the |i<yes> or |i<no> box
as specified by its argument.
|cw<|sp(|)director_name> identifies the director of the first author.
Finally, the command |cw<|sp(|)cover_sheet> prints the following cover sheet.
.Tm References	s
Note that this sample produces four pages:
one page of text, one page of references, and two pages of cover sheet.
.br
.ne 26v
|begin(ruled.display)
|postscript(file "samples/tmcs.ps",
			x 3.4i, y 7i, xoffset -.1i, yoffset -1.1i)
|blank_space(25v)
|end(ruled.display)
|blank_space(1v)
...........
.Tm Typesetting a Release Paper	s
.Tm Release Papers	s

|subsection(Typesetting a Release Paper)

There is a |i<monk> sample for release papers, which can be modified
in the same way as
the documents already discussed.
However, you may often want to convert a technical memorandum
into a release paper.
.Tm Content Formatting Commands	s
.Tm Footnotes
To do so, change the first line from
|cw<|sp(|)make(tm)> to |cw<|sp(|)make(rp)>
and then remove or comment out (see Section |remember(comment_section))
the information specific to the
|i<tm> style:

|begin(bullet_list)
|item |cw<other>
|item |cw<add_totals>
|item |cw<mercury>
|item |cw<distribute_cover_sheet>
|item |cw<distribute_complete_memo>
|item |cw<cover_sheet>
|item |cw<att>
|item |cw<signature>
|item |cw<director_name>
|end(bullet_list)

|section(Content Formatting Commands)

For any document type, it is possible to format section titles,
figure captions, footnotes, and two-column output.
It is possible to store page numbers, section numbers or figure numbers
for later use.
.Tm equations
.Tm Citations
.Tm Lists	s
An interface is provided to existing routines that produce
tabular output, equations, citations, and reference lists.
This section describes the relevant commands.
.Tm Comments	s
.Tm Comments	s

|subsection(Comments)
|save(comment_section)

Comments are valuable notations by and for the person preparing a document.
They can be used
to temporarily omit a section,
to mark a section that needs work or information,
and to retain an alternative word or phrase.
.Tm delimiters	s
Anything that appears within the delimiters of a |cw<|sp(|)comment>
is not processed as part of the document:
|i<monk> commands are ignored, and text does not appear in the final output.
.Tm Spacing	s
.Tm Spacing	s

|subsection(Spacing)


You can start a new page or leave vertical blank space using these commands.

|begin(table)
center;
lf5 2 l.
|sp(|s)new_page begins a new page
|sp(|s)new_column begins a new column
|sp(|s)blank_space leaves a blank line
|sp(|s)blank_space(3)	leaves 3 blank lines
|sp(|s)blank_space(1.5i)	leaves 1.5 inches
|sp(|s)blank_space(|sp(minus)1)	moves back one line
|end(table)
.Tm Positioning Text: Indenting and Centering	s

|subsection(Positioning Text: Indenting and Centering)

Indenting and centering are the most common ways of positioning text.
The input text

|begin(cwfigure)
|sp(|)indent(begins a new line and indents
the left hand margin as long as it applies
for a single line, a group of lines,
or an entire paper.
|sp(|)indent<Multiple commands will
indent progressively.>)
|end(cwfigure)

produces the following output:

|indent(begins a new line and indents
the left hand margin as long as it applies
for a single line, a group of lines,
or an entire paper.
|indent<Multiple commands will
.Tm delimiters	s
indent progressively.>)
|p
The command |cw<|sp(|)center> begins a new line and
centers each line of text within its delimiters.
The input, taken from Monty Python,

|begin(cwfigure)
|sp(|)begin(center)
I'm a lumberjack and I'm okay,
I sleep all night and I work all day.
I cut down trees, I eat my lunch,
I go to the lavat'ry,
On Wednesdays I go shopping and have
buttered scones for tea.
|sp(|)end(center)
|end(cwfigure)

produces

|begin(figure)
|begin(center)
I'm a lumberjack and I'm okay,
I sleep all night and I work all day.
I cut down trees, I eat my lunch,
I go to the lavat'ry,
On Wednesdays I go shopping and have
buttered scones for tea.
|end(center)
|end(figure)

|p
In contrast, the command |cw<|sp(|)center_block> centers the entire figure
as a block.
All the lines are left-justified and
the figure is centered based on the width of the longest line.
It produces

|begin(figure)
|begin(center_block)
I'm a lumberjack and I'm okay,
I sleep all night and I work all day.
I cut down trees, I eat my lunch,
I go to the lavat'ry,
On Wednesdays I go shopping and have
buttered scones for tea.
|end(center_block)
|end(figure)

|p
Multi-line quotations are appropriately indented and offset by the
command |cw<|sp(|)quotation>.

|quotation<Four score and seven years ago our fathers brought forth
to this continent a new nation, conceived in liberty and dedicated to
.Tm Document Styles	s
the proposition that all men are created equal.>

|subsection(Document Styles)|save(style_section)

By default,
all documents are single-spaced and single column.
Page width is 6 inches; length is 11 inches.
.Tm Spacing	s
The font family is |i{times};
the font size is 10p on a vertical spacing of 12p.
Paragraphs skip a line and begin the text flush with the left margin.
|p
You can change any of these defaults using the |cw(|sp(|)style) command.
The command |cw<|sp(|)style( page_width 7i)> sets the page width to 7 inches.
To get the maximum page width on most output devices, type:
|cw<|sp(|)style(page_offset 0, page_width 8.5i)>.
The |cw(page_length) and |cw(font_size) can be set in a similar manner.
|p
|cw<|sp(|)style(font_size 14)> changes the default font to size 14 to print
out your papers for your grandparents or to prepare invitations to a party.
|cw(|sp(|)big) or |cw(|sp(|)small)
will still provide one size bigger or smaller, respectively.
|table_caption(The alphabet in common type families)
|save(The alphabet in common type families)
|begin(table)
l l l
lfI lfI lfI
lfB lfB lfB
l l l
lf2 lf2 lf2
lf3 lf3 lf3
l l l
lf2 lf2 lf2
lf3 lf3 lf3 .
times	roman	abcdefghijklm...uvwxyz
	italics	abcdefghijklm...uvwxyz
	bold	abcdefghijklm...uvwxyz
|style(font_family palatino)
palatino	roman	abcdefghijklm...uvwxyz
	italics	abcdefghijklm...uvwxyz
	bold	abcdefghijklm...uvwxyz
|style(font_family helvetica)
helvetica	roman	abcdefghijklm...uvwxyz
	italics	abcdefghijklm...uvwxyz
	bold	abcdefghijklm...uvwxyz
|end(table)
.Tm Fonts	s
|style(font_family times)
|p
Font families provide complementary styles for each of the four fonts:
bold, italics, roman, and cw.
The standard font family is times.
Alternative font families include
palatino, helvetica, bembo, optima, souvenir, and cw,
but not all families are available on all output devices.
Some examples are shown in Table |remember(The alphabet in common type families).
To select helvetica, type |cw<|sp(|)style( font_family helvetica)>.
.Tm Spacing	s
|p

|style(double_space)

|cw<|sp(|)style(double_space)> double spaces the text, while
|cw<|sp(|)style( single_space)> restores single spacing.

|style(single_space)

You can change the paragraph style throughout the rest of your document
by typing the command |cw<|sp(|)style(paragraph_style indented)> or
|cw<|sp(|)style(paragraph_style numbered)>,
after which the simple command |cw<|sp(|)p>
will produce indented or numbered paragraphs.
To indent or number a single paragraph,
use |cw<|sp(|)p(i)> or |cw<|sp(|)p(n)>, respectively.
To restore the original style paragraph,
type |cw<|sp(|)style( paragraph_style flushed)>.
|p
The command |cw<|sp(|)style(one_column)> begins formatting text in one column.
The command |cw<|sp(|)style(two_column)> restores two column formatting.
.Tm Fonts	s

|subsection(Fonts)

|i<Monk> currently provides abbreviated commands for
four type faces and two type sizes.
|cw(|sp(|)bold) or |cw(|sp(|)b) makes the enclosed text |b(bold).
|cw(|sp(|)italics) or |cw(|sp(|)i) produces |italics(italicized text),
while |cw(|sp(|)roman) or |cw(|sp(|)r) restores the roman character set.
The command |cw<|sp(|)constant_width> or |cw<|sp(|)cw>
|cw(selects a font that resembles low quality line printer
output and is intended only for examples of such output).
.Tm Fonts	s
|p
When fonts are embedded, the innermost dominates.
For example,
|cw(|sp(|)italics<Italics flourish around the |sp(|)roman[roman] text>) produces
|italics<Italics flourish around the |roman[roman] text>.
If an output device is not able to italicize, then it will underline.
Because italics look better, it is always preferable to request them
so that the document will look its best on the available printing device.
However, it is possible to specify underlining using |cw(|sp(|)underline) or |cw(|sp(|)u).
|p
|cw(|sp(|)big) makes the font size |big(bigger),
while |cw(|sp(|)small) makes the font size |small(smaller).
These can be nested, so that |cw<|sp(|)big(|sp(|)big(this is big))> produces
|big(|big(this is big)).
.Tm Spacing	s
However, |cw<|sp(|)big> and |cw<|sp(|)small> are designed for in-line font changes;
they do not change the vertical spacing.
To change the size of a block of text, use the |cw<|sp(|)style> command
(see Section |remember(style_section)).
.Tm Special characters	s
.Tm Special characters	s

|subsection(Special characters)
|save(special_section)

|i<Monk> provides a long list of special characters, the appearance of which
is ultimately determined by the output device.
.Tm Letters	s
.Tm Mathematical Symbols	s

With the command |cw<|sp(|)sp>,
the user can print out the |i<monk> command character |cw(|sp(|s)),
Greek letters, and mathematical symbols.
|p
Here is a partial list of the fancy symbols
that appear in Appendix |remember(special_appendix):
|sp{/} |sp{minus} |sp{\}
|sp{.} |sp{degree} |sp{bullet}
|sp{copyright} |sp{registered} |sp{TM}
|sp{|} |sp{bold vertical}
|sp{dagger} |sp{section}
|sp{rhand} |sp{lhand}
|sp{hyphen} |sp{underrule}
|sp{open quote} |sp{close quote}
|sp{grave} |sp{cedilla} |sp{lower case umlaut} |sp{upper case umlaut}
|sp{alpha} |sp{beta} |sp{gamma} |sp{delta} |sp{epsilon}
|sp{+-} |sp{>=} |sp{<=} |sp{!=} |sp{-wig} |sp{wig}
|sp{times} |sp{divide} |sp{prop}
|sp{integral} |sp{partial}
|sp{not} |sp{or} |sp{rceil}
|sp{square} |sp{del} |sp{empty} |sp{member}
|sp{isuperset} |sp{cap}
|sp{->} |sp{uarrow} |sp{darrow}
|sp{cent} |sp{quarter} |sp{3quarter}
|sp{mu} |sp{nu} |sp{xi} |sp{omicron}
|sp{pi} |sp{ff} |sp{ffi} |sp{ffl} |sp{TEX} |sp{LATEX}.
.Tm Spacing	s
|p
|cw<|sp(|)sp> can also be used to control spacing and carriage movement
precisely.
There are spaces of five different widths,
shown below opposite the commands that produce them.

|comment[|begin(verbatim)
|cw<|sp(|)sp{zero width space}> `|sp{zero width space}'
]
|begin(cwfigure)
.ta 1.6i
|sp(|)sp{space}	|r<|big<|big<`|sp{space}'>>>
|sp(|)sp{digit space}	|r<|big<|big<`|sp{digit space}'>>>
|sp(|)sp{narrow space}	|r<|big<|big<`|sp{narrow space}'>>>
|sp(|)sp{half narrow space}	|r<|big<|big<`|sp{half narrow space}'>>>
|sp(|)sp{zero width space}	|r<|big<|big<`|sp{zero width space}'>>>
.ta
|end(cwfigure)

|p

|style(double_space)

The carriage can be moved up and down:
|cw<|sp(|)sp{up}> moves |sp{up}up,|sp{down}
while |cw<|sp(|)sp{reverse}> moves
|sp{reverse}up|sp{down}|sp{down}by twice as much.
The command |cw<|sp(|)sp{down}> moves |sp{down}down.|sp{up}

|style(single_space)

|p
|cw<|sp(|)sp{break and spread}> pads the current line according to the document
style and breaks to the next line.
.Tm hyphenation	s
To force hyphenation at a particular position in a word, use
|cw<|sp(|)sp{hyphenation}>.
|cw<|sp(|)sp{tab}> generates horizontal motion to the next tab stop;
|cw<|sp(|)sp{leader}> generates a string of periods to the next tab stop.
.Tm Headings	s
.Tm Section headings	s
.Tm Headings	s

|subsection(Headings)

|comment<|subsection(Section headings)>

|subsubsection(Numbered headings.)

|cw<|sp(|)chapter> starts a new page; leaves a blank space;
puts ``Chapter |i<n>'' against the right hand margin;
skips more space;
puts the title against the right hand margin;
and skips more space before beginning the text body.
No chapters appear in this document.
.Tm Content Formatting Commands	s
.Tm Headings	s
.Tm Headings	s
The commands
to declare the current section, subsection, and subsubsection include:

|begin(cwfigure)
|sp(|)section(Content Formatting Commands)
|sp(|)subsection(Headings)
.Tm List of Commands	s
|sp(|)subsubsection(Numbered headings.)
|end(cwfigure)

The first appendix at the end was produced by
|cw<|sp(|)appendix(List of Commands)>.
|paragraph(numbered)
This numbered paragraph is produced by the command
|cw<|sp(|)paragraph(numbered)>,
which may be abbreviated as |cw<|sp(|)paragraph(n)> or |cw<|sp(|)p(n)>.
.Tm Unnumbered headings	s

.Tm Unnumbered headings	s
.Tm Spacing	s
|subsubsection(Unnumbered headings.)

The analogous unnumbered headings produce the same spacing but omit
heading names, e.g. chapter, and numerical labels.
Chapter titles remain right justified,
and appendix titles are still centered;
but the various section titles are now flush with the left margin.
|cw<|sp(|)unnumbered_chapter> is useful for preface and acknowledgement
pages.
Available commands include:

|begin(cwfigure)
|sp(|)unnumbered_chapter(Preface)
|sp(|)unnumbered_section(|small[SECTION])
|sp(|)unnumbered_subsection(Subsection)
|sp(|)unnumbered_subsubsection(Subsubsection.)
|sp(|)unnumbered_appendix(|small[APPENDIX])
|end(cwfigure)

|p(f)
There are flushed and indented unnumbered paragraphs.
The indented paragraphs are used throughout this paper,
except for this paragraph,
which is produced using the command |cw<|sp(|)p(f)>.
.Tm Spacing	s
The commands to skip a line and begin the text flush with the left margin
include:

|begin(cwfigure)
|sp(|)paragraph(f)
|sp(|)p(f)
|sp(|)paragraph(flushed)
|sp(|)p(flushed)
|end(cwfigure)

The commands to indent the first line of the paragraph and
produce spacing identical to the flushed paragraph include:

|begin(cwfigure)
|sp(|)paragraph(i)
|sp(|)p(i)
|sp(|)paragraph(indented)
|sp(|)p(indented)
|end(cwfigure)
|blank_space(-1)

|subsubsection(Paragraphs.)

The command |cw(|sp(|)paragraph) or its abbreviation |cw(|sp(|)p)
is used throughout this paper to produce the default paragraph style.
In this paper the normal default,
|cw(flushed), was changed through use of the |cw<|sp(|)style> command to |cw(indented)
(see Section |remember(style_section)).
.Tm Setting page, heading and figure numbers	s

|subsection(Setting page, heading and figure numbers)

Page, heading, figure, and table numbers can be changed by the authors.
|index(set page numbers)|index(page numbers)
|index(section numbers)|index(section counters)
Set the current page counter to three using
the command |cw<|sp(|)set_counter(page 3)>.
At the next page boundary the page number will be incremented to four.
The command |cw<|sp(|)set_counter( table 12)>
sets the current table number to 12.
Multiple counters can be set with a single command:
|cw<|sp(|)set_counter(page 10, chapter 4, section 3, subsection 1)>.
Available commands include:

|begin(cwfigure)
|sp(|)set_counter(page 3)
|sp(|)set_counter(chapter 4)
|sp(|)set_counter(section 1)
|sp(|)set_counter(subsection 9)
|sp(|)set_counter(subsubsection 3)
|sp(|)set_counter(paragraph 0)
|sp(|)set_counter(appendix 3)
|sp(|)set_counter(footnote 17)
|sp(|)set_counter(note 122)
|sp(|)set_counter(figure 8)
|sp(|)set_counter(table 0)
|end(cwfigure)

|p
Care must be taken in positioning all these commands:
if you want to change a section number, then you must position the
|cw<|sp(|)set_counter> command before the section itself
so that the numbered title will be correct.
To number the next section four, you must type
|cw<|sp(|)set_counter(section 3)> before the |cw<|sp(|)section> command,
because the section number will be incremented
when the |cw<|sp(|)section> command itself is read.
Similarly,
if you want the page number in headers and footers to display a new value,
then the value must be changed before the break to the new page occurs.
To set the next page to page number three,
the command |cw<|sp(|)set_counter(page 2)> must appear on the current page.
|p
Once a value is set, the computer will use that number and increment it
appropriately; but if it is set wrong, it will remain wrong.
If you change counters, you are responsible for
insuring consistency and correctness.
.Tm Setting page headers and footers	s

|subsection(Setting page headers and footers)

Both page headers and footers can be set using the |cw<|sp(|)set_string> command
as follows:

|begin(cwfigure)
|sp(|)set_string(page_headers "'left'center'right'")
|sp(|)set_string(even_headers "'left'center'right'")
|sp(|)set_string(odd_headers "'left'center'right'")
|sp(|)set_string(page_footers "'left'center'right'")
|sp(|)set_string(even_footers "'left'center'right'")
|sp(|)set_string(odd_footers "'left'center'right'")
|end(cwfigure)

|cw<Page_headers>, |cw<even_headers>, and |cw<odd_headers> set the headers
on all pages, even pages, and odd pages, respectively.
|index(headers)|index(page headers)
Page headers appear above the even and odd headers.
.Tm Strings	s
All headers consist of three strings, any of which may be blank or long
enough to overlap.
.Tm Strings	s
Any character, can be used to delimit the strings.
.Tm Strings	s
The strings |i(left), |i(center), and |i(right) are left-justified, centered,
and right-justified, respectively.
Parallel commands serve
.Tm Lists	s
.Tm Lists	s
.Tm Letters	s
to specify page footers.|index(footers)|index(page footers)

|comment<|subsection(Grouping text - Displays)>

|subsection(Lists)

There are nine types of lists, which differ only in the labeling of list items:

|begin(cwfigure)
|sp(|)number_list
|sp(|)A_list
|sp(|)a_list
|sp(|)I_list
|sp(|)i_list
|sp(|)p_number_list
|sp(|)p_a_list
|sp(|)dash_list
|sp(|)bullet_list
|end(cwfigure)

A simple numbered list is created by the following commands:

|begin(cwfigure)
|sp(|)begin(number_list)
|sp(|)item first item
|sp(|)item second item is on two lines in the
input, but will be filled in the output
|sp(|)item third
|sp(|)end(number_list)
|end(cwfigure)

|p
|cw<|sp(|)A_list> items are marked with capital letters;
|cw<|sp(|)a_list> with lower-case letters followed by '|cw<)>'.
|cw<|sp(|)I_list> items are marked with upper-case roman numerals;
|cw<|sp(|)i_list> with lower-case roman numerals followed by '|cw<)>'.
Number and lower-case letter labels are enclosed in parentheses in
|cw<|sp(|)p_number_list> and |cw<|sp(|)p_a_list>, respectively.
Bullets or dashes label the items in a |cw<|sp(|)bullet_list>
or a |cw<|sp(|)dash_list>, respectively.
.Tm Lists	s
.Tm Lists	s
.Tm Lists	s
|p
Multi-level lists of up to six levels can be built by putting lists inside
lists.
A standard outline format of six levels consists of an outer |cw<|sp(|)I_list>,
an |cw<|sp(|)A_list>, a |cw<|sp(|)number_list>, an |cw<|sp(|)a_list>,
an inner |cw<|sp(|)number_list>, and a |cw<|sp(|)p_a_list>.
The following produces, in outline format, a description of two kingdoms.
.Tm Inserting other files	s

|begin(cwfigure)
|sp(|)begin(I_list)
|sp(|)item Animal Kingdom
|sp(|)begin(A_list)
 |sp(|)item Arthropods
|sp(|)begin(number_list)
  |sp(|)item Trilobites
  |sp(|)item Insects
|sp(|)end(number_list)
 |sp(|)item Chordates
|sp(|)begin(number_list)
  |sp(|)item Vertebrates
|sp(|)begin(a_list)
   |sp(|)item Birds
   |sp(|)item Reptiles
   |sp(|)item Mammals
|sp(|)begin(number_list)
    |sp(|)item Marsupials
    |sp(|)item Monotremes
|sp(|)begin(p_a_list)
     |sp(|)item platypus
     |sp(|)item echidna
|sp(|)end(p_a_list)
    |sp(|)item Placental mammals
|sp(|)begin(p_a_list)
     |sp(|)item Cetacea
     |sp(|)item Primates
|sp(|)end(p_a_list)
|sp(|)end(number_list)
|sp(|)end(a_list)
  |sp(|)item Notocords
|sp(|)end(number_list)
 |sp(|)item Mollusks
|sp(|)end(A_list)
|sp(|)item Plant Kingdom
|sp(|)end(I_list)
|end(cwfigure)

|begin(I_list)
|item Animal Kingdom
|begin(A_list)
 |item Arthropods
|begin(number_list)
  |item Trilobites
  |item Insects
|end(number_list)
 |item Chordates
|begin(number_list)
  |item Vertebrates
|begin(a_list)
   |item Birds
   |item Reptiles
   |item Mammals
|begin(number_list)
    |item Marsupials
    |item Monotremes
|begin(p_a_list)
     |item platypus
     |item echidna
|end(p_a_list)
    |item Placental mammals
|begin(p_a_list)
     |item Cetacea
     |item Primates
|end(p_a_list)
|end(number_list)
|end(a_list)
  |item Notocords
|end(number_list)
 |item Mollusks
|end(A_list)
|item Plant Kingdom
|end(I_list)

|subsection(Inserting other files)

Other files may be inserted using the command |cw<|sp(|)insert>.|index(insert)
These files will also be processed by |i<monk> and
may in turn insert other files.
Loops will indeed cause disaster.
A file that will not be processed may also be included
.Tm Figures
using the |cw<|sp(|)source> command.|index(source)

|subsection(Figures)

The lumberjack poem above was formatted as a figure.
.Tm Figures
Figures are printed with blank space above and below and can be split
across a page boundary.
Lines are not filled.
.Tm delimiters	s
If you want some lines to be filled, put them within the delimiters of the
command |cw<|sp(|)text>.
|cw<|sp(|)figure_caption> centers and labels the caption.
Given

|begin(cwfigure)
|sp(|)begin(figure)
Like the lumberjack poem above,
this text is within a |sp(|)cw<|sp(|)sp(|sp(|))figure>
command.
|sp(|)end(figure)
|sp(|)figure_caption(This is a figure caption).
|end(cwfigure)

|i<monk> produces

|begin(here)
|begin(figure)
Like the lumberjack poem above,
this text is within a |cw<|sp(|)figure>
command.
|end(figure)
.Tm Spacing	s
|figure_caption(This is a figure caption)
|end(here)

|p
To create a non-filled display without spacing above and below, use the
command |cw<|sp(|)verbatim>.
To indent either a figure or a verbatim display,
simply enclose it within an |cw<|sp(|)indent> command:

|begin(cwfigure)
|sp(|)begin(here)
|sp(|)begin(indent)
|sp(|)begin(figure)
This is a short figure
|sp(|)end(figure)
|sp(|)end(indent)
|sp(|)figure_caption(with a short caption)
|sp(|)end(here)
|end(cwfigure)

The command |cw<|sp(|)indent> can appear either inside or around the
|cw<|sp(|)figure> command.
However, because the caption is centered, it should not be indented.
.Tm delimiters	s
|p
The |cw<|sp(|)here> command keeps all the text within its delimiters
together on one page.
.Tm delimiters	s
It is important to include both the figure proper and its caption within
the delimiters so that the caption is never orphaned.
If there is not enough room on the current page, |cw<|sp(|)here>
leaves the rest of the
page blank and skips to the next page or next column to print the block.
.Tm Comments	s
It is useful after comments such as ``An example follows:'',
because it guarantees the relative position of the comment and the block.
|p
Another command, |cw<|sp(|)around>, also keeps the enclosed text together.
If it cannot print the block on the current page, however, it continues
printing text, postponing the block until there is enough room for it.
Usually the block will be positioned at the top of the next page;
however, if there are multiple large floating blocks that cannot fit together
on that page, then they will appear in order as soon as possible.
You can use the command |cw<|sp(|)around_placement> to force all outstanding
blocks to be printed at the end of a section or chapter.
.Tm equations
.Tm |i<eqn>	C
.Tm equations

|subsection(Equations)

|i<Monk> uses |i<eqn>|reference<latest eqn> to format equations.
The |i<monk> command |cw<|sp(|)equation> delimits an equation offset from the text.
Note that |i<monk> commands may appear within the line;
they do not need to be at the beginning of a line.

|begin(cwfigure)
|sp(|)begin(center)
|sp(|)equation_number[(1a)]
|sp(|)begin(equation)x = 2 pi int
sin ( omega t ) dt
|sp(|)end(equation)
|sp(|)end(center)
|end(cwfigure)

When formatted the above input produces:

|begin(center)
|equation_number[(1a)]
|begin(equation)x = 2 pi int
sin ( omega t ) dt
|end(equation)
|end(center)
|blank_space(-1)

The same output is produced by the short |i<monk> form:
|cw<|sp(|)equation[x = 2 pi int sin ( omega t ) dt]>.
Captions are specified using |cw<|sp(|)equation_caption>.
|p
The equation itself
is explained on page 2 of the Kernighan and Cherry memo.|footnote{Remember
that the |i<monk> command |cw<|sp(|)equation(...)> replaces all
instances of |cw<.EQ> and |cw<.EN> and that |cw<|sp(|)e(...)> replaces the
.Tm delimiters	s
.Tm equations
.Tm equations
|cw<$...$> delimiters for in-line equations.}
To write in-line equations,
delimit the equation using |cw<|sp(|)e>.
The input |cw{|sp(|)e<x = 2 pi int sin ( omega t ) dt>}
produces the equation |e<x = 2 pi int sin ( omega t ) dt>
without offsetting it.
.Tm tables
.Tm tables
.Tm tbl	C
.Tm tbl	C

|subsection(Tables)
|save(table_example)

|i<Monk> does not provide any new capabilities for formatting tables;
it uses |i(tbl)|reference(latest tbl).
.Tm tbl	C
It provides |i<monk>-style commands for positioning both the table and its caption
and automatically invokes the standard |i(tbl) formatting program.
The following input formats the table in the file |i(examples/example.table),
which appears as Appendix |remember(table_appendix).|footnote{The
formatting commands |cw<.TS> and |cw<.TE>
need to be omitted from the sample file.}
The table could be typed into the file directly; here it is put into another
file just to simplify and condense the |i<monk> example.

|begin(cwfigure)
|sp(|)begin(here)
|sp(|)table_caption(Example of table)
|sp(|)begin(table)
|sp(|)insert(examples/example.table)
|sp(|)end(table)
|sp(|)end(here)
|end(cwfigure)

|p
The command |cw<|sp(|)here> guarantees that the caption will not be separated
from the table if it happens to hit a page boundary.
The remainder of the page would be blank,
and the table with caption would be printed on the next page
or in the next column for multi-column documents.
.Tm tbl	C
Together |i<monk> and |i<tbl> produce

|begin(here)
\!.sp -1
|table_caption(Example of table)
|begin(table)
|comment<inserting examples/example.table starting here>
box;
c1 s1 s
c1 | c1 | c
l1 | l1 | n.
Major New York Bridges
=
Bridge	Designer	Length
_
Brooklyn	J. A. Roebling	1595
Manhattan	G. Lindenthal	1470
Williamsburg	L. L. Buck	1600
_
Queensborough	Palmer &	1182
	Hornbostel
_
		1380
Triborough	O. H. Ammann	_
		383
_
Bronx Whitestone	O. H. Ammann	2300
Throgs Neck	O. H. Ammann	1800
_
George Washington	O. H. Ammann	3500
|comment<inserting examples/example.table ending here>
|end(table)
|end(here)

|p
If you have a 5620 or a 630MTG terminal,
the interactive table editor |i<vtbl>
.Tm tables
.Tm tbl	C
can be used to prepare |i<tbl> input.|reference(Murrel bitmaps build tables)
Both the table structure and its text contents can be edited using |i<vtbl>.
.Tm Figures

|subsection(Computer Output)

The command |cw<|sp(|)computeroutput> produces a figure using a constant width
type that resembles low quality line printer output.

|begin(computeroutput)
Program: SINDSCAL-symmetric indscal

***********************************
parameters
 dim  irdata  itmax   irn
  2      2    200     9342
no. of matrices=5  no. of stim.=9
***********************************

 correlations    vaf       loss
 y(data)&yhat   (r**2)  (y-yhat)**2
   .161169     .025975    .974025
.Tm Computer Output	s
.Tm Computer Output	s
   .650655     .423352    .605140
|end(computeroutput)
|blank_space(-2)
|computeroutput_caption(Example of computer output)

|cw<|sp(|)computeroutput_caption (Example of computer output)>
provides the associated caption.
Currently these captions are the same as figure captions.
.Tm pictures
.Tm |i<pic>	C
.Tm typesetting	g

|subsection(Pictures)
|save(picture_example)

|i<Monk> commands also provide access to |i<pic>, a graphics language for
typesetting.|reference[latest pic]
Borrowing an example from the |i<PIC User Manual>,|footnote{Remember
that the |i<monk> command |cw<|sp(|)picture(...)> replaces all
instances of |cw<PS> and |cw<PE>.}
the |i<monk> input

|begin(cwfigure)
|sp(|)begin(here)
|sp(|)begin(picture)
|sp(|)insert(examples/example.pic1)
|sp(|)end(picture)
|sp(|)picture_caption(Example of picture)
|sp(|)end(here)
|end(cwfigure)

produces
.Tm |i<pic>	C

|begin(picture)
|comment<inserting examples/example.pic1 starting here>
A:	ellipse
	ellipse ht .2 wid .3 with .se at 1st ellipse.nw
	ellipse ht .2 wid .3 with .sw at 1st ellipse.ne
	circle rad .05 at 0.5 <A.nw,A.c>
	circle rad .05 at 0.5 <A.ne,A.c>
	arc from 0.25 <A.w,A.e> to 0.75 <A.w,A.e>
|comment<inserting examples/example.pic1 ending here>
|end(picture)
|picture_caption(Example of picture)

|p
If you have a 5620 or a 630MTG terminal,
the interactive drawing program |i<cip>
can be used to prepare |i<pic> input.|reference(latest cip)
|p
Another example from Kernighan's |i<PIC Revised User Manual> illustrates
.Tm delimiters	s
the use of an in-line equation embedded in a picture.|footnote{In
this example, the equation within the delimiters must be placed within the
.Tm Strings	s
.Tm Strings	s
|cw<|sp(|)e> command.}

|comment<box "|sp(|s)e[space 0 {H( omega )} over {1 - H( omega )}]">

|begin(cwfigure)
|sp(|)begin(here)
|sp(|)begin(picture)
arrow
box "|sp(|)e[{H( omega )} over {1 - H( omega )}]"
arrow
|sp(|)end(picture)
|sp(|)picture_caption(Picture with equation)
|sp(|)end(here)
|end(cwfigure)

produces the following beauty

|begin(here)
|begin(picture)
arrow
box "|e[{H( omega )} over {1 - H( omega )}]"
arrow
|end(picture)
|picture_caption(Picture with equation)
|end(here)


|subsection(Footnotes)

A numbered footnote is generated by the command |cw[|sp(|)footnote] appended to
.Tm Footnotes
the running text.|footnote(This is a numbered footnote)
The text |cw<|sp(|)footnote(This is yet another numbered footnote)>
produces
.Tm Footnotes
.Tm Footnotes
numbered footnotes.|footnote(This is yet another numbered footnote)
Analogously, |cw<|sp(|)dagnote> and |cw<|sp(|)starnote> produce
daggered footnotes|dagnote(This is a daggered footnote)
and starred footnotes.|starnote(This is a starred footnote)
Footnote types can be mixed, as in this section.
.Tm Citations
.Tm cite	G
.Tm Citations

|subsection(Citations)
|save(save_section)

|i<Monk> uses the program |i<cite> to handle citations.
The command |cw<|sp(|)save(tag)>
remembers the last important numbered item as the label
|i[tag].
It saves the number of the last figure, picture, equation, table,
section, subsection, subsubsection or appendix.
At this point the command |cw<|sp(|)save(this_section)> stores
the number of the current subsection as |cw(this_section).
|save(this_section)
The |i(monk) command |cw<|sp(|)remember(this_section)> is replaced by
that stored value wherever it appears in the text:
|cw[This section is |sp(|)remember(this_section)] appears in the
final output as This section is |remember(this_section).
It is crucial to understand that |cw[|sp(|)save] stores the most recent
counter, whatever its type.
If a numbered figure appears between the section header and the |cw[|sp(|)save]
command, then the number saved will be that figure number.
The tag name itself has no particular meaning.
Position is what counts.
|p
Page numbers can be saved using the command |cw<|sp(|)savepage.>
|cw<|sp(|)savepage(tag)> saves the current page number as
the label |cw[tag].
Again, |cw<|sp(|)remember(tag)> substitutes the number
associated with |cw[tag].
The text
|cw<the current page is |sp(|)savepage(foobar) |sp(|)remember(foobar)>
generates |cw<the current page is |savepage(foobar)|remember(foobar)>.
.Tm Citations
|p
If all citations are saved before they are used, then the user need
only enter the commands shown above in the |i<monk> input text.
If, however, the input uses a citation before it really is saved,
as in the example below, then there is more to do.

|begin(cwfigure)
All commands are listed in appendix
|sp(|)remember(appendix_commands).
.Tm List of Commands	s
.Tm cite	G
|sp(zero width space)|b(...)
|sp(|)appendix(|s(LIST OF COMMANDS))
|sp(|)save(appendix_commands)
|end(cwfigure)

Forward referencing depends on a file named |cw<.cite>,
which is generated each time the input is formatted.
.Tm cite	G
.Tm cite	G
.Tm cite	G
.Tm Citations
The |small<UNIX> command

|begin(figure)
|cw(cite |sp(minus)s .cite > myfile.cite)
|end(figure)

reports any redefinitions or unused definitions and produces a consistent
set of citations.
.Tm cite	G
The output file |cw<myfile.cite> needs to be inserted in the |i<monk> input
before the first citation.
.Tm cite	G
Do so using the command |cw<|sp(|)insert(myfile.cite)>.

|comment[
|subsection(Creating an Index)
You can specify keywords to be inserted in an index using |cw(|sp(|)index):
|cw<|sp(|)index(these words)>.
The keywords |i<these words> will appear in the index with the current page
number.]
........
.Tm References	s
.Tm References	s

|subsection(References)

|i<Monk> uses |i<prefer> to extract references from a
bibliographic database.|reference<Derr Prefer>
To use |i<prefer> capabilities with |i<monk>,
there are only two additional things you need to know.
First, the |cw<|sp(|)reference> command passes a partial reference to
|i<prefer>, as in:
|cw<|sp(|)|sp(zero width space)reference( Derr Prefer)>.
When this command is used, |i<monk> will automatically invoke |i<prefer>.
.Tm Advanced Monk	s
.Tm Preparing a large document	s
.Tm References	s
Second, the |cw<|sp(minus)R> option to |i<monk> serves to specify
a local or private bibliography file:

|begin(cwfigure)
monk |sp(minus)Sproof |sp(minus)R bibliofile myfile
|end(cwfigure)

|section(Advanced Monk)

|subsection(Preparing a large document)

When working on large documents,
it is important to be able to run off parts of the document separately,
while maintaining correct numbering and references to other parts of the document.
You can do this with |i<monk> by dividing the document into several files
and identifying each file appropriately.
|p
The top-level file should contain all style information
and include the various segments.
For a book with three chapters, the parent file |cw<myfile> might look like:

|begin(cwfigure)
|sp(|)make(kluwer)
|sp(|)style(page_width 7i)
|sp(|)insert(chapter1)
|sp(|)insert(chapter2)
|sp(|)insert(chapter3)
|end(cwfigure)

The first line of each chapter file should identify it as a part
of |cw<myfile>: |cw<|sp(|)part(myfile)>.
|p
When you run off the entire document, |i<monk> will remember the assorted
section, display, and page numbers.

|comment{as well as information you save using
|cw<|sp(|)save> or |cw<|sp(|)savepage>.}

These numbers will then be used when you run off individual parts.
Note that the numbers should always be reasonable approximations, but may
not always be correct.
When several sections are changing simultaneously, you will need to
generate the entire document periodically to guarantee that all numbers
are exact.

|comment{
|section(Error messages)
The following are the error messages currently given by monk:
|begin(figure)
|begin(cw)
usage: monk [-d database_dir -n ] filename; no option g
fiugre is not a monk command: use ``|sp(|)sp(|sp(|))'' to put `|sp(|)' in text
Expecting end for figure begun at line 112; got end for table
No end for figure begun at line 112
Unexpected end for figure
Mismatched delimiters around figure; expecting >, got EOF
Mismatched delimiters around figure; expecting >, got ]
|end(cw)
|end(figure)}
......
.Tm Modifying the standard formats	s

|subsection(Modifying the standard formats)

You can redefine existing |i<monk> commands,
such as |cw<|sp(|)ret_address> and |cw<|sp(|)p>,
and you can create new document types using the standard
|i<monk> database language.
An early description of the database language is available,|reference{monk
database}
but there is no up-to-date guide.
When the dust has settled,
a new manual will be written.
Currently, any augmentations you write should be considered temporary.

|comment{
When you change an existing |i<monk> command, the message
``|cw<file line_number: Redefining definition_name>''
will be printed, where |i<file> and |i<line_number> identify where the
redefinition of |i<definition_name> was encountered.
A definition can appear anywhere in user text, but must precede its first usage.
We recommend maintaining separate files of definitions, rather than including
them in-line with your text.
The definition file can be typed on your command line,
|cw<monk definition_file text_file>, or
included in your text file after the |cw<|sp(|)make> command,
|cw<|sp(|)insert(definition_file)>.
If you change a definition in the middle of a document,
all instances before the redefinition will use the original,
all later instances the new.
|p
Certain words are reserved as internal database commands and
cannot be redefined:
|cw<associate>, |cw<attribute>, |cw<begin>, |cw<comment>, |cw<define>, |cw<end>,
|cw<environment>, |cw<for>, |cw<if>, |cw<else>, |cw<ifvalue>, |cw<insert>,
F|cw<macro>, |cw<part>, and |cw<source>.}

|p
You can also cheat by adding commands in the underlying
formatting language;|reference<troff latest reference>|reference<troff tutorial latest>
this method may offer quick fixes,
but there is a possibility of conflict with the |i<monk> program.
.Tm Fonts	s
|i<Monk> expends considerable effort managing the state of
various attributes such as fonts and indentation level.
Therefore, if you change things without |i<monk>'s knowledge
by using |i<troff> directly,
|i<monk> may well undo your changes
in the normal course of processing its own input commands.
.Tm Other related programs	s
.Tm demonk	G
.Tm monkmerge	G

|subsection(Other related programs)

There are three other programs useful to |i(monk) users:
|i(demonk) is the moral equivalent of |i(deroff),
|i(monkmerge) is the functional equivalent of |i(soelim),
and |i(monkspell) is the |i(spell) preprocessor.
These tools are helpful when processing |i(monk) files using other
programs, such as indexing tools.
.Tm List of Commands	s

|reference_placement

|new_page
.ad l
|appendix(List of Commands)


|environment(section;
	divert.string Ts;

	minimum.lines 2v, protect, incr S0,
	clear S1 S2 S3,
	font bold,
	|ifvalue sectioning bychapter [
		number CH, text ".",
	]
	|ifvalue sectioning byappendix [
		number AC, text ".",
	]
	number S0,
	|ifvalue sectioning bysection [
		text ".",
	]
	text " ", string Ts, font roman,
	paragraph,
	clear string Rs,
	|ifvalue sectioning bychapter [
		store string.from.number Rs CH,
		add string Rs ".",
	]
	|ifvalue sectioning byappendix [
		store string.from.number Rs AC,
		add string Rs ".",
	]
	add string.from.number Rs S0,
	|ifvalue sectioning bysection [
		contents section 1 Rs Ts,
	]
	|ifvalue sectioning bychapter [
		contents section 2 Rs Ts,
	]
	|ifvalue sectioning byappendix [
		contents section 2 Rs Ts,
])
|comment<summary: section (numbered section; IT)>
.Tm Global Commands	s

|section(Global Commands)

|comment<inserting commands/global starting here>
.in +.4i
.ti -.4i
\f(CWA_list\fP alphabetized list using A.
.ti -.4i
\f(CWI_list\fP numbered list using I.
.ti -.4i
\f(CWa_list\fP alphabetized list using a)
.ti -.4i
\f(CWappendix\fP numbered appendix; can be saved|dagnote(important values can be saved and remembered; see section |remember(save_section))
.ti -.4i
\f(CWapproved\fP approved
.ti -.4i
\f(CWaround\fP keep a block together here or move it to a later page
.ti -.4i
\f(CWaround_placement\fP outputs all arounds
.ti -.4i
\f(CWauthordagnote\fP footnote marked with a dagger for author names in titlebox
.ti -.4i
\f(CWb\fP bold
.ti -.4i
\f(CWbargraph\fP a bar graph in bar
.ti -.4i
\f(CWbargraph_caption\fP the bar graph's caption; can be saved
.ti -.4i
\f(CWbi\fP bolditalics
.ti -.4i
\f(CWbig\fP increase point size
.ti -.4i
\f(CWblank_space\fP leave this much blank space
.ti -.4i
\f(CWbold\fP alias for b
.ti -.4i
\f(CWbolditalics\fP alias for bi
.ti -.4i
\f(CWbullet_list\fP bulleted list using \s-2\(bu\s+2
.ti -.4i
\f(CWcenter\fP center each line
.ti -.4i
\f(CWcenter_block\fP center all lines as a block
.ti -.4i
\f(CWchapter\fP numbered chapter; can be saved
.ti -.4i
\f(CWcolumn_style\fP width, offset
.ti -.4i
\f(CWcomment\fP comment out text
.ti -.4i
\f(CWcomputeroutput\fP figure using cw
.Tm Computer Output	s
.ti -.4i
\f(CWcomputeroutput_caption\fP the computer output's caption; can be saved
.ti -.4i
\f(CWconstant_width\fP alias for cw
.ti -.4i
\f(CWcw\fP constant width
.ti -.4i
\f(CWdagnote\fP footnote marked with a dagger
.ti -.4i
\f(CWdash_list\fP dashed list using \(em
.ti -.4i
\f(CWe\fP an inline equation in eqn
.ti -.4i
\f(CWendnote\fP numbered endnote
.ti -.4i
\f(CWendnote_placement\fP outputs endnotes
.ti -.4i
\f(CWequation\fP an equation in eqn
.ti -.4i
\f(CWequation_caption\fP the equation's caption; can be saved
.ti -.4i
\f(CWequation_number\fP the equation's number; can be saved
.ti -.4i
\f(CWfigure\fP a figure
.ti -.4i
\f(CWfigure_caption\fP the figure's caption; can be saved
.ti -.4i
\f(CWfootnote\fP numbered footnote
.Tm Footnotes
.ti -.4i
\f(CWfull\fP use full page width for footnotes and arounds
.ti -.4i
\f(CWgraph\fP a graph in grap
.ti -.4i
\f(CWgraph_caption\fP the graph's caption; can be saved
.ti -.4i
\f(CWhere\fP keep a block together on this page or start a new page
.ti -.4i
\f(CWi\fP italics
.ti -.4i
\f(CWi_list\fP numbered list using i)
.ti -.4i
\f(CWideal\fP a picture in ideal
.ti -.4i
\f(CWideal_caption\fP the ideal's caption; can be saved
.ti -.4i
\f(CWindent\fP indent by standard amount
.ti -.4i
\f(CWindex\fP save this item and page number for index
.ti -.4i
\f(CWindex_level0\fP index program top level without sublevels
.ti -.4i
\f(CWindex_level1\fP index program top level with sublevels
.ti -.4i
\f(CWindex_level2\fP index program sublevel
.ti -.4i
\f(CWindex_placement\fP put the index here
.ti -.4i
\f(CWinsert\fP insert a file here
.ti -.4i
\f(CWipa\fP international phonetic alphabet
.ti -.4i
\f(CWitalics\fP alias for i
.ti -.4i
\f(CWitem\fP item of a list; can be saved
.ti -.4i
\f(CWleft_adjust\fP left adjust the text
.ti -.4i
\f(CWleft_center_right\fP left justify, center, right justify a line
.Tm Footnotes
.ti -.4i
\f(CWnarrow\fP use current column for footnotes and arounds
.ti -.4i
\f(CWnew_column\fP skip to the next column
.ti -.4i
\f(CWnew_page\fP skip to the next page
.ti -.4i
\f(CWnotation\fP notation
.ti -.4i
\f(CWnumber_list\fP numbered list using 1.
.ti -.4i
\f(CWp\fP alias for paragraph
.ti -.4i
\f(CWp_a_list\fP alphabetized list using (a)
.ti -.4i
\f(CWp_number_list\fP numbered list using (1)
.ti -.4i
\f(CWparagraph\fP n, i, f, numbered, indented, flushed
.ti -.4i
\f(CWpart\fP multi-part document
.ti -.4i
\f(CWped\fP a picture in ped
.ti -.4i
\f(CWped_caption\fP the ped's caption; can be saved
.ti -.4i
\f(CWpicture\fP a picture in pic
.ti -.4i
\f(CWpicture_caption\fP the picture's caption; can be saved
.ti -.4i
\f(CWquotation\fP indent left and right
.ti -.4i
\f(CWr\fP roman
.ti -.4i
\f(CWreference\fP a reference in prefer
.ti -.4i
\f(CWreference_database\fP use this database file for prefer
.ti -.4i
\f(CWreference_include\fP  these reference in prefer
.ti -.4i
\f(CWreference_list\fP format these databases in prefer
.Tm References	s
.ti -.4i
\f(CWreference_placement\fP produce references here
.ti -.4i
\f(CWremember\fP remember the important thing or page number saved in this label
.ti -.4i
\f(CWroman\fP alias for r
.ti -.4i
\f(CWs\fP alias for small
.ti -.4i
\f(CWsave\fP save last important thing, shown as ``can be saved'', in this label
.ti -.4i
\f(CWsavepage\fP save last page number in this label
.ti -.4i
\f(CWsavestring\fP save string under this name
.ti -.4i
\f(CWsection\fP numbered section; can be saved
.ti -.4i
\f(CWset_counter\fP page, chapter, section, subsection, subsubsection, appendix, figure, table, footnote, note
.ti -.4i
\f(CWset_string\fP page_headers, even_headers, odd_headers, page_footers, even_footers, odd_footers
.ti -.4i
\f(CWsmall\fP reduce the point size
.ti -.4i
\f(CWsource\fP insert a file here without processing it
.Tm Special characters	s
.ti -.4i
\f(CWsp\fP special characters
.ti -.4i
\f(CWstarnote\fP footnote marked with a star
.ti -.4i
\f(CWstyle\fP controls general style of document
.ti -.4i
\f(CW  1_5_space\fP 1 and a half space
.ti -.4i
\f(CW  double_space\fP double space
.ti -.4i
\f(CW  font_family\fP times, palatino, bembo, optima, souvenir, helvetica, cw, gothic, condensed, narrow
.Tm Spacing	s
.ti -.4i
\f(CW  font_size\fP font_size font_spacing
.Tm Spacing	s
.ti -.4i
\f(CW  list_spacing\fP on, off
.ti -.4i
\f(CW  multi_column\fP multiple columns per page
.ti -.4i
\f(CW  one_column\fP full page output
.ti -.4i
\f(CW  page_length\fP length of the printed page
.ti -.4i
\f(CW  page_number\fP arabic, roman
.ti -.4i
\f(CW  page_offset\fP offset of the printed page
.ti -.4i
\f(CW  page_width\fP width of the printed page
.ti -.4i
\f(CW  paragraph_style\fP indented, numbered, flushed
.ti -.4i
\f(CW  reference\fP acm, apa, att, ieee, lsa, pami, spectrum
.ti -.4i
\f(CW  reference_option\fP tm, rp, nroff, troff, sort, sequence, a, d, t, ad, at, da, dt, ta, td, adt, atd, dat, dta, tad, tda
.ti -.4i
\f(CW  single_space\fP single space
.ti -.4i
\f(CW  triple_space\fP triple space
.ti -.4i
\f(CW  two_column\fP two columns per page
.ti -.4i
\f(CWsubsection\fP numbered subsection; can be saved
.ti -.4i
\f(CWsubsubsection\fP numbered subsubsection; can be saved
.Tm tbl	C
.ti -.4i
\f(CWtable\fP a table in tbl
.ti -.4i
\f(CWtable_caption\fP the table's caption; can be saved
.ti -.4i
\f(CWtc_entry\fP tc program entry
.ti -.4i
\f(CWtc_heading\fP tc program heading
.Tm Table of contents	s
.ti -.4i
\f(CWtc_placement\fP output the table of contents
.ti -.4i
\f(CWtext\fP fill the text
.ti -.4i
\f(CWu\fP underline
.ti -.4i
\f(CWunderline\fP alias for u
.ti -.4i
\f(CWunnumbered_appendix\fP unnumbered appendix
.ti -.4i
\f(CWunnumbered_chapter\fP unnumbered chapter
.ti -.4i
\f(CWunnumbered_footnote\fP unnumbered footnote
.ti -.4i
\f(CWunnumbered_section\fP unnumbered section
.ti -.4i
\f(CWunnumbered_subsection\fP unnumbered subsection
.ti -.4i
\f(CWunnumbered_subsubsection\fP unnumbered subsubsection
.ti -.4i
\f(CWverbatim\fP text exactly as it is
.Tm Letters	s
.in -.4i
|comment<inserting commands/global ending here>

|section(Letters)

|comment<inserting commands/letter starting here>
.in +.4i
.ti -.4i
\f(CWaddress\fP who to address the letter to
.ti -.4i
\f(CWbody\fP body of letter
.ti -.4i
\f(CWclosing\fP salutation
.ti -.4i
\f(CWdate\fP date
.ti -.4i
\f(CWgreeting\fP greeting
.ti -.4i
\f(CWret_address\fP return address
.ti -.4i
\f(CWsignature\fP signature
.Tm Memoranda	s
.in -.4i
|comment<inserting commands/letter ending here>

|section(Memoranda)

|comment<inserting commands/memo starting here>
.in +.4i
.ti -.4i
\f(CWabstract\fP abstract for coversheet
.ti -.4i
\f(CWauthor\fP name, initials, location, department, extension, room for title box and coversheet
.ti -.4i
\f(CWdate\fP date of tm for title box and coversheet
.ti -.4i
\f(CWdocument\fP number, file_case, work_program for title box and coversheet
.ti -.4i
\f(CWkeywords\fP keywords for cover sheet
.ti -.4i
\f(CWsignature\fP signatures and typing credits
.ti -.4i
\f(CWtitle\fP title of document for title box and coversheet
.ti -.4i
\f(CWtitlebox\fP produce title box here
.Tm Merit Review Form 1	s
.in -.4i
|comment<inserting commands/memo ending here>

|section(Merit Review Form 1)

|comment<inserting commands/form1 starting here>
.in +.4i
.ti -.4i
\f(CWdate\fP date for signature
.ti -.4i
\f(CWdepartment\fP department for titlebox
.ti -.4i
\f(CWname\fP name for titlebox and signature
.ti -.4i
\f(CWp\fP paragraph
.ti -.4i
\f(CWsignature\fP signature
.ti -.4i
\f(CWtitlebox\fP title box
.Tm Bi-Annual Research Reports	s
.in -.4i
|comment<inserting commands/form1 ending here>

|section(Bi-Annual Research Reports)

|comment<inserting commands/research starting here>
.in +.4i
.ti -.4i
\f(CWauthor\fP name, number for footer and title box
.ti -.4i
\f(CWdate\fP report, range for title box
.ti -.4i
\f(CWdepartment\fP name, number for title box
.ti -.4i
\f(CWp\fP alias for paragraph
.ti -.4i
\f(CWparagraph\fP i, indented
.ti -.4i
\f(CWsection\fP section
.ti -.4i
\f(CWsignature\fP signature
.ti -.4i
\f(CWtitlebox\fP title box
.Tm Center Phone Books	s
.in -.4i
|comment<inserting commands/research ending here>

|section(Center Phone Books)

|comment<inserting commands/centerpb starting here>
.in +.4i
.ti -.4i
\f(CWdate\fP date of center phone book
.ti -.4i
\f(CWdepartment\fP a department
.ti -.4i
\f(CWdepartment_title\fP title of department
.ti -.4i
\f(CWemployee\fP name, nicname, phone, room, email, secretary, consultant, contractor, intern, limited term, resident visitor, casual, post doc, temporary, mit coop, department
.ti -.4i
\f(CWlaboratory\fP a laboratory
.ti -.4i
\f(CWlaboratory_title\fP title of laboratory
.ti -.4i
\f(CWroom\fP name, phone, room, email
.ti -.4i
\f(CWtitlebox\fP title box
.Tm Technical Memoranda	s
.in -.4i
|comment<inserting commands/centerpb ending here>

|section(Technical Memoranda)

|comment<inserting commands/tm starting here>
.in +.4i
.ti -.4i
\f(CWabstract\fP abstract for coversheet
.ti -.4i
\f(CWadd_totals\fP text, other, figure, table, reference counts for cover sheet
.ti -.4i
\f(CWatt\fP is_release for cover sheet
.ti -.4i
\f(CWauthor\fP name, initials, location, department, extension, room for title box and coversheet
.ti -.4i
\f(CWcover_sheet\fP produce the coversheet here
.ti -.4i
\f(CWdate\fP date of tm for title box and coversheet
.ti -.4i
\f(CWdh_name\fP department head's name for cover sheet
.ti -.4i
\f(CWdirector_name\fP director's name for cover sheet
.ti -.4i
\f(CWdistribute_complete_memo\fP distribution list for complete memo
.ti -.4i
\f(CWdistribute_cover_sheet\fP distribution list for cover sheet
.ti -.4i
\f(CWdocument\fP number, file_case, work_program for title box and coversheet
.ti -.4i
\f(CWearlier\fP replacing earlier document_number for cover sheet
.ti -.4i
\f(CWgovernment_security\fP government security clearance for cover sheet
.ti -.4i
\f(CWkeywords\fP keywords for cover sheet
.ti -.4i
\f(CWmercury\fP chm, cmm, cmp, elc, lfs, man, mas, mkt, phy, std mercury distribution for cover sheet
.ti -.4i
\f(CWother\fP end of text body/start of attached pages
.ti -.4i
\f(CWproprietary_class\fP proprietary classification for cover sheet
.ti -.4i
\f(CWsignature\fP signatures and typing credits
.ti -.4i
\f(CWtitle\fP title of document for title box and coversheet
.ti -.4i
\f(CWtitlebox\fP produce title box here
.Tm Internal Memoranda	s
.in -.4i
|comment<inserting commands/tm ending here>

|section(Internal Memoranda)

|comment<inserting commands/im starting here>
.in +.4i
.ti -.4i
\f(CWabstract\fP abstract for coversheet
.ti -.4i
\f(CWadd_totals\fP text, other, figure, table, reference counts for cover sheet
.ti -.4i
\f(CWatt\fP is_release for cover sheet
.ti -.4i
\f(CWauthor\fP name, initials, location, department, extension, room for title box and coversheet
.ti -.4i
\f(CWcover_sheet\fP produce the coversheet here
.ti -.4i
\f(CWdate\fP date of tm for title box and coversheet
.ti -.4i
\f(CWdh_name\fP department head's name for cover sheet
.ti -.4i
\f(CWdirector_name\fP director's name for cover sheet
.ti -.4i
\f(CWdistribute_complete_memo\fP distribution list for complete memo
.ti -.4i
\f(CWdistribute_cover_sheet\fP distribution list for cover sheet
.ti -.4i
\f(CWdocument\fP number, file_case, work_program for title box and coversheet
.ti -.4i
\f(CWearlier\fP replacing earlier document_number for cover sheet
.ti -.4i
\f(CWgovernment_security\fP government security clearance for cover sheet
.ti -.4i
\f(CWkeywords\fP keywords for cover sheet
.ti -.4i
\f(CWmercury\fP chm, cmm, cmp, elc, lfs, man, mas, mkt, phy, std mercury distribution for cover sheet
.ti -.4i
\f(CWother\fP end of text body/start of attached pages
.ti -.4i
\f(CWproprietary_class\fP proprietary classification for cover sheet
.ti -.4i
\f(CWsignature\fP signatures and typing credits
.ti -.4i
\f(CWtitle\fP title of document for title box and coversheet
.ti -.4i
\f(CWtitlebox\fP produce title box here
.Tm Technical Correspondence	s
.in -.4i
|comment<inserting commands/im ending here>

|section(Technical Correspondence)

|comment<inserting commands/tc starting here>
.in +.4i
.ti -.4i
\f(CWabstract\fP abstract for coversheet
.ti -.4i
\f(CWadd_totals\fP text, other, figure, table, reference counts for cover sheet
.ti -.4i
\f(CWatt\fP is_release for cover sheet
.ti -.4i
\f(CWauthor\fP name, initials, location, department, extension, room for title box and coversheet
.ti -.4i
\f(CWcover_sheet\fP produce the coversheet here
.ti -.4i
\f(CWdate\fP date of tm for title box and coversheet
.ti -.4i
\f(CWdh_name\fP department head's name for cover sheet
.ti -.4i
\f(CWdirector_name\fP director's name for cover sheet
.ti -.4i
\f(CWdistribute_complete_memo\fP distribution list for complete memo
.ti -.4i
\f(CWdistribute_cover_sheet\fP distribution list for cover sheet
.ti -.4i
\f(CWdocument\fP number, file_case, work_program for title box and coversheet
.ti -.4i
\f(CWearlier\fP replacing earlier document_number for cover sheet
.ti -.4i
\f(CWgovernment_security\fP government security clearance for cover sheet
.ti -.4i
\f(CWkeywords\fP keywords for cover sheet
.ti -.4i
\f(CWmercury\fP chm, cmm, cmp, elc, lfs, man, mas, mkt, phy, std mercury distribution for cover sheet
.ti -.4i
\f(CWother\fP end of text body/start of attached pages
.ti -.4i
\f(CWproprietary_class\fP proprietary classification for cover sheet
.ti -.4i
\f(CWsignature\fP signatures and typing credits
.ti -.4i
\f(CWtitle\fP title of document for title box and coversheet
.ti -.4i
\f(CWtitlebox\fP produce title box here
.Tm Release Papers	s
.in -.4i
|comment<inserting commands/tc ending here>

|section(Release Papers)

|comment<inserting commands/rp starting here>
.in +.4i
.ti -.4i
\f(CWabstract\fP abstract for coversheet
.ti -.4i
\f(CWauthor\fP name, initials, location, department, extension, room for title box and coversheet
.ti -.4i
\f(CWdate\fP date of tm for title box and coversheet
.ti -.4i
\f(CWdocument\fP number, file_case, work_program for title box and coversheet
.ti -.4i
\f(CWkeywords\fP keywords for cover sheet
.ti -.4i
\f(CWtitle\fP title of document for title box and coversheet
.ti -.4i
\f(CWtitlebox\fP produce title box here
.Tm Association for Computing Machinery Proceedings	s
.in -.4i
|comment<inserting commands/rp ending here>

|section(Association for Computing Machinery Proceedings)

|comment<inserting commands/acm starting here>
.in +.4i
.ti -.4i
\f(CWabstract\fP abstract for coversheet
.ti -.4i
\f(CWauthor\fP name, initials, location, department, extension, room for title box and coversheet
.ti -.4i
\f(CWdate\fP date of tm for title box and coversheet
.ti -.4i
\f(CWdocument\fP number, file_case, work_program for title box and coversheet
.ti -.4i
\f(CWkeywords\fP keywords for cover sheet
.Tm References	s
.ti -.4i
\f(CWreference_placement\fP produce references here
.ti -.4i
\f(CWtitle\fP title of document for title box and coversheet
.ti -.4i
\f(CWtitlebox\fP produce title box here
.Tm Conference Proceedings Model Sheets	s
.in -.4i
|comment<inserting commands/acm ending here>

|section(Conference Proceedings Model Sheets)

|comment<inserting commands/model starting here>
.in +.4i
.ti -.4i
\f(CWabstract\fP abstract for coversheet
.ti -.4i
\f(CWauthor\fP name, initials, location, department, extension, room for title box and coversheet
.ti -.4i
\f(CWdate\fP date of tm for title box and coversheet
.ti -.4i
\f(CWdocument\fP number, file_case, work_program for title box and coversheet
.ti -.4i
\f(CWkeywords\fP keywords for cover sheet
.Tm References	s
.ti -.4i
\f(CWreference_placement\fP produce references here
.ti -.4i
\f(CWtitle\fP title of document for title box and coversheet
.ti -.4i
\f(CWtitlebox\fP produce title box here
.Tm Computer Science Press Books	s
.in -.4i
|comment<inserting commands/model ending here>

|section(Computer Science Press Books)

|comment<inserting commands/cspress starting here>
.in +.4i
.ti -.4i
\f(CWabstract\fP abstract for coversheet
.ti -.4i
\f(CWauthor\fP name, initials, location, department, extension, room for title box and coversheet
.ti -.4i
\f(CWdate\fP date of tm for title box and coversheet
.ti -.4i
\f(CWdocument\fP number, file_case, work_program for title box and coversheet
.ti -.4i
\f(CWkeywords\fP keywords for cover sheet
.ti -.4i
\f(CWtitle\fP title of document for title box and coversheet
.ti -.4i
\f(CWtitlebox\fP produce title box here
.Tm Kluwer Academic Publishers Books	s
.in -.4i
|comment<inserting commands/cspress ending here>

|section(Kluwer Academic Publishers Books)

|comment<inserting commands/kluwer starting here>
.in +.4i
.ti -.4i
\f(CWappendix\fP numbered appendix; IT
.ti -.4i
\f(CWchapter\fP numbered chapter; IT
.ti -.4i
\f(CWindex_placement\fP put the index here
.Tm References	s
.ti -.4i
\f(CWreference_placement\fP produce references here
.ti -.4i
\f(CWsection\fP numbered section; IT
.ti -.4i
\f(CWtc_heading\fP tc program heading
.ti -.4i
\f(CWunnumbered_appendix\fP unnumbered appendix
.ti -.4i
\f(CWunnumbered_chapter\fP unnumbered chapter
.ti -.4i
\f(CWunnumbered_section\fP unnumbered section
.in -.4i
|comment<inserting commands/kluwer ending here>

|section(Nursing Home Song Sheets)

|comment<inserting commands/song starting here>
.in +.4i
.ti -.4i
\f(CWd\fP directive to the singers
.ti -.4i
\f(CWm\fP margin text
.ti -.4i
\f(CWrefrain\fP refrain of the song
.ti -.4i
\f(CWsmaller\fP print the song smaller than normal
.ti -.4i
\f(CWstyle singers\fP full instructions
.ti -.4i
\f(CWtitle\fP title of the song
.ti -.4i
\f(CWverse\fP verse of the song
.Tm Special characters	s
.Tm Special characters	s
.Tm Spacing	s
.in -.4i
|comment<inserting commands/song ending here>

.ad b

|appendix(List of Special Characters)
|save(special_appendix)

The special characters that control spacing and carriage movements are
described in section |remember(special_section).
.Tm Special characters	s
All other special characters are listed below.
For each character,
column |cw<Char> contains the final printed character
and column |cw<Name> contains the keywords
given to |i<monk> to specify the character.
To print the character |sp(rhand), type |cw<|sp(|)sp(rhand)>
in your input file.
.Tm Special characters	s

|comment<inserting examples/special starting here>
|section(Common special characters)
.Tm Greek alphabet	s
.Tm Mathematical Symbols	s
|begin(table)
;
cfB 1 lfB cfB 1 lfB
cw(.3i)1 lw(.85i) cw(.3i)1 lw(.85i).
Char	Name	Char	Name
|sp{rhand}	rhand	|sp{lhand}	lhand
|sp{open quote}	open quote	|sp{close quote}	close quote
|sp{Em dash}	Em dash	|sp{minus}	minus
|sp{hyphen}	hyphen	|sp{underrule}	underrule
|sp{\}	|sp{\}	|sp{/}	/
|sp{degree}	degree	|sp{bullet}	bullet
|sp{dagger}	dagger	|sp{double dagger}	double dagger
|sp{section}	section
|sp{|} |sp{|}
|sp{.}	.
.T&
c1 le cz1 le.
		|sp{ATT logo}
|sp{TM}	TM		|s(ATT) logo
|sp{registered}	registered	|sp{copyright}	copyright
|end(table)
|section(Greek alphabet)
|begin(table)
;
cfB 1 lfB cfB 1 lfB
cw(.3i)1 lw(.85i) cw(.3i)1 lw(.85i).
Char	Name	Char	Name
|sp{alpha}	alpha	|sp{ALPHA}	ALPHA
|sp{beta}	beta	|sp{BETA}	BETA
|sp{gamma}	gamma	|sp{GAMMA}	GAMMA
|sp{delta}	delta	|sp{DELTA}	DELTA
|sp{epsilon}	epsilon	|sp{EPSILON}	EPSILON
|sp{zeta}	zeta	|sp{ZETA}	ZETA
|sp{eta}	eta	|sp{ETA}	ETA
|sp{theta}	theta	|sp{THETA}	THETA
|sp{iota}	iota	|sp{IOTA}	IOTA
|sp{kappa}	kappa	|sp{KAPPA}	KAPPA
|sp{lambda}	lambda	|sp{LAMBDA}	LAMBDA
|sp{mu}	mu	|sp{MU}	MU
|sp{nu}	nu	|sp{NU}	NU
|sp{xi}	xi	|sp{XI}	XI
|sp{omicron}	omicron	|sp{OMICRON}	OMICRON
|sp{pi}	pi	|sp{PI}	PI
|sp{rho}	rho	|sp{RHO}	RHO
|sp{sigma}	sigma	|sp{SIGMA}	SIGMA
|sp{terminal sigma}	terminal sigma
|sp{tau}	tau	|sp{TAU}	TAU
|sp{upsilon}	upsilon	|sp{UPSILON}	UPSILON
|sp{phi}	phi	|sp{PHI}	PHI
|sp{chi}	chi	|sp{CHI}	CHI
|sp{psi}	psi	|sp{PSI}	PSI
|sp{omega}	omega	|sp{OMEGA}	OMEGA
|end(table)
|section(Mathematical symbols)
|begin(table)
;
cfB 1 lfB cfB 1 lfB
cw(.3i)1 lw(.85i) cw(.3i)1 lw(.85i).
Char	Name	Char	Name
|sp{+}	+	|sp{-}	-
|sp{+-}	+-	|sp{*}	*
|sp{times}	times	|sp{divide}	divide
|sp{-wig}	-wig	|sp{wig}	wig
|sp{!=}	!=	|sp{==}	==
|sp{>=}	>=	|sp{<=}	<=
|sp{integral}	integral	|sp{partial}	partial
|sp{inf}	inf	|sp{del}	del
|sp{not}	not	|sp{or}	or
|sp{prop}	prop
|sp{lceil}	lceil	|sp{rceil}	rceil
|sp{lfloor}	lfloor	|sp{rfloor}	rfloor
|end(table)
|section(Mathematical set symbols)
|begin(table)
;
cfB 1 lfB cfB 1 lfB
cw(.3i)1 lw(.85i) cw(.3i)1 lw(.85i).
Char	Name	Char	Name
|sp{member}	member	|sp{cup}	cup
|sp{empty}	empty	|sp{cap}	cap
|sp{subset}	subset	|sp{superset}	superset
|sp{isubset}	isubset	|sp{isuperset}	isuperset
|end(table)
|section(Miscellaneous)
|begin(table)
;
cfB 1 lfB cfB 1 lfB
cw(.3i)1 lw(.85i) cw(.3i)1 lw(.85i).
Char	Name	Char	Name
|sp{fi}	fi	|sp{fl}	fl
|sp{ff}	ff	|sp{ffi}	ffi
|sp{ffl}	ffl
|sp{rule}	rule	|sp{box vertical rule}	box vertical rule
|sp{acute}	acute	|sp{grave}	grave
|sp{circle}	circle	|sp{square}	square
|sp{bold vertical}	bold vertical	|sp{footmark}	footmark
|sp{->}	->	|sp{<-}	<-
|sp{uarrow}	uarrow	|sp{darrow}	darrow
|sp{quarter}	quarter	|sp{half}	half
|sp{3quarter}	3quarter	|sp{cent}	cent
|sp{TEX}	TEX	|sp{LATEX}	LATEX
|end(table)
|comment<
|section(Carriage control)
|sp{down}	down
|sp{break and spread}	break and spread
|sp{reverse}	reverse
|sp{tab}	tab
|sp{up}	up
|sp{interrupt text processing}	interrupt text processing
|sp{conceal new line}	conceal new line>
|comment<inserting examples/special ending here>

|appendix(List Of Examples)

This appendix provides the input text for examples that appear in
this |i<Monk> user guide.
.Tm tbl	C
.Tm |i<pic>	C
.Tm References	s
They are not intended for a novice user,
but for those already familiar with these formatting tools.
If you want to learn how to use |i<tbl> or |i<pic>
do not look at these examples.
Please read the guides that are listed in the references.
These guides, which are readily available,
begin with simple examples.

|comment<|section(Table in section |remember(table_example))>
|section(Table in section 5.14)
|save(table_appendix)
.ta 1.4i 2.6i

|begin(figure)
|source(examples/example.table)
|end(figure)

.ta

|comment<|section(Picture in section |remember(picture_example))>
|section(Picture in section 5.16)

.ta .2i

|begin(figure)
|source(examples/example.pic1)
|end(figure)

.ta

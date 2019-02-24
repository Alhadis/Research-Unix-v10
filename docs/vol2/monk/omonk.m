|make(rp)

|insert(monk.mac)

|comment(VOL2HEADER)|set_counter(page 239-1)|set_string(page_headers "'''")|set_string(even_headers "'Typing Documents on the UNIX System: Using Monk 0.6''\f(NImonk\fP'")|set_string(odd_headers "'\f(NImonk\fP''Typing Documents on the UNIX System: Using Monk 0.6'")|set_string(even_footers "_\\nP__UNIX Papers_")|set_string(odd_footers "'Research Tenth Edition''\\nP'")

|environment(cwfigure;
	verbatim on, file.information, line.spacing 1,
	fill off, size -2, space -2, font cw, blank.lines -1;
	blank.lines -1, SPACE)
	|comment<summary: cwfigure (a constant-width figure)>

|environment(computeroutput;
	verbatim on, file.information, line.spacing 1,
	fill off, size -1, space -1, font cw, blank.lines -1,
	tab.stops "\w'        'u 2u*\w'        'u 3u*\w'        'u 4u*\w'        'u 5u*\w'        'u 6u*\w'        'u 7u*\w'        'u 8u*\w'        'u 9u*\w'        'u 10u*\w'        'u 11u*\w'        'u";

	tab.stops, blank.lines -1, SPACE)

|author(name "S. L. Murrel", initials SLM, location MH, department 11229,
	extension x5772, room 2C-578)

|author(name "T. J. Kowalski", initials TJK, location MH, department 11229,
	extension x2771, room 2C-568)

|document(number 11229-851210-11TMS, file_case 25952, work_program 311401-2299)

|document(number 11229-851210-11TMS, file_case 25952, work_program 311401-2199)

|date(December 10, 1985)

|title(Typing Documents on the |small(UNIX) System:
Using Monk 0.6)

|insert(monk.cite)

|begin(abstract)
|i<Monk> is a text-formatting program that produces high-quality
typeset output.
This paper, printed using |i<monk>,
describes how to produce letters, memos, release papers,
and technical memoranda complete with cover sheets.
We provide extensive details on the three easy steps
needed to prepare a document with |i<monk>:

|begin(number_list)

|item
Use the |i<monksample> command to get a copy of a sample document.

|item
Edit the copy, replacing the sample author, document information,
and the text body.
Add sections, appendices, captioned figures, tables, and equations.
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

|end(number_list)

|end(abstract)

|titlebox

|style(two_column)

|unnumbered_section(Introduction)

Preparing any document involves positioning words and figures on each page.
Using a typewriter,
the margins, line spacing, and tab positions are selected by moving levers.
Text is positioned by moving the sheet of paper left, right, up or down.
Using a computer, the text is interspersed with commands that tell a
formatting program what to do.
|p
|i<Monk> is a new package of commands for producing
|s(AT&T) letters, memos, technical memoranda, and their cover sheets.
This paper will enable you to prepare documents using
|i<monk>.|reference<monk high-level text compiler>|reference<monk database>
|reference<using monk 1985>
You need to know how to log onto a computer and how to use an editor.
If you don't, see the |small(UNIX) Text Editing and Phototypesetting
Starter Package.|reference(Editing Phototypesetting Starter Package)

|section(Mixing Text and Commands)

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
(e.g., double spacing and italics)
and include various sections and displays (e.g., chapters and tables).

|section(Command Scope)

|comment{|index(command delimiters)|index(italics)}

Using |i<monk>,
the sentence ``Italicize the words in |sp(|s)italics(parentheses)'' produces
``Italicize the words in |i(parentheses).''
All |i<monk> commands begin with the character `|sp(|s)'.
The command |cw(|sp(|)italics) selects the operation and
the parentheses delimit the words to be italicized.
There are seven pairs of delimiters:

|begin(center)
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

|section(Document Commands)

This section describes the commands that are specific to formatting
four types of documents:
letters, |small(AT&T) Bell Laboratories memoranda, technical memoranda,
and release papers.

|environment(ret_address;
	verbatim on, indent \n(U0u+\n(O0u-18n, fill off, here on;
	minimum.lines)

|environment(date;
	verbatim on, indent \n(U0u+\n(O0u-18n, fill off, here on;
	minimum.lines)

|environment(address;
	verbatim on, fill off, here on, minimum.lines;
	minimum.lines)

|environment(greeting;
	minimum.lines 2;
	minimum.lines)

|environment(body;
	minimum.lines, fill on;
	minimum.lines)

|environment(closing;
	minimum.lines 2, indent \n(U0u+\n(O0u-18n;
	minimum.lines)

|environment(signature;
	verbatim on, minimum.lines 2, indent \n(U0u+\n(O0u-18n;)

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
If the city, state, and zip code are typed on separate lines in the input file,
they will appear the same way in the final output:

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
|insert(samples/letter.nx)
|end(cwfigure)

|p
To copy this letter into a file named |cw<myfile> in your current directory,
type the |small(UNIX) command |cw(monksample letter > myfile).
Using this prototype, it is not necessary for you to remember that
the label for return address is |cw(ret_address).
You need only recognize the English-like descriptions and
replace the sample text as appropriate.
Although any text editor will suffice, the large display of a visual editor
allows you to glance at the file and spot sample text.
|p
It is convenient to tailor the samples by entering your
address, closing, and signature and then saving the modified file.
This new file can then be used for all your letters simply
by adding an appropriate greeting, the recipient's address, and the body.
If you prepare letters for more than one person, you might want to maintain
a directory of tailored prototypes rather than begin with the
standard |i<monk> sample.

|begin(cwfigure)
|insert(samples/tailor.nx)
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
Devices and fonts will vary with your location,
but commonly available types include:

|begin(figure)
|begin(table)
center;
c1FCW lfB
l1FCWp-2 l.
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

|p
If |s(TYPESETTER) is set and exported, then it determines the typesetter
or terminal fonts used in preparing the output.
Be careful to type |cw(monk |sp(minus)Tpost |sp(minus)Sapsend myfile)
or |cw(monk |sp(minus)Taps |sp(minus)Sprt myfile)
|b(only) when you intend to send
final copy to the high-quality, high-priced phototypesetter.

|comment{
|insert(samples/memo.titlebox)}

|p
When |i<monk> is run, the individualized prototype letter will appear as

|begin(figure)
|insert(samples/tailor)
|end(figure)

|comment{restore tm definition of date}

|environment(date; divert.string DT;)

|subsection(Typesetting a Memorandum)

Preparing a memorandum is similar to preparing a letter.
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
|insert(samples/memo.nx)
|end(cwfigure)

|p
The |cw<|sp(|)make> command asks |i<monk> to format the document as a memo.
The information about each author is given in a
separate |cw<|sp(|)author> command:
name, initials, location, department, extension, and room number.
Each instance of |cw<|sp(|)document> gives the document information for
the corresponding author.
Author, document, title, and date may appear in any order,
but should precede the command |cw<|sp(|)titlebox>,
which organizes, formats, and outputs all this information.

|insert(samples/reset_def)

|reset_author_doc

|author(name "S. L. Murrel", initials SLM,
   location MH, department 11229,
   extension x5772, room 2C-578)

|author(name "T. J. Kowalski", initials TJK,
   location MH, department 11229,
   extension x2771, room 2C-568)

|document(number 11229-851210-11TMS,
   file_case 25952, work_program 311401-2299)

|document(number 11229-851210-11TMS,
   file_case 25952, work_program 311401-2199)

|date(December 10, 1985)

|title(Typing Documents on the \s-1\&UNIX\s+1\& System:
Using Monk 0.3)

.nr yY \n(W1
.mk zZ
.sp |sp(|)7.7i

|titlebox

.nrW \n(yYu
.nrW1 \n(yYu
.ll\nWu
.rt \n(zZu
.pl -3.0i
.rn EF eF
.rn OF oF

|p
The titlebox follows the body of the memo,
which consists of paragraphs
separated by |cw<|sp(|)p>, simple font changes, and figures.
After the body comes the signature, any necessary notations,
and approvals.
|p
To omit information, simply delete the appropriate |i<monk> command.
Delete the entire line to omit notations or approvals.
To omit an author's location, omit the relevant labels and associated numbers,
e.g., |cw{|sp(|)author(name "W. H. MacWilliams", initials WHM, department 1344)}.

|subsection(Typesetting a Technical Memorandum)

This section describes how to prepare a technical memorandum.
The example below is the skeleton of the file that produced the previous |i(tm) version
of this document, excluding cover sheet information.
This prototype consists of the declaration of the document type,
document numbers,
date,
authors, title, abstract, document body, appendices, and references.
It can be copied with the command |cw(monksample tm > myfile).

|comment{entire tm here or simply body, ending before cover sheet info}

|begin(cwfigure)
|insert(samples/tmbody.nx)
|end(cwfigure)

.rn eF EF
.rn oF OF
.pl +3.0i

|p
The first line must specify the document type.
After that the order is flexible,
with the one restriction that all the
component information be available when larger units, such as the titlebox,
are printed.
The command |cw(|sp(|)titlebox) produces the standard header at the
top of technical memoranda.
It appears at the bottom of this page.
Thus the author, date, and document information must be specified first.
The title is not filled, so that newlines entered by the author are preserved:
here |cw<Using Monk 0.3> appears as
the second line of the title.|dagnote<In unfilled text,
input lines are reproduced unchanged in the output;
in filled text,
words from successive input lines may be combined to ensure ``filled''
(maximum length) lines in the output.>
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
|sp(|)author(name "T. J. Kowalski",
   initials TJK, location MH,
   department 11229)
|sp(|)document(number 11229-851210-11TMS,
   file_case 25952,
   work_program 311401-2299)
|sp(|)document(number 11229-851210-11TMS,
   file_case 25952,
   work_program 311401-2199)
|end(cwfigure)

is equivalent to the ordering given in the |i(tm) above.
Note that in these examples only part of the author information is included:
the room and extension numbers were omitted.
Any information can be omitted simply by deleting the corresponding |i<monk>
labels, e.g., |cw<room> and |cw<extension>.
Monk will always try to format the input information gracefully.
|p
To produce a cover sheet, additional information is needed:

|begin(cwfigure)
|insert(samples/tmcs.nx)
|end(cwfigure)

Again, this information can appear anywhere in the document as long as it
precedes the command |cw(|sp(|)cover_sheet).
Following local convention, keywords are filled, while the distribution lists
are not.
Although the title was not filled in the titlebox,
it is filled on the cover sheet because of space limitations.
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
then the command |cw<|sp(|)add_totals>(|cw<other> 1) suffices.
You should never have to use the complete form of the command:
|cw<|sp(|)add_totals(text 0, tables 0, figures 1, references 11,
other 2)>.
This command means that you are adding 2 additional pages by hand,
which include one figure and eleven references.
The counts of text pages and tables are untouched.
|p
There are six commands specific to the |s(AT&T B)ell |s(L)abs cover sheet
that provide distribution information.
Use |cw<|sp(|)mercury> to specify the company-wide announcements
that should include
the abstract of this document:
|cw<chm>, |cw<cmm>, |cw<cmp>, |cw<elc>, |cw<lfs>, |cw<man>, |cw<mas>, |cw<mkt>,
and |cw<phy>.
Use |cw<|sp(|)distribute_complete_memo> to
list individuals who should receive the complete memo and
|cw<|sp(|)distribute_cover_sheet>
to list those who should receive only the cover sheet.
The command |cw<|sp(|)att> gives |s(AT&T) release information:
|cw<is_release> marks the |i<yes> or |i<no> box
as specified by its argument.
|cw<|sp(|)director_name> identifies the director of the first author.
Finally, |cw<|sp(|)cover_sheet> prints the cover sheet.

|subsection(Typesetting a Release Paper)

There is a |i<monk> sample for release papers, which can be modified
in the same way as
the documents already discussed.
However, you may often need to convert a technical memorandum
to a release paper.
To do so, change the first line from
|cw<|sp(|)make(tm)> to |cw<|sp(|)make(rp)>
and then remove or comment out (see Section |remember(comment_section))
the information specific to the
|i<tm> style:

|begin(bullet_list)
|item |cw<other>,
|item |cw<add_totals>,
|item |cw<mercury>,
|item |cw<distribute_cover_sheet>,
|item |cw<distribute_complete_memo>,
|item |cw<cover_sheet>,
|item |cw<att>,
|item |cw<signature>,
|item |cw<director_name>
|end(bullet_list)

|section(Content Formatting Commands)

For any document type, it is possible to format section titles,
figure captions, footnotes, and two-column output.
It is possible to store page numbers, section numbers or figure numbers
for later use.
An interface is provided to existing routines that produce
tabular output, equations, citations, and reference lists.
This section describes the relevant commands.

|subsection(Comments)
|save(comment_section)

Comments are valuable notations by and for the person preparing a document.
They can be used
to temporarily omit a section,
to mark a section that needs work or information,
and to retain an alternative word or phrase.
Anything that appears within the delimiters of a |cw<|sp(|)comment>
is not processed as part of the document:
|i<monk> commands are ignored, and text does not appear in the final output.

|subsection(Spacing)

|comment{
|subsection(Spacing and Tabs)
You can start a new page, leave vertical blank space, and control tab stops.}

You can start a new page or leave vertical blank space.

|begin(table)
center;
lf5 2 l.
|sp(|s)new_page	begins a new page
|sp(|s)blank_space	leaves a blank line
|sp(|s)blank_space(3)	leaves 3 blank lines
|sp(|s)blank_space(1.5i)	leaves 1.5 inches
|sp(|s)blank_space(|sp(minus)1)	moves back one line
|comment{
|sp(|s)tab_stops(1.5i 4.25i 5.5i 6.75i)	sets tab stops
|sp(|s)tab_stops	restores previous tab stops}
|end(table)

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
I go to the lavatry,
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
I go to the lavatry,
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
I go to the lavatry,
On Wednesdays I go shopping and have
buttered scones for tea.
|end(center_block)
|end(figure)

|p
Multi-line quotations are appropriately indented and offset by the
command |cw<|sp(|)quotation>.

|quotation<Four score and seven years ago our fathers brought forth
to this continent a new nation, conceived in liberty and dedicated to
the proposition that all men are created equal.>

|subsection(Fonts)

|i<Monk> currently provides abbreviated commands for
four type faces and two type sizes.
|cw(|sp(|)bold) or |cw(|sp(|)b) makes the enclosed text |b(bold).
|cw(|sp(|)italics) or |cw(|sp(|)i) produces |italics(italicized text),
while |cw(|sp(|)roman) or |cw(|sp(|)r) restores the roman character set.
The command |cw<|sp(|)constant_width> or |cw<|sp(|)cw>
|cw(selects a font that resembles low quality line printer
output and is intended only for examples of such output).
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
However, |cw<|sp(|)big> and |cw<|sp(|)small> are designed for in-line font changes;
they do not change the vertical spacing.
To change the size of a block of text, use the |cw<|sp(|)style> command
(see Section |remember(style_section)).

|subsection(Special characters)
|save(special_section)

|i<Monk> provides a long list of special characters, the appearance of which
is ultimately determined by the output device.

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
|p
|cw<|sp(|)sp> can also be used to control spacing and carriage movement
precisely.
There are spaces of five different widths,
shown below opposite the commands that produce them.

|begin(verbatim)
|cw<|sp(|)sp{space}> `|sp{space}',
|cw<|sp(|)sp{digit space}> `|sp{digit space}',
|cw<|sp(|)sp{narrow space}> `|sp{narrow space}',
|cw<|sp(|)sp{half narrow space}> `|sp{half narrow space}'
|cw<|sp(|)sp{zero width space}> `|sp{zero width space}'
|end(verbatim)

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
To force hyphenation at a particular position in a word, use
|cw<|sp(|)sp{hyphenation}>.
|cw<|sp(|)sp{tab}> generates horizontal motion to the next tab stop;
|cw<|sp(|)sp{leader}> generates a string of periods to the next tab stop.

|subsection(Headings)

|comment<|subsection(Hyphenation and Justification)>
|comment<|subsection(Section headings)>

|subsubsection(Numbered headings.)

|cw<|sp(|)chapter> starts a new page; leaves a blank space;
puts ``Chapter |i<n>'' against the right hand margin;
skips more space;
puts the title against the right hand margin;
and skips more space before beginning the text body.
No chapters appear in this document.
The commands
to declare the current section, subsection, and subsubsection include:

|begin(cwfigure)
|sp(|)section(Content Formatting Commands)
|sp(|)subsection(Headings)
|sp(|)subsubsection(Numbered headings.)
|end(cwfigure)

The first appendix at the end was produced by
|cw<|sp(|)appendix(List of Commands)>.
|paragraph(numbered)
This numbered paragraph is produced by the command
|cw<|sp(|)paragraph(numbered)>,
which may be abbreviated as |cw<|sp(|)paragraph(n)> or |cw<|sp(|)p(n)>.

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

|subsubsection(Paragraphs)

The command |cw(|sp(|)paragraph) or its abbreviation |cw(|sp(|)p)
is used throughout this paper to produce the default paragraph style.
In this paper the normal default,
|cw(flushed), was changed through use of the |cw<|sp(|)style> command to |cw(indented)
(see Section |remember(style_section)).

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

|subsection(Setting page headers and footers)

Both page headers and footers can be set using the |cw<|sp(|)set_string> command
as follows:

|begin(cwfigure)
|sp(|)set_string(page_headers
    "'left'center'right'")
|sp(|)set_string(even_headers
    "'left'center'right'")
|sp(|)set_string(odd_headers
    "'left'center'right'")
|sp(|)set_string(page_footers
    "'left'center'right'")
|sp(|)set_string(even_footers
    "'left'center'right'")
|sp(|)set_string(odd_footers
    "'left'center'right'")
|end(cwfigure)

|cw<page_headers>, |cw<even_headers>, and |cw<odd_headers> set the headers
on all pages, even pages, and odd pages, respectively.
|index(headers)|index(page headers)
Page headers appear above the even and odd headers.
All headers consist of three strings, any of which may be blank or long
enough to overlap.
Any character, can be used to delimit the strings.
The strings |i(left), |i(center), and |i(right) are left-justified, centered,
and right-justified, respectively.
Parallel commands serve
to specify page footers.|index(footers)|index(page footers)

|comment<|subsection(Grouping text - Displays)>

|subsection(Inserting other files)

Other files may be inserted using the command |cw<|sp(|)insert>.|index(insert)
These files will also be processed by |i<monk> and
may in turn insert other files.
Loops will indeed cause disaster.
A file that will not be processed may also be included
using the |cw<|sp(|)source> command.|index(source)

|subsection(Figures)

The lumberjack poem above was formatted as a figure.
Figures are printed with blank space above and below and can be split
across a page boundary.
Lines are not filled.
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
|p
The |cw<|sp(|)here> command keeps all the text within its delimiters
together on one page.
It is important to include both the figure proper and its caption within
the delimiters so that the caption is never orphaned.
If there is not enough room on the current page, |cw<|sp(|)here>
leaves the rest of the
page blank and skips to the next page or next column to print the block.
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
is explained on page 2 of the Kernighan and Cherry memo.|footnote{
Remember that the |i<monk> command |cw<|sp(|)equation(...)> replaces all
instances of |cw<.EQ> and |cw<.EN> and that |cw<|sp(|)e(...)> replaces the
|cw<$...$> delimiters for in-line equations.}
To write equations in-line with text, delimit the equation using |cw<|sp(|)e>.
The input |cw{|sp(|)e<x = 2 pi int sin ( omega t ) dt>}
produces the equation |e<x = 2 pi int sin ( omega t ) dt>
without offsetting it.

|subsection(Tables)
|save(table_example)

|i<Monk> does not provide any new capabilities for formatting tables;
it uses |i(tbl)|reference(latest tbl).
It provides |i<monk>-style commands for positioning both the table and its caption
and automatically invokes the standard |i(tbl) formatting program.
The following input formats the table in the file |i(examples/example.table),
which appears as Appendix |remember(table_appendix).|footnote{
The formatting commands |cw<.TS> and |cw<.TE>
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
Together |i<monk> and |i<tbl> produce

|begin(here)
|table_caption(Example of table)
|begin(table)
|insert(examples/example.table)
|end(table)
|end(here)

|p
If you have a 5620 or a 630MTG terminal,
the interactive table editor |i<vtbl>
can be used to prepare |i<tbl> input.|reference(Murrel bitmaps build tables)

|comment[
|subsection(Computer Programs and Output)
Two special types of figures are provided for computer-related text.
The command |cw<|sp(|)program> beautifies the source code of C,
a high-level programming language.
It will try to beautify anything appearing within its delimiters following a simple
set of rules that identify program segments.
|begin(program)
main()
{
	printf("Hello World\n");
}
|end(program)
Used inappropriately, it will produce good looking garbage.
|cw<|sp(|)computeroutput_caption (Example of computer output)> and
|cw<|sp(|)program_caption (Example of program)>
provide the associated captions.]

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
   .650655     .423352    .605140
|end(computeroutput)
|blank_space(-1)
|computeroutput_caption(Example of computer output)

|cw<|sp(|)computeroutput_caption (Example of computer output)>
provides the associated caption.
Currently these captions are the same as figure captions.

|subsection(Pictures)
|save(picture_example)

|i<Monk> commands also provide access to |i<pic>, a graphics language for
typesetting.|reference[latest pic]
Borrowing an example from the |i<PIC User Manual>,|footnote{
Remember that the |i<monk> command |cw|sp(|)picture(...)> replaces all
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

|begin(picture)
|insert(examples/example.pic1)
|end(picture)
|picture_caption(Example of picture)

|p
If you have a 5620 or a 630MTG terminal,
the interactive drawing program |i<cip>
can be used to prepare |i<pic> input.|reference(latest cip)
|p
Another example from Kernighan's |i<PIC Revised User Manual> illustrates
the use of an in-line equation embedded in a picture.|footnote{
In this example, the equation within the delimiters must be placed within the
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

|comment{
|subsection(Strings)
|cw<|sp(|)savestring> saves strings for you to use later.
|savestring(os "The UNIX operating system")
After the command |cw<|sp(|)savestring(os "The UNIX operating system")>,
|cw<|sp(s)remember(os)> is replaced by |remember(os).
They are especially useful when you want to repeatedly use some long name or
phrase.
}

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
The command |cw<|sp(|)remember(this_section)> is replaced by
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
|p
If all citations are saved before they are used, then the user need
only enter the commands shown above in the |i<monk> input text.
If, however, the input uses a citation before it really is saved,
as in the example below, then there is more to do.

|begin(cwfigure)
All commands are listed in appendix
|sp(|)remember(appendix_commands).
|sp(zero width space)|b(...)
|sp(|)appendix(|s(LIST OF COMMANDS))
|sp(|)save(appendix_commands)
|end(cwfigure)

Forward referencing depends on a file named |cw<.cite>,
which is generated each time the input is formatted.
The |small<UNIX> command

|begin(figure)
|cw(cite |sp(minus)s .cite > myfile.cite)
|end(figure)

reports any redefinitions or unused definitions and produces a consistent
set of citations.
The output file |cw<myfile.cite> needs to be inserted in the |i<monk> input
before the first citation.
Do so using the command |cw<|sp(|)insert(myfile.cite)>.

|comment[
|subsection(Creating an Index)
You can specify keywords to be inserted in an index using |cw(|sp(|)index):
|cw<|sp(|)index(these words)>.
The keywords |i<these words> will appear in the index with the current page
number.]
........

|subsection(Document Styles)|save(style_section)

By default,
all documents are single-spaced and single column.
Page width is 6 inches; length is 11 inches.
The font family is |i{times} and the font size 10.
Paragraphs skip a line and begin the text flush with the left margin.
|p
You can change any of these defaults using the |cw(|sp(|)style) command.
The command |cw<|sp(|)style(page_width 7i)> sets the page width to 7 inches.
To get the maximum page width on most output devices, type:
|cw<|sp(|)style(page_offset 0, page_width 8.5i)>.
The |cw(page_length) and |cw(font_size) can be set in a similar manner.
|p
|cw<|sp(|)style(font_size 14)> changes the default font to size 14 to print
out your papers for your grandparents or to prepare invitations to a party.
|cw(|sp(|)big) or |cw(|sp(|)small)
will still provide one size bigger or smaller, respectively.
|p
Font families provide complementary styles for each of the four fonts:
bold, italics, roman, and cw.
The standard font family is times.
Alternative font families include
palatino, helvetica, bembo, optima, souvenir, and cw,
but not all families are available on all output devices.
Some examples are shown in Table |remember(The alphabet in common type families).
To select helvetica, type |cw<|sp(|)style( font_family helvetica)>.

|begin(around)
\!.sp -1
|table_caption(The alphabet in common type families)
|save(The alphabet in common type families)
|begin(verbatim)
.ta .7i 1.3i
|comment{|b<Family	Face	Sample>}
|r(times	roman	abcdefghijklm...uvwxyz)
|i(	italics	abcdefghijklm...uvwxyz)
|b(	bold	abcdefghijklm...uvwxyz)
|style(font_family palatino)
|r(palatino	roman	abcdefghijklm...uvwxyz)
|i(	italics	abcdefghijklm...uvwxyz)
|b(	bold	abcdefghijklm...uvwxyz)
|style(font_family helvetica)
|r(helvetica	roman	abcdefghijklm...uvwxyz)
|i(	italics	abcdefghijklm...uvwxyz)
|b(	bold	abcdefghijklm...uvwxyz)
|end(verbatim)

|style(font_family times)
|end(around)

.ta
|p

|style(double_space)

|cw<|sp(|)style(double_space)> double spaces the text, while
|cw<|sp(|)style( single_space)> restores single spacing.

|style(single_space)

You can change the paragraph style throughout the rest of your document
by typing |cw<|sp(|)style(paragraph_style indented)> or
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
|p
Multi-level lists of up to six levels can be built by putting lists inside
lists.
A standard outline format of six levels consists of an outer |cw<|sp(|)I_list>,
an |cw<|sp(|)A_list>, a |cw<|sp(|)number_list>, an |cw<|sp(|)a_list>,
an inner |cw<|sp(|)number_list>, and a |cw<|sp(|)p_a_list>.
The following produces, in outline format, a description of two kingdoms.

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

|subsection(Footnotes)

A numbered footnote is generated by the command |cw[|sp(|)footnote] appended to
the running text.|footnote(This is the first numbered footnote on this page)
The text |cw<|sp(|)footnote(This is the second numbered footnote on this page)>
produces
numbered footnotes.|footnote(This is the second numbered footnote on this page)
Analogously, |cw<|sp(|)dagnote> and |cw<|sp(|)starnote> produce
daggered footnotes|dagnote(This is a daggered footnote)
and starred footnotes.|starnote(This is a starred footnote)
Footnote types can be mixed, as in this section.

|subsection(References)

|i<Monk> uses |i<prefer> to extract references from a
bibliographic database.|reference<Derr Prefer>
To use |i<prefer> capabilities with |i<monk>,
there are only two additional things you need to know.
First, the |cw<|sp(|)reference> command passes a partial reference to
|i<prefer>, as in:
|cw<|sp(|)|sp(zero width space)reference( Derr Prefer)>.
When this command is used, |i<monk> will automatically invoke |i<prefer>.
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
|subsection(Table of contents)
To run off a table of contents with your document,
use the |cw(|sp(minus)ZZZ) option.
To run off only a table of contents, ZZZ.
By default, this will produce ZZZ.
|p
You have the following options: ZZZ.}

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
but the results might not work smoothly with the |i<monk> program.

|subsection(Other related programs)

There are two other programs useful to |i(monk) users:
|i(demonk) is the moral equivalent of |i(deroff), and |i(monkmerge)
is the functional equivalent of |i(soelim).
These tools are helpful when processing |i(monk) files by other
programs like |i(spell).

|reference_placement

|new_page
.ad l
|appendix(List of Commands)

|section(Global Commands)

|insert(commands/global)

|section(Letters)

|insert(commands/letter)

|section(Memoranda)

|insert(commands/memo)

|section(Merit Review Form 1)

|insert(commands/form1)

|section(Bi-Annual Research Reports)

|insert(commands/research)

|section(Center Phone Books)

|insert(commands/centerpb)

|section(Technical Memoranda)

|insert(commands/tm)

|section(Internal Memoranda)

|insert(commands/im)

|section(Technical Correspondence)

|insert(commands/tc)

|section(Release Papers)

|insert(commands/rp)

|section(Association for Computing Machinery Proceedings)

|insert(commands/acm)

|section(Conference Proceedings Model Sheets)

|insert(commands/model)

|section(Computer Science Press Books)

|insert(commands/cspress)

|section(Kluwer Academic Publishers Books)

|insert(commands/kluwer)

|section(Nursing Home Song Sheets)

|insert(commands/song)

.ad b

|appendix(List of Special Characters)
|save(special_appendix)

The special characters that control spacing and carriage movements are
described in section |remember(special_section).
All other special characters are listed below.
For each character,
column |cw<Char> contains the final printed character
and column |cw<Name> contains the keywords
given to |i<monk> to specify the character.
To print the character |sp(rhand), type |cw<|sp(|)sp(rhand)>
in your input file.

|insert(examples/special)

|appendix(List Of Examples)

This appendix provides the input text for examples that appear in
this |i<Monk> user guide.
They are not intended for a novice user,
but for those already familiar with these formatting tools.
If you want to learn how to use |i<tbl> or |i<pic>
do not look at these examples.
Please read the guides that are listed in the references.
These guides, which are readily available,
begin with simple examples.

|comment<|section(Table in section |remember(table_example))>
|section(Table in section 4.12)
|save(table_appendix)

.ta 1.4i 2.6i

|begin(figure)
|source(examples/example.table)
|end(figure)

.ta

|comment<|section(Picture in section |remember(picture_example))>
|section(Picture in section 4.14)

.ta .2i

|begin(figure)
|source(examples/example.pic1)
|end(figure)

.ta

|keywords(Word Processing,
Text Processing,
Formatting)

|comment{
|mercury(cmp)
|att(is_release yes)
|director_name(J. L. Flanagan)
|distribute_complete_memo(Executive Directors 112
Directors 112
Department Heads 1122
Department Heads 1127
Directors 113
Department Heads 1138)
|distribute_cover_sheet(A. A. Penzias
1122 MTS)
|cover_sheet}

Each bibliographic reference may be classified as a particular type,
e.g. book, article, thesis.
Each type, in turn, is defined by a set of attributes.
Below is a list of all such attributes.
For any one type, some of the attributes are required, some
optional, some undefined, and some ignored
(e.g. the attribute is there for some purpose
other than formatting such as indexing or recordkeeping).
Next to each attribute name is a brief description of its function.

|begin(small)
|begin(table)
center;
lFCW lw(1.75i).
%type	document type
%author	T{
name of author (or editor in case of edited book)
T}
%title	reference title
%publisher	T{
publisher, organization, institution, or school
T}
%address	address of publisher
%booktitle	T{
title of book, part of which is being cited
T}
%editor	T{
editor of book, journal, proceedings or author of reviewed article
T}
%date	publication date
%report	technical report number
%tm	tm number
%journal	T{
name of journal, magazine, or proceedings
T}
%volume	book or journal volume number
%number	journal number
%part	journal part number
%pages	pages on which reference appears
%also	refers by search keys to another entry
%also_begin	begin another entry
%also_end	end another entry
%other	T{
any other information to be included with the reference
T}
%keywords	T{
keywords used for searching (an ignored attribute)
T}
%X	T{
location of the reference (an ignored attribute)
T}
%Y	read status (an ignored attribute)
%Z	comments (an ignored attribute)
|end(table)
|end(small)

What follows is a list of currently defined bibliography types,
along with their
required and optional attributes.
The only attributes that can appear more than once in a single reference
are |cw(%author) and |cw(%editor).
The |cw(%type) attribute is required by all types.
The attributes |cw(%also), |cw(%also_begin), |cw(%also_end), and |cw(%other) are optional
for all types.
Finally, |cw(%X), |cw(%Y), |cw(%Z), and |cw(%keywords) are optional ignored attributes.
|begin(small)
|begin(table)
center, tab(:);
l lw(1.75i).
.hy 0
_
|cw(article):journal or magazine article
required:T{
|cw(%author) |cw(%title) |cw(%journal) |cw(%date)
T}
optional:T{
|cw(%volume) |cw(%number) |cw(%part) |cw(%pages)
T}
_
|cw(book):a published book
required:T{
|cw(%author) |cw(%title) |cw(%publisher) |cw(%date)
T}
optional:|cw(%address) |cw(%volume)
_
|cw(editedbook):an edited book	
required:T{
|cw(%author) |cw(%title) |cw(%publisher) |cw(%date)
T}
optional:|cw(%address) |cw(%volume)
_
|cw(incollection):T{
article or chapter in a collection
T}
required:T{
|cw(%author) |cw(%title) |cw(%booktitle) |cw(%publisher) |cw(%date)
T}
optional:T{
|cw(%address) |cw(%editor) |cw(%volume) |cw(%pages)
T}
_
|cw(inproceedings):paper in a proceedings
required:T{
|cw(%author) |cw(%title) |cw(%journal) |cw(%date)
T}
optional:T{
|cw(%volume) |cw(%number) |cw(%part) |cw(%publisher) |cw(%address) |cw(%editor) |cw(%pages)
T}
_
|cw(manual):a manual or guide
required:|cw(%title) |cw(%date)
optional:T{
|cw(%author) |cw(%volume) |cw(%editor) |cw(%publisher) |cw(%address)
T}
.ne 3
_
|cw(mastersthesis):Master's Thesis
required:T{
|cw(%author) |cw(%title) |cw(%publisher) |cw(%date)
T}
optional:|cw(%address)
_
|cw(misc):miscellaneous type
optional:T{
|cw(%author) |cw(%title) |cw(%volume) |cw(%publisher) |cw(%address) |cw(%pages) |cw(%date)
T}
_
|cw(pamphlet):a pamphlet or booklet
required:|cw(%title)
optional:T{
|cw(%author) |cw(%date) |cw(%publisher) |cw(%address)
T}
_
|cw(phdthesis):Ph.D. Thesis
required:T{
|cw(%author) |cw(%title) |cw(%publisher) |cw(%date)
T}
optional:|cw(%address)
_
|cw(proceedings):a conference proceedings
required:|cw(%title) |cw(%publisher) |cw(%date)
optional:T{
|cw(%author) |cw(%volume) |cw(%number) |cw(%part) |cw(%address) |cw(%editor)
T}
_
|cw(review):a review of another piece
required:|cw(%author) |cw(%journal) |cw(%date)
optional:T{
|cw(%title) |cw(%booktitle) |cw(%editor) |cw(%publisher) |cw(%volume) |cw(%number) |cw(%part) |cw(%pages)
T}
_
|cw(techreport):technical report
required:T{
|cw(%author) |cw(%title) |cw(%publisher) |cw(%date)
T}
optional:|cw(%report)
_
|cw(tm):T{
AT&T Bell Labs technical memorandum
T}
required:T{
|cw(%author) |cw(%title) |cw(%tm) |cw(%date)
T}
_
|cw(unpublished):unpublished manuscript
required:|cw(%author) |cw(%title)
optional:|cw(%date)
_
.hy
|end(table)
|end(small)

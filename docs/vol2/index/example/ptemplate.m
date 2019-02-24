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
lFCW lw(2i).
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
are %author and %editor.
The %type attribute is required by all types.
The attributes %also, %also_begin, %also_end, and %other are optional
for all types.
Finally, %X, %Y, %Z, and %keywords are optional ignored attributes.

|new_page
|begin(small)
|begin(here)
|begin(table)
l l.
article	journal or magazine article
required:	optional:
  %author	  %volume
  %title	  %number
  %journal	  %part
  %date	  %pages
|end(table)
|end(here)
|begin(here)
|begin(table)
l l.
book	a published book
	required:	optional:
	  %author	  %address
	  %title	  %volume
	  %publisher
	  %date
|end(table)
|end(here)
|begin(here)
|begin(table)
l l.
editedbook	an edited book	
required:	optional:
  %author	  %address
  %title	  %volume
  %publisher
  %date
|end(table)
|end(here)
|begin(here)
|begin(table)
l l.
incollection	article or chapter in a collection
required:	optional:
  %author	  %address
  %title	  %editor
  %booktitle	  %volume
  %publisher	  %pages
  %date
|end(table)
|end(here)
|begin(here)
|begin(table)
l l.
inproceedings	paper in a proceedings
required:	optional:
  %author	  %volume
  %title	  %number
  %journal	  %part
  %date	  %publisher
	  %address
	  %editor
	  %pages
|end(table)
|end(here)
|begin(here)
|begin(table)
l l.
manual	a manual or guide
required:	optional:
  %title	  %author
  %date	  %volume
	  %editor
	  %publisher
	  %address
|end(table)
|end(here)
|begin(here)
|begin(table)
l l.
mastersthesis	Master's Thesis
required:	optional:
  %author	  %address
  %title
  %publisher
  %date
|end(table)
|end(here)
|begin(here)
|begin(table)
l l.
misc	miscellaneous type
	optional:
	  %author
	  %title
	  %volume
	  %publisher
	  %address
	  %pages
	  %date
|end(table)
|end(here)
|begin(here)
|begin(table)
l l.
pamphlet	a pamphlet or booklet
required:	optional:
  %title	  %author
	  %date 
	  %publisher
	  %address
|end(table)
|end(here)
|begin(here)
|begin(table)
l l.
phdthesis	Ph.D. Thesis
required:	optional:
  %author	  %address
  %title
  %publisher
  %date
|end(table)
|end(here)
|begin(here)
|begin(table)
l l.
proceedings	an conference proceedings
required:	optional:
  %title	  %author
  %publisher	  %volume
  %date	  %number
	  %part
	  %address
	  %editor
|end(table)
|end(here)
|begin(here)
|begin(table)
l l.
review	a review of another piece
required:	optional:
  %author	  %title
  %journal	  %booktitle
  %date	  %editor
	  %publisher
	  %volume
	  %number
	  %part
	  %pages
|end(table)
|end(here)
|begin(here)
|begin(table)
l l.
techreport	technical report
required:	optional:
  %author	  %report
  %title
  %publisher
  %date
|end(table)
|end(here)
|begin(here)
|begin(table)
l l.
tm	AT&T Bell Labs technical memorandum
required:
  %author
  %title
  %tm
  %date
|end(table)
|end(here)
|begin(here)
|begin(table)
l l.
unpublished	unpublished manuscript
required:	optional:
  %author	  %date
  %title

|end(table)
|end(here)
|end(small)
|style(one_column)

|environment(chapter;
	new.page, spacing on, blank.lines 2i,
	incr chapter_counter,
	clear section_counter subsection_counter subsubsection_counter
	paragraph_counter,
	set string temporary_string "Chapter ",
	add string.from.number temporary_string chapter_counter,
	inline off, size +10, font.family helvetica,
	tab.stops \nWuR,
	tab, string temporary_string,
	divert.string temporary_string;

	sectioning bychapter,
	inline off, size +4, font.family helvetica, blank.lines .25i,
	tab.stops \nWuR,
	font bold, tab, string temporary_string, new.line,
	font roman, font.family times, size -4, inline on,
	minimum.lines 1.25i, paragraph,
	store string.from.number remember_string chapter_counter,
	contents section 1 remember_string temporary_string)
	|comment<summary: chapter (numbered chapter; IT)>

|environment(section;
	divert.string temporary_string;

	minimum.lines 2, protect, incr section_counter,
	clear subsection_counter subsubsection_counter paragraph_counter,
	font bold,
	|ifvalue sectioning bychapter [
		number chapter_counter, text ".",
	]
	|ifvalue sectioning byappendix [
		number appendix_counter, text ".",
	]
	number section_counter,
	|ifvalue sectioning bysection [
		text ".",
	]
	text " ", string temporary_string, font roman,
	paragraph,
	clear string remember_string,
	|ifvalue sectioning bychapter [
		store string.from.number remember_string chapter_counter,
		add string remember_string ".",
	]
	|ifvalue sectioning byappendix [
		store string.from.number remember_string appendix_counter,
		add string remember_string ".",
	]
	add string.from.number remember_string section_counter,
	|ifvalue sectioning bysection [
		contents section 1 remember_string temporary_string,
	]
	|ifvalue sectioning bychapter [
		contents section 2 remember_string temporary_string,
	]
	|ifvalue sectioning byappendix [
		contents section 2 remember_string temporary_string,
	])
	|comment<summary: section (numbered section; IT)>

|environment(subsection;
	divert.string temporary_string;

	minimum.lines, protect, incr subsection_counter,
	clear subsubsection_counter paragraph_counter,
	font bold,
	|ifvalue sectioning bychapter [
		number chapter_counter, text ".",
	]
	|ifvalue sectioning byappendix [
		number appendix_counter, text ".",
	]
	number section_counter, text ".",
	number subsection_counter, text " ", string temporary_string, font roman,
	paragraph,
	clear string remember_string,
	|ifvalue sectioning bychapter [
		store string.from.number remember_string chapter_counter,
		add string remember_string ".",
	]
	|ifvalue sectioning byappendix [
		store string.from.number remember_string appendix_counter,
		add string remember_string ".",
	]
	add string.from.number remember_string section_counter,
	add string remember_string ".",
	add string.from.number remember_string subsection_counter,
	|ifvalue sectioning bysection [
		contents section 2 remember_string temporary_string,
	]
	|ifvalue sectioning bychapter [
		contents section 3 remember_string temporary_string,
	]
	|ifvalue sectioning byappendix [
		contents section 3 remember_string temporary_string,
	])
	|comment<summary: subsection (numbered subsection; IT)>

|environment(subsubsection;
	divert.string temporary_string;

	minimum.lines, protect, incr subsubsection_counter,
	clear paragraph_counter,
	font italics,
	|ifvalue sectioning bychapter [
		number chapter_counter, text ".",
	]
	|ifvalue sectioning byappendix [
		number appendix_counter, text ".",
	]
	number section_counter, text ".",
	number subsection_counter, text ".",
	number subsubsection_counter, text " ", string temporary_string, font roman,
	clear string remember_string,
	|ifvalue sectioning bychapter [
		store string.from.number remember_string chapter_counter,
		add string remember_string ".",
	]
	|ifvalue sectioning byappendix [
		store string.from.number remember_string appendix_counter,
		add string remember_string ".",
	]
	add string.from.number remember_string section_counter,
	add string remember_string ".",
	add string.from.number remember_string subsection_counter,
	add string remember_string ".",
	add string.from.number remember_string subsubsection_counter,
	|ifvalue sectioning bysection [
		contents section 3 remember_string temporary_string,
	]
	|ifvalue sectioning bychapter [
		contents section 4 remember_string temporary_string,
	]
	|ifvalue sectioning byappendix [
		contents section 4 remember_string temporary_string,
	])
	|comment<summary: subsubsection (numbered subsubsection; IT)>

|associate(paragraph;
	minimum.lines;

	|ifvalue localparagraph.style indented [
		indent.line +3,
	]
	|ifvalue localparagraph.style numbered [
		incr paragraph_counter, number format paragraph_counter i,
		store string.from.number temporary_string paragraph_counter,
		add string temporary_string ") ",
		string italics temporary_string,
	]
	|ifvalue globalparagraph.style indented [
		localparagraph.style indented,
	]
	|ifvalue globalparagraph.style numbered [
		localparagraph.style numbered,
	]
	|ifvalue globalparagraph.style flushed [
		localparagraph.style flushed,
	];

	[numbered;;
		localparagraph.style numbered]
	[n;;
		localparagraph.style numbered]
	[indented;;
		localparagraph.style indented]
	[i;;
		localparagraph.style indented]
	[flushed;;
		localparagraph.style flushed]
	[f;;
		localparagraph.style flushed])
	|comment<summary: paragraph (n, i, f, numbered, indented, flushed)>

|associate(p;
	minimum.lines;

	|ifvalue localparagraph.style indented [
		indent.line +3,
	]
	|ifvalue localparagraph.style numbered [
		incr paragraph_counter, number format paragraph_counter i,
		store string.from.number temporary_string paragraph_counter,
		add string temporary_string ") ",
		string italics temporary_string,
	]
	|ifvalue globalparagraph.style indented [
		localparagraph.style indented,
	]
	|ifvalue globalparagraph.style numbered [
		localparagraph.style numbered,
	]
	|ifvalue globalparagraph.style flushed [
		localparagraph.style flushed,
	];

	[numbered;;
		localparagraph.style numbered]
	[n;;
		localparagraph.style numbered]
	[indented;;
		localparagraph.style indented]
	[i;;
		localparagraph.style indented]
	[flushed;;
		localparagraph.style flushed]
	[f;;
		localparagraph.style flushed])
	|comment<summary: p (alias for paragraph)>

|environment(appendix;
	new.page, spacing on, blank.lines 4,
	number format appendix_counter A, incr appendix_counter,
	clear section_counter subsection_counter subsubsection_counter
	paragraph_counter,
	set string remember_string "Appendix ",
	add string.from.number remember_string appendix_counter,
	divert.string temporary_string;

	sectioning byappendix,
	center on, size +1, font bold,
	string remember_string, blank.lines, string temporary_string,
	font roman, size -1, center off,
	minimum.lines 4, paragraph,
	store string.from.number remember_string appendix_counter,
	contents section 1 remember_string temporary_string)
	|comment<summary: appendix (numbered appendix; IT)>

|environment(unnumbered_chapter;
	new.page, spacing on, blank.lines 2i,
	divert.string temporary_string;

	sectioning bychapter,
	inline off, size +4, font.family helvetica, blank.lines .25i,
	tab.stops \nWuR,
	font bold, tab, string temporary_string, new.line,
	font roman, font.family times, size -4, inline on,
	minimum.lines 1.25i, paragraph,
	clear string remember_string,
	contents section 1 remember_string temporary_string)
	|comment<summary: unnumbered_chapter (unnumbered chapter)>

|environment(unnumbered_section;
	divert.string temporary_string;

	minimum.lines 2, protect, font bold,
	string temporary_string, font roman,
	paragraph,
	clear string remember_string,
	|ifvalue sectioning bysection [
		contents section 1 remember_string temporary_string,
	]
	|ifvalue sectioning bychapter [
		contents section 2 remember_string temporary_string,
	]
	|ifvalue sectioning byappendix [
		contents section 2 remember_string temporary_string,
	])
	|comment<summary: unnumbered_section (unnumbered section)>

|environment(unnumbered_subsection;
	divert.string temporary_string;

	minimum.lines, protect, font bold,
	string temporary_string, font roman,
	paragraph,
	clear string remember_string,
	|ifvalue sectioning bysection [
		contents section 2 remember_string temporary_string,
	]
	|ifvalue sectioning bychapter [
		contents section 3 remember_string temporary_string,
	]
	|ifvalue sectioning byappendix [
		contents section 3 remember_string temporary_string,
	])
	|comment<summary: unnumbered_subsection (unnumbered subsection)>

|environment(unnumbered_subsubsection;
	divert.string temporary_string;

	minimum.lines, protect, font italics,
	string temporary_string, font roman,
	clear string remember_string,
	|ifvalue sectioning bysection [
		contents section 3 remember_string temporary_string,
	]
	|ifvalue sectioning bychapter [
		contents section 4 remember_string temporary_string,
	]
	|ifvalue sectioning byappendix [
		contents section 4 remember_string temporary_string,
	])
	|comment<summary: unnumbered_subsubsection (unnumbered subsubsection)>

|environment(unnumbered_appendix;
	divert.string temporary_string;

	new.page, spacing on, blank.lines 4,
	sectioning byappendix,
	center on, size +1, font bold,
	string temporary_string,
	font roman, size -1, center off,
	minimum.lines 4, paragraph,
	store string.from.number remember_string appendix_counter,
	contents section 1 remember_string temporary_string)
	|comment<summary: unnumbered_appendix (unnumbered appendix)>

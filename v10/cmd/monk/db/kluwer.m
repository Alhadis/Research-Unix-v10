|include(global.h)

|include(macro.h)

|environment(tc_heading;
	clear string even_header odd_header page_header,
	new.page odd, tab.stops \nWuR,
	divert.string temporary_string;

	inline off, size +4, font.family helvetica, center on,
	font bold, string temporary_string, font roman,
	center off, font.family times, size -4, inline on,
	minimum.lines,
	clear string remember_string,
	contents section 1 remember_string temporary_string,
	set string even_header "\(ts\s9\\n'page_counter'\(ts\(ts\f2",
	add string.from.string even_header temporary_string,
	add string even_header "\fP\s0\(ts",
	set string odd_header "\(ts\s9\f2",
	add string.from.string odd_header temporary_string,
	add string odd_header "\fP\(ts\(ts\\n'page_counter'\s0\(ts")
	|comment<summary: tc_heading (tc program heading)>

|environment(index_placement;
	clear string even_header odd_header page_header,
	new.page odd, spacing on, blank.lines 2i,
	set string temporary_string "INDEX", inline off,
	size +4, inline on, font.family helvetica, blank.lines .25i,
	tab.stops \nWuR,
	font bold, tab, string temporary_string, new.line,
	font roman, font.family times, size -4, inline on,
	minimum.lines 1.25i, paragraph,
	clear string remember_string,
	contents section 1 remember_string temporary_string,
	set string even_header "\(ts\s9\\n'page_counter'\(ts\(ts\f2",
	add string.from.string even_header temporary_string,
	add string even_header "\fP\s0\(ts",
	set string odd_header "\(ts\s9\f2",
	add string.from.string odd_header temporary_string,
	add string odd_header "\fP\(ts\(ts\\n'page_counter'\s0\(ts",
	minimum.lines;)
	|comment<summary: index_placement (put the index here)>

|environment(reference_placement;
	|ifvalue sawreference on [
		clear string even_header odd_header page_header,
		new.page odd, spacing on, blank.lines 2i,
		set string temporary_string "REFERENCES", inline off,
		size +4, inline on, font.family helvetica, blank.lines .25i,
		tab.stops \nWuR,
		font bold, tab, string temporary_string, new.line,
		font roman, font.family times, size -4, inline on,
		minimum.lines 1.25i, paragraph,
		clear string remember_string,
		contents section 1 remember_string temporary_string,
		set string even_header "\(ts\s9\\n'page_counter'\(ts\(ts\f2",
		add string.from.string even_header temporary_string,
		add string even_header "\fP\s0\(ts",
		set string odd_header "\(ts\s9\f2",
		add string.from.string odd_header temporary_string,
		add string odd_header "\fP\(ts\(ts\\n'page_counter'\s0\(ts",
		reference.placement
	];

	sawreference off)
	|comment<summary: reference_placement (produce references here)>

|include(section.h)

|environment(chapter;
	clear string even_header odd_header page_header,
	new.page odd, spacing on, blank.lines 2i,
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
	contents section 1 remember_string temporary_string,
	set string even_header "\(ts\s9\\n'page_counter'\(ts\f2",
	add string.from.string even_header temporary_string,
	add string even_header "\(tsChapter ",
	add string.from.number even_header chapter_counter,
	add string even_header "\fP\s0\(ts",
	set string odd_header "\(ts\s9\f2Chapter ",
	add string.from.number odd_header chapter_counter,
	add string odd_header "\(ts",
	add string.from.string odd_header temporary_string,
	add string odd_header "\fP\(ts\\n'page_counter'\s0\(ts")
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
	]
	set string odd_header "\(ts\s9\f2Section ",
	|ifvalue sectioning bychapter [
		add string.from.number odd_header chapter_counter,
		add string odd_header ".",
	]
	|ifvalue sectioning byappendix [
		add string.from.number odd_header appendix_counter,
		add string odd_header ".",
	]
	add string.from.number odd_header section_counter,
	add string odd_header "\(ts",
	add string.from.string odd_header temporary_string,
	add string odd_header "\fP\(ts\\n'page_counter'\s0\(ts")
	|comment<summary: section (numbered section; IT)>

|environment(appendix;
	clear string even_header odd_header page_header,
	new.page odd, spacing on, blank.lines 2i,
	number format appendix_counter A, incr appendix_counter,
	clear section_counter subsection_counter subsubsection_counter
	paragraph_counter,
	set string temporary_string "Appendix ",
	add string.from.number temporary_string appendix_counter,
	inline off, size +4, font.family helvetica,
	tab.stops \nWuR,
	font bold, tab, string temporary_string, font roman,
	divert.string remember_string;

	sectioning byappendix,
	inline off, size +4, font.family helvetica, blank.lines .25i,
	tab.stops \nWuR,
	font bold, tab, string remember_string, new.line,
	font roman, font.family times, size -4, inline on,
	minimum.lines 1.25i, paragraph,
	store string.from.number remember_string appendix_counter,
	contents section 1 remember_string temporary_string,
	set string even_header "\(ts\s9\\n'page_counter'\(ts\f2",
	add string.from.string even_header temporary_string,
	add string even_header "\(tsAppendix ",
	add string.from.number even_header appendix_counter,
	add string even_header "\fP\s0\(ts",
	set string odd_header "\(ts\s9\f2Appendix ",
	add string.from.number odd_header appendix_counter,
	add string odd_header "\(ts",
	add string.from.string odd_header temporary_string,
	add string odd_header "\fP\(ts\\n'page_counter'\s0\(ts")
	|comment<summary: appendix (numbered appendix; IT)>

|environment(unnumbered_chapter;
	clear string even_header odd_header page_header,
	new.page odd, spacing on, blank.lines 2i,
	divert.string temporary_string;

	sectioning bychapter,
	inline off, size +4, font.family helvetica, blank.lines .25i,
	tab.stops \nWuR,
	font bold, tab, string temporary_string, new.line,
	font roman, font.family times, size -4, inline on,
	minimum.lines 1.25i, paragraph,
	clear string remember_string,
	contents section 1 remember_string temporary_string,
	set string even_header "\(ts\s9\\n'page_counter'\(ts\(ts\f2",
	add string.from.string even_header temporary_string,
	add string even_header "\fP\s0\(ts",
	set string odd_header "\(ts\s9\f2",
	add string.from.string odd_header temporary_string,
	add string odd_header "\fP\(ts\(ts\\n'page_counter'\s0\(ts")
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
	]
	set string odd_header "\(ts\s9\f2",
	add string.from.string odd_header temporary_string,
	add string odd_header "\fP\(ts\(ts\\n'page_counter'\s0\(ts")
	|comment<summary: unnumbered_section (unnumbered section)>

|environment(unnumbered_appendix;
	clear string even_header odd_header page_header,
	divert.string temporary_string;

	new.page odd, spacing on, blank.lines 2i,
	sectioning byappendix,
	inline off, size +4, font.family helvetica, blank.lines .25i,
	tab.stops \nWuR,
	font bold, tab, string temporary_string, new.line,
	font roman, font.family times, size -4, inline on,
	minimum.lines 1.25i, paragraph,
	clear string remember_string,
	contents section 1 remember_string temporary_string,
	set string even_header "\(ts\s9\\n'page_counter'\(ts\(ts\f2",
	add string.from.string even_header temporary_string,
	add string even_header "\fP\s0\(ts",
	set string odd_header "\(ts\s9\f2",
	add string.from.string odd_header temporary_string,
	add string odd_header "\fP\(ts\(ts\\n'page_counter'\s0\(ts")
	|comment<summary: unnumbered_appendix (unnumbered appendix)>

|environment(:document;
	line.length 5i,
	page.length 9.5i,
	clear string even_header odd_header page_header, reference.option rp;)

|environment(:document_save;
	;)

|environment(:document_restore;
	;)

|macro(top.of.page;
	[
'\"	;P page number register TEMP
.		de TP
'		sp
.		af ;P \\g'page_counter'		\" save format of P
.		af page_counter 1			\" normal format for next line
.		nr ;P \\n'page_counter'			\" must use different name for P
.		af page_counter \\g(;P			\" restore format to P
.		af ;P 1					\" normal format for control register
'		sp 2
.		if \\n(;P-1 .tl \\*(page_header		\" output headers
.		if !\\n(;P%2 .tl \\*(even_header
.		if \\n(;P%2 .tl \\*(odd_header
'		sp .75i
.		rr ;P					\" free storage
..
])

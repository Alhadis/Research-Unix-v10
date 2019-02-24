|associate(set_counter;;;

	[page $;;
		set page_counter $]
	[chapter $;;
		set chapter_counter $]
	[section $;;
		set section_counter $]
	[subsection $;;
		set subsection_counter $]
	[subsubsection $;;
		set subsubsection_counter $]
	[paragraph $;;
		set paragraph_counter $]
	[appendix $;;
		set appendix_counter $]
	[figure $;;
		set figure_counter $]
	[table $;;
		set table_counter $]
	[footnote $;;
		set footnote_counter $]
	[note $;;
		set note_counter $])
|comment<summary: set_counter (page, chapter, section, subsection, subsubsection, appendix, figure, table, footnote, note)>

|associate(set_string;;;

	[page_headers $;;
		set string page_header $]
	[even_headers $;;
		set string even_header $]
	[odd_headers $;;
		set string odd_header $]
	[page_footers $;;
		set string page_footer $]
	[even_footers $;;
		set string even_footer $]
	[odd_footers $;;
		set string odd_footer $])
|comment<summary: set_string (page_headers, even_headers, odd_headers, page_footers, even_footers, off_footers)>

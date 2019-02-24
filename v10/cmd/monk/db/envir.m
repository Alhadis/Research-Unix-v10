|environment(roman;
	font roman;)
	|comment<summary: roman (alias for r)>

|environment(r;
	font roman;)
	|comment<summary: r (roman)>

|environment(italics;
	font italics;)
	|comment<summary: italics (alias for i)>

|environment(i;
	font italics;)
	|comment<summary: i (italics)>

|environment(bold;
	font bold;)
	|comment<summary: bold (alias for b)>

|environment(b;
	font bold;)
	|comment<summary: b (bold)>

|environment(bolditalics;
	font bolditalics;)
	|comment<summary: bolditalics (alias for bi)>

|environment(bi;
	font bolditalics;)
	|comment<summary: bi (bolditalics)>

|environment(constant_width;
	font cw;)
	|comment<summary: constant_width (alias for cw)>

|environment(cw;
	font cw;)
	|comment<summary: cw (constant width)>

|environment(here;
	here on;)
	|comment<summary: here (keep a block together on this page or start a new page)>

|environment(around;
	around on;)
	|comment<summary: around (keep a block together here or move it to the next page)>

|environment(around_placement;
	;

	around_placement)
	|comment<summary: around_placement (output all arounds here)>

|environment(full;
	around_width full, footnote_width full;)
	|comment<summary: full (use the whole page for footnotes and arounds)>

|environment(narrow;
	around_width narrow, footnote_width narrow;)
	|comment<summary: narrow (use just this column for footnotes and arounds)>

|associate(blank_space;;;

	[$;
		spacing on, blank.lines $;]
	[;
		spacing on, blank.lines 1;])
	|comment<summary: blank_space (leave this much blank space)>

|associate(new_page;;;

	[1;
		spacing on, new.page;]
	[2;
		spacing on, new.page, spacing on, new.page;]
	[3;
		spacing on, new.page, spacing on, new.page, spacing on, new.page;]
	[4;
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page;]
	[5;
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page;]
	[6;
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page;]
	[7;
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page;]
	[8;
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page;]
	[9;
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page;]
	[10;
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page;]
	[11;
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page;]
	[12;
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page;]
	[13;
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page;]
	[14;
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page;]
	[15;
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page,
		spacing on, new.page, spacing on, new.page, spacing on, new.page;]
	[;
		spacing on, new.page;])
	|comment<summary: new_page (skip to the next next page)>

|associate(column_style; column next;;

	[width $;;
		column width $]
	[offset $;;
		column offset $])
	|comment<summary: column_style (width, offset)>

|associate(new_column;;;

	[$;;
		column old $]
	[;;
		column new])
	|comment<summary: new_column (skip to the next column)>

|environment(indent;
	indent +3;)
	|comment<summary: indent (indent by standard amount)>

|environment(center;
	center on;)
	|comment<summary: center (center each line)>

|environment(center_block;
	fill off, center.block on;)
	|comment<summary: center_block (center all lines as a block)>

|environment(small;
	size -1;)
	|comment<summary: small (reduce the point size)>

|environment(s;
	size -1;)
	|comment<summary: s (alias for small)>

|environment(big;
	size +1;)
	|comment<summary: big (increase point size)>

|environment(u;
	underline on;)
	|comment<summary: u (underline)>

|environment(underline;
	underline on;)
	|comment<summary: underline (alias for u)>

|environment(quotation;
	indent +.5i, line.length -.5i, line.spacing 1;

	line.length +.5i, new.line)
	|comment<summary: quotation (indent left and right)>

|environment(verbatim;
	verbatim on, file.information, line.spacing 1, fill off;)
	|comment<summary: verbatim (text exactly as it is)>

|environment(text;
	fill on;)
	|comment<summary: text (fill the text)>

|environment(left_adjust;
	adjust left;)
	|comment<summary: left_adjust (left adjust the text)>

|associate(left_center_right;

	set string temporary_string "`";

	title temporary_string;

	[$;
		add string temporary_string $, add string temporary_string "`";])
	|comment<summary: left_center_right (left justify, center, right justify a line>

|environment(footnote;
	size -2, vertical.motion -.4m,
	number ++ footnote_counter, vertical.motion +.4m, space -2, line.spacing 1,
	footnote on, indent 0.2i, indent.line -0.2i,
	number footnote_counter, text ".", horizontal.motion "|0.2i";

	SPACE)
	|comment<summary: footnote (numbered footnote)>

|environment(unnumbered_footnote;
	size -2, space -2, line.spacing 1,
	footnote on, indent 0.2i;

	SPACE)
	|comment<summary: unnumbered_footnote (unnumbered footnote)>

|environment(dagnote;
	size -2, vertical.motion -.4m,
	text "\(dg", vertical.motion +.4m, space -2, line.spacing 1,
	footnote on, indent 0.2i, indent.line -0.2i,
	text "\(dg", horizontal.motion "|0.2i";

	SPACE)
	|comment<summary: dagnote (footnote marked with a dagger)>

|environment(authordagnote;
	size -2, space -2, line.spacing 1,
	footnote on, indent 0.2i, indent.line -0.2i,
	text "\(dg", horizontal.motion "|0.2i";

	SPACE)
	|comment<summary: authordagnote (footnote marked with a dagger for author names)>

|environment(starnote;
	size -2, vertical.motion -.4m,
	text "\(**", vertical.motion +.4m, space -2, line.spacing 1,
	footnote on, indent 0.2i, indent.line -0.2i,
	text "\(**", horizontal.motion "|0.2i";

	SPACE)
	|comment<summary: starnote (footnote marked with a star)>

|environment(endnote;
	size -2, vertical.motion -.4m,
	number ++ endnote_counter, vertical.motion +.4m, size +2,
	append.output on endnote_output,
	|ifvalue sawendnote on [
		minimum.lines,
	]
	indent.line 3, number endnote_counter, text ". ";

	sawendnote on)
	|comment<summary: endnote (numbered endnote)>

|environment(endnote_placement;
	|ifvalue sawendnote on [
		new.page,
		set string temporary_string "NOTES",
		inline off, size +1, center on,
		font bold, string temporary_string, font roman,
		center off, size -1, inline on,
		minimum.lines,
		clear string remember_string,
		contents section 1 remember_string temporary_string,
		line.spacing 1, macro endnote_output
	];

	sawendnote off)
	|comment<summary: endnote_placement (produce endnotes here)>

|environment(notation;
	minimum.lines, fill off;

	minimum.lines)
	|comment<summary: notation (notation)>

|environment(approved;
	minimum.lines 2, text bold "Approval:  ", blank.lines 3,
	line 3.5i, tab.stops 1.5iC, new.line, tab;

	tab, minimum.lines)
	|comment<summary: approved (approved)>

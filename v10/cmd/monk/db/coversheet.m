|REGISTER(current_diversion, .z)

|associate(mercury;
	clear temporary_counter, clear string temporary_array[0] temporary_array[1]
	temporary_array[2] temporary_array[3] temporary_array[4] temporary_array[5]
	temporary_array[6] temporary_array[7] temporary_array[8] temporary_array[9];

	if begin EQUAL(temporary_array[0], yes),
		incr mercury_counter,
		set string mercury_array[mercury_counter]
		"CHM - Chemistry and Materials",
	if end,
	if begin EQUAL(temporary_array[1], yes),
		incr mercury_counter, set string mercury_array[mercury_counter]
		"CMM - Communications",
	if end,
	if begin EQUAL(temporary_array[2], yes),
		incr mercury_counter, set string mercury_array[mercury_counter]
		"CMP - Computing",
	if end,
	if begin EQUAL(temporary_array[3], yes),
		incr mercury_counter, set string mercury_array[mercury_counter]
		"ELC - Electronics",
	if end,
	if begin EQUAL(temporary_array[4], yes),
		incr mercury_counter, set string mercury_array[mercury_counter]
		"LFS - Life Sciences",
	if end,
	if begin EQUAL(temporary_array[5], yes),
		incr mercury_counter,
		set string mercury_array[mercury_counter]
		"MAN - Manufacturing",
	if end,
	if begin EQUAL(temporary_array[6], yes),
		incr mercury_counter,
		set string mercury_array[mercury_counter]
		"MAS - Mathematics and Statistics",
	if end,
	if begin EQUAL(temporary_array[7], yes),
		incr mercury_counter, set string mercury_array[mercury_counter]
		"MKT - Marketing",
	if end,
	if begin EQUAL(temporary_array[8], yes),
		incr mercury_counter, set string mercury_array[mercury_counter]
		"PHY - Physics",
	if end,
	if begin EQUAL(temporary_array[9], yes),
		incr mercury_counter, set string mercury_array[mercury_counter]
		"STD - Standards",
	if end;

	[chm;
		incr temporary_counter, incr mercury_counter,
		set string mercury_array[mercury_counter]
		"CHM - Chemistry and Materials";]
	[cmm;
		incr temporary_counter, incr mercury_counter,
		set string mercury_array[mercury_counter] "CMM - Communications";]
	[cmp;
		incr temporary_counter, incr mercury_counter,
		set string mercury_array[mercury_counter] "CMP - Computing";]
	[elc;
		incr temporary_counter, incr mercury_counter,
		set string mercury_array[mercury_counter] "ELC - Electronics";]
	[lfs;
		incr temporary_counter, incr mercury_counter,
		set string mercury_array[mercury_counter] "LFS - Life Sciences";]
	[man;
		incr temporary_counter, incr mercury_counter,
		set string mercury_array[mercury_counter] "MAN - Manufacturing";]
	[mas;
		incr temporary_counter, incr mercury_counter,
		set string mercury_array[mercury_counter]
		"MAS - Mathematics and Statistics";]
	[mkt;
		incr temporary_counter, incr mercury_counter,
		set string mercury_array[mercury_counter] "MKT - Marketing";]
	[phy;
		incr temporary_counter, incr mercury_counter,
		set string mercury_array[mercury_counter] "PHY - Physics";]
	[std;
		incr temporary_counter, incr mercury_counter,
		set string mercury_array[mercury_counter] "STD - Standards";]
	[yes;
		incr temporary_counter,
		set string temporary_array[temporary_counter] yes;]
	[no;
		incr temporary_counter,
		set string temporary_array[temporary_counter] no;])
	|comment<summary: mercury (chm, cmm, cmp, elc, lfs, man, mas, mkt, phy, std mercury distribution for cover sheet)>

|associate(proprietary_class;;;
	[yes;
		set string proprietary_string yes;]
	[no;
		set string proprietary_string no;])
	|comment<summary: proprietary_class (proprietary classification for cover sheet)>

|associate(government_security;;;
	[yes;
		set string government_string yes;]
	[no;
		set string government_string no;])
	|comment<summary: government_security (government security clearance for cover sheet)>

|associate(earlier;;;

	[document_number $;
		set string earlier_document_number_string $;]
	[$;
		set string earlier_document_number_string $;])
	|comment<summary: earlier (replacing earlier document_number for cover sheet)>

|associate(att;;;

	[is_release yes;
		set string att_release_string yes;]
	[is_release no;
		set string att_release_string no;]
	[yes;
		set string att_release_string yes;]
	[no;
		set string att_release_string no;])
	|comment<summary: att (is_release for cover sheet)>

|environment(dh_name;
	divert.string dh_string;)
	|comment<summary: dh_name (department head's name for cover sheet)>

|environment(director_name;
	divert.string director_string;)
	|comment<summary: director_name (director's name for cover sheet)>

|environment(distribute_complete_memo;
	divert.input on complete_memo_string;)
	|comment<summary: distribute_complete_memo (distribution list for complete memo for cover sheet)>

|environment(distribute_cover_sheet;
	divert.input on cover_sheet_string;)
	|comment<summary: distribute_cover_sheet (distribution list for cover sheet for cover sheet)>

|associate(add_totals;;;

	[text $;
		add constant text_counter $, add constant other_marker $;]
	[other $;
		add constant other_counter $;]
	[figures $;
		add constant figure_counter $;]
	[tables $;
		add constant table_counter $;]
	[references $;
		add constant reference_counter $;])
	|comment<summary: add_totals (text, other, figure, table, reference counts for cover sheet)>

|environment(other;
		store other_marker page_counter;)
	|comment<summary: other (end of text body/start of attached pages)>

|environment(cover_sheet;
	clear string page_header page_footer, new.page, line.spacing 1,
	inline off, size 10, inline on, font.family helvetica,
	line.length 7.5i, page.length +.75i, page.offset .3i, indent 0,
	cs_heading_page1,
	cs_title,
	cs_authors,
	cs_numbers,
	cs_keywords,
	cs_mercury,
	cs_abstract_part1,
	cs_footer_page1,
	cs_abstract_part2,
	cs_heading_page2,
	cs_distribution_list,
	cs_spacing,
	cs_government,
	cs_att,
	cs_signatures,
	cs_proprietary,
	cs_earlier,
	cs_footer_page2,
	cs_abstract_continuation,
	cs_distribution_continuation;)
	|comment<summary: cover_sheet (produce the coversheet here)>

|environment(cs_heading_page1;
	spacing on, goto 0.2i, inline off, size 16, space 18, font bold,
	title "AT&T Bell Laboratories" " " "Document Cover Sheet",
	title " " " " "for Technical Memorandum";

	thick.line)

|environment(cs_title;
	new.line, remember position0,
	text bold "Title:", horizontal.motion 5.5i,
	text bold "Author's Date:",
	return position0,
	indent +.5i, line.length 5.0i,
	macro title_string,
	line.length 7.5i, indent -.5i,
	return position0, blank.lines, indent.line 6i,
	string date_string, new.line;

	blank.lines -0.1i, thick.line)

|environment(cs_authors;
	tab.stops 1.5i 4.45i 5.7i 6.95i,
	if else GT(author_counter, 1),
		set string plural_string "s",
	if else,
		clear string plural_string,
	tab, text bold "Author",
	string bold plural_string,
	tab, text bold "Location", tab, text bold "Ext.",
	tab, text bold "Dept.",
	tab.stops 4.2i 4.5i 5.5i 6.7i, indent .2i,
|for i in 1 2 3 4 5 6 7 8 9 [
	if begin GE(author_counter, $i),
		new.line, string author_array[$i], tab,
		string location_array[$i], text " ", string room_array[$i], tab,
		string extension_array[$i], tab, string department_array[$i],
	if end,
]
	new.line;

	blank.lines -0.1i, thick.line)

|environment(cs_numbers;
	tab.stops 0.9i 3.4i 5.65i,
	if else GT(document_counter, 1),
		set string plural_string "s",
	if else,
		clear string plural_string,
	tab, text bold "Document No",
	string bold plural_string, text bold ".",
	tab, text bold "Filing Case No",
	string bold plural_string, text bold ".",
	tab, text bold "Project No",
	string bold plural_string, text bold ".",
	tab.stops 0.8i 3.5i 5.75i,
|for i in 1 2 3 4 5 6 7 8 9 [
	if begin GE(document_counter, $i),
		new.line, tab, string document_array[$i],
		tab, string file_case_array[$i], tab, string work_program_array[$i],
	if end,
]
	new.line;

	blank.lines -0.1i, thick.line)

|environment(cs_keywords;
	text bold "Keywords:",
	blank.lines 0.1i, indent.line .2i, adjust off,
	macro keyword_string, new.line;

	blank.lines -0.1i, thick.line)

|environment(cs_mercury;
	tab.stops 2i 4i,
	text bold "MERCURY Announcement Bulletin Sections:",
	blank.lines 0.1i, indent .9i, inline off, size 8, space 10,
|for i in 1 4 7 [
	if begin GE(mercury_counter, $i),
		set i_plus_one $i+1,
		set i_plus_two $i+2,
		string mercury_array[$i], tab,
		string mercury_array[i_plus_one], tab,
		string mercury_array[i_plus_two],
	if end,
]
	;

	blank.lines -0.1i, thick.line)

|environment(cs_abstract_part1;
	text bold "Abstract:", minimum.lines 0, paragraph,
	remember position0;)

|environment(cs_footer_page1;
	goto 9.375i,
	remember position1,
	thick.line,

	store total_counter page_counter, decr total_counter,
	add total_counter text_counter, add total_counter other_counter,
	add constant total_counter 2,
	store other_counter total_counter, sub other_counter other_marker,
	store text_counter total_counter, sub text_counter other_counter,
	size -2,
	text bold "Total Pages ", text "(including document cover sheet): ",
	size +2, number total_counter,

	return position1, blank.lines .5i, indent .6i,
	if else begin EQUAL(proprietary_string, yes),
		text bold "       AT&T \(em PROPRIETARY (Restricted)",
		indent -.1i,
		new.line, text "Solely for authorized persons having a need to know",
		new.line, text "Use pursuant to Company Instructions.",
		new.line, text "Requires Department Head approval (see below).",
	if end,
	if else begin,
		text bold "       AT&T \(em PROPRIETARY",
		indent -.1i,
		new.line, text "Use pursuant to Company Instructions",
	if end,
	indent 0,

	return position1, blank.lines .2i, indent.line 5.25i,
	text bold "Mailing Label";)

|environment(cs_abstract_part2;
	return position0, indent .2i, line.length 7i,
	invoke at constant 9.375i catch_diverter, macro abstract_string,
	if begin EQUAL(current_diversion, catch_output),
		invoke end, rename catch_output abstract_string,
		store diversionheight more_abstract,
	if end;

	invoke off catch_diverter, line.length 7.5i)

|environment(cs_heading_page2;
	new.page, spacing on, goto 0.2i,

	text bold "Initial Distribution Specifications",
	horizontal.motion 3.0i,
	string bold document_array[1],
	new.line;

	blank.lines -0.1i, thick.line)

|environment(cs_distribution_list;
	horizontal.motion 1.25i,
	text bold "Complete Copy", horizontal.motion 2.75i,
	text bold "Cover Sheet Only",
	new.line, remember position0, fill off, indent .2i,
	set temporary_counter 6.25i,
	if UNEQUAL(proprietary_string, yes),
		if NULL(dh_string),
			if NULL(director_string),
				add constant temporary_counter 0.6875i,
	if UNEQUAL(government_string, yes),
		add constant temporary_counter 0.25i,
	if LE(author_counter, 3),
		add constant temporary_counter 0.4375i,
	if LE(author_counter, 6),
		add constant temporary_counter 0.4375i,
	if NULL(earlier_document_number_string),
		add constant temporary_counter 0.25i,
	invoke on number temporary_counter catch_diverter,
	macro complete_memo_string,
	if else begin EQUAL(current_diversion, catch_output),
		invoke end, rename catch_output complete_memo_string,
		store diversionheight more_completememo,
	if end,
	if else,
		clear string complete_memo_string,
	return position0, indent 4i,
	invoke on number temporary_counter catch_diverter,
	macro cover_sheet_string,
	if else begin EQUAL(current_diversion, catch_output),
		invoke end, rename catch_output cover_sheet_string,
		store diversionheight more_coversheet,
	if end,
	if else,
		clear string cover_sheet_string;

	indent 0, invoke off catch_diverter)

|environment(cs_spacing;
	goto 6.75i,

	if UNEQUAL(proprietary_string, yes),
		if NULL(dh_string),
			if NULL(director_string),
				blank.lines 0.6875i,
	if UNEQUAL(government_string, yes),
		blank.lines 0.25i,
	if LE(author_counter, 3),
		blank.lines 0.4375i,
	if LE(author_counter, 6),
		blank.lines 0.4375i,
	if NULL(earlier_document_number_string),
		blank.lines 0.25i;

	thick.line)

|environment(cs_government;
	if begin EQUAL(government_string, yes),
		text bold "Government Security Classified",
		blank.lines -0.1i, thick.line,
	if end;)

|environment(cs_att;
	if else begin EQUAL(att_release_string, no),
		text bold "Future AT&T Distribution by ITDS - ",
		text "Obtain approval for release to any AT&T employee",
		blank.lines -0.1i, thick.line,
	if end,
	if else begin,
		text bold "Future AT&T Distribution by ITDS - ",
		text "Release to any AT&T employee (excluding contract employees)",
		blank.lines -0.1i, thick.line,
	if end;)

|environment(cs_signatures;
	if else GT(author_counter, 1),
		set string plural_string "s",
	if else,
		clear string plural_string,
	text bold "Author Signature",
	string bold plural_string, new.line,
	if EQUAL(author_counter, 0),
		blank.lines 0.4375i,
|for i in 0 3 6 [
	tab.stops 2.635i 5.25i,
	if GT(author_counter,$i),
		blank.lines 0.1i,
	if EQUAL(author_counter, ($i+1)),
		line 2.25i,
	if begin EQUAL(author_counter, ($i+2)),
		line 2.25i, tab, line 2.25i,
	if end,
	if begin GT(author_counter, ($i+2)),
		line 2.25i, tab, line 2.25i, tab, line 2.25i,
	if end,
	new.line,
	if begin GT(author_counter, $i),
		tab.stops 0.25i 2.875i 5.5i,
		set i_plus_one $i+1,
		set i_plus_two $i+2,
		set i_plus_three $i+3,
		tab, string author_array[i_plus_one],
		tab, string author_array[i_plus_two],
		tab, string author_array[i_plus_three],
	if end,
]
	;

	blank.lines -0.1i, thick.line)

|environment(cs_proprietary;
	if begin EQUAL(proprietary_string, yes),
		text bold "Organizational Approval ",
		size -1, text "(Department Head approval required)", size +1,
		if begin NULL(dh_string),
			if begin NULL(director_string),
				tab.stops .5i 4i,
				blank.lines 0.1i,
				tab, tab, line 3i,
				new.line, tab.stops 7iR,
				tab, text "Department Head",
				blank.lines -0.1i, thick.line,
			if end,
		if end,
	if end,

|LITERAL( a_kludge, \*('dh_string'\*('director_string')

	if begin NOTNULL( a_kludge),
			if begin UNEQUAL(proprietary_string, yes),
				text bold "Organizational Approval ",
				size -1, text "(Optional)", size +1,
			if end,
			tab.stops .5i 4i,
			blank.lines 0.1i,
			if else begin NOTNULL(dh_string),
				if else begin NOTNULL(director_string),
					tab, line 3i, tab, line 3i,
				if end,
				if else begin,
					tab, tab, line 3i,
				if end,
			if end,
			if else begin,
				tab, tab, line 3i,
			if end,
			new.line, tab.stops .8i 4.3i,
			if else begin NOTNULL(dh_string),
				if else begin NOTNULL(director_string),
					tab,
					string dh_string, text ", Department Head",
					tab,
					string director_string, text ", Director",
				if end,
				if else begin,
					tab, tab,
					string dh_string, text ", Department Head",
				if end,
			if end,
			if else begin,
				tab, tab,
				string director_string, text ", Director",
			if end,
			blank.lines -0.1i, thick.line,
	if end;)

|environment(cs_earlier;
	if begin NOTNULL(earlier_document_number_string),
		text bold
		    "Supersedes or amends document number  ",
		string earlier_document_number_string,
		new.line, blank.lines -0.1i, thick.line,
	if end;)

|environment(cs_footer_page2;
	text bold "For Use by Recipient of Cover Sheet:",
	new.line, remember position0, size -3, space -4,
	blank.lines -.1i, line.length 3.5i, indent .1i, indent.line -.1i,
	text "Computing network users may order copies via the ",
	text italics "library -k ", text "command;",
	new.line, text "for information, type ",
	text italics "man library ", text "after the ",
	size -1, text "UNIX", size +1, text " prompt.",
	new.line, blank.lines 0.05i, indent.line -.1i,
	text "Otherwise:",
	new.line,
	text "Enter ", size -1, text "PAN", size +1, text " if ",
	size -1, text "AT&T-BL (SS#", size +1, text " if non ",
	size -1, text "AT&T-BL.) ", size +1, line 1.2i,
	new.line, text "Return this sheet to any ",
	size -1, text "ITDS", size +1, text " location.",
	new.line, return position0, line.length 8.0i, indent 4i,
	text "\ Internal Technical Document Service",
	blank.lines, tab.stops 1i 2i 3i,
	text "( ) AK 2H-28", tab, text "( ) IH 7M-103", tab,
	text "( ) DR 2F-19", tab, text "( ) NW-ITDS", new.line,
	text "( ) ALC 1B-102", tab, text "( ) MV 3L-19", tab,
	text "( ) INH 1C-114", tab, text "( ) PR 5-2120", new.line,
	text "( ) CB 30-2011", tab, text "( ) WH 3E-204", tab,
	text "( ) IW 2Z-156", new.line,
	text "( ) HO 4F-112", tab, tab, text "( ) MT 3B-117", new.line,
	line.length 7.5i;

	indent 0, SPACE)

|environment(cs_abstract_continuation;
	if begin GT(more_abstract, 0),
		new.page, spacing on, goto 0.2i,

		horizontal.motion 5.2i, string bold document_array[1],
		new.line, blank.lines -0.1i, thick.line,

		new.line, text bold "Title:",
		horizontal.motion 0.375i, macro title_string,
		new.line, blank.lines -0.1i, thick.line,

		center on, text bold "Abstract (continued)", new.line, center off,
		fill off, macro abstract_string, fill on,
		blank.lines -0.1i, thick.line,
	if end;)

|environment(cs_distribution_continuation;
	store temporary_counter more_completememo,
	store totrap position0,
	if GT(more_coversheet, temporary_counter),
		store temporary_counter more_coversheet,

	if begin GT(temporary_counter, 0),
		if begin GE(temporary_counter, position0),
			new.page, spacing on, goto 0.2i,

			horizontal.motion 5.2i, string bold document_array[1],
			new.line, blank.lines -0.1i, thick.line,

			new.line, text bold "Title:",
			horizontal.motion 0.375i, macro title_string,
			new.line, blank.lines -0.1i, thick.line,
		if end,
		new.line,

		remember position0,
		if begin GT(more_completememo, 0),
			horizontal.motion 1.25i,
			text bold "Complete Copy (continued)", new.line,
			fill off, macro complete_memo_string, fill on,
			if begin GT(more_completememo, more_coversheet),
				blank.lines -0.1i, thick.line,
			if end,
		if end,

		if begin GT(more_coversheet, 0),
			return position0, horizontal.motion 4.5i,
			text bold "Cover Sheet Only (continued)", new.line,
			fill off, macro cover_sheet_string, fill on,
			if begin GE(more_coversheet, more_completememo),
				blank.lines -0.1i, thick.line,
			if end,
		if end,
	if end,
	new.line;)

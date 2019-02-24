|include(macro.h)

|include(global.h)

|environment(reference_placement;
	|ifvalue sawreference on [
		minimum.lines 2, protect,
		set string temporary_string "REFERENCES",
		inline off, size +1, center on,
		font bold, string temporary_string, font roman,
		center off, size -1, inline on,
		minimum.lines,
		clear string remember_string,
		contents section 1 remember_string temporary_string,
		reference.placement
	];

	sawreference off)
	|comment<summary: reference_placement (produce references here)>

|include(titlebox.h)
|include(titlebox.m)

|environment(titlebox;
	;

	new.page, fill off, inline off, size +2, space +2, font bold, center on,
	macro title_string, font roman, size -2, space -2, SPACE, inline on, fill on,
	blank.lines,
|for i in 1 2 3 4 5 6 7 8 9 [
	if begin GE(author_counter, $i),
		new.line, size +1, font italics,
		string author_array[$i], font roman, size -1,
		if begin UNEQUAL(location_array[$i], location_array[$i+1]),
			if begin EQUAL(location_array[$i], BC),
				text "Boston College", new.line,
				text "Chestnut Hill, MA 02167", new.line,
			if end,
			if begin EQUAL(location_array[$i], CM),
				text "Carnegie-Mellon University", new.line,
				text "Electrical and Computer Engineering Department", new.line,
				text "Pittsburgh, PA 15213", new.line,
			if end,
			if begin EQUAL(location_array[$i], SDSU),
				text "San Diego State University", new.line,
				text "San Diego, CA 92000", new.line,
			if end,
			if begin EQUAL(location_array[$i], UA),
				text "University of Antwerp", new.line,
				text "B-2610 Antwerp, Belgium", new.line,
			if end,
			if begin EQUAL(location_array[$i], UCB),
				text "University of California", new.line,
				text "Berkeley, CA 94720", new.line,
			if end,
			if begin EQUAL(location_array[$i], UNL),
				text "University of Nebraska", new.line,
				text "Lincoln, NE 68588", new.line,
			if end,
			if begin EQUAL(location_array[$i], AK),
				text "AT&T Bell Laboratories", new.line,
				text "Norcross, GA 30071", new.line,
			if end,
			if begin EQUAL(location_array[$i], AL),
				text "AT&T Bell Laboratories", new.line,
				text "Allentown, PA 18103", new.line,
			if end,
			if begin EQUAL(location_array[$i], ALC),
				text "AT&T Bell Laboratories", new.line,
				text "Allentown, PA 18103-6265", new.line,
			if end,
			if begin EQUAL(location_array[$i], AN),
				text "AT&T Bell Laboratories", new.line,
				text "Andover, MA 01810", new.line,
			if end,
			if begin EQUAL(location_array[$i], CB),
				text "AT&T Bell Laboratories", new.line,
				text "Columbus, OH 43213", new.line,
			if end,
			if begin EQUAL(location_array[$i], CH),
				text "AT&T Bell Laboratories", new.line,
				text "Chester, NJ 07930", new.line,
			if end,
			if begin EQUAL(location_array[$i], DR),
				text "AT&T Bell Laboratories", new.line,
				text "Denver, CO 80234", new.line,
			if end,
			if begin EQUAL(location_array[$i], HL),
				text "AT&T Bell Laboratories", new.line,
				text "Short Hills, NJ 07078-0905", new.line,
			if end,
			if begin EQUAL(location_array[$i], HO),
				text "AT&T Bell Laboratories", new.line,
				text "Holmdel, NJ 07733-1988", new.line,
			if end,
			if begin EQUAL(location_array[$i], HOH),
				text "AT&T Bell Laboratories", new.line,
				text "Holmdel, NJ 07733-0400", new.line,
			if end,
			if begin EQUAL(location_array[$i], HR),
				text "AT&T Bell Laboratories", new.line,
				text "Middletown, NJ 07748-3052", new.line,
			if end,
			if begin EQUAL(location_array[$i], IH),
				text "AT&T Bell Laboratories", new.line,
				text "Naperville, IL 60566-7033", new.line,
			if end,
			if begin EQUAL(location_array[$i], INH),
				text "AT&T Bell Laboratories", new.line,
				text "Indianapolis, IN 46206", new.line,
			if end,
			if begin EQUAL(location_array[$i], INP),
				text "AT&T Bell Laboratories", new.line,
				text "Naperville, IL 60566-7050", new.line,
			if end,
			if begin EQUAL(location_array[$i], KD),
				text "AT&T Bell Laboratories", new.line,
				text "Piscataway, NJ 08854", new.line,
			if end,
			if begin EQUAL(location_array[$i], LC),
				text "AT&T Bell Laboratories", new.line,
				text "Warren, NJ 07060", new.line,
			if end,
			if begin EQUAL(location_array[$i], LZ),
				text "AT&T Bell Laboratories", new.line,
				text "Lincroft, NJ 07738-1526", new.line,
			if end,
			if begin EQUAL(location_array[$i], MH),
				text "AT&T Bell Laboratories", new.line,
				text "Murray Hill, NJ 07974-2070", new.line,
			if end,
			if begin EQUAL(location_array[$i], MO),
				text "AT&T Bell Laboratories", new.line,
				text "Morristown, NJ 07960-4499", new.line,
			if end,
			if begin EQUAL(location_array[$i], MT),
				text "AT&T Bell Laboratories", new.line,
				text "Middletown, NJ 07748-4801", new.line,
			if end,
			if begin EQUAL(location_array[$i], MV),
				text "AT&T Bell Laboratories", new.line,
				text "North Andover, MA 01845", new.line,
			if end,
			if begin EQUAL(location_array[$i], PK),
				text "AT&T Bell Laboratories", new.line,
				text "Parsippany, NJ 07054-11794", new.line,
			if end,
			if begin EQUAL(location_array[$i], RD),
				text "AT&T Bell Laboratories", new.line,
				text "Reading, PA 19604", new.line,
			if end,
			if begin EQUAL(location_array[$i], SF),
				text "AT&T Bell Laboratories", new.line,
				text "Summit, NJ 07901-1444", new.line,
			if end,
			if begin EQUAL(location_array[$i], WB),
				text "AT&T Bell Laboratories", new.line,
				text "West Long Branch, NJ 07764-1394", new.line,
			if end,
			if begin EQUAL(location_array[$i], WH),
				text "AT&T Bell Laboratories", new.line,
				text "Whippany, NJ 07981-0903", new.line,
			if end,
			if begin EQUAL(location_array[$i], WI),
				text "AT&T Bell Laboratories", new.line,
				text "Ward Hill, MA 01830", new.line,
			if end,
		if end,
	if end,
]
	center off, goto 2i, column on,

	|ifvalue sawabstract on {
		blank.lines, center on, size +1, text italics "ABSTRACT",
		size -1, center off, minimum.lines, paragraph,
		macro abstract_string,
		minimum.lines,
	}

	|ifvalue sawkeywords on {
		font italics, text "Keywords:", macro keyword_string, font roman,
	})
	|comment<summary: titlebox (produce title box here)>

|environment(:document;
	clear author_counter document_counter,
	format.type acm,
	column 1 3.33i 0.4375i, column 2 3.33i 4.0975i,
	page.length 9.625i,
	inline off, size 9, inline on, space 11, reference.option rp, reference.style acm;)

|environment(:document_save;
	;)

|environment(:document_restore;
	;)

|macro(top.of.page;
	[
'\"	;P page number register TEMP
.		de TP
'		sp |.75i
.		rr ;P					\" free storage
..
])

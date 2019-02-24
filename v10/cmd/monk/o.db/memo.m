|include(macro.h)

|include(titlebox.h)
|include(titlebox.m)

|environment(titlebox;
	;

	new.page, indent.line +4.375i, string att_logo,
	blank.lines 2, size -2, remember position0,
	page.offset -.5i, text "subject:", new.line,
	return position0, indent +4.875i, text "date:", blank.lines,
	text "from:", indent -4.875i, new.line, page.offset +.5i, size +2,
	line.length -3i, font bold, return position0, fill off,
	macro title_string, fill on, line.length +3i, new.line,
	size -1,
|for i in 1 2 3 4 5 6 7 8 9 [
	if begin GE_NUMBER(document_counter, $i),
		new.line, text "Work Program ", string work_program_array($i),
		text " File Case ", string file_case_array($i),
	if end,
]
	size +1,
	return position0, indent +4.75i, line.length +3i,
	string date_string,
|for i in 1 2 3 4 5 6 7 8 9 [
	if begin GE_NUMBER(author_counter, $i),
		blank.lines,
		string author_array($i), new.line,
		string location_array($i), text " ",
		string department_array($i), new.line,
		string room_array($i), text " ",
		string extension_array($i),
	if end,
]
	line.length -3i, spacing on, blank.lines, fill off,
	string document_array(1), fill on, indent -4.75i,
	minimum.lines, inline off, font roman, inline on)
	|comment<summary: titlebox (produce title box here)>

|environment(signature;
	here on, minimum.lines 1, indent +30, line.length +3i,
|for i in 1 2 3 4 5 6 7 8 9 [
	if begin GE_NUMBER(author_counter, $i),
		blank.lines 3,
		string bold author_array($i),
	if end,
]
	line.length -3i, indent -30, blank.lines -1;

	minimum.lines 3)
	|comment<summary: signature (signatures and typing credits)>

|environment(:document;
	clear author_counter document_counter;)

|environment(:document_save;
	;)

|environment(:document_restore;
	;)

|include(global.h)

|include(macro.h)

|STRING(name_string, N0)
|STRING(number_string, N1)
|STRING(range_string, N2)
|STRING(author_string, N3)
|STRING(wpn_string, N4)

|REGISTER(position, P0)

|associate(department;;;

	[name $;
		set string name_string $;]
	[number $;
		set string number_string $;])
	|comment<summary: department (name, number for title box)>

|associate(date;;;

	[report $;
		set string date_string $;]
	[range $;
		set string range_string $;])
	|comment<summary: date (report, range for title box)>

|associate(author;;;

	[name $;
		set string author_string $;]
	[number $;
		set string wpn_string $;])
	|comment<summary: author (name, number for footer and title box)>

|environment(titlebox;

	font bold, line.length +.5i, page.offset -.25i,
	spacing on, goto .5i, blank.lines -1,
	set string temporary_string "`Research Report`Department ",
	add string.from.string temporary_string number_string,
	add string temporary_string "`",
	add string.from.string temporary_string date_string,
	add string temporary_string "`",
	title temporary_string,

	blank.lines .5,
	set string temporary_string "`",
	add string.from.string temporary_string range_string,
	add string temporary_string "`",
	add string.from.string temporary_string name_string,
	add string temporary_string "`\s-1WPN\s+1 ",
	add string.from.string temporary_string wpn_string,
	add string temporary_string "`",
	title temporary_string,

	page.offset +.25i, line.length -.5i, minimum.lines 1;)
	|comment<summary: titlebox (title box)>

|associate(paragraph;
	minimum.lines .5, clear string temporary_string;;

	[indented;
		indent.line 3;]
	[i;
		indent.line 3;])
	|comment<summary: paragraph (i, indented)>

|associate(p;
	minimum.lines .5, clear string temporary_string;;

	[indented;
		indent.line 3;]
	[i;
		indent.line 3;])
	|comment<summary: p (alias for paragraph)>

|environment(section;
	minimum.lines .5, protect, font bold;

	paragraph)
	|comment<summary: section (section)>

|environment(signature;
	minimum.lines 3,
	remember position,
	indent.line .3125i,
	text "AT&T \(em PROPRIETARY", new.line,
	text "Use pursuant to Company Instructions", new.line,
	return position,
	indent 3i, blank.lines 1,
	tab.stops 1.5iC, fill off, tab, string author_string, tab;)
	|comment<summary: signature (signature)>

|environment(:document;
	page.offset +.25i;)

|environment(:document_save;
	;)

|environment(:document_restore;
	;)

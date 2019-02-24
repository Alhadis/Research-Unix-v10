|include(macro.h)

|DEFINE_STRING(name_string, N0)
|DEFINE_STRING(nicname_string, N1)
|DEFINE_STRING(phone_string, N2)
|DEFINE_STRING(room_string, N3)
|DEFINE_STRING(email_string, N4)
|DEFINE_STRING(pre_string, N5)

|environment(date;
	divert.string date_string;)
	|comment<summary: date (date of center phone book)>

|environment(titlebox;
	new.line, spacing on, goto .75i,
	indent.line +6.0i, string att_logo, new.line,
	indent.line +6.5i, string date_string,
	goto 9.75i, page.offset 2.625i, line.length 2.625i, center on,
	text "AT&T \(em PROPRIETARY", new.line,
	text "Use pursuant to Company Instructions", new.line,
	center off;)
	|comment<summary: titlebox (title box)>

|environment(laboratory;
	goto .5i;
	size 8, space 8,
	tab.stops "\w'\s-1\f3\&9999\fP\s+1\& 'u +\w'# 'u +\w'Papathomas,T.V. (\f2\&Thomas\fP\&) 'u +\w'maureen 'u",
	line.length 8.5, column on)
	|comment<summary: laboratory (a laboratory)>

|environment(laboratory_title;
	inline off, font bold, size 12, space 14, center on;

	minimum.lines .5)
	|comment<summary: laboratory_title (title of laboratory)>

|associate(employee;
	clear string name_string nicname_string phone_string email_string
	room_string pre_string;

	size -1, string bold phone_string, size +1, tab,
	string pre_string, tab,
	string name_string, text " (",
	string italics nicname_string, text ")", tab,
	string email_string, tab,
	size -1, string bold room_string, size +1, new.line;

	[name $;
		set string name_string $;]
	[nicname $;
		set string nicname_string $;]
	[phone $;
		set string phone_string $;]
	[room $;
		set string room_string $;]
	[email $;
		set string email_string $;]
	[secretary;
		set string pre_string "$";]
	[casual;
		set string pre_string "*";]
	[consultant;
		set string pre_string "%";]
	[contractor;
		set string pre_string "{";]
	[intern;
		set string pre_string "!";]
	[limited term;
		set string pre_string "<";]
	[post doc;
		set string pre_string ">";]
	[resident visitor;
		set string pre_string "#";]
	[temporary;
		set string pre_string "+";]
	[mit coop;
		set string pre_string "}";]
	[department 45266;
		set string pre_string "\s-1\&@\s+1\&";])
	|comment<summary: employee (name, nicname, phone, room, email, secretary, casual, consultant, contractor, intern, limited term, post doc, resident visitor, temporary, mit coop,department 45266)>

|environment(department_title;
	inline off, font bold, center on;

	minimum.lines .5)
	|comment<summary: department_title (title of department)>

|environment(department;
	here on, minimum.lines 1.5;)
	|comment<summary: department (a department)>

|associate(room;
	clear string name_string phone_string room_string email_string;

	size -1, string bold phone_string, size +1, tab, tab,
	string name_string, tab,
	string email_string, tab,
	size -1, string bold room_string, size +1, new.line;

	[name $;
		set string name_string $;]
	[phone $;
		set string phone_string $;]
	[room $;
		set string room_string $;]
	[email $;
		set string email_string $;])
	|comment<summary: room (name, phone, room, email)>

|environment(:document;
	page.offset 0.0i,
	column 1 2.625i 0.0i, column 2 2.625i 2.6875i, column 3 2.625i 5.3125i,
	tab.stops "\w'\s-1\f3\&9999\fP\s+1\& 'u +\w'# 'u +\w'Flanagan,J.L. (\f2\&Jim\fP\&)  'u +\w'june  'u",
	fill off;)

|environment(:document_save;
	;)

|environment(:document_restore;
	;)

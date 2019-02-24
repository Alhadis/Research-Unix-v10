|include(global.h)

|include(macro.h)

|DEFINE_STRING(name_string, N0)
|DEFINE_STRING(department_string, N1)

|DEFINE_NUMBER(position, P0)

|environment(name;
	divert.string name_string;)
	|comment<summary: name (name for titlebox and signature)>

|environment(department;
	divert.string department_string;)
	|comment<summary: department (department for titlebox)>

|environment(date;
	divert.string date_string;)
	|comment<summary: date (date for signature)>

|environment(titlebox;

	font bold, spacing on, goto .5i, blank.lines -1,
	title "``Employee's Report on Activities and Accomplishments``",
	blank.lines .5,
	line.length -.5i, page.offset +.25i,
	set string temporary_string "`",
	add string.from.string temporary_string name_string,
	add string temporary_string "``Department ",
	add string.from.string temporary_string department_string,
	add string temporary_string "`",
	title temporary_string;

	page.offset -.25i, line.length +.5i, minimum.lines 1)
	|comment<summary: titlebox (title box)>

|associate(p;
	minimum.lines .5, clear string temporary_string;;)
	|comment<summary: p (paragraph)>

|environment(signature;
	minimum.lines 3, indent 3i,
	line 3.0i, tab.stops 1.5iC, new.line, tab, string name_string,
	text ", ", string date_string, tab;)
	|comment<summary: signature (signature)>

|environment(:document;;)

|environment(:document_save;
	;)

|environment(:document_restore;
	;)

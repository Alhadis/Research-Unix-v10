|include(global.h)

|associate(style;;;

	[singers;;
		draft on, page.offset 0.125i, indent .75i,
		inline off, size 14, inline on, space 16])
	|comment<summary: style singers (full instructions)>

|environment(title;
	new.page, blank.lines 1, center on, font bold;

	minimum.lines 1)
	|comment<summary: title (title of the song)>

|environment(verse;
	minimum.lines .5, here on;)
	|comment<summary: verse (verse of the song)>

|environment(refrain;
	minimum.lines .5, indent +2, here on;

	minimum.lines .5)
	|comment<summary: refrain (refrain of the song)>

|environment(smaller;
	inline off, size -2, inline on, space -2;

	SPACE)
	|comment<summary: smaller (print the song a little smaller than normal)>

|environment(d;
|ifvalue draft off {
	divert.input on remember_string}
|ifvalue draft on {
	font italics};

|ifvalue draft on {
	font roman})
	|comment<summary: d (directive to the singers)>

|environment(m;
|ifvalue draft off {
	divert.input on remember_string}
|ifvalue draft on {
	indent.line -.75i};

|ifvalue draft on {
	horizontal.motion "|.75i"})
	|comment<summary: m (margin text)>

|environment(:document;
	font.family helvetica, inline off, size 22, inline on, space 24, fill off,
	page.offset 0.5i;)

|environment(:document_save;
	;)

|environment(:document_restore;
	;)

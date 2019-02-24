|include(macro.h)

|include(global.h)

|include(titlebox.h)
|include(titlebox.m)

|environment(titlebox;
	minimum.lines 2.75i;)
	|comment<summary: titlebox (produce title box here)>

|environment(:document;
	clear author_counter document_counter,
	line.length 5.25i,
	page.length 10.625i,
	inline off, size 12, inline on, space 14, reference.option rp;

	clear string temporary_string)

|comment<the above `clear string temporary_string' and the `include global.h'
	are a kludge to fix a problem troff has with files that don't end in \n>

|environment(:document_save;
	;)

|environment(:document_restore;
	;)

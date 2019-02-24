|include(global.h)

|include(macro.h)

|include(envir.h)
|include(envir.m)

|include(index.m)

|include(list.h)
|include(list.m)

|include(preproc.h)
|include(preproc.m)

|include(section.h)
|include(section.m)

|include(set.m)

|include(special.m)

|include(style.h)
|include(style.m)

|include(t_contents.m)

|include(whitespace.m)

|environment(:common;
	file.information,
	clear appendix_counter section_counter subsection_counter
		subsubsection_counter footnote_counter table_counter
		figure_counter reference_counter,
	number incr footnote_counter 1,
	number incr note_counter 1,
	if t, page.offset 1.0i;)

|environment(:common_newfile;
	|ifvalue verbatim off [
		file.information,
	];

	|ifvalue verbatim off [
		file.information,
	])

|environment(:common_save;
	;)

|environment(:common_restore;
	;)

	|comment<summary: insert (insert a file here)>
	|comment<summary: source (insert a file here without processing it)>
	|comment<summary: part (multi-part document)>
	|comment<summary: comment (comment out text)>

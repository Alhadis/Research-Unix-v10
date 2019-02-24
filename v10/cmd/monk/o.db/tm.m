|include(macro.h)

|include(titlebox.h)
|include(titlebox.m)

|include(envir.h)

|include(preproc.h)

|include(coversheet.h)
|include(coversheet.m)

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
	clear author_counter document_counter mercury_counter
		other_counter text_counter other_marker,
	clear string government_string proprietary_string;)

|environment(:document_save;
	;)

|environment(:document_restore;
	;)

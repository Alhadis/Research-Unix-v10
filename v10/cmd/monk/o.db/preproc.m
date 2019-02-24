|environment(reference;
	verbatim on, reference on;

	sawreference on)
	|comment<summary: reference (a reference in prefer)>

|environment(reference_database;
	verbatim on, reference database;)
	|comment<summary: reference_database (use this database file for prefer)>

|environment(reference_include;
	verbatim on, reference include;)
	|comment<summary: reference_include (include these reference in prefer)>

|environment(reference_list;
	verbatim on, reference list;)
	|comment<summary: reference_list (format these databases in prefer)>

|environment(reference_placement;
	|ifvalue sawreference on [
		new.page,
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

|environment(table;
	verbatim on, file.information, minimum.lines 1, line.spacing 1, adjust off,
	table on;

	minimum.lines 1)
	|comment<summary: table (a table in tbl)>

|environment(table_caption;
	divert.string temporary_string;

	incr table_counter, minimum.lines 1, center on, font bold,
	text "Table ", number table_counter, text ". ", font roman,
	string temporary_string, center off, minimum.lines 1,
	store string.from.number remember_string table_counter,
	contents table 1 remember_string temporary_string)
	|comment<summary: table_caption (the table's caption; IT)>

|environment(picture;
	verbatim on, file.information, minimum.lines 1, line.spacing 1, fill off, picture on;

	minimum.lines 1)
	|comment<summary: picture (a picture in pic)>

|environment(picture_caption;
	divert.string temporary_string;

	incr figure_counter, minimum.lines 1, center on, font bold,
	text "Figure ", number figure_counter, text ". ", font roman,
	string temporary_string, center off, minimum.lines 1,
	store string.from.number remember_string figure_counter,
	contents figure 1 remember_string temporary_string)
	|comment<summary: picture_caption (the picture's caption; IT)>

|environment(ped;
	verbatim on, file.information, minimum.lines 1, line.spacing 1, ped on;

	minimum.lines 1)
	|comment<summary: ped (a picture in ped)>

|environment(ped_caption;
	divert.string temporary_string;

	incr figure_counter, minimum.lines 1, center on, font bold,
	text "Figure ", number figure_counter, text ". ", font roman,
	string temporary_string, center off, minimum.lines 1,
	store string.from.number remember_string figure_counter,
	contents figure 1 remember_string temporary_string)
	|comment<summary: ped_caption (the ped's caption; IT)>

|environment(ideal;
	verbatim on, file.information, minimum.lines 1, line.spacing 1, ideal on;

	minimum.lines 1)
	|comment<summary: ideal (a picture in ideal)>

|environment(ideal_caption;
	divert.string temporary_string;

	incr figure_counter, minimum.lines 1, center on, font bold,
	text "Figure ", number figure_counter, text ". ", font roman,
	string temporary_string, center off, minimum.lines 1,
	store string.from.number remember_string figure_counter,
	contents figure 1 remember_string temporary_string)
	|comment<summary: ideal_caption (the ideal's caption; IT)>

|environment(equation;
	verbatim on, file.information, minimum.lines 1, line.spacing 1,
	equation on;

	clear string equation_string, minimum.lines 1)
	|comment<summary: equation (an equation in eqn)>

|environment(equation_caption;
	divert.string temporary_string;

	incr figure_counter, minimum.lines 1, center on, font bold,
	text "Figure ", number figure_counter, text ". ", font roman,
	string temporary_string, center off, minimum.lines 1,
	store string.from.number remember_string figure_counter,
	contents figure 1 remember_string temporary_string)
	|comment<summary: equation_caption (the equation's caption; IT)>

|environment(equation_number;
	divert.string equation_string;

	store string remember_string equation_string)
	|comment<summary: equation_number (the equation's number; IT)>

|environment(e;
	|ifvalue verbatim off [
		concatenate, file.information,
	]
	inline.equation on;)
	|comment<summary: e (an inline equation in eqn)>

|environment(graph;
	verbatim on, file.information, minimum.lines 1, line.spacing 1, graph on;

	minimum.lines 1)
	|comment<summary: graph (a graph in grap)>

|environment(graph_caption;
	divert.string temporary_string;

	incr figure_counter, minimum.lines 1, center on, font bold,
	text "Figure ", number figure_counter, text ". ", font roman,
	string temporary_string, center off, minimum.lines 1,
	store string.from.number remember_string figure_counter,
	contents figure 1 remember_string temporary_string)
	|comment<summary: graph_caption (the graph's caption; IT)>

|environment(bargraph;
	verbatim on, file.information, minimum.lines 1, line.spacing 1, bargraph on;

	minimum.lines 1)
	|comment<summary: bargraph (a bar graph in bar)>

|environment(bargraph_caption;
	divert.string temporary_string;

	incr figure_counter, minimum.lines 1, center on, font bold,
	text "Figure ", number figure_counter, text ". ", font roman,
	string temporary_string, center off, minimum.lines 1,
	store string.from.number remember_string figure_counter,
	contents figure 1 remember_string temporary_string)
	|comment<summary: bargraph_caption (the bar graph's caption; IT)>

|environment(save;
	verbatim on, citation save on;

	citation save off string remember_string)
	|comment<summary: save (save last important thing, shown as IT, in this label)>

|environment(savepage;
	verbatim on, citation save on;

	citation save off number page_counter)
	|comment<summary: savepage (save last page number in this label)>

|associate(savestring;;;
	[$name $string;
		set string temporary_string $string,
		citation save string $name temporary_string;])
	|comment<summary: savestring (save string under this name)>

|environment(remember;
	verbatim on, citation remember on;

	citation remember off)
	|comment<summary: remember (remember the important thing or page number saved in this label)>

|environment(index;
	index on;)
	|comment<summary: index (save this item and page number for an index)>

|environment(figure;
	verbatim on, file.information, minimum.lines 1, line.spacing 1, fill off;

	minimum.lines 1)
	|comment<summary: figure (a figure)>

|environment(figure_caption;
	divert.string temporary_string;

	incr figure_counter, minimum.lines 1, center on, font bold,
	text "Figure ", number figure_counter, text ". ", font roman,
	string temporary_string, center off, minimum.lines 1,
	store string.from.number remember_string figure_counter,
	contents figure 1 remember_string temporary_string)
	|comment<summary: figure_caption (the figures caption; IT)>

|environment(computeroutput;
	verbatim on, file.information, minimum.lines 1, line.spacing 1,
	fill off, size -1, space -1, computeroutput on;

	SPACE, minimum.lines 1)
	|comment<summary: computeroutput (computeroutput using cw)>

|environment(computeroutput_caption;
	divert.string temporary_string;

	incr figure_counter, minimum.lines 1, center on, font bold,
	text "Figure ", number figure_counter, text ". ", font roman,
	string temporary_string, center off, minimum.lines 1,
	store string.from.number remember_string figure_counter,
	contents figure 1 remember_string temporary_string)
	|comment<summary: computeroutput_caption (the computeroutput's caption; IT)>

|environment(ipa;
	|ifvalue verbatim off [
		concatenate, file.information,
	]
	ipa on;)
	|comment<summary: ipa (international phonetic alphabet)>

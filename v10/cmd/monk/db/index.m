|environment(index_placement;
	new.page, tab.stops \nWuR,
	set string temporary_string "INDEX",
	inline off, size +1, center on,
	font bold, string temporary_string, font roman,
	center off, size -1, inline on,
	minimum.lines,
	clear string remember_string,
	contents section 1 remember_string temporary_string;)
	|comment<summary: index_placement (put the index here)>

|environment(index_level0;
	indent .2i, indent.line -.2i, protect, text "\&";)
	|comment<summary: index_level0 (index program top level without sublevels)>

|environment(index_level1;
	indent .4i, indent.line -.4i, protect, text "\&";)
	|comment<summary: index_level1 (index program top level with sublevels)>

|environment(index_level2;
	indent .4i, indent.line -.2i, protect, text "\&";)
	|comment<summary: index_level2 (index program sublevel)>

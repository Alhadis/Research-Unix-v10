|environment(tc_placement;
	minimum.lines;)
	|comment<summary: tc_placement (put the table of contents here)>

|environment(tc_heading;
	new.page, tab.stops \nWuR,
	divert.string temporary_string;

	inline off, size +1, center on,
	font bold, string temporary_string, font roman,
	center off, size -1, inline on,
	minimum.lines,
	clear string remember_string,
	contents section 1 remember_string temporary_string)
	|comment<summary: tc_heading (tc program heading)>

|associate(tc_entry;;;
	[0 $sequence $text $page;
		minimum.lines, font bold,
		text $text, tab, text $page;]
	[1 $sequence $text $page;
		minimum.lines, font bold, text $sequence,
		horizontal.motion "|0.3i", text $text, tab, text $page;]
	[2 $sequence $text $page;
		minimum.lines 0,
		horizontal.motion "|0.3i", text $sequence,
		horizontal.motion "|0.6i", text $text, tab, text $page;]
	[3 $sequence $text $page;
		minimum.lines 0,
		horizontal.motion "|0.6i", text $sequence,
		horizontal.motion "|0.9i", text $text, tab, text $page;])
	|comment<summary: tc_entry (tc program entry)>

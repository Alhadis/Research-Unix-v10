|environment(bullet_list;
	list.type fixed, list.pre "\&", list.format 1, list.post "\s-2\(bu\s+2\ ",
	list.indent "nnn", list on;

	list off)
	|comment<summary: bullet_list (bulletted list using \s-2\(bu\s+2)>

|environment(dash_list;
	list.type fixed, list.pre "\&", list.format 1, list.post "\(em\ ",
	list.indent "nnn", list on;

	list off)
	|comment<summary: dash_list (dashed list using \(em)>

|environment(number_list;
	list.type calculated, list.pre "\&", list.format 1, list.post ".\ \ ",
	list.indent "nnnnn", list on;

	list off)
	|comment<summary: number_list (numbered list using 1.)>

|environment(p_number_list;
	list.type calculated, list.pre "(", list.format 1, list.post ")\ \ ",
	list.indent "nnnnn", list on;

	list off)
	|comment<summary: p_number_list (numbered list using (1))>

|environment(A_list;
	list.type calculated, list.pre "\&", list.format A, list.post ".\ \ ",
	list.indent "nnnnn", list on;

	list off)
	|comment<summary: A_list (alphabetized list using A.)>

|environment(a_list;
	list.type calculated, list.pre "\&", list.format a, list.post ")\ \ ",
	list.indent "nnnnn", list on;

	list off)
	|comment<summary: a_list (alphabetized list using a))>

|environment(p_a_list;
	list.type calculated, list.pre "(", list.format a, list.post ")\ \ ",
	list.indent "nnnnn", list on;

	list off)
	|comment<summary: p_a_list (alphabetized list using (a))>

|environment(I_list;
	list.type calculated, list.pre "\&", list.format I, list.post ".\ \ ",
	list.indent "nnnnn", list on;

	list off)
	|comment<summary: I_list (numbered list using I.)>

|environment(i_list;
	list.type calculated, list.pre "\&", list.format i, list.post ")\ \ ",
	list.indent "nnnnn", list on;

	list off)
	|comment<summary: i_list (numbered list using i))>

|environment(text_list;
	list.type fixed, list.pre "\&", list.format 1, list.post "\ ",
	list.indent "nnnnn", list on;

	list off)
	|comment<summary: text_list (text list using text from item))>

|associate(item;
	clear string temporary_string;

	|ifvalue list.spacing on [
		blank.lines .5,
	]
	|ifvalue list.type fixed [
		add string.from.string temporary_string premark_string,
		add string.from.string temporary_string postmark_string,
		store string remember_string postmark_string,
	]
	|ifvalue list.type calculated [
		incr list_counter,
		number format.from.string list_counter format_string,
		add string.from.string temporary_string premark_string,
		add string.from.number temporary_string list_counter,
		add string.from.string temporary_string postmark_string,
		store string remember_string premark_string,
		add string.from.number remember_string list_counter,
	]
	store width temporary_width indent_string,
	store width width_counter temporary_string,
	if else begin GT(width_counter, temporary_width),
		clear width_counter, sub width_counter temporary_width,
		indent.line number width_counter,
	if end,
	if else begin,
		clear temporary_width, sub temporary_width width_counter,
		indent.line number temporary_width,
	if end,
	string temporary_string, concatenate;
	[$;;
		set string temporary_string $])
	|comment<summary: item (item of a list; IT)>

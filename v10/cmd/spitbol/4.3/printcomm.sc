input (.in, 0, "spitv43.cmt")
lineno = 100
pageno = 0

while (line = in) {
	if (line ? fence && span(" ") &&
	    ("EJC" && (span(" ") | "") && rpos(0) |
	    "TTL" && span(" ") && rem . ttl))
		lineno = 100
	else {
		if (lineno > 60)
			newpage()
		lineno = lineno + 1
		output = line
	}
}

procedure newpage() {
	if (lineno > 1)
		eject()
	output = ""
	output = ""
	pageno = pageno + 1
	lineno = 1
	output = "Page " && rpad(pageno, 4) && "            " && ttl
	output = ""
}

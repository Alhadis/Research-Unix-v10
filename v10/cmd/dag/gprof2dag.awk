BEGIN			{
	total = -1.0;
	maxcolor = .65
	minsaturation = .3;

	print ".GS";
	print ".PS";
	print"/ShadedEllipse {";
	print"    /height exch def";
	print"    /width exch def";
	print"    /nodename exch def";
	print"    currentpoint            % save for label";
	print"    % distort user space";
	print"    gsave";
	print"        currentpoint translate";
	print"        0 0 moveto";
	print"        width height div 1 scale    % scale in x";
	print"        height 2 div 0 rmoveto";
	print"        0 0 height 2 div 0 360 arc closepath";
	print"        gsave";
	print"            fill";
	print"        grestore";
	print"        0 setgray stroke";
	print"    grestore";
	print"    moveto";
	print"    gsave";
	print"        0 setgray";
	print"        nodename width .9 mul height .9 mul daglabel";
	print"    grestore";
	print"} def";
	print"/ShadedEllipse_clip {Ellipse_clip} bind def";
	print".PE";
	printf ("draw nodes as ShadedEllipse color \"[%.2f %.2f 1]\";\n",maxcolor,minsaturation);

}

$1 ~ /\[/ && $0 !~ "as a"		{
					if ($5 == "start") total = $3 + $4;
					else {if (total <= 0.0) exit;}
					if (NF == 6) {
						tail = $5
						source = 1
					}
					else {
						tail = $6
						source = 0
					}
					first = 1

					while (getline) {
						if (NF == 0) {
							if (first == 1) {
								if (source == 1)
									print "node " tail ";"
							}
							else
								print ";"
							break
						}
						if ( $2 ~ /\./ ) {
							color = maxcolor * (1.0 - ($1 + $2)/total)

							if (first == 1) {
								first = 0
								printf ("edge from %s to %s color \"[%.2f 1 1]\"", tail, $4, color)
							}
							else  {
								printf (",\n\tto %s color \"[%.2f 1 1]\"", $4, color)
							}
							Indegree[$4] += 1;
						}
						else { 		# Recursive call
							if (first == 1) {
								first = 0
								printf ("edge from %s to %s ", tail, $2)
							}
							else {
								printf (",\n\tto %s", $2)
							}
						}
					}
				}

$1 == "flat" {
	while ($1 != "granularity:") getline;
	getline;
	getline;
	getline;
	getline;
	while (Indegree[$(NF - 1)] <= 0) getline;
	scale = $1;
	while (NF > 0) {
		if ($1 == "") exit;
		if ($(NF - 2) == "<cycle") func_name = $(NF - 3);
		else func_name = $(NF - 1);
		if (Indegree[func_name] > 0) {
			color = maxcolor * (1.0 - $1/scale);
			saturation = minsaturation + ((1.0 - minsaturation) * $1/scale);
			printf("draw %s color \"[%.2f %.2f 1]\";\n",func_name,color,saturation);
		}
		getline;
	}
	exit;
}

END				{ print ".GE" }

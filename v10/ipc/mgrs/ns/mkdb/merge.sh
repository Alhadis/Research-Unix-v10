echo merging $1 1>&2
sort -n | awk '
#
#	merge all lines with equivalent leading ids
#
BEGIN {
	line = ""; id = 0; last = ""
}

{
	#
	# skip duplicates
	#
	if($0==last)
		next;
	last = $0;
	if($1 != id){
		if(line!="")
			print line;
		id = $1;
		line = $2;
	} else
		line = line " " $2;
	for(i=3; i<=NF; i++)
		line = line " " $i;
}

END {
	if(line!="")
		print line;
}
'

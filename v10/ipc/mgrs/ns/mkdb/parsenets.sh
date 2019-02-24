echo Process $* 1>&2
cat $* | awk '
#
#	Parse networks file.  Output format is
#		<uid> INET,<number> <name> <name> ...
#	merge entries with equivalent internet numbers
#
BEGIN {
	notinet = "[^0-9.]";
}
#
# 	ignore blank and comment lines
#
$0 ~ /^#/	{next;}
NF>1 {
#	
#	check that internet address has only digits and '.'s
#
	if(match($2,notinet)) {
		print "Line " NR ": Illegal internet address " $1 | "cat 1>&2"
		next;
	}
#
#	output the line with an id #
#
	for(i=3; i<=NF; i++)
		line = line " " $i
	print "INET," $2 ".0 " line
}
'

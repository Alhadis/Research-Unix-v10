echo Process $* 1>&2
cat $* | awk '
#
#	Parse hosts file.  Output format is
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
	if(match($1,notinet)) {
		print "Line " NR ": Illegal internet address " $1 | "cat 1>&2"
		next;

	}
#
#	label domains 
#
	line = "INET," $1;
	for (i=2; i<=NF; i++)
		if(match($i, ".com")){
			line = line " DOMAIN," $i
		} else
			line = line " " $i
#
#	output the line with an id #
#
	print line
}
'

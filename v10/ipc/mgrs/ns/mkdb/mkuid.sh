echo "add unique id" 1>&2
cat $* | awk '
#
#	unique is an array that associates a numeric id with each
#	symbolic one
#
BEGIN {
	nextid=1;
}
#
#	add com/att to the front of a dk string to make a unique id
#
$1=="uucp" {
	line = $2 " " $2 ".att.com";	# uucp host name
	for (i=3; i<=NF; i++)
		if(match($i, "/")){
			n = split($i,a,"/");
			uid = a[n];
			for(j=n-1; j>=1; j--)
				uid = uid "." a[j];
			line = line " DK," $i " DOMAIN," uid ".att.com"
		} else
			line = line " " $i
	print line
	next;
}
#
#	invert order of domain components       to make a unique id
#
$1=="hosts" {
	line = $2;	# inet number
	for (i=3; i<=NF; i++)
		if(match($i, ".att.com")){
			line = line " DOMAIN," $i
		} else
			line = line " " $i
	print line
	next;
}
' | rmdups.sh

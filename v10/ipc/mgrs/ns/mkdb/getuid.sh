echo "merge on datakit and domain names" 1>&2
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
	id = 0
	for (i=2; i<=NF; i++)
		if(match($i, "/")){
			str = "com/att/" $i
			if(str in unique){
				id = unique[str];
			} else {
				print nextid " UNIQ," str
				id = nextid;
				nextid = nextid+1;
			}
			break;
		}
	if(id==0){
		id = nextid;
		nextid = nextid+1;
	}
	line = ""
	for (i=2; i<=NF; i++)
		if(match($i, "/")){
			str = "com/att/" $i
			unique[str] = id;
			line = line " DK," $i
		} else
			line = line " " $i
	print id " " line
	next;
}
#
#	invert order of domain components       to make a unique id
#
$1=="hosts" {
	id = 0
	for (i=2; i<=NF; i++)
		if(match($i, ".att.com")){
			n = split($i,a,".");
			str = a[n];
			for(j=n-1; j>=1; j--)
				str = str "/" a[j];
			if(str in unique){
				id = unique[str];
			} else {
				print nextid " UNIQ," str
				id = nextid;
				nextid = nextid+1;
			}
			break;
		}
	if(id==0){
		id = nextid;
		nextid = nextid+1;
	}
	line = ""
	for (i=2; i<=NF; i++)
		if(match($i, ".att.com")){
			n = split($i,a,".");
			str = a[n];
			for(j=n-1; j>=1; j--)
				str = str "/" a[j];
			unique[str] = id;
			line = line " DOMAIN," $i
		} else
			line = line " " $i
	print id " " line
	next;
}
' | merge.sh | rmdups.sh

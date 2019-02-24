echo Making index 1>&2
cat $* | awk '
#
#	make index of the form:
#		value,K|D|I|N|T
#
BEGIN { nc = 0; }
{
	for(i = 1; i <= NF; i++) {
		if(match($i,",")) {
			n = RSTART +1;
			if(match($i,"^DK"))
				type = "K";
			else if(match($i,"^SERVICE"))
				continue;
			else 
				type = substr($i, 1, 1);
			printf("%s,%s %d\n",substr($i,n),type,nc);
		} else {
			
			printf("%s %d\n", $i,nc);
		}
	}
	nc = nc + length($0) + 1;
}

' | sort -u

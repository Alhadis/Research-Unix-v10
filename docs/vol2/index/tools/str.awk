awk '
/^\.(ig|de)/	{
	print $0
	while(getline && $0 !~ /^\.\./)
		print $0
	print $0
	next
	}
/^\.ds/	{
	print $0
	if($0 ~ /\&/)gsub(/\&/,"\\\\&")
	if($0 ~ /\\\"/)sub(/\\\".*/,"")
	if(length($1) >3){
		name=substr($1,4,length($1)-3)
		start=2
	}
	else{ name=$2
		start=3
	}
	if(length(name) == 1){
		pat1[name]=$start
		if(NF > start)
			for(i=start+1;i<=NF;i++)
				pat1[name]=pat1[name] " " $i
	}
	else{
		if(name ~ /\(/)sub(/\(/,"\\\(",name)
		pat2[name]=$start
		if(NF > start)
			for(i=start+1;i<=NF;i++)
				pat2[name]=pat2[name] " " $i
	}
	next
	}
$0 ~ /\\\*/	{
	gsub(/\\\*\(/,"foo2")
	gsub(/\\\*/,"foo1")
	if($0 ~ "foo2")
		for(i in pat2){
			str="foo2" i
			if($0 ~ str){
				gsub(str,pat2[i])
			}
		}
	if($0 ~ /foo/){
		for(i in pat1){
			str="foo1" i
			if($0 ~ str){
				gsub(str,pat1[i])
			}
		}
	}
	print $0
	next
}
/.*/	{print $0}
' $1

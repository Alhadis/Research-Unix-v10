BEGIN	{
	longest = 0
	overf = 0
	i = 1
	}
/^%/	{
	total += length($0)+1
	next
	}
/\\def/	{
	total += length($0)+1
	next
	}
/\\title/	{
	place[i] = total
	total += length($0) + 1
	gettitl($0)
	text[i] = title
	hsize += length(title)+1
	i++
	l = length(title)
	if(longest < l+1)
		longest = l + 1
	if(l > 23){
		while(l>=23){
			overf++;
			l -= 23
		}
	}
	next
	}
/\\([Ss]ection|sub.*section|newsection|chapter)[^a-zA-Z]/	{
	place[i] = total
	if($0 ~ /{/ && $0 ~ /}/){
		line=$0
		op=gsub("{","",line)
		cl=gsub("}","",line)
		n=index($0,"{") +1
		if(op == cl && cl>1){
			sub("}","")
			sub(/{\\tt/,"")
		}
		heading = substr($0, n, index($0,"}")-n)
	}
	else if($0 ~ /{/){
		total += length($0) + 1
		nlines = 0
		n=index($0, "{")
		heading = substr($0, n+1, length($0)-n)
		while(getline ){
			if(nlines++ > 3){
				print "too many lines in heading:" heading |"cat 1>&2"
				break
			}
			total += length($0) +1
			if($0 ~ /}/){
				heading = heading " " substr($0, 1, index($0,"}")-1)
				break
			}
			else
				heading = heading " " $0
		}
	}
	total += length($0) +1
	text[i] = heading
	hsize += length(heading)+1
	l = length(heading)
	if(longest < l+1)
		longest = l + 1
	if(l > 23){
		while(l>=23){
			overf++;
			l -= 23
		}
	}
	heading = ""
	i++
	next
	}
/.*/	{
	total += length($0) + 1
	}
END	{
	if(i > 1){
#		print hsize, longest, i-1, overf
		for(n=1;n<i;n++){
#			print place[n]
			print text[n]
		}
	}
	}
function gettitl(A){
	if(A ~ "%"){
		split(A,z,"%")
		a=z[1]
	}
	else a=A
	if(a ~ /\\thanks/){
		if(a ~ /}/)
			sub(/\\thanks{.*}/," ",a)
		else{
			thanks=1
			sub(/\\thanks.*/, " ",a)
		}
	}
	left=gsub(/{/," ",a)
	right = gsub(/}/," ",a)
	if(thanks)left += 1
	if(a ~ /\\[a-z][a-z]* /)gsub(/\\[a-z][a-z]* /," ",a)
	if(a ~ /\\[^a-z]/)gsub(/\\[^a-z]/," ",a)
	title = a
	while(left != right){
		if((getline)<1){print "ran out of file"
			exit
		}
		total += length($0) +1
		if($0 ~ "%"){
			split($0,z,"%")
			if(length(z[1]) == 0)continue
			a = z[1]
		}
		else a = $0
		if(thanks){
			if(a ~ /}/){
				thanks=0
				right += 1
				sub(/}/,"XXX",a)
				sub(/.*XXX/," ",a)
			}
			else continue
		}
		if(a ~ /\\thanks/){
			b=a
			left += gsub(/{/," ",b)
			right += gsub(/}/," ",b)
			if(index(/}/,a)){
				gsub(/\\thanks.*}/," ",a)
				title = title " " a
				continue
			}
			thanks=1
			continue
		}
		left+=gsub(/{/," ",a)
		right += gsub(/}/," ",a)
		if(a ~ /\\[a-z][a-z]* /)gsub(/\\[a-z][a-z]* /," ",a)
		if(a ~ /\\[^a-z]/)gsub(/\\[^a-z]/," ",a)
		title = title " " a
	}
	gsub(/[ ][ ]*/," ",title)
	sub(/^ /,"", title)
}

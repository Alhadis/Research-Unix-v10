BEGIN	{
	longest = 0
	overf = 0
	i = 1
	}
(/^\.sh/&& NF == 1)||
/^\.(([NS]H)|(TL)|(TI))/	{
	place[i] = total
	if($0 ~ /SH/ && NF > 1|| $0 ~ /NH/ && (NF > 2 || $2 ~ /[A-Za-z]/)){
		if($0 ~ /"/){
			n = split($0,x,"\"")
			heading = x[2]
		}
		else heading = $2
		hsize += length(heading) + 1
	}
	else {
	total += length($0) + 1
	nlines = 0
	while(getline){
		if(nlines++ > 3){
			print "too many lines in heading:" heading |"cat 1>&2"
			break
		}
		total += length($0) +1
		hsize += length($0) +1
		if($0 ~ /^\.UX/){
			nlines--
			heading = heading " UNIX"
			hsize += 1
		}
		else if($0 ~ /^\./){
			if($1 ~ /^\.P/ || $1 ~ /^\.LP/|| $1 ~ /^\.[AT]/)break
			if(heading == "")heading = $2
			else heading = heading " " $2
			if(NF > 2)for(n=3;n<=NF;n++)
				heading = heading " " $n
		}
		else if(heading == "")
			heading = $0
		else
			heading = heading " " $0
	}
	}
	total += length($0) +1
	text[i] = heading
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
/^\.(H[U ]|[su]h)/	{
	place[i] = total
	total += length($0) + 1
	if($0 ~ /"/){
		n = split($0, hd, "\"")
		if(n == 3)
			heading = hd[2]
		else{
			heading = hd[2]
			print "unbalanced quotes: " $0 >"err.hd"
		}
	}
	else if($1 ~ /HU|uh/)heading = $2
	else  heading = $3
	text[i] = heading
	l = length(heading)
	hsize += l + 1
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
/^\.de/	{
	total += length($0) +1
	while(getline && $0 !~ /^\.\.$/){
		total += length($0) +1
	}
	total += length($0) +1
	next
	}
/.*/	{
	total += length($0) + 1
	}
END	{
	if(i > 1){
#		print hsize " " longest " " i-1 " " overf
		for(n=1;n<i;n++){
#			print place[n]
			print text[n]
		}
	}
	}

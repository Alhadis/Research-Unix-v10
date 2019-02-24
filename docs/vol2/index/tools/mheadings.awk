BEGIN	{
	monkdel = "\(\)\[\]<>{\}\"\"\'\'\`\'"
	longest = 0
	overf = 0
	i = 1
	havetitle=0
	}
/^\|title[^a-z_]/	{
	place[i] = total
	total += length($0) + 1
	if(havetitle)next
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
	havetitle=1
	next
	}
/\|(section|subsection|subsubsection|unnumbered_section|unnumbered_subsection)/	{
	place[i] = total
	save=$0
	heading = ""
	rdelim=getdel()
	if(pldelim == prdelim)getequal()
	else getpair()
	total += length(save) +1
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
function gettitl(	rdelim,n,m,junk){
	rdelim = getdel()
	title=$0
	n=gsub(ldelim,"")
	junk=$0
	m=gsub(rdelim,"",junk)
	if($0 !~ rdelim || n != m-1){
		while(getline > 0){
			total += length($0) +1
			title=title " " $0
			n=sub(ldelim,"")
			if($0 ~ rdelim && n == 0)break
		}
	}
	title=substr(title,1,length(title)-1)
}
function getdel(){
	if($0 !~ /^\|/ && $0 ~ /..*\|/)sub(/^..*\|/,"\|")
	sub(/\|[a-z_ \t]*/,"")
	pldelim = substr($0, 0, 1)
	sub(/./,"")
	b = substr(monkdel,index(monkdel,pldelim)+1,1)
	prdelim = b
	ldelim = "\\" pldelim
	return("\\" b)
}
function getpair(){
	lsave=$0
	n = xn = gsub(ldelim,":")
	ind = index(lsave,pldelim)
	m = xm = gsub(rdelim,":")
	imd = index(lsave,prdelim)
	if(m>0 && n+1 == m){
		heading = substr(lsave, 1, index(lsave,prdelim)-1)
		return
	}
	total += length(save) + 1
	nlines = 0
	heading = $0
	while(getline ){
		save=$0
		if(nlines++ > 3){
		   print "too many lines in heading:" heading |"cat 1>&2"
			break
		}
		lsave=$0
		n=gsub(ldelim,"X")
		xn += n
		ind = index(lsave,pldelim)
		m=gsub(rdelim,"Z")
		xm += m
		imd = index(lsave,prdelim)
		if(m>0 && xn+1 == xm){
			if(ind > imd && n+1 == m){
			if(heading == "")
			   heading=substr(lsave, 1, index(lsave,prdelim)-1)
			else
			heading=heading " " substr(lsave,1,index(lsave,prdelim)-1)
			}
			else{
				new=getind()
				if(heading == "")heading=substr(lsave, 1,new-1)
				else heading = heading " " substr(lsave, 1,new-1)
			}
			break
		}
		else{
			total += length(lsave) +1
			if(heading == "")heading = lsave
			else heading = heading " " lsave
		}
	}
}
function getind(){
	split($0,foo,"Z")
	sum=0
	for(j=1;j<=m;j++)
		sum += length(foo[j])+1
	return(sum)
}
function getequal(){
	cmd= "\\|[a-zA-Z_]*" prdelim
	lsave=$0
	n = xn = gsub(ldelim,":")
	if(n==1){
		heading = substr(lsave, 1, index(lsave,prdelim)-1)
		return
	}
	total += length(save) + 1
	heading = $0
	while(getline){
		save=lsave=$0
		ind=0
		if($0 ~ cmd){
			while($0 ~ cmd)
				ind=getcmd()
		}
		nind=index($0,pldelim)
		n=gsub(ldelim,":")
		xn += n
		if(xn == 1){
			if(heading == "")heading=substr(save,1,nind+ind-1)
			else heading=heading " " substr(save,1,nind+ind-1)
			break
		}
		else{
			total += length(lsave) +1
			if(heading == "")heading = lsave
			else heading = heading " " lsave
		}
	}
}
function getcmd(	i,nsv,sum){
	sum=length($0)
	sub(cmd,"")
	i=sub(ldelim,"")
	sum=sum-length($0)
	if(!i){
		total += length(lsave) +1
		if(heading == "")heading = lsave
		else heading = heading " " lsave
		getline
		save=lsave=$0
		sub(ldelim,"")
	}
	return(sum)
}

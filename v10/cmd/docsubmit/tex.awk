BEGIN	{
	if(texinp != "")
		ninputs = split(texinp, texinputs, ":")
	else {
		ninputs = 1
		texinputs[1] = "."
	}
	idef=0
	}
/\\newcommand/	{
	ckdef(0, $0, 1)
	next
	}
/\\def[^a-z]/	{
	ckdef(0, $0, 0)
	next
	}
/\\(input|include)[{ ]/	{
	getfile()
	next
	}
/\\bibliography[{ ]/	{
	n = substr($0, index($0,"{")+1, index($0,"}")-index($0,"{")-1)
	if(n ~ ","){
		num=split(n, b, ",")
		for(i=1;i<=num;i++)
			system("cp " b[i] ".bbl " dir)
			close("cp " b[i] ".bbl " dir)
	}
	else{
		system("cp " n ".bbl " dir)
		close("cp " n ".bbl " dir)
	}
	}
/^\\includepicture/	{
	n=index($0,"{")+1
	fname=substr($0,n,index($0,"}")-n)
	system ("cp " fname ".t " dir )
	close ("cp " fname ".t " dir )
	system ("cp " fname ".tex " dir )
	close ("cp " fname ".tex " dir )
	}
/\\special/	{
	getpost($0)
	next
	}
/\\/	{
	isdef($0)
	next
	}
/.*/	{
	print $0
	}
function getfile(	saveline, n, bname, tname, j, name, flag){
	saveline = $0
	if($0 ~ /\\input/)n = index($0,"\\input")+7
	else n=index($0,"\\include")+9
	bname = substr($0,n)
	gsub(/^[ {]*/,"",bname)
	gsub(/[ }].*/,"",bname)
	gsub(/[ }]$/,"",bname)
	tname = bname ".tex"
	for(j=1; j<= ninputs; j++){
		name = texinputs[j] "/" tname
		if((getline <name) >=0)break
		name = texinputs[j] "/" bname
		if((getline <name) >=0)break
		name = ""
	}
	if(name == ""){
		print "cannot open file " bname " or " tname | "cat 1>&2"
		return
	}
	if(name ~ /\/usr\/lib\/tex\/macros/){
		print saveline
		return
	}
	flag=0
	while(1){
		if(flag)if((getline <name)<1)break
		flag=1
		if($0 ~ /\\newcommand/){
			ckdef(name, $0, 1)
			continue
		}
		if($0 ~ /\\def[^a-z]/){
			ckdef(name, $0, 0)
			continue
		}
		if($0 ~ /\\(input|include)[{ ]/){
			getfile()
		}
		else if($0 ~ /\\special/)getpost($0)
		else if($0 ~ /\\/){
			isdef($0)
		}
		else print $0
	}
	close(name)
}
function getpost(str){
	if(str ~ /psfile/){
		n=substr(str,index(str,"psfile"))
		if(n ~ / /)sub(/ .*/, "", n)
		if(n ~ /\=/)sub(/.*=/, "", n)
		if(n ~ /,$/)sub(/,/, "", n)
		if(n ~ /,.*/)sub(/,.*/, "", n)
		system( "cp " n " " dir)
		close( "cp " n " " dir)
		if(n ~ /\//){
			num = split(n, xa, /\//)
			sub(n, xa[num], str)
		}
		print str
	}
	else print str
	}
function ckdef(file, first,new){
	print first
	if(new){
		n=index(first,"{")+1
		name=substr(first,n,index(first,"}")-n)
		name = "\\" name
		n=index(first,"[")+1
		if(n > 1)
			args=substr(first,n,index(first,"]")-n)+0
		else args=0
	}
	else {
		sub(/\\def\\/,"")
		savs = first
		if(first ~ /^[A-Za-z]/){
			abc=first
			sub(/[^A-Za-z]/,"{",abc)
			name = "\\\\" substr(first,0,index(abc,"{")-1)
		}
		else name = "\\\\" substr(first,0,1)
		if(savs ~ /#/){
			if(savs ~ "{")gsub(/{.*/, "",savs)
			n=split(savs, z, "#")
			args = n-1
		}
		else args=0
		xdef=1
	}
	if(first ~ "%"){
		n=split(first, z, "%")
		save = z[1]
		left=gsub(/{/,":",z[1])
		right=gsub(/}/,";",z[1])
	}
	else{
		save = $0
		left=gsub(/{/,":")
		right=gsub(/}/,";")
	}
	psav = isav = 0 
	if($0 ~ /special/){
		getpattern(file, save)
		psav++
	}
	while(left != right || (left == 1 && !xdef)){
		if(file== 0){
			if((getline) <1){
				print "ran out of file in newcommand"|"cat 1>&2"
				exit
			}
		}
		else if((getline<file) < 1){
				print "ran out of file in newcommand"|"cat 1>&2"
				exit
			}
		save=$0
		left +=gsub(/{/,":")
		right+=gsub(/}/,";")
		if($0 ~ /special/){
			getpattern(file, save)
			psav++
		}
		if($0 ~ /(input|include)/){
			getpattern(file, save)
			isav++
		}
		else print save
	}
	if(psav||isav){
		argct[idef] = args
		if(psave)post[idef] = psav
		else inp[idef]=isav
		def[idef++] = name
	}

}
function getpattern(file, command){
	save = command
	if(command ~ /psfile/){
		n=substr(command, index(command, "psfile"))
		sub(/.*=[ \t]*/,"",n)
		sub(/}*$/,"",n)
		sub(/ .*/,"",n)
		ckargs(n,save)
	}
	if(command ~ /(input|include)/){
		if(command ~ /\\input/)n = index(command,"\\input")+7
		else n=index(command,"\\include")+9
		bname = substr(command,n)
		gsub(/^[ ]*/,"",bname)
		gsub(/[ }].*/,"",bname)
		gsub(/[ }]$/,"",bname)
		ckargs(bname,save)
	}
}
function ckargs(pat,command){
	if(pat ~ /#/)
		num = substr(pat,index(pat,"#")+1,1)
	else num = 0
	pattern[idef,num] = pat
	if(command ~ /\//){
		num = split(pat, xa, /\//)
		sub(n, xa[num], command)
	}
	print command
}
function cpfile(name, pat){
	if(name ~ "}")
		sub(/}.*/,"", name)
	sub(/#./,name,pat)
	system("cp " pat " " dir)
	close("cp " pat " " dir)
	if(name ~ /\//){
		num = split(name, xa, /\//)
		sub(name, xa[num],string)
	}
}
function isdef(string,	i, gotarg, a, x, j, l, r){
	if(idef){
		for(i=0;i<idef;i++){
			if(string !~ def[i])continue
			if((i,0) in pattern){
				system("cp " pattern[idef,0] " " dir)
				close("cp " pattern[idef,0] " " dir)
				break
			}
			gotarg=0
			a=string
			while(gotarg<argct[i]){
				x=0
				if(j=index(a,"{")){
					x++
					a = substr(a,j+1)
					if((i,gotarg+1) in pattern)
						cpfile(a, pattern[i,gotarg+1])
					while(x){
						l=index(a,"{")
						r=index(a,"}")
						if(!l && !r){
							getline
							string = string "\n" $0
							a=$0
							continue
						}
						if(l<r && l>0){
							x++
							a=substr(a,l+1)
						}
						else{
							a=substr(a,r+1)
							x--
						}
					}
					gotarg++
				}
				else {
					getline
					string = string "\n" $0
					a = $0
				}
			}
			break
		}
	}
	print string
}

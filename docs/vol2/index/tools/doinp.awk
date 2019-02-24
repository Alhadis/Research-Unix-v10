BEGIN	{
	if(texinp != "")
		ninputs = split(texinp, texinputs, ":")
	else {
		ninputs = 1
		texinputs[1] = "."
	}
print "ninputs " ninputs|"cat 1>&2"
	idef=0
	}
/\\(input|include)[{ ]/	{
	getfile("", "")
	next
	}
/.*/	{
	print $0
	}
function getfile(fname, oname,	saveline, n, bname, tname, j, name, flag){
	if(fname == ""){
		saveline = $0
		if($0 ~ /\\input/)n = index($0,"\\input")+7
		else n=index($0,"\\include")+9
		bname = substr($0,n)
		gsub(/^[ {]*/,"",bname)
		gsub(/[ }].*/,"",bname)
		gsub(/[ }]$/,"",bname)
		tname = bname ".tex"
	}
	else {
		bname = fname
		tname = fname
	}
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
	if(name ~ /\/usr\/lib\/tex\/macros/|| name ~ /macros.tex/){
		if(oname == "")print saveline
		else print saveline >oname
		return
	}
	flag=0
	while(1){
		if(flag)if((getline <name)<1)break
		flag=1
		print $0
	}
	close(name)
}

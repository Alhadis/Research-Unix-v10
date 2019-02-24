BEGIN	{seen[""] = 0
	monkdel = "\(\)\[\]<>{\}\"\"\'\'\`\'"
	EQstr="(^\\.EQ)|(\\|begin *[(<[{'\"\`]equation *)|(\\|equation[^_])"
	ENstr="(^\\.EN)|(\\|end *[(<[{'\"\`]equation)"
	TSstr="(^\\.TS)|(\\|begin *[(<[{'\"\`]table *)|(\\|table[^_])"
	TEstr="(^\\.TE)|(\\|end *[(<[{'\"\`]table)"
	GSstr="(^\\.GS)|(\\|begin *[(<[{'\"\`]ped *)|(\\|ped[^_])"
	GEstr="(^\\.GE)|(\\|end *[(<[{'\"\`]ped)"
	PSstr="(^\\.(PS|G1))|(\\|begin *[(<[{'\"\`](picture|graph) *)|(\\|(picture|graph)[^_])"
	PEstr="(^\\.(P[EF]|G2))|(\\|end *[(<[{'\"\`](picture|graph))"
	ISstr="(^\\.IS)|(\\|begin *[(<[{'\"\`]ideal *)|(\\|ideal[^_])"
	IEstr="(^\\.I[EF])|(\\|end *[(<[{'\"\`]ideal)"
	picoutput = "This paper appears to have pic output included.\nWe do not accept papers with pic output.\nPlease resubmit it with the pic input instead.\nIf the pic input is in separate files, you can include it by saying:\n.PS <picfile"
	tbloutput = "This paper appears to have tbl output included.\nWe do not accept papers with tbl output.\nPlease resubmit it with the tbl input instead.\n"
	tpedoutput = "This paper appears to have tped output included.\nWe do not accept papers with tped output.\nPlease resubmit it with the tped input instead.\n"
	intbl = ineq = tped = 0
	ndelim=""
	macs=".BM .BP .BI"
	n=split(macs,arr," ")
	for(i=1;i<=n;i++)
		incmacro[arr[i]]=1
	}
/^\.ig/	{
	while(getline>0 && $0 !~ /^\.\./);
	next
	}
#			inmacro seeded with .BM .BP .PI also has
#			macros including picture macro, try to open all args
/^\./	{
	if(($1 in incmacro) && (NF > 1)){
		tryall()
		next
	}
	}
#			.so - get input file
#			or .de that includes .so - try to get file as arg2
/^\.so/ || $1 in hasso	{
	getfile($2)
	next
	}
/\|comment/	{
	comment("")
	next
	}
/\|(insert|source)/	{
	mgetfile()
	next
	}
/\|author/	{
	author("")
	next
	}
/\|title[^a-z_]/	{
	title("")
	next
	}
#			bwk macro
/^\.ge/	{
	system( "trget " NF " " $2 " " $3 " " $4 " " $5 )
	close( "trget " NF " " $2 " " $3 " " $4 " " $5 )
	next
	}
#			bwk macro
/^\.ru/	{
	system( $2 " " $3 " " $4 " " $5 " " $6 " " $7 " " $8 " " $9 )
	close( $2 " " $3 " " $4 " " $5 " " $6 " " $7 " " $8 " " $9 )
	next
	}
#			inline .sy - execute
/^\.sy/	{
	system( $2 " " $3 " " $4 " " $5 " " $6 " " $7 " " $8 " " $9 )
	close( $2 " " $3 " " $4 " " $5 " " $6 " " $7 " " $8 " " $9 )
	next
	}
/^\.bm/	{
	next
	}
#			switch in and out of eqn
$0 ~ EQstr	{ ineq = 1
		print $0
		if($0 !~ /^\./ && $0 !~ /\|begin/)
			ndelim=getdel()
		next
		}
$0 ~ ENstr	{ ineq = 0}
#			switch in and out of tbl
$0 ~ TSstr	{ intbl = 1
		print $0
		if($0 !~ /^\./ && $0 !~ /\|begin/)
			ndelim=getdel()
		next
		}
$0 ~ TEstr	{ intbl = 0}
#			all tbl output has this - unlikely otherwise - reject
/^\.de 35/	{
	if(intbl){
		print tbloutput|"cat 1>&2"
		exit 1
	}
	}
#			save eqn delimiters - # bad for tbl, pic - warn
/^[ ]*delim[ ]*/	{
	if(ineq){
		if($2 ~ /#/)
		print "eqn delimiters in " $0 " will break tbl, pic etc"|"cat 1>&2"
		delim = $0
	}
	}
#			macro definition - check for embedded picture macros
#			bitmaps, or .so
/^\.(de|am)/	{
	if($0 ~ /IS/)
		print ".IS is defined in " file " check it for ideal"|"cat 1>&2"
	print $0
	xname = $2
	while((getline) > 0){
		if($0 ~ /^\.ig/){
			while((getline) >0 && $0 !~ /^\.\./);
			continue
		}
		if($0 ~ /^\.\./)break
		if($0 ~ /^\.(BM|BP|PI)/)incmacro["." xname]=1
		if($0 ~ /^\.so/){
			hasso["." xname] = 1
			print "The .so embedded in macro " xname " may cause files not to be shipped " | "cat 1>&2"
			defso = 1
		}
		print $0
	}
	}
#			.PS <file
#			check if file contains .PS
#			replace .PS <file with file (and .PS in needed)
#			copy other files if copy thru included
/^\.PS[ \t]*</	{
	gsub(/ /,"")
	a=substr($0,index($0,"<")+1)
	if((getline<a) >0){
		pflag=0
		start=1
		if($0 ~ /^\./){
			print $0
			pflag=1
			if($0 !~ /^\.PS/){
				while(getline<a > 0){
					if($0 !~ /^\./)break
					print $0
					if($0 ~ /^\.PS/)pflag=0
				}
				if(pflag)print ".PS"
				start=0
			}
		}
		else{ print ".PS"
			start=0
		}
		pflag = getpic(a,start,0,"")
		if(pflag)print ".PE"
		close(a)
	}
	else {
		print "couldn't open pic file:" a|"cat 1>&2"
		print "from line: " $0|"cat 1>&2"
		exit 1
	}
	next
	}
#			tped .GS may name file to include
#			if so, copy file; replace full path with file name
#			else toggle tped flag
$0 ~ GSstr	{
	if($0 ~ /^\.GS/ && NF > 1){
		for(t=2; t<=NF;t++){
			if($t !~ "-" && $t !~ "="){
				system( "cp " $t " " dir )
				close( "cp " $t " " dir )
				if($0 ~ "/"){
					nstat=split($0,stat,"/")
					cmd = ""
					for(xx=1;xx<t;xx++)
						cmd= cmd $xx " "
					print cmd stat[nstat]
				}
				else print $0
				next
			}
		}
	}
	intped=1
	print $0
	if($0 !~ /^\./ && $0 !~ /\|begin/)
		ndelim=getdel()
	next
	}
$0 ~ GEstr	{
	intped=0
	}
#			.sp -1 clue of included tped output
/^\.sp -1/	{
	if(intped){
		print tpedout|"cat 1>&2"
		exit 1
	}
	}
#			grap/pic may include file
$0 ~ PSstr	{
	getpic("",0,1,"")
	next
	}
#			ideal - may include file
$0 ~ ISstr	{
	print $0
	if($0 !~ /^\./ && $0 !~ /\|begin/)
		idelim=getdel()
	else idelim=IEstr
	while(getline){
		if($0 ~ idelim){
			print $0
			next
		}
		else if($0 ~ /^[\.]+include/)
			doinclude($1, $2, 0)
		else print $0
	}
	}
#			macro called that includes .so probably not included
/^\./	{
	if($1 in hasso)
		print $2 "with embedded .so called"| "cat 1>&2"
	}
/.*/	{
	if(ndelim != "")
		if($0 ~ ndelim){
			ineq=intbl=0
			ndelim=""
		}
	print $0
	}
function havecopy(cmd, file, i, ofile,	quotes,n,rname,NAME,j){
	if(file ~ /"/){
		quotes=1
		n = split(file, name, "\"")
		if(n>1)rname = name[2]
	}
	else {
		rname = file
		quotes=0
	}
	if(rname ~ "/"){
		n = split(rname,nn, "/")
		NAME=nn[n]
	}
	else NAME=rname
	if(i > 2)for(j=2;j<i;j++)cmd = cmd " " $j
	if(quotes)cmd = cmd " \"" NAME "\""
	else cmd = cmd " " NAME
	if(NF >= i+1)
		for(j=i+1; j<=NF; j++)cmd = cmd " " $j
	if(ofile != "")print cmd >>ofile
	else print cmd
	if(rname in seen)return
	getpic(rname,1,0,dir "/" NAME)
	close(rname)
	close(dir "/" NAME)
	seen[rname]=1
}
function tryall(	cmd,i,tryit,found){
	cmd=$1
	found=0
	for(i=2;i<=NF;i++){
		if((getline tryit <$i)>0){
			found=1
			cmd= cmd " " getcopy($i)
		}
		else cmd = cmd " " $i
		close($i)
	}
	print cmd
	if(!found){
		print "no files found in " cmd|"cat 1>&2"
	}
}
function getcopy(file,	rname,quotes,n,NAME){
	rname=file
	quotes=0
	if(file ~ /"/){
		quotes=1
		n=split(file,name,"\"")
		if(n>1)rname=name[2]
	}
	system( "cp " rname " " dir )
	close( "cp " rname " " dir )
	if(rname ~ "/"){
		n = split(rname,nn, "/")
		NAME=nn[n]
	}
	else NAME=rname
	if(quotes)return("\"" NAME "\"")
	else return(NAME)
}
function getfile(file,	File, eflag,first,indef,xname,fintbl,finpic,saveline){
	if(file ~ /\/usr\/lib\//|| file ~ /\/exptools\//){
		print $0
		return
	}
	saveline=$0
	if((getline <file) > 0){
		if($0 ~ /@\(#\)dcs:[cC][Ss]macros/){
			print saveline
			return
		}
		if($0 ~ /^\.lf/){
			File=$NF
			saveline=$0
			if((getline <File)<=0){
			print "This paper contains preprocessor output evidenced by  " saveline| "cat 1>&2"
			print "and I can't find the source file " File|"cat 1>&2"
			while((getline < file) >0 && $0 ~ /^\.lf/)
				print $0| "cat 1>&2"
			print "paper not submitted" | "cat 1>&2"
			exit 1
			}
		}
		else File = file
	}
	else {
		print "cannot open file " file | "cat 1>&2"
		return
	}
	if(File in seen){
		close(File)
		return
	}
	if(delim != "" && !ismonk){
		print ".EQ\ndelim off\n.EN"
		eflag=1
	}
	seen[File] = 1
	first=1
	while(1){
		if(first==0)if((getline <File)<=0)break
		first = 0
		if($0 ~ /^\.ig/){
			while((getline<File)>0 && $0 !~ /^\.\./);
			continue
		}
		if($0 ~ /^\.(de|am)/){
			if(fintbl && $0 ~ /^\.de 35/){
				print tbloutput|"cat 1>&2"
		print "The tbl output is in the .so file named:" File|"cat 1>&2"
				exit 1
			}
			indef=1
			xname = $2
		}
		if(indef && $0 ~ /^\.(BM|BP|PI)/)incmacro["." xname]=1
		if($0 ~ /^\.\.$/)indef=0
		if($0 ~ /^\.sy/ && indef == 0){
	print "the .sy commands in this paper will not be handled" indef|"cat 1>&2"
			if(debug ~ /-d/){
				print $0
				continue
			}
			print "paper not submitted" | "cat 1>&2"
			exit 1
		}
		if($0 ~ TSstr)fintbl=1
		if($0 ~ TEstr)fintbl=0
		if($0 ~ PSstr)getpic(File,0,1,"")
		else if($0 ~ /\|comment/)comment(File)
		else if($0 ~ /\|(insert|source)/)mgetfile()
		else if($0 ~ /\|author/)author(File)
		else if($0 ~ /\|title[^a-z_]/)title(File)
		else if(($1 in incmacro)&& (NF > 1)&&!indef)
			tryall()
		else if($0 ~ /^\.so/){
			if(!indef)getfile($2)
			else hasso["." xname]=1
		}
		else print $0
	}
	close(File)
	if(eflag){
		print ".EQ\n" delim "\n.EN"
		eflat = 0
	}
}
function doinclude(cmd, fname, fileflag, file){
	if(fname ~ "/"){
		n=split(fname,name,"/")
		if(fileflag)print cmd " " name[n] >(dir "/" file)
		else print cmd " " name[n]
	}
	else if(fileflag)print cmd " " fname >(dir "/" file)
	else print cmd " " fname
	while((getline<fname)>0){
		if($0 ~ /^[\.]+include/)
			doinclude($1, $2, 1, fname)
		else print $0 >(dir "/" fname)
	}
	close(fname)
}
function getpic(file,flag,iscmd, ofile,		rfile, inmonk, pflag, i, pdelim){
	pflag = 1
	inmonk=0
	if(iscmd && $0 !~ /^\./ && $0 !~ /\|begin/){
		i=$0
		pdelim=getdel()
		$0=i
	}
	else pdelim=PEstr
	rfile=file
	while(1){
		if(flag){
			if(rfile == ""){
				if(getline <= 0)break
			}
			else if((getline<rfile)<=0){
				if(!inmonk)break
				inmonk=0
				close(rfile)
				rfile=file
				if(ofile != "")print "...\\\"end insert" >>ofile
				else print "...\\\"end insert"
				continue
			}
		}
		flag=1
		if($0 ~ /\|(insert|source)/){
			if(ofile != "")print "...\\\"begin insert" >>ofile
			else print "...\\\"begin insert"
			rfile=getname()
			inmonk=1
			continue
		}
		if(!inmonk && $0 ~ pdelim){
			pflag=0
			if(ofile != "")print $0 >>ofile
			else print $0
			break
		}
		if($0 ~ /(copy|include)/){
			for(i=1;i<NF;i++)
				if($i ~ /(copy|include)/)break
			i++
			if (($i !~ "thru") && ($i !~ "through")) {
				havecopy($1, $i, i, ofile);
			}
			else {
				if(ofile != "")print $0 >>ofile
				else print $0
			}
		}
		else if($0 ~ /^\.sp -1/){	#sign in pic output included
				print picoutput|"cat 1>&2"
				exit 1
		}
		else {
			if(ofile != "")print $0 >>ofile
			else print $0
		}
	}
	return(pflag)
}
function getdel(){
	if($0 !~ /^\|/ && $0 ~ /..*\|/)sub(/^..*\|/,"\|")
	sub(/\|[a-z \t]*/,"")
	ldelim = substr($0, 0, 1)
	sub(/./,"")
	b = substr(monkdel,index(monkdel,ldelim)+1,1)
	prdelim = b
	return("\\" b)
}
function comment(fname,	rdelim){	#print comment to avoid losing lines
	print $0
	rdelim = getdel()
	if($0 ~ rdelim)return
	if(fname == ""){
		while(getline > 0){
			print $0
			if($0 ~ rdelim)break
		}
	}
	else while((getline<fname)>0){
		print $0
		if($0 ~ rdelim)break
	}
}
#			reorder author/title in monk
function author(fname,	rdelim){
	if(titleflag)print $0
	else if(sauthor == "")sauthor = $0
	else sauthor = sauthor "\n" $0
	rdelim = getdel()
	if($0 !~ rdelim){
		while(1){
			if(fname == ""){
				if(getline <= 0)break
			}
			else {
				if((getline<fname) <= 0)break
			}
			if(titleflag)print $0
			else sauthor = sauthor "\n" $0
			if($0 ~ rdelim)break
		}
	}
}
function title(fname,	rdelim){
	print $0
	rdelim = getdel()
	if($0 !~ rdelim){
		if(fname == "")
			while(getline > 0){
				print $0
				if($0 ~ rdelim)break
			}
		else while((getline<fname) > 0){
				print $0
				if($0 ~ rdelim)break
		}
	}
	if(sauthor != ""){
		print sauthor
		sauthor = ""
	}
	titleflag=1
}
function mgetfile(	rname){
	rname=getname()
	if(rname in seen)return
	ismonk=1
	getfile(rname)
}
function getname(	del,n){
	del=getdel()
	n=split($0, xa, del)
	if(n>0)return(xa[1])
	else return($0)
}

PHOME=${PHOME:-/usr/lib/prefer}
export PHOME
$PHOME/prefawk '
BEGIN	{typestr = "%type"; edstr = "%editor"}

/^%A/	{$1 = "%author"} 
/^%B/	{$1 = "%booktitle"} 
/^%C/	{$1 = "%address"} 
/^%d/	{$1 = "%date"} 
/^%D/	{$1 = "%date"} 
/^%E/	{$1 = "%editor"} 
/^%G/	{$1 = "%government"} 
/^%I/	{$1 = "%publisher"} 
/^%J/	{$1 = "%journal"} 
/^%K/	{$1 = "%keyword"} 
/^%L/	{$1 = "%label"} 
/^%M/	{$1 = "%tm"} 
/^%N/	{$1 = "%number"} 
/^%O/	{$1 = "%other"} 
/^%P/	{$1 = "%pages"} 
/^%r/	{$1 = "%report"} 
/^%R/	{$1 = "%report"} 
/^%T/	{$1 = "%title"} 
/^%V/	{$1 = "%volume"} 

/^%/	{
	if(($1 != "%author") && ($1 != "%publisher"))
		print; 
	if($1 != "%editor")
		ineditor = 0
	inref = 1;
}

$1 == "%author"	{
	if ($NF ~ /\([Ee]d.*\)/) {
		edited++;
		$NF = "";
	}
	print;
}

$1 == "%publisher"	{
	publisher++ ;
	if($2 == "PhD") {
		phd++;
		printf "%s", $1;
		for (i=4; i <= NF; i++)
			printf " %s", $i;
		print "";
	}
	else if($2 ~ /Master*/) {
		masters++;
		printf "%s",$1;
		for (i=4; i <= NF; i++)
			printf " %s", $i;
		print "";
	}
	else print;
}

$1 == "%editor"		{ editor++; ineditor = 1;}
$1 == "%author"		{ author++ }
$1 == "%journal"	{ journal++ }
$1 == "%tm"		{ tm++ }
$1 == "%report"		{ report++ }
$1 == "%booktitle"	{ booktitle++ }
/[Pp]roc/ || /[Cc]onf/	{ if ($1 == "%journal") inproceedings++;}

/^[^%]/	{		# handle multiple editors
	if(ineditor) {
		printf "%s ", edstr;
		if($1 == "and")
			print substr($0,length($1)+2);
		else
			print;
	}
	else print;	# handle other multiple lines (like %Z)
}
# blank line, so now infer document type
/^$/	{
	if(inref) infer();
}


func infer()
{
	printf "%s ", typestr;
	if(edited) print "editedbook";
	else if(masters) print "mastersthesis";
	else if(phd) print "phdthesis";
	else if(inproceedings) print "inproceedings";
	else if(journal) print "article";
	else if(booktitle) print "incollection";
	else if(tm) print "tm";
	else if(report) print "techreport";
	else if(publisher) print "book";
	else print "misc";
	print;

	inref = 0;
	editor = 0;
	author = 0;
	journal = 0;
	inproceedings = 0;
	booktitle = 0;
	publisher = 0;
	tm = 0;
	report = 0;
	book = 0;
	tflag = 0;
	ineditor = 0;
	phd = 0;
	masters = 0;
	edited = 0;
}

END {
	if(inref) infer();
}
' $1

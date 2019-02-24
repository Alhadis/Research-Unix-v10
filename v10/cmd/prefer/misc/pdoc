sed '
/^%A/	s/%A /%author /
/^%B/	s/%B /%booktitle /
/^%C/	s/%C /%address /
/^%d/	s/%d /%date /
/^%D/	s/%D /%date /
/^%E/	s/%E /%editor /
/^%G/	s/%G /%government /
/^%I/	s/%I /%publisher /
/^%J/	s/%J /%journal /
/^%K/	s/%K /%keyword /
/^%L/	s/%L /%label /
/^%M/	s/%M /%tm /
/^%N/	s/%N /%number /
/^%O/	s/%O /%other /
/^%P/	s/%P /%pages /
/^%r/	s/%r /%report /
/^%R/	s/%R /%report /
/^%T/	s/%T /%title /
/^%V/	s/%V /%volume /
' $1 | sed '
/\.\[/ N
/\.\[\n\$LIST\$/ N
/\.\[\n\$LIST\$\n\.\]/ c\
|reference_placement
/^\.\[/ s/\.\[/|reference</
/^\.\]/ s/\.\]/%type >/
' | awk '
BEGIN	{typestr = "%type"; edstr = "%editor"; reffields = 0;}


/^%/	{
	if(($1 != "%author") && ($1 != "%publisher") && ($1 != "%type"))
		print; 
	if($1 != "%editor")
		ineditor = 0
	if($1 != "%type") reffields++;
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
	else print;
}
/^$/ 	{print}		# blank lines

# now infer document type
$1 == "%type"	{
	if(reffields) {
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
	}
	print ">";

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
	reffields = 0;
}
'

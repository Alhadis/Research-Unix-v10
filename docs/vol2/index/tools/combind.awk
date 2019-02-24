
BEGIN	{FS=":"}
	{ if($1 != laststr){
		if(start){
			if(range)pages = pages "-" npage
			print laststr "," pages
		}
		start=1
		laststr=$1
		pages = " " $2
		npage = 0 + $2
		range=0
	}
	else {
		if($2 == npage+1){
			range=1
			npage++
			next
		}
		else if(range) pages = pages "-" npage "," $2
		else pages = pages "," $2
		npage = 0 + $2
		range = 0
	}
}
END	{
	if(range)pages = pages "-" npage
	print laststr "," pages
	}


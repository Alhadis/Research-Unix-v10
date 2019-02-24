	{
	if(last == $0)next
	if(last "s" == $0){
		print last
		last = ""
		next
	}
	else if(last != "")
		print last
	last = $0
	}
END	{
	if(last != "")print last
}

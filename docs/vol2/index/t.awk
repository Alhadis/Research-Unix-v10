awk '
	{line=$0
	sub(/ /,"\n\'\''ti +.1i\n\\\\&",line)
	print "\\&" line
	}'

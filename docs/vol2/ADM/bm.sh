#!/bin/sh
awk ' BEGIN	{ pat = "^x X PI:[^:]*:[^:]*:[^:]*:[^:]*:[^:]*:[^,]*,[^,]*,[^,]*,"; }
/^x X COORD/	{
	found[$4] = 1
	coord[$4] = $11
	next
}
/^x X PI/	{
	split($3, a, ":")
	off = coord[a[6]]-a[2]
	if(found[a[6]] && off && match($0, pat)){
		pre = substr($0, RSTART, RLENGTH)
		post = substr($0, RSTART+RLENGTH)
		newx = post+off
		sub("^[0-9]*", "" newx, post)
		print "# increasing following xoff by " off
		$0 = pre post
	}
}
	{ print $0 }
'

echo removing duplicate entries 1>&2
awk '
#
#	get rid of duplicate entries per line
#
{
	for(i=1; i<=NF; i++)
		unique[$i] = "x"
	line = $1
	delete unique[$1]
	for(i=2; i<=NF; i++)
		if($i in unique) {
			line = line " " $i
			delete unique[$i]
		}
	print line
}
'

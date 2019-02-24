for j in 1 2 3 4 5 6 7 8 9
do
cd /usr/man/man$j
for i in *.$j
do
case $i in
\=.*)echo $i; continue;;
*) echo $i "	" `awk '
/^\.SH [\"]*SEE[\\]* ALSO[\"]*/	{
	while(getline>0){
		if($1 ~ /\.IR/)string = string $2 $3
		if($1 ~ /^\.SH/)break
	}
	print string
}' $i`;;
esac
done
cd ..
done

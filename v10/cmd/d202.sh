# this only allows a single input file
flags=
files=
for i
do
	case $i in
	-*)
		flags="$flags $i"
		;;
	*)
		files=$i
		;;
	esac
done
cat $files | rx nj/astro/arend.d202 "$flags"

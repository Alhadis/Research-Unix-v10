trap 'rm -f /tmp/cs$$; trap 0; exit' 0 1 2 3 15
L=/usr/lib/tmac
T=/tmp/cs$$
	$L/cstrans $T $1 $2
sh $T
rm $T

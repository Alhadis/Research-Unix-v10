awk '
BEGIN {	n = ARGV[1]	# numer of elements to generate and sort
	for (i = 1; i <= n; i++) x[i] = 1 + int(25*rand())
	for (i = 2; i <= n; i++) {
		for (j = i; j > 1 && x[j] < x[j-1]; j--)
			swap(j-1, j)
		show()
	}
}
function swap(i, j,   t) {
	t = x[i];  x[i] = x[j];  x[j] = t
}
function show(  i) {
	for (i = 1; i <= n; i++)
		printf("%3d", x[i])
	printf("\n")
}' $*

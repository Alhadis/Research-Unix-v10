#
#	double frexp (value, eptr)
#		double value;
#		int *eptr;
#
#	Frexp breaks "value" up into a fraction and an exponent.
#	It stores the exponent indirectly through eptr, and
#	returns the fraction.  More specifically, after
#
#		double d, frexp();
#		int e;
#		d = frexp (x, &e);
#
#	then |d| will be less than 1, and x will be equal to d*(2**e).
#	Further, if x is not zero, d will be no less than 1/2, and if
#	x is zero, both d and e will be zero too.
#
#
#	Entry point
#
	.text
	.globl	_frexp
_frexp:	.word	0x0000

	movd	4(ap),r0		# (r0,r1) := value
	extzv	$7,$8,r0,*12(ap)	# Fetch exponent
	jeql	fr1			# If exponent zero, we're done
	subl2	$128,*12(ap)		# Bias the exponent appropriately
	insv	$128,$7,$8,r0		# Force the result exponent to biased 0
fr1:	ret

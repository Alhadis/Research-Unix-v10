#!/bin/sh -p
pl -ag \
 | awk '/^Name:/ { name = $2; }
	/^Shares:/ { shares = $2 }
	/^Allocated share:/ { ashare = $3 }
	/^Effective share:/ { eshare = $3 }
	/^Normalised usage:/ { printf("%-10sShares:%4s  Share:%8s  E-Share:%9s  Usage:%13s\n", name, shares, ashare, eshare, $3); }' \
 | sort -t: +4nr -5 +3n

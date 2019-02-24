#!/bin/sh -p
pl -ag \
 | awk '/^Name:/ { lname = $2 }
	/^Scheduling group:/ { sgroupname = $3 }
	/^Charge:/ { printf("%-16sCharge:%20s  Scheduling group: %s\n", lname, $2, sgroupname) }' \
 | sort +2

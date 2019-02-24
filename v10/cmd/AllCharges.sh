#!/bin/sh -p
pwintf 1 '%-16sCharge:%15.0f  Scheduling group: %s\n' lname charge sgroupname \
 | sort -t':' +1n

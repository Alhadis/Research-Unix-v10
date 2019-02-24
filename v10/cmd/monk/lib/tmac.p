.de PS	\" start picture (bwk)
.	\" $1 is height, $2 is width in inches
.ls 1
.if \\$1>0 .sp .35
.in (\\n(.lu-\\$2)/2u
.ne \\$1u
..
.de PE	\" end of picture
.ls
.in
.if \\$1>0 .sp .65
..

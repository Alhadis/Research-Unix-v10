awk '
BEGIN	      {	OFS = "\t"; w = 1024 }
$1 == "m"     { s = $2
		ehist[s] = e = s + $3
		sx = s % w; sy = int(s / w)
		ex = e % w; ey = int(e / w)
		if (sy == ey) print "a" s ": line", sx, sy, ex, sy
		else {	print "a" s ": line " sx " " sy " " w " " sy
			for (i = sy+1; i <= ey-1; i++)
				print "b" i ": line", 0, i, w, i
			print "c" ey ": line", 0, ey, ex, ey
		}
	      }
$1 == "f"     {	s = $2
		e = ehist[s]
		sx = s % w; sy = int(s / w)
		ex = e % w; ey = int(e / w)
		if (sy == ey) print "erase a" s
		else {	print "erase a" s
			for (i = sy+1; i <= ey-1; i++)
				print "erase b" i
			print "erase c" ey
		}
	      }
	      {	print "click call" }
' $*

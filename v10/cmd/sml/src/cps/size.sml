(* Copyright 1989 by AT&T Bell Laboratories *)
structure CPSsize =
struct
local open CPS

fun cells_list (l : 'a list) = 2 * length l
fun descriptors_list (l : 'a list) = length l

val rec cells =
  fn RECORD(vl,w,ce) => 2 + 3 + cells_list vl + cells ce
   | SELECT(i,v,w,ce) => 2 + 4 + cells ce
   | OFFSET(i,v,w,ce) => 2 + 4 + cells ce
   | APP(v,vl) => 2 + 2 + cells_list vl
   | FIX(l,ce) => 2 + 2 + cells_list l + cells ce
		+ fold (fn((f,vl,e),b) => 3 + cells_list vl + cells e + b) l 0
   | SWITCH(v,l) => 2 + 2 + cells_list l + fold (fn(a,b) => cells a + b) l 0
   | PRIMOP(_,vl,wl,ce) => 
	2 + 4 + cells_list vl + cells_list wl + cells_list ce
		 + fold (fn(a,b) => cells a + b) ce 0

val rec descriptors =
  fn RECORD(vl,w,ce) => 2 + descriptors_list vl + descriptors ce
   | SELECT(i,v,w,ce) => 2 + descriptors ce
   | OFFSET(i,v,w,ce) => 2 + descriptors ce
   | APP(v,vl) => 2 + descriptors_list vl
   | FIX(l,ce) => 2 + descriptors_list l + descriptors ce
		+ fold (fn((f,vl,e),b) => 1 + descriptors_list vl
			+ descriptors e + b) l 0
   | SWITCH(v,l) => 2 + descriptors_list l
			+ fold (fn(a,b) => descriptors a + b) l 0
   | PRIMOP(_,vl,wl,ce) => 
	2 + descriptors_list vl + descriptors_list wl + descriptors_list ce
		 + fold (fn(a,b) => descriptors a + b) ce 0
in
fun printsize ce =
  let val c = cells ce
      val d = descriptors ce
  in  print "CPSsize: #cells="; print c; print "; #descriptors="; print d;
      print "\n"; ()
  end
end

end

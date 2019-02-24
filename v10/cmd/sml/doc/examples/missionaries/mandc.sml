(* Missionaries and canabals state space *)
structure MandC =
struct

  datatype loc = LEFT | RIGHT  (* location of boat *)

  type sit = {mis:int,can:int,boat:loc}
  type state = sit list

  (* sit, short for situation, represents number of missionaries and canabals
       where the boat is, and the location of the boat.
     state is a list of situation, representing a history of moves *)

  fun member(s, []:state) = false
    | member(s,s'::rest) = s = s' orelse member(s,rest)

  (* safe checks that there are at least as many missionaries as canabals
     where ever they are mixed, and also checks that the latest situation
     hasn't occurred before in the state *)
  fun safe((new as {mis,can,...})::old : state) = 
      (mis=0 orelse (mis>=can andalso (3-mis = 0 orelse 3-mis >= 3-can)))
      andalso not(member(new,old))

  fun printSit (h:string) ({mis,can,boat}: sit) =
        (print h; print mis;
         print " "; print can;
         print " "; case boat of LEFT => print "LEFT" | _ => print "RIGHT";
	 print "\n")

  fun goal(s as {mis=3,can=3,boat=RIGHT}::_ : state) = 
        (app (fn sit => printSit "% " sit) (rev s); true)
    | goal(sit::_) =
	(printSit "? " sit; false)

  (* possible configurations of passengers on the canoe, (missionaries,canabals) *)
  val passengers = [(2,0),(1,1),(0,2),(1,0),(0,1)]

  fun cross LEFT = RIGHT
    | cross RIGHT = LEFT

  (* moves generates list of possible successors of its state argument *)
  fun moves (s as {mis,can,boat}::_) = 
      let fun mv [] = []
	    | mv ((m,c)::rest) =
		if mis >= m andalso can >= c 
		then ({mis=3-mis+m,can=3-can+c,boat=cross(boat)}::s)::mv rest
		else mv rest
       in fold (fn (s,l) => if safe s then s::l else l) (mv passengers) []
      end

  val initial : state = [{mis=3, can=3, boat=LEFT}]  (* initial state *)

end (* MandC *)



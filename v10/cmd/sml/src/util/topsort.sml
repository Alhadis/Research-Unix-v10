(* Copyright 1989 by AT&T Bell Laboratories *)
structure Topsort =
struct

 (* precondition: no cycles *)
   fun topsort1 (op < : ('item * 'item) -> bool) =
     let fun sort nil = nil
           | sort (x::l) =
               let fun f(nil, less, unc, gtr) = 
		       sort less @ [x] @ sort unc @ sort gtr
		     | f(a::ar, less, unc, gtr) =
		        if a < x then f(ar, a::less, unc, gtr)
			else if x < a then f(ar, less, unc, a::gtr)
			else f(ar, less, a::unc, gtr)
		in f(l, nil, nil, nil)
               end
      in sort
     end

   exception Cycle

  (* detects cycles, but is slower *)
   fun topsort2 (op < : ('item * 'item) -> bool) items =
    let fun sort (nil, sorted) = sorted
	  | sort (items,sorted) =
	       let fun f(a::r, less, sorted, ??) =
	               if exists (fn z => a<z) items
			   then f(r, a::less, sorted, ??)
			   else f(r, less, a::sorted, true)
		     | f(nil, less, sorted, true) = sort(less, sorted)
		     | f(nil, _, _, false) = raise Cycle
                in f (items, nil, sorted, false)
	       end
     in sort(items,nil)
    end

end
		      


(* Copyright 1989 by AT&T Bell Laboratories *)
structure Sort = struct

fun sort (op > : ('x * 'x -> bool)) =
    let fun s (a::b::c) =
	    let val (x,y) = if a>b then (b,a) else (a,b)
		fun insert' [] = [y]
		  | insert' (l as c::d) = if y>c then c::insert' d else y::l
		fun insert [] = [x,y]
 	          | insert (l as c::d) = if x>c then c::insert d else x::insert' l
	    in insert(s c)
	    end
	  | s l = l
    in s
    end

fun sorted (op >) =
  let fun s (x::(rest as (y::_))) = not(x>y) andalso s rest
        | s l = true
  in s
  end

end

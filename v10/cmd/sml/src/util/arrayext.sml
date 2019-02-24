(* Copyright 1989 by AT&T Bell Laboratories *)
(* util/arrayext.sml *)

signature ARRAYEXT =
sig
  val copy : '1a array * int -> '1a array
  val app : ('a -> 'b) * 'a array * int -> unit
  val map : ('1a -> '1a) * '1a array * int -> '1a array
end

structure ArrayExt : ARRAYEXT =
struct

  (* copy -- was copyarray.  Used in EnvAccess and Parse *)
  fun copy(a,n) =
      (* assume n <= length a *)
      let val new = array(n,a sub 0)
	  fun loop i = (update(new,i,a sub i); loop(i+1))
       in loop 0
	  handle Subscript => new
      end

  (* app -- was arrayapp.  Used in SigMatch and Parse *)
  fun app (f,a,start) =  
      let fun loop i = (f(a sub i); loop(i+1))
       in loop start handle Subscript => ()
      end

  (* map -- was arraymap.  Used in SigMatch, and somewhat specialized *)
  fun map (f,a,start) =
      let val b = array(Array.length a, a sub 0) handle Subscript => a
	  fun loop i = (update(b,i,f(a sub i)); loop(i+1))
       in loop start handle Subscript => b
      end

end
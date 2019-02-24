(* Copyright 1989 by AT&T Bell Laboratories *)
(* util/strghash.sml *)

structure StrgHash =
struct

  val prime = 8388593 (* largest prime less than 2^23 *)
  val base = 128

(* the simple version --
    fun hashString(str: string) : int =
        let fun loop (0,n) = n
	      | loop (i,n) = 
	          let val i = i-1
		      val n' = (base * n + ordof(str,i)) 
		   in loop (i, (n' - prime * (n' div prime)))
		  end
	 in loop (size str,0)
	end
*)

  fun hashString(str: string) : int =
      let val l = size str
       in case l
	    of 0 => 0
	     | 1 => ord str
	     | 2 => ordof(str,0) + base * ordof(str,1)
	     | 3 => ordof(str,0) + base * (ordof(str,1) + base * ordof(str,2))
	     | _ =>
		let fun loop (0,n) = n
		      | loop (i,n) = 
			  let val i = i-1
			      val n' = (base * n + ordof(str,i)) 
			   in loop (i, (n' - prime * (n' div prime)))
			  end
		 in loop (l,0)
		end
      end

end (* structure StrgHash *)




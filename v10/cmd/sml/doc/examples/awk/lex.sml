(* lex.sml *)

signature LEX =
sig
  val words: string -> string list
end

structure Lex1 : LEX =
struct

  fun separator " " = true
    | separator "\t" = true
    | separator "\n" = true
    | separator _ = false

  fun words s =
      let fun getword(w,[])  = [implode(rev w)]
	    | getword(w,c::rest) = 
		if separator(c)
		then implode(rev w) :: skip rest
		else getword(c::w,rest)
	  and skip [] = []
	    | skip(c::rest) =
		if separator c
		then skip rest
		else getword([c],rest)
       in skip(explode s)
      end

end (* Lex1 *)

structure Lex2 : LEX =
struct

  fun separator " " = true
    | separator "\t" = true
    | separator "\n" = true
    | separator _ = false

  fun words(s: string) =
      let val len  = length s
	  fun skip n = 
	      let fun getword m =
		  if m>=len orelse separator(substring(s,m,1))
		  then substring(s,n,(m-n))::skip(m+1)
		  else getword(m+1)
	       in if n>=len
		  then []
		  else if separator(substring(s,n,1))
		  then skip(n+1)
		  else getword(n+1)
	      end
       in skip 0
      end

end (* Lex2 *)
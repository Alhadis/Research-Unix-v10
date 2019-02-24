(* Copyright 1989 by AT&T Bell Laboratories *)
structure PrintUtil : PRINTUTIL = struct

  structure Symbol : SYMBOL = Symbol

  fun newline () = print "\n"
  fun tab 0 = () | tab n = (print " "; tab(n-1))

  fun printSequence (separator: string) pr elems =
      let fun prElems [el] = pr el
	    | prElems (el::rest) = (pr el; print separator; prElems rest)
	    | prElems [] = ()
       in prElems elems
      end

  fun printClosedSequence (front: string, sep, back:string) pr elems =
      (print front; printSequence sep pr elems; print back)

  fun printSym(s: Symbol.symbol) = print(Symbol.name s)
      (* fix -- maybe this belongs in Symbol *)

  fun formatQid p =
    let fun f [s] = [Symbol.name s]
          | f (a::r) = Symbol.name a :: "." :: f r
     in implode(f p)
    end

  val stringDepth = System.Control.Print.stringDepth
  local
  fun decimal i = let val m = Integer.makestring
		  in  m(i div 100)^m((i div 10)mod 10)^m(i mod 10) end
  val ctrl_a = 1
  val ctrl_z = 26
  val offset = ord "A" - ctrl_a
  val smallestprintable = ord " "
  val biggestprintable = ord "~"
  fun ml_char "\n" = "\\n"
    | ml_char "\t" = "\\t"
    | ml_char "\\" = "\\\\"
    | ml_char "\"" = "\\\""
    | ml_char c =
	  let val char = ord c
	  in  if char >= ctrl_a andalso char <= ctrl_z
	      then "\\^" ^ chr(char+offset)
	      else if char >= smallestprintable andalso char <= biggestprintable
		   then c
	      else "\\" ^ decimal char
	  end
  in
      fun mlstr s = "\"" ^ implode(map ml_char (explode s)) ^ "\""
      fun pr_mlstr s =
	  let val depth = !stringDepth
	      fun pr i =
		  if i=depth then print "#"
		  else (let val ch = substring(s,i,1)
			in  print(ml_char ch); pr (i+1)
			end handle Substring => ())
	  in print "\""; pr 0; print "\""
	  end

    fun nlindent n = (newline(); tab n)

    fun printvseq ind (sep:string) pr elems =
	let fun prElems [el] = pr el
	      | prElems (el::rest) = (pr el; nlindent ind; print sep; prElems rest)
	      | prElems [] = ()
	 in prElems elems
	end

  end (* local *)

end (* structure PrintUtil *)

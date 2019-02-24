(* awk.sml *)

signature AWK =
sig
  val makeInt : string -> int  (* translates a digit string into an int *)
  val awk : instream * (string list  -> unit) * (unit -> unit) -> unit
end

structure Awk : AWK =
struct

  val digit0 = ord "0"
  and digit9 = ord "9"
  exception MakeInt
  fun makeInt (s:string) : int =
      (* translates string s into an integer.
         Bug: doesn't deal with negative integers *)
      let val len = length s
	  fun translate(i,n) =
	      if i<len
	      then let val c = ordof(s,i)
		    in if digit0 <= c andalso c <= digit9
		       then translate(i+1,10*n+c-digit0)
		       else raise MakeInt
		   end
	      else n
       in translate(0,0)
      end

  exception END

  fun parseStream (instream) = 
      fn () => if end_of_stream instream
	       then raise END
	       else Lex2.words(input_line(instream))

  fun awk(instream,step,final) =
      let val lines = parseStream instream
	  fun iter () = (step(lines()); iter())
       in iter() handle END => final()
      end

end



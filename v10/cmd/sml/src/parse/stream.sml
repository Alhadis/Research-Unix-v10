(* Copyright 1989 by AT&T Bell Laboratories *)
signature STREAM =
 sig type 'a stream
     val streamify : (unit -> '1a) -> '1a stream
     val cons : '1a * '1a stream -> '1a stream
     val get : '1a stream -> '1a * '1a stream
 end

abstraction Stream : STREAM =
struct
   datatype 'a str = EVAL of 'a * 'a str ref | UNEVAL of (unit->'a)

   type 'a stream = 'a str ref

   fun get(ref(EVAL t)) = t
     | get(s as ref(UNEVAL f)) = 
	    let val t = (f(), ref(UNEVAL f)) in s := EVAL t; t end

   fun streamify f = ref(UNEVAL f)
   fun cons(a,s) = ref(EVAL(a,s))

end

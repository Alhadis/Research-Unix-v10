(* Copyright 1989 by AT&T Bell Laboratories *)
structure Overloads = (* New Jersey Overloads *)
  struct
   local open Initial in
    overload makestring : ('a -> string)
	  as Bool.makestring and Integer.makestring and Real.makestring
    overload print : ('a -> unit)
	  as Bool.print and Integer.print and Real.print and String.print
    overload ~ :   ('a -> 'a)        as Integer.~   and Real.~
    overload + :   ('a * 'a -> 'a)   as Integer.+   and Real.+
    overload - :   ('a * 'a -> 'a)   as Integer.-   and Real.-
    overload * :   ('a * 'a -> 'a)   as Integer.*   and Real.*
    overload < :   ('a * 'a -> bool) as Integer.<   and Real.<  and String.<
    overload > :   ('a * 'a -> bool) as Integer.>   and Real.>  and String.>
    overload <= :  ('a * 'a -> bool) as Integer.<=  and Real.<= and String.<=
    overload >= :  ('a * 'a -> bool) as Integer.>=  and Real.>= and String.>=
    overload abs : ('a -> 'a)        as Integer.abs and Real.abs

   end (* local open Initial *)
  end (* structure Overloads *)

(* Standard environment, as defined in Definition of SML, Version 2 *)

structure Standard = 
  struct

    infix 7 * / div mod
    infix 6 + - ^
    infixr 5 :: @
    infix 4 = <> < > <= >=
    infix 3 o :=

    val ! = Ref.!
    val op := = Ref.:=
    val not = Bool.not
    val floor = Real.floor
    val truncate = Real.truncate
    val ceiling = Real.ceiling
    val real = Real.real
    val sin = Real.sin and cos = Real.cos 
    and sqrt = Real.sqrt and arctan = Real.arctan
    and exp = Real.exp and ln = Real.ln
    and rev = List.rev and map = List.map and op @ = List.@
    exception Floor=Real.Floor and Sqrt=Real.Sqrt
    exception Exp=Real.Exp and Ln=Real.Ln
    val op / = Real./
    
    exception Mod
    exception Quot
    exception Prod
    exception Sum
    exception Diff
    exception Chr = String.Chr and Ord = String.Ord
    exception Div = Integer.Div

    val op + = fn (a,b) => Integer.+(a,b) handle Integer.Overflow => raise Sum
    val op * = fn (a,b) => Integer.*(a,b) handle Integer.Overflow => raise Prod
    val op div = fn (a,b) =>
       (if Integer.>=(a,0)
	then if Integer.>=(b,0)
	     then Integer.div(a,b)
	     else Integer.-(Integer.div(Integer.-(a,1),b),1)
	else if Integer.>=(b,0)
	     then Integer.-(Integer.div(Integer.+(a,1),b),1)
	     else Integer.div(a,b)) handle Integer.Overflow => raise Div
    val op - = fn (a,b) => Integer.-(a,b) handle Integer.Overflow => raise Diff
    val op mod = fn (a,b) => Integer.-(a,Integer.*(a div b,b))
			     handle Div => raise Mod
			          | Integer.Overflow => raise Mod
    structure BasicIO : BASICIO = IO
    open BasicIO
    val output = fn (f,s) => output f s
    val input = fn (f,i) => input f i
    val size = String.size
    val explode = String.explode and implode = String.implode
    val op ^ = String.^
    val chr = String.chr and ord = String.ord
        
    val op o = General.o and op = = General.= and op <> = General.<>
    exception Bind= General.Bind and Match = General.Match
    exception Interrupt = General.Interrupt
  end (* structure Standard *)

structure Standard =
  struct
    open Standard
    overload ~ : ('a->'a) as Integer.~ and Real.~
    overload + : ('a*'a -> 'a) as Standard.+ and Real.+
    overload - : ('a*'a -> 'a) as Standard.- and Real.-
    overload * : ('a*'a -> 'a) as Standard.* and Real.*
    overload < : ('a*'a -> bool) as Integer.< and Real.<
    overload > : ('a*'a -> bool) as Integer.> and Real.>
    overload <= : ('a*'a -> bool) as Integer.<= and Real.<=
    overload >= : ('a*'a -> bool) as Integer.>= and Real.>=
    overload abs : ('a->'a) as Integer.abs and Real.abs

  end (* structure Environ *)

structure Tree : FORMULA =
struct
    datatype formula = NUM of int 
		     | CELLREF of formula * formula
		     | BINOP of (int*int->int) * formula * formula
		     | IF of formula * formula * formula
   fun eval f a =
     let fun g  (NUM i) = i
           | g (CELLREF(x,y)) = a sub (g x)  sub (g y)
	   | g (BINOP(p,x,y)) = p(g x, g y)
	   | g (IF(x,y,z)) = if g x <> 0 then g y else g z
      in g f
     end
end

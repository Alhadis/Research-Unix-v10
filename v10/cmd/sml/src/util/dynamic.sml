(* Copyright 1989 by AT&T Bell Laboratories *)
signature STATIC_ARRAY = sig type array
			     exception Subscript
			     type elem
			     val array : int * elem -> array
			     val sub : array * int -> elem
			     val update : (array * int * elem) -> unit
			     val length : array -> int
			 end

signature DYNAMIC_ARRAY = sig type array 
			      exception Subscript 
			      type elem 
			      val array : elem -> array
			      val sub : array * int -> elem
			      val update : (array * int * elem) -> unit
			  end

functor Dynamic( A : STATIC_ARRAY ) : DYNAMIC_ARRAY =
struct
     type array = {default: A.elem, arr: A.array ref}
     type elem = A.elem
     exception Subscript
     fun array e = {default=e, arr= ref(A.array(0,e))};
     fun {default, arr as ref a} sub i = 
	A.sub(a,i)
	handle A.Subscript =>
	    if i < 0 then raise Subscript
	    else default
     fun update ({default,arr as ref a}, i, e) =
       A.update(a,i,e)
       handle A.Subscript =>
       if i<0 then raise Subscript
       else
       let val size = A.length a
	   val newsize = i + size + 1
	   val a2 = A.array(newsize, default)
	   fun copy j = (A.update(a2,j,A.sub(a,j)); copy(j-1))
       in (copy (size - 1) handle A.Subscript => ());
	  arr := a2;
	  A.update(a2,i,e)			
       end

end



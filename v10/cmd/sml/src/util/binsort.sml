(* Copyright 1989 by AT&T Bell Laboratories *)
(* binsort.sml *)

structure BinSort = struct

datatype 'a tree = NIL
		 | NODE of {left : 'a tree ref, right : 'a tree ref, value : 'a ref}

fun mkSortTree () = ref NIL

fun insert (op >, tree) v =
    let fun insert' tr =
	    case !tr of
	      NIL => tr := NODE{left=ref NIL, right=ref NIL, value=ref v}
	    | NODE{left, right, value} =>
		if !value > v then insert'(left)
		else if v > !value then insert'(right)
		else value := v
    in insert'(tree)
    end

exception Finished

fun generator (tree) =
    let val LCS = ref(fn()=>raise Match)
	fun loop (NIL,k) = k()
	  | loop (NODE{left,right,value},k) =
	    loop (!left,fn()=>(LCS := fn()=>loop(!right,k); !value))
    in (LCS := fn()=>loop(tree,fn()=>raise Finished);fn()=> !LCS())
    end

end (* BinSort *)

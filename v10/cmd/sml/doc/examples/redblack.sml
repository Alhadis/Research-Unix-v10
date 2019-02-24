(* redblack.sml *)

functor RedBlack(B : sig type key
			 val > : key*key->bool
		     end):
	    sig type tree
		type key
		val empty : tree
		val insert : key * tree -> tree
		val lookup : key * tree -> key
		exception Notfound of key
	    end =
struct
 open B
 datatype color = RED | BLACK
 datatype tree = empty | tree of key * color * tree * tree
 exception Notfound of key

 fun insert (key,t) =
  let fun f empty = tree(key,RED,empty,empty)
        | f (tree(k,BLACK,l,r)) =
	    if key>k
	    then case f r
		 of r as tree(rk,RED, rl as tree(rlk,RED,rll,rlr),rr) =>
			(case l
			 of tree(lk,RED,ll,lr) =>
				tree(k,RED,tree(lk,BLACK,ll,lr),
					   tree(rk,BLACK,rl,rr))
			  | _ => tree(rlk,BLACK,tree(k,RED,l,rll),
						tree(rk,RED,rlr,rr)))
		  | r as tree(rk,RED,rl, rr as tree(rrk,RED,rrl,rrr)) =>
			(case l
			 of tree(lk,RED,ll,lr) =>
				tree(k,RED,tree(lk,BLACK,ll,lr),
					   tree(rk,BLACK,rl,rr))
			  | _ => tree(rk,BLACK,tree(k,RED,l,rl),rr))
	          | r => tree(k,BLACK,l,r)
	    else if k>key
	    then case f l
	         of l as tree(lk,RED,ll, lr as tree(lrk,RED,lrl,lrr)) =>
			(case r
			 of tree(rk,RED,rl,rr) =>
				tree(k,RED,tree(lk,BLACK,ll,lr),
					   tree(rk,BLACK,rl,rr))
			  | _ => tree(lrk,BLACK,tree(lk,RED,ll,lrl),
						tree(k,RED,lrr,r)))
		  | l as tree(lk,RED, ll as tree(llk,RED,lll,llr), lr) =>
			(case r
			 of tree(rk,RED,rl,rr) =>
				tree(k,RED,tree(lk,BLACK,ll,lr),
					   tree(rk,BLACK,rl,rr))
			  | _ => tree(lk,BLACK,ll,tree(k,RED,lr,r)))
	          | l => tree(k,BLACK,l,r)
	    else tree(key,BLACK,l,r)
        | f (tree(k,RED,l,r)) =
	    if key>k then tree(k,RED,l, f r)
	    else if k>key then tree(k,RED, f l, r)
	    else tree(key,RED,l,r)
   in case f t
      of tree(k,RED, l as tree(_,RED,_,_), r) => tree(k,BLACK,l,r)
       | tree(k,RED, l, r as tree(_,RED,_,_)) => tree(k,BLACK,l,r)
       | t => t
  end

 fun lookup (key,t) =
  let fun look empty = raise (Notfound key)
	| look (tree(k,_,l,r)) =
		if k>key then look l
		else if key>k then look r
		else k
   in look t
  end

end

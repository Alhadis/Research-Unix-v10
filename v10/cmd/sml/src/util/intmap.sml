(* Copyright 1989 by AT&T Bell Laboratories *)
structure Intmap : INTMAP =
struct
  datatype 'a bucket = NIL | B of (int * 'a * 'a bucket)
  datatype 'a intmap =
    H of {table: 'a bucket array ref,elems: int ref,exn: exn,name: string option}
  fun bucketapp f =
      let fun loop NIL = ()
	    | loop(B(i,j,r)) = (f(i,j); loop r)
      in loop
      end
  fun namednew(name, size, exn) =
      H {table=ref(array(size,NIL)),elems=ref 0,exn=exn,name=SOME name}
  fun new(size, exn) =
      H {table=ref(array(size,NIL)),elems=ref 0,exn=exn,name=NONE}
  fun map (H{table,exn,...}) =
      (fn i => let fun find NIL = raise exn
		     | find(B(i',j,r)) = if i=i' then j else find r
		   val ref a = table
	       in find(a sub Bits.andb(i,(Array.length a)-1))
	       end)
  fun rem (H{table=ref a,elems,...}) i =
      let fun f(B(i',j,r)) = if i=i' then (dec elems; r) else B(i',j,f r)
	    | f x = x
	  val index = Bits.andb(i,(Array.length a)-1)
      in update(a, index, f(a sub index))
      end
  fun app f (H{table=ref a,...}) =
      let fun zap 0 = ()
	    | zap n = let val m = n-1 in bucketapp f (a sub m); zap m end
      in  zap(Array.length a)
      end
  fun add (m as H{table as ref a, elems, name, ...}) (v as (i,j)) =
    let val size = Array.length a
    in if !elems <> size
       then let val index = Bits.andb(i, size-1)
	        fun f(B(i',j',r)) = if i=i' then B(i,j,r) else B(i',j',f r)
		  | f x = (inc elems; B(i,j,x))
	    in update(a,index,f(a sub index))
	    end
       else let val newsize = size+size
	        val newsize1 = newsize-1
	        val new = array(newsize,NIL)
		fun bucket n =
		    let fun add'(a,b,B(i,j,r)) =
		            if Bits.andb(i,newsize1) = n
		            then add'(B(i,j,a),b,r)
		            else add'(a,B(i,j,b),r)
		          | add'(a,b,NIL) = 
		            (update(new,n,a);
			     update(new,n+size,b);
			     bucket(n+1))
		    in add'(NIL,NIL,a sub n)
		    end
	    in (case name of
		  NONE => ()
	        | SOME name =>
		  (print("\nIncreasing size of intmap " ^ name ^ " to: ");
		   print newsize; print "\n"));
	       bucket 0 handle Subscript => ();
	       table := new;
	       add m v
	    end
    end
  fun intMapToList(H{table,...})=
     let val a = !table;
	 val last = Array.length a - 1
         fun loop (0, NIL, acc) = acc
         |   loop (n, B(i,j,r), acc) = loop(n, r, (i,j)::acc)
         |   loop (n, NIL, acc) = loop(n-1, a sub (n-1), acc)
      in loop(last,a sub last,[])
     end
end

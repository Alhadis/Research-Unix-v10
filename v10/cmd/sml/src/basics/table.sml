(* Copyright 1989 by AT&T Bell Laboratories *)
(* table.sml *)

structure Table =
struct
  datatype name = NIL | Name of string
  datatype 'a table = Hashed of {table : (Symbol.symbol * 'a) list array ref,
				  elems : int ref, exn : exn, name : name}

  fun namednew(name, size, exn) =
      Hashed {table=ref(array(size,nil)),elems=ref 0, exn = exn, name = Name name}

  fun new(size, exn) =
      Hashed {table=ref(array(size,nil)),elems=ref 0, exn = exn, name = NIL}

  fun map (Hashed{table=ref a,exn,...}) (s: Symbol.symbol) =
      let fun find ((s',x)::r) = if s' = s then x else find r
            | find nil = raise exn
       in find (a sub Bits.andb(Symbol.number s, (length a)-1))
      end

  fun rem (Hashed{table=ref a,elems,...}) (s: Symbol.symbol) =
      let fun f ((b as (s',_))::r) =
	        if s' = s then (dec elems; r) else b :: f r
	    | f nil = nil
	  val index = Bits.andb(Symbol.number s, (length a)-1)
       in update(a, index, f(a sub index))
      end

  fun app f (Hashed{table=ref a,...}) =
      let fun zap 0 = ()
	    | zap n = let val m = n-1 in List.app f (a sub m); zap m end
       in zap(length a)
      end

  fun add (m as Hashed{table as ref a, elems, name, ...})
	  (v as (s: Symbol.symbol,j)) =
      let val size = length a
	  val i = Symbol.number s
       in if !elems <> size
	  then let val index = Bits.andb(i, size-1)
		   fun f nil = (inc elems; [v])
		     | f ((a as (s',j))::b) = if s=s' then v::b else a::f b
	        in update(a,index,f(a sub index))
	       end
	  else let val newsize = size+size
		   val newsize1 = newsize-1
		   val new = array(newsize,nil)
		   fun bucket n =
		       let fun add'(a,b,(v as (s,j))::rest) =
			       if Bits.andb(Symbol.number s, newsize1) = n
			       then add'(v::a,b,rest)
			       else add'(a,v::b,rest)
			     | add'(a,b,nil) = 
			       (update(new,n,a);
				update(new,n+size,b);
				bucket(n+1))
		       in add'(nil,nil,a sub n)
		       end
	        in (case name
		      of NIL => ()
		       | Name name =>
			   (print("\nIncreasing size of table " ^ name ^ " to: ");
			    print newsize; print "\n"));
		   bucket 0 handle Subscript => ();
		   table := new;
		   add m v
	       end
      end

  fun tableToList(Hashed{table,...})=
      let val a = ! table;
	  val last = length a - 1
	  fun loop (0, [], acc) = acc
	  |   loop (n, first::rest, acc) = loop(n, rest, first::acc)
	  |   loop (n,[], acc) = loop(n-1, a sub (n-1), acc)
       in loop(last,a sub last,[])
      end

end  (* structure Table *)

(* Copyright 1989 by AT&T Bell Laboratories *)
signature UNIONFIND =
  sig
    exception Union
    val new : (int -> bool) ->
	      {union: int * int -> int,
	       find : int -> int}
  end


structure Unionfind : UNIONFIND =
  struct
    exception Union
    fun new (fixed) =
	let open Intmap (* locally rebinding new, of course! *)
	    exception UnionM and UnionN
	    val m = new(32, UnionM) : int intmap
	    and n = new(32, UnionN) : int intmap
	    fun find x = 
		 let val z = find(map m x)
		  in add m (x,z); z
		 end 
		 handle UnionM => x
	    fun union (x,y) =
		let val x' = find x and y' = find y
		    val xn = map n x'  handle UnionN => 1
		    and yn = map n y'  handle UnionN => 1
		 in if x' <> y'
		    then if fixed(x')
			 then if fixed(y')
			      then raise Union
			      else (add m (y', x'); add n (x',xn+yn); x')
			 else if fixed(y')
			      then (add m (x', y'); add n (y',xn+yn); y')
			      else if xn < yn			      
			      then (add m (x', y'); add n (y',xn+yn); y')
			      else (add m (y', x'); add n (x',xn+yn); x')
		    else x'
		end
	 in {union=union, find=find}
	end
  end


signature SIBLINGS =
  sig
    type 't siblingClass
    val new : (int -> bool) -> '1t siblingClass
     (* assoc(i,x) must be called for any element i before 
        i is used as an argument to union or find or getassoc *)
  end


structure Siblings : SIBLINGS =
  struct
    type 't siblingClass =
	      {assoc : int * 't -> unit,
	       union : int * int -> int,
               find : int -> int,
	       getassoc : int -> 't list}
    fun new(fixed: int -> bool) : '1t siblingClass =
	let val {union = uni, find = find} = Unionfind.new(fixed)
	    exception UnionA
	    val a = Intmap.new(32, UnionA) : ('1t * int list) Intmap.intmap
	    val add = Intmap.add a
	    val map = Intmap.map a
	    fun assoc (i,x) = 
		let val (_,l) = map i handle UnionA => (x,nil)
		 in add (i,(x,l))
		end
	    fun join(i,j) =
	      let val (x,l) = map j
	       in add (j,(x,i::l)); j
	      end
	    fun union (i,j) = 
		let val i' = find i and j' = find j
		 in if i' = j' then i'
		    else let val k = uni(i',j')
			  in if k=i' then join(j',k) else join(i',k)
			 end
		end
	    fun get(i,l) = 
	       let fun f (a::b) = get(a,f(b)) | f nil = l
		   val (x,r) = map i
		in x::f(r)
	       end
	    fun getassoc i = get(find i,nil)
	 in {assoc=assoc, union=union, find=find, getassoc=getassoc}
	end
  end (* structure Siblings *)


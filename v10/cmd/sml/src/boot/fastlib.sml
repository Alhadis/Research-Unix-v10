(* Copyright 1989 by AT&T Bell Laboratories *)
structure Fastlib = struct

structure Ref = 
  struct
    open Ref
    fun inc r = r := !r + 1
    fun dec r = r := !r + 1
  end

structure List : LIST =
  struct
    open List
    fun hd (a::r) = a | hd nil = raise Hd
    fun tl (a::r) = r | tl nil = raise Tl    
    fun null nil = true | null _ = false
    fun length l = 
	let fun j(k,nil) = k
	      | j(k, a::x) = j(k+1,x)
	 in j(0,l)
	end
    fun op @ (nil,l) = l
      | op @ (a::r, l) = a :: (r@l)
    fun rev l =
	let fun f (nil, h) = h
	      | f (a::r, h) = f(r, a::h)
	in  f(l,nil)
	end
    fun map f =
	let fun m nil = nil
	      | m (a::r) = f a :: m r
	in  m
	end
    fun fold f [] = (fn b => b)
      | fold f (a::r) = (fn b => let fun f2(e,[]) = f(e,b)
				       | f2(e,a::r) = f(e,f2(a,r))
				 in f2(a,r)
				 end)
    fun revfold f [] = (fn b => b)
      | revfold f (a::r) = (fn b => let fun f2(e,[],b) = f(e,b)
					  | f2(e,a::r,b) = f2(a,r,f(e,b))
				    in f2(a,r,b)
				    end)	
    fun app f = let fun a2 (e::r) = (f e; a2 r) | a2 nil = () in a2 end
    fun revapp f = let fun a2 (e::r) = (a2 r; f e; ()) | a2 nil = () in a2 end
    fun nthtail(e,0) = e 
      | nthtail(e::r,n) = nthtail(r,n-1)
      | nthtail _ = raise NthTail
    fun nth x = hd(nthtail x) handle NthTail => raise Nth | Hd => raise Nth
    fun exists pred =
	let fun f nil = false
	      | f (hd::tl) = pred hd orelse f tl
	in  f
	end
  end

structure ByteArray  =
  struct
    open ByteArray
   local open System.Unsafe
    in
    val length = cast slength
    fun update(arg as (s,i,c)) =
	if i<0 orelse i >= slength s then raise Subscript
	else if c<0 orelse c>255 then raise Range
	else store arg
    val op sub = fn (s,i) => if i<0 orelse i>= slength s then raise Subscript
			     else ordof(s,i)
    fun extract(ba,i,1) = if i<0 orelse i >= slength ba then raise Subscript
			  else cast(ordof(ba,i))
      | extract(s,i,len) = 
	  if i<0 orelse i+len > slength s orelse len<0 then raise Subscript
	  else if len=0 then Assembly.bytearray0
	  else let val a = Assembly.A.create_b len
		   fun copy j =  if j=len then ()
				 else (store(a,j,ordof(s,i+j)); copy(j+1))
	       in  copy 0; a
	       end
    fun app f ba = 
	let val len = slength ba
	    fun app' i = if i >= len then ()
			 else (f(ba sub i); app'(i+1))
	in  app' 0
	end
    fun revapp f ba = 
	let fun revapp' i = if i < 0 then ()
			    else (f(ba sub i); revapp'(i-1))
	in  revapp'(slength ba - 1)
	end
    fun fold f ba x = 
	let fun fold'(i,x) = if i < 0 then x else fold'(i-1, f(ordof(ba,i),x))
	in  fold'(slength ba - 1, x)
	end
    fun revfold f ba x = 
	let val len = slength ba
	    fun revfold'(i,x) = if i >= len then x
				else revfold'(i+1,f(ordof(ba,i),x))
	in  revfold'(0,x)
	end
    end
  end


structure String =
  struct
    open String
    local open System.Unsafe
	  val op > = Integer.> and op >= = Integer.>=
	  val op < = Integer.< and op <= = Integer.<=
     in
    fun length s = if boxed s then slength s else 1

    val size = length
    fun substring("",0,0) = "" (* never call create_s with 0 *)
      | substring("",_,_) = raise Substring
      | substring(s,i,0) = if i>=0 
			    then if boxed s then if i <= slength s
					         then "" else raise Substring
 					    else if i<=1 
					         then "" else raise Substring
			    else raise Substring
      | substring(s,0,1) = if boxed s then cast(ordof(s,0)) else s
      | substring(s,i,1) =
	     if boxed s then if i>=0 andalso i < slength s 
				    then cast(ordof(s,i))
				    else raise Substring
			else if i=0 then s else raise Substring
      | substring(s,i,len) = 
	  if boxed s andalso i>=0 andalso i+len <= slength s
		andalso len >= 0
	  then let val a = Assembly.A.create_s(len)
		   fun copy j = if j=len then ()
				else (store(a,j,ordof(s,i+j)); copy(j+1))
	       in  copy 0; a
	       end
	  else raise Substring

    fun explode s =
	  if boxed s
	    then let fun f(l,~1) = l
		       | f(l, i) = f(cast(ordof(s,i)) :: l, i-1)
		  in f(nil, slength s - 1)
		 end
	    else [s]
    fun op ^ ("",s) = s
      | op ^ (s,"") = s
      | op ^ (x,y) =
	  if boxed x 
	  then if boxed y
	       then let val xl = slength x and yl = slength y
			val a = Assembly.A.create_s(xl+yl)
			fun copyx n = if n=xl then ()
			      else (store(a,n,ordof(x,n)); copyx(n+1))
			fun copyy n = if n=yl then ()
			      else (store(a,xl+n,ordof(y,n)); copyy(n+1))
		     in copyx 0; copyy 0; a
		    end
	      else let val xl = slength x
		       val a = Assembly.A.create_s(xl+1)
			fun copyx n = if n=xl then ()
			      else (store(a,n,ordof(x,n)); copyx(n+1))
		    in copyx 0; store(a,xl,cast y); a
		   end
	  else if boxed y		       
	       then let val yl = slength y
			val a = Assembly.A.create_s(1+yl)
			fun copyy n = if n=yl then ()
			      else (store(a,1+n,ordof(y,n)); copyy(n+1))
		     in store(a,0,cast x); copyy 0; a
		    end
	      else let val a = Assembly.A.create_s 2
		    in store(a,0,cast x); store(a,1,cast y); a
		   end
    fun chr i = if i<0 orelse i>255 then raise Chr else cast i
    fun ord "" = raise Ord
      | ord s = if boxed s then ordof(s,0) else cast s
    val ordof = fn (s,i) =>
	  if boxed s
            then if i<0 orelse i>= slength s then raise Ord else ordof(s,i)
	    else if i=0 then cast s else raise Ord
    fun implode (sl:string list) =
	  let val len = List.fold(fn(s,l) => length s + l) sl 0
	  in  case len
	       of 0 => ""
		| 1 => let fun find (""::tl) = find tl
			     | find (hd::_) = cast hd
			     | find nil = "" (* impossible *)
		       in  find sl
		       end
		| _ => let val new = Assembly.A.create_s len
			   fun copy (nil,_) = ()
			     | copy (s::tl,base) =
				let val len = length s
				    fun copy0 0 = ()
				      | copy0 i =
					let val next = i-1
					in  store(new,base+next,ordof(s,next));
					    copy0 next
					end
				in  copy0 len;
				    copy(tl,base+len)
				end
			in  copy(sl,0);
			    new
			end
	  end
    end (* local *)
  end  (* structure String *)

structure General =
  struct
    open General
    fun f o g = fn x => f(g x)
    fun a before b = a
  end (* structure General *)

structure Array =
  struct
   open Array
   local open System.Unsafe in
    val op sub : 'a array * int -> 'a =
	  fn (a,i) =>
	     if i<0 orelse i >= length a then raise Subscript
	     else subscript(a,i)
    val update : 'a array * int * 'a -> unit =
	  fn (a,i,v) => 
	     if i<0 orelse i >= length a then raise Subscript
	     else update(a,i,v)
   end (* local open ... *)
  end (* structure Array *)

structure Integer =
  struct
    open Integer
    fun op mod(a:int,b:int):int = a-((a div b) * b)
    fun min(a,b) = if a<b then a else b
    fun max(a,b) = if a>b then a else b
    fun abs a = if a<0 then ~a else a
  end

 val inc = Ref.inc
 val dec = Ref.dec
 val hd = List.hd and tl = List.tl
 val null = List.null and length = List.length
 val op @ = List.@ and rev = List.rev
 val map = List.map and fold = List.fold and revfold=List.revfold
 val app = List.app and revapp = List.revapp
 val nthtail = List.nthtail and nth = List.nth and exists = List.exists
 val substring = String.substring and explost = String.explode
 val op ^ = String.^ and chr = String.chr and ord = String.ord
 val implode=String.implode
 val op o = General.o and op before = General.before
 val op sub = Array.sub and update= Array.update
 val min = Integer.min and max = Integer.max 

end

(* Copyright 1989 by AT&T Bell Laboratories *)
structure Interp : sig val interp : Lambda.lexp -> 'a end = struct


open Access Basics Lambda ErrorMsg
structure U = System.Unsafe
val cast = U.cast
datatype 'a env = BIND of 'a env * lvar * 'a

val MTENV : 'a env = cast()

fun realc s =
  let val (sign,s) = case explode s of "~"::rest => (~1.0,rest) | s => (1.0,s)
      fun j(exp,d::dl,mant) = j(exp,dl,mant * 0.1 + real(d))
        | j(0,nil,mant) = mant*sign
        | j(exp,nil,mant) = if exp>0 then j(exp-1,nil,mant*10.0)
				     else j(exp+1,nil,mant*0.1)
      fun h(esign,wholedigits,diglist,exp,nil) = 
			j(esign*exp+wholedigits-1,diglist,0.0)
        | h(es,fd,dl,exp,d::s) = h(es,fd,dl,exp*10+(ord d - ord "0"),s)
      fun g(i,r,"E"::"~"::s)=h(~1,i,r,0,s)
	| g(i,r,"E"::s)=h(1,i,r,0,s)
	| g(i,r,d::s) = g(i, (ord d - ord "0")::r, s)
	| g(i,r,nil) = h(1,i,r,0,nil)
      fun f(i,r,"."::s)=g(i,r,s)
        | f(i,r,s as "E"::_)=g(i,r,s)
        | f(i,r,d::s) = f(i+1,(ord(d)-ord("0"))::r,s)
   in f(0,nil,s)
  end

fun look v =
 let fun f(BIND(e,w,x)) = if v=w then x else f e
  in f
 end

val upd = BIND

 val rec M = 
   fn APP(FN(v,b),a) => let val a' = M a and b' = M b
                         in fn r => cast(b' (upd(r,v, cast (a' r))))
                        end
    | APP(a,b) => cast let val a' = cast(M a) and b' = M b
                   in fn r => cast((a' r) (b' r))
                  end
    | FN(v,b) => let val b' = M b
                  in fn r => cast (fn x =>  b' (upd(r,v,x)))
                 end
    | SELECT(i,a) => let val a' = cast(M a) 
		      in fn r => cast(U.subscript(a' r, i))
		     end
    | RECORD nil => (fn r => cast())
    | RECORD [a,b] => let val a' = M a and b' = M b
		       in fn r => cast (a' r, b' r)
		      end
    | RECORD [a,b,c] => let val a' = M a and b' = M b and c' = M c
		       in fn r => cast (a' r, b' r, c' r)
		      end
    | RECORD [a,b,c,d] => let val a' = M a and b' = M b
			      and c' = M c and d' = M d
		       in fn r => cast (a' r, b' r, c' r, d' r)
		      end
    | RECORD [a,b,c,d,e] => let val a' = M a and b' = M b
			      and c' = M c and d' = M d
			      and e' = M e
		       in fn r => cast (a' r, b' r, c' r, d' r, e' r)
		      end
    | RECORD [a,b,c,d,e,f] => let val a' = M a and b' = M b
			      and c' = M c and d' = M d
			      and e' = M e and f' = M f
		       in fn r => cast (a' r, b' r, c' r, d' r, e' r, f' r)
		      end
    | RECORD l => let val l' = map M l
		   in fn r => (cast arrayoflist)(map (fn x => x r) l')
		  end
    | INT i => (fn r => cast i)
    | STRING s => (fn r => cast s)
    | REAL s => let val x = realc s in fn r => cast x end
    | PRIM P.cast => (fn r => cast(fn x => x))
    | PRIM P.+ => (fn r => cast Integer.+)  
    | PRIM P.- => (fn r => cast Integer.-)
    | PRIM P.* => (fn r => cast Integer.*)
    | PRIM P.div => (fn r => cast Integer.div)
    | PRIM P.orb => (fn r => cast Bits.orb)
    | PRIM P.andb => (fn r => cast Bits.andb)
    | PRIM P.xorb => (fn r => cast Bits.xorb)
    | PRIM P.rshift => (fn r => cast Bits.rshift)
    | PRIM P.lshift => (fn r => cast Bits.lshift)
    | PRIM P.! => (fn r => cast !)
    | PRIM P.:= => (fn r => cast op :=)
    | PRIM P.unboxedassign => (fn r => cast op :=)
    | PRIM P.makeref => (fn r => cast ref)
    | PRIM P.~ => (fn r => cast Integer.~)
    | PRIM P.ieql => (fn r => cast (fn(a:int,b) => a=b))
    | PRIM P.ineq => (fn r => cast (fn(a:int,b) => a<>b))
    | PRIM P.> => (fn r => cast Integer.>)
    | PRIM P.< => (fn r => cast Integer.<)
    | PRIM P.>= => (fn r => cast Integer.>=)
    | PRIM P.<= => (fn r => cast Integer.<=)
    | PRIM P.subscript => (fn r => cast U.subscript)
    | PRIM P.update => (fn r => cast U.update)
    | PRIM P.unboxedupdate => (fn r => cast U.update)
    | PRIM P.alength => (fn r => cast Array.length)
    | PRIM P.slength => (fn r => cast String.size)
    | PRIM P.store => (fn r => cast U.store)
    | PRIM P.ordof => (fn r => cast U.ordof)
    | PRIM P.fneg => (fn r => cast Real.~)
    | PRIM P.fadd => (fn r => cast Real.+)
    | PRIM P.fdiv => (fn r => cast Real./)
    | PRIM P.fmul => (fn r => cast Real.*)
    | PRIM P.fsub => (fn r => cast Real.-)
    | PRIM P.feql => (fn r => cast (fn(a:real,b)=>a=b))
    | PRIM P.fneq => (fn r => cast (fn(a:real,b)=>a<>b))
    | PRIM P.fgt => (fn r => cast Real.>)
    | PRIM P.fge => (fn r => cast Real.>=)
    | PRIM P.fle => (fn r => cast Real.<=)
    | PRIM P.flt => (fn r => cast Real.<)
    | PRIM P.boxed => (fn r => cast U.boxed)
    | PRIM P.callcc => (fn r => cast callcc)
    | PRIM P.throw => (fn r => cast throw)
    | PRIM _ => impossible "bad primop in interp"
    | VAR v => look v
    | HANDLE(a,b) => let val a' = cast (M a) and b' = cast(M b)
                      in fn r => (a' r handle e => b' r e)
                     end
    | RAISE a => let val a' = cast (M a) in fn r => raise(a' r) end
    | FIX(nl,fl,b) => 
         let fun g(n::nl,f::fl) = let val f' = M f
			              val fl' = g(nl,fl)
                                   in fn rr => cast (upd(fl' rr,n, 
						fn x => cast(f'(!rr)) x))
                                  end
                | g(nil,_) = cast(fn rr => !rr)
             val l = g(nl,fl)
             val b' = cast(M b)
          in fn r => cast (let val rr = ref (cast r)
		            in rr := l (cast rr); b'(!rr)
                           end)
         end
   | SWITCH(e,l as (DATAcon(DATACON{rep=VARIABLE _,...}), _)::_, SOME d) =>
     let fun trans(DATAcon(DATACON{rep=VARIABLE(PATH p), const=true,...}), a)=
		(rev(1::p), M a)
           | trans(DATAcon(DATACON{rep=VARIABLE(PATH p), const=false,...}), a)=
		(rev p, M a)
         val cases = map trans l and d' = M d and e' = M e
     in fn r => cast(let val e'' : int = U.subscript(cast(e' r),1)
		    fun select(x,i::rest) = select(U.subscript(cast x,i),rest)
		      | select(x,nil) = cast x
		    fun find ((v::path, answer)::rest) =
			 if select(look v r,path)=e'' then answer r
			    else find rest
	              | find nil = d' r
		 in find cases
		end)
     end
     | SWITCH(e,l as (REALcon _, _)::_, SOME d) =>
     let fun trans(REALcon i, a)= (realc i, M a)
         val cases = map trans l and d' = M d and e' = M e
     in fn r => cast (let val e'':real = cast(e' r)
		    fun find ((i, answer)::rest) =
			 if i=e'' then answer r else find rest
	              | find nil = d' r
		 in find cases
		end)
     end
     | SWITCH(e,l as (INTcon _, _)::_, SOME d) =>
     let fun trans(INTcon i, a)= (i, M a)
         val cases = map trans l and d' = M d and e' = M e
     in fn r => cast (let val e'':int = cast(e' r)
		    fun find ((i, answer)::rest) =
			 if i=e'' then answer r else find rest
	              | find nil = d' r
		 in find cases
		end)
     end
     | SWITCH(e,l as (STRINGcon _, _)::_, SOME d) =>
     let fun trans(STRINGcon i, a)= (i, M a)
         val cases = map trans l and d' = M d and e' = M e
     in fn r => cast(let val e'':string = cast(e' r)
		    fun find ((i, answer)::rest) =
			 if i=e'' then answer r else find rest
	              | find nil = d' r
		 in find cases
		end)
     end
   | SWITCH(e, l as (DATAcon(DATACON _), _)::_, d) =>
     let val d' = case d of SOME d0 => M d0
                          | NONE => fn r =>  impossible "no default in interp"
         val e' = M e
         fun f((DATAcon(DATACON{rep=CONSTANT i,...}),ans)::rest) =
		let val rest' = f rest
		    val ans' = M ans
	         in fn x => if x=i then ans' else rest' x
		end
           | f((DATAcon(DATACON{rep=TAGGED i,...}),ans)::rest) =
		let val rest' = f rest
		    val ans' = M ans
	         in fn x => if U.boxed x andalso U.subscript(cast x,1)=i 
				then ans' else rest' x
		end
           | f((DATAcon(DATACON{rep=TRANSPARENT,...}),ans)::rest) =
		let val rest' = f rest
		    val ans' = M ans
	         in fn x => if U.boxed x
				then ans' else rest' x
		end
           | f((DATAcon(DATACON{rep=TRANSB,...}),ans)::rest) =
		let val rest' = f rest
		    val ans' = M ans
	         in fn x => if U.boxed x
				then ans' else rest' x
		end
           | f nil = fn x => d'
	 val cases = f l
      in fn r => cases(e' r) r
     end

 fun interp lexp = cast(M lexp MTENV)


end

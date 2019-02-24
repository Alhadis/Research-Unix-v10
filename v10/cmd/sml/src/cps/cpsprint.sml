(* Copyright 1989 by AT&T Bell Laboratories *)
structure CPSprint =
struct

open CPS Prim

fun show say ctable =
  let fun sayv v =
	(case ctable v
	  of INTconst i => say(makestring i)
	   | REALconst r => say r
	   | STRINGconst s => (say "\""; say s; say "\"")
	) handle Ctable => say (Access.lvarName v)
      fun sayvlist [v] = sayv v
        | sayvlist nil = ()
	| sayvlist (v::vl) = (sayv v; say ","; sayvlist vl)
      fun saypath(OFFp 0) = ()
	| saypath(OFFp i) = (say "+"; say(makestring i))
	| saypath(SELp(j,p)) = (say "."; say(makestring j); saypath p)
      fun sayvp (v,path) = (sayv v; saypath path)
      fun saylist f [x] = f x | saylist f nil = () 
	| saylist f (x::r) = (f x; say ","; saylist f r)
      fun indent n =
	let fun space 0 = () | space k = (say " "; space(k-1))
	    fun nl() = say "\n"
    	    val rec f =
	     fn RECORD(vl,v,c) =>
		    (space n; say "{"; saylist sayvp vl; say "} -> "; sayv v;
		     nl(); f c)
	      | SELECT(i,v,w,c) =>
		    (space n; sayv v; say "."; say(makestring i); say " -> ";
		     sayv w; nl(); f c)
	      | OFFSET(i,v,w,c) =>
		    (space n; sayv v; say "+"; say(makestring i); say " -> ";
		    sayv w; nl(); f c)
	      | APP(w,vl) => 
		    (space n; sayv w; say "("; sayvlist vl; say ")\n")
	      | FIX(bl,c) =>
		    let fun g(v,wl,d) = 
			    (space n; sayv v; say "("; sayvlist wl;
			     say ") =\n"; indent (n+3) d)
		     in app g bl; f c
		    end
	      | SWITCH(v,cl) =>
		   let fun g(i,c::cl) =
			(space(n+1); say(makestring(i:int));
			 say " =>\n"; indent (n+3) c; g(i+1,cl))
			 | g(_,nil) = ()
		    in space n; say "case "; sayv v; say " of\n"; g(0,cl)
		   end
	      | PRIMOP(_,nil,nil,nil) => ()
	      | PRIMOP(i,vl,wl,[c]) =>
		   (space n; say(inLineName i); say "("; sayvlist vl;
		    say ") -> "; sayvlist wl; nl(); f c)
	      | PRIMOP(i,vl,nil,[t,f]) =>
	          (space n; say "if "; say(inLineName i);
			 say "("; sayvlist vl; say ") then\n";
		    indent (n+3) t;
		    space n; say "else\n";
		    indent (n+3) f
		   )
	      | PRIMOP(i,vl,wl,cl) =>
	          (space n; say "case "; say(inLineName i);
			 say "("; sayvlist vl; say ") -> ";
			sayvlist wl; say " of\n";
		   let fun g(i,c::cl) =
			(space(n+1); say(makestring i); say " =>\n";
			    indent (n+3) c; g(i+1,cl))
			 | g(_,nil) = ()
		    in g(0,cl)
		   end)
         in f
        end
 in  indent 0
 end
end

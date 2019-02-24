functor Parse(F : FORMULA) : PARSE = 
struct
  structure F = F
  
  exception Syntax of string

  datatype token = NUMtok of int 
		 | ALPHAtok of string
		 | PUNCTtok of string 
		 | EOFtok

  fun for (i,j) f = if i<=j then (f i; for(i+1,j) f) else ()
  fun forall (a::r) f = (f a; forall r f)
    | forall nil f = ()

  datatype class = DIGIT | BLANK | ALPHA | PUNCT | OTHER | EOF
  val class = array(257,OTHER)
  val _ = 
      (for (ord "0", ord "9") (fn s => update(class,s,DIGIT));
       for (ord "a", ord "z") (fn s => update(class,s,ALPHA));
       for (ord "A", ord "Z") (fn s => update(class,s,ALPHA));
       forall [" ","\t","\n"] (fn s => update(class,(ord s),BLANK));
       forall ["(",")","[","]",",","+","-","*","/"]
	       (fn s => update(class,(ord s),PUNCT));
       update(class, 256, EOF))
	      
  fun parse(str : string) : F.formula =
    let fun gettoken pos = 
	    let fun char(p) = ordof(str,p) handle Ord => 256
	        fun digit(z,p) = 
		    let val c = char p
		     in case class sub c 
			 of DIGIT => digit(z*10+c-ord("0"), p+1)
			  | _ => (p, NUMtok z)
		    end
		fun alpha(s,p) =
		    let val c = char p
		     in case class sub c 
			 of ALPHA => alpha(s,p+1)
			  | _ => (p, ALPHAtok(substring(str,s,p-s)))
		    end
		val c = char pos
	     in case class sub c
	         of BLANK => gettoken(pos+1)
		  | ALPHA => alpha(pos,pos)
		  | DIGIT => digit(0,pos)
		  | PUNCT => (pos+1, PUNCTtok(chr c))
		  | EOF => (pos, EOFtok)
		  | _ => raise (Syntax "illegal character")
	    end

	fun atom (p, NUMtok n)  =  (gettoken p, F.NUM n)
	  | atom (p, PUNCTtok "[") =
		(case exp(gettoken p)
		  of ((p1, PUNCTtok ","), e1) =>
			   (case exp(gettoken p1)
			     of ((p2, PUNCTtok "]"), e2) =>
				      (gettoken p2, F.CELLREF(e1,e2))
			      | _ => raise (Syntax "] expected"))
	           | _ => raise (Syntax ", expected"))
          | atom (p, PUNCTtok "(") =
		(case exp(gettoken p)
		  of ((p1, PUNCTtok ")"), e1) => (gettoken p1, e1)
	           | _ => raise (Syntax ") expected"))
	  | atom _ = raise (Syntax "bogus atom")

	and term' ((p, PUNCTtok "*"), e1) = 
	    let val (s, e2) = atom(gettoken p)
	     in term'(s, F.BINOP(Integer.*, e1, e2))
            end
	  | term' ((p, PUNCTtok "/"), e1) = 
	    let val (s, e2) = atom(gettoken p)
	     in term'(s, F.BINOP(Integer.div, e1, e2))
            end
	  | term' x = x

        and term s = term' (atom s)

	and exp' ((p, PUNCTtok "+"), e1) = 
	    let val (s, e2) = term(gettoken p)
	     in exp'(s, F.BINOP(Integer.+, e1, e2))
            end
	  | exp' ((p, PUNCTtok "-"), e1) = 
	    let val (s, e2) = term(gettoken p)
	     in exp'(s, F.BINOP(Integer.-, e1, e2))
            end
	  | exp' x = x

	and exp (p, ALPHAtok "if") = 
	    (case exp(gettoken p)
	      of ((p',ALPHAtok "then"),e1) =>
		(case exp(gettoken p')
	          of ((p'', ALPHAtok "else"),e2) =>
		     (case exp(gettoken p'')
		       of (s,e3) => (s, F.IF(e1,e2,e3)))
		   | _ => raise (Syntax "else expected"))
               | _ => raise (Syntax "then expected"))
	  | exp s = exp' (term s)

     in case exp (gettoken 0)
         of ( (_, EOFtok), e) => e
          | _ => raise (Syntax "garbage at end of formula")
    end

end

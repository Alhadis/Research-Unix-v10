functor Spreadsheet(P : PARSE) : SPREADSHEET =
struct

 structure F = P.F

 fun array2(i,j,v) =  
    let fun init 0 = nil | init i = array(j,v) :: init(i-1)
   in arrayoflist(init i)
  end
 fun update2(a,i,j,v) = update(a sub i, j, v)

 val dim = 10

 val vtable = array2(dim,dim,0)
 val ftable = array2(dim,dim,F.eval(P.parse "0"))
 val stable = array2(dim,dim,"0")

 fun set(i,j,s) = (update2(ftable,i,j, F.eval(P.parse s));
		   update2(stable,i,j,s))

 fun for (i,j) f = if i<=j then (f i; for(i+1,j) f) else ()

 fun eval() = for(0, dim-1)
	        (fn i => for(0, dim-1)
			    (fn j => update2(vtable,i,j,
				     	     (ftable sub i sub j) vtable)));

 fun get(i,j) =  (stable sub i sub j, vtable sub i sub j)
end


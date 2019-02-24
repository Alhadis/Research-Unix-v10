(* Generators and Iterators *)

exception DONE

(* An arithmetic range generator: numbers from n upto (or downto) m *)
infix to;
fun (n:int) to (m:int) =
    let val dir = if n<=m then 1 else ~1;
	val pos = ref(n-dir);
	val test = if n<=m then (fn () => !pos>m) else (fn () => !pos<m)
     in (fn () => (pos := !pos+dir; 
		   if test() then raise DONE else !pos))
    end

(* Exercise: generalize this to take a step parameter. *)


(* A list generator: generates elements of a list in succession *)
fun over l =
    let val rest = ref l
     in fn () => case !rest
		   of []  => raise DONE
		    | x::l  => (rest:=l; x)
    end


(* A loop iterator *)
fun loop gen f = (f(gen ()); loop gen f) handle DONE => ()

(* A collect iterator -- collects results in a list *)
fun collect gen f = f(gen())::(collect gen f) handle DONE => [];


(* examples *)
loop (3 to 5) (fn (n:int) => print((makestring n)^"\n"));

collect (3 to 5) (fn n => n*n);

fun sq (x:int) = x*x;
collect (over [1,2,3]) sq;


(* Exercises:
1. Define iterators over pairs of generators.
2. Define a generator that produces the first n elements of a stream.
3. Define a function that maps generators to streams.
4. Define a binary tree type and generators that traverse trees.
5. Try to think of some other interesting iterators beside "loop" and
   "collect".
*)

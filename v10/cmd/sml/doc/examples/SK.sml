(* ----- An Interpreter for Call-by-Value SK-calculus ----- *)

datatype Term = S | K | appl of Term * Term;

fun Eval(appl(a,b)) = Apply(appl(Eval a,Eval b)) |
    Eval a = a

and Apply(appl(appl(K,a),b)) = a |
    Apply(appl(appl(appl(S,a),b),c)) = Eval(appl(appl(a,c),appl(b,c))) |
    Apply a = a;

infix *;
fun f * g = appl(f,g);      (* abbreviation for application *)

val I = Eval(S * K *  K);   (* I a = a *)

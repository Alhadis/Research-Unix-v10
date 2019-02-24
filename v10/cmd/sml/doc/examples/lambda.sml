(* ----- An Interpreter for Call-by-Value lambda-calculus ----- *)

datatype ide = IDE of string

datatype term = VAR of ide | LAMB of ide * term | APPL of term * term

datatype env = ENV of ide -> value
     and value = CLOSURE of term * env

exception Unbound_var of string

val Arid = ENV(fn (IDE s) => raise Unbound_var s)

fun LookUp(ide, ENV f) = f ide

fun Extend(ide, value, env) =
      ENV(fn ide' => if ide'=ide then value else LookUp(ide',env))

fun Eval(VAR ide, env) = LookUp(ide,env) |
    Eval(lamb as LAMB _, env) = CLOSURE(lamb,env) |
    Eval(APPL(rator,rand), env) = Apply(Eval(rator,env),Eval(rand,env))

and Apply(CLOSURE(LAMB(bind,body),env),arg) =
      Eval(body,Extend(bind,arg,env))

fun i s = IDE s            (* abbreviation for identifiers *)
and v s = VAR(IDE s)       (* abbreviation for variables *)

infix *
fun f * g = APPL(f,g)      (* abbreviation for application *)

val I = LAMB(i"a",v"a")                       (* I a = a *)
val K = LAMB(i"a",LAMB(i"b",v"a"))            (* K a b = a *)
val S = LAMB(i"a",LAMB(i"b",LAMB(i"c",        (* S a b c = (a c) (b c) *)
           (v"a" * v"c") * (v"b" * v"c"))))
val D = LAMB(i"a", v"a" * v"a")               (* D a = a a *)





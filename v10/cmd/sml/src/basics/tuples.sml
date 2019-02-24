(* Copyright 1989 by AT&T Bell Laboratories *)
signature TUPLES = sig
  structure Basics : BASICS
  val numlabel : int -> Basics.label
  val mkTUPLEtyc : int -> Basics.tycon
  val isTUPLEtyc : Basics.tycon -> bool
  val mkRECORDtyc : Basics.label list -> Basics.tycon
end

structure Tuples : TUPLES = struct

structure Basics = Basics

structure Labels =
  struct datatype labelOpt = NOlabel | SOMElabel of Basics.label
         datatype tyconOpt = NOtycon | SOMEtycon of Basics.tycon
  end

open Labels Basics

structure LabelArray =
    Dynamic (struct open Array
	       type array = Labels.labelOpt array
	       type elem = Labels.labelOpt
	     end)

structure TyconArray =
    Dynamic (struct open Array
	       type array = Labels.tyconOpt array
	       type elem = Labels.tyconOpt
	     end)

exception New
val tyconTable = IntStrMap.new(32,New) : tycon IntStrMap.intstrmap
val tyconMap = IntStrMap.map tyconTable
val tyconAdd = IntStrMap.add tyconTable

fun labelsToSymbol(labels: label list) : Symbol.symbol =
    let fun wrap [] = ["}"]
	  | wrap [id] = [Symbol.name id, "}"]
	  | wrap (id::rest) = Symbol.name id :: "," :: wrap rest
     in Symbol.symbol(implode("{" :: wrap labels))
    end

fun mkRECORDtyc(labels: label list) : tycon =
    let val recordName = labelsToSymbol labels
        val number = Symbol.number recordName
        val name = Symbol.name recordName
     in tyconMap(number,name)
	handle New =>
	  let val tycon = TYCON{stamp = Stampset.newStamp(Stampset.fixedTycStamps),
				arity = length labels,
				eq    = ref YES,
				path  = [recordName],
				kind  = RECORDtyc labels}
	   in tyconAdd(number,name,tycon);
	      tycon
	  end
    end

val numericLabels = LabelArray.array(NOlabel)
val tupleTycons = TyconArray.array(NOtycon)

fun numlabel i =
    case LabelArray.sub(numericLabels,i)
      of NOlabel =>
	   let val newlabel = Symbol.symbol(makestring i)
	    in LabelArray.update(numericLabels,i,SOMElabel(newlabel));
	       newlabel
	   end
       | SOMElabel(label) => label

fun numlabels n =
    let fun labels (0,acc) = acc
	  | labels (i,acc) = labels (i-1, numlabel i :: acc)
    in labels (n,nil)
    end

fun mkTUPLEtyc n =
    case TyconArray.sub(tupleTycons,n)
      of NOtycon =>
           let val tycon = mkRECORDtyc(numlabels n)
	    in TyconArray.update(tupleTycons,n,SOMEtycon(tycon));
	       tycon
	   end
       | SOMEtycon(tycon) => tycon

fun checklabels (2,nil) = false   (* {1:t} is not a tuple *)
  | checklabels (n,nil) = true
  | checklabels (n, lab::labs) = 
	Symbol.eq(lab, numlabel n) andalso checklabels(n+1,labs)

fun isTUPLEtyc(TYCON{kind=RECORDtyc labels,...}) = checklabels(1,labels)
  | isTUPLEtyc _ = false
    
end (* structure Tuples *)

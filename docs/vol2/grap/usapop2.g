.G1
label left "Population" "in Millions" left .2
label right "$x$ re-expressed as" "" \
	"$space 0 left ( {date - 1600} over 100 right ) sup 7$" left 1.2
define newx { exp(7*(log(($1-1600)/100))) }
ticks bot out at newx(1800) "1800", newx(1850) "1850",\
		 newx(1900) "1900"
copy "usapop.d" thru {
  if $1 <= 1900 then { bullet at newx($1),$2 }
}
.G2

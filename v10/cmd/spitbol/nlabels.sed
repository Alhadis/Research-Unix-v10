/^cnc16/{
n
n
s/beqlu[ 	]*cnc08/bnequ 0f; jbr cnc08; 0:/
}
/^srh01/{
n
s/bgtru[ 	]*srh03/blequ 0f; jbr srh03; 0:/
}
/	jmp	\([^(*]\)/s//	jbr	\1/
/	jbr	\(er_26[1267]\)/s//	jmp	\1/

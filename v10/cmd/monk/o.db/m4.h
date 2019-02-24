define(`DEFINE_NUMBER', `define($1,$2)')
define(`DEFINE_STRING_ARRAY', `changequote`'define($1,$2\n$`'1)`'changequote(+Q+,-Q-)')
define(`DEFINE_STRING', `define($1,$2)')
define(`UNEQUAL_STRING_REGISTERS',`changequote`'ifelse(eval(len($1)==1&&len($2)==1),1,"!@\*$1@\*$2@",eval(len($1)==1&&len($2)==2),1,"!@\*$1@\*`('$2@",eval(len($1)==2&&len($2)==1),1,"!@\*`('$1@\*$2@","!@\*`('$1@\*`('$2@")`'changequote(+Q+,-Q-)')
define(`EQUAL_STRING',`changequote`'ifelse(len($1),1,"@\*$1@$2@","@\*`('$1@$2@")`'changequote(+Q+,-Q-)')
define(`UNEQUAL_STRING',`changequote`'ifelse(len($1),1,"!@\*$1@$2@","!@\*`('$1@$2@")`'changequote(+Q+,-Q-)')
define(`NULL_STRING', `changequote`'ifelse($#,1,"@\*`('$1@@",$#,2,"@\*`('$1\*`('$2@@",$#,3,"@\*`('$1\*`('$2\*`('$3@@",$#,4,"@\*`('$1\*`('$2\*`('$3\*`('$4@@","$#")`'changequote(+Q+,-Q-)')
define(`NOT_NULL_STRING', `changequote`'ifelse($#,1,"!@\*`('$1@@",$#,2,"!@\*`('$1\*`('$2@@",$#,3,"!@\*`('$1\*`('$2\*`('$3@@",$#,4,"!@\*`('$1\*`('$2\*`('$3\*`('$4@@")`'changequote(+Q+,-Q-)')
define(`GT_NUMBER',`changequote`'ifelse(len($1),1,"\n$1>$2","\n`('$1>$2")`'changequote(+Q+,-Q-)')
define(`GE_NUMBER',`changequote`'ifelse(len($1),1,"\n$1>=$2","\n`('$1>=$2")`'changequote(+Q+,-Q-)')
define(`EQ_NUMBER',`changequote`'ifelse(len($1),1,"\n$1=$2","\n`('$1=$2")`'changequote(+Q+,-Q-)')
define(`LE_NUMBER',`changequote`'ifelse(len($1),1,"\n$1<=$2","\n`('$1<=$2")`'changequote(+Q+,-Q-)')
define(`GT_NUMBER_REGISTER',`changequote`'ifelse(eval(len($1)==1&&len($2)==1),1,"\n$1>\n$2",eval(len($1)==1&&len($2)==2),1,"\n$1>\n`('$2",eval(len($1)==2&&len($2)==1),1,"\n`('$1>\n$2","\n`('$1>\n`('$2")`'changequote(+Q+,-Q-)')
define(`GE_NUMBER_REGISTER',`changequote`'ifelse(eval(len($1)==1&&len($2)==1),1,"\n$1>=\n$2",eval(len($1)==1&&len($2)==2),1,"\n$1>=\n`('$2",eval(len($1)==2&&len($2)==1),1,"\n`('$1>=\n$2","\n`('$1>=\n`('$2")`'changequote(+Q+,-Q-)')
define(`LE_CONSTANT',`"$1<=$2"')
define(`EQUAL_MACRO_ARGUMENT', `"\$$1=$2"')
define(`EQUAL_DIVERSION', `"@\n(.z@$1@"')
changecom
undefine(`undefine', `defn', `pushdef', `popdef', `ifdef', `shift',
	`changecom', `divert', `undivert', `divnum', `dnl', `incr',
	`decr', `index', `substr', `translit', `sinclude',
	`syscmd', `sysval', `maketemp', `m4exit', `m4wrap', `errprint', `dumpdef',
	`traceon', `traceoff')
changequote(`+Q+',`-Q-')

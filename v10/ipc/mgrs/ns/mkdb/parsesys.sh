cat /usr/lib/uucp/Systems.local /usr/lib/uucp/Systems.dk /usr/lib/uucp/Systems /usr/lib/uucp/Systems.gen /usr/lib/uucp/Systems.tcp | awk '
#
#  Parse systems files.  All lines with the same system name are output
#  with the same id number.  Output format is
#	<sysname> TEL,<telno>
#	<sysname> <dkname> DOMAIN,<domain name>
#
BEGIN {
	line = "";
}
#
#  ignore blank and comment lines
#
$0 ~ /^#/	{next;}
NF<3 		{next;}
#
#	telephone access
#
$3 ~ /ACU/ {
	line = "TEL," $5
}
#
#	dk system
#
$3 ~ /DK/ {
	if(i = match($5,"\.uucp$"))
		dk = substr($5,1,i-1);
	else
		dk = $5;
	n = split(dk,a,"/");
	uid = a[n];
	for(j=n-1; j>=1; j--)
		uid = uid "." a[j];
	line = "DK," dk " DOMAIN," uid ".att.com"
}
#
#	systems that poll us
#
$2 ~ /[nN][Ee][Vv][Ee][Rr]/ {
	line = $1
}
#
#	output the line
#
{
	print $1 " " line " SERVICE,uucp"
}
' | uniq

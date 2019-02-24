s/^[0-9. ]*//
s/\\(em/-/
s/\\(dg//
s/\\f(..//
s/\\f[A-Z1-9]//g
s/\\&//
s/\\-/-/
s/"//g
s/\.$//
/Example/d
/Reference/d
/Introduction/d
/Acknowledgment/d
/Acknowledgement/d
/Discussion/d
/Reflections/d
/Pros /d
/CONCLUSION/d
/Conclusion/d
/Commentary/d
/REFERENCE/d
/INTRODUCTION/d
/Summary/d
/See Also/d
/Exercise/d
/Answer/d
/ANSWER/d
/ACKNOWLEDGEMENT/d
s/  / /
s/ $//

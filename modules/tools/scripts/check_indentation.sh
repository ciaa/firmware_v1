# remove this line when the generator gets stable
#
#   exclude-dir="out" \
#
INDENT=$( egrep --with-filename --line-number --recursive \
   -ve "^( {3})+" \
   -ve "^$" \
   -ve "^[a-zA-Z0-9]+" \
   -ve "^_" \
   -ve "^[-#}{?<\$\.]+" \
   -ve "^ *\*" \
   -ve "^ */" \
   --include="*.c" \
   --include="*.C" \
   --include="*.h" \
   --include="*.H" \
   --include="*.php" \
   --include="*.sh" \
   --exclude-dir="externals" \
   --exclude-dir="out" \
   --exclude-dir="\.git" \
   * )


INDENT=${INDENT}$( egrep --with-filename --line-number --recursive \
   -e "^ +	" \
   -e "^	" \
   --include="*.c" \
   --include="*.C" \
   --include="*.h" \
   --include="*.H" \
   --include="*.php" \
   --include="*.sh" \
   --exclude-dir="externals" \
   --exclude-dir="out" \
   --exclude-dir="\.git" \
   * )

if [ "$INDENT"  ] ; then
   echo "There is bad indentation"
   echo "$INDENT"
   exit 1
else
   echo "Indentation OK"
fi

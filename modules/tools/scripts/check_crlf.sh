# remove this line when the generator gets stable
#
#   exclude-dir="out" \
#
# keep synchronized with fixer

CRLF=$( egrep --recursive --files-with-matches "^" \
   --include="*.c" \
   --include="*.C" \
   --include="*.h" \
   --include="*.H" \
   --include="*.php" \
   --include="Makefile*" \
   --include="*.sh" \
   --exclude-dir="externals" \
   --exclude-dir="out" \
   --exclude-dir="\.git" \
   * )

if [ "$CRLF"  ] ; then
   echo "FAIL, there are files with DOS EOL (CRLF)"
   echo "$CRLF"
   exit 1
else
   echo "EOL OK"
fi


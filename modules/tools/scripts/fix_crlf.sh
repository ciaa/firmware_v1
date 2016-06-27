# remove this line when the generator gets stable
#
#   exclude-dir="out" \
#
# keep synchronized with checker

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
   for FILE in $CRLF; do
      echo "Fixing DOS EOL (CRLF): $FILE"
      dos2unix "$FILE"
   done
else
   echo "EOL OK"
fi


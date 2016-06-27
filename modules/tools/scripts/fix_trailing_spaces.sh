# remove this line when the generator gets stable
#
#   exclude-dir="out" \
#
# keep synchronized with checker

TRAILING=$( egrep --files-with-matches --recursive " +$" \
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

if [ "$TRAILING"  ] ; then
   for FILE in $TRAILING; do
      echo "Fixing trailing spaces: $FILE"
      sed -i -e 's/ *$//' "$FILE"
   done
else
   echo "Trailing spaces OK"
fi

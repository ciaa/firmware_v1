# remove this line when the generator gets stable
#
#   -and -not -wholename "./out/*" \
#

TRAILING=$(find . -type f \
   -and -not -wholename "./out/*" \
   -and -not -wholename "./.git*" \
   -and -not -wholename "./externals*" \
   -and -not -name "*.png" \
   -and -not -name "*~" \
   -and -not -name "*.zargo" \
   -and -not -name "*.axf" \
   -and -not -name "*.o" \
   -and -not -name "*.exe" \
   -and -not -name "*.a" \
   -and -not -name "*.swp" \
   -exec egrep -n -H " +$" {} \;)

if [ "$TRAILING"  ] ; then
   echo "FAIL, there are trailing spaces"
   echo "$TRAILING"
   exit 1
else
   echo "Trailing spaces OK"
fi

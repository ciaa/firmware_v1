In order to check "check_indentation.sh", you have to copy indentation.txt to indentation.h, run the check and remove indentation.h.

$ cp tests/tests/indentation_check.h.txt tests/tests/indentation_check.h 

$ ./check_indentation.sh  | grep fail | wc -l
0

$ rm tests/indentation_check.h


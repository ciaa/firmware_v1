#!/usr/bin/perl

use warnings;
use strict;
use File::Find;

############################# CONFIGURATION ###################################
###############################################################################
# set path to examples
my $examples_dir = "examples";

###############################################################################
# set output directory for CI
my $ci_out_dir   = "out/ci";

############################# END OF CONFIGURATION ############################
print "CIAA Firmware - Continuous Integration\n\n";
opendir my $examples, "$examples_dir/" or die "$0: opendir $!";

while (defined(my $example = readdir $examples)) {
   if ( ($example eq ".") || ($example eq "..") )
   {
      next;
   }
   print "Testing: $example\n";
   $ENV{'PROJECT'} = "$examples_dir/$example";
   system("make clean > $ci_out_dir/$example.log 2>&1");
   system("make generate >> $ci_out_dir/$example.log 2>&1");
   system("make >> $ci_out_dir/$example.log 2>&1");
}

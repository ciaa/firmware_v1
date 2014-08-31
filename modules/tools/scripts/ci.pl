#!/usr/bin/perl

use warnings;
use strict;
use File::Find;

use lib qw(modules/tools/scripts);
use Module;

############################# CONFIGURATION ###################################
###############################################################################
# set path to examples
my $modules_dir = "modules";

###############################################################################
# set output directory for CI
my $ci_out_dir   = "out/ci";

############################# END OF CONFIGURATION ############################
print "CIAA Firmware - Continuous Integration\n\n";
opendir my $modules, "$modules_dir/" or die "$0: opendir $!";
my @mods;

while (defined(my $module = readdir $modules)) {
   if ( ($module eq ".") || ($module eq "..") )
   {
      next;
   }
   push(@mods, new Module($module));
}

foreach my $mod (@mods)
{
   $mod->print();
   $mod->runTests();
}


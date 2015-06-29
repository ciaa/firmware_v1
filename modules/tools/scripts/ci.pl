#!/usr/bin/perl

use warnings;
use strict;
use File::Find;

use lib qw(modules/tools/scripts);
use Module;
use OutSummary;

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
   print "adding module to ci: $module\n";
   push(@mods, new Module($module));
}

# for each module
foreach my $mod (@mods)
{
   # run the tests of each module
   print "\nRunning tests for module: $mod->{_name}\n";
   $mod->runTests();
}

# create the coverage reports
my $results = `make results`;

my $out = new OutSummary("out/ci", @mods);

$out->genReport();

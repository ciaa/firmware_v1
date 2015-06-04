#!/usr/bin/perl

use warnings;
use strict;
use File::Find;

use lib qw(modules/tools/scripts);

############################# CONFIGURATION ###################################
###############################################################################
# set path to examples
my $modules_dir = "modules";

############################# END OF CONFIGURATION ############################
my $num_args = $#ARGV + 1;
if ($num_args != 2) {
   print "\nUsage: diff.pl tag1 master\n";
   exit;
}

my $from = $ARGV[0];
my $to = $ARGV[1];

print "CIAA Firmware - Differences between branches \n\n";
opendir my $modules, "$modules_dir/" or die "$0: opendir $!";
my @mods;

while (defined(my $module = readdir $modules)) {
   if ( ($module eq ".") || ($module eq "..") )
   {
      next;
   }
   print "***************************************************************\n";
   print "*** Differences for module: $module between \n";
   print "***************************************************************\n";
   my $cmd = "git diff $from $to $modules_dir\/$module";
   print "Running: $cmd\n";
   print "Differences: " . `$cmd | wc -l`;

}


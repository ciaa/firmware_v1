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
my @changed_mods;

while (defined(my $module = readdir $modules)) {
   if ( ($module eq ".") || ($module eq "..") )
   {
      next;
   }
   my $cmd = "git diff $from $to $modules_dir\/$module";
   #print "Running: $cmd\n";
   my $diff_count = `$cmd | wc -l`;
   chomp($diff_count);
   if ($diff_count != 0 )
   {
      print "***************************************************************\n";
      print "*** Differences for module: $module between $from .. $to\n";
      print "***************************************************************\n";
      print "Differences: " . $diff_count ."\n";
      my $cmd = "git diff $from $to --stat $modules_dir\/$module";
      print "Running: $cmd\n";
      print "Differences: " . `$cmd` . "\n";
   } else
   {
#      print "***************************************************************\n";
#      print "*** NO Differences for module: $module between $from .. $to\n";
#      print "***************************************************************\n";
   }

}


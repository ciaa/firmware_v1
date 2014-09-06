#!/usr/bin/perl

package OutSummary;
use Html;
use Time::Piece;

use strict;
use warnings;

sub new {
   my $class = shift;
   my $self = {
      _outdir => shift,
      _mods => [@_],
   };
   bless $self, $class;
   return $self;
}

use constant {
   STYLE_OK => 'style="background-color:#00D000"',
   STYLE_WARN => 'style="background-color:#D0D000"',
   STYLE_NOT_OK => 'style="background-color:#D00000"',
};

sub genReport
{
   my $self = shift;
   open FILE, ">", $self->{_outdir} . "/index.html";

   print FILE "<html>";
   print FILE "<title>CIAA Firmware Quality Report</title>";
   print FILE "<body>";
   print FILE "<h1>CIAA Firmware Quality Report</h1>";
   print FILE "<p>Generated....: " . Time::Piece::localtime->strftime('%F %T') . "</p>";
   print FILE "<p>GIT Revision.: " . `git log -1 --format="%H"` . "</p>";

   print FILE "<table border='1'>";

   print FILE "<tr><th>Module</th><th>Tested Files</th><th>Test Cases</th><th>Function Coverage</th><th>Line Coverage</th><th>Branch coverage</th></tr>";

   foreach my $mod (@{$self->{_mods}})
   {
      printf FILE "<tr><td><a href=\"#" . $mod->getName() . "\">" . $mod->getName() . "</a></td>";
      my $countOfTests = $mod->getCountOfTests();
      my $countOfFiles = $mod->getCountOfFiles();
      my $style = STYLE_NOT_OK;
      if ($countOfFiles == $countOfTests)
      {
         $style = STYLE_OK;
      }
      printf FILE "<td $style>" . $countOfTests . "/" . $countOfFiles . "</td>";
      printf FILE "<td>" . "</td>";
      printf FILE "<td>" . "</td>";
      printf FILE "<td>" . "</td>";
      printf FILE "<td>" . "</td></tr>";
   }

   print FILE "</table>";
   print FILE "</body></html>";

   foreach my $mod (@{$self->{_mods}})
   {
      # todo perform the report of each module
      printf FILE "<a name=\"$mod->{_name}\"/><h2>Report of Module: $mod->{_name}</h2>";

      print FILE "<table border =\"1\">";
      print FILE "<tr><th>File</th><th>Has test file</th><th>Test Cases</th><th>Function Coverage</th><th>Line Coverage</th><th>Branch coverage</th></tr>";
      foreach my $file ($mod->getFiles())
      {
         printf FILE "<tr><td><a href=\"../../" . $file->getReportFile() . "\">" . $file->getName() . "</a></td>";

         my $hasTest = "no";
         my $hasTestStyle = STYLE_NOT_OK;
         if ($file->hasTest()) {
            $hasTest = "yes";
            $hasTestStyle = STYLE_OK;
         }

         printf FILE "<td $hasTestStyle>" . $hasTest . "</td>";
         printf FILE "<td>" . "</td>";

         $hasTestStyle = STYLE_NOT_OK;
         if ($file->getFuncs() eq $file->getCovFuncs()) {
            $hasTestStyle = STYLE_OK;
         }
         printf FILE "<td $hasTestStyle>" . $file->getFuncs() . "/" . $file->getCovFuncs() . "</td>";

         $hasTestStyle = STYLE_NOT_OK;
         if ($file->getLines() eq $file->getCovLines()) {
            $hasTestStyle = STYLE_OK;
         }
         printf FILE "<td $hasTestStyle>" . $file->getLines() . "/" . $file->getCovLines() . "</td>";

         $hasTestStyle = STYLE_WARN;
         if ($file->getBranches() eq $file->getCovBranches()) {
            $hasTestStyle = STYLE_OK;
         }
         printf FILE "<td $hasTestStyle>" . $file->getBranches() . "/" . $file->getCovBranches() . "</td>";
      }
      print FILE "</table>";

#      foreach my $file ($mod->getFiles())
#      {
#         print FILE "<h3><a name=\"" . $file->getName() . "\"/>Report of file " . $file->getName() . "</h3>";
      #
      #   print FILE "<table border =\"1\">";
      #   print FILE "<tr><th>Test Case</th><th>Result</th></tr>";
      #   foreach my $file ($mod->getFiles())
      #   {
      #      printf FILE "<tr><td>" . "</td>";
      #      printf FILE "<td>" . $file->getCovFuncs() . "</td>";
      #      printf FILE "<td>" . "</td></tr>";
      #   }
      #   print FILE "</table>";
      #   print FILE "<h4>Not tested lines</h4>";
      #   print FILE "<p>TODO</p>";
#      }

   }

   close FILE;

}

1;

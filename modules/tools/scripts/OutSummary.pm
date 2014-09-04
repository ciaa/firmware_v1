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
      my $style = 'style="background-color:#D00000"';
      if ($countOfFiles == $countOfTests)
      {
         $style = 'style="background-color:#00D000"';
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
         printf FILE "<tr><td><a href=\"#" . $file->getName() . "\">" . $file->getName() . "</a></td>";

         my $hasTest = "no";
         my $hasTestStyle = 'style="background-color:#D00000"';
         if ($file->hasTest()) {
            $hasTest = "yes";
            $hasTestStyle = 'style="background-color:#00D000"';
         }

         printf FILE "<td $hasTestStyle>" . $hasTest . "</td>";
         printf FILE "<td>" . "</td>";
         printf FILE "<td>" . $file->getFunc() . "/" . $file->getCovFunc() . "</td>";
         printf FILE "<td>" . "</td>";
         printf FILE "<td>" . "</td></tr>";
      }
      print FILE "</table>";

      foreach my $file ($mod->getFiles())
      {
         print FILE "<h3><a name=\"" . $file->getName() . "\"/>Report of file " . $file->getName() . "</h3>";

         print FILE "<table border =\"1\">";
         print FILE "<tr><th>Test Case</th><th>Result</th></tr>";
         foreach my $file ($mod->getFiles())
         {
            printf FILE "<tr><td>" . "</td>";
            printf FILE "<td>" . $file->getCovFunc() . "</td>";
            printf FILE "<td>" . "</td></tr>";
         }
         print FILE "</table>";
         print FILE "<h4>Not tested lines</h4>";
         print FILE "<p>TODO</p>";
      }

   }

   close FILE;

}

1;

#!/usr/bin/perl

package OutSummary;
use Html;
use POSIX qw(strftime);

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
   print FILE "<p>Generated....: " . (strftime "%F %T", localtime) . "</p>";
   print FILE "<p>GIT Revision.: " . `git log -1 --format="%H"` . "</p>";

   print FILE "<table border='1'>";

   print FILE "<tr><th>Module</th><th>Tested Files</th><th>Compiled Tests</th><th>Test Cases</th><th>Function Coverage</th><th>Line Coverage</th><th>Branch coverage</th></tr>";

   foreach my $mod (@{$self->{_mods}})
   {
      my $row = "";
      my $status = STYLE_OK;


      my $countOfTests = $mod->getCountOfTests();
      my $countOfFiles = $mod->getCountOfFiles();
      my $style = STYLE_OK;
      if ( ($countOfFiles != $countOfTests) || ($countOfFiles == 0))
      {
         $style = STYLE_NOT_OK;
         $status = STYLE_NOT_OK;
      }
      $row .= "<td $style>" . $countOfTests . "/" . $countOfFiles . "</td>";

      $style = STYLE_OK;
      if ($mod->getCompiled() ne $mod->getCountOfTests()) {
         $style = STYLE_NOT_OK;
         $status = STYLE_NOT_OK;
      }
      $row .= "<td $style>" . $mod->getCompiled() . "/" . $mod->getCountOfTests() . "</td>";


      $style = STYLE_OK;
      if ($mod->getCountOfTestCases() != $mod->getCountOfPass()) {
         $style = STYLE_NOT_OK;
         $status = STYLE_NOT_OK;
      }
      $row .= "<td $style>" . $mod->getCountOfPass() . "/" . $mod->getCountOfTestCases() . "</td>";

      $style = STYLE_OK;
      if ($mod->getFuncs() ne $mod->getCovFuncs()) {
         $style = STYLE_NOT_OK;
         $status = STYLE_NOT_OK;
      }
      $row .= "<td $style>" . $mod->getCovFuncs() . "/" . $mod->getFuncs() . "</td>";

      $style = STYLE_OK;
      if ($mod->getLines() ne $mod->getCovLines()) {
         $style = STYLE_NOT_OK;
         $status = STYLE_NOT_OK;
      }
      $row .= "<td $style>" . $mod->getCovLines() . "/" . $mod->getLines() . "</td>";

      $style = STYLE_OK;
      if ($mod->getBranches() ne $mod->getCovBranches()) {
         $style = STYLE_WARN;
      }
      $row .= "<td $style>" . $mod->getCovBranches() . "/" . $mod->getBranches() . "</td>";

      # summary
      $row = "<tr><td $status><a href=\"#" . $mod->getName() . "\">" . $mod->getName() . "</a></td>" . $row;

      print FILE $row;
   }

   print FILE "</table>";
   print FILE "</body></html>";

   foreach my $mod (@{$self->{_mods}})
   {
      # todo perform the report of each module
      printf FILE "<a name=\"$mod->{_name}\"/><h2>Report of Module: $mod->{_name}</h2>";

      print FILE "<table border =\"1\">";
      print FILE "<tr><th>File</th><th>Has test file</th><th>Compiles</th><th>Test Cases</th><th>Function Coverage</th><th>Line Coverage</th><th>Branch coverage</th></tr>";
      foreach my $file ($mod->getFiles())
      {
         my $row = "";
         my $status = STYLE_OK;

         my $hasTest = "yes";
         my $hasTestStyle = STYLE_OK;
         if (!$file->hasTest()) {
            $hasTest = "no";
            $hasTestStyle = STYLE_NOT_OK;
            $status = STYLE_NOT_OK;
         }
         $row .= "<td $hasTestStyle>" . $hasTest . "</td>";

         $hasTest = "yes";
         $hasTestStyle = STYLE_OK;
         if (!$file->getHasCompile()) {
            $hasTest = "no";
            $hasTestStyle = STYLE_NOT_OK;
            $status = STYLE_NOT_OK;
         }
         $row .= "<td $hasTestStyle>" . $hasTest . "</td>";

         $hasTestStyle = STYLE_OK;
         if ($file->getCountOfPass() != $file->getCountOfTests()) {
            $hasTestStyle = STYLE_NOT_OK;
            $status = STYLE_NOT_OK;
         }
         $row .= "<td $hasTestStyle>" . $file->getCountOfPass() . "/" . $file->getCountOfTests() . "</td>";

         $hasTestStyle = STYLE_OK;
         if ($file->getFuncs() ne $file->getCovFuncs()) {
            $hasTestStyle = STYLE_NOT_OK;
            $status = STYLE_NOT_OK;
         }
         $row .= "<td $hasTestStyle>" . $file->getCovFuncs() . "/" . $file->getFuncs() . "</td>";

         $hasTestStyle = STYLE_OK;
         if ($file->getLines() ne $file->getCovLines()) {
            $hasTestStyle = STYLE_NOT_OK;
            $status = STYLE_NOT_OK;
         }
         $row .= "<td $hasTestStyle>" . $file->getCovLines() . "/" . $file->getLines() . "</td>";

         $hasTestStyle = STYLE_OK;
         if ($file->getBranches() ne $file->getCovBranches()) {
            $hasTestStyle = STYLE_WARN;
         }
         $row .= "<td $hasTestStyle>" . $file->getCovBranches() . "/" . $file->getBranches() . "</td>";

         # summary of the row
         my $href = $file->getName();
         if ($file->hasTest())
         {
            $href = "<a href=\"../../" . $file->getReportFile() . "\">" . $file->getName() . "</a>";
         }

         $row = "<tr><td $status>" . $href . "</td>" . $row;

         print FILE $row;
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

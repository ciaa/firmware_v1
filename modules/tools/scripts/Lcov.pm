#!/usr/bin/perl

package Lcov;

use strict;
use warnings;


sub new {
   my $class = shift;
   my $self = {
      _file => shift,
   };
   my $cmd = "find -name " . $self->{_file} . ".gcov.html | head -n 1";
   my $result = `$cmd`;
   chomp($result);
   my $filename = $result;

   $self->{_completeFilename} = $filename;

   if ($filename)
   {
      # get function coverage
      $cmd = "grep -A 2 '<td class=\"headerItem\">Functions:</td>' " . $filename . " | grep -v '<td class=\"headerItem\">Functions:</td>' | sed \"s/ *<td class=\\\"headerCovTableEntry\\\">//g\" | sed \"s/<\\\/td>//g\"";
      $result = `$cmd`;
      chomp($result);
      my @result = split("\n", $result);
      $self->{_funcs} = int($result[1]);
      $self->{_covFuncs} = int($result[0]);

      # get line coverage
      $cmd = "grep -A 2 '<td class=\"headerItem\">Lines:</td>' " . $filename . " | grep -v '<td class=\"headerItem\">Lines:</td>' | sed \"s/ *<td class=\\\"headerCovTableEntry\\\">//g\" | sed \"s/<\\\/td>//g\"";
      $result = `$cmd`;
      chomp($result);
      @result = split("\n", $result);
      $self->{_lines} = int($result[1]);
      $self->{_covLines} = int($result[0]);

      # get branch coverage
      $cmd = "grep -A 2 '<td class=\"headerItem\">Branches:</td>' " . $filename . " | grep -v '<td class=\"headerItem\">Branches:</td>' | sed \"s/ *<td class=\\\"headerCovTableEntry\\\">//g\" | sed \"s/<\\\/td>//g\"";
      $result = `$cmd`;
      chomp($result);
      @result = split("\n", $result);
      $self->{_branches} = int($result[1]);
      $self->{_covBranches} = int($result[0]);
   }
   else
   {
      $self->{_funcs} = "0";
      $self->{_covFuncs} = "0";
      $self->{_lines} = "0";
      $self->{_covLines} = "0";
      $self->{_branches} = "0";
      $self->{_covBranches} = "0";
   }


   bless $self, $class;
   return $self;
}

sub getReportFile
{
   my $self = shift;

   return $self->{_completeFilename};
}

sub getCovFuncs
{
   my $self = shift;

   return $self->{_covFuncs};
}

sub getFuncs
{
   my $self = shift;

   return $self->{_funcs};
}

sub getCovLines
{
   my $self = shift;

   return $self->{_covLines};
}

sub getLines
{
   my $self = shift;

   return $self->{_lines};
}

sub getCovBranches
{
   my $self = shift;

   return $self->{_covBranches};
}

sub getBranches
{
   my $self = shift;

   return $self->{_branches};
}

1;

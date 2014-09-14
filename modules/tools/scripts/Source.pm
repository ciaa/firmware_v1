#!/usr/bin/perl

package Source;

use strict;
use warnings;
use Lcov;
use Ceedling;

sub new {
   my $class = shift;
   my $self = {
      _mod => shift,
      _rel_path => shift,
      _name => shift,
   };
   $self->{_lcov} = new Lcov($self->{_name});
   $self->{_ceedling} = new Ceedling();

#   print "File $self->{_name}\n";
   bless $self, $class;
   return $self;
}

sub print {
   my $self = shift;

   return $self->{_mod}->getName() . "/" . $self->{_name};
}

sub getFile
{
   my $self = shift;

   return $self->{_mod}->getName() . "/" . $self->{_name};
}

sub getName
{
   my $self = shift;

   return $self->{_name};
}

sub hasTest
{
   my $self = shift;
   my $ret = 0;

   if (-e "modules/" . $self->{_mod}->getName() . "/test/utest/src/test_" . $self->{_name})
   {
      $ret = 1;
   }

   return $ret;
}

sub runTest
{
   my $self = shift;

   my $result = "";

   if ($self->hasTest())
   {
      # get file without extension
      my $test = $self->{_name};
      $test =~ s{\.[^.]+$}{};
      my $cmd = "make tst_" . $self->{_mod}->getName() . "_" . $test . "\n";
      $result = `$cmd`;
   }

   $self->{_ceedling}->loadResults($result);
}

sub getFuncs
{
   my $self = shift;

   return $self->{_lcov}->getFuncs();
}

sub getCovFuncs
{
   my $self = shift;

   return $self->{_lcov}->getCovFuncs();
}

sub getLines
{
   my $self = shift;

   return $self->{_lcov}->getLines();
}

sub getCovLines
{
   my $self = shift;

   return $self->{_lcov}->getCovLines();
}

sub getBranches
{
   my $self = shift;

   return $self->{_lcov}->getBranches();
}

sub getCovBranches
{
   my $self = shift;

   return $self->{_lcov}->getCovBranches();
}

sub getReportFile
{
   my $self = shift;

   return $self->{_lcov}->getReportFile();
}

sub getHasCompile
{
   my $self = shift;
   my $ret = 0;

   my $name = $self->{_name};

   $name =~ s/\.c/\.bin/;
   $name = "out/bin/" . $name;

   if (-e $name)
   {
      $ret = 1;
   }

   return $ret;
}

sub getCountOfTests {
   my $self = shift;

   return $self->{_ceedling}->getCountOfTests();
}

sub getCountOfPass{
   my $self = shift;

   return $self->{_ceedling}->getCountOfPass();
}

1;

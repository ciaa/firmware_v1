#!/usr/bin/perl

package Source;

use strict;
use warnings;
use Lcov;

sub new {
   my $class = shift;
   my $self = {
      _mod => shift,
      _rel_path => shift,
      _name => shift,
   };
   $self->{_lcov} = new Lcov($self->{_name});

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

   if ($self->hasTest())
   {
      # get file without extension
      my $test = $self->{_name};
      $test =~ s{\.[^.]+$}{};
      my $cmd = "make tst_" . $self->{_mod}->getName() . "_" . $test . "\n";
      my $result = `$cmd`;
   }
}

sub getFunc
{
   return 10;
}

sub getCovFunc
{
   my $self = shift;

   return $self->{_lcov}->getCovFunc();
}

1;

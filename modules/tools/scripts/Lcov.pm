#!/usr/bin/perl

package Lcov;

use strict;
use warnings;


sub new {
   my $class = shift;
   my $self = {
      _file => shift,
   };
   my $cmd = "find -name " . $self->{_file} . ".gcov.html";
   $cmd = `$cmd`;
   $self->{_completefilename} = chomp($cmd);
   bless $self, $class;
   return $self;
}

sub getCovFunc
{
   my $self = shift;
   my $ret = 0;

   if (-e $self->{_completefilename})
   {
      $ret = 1;
   }
   else
   {
      $ret = 2;
   }

   return $ret;
}

1;

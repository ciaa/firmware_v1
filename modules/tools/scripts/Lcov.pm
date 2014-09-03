#!/usr/bin/perl

package Lcov;

use strict;
use warnings;


sub new {
   my $class = shift;
   my $self = {
      _file => shift,
   };
   bless $self, $class;
   return $self;
}

1;

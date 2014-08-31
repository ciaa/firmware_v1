#!/usr/bin/perl

package Module;
use Source;

sub new
{
   my $class = shift;
   my $self = {
      _name => shift
   };
   print "Module name $self->{_name}\n";
   bless $self, $class;
   return $self;
}

1;

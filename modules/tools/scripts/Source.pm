#!/usr/bin/perl

package Source;

sub new {
   my $class = shift;
   my $self = {
      _name => shift
   };
   print "File $self->{_name}\n";
   bless $self, $class;
   return $self;
}

1;

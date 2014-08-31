#!/usr/bin/perl

package Module;

use strict;
use warnings;
use File::Find;

use Source;

sub new
{
   my $class = shift;
   my $self = {
      _name => shift
   };
   print "Module name $self->{_name}\n";

   if (opendir my $files, "modules/$self->{_name}/src/")
   {
   while (defined (my $file = readdir $files)) {
      if ( ($file eq ".") || ($file eq "..") )
      {
         next;
      }
      my $obj = new Source($file);
   }


   closedir $files;
   }
   else
   {
   print "Module without files.\n";
   }
   bless $self, $class;
   return $self;
}

1;

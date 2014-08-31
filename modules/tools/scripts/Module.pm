#!/usr/bin/perl

package Module;

use strict;
use warnings;
use File::Find;

use Source;

our @files;

sub new
{
   my $class = shift;
   my $self = {
      _name => shift,
      _files => [],
      _file => undef
   };
   bless $self, $class;


   if (opendir my $files_dir, "modules/$self->{_name}/src/")
   {
#      print "Module $self->{_name}\n";

      while (defined (my $file = readdir $files_dir)) {
         if ( ($file eq ".") || ($file eq "..") || (!($file =~ /\.c$/)) )
         {
            next;
         }
         push(@{$self->{_files}}, new Source($self, "modules/" . $self->{_name} . "/src", $file));
         $self->{_file} = "src/$file";
      }

      closedir $files_dir;
   }
   else
   {
#      print "Module $self->{_name} without files.\n";
   }
   return $self;
}

sub getName
{
   my $self = shift;

   return $self->{_name};
}

sub getCountOfFiles
{
   my $self = shift;

   return scalar @{$self->{_files}};
}

sub print
{
   my $self = shift;

   print "Module Name: $self->{_name}\n";
   print " -> Count of files: " . $self->getCountOfFiles() . "\n";
   foreach my $file (@{$self->{_files}})
   {
         print "  -> File: " . $file->print() . " - has test: " . $file->hasTest() . "\n";
   }
}

sub runTests
{
   my $self = shift;

   foreach my $file (@{$self->{_files}})
   {
      $file->runTest();
   }
}

1;

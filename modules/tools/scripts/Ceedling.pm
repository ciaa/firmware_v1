#!/usr/bin/perl

package Ceedling;

use strict;
use warnings;

sub new {
   my $class = shift;
   my $self = {
      _results => "",
   };

   bless $self, $class;
   return $self;
}

sub loadResults {
   my $self = shift;

   $self->{_results} = "";

   my $results = shift;

   chomp($results);

   my @lines = split /\n/, $results;

   foreach my $line (@lines) {
      if ( ($line =~ /:PASS/) || ($line =~ /:FAIL:/) )
      {
         $self->{_results} .= $line . "\n";
      }
   }
}

sub getCountOfTests {
   my $self = shift;

   my @lines = split /\n/, $self->{_results};

   return scalar @lines;
}

sub getCountOfPass {
   my $self = shift;
   my $ret = 0;
   my @lines = split /\n/, $self->{_results};

   foreach my $line (@lines) {
      if ($line =~ /:PASS/)
      {
         $ret++;
      }
   }

   return $ret;
}

1;

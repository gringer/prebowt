#!/usr/bin/perl

use warnings;
use strict;

use FindBin qw($Bin);
use Cwd 'abs_path';
use File::Copy;

our $version = "1.01";

=head1 NAME

reheader.pl -- rewrites file headers for source code found in src,
include, and util directories.

=cut

# process src directories
my $rootDir = abs_path("$Bin/..");

my %lcHeaders = ( 'cpp' => '/** <header>',
                  'hpp' => '/** <header>',
                  'pl' => '=head1 LICENSE' );

my %lcFooters = ( 'cpp' => '</header> **/',
                  'hpp' => '</header> **/',
                  'pl' => '=cut' );

my @files = grep {-f}
  glob("$rootDir/src/*.cpp $rootDir/util/*.prl $rootDir/include/*.hpp");

foreach my $fName (@files){
  my $extension = ($fName =~ s/^.*\.//r);
  print("$fName $extension\n");
  copy("$fName", "$fName.old");
  open(my $in, "< $fName.old")
    or die ("Unable to open $fName.old for reading");
  open(my $out, "> $fName")
    or die ("Unable to open $fName for writing");
  while(<$in>){
    chomp;
    if($_ eq $lcHeaders{$extension}){
      print("Found header\n");
      print($out "$lcHeaders{$extension}\n\n");
      for($_ = <$in>, chomp;
          $_ ne $lcFooters{$extension}; $_ = <$in>, chomp){}
      open(my $licenseFile, "< $rootDir/doc/license-header.txt")
        or die ("Unable to open license file for reading");
      while(<$licenseFile>){
        print($out "$_");
      }
      print($out "\n$lcFooters{$extension}\n");
    } else {
      print($out "$_\n");
    }
  }
}

=head1 LICENSE

Copyright 2014 David Eccles (gringer) <bioinformatics@gringene.org>

This file is part of preBowt

Permission to use, copy, modify, and/or distribute this software for
any purpose with or without fee is hereby granted, provided that the
above copyright notice and this permission notice appear in all
copies.

The software is provided "as is" and the author disclaims all
warranties with regard to this software including all implied
warranties of merchantability and fitness. In no event shall the
author be liable for any special, direct, indirect, or consequential
damages or any damages whatsoever resulting from loss of use, data or
profits, whether in an action of contract, negligence or other
tortious action, arising out of or in connection with the use or
performance of this software.

=cut

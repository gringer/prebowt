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
  my $fMode = (stat($fName))[2];
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
      close($licenseFile);
      print($out "\n$lcFooters{$extension}\n");
    } else {
      print($out "$_\n");
    }
  }
  close($in);
  close($out);
  chmod($fMode, $fName);
}

=head1 LICENSE

Copyright 2014 David Eccles (gringer) <bioinformatics@gringene.org>

This file is part of preBowt

preBowt is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

preBowt is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with preBowt. If not, see <http://www.gnu.org/licenses/>

=cut

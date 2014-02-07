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

my %lcPrefix = ( 'cpp' => ' * ',
                  'hpp' => ' * ',
                  'pl' => '' );

my %lcEmPrefix = ( 'cpp' => ' *',
                   'hpp' => ' *',
                   'pl' => '' );

my %lcFooters = ( 'cpp' => '</header> **/',
                  'hpp' => '</header> **/',
                  'pl' => '=cut' );

while(<>){
  my $fName = $ARGV;
  my $extension = ($fName =~ s/^.*\.//r);
  if(!exists($lcHeaders{$extension})){
    die("File extension '$extension' cannot be handled by this utility!\n");
  }
  chomp;
  if($_ eq $lcHeaders{$extension}){
    print(STDERR "Found header\n");
    print("$lcHeaders{$extension}\n\n");
    for($_ = <>, chomp;
        $_ ne $lcFooters{$extension}; $_ = <>, chomp){}
    open(my $licenseFile, "< $rootDir/doc/license-header.txt")
      or die ("Unable to open license file for reading");
    while(<$licenseFile>){
      if(/^$/){
        print("$lcEmPrefix{$extension}$_");
      } else {
        print("$lcPrefix{$extension}$_");
      }
    }
    close($licenseFile);
    print("\n$lcFooters{$extension}\n");
  } else {
    print("$_\n");
  }
}

=head1 LICENSE

This file is part of preBowt -- a prefix-based BWT-like transform of
genetic data.

Copyright 2014 David Eccles (gringer) <bioinformatics@gringene.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
without any warranty; without even the implied warranty of
merchantability or fitness for a particular purpose. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

=cut

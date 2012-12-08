#!/usr/bin/perl

# This is a perl script for generating lookup tables
# for sub-pixel values that fade gracefully.
#
# Invoke as ./fade_table.pl 100 255 2.5
# Where 100 is the number of line in the table, 255
# is the maximum value and 2.5 is the power (larger
# number make tables more sensitibve at dim values).

use strict;
use warnings;

my $numvals = shift;
my $maxval = shift;
my $power = shift;

print "{\n  ";

my $i;
for ( $i = 0 ; $i < $numvals ; $i++ ) {
  #normalise value to maxvalue
  my $val = $i / ($numvals - 1) * $maxval;

  #clculate the brightness
  my $result = (($val / $maxval) ** $power) * $maxval;

  #only let the first value be 0
  if (0 != $i and $result < 1) {
    $result = 1;
  }

  printf "%03d", $result;

  #newlines and commas
  if (0 == ($i + 1) % 20 and $i != $numvals - 1) {
    print ",\n  ";
  } elsif ($i != $numvals - 1) {
    print ",";
  }
}

print "}\n";

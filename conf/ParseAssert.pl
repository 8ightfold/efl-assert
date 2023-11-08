##===- conf/ParseAssert.pl ------------------------------------------===##
##
## Copyright (C) 2023 Eightfold
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
##     http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
##     limitations under the License.
##
##===----------------------------------------------------------------===##
##
##  Extracts relevant info from `assert.h` and generates a macro body.
##
##===----------------------------------------------------------------===##

use warnings;
use strict;
use List::Util qw(pairs);

## ReplaceAll(String)
sub ExtractArg {
  my $expr = shift;
  return ($expr =~ /assert\(\s*(\w+)\s*\)\s*(.*)/) ? 
    ("$2", "$1") : ();
}

sub ReplaceAll {
  my ($p) = @_;
  my ($body, $arg) = ($p->[0], $p->[1]);
  my $arg_re = qr/\Q$arg/;
  return () if ($body !~ /(__?assert.*?)\s*\(/g);
  my $b = $body =~ s/#\s*${arg_re}/str/;
  return () if !$b;
  $body =~ s/$arg_re/chk/;
  return $body;
};

##=== Main ===##
my $filepath = $ARGV[0];
open(my $handle, '<', $filepath) or die $!;

my $define_re = qr/^\s*#\s*define\s+assert\([^)]+\)/;
my $head_re = qr/^.*?(assert\([^\\\n]*)\\?$/m;
my @defines = ();

# Extract defines through iteration
while(my $line = <$handle>) {
  if($line =~ /${define_re}[^\\\n]*\\$/m) {
    my $define = "$1" if ($line =~ $head_re);
    while(($line = <$handle>) =~ /^([^\\\n]*)\\$/m) {
      $define = $define . "$1";
    }
    $line =~ /^([^\n]*)$/m;
    push(@defines, $define . "$1");
  } elsif($line =~ /${define_re}[^\\\n]*$/m) {
    ($line =~ $head_re);
    push(@defines, "$1");
  }
}

my @args = map { ExtractArg($_) } @defines;
my @macros = map { ReplaceAll($_) } pairs @args;
my $def = $macros[0];
print $def;

close $handle;

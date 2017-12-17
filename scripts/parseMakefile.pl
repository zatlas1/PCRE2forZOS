#!/usr/local/bin/perl
# for now perl parseMakefile.pl > aaa.txt
use strict;
no strict "refs";
use warnings;
use Data::Dumper;

my $sw = 0;
 
my $base=$ENV{'BASE'};
my $bin=$ENV{'BIN'};
my $inlib=$ENV{'INLIB'};
my $infile=$ARGV[0]; 

our $var;
our $val;
our %hash;
our $result = \%hash;
our @with;
our $S_IN_VAR = 0;
our $S_IN_PROCESS = 0;
our $S_ELSE_ENDIF = 0;
our $i;

$infile = "C:\\projects\\pcre2-10.20.380\\Makefile.am";

open IN, "< $infile" or die "cannot open $infile $!";

push @with, 'baseline';
$result -> {$with[$#with]} = {};

while (<IN>)
{
	chomp;
	$i ++;
	if (/^\s*(?:\#|dnl|$)/)
	{
		unsetinvar();
	}
	elsif (/^\s*if\s+(\w+)\s*$/)
	{
		push @with, $1;
		$result -> {$with[$#with]} = {} if ! defined ($result -> {$with[$#with]});
		unsetinvar();
	}
	elsif (/^\s*else\s*(?:\#|$)/)
	{
		$S_ELSE_ENDIF = 1;
		my $pop = pop @with;
		unsetinvar();
	}
	elsif (/^\s*endif\s*(?:\#|$)/)
	{
		if (!$S_ELSE_ENDIF)
		{
			my $pop = pop @with;
		}
		$S_ELSE_ENDIF = 0;
		unsetinvar();
	}
	elsif ($S_IN_VAR)
	{
		add2var ($_);
	}
	elsif ($S_IN_PROCESS)
	{
		next; # skip for now
	}
	elsif (/\s*(\w+)\s*=/a)
	{
		#VAR=values
		$var = $1;
		s/\s*(\w+)\s*=//a;
		$S_IN_VAR = 1;
		if ($result->{$with[$#with]}->{$var})
		{
			print "error $i: overwiting $var in $with[$#with]\n";
		}
		add2var ($_);
	}
	elsif (/\s*(\w+)\s*\+=/)
	{
		#VAR=values
		$var = $1;
		s/\s*(\w+)\s*\+=//;
		$S_IN_VAR = 1;
		if (!$result->{$with[$#with]}->{$var})
		{
			print "error $i: adding to nonexisting $var in $with[$#with]\n";
		}		
		add2var (' ' . $_);
	}
	elsif (/s*[^\s:]*\:/)
	{
		$S_IN_PROCESS = 1;
	}
	else
	{
		# I really do not need this information.  Could expand on it later
		print "unknown situation in line $i: $_\n";
	}
}
print "HASH ", Dumper (\%hash);
exit;
	
sub add2var
{
	if (/\s*(\S(?:.*|)|)\\$/)
	{
		$val .= $1;
	}
	elsif (/\s*(\S(?:.*|)|)$/)
	{
		$val .= ($1 ? $1 : ' ');
		$S_IN_VAR = 0;
		$result->{$with[$#with]}->{$var} = $val;
		$var = '';
		$val = '';
	}
}

sub unsetinvar
{
	if ($S_IN_VAR)
	{
		print "Error $i: incomplete variable $var in line $i\n";
		$result->{$with[$#with]}->{$var}  = $val;
	}
	$S_IN_VAR = 0;
	$S_IN_PROCESS = 0;
	$var = '';
	$val = '';	
}		

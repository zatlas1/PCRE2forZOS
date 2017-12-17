use strict;
use warnings;
use Data::Dumper;

my $infile=$ARGV[0];
my $libname=$ARGV[1];
print "TYPE,MEMBER,TOKEN\n";

open SRC, "< $infile" or die "cannot open $infile $!\n";
my $member='';
while (<SRC>)
{
	my $missing = '';
	my $type='';
	if (/\s+C\s+\'$libname\(([\#\$\@a-zA-Z0-9]+)\)\'/)  # Note that this pattern is not \w plus
	{
		$member = $1;
	}
	elsif (/IEW2456E\s+9207\s+SYMBOL\s+([\#\$\@\_a-zA-Z0-9]+)\s+UNRESOLVED/) # Note that this pattern is \w plus
	{
		$missing = $1;
		$type='missing';
	}
	elsif (/\s+([\#\$\_\@a-zA-Z0-9]+)\s+CSECT\s+\w+\s+SYSLIN/)  
	{
		$missing = $1;
		if ($missing=~/^$member\#/)
		{
			#print "found1,$member,$missing\n";	
		}
		else
		{
			$type='found2';	
		}		
	}
	elsif (/\s+\w+\s+\w+\s+([\#\$\_\@a-zA-Z0-9]+)\s+LABEL\s+/)  # Note that this pattern is \w plus
	{
		$missing = $1;
		if ($missing eq $member or $missing eq 'CEELLIST' or $missing eq 'IEWBLIT')		
		{
		}
		else
		{
			$type='found3';	
		}
	}
	elsif (/\s+[\#\$\-\_\@a-zA-Z0-9]+\s+:\=\s+([\#\$\_\@a-zA-Z0-9]+)\s*$/)  # Note that the left hand side pattern is \w plus, plus dash
	#_pcre_ucd-ecords := _pcre_ucd_records
	{
		$missing = $1;
		$type="found4";
	}
	if ($type ne '')
	{
		if ($missing !~ /\#/ and $missing !~ 
			/(regfree|regerror|regexec|regcomp|main|isregfile|
			closedirectory|readdirectory|opendirectory|isdirectory|CEESTART|
			utf8_table4|utf8_table3|
			CEEMAIN|CEEROOTD|CEEINT|CEEBPIRB|CEEBPIRC|EXIT|
			CLOCK|REGFREE|REGEXEC|REGERROR|REGCOMP)/x)
		{
			print "$type,$member,$missing\n";	
		}
	}
}
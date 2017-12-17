use strict;
use warnings;

my $file = "C:\\projects\\pcreport\\zos\.jcl\.txt";
my $i = 0;
my $j = 0;
open P, "< $file";
while (<P>)
{
	$i ++;
	chomp;
	for ($j=0; $j < length ($_); $j++)
	{
		my $k = substr ($_, $j, 1);
		if (ord ($k) > 127 || ord ($k) < 20)
		{
			print "line $i: column $j: char " . ord($k). ": $_";
			next;
		}
	}
}
close P;
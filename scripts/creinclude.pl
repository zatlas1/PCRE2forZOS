#!/usr/local/bin/perl
use strict;
use warnings;
use Data::Dumper;
use DBI;
 
#my $base=$ENV{'BASE'};
my $bin=$ENV{'BIN'};
my $xreftab = $ARGV[0];
my $outfile = $ARGV[1];

open OF, "> $outfile"
	or die "cannot open $outfile $!\n";

my $dbh = DBI->connect ("dbi:CSV:", "", "", {
    f_dir => $bin,
    })  or
    die "Cannot connect: $DBI::errstr";

# Selecting
$dbh->{RaiseError} = 1;

my $query;
my $sth;
my $member='';
my %missing = ();
my %found = ();
#TYPE,MEMBER,TOKEN
{
	$query = "select distinct member, token from $xreftab where type = \'missing\' order by member";
	$sth   = $dbh->prepare ($query);
	$sth->execute ();
	my @tuple=();
	while (my $row = $sth->fetchrow_hashref) 
	{
		if ($member eq '')
		{
			$member = $row-> {member};
		}
		if ($member eq $row-> {member})
		{
			push @tuple, $row-> {token};
		}
		else
		{
			my @lcltuple=@tuple;
			$missing{$member} = \@lcltuple;
			@tuple=();
			push @tuple, $row-> {token};
			$member = $row-> {member};
		}
	}
	$missing{$member} = \@tuple;
	$sth->finish ();
	#print Dumper(\%missing);
}
my $token = '';
{
	$query = "select distinct member, token from $xreftab where type like \'found\%\' and member not like \'\%DEMO\' and member not like \'\%TEST\' and member not like \'\%API\' and member not like \'\%GREP\'  and member not like \'\%POSIX\'  order by token";
	$sth   = $dbh->prepare ($query);
	$sth->execute ();
	while (my $row = $sth->fetchrow_hashref) 
	{
		$found{$row-> {token}} = $row-> {member};
	}
	$sth->finish ();
	print Dumper(\%found);
}
for my $miss (keys %missing)
{
	my $vec = $missing{$miss};
	print OF "./ ADD NAME=" . uc($miss) . "\n";
	my %temp = ();
	for my $vec1 (@$vec)
	{
		print "VEC1 $vec1\n" if !defined ($found{$vec1});
		if (defined ($found{$vec1}) and !exists $temp {$found{$vec1}})
		{
			if ($vec1 eq 'PCR2GREP')
			{
				print "PCR2GREP ignored\n";
			}
			else
			{
				print OF  " INCLUDE SYSLIB\(" .
					(exists ($found{$vec1}) ? uc ($found{$vec1}) : "UNKNOWN") . "\)\n";
			}
			$temp {$found{$vec1}} = 1;
		}
	}
}
close OF;

#!/usr/local/bin/perl
use strict;
use warnings;
use Data::Dumper;
use DBI;
#use File::Find::Rule; 
my $sw = 0;
 
my $base=$ENV{'BASE'};
my $bin=$ENV{'BIN'};
my $inlib=$ENV{'INLIB'};
my $outfile=$ARGV[0];
#my $outrel= $ARGV[1];
$outfile = $bin . "\\" . $outfile;
# here add manually unsourced (manually dealt with) files

# "Creating database handle"
my $dbh = DBI->connect ("dbi:CSV:",  "", "", {
    f_dir => $bin,
    })  or
    die "Cannot connect: $DBI::errstr";
# Selecting
my %dict = ();
$dbh->{RaiseError} = 1;
my $query = "select distinct filename from testlist\.txt where copy = \'y\'";
my $sth   = $dbh->prepare ($query);
#print Dumper (\%$sth); exit;
$sth->execute ();
open OUT, "> $outfile";
while (my $row = $sth->fetchrow_hashref) 
{
	#print "TEST $row->{filename}\n";
	copy_replace ($row->{filename});
}
$sth->finish ();
$dbh->disconnect;

close OUT;
exit 0;

sub copy_replace
{
	my $filename = shift;
	my $member = $filename;
	$member =~ s/^test/tst/;
	$member =~ s/^grep/grp/;
	$member =~ s/input/in/;
	$member =~ s/output/ot/;
	$member =~ s/test/t/;
	$member =~ s/grep/g/;
	$member =~ s/file/f/;
	$member =~ s/list/ls/;
	$member =~ s/-//;
	die "$member too long" if length($member) > 8;
	
	$filename= $base . "\\" . $inlib . "\\" . $filename;
	my $i = 0;
	my $nl = 1;
	open LDREP, "< $filename" or die "cannot open $filename $!\n";
	print OUT "\.\/ ADD NAME=" , uc($member) , " \n";

	while (<LDREP>)
	{
		$i++;
		my $s = $_;
		$nl = ($_ =~ /\n/ ? 1 : 0);

		if (/\\(\ÿ|\^)/)
		{
			my $ofend = $1;
			#print "$filename $i $ofend\n";
		}
		$s=~ s/\t/   /g;
		$s=~ s/\ÿ/\\xFF/g;
		$s=~ s/\^/\xAC/g;  # ^ ==> ¬ ;in FTP ac ==> 5f

		print OUT $s;
	}
	print OUT "\n" if $nl == 0;
	close LDREP;
}

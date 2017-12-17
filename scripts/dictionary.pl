#!/usr/local/bin/perl
use strict;
use warnings;
use Data::Dumper;
use DBI;
use Getopt::Long;
my $dict='';
#use File::Find::Rule; 
my $result = GetOptions ("dict=s"   => \$dict);  
# in command line put -- as first argument if there is no older dictionary
# or --dict=dictionary_name which is a txt (or csv) file in $bin.
# note that if the argument to the program is same as the existing dictionary,
# the program will happily overwrite it
 
my $base=$ENV{'BASE'};
my $bin=$ENV{'BIN'};
my $inlib=$ENV{'INLIB'};
my $dictfile = $ARGV[0];
$dictfile = $bin . "\\" . $dictfile;

# In moving to PCRE2, I've decided to leave the config.h name as is
my %nosource =qw(
config.h 0 
pcre2.h 0
pcre2_chartable.c 0);

my %dictionary = ();
my %duplicates = ();

=doc
my $finder = File::Find::Rule->new()->name(qr/\.(c|cc|h)?$/i)->start("$base\\$lib"); 
 
while( my $file = $finder->match()  ){ 
   print "$file\n"; 
=cut

# "Creating database handle"
my $dbh = DBI->connect ("dbi:CSV:", "", "", {
    f_dir => $bin,
    })  or
    die "Cannot connect: $DBI::errstr";

# Selecting
$dbh->{RaiseError} = 1;

my $query;
my $sth;
if ($dict ne '')
{
	$query = "select component, type, lib, member, source_loc, change from $dict where lib <> 'POSIX'";
	$sth   = $dbh->prepare ($query);
	$sth->execute ();
	while (my $row = $sth->fetchrow_hashref) 
	{
		my $x;
		$x = $row-> {member};
		if (!defined ($x))
		{
			print Dumper ($row);
		}
		else{
		my @tuple=();
		$tuple[0] = $row-> {lib};
		$tuple[1] = $row-> {type};
		$tuple[2] = $row-> {member};
		$tuple[3] = $row-> {source_loc};
		$tuple[4] = $row-> {change};
		$dictionary{$row->{component}} = \@tuple;
		$duplicates{$row-> {member}} = $row->{component};
		}
	}
	$sth->finish ();
}

$query = "select distinct source, source_type, source_loc from include\.txt where source_type = \'c\' or source_type = \'h\' or source_type = \'cc\'";
$sth   = $dbh->prepare ($query);
#print Dumper (\%$sth); exit;
$sth->execute ();
while (my $row = $sth->fetchrow_hashref) 
{
	cre_dictionary ($row->{source}, $row-> {source_type}, $row->{source_loc});
}
$sth->finish ();
$dbh->disconnect; 
cre_dictionary ("pcre2.h", "h", $base . "\\" . $inlib);
cre_dictionary ("config.h", "h", $base . "\\" . $inlib);
cre_dictionary ("pcre2_chartables.c", "c", $base . "\\" . $inlib);
cre_dictposix ("regfree", 'P', 'POSIX', 'PREGFREE', 'Y');
cre_dictposix ("regerror", 'P', 'POSIX', 'PREGERR', 'Y');
cre_dictposix ("regcomp", 'P', 'POSIX', 'PREGCOMP', 'Y');
cre_dictposix ("regexec", 'P', 'POSIX', 'PREGEXEC', 'Y');
#print Dumper (\%dictionary);
open OUT, "> $dictfile" or die "cannot open $dictfile $!";
print OUT "COMPONENT,TYPE,LIB,MEMBER,SOURCE_LOC,CHANGE\n";
foreach my $key (keys (%dictionary))
{
	#print "$key\n";
	my $tref = $dictionary{$key};
	my @tuple = @$tref;
	print OUT $key, ",", $tuple[1], ",", $tuple[0], ",", uc($tuple[2]), ",", $tuple[3], ",", $tuple[4], "\n";
}

exit 0;

sub cre_dictionary
{
	#don't forget to separate c, h, sljit, pcre, pcre16
	my $source = shift;
	my $source_type = shift;
	my $source_loc = shift;
	my $what = shift || 'Y';
	my $new_source= $source;
	my $suffix = '';
	my @tuple=();
	if (exists ($dictionary{$source}))
	{
		my $tref = $dictionary{$source};
		@tuple = @$tref;
		# usually we run this for new version (i.e. new folder name)
		$tuple[3] = $source_loc;
	}
	else
	{
		if ($new_source =~ /^pcre2_/)
		{
			$suffix = '2'; #PCRE 8
			$tuple[0]='pcre2';
			$new_source=~ s/^(pcre2_)//;
		}
		elsif ($new_source =~ /^pcre2(?!\.h)/)
		{
			$suffix = 'd'; # PCRE Demo
			$tuple[0]='pcre2';
			$new_source=~ s/^(pcre2)//;
		}	
		elsif ($new_source =~ /^sljitNative/)
		{
			$suffix = 'n'; # SLJIT Native support
			$tuple[0]='sljit';
			$new_source=~ s/^(sljitNative)//;
		}	
		elsif ($new_source =~ /^sljit/)
		{
			$suffix = 'j'; # SLJIT 
			$tuple[0]='sljit';
			$new_source=~ s/^(sljit)//;
		}
		else
		{
			#$suffix = '#';
			$tuple[0]='pcre2';
		}
		$new_source=~ s/_//g;
		$new_source=~ s/\.(cc|c|h)$//;
		$new_source .= $suffix;
		$new_source = substr ($new_source,0,7) . $suffix if (length ($new_source) > 8);
		if (exists ($duplicates{$new_source})) 
		{	
			my $j = length ($new_source) - length ($source_type);
			$j = ($j < 8 ? $j : 8 - length ($source_type));
			$new_source = substr ($new_source,0,$j).= $source_type;
		}
		if (exists ($duplicates{$new_source}))
		{
			print "duplicste $source $new_source\n";
		}
		else 
		{
			$duplicates{$new_source} = $source;
		}
		$tuple[1]= $source_type;
		$tuple[2]= $new_source;
		$tuple[4] = $what;
		if (exists ($nosource{$source}))
		{
			$tuple[3] = $bin;
		}
		else
		{
			$tuple[3]= $source_loc;
		}
		print $tuple[0], ",", $tuple[1], ",", $tuple[0], ",", uc($tuple[2]), ",", $tuple[3], ",", $tuple[4], "\n";
	}
	if (length($source) == 0)
	{
		print "Check $new_source\n";
	}
	$dictionary{$source} = \@tuple;
}
sub cre_dictposix 
{
	my $source = shift;
	my $source_type = shift;
	my $source_loc = shift;
	my $newname = shift;
	my $what = shift || 'Y';
	my @tuple=();
	$tuple[3] = '';
	$tuple[1] = $source_type;
	$tuple[0] = $source_loc ;
	$tuple[2] = $newname ;
	$tuple[4] = $what ;
	print $source, ",", $tuple[1], ",", $tuple[2], ",", uc($tuple[3]), ",", '', ",", $tuple[4], "\n";
	$dictionary{$source} = \@tuple;
}
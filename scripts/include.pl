#!/usr/local/bin/perl
use strict;
use warnings;
use Data::Dumper;
use DBI;
#use File::Find::Rule; 
 
my $base=$ENV{'BASE'};
my $bin=$ENV{'BIN'};
my $inlib=$ENV{'INLIB'};
my %ignorelib=();
my %ignoreext=();
my @ilib=();
my @iext=();
if (exists $ENV{'IGNORELIB'})
{
	@ilib=split /,/,$ENV{'IGNORELIB'};
}
if (exists $ENV{'IGNOREEXT'})
{
	@iext=split /,/,$ENV{'IGNOREEXT'};
}
for (@ilib)
{
	$ignorelib{$_} = 1;
}
for (@iext)
{
	$ignoreext{$_} = 1;
}
=doc
my $finder = File::Find::Rule->new()->name(qr/\.(c|cc|h)?$/i)->start("$base\\$inlib"); 
 
while( my $file = $finder->match()  ){ 
   print "$file\n"; 
=cut

# "Creating database handle"
my $dbh = DBI->connect ("dbi:CSV:", "", "", {
    f_dir => $bin,
    })  or
    die "Cannot connect: $DBI::errstr";

# Simple statements
$dbh->do ("DROP TABLE include.txt");
$dbh->do 
("CREATE TABLE include.txt (SOURCE VARCHAR(256), SOURCE_TYPE VARCHAR(10), SOURCE_LOC VARCHAR(256), INCLUDE VARCHAR(256), INCLUDE_TYPE VARCHAR(10), INCLUDE_ENC CHAR(1))") or
    die "Cannot prepare: " . $dbh->errstr ();

=doc
# Selecting
$dbh->{RaiseError} = 1;
my $sth = $dbh->prepare ("select * from foo");
$sth->execute;
while (my @row = $sth->fetchrow_array) {
    print "id: $row[0], name: $row[1]\n";
    }

# Updates
my $sth = $dbh->prepare ("UPDATE a SET name = ? WHERE id = ?");
$sth->execute ("DBI rocks!", 1);
$sth->finish;
=cut

my $source='';
my $source_type='';
my $source_loc='';
my $include=''; 
my $include_type='';
my $include_enc='';
my @info=();

$dbh->disconnect;
# $..  @..  %..
process_files ($base . "\\" . $inlib);

# Accepts one argument: the full path to a directory.
# Returns: nothing.
sub process_files 
{
    my $path = shift;

    # Open the directory.
    opendir (DIR, $path)
        or die "PF001-Unable to open $path: $!";
    my @files = grep { !/^\.{1,2}$/ } readdir (DIR);
    closedir (DIR);
    @files = map { $path . "\\" . $_ } @files;

    for (@files) 
	{
        # If the file is a directory
        if (-d $_) 
		{
            my $lib = $_;
			$lib =~ s/.*\\([^\\]*)$/$1/;
			if (!exists $ignorelib{$lib})
			{
				# Here is where we recurse.
				process_files ($_);
			}
			else 
			{
				print "Ignoring lib $_\n";
			}
        } 
		else 
		{
            my $ext = $_;
			$ext =~ s/.*\.([^\.\\]*)$/$1/;
			if (!exists $ignoreext{$ext})
			{		
				find_include ($_, $path);
			}
			else
			{
				print "Ignoring ext $_\n";
			}
        }
    }
}

sub find_include
{
	my $infile = shift;
	$source_loc = shift;
	$source = $infile;
	my $path= quotemeta($source_loc) . "\\\\";
	$source =~ s/$path//;
	$source_type = $source;
	$source_type =~ s/^(.*\.)(\w+)$/$2/;
	my $incind = 0;
	
	open LDREP, "< $infile";
	while (<LDREP>)
	{
		#print $_;
		# No attempt to recognize comments
		if ($_ =~ /\#\s*include\s*(\"|<)\s*(\w+)(\.(\w+)(\"|>)|(\"|>))/i)
		{
			#print $_;
			$incind= 1;
			$include=$2; 
			$include_type=$4;
			$include_enc=$1;
			$include_type='' unless defined $include_type;
			$include = $include . "\." . $include_type unless $include_type eq '';
			@info = ();
			@info= ($source, $source_type, $source_loc, $include, $include_type, $include_enc);
			#print Dumper (\@info);
			$dbh->do ("INSERT INTO include.txt VALUES (\'$info[0]\',\'$info[1]\',\'$info[2]\',\'$info[3]\',\'$info[4]\',\'$info[5]\')")
		}
		else
		{
		}
	}
	if ($incind == 0)
	{
		@info = ();
		@info= ($source, $source_type, $source_loc,'','','');
		$dbh->do ("INSERT INTO include.txt VALUES (\'$info[0]\',\'$info[1]\',\'$info[2]\',\'$info[3]\',\'$info[4]\',\'$info[5]\')")
	}
	close LDREP;
}